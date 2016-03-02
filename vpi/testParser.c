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
  result(ParserStart(parser) == 1);
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

void TestParser0023() { // test for add
  parserData *parser;
  char data[] = "tst/test_parser_pattern0023.txt";
  parser = ParserNew(data);
  result(ParserStart(parser) == 0);
  ParserDelete(&parser);
}

void TestParser0024() { // test for sub
  parserData *parser;
  char data[] = "tst/test_parser_pattern0024.txt";
  parser = ParserNew(data);
  result(ParserStart(parser) == 0);
  ParserDelete(&parser);
}

void TestParser0025() { // test for stav
  parserData *parser;
  char data[] = "tst/test_parser_pattern0025.txt";
  parser = ParserNew(data);
  result(ParserStart(parser) == 0);
  ParserDelete(&parser);
}

void TestParser0026() { // test for stva
  parserData *parser;
  char data[] = "tst/test_parser_pattern0026.txt";
  parser = ParserNew(data);
  result(ParserStart(parser) == 0);
  ParserDelete(&parser);
}

void TestParser0027() { // test for load
  parserData *parser;
  char data[] = "tst/test_parser_pattern0027.txt";
  parser = ParserNew(data);
  result(ParserStart(parser) == 0);
  ParserDelete(&parser);
}

void TestParser0028() { // test for awc
  parserData *parser;
  char data[] = "tst/test_parser_pattern0028.txt";
  parser = ParserNew(data);
  result(ParserStart(parser) == 0);
  ParserDelete(&parser);
}

void TestParser0029() { // test for dupe
  parserData *parser;
  char data[] = "tst/test_parser_pattern0029.txt";
  parser = ParserNew(data);
  result(ParserStart(parser) == 0);
  ParserDelete(&parser);
}

void TestParser0030() { // test for esba
  parserData *parser;
  char data[] = "tst/test_parser_pattern0030.txt";
  parser = ParserNew(data);
  result(ParserStart(parser) == 0);
  ParserDelete(&parser);
}

void TestParser0031() { // test for reba
  parserData *parser;
  char data[] = "tst/test_parser_pattern0031.txt";
  parser = ParserNew(data);
  result(ParserStart(parser) == 0);
  ParserDelete(&parser);
}

void TestParser0032() { // test for zsp
  parserData *parser;
  char data[] = "tst/test_parser_pattern0032.txt";
  parser = ParserNew(data);
  result(ParserStart(parser) == 0);
  ParserDelete(&parser);
}

void TestParser0033() { // test for cmps
  parserData *parser;
  char data[] = "tst/test_parser_pattern0033.txt";
  parser = ParserNew(data);
  result(ParserStart(parser) == 0);
  ParserDelete(&parser);
}

void TestParser0034() { // test for cmpu
  parserData *parser;
  char data[] = "tst/test_parser_pattern0034.txt";
  parser = ParserNew(data);
  result(ParserStart(parser) == 0);
  ParserDelete(&parser);
}

void TestParser0035() { // test for rev
  parserData *parser;
  char data[] = "tst/test_parser_pattern0035.txt";
  parser = ParserNew(data);
  result(ParserStart(parser) == 0);
  ParserDelete(&parser);
}

void TestParser0036() { // test for shll
  parserData *parser;
  char data[] = "tst/test_parser_pattern0036.txt";
  parser = ParserNew(data);
  result(ParserStart(parser) == 0);
  ParserDelete(&parser);
}

void TestParser0037() { // test for shrl
  parserData *parser;
  char data[] = "tst/test_parser_pattern0037.txt";
  parser = ParserNew(data);
  result(ParserStart(parser) == 0);
  ParserDelete(&parser);
}

int main() {
//  TestParser0000();
//  TestParser0001();
//  TestParser0002(); // test for duplicate labels
//  TestParser0003(); // test for labels with underscore character
//  TestParser0004(); // test for code generator dword
//  TestParser0005(); // test for code generator halt
  TestParser0006(); // test for code generator pushc
//  TestParser0007(); // test for code generator pushwc
//  TestParser0008(); // test for comments
//  TestParser0009(); // test for push relative
//  TestParser0010(); // test for convert relative address
//  TestParser0011(); // test for add stack pointer
//  TestParser0012(); // test for call
//  TestParser0013(); // test for ja
//  TestParser0014(); // test for jct
//  TestParser0015(); // test for jp
//  TestParser0016(); // test for jn
//  TestParser0017(); // test for jz
//  TestParser0018(); // test for jnz
//  TestParser0019(); // test for jodd
//  TestParser0020(); // test for jzon
//  TestParser0021(); // test for jzop
//  TestParser0022(); // test for ret
//  TestParser0023(); // test for add
//  TestParser0024(); // test for sub
//  TestParser0025(); // test for stav
//  TestParser0026(); // test for stva
//  TestParser0027(); // test for load
//  TestParser0028(); // test for awc
//  TestParser0029(); // test for dupe
//  TestParser0030(); // test for esba
//  TestParser0031(); // test for reba
//  TestParser0032(); // test for zsp
//  TestParser0033(); // test for cmps
//  TestParser0034(); // test for cmpu
//  TestParser0035(); // test for rev
//  TestParser0036(); // test for shll
//  TestParser0037(); // test for shrl

  success();

  return 0;
}
