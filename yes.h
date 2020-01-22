/*
 * Thanks to the author of the following Reddit post:
 * https://old.reddit.com/r/unix/comments/6gxduc/how_is_gnu_yes_so_fast/
 * It provides a good overview on the speed of GNU yes.
 */

#ifndef YES_H
#define YES_H

#include <stdio.h>
#define NAME  "yes"
#define SIZE  BUFSIZ * 2

/* function prototypes */
void yes(char *string, usize size);
void help(void);

#endif
