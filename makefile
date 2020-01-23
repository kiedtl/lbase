.SUFFIXES: .o .c

WARNINGS = -Wall -Wextra -pedantic -Wmissing-prototypes \
	   -Wold-style-definition -Werror \
	   -Wno-unused-parameter
INC      = -Isub/ccommon/ -I.

LIB      = common.o argoat.o
BIN      = basename cat echo head wc yes true false
OBJ      = $(BIN:=.o)
SRC      = $(BIN:=.c)

CC       = cc
LD       = lld
CFLAGS   = -std=c99 -O3 $(WARNINGS) $(INC) -ggdb
LDFLAGS  = -fuse-ld=$(LD)

DESTDIR  =
PREFIX   = /usr/local/

all: $(BIN)

clean:
	rm -f $(BIN) *.o

$(BIN): $(LIB) $(OBJ)

$(OBJ): common.h

.o:
	@echo "CC\t$@"
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LIB)

.c.o:
	@echo "CC\t$@"
	@$(CC) $(CFLAGS) -c $<

.PHONY: all clean
