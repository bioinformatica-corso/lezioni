# Makefile for suftest and test

# options
CC						= gcc
CFLAGS				= -std=c11 -m64 -O3 -fomit-frame-pointer -g
CPPFLAGS			= -Wall -DNDEBUG
LDFLAGS				=
LDLIBS				=
#TARGET_ARCH		=

# targets
.PHONY: all
all: suftest
suftest: sais.o suftest.o

distclean: clean
clean:
	$(RM) suftest *.o

# dependencies
sais.o suftest.o: sais.h Makefile
