#include "common.h"
#include "types.h"

/* get number of digits in integer */
usize
intlen(u64 i)
{
	if (i == 0) return 1;
	
	usize c = 0;
	while (i > 0) {
		i = i / 10;
		c++;
	}

	return c;
}
