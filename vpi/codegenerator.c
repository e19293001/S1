#include "codegenerator.h"

void codeGenEmmitInstruction(codeGen *cg, int cgt, char *instruction) {
  if (cgt == cgTypeDWORD) {
    printf("+%s    %04X ; %s %s\n", cg->symD->programcounter, cg->symD->addressInt, instruction, cg->symD->name);
    fprintf(cg->fp, "+%s    %04X\n", cg->symD->programcounter, cg->symD->addressInt);
  }
  else if (cgt == cgTypePUSH) {
    printf("+%s    0%s ; %s %s\n", cg->symD->programcounter, cg->symD->address, instruction, cg->symD->name);
    fprintf(cg->fp, "+%s    0%s\n", cg->symD->programcounter, cg->symD->address);
  }
  else if (cgt == cgTypePUSHC) {
    printf("+%s    1%s ; %s %s\n", cg->symD->programcounter, cg->symD->address, instruction, cg->symD->name);
    fprintf(cg->fp, "+%s    1%s\n", cg->symD->programcounter, cg->symD->address);
  }
  else if (cgt == cgTypePUSHR) {
    printf("+%s    2%03X ; %s %s\n", cg->symD->programcounter, cg->symD->addressInt, instruction, cg->symD->name);
    fprintf(cg->fp, "+%s    2%03X\n", cg->symD->programcounter, cg->symD->addressInt);
  }
  else if (cgt == cgTypePUSHWC) {
    printf("+%s    F700 ; %s %s\n", cg->symD->programcounter, instruction, cg->symD->name);
    fprintf(cg->fp, "+%s    F700\n", cg->symD->programcounter);
    printf("+%04x    0%s\n", (atoi(cg->symD->programcounter)+1), cg->symD->address);
    fprintf(cg->fp, "+%04x    0%s\n", (atoi(cg->symD->programcounter)+1), cg->symD->address);
  }
  else if (cgt == cgTypeAWC) {
    printf("+%s    F700 ; %s %s\n", cg->symD->programcounter, instruction, cg->symD->name);
    fprintf(cg->fp, "+%s    F700\n", cg->symD->programcounter);
    printf("+%04x    0%s\n", (atoi(cg->symD->programcounter)+1), cg->symD->address);
    fprintf(cg->fp, "+%04x    0%s\n", (atoi(cg->symD->programcounter)+1), cg->symD->address);
  }
  else if (cgt == cgTypeCORA) {
    printf("+%s    3%s ; %s %s\n", cg->symD->programcounter, cg->symD->address, instruction, cg->symD->name);
    fprintf(cg->fp, "+%s    3%s\n", cg->symD->programcounter, cg->symD->address);
  }
  else if (cgt == cgTypeASP) {
    printf("+%s    4%s ; %s %s\n", cg->symD->programcounter, cg->symD->address, instruction, cg->symD->name);
    fprintf(cg->fp, "+%s    4%s\n", cg->symD->programcounter, cg->symD->address);
  }
  else if (cgt == cgTypeCALL) {
    printf("+%s    5%s ; %s %s\n", cg->symD->programcounter, cg->symD->address, instruction, cg->symD->name);
    fprintf(cg->fp, "+%s    5%s\n", cg->symD->programcounter, cg->symD->address);
  }
  else if (cgt == cgTypeJA) {
    printf("+%s    6%s ; %s %s\n", cg->symD->programcounter, cg->symD->address, instruction, cg->symD->name);
    fprintf(cg->fp, "+%s    6%s\n", cg->symD->programcounter, cg->symD->address);
  }
  else if (cgt == cgTypeJCT) {
    printf("+%s    7%s ; %s %s\n", cg->symD->programcounter, cg->symD->address, instruction, cg->symD->name);
    fprintf(cg->fp, "+%s    7%s\n", cg->symD->programcounter, cg->symD->address);
  }
  else if (cgt == cgTypeJP) {
    printf("+%s    8%s ; %s %s\n", cg->symD->programcounter, cg->symD->address, instruction, cg->symD->name);
    fprintf(cg->fp, "+%s    8%s\n", cg->symD->programcounter, cg->symD->address);
  }
  else if (cgt == cgTypeJN) {
    printf("+%s    9%s ; %s %s\n", cg->symD->programcounter, cg->symD->address, instruction, cg->symD->name);
    fprintf(cg->fp, "+%s    9%s\n", cg->symD->programcounter, cg->symD->address);
  }
  else if (cgt == cgTypeJZ) {
    printf("+%s    A%s ; %s %s\n", cg->symD->programcounter, cg->symD->address, instruction, cg->symD->name);
    fprintf(cg->fp, "+%s    A%s\n", cg->symD->programcounter, cg->symD->address);
  }
  else if (cgt == cgTypeJNZ) {
    printf("+%s    B%s ; %s %s\n", cg->symD->programcounter, cg->symD->address, instruction, cg->symD->name);
    fprintf(cg->fp, "+%s    B%s\n", cg->symD->programcounter, cg->symD->address);
  }
  else if (cgt == cgTypeJODD) {
    printf("+%s    C%s ; %s %s\n", cg->symD->programcounter, cg->symD->address, instruction, cg->symD->name);
    fprintf(cg->fp, "+%s    C%s\n", cg->symD->programcounter, cg->symD->address);
  }
  else if (cgt == cgTypeJZON) {
    printf("+%s    D%s ; %s %s\n", cg->symD->programcounter, cg->symD->address, instruction, cg->symD->name);
    fprintf(cg->fp, "+%s    D%s\n", cg->symD->programcounter, cg->symD->address);
  }
  else if (cgt == cgTypeJZOP) {
    printf("+%s    E%s ; %s %s\n", cg->symD->programcounter, cg->symD->address, instruction, cg->symD->name);
    fprintf(cg->fp, "+%s    E%s\n", cg->symD->programcounter, cg->symD->address);
  }
  else if (cgt == cgTypeRET) {
    printf("+%s    F000 ; %s\n", cg->symD->programcounter, instruction);
    fprintf(cg->fp, "+%s    F000\n", cg->symD->programcounter);
  }
  else if (cgt == cgTypeADD) {
    printf("+%s    F100 ; %s\n", cg->symD->programcounter, instruction);
    fprintf(cg->fp, "+%s    F100\n", cg->symD->programcounter);
  }
  else if (cgt == cgTypeSUB) {
    printf("+%s    F200 ; %s\n", cg->symD->programcounter, instruction);
    fprintf(cg->fp, "+%s    F200\n", cg->symD->programcounter);
  }
  else if (cgt == cgTypeSTAV) {
    printf("+%s    F300 ; %s\n", cg->symD->programcounter, instruction);
    fprintf(cg->fp, "+%s    F300\n", cg->symD->programcounter);
  }
  else if (cgt == cgTypeSTVA) {
    printf("+%s    F400 ; %s\n", cg->symD->programcounter, instruction);
    fprintf(cg->fp, "+%s    F400\n", cg->symD->programcounter);
  }
  else if (cgt == cgTypeLOAD) {
    printf("+%s    F500 ; %s\n", cg->symD->programcounter, instruction);
    fprintf(cg->fp, "+%s    F500\n", cg->symD->programcounter);
  }
  else if (cgt == cgTypeDUPE) {
    printf("+%s    F800 ; %s\n", cg->symD->programcounter, instruction);
    fprintf(cg->fp, "+%s    F800\n", cg->symD->programcounter);
  }
  else if (cgt == cgTypeESBA) {
    printf("+%s    F900 ; %s\n", cg->symD->programcounter, instruction);
    fprintf(cg->fp, "+%s    F900\n", cg->symD->programcounter);
  }
  else if (cgt == cgTypeREBA) {
    printf("+%s    FA00 ; %s\n", cg->symD->programcounter, instruction);
    fprintf(cg->fp, "+%s    FA00\n", cg->symD->programcounter);
  }
  else if (cgt == cgTypeZSP) {
    printf("+%s    FB00 ; %s\n", cg->symD->programcounter, instruction);
    fprintf(cg->fp, "+%s    FB00\n", cg->symD->programcounter);
  }
  else if (cgt == cgTypeCMPS) {
    printf("+%s    FC%02X ; %s %s\n", cg->symD->programcounter, cg->symD->addressInt, instruction, cg->symD->name);
    fprintf(cg->fp, "+%s    FC%02X\n", cg->symD->programcounter, cg->symD->addressInt);
  }
  else if (cgt == cgTypeCMPU) {
    printf("+%s    FD%02X ; %s %s\n", cg->symD->programcounter, cg->symD->addressInt, instruction, cg->symD->name);
    fprintf(cg->fp, "+%s    FD%02X\n", cg->symD->programcounter, cg->symD->addressInt);
  }
  else if (cgt == cgTypeREV) {
    printf("+%s    FE00 ; %s\n", cg->symD->programcounter, instruction);
    fprintf(cg->fp, "+%s    FE00\n", cg->symD->programcounter);
  }
  else if (cgt == cgTypeSHLL) {
    printf("+%s    FF0%01X ; %s %s\n", cg->symD->programcounter, (cg->symD->addressInt & 0xF), instruction, cg->symD->name);
    fprintf(cg->fp, "+%s    FF0%01X\n", cg->symD->programcounter, (cg->symD->addressInt & 0xF));
  }
  else if (cgt == cgTypeSHRL) {
    printf("+%s    FF1%01X ; %s %s\n", cg->symD->programcounter, (cg->symD->addressInt & 0xF), instruction, cg->symD->name);
    fprintf(cg->fp, "+%s    FF1%01X\n", cg->symD->programcounter, (cg->symD->addressInt & 0xF));
  }
  else if (cgt == cgTypeSHRA) {
    printf("+%s    FF2%01X ; %s %s\n", cg->symD->programcounter, (cg->symD->addressInt & 0xF), instruction, cg->symD->name);
    fprintf(cg->fp, "+%s    FF2%01X\n", cg->symD->programcounter, (cg->symD->addressInt & 0xF));
  }
  else if (cgt == cgTypeNEG) {
    printf("+%s    FF30 ; %s\n", cg->symD->programcounter, instruction);
    fprintf(cg->fp, "+%s    FF30\n", cg->symD->programcounter);
  }
  else if (cgt == cgTypeMULT) {
    printf("+%s    FF40 ; %s\n", cg->symD->programcounter, instruction);
    fprintf(cg->fp, "+%s    FF40\n", cg->symD->programcounter);
  }
  else if (cgt == cgTypeDIV) {
    printf("+%s    FF50 ; %s\n", cg->symD->programcounter, instruction);
    fprintf(cg->fp, "+%s    FF50\n", cg->symD->programcounter);
  }
  else if (cgt == cgTypeREM) {
    printf("+%s    FF60 ; %s\n", cg->symD->programcounter, instruction);
    fprintf(cg->fp, "+%s    FF60\n", cg->symD->programcounter);
  }
  else if (cgt == cgTypeADDY) {
    printf("+%s    FF70 ; %s\n", cg->symD->programcounter, instruction);
    fprintf(cg->fp, "+%s    FF70\n", cg->symD->programcounter);
  }
  else if (cgt == cgTypeOR) {
    printf("+%s    FF80 ; %s\n", cg->symD->programcounter, instruction);
    fprintf(cg->fp, "+%s    FF80\n", cg->symD->programcounter);
  }
  else if (cgt == cgTypeXOR) {
    printf("+%s    FF90 ; %s\n", cg->symD->programcounter, instruction);
    fprintf(cg->fp, "+%s    FF90\n", cg->symD->programcounter);
  }
  else if (cgt == cgTypeAND) {
    printf("+%s    FFA0 ; %s\n", cg->symD->programcounter, instruction);
    fprintf(cg->fp, "+%s    FFA0\n", cg->symD->programcounter);
  }
  else if (cgt == cgTypeFLIP) {
    printf("+%s    FFB0 ; %s\n", cg->symD->programcounter, instruction);
    fprintf(cg->fp, "+%s    FFB0\n", cg->symD->programcounter);
  }
  else if (cgt == cgTypeCALI) {
    printf("+%s    FFC0 ; %s\n", cg->symD->programcounter, instruction);
    fprintf(cg->fp, "+%s    FFC0\n", cg->symD->programcounter);
  }
  else if (cgt == cgTypeSCT) {
    printf("+%s    FFD0 ; %s\n", cg->symD->programcounter, instruction);
    fprintf(cg->fp, "+%s    FFD0\n", cg->symD->programcounter);
  }
  else if (cgt == cgTypeROT) {
    printf("+%s    FFE0 ; %s\n", cg->symD->programcounter, instruction);
    fprintf(cg->fp, "+%s    FFE0\n", cg->symD->programcounter);
  }
  else if (cgt == cgTypePSP) {
    printf("+%s    FFF0 ; %s\n", cg->symD->programcounter, instruction);
    fprintf(cg->fp, "+%s    FFF0\n", cg->symD->programcounter);
  }
  else if (cgt == cgTypeBPBP) {
    printf("+%s    FFF1 ; %s\n", cg->symD->programcounter, instruction);
    fprintf(cg->fp, "+%s    FFF1\n", cg->symD->programcounter);
  }
  else if (cgt == cgTypePOBP) {
    printf("+%s    FFF2 ; %s\n", cg->symD->programcounter, instruction);
    fprintf(cg->fp, "+%s    FFF2\n", cg->symD->programcounter);
  }
  else if (cgt == cgTypePBP) {
    printf("+%s    FFF3 ; %s\n", cg->symD->programcounter, instruction);
    fprintf(cg->fp, "+%s    FFF3\n", cg->symD->programcounter);
  }
  else if (cgt == cgTypeBCPY) {
    printf("+%s    FFF4 ; %s\n", cg->symD->programcounter, instruction);
    fprintf(cg->fp, "+%s    FFF4\n", cg->symD->programcounter);
  }
  else if (cgt == cgTypeUOUT) {
    printf("+%s    FFF5 ; %s\n", cg->symD->programcounter, instruction);
    fprintf(cg->fp, "+%s    FFF5\n", cg->symD->programcounter);
  }
  else if (cgt == cgTypeSIN) {
    printf("+%s    FFF6 ; %s\n", cg->symD->programcounter, instruction);
    fprintf(cg->fp, "+%s    FFF6\n", cg->symD->programcounter);
  }
  else if (cgt == cgTypeSOUT) {
    printf("+%s    FFF7 ; %s\n", cg->symD->programcounter, instruction);
    fprintf(cg->fp, "+%s    FFF7\n", cg->symD->programcounter);
  }
  else if (cgt == cgTypeHIN) {
    printf("+%s    FFF8 ; %s\n", cg->symD->programcounter, instruction);
    fprintf(cg->fp, "+%s    FFF8\n", cg->symD->programcounter);
  }
  else if (cgt == cgTypeHOUT) {
    printf("+%s    FFF9 ; %s\n", cg->symD->programcounter, instruction);
    fprintf(cg->fp, "+%s    FFF9\n", cg->symD->programcounter);
  }
  else if (cgt == cgTypeAIN) {
    printf("+%s    FFFA ; %s\n", cg->symD->programcounter, instruction);
    fprintf(cg->fp, "+%s    FFFA\n", cg->symD->programcounter);
  }
  else if (cgt == cgTypeAOUT) {
    printf("+%s    FFFB ; %s\n", cg->symD->programcounter, instruction);
    fprintf(cg->fp, "+%s    FFFB\n", cg->symD->programcounter);
  }
  else if (cgt == cgTypeDIN) {
    printf("+%s    FFFC ; %s\n", cg->symD->programcounter, instruction);
    fprintf(cg->fp, "+%s    FFFC\n", cg->symD->programcounter);
  }
  else if (cgt == cgTypeDOUT) {
    printf("+%s    FFFD ; %s\n", cg->symD->programcounter, instruction);
    fprintf(cg->fp, "+%s    FFFD\n", cg->symD->programcounter);
  }
  else if (cgt == cgTypeNOOP) {
    printf("+%s    FFFE ; %s\n", cg->symD->programcounter, instruction);
    fprintf(cg->fp, "+%s    FFFE\n", cg->symD->programcounter);
  }
  else if (cgt == cgTypeHALT) {
    printf("+%s    FFFF ; %s\n", cg->symD->programcounter, instruction);
    fprintf(cg->fp, "+%s    FFFF\n", cg->symD->programcounter);
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

