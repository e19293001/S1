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
  ret->errorcode = 0;
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

        sprintf(symD->data, "%03x", atoi(pushwcToken.image));
        ret = tstInsert(ret, idToken.image, symD);
        progcntr+=2;
      }
      else if (lparser->currentToken.kind == CORA) {
        Token coraToken;
        ParserSymbolsAdvance(lparser); // cora
        coraToken = lparser->currentToken;
        ParserSymbolsAdvance(lparser); // operand

        sprintf(symD->data, "%03x", atoi(coraToken.image));
        ret = tstInsert(ret, idToken.image, symD);
        progcntr++;
      }
      else if (lparser->currentToken.kind == ASP) {
        Token aspToken;
        ParserSymbolsAdvance(lparser); // asp
        aspToken = lparser->currentToken;
        ParserSymbolsAdvance(lparser); // operand

        sprintf(symD->data, "%03x", atoi(aspToken.image));
        ret = tstInsert(ret, idToken.image, symD);
        progcntr++;
      }
      else if (lparser->currentToken.kind == CALL) {
        Token callToken;
        ParserSymbolsAdvance(lparser); // call
        callToken = lparser->currentToken;
        ParserSymbolsAdvance(lparser); // operand

        sprintf(symD->data, "%03x", atoi(callToken.image));
        ret = tstInsert(ret, idToken.image, symD);
        progcntr++;
      }
      else if (lparser->currentToken.kind == JA) {
        Token jaToken;
        ParserSymbolsAdvance(lparser); // ja
        jaToken = lparser->currentToken;
        ParserSymbolsAdvance(lparser); // operand

        sprintf(symD->data, "%03x", atoi(jaToken.image));
        ret = tstInsert(ret, idToken.image, symD);
        progcntr++;
      }
      else if (lparser->currentToken.kind == JCT) {
        Token jctToken;
        ParserSymbolsAdvance(lparser); // jct
        jctToken = lparser->currentToken;
        ParserSymbolsAdvance(lparser); // operand

        sprintf(symD->data, "%03x", atoi(jctToken.image));
        ret = tstInsert(ret, idToken.image, symD);
        progcntr++;
      }
      else if (lparser->currentToken.kind == JP) {
        Token jpToken;
        ParserSymbolsAdvance(lparser); // jp
        jpToken = lparser->currentToken;
        ParserSymbolsAdvance(lparser); // operand

        sprintf(symD->data, "%03x", atoi(jpToken.image));
        ret = tstInsert(ret, idToken.image, symD);
        progcntr++;
      }
      else if (lparser->currentToken.kind == ID) {
        strncpy(symD->name, idToken.image, strlen(idToken.image));
        sprintf(symD->address, "%03x", progcntr);
        ret = tstInsert(ret, idToken.image, symD);
      }
      else {
        ret = tstInsert(ret, idToken.image, symD);
      }        
      symDataDelete(&symD);

    }
    else if (lparser->currentToken.kind == PUSHC ||
             lparser->currentToken.kind == PUSHR ||
             lparser->currentToken.kind == CORA ||
             lparser->currentToken.kind == ASP ||
             lparser->currentToken.kind == CALL ||
             lparser->currentToken.kind == DWORD ||
             lparser->currentToken.kind == JA ||
             lparser->currentToken.kind == JCT ||
             lparser->currentToken.kind == JP ||
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
  if (t->errorcode == -1) {
    printf("error occured.\n");
  }
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
    dword(lparser);
    lparser->addrCntr++;

    program(lparser);
  }
  else if (lparser->currentToken.kind == ID) {
    label(lparser);
    program(lparser);
  }
  else if (lparser->currentToken.kind == PUSHR) {
    pushr(lparser);
    lparser->addrCntr++;
    program(lparser);
  }
  else if (lparser->currentToken.kind == CORA) {
    cora(lparser);
    if (lparser->errorcode == -1) {
      return;
    }
    lparser->addrCntr++;
    program(lparser);
  }
  else if (lparser->currentToken.kind == ASP) {
    asp(lparser);
    if (lparser->errorcode == -1) {
      return;
    }
    lparser->addrCntr++;
    program(lparser);
  }
  else if (lparser->currentToken.kind == CALL) {
    call(lparser);
    if (lparser->errorcode == -1) {
      return;
    }
    lparser->addrCntr++;
    program(lparser);
  }
  else if (lparser->currentToken.kind == JA) {
    ja(lparser);
    if (lparser->errorcode == -1) {
      return;
    }
    lparser->addrCntr++;
    program(lparser);
  }
  else if (lparser->currentToken.kind == JCT) {
    jct(lparser);
    if (lparser->errorcode == -1) {
      return;
    }
    lparser->addrCntr++;
    program(lparser);
  }
  else if (lparser->currentToken.kind == JP) {
    jp(lparser);
    if (lparser->errorcode == -1) {
      return;
    }
    lparser->addrCntr++;
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

void jp(parserData *lparser) {
  assert(consume(lparser, JP) == 0);

  if (lparser->cg->symD == NULL) {
    symData *symD = symDataNew();
    lparser->cg->symD = symD;
    if (expression(lparser) != 0) {
      symDataDelete(&symD);
      lparser->errorcode = -1;
      return;
    }
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeJP, "jp");
    symDataDelete(&symD);
    lparser->cg->symD = NULL;
  }
  else {
    if (expression(lparser) != 0) {
      lparser->errorcode = -1;
      return;
    }
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeJP, "jp");
  }
}

void jct(parserData *lparser) {
  assert(consume(lparser, JCT) == 0);

  if (lparser->cg->symD == NULL) { // guba
    symData *symD = symDataNew();
    lparser->cg->symD = symD;
    if (expression(lparser) != 0) {
      symDataDelete(&symD);
      lparser->errorcode = -1;
      return;
    }
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeJCT, "jct");
    symDataDelete(&symD);
    lparser->cg->symD = NULL;
  }
  else {
    if (expression(lparser) != 0) {
      lparser->errorcode = -1;
      return;
    }
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeJCT, "jct");
  }
}

void ja(parserData *lparser) {
  assert(consume(lparser, JA) == 0);

  if (lparser->cg->symD == NULL) {
    symData *symD = symDataNew();
    lparser->cg->symD = symD;
    if (expression(lparser) != 0) {
      symDataDelete(&symD);
      lparser->errorcode = -1;
      return;
    }
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeJA, "ja");
    symDataDelete(&symD);
    lparser->cg->symD = NULL;
  }
  else {
    if (expression(lparser) != 0) {
      lparser->errorcode = -1;
      return;
    }
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeJA, "ja");
  }
}

void asp(parserData *lparser) {
  assert(consume(lparser, ASP) == 0);

  if (lparser->cg->symD == NULL) {
    symData *symD = symDataNew();
    lparser->cg->symD = symD;
    if (expression(lparser) != 0) {
      symDataDelete(&symD);
      lparser->errorcode = -1;
      return;
    }
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeASP, "asp");
    symDataDelete(&symD);
    lparser->cg->symD = NULL;
  }
  else {
    if (expression(lparser) != 0) {
      lparser->errorcode = -1;
      return;
    }
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeASP, "asp");
  }
}

void call(parserData *lparser) {
  assert(consume(lparser, CALL) == 0);

  if (lparser->cg->symD == NULL) {
    symData *symD = symDataNew();
    lparser->cg->symD = symD;
    if (expression(lparser) != 0) {
      symDataDelete(&symD);
      lparser->errorcode = -1;
      return;
    }
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeCALL, "call");
    symDataDelete(&symD);
    lparser->cg->symD = NULL;
  }
  else {
    if (expression(lparser) != 0) {
      lparser->errorcode = -1;
      return;
    }
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeCALL, "call");
  }
}

void cora(parserData *lparser) {
  assert(consume(lparser, CORA) == 0);

  if (lparser->cg->symD == NULL) {
    symData *symD = symDataNew();
    lparser->cg->symD = symD;
    if (expression(lparser) != 0) {
      symDataDelete(&symD);
      lparser->errorcode = -1;
      return;
    }
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeCORA, "cora");
    symDataDelete(&symD);
    lparser->cg->symD = NULL;
  }
  else {
    if (expression(lparser) != 0) {
      lparser->errorcode = -1;
      return;
    }
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeCORA, "cora");
  }
}

void pushr(parserData *lparser) {
  assert(consume(lparser, PUSHR) == 0);

  if (lparser->cg->symD == NULL) {
    symData *symD = symDataNew();
    lparser->cg->symD = symD;
    if (expression(lparser) != 0) {
      symDataDelete(&symD);
      return;
    }
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypePUSHR, "pr");
    symDataDelete(&symD);
    lparser->cg->symD = NULL;
  }
  else {
    if (expression(lparser) != 0) {
      return;
    }
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
    if (expression(lparser) != 0) {
      symDataDelete(&symD);
      return;
    }
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypePUSH, "p");
    symDataDelete(&symD);
    lparser->cg->symD = NULL;
  }
  else {
    if (expression(lparser) != 0) {
      return;
    }
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
    if (expression(lparser) != 0) {
      symDataDelete(&symD);
      return;
    }
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypePUSHC, "pc");
    symDataDelete(&symD);
    lparser->cg->symD = NULL;
  }
  else {
    if (expression(lparser) != 0) {
      return;
    }
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
    if (expression(lparser) != 0) {
      return;
    }
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeDWORD, "dw");
    symDataDelete(&(lparser->cg->symD));
  }
  else {
    if (expression(lparser) != 0) {
      return;
    }
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeDWORD, "dw");
  }
}

void pushwc(parserData *lparser) {
  assert(consume(lparser, PUSHWC) == 0);
  if (lparser->cg->symD == NULL) {
    symData *symD = symDataNew();
    lparser->cg->symD = symD;
    if (expression(lparser) != 0) {
      symDataDelete(&symD);
      return;
    }
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypePUSHWC, "pwc");
    symDataDelete(&symD);
    lparser->cg->symD = NULL;
  }
  else {
    if (expression(lparser) != 0) {
      return;
    }
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypePUSHWC, "pwc");
  }
}

int expression(parserData *lparser) {
  int ret = 0;
  if (lparser->currentToken.kind == UNSIGNED) {
    Token tkn = lparser->currentToken;
    assert(consume(lparser, UNSIGNED) == 0);
    lparser->cg->symD->addressInt = atoi(tkn.image);
    sprintf(lparser->cg->symD->address, "%03x", atoi(tkn.image));
    sprintf(lparser->cg->symD->name, "%03x", atoi(tkn.image));
  }
  else if (lparser->currentToken.kind == ID) {
    Token tkn = lparser->currentToken;
    tstrie *ltstrie;
    assert(consume(lparser, ID) == 0);
    if ((ltstrie = tstSearch(lparser->trieRootNode, tkn.image)) == NULL) {
      printf("[ expression ] unknown symbol: %s\n", tkn.image);
      ret = 1;
    }
    else {
      lparser->cg->symD = ltstrie->symD;
    }
  }
  else {
    printf("error: Unknown token found: %s\n", tokenImage[lparser->currentToken.kind]);
    ret = 1;
  }

  return ret;
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
