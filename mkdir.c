#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <sys/stat.h>
#include "mkdir.h"
#include "types.h"
#include "handlers.h"

int
main(int argc, char **argv)
{
	usize ret = 0;

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
		{ "mode",    1, (void*) &opts->dmode,   handle_mode },
		{ "m",       1, (void*) &opts->dmode,   handle_mode },
		{ "verbose", 0, (void*) &opts->verbose, handle_bool },
		{ "v",       0, (void*) &opts->verbose, handle_bool },
		{ "help",    0, NULL,                   help },
		{ "h",       0, NULL,                   help},
		{ "version", 0, NULL,                   handle_version },
		{ "v",       0, NULL,                   handle_version },
	};

	struct argoat args = { sprigs, sizeof(sprigs), files, 0, argc };
	argoat_graze(&args, argc, argv);

	for (usize i = 0; i < files_len; ++i) {
		if (mkdir(files[i], &opts) != 0)
			ret = 1;
	}

	return ret;
}

usize
mkdir(char *path, struct Options *opts)
{
	usize ret = 0;

	if (strlen(path) >= PATH_MAX) {
		EPRINT("%s: '%s': path too long.\n", NAME, path);
		return 1;
	}

	if (!opts->parents) {
		if ((ret = mkdir(path, opts->dmode)) < 0) {
			EPRINT("%s: '%s': ", NAME, path);
			perror("");
			return 1;
		}
	} else {
		char buf[PATH_MAX], *token;
		buf[0] = '/';

		while ((token = strsep(path, "/"))) {
			strcat(&buf, token);

			if (mkdir(buf, opts->dmode) < 0 && errno != EEXIST) {
				EPRINT("%s: '%s': cannot create: ", NAME, buf);
				perror("");
				ret = 1;
			}
		}

		if (mkdir(buf, opts->dmode) < 0 && errno != EEXIST) {
			EPRINT("%s: '%s': cannot create: ", NAME, path);
			perror("");
			return 1;
		}
	}
	
	if (opts->verbose)
		fprintf(stdout, "%s: '%s': created.\n", NAME, path);

	return ret;
}
