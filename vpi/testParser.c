#include <stdio.h>
#include "parser.h"

void TestParser0000() {
  parserData *parser;
  char data[] = "tst/test_parser_pattern0000.txt";
  parser = ParserNew(data);
  assert(ParserStart(parser) == 0);
  ParserDelete(&parser);
}

void TestParser0001() {
  parserData *parser;
  char data[] = "tst/test_parser_pattern0001.txt";
  parser = ParserNew(data);
  assert(ParserStart(parser) == 0);
  ParserDelete(&parser);
}

void TestParser0002() { // test for duplicate labels
  parserData *parser;
  char data[] = "tst/test_parser_pattern0002.txt";
  parser = ParserNew(data);
  assert(ParserStart(parser) == 1);
  ParserDelete(&parser);
}

void TestParser0003() { // test for labels with underscore character
  parserData *parser;
  char data[] = "tst/test_parser_pattern0003.txt";
  parser = ParserNew(data);
  assert(ParserStart(parser) == 0);
  ParserDelete(&parser);
}

/*
 +0000  0001      p a1
                  a1:
 +0001  0004      p x
 +0002  0003      p @s
                  @s:
 +0003  FFFF      halt
 +0004  0062      x: dw 98
 +0005  0064      s: dw 100
 +0006  0005      p s
*/
void TestParser0004() { // test for code generator to test for labels pointing to dwords
  parserData *parser;
  char data[] = "tst/test_parser_pattern0004.txt";

  parser = ParserNew(data);
  assert(ParserStart(parser) == 0);
  ParserDelete(&parser);
}

void TestParser0005() { // test for code generator to test for labels pointing to halt
  parserData *parser;
  char data[] = "tst/test_parser_pattern0005.txt";
  parser = ParserNew(data);
  assert(ParserStart(parser) == 0);
  ParserDelete(&parser);
}

void TestParser0006() { // test for code generator to test for labels pointing to pushc
  parserData *parser;
  char data[] = "tst/test_parser_pattern0006.txt";
  parser = ParserNew(data);
  assert(ParserStart(parser) == 0);
  ParserDelete(&parser);
}

void TestParser0007() { // test for code generator for pushwc
  parserData *parser;
  char data[] = "tst/test_parser_pattern0007.txt";
  parser = ParserNew(data);
  assert(ParserStart(parser) == 0);
  ParserDelete(&parser);
}

void TestParser0008() { // test for comments
  parserData *parser;
  char data[] = "tst/test_parser_pattern0008.txt";
  parser = ParserNew(data);
  assert(ParserStart(parser) == 0);
  ParserDelete(&parser);
}

void TestParser0009() { // test for push relative
  parserData *parser;
  char data[] = "tst/test_parser_pattern0009.txt";
  parser = ParserNew(data);
  assert(ParserStart(parser) == 0);
  ParserDelete(&parser);
}

int main() {
  TestParser0000();
  TestParser0001();
  
  TestParser0002(); // test for duplicate labels
  TestParser0003(); // test for labels with underscore character
  
  TestParser0004(); // test for code generator dword
  TestParser0005(); // test for code generator halt
  TestParser0006(); // test for code generator pushc
  TestParser0007(); // test for code generator pushwc
  TestParser0008(); // test for comments
  TestParser0009(); // test for push relative
  return 0;
}
