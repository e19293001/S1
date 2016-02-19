#include "codegenerator.h"

void codeGenEmmitInstruction(codeGen *cg, int cgt, char *instruction) {
  // if cg->type . . .
  if (cgt == cgTypeDWORD) {
    printf("+%s    %04x ; %s %s\n", cg->symD->programcounter, cg->symD->addressInt, instruction, cg->symD->name);
  }
  else if (cgt == cgTypePUSH) {
    printf("+%s    0%s ; %s %s\n", cg->symD->programcounter, cg->symD->address, instruction, cg->symD->name);
  }
  else if (cgt == cgTypePUSHC) {
    printf("+%s    1%s ; %s %s\n", cg->symD->programcounter, cg->symD->address, instruction, cg->symD->name);
  }
  else if (cgt == cgTypePUSHWC) {
    printf("+%s    F700 ; %s %s\n", cg->symD->programcounter, instruction, cg->symD->name);
    printf("+%04x    0%s\n", (atoi(cg->symD->programcounter)+1), cg->symD->address);
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

void codeGenDelete(codeGen *cg) {
  fclose(cg->fp);
  free(cg->filename);
  free(cg);
}

