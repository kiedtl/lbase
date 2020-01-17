WARNINGS = -Wall -Wextra -pedantic -Wmissing-prototypes \
	   -Wold-style-definition -Werror
INC      = -Isub/ccommon/

CC       = gcc
CFLAGS   = -std=c99 -O3 $(WARNING) $(INC)
LDFLAGS  =

DESTDIR  =
PREFIX   = /usr/local/

all: lbase
lbase: cat

clean:
	rm -f cat

cat: cat.o
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: all lbase clean
