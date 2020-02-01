#ifndef MKDIR_H
#define MKDIR_H

#include <sys/stat.h>
#include "argoat.h"

#define NAME "mkdir"

mode_t mask;

typedef struct Options {
	mode_t dmode;
	bool parents;
	bool verbose;
} Options;

struct Options *opts;

usize mkdir(char *path, struct Options *opts);

#endif
