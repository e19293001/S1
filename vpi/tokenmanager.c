#include "tokenmanager.h"

const char *tokenImage[] = {
  "_EOF",
  "PUSH",
  "PUSHC",
  "PUSHWC",
  "HALT",
  "ID",
  "UNSIGNED",
  "OPERATOR",
  "DWORD",
  "SEMICOL",
  "COLON",
  "ERROR"
};


TokenManager* TokenManagerNew(char *inFileName) {
  TokenManager *ret = malloc(sizeof(TokenManager));
  if ((ret->inFile = fopen(inFileName, "r")) == NULL) {
    printf("could not open file: %s\n", inFileName);
    exit(-1);
  }
  ret->currentChar = '\n';
  ret->currentColumnNumber = 0;
  ret->currentLineNumber = 0;
  memset(ret->inputLine, '\0', 512);
  ret->tok = NULL;
  return ret;
}

void getNextChar(TokenManager **t) {
  TokenManager *tm = *t;

//  printf("getNextChar\n");
  if (tm->currentChar == EOF) {
    return;
  }
//  printf("getNextChar\n");

// TODO: add functionality here
  if (tm->currentChar == '\n' ||
      tm->currentChar == '\r') {
    if ((fgets(tm->inputLine, 512, tm->inFile)) == NULL) {
      printf("reached end of file\n");
      tm->currentChar = EOF;
      return;
    }
    else {
      int sz;
      sz = strlen(tm->inputLine);
      tm->inputLine[sz-1] = '\0';
      printf("[%s]\n", tm->inputLine);
      tm->inputLine[sz-1] = '\n';
      tm->currentColumnNumber = 0;
      tm->currentLineNumber++;
    }
  }

  tm->currentChar = tm->inputLine[tm->currentColumnNumber++];
}

Token TokenManagerGetNextToken(TokenManager **t) {
  Token ret;

  while (isspace((*t)->currentChar)) {
//    printf("white space\n");
    getNextChar(t);
  }

  ret.next = NULL;
  ret.beginLine = (*t)->currentLineNumber;
  ret.beginColumn = (*t)->currentColumnNumber;

  if ((*t)->currentChar == EOF) {
    memset(ret.image, '\0',512);
    strncpy(ret.image, "<EOF>", 512);
    ret.endLine = (*t)->currentLineNumber;
    ret.endColumn = (*t)->currentColumnNumber;
    ret.kind = _EOF;
  }
  else if (isdigit((*t)->currentChar)) {
    int indxToImage = 0;
    memset(&ret.image, '\0', 512);
//    printf("after memset -- %s\n", ret.image);
    do {
//      printf("[%c]", (*t)->currentChar);
      ret.image[indxToImage++] = (*t)->currentChar;
//      printf(" -- %s", ret.image);
      ret.endLine = (*t)->currentLineNumber;
      ret.endColumn = (*t)->currentColumnNumber;
      getNextChar(t);
    } while (isdigit((*t)->currentChar));
    ret.kind = UNSIGNED;
//    printf("found digit. %s\n", ret.image);
  }
  else if (isalpha((*t)->currentChar)) {
    int indxToImage = 0;
    memset(ret.image, '\0', 512);
    do {
//      printf("[%c]", (*t)->currentChar);
      ret.image[indxToImage++] = (*t)->currentChar;
      ret.endLine = (*t)->currentLineNumber;
      ret.endColumn = (*t)->currentColumnNumber;
      getNextChar(t);
    } while (isalnum((*t)->currentChar));
    ret.image[indxToImage] = '\0';

    if ((strncmp(ret.image, "p", 512)) == 0) {
      ret.kind = PUSH;
    }
    else if ((strncmp(ret.image, "pc", 512)) == 0) {
      ret.kind = PUSHC;
    }
    else if ((strncmp(ret.image, "pwc", 512)) == 0) {
      ret.kind = PUSHWC;
    }
    else if ((strncmp(ret.image, "halt", 512)) == 0) {
      ret.kind = HALT;
    }
    else if ((strncmp(ret.image, "dw", 512)) == 0) {
      ret.kind = DWORD;
    }
    else {
      //printf("\nfound ID. %s\n", ret.image);
      ret.kind = ID;
    }
  }
  else {
    switch((*t)->currentChar) {
    case '+': {
      getNextChar(t);
      memset(ret.image, '\0', 512);
      ret.image[0] = '+';
      ret.kind = OPERATOR;
      break;
    }
    case '-': {
      getNextChar(t);
      memset(ret.image, '\0', 512);
      ret.image[0] = '-';
      ret.kind = OPERATOR;
      break;
    }
    case ';': {
      getNextChar(t);
      memset(ret.image, '\0', 512);
      ret.image[0] = ';';
      ret.kind = SEMICOL;
      break;
    }
    case ':': {
      getNextChar(t);
      memset(ret.image, '\0', 512);
      ret.image[0] = ':';
      ret.kind = COLON;
      break;
    }
    case '=': {
      getNextChar(t);
      memset(ret.image, '\0', 512);
      ret.image[0] = '=';
      ret.kind = OPERATOR;
      break;
    }
    default: {
      printf("error token found: %c\n", (*t)->currentChar);
      getNextChar(t);
      ret.kind = ERROR;
    }
    }
  }

  return ret;
}

void TokenManagerDelete(TokenManager* t) {
  fclose(t->inFile);
  free(t);
}
