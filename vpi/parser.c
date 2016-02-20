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

tstrie* ParseSymbols(char *s, int *errorcode) {
  tstrie* ret = NULL;
//  symData* ret = st;
  parserData *lparser;

  int progcntr = 0;
  *errorcode = 0;

  lparser = ParserNew(s);
  lparser->currentToken = TokenManagerGetNextToken(&lparser->tm);

  do {
    if (lparser->currentToken.kind == ID) {
      Token idToken = lparser->currentToken;
      symData *symD = NULL;
      char num[256];
      
      memset(num, 0, 256);

//      printf("progcntr: %0d image: %s kind: %s\n", progcntr, idToken.image, tokenImage[idToken.kind]);

      if (tstSearch(ret, idToken.image) != NULL) {
        printf("ERROR label already exists. progcntr: %0d image: %s kind: %s\n", progcntr, idToken.image, tokenImage[idToken.kind]);
        ParserDelete(&lparser);
        tstDelete(&ret);
        *errorcode = 1;
        return NULL;
      }
        
      symD = symDataNew();
      
      strncpy(symD->name, idToken.image, strlen(idToken.image));
      sprintf(symD->address, "%03x", progcntr);
      //printf("idToken.image: %s symD->address: %s\n", idToken.image, symD->address);

      ParserSymbolsAdvance(lparser); // id
      ParserSymbolsAdvance(lparser); // colon

      if (lparser->currentToken.kind == DWORD) {
        Token dwToken;
        ParserSymbolsAdvance(lparser); // dw
        dwToken = lparser->currentToken;
        ParserSymbolsAdvance(lparser); // unsigned

        sprintf(symD->data, "%04x", atoi(dwToken.image));
        ret = tstInsert(ret, idToken.image, symD);
        progcntr++;
      }
      else if (lparser->currentToken.kind == PUSH) {
        Token pushToken;
        ParserSymbolsAdvance(lparser); // dw
        pushToken = lparser->currentToken;
        ParserSymbolsAdvance(lparser); // unsigned

        sprintf(symD->data, "%03x", atoi(pushToken.image));
        ret = tstInsert(ret, idToken.image, symD);
        progcntr++;
      }
      else if (lparser->currentToken.kind == HALT) {
        Token haltToken;
        haltToken = lparser->currentToken;
        ParserSymbolsAdvance(lparser); // halt

        strncpy(symD->data, haltToken.image, strlen(haltToken.image));
        ret = tstInsert(ret, idToken.image, symD);
        progcntr++;
      }
      else if (lparser->currentToken.kind == PUSHR) {
        Token pushrToken;
        ParserSymbolsAdvance(lparser); // pushc
        pushrToken = lparser->currentToken;
        ParserSymbolsAdvance(lparser); // operand

        strncpy(symD->data, pushrToken.image, strlen(pushrToken.image));
        ret = tstInsert(ret, idToken.image, symD);
        progcntr++;
      }
      else if (lparser->currentToken.kind == PUSHC) {
        Token pushcToken;
        ParserSymbolsAdvance(lparser); // pushc
        pushcToken = lparser->currentToken;
        ParserSymbolsAdvance(lparser); // operand

        strncpy(symD->data, pushcToken.image, strlen(pushcToken.image));
        ret = tstInsert(ret, idToken.image, symD);
        progcntr++;
      }
      else if (lparser->currentToken.kind == PUSHWC) {
        Token pushwcToken;
        ParserSymbolsAdvance(lparser); // pushwc
        pushwcToken = lparser->currentToken;
        ParserSymbolsAdvance(lparser); // operand

//        printf("pushwcToken.image: %s\n", pushwcToken.image);
//        printf("symD->address: %s\n", symD->address);
        sprintf(symD->data, "%03x", atoi(pushwcToken.image));
        ret = tstInsert(ret, idToken.image, symD);
        progcntr+=2;
      }
      else if (lparser->currentToken.kind == ID) {
        strncpy(symD->name, idToken.image, strlen(idToken.image));
        sprintf(symD->address, "%03x", progcntr);
        ret = tstInsert(ret, idToken.image, symD);
      }
      symDataDelete(&symD);

    }
    else if (lparser->currentToken.kind == PUSHC ||
             lparser->currentToken.kind == PUSHR ||
             lparser->currentToken.kind == PUSH) {
//      printf("pushcToken.image: %s\n", lparser->currentToken.image);
      ParserSymbolsAdvance(lparser);
      ParserSymbolsAdvance(lparser);
      progcntr++;
    }
    else if (lparser->currentToken.kind == PUSHWC) {
      ParserSymbolsAdvance(lparser);
      ParserSymbolsAdvance(lparser);
      progcntr+=2;
    }
    else if (lparser->currentToken.kind == HALT) {
      ParserSymbolsAdvance(lparser);
      progcntr++;
    }
    else {
      printf("TOKEN NOT SUPPORTED [%s] kind: %d image: %s progcntr: %0d\n", lparser->currentToken.image, lparser->currentToken.kind, tokenImage[lparser->currentToken.kind], progcntr);
      ParserSymbolsAdvance(lparser);
    }
  } while (lparser->currentToken.kind != _EOF);

  ParserDelete(&lparser);

  return ret;
}

int ParserStart(parserData *t) {
//  int indexDwords;
  int error;
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

  t->trieRootNode = ParseSymbols(t->filename, &error);

  if (error == 1) {
    printf("error occured.\n");
    return 1;
  }
  
  t->currentToken = TokenManagerGetNextToken(&t->tm);
  program(t);
  if (t->currentToken.kind != _EOF) {
    printf("expecting EOF. token is: %s\n", tokenImage[t->currentToken.kind]);
    //exit(-1);
    return 1;
  }
  return 0;
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
    lparser->addrCntr+=2;

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
  else if (lparser->currentToken.kind == PUSHR) {
    //    printf("call label\n");
    pushr(lparser);
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

void pushr(parserData *lparser) {
  assert(consume(lparser, PUSHR) == 0);

  if (lparser->cg->symD == NULL) { // guba
    symData *symD = symDataNew();
    lparser->cg->symD = symD;
    expression(lparser);
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypePUSHR, "pr");
    symDataDelete(&symD);
    lparser->cg->symD = NULL;
//    lparser->cg->symD = symDataNew();
//    expression(lparser);
//    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
//    codeGenEmmitInstruction(lparser->cg, cgTypePUSHR, "pr");
//    symDataDelete(&(lparser->cg->symD));
  }
  else {
    expression(lparser);
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypePUSHR, "pr");
  }
}

void label(parserData *lparser) {
  assert(consume(lparser, ID) == 0);
  assert(consume(lparser, COLON) == 0);
}

void push(parserData *lparser) {
  assert(consume(lparser, PUSH) == 0);
  if (lparser->cg->symD == NULL) {
    symData *symD = symDataNew();
    lparser->cg->symD = symD;
    expression(lparser);
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypePUSH, "p");
    symDataDelete(&symD);
    lparser->cg->symD = NULL;

//    lparser->cg->symD = symDataNew();
//    expression(lparser);
//    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
//    codeGenEmmitInstruction(lparser->cg, cgTypePUSH, "p");
//    symDataDelete(&(lparser->cg->symD));
  }
  else {
    expression(lparser);
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypePUSH, "p");
  }
}

void pushc(parserData *lparser) {
  assert(consume(lparser, PUSHC) == 0);
  if (lparser->cg->symD == NULL) {
    symData *symD;
    symD = symDataNew();
    lparser->cg->symD = symD;
    //lparser->cg->symD = symDataNew();
    expression(lparser);
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypePUSHC, "pc");
    symDataDelete(&symD);
    lparser->cg->symD = NULL;
    //symDataDelete(&(lparser->cg->symD));
  }
  else {
    expression(lparser);
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypePUSHC, "pc");
  }
}

void halt(parserData *lparser) {
  assert(consume(lparser, HALT) == 0);
  if (lparser->cg->symD == NULL) {
    lparser->cg->symD = symDataNew();
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeHALT, "halt");
    symDataDelete(&(lparser->cg->symD));
  }
  else {
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeHALT, "halt");
  }
}

void dword(parserData *lparser) {
  assert(consume(lparser, DWORD) == 0);
  if (lparser->cg->symD == NULL) {
    lparser->cg->symD = symDataNew();
    expression(lparser);
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeDWORD, "dw");
    symDataDelete(&(lparser->cg->symD));
  }
  else {
    expression(lparser);
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeDWORD, "dw");
  }
}

void pushwc(parserData *lparser) {
  assert(consume(lparser, PUSHWC) == 0);
  if (lparser->cg->symD == NULL) {
    symData *symD = symDataNew();
    lparser->cg->symD = symD;
    expression(lparser);
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypePUSHWC, "pwc");
    symDataDelete(&symD);
    lparser->cg->symD = NULL;
    

//    lparser->cg->symD = symDataNew();
//    expression(lparser);
//    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
//    codeGenEmmitInstruction(lparser->cg, cgTypePUSHWC, "pwc");
//    symDataDelete(&(lparser->cg->symD));
  }
  else {
    expression(lparser);
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypePUSHWC, "pwc");
  }
}

void expression(parserData *lparser) {
  if (lparser->currentToken.kind == UNSIGNED) {
    Token tkn = lparser->currentToken;
    assert(consume(lparser, UNSIGNED) == 0);
    //if (lparser->cg->symD != NULL) printf("[ expression ] this is not NULL address: %08x\n", lparser->cg->symD);
    lparser->cg->symD->addressInt = atoi(tkn.image);
    sprintf(lparser->cg->symD->address, "%03x", atoi(tkn.image));
    sprintf(lparser->cg->symD->name, "%03x", atoi(tkn.image));
  }
  else if (lparser->currentToken.kind == ID) {
    Token tkn = lparser->currentToken;
    tstrie *ltstrie;
    assert(consume(lparser, ID) == 0);
    if ((ltstrie = tstSearch(lparser->trieRootNode, tkn.image)) == NULL) {
      //tstDump(lparser->trieRootNode);
      printf("[ expression ] unknown symbol: %s\n", tkn.image);
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

int consume(parserData *lparser, int expected) {
  if (lparser->currentToken.kind == expected) {
    ParserAdvance(lparser);
    return 0;
    //    printf("consume: %s\n", tokenImage[expected]);
  }
  else {
    printf("consume: %s\n", tokenImage[expected]);
    printf("Found token \"%s\", ", lparser->currentToken.image);
    printf("expecting kind of \"%s\"\n", tokenImage[expected]);
    //exit(-1);
    return -1;
  }
}

void ParserDelete(parserData **t) {
  TokenManagerDelete(&((*t)->tm));
  free((*t)->filename);
  tstDelete(&((*t)->trieRootNode));
  codeGenDelete(&(*t)->cg);
  free(*t);
  *t = NULL;
}
