#ifndef WC_H
#define WC_H

#include <stdio.h>
#include "types.h"
#define NAME     "wc"
#define PADDING  5

/* limitation of argoat */
#define FILE_MAX 999

/* argument parsing */
usize files_len;

enum Mode {
	mode_normal,
	mode_bytes,
	mode_chars,
	mode_lines,
	mode_width,
	mode_words
};

typedef struct Options {
	bool normal;
	bool bytes;
	bool chars;
	bool lines;
	bool width;
	bool words;
	bool verbose;
	Mode cmode;
} Options;

typedef struct Result {
	u64 bytes;
	u64 chars;
	u64 lines;
	u64 width;
	u64 words;
} Result;

/* function prototypes */
void handle_main(void *data, char **pars, const int pars_count);
struct Result wc(FILE *f);
void format_results(struct Result result[], usize count);
void handle_flag(void *data, char **pars, const int pars_count);
void help(void *data, char **pars, const int pars_count);
void version(void *data, char **pars, const int pars_count);

#endif
