#include <stdio.h>
#include "parser.h"

void success() {
  printf("##############################\n");
  printf("########    passed   #########\n");
  printf("##############################\n");
}

void result(int x) {
  if (x) {
    printf("passed.\n");
  }
  else {
    printf("##############################\n");
    printf("########   failed    #########\n");
    printf("##############################\n");
    exit(-1);
  }
}

void TestParser0000() {
  parserData *parser;
  char data[] = "tst/test_parser_pattern0000.txt";
  parser = ParserNew(data);
  result(ParserStart(parser) == 0);
  ParserDelete(&parser);
}

void TestParser0001() {
  parserData *parser;
  char data[] = "tst/test_parser_pattern0001.txt";
  parser = ParserNew(data);
  result(ParserStart(parser) == 0);
  ParserDelete(&parser);
}

void TestParser0002() { // test for duplicate labels
  parserData *parser;
  char data[] = "tst/test_parser_pattern0002.txt";
  parser = ParserNew(data);
  result(ParserStart(parser) == 1);
  ParserDelete(&parser);
}

void TestParser0003() { // test for labels with underscore character
  parserData *parser;
  char data[] = "tst/test_parser_pattern0003.txt";
  parser = ParserNew(data);
  result(ParserStart(parser) == 0);
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
  result(ParserStart(parser) == 0);
  ParserDelete(&parser);
}

void TestParser0005() { // test for code generator to test for labels pointing to halt
  parserData *parser;
  char data[] = "tst/test_parser_pattern0005.txt";
  parser = ParserNew(data);
  result(ParserStart(parser) == 0);
  ParserDelete(&parser);
}

void TestParser0006() { // test for code generator to test for labels pointing to pushc
  parserData *parser;
  char data[] = "tst/test_parser_pattern0006.txt";
  parser = ParserNew(data);
  result(ParserStart(parser) == 0);
  ParserDelete(&parser);
}

void TestParser0007() { // test for code generator for pushwc
  parserData *parser;
  char data[] = "tst/test_parser_pattern0007.txt";
  parser = ParserNew(data);
  result(ParserStart(parser) == 0);
  ParserDelete(&parser);
}

void TestParser0008() { // test for comments
  parserData *parser;
  char data[] = "tst/test_parser_pattern0008.txt";
  parser = ParserNew(data);
  result(ParserStart(parser) == 0);
  ParserDelete(&parser);
}

void TestParser0009() { // test for push relative
  parserData *parser;
  char data[] = "tst/test_parser_pattern0009.txt";
  parser = ParserNew(data);
  result(ParserStart(parser) == 0);
  ParserDelete(&parser);
}

void TestParser0010() { // test for convert relative address
  parserData *parser;
  char data[] = "tst/test_parser_pattern0010.txt";
  parser = ParserNew(data);
  result(ParserStart(parser) == 0);
  ParserDelete(&parser);
}

void TestParser0011() { // test for add stack pointer
  parserData *parser;
  char data[] = "tst/test_parser_pattern0011.txt";
  parser = ParserNew(data);
  result(ParserStart(parser) == 0);
  ParserDelete(&parser);
}

void TestParser0012() { // test for call
  parserData *parser;
  char data[] = "tst/test_parser_pattern0012.txt";
  parser = ParserNew(data);
  result(ParserStart(parser) == 0);
  ParserDelete(&parser);
}

void TestParser0013() { // test for ja
  parserData *parser;
  char data[] = "tst/test_parser_pattern0013.txt";
  parser = ParserNew(data);
  result(ParserStart(parser) == 0);
  ParserDelete(&parser);
}

void TestParser0014() { // test for jct
  parserData *parser;
  char data[] = "tst/test_parser_pattern0014.txt";
  parser = ParserNew(data);
  result(ParserStart(parser) == 0);
  ParserDelete(&parser);
}

void TestParser0015() { // test for jp
  parserData *parser;
  char data[] = "tst/test_parser_pattern0015.txt";
  parser = ParserNew(data);
  result(ParserStart(parser) == 0);
  ParserDelete(&parser);
}

void TestParser0016() { // test for jn
  parserData *parser;
  char data[] = "tst/test_parser_pattern0016.txt";
  parser = ParserNew(data);
  result(ParserStart(parser) == 0);
  ParserDelete(&parser);
}

void TestParser0017() { // test for jz
  parserData *parser;
  char data[] = "tst/test_parser_pattern0017.txt";
  parser = ParserNew(data);
  result(ParserStart(parser) == 0);
  ParserDelete(&parser);
}

void TestParser0018() { // test for jnz
  parserData *parser;
  char data[] = "tst/test_parser_pattern0018.txt";
  parser = ParserNew(data);
  result(ParserStart(parser) == 0);
  ParserDelete(&parser);
}

void TestParser0019() { // test for jodd
  parserData *parser;
  char data[] = "tst/test_parser_pattern0019.txt";
  parser = ParserNew(data);
  result(ParserStart(parser) == 0);
  ParserDelete(&parser);
}

void TestParser0020() { // test for jzon
  parserData *parser;
  char data[] = "tst/test_parser_pattern0020.txt";
  parser = ParserNew(data);
  result(ParserStart(parser) == 0);
  ParserDelete(&parser);
}

void TestParser0021() { // test for jzop
  parserData *parser;
  char data[] = "tst/test_parser_pattern0021.txt";
  parser = ParserNew(data);
  result(ParserStart(parser) == 0);
  ParserDelete(&parser);
}

void TestParser0022() { // test for ret
  parserData *parser;
  char data[] = "tst/test_parser_pattern0022.txt";
  parser = ParserNew(data);
  result(ParserStart(parser) == 0);
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
  TestParser0010(); // test for convert relative address
  TestParser0011(); // test for add stack pointer
  TestParser0012(); // test for call
  TestParser0013(); // test for ja
  TestParser0014(); // test for jct
  TestParser0015(); // test for jp
  TestParser0016(); // test for jn
  TestParser0017(); // test for jz
  TestParser0018(); // test for jnz
  TestParser0019(); // test for jodd
  TestParser0020(); // test for jzon
  TestParser0021(); // test for jzop
  TestParser0022(); // test for ret

  success();

  return 0;
}
