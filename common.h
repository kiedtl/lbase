#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <string.h>
#include <types.h>
#define LBASE_VERSION  "0.0.1"

#define VERSION(n)    fprintf(stdout, "lbase->%s v%s\n", n, LBASE_VERSION);
#define EPRINT(...)   fprintf(stderr, __VA_ARGS__);

#endif
