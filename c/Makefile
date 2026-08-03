# PhotonicSort — highly optimized C
# Give everything. Take nothing. Become photonic.

CC      ?= gcc
CFLAGS  ?= -O3 -std=c11 -Wall -Wextra -Wpedantic -fno-strict-aliasing
CFLAGS  += -I.
# Uncomment for local machine peak (portable releases should omit):
# CFLAGS += -march=native -flto
LDFLAGS ?=
LDLIBS  ?=

.PHONY: all clean test demo release

all: demo test_bin

photonic_sort.o: photonic_sort.c photonic_sort.h
	$(CC) $(CFLAGS) -c photonic_sort.c -o photonic_sort.o

demo: photonic_sort.o examples/demo.c
	$(CC) $(CFLAGS) examples/demo.c photonic_sort.o -o demo $(LDFLAGS) $(LDLIBS)

test_bin: photonic_sort.o tests/test_photonic_sort.c
	$(CC) $(CFLAGS) tests/test_photonic_sort.c photonic_sort.o -o test_photonic_sort $(LDFLAGS) $(LDLIBS)

test: test_bin
	./test_photonic_sort

demo-run: demo
	./demo

release: CFLAGS += -DNDEBUG -O3
release: clean all test

clean:
	rm -f photonic_sort.o demo test_photonic_sort
