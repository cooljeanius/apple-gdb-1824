/* syslex_wrap.c: Copyright 2012 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street - Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* Wrapper source to ensure that config.h is the first header file seen by
   the compiler.  */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif /* HAVE_CONFIG_H */
#if defined(HAVE_STRING_H) || (defined(STDC_HEADERS) && defined(__STDC__))
# include <string.h>
#else
# ifdef HAVE_STRINGS_H
#  include <strings.h>
# else
#  ifdef HAVE_MEMORY_H
#   include <memory.h>
#  else
#   if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#    warning "syslex_wrap.c needs a string-related header"
#   endif /* __GNUC__ && !__STRICT_ANSI__ */
#  endif /* HAVE_MEMORY_H */
# endif /* HAVE_STRINGS_H */
#endif /* HAVE_STRING_H || (STDC_HEADERS && __STDC__) */
/* in case these got re-defined on us: */
#ifdef malloc
# undef malloc
#endif /* malloc */
#ifdef realloc
# undef realloc
#endif /* realloc */

#include "syslex.c"

/* End of syslex_wrap.c */
