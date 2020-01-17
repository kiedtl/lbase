#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "types.h"
#include "common.h"
#include "cat.h"

int
main(int argc, char *argv[])
{
	if (argc < 2) {
		cat(stdin, "-");
		return 0;
	}

	// don't read yourself!
	for (usize i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-") == 0) {
			cat(stdin, "-");
			continue;
		}

		if (strcmp(argv[i], "-u") == 0)
			continue;

		FILE *f;
		if ((f = fopen(argv[i], "r")) == NULL) {
			EPRINT("%s: %s: ", NAME, argv[i]);
			perror("");
			return 0;
		}

		cat(f, argv[i]);
	}

	return 0;
}

void
cat(FILE *f, char *path)
{
	/* if it's stdin, just read one byte at a time */
	if (f == stdin) {
		int c;
		while ((c = getc(stdin)) != EOF)
			putchar(c);
		return;
	}

	int chunk;
	struct stat st;
	stat(path, &st);
	chunk = st.st_size;

	size_t nread;

	// prevent segfault when reading
	// large files
	if (chunk > 32767)
		chunk = 32767;

	char buffer[chunk];

	while ((nread = fread(buffer, 1, chunk, f)) > 0)
		fwrite(buffer, sizeof(char), chunk, stdout);

	fclose(f);
}
