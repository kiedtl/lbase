#include "common.h"
#include <stdio.h>
#include "yes.h"
#include "types.h"
#include <string.h>
#include <stdlib.h>

int
main(int argc, char *argv[])
{
	if (argc < 2) {
		yes("y");
		return 0;
	}

	if (strcmp(argv[1], "--help") == 0
		|| strcmp(argv[1], "-h") == 0) {
		help();
		return 0;
	} else if (strcmp(argv[1], "--version") == 0
		|| strcmp(argv[1], "-v") == 0) {
		VERSION(NAME);
		return 0;
	} else
		yes(argv[1]);

	return 0;
}

void
yes(char *string)
{
	for (;;) puts(string);
}

void
help(void)
{
}
