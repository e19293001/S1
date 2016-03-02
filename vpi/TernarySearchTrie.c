#include "TernarySearchTrie.h"

tstrie* tstSearchR(tstrie *t, char *s, int i, int cntr);

symData* symDataNew() {
  symData *ret;

  ret = malloc(sizeof(symData));
  memset(ret->name, '\0', 512);
  memset(ret->data, '\0', 512);
  memset(ret->address, '\0', 512);
  memset(ret->programcounter, '\0', 512);

  return ret;
}

void symDataDump(symData* sd) {
  if (sd != NULL) {
    printf("[ symDataDump ] name: %s\n", sd->name);
    printf("[ symDataDump ] data: %s\n", sd->data);
    printf("[ symDataDump ] address: %s\n", sd->address);
  }
  else {
    printf("[ symDataDump ] sd is NULL\n");
  }
}

void symDataDelete(symData** t) {
  if (*t != NULL) {
    //symDataDump(*t);
//    printf("*t is not NULL freeing %08x name: %s\n", *t, (*t)->name);
    free(*t);
  }
  *t = NULL;
}
symData* symDataCopy(symData *dst) {
  symData *src;
  if (dst == NULL) {
    printf("[ symDataCopy ] WARNING: copying NULL symData.\n");
    return NULL;
  }
  else {
//    printf("[ symDataCopy ]\n");
    src = symDataNew();
    strcpy(src->name, dst->name);
    strcpy(src->data, dst->data);
    strcpy(src->address, dst->address);
    src->addressInt = dst->addressInt;
//    printf("[ symDataCopy ] src->addressInt: %0d\n", src->addressInt);
  }

  return src;
}

void printspace(int cnt) {
  int x;
  for (x = 0; x < cnt+1; x++) {
    printf(" ");
  }
}

tstrie* tstNew(char i) {
  tstrie* ret;

//  printf("[ tstNew ] creating...\n");
  ret = malloc(sizeof(tstrie));
  ret->item = i;
  ret->symD = NULL;
  //ret->data = calloc(strlen(d), sizeof(char));
  //strncpy(ret->data, d, strlen(d));
  //ret->dat
  ret->left = NULL;
  ret->middle = NULL;
  ret->right = NULL;
//  printf("[ tstNew ] done.\n");

  return ret;
}

tstrie* tstSearchR(tstrie *t, char *s, int i, int cntr) {
  char val = s[i];

  if (t == NULL) {
//    printf("t is null\n");
    return NULL;
  }
  if (val < t->item) {
//    printf("current: val: %c t->item: %c ", val, t->item);
//    printf("[ turning left.\n");
    return tstSearchR(t->left, s, i, cntr+1);
  }
  if (val > t->item) {
//    printf("current: val: %c t->item: %c ", val, t->item);
//    printf("[ turning right.\n");
    return tstSearchR(t->right, s, i, cntr+1);
  }

  if (i < strlen(s)-1) {
//    printf("current: val: %c t->item: %c ", val, t->item);
//    printf("[ going to middle.\n");
    return tstSearchR(t->middle, s, i+1, cntr+1);
  }

//  printf("returning... current: *s: %s val: %c t->item: %c strlen(s): %0d\n", s, val, t->item, strlen(s));
  if (t->symD == NULL) {
//    printf("t->symD is null returning NULL\n");
    return NULL;
  }
//  else {
////    printf("present: %s\n", t->symD->name);
//    
//    if (t->symD->name[0] == '\0') {
//      printf("name is null\n");
//      return NULL;
//    }
//    else {
//      printf("name is not null: %s\n", t->symD->name);
//    }
//  }

  return t;
}

//tstrie* tstSearch(tstrie *t, char *s) {
tstrie* tstSearch(tstrie *t, char *s) {
  tstrie *ret;

//  printf("--- [ searching for: %s\n", s);
  ret = tstSearchR(t, s, 0, 0);

  return ret;
}

tstrie* tstInsertR(tstrie *t, char *s, symData* val, int i, int cntr) {
  char charindxd = s[i];

  if (t == NULL) {
    t = tstNew(charindxd);
  }
  if (charindxd < t->item) {
    t->left =  tstInsertR(t->left, s, val, i, cntr+1);
  }
  else if (charindxd > t->item) {
    t->right = tstInsertR(t->right, s, val, i, cntr+1);
  }
  else if (i < strlen(s)-1) {
    t->middle = tstInsertR(t->middle, s, val, i+1, cntr+1);
  }
  else {

    if (t->symD == NULL) {
//      printf("searching for %s\n", s);
//      printf("t->symD is NULL\n");
      t->symD = symDataCopy(val);
//      printf("created new");
    }
    else {
      symDataDelete(&(t->symD)); // free first before creating another one.
      t->symD = symDataCopy(val);
    }
  }

  return t;
}

tstrie* tstInsert(tstrie *t, char *s, symData *val) {
  tstrie *ret;

//  printf("++++++++++++++++++++++++++++++++++++++ start\n");
//  printf("inserting [%s] with val: \n", s);
//  symDataDump(val);
  ret = tstInsertR(t, s, val, 0, 0);
//  printf("++++++++++++++++++++++++++++++++++++++ end\n");

  return ret;
}

void tstCopy(tstrie *src, tstrie **dst) {
//  tstDump(src);
  //(*dst) = tstNew(src->item, src->data);
  (*dst) = tstNew(src->item);

  //(*dst)->item = src->item;
  //(*dst)->data = src->data;
  //(*dst)->left = src->left;
  //(*dst)->middle = src->middle;
  //(*dst)->right = src->right;

//  printf("[ tstCopy ] now trying to branch...\n");
  if (src->left != NULL) {
//    printf("[ tstCopy ] going left\n");
    tstCopy(src->left,&((*dst)->left));
  }
  if (src->middle != NULL) {
//    printf("[ tstCopy ] going middle\n");
    tstCopy(src->middle,&((*dst)->middle));
  }
  if (src->right != NULL) {
//    printf("[ tstCopy ] going right\n");
    tstCopy(src->right,&((*dst)->right));
  }
//  tstDump(*dst);
//  printf("[ tstCopy ] done.\n");
//  printf("[ tstCopy ] src->item: %c src->data: %0d (*dst)->item: %c (*dst)->data: %0d\n", src->item, src->data, (*dst)->item, (*dst)->data);
}

void tstDelete(tstrie **t) {
  if (*t != NULL) {
    tstDelete(&(*t)->left);
    tstDelete(&(*t)->middle);
    tstDelete(&(*t)->right);
    symDataDelete(&(*t)->symD);
    free(*t);
    *t = NULL;
  }
}

void tstDump(tstrie *t) {
  if (t == NULL) {
    printf("[ tstDump ] t is NULL\n");
  }
  else {
    printf("[\n");
    tstDump_(t, 0);
    printf("]");
    printf("\n");
  }
}

void tstDump_(tstrie *t, int cnt) {
//  if (t == NULL) {
//    printf("null here.\n");
//  }
//  else {
  if (t != NULL) {
    printspace(cnt);
//    printf("->%c--%s\n", t->item, (t->data == NULL) ? "blank" : t->data);
//    printf("->%c--%s\n", t->item, (t->symD->data == NULL) ? "blank" : t->symD->data);
//    printspace(cnt);
    if (t->left != NULL) {
//      printf(" turning left\n");
      tstDump_(t->left, cnt+1);
    }
    else {
      printf(" left is null.\n");
    }
    printspace(cnt);
    if (t->middle != NULL) {
      printf(" go to middle\n");
      tstDump_(t->middle, cnt+1);
    }
    else {
      printf(" middle is null.\n");
    }
    printspace(cnt);
    if (t->right != NULL) {
      printf(" turning right\n");
      tstDump_(t->right, cnt+1);
    }
    else {
      printf(" right is null.\n");
    }
    symDataDump(t->symD);
  }
}  

/*
[s-0-0]
|---[t-1-1]
|   `---[h-2-2]
|       `---[e-3-3]
|---[h-1-1]
|   |---[e-2-2]
|   |   |---[o-3-3]
|   |   |   `---[r-4-4]
|   |   |       `---[e-5-5]
|   |   `---[l-3-3]
|   |       |---[l-4-4]
|   |           `---[s-5-5]
|   `---[e-2-2]
|       `---[l-3-3]
|           |---[l-4-4]
|           |   `---[s-5-5]
|           `---[a-4-4]
`---[b-1-1]
    `---[y-2-2]
*/

void tstDumpGraphical_(tstrie *t, int branch, int level) {
//  int ib = 0;
  int il = 0;
  if (t == NULL) {
    return;
  }
  else {
    for (il = 0; il < level; il++) {
      printf("|---");
    }

    printf("[%c-%0d-%0d]\n", t->item, branch, level);
    tstDumpGraphical_(t->right, branch+1, level+1);
    tstDumpGraphical_(t->middle, branch+1, level+1);
    tstDumpGraphical_(t->left, branch+1, level+1);
  }
}

//void tstDumpGraphical_(tstrie *t, int branch, int level) {
//  tstqueue *stack = NULL;
//  stack = tstqPushfront(stack, t);
//  printf("size of stack: %0d\n", tstqSize(stack));
//
//  while (tstqSize(stack) != 0) {
//    tstrie *trie;
//    tstqueue *hold;
//    int i;
//    //tstqDump(stack);
//    for (i = 0; i < tstqSize(stack); i++) {
//      printf("|---");
//    }
//    hold = tstqPopfront(&stack);
//    printf("[%c-%0d]\n", hold->item->item, tstqSize(stack));
//    if (hold->item->left != NULL) {
//      stack = tstqPushfront(stack, hold->item->left);
//    }
//    if (hold->item->middle != NULL) {
//      stack = tstqPushfront(stack, hold->item->middle);
//    }
//    if (hold->item->right != NULL) {
//      stack = tstqPushfront(stack, hold->item->right);
//    }
//    tstqDelete(hold);
//  }
//}


void tstDumpGraphical(tstrie *t) {
  tstDumpGraphical_(t, 0, 0);
}
