CC = g++
CFLAGS = -Wall -Werror -ansi -pedantic -I ../../libutility/include -I ../../libfeature/include -I ../include

MACHINE = $(shell uname -m)
LDFLAGS = -lfeature -lsegtree -lutility
LDPATH = -L ../../libfeature/lib/${MACHINE} \
				 -L ../../libutility/lib/${MACHINE} \
				 -L ../lib/${MACHINE}

.PHONY: all clean


all: CFLAGS +=  -O2
debug: CFLAGS += -g

all: example
debug: example

example: example.cpp
	${CC} ${CFLAGS} $(LDPATH) $< ${LDFLAGS} -o $@

clean:
	rm -f example
