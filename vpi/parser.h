#ifndef parser_h
#define parser_h

#include "tokenmanager.h"
#include "TernarySearchTrie.h"
#include "codegenerator.h"
#include "memlist.h"

typedef struct parserData {
  TokenManager *tm;
  tstrie *trieRootNode;
  codeGen *cg;
  Token currentToken;
  Token previousToken;
  unsigned int addrCntr;
  char *filename;
} parserData;

parserData *_parser;

parserData* ParserNew(char *s);
void ParserConsume();
Token ParserGetToken();
void ParserAdvance(parserData *lparser);
void ParserSymbolsAdvance(parserData *t);
void ParserDelete(parserData *t);
void ParserStart(parserData *t);

int ParserImageExists();

tstrie* ParseSymbols(char *s);
void program(parserData *lparser);
void push(parserData *lparser);
void pushc(parserData *lparser);
void pushwc(parserData *lparser);
void halt(parserData *lparser);
void dword(parserData *lparser);
void expression(parserData *lparser);
void label(parserData *lparser);

void consume(parserData *lparser, int expected);

parserData* ParserCopy(parserData t);

#endif
