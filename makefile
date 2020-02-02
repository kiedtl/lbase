#
# lbase: a reimplementation of the GNU coreutils
# see the LICENSE for more information.
#
# NOTE: CFLAGS_OPTS is defined on a target-by-target basis

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
CFLAGS   = -std=c99 $(WARNINGS) $(INC) \
	   -D_BSD_SOURCE
LDFLAGS  = -fuse-ld=$(LD)

DESTDIR  =
PREFIX   = /usr/local/

all: debug

clean:
	rm -f $(BIN) *.o

debug: CFLAGS_OPTS := -ggdb
debug: $(BIN)

lbase: CFLAGS_OPTS := -O3 -s
lbase: $(BIN)

$(BIN): $(LIB) $(@=.o)

$(OBJ): common.h

.o:
	@echo "CC\t$@"
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LIB)

.c.o:
	@echo "CC\t$@"
	@$(CC) $(CFLAGS) -c $<

.PHONY: all clean lbase debug
