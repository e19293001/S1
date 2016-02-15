#ifndef memlist_h
#define memlist_h

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct memlist{
  char data[512];
  struct memlist* next;
}memlist;

memlist* memlistNew();
char* memlistGetItem(memlist* ml, int x);
void memlistPushBackItem(memlist* ml, char* str);
memlist* memlistPushBackItem_(memlist* ml, char* str);
void memlistDelete(memlist* ml);
memlist* memlistUpdateItem(memlist* ml, int indx, char* str);

#endif
