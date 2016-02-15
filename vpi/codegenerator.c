#include "codegenerator.h"

void codeGenEmmitInstruction(codeGen *cg, int cgt) {
  // if cg->type . . .
  if (cgt == cgTypeDWORD) {
    fprintf(cg->fp, "+%s %s\n", cg->symD->address, cg->symD->name);
    printf("+%s %s\n", cg->symD->address, cg->symD->name);
  }
  else if (cgt == cgTypePUSH) {
    printf("+%s 0%s\n", cg->symD->address, cg->symD->name);
//    printf("+%s %s%s\n", cg->symD->address, cg->symD->name, cg->symD->name);
//    fprintf(cg->fp, "+%s %s%s\n", cg->symD->address, cg->symD->name, cg->symD->name);
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

