#ifndef tokenmanager_h
#define tokenmanager_h

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>


typedef enum { 
  _EOF,
  PUSH,
  PUSHC,
  PUSHWC,
  HALT,
  ID,
  UNSIGNED,
  OPERATOR,
  DWORD,
  SEMICOL,
  COLON,
  PUSHR,
  CORA,
  ASP,
  CALL,
  JA,
  JCT,
  JP,
  JN,
  JZ,
  JNZ,
  JODD,
  JZON,
  JZOP,
  RET,
  ADD,
  SUB,
  STAV,
  STVA,
  LOAD,
  AWC,
  DUPE,
  ESBA,
  REBA,
  ZSP,
  CMPS,
  CMPU,
  REV,
  SHLL,
  SHRL,
  SHRA,
  NEG,
  MULT,
  DIV,
  REM,
  ADDY,
  OR,
  XOR,
  FLIP,
  AND,
  CALI,
  SCT,
  ROT,
  PSP,
  BPBP,
  POBP,
  PBP,
  BCPY,
  UOUT,
  SIN,
  SOUT,
  HIN,
  HOUT,
  AIN,
  AOUT,
  DIN,
  DOUT,
  NOOP,
  ERROR
} tokenConst; // update this with tokenImage in tokenmanager.c
 
extern const char *tokenImage[];

typedef struct Token {
  int kind;
  int beginLine;
  int beginColumn;
  int endLine;
  int endColumn;
  char image[512];

  struct Token *next;
} Token;

Token TokenNew();

typedef struct TokenManager {
  FILE *inFile;
  char currentChar;
  int currentColumnNumber;
  int currentLineNumber;
  char inputLine[512];
  int printEnable;
  Token *tok;
} TokenManager;

void TokenDump(char* s, Token t);
TokenManager* TokenManagerNew(char *inFileName);
void getNextChar();
Token TokenManagerGetNextToken(TokenManager **t);
void TokenManagerDelete(TokenManager** t);
#endif
