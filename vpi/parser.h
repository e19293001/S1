#ifndef parser_h
#define parser_h

#include "tokenmanager.h"
#include "TernarySearchTrie.h"

//typedef struct symtab {
//  tstrie *trieRootNode;
//} symtab;

typedef struct parserData {
  TokenManager *tm;
//  symtab* st;
  tstrie *st;
  Token currentToken;
  Token previousToken;
  unsigned int addrCntr;
  char *filename;
} parserData;

parserData *_parser;

parserData* ParserNew(char *s);
void ParserConsume();
Token ParserGetToken();
void ParserAdvance();
void ParserSymbolsAdvance(parserData **t);
void ParserDelete(parserData *t);
void ParserStart(parserData *t);

int ParserImageExists();

tstrie* ParseSymbols();

void program();
void push();
void pushc();
void pushwc();
void halt();
void dword();
void expression();
void label();

void consume(int expected);

parserData* ParserCopy(parserData t);

#endif
