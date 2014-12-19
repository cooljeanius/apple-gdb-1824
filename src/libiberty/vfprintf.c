/* vfprintf.c: Provide a version vfprintf in terms of _doprnt.
   By Kaveh Ghazi  (ghazi@caip.rutgers.edu)  3/29/98
   Copyright (C) 1998 Free Software Foundation, Inc.
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "ansidecl.h"
#include <stdarg.h>
#include <stdio.h>
#undef vfprintf

#if (defined(PROTOTYPES) || defined(__PROTOTYPES) || __STDC__) && \
    (!defined(__STRICT_ANSI__) || defined(__STDC_VERSION__)) && \
    (!defined(_doprnt) || !defined(HAVE__DOPRNT))
extern int _doprnt(const char *, va_list, FILE *);
#endif /* (protos) && (non-pedantic C version) && !_doprnt */

int
vfprintf(FILE *stream, const char *format, va_list ap)
{
  return _doprnt(format, ap, stream);
}

/* EOF */
