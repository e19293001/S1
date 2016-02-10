#include "parser.h"

parserData* ParserNew(char *s) {
  parserData *ret;

  ret = malloc(sizeof(parserData));
  ret->st = NULL;

  ret->tm = TokenManagerNew(s);
  ret->currentToken.next = NULL;
  ret->previousToken.next = NULL;
  ret->addrCntr = 0;
  ret->filename = calloc(strlen(s)+1, sizeof(char));
  strncpy(ret->filename, s, strlen(s));

  return ret;
}

tstrie* ParseSymbols() {
  tstrie* ret = NULL;
  parserData *lparser;

  lparser = ParserNew(_parser->filename);
  lparser->currentToken = TokenManagerGetNextToken(&lparser->tm);
  do {
    ParserSymbolsAdvance(&lparser);
    if (lparser->currentToken.kind == ID) {
      Token tkLabel;
      Token tkUnsigned;
      printf("found label: %s\n", lparser->currentToken.image);
      tkLabel = lparser->currentToken;
      ParserSymbolsAdvance(&lparser);
      if (lparser->currentToken.kind == COLON) {
        ParserSymbolsAdvance(&lparser);
        if (lparser->currentToken.kind == DWORD) {
          printf("  found dword: %s\n", lparser->currentToken.image);
          ParserSymbolsAdvance(&lparser);
          if (lparser->currentToken.kind == UNSIGNED) {
            printf("    found unsigned: %s\n", lparser->currentToken.image);
            tkUnsigned = lparser->currentToken;
            printf("    tkLabel.image: %s\n", tkLabel.image);
            ret = tstInsert(ret, tkLabel.image, tkUnsigned.image);
          }
        }
      }
    }
  } while (lparser->currentToken.kind != _EOF);

  ParserDelete(lparser);

  return ret;
}

void ParserStart(parserData *t) {
  _parser = t;
  _parser->currentToken = TokenManagerGetNextToken(&_parser->tm);
  _parser->st = ParseSymbols();
  program();
  if (_parser->currentToken.kind != _EOF) {
    printf("expecting EOF. token is: %s\n", tokenImage[_parser->currentToken.kind]);
    exit(-1);
  }
  printf("address counter: %0d\n", _parser->addrCntr++);
}

void program() {
  if (_parser->currentToken.kind == PUSH) {
    push();
    program();
    _parser->addrCntr++;
  }
  else if (_parser->currentToken.kind == PUSHC) {
    pushc();
    program();
    _parser->addrCntr++;
  }
  else if (_parser->currentToken.kind == PUSHWC) {
//    printf("call pushwc\n");
    pushwc();
    program();
    _parser->addrCntr++;
  }
  else if (_parser->currentToken.kind == HALT) {
//    printf("call halt\n");
    halt();
    program();
    _parser->addrCntr++;
  }
  else if (_parser->currentToken.kind == DWORD) {
//    printf("call dword\n");
    dword();
    program();
    _parser->addrCntr++;
  }
  else if (_parser->currentToken.kind == ID) {
//    printf("call label\n");
    label();
    program();
  }
  else if (_parser->currentToken.kind == _EOF) {
    // do nothing
  }
  else {
    printf("error unknown token %s\n", tokenImage[_parser->currentToken.kind]);
    return;
  }
}

void label() {
  consume(ID);
  consume(COLON);
}

void push() {
//  if (_parser->currentToken.kind == PUSH) {
    consume(PUSH);
    //consume(UNSIGNED);
    expression();
//  }
}

void pushc() {
//  if (_parser->currentToken.kind == PUSHC) {
    consume(PUSHC);
    //consume(UNSIGNED);
    expression();
//  }
}

void pushwc() {
//  if (_parser->currentToken.kind == PUSHWC) {
    consume(PUSHWC);
    //consume(UNSIGNED);
    expression();
//  }
}

void halt() {
  if (_parser->currentToken.kind == HALT) {
    consume(HALT);
  }
}

void dword() {
  if (_parser->currentToken.kind == DWORD) {
    consume(DWORD);
    consume(UNSIGNED);
  }
}

void expression() {
//  printf("expression currentToken: %s\n", tokenImage[_parser->currentToken.kind]);
  if (_parser->currentToken.kind == UNSIGNED) {
    consume(UNSIGNED);
  }
  else if (_parser->currentToken.kind == ID) {
    Token tkn = _parser->currentToken;
    consume(ID);
//    _parser->st = tstInsert(_parser->st, tkn.image, 0);
    tstDump(_parser->st);
  }
  else {
    printf("error: Unknown token found: %s\n", tokenImage[_parser->currentToken.kind]);
    return;
  }
}

Token ParserGetToken() {
  Token ret;

  return ret;
}

void ParserSymbolsAdvance(parserData **t) {
  (*t)->previousToken = (*t)->currentToken;
  if ((*t)->currentToken.next != NULL) {
    (*t)->currentToken = *((*t)->currentToken.next);
  }
  else {
    (*t)->currentToken = TokenManagerGetNextToken(&((*t)->tm));
  }
}

void ParserAdvance() {
  _parser->previousToken = _parser->currentToken;

  if (_parser->currentToken.next != NULL) {
    _parser->currentToken = *(_parser->currentToken.next);
  }
  else {
    _parser->currentToken = TokenManagerGetNextToken(&_parser->tm);
  }
}

void consume(int expected) {
  if (_parser->currentToken.kind == expected) {
    ParserAdvance();
//    printf("consume: %s\n", tokenImage[expected]);
  }
  else {
    printf("consume: %s\n", tokenImage[expected]);
    printf("Found token \"%s\", ", _parser->currentToken.image);
    printf("expecting kind of \"%s\"\n", tokenImage[expected]);
    exit(-1);
  }
}

void ParserDelete(parserData *t) {
  TokenManagerDelete(t->tm);
  free(t->filename);
  tstDelete(t->st);
  free(t);
}


int ParserImageExists(Token t) {
  if (tstSearch(_parser->st, _parser->currentToken.image) == NULL) {
    return 0;
  }
  else {
    return 1;
  }
}

