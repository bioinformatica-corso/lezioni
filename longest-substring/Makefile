#
# GPLv2
P = substring
CFLAGS_STD = -g -Wall -O0 -march=native -Wno-deprecated -Wno-parentheses -Wno-format
STD_LIBS = zlib bdw-gc

LIBS 	= cmdline.o sais-lite-2.4.1/sais.o substring.o rmq.o gsa.o
CFLAGS_EXTRA =  -m64 -std=c11 -Wall -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes
CFLAGS_LIBS = `pkg-config --cflags $(STD_LIBS)`
CFLAGS_TEST =  -DTEST_EVERYTHING  `pkg-config --cflags $(DEBUG_LIBS)`
LDLIBS = `pkg-config --libs $(STD_LIBS)` -lm
CFLAGS = $(CFLAGS_STD) $(CFLAGS_EXTRA) $(CFLAGS_LIB)
debug: CFLAGS += -DDEBUG

CC_FULL = $(CC) $(CFLAGS) $(CFLAGS_LIBS)
bin: $(P) $(LIBS)

debug: $(P)

OBJECTS: substring.o rmq.o gsa.o

$(P): $(LIBS)
	$(CC_FULL) -o $@ $^ $(LDLIBS)

# all: $(P) doc check
# 	echo $(OBJECTS)


sais-lite-2.4.1/sais.o: sais-lite-2.4.1/sais.c sais-lite-2.4.1/sais.h
	cd sais-lite-2.4.1 && make

clean:
	@echo "Cleaning..."
	rm -rf *.o *.d $(P) cmdline.* tests/output/*

test: run-tests.sh $(P)
	./run-tests.sh

.PHONY: clean

ifneq "$(MAKECMDGOALS)" "clean"
-include ${SOURCES:.c=.d}
-include ${T_SOURCES:.c=.d}
endif

cmdline.c cmdline.h: $(P).ggo
	gengetopt -i $< --output-dir=./
