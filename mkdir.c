#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "mkdir.h"
#include "types.h"

int
main(int argc, char **argv)
{
	/* init default options */
	mask = umask(0);

	opts = (struct Options*) malloc(1 * sizeof(struct Options*));
	if (opts == NULL) {
		EPRINT("%s: unable to initialize options: ", NAME);
		perror();
		exit(1);
	} else {
		opts->verbose = FALSE;
		opts->parents = FALSE;
		opts->dmode   = 0777 & ~mask;
	}

	usize files_len = 0;
	char *files[argc];
	const struct argoat_sprig sprigs[11] = {
		{ NULL,      0, (void*) &files_len, handle_main },
		{ "parents", 0, (void*) &opts->parents, handle_bool },
		{ "p",       0, (void*) &opts->parents, handle_bool },
		{ "mode",    1, (void*) &opts->dmode,   handle_mode_t },
		{ "m",       1, (void*) &opts->dmode,   handle_mode_t },
		{ "verbose", 0, (void*) &opts->verbose, handle_bool },
		{ "v",       0, (void*) &opts->verbose, handle_bool },
		{ "help",    0, NULL,                   help },
		{ "h",       0, NULL,                   help},
		{ "version", 0, NULL,                   handle_version },
		{ "v",       0, NULL,                   handle_version },
	};

	struct argoat args = { sprigs, sizeof(sprigs), files, 0, argc };
	argoat_graze(&args, argc, argv);

	return 0;
}
