/* bucomm.h -- binutils common include file.
   Copyright 1991, 1992, 1993, 1994, 1995, 1996, 1997, 1998, 1999, 2000,
   2001, 2002, 2003 Free Software Foundation, Inc.

   This file is part of GNU Binutils.

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

#ifndef _BUCOMM_H
#define _BUCOMM_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "bucomm.h expects config.h to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_CONFIG_H */

#include "ansidecl.h"
#ifdef HAVE_STDIO_H
# include <stdio.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "bucomm.h expects <stdio.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_STDIO_H */
#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "bucomm.h expects <sys/types.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_SYS_TYPES_H */

#include "bin-bugs.h"
#ifndef _BIN_SYSDEP_H
# include "sysdep.h"
#endif /* !_BIN_SYSDEP_H */

#ifdef HAVE_STDARG_H
# include <stdarg.h>
#else
# ifdef HAVE_VARARGS_H
#  include <varargs.h>
# else
#  if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#   warning "bucomm.h expects either <stdarg.h> or <varargs.h> to be included."
#  endif /* __GNUC__ && !__STRICT_ANSI__ */
# endif /* HAVE_VARARGS_H */
#endif /* HAVE_STDARG_H */

#ifdef USE_BINARY_FOPEN
# include "fopen-bin.h"
#else
# include "fopen-same.h"
#endif /* USE_BINARY_FOPEN */

#ifdef HAVE_ERRNO_H
# include <errno.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "bucomm.h expects <errno.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_ERRNO_H */
#ifndef errno
extern int errno;
#endif /* !errno */

#ifdef HAVE_UNISTD_H
# include <unistd.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "bucomm.h expects <unistd.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_UNISTD_H */

#ifdef HAVE_STRING_H
# include <string.h>
#else
# ifdef HAVE_STRINGS_H
#  include <strings.h>
# else
extern char *strchr();
extern char *strrchr();
# endif /* HAVE_STRINGS_H */
#endif /* HAVE_STRING_H */

#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "bucomm.h expects <stdlib.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_STDLIB_H */

#ifdef HAVE_MALLOC_H
# include <malloc.h>
#else
# ifdef HAVE_MALLOC_MALLOC_H
#  include <malloc/malloc.h>
# else
#  if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#   warning "bucomm.h expects a malloc-related header to be included."
#  endif /* __GNUC__ && !__STRICT_ANSI__ */
# endif /* HAVE_MALLOC_MALLOC_H */
#endif /* HAVE_MALLOC_H */

#ifdef HAVE_FCNTL_H
# include <fcntl.h>
#else
# ifdef HAVE_SYS_FILE_H
#  include <sys/file.h>
# else
#  if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#   warning "bucomm.h expects either <fcntl.h> or <sys/file.h> to be included."
#  endif /* __GNUC__ && !__STRICT_ANSI__ */
# endif /* HAVE_SYS_FILE_H */
#endif /* HAVE_FCNTL_H */

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

#if defined(__GNUC__) && !defined(C_ALLOCA)
# undef alloca
# define alloca __builtin_alloca
#else
# if defined(HAVE_ALLOCA_H) && !defined(C_ALLOCA)
#  include <alloca.h>
# else
#  ifndef alloca /* predefined by HP cc +Olibcalls */
#   if !defined (__STDC__) && !defined (__hpux)
char *alloca ();
#   else
void *alloca ();
#   endif /* !__STDC__, !__hpux */
#  endif /* !alloca */
# endif /* HAVE_ALLOCA_H && !C_ALLOCA */
#endif /* __GNUC__ && !C_ALLOCA */

#ifdef HAVE_LOCALE_H
# include <locale.h>
#else
# ifndef NOT_PRINTING_A_WARNING_HERE
#  define NOT_PRINTING_A_WARNING_HERE 1 /* actually printing a warning causes too many, I think... */
# endif /* !NOT_PRINTING_A_WARNING_HERE */
#endif /* HAVE_LOCALE_H */

#ifdef ENABLE_NLS
# ifdef HAVE_LIBINTL_H
#  include <libintl.h>
#  define _(String) gettext (String)
#  ifdef gettext_noop
#   define N_(String) gettext_noop (String)
#  else
#   define N_(String) (String)
#  endif /* gettext_noop */
#  ifndef gettext
#   define gettext(Msgid) (Msgid)
#  endif /* !gettext */
#  ifndef dgettext
#   define dgettext(Domainname, Msgid) (Msgid)
#  endif /* !dgettext */
#  ifndef dcgettext
#   define dcgettext(Domainname, Msgid, Category) (Msgid)
#  endif /* !dcgettext */
#  ifndef textdomain
#   define textdomain(Domainname) while (0) /* nothing */
#  endif /* !textdomain */
#  ifndef bindtextdomain
#   define bindtextdomain(Domainname, Dirname) while (0) /* nothing */
#  endif /* !bindtextdomain */
# else
#  ifdef HAVE_LIBGETTEXT_H
#   include <libgettext.h>
#  else
#   ifdef HAVE_GETTEXT_H
#    include <gettext.h>
#   else
#    if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#     warning "bucomm.h expects <libintl.h> (or another similar header) to be included."
#    endif /* __GNUC__ && !__STRICT_ANSI__ */
#   endif /* HAVE_GETTEXT_H */
#  endif /* HAVE_LIBGETTEXT_H */
# endif /* HAVE_LIBINTL_H */
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
#ifndef BUFSIZE
# define BUFSIZE 8192
#endif /* !BUFSIZE */

/* bucomm.c */

/* Return the filename in a static buffer: */
const char *bfd_get_archive_filename(bfd *) ATTRIBUTE_W_U_R;

void bfd_nonfatal(const char *);

void bfd_fatal(const char *) ATTRIBUTE_NORETURN;

void report(const char *, va_list) ATTRIBUTE_PRINTF(1,0);

void fatal(const char *, ...) ATTRIBUTE_PRINTF_1 ATTRIBUTE_NORETURN;

void non_fatal(const char *, ...) ATTRIBUTE_PRINTF_1;

void set_default_bfd_target(void);

void list_matching_formats(char **);

void list_supported_targets(const char *, FILE *);

void list_supported_architectures(const char *, FILE *);

int display_info(void);

void print_arelt_descr(FILE *, bfd *, bfd_boolean);

char *make_tempname(char *) ATTRIBUTE_W_U_R;

bfd_vma parse_vma(const char *, const char *);

off_t get_file_size(const char *);

extern const char *program_name;

/* filemode.c */
void mode_string(unsigned long, char *);

/* version.c */
extern void print_version(const char *);

/* rename.c */
extern void set_times(const char *, const struct stat *);

extern int smart_rename(const char *, const char *, int);

/* libiberty prototypes: */
void *xmalloc(size_t) ATTRIBUTE_MALLOC ATTRIBUTE_W_U_R;

void *xrealloc(void *, size_t) ATTRIBUTE_W_U_R;

/* just in case: */
#if defined(HAVE_MKSTEMP) && (!defined(HAVE_DECL_MKSTEMP) || !HAVE_DECL_MKSTEMP)
int mkstemp(char *) ATTRIBUTE_W_U_R;
#endif /* HAVE_MKSTEMP && !HAVE_DECL_MKSTEMP */

#endif /* _BUCOMM_H */

/* EOF */
