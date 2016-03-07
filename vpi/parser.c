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

      //TokenDump("[ ParseSymbols ]", idToken);
      //printf("progcntr: %0d image: %s kind: %s\n", progcntr, idToken.image, tokenImage[idToken.kind]);

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
      symD->addressInt = progcntr;
      //sprintf(symD->addressInt, "%03x", progcntr);
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
      else if (lparser->currentToken.kind == PUSH ||
               lparser->currentToken.kind == PUSHWC ||
               lparser->currentToken.kind == AWC ||
               lparser->currentToken.kind == CORA ||
               lparser->currentToken.kind == ASP ||
               lparser->currentToken.kind == CALL ||
               lparser->currentToken.kind == JA ||
               lparser->currentToken.kind == JCT ||
               lparser->currentToken.kind == JP ||
               lparser->currentToken.kind == JN ||
               lparser->currentToken.kind == JZ ||
               lparser->currentToken.kind == JNZ ||
               lparser->currentToken.kind == JODD ||
               lparser->currentToken.kind == JZON ||
               lparser->currentToken.kind == JZOP ||
               lparser->currentToken.kind == RET ||
               lparser->currentToken.kind == ADD ||
               lparser->currentToken.kind == SUB ||
               lparser->currentToken.kind == STVA ||
               lparser->currentToken.kind == LOAD ||
               lparser->currentToken.kind == DUPE ||
               lparser->currentToken.kind == ESBA ||
               lparser->currentToken.kind == REBA ||
               lparser->currentToken.kind == ZSP) {
        Token cmdToken;
        ParserSymbolsAdvance(lparser); // cmd
        cmdToken = lparser->currentToken;
        ParserSymbolsAdvance(lparser); // operand

        sprintf(symD->data, "%03x", atoi(cmdToken.image));
        ret = tstInsert(ret, idToken.image, symD);
        progcntr++;
      }
      else if (lparser->currentToken.kind == CMPU ||
               lparser->currentToken.kind == CMPS) {
        Token cmpuToken;
        ParserSymbolsAdvance(lparser); // cmpu
        cmpuToken = lparser->currentToken;
        ParserSymbolsAdvance(lparser); // operand

        sprintf(symD->data, "%02x", atoi(cmpuToken.image));
        ret = tstInsert(ret, idToken.image, symD);
        progcntr++;
      }
      else if (lparser->currentToken.kind == PSP ||
               lparser->currentToken.kind == ROT ||
               lparser->currentToken.kind == SCT ||
               lparser->currentToken.kind == CALI ||
               lparser->currentToken.kind == AND ||
               lparser->currentToken.kind == FLIP ||
               lparser->currentToken.kind == XOR ||
               lparser->currentToken.kind == OR ||
               lparser->currentToken.kind == ADDY ||
               lparser->currentToken.kind == REM ||
               lparser->currentToken.kind == DIV ||
               lparser->currentToken.kind == MULT ||
               lparser->currentToken.kind == NEG ||
               lparser->currentToken.kind == HALT) {
        Token haltToken;
        haltToken = lparser->currentToken;
        ParserSymbolsAdvance(lparser); // halt

        strncpy(symD->data, haltToken.image, strlen(haltToken.image));
        ret = tstInsert(ret, idToken.image, symD);
        progcntr++;
      }
      else if (lparser->currentToken.kind == PUSHC ||
               lparser->currentToken.kind == SHRA ||
               lparser->currentToken.kind == PUSHR) {
        Token cmdToken;
        ParserSymbolsAdvance(lparser); // cmd
        cmdToken = lparser->currentToken;
        ParserSymbolsAdvance(lparser); // operand

        strncpy(symD->data, cmdToken.image, strlen(cmdToken.image));
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
             lparser->currentToken.kind == JN ||
             lparser->currentToken.kind == JZ ||
             lparser->currentToken.kind == JNZ ||
             lparser->currentToken.kind == JODD ||
             lparser->currentToken.kind == JZON ||
             lparser->currentToken.kind == JZOP ||
             lparser->currentToken.kind == CMPS ||
             lparser->currentToken.kind == CMPU ||
             lparser->currentToken.kind == SHLL ||
             lparser->currentToken.kind == SHRL ||
             lparser->currentToken.kind == SHRA ||
             lparser->currentToken.kind == PUSH) {
      ParserSymbolsAdvance(lparser);
      ParserSymbolsAdvance(lparser);
      progcntr++;
    }
    else if (lparser->currentToken.kind == AWC ||
             lparser->currentToken.kind == PUSHWC) {
      ParserSymbolsAdvance(lparser);
      ParserSymbolsAdvance(lparser);
      progcntr+=2;
    }
    else if (lparser->currentToken.kind == PSP ||
             lparser->currentToken.kind == ROT ||
             lparser->currentToken.kind == SCT ||
             lparser->currentToken.kind == CALI ||
             lparser->currentToken.kind == AND ||
             lparser->currentToken.kind == FLIP ||
             lparser->currentToken.kind == XOR ||
             lparser->currentToken.kind == OR ||
             lparser->currentToken.kind == ADDY ||
             lparser->currentToken.kind == REM ||
             lparser->currentToken.kind == DIV ||
             lparser->currentToken.kind == MULT ||
             lparser->currentToken.kind == NEG ||
             lparser->currentToken.kind == HALT ||
             lparser->currentToken.kind == ADD ||
             lparser->currentToken.kind == SUB ||
             lparser->currentToken.kind == STAV ||
             lparser->currentToken.kind == STVA ||
             lparser->currentToken.kind == LOAD ||
             lparser->currentToken.kind == DUPE ||
             lparser->currentToken.kind == ESBA ||
             lparser->currentToken.kind == REBA ||
             lparser->currentToken.kind == ZSP ||
             lparser->currentToken.kind == REV ||
             lparser->currentToken.kind == RET) {
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
  else if (lparser->currentToken.kind == JN) {
    jnn(lparser);
    if (lparser->errorcode == -1) {
      return;
    }
    lparser->addrCntr++;
    program(lparser);
  }
  else if (lparser->currentToken.kind == JZ) {
    jzz(lparser);
    if (lparser->errorcode == -1) {
      return;
    }
    lparser->addrCntr++;
    program(lparser);
  }
  else if (lparser->currentToken.kind == JNZ) {
    jnz(lparser);
    if (lparser->errorcode == -1) {
      return;
    }
    lparser->addrCntr++;
    program(lparser);
  }
  else if (lparser->currentToken.kind == JODD) {
    jodd(lparser);
    if (lparser->errorcode == -1) {
      return;
    }
    lparser->addrCntr++;
    program(lparser);
  }
  else if (lparser->currentToken.kind == JZON) {
    jzon(lparser);
    if (lparser->errorcode == -1) {
      return;
    }
    lparser->addrCntr++;
    program(lparser);
  }
  else if (lparser->currentToken.kind == JZOP) {
    jzop(lparser);
    if (lparser->errorcode == -1) {
      return;
    }
    lparser->addrCntr++;
    program(lparser);
  }
  else if (lparser->currentToken.kind == RET) {
    ret(lparser);
    if (lparser->errorcode == -1) {
      return;
    }
    lparser->addrCntr++;
    program(lparser);
  }
  else if (lparser->currentToken.kind == ADD) {
    add(lparser);
    if (lparser->errorcode == -1) {
      return;
    }
    lparser->addrCntr++;
    program(lparser);
  }
  else if (lparser->currentToken.kind == SUB) {
    sub(lparser);
    if (lparser->errorcode == -1) {
      return;
    }
    lparser->addrCntr++;
    program(lparser);
  }
  else if (lparser->currentToken.kind == STAV) {
    stav(lparser);
    if (lparser->errorcode == -1) {
      return;
    }
    lparser->addrCntr++;
    program(lparser);
  }
  else if (lparser->currentToken.kind == STVA) {
    stva(lparser);
    if (lparser->errorcode == -1) {
      return;
    }
    lparser->addrCntr++;
    program(lparser);
  }
  else if (lparser->currentToken.kind == LOAD) {
    load(lparser);
    if (lparser->errorcode == -1) {
      return;
    }
    lparser->addrCntr++;
    program(lparser);
  }
  else if (lparser->currentToken.kind == AWC) {
    awc(lparser);
    if (lparser->errorcode == -1) {
      return;
    }
    lparser->addrCntr++;
    program(lparser);
  }
  else if (lparser->currentToken.kind == DUPE) {
    dupe(lparser);
    if (lparser->errorcode == -1) {
      return;
    }
    lparser->addrCntr++;
    program(lparser);
  }
  else if (lparser->currentToken.kind == ESBA) {
    esba(lparser);
    if (lparser->errorcode == -1) {
      return;
    }
    lparser->addrCntr++;
    program(lparser);
  }
  else if (lparser->currentToken.kind == REBA) {
    reba(lparser);
    if (lparser->errorcode == -1) {
      return;
    }
    lparser->addrCntr++;
    program(lparser);
  }
  else if (lparser->currentToken.kind == ZSP) {
    zsp(lparser);
    if (lparser->errorcode == -1) {
      return;
    }
    lparser->addrCntr++;
    program(lparser);
  }
  else if (lparser->currentToken.kind == CMPS) {
    cmps(lparser);
    if (lparser->errorcode == -1) {
      return;
    }
    lparser->addrCntr++;
    program(lparser);
  }
  else if (lparser->currentToken.kind == CMPU) {
    cmpu(lparser);
    if (lparser->errorcode == -1) {
      return;
    }
    lparser->addrCntr++;
    program(lparser);
  }
  else if (lparser->currentToken.kind == REV) {
    rev(lparser);
    if (lparser->errorcode == -1) {
      return;
    }
    lparser->addrCntr++;
    program(lparser);
  }
  else if (lparser->currentToken.kind == SHLL) {
    shll(lparser);
    if (lparser->errorcode == -1) {
      return;
    }
    lparser->addrCntr++;
    program(lparser);
  }
  else if (lparser->currentToken.kind == SHRL) {
    shrl(lparser);
    if (lparser->errorcode == -1) {
      return;
    }
    lparser->addrCntr++;
    program(lparser);
  }
  else if (lparser->currentToken.kind == SHRA) {
    shra(lparser);
    if (lparser->errorcode == -1) {
      return;
    }
    lparser->addrCntr++;
    program(lparser);
  }
  else if (lparser->currentToken.kind == NEG) {
    neg(lparser);
    if (lparser->errorcode == -1) {
      return;
    }
    lparser->addrCntr++;
    program(lparser);
  }
  else if (lparser->currentToken.kind == MULT) {
    mult(lparser);
    if (lparser->errorcode == -1) {
      return;
    }
    lparser->addrCntr++;
    program(lparser);
  }
  else if (lparser->currentToken.kind == DIV) {
    divv(lparser);
    if (lparser->errorcode == -1) {
      return;
    }
    lparser->addrCntr++;
    program(lparser);
  }
  else if (lparser->currentToken.kind == REM) {
    rem(lparser);
    if (lparser->errorcode == -1) {
      return;
    }
    lparser->addrCntr++;
    program(lparser);
  }
  else if (lparser->currentToken.kind == ADDY) {
    addy(lparser);
    if (lparser->errorcode == -1) {
      return;
    }
    lparser->addrCntr++;
    program(lparser);
  }
  else if (lparser->currentToken.kind == OR) {
    or(lparser);
    if (lparser->errorcode == -1) {
      return;
    }
    lparser->addrCntr++;
    program(lparser);
  }
  else if (lparser->currentToken.kind == XOR) {
    xor(lparser);
    if (lparser->errorcode == -1) {
      return;
    }
    lparser->addrCntr++;
    program(lparser);
  }
  else if (lparser->currentToken.kind == FLIP) {
    flip(lparser);
    if (lparser->errorcode == -1) {
      return;
    }
    lparser->addrCntr++;
    program(lparser);
  }
  else if (lparser->currentToken.kind == AND) {
    and(lparser);
    if (lparser->errorcode == -1) {
      return;
    }
    lparser->addrCntr++;
    program(lparser);
  }
  else if (lparser->currentToken.kind == CALI) {
    cali(lparser);
    if (lparser->errorcode == -1) {
      return;
    }
    lparser->addrCntr++;
    program(lparser);
  }
  else if (lparser->currentToken.kind == SCT) {
    sct(lparser);
    if (lparser->errorcode == -1) {
      return;
    }
    lparser->addrCntr++;
    program(lparser);
  }
  else if (lparser->currentToken.kind == ROT) {
    rot(lparser);
    if (lparser->errorcode == -1) {
      return;
    }
    lparser->addrCntr++;
    program(lparser);
  }
  else if (lparser->currentToken.kind == PSP) {
    psp(lparser);
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

void psp(parserData *lparser) {
  assert(consume(lparser, PSP) == 0);
  if (lparser->cg->symD == NULL) {
    lparser->cg->symD = symDataNew();
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypePSP, "psp");
    symDataDelete(&(lparser->cg->symD));
  }
  else {
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypePSP, "psp");
  }
}

void rot(parserData *lparser) {
  assert(consume(lparser, ROT) == 0);
  if (lparser->cg->symD == NULL) {
    lparser->cg->symD = symDataNew();
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeROT, "rot");
    symDataDelete(&(lparser->cg->symD));
  }
  else {
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeROT, "rot");
  }
}

void sct(parserData *lparser) {
  assert(consume(lparser, SCT) == 0);
  if (lparser->cg->symD == NULL) {
    lparser->cg->symD = symDataNew();
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeSCT, "sct");
    symDataDelete(&(lparser->cg->symD));
  }
  else {
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeSCT, "sct");
  }
}

void cali(parserData *lparser) {
  assert(consume(lparser, CALI) == 0);
  if (lparser->cg->symD == NULL) {
    lparser->cg->symD = symDataNew();
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeCALI, "cali");
    symDataDelete(&(lparser->cg->symD));
  }
  else {
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeCALI, "cali");
  }
}

void and(parserData *lparser) {
  assert(consume(lparser, AND) == 0);
  if (lparser->cg->symD == NULL) {
    lparser->cg->symD = symDataNew();
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeAND, "and");
    symDataDelete(&(lparser->cg->symD));
  }
  else {
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeAND, "and");
  }
}

void flip(parserData *lparser) {
  assert(consume(lparser, FLIP) == 0);
  if (lparser->cg->symD == NULL) {
    lparser->cg->symD = symDataNew();
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeFLIP, "flip");
    symDataDelete(&(lparser->cg->symD));
  }
  else {
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeFLIP, "flip");
  }
}

void xor(parserData *lparser) {
  assert(consume(lparser, XOR) == 0);
  if (lparser->cg->symD == NULL) {
    lparser->cg->symD = symDataNew();
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeXOR, "xor");
    symDataDelete(&(lparser->cg->symD));
  }
  else {
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeXOR, "xor");
  }
}

void or(parserData *lparser) {
  assert(consume(lparser, OR) == 0);
  if (lparser->cg->symD == NULL) {
    lparser->cg->symD = symDataNew();
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeOR, "or");
    symDataDelete(&(lparser->cg->symD));
  }
  else {
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeOR, "or");
  }
}

void addy(parserData *lparser) {
  assert(consume(lparser, ADDY) == 0);
  if (lparser->cg->symD == NULL) {
    lparser->cg->symD = symDataNew();
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeADDY, "addy");
    symDataDelete(&(lparser->cg->symD));
  }
  else {
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeADDY, "addy");
  }
}

void rem(parserData *lparser) {
  assert(consume(lparser, REM) == 0);
  if (lparser->cg->symD == NULL) {
    lparser->cg->symD = symDataNew();
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeREM, "rem");
    symDataDelete(&(lparser->cg->symD));
  }
  else {
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeREM, "rem");
  }
}

void divv(parserData *lparser) {
  assert(consume(lparser, DIV) == 0);
  if (lparser->cg->symD == NULL) {
    lparser->cg->symD = symDataNew();
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeDIV, "div");
    symDataDelete(&(lparser->cg->symD));
  }
  else {
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeDIV, "div");
  }
}

void neg(parserData *lparser) {
  assert(consume(lparser, NEG) == 0);
  if (lparser->cg->symD == NULL) {
    lparser->cg->symD = symDataNew();
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeNEG, "neg");
    symDataDelete(&(lparser->cg->symD));
  }
  else {
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeNEG, "neg");
  }
}

void mult(parserData *lparser) {
  assert(consume(lparser, MULT) == 0);
  if (lparser->cg->symD == NULL) {
    lparser->cg->symD = symDataNew();
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeMULT, "mult");
    symDataDelete(&(lparser->cg->symD));
  }
  else {
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeMULT, "mult");
  }
}

void shra(parserData *lparser) {
  assert(consume(lparser, SHRA) == 0);

  if (lparser->cg->symD == NULL) {
    symData *symD = symDataNew();
    lparser->cg->symD = symD;
    if (expression(lparser) != 0) {
      symDataDelete(&symD);
      lparser->errorcode = -1;
      return;
    }
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeSHRA, "shra");
    symDataDelete(&symD);
    lparser->cg->symD = NULL;
  }
  else {
    if (expression(lparser) != 0) {
      lparser->errorcode = -1;
      return;
    }
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeSHRA, "shra");
  }
}

void shrl(parserData *lparser) {
  assert(consume(lparser, SHRL) == 0);

  if (lparser->cg->symD == NULL) {
    symData *symD = symDataNew();
    lparser->cg->symD = symD;
    if (expression(lparser) != 0) {
      symDataDelete(&symD);
      lparser->errorcode = -1;
      return;
    }
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeSHRL, "shrl");
    symDataDelete(&symD);
    lparser->cg->symD = NULL;
  }
  else {
    if (expression(lparser) != 0) {
      lparser->errorcode = -1;
      return;
    }
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeSHRL, "shrl");
  }
}

void shll(parserData *lparser) {
  assert(consume(lparser, SHLL) == 0);

  if (lparser->cg->symD == NULL) {
    symData *symD = symDataNew();
    lparser->cg->symD = symD;
    if (expression(lparser) != 0) {
      symDataDelete(&symD);
      lparser->errorcode = -1;
      return;
    }
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeSHLL, "shll");
    symDataDelete(&symD);
    lparser->cg->symD = NULL;
  }
  else {
    if (expression(lparser) != 0) {
      lparser->errorcode = -1;
      return;
    }
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeSHLL, "shll");
  }
}

void rev(parserData *lparser) {
  assert(consume(lparser, REV) == 0);
  if (lparser->cg->symD == NULL) {
    lparser->cg->symD = symDataNew();
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeREV, "rev");
    symDataDelete(&(lparser->cg->symD));
  }
  else {
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeREV, "REV");
  }
}

void cmpu(parserData *lparser) {
  assert(consume(lparser, CMPU) == 0);

  if (lparser->cg->symD == NULL) {
    symData *symD = symDataNew();
    lparser->cg->symD = symD;
    if (expression(lparser) != 0) {
      symDataDelete(&symD);
      lparser->errorcode = -1;
      return;
    }
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeCMPU, "cmpu");
    symDataDelete(&symD);
    lparser->cg->symD = NULL;
  }
  else {
    if (expression(lparser) != 0) {
      lparser->errorcode = -1;
      return;
    }
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeCMPU, "cmpu");
  }
}

void cmps(parserData *lparser) {
  assert(consume(lparser, CMPS) == 0);

  if (lparser->cg->symD == NULL) {
    symData *symD = symDataNew();
    lparser->cg->symD = symD;
    if (expression(lparser) != 0) {
      symDataDelete(&symD);
      lparser->errorcode = -1;
      return;
    }
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeCMPS, "cmps");
    symDataDelete(&symD);
    lparser->cg->symD = NULL;
  }
  else {
    if (expression(lparser) != 0) {
      lparser->errorcode = -1;
      return;
    }
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeCMPS, "cmps");
  }
}

void zsp(parserData *lparser) {
  assert(consume(lparser, ZSP) == 0);
  if (lparser->cg->symD == NULL) {
    lparser->cg->symD = symDataNew();
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeZSP, "zsp");
    symDataDelete(&(lparser->cg->symD));
  }
  else {
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeZSP, "zsp");
  }
}

void reba(parserData *lparser) {
  assert(consume(lparser, REBA) == 0);
  if (lparser->cg->symD == NULL) {
    lparser->cg->symD = symDataNew();
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeREBA, "reba");
    symDataDelete(&(lparser->cg->symD));
  }
  else {
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeREBA, "reba");
  }
}

void esba(parserData *lparser) {
  assert(consume(lparser, ESBA) == 0);
  if (lparser->cg->symD == NULL) {
    lparser->cg->symD = symDataNew();
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeESBA, "esba");
    symDataDelete(&(lparser->cg->symD));
  }
  else {
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeESBA, "esba");
  }
}

void dupe(parserData *lparser) {
  assert(consume(lparser, DUPE) == 0);
  if (lparser->cg->symD == NULL) {
    lparser->cg->symD = symDataNew();
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeDUPE, "dupe");
    symDataDelete(&(lparser->cg->symD));
  }
  else {
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeDUPE, "dupe");
  }
}

void awc(parserData *lparser) {
  assert(consume(lparser, AWC) == 0);
  if (lparser->cg->symD == NULL) {
    symData *symD = symDataNew();
    lparser->cg->symD = symD;
    if (expression(lparser) != 0) {
      symDataDelete(&symD);
      return;
    }
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeAWC, "awc");
    symDataDelete(&symD);
    lparser->cg->symD = NULL;
  }
  else {
    if (expression(lparser) != 0) {
      return;
    }
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeAWC, "awc");
  }
}

void load(parserData *lparser) {
  assert(consume(lparser, LOAD) == 0);
  if (lparser->cg->symD == NULL) {
    lparser->cg->symD = symDataNew();
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeLOAD, "load");
    symDataDelete(&(lparser->cg->symD));
  }
  else {
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeLOAD, "load");
  }
}

void stva(parserData *lparser) {
  assert(consume(lparser, STVA) == 0);
  if (lparser->cg->symD == NULL) {
    lparser->cg->symD = symDataNew();
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeSTVA, "stva");
    symDataDelete(&(lparser->cg->symD));
  }
  else {
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeSTVA, "stva");
  }
}

void stav(parserData *lparser) {
  assert(consume(lparser, STAV) == 0);
  if (lparser->cg->symD == NULL) {
    lparser->cg->symD = symDataNew();
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeSTAV, "stav");
    symDataDelete(&(lparser->cg->symD));
  }
  else {
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeSTAV, "stav");
  }
}

void sub(parserData *lparser) {
  assert(consume(lparser, SUB) == 0);
  if (lparser->cg->symD == NULL) {
    lparser->cg->symD = symDataNew();
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeSUB, "sub");
    symDataDelete(&(lparser->cg->symD));
  }
  else {
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeSUB, "sub");
  }
}

void add(parserData *lparser) {
  assert(consume(lparser, ADD) == 0);
  if (lparser->cg->symD == NULL) {
    lparser->cg->symD = symDataNew();
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeADD, "add");
    symDataDelete(&(lparser->cg->symD));
  }
  else {
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeADD, "add");
  }
}

void ret(parserData *lparser) {
  assert(consume(lparser, RET) == 0);
  if (lparser->cg->symD == NULL) {
    lparser->cg->symD = symDataNew();
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeRET, "ret");
    symDataDelete(&(lparser->cg->symD));
  }
  else {
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeRET, "ret");
  }
}

void jzop(parserData *lparser) {
  assert(consume(lparser, JZOP) == 0);

  if (lparser->cg->symD == NULL) {
    symData *symD = symDataNew();
    lparser->cg->symD = symD;
    if (expression(lparser) != 0) {
      symDataDelete(&symD);
      lparser->errorcode = -1;
      return;
    }
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeJZOP, "jzop");
    symDataDelete(&symD);
    lparser->cg->symD = NULL;
  }
  else {
    if (expression(lparser) != 0) {
      lparser->errorcode = -1;
      return;
    }
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeJZOP, "jzop");
  }
}

void jzon(parserData *lparser) {
  assert(consume(lparser, JZON) == 0);

  if (lparser->cg->symD == NULL) {
    symData *symD = symDataNew();
    lparser->cg->symD = symD;
    if (expression(lparser) != 0) {
      symDataDelete(&symD);
      lparser->errorcode = -1;
      return;
    }
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeJZON, "jzon");
    symDataDelete(&symD);
    lparser->cg->symD = NULL;
  }
  else {
    if (expression(lparser) != 0) {
      lparser->errorcode = -1;
      return;
    }
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeJZON, "jzon");
  }
}

void jnz(parserData *lparser) {
  assert(consume(lparser, JNZ) == 0);

  if (lparser->cg->symD == NULL) {
    symData *symD = symDataNew();
    lparser->cg->symD = symD;
    if (expression(lparser) != 0) {
      symDataDelete(&symD);
      lparser->errorcode = -1;
      return;
    }
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeJNZ, "jnz");
    symDataDelete(&symD);
    lparser->cg->symD = NULL;
  }
  else {
    if (expression(lparser) != 0) {
      lparser->errorcode = -1;
      return;
    }
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeJNZ, "jnz");
  }
}

void jodd(parserData *lparser) {
  assert(consume(lparser, JODD) == 0);

  if (lparser->cg->symD == NULL) {
    symData *symD = symDataNew();
    lparser->cg->symD = symD;
    if (expression(lparser) != 0) {
      symDataDelete(&symD);
      lparser->errorcode = -1;
      return;
    }
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeJODD, "jodd");
    symDataDelete(&symD);
    lparser->cg->symD = NULL;
  }
  else {
    if (expression(lparser) != 0) {
      lparser->errorcode = -1;
      return;
    }
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeJODD, "jodd");
  }
}

void jzz(parserData *lparser) {
  assert(consume(lparser, JZ) == 0);

  if (lparser->cg->symD == NULL) {
    symData *symD = symDataNew();
    lparser->cg->symD = symD;
    if (expression(lparser) != 0) {
      symDataDelete(&symD);
      lparser->errorcode = -1;
      return;
    }
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeJZ, "jz");
    symDataDelete(&symD);
    lparser->cg->symD = NULL;
  }
  else {
    if (expression(lparser) != 0) {
      lparser->errorcode = -1;
      return;
    }
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeJZ, "jz");
  }
}

void jnn(parserData *lparser) {
  assert(consume(lparser, JN) == 0);

  if (lparser->cg->symD == NULL) {
    symData *symD = symDataNew();
    lparser->cg->symD = symD;
    if (expression(lparser) != 0) {
      symDataDelete(&symD);
      lparser->errorcode = -1;
      return;
    }
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeJN, "jn");
    symDataDelete(&symD);
    lparser->cg->symD = NULL;
  }
  else {
    if (expression(lparser) != 0) {
      lparser->errorcode = -1;
      return;
    }
    sprintf(lparser->cg->symD->programcounter, "%04x", lparser->addrCntr);
    codeGenEmmitInstruction(lparser->cg, cgTypeJN, "jn");
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
    sprintf(lparser->cg->symD->address, "%03x", atoi(tkn.image));
    sprintf(lparser->cg->symD->name, "%03x", atoi(tkn.image));
    lparser->cg->symD->addressInt = atoi(tkn.image);
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
