P=oscchief
OBJECTS=
CC=gcc
CFLAGS=`pkg-config --static --cflags liblo` -Wextra -Wall -Wno-missing-field-initializers -Wno-unused-parameter -std=gnu99 -ggdb -Wno-unused-function -Bstatic
#LDLIBS=`pkg-config --static --libs liblo`
LDLIBS=-L/usr/local/lib /usr/local/lib/liblo.a -lpthread

prefix=/usr/local


all: $(P)

$(P): main.c
	$(CC) $(CFLAGS) $(LDLIBS) $(LDFLAGS) -o $@ $^

clean:
	rm -rf $(P) *.dSYM

install: $(P)
	install -d -m 0755 $(prefix)/bin
	install -m 0755 $(P) $(prefix)/bin

uinstall: