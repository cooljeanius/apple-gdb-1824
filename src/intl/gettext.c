/* gettext.c
   Implementation of gettext(3) function.
   Copyright (C) 1995, 1997 Free Software Foundation, Inc.

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
#  warning gettext.c expects "config.h" to be included.
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_CONFIG_H */

#ifdef _LIBC
# define __need_NULL
# ifdef HAVE_STDDEF_H
#  include <stddef.h>
# else
#  if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#   warning "gettext.c expects <stddef.h> to be included."
#  endif /* __GNUC__ && !__STRICT_ANSI__ */
# endif /* HAVE_STDDEF_H */
#else
# if defined STDC_HEADERS || defined HAVE_STDLIB_H
#  include <stdlib.h> /* Just for NULL.  */
# else
#  ifdef HAVE_STRING_H
#   include <string.h>
#  else
#   define NULL ((void *)0)
#  endif /* HAVE_STRING_H */
# endif /* STDC_HEADERS || HAVE_STDLIB_H */
#endif /* _LIBC */

#if defined _LIBC || defined HAVE_LIBINTL_H
# include <libintl.h>
#else
# include "libgettext.h"
#endif /* _LIBC || HAVE_LIBINTL_H */

/* @@ end of prolog @@ */

/* Names for the libintl functions are a problem.  They must not clash
   with existing names and they should follow ANSI C.  But this source
   code is also used in GNU C Library where the names have a __
   prefix.  So we have to make a difference here.  */
#ifdef _LIBC
# define GETTEXT __gettext
# define DGETTEXT __dgettext
#else
# define GETTEXT gettext__
# define DGETTEXT dgettext__
#endif /* _LIBC */

/* Look up MSGID in the current default message catalog for the current
   LC_MESSAGES locale.  If not found, returns MSGID itself (the default
   text).  */
char *GETTEXT(const char *msgid)
{
  return DGETTEXT(NULL, msgid);
}

#ifdef _LIBC
/* Alias for function name in GNU C Library: */
weak_alias(__gettext, gettext);
#endif /* _LIBC */

/* EOF */
