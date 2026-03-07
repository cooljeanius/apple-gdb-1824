/*
 * tclUnixThrd.h --
 *
 *      This header file defines things for thread support.
 *
 * Copyright (c) 1998 Sun Microsystems, Inc.
 *
 * See the file "license.terms" for information on usage and redistribution
 * of this file, and for a DISCLAIMER OF ALL WARRANTIES.
 *
 * SCCS: @(#)
 */

#ifndef _TCLUNIXTHRD
# define _TCLUNIXTHRD

# ifdef HAVE_CONFIG_H
#  include "config.h"
# else
#  if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#   warning Not including "config.h"
#  endif /* __GNUC__ && !__STRICT_ANSI__ */
# endif /* HAVE_CONFIG_H */

# ifdef TCL_THREADS

#  define THIS_IS_AN_EMPTY_DEFINE

# endif /* TCL_THREADS */
#endif /* _TCLUNIXTHRD */
