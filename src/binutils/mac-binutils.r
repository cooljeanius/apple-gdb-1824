/* mac-binutils.r: Rez resources for GNU binutils. */

#include "SysTypes.r"

#ifndef VERSION_STRING
# define VERSION_STRING "6.3.50-20050815 (Apple version gdb-1824)"
#endif /* !VERSION_STRING */

/* Version resources. */

resource 'vers' (1)  {
	0,
	0,
	0,
	0,
	verUs,
	VERSION_STRING,
	VERSION_STRING  " (C) 1986-95 FSF, Inc."
};

resource 'vers' (2, purgeable)  {
	0,
	0,
	0,
	0,
	verUs,
	VERSION_STRING,
	"binutils " VERSION_STRING " for MPW"
};

#ifdef WANT_CFRG

#include "CodeFragmentTypes.r"

resource 'cfrg' (0) {
	{
		kPowerPC,
		kFullLib,
		kNoVersionNum, kNoVersionNum,
		0,0,
		kIsApp, kOnDiskFlat, kZeroOffset, kWholeFork,
		PROG_NAME
	}
};

#endif /* WANT_CFRG */
