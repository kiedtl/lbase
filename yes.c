#include "common.h"
#include <unistd.h>
#include "yes.h"
#include "types.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int
main(int argc, char *argv[])
{
	if (argc > 1) {
		if (strcmp(argv[1], "--help") == 0
			|| strcmp(argv[1], "-h") == 0) {
			help();
			return 0;
		} else if (strcmp(argv[1], "--version") == 0
			|| strcmp(argv[1], "-v") == 0) {
			VERSION(name);
			return 0;
		}
	}
	
	if (argc < 2) yes("y\n", 2);
	else {
		usize size = strlen(argv[1]) + 1;
		char string[size];
		strcpy(string, argv[1]);
		string[size - 1] = '\n';
		yes(string, size);
	}
}

void
yes(char *string, usize size)
{
	char *buf = malloc(SIZE);
	usize bufused = 0;

	while (bufused < SIZE) {
		memcpy(buf + bufused, string, size);
		bufused += size;
	}

	while (write(1, buf, SIZE));
}

void
help(void)
{
	EPRINT("Usage: %s [STRING]\n", name);
	EPRINT("   or: %s [--help|--version]\n\n", name);
	EPRINT("Repeatedly output STRING until terminated.\n");
	EPRINT("When STRING is null, output 'y'.\n\n");
	EPRINT("FLAGS:\n");
	EPRINT("    --help       display this help and exit.\n");
	EPRINT("    --version    display version and exit.\n\n");
	EPRINT("Report bugs to https://github.com/kiedtl/lbase.\n");
}
