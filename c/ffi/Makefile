#
# GPLv2
OBJECTS: substring substring.o substrings.so substringcython.c
bin: $(P) $(OBJECTS)
P = substring
CFLAGS_STD = -g -Wall -O3 -march=native -Wno-deprecated -Wno-parentheses -Wno-format

CFLAGS_EXTRA =  -std=c11 -Wall -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes
CFLAGS = $(CFLAGS_STD) $(CFLAGS_EXTRA) $(CFLAGS_LIB)



substrings.so: substring.c substring.h
	gcc -shared -fPIC -o substrings.so -std=c11 substring.c

clean:
	@echo "Cleaning..."
	rm -rf *.o *.so *.d $(P) tests/output/*

test: run-tests.sh $(P)
	./run-tests.sh

substringcython.c: substringcython.pyx setup.py
	python setup.py build_ext --inplace

.PHONY: clean

ifneq "$(MAKECMDGOALS)" "clean"
-include ${SOURCES:.c=.d}
-include ${T_SOURCES:.c=.d}
endif
