#include ./makefrag
#iveriloginclude=$(ICARUSVPI)
#coptions=-I$(iveriloginclude)

main=vpiTest

all: compile runsim

$(main).vvp: $(main).v
	iverilog $^ -o $@

compile: cxternals $(main).vvp

runsim:
	vvp -M. -ms1Assembler $(main).vvp

clean:
	rm -fr *.vpi *.vvp *.o

cxternals: s1Assembler.vpi

s1Assembler.vpi: s1Assembler.c
	iverilog-vpi $^

val:
#	valgrind -v vvp -M. -ms1Assembler $(main).vvp 
	valgrind --leak-check=full --show-reachable=yes -v vvp -M. -ms1Assembler $(main).vvp 
