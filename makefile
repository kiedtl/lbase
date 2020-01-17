WARNINGS = -Wall -Wextra -pedantic -Wmissing-prototypes \
	   -Wold-style-definition -Werror
INC      = -Isub/ccommon/

CC       = gcc
CFLAGS   = -std=c99 -O3 $(WARNINGS) $(INC) -ggdb
LDFLAGS  =

DESTDIR  =
PREFIX   = /usr/local/

all: lbase
lbase: cat head

clean:
	rm -f cat head *.o

.c.o:
	@echo "CC\t$@"
	@$(CC) $(CFLAGS) -c $<

cat: cat.o
	@echo "CC\t$@"
	@$(CC) $(CFLAGS)   -o $@ $^

head: head.o argoat.o
	@echo "CC\t$@"
	@$(CC) $(CFLAGS)   -o $@ $^

.PHONY: all lbase clean
