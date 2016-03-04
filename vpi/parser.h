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
void call(parserData *lparser);
void ja(parserData *lparser);
void jct(parserData *lparser);
void jp(parserData *lparser);
void jnn(parserData *lparser);
void jzz(parserData *lparser);
void jnz(parserData *lparser);
void jodd(parserData *lparser);
void jzon(parserData *lparser);
void jzop(parserData *lparser);
void ret(parserData *lparser);
void add(parserData *lparser);
void sub(parserData *lparser);
void stav(parserData *lparser);
void stva(parserData *lparser);
void load(parserData *lparser);
void awc(parserData *lparser);
void dupe(parserData *lparser);
void esba(parserData *lparser);
void reba(parserData *lparser);
void zsp(parserData *lparser);
void cmps(parserData *lparser);
void cmpu(parserData *lparser);
void rev(parserData *lparser);
void shll(parserData *lparser);
void shrl(parserData *lparser);
void shra(parserData *lparser);
void neg(parserData *lparser);
void mult(parserData *lparser);
void divv(parserData *lparser);
void rem(parserData *lparser);
void addy(parserData *lparser);
void or(parserData *lparser);
void xor(parserData *lparser);
void flip(parserData *lparser);
void and(parserData *lparser);
void cali(parserData *lparser);

int consume(parserData *lparser, int expected);

parserData* ParserCopy(parserData t);

#endif
