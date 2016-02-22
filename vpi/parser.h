#ifndef parser_h
#define parser_h

#include <assert.h>
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
  int errorcode;
} parserData;

parserData *_parser;

void symD2cg(codeGen *cg, symData *symD);

parserData* ParserNew(char *s);
void ParserConsume();
Token ParserGetToken();
void ParserAdvance(parserData *lparser);
void ParserSymbolsAdvance(parserData *t);
void ParserDelete(parserData **t);
int ParserStart(parserData *t);

int ParserImageExists();

tstrie* ParseSymbols(char *s, int *errorcode);
void program(parserData *lparser);
void push(parserData *lparser);
void pushc(parserData *lparser);
void pushwc(parserData *lparser);
void halt(parserData *lparser);
void dword(parserData *lparser);
int expression(parserData *lparser);
void label(parserData *lparser);
void pushr(parserData *lparser);
void cora(parserData *lparser);
void asp(parserData *lparser);

int consume(parserData *lparser, int expected);

parserData* ParserCopy(parserData t);

#endif
