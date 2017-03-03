/* sysdep.h -- handle host dependencies for binutils
   Copyright 1991, 1992, 1993, 1994, 1995, 1996, 1997, 1998, 1999, 2000,
   2001, 2002, 2003, 2005, 2006, 2007, 2008, 2009, 2012
   Free Software Foundation, Inc.

   This file is part of GNU Binutils.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St., 5th Floor, Boston, MA 02110-1301, USA */

#ifndef _BIN_SYSDEP_H
#define _BIN_SYSDEP_H

#include "alloca-conf.h"
#include "ansidecl.h"
#include <stdio.h>
#include <sys/types.h>

#include "bfdver.h"

#include <stdarg.h>

#ifdef USE_BINARY_FOPEN
# include "fopen-bin.h"
#else
# include "fopen-same.h"
#endif /* USE_BINARY_FOPEN */

#include <errno.h>
#ifndef errno
extern int errno;
#endif /* !errno */

#ifdef HAVE_UNISTD_H
# include <unistd.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
 #  warning "sysdep.h expects <unistd.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_UNISTD_H */

#ifdef STRING_WITH_STRINGS
# include <string.h>
# include <strings.h>
#else
# ifdef HAVE_STRING_H
#  include <string.h>
# else
#  ifdef HAVE_STRINGS_H
#   include <strings.h>
#  else
extern char *strchr();
extern char *strrchr();
#  endif /* HAVE_STRINGS_H */
# endif /* HAVE_STRING_H */
#endif /* STRING_WITH_STRINGS */

#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
 #  warning "sysdep.h expects <stdlib.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_STDLIB_H */

#ifdef HAVE_FCNTL_H
# include <fcntl.h>
#else
# ifdef HAVE_SYS_FILE_H
#  include <sys/file.h>
# else
#  if defined(__GNUC__) && !defined(__STRICT_ANSI__)
 #   warning "sysdep.h expects a header for file-related functions to be included."
#  endif /* __GNUC__ && !__STRICT_ANSI__ */
# endif /* HAVE_SYS_FILE_H */
#endif /* HAVE_FCNTL_H */

#ifdef HAVE_SYS_STAT_H
# include <sys/stat.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
 #  warning "sysdep.h expects <sys/stat.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_SYS_STAT_H */

/* Where is this next one supposed to be? Usually in "../include": */
#include "binary-io.h"

#if !HAVE_DECL_STPCPY
extern char *stpcpy(char *, const char *);
#endif /* !HAVE_DECL_STPCPY */

#if !HAVE_DECL_STRSTR
extern char *strstr();
#endif /* !HAVE_DECL_STRSTR */

#ifdef HAVE_SBRK
# if !HAVE_DECL_SBRK
extern char *sbrk();
# endif /* !HAVE_DECL_SBRK */
#endif /* HAVE_SBRK */

#if !HAVE_DECL_GETENV
extern char *getenv();
#endif /* !HAVE_DECL_GETENV */

#if !HAVE_DECL_ENVIRON
extern char **environ;
#endif /* !HAVE_DECL_ENVIRON */

#if !HAVE_DECL_FPRINTF
extern int fprintf(FILE *, const char *, ...);
#endif /* !HAVE_DECL_FPRINTF */

#if !HAVE_DECL_SNPRINTF
extern int snprintf(char *, size_t, const char *, ...);
#endif /* !HAVE_DECL_SNPRINTF */

#if !HAVE_DECL_VSNPRINTF
extern int vsnprintf(char *, size_t, const char *, va_list);
#endif /* HAVE_DECL_VSNPRINTF */

#ifndef O_RDONLY
# define O_RDONLY 0
#endif /* !O_RDONLY */

#ifndef O_RDWR
# define O_RDWR 2
#endif /* !O_RDWR */

#ifndef SEEK_SET
# define SEEK_SET 0
#endif /* !SEEK_SET */
#ifndef SEEK_CUR
# define SEEK_CUR 1
#endif /* !SEEK_CUR */
#ifndef SEEK_END
# define SEEK_END 2
#endif /* !SEEK_END */

#ifdef HAVE_LOCALE_H
# ifndef ENABLE_NLS
/* The Solaris version of locale.h always includes libintl.h.  If we have
 * been configured with --disable-nls then ENABLE_NLS will not be defined
 * and the dummy definitions of bindtextdomain (et al) below will conflict
 * with the defintions in libintl.h. So we define these values to prevent
 * the bogus inclusion of libintl.h.  */
#  define _LIBINTL_H
#  define _LIBGETTEXT_H
# endif /* !ENABLE_NLS */
# include <locale.h>
#else
# ifdef ENABLE_NLS
#  if defined(__GNUC__) && !defined(__STRICT_ANSI__)
 #   warning "sysdep.h expects <locale.h> to be included."
#  endif /* __GNUC__ && !__STRICT_ANSI__ */
# endif /* ENABLE_NLS */
#endif /* HAVE_LOCALE_H */

#ifdef ENABLE_NLS
# ifdef HAVE_LIBINTL_H
#  include <libintl.h>
# else
#  if defined(__GNUC__) && !defined(__STRICT_ANSI__)
 #   warning "sysdep.h expects <libintl.h> to be included."
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
# define dgettext(Domainname, Msgid) (Msgid)
# define dcgettext(Domainname, Msgid, Category) (Msgid)
# define textdomain(Domainname) while (0) /* nothing */
# define bindtextdomain(Domainname, Dirname) while (0) /* nothing */
# define _(String) (String)
# define N_(String) (String)
#endif /* ENABLE_NLS */

/* Used by ar.c and objcopy.c.  */
#ifdef BUFSIZE
# undef BUFSIZE
#endif /* !BUFSIZE */
#define BUFSIZE 8192

/* For PATH_MAX.  */
#ifdef HAVE_LIMITS_H
# include <limits.h>
#else
# ifndef PATH_MAX
#  if defined(__GNUC__) && !defined(__STRICT_ANSI__)
 #   warning "sysdep.h expects <limits.h> to be included for PATH_MAX"
#  endif /* __GNUC__ && !__STRICT_ANSI__ */
# endif /* !PATH_MAX */
#endif /* HAVE_LIMITS_H */

#ifndef PATH_MAX
/* For MAXPATHLEN: */
# ifdef HAVE_SYS_PARAM_H
#  include <sys/param.h>
# else
#  ifndef MAXPATHLEN
#   if defined(__GNUC__) && !defined(__STRICT_ANSI__)
 #    warning "sysdep.h wants to include <sys/param.h> for MAXPATHLEN"
#   endif /* __GNUC__ && !__STRICT_ANSI__ */
#  endif /* !MAXPATHLEN */
# endif /* HAVE_SYS_PARAM_H */
# ifndef PATH_MAX
#  ifdef MAXPATHLEN
#   define PATH_MAX MAXPATHLEN
#  else
#   define PATH_MAX 1024
#  endif /* MAXPATHLEN */
# endif /* !PATH_MAX */
#endif /* !PATH_MAX */

/* poison some unwanted functions: */
#if (defined(__GNUC__) && defined(__GNUC_MINOR__) && (__GNUC__ >= 3)) && \
    !defined(NO_POISON) && !defined(FLEX_SCANNER)
# ifdef vsprintf
#  undef vsprintf
# endif /* vsprintf */
/* libiberty provides replacements: */
# pragma GCC poison strdup strndup memdup strerror vsprintf atexit exit
# if defined(HAVE_STRLCPY) && defined(PREFER_BSDISMS)
#  pragma GCC poison strcpy
# endif /* HAVE_STRLCPY && PREFER_BSDISMS */
# if defined(HAVE_STRLCAT) && defined(PREFER_BSDISMS)
#  pragma GCC poison strcat
# endif /* HAVE_STRLCAT && PREFER_BSDISMS */
# if defined(HAVE_FGETS)
#  pragma GCC poison gets
# endif /* HAVE_FGETS */
# if defined(HAVE_STRTOK_R) && defined(_REENTRANT)
#  pragma GCC poison strtok
# endif /* HAVE_STRTOK_R && _REENTRANT */
#endif /* gcc3+ && !NO_POISON && !FLEX_SCANNER */

#endif /* _BIN_SYSDEP_H */

/* EOF */
