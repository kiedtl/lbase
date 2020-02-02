#ifndef MKDIR_H
#define MKDIR_H

#include <sys/stat.h>
#include "types.h"
#include "argoat.h"

const char *name = "mkdir";
mode_t mask;

typedef struct Options {
	mode_t dmode;
	bool parents;
	bool verbose;
} Options;

struct Options *opts;

isize makedir(char *path, struct Options *opts);
void help(void *data, char **pars, const int pars_count);

#endif
