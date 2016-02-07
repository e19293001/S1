include ./makefrag
iveriloginclude=/usr/include/iverilog/
coptions=-I$(iveriloginclude)

main=s1VpiTestTop

all: compile runsim

$(main).vvp: $(main).v
	iverilog $^ -o $@

compile: cxternals $(main).vvp

runsim:
	vvp -M. -ms1Assembler $(main).vvp

clean:
	rm -fr *.vpi *.vvp *.o

