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
	};

	/* read stdin if no arguments */
	if (argc < 2) {
		head(stdin, &opts);
		return 0;
	}

	/* parse arguments with argoat */
	char *files[FILE_MAX];
	const struct argoat_sprig sprigs[14] = {
		{ NULL,              0, NULL,                  handle_main   },
		{ "version",         0, NULL,                  version       },
		{ "help",            0, NULL,                  help          },
		{ "h",               0, NULL,                  help          },
		{ "bytes",           1, (void*) &opts.bytes,   handle_number },
		{ "c",               1, (void*) &opts.bytes,   handle_number },
		{ "lines",           1, (void*) &opts.lines,   handle_number },
		{ "n",               1, (void*) &opts.lines,   handle_number },
		{ "q",               0, (void*) &opts.quiet,   handle_bool   },
		{ "quiet",           0, (void*) &opts.quiet,   handle_bool   },
		{ "silent",          0, (void*) &opts.quiet,   handle_bool   },
		{ "verbose",         0, (void*) &opts.verbose, handle_bool   },
		{ "v",               0, (void*) &opts.verbose, handle_bool   },
	};

	struct argoat args = { sprigs, sizeof(sprigs), files, 0, FILE_MAX };

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
	exit(0);
}

void
version(void *data, char **pars, const int pars_count)
{
	VERSION(NAME);
	exit(0);
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
	/* suffix buffer */
	char suffix[5] = { 0, 0, 0, 0, 0 };

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

	/*
	 * add new digit to amount
	 * stop when invalid digit is encountered
	 *
	*/
	bool contin = TRUE;
	for (usize i = 0; contin && i < len; ++i, ++p) {
		/* handle negative integers */
		if (*p == '-' && i == 0) {
			EPRINT("%s: negative integers are invalid in this context.\n", NAME);
			exit(1);
		}

		if (*p > 0x2F && *p < 0x3A)
			amount = (amount * 10) + (*p - '0');
		else {
			contin = FALSE;
			--p;
		}
	}

	/* 
	 * check suffix
	*/
	usize i = 0;
	for (; i < sizeof(suffix) && *p; ++i, ++p)
		suffix[i] = *p;

	/* TODO: cleanup this mess */
	if (strlen(suffix) == 0) {
		/* do nothing */
	} else if (strcmp(suffix, "b") == 0)
		amount = amount * 512;
	else if (strcmp(suffix, "kb") == 0)
		amount = amount * 1000;
	else if (strcmp(suffix, "mb") == 0)
		amount = amount * 1000 * 1000;
	else if (strcmp(suffix, "gb") == 0)
		amount = amount * 1000 * 1000 * 1000;
	else if (strcmp(suffix, "tb") == 0)
		amount = amount * 1000 * 1000 * 1000 * 1000;
	else if (strcmp(suffix, "pb") == 0)
		amount = amount * 1000 * 1000 * 1000 * 1000 * 1000;
	else if (strcmp(suffix, "eb") == 0)
		amount = amount * 1000 * 1000 * 1000 * 1000 * 1000 * 1000;
	else if (strcmp(suffix, "zb") == 0)
		amount = amount * 1000 * 1000 * 1000 * 1000 * 1000 * 1000 * 1000;
	else if (strcmp(suffix, "yb") == 0)
		amount = amount * 1000 * 1000 * 1000 * 1000 * 1000 * 1000 * 1000 * 1000;
	else if (strcmp(suffix, "k") == 0 || strcmp(suffix, "kib") == 0)
		amount = amount * 1024;
	else if (strcmp(suffix, "m") == 0 || strcmp(suffix, "mib") == 0)
		amount = amount * 1024 * 1024;
	else if (strcmp(suffix, "g") == 0 || strcmp(suffix, "gib") == 0)
		amount = amount * 1024 * 1024 * 1024;
	else if (strcmp(suffix, "t") == 0 || strcmp(suffix, "tib") == 0)
		amount = amount * 1024 * 1024 * 1024 * 1024;
	else if (strcmp(suffix, "p") == 0 || strcmp(suffix, "pib") == 0)
		amount = amount * 1024 * 1024 * 1024 * 1024 * 1024;
	else if (strcmp(suffix, "e") == 0 || strcmp(suffix, "eib") == 0)
		amount = amount * 1024 * 1024 * 1024 * 1024 * 1024 * 1024;
	else if (strcmp(suffix, "z") == 0 || strcmp(suffix, "zib") == 0)
		amount = amount * 1024 * 1024 * 1024 * 1024 * 1024 * 1024 * 1024;
	else if (strcmp(suffix, "y") == 0 || strcmp(suffix, "yib") == 0)
		amount = amount * 1024 * 1024 * 1024 * 1024 * 1024 * 1024 * 1024 * 1024;
	else {
		EPRINT("%s: either '%s' isn't a valid number,", NAME, pars[0]);
		EPRINT("or '%s' is garbage.\n", suffix);
		exit(1);
	}


	
	*((usize*) data) = amount;
}

void
handle_bool(void *data, char **pars, const int pars_count)
{
	*((bool*) data) = TRUE;
}
