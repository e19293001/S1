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


int main() {
  //TestParser0000();
  TestParser0001();
  return 0;
}
