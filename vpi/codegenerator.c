#include "codegenerator.h"

void codeGenEmmitInstruction(codeGen *cg, int cgt, char *instruction) {
  if (cgt == cgTypeDWORD) {
    printf("+%s    %04x ; %s %s\n", cg->symD->programcounter, cg->symD->addressInt, instruction, cg->symD->name);
  }
  else if (cgt == cgTypePUSH) {
    printf("+%s    0%s ; %s %s\n", cg->symD->programcounter, cg->symD->address, instruction, cg->symD->name);
  }
  else if (cgt == cgTypePUSHC) {
    printf("+%s    1%s ; %s %s\n", cg->symD->programcounter, cg->symD->address, instruction, cg->symD->name);
  }
  else if (cgt == cgTypePUSHR) {
    printf("+%s    2%03x ; %s %s\n", cg->symD->programcounter, cg->symD->addressInt, instruction, cg->symD->name);
  }
  else if (cgt == cgTypePUSHWC) {
    printf("+%s    F700 ; %s %s\n", cg->symD->programcounter, instruction, cg->symD->name);
    printf("+%04x    0%s\n", (atoi(cg->symD->programcounter)+1), cg->symD->address);
  }
  else if (cgt == cgTypeAWC) {
    printf("+%s    F700 ; %s %s\n", cg->symD->programcounter, instruction, cg->symD->name);
    printf("+%04x    0%s\n", (atoi(cg->symD->programcounter)+1), cg->symD->address);
  }
  else if (cgt == cgTypeCORA) {
    printf("+%s    3%s ; %s %s\n", cg->symD->programcounter, cg->symD->address, instruction, cg->symD->name);
  }
  else if (cgt == cgTypeASP) {
    printf("+%s    4%s ; %s %s\n", cg->symD->programcounter, cg->symD->address, instruction, cg->symD->name);
  }
  else if (cgt == cgTypeCALL) {
    printf("+%s    5%s ; %s %s\n", cg->symD->programcounter, cg->symD->address, instruction, cg->symD->name);
  }
  else if (cgt == cgTypeJA) {
    printf("+%s    6%s ; %s %s\n", cg->symD->programcounter, cg->symD->address, instruction, cg->symD->name);
  }
  else if (cgt == cgTypeJCT) {
    printf("+%s    7%s ; %s %s\n", cg->symD->programcounter, cg->symD->address, instruction, cg->symD->name);
  }
  else if (cgt == cgTypeJP) {
    printf("+%s    8%s ; %s %s\n", cg->symD->programcounter, cg->symD->address, instruction, cg->symD->name);
  }
  else if (cgt == cgTypeJN) {
    printf("+%s    9%s ; %s %s\n", cg->symD->programcounter, cg->symD->address, instruction, cg->symD->name);
  }
  else if (cgt == cgTypeJZ) {
    printf("+%s    A%s ; %s %s\n", cg->symD->programcounter, cg->symD->address, instruction, cg->symD->name);
  }
  else if (cgt == cgTypeJNZ) {
    printf("+%s    B%s ; %s %s\n", cg->symD->programcounter, cg->symD->address, instruction, cg->symD->name);
  }
  else if (cgt == cgTypeJODD) {
    printf("+%s    C%s ; %s %s\n", cg->symD->programcounter, cg->symD->address, instruction, cg->symD->name);
  }
  else if (cgt == cgTypeJZON) {
    printf("+%s    D%s ; %s %s\n", cg->symD->programcounter, cg->symD->address, instruction, cg->symD->name);
  }
  else if (cgt == cgTypeJZOP) {
    printf("+%s    E%s ; %s %s\n", cg->symD->programcounter, cg->symD->address, instruction, cg->symD->name);
  }
  else if (cgt == cgTypeRET) {
    printf("+%s    F000 ; %s\n", cg->symD->programcounter, instruction);
  }
  else if (cgt == cgTypeADD) {
    printf("+%s    F100 ; %s\n", cg->symD->programcounter, instruction);
  }
  else if (cgt == cgTypeSUB) {
    printf("+%s    F200 ; %s\n", cg->symD->programcounter, instruction);
  }
  else if (cgt == cgTypeSTAV) {
    printf("+%s    F300 ; %s\n", cg->symD->programcounter, instruction);
  }
  else if (cgt == cgTypeSTVA) {
    printf("+%s    F400 ; %s\n", cg->symD->programcounter, instruction);
  }
  else if (cgt == cgTypeLOAD) {
    printf("+%s    F500 ; %s\n", cg->symD->programcounter, instruction);
  }
  else if (cgt == cgTypeDUPE) {
    printf("+%s    F800 ; %s\n", cg->symD->programcounter, instruction);
  }
  else if (cgt == cgTypeESBA) {
    printf("+%s    F900 ; %s\n", cg->symD->programcounter, instruction);
  }
  else if (cgt == cgTypeREBA) {
    printf("+%s    FA00 ; %s\n", cg->symD->programcounter, instruction);
  }
  else if (cgt == cgTypeZSP) {
    printf("+%s    FB00 ; %s\n", cg->symD->programcounter, instruction);
  }
  else if (cgt == cgTypeCMPS) {
    printf("+%s    FC%02X ; %s %s\n", cg->symD->programcounter, cg->symD->addressInt, instruction, cg->symD->name);
  }
  else if (cgt == cgTypeCMPU) {
    printf("+%s    FD%02X ; %s %s\n", cg->symD->programcounter, cg->symD->addressInt, instruction, cg->symD->name);
  }
  else if (cgt == cgTypeREV) {
    printf("+%s    FE00 ; %s\n", cg->symD->programcounter, instruction);
  }
  else if (cgt == cgTypeSHLL) {
    printf("+%s    FF0%01X ; %s %s\n", cg->symD->programcounter, (cg->symD->addressInt & 0xF), instruction, cg->symD->name);
  }
  else if (cgt == cgTypeSHRL) {
    printf("+%s    FF1%01X ; %s %s\n", cg->symD->programcounter, (cg->symD->addressInt & 0xF), instruction, cg->symD->name);
  }
  else if (cgt == cgTypeSHRA) {
    printf("+%s    FF2%01X ; %s %s\n", cg->symD->programcounter, (cg->symD->addressInt & 0xF), instruction, cg->symD->name);
  }
  else if (cgt == cgTypeNEG) {
    printf("+%s    FF30 ; %s\n", cg->symD->programcounter, instruction);
  }
  else if (cgt == cgTypeMULT) {
    printf("+%s    FF40 ; %s\n", cg->symD->programcounter, instruction);
  }
  else if (cgt == cgTypeDIV) {
    printf("+%s    FF50 ; %s\n", cg->symD->programcounter, instruction);
  }
  else if (cgt == cgTypeREM) {
    printf("+%s    FF60 ; %s\n", cg->symD->programcounter, instruction);
  }
  else if (cgt == cgTypeADDY) {
    printf("+%s    FF70 ; %s\n", cg->symD->programcounter, instruction);
  }
  else if (cgt == cgTypeOR) {
    printf("+%s    FF80 ; %s\n", cg->symD->programcounter, instruction);
  }
  else if (cgt == cgTypeXOR) {
    printf("+%s    FF90 ; %s\n", cg->symD->programcounter, instruction);
  }
  else if (cgt == cgTypeAND) {
    printf("+%s    FFA0 ; %s\n", cg->symD->programcounter, instruction);
  }
  else if (cgt == cgTypeFLIP) {
    printf("+%s    FFB0 ; %s\n", cg->symD->programcounter, instruction);
  }
  else if (cgt == cgTypeCALI) {
    printf("+%s    FFC0 ; %s\n", cg->symD->programcounter, instruction);
  }
  else if (cgt == cgTypeSCT) {
    printf("+%s    FFD0 ; %s\n", cg->symD->programcounter, instruction);
  }
  else if (cgt == cgTypeROT) {
    printf("+%s    FFE0 ; %s\n", cg->symD->programcounter, instruction);
  }
  else if (cgt == cgTypePSP) {
    printf("+%s    FFF0 ; %s\n", cg->symD->programcounter, instruction);
  }
  else if (cgt == cgTypeBPBP) {
    printf("+%s    FFF1 ; %s\n", cg->symD->programcounter, instruction);
  }
  else if (cgt == cgTypePOBP) {
    printf("+%s    FFF2 ; %s\n", cg->symD->programcounter, instruction);
  }
  else if (cgt == cgTypePBP) {
    printf("+%s    FFF3 ; %s\n", cg->symD->programcounter, instruction);
  }
  else if (cgt == cgTypeBCPY) {
    printf("+%s    FFF4 ; %s\n", cg->symD->programcounter, instruction);
  }
  else if (cgt == cgTypeUOUT) {
    printf("+%s    FFF5 ; %s\n", cg->symD->programcounter, instruction);
  }
  else if (cgt == cgTypeSIN) {
    printf("+%s    FFF6 ; %s\n", cg->symD->programcounter, instruction);
  }
  else if (cgt == cgTypeSOUT) {
    printf("+%s    FFF7 ; %s\n", cg->symD->programcounter, instruction);
  }
  else if (cgt == cgTypeHIN) {
    printf("+%s    FFF8 ; %s\n", cg->symD->programcounter, instruction);
  }
  else if (cgt == cgTypeHALT) {
    printf("+%s    FFFF ; %s\n", cg->symD->programcounter, instruction);
  }
  else {
    printf("unknown cgType.\n");
  }
}

codeGen *codeGenNew(char *s) {
  codeGen *ret;
  ret = malloc(sizeof(codeGen));
  ret->filename = calloc(strlen(s)+1, sizeof(char));
  strncpy(ret->filename, s, strlen(s));
  ret->symD = NULL;
  if ((ret->fp = fopen(s, "w")) == NULL) {
    printf("[ codeGenNew ] can not open file: %s\n", s);
  }
  return ret;
}

void codeGenDelete(codeGen **cg) {
  fclose((*cg)->fp);
  free((*cg)->filename);
  free(*cg);
  *cg = NULL;
}

