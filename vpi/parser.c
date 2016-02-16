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
      Token idToken = lparser->currentToken;
      symData *symD;
      char num[256];
      memset(num, 0, 256);
//      sprintf(num, "%03d", progcntr);

//      printf("FOUND ID lparser->currentToken.image: %s\n", lparser->currentToken.image);
      if (tstSearch(ret, idToken.image) != NULL) {
        printf("ERROR label already exists.\n");
        exit(-1);
      }
        
      symD = symDataNew();
      strncpy(symD->name, idToken.image, strlen(idToken.image));
      //strcpy(symD->address, "address");
      sprintf(symD->address, "%03x", progcntr);
//      ret = tstInsert(ret, idToken.image, symD);

      ParserSymbolsAdvance(lparser); // id
      ParserSymbolsAdvance(lparser); // colon

      //printf("lparser->image: %s\n", lparser->currentToken.image);

      if (lparser->currentToken.kind == DWORD) {
        Token dwToken;
        ParserSymbolsAdvance(lparser); // dw
        dwToken = lparser->currentToken;
        ParserSymbolsAdvance(lparser); // unsigned

        //printf("%04x\n", atoi(dwToken.image));
        sprintf(symD->data, "%04x", atoi(dwToken.image));
        //strncpy(symD->data, dwToken.image, strlen(dwToken.image));
        ret = tstInsert(ret, idToken.image, symD);
        progcntr++;
      }
      else if (lparser->currentToken.kind == PUSH) {
        Token dwToken;
        ParserSymbolsAdvance(lparser); // dw
        dwToken = lparser->currentToken;
        ParserSymbolsAdvance(lparser); // unsigned

        //printf("%04x\n", atoi(dwToken.image));
        sprintf(symD->data, "%03x", atoi(dwToken.image));
        //strncpy(symD->data, dwToken.image, strlen(dwToken.image));
        ret = tstInsert(ret, idToken.image, symD);
        progcntr++;
      }
      symDataDelete(symD);
    }
    else if (lparser->currentToken.kind == PUSHC ||
             lparser->currentToken.kind == PUSH ||
             lparser->currentToken.kind == PUSHWC) {
      ParserSymbolsAdvance(lparser);
      ParserSymbolsAdvance(lparser);
      progcntr++;
    }
    else if (lparser->currentToken.kind == HALT) {
      ParserSymbolsAdvance(lparser);
      progcntr++;
    }
    else {
      printf("TOKEN NOT SUPPORTED %s\n", lparser->currentToken.image);
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
    printf("symbol table is null.\n");
    return;
  }

//  tstDump(t->trieRootNode);

  t->currentToken = TokenManagerGetNextToken(&t->tm);
  program(t);
  if (t->currentToken.kind != _EOF) {
    printf("expecting EOF. token is: %s\n", tokenImage[t->currentToken.kind]);
    exit(-1);
  }

//  printf("address counter: %0d\n", t->addrCntr++);

}

void program(parserData *lparser) {
  if (lparser->currentToken.kind == PUSH) {
    push(lparser);
    lparser->addrCntr++;

    program(lparser);
  }
  else if (lparser->currentToken.kind == PUSHC) {
    pushc(lparser);
    lparser->addrCntr++;

    program(lparser);
  }
  else if (lparser->currentToken.kind == PUSHWC) {
    //    printf("call pushwc\n");
    pushwc(lparser);
    lparser->addrCntr++;

    program(lparser);
  }
  else if (lparser->currentToken.kind == HALT) {
    halt(lparser);
    lparser->addrCntr++;

    program(lparser);
  }
  else if (lparser->currentToken.kind == DWORD) {
    //    printf("call dword\n");
    dword(lparser);
    lparser->addrCntr++;

    program(lparser);
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
  consume(lparser, PUSH);
  expression(lparser);
  sprintf(lparser->cg->symD->programcounter, "%04d", lparser->addrCntr);
  codeGenEmmitInstruction(lparser->cg, cgTypePUSH);
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
  consume(lparser, HALT);
  if (lparser->cg->symD == NULL) {
    lparser->cg->symD = symDataNew();
    sprintf(lparser->cg->symD->programcounter, "%04d", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeHALT);
    symDataDelete(lparser->cg->symD);
  }
  else {
    sprintf(lparser->cg->symD->programcounter, "%04d", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeHALT);
  }
}

void dword(parserData *lparser) {
  consume(lparser, DWORD);
  expression(lparser);
  sprintf(lparser->cg->symD->programcounter, "%04d", lparser->addrCntr);
  codeGenEmmitInstruction(lparser->cg, cgTypeDWORD);
}

void expression(parserData *lparser) {
  if (lparser->currentToken.kind == UNSIGNED) {
    consume(lparser, UNSIGNED);
  }
  else if (lparser->currentToken.kind == ID) {
    Token tkn = lparser->currentToken;
    tstrie *ltstrie;
    consume(lparser, ID);
    if ((ltstrie = tstSearch(lparser->trieRootNode, tkn.image)) == NULL) {
      printf("unknown symbol: %s\n", tkn.image);
      exit(-1);
    }
    lparser->cg->symD = ltstrie->symD;
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
