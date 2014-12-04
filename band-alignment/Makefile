#
# GPLv2
P = band
CFLAGS_STD = -g -Wall -O0 -march=native -Wno-deprecated -Wno-parentheses -Wno-format
STD_LIBS = zlib bdw-gc

OBJS 	= cmdline.o $(P).o
CFLAGS_EXTRA =  -m64 -std=c11 -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes
CFLAGS_LIBS = `pkg-config --cflags $(STD_LIBS)`
CFLAGS_TEST =  -DTEST_EVERYTHING
LDLIBS = `pkg-config --libs $(STD_LIBS)` -lm
CFLAGS = $(CFLAGS_STD) $(CFLAGS_EXTRA)
debug: CFLAGS += -DDEBUG

CC_FULL = $(CC) $(CFLAGS) $(CFLAGS_LIBS)
bin: $(P)

debug: $(P)

$(P): $(OBJS)
	$(CC_FULL) -o $@ $^ $(LDLIBS)

# all: $(P) doc check
# 	echo $(OBJECTS)

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
