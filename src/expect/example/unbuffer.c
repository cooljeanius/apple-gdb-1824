/* unbuffer.c */

#include <stdio.h>
#include "expect.h"

int
main(int argc, char *argv[])
{
	argv++;
	exp_timeout = -1;
	exp_expectl(exp_spawnv(*argv,argv),exp_end);
}

/* EOF */
