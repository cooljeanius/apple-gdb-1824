/* dgettext.c: Implementation of the dgettext(3) function -*- C -*-
   Copyright (C) 1995, 1996, 1997 Free Software Foundation, Inc.

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
#  warning dgettext.c expects "config.h" to be included.
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_CONFIG_H */

#if defined HAVE_LOCALE_H || defined _LIBC
# include <locale.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "dgettext.c expects <locale.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_LOCALE_H */

#include "gettext.h"
#include "gettextP.h"
#if defined _LIBC || defined HAVE_LIBINTL_H
# include <libintl.h>
#else
# include "libgettext.h"
# include "libgnuintl.h"
#endif /* HAVE_LIBINTL_H */
#include "hash-string.h"

/* @@ end of prolog @@ */

/* Names for the libintl functions are a problem.  They must not clash
   with existing names and they should follow ANSI C.  But this source
   code is also used in GNU C Library where the names have a __
   prefix.  So we have to make a difference here.  */
#ifdef _LIBC
# define DGETTEXT __dgettext
# define DCGETTEXT __dcgettext
#else
# define DGETTEXT dgettext__
# if defined(HAVE_DCGETTEXT__) || (defined(HAVE_DECL_DCGETTEXT__) && HAVE_DECL_DCGETTEXT__)
#  define DCGETTEXT dcgettext__
# elif defined(__HAVE_DCGETTEXT) || (defined(HAVE___DECL_DCGETTEXT) && HAVE___DECL_DCGETTEXT)
#  define DCGETTEXT __dcgettext
# elif defined(HAVE_LIBINTL_DCGETTEXT) || (defined(HAVE_DECL_LIBINTL_DCGETTEXT) && HAVE_DECL_LIBINTL_DCGETTEXT)
#  define DCGETTEXT libintl_dcgettext
# else
#  define DCGETTEXT dcgettext
# endif /* HAVE_DCGETTEXT__ || HAVE_DECL_DCGETTEXT__ || etc. */
#endif /* _LIBC */

/* Look up MSGID in the DOMAINNAME message catalog of the current
   LC_MESSAGES locale.  */
char *DGETTEXT(const char *domainname, const char *msgid)
{
  return DCGETTEXT(domainname, msgid, LC_MESSAGES);
}

#ifdef _LIBC
/* Alias for function name in GNU C Library.  */
weak_alias(__dgettext, dgettext);
#endif /* _LIBC */

/* EOF */
