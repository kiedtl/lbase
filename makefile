WARNINGS = -Wall -Wextra -pedantic -Wmissing-prototypes \
	   -Wold-style-definition -Werror
INC      = -Isub/ccommon/

CC       = gcc
CFLAGS   = -std=c99 -O3 $(WARNING) $(INC) -ggdb
LDFLAGS  =

DESTDIR  =
PREFIX   = /usr/local/

all: lbase
lbase: cat head

clean:
	rm -f cat head *.o

cat: cat.o
	$(CC) $(CFLAGS)   -o $@ $^

head: head.o argoat.o
	$(CC) $(CFLAGS)   -o $@ $^

.PHONY: all lbase clean
