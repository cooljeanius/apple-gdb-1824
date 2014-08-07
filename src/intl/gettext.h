/* gettext.h
   Internal header for GNU gettext internationalization functions.
   Copyright (C) 1995, 1997 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the GNU C Library; see the file COPYING.LIB. If not,
   write to the Free Software Foundation, 51 Franklin Street - 5th Floor,
   Boston, MA 02110-1301, USA.  */

#ifndef _GETTEXT_H
#define _GETTEXT_H 1

#ifdef HAVE_CONFIG_H
# include "config.h"
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__) && !defined(HAVE_CONFDEFS_H)
#  warning gettext.h expects "config.h" to be included.
# endif /* __GNUC__ && !__STRICT_ANSI__ && !HAVE_CONFDEFS_H */
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_STDIO_H
# include <stdio.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "gettext.h expects <stdio.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_STDIO_H */

#if HAVE_LIMITS_H || _LIBC
# include <limits.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "gettext.h expects <limits.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_LIMITS_H */

/* @@ end of prolog @@ */

/* The magic number of the GNU message catalog format: */
#define _MAGIC 0x950412de
#define _MAGIC_SWAPPED 0xde120495

/* Revision number of the currently used .mo (binary) file format: */
#define MO_REVISION_NUMBER 0

/* The following contortions are an attempt to use the C preprocessor
   to determine an unsigned integral type that is 32 bits wide.  An
   alternative approach is to use autoconf's AC_CHECK_SIZEOF macro, but
   doing that would require that the configure script compile and *run*
   the resulting executable.  Locally running cross-compiled executables
   is usually not possible.  */

#if __STDC__
# define UINT_MAX_32_BITS 4294967295U
#else
# define UINT_MAX_32_BITS 0xFFFFFFFF
#endif /* __STDC__ */

/* If UINT_MAX is NOT defined, assume it is a 32-bit type.
   This should be valid for all systems GNU cares about because
   that does NOT include 16-bit systems, and only modern systems
   (that certainly have <limits.h>) have 64+-bit integral types.  */

#ifndef UINT_MAX
# define UINT_MAX UINT_MAX_32_BITS
#endif /* !UINT_MAX */

#if (UINT_MAX == UINT_MAX_32_BITS)
typedef unsigned nls_uint32;
#else
# if (USHRT_MAX == UINT_MAX_32_BITS)
typedef unsigned short nls_uint32;
# else
#  if (ULONG_MAX == UINT_MAX_32_BITS)
typedef unsigned long nls_uint32;
#  else
  /* The following line is intended to throw an error. Using #error is
   * not portable enough (really? it is in the standard...): */
  "Cannot determine unsigned 32-bit data type."
#  endif /* ULONG_MAX == UINT_MAX_32_BITS */
# endif /* USHRT_MAX == UINT_MAX_32_BITS */
#endif /* UINT_MAX == UINT_MAX_32_BITS */


/* Header for binary .mo file format: */
struct mo_file_header
{
  /* The magic number: */
  nls_uint32 magic;
  /* The revision number of the file format: */
  nls_uint32 revision;
  /* The number of strings pairs: */
  nls_uint32 nstrings;
  /* Offset of table with start offsets of original strings: */
  nls_uint32 orig_tab_offset;
  /* Offset of table with start offsets of translation strings: */
  nls_uint32 trans_tab_offset;
  /* Size of hashing table: */
  nls_uint32 hash_tab_size;
  /* Offset of first hashing entry: */
  nls_uint32 hash_tab_offset;
};

struct string_desc
{
  /* Length of addressed string: */
  nls_uint32 length;
  /* Offset of string in file: */
  nls_uint32 offset;
};

/* @@ begin of epilog @@ */

#endif	/* gettext.h  */

/* EOF */
