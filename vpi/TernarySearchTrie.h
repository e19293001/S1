#ifndef TernarySearchTrie_h
#define TernarySearchTrie_h

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct tstrie {
  char item;
  int data;
  struct tstrie* left;
  struct tstrie* middle;
  struct tstrie* right;
} tstrie;

tstrie* tstNew(char i, int d);
tstrie* tstSearch(tstrie *t, char *s);
tstrie* tstInsertR(tstrie *t, char *s, int val, int i, int cntr);
tstrie* tstInsert(tstrie *t, char *s, int val);
void tstCopy(tstrie *src, tstrie **dst);
void tstDelete(tstrie *t);
void tstDump_(tstrie *t, int cnt);
void tstDump(tstrie *t);
void tstDumpGraphical(tstrie *t);
void tstDumpGraphical_(tstrie *t, int branch, int level);

#endif
