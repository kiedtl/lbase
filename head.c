#include <string.h>
#include <stdio.h>
#include "head.h"
#include "types.h"
#include "common.h"
#include "argoat.h"

struct Options *opts;

int
main(int argc, char **argv)
{
	if (argc < 2) {
		head(stdin, "");
		return 0;
	}

	/* default options */
	opts->lines          = 10;
	opts->bytes          = 0;
	opts->quiet          = FALSE;
	opts->verbose        = FALSE;
	opts->nul_terminated = FALSE;

	/* parse arguments with argoat */
	char *files[FILE_MAX];
	const struct argoat_sprig sprigs[5] = {
		{ NULL,      0, NULL, handle_main },
		{ "version", 0, NULL, version     },
		{ "v",       0, NULL, version     },
		{ "help",    0, NULL, help        },
		{ "h",       0, NULL, help        },
	};

	struct argoat args = { sprigs, 5, files, 0, FILE_MAX };

	argoat_graze(&args, argc, argv);
	usize files_len = sizeof(files) / sizeof(files[0]);

	/* iterate over files, opening them and passing them to head() */
	for (usize i = 0; i < files_len; i++) {
		if (strcmp(files[i], "-") == 0) {
			head(stdin, "");
			continue;
		}

		FILE *f;
		if ((f = fopen(files[i], "r")) == NULL) {
			EPRINT("%s: %s: ", NAME, files[i]);
			perror("");
			return 1;
		}

		/* print header */
		if (opts->verbose || files_len > 1) {
			if (!opts->quiet)
				fprintf(stdout, "\n==> %s <==\n", files[i]);
		}

		head(f, files[i]);
	}
}

void
head(FILE *f, char *path)
{
	/* if opts->bytes is 0, use lines mode instead */
	if (opts->bytes > 0) {
		char c;
		for (usize i = 0; (c == getc(f)) != EOF && i < opts->bytes; i++) {
			putchar(c);
		}
	} else if (opts->lines > 0) {
		char c;
		usize l = 0;
		while ((c == getc(f)) != EOF && l < opts->lines) {
			putchar(c);

			/* increment if char is newline */
			if (c == 10) l++;
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

/* dummy function */
void
handle_main(void *data, char **pars, const int pars_count)
{

}
