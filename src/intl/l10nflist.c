/* l10nflist.c
   Handle list of needed message catalogs
   Copyright (C) 1995, 1996, 1997 Free Software Foundation, Inc.
   Contributed by Ulrich Drepper <drepper@gnu.ai.mit.edu>, 1995.

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
#  warning l10nflist.c expects "config.h" to be included.
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_CONFIG_H */

#if !defined(__has_include)
# define __has_include(foo) 0
#endif /* !__has_include */

#if defined(HAVE_STRING_H) || defined(_LIBC) || __has_include(<string.h>)
# ifndef _GNU_SOURCE
#  define _GNU_SOURCE 1
# endif /* !_GNU_SOURCE */
# include <string.h>
#else
# if defined(HAVE_STRINGS_H) || __has_include(<strings.h>)
#  include <strings.h>
#  ifndef memcpy
#   define memcpy(Dst, Src, Num) bcopy(Src, Dst, Num)
#  endif /* !memcpy */
# else
#  if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#   warning "l10nflist.c expects a string-related header to be included."
#  endif /* __GNUC__ && !__STRICT_ANSI__ */
# endif /* HAVE_STRINGS_H */
#endif /* HAVE_STRING_H || _LIBC */
#if !HAVE_STRCHR && !defined _LIBC
# ifndef strchr
#  define strchr index
# endif /* !strchr */
#endif /* HAVE_STRING_H || _LIBC */

#if defined(_LIBC) || defined(HAVE_ARGZ_H) || __has_include(<argz.h>)
# include <argz.h>
#else
# if defined(__GNUC__) && defined(__STDC__) && defined(ANSI_PROTOTYPES) \
     && !defined(__STRICT_ANSI__)
#  warning "l10nflist.c expects <argz.h> to be included on some systems."
# endif /* __GNUC__ && __STDC__ && ANSI_PROTOTYPES && !__STRICT_ANSI__ */
#endif /* _LIBC || HAVE_ARGZ_H */

#if defined(HAVE_CTYPE_H) || __has_include(<ctype.h>)
# include <ctype.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "l10nflist.c expects <ctype.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_CTYPE_H */

#if defined(HAVE_SYS_TYPES_H) || __has_include(<sys/types.h>)
# include <sys/types.h>
#else
# if defined(HAVE_GCC_SYS_TYPES_H) || __has_include(<gcc/sys-types.h>)
#  include <gcc/sys-types.h>
# else
#  if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#   warning "l10nflist.c expects <sys/types.h> (or something) to be included."
#  endif /* __GNUC__ && !__STRICT_ANSI__ */
# endif /* HAVE_GCC_SYS_TYPES_H */
#endif /* HAVE_SYS_TYPES_H */

#if defined(HAVE_MALLOC_H) || __has_include(<malloc.h>)
# include <malloc.h>
#else
# if defined(HAVE_MALLOC_MALLOC_H) || __has_include(<malloc/malloc.h>)
#  include <malloc/malloc.h>
# else
#  if defined(HAVE_SYS_MALLOC_H) && defined(HAVE_U_SHORT) && defined(HAVE_U_INT64_T)
#   include <sys/malloc.h>
#  else
#   if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#    warning "l10nflist.c expects a malloc-related header to be included."
#   endif /* __GNUC__ && !__STRICT_ANSI__ */
#  endif /* HAVE_SYS_MALLOC_H && HAVE_U_SHORT && HAVE_U_INT64_T */
# endif /* HAVE_MALLOC_MALLOC_H */
#endif /* HAVE_MALLOC_H */

#if defined(STDC_HEADERS) || defined(_LIBC) || defined(HAVE_STDLIB_H)
# include <stdlib.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "l10nflist.c expects <stdlib.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* STDC_HEADERS || _LIBC || HAVE_STDLIB_H */

#include "loadinfo.h"

/* On some strange systems still no definition of NULL is found. Sigh! */
#ifndef NULL
# if defined __STDC__ && __STDC__
#  define NULL ((void *)0)
# else
#  define NULL 0
# endif /* __STDC__ && __STDC__ */
#endif /* !NULL */

#ifndef PARAMS
# if (defined(__STDC__) && __STDC__) || defined(__GNUC__) || \
     defined(__SUNPRO_C) || defined(__cplusplus) || defined(PROTOTYPES) \
     (defined(__PROTOTYPES) && __PROTOTYPES)
#  define PARAMS(args) args
# else
#  define PARAMS(args) ()
# endif /* __STDC__ */
#endif /* !PARAMS */

/* @@ end of prolog @@ */

#ifdef _LIBC
/* Rename the non ANSI C functions. This is required by the standard
 * because some ANSI C functions will require linking with this object
 * file and the name space must not be polluted.  */
# ifndef stpcpy
#  define stpcpy(dest, src) __stpcpy(dest, src)
# endif /* !stpcpy */
#else
# ifndef HAVE_STPCPY
static char *stpcpy PARAMS((char *dest, const char *src));
# endif /* !HAVE_STPCPY */
#endif /* _LIBC */

/* Define function which are usually not available.  */

#if !defined _LIBC && !defined HAVE___ARGZ_COUNT
/* Returns the number of strings in ARGZ.  */
static size_t argz_count__ PARAMS((const char *argz, size_t len));

static size_t argz_count__(const char *argz, size_t len)
{
  size_t count = 0;
  while (len > 0) {
      size_t part_len = strlen(argz);
      argz += (part_len + 1);
      len -= (part_len + 1);
      count++;
  }
  return count;
}
# undef __argz_count
# define __argz_count(argz, len) argz_count__ (argz, len)
#endif	/* !_LIBC && !HAVE___ARGZ_COUNT */

#if !defined _LIBC && !defined HAVE___ARGZ_STRINGIFY
/* Make '\0' separated arg vector ARGZ printable by converting all the '\0's
   except the last into the character SEP.  */
static void argz_stringify__ PARAMS((char *argz, size_t len, int sep));

static void argz_stringify__(char *argz, size_t len, int sep)
{
  while (len > 0) {
      size_t part_len = strlen(argz);
      argz += part_len;
      len -= (part_len + 1);
      if (len > 0) {
	*argz++ = (char)sep;
      }
  }
}
# undef __argz_stringify
# define __argz_stringify(argz, len, sep) argz_stringify__(argz, len, sep)
#endif	/* !_LIBC && !HAVE___ARGZ_STRINGIFY */

#if !defined _LIBC && !defined HAVE___ARGZ_NEXT
static char *argz_next__ PARAMS((char *argz, size_t argz_len,
                                 const char *entry));

static char *argz_next__(char *argz, size_t argz_len, const char *entry)
{
  if (entry)
    {
      if (entry < argz + argz_len)
        entry = strchr (entry, '\0') + 1;

      return entry >= argz + argz_len ? NULL : (char *) entry;
    }
  else
    if (argz_len > 0)
      return argz;
    else
      return 0;
}
# undef __argz_next
# define __argz_next(argz, len, entry) argz_next__ (argz, len, entry)
#endif	/* !_LIBC && !HAVE___ARGZ_NEXT */


/* Return number of bits set in X.  */
static int pop PARAMS((int x));

static
#if !defined(__STRICT_ANSI__)
inline
#endif /* !__STRICT_ANSI__ */
int pop(int x)
{
  /* We assume that no more than 16 bits are used.  */
  x = ((x & ~0x5555) >> 1) + (x & 0x5555);
  x = ((x & ~0x3333) >> 2) + (x & 0x3333);
  x = ((x >> 4) + x) & 0x0f0f;
  x = ((x >> 8) + x) & 0xff;

  return x;
}


struct loaded_l10nfile *
_nl_make_l10nflist(struct loaded_l10nfile **l10nfile_list,
                   const char *dirlist, size_t dirlist_len, int mask,
                   const char *language, const char *territory,
                   const char *codeset, const char *normalized_codeset,
                   const char *modifier, const char *special,
                   const char *sponsor, const char *revision,
                   const char *filename, int do_allocate)
{
  char *abs_filename;
  struct loaded_l10nfile *last = NULL;
  struct loaded_l10nfile *retval;
  char *cp;
  size_t entries;
  int cnt;

  /* Allocate room for the full file name: */
  abs_filename = (char *) malloc (dirlist_len
				  + strlen (language)
				  + ((mask & TERRITORY) != 0
				     ? strlen (territory) + 1 : 0)
				  + ((mask & XPG_CODESET) != 0
				     ? strlen (codeset) + 1 : 0)
				  + ((mask & XPG_NORM_CODESET) != 0
				     ? strlen (normalized_codeset) + 1 : 0)
				  + (((mask & XPG_MODIFIER) != 0
				      || (mask & CEN_AUDIENCE) != 0)
				     ? strlen (modifier) + 1 : 0)
				  + ((mask & CEN_SPECIAL) != 0
				     ? strlen (special) + 1 : 0)
				  + (((mask & CEN_SPONSOR) != 0
				      || (mask & CEN_REVISION) != 0)
				     ? (1 + ((mask & CEN_SPONSOR) != 0
					     ? strlen (sponsor) + 1 : 0)
					+ ((mask & CEN_REVISION) != 0
					   ? strlen (revision) + 1 : 0)) : 0)
				  + 1 + strlen (filename) + 1);

  if (abs_filename == NULL) {
    return NULL;
  }

  retval = NULL;
  last = NULL;

  /* Construct file name: */
  memcpy(abs_filename, dirlist, dirlist_len);
  __argz_stringify(abs_filename, dirlist_len, ':');
  cp = (abs_filename + (dirlist_len - 1));
  *cp++ = '/';
  cp = stpcpy(cp, language);

  if ((mask & TERRITORY) != 0) {
      *cp++ = '_';
      cp = stpcpy(cp, territory);
  }
  if ((mask & XPG_CODESET) != 0) {
      *cp++ = '.';
      cp = stpcpy(cp, codeset);
  }
  if ((mask & XPG_NORM_CODESET) != 0) {
      *cp++ = '.';
      cp = stpcpy (cp, normalized_codeset);
  }
  if ((mask & (XPG_MODIFIER | CEN_AUDIENCE)) != 0) {
      /* This component can be part of both syntaces but has different
       * leading characters.  For CEN we use `+', else `@'.  */
      *cp++ = (((mask & CEN_AUDIENCE) != 0) ? '+' : '@');
      cp = stpcpy(cp, modifier);
  }
  if ((mask & CEN_SPECIAL) != 0) {
      *cp++ = '+';
      cp = stpcpy(cp, special);
  }
  if ((mask & (CEN_SPONSOR | CEN_REVISION)) != 0) {
      *cp++ = ',';
      if ((mask & CEN_SPONSOR) != 0) {
	cp = stpcpy(cp, sponsor);
      }
      if ((mask & CEN_REVISION) != 0) {
	  *cp++ = '_';
	  cp = stpcpy(cp, revision);
      }
  }

  *cp++ = '/';
  stpcpy(cp, filename);

  /* Look in list of already loaded domains whether it is already
   * available: */
  last = NULL;
  for ((retval = *l10nfile_list); (retval != NULL); (retval = retval->next)) {
    if (retval->filename != NULL) {
	int compare = strcmp(retval->filename, abs_filename);
	if (compare == 0) {
	  /* We found it!  */
	  break;
        }
	if (compare < 0) {
	    /* It is (was?) not in the list: */
	    retval = NULL;
	    break;
        }

	last = retval;
    }
  }

  if ((retval != NULL) || (do_allocate == 0)) {
      free(abs_filename);
      return retval;
  }

  retval = (struct loaded_l10nfile *)
    malloc(sizeof(*retval) + (__argz_count(dirlist, dirlist_len)
                              * (1UL << (size_t)pop(mask))
                              * sizeof(struct loaded_l10nfile *)));
  if (retval == NULL) {
    free(abs_filename);
    return NULL;
  }

  retval->filename = abs_filename;
  retval->decided = ((__argz_count(dirlist, dirlist_len) != 1)
		     || (((mask & XPG_CODESET) != 0)
			 && ((mask & XPG_NORM_CODESET) != 0)));
  retval->data = NULL;

  if (last == NULL) {
      retval->next = *l10nfile_list;
      *l10nfile_list = retval;
  } else {
      retval->next = last->next;
      last->next = retval;
  }

  entries = 0;
  /* If the DIRLIST is a real list the RETVAL entry corresponds not to
     a real file. So we have to use the DIRLIST separation mechanism
     of the inner loop.  */
  cnt = __argz_count (dirlist, dirlist_len) == 1 ? mask - 1 : mask;
  for (; cnt >= 0; --cnt)
    if ((cnt & ~mask) == 0
	&& ((cnt & CEN_SPECIFIC) == 0 || (cnt & XPG_SPECIFIC) == 0)
	&& ((cnt & XPG_CODESET) == 0 || (cnt & XPG_NORM_CODESET) == 0))
      {
	/* Iterate over all elements of the DIRLIST.  */
	char *dir = NULL;

	while ((dir = __argz_next ((char *) dirlist, dirlist_len, dir))
	       != NULL)
	  retval->successor[entries++]
	    = _nl_make_l10nflist (l10nfile_list, dir, strlen (dir) + 1, cnt,
				  language, territory, codeset,
				  normalized_codeset, modifier, special,
				  sponsor, revision, filename, 1);
      }
  retval->successor[entries] = NULL;

  return retval;
}


/* Normalize codeset name. There is no standard for the codeset
   names. Normalization allows the user to use any of the common
   names.  */
const char *_nl_normalize_codeset(const unsigned char *codeset,
                                  size_t name_len)
{
  int len = 0;
  int only_digit = 1;
  char *retval;
  char *wp;
  size_t cnt;

  for ((cnt = 0); (cnt < name_len); ++cnt) {
    if (isalnum(codeset[cnt])) {
	++len;

	if (isalpha(codeset[cnt])) {
	  only_digit = 0;
        }
    }
  }

  retval = (char *)malloc((only_digit ? 3L : 0L) + (size_t)len + 1L);

  if (retval != NULL) {
      if (only_digit) {
	wp = stpcpy(retval, "iso");
      } else {
	wp = retval;
      }

      for ((cnt = 0); (cnt < name_len); ++cnt) {
	if (isalpha(codeset[cnt])) {
	  *wp++ = (char)tolower(codeset[cnt]);
	} else if (isdigit(codeset[cnt])) {
	  *wp++ = (char)codeset[cnt];
        }
      }

      *wp = '\0';
  }

  return (const char *)retval;
}


/* @@ begin of epilog @@ */

/* We do NOT want libintl.a to depend on any other library. So we
   avoid the non-standard function stpcpy. In GNU C Library this
   function is available, though. Also allow the symbol HAVE_STPCPY
   to be defined.  */
#if (!defined(_LIBC) || !_LIBC) && !HAVE_STPCPY
static char *stpcpy(char *dest, const char *src)
{
  while ((*dest++ = *src++) != '\0') {
    /* Do nothing. */ ;
  }
  return (dest - 1);
}
#endif /* !_LIBC && !HAVE_STPCPY */

/* EOF */
