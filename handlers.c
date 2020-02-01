#include <string.h>
#include "types.h"
#include "handlers.h"

void
handle_main(void *data, char **pars, const int pars_count)
{
	*((usize*) data) = pars_count;
}

void
handle_bool(void *data, char **pars, const int pars_count)
{
	*((bool*) data) = TRUE;
}

void
handle_usize(void *data, char **pars, const int pars_count)
{
	/*
	 * behold: atoi() in just 7 lines!
	 * credits: me (kiedtl), domsson (on codereview.stackexchange.com).
	 */
	usize buffer;
	char *ptr = &pars[0];

	while (*ptr) {
		if (*ptr >= '0' && *ptr <= '9')
			buffer = (buffer * 10) + (*ptr - '0');
		++ptr;
	}

	*((usize*) data = buffer;
}

/*
 * TODO: cleanup this mess
 */
void
handle_number(void *data, char **pars, const int pars_count)
{
        /*
         * format: <integer><unit>
         * e.g.: 200MiB, 3kB, 28G, etc.
         * parse integer first, then parse
         * suffix.
         */
        u64  amount = 0;

        /* suffix buffer */
        char suffix[5] = { 0, 0, 0, 0, 0 };

        /*
         * only first parameter is parsed,
         * the rest are ignored.
        */
        u64  len = strlen(pars[0]);
        char *p = pars[0];

        /* convert suffixes to lowercase */
        char *tmp = p;
        while ((*p = tolower(*p))) ++p;
        p = tmp;

        /*
         * add new digit to amount
         * stop when invalid digit is encountered
         *
        */
        bool contin = TRUE;
        for (usize i = 0; contin && i < len; ++i, ++p) {
                /* handle negative integers */
                if (*p == '-' && i == 0) {
                        EPRINT("%s: negative integers are invalid in this context.\n", NAME);
                        exit(1);
                }

                if (*p > 0x2F && *p < 0x3A)
                        amount = (amount * 10) + (*p - '0');
                else {
                        contin = FALSE;
                        --p;
                }
        }

        /* 
         * check suffix
        */
        usize i = 0;
        for (; i < sizeof(suffix) && *p; ++i, ++p)
                suffix[i] = *p;

        /* TODO: cleanup this mess */
        if (strlen(suffix) == 0) {
                /* do nothing */
        } else if (strcmp(suffix, "b") == 0)
                amount = amount * 512;
        else if (strcmp(suffix, "kb") == 0)
                amount = amount * 1000;
        else if (strcmp(suffix, "mb") == 0)
                amount = amount * 1000 * 1000;
        else if (strcmp(suffix, "gb") == 0)
                amount = amount * 1000 * 1000 * 1000;
        else if (strcmp(suffix, "tb") == 0)
                amount = amount * 1000 * 1000 * 1000 * 1000;
        else if (strcmp(suffix, "pb") == 0)
                amount = amount * 1000 * 1000 * 1000 * 1000 * 1000;
        else if (strcmp(suffix, "eb") == 0)
                amount = amount * 1000 * 1000 * 1000 * 1000 * 1000 * 1000;
        else if (strcmp(suffix, "zb") == 0)
                amount = amount * 1000 * 1000 * 1000 * 1000 * 1000 * 1000 * 1000;
        else if (strcmp(suffix, "yb") == 0)
                amount = amount * 1000 * 1000 * 1000 * 1000 * 1000 * 1000 * 1000 * 1000;
        else if (strcmp(suffix, "k") == 0 || strcmp(suffix, "kib") == 0)
                amount = amount * 1024;
        else if (strcmp(suffix, "m") == 0 || strcmp(suffix, "mib") == 0)
                amount = amount * 1024 * 1024;
        else if (strcmp(suffix, "g") == 0 || strcmp(suffix, "gib") == 0)
                amount = amount * 1024 * 1024 * 1024;
        else if (strcmp(suffix, "t") == 0 || strcmp(suffix, "tib") == 0)
                amount = amount * 1024 * 1024 * 1024 * 1024;
        else if (strcmp(suffix, "p") == 0 || strcmp(suffix, "pib") == 0)
                amount = amount * 1024 * 1024 * 1024 * 1024 * 1024;
        else if (strcmp(suffix, "e") == 0 || strcmp(suffix, "eib") == 0)
                amount = amount * 1024 * 1024 * 1024 * 1024 * 1024 * 1024;
        else if (strcmp(suffix, "z") == 0 || strcmp(suffix, "zib") == 0)
                amount = amount * 1024 * 1024 * 1024 * 1024 * 1024 * 1024 * 1024;
        else if (strcmp(suffix, "y") == 0 || strcmp(suffix, "yib") == 0)
                amount = amount * 1024 * 1024 * 1024 * 1024 * 1024 * 1024 * 1024 * 1024;
        else {
                EPRINT("%s: either '%s' isn't a valid number,", NAME, pars[0]);
                EPRINT("or '%s' is garbage.\n", suffix);
                exit(1);
        }

        *((usize*) data) = amount;
}

void
handle_mode(void *data, char **pars, const int pars_count)
{
	char *end, *token;
	usize op;

	mode_t who = 0, perm, clear;

	i64 oct = strtol(&pars[0], &end, 8);
	
	/* check if that's all there is */
	if (*end == 0) {
		/* check if mode is invalid */
		if (oct < 0 || oct > 07777) {
			EPRINT("%s: '%s': invalid mode.\n", NAME, pars[0]);
			exit(1);
		}
		
		*((usize*) data) = (usize) oct;
	}

	while ((token = strsep(&pars[0], ","))) {
		for (; *token; ++token) {
			/* find out who's permission bits we're dealing with */
			switch (*token) {
				case 'u': /* user */
					who |= S_IRWXU|S_ISUID;
					continue;
				case 'g': /* group */
					who |= S_IRWXG|S_ISGID;
					continue;
				case 'o': /* other aka "everyone else" */
					who |= S_IRWXO;
					continue;
				case 'a': /* all aka "everyone" */
					who |= S_IRWXU|S_ISUID|S_IRWXG|S_ISGID|S_IRWXO;
					continue;
			}

			break;
		}

		if (who) {
			clear = who;
		} else {
			clear = s_isuid|isgid|s_isvtx|s_irwxu|s_irwxg|s_irxwo;
			who = ~mask;
		}

		while (*token) {
		}
	}
}
