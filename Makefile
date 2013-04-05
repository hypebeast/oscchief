P=oscchief
OBJECTS=
CC=gcc
CFLAGS=`pkg-config --static --cflags liblo` -Wextra -Wall -Wno-missing-field-initializers -Wno-unused-parameter -std=gnu99 -ggdb -Wno-unused-function
LDLIBS=`pkg-config --static --libs liblo`

prefix=/usr/local


all: $(P)

$(P): main.c
	$(CC) $(CFLAGS) $(LDLIBS) $(LDFLAGS) -o $@ $^

clean:
	rm -rf $(P) *.dSYM

install:

uinstall: