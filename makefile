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
TEST=$(TSTDIR)/TestS1sub.v

#TEST=$(TSTDIR)/TestS1pc.v
#UTILS=testFile.v
#TEST=$(TSTDIR)/TestS1PushConstant.v
OUT=S1

DIRS=-I./ -I$(TSTDIR)/

all: comp run

comp: $(OUT) $(TEST)
	iverilog -ffiles $(TEST) $(DIRS) -o $(OUT)

$(OUT): $(FILES)

run:
	vvp $(OUT)

clean:
	rm -fr $(OUT)

tex:
	pdflatex S1Push.tex

wave:
	gtkwave sim.vcd &
