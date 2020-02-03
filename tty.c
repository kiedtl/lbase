#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "argoat.h"
#include "common.h"
#include "handlers.h"
#include "tty.h"

int
main(int argc, char **argv)
{
	/* default options */
	quiet = FALSE;

	const struct argoat_sprig sprigs[8] = {
		{ NULL,      0, NULL,               NULL           },
		{ "silent",  0, (void*) &quiet,     handle_bool    },
		{ "s",       0, (void*) &quiet,     handle_bool    },
		{ "quiet",   0, (void*) &quiet,     handle_bool    },
		{ "q",       0, (void*) &quiet,     handle_bool    },
		{ "help",    0, NULL,               help           },
		{ "h",       0, NULL,               help           },
		{ "version", 0, NULL,               handle_version },
	};

	struct argoat args = { sprigs, sizeof(sprigs), NULL, 0, 0 };
	argoat_graze(&args, argc, argv);

	return tty();
}

usize
tty(void)
{
	char *tty = ttyname(STDIN_FILENO);

	if (tty == NULL) {
		if (!quiet) {
			if (errno == EBADF) {
				EPRINT("%s: bad file descriptor.\n", name);
			} else if (errno == ENODEV) {
				EPRINT("%s: could not retrieve name.\n", name);
			} else if (errno == ENOTTY) {
				EPRINT("%s: stdin: not a tty.\n", name);
			}
		}

		return 1;
	} else {
		if (!quiet) puts(tty);
	}

	return 0;
}

void
help(void *data, char **pars, const int pars_count)
{
	EPRINT("usage: %s [-s|--silent] [-q|--quiet]\n", name);
	EPRINT("print the path of the TTY connected to stdin.\n\n");
	EPRINT("FLAGS:\n");
	EPRINT("    -q, --quiet     do not print anything, just return an exit status.\n");
	EPRINT("    -s, --silent    same as --quiet.\n");
	EPRINT("    -h, --help      print this help message and exit.\n");
	EPRINT("        --version   print version and exit.\n\n");
	EPRINT("EXAMPLES:\n");
	EPRINT("    tty             print the path of the TTY connected to stdin and exit.\n");
	EPRINT("    tty --silent    if stdin is connected to a TTY, return 0; else, return 1.\n");
	EPRINT("\nReport bugs to https://github.com/kiedtl/lbase.\n");
	exit(0);
}
