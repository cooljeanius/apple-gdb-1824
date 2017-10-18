/* sysdep.h -- handle host dependencies for the BFD library.
 * Copyright 1995, 1996, 1997, 1998, 1999, 2000, 2001
 * Free Software Foundation, Inc.
 * Written by Cygnus Support.  */
/*
This file is part of BFD, the Binary File Descriptor library.

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
Foundation, Inc., 51 Franklin St., 5th Floor, Boston, MA 02110-1301, USA */

#ifndef BFD_SYSDEP_H
#define BFD_SYSDEP_H

#if defined(PACKAGE) && defined(PACKAGE_BUGREPORT) && \
    defined(PACKAGE_NAME) && defined(PACKAGE_STRING) && \
    defined(PACKAGE_TARNAME) && defined(PACKAGE_URL) && \
    defined(PACKAGE_VERSION) && defined(VERSION)
# if defined(__GNUC__) && !defined(__STRICT_ANSI__) && \
     !defined(__BFD_MACH_O_C__)
 #  warning "sysdep.h should be included in lieu of config.h"
# endif /* __GNUC__ && !__STRICT_ANSI__ && !__BFD_MACH_O_C__ */
#endif /* PACKAGE and similar */

#ifdef HAVE_CONFIG_H
# if !defined(__CONFIG_H__) || !defined(__BFD_CONFIG_H__)
#  include "config.h"
# else
#  ifdef _INCLUDING_CONFIG_H
#   if defined(__GNUC__) && !defined(__STRICT_ANSI__)
 #    warning "the config.h from ../intl might have already been included."
#   endif /* __GNUC__ && !__STRICT_ANSI__ */
#  else
#   ifdef _APPLE_GDB_TOP_SRC_CONFIG_H
#    if defined(__GNUC__) && !defined(__STRICT_ANSI__)
 #     warning "the config.h from our parent directory might have already been included."
#    endif /* __GNUC__ && !__STRICT_ANSI__ */
#   else
#    if defined(__GNUC__) && !defined(__STRICT_ANSI__) && !defined(__BFD_CONFIG_H__)
 #     warning "config.h might have already been included and it might have been a different one."
#    endif /* __GNUC__ && !__STRICT_ANSI__ && !__BFD_CONFIG_H__ */
#   endif /* _APPLE_GDB_TOP_SRC_CONFIG_H */
#  endif /* _INCLUDING_CONFIG_H */
# endif /* !__CONFIG_H__ || !__BFD_CONFIG_H__ */
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
 #  warning sysdep.h expects "config.h" to be included.
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_CONFIG_H */

#include "ansidecl.h"

#ifdef HAVE_STDDEF_H
# include <stddef.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
 #  warning "sysdep.h expects <stddef.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_STDDEF_H */

#ifdef HAVE_STDIO_H
# include <stdio.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
 #  warning "sysdep.h expects <stdio.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_STDIO_H */
#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
 #  warning "sysdep.h expects <sys/types.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_SYS_TYPES_H */
#ifdef HAVE_SYS_STAT_H
# include <sys/stat.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
 #  warning "sysdep.h expects <sys/stat.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_SYS_STAT_H */

#ifdef HAVE_ERRNO_H
# include <errno.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
 #  warning "sysdep.h expects <errno.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_ERRNO_H */
#if !(defined(errno) || defined(_MSC_VER) && defined(_INC_ERRNO))
extern int errno;
#endif /* !(errno || _MSC_VER && _INC_ERRNO) */

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

#ifdef HAVE_MALLOC_H
# include <malloc.h>
#else
# ifdef HAVE_MALLOC_MALLOC_H
#  include <malloc/malloc.h>
# else
#  if defined(__GNUC__) && !defined(__STRICT_ANSI__)
 #   warning "sysdep.h expects a malloc-related header to be included."
#  endif /* __GNUC__ && !__STRICT_ANSI__ */
# endif /* HAVE_MALLOC_MALLOC_H */
#endif /* HAVE_MALLOC_H */

#ifdef TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# ifdef HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  ifdef HAVE_TIME_H
#   include <time.h>
#  else
#   if defined(__GNUC__) && !defined(__STRICT_ANSI__)
 #     warning "sysdep.h expects a time-related header to be included."
#   endif /* __GNUC__ && !__STRICT_ANSI__ */
#  endif /* HAVE_TIME_H */
# endif /* HAVE_SYS_TIME_H */
#endif /* TIME_WITH_SYS_TIME */

#ifdef HAVE_UNISTD_H
# include <unistd.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
 #  warning "sysdep.h expects <unistd.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_UNISTD_H */

#ifdef HAVE_SYS_RESOURCE_H
# include <sys/resource.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
 #  warning "sysdep.h expects <sys/resource.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_SYS_RESOURCE_H */

#ifdef USE_BINARY_FOPEN
# include "fopen-bin.h"
#else
# include "fopen-same.h"
#endif /* USE_BINARY_FOPEN */

#ifdef HAVE_FCNTL_H
# include <fcntl.h>
#else
# ifdef HAVE_SYS_FILE_H
#  include <sys/file.h>
# else
#  if defined(__GNUC__) && !defined(__STRICT_ANSI__)
 #   warning "sysdep.h expects a file-related header to be included."
#  endif /* __GNUC__ && !__STRICT_ANSI__ */
# endif /* HAVE_SYS_FILE_H */
#endif /* HAVE_FCNTL_H */

#ifndef O_RDONLY
# define O_RDONLY 0
#endif /* !O_RDONLY */
#ifndef O_WRONLY
# define O_WRONLY 1
#endif /* !O_WRONLY */
#ifndef O_RDWR
# define O_RDWR 2
#endif /* !O_RDWR */
#ifndef O_ACCMODE
# define O_ACCMODE (O_RDONLY | O_WRONLY | O_RDWR)
#endif /* !O_ACCMODE */

#ifndef SEEK_SET
# define SEEK_SET 0
#endif /* !SEEK_SET */
#ifndef SEEK_CUR
# define SEEK_CUR 1
#endif /* !SEEK_CUR */

#include "filenames.h"

#if !HAVE_DECL_FFS
extern int ffs(int);
#endif /* !HAVE_DECL_FFS */

#if !HAVE_DECL_FREE
extern void free();
#endif /* !HAVE_DECL_FREE */

#if !HAVE_DECL_GETENV
extern char *getenv();
#endif /* !HAVE_DECL_GETENV */

#if !HAVE_DECL_MALLOC
extern PTR malloc();
#endif /* !HAVE_DECL_MALLOC */

#if !HAVE_DECL_REALLOC
extern PTR realloc();
#endif /* !HAVE_DECL_REALLOC */

#if !HAVE_DECL_STPCPY
extern char *stpcpy(char *__dest, const char *__src);
#endif /* !HAVE_DECL_STPCPY */

#if !HAVE_DECL_STRSTR
extern char *strstr();
#endif /* !HAVE_DECL_STRSTR */

#ifdef HAVE_FTELLO
# if !HAVE_DECL_FTELLO
extern off_t ftello(FILE *stream);
# endif /* !HAVE_DECL_FTELLO */
#endif /* HAVE_FTELLO */

#ifdef HAVE_FTELLO64
# if !HAVE_DECL_FTELLO64
extern off64_t ftello64(FILE *stream);
# endif /* !HAVE_DECL_FTELLO64 */
#endif /* HAVE_FTELLO64 */

#ifdef HAVE_FSEEKO
# if !HAVE_DECL_FSEEKO
extern int fseeko(FILE *stream, off_t offset, int whence);
# endif /* !HAVE_DECL_FSEEKO */
#endif /* HAVE_FSEEKO */

#ifdef HAVE_FSEEKO64
# if !HAVE_DECL_FSEEKO64
extern int fseeko64(FILE *stream, off64_t offset, int whence);
# endif /* !HAVE_DECL_FSEEKO64 */
#endif /* HAVE_FSEEKO64 */

#if !HAVE_DECL_STRNLEN
size_t strnlen(const char *, size_t);
#endif /* !HAVE_DECL_STRNLEN */

/* Define offsetof for those systems which lack it: */
#ifndef offsetof
# define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif /* !offsetof */

#ifdef ENABLE_NLS
# ifdef HAVE_LIBINTL_H
#  include <libintl.h>
# else
#  if defined(__GNUC__) && !defined(__STRICT_ANSI__)
 #   warning "sysdep.h expects <libintl.h> to be included."
#  endif /* __GNUC__ && !__STRICT_ANSI__ */
# endif /* HAVE_LIBINTL_H */
/* Note the use of dgetext() and PACKAGE here, rather than gettext().
 *
 * This is because the code in this directory is used to build a library which
 * will be linked with code in other directories to form programs.  We want to
 * maintain a seperate translation file for this directory however, rather
 * than being forced to merge it with that of any program linked to libbfd.
 * This is a library, so it cannot depend on the catalog currently loaded.
 *
 * In order to do this, we have to make sure that when we extract messages we
 * use the OPCODES domain rather than the domain of the program that included
 * the bfd library, (eg OBJDUMP).  Hence we use dgettext (PACKAGE, String)
 * and define PACKAGE to be 'bfd'.  (See the code in configure).  */
# define _(String) dgettext (PACKAGE, String)
# ifdef gettext_noop
#  define N_(String) gettext_noop (String)
# else
#  define N_(String) (String)
# endif /* gettext_noop */
# ifndef gettext
#  define gettext(Msgid) (Msgid)
# endif /* !gettext */
# ifndef dgettext
#  define dgettext(Domainname, Msgid) (Msgid)
# endif /* !dgettext */
# ifndef dcgettext
#  define dcgettext(Domainname, Msgid, Category) (Msgid)
# endif /* !dcgettext */
# ifndef textdomain
#  define textdomain(Domainname) while (0) /* nothing */
# endif /* !textdomain */
# ifndef bindtextdomain
#  define bindtextdomain(Domainname, Dirname) while (0) /* nothing */
# endif /* !bindtextdomain */
#else
# define gettext(Msgid) (Msgid)
# define dgettext(Domainname, Msgid) (Msgid)
# define dcgettext(Domainname, Msgid, Category) (Msgid)
# define textdomain(Domainname) while (0) /* nothing */
# define bindtextdomain(Domainname, Dirname) while (0) /* nothing */
# define _(String) (String)
# define N_(String) (String)
#endif /* ENABLE_NLS */

/* poison some unwanted functions: */
#if (defined(__GNUC__) && defined(__GNUC_MINOR__) && (__GNUC__ >= 3)) && \
    !defined(NO_POISON) && !defined(FLEX_SCANNER)
/* libiberty provides replacements: */
# pragma GCC poison strdup memdup strerror atexit exit
# ifndef strndup
#  pragma GCC poison strndup
# endif /* !strndup */
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
# pragma GCC poison setlinebuf bcmp bzero strsave strnicmp extra_func_to_poison
#endif /* gcc3+ && !NO_POISON && !FLEX_SCANNER */

#endif /* ! defined (BFD_SYSDEP_H) */

/* EOF */
