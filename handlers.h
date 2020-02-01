/* handlers.h:
 * this file defines common handlers
 * that are used to handle events from argoat
 * 	- handle_main:    update files_len with argument count
 * 	- handle_bool:    handle booleans
 * 	- handle_usize:   parse char* into usize (integer)
 * 	- handle_number:  parse stuff like "128KiB" into usize (integer)
 * 	- handle_mode:    parse file modes into mode_t
 */

#ifndef HANDLERS_H
#define HANDLERS_H


/* function prototypes */
void handle_main(void *data, char **pars, const int pars_count);
void handle_bool(void *data, char **pars, const int pars_count);
void handle_usize(void *data, char **pars, const int pars_count);
void handle_number(void *data, char **pars, const int pars_count);
void handle_mode(void *data, char **pars, const int pars_count);
void handle_version(void *data, char **pars, const int pars_count);

#endif
