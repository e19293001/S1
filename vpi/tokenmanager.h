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
  Token *tok;
} TokenManager;

TokenManager* TokenManagerNew(char *inFileName);
void getNextChar();
Token TokenManagerGetNextToken(TokenManager **t);
void TokenManagerDelete(TokenManager* t);

#endif
