#include <libgen.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "basename.h"
#include "argoat.h"
#include "common.h"

int
main(int argc, char **argv)
{
	if (argc < 2) {
		fprintf(stdout, ".\n");
		return 0;
	}

	if (strcmp(argv[1], "--help") == 0
			|| strcmp(argv[1], "-h") == 0) {
		help();
		return 0;
	} else if (strcmp(argv[1], "--version") == 0) {
		VERSION(name);
		return 0;
	}

	/*
	 * remove suffix
	 */
	isize offset;
	char *result = basename(argv[1]);
	if (argc > 2) {
		offset = strlen(result) - strlen(argv[2]);
		if (offset > 0 && strcmp(result + offset, argv[2]) == 0)
			result[offset] = 0;
	}

	puts(result);
}

void
help(void)
{
	EPRINT("Usage: %s [PATH] [SUFFIX]\n", name);
	EPRINT("   or: %s [--help|--version]\n\n", name);
	EPRINT("Print PATH with just the filename. If SUFFIX is\n");
	EPRINT("provided, PATH will be printed with SUFFIX removed.\n");
	EPRINT("If PATH is null, . will be printed instead.\n\n");
	EPRINT("FLAGS:\n");
	EPRINT("    -h    --help       print this help message and exit.\n");
	EPRINT("          --version    print version information and exit.\n\n");
	EPRINT("EXAMPLES:\n");
	EPRINT("    basename /usr/bin/basename          -> \"basename\"\n");
	EPRINT("    basename /usr/include/stdio.h .h    -> \"stdio\"\n\n");
	EPRINT("Report bugs to https://github.com/kiedtl/lbase.\n");
}
