TSTDIR=tst
#TEST=$(TSTDIR)/TestS1.v
#TEST=$(TSTDIR)/TestS1Push.v
#TEST=$(TSTDIR)/TestS1asp.v

#TEST=$(TSTDIR)/TestS1call.v
#TEST=$(TSTDIR)/TestS1ja.v
#TEST=$(TSTDIR)/TestS1jp.v
#TEST=$(TSTDIR)/TestS1jn.v
#TEST=$(TSTDIR)/TestS1jz.v
#TEST=$(TSTDIR)/TestS1jnz.v
#TEST=$(TSTDIR)/TestS1jodd.v
#TEST=$(TSTDIR)/TestS1jzon.v
#TEST=$(TSTDIR)/TestS1jzop.v

#TEST=$(TSTDIR)/TestS1add.v
#TEST=$(TSTDIR)/TestS1ld.v
#TEST=$(TSTDIR)/TestS1ret.v
#TEST=$(TSTDIR)/TestS1stav.v
#TEST=$(TSTDIR)/TestS1stva.v
#TEST=$(TSTDIR)/TestS1sub.v

#main=TestValuePlusArgs
main=TestAout

#TSTPATTERN='vpi/tst/test_parser_pattern0000.txt'
#TSTPATTERN='vpi/tst/test_parser_pattern0001.txt'
#TSTPATTERN='vpi/tst/test_parser_pattern0003.txt'
#TSTPATTERN='vpi/tst/test_parser_pattern0003.txt'
#TSTPATTERN='vpi/tst/test_parser_pattern0003.txt'

#TSTPATTERN='stm/ptn0000.txt'
#TSTPATTERN=stm/ptnTestAout0000.txt
#TSTPATTERN=stm/ptnTestDout0000.txt
#TSTPATTERN=stm/ptnTestSout0000.txt
#TSTPATTERN=stm/ptnTestAin0000.txt
#TSTPATTERN=stm/ptnTestUout0000.txt
#TSTPATTERN=stm/ptnTestHout0000.txt
#TSTPATTERN=stm/ptnTestPr0000.txt
#TSTPATTERN=stm/ptnTestCora0000.txt
#TSTPATTERN=stm/ptnTestAwc0000.txt
#TSTPATTERN=stm/ptnTestNeg0000.txt
#TSTPATTERN=stm/ptnTestOr0000.txt
#TSTPATTERN=stm/ptnTestXor0000.txt
#TSTPATTERN=stm/ptnTestAnd0000.txt
TSTPATTERN=stm/ptnTestFlip0000.txt


#TEST=$(TSTDIR)/TestS1pc.v
#UTILS=testFile.v
#TEST=$(TSTDIR)/TestS1PushConstant.v
OUT=S1

DIRS=-I./ -I$(TSTDIR)/
TEST=$(TSTDIR)/$(main).v

#all: comp run
all: comp-vpi runsim

comp: $(OUT) $(TEST)
	iverilog -ffiles $(TEST) $(DIRS) -o $(OUT)

$(OUT): $(FILES)

run:
	vvp $(OUT) +TSTPATTERN=$(TSTPATTERN)

clean:
	cd vpi && rm -fr *.vpi *.vvp *.o $(obj) bin tst/*.asm && cd ..
	rm -fr $(OUT)
	rm -fr doc/*.{aux,log}
	rm -fr stm/*.asm

tex:
	pdflatex S1Push.tex

wave:
	gtkwave sim.vcd &

# vpi
IVERILOG_LIB_PATH=$(IVERILOG_LIB_PATH_)
IVERILOG_INCLUDE_PATH=$(IVERILOG_INCLUDE_PATH_)

obj=obj
COPTS=-g -Wall -I$(IVERILOG_INCLUDE_PATH) -L$(IVERILOG_LIB_PATH)

PARSEROBJS=vpi/$(obj)/testParser.o vpi/$(obj)/parser.o vpi/$(obj)/TernarySearchTrie.o vpi/$(obj)/tokenmanager.o vpi/$(obj)/codegenerator.o vpi/$(obj)/memlist.o

mkdirs:
	mkdir -p vpi/$(obj) vpi/bin

comp-vpi: mkdirs vpi/$(obj)/$(main).vvp vpi/bin/s1Assembler.vpi vpi/bin/getInput.vpi 

runsim:
	vvp -M. -mvpi/bin/s1Assembler -mvpi/bin/getInput vpi/$(obj)/$(main).vvp +TSTPATTERN=$(TSTPATTERN) 

vpi/$(obj)/$(main).vvp: $(TEST) rtl/S1.v tst/testFile.v tst/MemoryModel.v
	iverilog -ffiles $(DIRS) $(TEST) -o $@ 

vpi/$(obj)/s1Assembler.o: vpi/s1Assembler.c 
	gcc -c -fpic $^ -o $@ $(COPTS)

vpi/bin/getInput.vpi: $(PARSEROBJS) vpi/$(obj)/getInput.o
	gcc -shared -o $@ $^ $(COPTS)

vpi/$(obj)/getInput.o: vpi/getInput.c
	gcc -c -fpic $< -o $@ $(COPTS)

vpi/bin/s1Assembler.vpi: $(PARSEROBJS) vpi/$(obj)/s1Assembler.o 
	gcc -shared -o $@ $^ $(COPTS)

vpi/$(obj)/testParser.o: vpi/testParser.c
	gcc -c -fpic $< -o $@ $(COPTS)

vpi/$(obj)/codegenerator.o: vpi/codegenerator.c vpi/codegenerator.h
	gcc -c -fpic $< -o $@ $(COPTS)

vpi/$(obj)/memlist.o: vpi/memlist.c vpi/memlist.h
	gcc -c -fpic $< -o $@ $(COPTS)

vpi/$(obj)/parser.o: vpi/parser.c vpi/parser.h
	gcc -c -fpic $< -o $@ $(COPTS)

vpi/$(obj)/TernarySearchTrie.o: vpi/TernarySearchTrie.c vpi/TernarySearchTrie.h
	gcc -c -fpic $< -o $@ $(COPTS)

vpi/$(obj)/tokenmanager.o: vpi/tokenmanager.c vpi/tokenmanager.h
	gcc -c -fpic $< -o $@ $(COPTS)


