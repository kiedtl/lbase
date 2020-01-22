WARNINGS = -Wall -Wextra -pedantic -Wmissing-prototypes \
	   -Wold-style-definition -Werror \
	   -Wno-unused-parameter
INC      = -Isub/ccommon/

BIN      = cat head wc yes

CC       = gcc
CFLAGS   = -std=c99 -O3 $(WARNINGS) $(INC) -ggdb
LDFLAGS  =

DESTDIR  =
PREFIX   = /usr/local/

all: lbase
lbase: $(BIN)

clean:
	rm -f $(BIN) *.o

.c.o:
	@echo "CC\t$@"
	@$(CC) $(CFLAGS) -c $<

cat: cat.o
	@echo "CC\t$@"
	@$(CC) $(CFLAGS)   -o $@ $^

head: head.o argoat.o
	@echo "CC\t$@"
	@$(CC) $(CFLAGS)   -o $@ $^

wc: wc.o argoat.o common.o
	@echo "CC\t$@"
	@$(CC) $(CFLAGS)   -o $@ $^

yes: yes.o
	@echo "CC\t$@"
	@$(CC) $(CFLAGS)   -o $@ $^

.PHONY: all lbase clean
