/* textdomain.c
   Implementation of the textdomain(3) function.
   Copyright (C) 1995, 1996, 1997, 1998 Free Software Foundation, Inc.
   Written by Ulrich Drepper <drepper@gnu.ai.mit.edu>, 1995.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.  */

#ifdef HAVE_CONFIG_H
# include "config.h"
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning textdomain.c expects "config.h" to be included.
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_CONFIG_H */

#if defined STDC_HEADERS || defined _LIBC || defined HAVE_STDLIB_H
# include <stdlib.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "textdomain.c expects <stdlib.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_STDLIB_H */

#if defined STDC_HEADERS || defined HAVE_STRING_H || defined _LIBC
# include <string.h>
#elif defined HAVE_STRINGS_H
# include <strings.h>
# ifndef memcpy
#  define memcpy(Dst, Src, Num) bcopy(Src, Dst, Num)
# endif /* !memcpy */
#endif /* STDC_HEADERS || HAVE_STRING_H || _LIBC */

#if defined _LIBC || defined HAVE_LIBINTL_H
# include <libintl.h>
#else
# include "libgettext.h"
#endif /* _LIBC || HAVE_LIBINTL_H */

/* @@ end of prolog @@ */

/* Name of the default text domain.  */
extern const char _nl_default_default_domain[];

/* Default text domain in which entries for gettext(3) are to be found.  */
extern const char *_nl_current_default_domain;


/* Names for the libintl functions are a problem.  They must not clash
   with existing names and they should follow ANSI C.  But this source
   code is also used in GNU C Library where the names have a __
   prefix.  So we have to make a difference here.  */
#ifdef _LIBC
# define TEXTDOMAIN __textdomain
# ifndef strdup
#  define strdup(str) __strdup(str)
# endif /* !strdup */
#else
# define TEXTDOMAIN textdomain__
#endif /* _LIBC */

/* Set the current default message catalog to DOMAINNAME.
   If DOMAINNAME is null, return the current default.
   If DOMAINNAME is "", reset to the default of "messages".  */
char *TEXTDOMAIN(const char *domainname)
{
  char *old;

  /* A NULL pointer requests the current setting.  */
  if (domainname == NULL)
    return (char *) _nl_current_default_domain;

  old = (char *) _nl_current_default_domain;

  /* If domain name is the null string set to default domain "messages": */
  if ((domainname[0] == '\0')
      || (strcmp(domainname, _nl_default_default_domain) == 0))
    _nl_current_default_domain = _nl_default_default_domain;
  else
    {
      /* If the following malloc fails `_nl_current_default_domain'
	 will be NULL.  This value will be returned and so signals we
	 are out of core.  */
#if defined _LIBC || defined HAVE_STRDUP
      _nl_current_default_domain = strdup (domainname);
#else
      size_t len = (strlen(domainname) + 1);
      char *cp = (char *)malloc(len);
      if (cp != NULL) {
	memcpy(cp, domainname, len);
      }
      _nl_current_default_domain = cp;
#endif /* _LIBC || HAVE_STRDUP */
    }

  if (old != _nl_default_default_domain) {
    free(old);
  }

  return (char *)_nl_current_default_domain;
}

#ifdef _LIBC
/* Alias for function name in GNU C Library.  */
weak_alias (__textdomain, textdomain);
#endif /* _LIBC */

/* EOF */
