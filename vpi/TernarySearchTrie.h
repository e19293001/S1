#ifndef TernarySearchTrie_h
#define TernarySearchTrie_h

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct symData {
  char name[512];
  char data[512];
  char address[512];
} symData;

symData* symDataNew();
void symDataDump(symData* sd);
void symDataDelete(symData *t);
symData* symDataCopy(symData *dst);

typedef struct tstrie {
  char item;
  symData *symD;
  struct tstrie* left;
  struct tstrie* middle;
  struct tstrie* right;
} tstrie;

tstrie* tstNew(char i);
tstrie* tstSearch(tstrie *t, char *s);
tstrie* tstInsertR(tstrie *t, char *s, symData *val, int i, int cntr);
tstrie* tstInsert(tstrie *t, char *s, symData *val);
void tstCopy(tstrie *src, tstrie **dst);
void tstDelete(tstrie *t);
void tstDump_(tstrie *t, int cnt);
void tstDump(tstrie *t);
void tstDumpGraphical(tstrie *t);
void tstDumpGraphical_(tstrie *t, int branch, int level);

#endif
