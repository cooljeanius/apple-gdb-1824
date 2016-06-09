/* symcat.h: Symbol concatenation utilities.

   Copyright (C) 1998, 2000 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along
   with this program; if not, write to the Free Software Foundation, Inc.,
   51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.  */

#ifndef SYM_CAT_H
#define SYM_CAT_H

#if defined(__GNUC__) && defined(__GNUC_MINOR__) && !defined(__cplusplus)
# if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6))
 #  pragma GCC diagnostic push
 #  pragma GCC diagnostic warning "-Wtraditional"
# endif /* gcc 4.6+ */
#endif /* GCC (non-c++) */

#if defined(__STDC__) || defined(ALMOST_STDC) || defined(HAVE_STRINGIZE)
# define CONCAT2(a,b)	 a##b
# define CONCAT3(a,b,c)	 a##b##c
# define CONCAT4(a,b,c,d) a##b##c##d
# define STRINGX(s) #s
#else
/* Note one should never pass extra whitespace to the CONCATn macros,
 * e.g. CONCAT2(foo, bar) because traditional C will keep the space between
 * the two labels instead of concatenating them.  Instead, make sure to
 * write CONCAT2(foo,bar).  */
# define CONCAT2(a,b)	 a/**/b
# define CONCAT3(a,b,c)	 a/**/b/**/c
# define CONCAT4(a,b,c,d) a/**/b/**/c/**/d
# define STRINGX(s) "s"
#endif /* __STDC__ || ALMOST_STDC || HAVE_STRINGIZE */

#define XCONCAT2(a,b)     CONCAT2(a,b)
#define XCONCAT3(a,b,c)   CONCAT3(a,b,c)
#define XCONCAT4(a,b,c,d) CONCAT4(a,b,c,d)

/* Note the layer of indirection here is typically used to allow
 * stringification of the expansion of macros.  I.e. "#define foo
 * bar", "XSTRING(foo)", to yield "bar".  Be aware that this only
 * works for __STDC__, not for traditional C which will still resolve
 * to "foo".  */
#define XSTRING(s) STRINGX(s)

/* keep condition the same as where we push: */
#if defined(__GNUC__) && defined(__GNUC_MINOR__) && !defined(__cplusplus)
# if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6))
 #  pragma GCC diagnostic pop
# endif /* gcc 4.6+ */
#endif /* GCC (non-c++) */

/* in case the popping failed: */
#if defined(__GNUC__) && defined(__GNUC_MINOR__) && !defined(__clang__) && \
    !defined(__cplusplus)
# if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 2)) 
 #  pragma GCC diagnostic ignored "-Wtraditional"
# endif /* gcc 4.2+ */
#endif /* gcc && !__clang__ && !__cplusplus */

#endif /* SYM_CAT_H */

/* EOF */
