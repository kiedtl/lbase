#ifndef BASENAME_H
#define BASENAME_H

#include "types.h"

const char* name = "basename";

/* limitation of argoat, not basename */
#define FILE_MAX 999

/* argument parsing */
usize files_len;

/* function prototypes */
void help(void);

#endif
