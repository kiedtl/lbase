#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "head.h"
#include "types.h"
#include "common.h"
#include "argoat.h"
#include "handlers.h"

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
		{ NULL,      0, NULL,                  handle_main    },
		{ "version", 0, NULL,                  handle_version },
		{ "help",    0, NULL,                  help           },
		{ "h",       0, NULL,                  help           },
		{ "bytes",   1, (void*) &opts.bytes,   handle_number  },
		{ "c",       1, (void*) &opts.bytes,   handle_number  },
		{ "lines",   1, (void*) &opts.lines,   handle_number  },
		{ "n",       1, (void*) &opts.lines,   handle_number  },
		{ "q",       0, (void*) &opts.quiet,   handle_bool    },
		{ "quiet",   0, (void*) &opts.quiet,   handle_bool    },
		{ "silent",  0, (void*) &opts.quiet,   handle_bool    },
		{ "verbose", 0, (void*) &opts.verbose, handle_bool    },
		{ "v",       0, (void*) &opts.verbose, handle_bool    },
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
			EPRINT("%s: %s: ", name, files[i]);
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
	EPRINT("Usage: %s [OPTIONS] [FILE]\n", name);
	EPRINT("Print the first 10 lines of each provided FILE to stdout.\n");
	EPRINT("With more than one FILE, precede each with a header giving the file path.\n");
	EPRINT("With no FILE, or when FILE is -, read from stdin.\n\n");
	EPRINT("FLAGS:\n");
	EPRINT("     -q    --quiet      never print headers for FILE.\n");
	EPRINT("           --silent     same as --quiet.\n");
	EPRINT("     -v    --verbose    always print headers for FILE.\n");
	EPRINT("     -h    --help       show this help and exit.\n");
	EPRINT("           --version    print lbase version and exit.\n\n");
	EPRINT("OPTIONS:\n");
	EPRINT("    -c     --bytes [NUM]    print first NUM bytes of FILE.\n");
	EPRINT("    -n     --lines [NUM]    print first NUM lines of FILE.\n\n");
	EPRINT("lbase head supports all multiplier suffixes of GNU head: b, kB, KiB, etc.\n\n");
	EPRINT("Report issues to https://github.com/kiedtl/lbase.\n");

	exit(0);
}
