#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "echo.h"

struct Options *opts;

int
main(int argc, char **argv)
{
	/* default options */
	opts = (struct Options*) malloc(1 * sizeof(struct Options*));
	opts->newline = TRUE;
	opts->escapes = FALSE;

	/* if there aren't any arguments, just exit */
	if (argc < 2) {
		fprintf(stdout, "\n");
		return 0;
	}

	/* was argv[1] an option, or a string to be printed? */
	bool isopts = FALSE;
	if (strcmp(argv[1], "-n") == 0) {
		isopts = TRUE;
		opts->newlines = FALSE;
	} else if (strcmp(argv[1], "-e") == 0) {
		isopts = TRUE;
		opts->escapes = TRUE;
	} else if (strcmp(argv[1], "-ne") == 0
		|| strcmp(argv[1], "-en") == 0) {
		isopts = TRUE;
		opts->escapes = TRUE;
		opts->newline = FALSE;
	}

	if (isopts)
		echo(2, argc - 2, argv);
	else
		echo(1, argc - 1, argv);

	return 0;
}

/*
 * FIXME: cleanup this mess
 * TODO:  support escape sequences \0NNN and \xNNN
 */
void
echo(usize from, usize size, char **string) {
	/* print out string one at a time */
	for (usize i = from; i < size; ++i) {
		usize len = strlen(string[i]);
		for (usize c = 0; c < len; ++c) {
			char ch = string[i][c];
			if (ch == '\\' && opts->escapes) {
				++c;
				ch = string[i][c];

				switch (ch) {
				case 'a':
					putchar(7);
					break;
				case 'b':
					putchar(8);
					break;
				case 'c':
					if (opts->newline)
						putchar(10);
					exit(0);
					break;
				case 'e':
					putchar(27);
					break;
				case 'f':
					putchar(12);
					break;
				case 'n':
					putchar(10);
					break;
				case 'r':
					putchar(13);
					break;
				case 't':
					putchar(9);
					break;
				case 'v':
					putchar(11);
					break;
				case '\\':
				default:
					break;
				}
			} else putchar(ch);
		}
	}

	if (opts->newline)
		fprintf(stdout, "\n");
}
