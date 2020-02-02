#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <sys/stat.h>
#include "common.h"
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
		EPRINT("%s: unable to initialize options: ", name);
		perror("");
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
	};

	struct argoat args = { sprigs, sizeof(sprigs), files, 0, argc };
	argoat_graze(&args, argc, argv);

	for (usize i = 0; i < files_len; ++i) {
		if (makedir(files[i], opts) != 0)
			ret = 1;
	}

	return ret;
}

isize
makedir(char *path, struct Options *opts)
{
	isize ret = 0;

	if (strlen(path) >= PATH_MAX) {
		EPRINT("%s: '%s': path too long.\n", name, path);
		return 1;
	}

	if (!opts->parents) {
		if ((ret = mkdir(path, opts->dmode)) < 0) {
			EPRINT("%s: '%s': ", name, path);
			perror("");
			return 1;
		} else {
			if (opts->verbose)
				fprintf(stdout, "%s: '%s': created.\n", name, path);
		}
	} else {
		char buf[PATH_MAX], *token;
		if (path[0] == '/') buf[0] = '/';
		bool isfirst = TRUE;

		while ((token = strsep(&path, "/")) != NULL) {
			if (!isfirst) strcat((char*) &buf, "/");
			strcat((char*) &buf, token);

			isize status;
			if ((status = mkdir(buf, opts->dmode)) < 0
					&& errno != EEXIST) {
				EPRINT("%s: '%s': cannot create: ", name, buf);
				perror("");
				ret = 1;
			}

			if (opts->verbose && status == 0)
				fprintf(stdout, "%s: '%s': created.\n", name, buf);

			isfirst = FALSE;
		}
	}

	return ret;
}

/*
 * TODO: implement
 */
void
help(void *data, char **pars, const int pars_count)
{
	EPRINT("usage: %s [OPTIONS] [DIRECTORY]\n", name);
	EPRINT("create DIRECTORY(ies), if they don't already exist.\n\n");
	EPRINT("FLAGS:\n");
	EPRINT("    -p    --parents        create parent directories if they don't exit.\n");
	EPRINT("    -v    --verbose        on each directory creation, print a message.\n");
	EPRINT("    -h    --help           print this help message and exit.\n");
	EPRINT("          --version        print version and exit.\n\n");
	EPRINT("OPTIONS:\n");
	EPRINT("    -m    --mode [MODE]    mode to set file to.\n\n");
	EPRINT("EXAMPLES:\n");
	EPRINT("    mkdir -v etc           create 'etc' in the current directory and print message.\n");
	EPRINT("    mkdir images           create 'images' in the current directory.\n");
	EPRINT("    mkdir -p /usr/share/   create 'share' in /usr. if /usr doesn't exist, create it.\n");
	EPRINT("\nReport issues to https://github.com/kiedtl/lbase.\n");
}
