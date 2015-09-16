
/**
 * \file strsignal.c
 *
 * Time-stamp:        "2010-07-16 15:12:27 bkorb"
 *
 *  This file is part of AutoGen.
 *
 *  AutoGen Copyright (c) 1992-2012 by Bruce Korb - all rights reserved
 *
 *  AutoGen is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  AutoGen is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  As a special exception, Bruce Korb gives permission for additional
 *  uses of the text contained in the release of strsignal.
 *
 *  The exception is that, if you link the strsignal library with other
 *  files to produce an executable, this does not by itself cause the
 *  resulting executable to be covered by the GNU General Public License.
 *  Your use of that executable is in no way restricted on account of
 *  linking the strsignal library code into it.
 *
 *  This exception does not however invalidate any other reasons why
 *  the executable file might be covered by the GNU General Public License.
 *
 *  This exception applies only to the code released by Bruce Korb under
 *  the name strsignal.  If you copy code from other sources under the
 *  General Public License into a copy of strsignal, as the General Public
 *  License permits, the exception does not apply to the code that you add
 *  in this way.  To avoid misleading anyone as to the status of such
 *  modified files, you must delete this exception notice from them.
 *
 *  If you write modifications of your own for strsignal, it is your choice
 *  whether to permit this exception to apply to your modifications.
 *  If you do not wish that, delete this exception notice.
 */

#include "compat.h"

/*  Routines imported from standard C runtime libraries. */

#if ! defined(HAVE_STRSIGNAL)

#ifdef __STDC__
# include <stddef.h>
#else   /* !__STDC__ */
#  ifndef const
#    define const
#  endif
#endif  /* __STDC__ */

#ifdef HAVE_SYS_SIGLIST
#  include <signal.h>
#endif

/*
 *  Import the generated tables
 */
#include "strsignal.h"
#endif

#ifndef HAVE_STRSIGNAL

/*

NAME

    strsignal -- map a signal number to a signal message string

SYNOPSIS

    char *strsignal (int signo)

DESCRIPTION

    Maps an signal number to an signal message string, the contents of
    which are implementation defined.  On systems which have the external
    variable sys_siglist, these strings will be the same as the ones used
    by psignal().

    If the supplied signal number is within the valid range of indices
    for the sys_siglist, but no message is available for the particular
    signal number, then returns the string "Signal NUM", where NUM is the
    signal number.

    If the supplied signal number is not a valid index into sys_siglist,
    returns NULL.

    The returned string is only guaranteed to be valid only until the
    next call to strsignal.

    Also, though not declared "const", it is.
*/

char *
strsignal( int signo )
{
    if (SIGNAL_IN_RANGE( signo ))
        return (char*)SIGNAL_INFO( signo );

    return NULL;
}
#endif  /* HAVE_STRSIGNAL */

/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * indent-tabs-mode: nil
 * End:
 * end of compat/strsignal.c */
