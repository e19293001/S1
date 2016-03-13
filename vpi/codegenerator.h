#ifndef codegenerator_h
#define codegenerator_h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "TernarySearchTrie.h"

typedef enum {
  cgTypeDWORD,
  cgTypeHALT,
  cgTypePUSH,
  cgTypePUSHC,
  cgTypePUSHWC,
  cgTypePUSHR,
  cgTypeCORA,
  cgTypeASP,
  cgTypeCALL,
  cgTypeJA,
  cgTypeJCT,
  cgTypeJP,
  cgTypeJN,
  cgTypeJZ,
  cgTypeJNZ,
  cgTypeJODD,
  cgTypeJZON,
  cgTypeJZOP,
  cgTypeADD,
  cgTypeSUB,
  cgTypeSTAV,
  cgTypeSTVA,
  cgTypeLOAD,
  cgTypeAWC,
  cgTypeDUPE,
  cgTypeESBA,
  cgTypeREBA,
  cgTypeZSP,
  cgTypeCMPS,
  cgTypeCMPU,
  cgTypeREV,
  cgTypeSHLL,
  cgTypeSHRL,
  cgTypeSHRA,
  cgTypeNEG,
  cgTypeMULT,
  cgTypeDIV,
  cgTypeREM,
  cgTypeADDY,
  cgTypeOR,
  cgTypeXOR,
  cgTypeFLIP,
  cgTypeAND,
  cgTypeRET,
  cgTypeCALI,
  cgTypeSCT,
  cgTypeROT,
  cgTypePSP,
  cgTypeBPBP,
  cgTypePOBP,
  cgTypePBP,
  cgTypeBCPY,
  cgTypeUOUT
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

void codeGenEmmitInstruction(codeGen *cg, int cgt, char *instruction);

void codeGenDelete(codeGen **cg);
#endif
