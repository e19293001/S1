#include "TernarySearchTrie.h"
#include "QueueTernarySearchTrie.h"

tstrie* tstSearchR(tstrie *t, char *s, int i, int cntr);

void printspace(int cnt) {
  int x;
  for (x = 0; x < cnt+1; x++) {
    printf(" ");
  }
}

tstrie* tstNew(char i, int d) {
  tstrie* ret;

//  printf("[ tstNew ] creating...\n");
  ret = malloc(sizeof(tstrie));
  ret->item = i;
  ret->data = d;
  ret->left = NULL;
  ret->middle = NULL;
  ret->right = NULL;
//  printf("[ tstNew ] done.\n");

  return ret;
}

tstrie* tstSearchR(tstrie *t, char *s, int i, int cntr) {
  char val = s[i];
  int x;

  for (x = 0; x < cntr+1; x++) {
    printf(" ");
  }
//  (t == NULL) ? 
//    printf("val: %c t is null\n", val) :
//    printf("val: %c t->item: %c ", val, t->item);
  
  if (t == NULL) {
    printf("t is null\n");
    return NULL;
  }
  if (val < t->item) {
    printf("current: val: %c t->item: %c ", val, t->item);
    printf("[ turning left.\n");
    return tstSearchR(t->left, s, i, cntr+1);
  }
  if (val > t->item) {
    printf("current: val: %c t->item: %c ", val, t->item);
    printf("[ turning right.\n");
    return tstSearchR(t->right, s, i, cntr+1);
  }
  if (i < strlen(s)-1) {
    printf("current: val: %c t->item: %c ", val, t->item);
    printf("[ going to middle.\n");
    return tstSearchR(t->middle, s, i+1, cntr+1);
  }

  printf("returning... current: val: %c t->item: %c\n", val, t->item);

  return t;
}

tstrie* tstSearch(tstrie *t, char *s) {
  tstrie *ret;

  printf("--- [ searching for: %s\n", s);
  ret = tstSearchR(t, s, 0, 0);

  return ret;
}

tstrie* tstInsertR(tstrie *t, char *s, int val, int i, int cntr) {
  char charindxd = s[i];
//  tstrie *ret = t;
  printspace(cntr);
  printf("[ tstInsert ] s: %s val: %0d i: %0d cntr: %0d\n", s, val, i, cntr);
  //printf("++++++++++++++++++++++++++++++++++++++\n");
  if (t == NULL) {
//    printf("t is NULL ");
//    printf("inserting: %c\n", charindxd);
    t = tstNew(charindxd, 888);
    //tstDump(t);
  }
  if (charindxd < t->item) {
//    printf("[ turning left.\n");
    //printf("current: val: %c t->item: %c\n", val, t->item);
    t->left =  tstInsertR(t->left, s, val, i, cntr+1);
  }
  else if (charindxd > t->item) {
//    printf("[ turning right.\n");
    //printf("current: val: %c t->item: %c\n", val, t->item);
    t->right = tstInsertR(t->right, s, val, i, cntr+1);
  }
  else if (i < strlen(s)-1) {
//    printf("[ going to middle.\n");
    //printf("current: val: %c t->item: %c\n", val, t->item);
    t->middle = tstInsertR(t->middle, s, val, i+1, cntr+1);
  }
  else {
    printf("[ assigning value: %d\n", val);
    t->data = val;
  }
//  printf("returning %c\n", ret->item);
  return t;
}

tstrie* tstInsert(tstrie *t, char *s, int val) {
  tstrie *ret;

  printf("++++++++++++++++++++++++++++++++++++++ start\n");
  printf("inserting [%s]\n", s);
  ret = tstInsertR(t, s, val, 0, 0);
  printf("++++++++++++++++++++++++++++++++++++++ end\n");

  return ret;
}

void tstCopy(tstrie *src, tstrie **dst) {
//  tstDump(src);
  (*dst) = tstNew(src->item, src->data);
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

void tstDelete(tstrie *t) {
//  printf("[ tstDelete ]\n");
  if (t != NULL) {
    if (t->left != NULL) {
      tstDelete(t->left);
    }
    if (t->middle != NULL) {
      tstDelete(t->middle);
    }
    if (t->right != NULL) {
      tstDelete(t->right);
    }
    free(t);
  }
}


void tstDump(tstrie *t) {
  printf("[\n");
  tstDump_(t, 0);
  printf("]");
  printf("\n");
}

void tstDump_(tstrie *t, int cnt) {
  if (t == NULL) {
    printf("null here.\n");
  }
  else {
    printspace(cnt);
    printf("->%c--%0d\n", t->item, t->data);
    printspace(cnt);
    if (t->left != NULL) {
      printf(" turning left\n");
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
