#include "memlist.h"

memlist* memlistNew() {
  memlist* ret;
  ret = malloc(sizeof(memlist));
  memset(ret->data, '\0', 512);
  ret->next = NULL;

  return ret;
}

char* memlistGetItem(memlist* ml, int x) {
  memlist* it;
  int itnum;
  for (itnum = 0; itnum < x; itnum++) {
    if (it == NULL) {
      return NULL;
    }
    it = it->next;
  }
  return it->data;
}

void memlistPushBackItem(memlist* ml, char* str) {
  memlist *head;
  memlist *tail;
  head = ml;
  tail = memlistPushBackItem_(ml, str);
  ml = head;
}

void memlistDelete(memlist* ml) {
  if (ml == NULL) {
    return;
  }
  else {
    memlistDelete(ml->next);
  }
  free(ml);
}

memlist* memlistPushBackItem_(memlist* ml, char* str) {
  memlist *ret;
  if (ml != NULL) {
    ret = memlistPushBackItem_(ml->next, str);
  }
  else {
    ml = memlistNew();
    strncpy(ml->data, str, strlen(str));
    ret = ml;
  }

  return ret;
}

memlist* memlistUpdateItem(memlist* ml, int indx, char* str) {
  if (indx == 0) {
    if (ml != NULL) {
      strncpy(ml->data, str, strlen(str));
    }
    else {
      printf("[ memlistUpdateItem ] index out of bounds.\n");
      return NULL;
    }
  }
  else {
    memlistUpdateItem(ml->next, indx++, str);
  }
  return ml;
}

