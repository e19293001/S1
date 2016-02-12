#include "parser.h"

parserData* ParserNew(char *s) {
  parserData *ret;

  ret = malloc(sizeof(parserData));
  ret->st = malloc(sizeof(symtab));
  ret->st->trieRootNode = NULL;

  ret->tm = TokenManagerNew(s);
  ret->currentToken.next = NULL;
  ret->previousToken.next = NULL;
  ret->addrCntr = 0;
  ret->filename = calloc(strlen(s)+1, sizeof(char));
  strncpy(ret->filename, s, strlen(s));

  return ret;
}

tstrie* ParseSymbols(char *s) {
  tstrie* ret = NULL;
  parserData *lparser;

  int progcntr = 0;

  lparser = ParserNew(s);
  lparser->currentToken = TokenManagerGetNextToken(&lparser->tm);
  do {
    if (lparser->currentToken.kind == ID) {
      char num[256];
      memset(num, 0, 256);
      sprintf(num, "%d", progcntr);
      printf("label found: %s address: %0d\n", lparser->currentToken.image, progcntr);
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
        ret = tstInsert(ret, lparser->currentToken.image, num);
      }
      ParserSymbolsAdvance(lparser); // ID
      ParserSymbolsAdvance(lparser); // COLON
    }
    else if (lparser->currentToken.kind == DWORD) {
      ParserSymbolsAdvance(lparser);
      ParserSymbolsAdvance(lparser); // unsigned
      progcntr++;
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
//  _parser = t;
//  _parser->currentToken = TokenManagerGetNextToken(&_parser->tm);
//  _parser->st->trieRootNode = ParseSymbols();

  if ((t->st->trieRootNode = ParseSymbols(t->filename)) == NULL) {
    return;
  }

//  t->st->trieRootNode = ParseSymbols(t->filename);
  t->currentToken = TokenManagerGetNextToken(&t->tm);
  program(t);
  if (t->currentToken.kind != _EOF) {
    printf("expecting EOF. token is: %s\n", tokenImage[t->currentToken.kind]);
    exit(-1);
  }
  printf("address counter: %0d\n", t->addrCntr++);
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
  consume(lparser, PUSH);
  expression(lparser);
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
    consume(lparser, UNSIGNED);
  }
  else if (lparser->currentToken.kind == ID) {
    Token tkn = lparser->currentToken;
    consume(lparser, ID);
    if (tstSearch(lparser->st->trieRootNode, tkn.image) == NULL) {
      printf("unknown symbol: %s\n", tkn.image);
      exit(-1);
    }
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
  TokenManagerDelete(t->tm);
  free(t->filename);
  tstDump(t->st->trieRootNode);
  tstDelete(t->st->trieRootNode);
  free(t->st);
  free(t);
}

// int ParserImageExists(Token t, parserData *lparser) { 
//   if (tstSearch(lparser->st->trieRootNode, lparser->currentToken.image) == NULL) {
//     return 0;
//   }
//   else {
//     return 1;
//   }
// }

