#include "codegenerator.h"

void codeGenEmmitInstruction(codeGen *cg, int cgt) {
  // if cg->type . . .
  if (cgt == cgTypeDWORD) {
    fprintf(cg->fp, "+%04d %s\n", cg->address, cg->opcode);
    printf("+%04d %s\n", cg->address, cg->opcode);
  }
  else if (cgt == cgTypePUSH) {
    printf("+%04d %s%s\n", cg->address, cg->opcode, cg->operand);
    fprintf(cg->fp, "+%04d %s%s\n", cg->address, cg->operand, cg->opcode);
  }
}

codeGen *codeGenNew(char *s) {
  codeGen *ret;
  ret = malloc(sizeof(codeGen));
  ret->filename = calloc(strlen(s)+1, sizeof(char));
  strncpy(ret->filename, s, strlen(s));
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

