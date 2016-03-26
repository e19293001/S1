# IVERILOG_LIB_PATH_ and IVERILOG_INCLUDE_PATH_ must be set

IVERILOG_LIB_PATH=$(IVERILOG_LIB_PATH_)
IVERILOG_INCLUDE_PATH=$(IVERILOG_INCLUDE_PATH_)

PARSEROBJS=$(obj)/testParser.o $(obj)/parser.o $(obj)/TernarySearchTrie.o $(obj)/tokenmanager.o $(obj)/codegenerator.o $(obj)/memlist.o

main=vpiTest
obj=obj
COPTS=-g -Wall -I$(IVERILOG_INCLUDE_PATH) -L$(IVERILOG_LIB_PATH)

#all: mkdirs compile runsim
all: mkdirs testParser runTest

test: mkdirs testParser runTest

mkdirs:
	mkdir -p $(obj) bin

$(obj)/$(main).vvp: $(main).v
	iverilog $^ -o $@

compile: bin/s1Assembler.vpi $(obj)/$(main).vvp

bin/s1Assembler.vpi: $(PARSEROBJS) $(obj)/s1Assembler.o 
	gcc -shared -o $@ $^ $(COPTS)

$(obj)/s1Assembler.o: s1Assembler.c 
	gcc -c -fpic $^ -o $@ $(COPTS)

runsim:
	vvp -M. -mbin/s1Assembler $(obj)/$(main).vvp

clean:
	rm -fr *.vpi *.vvp *.o $(obj) bin tst/*.asm

#cxternals: s1Assembler.vpi

#s1Assembler.vpi: s1Assembler.c
#	iverilog-vpi $^

val: all
#	valgrind -v vvp -M. -mbin/s1Assembler $(obj)/$(main).vvp
#	valgrind --leak-check=full --show-reachable=yes -v vvp -M. -ms1Assembler $(main).vvp 
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes -v ./bin/testParser

testParser: $(PARSEROBJS)
	gcc $^ -o bin/testParser $(COPTS) -lvpi 

$(obj)/testParser.o: testParser.c
	gcc -c -fpic $< -o $@ $(COPTS)

$(obj)/codegenerator.o: codegenerator.c codegenerator.h
	gcc -c -fpic $< -o $@ $(COPTS)

$(obj)/memlist.o: memlist.c memlist.h
	gcc -c -fpic $< -o $@ $(COPTS)

$(obj)/parser.o: parser.c parser.h
	gcc -c -fpic $< -o $@ $(COPTS)

$(obj)/TernarySearchTrie.o: TernarySearchTrie.c TernarySearchTrie.h
	gcc -c -fpic $< -o $@ $(COPTS)

$(obj)/tokenmanager.o: tokenmanager.c tokenmanager.h
	gcc -c -fpic $< -o $@ $(COPTS)

runTest:
	./bin/testParser

gdb:
	gdb ./bin/testParser
