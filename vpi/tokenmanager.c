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
  "PUSHR",
  "CORA",
  "ASP",
  "CALL",
  "JA",
  "JCT",
  "JP",
  "JN",
  "JZ",
  "JNZ",
  "JODD",
  "JZON",
  "JZOP",
  "RET",
  "ADD",
  "SUB",
  "STAV",
  "STVA",
  "LOAD",
  "AWC",
  "DUPE",
  "ESBA",
  "REBA",
  "ZSP",
  "CMPS",
  "CMPU",
  "REV",
  "SHLL",
  "SHRL",
  "ERROR"
};


TokenManager* TokenManagerNew(char *inFileName) {
  TokenManager *ret = malloc(sizeof(TokenManager));
  //printf("opening: %s\n", inFileName);
  if ((ret->inFile = fopen(inFileName, "r")) == NULL) {
    printf("could not open file: %s\n", inFileName);
    exit(-1);
  }
  //printf("ret->inFile: %0d\n", ret->inFile);
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
//      printf("reached end of file\n");
      tm->currentChar = EOF;
      return;
    }
    else {
      int sz;
      sz = strlen(tm->inputLine);
      tm->inputLine[sz-1] = '\0';
//      printf("[ --- %s --- ]\n", tm->inputLine);
      printf("[%s]\n", tm->inputLine);
      tm->inputLine[sz-1] = '\n';
      tm->currentColumnNumber = 0;
      tm->currentLineNumber++;
    }
  }

  tm->currentChar = tm->inputLine[tm->currentColumnNumber++];
  if (tm->currentChar == ';') {
    tm->currentChar = '\n';
  }
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
  else if (isalpha((*t)->currentChar) ||
           (*t)->currentChar == '@' ||
           (*t)->currentChar == '_') {
    int indxToImage = 0;
    memset(ret.image, '\0', 512);
    do {
//      printf("[%c]", (*t)->currentChar);
      ret.image[indxToImage++] = (*t)->currentChar;
      ret.endLine = (*t)->currentLineNumber;
      ret.endColumn = (*t)->currentColumnNumber;
      getNextChar(t);
    } while (isalnum((*t)->currentChar) || (*t)->currentChar == '_');
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
    else if ((strncmp(ret.image, "pr", 512)) == 0) {
      ret.kind = PUSHR;
    }
    else if ((strncmp(ret.image, "cora", 512)) == 0) {
      ret.kind = CORA;
    }
    else if ((strncmp(ret.image, "asp", 512)) == 0) {
      ret.kind = ASP;
    }
    else if ((strncmp(ret.image, "call", 512)) == 0) {
      ret.kind = CALL;
    }
    else if ((strncmp(ret.image, "ja", 512)) == 0) {
      ret.kind = JA;
    }
    else if ((strncmp(ret.image, "jct", 512)) == 0) {
      ret.kind = JCT;
    }
    else if ((strncmp(ret.image, "jp", 512)) == 0) {
      ret.kind = JP;
    }
    else if ((strncmp(ret.image, "jn", 512)) == 0) {
      ret.kind = JN;
    }
    else if ((strncmp(ret.image, "jz", 512)) == 0) {
      ret.kind = JZ;
    }
    else if ((strncmp(ret.image, "jnz", 512)) == 0) {
      ret.kind = JNZ;
    }
    else if ((strncmp(ret.image, "jodd", 512)) == 0) {
      ret.kind = JODD;
    }
    else if ((strncmp(ret.image, "jzon", 512)) == 0) {
      ret.kind = JZON;
    }
    else if ((strncmp(ret.image, "jzop", 512)) == 0) {
      ret.kind = JZOP;
    }
    else if ((strncmp(ret.image, "ret", 512)) == 0) {
      ret.kind = RET;
    }
    else if ((strncmp(ret.image, "add", 512)) == 0) {
      ret.kind = ADD;
    }
    else if ((strncmp(ret.image, "sub", 512)) == 0) {
      ret.kind = SUB;
    }
    else if ((strncmp(ret.image, "stav", 512)) == 0) {
      ret.kind = STAV;
    }
    else if ((strncmp(ret.image, "stva", 512)) == 0) {
      ret.kind = STVA;
    }
    else if ((strncmp(ret.image, "load", 512)) == 0) {
      ret.kind = LOAD;
    }
    else if ((strncmp(ret.image, "awc", 512)) == 0) {
      ret.kind = AWC;
    }
    else if ((strncmp(ret.image, "dupe", 512)) == 0) {
      ret.kind = DUPE;
    }
    else if ((strncmp(ret.image, "esba", 512)) == 0) {
      ret.kind = ESBA;
    }
    else if ((strncmp(ret.image, "reba", 512)) == 0) {
      ret.kind = REBA;
    }
    else if ((strncmp(ret.image, "zsp", 512)) == 0) {
      ret.kind = ZSP;
    }
    else if ((strncmp(ret.image, "cmps", 512)) == 0) {
      ret.kind = CMPS;
    }
    else if ((strncmp(ret.image, "cmpu", 512)) == 0) {
      ret.kind = CMPU;
    }
    else if ((strncmp(ret.image, "rev", 512)) == 0) {
      ret.kind = REV;
    }
    else if ((strncmp(ret.image, "shll", 512)) == 0) {
      ret.kind = SHLL;
    }
    else if ((strncmp(ret.image, "shrl", 512)) == 0) {
      ret.kind = SHRL;
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
  printf("line: %0d kind: %0d image: %s type: %s\n", ret.beginLine, ret.kind, ret.image, tokenImage[ret.kind]);
  return ret;
}

void TokenManagerDelete(TokenManager** t) {
//  printf("deleting t->infile: %0d\n", t->inFile);
  fclose((*t)->inFile);
  free(*t);
  *t = NULL;
}

