#include <stdio.h>
#include "parser.h"

int main() {
  parserData *parser;
  parser = ParserNew("tst/test_parser_pattern0000.txt");
  ParserStart(parser);
  ParserDelete(parser);
  return 0;
}
