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
      else if (lparser->currentToken.kind == AWC) {
        Token awcToken;
        ParserSymbolsAdvance(lparser); // awc
        awcToken = lparser->currentToken;
        ParserSymbolsAdvance(lparser); // operand

        sprintf(symD->data, "%03x", atoi(awcToken.image));
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
      else if (lparser->currentToken.kind == JN) {
        Token jnToken;
        ParserSymbolsAdvance(lparser); // jn
        jnToken = lparser->currentToken;
        ParserSymbolsAdvance(lparser); // operand

        sprintf(symD->data, "%03x", atoi(jnToken.image));
        ret = tstInsert(ret, idToken.image, symD);
        progcntr++;
      }
      else if (lparser->currentToken.kind == JZ) {
        Token jzToken;
        ParserSymbolsAdvance(lparser); // jz
        jzToken = lparser->currentToken;
        ParserSymbolsAdvance(lparser); // operand

        sprintf(symD->data, "%03x", atoi(jzToken.image));
        ret = tstInsert(ret, idToken.image, symD);
        progcntr++;
      }
      else if (lparser->currentToken.kind == JNZ) {
        Token jnzToken;
        ParserSymbolsAdvance(lparser); // jnz
        jnzToken = lparser->currentToken;
        ParserSymbolsAdvance(lparser); // operand

        sprintf(symD->data, "%03x", atoi(jnzToken.image));
        ret = tstInsert(ret, idToken.image, symD);
        progcntr++;
      }
      else if (lparser->currentToken.kind == JODD) {
        Token joddToken;
        ParserSymbolsAdvance(lparser); // jodd
        joddToken = lparser->currentToken;
        ParserSymbolsAdvance(lparser); // operand

        sprintf(symD->data, "%03x", atoi(joddToken.image));
        ret = tstInsert(ret, idToken.image, symD);
        progcntr++;
      }
      else if (lparser->currentToken.kind == JZON) {
        Token jzonToken;
        ParserSymbolsAdvance(lparser); // jzon
        jzonToken = lparser->currentToken;
        ParserSymbolsAdvance(lparser); // operand

        sprintf(symD->data, "%03x", atoi(jzonToken.image));
        ret = tstInsert(ret, idToken.image, symD);
        progcntr++;
      }
      else if (lparser->currentToken.kind == JZOP) {
        Token jzopToken;
        ParserSymbolsAdvance(lparser); // jzop
        jzopToken = lparser->currentToken;
        ParserSymbolsAdvance(lparser); // operand

        sprintf(symD->data, "%03x", atoi(jzopToken.image));
        ret = tstInsert(ret, idToken.image, symD);
        progcntr++;
      }
      else if (lparser->currentToken.kind == RET) {
        Token retToken;
        ParserSymbolsAdvance(lparser); // ret
        retToken = lparser->currentToken;
        ParserSymbolsAdvance(lparser); // operand

        sprintf(symD->data, "%03x", atoi(retToken.image));
        ret = tstInsert(ret, idToken.image, symD);
        progcntr++;
      }
      else if (lparser->currentToken.kind == ADD) {
        Token addToken;
        ParserSymbolsAdvance(lparser); // add
        addToken = lparser->currentToken;
        ParserSymbolsAdvance(lparser); // operand

        sprintf(symD->data, "%03x", atoi(addToken.image));
        ret = tstInsert(ret, idToken.image, symD);
        progcntr++;
      }
      else if (lparser->currentToken.kind == SUB) {
        Token subToken;
        ParserSymbolsAdvance(lparser); // sub
        subToken = lparser->currentToken;
        ParserSymbolsAdvance(lparser); // operand

        sprintf(symD->data, "%03x", atoi(subToken.image));
        ret = tstInsert(ret, idToken.image, symD);
        progcntr++;
      }
      else if (lparser->currentToken.kind == STAV) {
        Token stavToken;
        ParserSymbolsAdvance(lparser); // stav
        stavToken = lparser->currentToken;
        ParserSymbolsAdvance(lparser); // operand

        sprintf(symD->data, "%03x", atoi(stavToken.image));
        ret = tstInsert(ret, idToken.image, symD);
        progcntr++;
      }
      else if (lparser->currentToken.kind == STVA) {
        Token stvaToken;
        ParserSymbolsAdvance(lparser); // stva
        stvaToken = lparser->currentToken;
        ParserSymbolsAdvance(lparser); // operand

        sprintf(symD->data, "%03x", atoi(stvaToken.image));
        ret = tstInsert(ret, idToken.image, symD);
        progcntr++;
      }
      else if (lparser->currentToken.kind == LOAD) {
        Token loadToken;
        ParserSymbolsAdvance(lparser); // load
        loadToken = lparser->currentToken;
        ParserSymbolsAdvance(lparser); // operand

        sprintf(symD->data, "%03x", atoi(loadToken.image));
        ret = tstInsert(ret, idToken.image, symD);
        progcntr++;
      }
      else if (lparser->currentToken.kind == DUPE) {
        Token dupeToken;
        ParserSymbolsAdvance(lparser); // dupe
        dupeToken = lparser->currentToken;
        ParserSymbolsAdvance(lparser); // operand

        sprintf(symD->data, "%03x", atoi(dupeToken.image));
        ret = tstInsert(ret, idToken.image, symD);
        progcntr++;
      }
      else if (lparser->currentToken.kind == ESBA) {
        Token esbaToken;
        ParserSymbolsAdvance(lparser); // esba
        esbaToken = lparser->currentToken;
        ParserSymbolsAdvance(lparser); // operand

        sprintf(symD->data, "%03x", atoi(esbaToken.image));
        ret = tstInsert(ret, idToken.image, symD);
        progcntr++;
      }
      else if (lparser->currentToken.kind == REBA) {
        Token rebaToken;
        ParserSymbolsAdvance(lparser); // reba
        rebaToken = lparser->currentToken;
        ParserSymbolsAdvance(lparser); // operand

        sprintf(symD->data, "%03x", atoi(rebaToken.image));
        ret = tstInsert(ret, idToken.image, symD);
        progcntr++;
      }
      else if (lparser->currentToken.kind == ZSP) {
        Token zspToken;
        ParserSymbolsAdvance(lparser); // zsp
        zspToken = lparser->currentToken;
        ParserSymbolsAdvance(lparser); // operand

        sprintf(symD->data, "%03x", atoi(zspToken.image));
        ret = tstInsert(ret, idToken.image, symD);
        progcntr++;
      }
      else if (lparser->currentToken.kind == CMPS) {
        Token cmpsToken;
        ParserSymbolsAdvance(lparser); // cmps
        cmpsToken = lparser->currentToken;
        ParserSymbolsAdvance(lparser); // operand

        sprintf(symD->data, "%02x", atoi(cmpsToken.image));
        ret = tstInsert(ret, idToken.image, symD);
        progcntr++;
      }
      else if (lparser->currentToken.kind == CMPU) {
        Token cmpuToken;
        ParserSymbolsAdvance(lparser); // cmpu
        cmpuToken = lparser->currentToken;
        ParserSymbolsAdvance(lparser); // operand

        sprintf(symD->data, "%02x", atoi(cmpuToken.image));
        ret = tstInsert(ret, idToken.image, symD);
        progcntr++;
      }
      else if (lparser->currentToken.kind == CMPU) {
        Token revToken;
        ParserSymbolsAdvance(lparser); // rev
        revToken = lparser->currentToken;
        ParserSymbolsAdvance(lparser); // operand

        sprintf(symD->data, "%02x", atoi(revToken.image));
        ret = tstInsert(ret, idToken.image, symD);
        progcntr++;
      }
      else if (lparser->currentToken.kind == SHLL) {
        Token shllToken;
        ParserSymbolsAdvance(lparser); // shll
        shllToken = lparser->currentToken;
        ParserSymbolsAdvance(lparser); // operand

        sprintf(symD->data, "%01x", atoi(shllToken.image) & 0xF);
        ret = tstInsert(ret, idToken.image, symD);
        progcntr++;
      }
      else if (lparser->currentToken.kind == SHRL) {
        Token shrlToken;
        ParserSymbolsAdvance(lparser); // shrl
        shrlToken = lparser->currentToken;
        ParserSymbolsAdvance(lparser); // operand

        sprintf(symD->data, "%01x", atoi(shrlToken.image) & 0xF);
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
             lparser->currentToken.kind == PUSH) {
      // printf("pushcToken.image: %s\n", lparser->currentToken.image);
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
    else if (lparser->currentToken.kind == HALT ||
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
  else if (lparser->currentToken.kind == _EOF) {
    // do nothing
  }
  else {
    printf("error unknown token %s\n", tokenImage[lparser->currentToken.kind]);
    return;
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
