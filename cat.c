#include <stdio.h>
#include <string.h>
#include "types.h"
#include "common.h"
#include "cat.h"

int
main(int argc, char *argv[])
{
	if (argc < 2) {
		cat(stdin);
		return 0;
	}

	// don't read yourself!
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-") == 0) {
			cat(stdin);
			continue;
		}

		if (strcmp(argv[i], "-u") == 0)
			continue;

		FILE *f;
		if ((f = fopen(argv[i], "r")) == NULL) {
			EPRINT("%s: error:");
			perror("fopen()");
		}

		cat(f);
	}

	return 0;
}

void
cat(FILE *f)
{
	size_t nread;
	char buffer[CHUNK];

	while ((nread = fread(buffer, 1, sizeof(buffer), f)) > 0)
		puts(buffer);

	fclose(f);
}
