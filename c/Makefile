# PhotonicSort C11 — adaptive hybrid sort
# Profiles: default, release (portable), native (local), STATIC=1 (fully static)

CC       ?= gcc
AR       ?= ar
STD      := -std=c11
WARN     := -Wall -Wextra -Wpedantic
OPT_BASE := -O3 -fno-strict-aliasing -fomit-frame-pointer -pipe
OPT_SEC  := -ffunction-sections -fdata-sections
LDFLAGS_GC := -Wl,--gc-sections

CFLAGS   ?= $(STD) $(WARN) $(OPT_BASE) $(OPT_SEC)
CFLAGS   += -I.
LDFLAGS  ?= $(LDFLAGS_GC)
LDLIBS   ?=

ifeq ($(LTO),1)
  CFLAGS  += -flto
  LDFLAGS += -flto
endif

ifeq ($(NATIVE),1)
  CFLAGS += -march=native -mtune=native
endif

# Fully static link (glibc or musl). Required for scratch/distroless-static images.
ifeq ($(STATIC),1)
  LDFLAGS += -static
endif

.PHONY: all clean test demo release native static lib install help

all: lib demo test_bin

photonic_sort.o: photonic_sort.c photonic_sort.h
	$(CC) $(CFLAGS) -c photonic_sort.c -o photonic_sort.o

libphotonic_sort.a: photonic_sort.o
	$(AR) rcs $@ photonic_sort.o

lib: libphotonic_sort.a

demo: photonic_sort.o examples/demo.c
	$(CC) $(CFLAGS) examples/demo.c photonic_sort.o -o demo $(LDFLAGS) $(LDLIBS)

test_bin: photonic_sort.o tests/test_photonic_sort.c
	$(CC) $(CFLAGS) tests/test_photonic_sort.c photonic_sort.o -o test_photonic_sort $(LDFLAGS) $(LDLIBS)

test: test_bin
	./test_photonic_sort

demo-run: demo
	./demo

release:
	$(MAKE) clean
	$(MAKE) all test CFLAGS="$(STD) $(WARN) $(OPT_BASE) $(OPT_SEC) -DNDEBUG -flto -I." LDFLAGS="$(LDFLAGS_GC) -flto"

# Portable fully-static release (for scratch / distroless-static)
static:
	$(MAKE) clean
	$(MAKE) all test CFLAGS="$(STD) $(WARN) $(OPT_BASE) $(OPT_SEC) -DNDEBUG -I." LDFLAGS="$(LDFLAGS_GC) -static" STATIC=1

native:
	$(MAKE) clean
	$(MAKE) all test NATIVE=1 LTO=1

PREFIX  ?= /usr/local
INCLUDEDIR ?= $(PREFIX)/include
LIBDIR     ?= $(PREFIX)/lib

install: libphotonic_sort.a photonic_sort.h
	install -d $(DESTDIR)$(INCLUDEDIR) $(DESTDIR)$(LIBDIR)
	install -m 644 photonic_sort.h $(DESTDIR)$(INCLUDEDIR)/
	install -m 644 libphotonic_sort.a $(DESTDIR)$(LIBDIR)/

clean:
	rm -f photonic_sort.o demo test_photonic_sort libphotonic_sort.a

help:
	@echo "Targets: all lib demo test release static native install clean"
	@echo "Options: LTO=1  NATIVE=1  STATIC=1  CC=clang  PREFIX=/usr/local"
