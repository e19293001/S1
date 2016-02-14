#ifndef codegenerator_h
#define codegenerator_h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "TernarySearchTrie.h"

typedef enum {
  cgTypeDWORD,
  cgTypePUSH
} cgType;

typedef struct codeGen {
  FILE *fp;
//  int type;

//  char *opcode;
//  char *operand;
//  int address;
  symData *symD;

  char *filename;
} codeGen;

codeGen *codeGenNew(char *s);

void codeGenEmmitInstruction(codeGen *cg, int cgt);

void codeGenDelete(codeGen *cg);
#endif
