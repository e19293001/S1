#include ./makefrag
#iveriloginclude=$(ICARUSVPI)
#coptions=-I$(iveriloginclude)

main=vpiTest
obj=obj
COPTS=-g -Wall

#all: compile runsim
all: mkdirs testParser runTest

mkdirs:
	mkdir -p $(obj) bin

$(main).vvp: $(main).v
	iverilog $^ -o $@

compile: cxternals $(main).vvp

runsim:
	vvp -M. -ms1Assembler $(main).vvp

clean:
	rm -fr *.vpi *.vvp *.o $(obj) bin

cxternals: s1Assembler.vpi

s1Assembler.vpi: s1Assembler.c
	iverilog-vpi $^

val:
#	valgrind -v vvp -M. -ms1Assembler $(main).vvp
#	valgrind --leak-check=full --show-reachable=yes -v vvp -M. -ms1Assembler $(main).vvp 
	valgrind --leak-check=full --show-reachable=yes ./bin/testParser

testParser: $(obj)/testParser.o $(obj)/parser.o $(obj)/TernarySearchTrie.o $(obj)/tokenmanager.o
	gcc $^ -o bin/testParser $(COPTS)

$(obj)/testParser.o: testParser.c
	gcc -c $< -o $@ $(COPTS)

$(obj)/parser.o: parser.c parser.h
	gcc -c $< -o $@ $(COPTS)

$(obj)/TernarySearchTrie.o: TernarySearchTrie.c TernarySearchTrie.h
	gcc -c $< -o $@ $(COPTS)

$(obj)/tokenmanager.o: tokenmanager.c tokenmanager.h
	gcc -c $< -o $@ $(COPTS)

runTest:
	./bin/testParser
