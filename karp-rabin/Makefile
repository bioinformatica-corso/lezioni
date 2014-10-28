# Makefile for Constrained Persistent Perfect Phylogeny
P = karp-rabin

CFLAGS_STD = -g -Wall -DDEBUG -O3 -march=native -Wno-deprecated -Wno-parentheses -Wno-format
STD_LIBS = zlib gsl

LIBS 	= cmdline.o
CFLAGS_EXTRA =  -m32 -std=c11 -Wall -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes
CFLAGS_LIBS = `pkg-config --cflags $(STD_LIBS)`
CFLAGS_TEST =  -DTEST_EVERYTHING  `pkg-config --cflags $(DEBUG_LIBS)`
LDLIBS = `pkg-config --libs $(STD_LIBS)`
CFLAGS = $(CFLAGS_STD) $(CFLAGS_EXTRA) $(CFLAGS_LIB)

CC_FULL = $(CC) $(CFLAGS) -I$(SRC_DIR) -I$(LIB_DIR) $(CFLAGS_LIBS)

bin: $(P) $(LIBS)

$(P): $(P).c $(P).h $(LIBS)
# $(P): $(OBJECTS)
# 	@echo 'Linking $@'
# 	@mkdir -p $(BIN_DIR)
# 	$(CC_FULL) -o $@ $^ $(LDLIBS)

# all: $(P) doc check
# 	echo $(OBJECTS)

# ${OBJ_DIR}/%.o: $(SRC_DIR)/%.c $(LIBS)
# 	@echo '* Compiling $<'
# 	@mkdir -p $(dir $@)
# 	$(CC_FULL) -MM -MF $(patsubst %.c,%.d,$<)  -MT $@ $<
# 	$(CC_FULL) -o $@ -c $<

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
