#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "types.h"
#include "common.h"
#include "cat.h"

int
main(int argc, char *argv[])
{
	/* read stdin if no arguments */
	if (argc < 2) {
		cat(stdin, "");
		return 0;
	}

	/* ignore first argument */
	for (usize i = 1; i < (usize) argc; i++) {
		/*
		 * primitive argument parsing
		 * reasons for not using full-fledged
		 * argument parsing:
		 * a) pure laziness
		 * b) for speed (?)
		 * c) we only support --help and --version anyway
		*/
		if (argv[i][0] == '-') {
			/* if argument is -, read from stdin */
			if (strlen(argv[i]) == 1) {
				cat(stdin, "");
				continue;
			} else {
				switch (argv[i][1]) {
				case '-':
					if (strcmp(argv[i], "--help") == 0) {
						help();
						return 0;
					} else if (strcmp(argv[i], "--version") == 0) {
						VERSION(name);
						return 0;
					}

					break;
				/* ignored */
				case 'u':
				case 'A':
				case 'b':
				case 'e':
				case 'E':
				case 'n':
				case 's':
				case 't':
				case 'T':
				case 'v':
					continue;
				/* show help */
				case 'h':
					help();
					return 0;
					break;
				/* everything else */
				default:
					EPRINT("%s: unknown option '%c'.\n", name, argv[i][1]);
					return 0;
					break;
				}
			}
		}

		FILE *f;
		if ((f = fopen(argv[i], "r")) == NULL) {
			EPRINT("%s: %s: ", name, argv[i]);
			perror("");
			return 0;
		}

		cat(f, argv[i]);
	}

	return 0;
}

/* show help */
void
help(void)
{
	EPRINT("Usage: %s [--help|--version] [FILE]...\n", name);
	EPRINT("Concatenate FILE(s) to stdout.\n");
	EPRINT("With no arguments, or when FILE is -, stdin is read.\n\n");
	EPRINT("FLAGS:\n")
	EPRINT("        --version    Show version and exit.\n")
	EPRINT("    -h  --help       Show this help message and exit.\n\n");
	EPRINT("EXAMPLES:\n");
	EPRINT("    cat f - g    Read file f, then stdin, then g.\n");
	EPRINT("    cat          Copy standard input to standard output.\n\n");
	EPRINT("Report issues to https://github.com/kiedtl/lbase.\n");
}

/* meat of this program */
void
cat(FILE *f, char *path)
{
	/* if it's stdin, just read one byte at a time */
	if (f == stdin) {
		int c;
		while ((c = getc(stdin)) != EOF)
			putchar(c);
		return;
	}

	int chunk;
	struct stat st;
	stat(path, &st);
	chunk = st.st_size;

	size_t nread;

	// prevent segfault when reading
	// large files
	if (chunk > 32767)
		chunk = 32767;

	char buffer[chunk];

	while ((nread = fread(buffer, 1, chunk, f)) > 0)
		fwrite(buffer, sizeof(char), chunk, stdout);
	
	if (f == stdin)
		fclose(f);
}
