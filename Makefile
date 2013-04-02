P=oscchief
OBJECTS=
CC=gcc
CFLAGS=-Wextra -Wall -Wno-missing-field-initializers -Wno-unused-parameter -std=gnu99 -ggdb -Wno-unused-function $(EXTRA_CFLAGS)
LDLIBS=

prefix=/usr/local


all: oscchief

$(P): main.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

clean:
	rm -rf $(P) *.dSYM

install:

uinstall: