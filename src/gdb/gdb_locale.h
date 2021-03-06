/* gdb-locale.h: GDB-friendly replacement for <locale.h>.
   Copyright 2002 Free Software Foundation, Inc.

   This file is part of GDB.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

#ifndef GDB_LOCALE_H
#define GDB_LOCALE_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
 #  warning gdb_locale.h expects "config.h" to be included.
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_LOCALE_H
# include <locale.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
 #  warning "gdb_locale.h expects <locale.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_LOCALE_H */

#ifdef ENABLE_NLS
# ifdef HAVE_LIBINTL_H
#  include <libintl.h>
# else
#  if defined(__GNUC__) && !defined(__STRICT_ANSI__)
 #   warning "gdb_locale.h expects <libintl.h> to be included."
#  endif /* __GNUC__ && !__STRICT_ANSI__ */
# endif /* HAVE_LIBINTL_H */
# define _(String) gettext (String)
# ifdef gettext_noop
#  define N_(String) gettext_noop (String)
# else
#  define N_(String) (String)
# endif /* gettext_noop */
#else
# define gettext(Msgid) (Msgid)
# define _gettext(Msgid) (Msgid)
# define dgettext(Domainname, Msgid) (Msgid)
# define dcgettext(Domainname, Msgid, Category) (Msgid)
# define textdomain(Domainname) while (0) /* nothing */
# define bindtextdomain(Domainname, Dirname) while (0) /* nothing */
# define _(String) (String)
# define N_(String) (String)
#endif /* ENABLE_NLS */

#endif /* GDB_LOCALE_H */

/* EOF */
