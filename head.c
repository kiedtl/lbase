#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "head.h"
#include "types.h"
#include "common.h"
#include "argoat.h"

int
main(int argc, char **argv)
{
	/* default options */
	struct Options opts = {
		opts.lines          = 10,
		opts.bytes          = 0,
		opts.quiet          = FALSE,
		opts.verbose        = FALSE,
		opts.nul_terminated = FALSE
	};

	/* read stdin if no arguments */
	if (argc < 2) {
		head(stdin, &opts);
		return 0;
	}

	/* parse arguments with argoat */
	char *files[FILE_MAX];
	const struct argoat_sprig sprigs[9] = {
		{ NULL,      0, NULL,                handle_main   },
		{ "version", 0, NULL,                version       },
		{ "v",       0, NULL,                version       },
		{ "help",    0, NULL,                help          },
		{ "h",       0, NULL,                help          },
		{ "bytes",   1, (void*) &opts.bytes, handle_number },
		{ "c",       1, (void*) &opts.bytes, handle_number },
		{ "lines",   1, (void*) &opts.lines, handle_number },
		{ "n",       1, (void*) &opts.lines, handle_number },
	};

	struct argoat args = { sprigs, 9, files, 0, FILE_MAX };

	files_len = 0;
	argoat_graze(&args, argc, argv);

	/* read stdin if no files provided */
	if (files_len < 1) {
		head(stdin, &opts);
		return 0;
	}

	/* iterate over files, opening them and passing them to head() */
	for (usize i = 0; i < files_len; ++i) {
		if (strcmp(files[i], "-") == 0) {
			head(stdin, &opts);
			continue;
		}

		FILE *f;
		if ((f = fopen(files[i], "r")) == NULL) {
			EPRINT("%s: %s: ", NAME, files[i]);
			perror("");
			return 1;
		}

		/* print header */
		if (opts.verbose || files_len > 1) {
			if (!opts.quiet)
				fprintf(stdout, "\n==> %s <==\n", files[i]);
		}

		head(f, &opts);
	}
}

void
head(FILE *f, struct Options *opts)
{
	/* if opts->bytes is 0, use lines mode instead */
	if (opts->bytes > 0) {
		int c = 0;
		usize i = 0;
		while ((c = getc(f)) != EOF) {
			putchar(c);
			++i;

			if (i >= opts->bytes) break;
		}
	} else if (opts->lines > 0) {
		int c = 0;
		usize l = 0;
		while ((c = getc(f)) != EOF && l < opts->lines) {
			putchar(c);

			/* increment if char is newline */
			if (c == 10) ++l;
		}
	}
}

void
help(void *data, char **pars, const int pars_count)
{

}

void
version(void *data, char **pars, const int pars_count)
{
	VERSION(NAME);
}

void
handle_main(void *data, char **pars, const int pars_count)
{
	files_len = pars_count;
}

/* parse pseudo-integers like 100kB or 20MiB */
void
handle_number(void *data, char **pars, const int pars_count)
{
	/*
	 * format: <integer><unit>
	 * e.g.: 200MiB, 3kB, 28G, etc.
	 * parse integer first, then parse
	 * suffix.
	*/
	u64  amount = 0;
	//char suffix[10];

	/*
	 * only first parameter is parsed,
	 * the rest are ignored.
	*/
	u64  len = strlen(pars[0]);
	char *p = pars[0];

	/* convert suffixes to lowercase */
	char *tmp = p;
	while ((*p = tolower(*p))) ++p;
	p = tmp;

	/* add new digit to amount */
	bool contin = TRUE;
	for (usize i = 0; contin && i < len; ++i, ++p) {
		switch (*p) {
		case '0':
			amount = (amount * 10);
			break;
		case '1':
			amount = (amount * 10) + 1;
			break;
		case '2':
			amount = (amount * 10) + 2;
			break;
		case '3':
			amount = (amount * 10) + 3;
			break;
		case '4':
			amount = (amount * 10) + 4;
			break;
		case '5':
			amount = (amount * 10) + 5;
			break;
		case '6':
			amount = (amount * 10) + 6;
			break;
		case '7':
			amount = (amount * 10) + 7;
			break;
		case '8':
			amount = (amount * 10) + 8;
			break;
		case '9':
			amount = (amount * 10) + 9;
			break;
		default:
			contin = FALSE;
			break;
		}
	}

	/* 
	 * check suffix
	 * a sidenote: the dim bulbs at GNU
	 * support P, E, Z, Y as suffixes,
	 * but I see no reason to support that, 
	 * especially since the maximum file size
	 * of ext4 is 16T :P
	*/
	
	/* TODO: support suffixes */

	*((usize*) data) = amount;
}
