P = dc

CFLAGS_STD = -g -Wall -DDEBUG -O0 -march=native -Wno-deprecated -Wno-parentheses -Wno-format
STD_LIBS = #zlib gsl

LIBS 	= #cmdline.o
CFLAGS_EXTRA =  -std=c11 -Wall -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes
CFLAGS_LIBS = #`pkg-config --cflags $(STD_LIBS)`
LDLIBS = #`pkg-config --libs $(STD_LIBS)`
CFLAGS = $(CFLAGS_STD) $(CFLAGS_EXTRA) $(CFLAGS_LIB)

CC_FULL = $(CC) $(CFLAGS) -I$(SRC_DIR) -I$(LIB_DIR) $(CFLAGS_LIBS)

bin: $(P) $(LIBS)

$(P): $(P).c $(LIBS)

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
