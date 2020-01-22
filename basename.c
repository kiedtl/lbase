#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "basename.h"
#include "argoat.h"
#include "common.h"

int
main(int argc, char **argv)
{
	if (argc < 2) {
		fprintf(stdout, ".\n");
		return 0;
	}

	basename(argv[1]);
}

/*
 * FIXME: condense multiple loops into one loop(?)
 */
void
basename(char *operand)
{
	usize size = strlen(operand);

	/* handle strings that are // */
	if (strcmp(operand, "//") == 0) {
		fprintf(stdout, "//\n");
		exit(0);
	}

	/*
	 * check if string is entirely /, and if so,
	 * output / and exit.
	 */
	bool isall = FALSE;
	for (usize i = 0; i < size; ++i) {
		if (operand[i] == "/")
			isall = TRUE;
		else isall = FALSE;
	}

	if (isall = TRUE) {
		fprintf(stdout, "/");
		exit(0);
	}
}
