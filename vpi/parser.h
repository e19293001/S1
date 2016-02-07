#ifndef parser_h
#define parser_h

#include "tokenmanager.h"
#include "TernarySearchTrie.h"

typedef struct parserData {
  TokenManager *tm;
  tstrie *st;
  Token currentToken;
  Token previousToken;
} parserData;

parserData *_parser;

parserData* ParserNew(char *s);
void ParserConsume();
Token ParserGetToken();
void ParserAdvance();
void ParserDelete(parserData *t);
void ParserStart(parserData *t);

int ParserImageExists();

void program();
void push();
void pushc();
void pushwc();
void halt();
void dword();
void expression();

void consume(int expected);

#endif
