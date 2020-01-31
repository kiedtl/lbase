/*
 * TODO: support escapes
 * TODO: optimize this (currently several times slower than GNU echo)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "echo.h"

int
main(int argc, char **argv)
{
	/* default options */
	struct Options opts = {
		opts.newline = TRUE,
		opts.escapes = FALSE,
	};

	/* if there aren't any arguments, just exit */
	if (argc < 2) {
		fprintf(stdout, "\n");
		return 0;
	}

	/* was argv[1] an option, or a string to be printed? */
	bool isopts = TRUE;
	if (argv[1][0] == '-') {
		if (strcmp(argv[1], "-n") == 0)
			opts.newline = FALSE;
		else if (strcmp(argv[1], "-e") == 0)
			opts.escapes = TRUE;
		else if (strcmp(argv[1], "-ne") == 0
			|| strcmp(argv[1], "-en") == 0) {
			opts.escapes = TRUE;
			opts.newline = FALSE;
		} else
			isopts = FALSE;
	} else
		isopts = FALSE;


	if (isopts)
		echo(2, argc, argv, &opts);
	else
		echo(1, argc, argv, &opts);

	return 0;
}

void
echo(usize from, usize size, char **string, struct Options *opts) {
	/* print out string one at a time */
	for (usize i = from; i < size; ++i) {
		fprintf(stdout, "%s ", string[i]);
	}

	if (opts->newline) putchar('\n');
}
