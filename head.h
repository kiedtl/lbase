#ifndef HEAD_H
#define HEAD_H

#include <stdio.h>
#include "argoat.h"
#include "types.h"
#define NAME     "head"

/* limitation of argoat, not head */
#define FILE_MAX 999

/* argument parsing */
typedef struct Options {
	usize lines;
	usize bytes;
	bool  quiet;
	bool  verbose;
	bool  nul_terminated;
} Options;

/* function prototypes */
void head(FILE *f, char *path);
void handle_main(void *data, char **pars, const int pars_count);
void help(void *data, char **pars, const int pars_count);
void version(void *data, char **pars, const int pars_count);

#endif
