#include "parser.h"

parserData* ParserNew(char *s) {
  parserData *ret;
  char *str;

  ret = malloc(sizeof(parserData));

  str = calloc(strlen(s)+1, sizeof(char));

  strncpy(str, s, strlen(s));
  str[strlen(s) - 3] = 'a';
  str[strlen(s) - 2] = 's';
  str[strlen(s) - 1] = 'm';

  ret->trieRootNode = NULL;
  ret->cg = codeGenNew(str);

  ret->tm = TokenManagerNew(s);
  ret->currentToken.next = NULL;
  ret->previousToken.next = NULL;
  ret->addrCntr = 0;
  ret->filename = calloc(strlen(s)+1, sizeof(char));
  strncpy(ret->filename, s, strlen(s));

  free(str);

  return ret;
}

tstrie* ParseSymbols(char *s) {
  tstrie* ret = NULL;
//  symData* ret = st;
  parserData *lparser;

  int progcntr = 0;

  lparser = ParserNew(s);
  lparser->currentToken = TokenManagerGetNextToken(&lparser->tm);

  do {
    if (lparser->currentToken.kind == ID) {
//      printf("label found: %s address: %0d\n", lparser->currentToken.image, progcntr);
      if (tstSearch(ret, lparser->currentToken.image) != NULL) {
        printf("ERROR label already used or declared: %s\n", lparser->currentToken.image);
        if (ret != NULL) {
          printf("freeing ret\n");
          tstDelete(ret);
        }
        ParserDelete(lparser);
        return NULL;
      }
      else {
        char num[256];
        symData *symD;
        memset(num, 0, 256);
        sprintf(num, "%03d", progcntr);
        
//        st->address = num;

//        printf("lparser->currentToken.image: %s\n", lparser->currentToken.image);
        symD = symDataNew(symD);
        strncpy(symD->name, lparser->currentToken.image, strlen(lparser->currentToken.image));
//        strncpy(symD->data, lparser->currentToken.image, strlen(lparser->currentToken.image));
//        strncpy(symD->address, lparser->currentToken.image, strlen(lparser->currentToken.image));
//        strcpy(symD->data,"data");
        strcpy(symD->address, "address");
        sprintf(symD->address, "%0d\n", progcntr);
        ret = tstInsert(ret, lparser->currentToken.image, symD);
        symDataDump(symD);
        symDataDelete(symD);
//        tstDump(ret);
//        ret = tstInsert(ret, lparser->currentToken.image, num);
      }
      ParserSymbolsAdvance(lparser); // ID
      ParserSymbolsAdvance(lparser); // COLON
    }
    else if (lparser->currentToken.kind == DWORD) {
      ParserSymbolsAdvance(lparser);
      ParserSymbolsAdvance(lparser); // unsigned
      progcntr++;

      //st->address++;
      //st->numOfDwords++;
    }
    else if (lparser->currentToken.kind == PUSHC ||
             lparser->currentToken.kind == PUSH ||
             lparser->currentToken.kind == PUSHWC) {
      ParserSymbolsAdvance(lparser);
      ParserSymbolsAdvance(lparser);
      progcntr++;
    }
    else if (lparser->currentToken.kind == HALT) {
      halt(lparser);
      progcntr++;
    }
    else {
      ParserSymbolsAdvance(lparser);
    }
  } while (lparser->currentToken.kind != _EOF);

  ParserDelete(lparser);

  return ret;
}

void ParserStart(parserData *t) {
//  int indexDwords;
  printf("[ ParserNew ] input file: %s\n", t->filename);

  { // for debugging only
    char *c;
    c = calloc(strlen(t->filename)+1, sizeof(char));
    strncpy(c,t->filename, strlen(t->filename));
    c[strlen(t->filename) - 3] = 'a';
    c[strlen(t->filename) - 2] = 's';
    c[strlen(t->filename) - 1] = 'm';
    printf("[ ParserNew ] output file: %s\n", c);
    free(c);
  }

  if ((t->trieRootNode = ParseSymbols(t->filename)) == NULL) {
    return;
  }

//  tstDump(t->trieRootNode);

  t->currentToken = TokenManagerGetNextToken(&t->tm);
  program(t);
  if (t->currentToken.kind != _EOF) {
    printf("expecting EOF. token is: %s\n", tokenImage[t->currentToken.kind]);
    exit(-1);
  }
  printf("address counter: %0d\n", t->addrCntr++);
//  printf("t->st->numOfDwords: %0d\n", t->st->numOfDwords);
//  t->cg->opcode = "0000";
//  t->cg->address = t->addrCntr;
//  for(indexDwords = 0; indexDwords < t->st->numOfDwords; indexDwords++) {
//    codeGenEmmitInstruction(t->cg, cgTypeDWORD);
//  }
}

void program(parserData *lparser) {
  if (lparser->currentToken.kind == PUSH) {
    push(lparser);
    program(lparser);
    lparser->addrCntr++;
  }
  else if (lparser->currentToken.kind == PUSHC) {
    pushc(lparser);
    program(lparser);
    lparser->addrCntr++;
  }
  else if (lparser->currentToken.kind == PUSHWC) {
    //    printf("call pushwc\n");
    pushwc(lparser);
    program(lparser);
    lparser->addrCntr++;
  }
  else if (lparser->currentToken.kind == HALT) {
    //    printf("call halt\n");
    halt(lparser);
    program(lparser);
    lparser->addrCntr++;
  }
  else if (lparser->currentToken.kind == DWORD) {
    //    printf("call dword\n");
    dword(lparser);
    program(lparser);
    lparser->addrCntr++;
  }
  else if (lparser->currentToken.kind == ID) {
    //    printf("call label\n");
    label(lparser);
    program(lparser);
  }
  else if (lparser->currentToken.kind == _EOF) {
    // do nothing
  }
  else {
    printf("error unknown token %s\n", tokenImage[lparser->currentToken.kind]);
    return;
  }
}

void label(parserData *lparser) {
  consume(lparser, ID);
  consume(lparser, COLON);
}

void push(parserData *lparser) {
  Token ltoken;
  consume(lparser, PUSH);
  ltoken = lparser->currentToken;
  expression(lparser);
//  memset(lparser->cg->opcode, '\0', 512);
//  memset(lparser->cg->address, '\0', 512);
//  strncpy(lparser->cg->opcode,"A", 1);
//  sprintf(lparser->cg->address, "%d", lparser->addrCntr);
//  codeGenEmmitInstruction(lparser->cg, cgTypePUSH);
}

void pushc(parserData *lparser) {
  consume(lparser, PUSHC);
  expression(lparser);
}

void pushwc(parserData *lparser) {
  consume(lparser, PUSHWC);
  expression(lparser);
}

void halt(parserData *lparser) {
  if (lparser->currentToken.kind == HALT) {
    consume(lparser, HALT);
  }
}

void dword(parserData *lparser) {
  if (lparser->currentToken.kind == DWORD) {
    consume(lparser, DWORD);
    consume(lparser, UNSIGNED);
  }
}

void expression(parserData *lparser) {
  if (lparser->currentToken.kind == UNSIGNED) {
//    lparser->cg->operand = lparser->currentToken.image;
    consume(lparser, UNSIGNED);
  }
  else if (lparser->currentToken.kind == ID) {
    Token tkn = lparser->currentToken;
    tstrie *ltstrie;
    symData *lsymD;
    consume(lparser, ID);
    if ((ltstrie = tstSearch(lparser->trieRootNode, tkn.image)) == NULL) {
      printf("unknown symbol: %s\n", tkn.image);
      exit(-1);
    }
    lsymD = ltstrie->symD;
    //symDataDump(lsymD);
    //lparser->cg->operand = ltstrie->data;
  }
  else {
    printf("error: Unknown token found: %s\n", tokenImage[lparser->currentToken.kind]);
    return;
  }
}

Token ParserGetToken() {
  Token ret;

  return ret;
}

void ParserSymbolsAdvance(parserData *t) {
  t->previousToken = t->currentToken;
  if (t->currentToken.next != NULL) {
    t->currentToken = *(t->currentToken.next);
  }
  else {
    t->currentToken = TokenManagerGetNextToken(&t->tm);
  }
}

void ParserAdvance(parserData *lparser) {
  lparser->previousToken = lparser->currentToken;

  if (lparser->currentToken.next != NULL) {
    lparser->currentToken = *(lparser->currentToken.next);
  }
  else {
    lparser->currentToken = TokenManagerGetNextToken(&lparser->tm);
  }
}

void consume(parserData *lparser, int expected) {
  if (lparser->currentToken.kind == expected) {
    ParserAdvance(lparser);
    //    printf("consume: %s\n", tokenImage[expected]);
  }
  else {
    printf("consume: %s\n", tokenImage[expected]);
    printf("Found token \"%s\", ", lparser->currentToken.image);
    printf("expecting kind of \"%s\"\n", tokenImage[expected]);
    exit(-1);
  }
}

void ParserDelete(parserData *t) {
//  printf("[ ParserDelete ]\n");
  TokenManagerDelete(t->tm);
  free(t->filename);
//  tstDump(t->st->trieRootNode);
//  tstDelete(t->st->trieRootNode);
  tstDelete(t->trieRootNode);
//  free(t->st);
  codeGenDelete(t->cg);
  free(t);
}
