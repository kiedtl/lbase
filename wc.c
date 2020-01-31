#include <stdio.h>
#include <stdlib.h>
#include "wc.h"
#include "common.h"
#include "argoat.h"
#include "types.h"

struct Options *opts;

int
main(int argc, char *argv[])
{
	/* number of non-flag arguments, will be set in handle_main */
	usize files_len = 0;

	/* default options */
	opts = (struct Options*) malloc(1 * sizeof(struct Options*));
	if (opts == NULL) {
		EPRINT("%s: unable to initialize options: ", NAME);
		perror("");
		exit(1);
	} else {
		opts->normal  = TRUE;
		opts->bytes   = FALSE;
		opts->chars   = FALSE;
		opts->lines   = FALSE;
		opts->width   = FALSE;
		opts->words   = FALSE;
	}

	/* parse arguments with argoat */
	char *files[FILE_MAX];
	const struct argoat_sprig sprigs[14] = {
		{ NULL,               0, (void*) &files_len,   handle_main },
		{ "bytes",            0, (void*) &opts->bytes, handle_flag },
		{ "c",                0, (void*) &opts->bytes, handle_flag },
		{ "chars",            0, (void*) &opts->chars, handle_flag },
		{ "m",                0, (void*) &opts->chars, handle_flag },
		{ "lines",            0, (void*) &opts->chars, handle_flag },
		{ "l",                0, (void*) &opts->lines, handle_flag },
		{ "words",            0, (void*) &opts->words, handle_flag },
		{ "w",                0, (void*) &opts->words, handle_flag },
		{ "max-lines-length", 0, (void*) &opts->width, handle_flag },
		{ "L",                0, (void*) &opts->width, handle_flag },
		{ "help",             0, NULL,                 help,       },
		{ "h",                0, NULL,                 help,       },
		{ "version",          0, NULL,                 version,    },
	};

	struct argoat args = { sprigs, sizeof(sprigs), files, 0, FILE_MAX };
	argoat_graze(&args, argc, argv);

	struct Result results[files_len];

	/* main loop */

	/* read from stdin if no files provided */
	if (argc < 2 || files_len == 0) {
		results[0] = wc(stdin, " ");
		format_results(results, 1);
		if (opts) free(opts);
		return 0;
	}

	for (usize i = 0; i < files_len; ++i) {
		if (strcmp(files[i], "-") == 0) {
			results[i] = wc(stdin, " ");
			continue;
		}

		FILE *f;
		if ((f = fopen(files[i], "r")) == NULL) {
			EPRINT("%s: %s: ", NAME, files[i]);
			perror("");

			results[i].error = TRUE;
			continue;
		}
		
		results[i] = wc(f, files[i]);
	}

	format_results(results, files_len);

	if (opts) free(opts);
	return 0;
}

void
handle_main(void *data, char **pars, const int pars_count)
{
	*((usize*) data) = pars_count;
}

void
handle_flag(void *data, char **pars, const int pars_count)
{
	opts->normal = FALSE;
	*((bool*) data) = TRUE;
}

struct Result
wc(FILE *f, char *path)
{
	struct Result result = {
		result.error = FALSE,
		result.bytes = 0,
		result.chars = 0,
		result.lines = 0,
		result.width = 0,
		result.words = 0,
		result.path  = path,
	};

	u64 c_width   = 0;

	i32 c         = 0;
	bool word     = FALSE;

	while ((c = getc(f)) != EOF) {
		/* count bytes */
		++result.bytes;

		/*
		 * count UTF-8 characters
		 * skip UTF-8 continuation characters
		 */
		if ((c & 0xc0) != 0x80) {
			++result.chars;

			/* tabs have a width of 8 */
			if (c == 9) c_width += 8;
			else ++c_width;
		}

		/* count newlines/line width */
		if (c == 10) {
			if (c_width > result.width)
				result.width = c_width;
			c_width = 0;

			++result.lines;
		}

		/* count words */

		if (c != 32)
			word = TRUE;
		else if (word) {
			word = FALSE;
			++result.words;
		}
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

	struct Result totals = {
		totals.error = FALSE,
		totals.bytes = 0,
		totals.chars = 0,
		totals.lines = 0,
		totals.width = 0,
		totals.words = 0,
		totals.path = "total",
	};

	/* calculate maximum width and totals */
	for (usize i = 0; i < count; ++i) {
		if (results[i].error) continue;

		totals.bytes += results[i].bytes;
		usize byte_len = intlen(results[i].bytes);
		if (byte_len > byte_padding)
			byte_padding = byte_len;

		totals.chars += results[i].chars;
		usize char_len = intlen(results[i].chars);
		if (char_len > char_padding)
			char_padding = char_len;

		totals.lines += results[i].lines;
		usize line_len = intlen(results[i].lines);
		if (line_len > line_padding)
			line_padding = line_len;

		totals.words += results[i].words;
		usize word_len = intlen(results[i].words);
		if (word_len > word_padding)
			word_padding = word_len;
	
		totals.width += results[i].width;
		usize width_len = intlen(results[i].width);
		if (width_len > width_padding)
			width_padding = width_len;
	}

	/* print out padding, then information */
	/* TODO: cleanup */
	for (usize i = 0; i < count; ++i) {
		if (results[i].error) continue;

		/* print newlines */
		if (opts->normal || opts->lines)
			fprintf(stdout, "%c[%iC%lli", 0x1B, 
				(PADDING + line_padding) - intlen(results[i].lines),
				results[i].lines);

		/* print word count */
		if (opts->normal || opts->words)
			fprintf(stdout, "%c[%iC%lli", 0x1B,
				(PADDING + word_padding) - intlen(results[i].words),
				results[i].words);

		/* print characters */
		if (opts->normal || opts->chars)
			fprintf(stdout, "%c[%iC%lli", 0x1B,
				(PADDING + char_padding) - intlen(results[i].chars),
				results[i].chars);

		/* print bytes */
		if (opts->bytes)
			fprintf(stdout, "%c[%iC%lli", 0x1B,
				(PADDING + byte_padding) - intlen(results[i].bytes),
				results[i].bytes);

		/* print maximum line length */
		if (opts->width)
			fprintf(stdout, "%c[%iC%lli", 0x1B,
				(PADDING + width_padding) - intlen(results[i].width),
				results[i].width);

		/* print file path */
		fprintf(stdout, "%c[%iC%s\n", 0x1B, PADDING, results[i].path);
	}

	/* print total */
	if (count > 1) {
		if (opts->normal || opts->lines)
			fprintf(stdout, "%c[%iC%lli", 0x1B,
				(PADDING + line_padding) - intlen(totals.lines), totals.lines);
		if (opts->normal || opts->words)
			fprintf(stdout, "%c[%iC%lli", 0x1B,
				(PADDING + word_padding) - intlen(totals.words), totals.words);
		if (opts->normal || opts->chars)
			fprintf(stdout, "%c[%iC%lli", 0x1B,
				(PADDING + line_padding) - intlen(totals.lines), totals.chars);
		if (opts->bytes)
			fprintf(stdout, "%c[%iC%lli", 0x1B,
				(PADDING + line_padding) - intlen(totals.lines), totals.bytes);
		if (opts->width)
			fprintf(stdout, "%c[%iC%lli", 0x1B,
				(PADDING + line_padding) - intlen(totals.lines), totals.width);
		fprintf(stdout, "%c[%iC%s\n", 0x1B, PADDING, totals.path);
	}
}

void
help(void *data, char **pars, const int pars_count)
{
	EPRINT("Usage: wc [OPTION] [FILE]\n");
	EPRINT("Print (newline|word|char|bytes) count for FILE.\n");
	EPRINT("If more than one FILE, print a total count.\n\n");
	EPRINT("If no FILE is provided, or if FILE is -, read from stdin.\n\n");
	EPRINT("FLAGS:\n");
	EPRINT("    -c    --bytes              print byte count of FILE.\n");
	EPRINT("    -m    --chars              print UTF-8 character count of FILE.\n");
	EPRINT("    -l    --lines              print newline count of FILE.\n");
	EPRINT("    -w    --words              print word count of FILE.\n");
	EPRINT("    -L    --max-line-length    print maximum line length of FILE.\n");
	EPRINT("    -h    --help               print this help message and exit.\n");
	EPRINT("          --version            print lbase version and exit.\n\n");
	EPRINT("EXAMPLES:\n");
	EPRINT("    wc FILE -l    print line count from FILE.\n");
	EPRINT("    wc -cwl       print character, word, and line count from stdin.\n");
	EPRINT("    wc            same as above.\n\n");
	EPRINT("Report bugs to https://github.com/kiedtl/lbase.\n");

	exit(0);
}

void
version(void *data, char **pars, const int pars_count)
{
	VERSION(NAME);
	exit(0);
}
