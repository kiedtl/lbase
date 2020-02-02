#ifndef ECHO_H
#define ECHO_H

#include "types.h"

const char* name = "echo";

/* argument parsing */
typedef struct Options {
	bool newline;
	bool escapes;
} Options;

/* function protoypes */
void echo(usize from, usize size, char **string, struct Options *opts);

#endif
