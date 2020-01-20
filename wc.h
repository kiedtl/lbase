#ifndef WC_H
#define WC_H

#include <stdio.h>
#include "types.h"
#define NAME     "wc"
#define PADDING  2

/* limitation of argoat */
#define FILE_MAX 999

/* argument parsing */
usize files_len;

typedef struct Options {
	bool normal;
	bool bytes;
	bool chars;
	bool lines;
	bool width;
	bool words;
} Options;

typedef struct Result {
	u64 bytes;
	u64 chars;
	u64 lines;
	u64 width;
	u64 words;
	char *path;
} Result;

/* function prototypes */
void handle_main(void *data, char **pars, const int pars_count);
struct Result wc(FILE *f, char *path);
void format_results(struct Result result[], usize count);
void handle_flag(void *data, char **pars, const int pars_count);
void help(void *data, char **pars, const int pars_count);
void version(void *data, char **pars, const int pars_count);

#endif
