#include <errno.h>
#include <stdio.h>
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

	const struct argoat_sprig sprigs[5] = {
		{ NULL,     0, NULL,               NULL        },
		{ "silent", 0, (void*) &quiet,     handle_bool },
		{ "s",      0, (void*) &quiet,     handle_bool },
		{ "quiet",  0, (void*) &quiet,     handle_bool },
		{ "q",      0, (void*) &quiet,     handle_bool },
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
