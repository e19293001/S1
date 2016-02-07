#include "parser.h"

parserData* ParserNew(char *s) {
  parserData *ret;

  ret = malloc(sizeof(parserData));
  ret->st = NULL;

  ret->tm = TokenManagerNew(s);
  ret->currentToken.next = NULL;
  ret->previousToken.next = NULL;

  return ret;
}

void ParserStart(parserData *t) {
  _parser = t;
  _parser->currentToken = TokenManagerGetNextToken(&_parser->tm);
  program();
  if (_parser->currentToken.kind != _EOF) {
    printf("expecting EOF. token is: %s\n", tokenImage[_parser->currentToken.kind]);
    exit(-1);
  }
}

void program() {
  if (_parser->currentToken.kind == PUSH) {
    push();
    program();
  }
  else if (_parser->currentToken.kind == PUSHC) {
    pushc();
    program();
  }
  else if (_parser->currentToken.kind == PUSHWC) {
//    printf("call pushwc\n");
    pushwc();
    program();
  }
  else if (_parser->currentToken.kind == HALT) {
//    printf("call halt\n");
    halt();
    program();
  }
  else if (_parser->currentToken.kind == DWORD) {
//    printf("call dword\n");
    dword();
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
    consume(UNSIGNED);
    //expression();
//  }
}

void pushwc() {
//  if (_parser->currentToken.kind == PUSHWC) {
    consume(PUSHWC);
    consume(UNSIGNED);
    //expression();
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
    consume(COLON);
    consume(ID);
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
    _parser->st = tstInsert(_parser->st, tkn.image, 0);
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

