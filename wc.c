#include <stdio.h>
#include <stdlib.h>
#include "wc.h"
#include "common.h"
#include "argoat.h"
#include "types.h"

struct Options opts;

int
main(int argc, char *argv[])
{
	/* default options */
	opts = {
		opts.normal  = TRUE,
		opts.bytes   = FALSE,
		opts.chars   = FALSE,
		opts.lines   = FALSE,
		opts.width   = FALSE,
		opts.words   = FALSE,
		opts.verbose = FALSE,
		opts.cmode   = mode_normal,
	};

	/* read stdin if no arguments */
	if (argc < 2) {
		wc(stdin, "stdin");
		return 0;
	}

	/* parse arguments with argoat */
	char *files[FILE_MAX];
	const struct argoat_sprig sprigs[12] = {
		{ NULL,               0, NULL,                handle_main },
		{ "bytes",            0, (void*) &opts.bytes, handle_flag },
		{ "c",                0, (void*) &opts.bytes, handle_flag },
		{ "chars",            0, (void*) &opts.chars, handle_flag },
		{ "m",                0, (void*) &opts.chars, handle_flag },
		{ "lines",            0, (void*) &opts.chars, handle_flag },
		{ "l",                0, (void*) &opts.lines, handle_flag },
		{ "max-lines-length", 0, (void*) &opts.lines, handle_flag },
		{ "L",                0, (void*) &opts.lines, handle_flag },
		{ "help",             0, NULL,                help,       },
		{ "h",                0, NULL,                help,       },
		{ "version",          0, NULL,                version,    },
	};

	/* parse arguments, main loop is in handle_main  */
	struct argoat args = { sprigs, sizeof(sprigs), files, 0, FILE_MAX };
	argoat_graze(&args, argc, argv);
}

void
handle_main(void *data, char **pars, const int pars_count)
{
	/* main loop */
	for (usize i = 0; i < pars_count; ++i) {
		if (strcmp(pars[i], "-") == 0) {
			wc(stdin, "stdin");
			continue;
		}

		FILE *f;
		if ((f = fopen(pars[i], "r")) == NULL) {
			EPRINT("%s: %s: ", NAME, pars[i]);
			perror("");
			exit(0);
		}

		wc(f, pars[i]);
	}
}

void
handle_flag(void *data, char **pars, const int pars_count)
{
	opts.normal = FALSE;
	*((bool*) data) = TRUE;
}

struct Result
wc(FILE *f, char *path)
{
	struct Result result = {
		result.bytes = 0;
		result.chars = 0;
		result.lines = 0;
		result.width = 0;
		result.words = 0;
	}

	u64 c_width = 0;

	i32 last_char;
	i32 c;

	while ((c = getc(f)) != EOF) {
		/* count bytes */
		++result.bytes;

		/* count UTF-8 characters */
		if ((c & 0xc0) != 0x80)
			++result.chars;

		/* count newlines/line width */
		if (c == 10) {
			if (c_width > result.width)
				result.width = c_width;
			c_width = 0;

			++result.lines;
		}

		/* count non-consecutive spaces */
		if (last_char != 32 && c == 32)
			++result.words;

		last_char = c;
		++c_width;
	}

	return result;
}

void
format_results(struct Result results[], usize count)
{
	usize byte_padding  = 0;
	usize char_padding  = 0;
	usize line_padding  = 0;
	usize word_padding  = 0;
	usize width_padding = 0;

	/* calculate maximum width */
	for (usize i = 0; i < count; ++i) {
		usize byte_len = intlen(results[i].bytes);
		if (byte_len > byte_padding)
			byte_padding = byte_len;

		usize char_len = intlen(result[i].chars);
		if (char_len > char_padding)
			char_padding = char_len;

		usize line_len = intlen(result[i].lines);
		if (line_len > line_padding)
			line_padding = line_len;

		usize word_len = intlen(result[i].words);
		if (word_len > word_padding)
			word_padding = word_len;
		
		usize width_len = intlen(result[i].width);
		if (width_len > width_padding)
			width_padding = width_len;
	}

	/* print out padding, then information */
	/* TODO: cleanup */
	//for (usize i = 0; i < z

	/* print total */
}

void
help(void *data, char **pars, const int pars_count)
{
	exit(0);
}

void
version(void *data, char **pars, const int pars_count)
{
	VERSION(NAME);
	exit(0);
}
