/* 
 * gettod.c --
 *
 *	This file provides the gettimeofday function on systems
 *	that only have the System V ftime function.
 *
 * Copyright (c) 1995 Sun Microsystems, Inc.
 *
 * See the file "license.terms" for information on usage and redistribution
 * of this file, and for a DISCLAIMER OF ALL WARRANTIES.
 *
 * RCS: @(#) gettod.c,v 1.5 2003/01/21 19:39:57 hunt Exp
 */

#include "tcl.h"
#include "tclPort.h"
#include <sys/timeb.h>

#undef timezone

int
gettimeofday(tp, tz)
struct timeval *tp;
struct timezone *tz;
{
    struct timeb t;
    ftime(&t);
    tp->tv_sec = t.time;
    tp->tv_usec = t. millitm * 1000;
    return 0;
}

