#ifndef TTY_H
#define TTY_H

#include "types.h"

const char *name = "tty";
bool quiet;

usize tty(void);
void help(void *data, char **pars, const int pars_count);

#endif
