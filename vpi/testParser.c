#include <stdio.h>
#include "parser.h"

void TestParser0000() {
  parserData *parser;
  char data[] = "tst/test_parser_pattern0000.txt";
  parser = ParserNew(data);
  ParserStart(parser);
  ParserDelete(parser);
}

void TestParser0001() {
  parserData *parser;
  char data[] = "tst/test_parser_pattern0001.txt";
  parser = ParserNew(data);
  ParserStart(parser);
  ParserDelete(parser);
}

void TestParser0002() { // test for duplicate labels
  parserData *parser;
  char data[] = "tst/test_parser_pattern0002.txt";
  parser = ParserNew(data);
  ParserStart(parser);
  ParserDelete(parser);
}

void TestParser0003() { // test for labels with underscore character
  parserData *parser;
  char data[] = "tst/test_parser_pattern0003.txt";
  parser = ParserNew(data);
  ParserStart(parser);
  ParserDelete(parser);
}

void TestParser0004() { // test for code generator
  parserData *parser;
  char data[] = "tst/test_parser_pattern0004.txt";
  parser = ParserNew(data);
  ParserStart(parser);
  ParserDelete(parser);
}

int main() {
//  TestParser0000();
//  TestParser0001();
//
//  TestParser0002(); // test for duplicate labels
//  TestParser0003(); // test for labels with underscore character
  TestParser0004(); // test for code generator
  return 0;
}
