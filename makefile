.SUFFIXES: .o .c

WARNINGS = -Wall -Wextra -pedantic -Wmissing-prototypes \
	   -Wold-style-definition -Werror \
	   -Wno-unused-parameter
INC      = -Isub/ccommon/ -I.

LIB      = common.o handlers.o argoat.o
BIN      = basename cat echo false head mkdir wc yes true
OBJ      = $(BIN:=.o)
SRC      = $(BIN:=.c)

CC       = cc
LD       = gold
CFLAGS   = -std=c99 -O3 $(WARNINGS) $(INC) -ggdb \
	   -D_BSD_SOURCE
LDFLAGS  = -fuse-ld=$(LD)

DESTDIR  =
PREFIX   = /usr/local/

all: $(BIN)

clean:
	rm -f $(BIN) *.o

$(BIN): $(LIB) $(@=.o)

$(OBJ): common.h

.o:
	@echo "CC\t$@"
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LIB)

.c.o:
	@echo "CC\t$@"
	@$(CC) $(CFLAGS) -c $<

.PHONY: all clean
