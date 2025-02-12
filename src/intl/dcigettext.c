/* dcigettext.c
   Implementation of the internal dcigettext function.
   Copyright (C) 1995-1999, 2000-2003 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street - 5th Floor, Boston, MA 02110-1301,
   USA.  */

/* Tell glibc's <string.h> to provide a prototype for mempcpy().
   This must come before <config.h> because <config.h> may include
   <features.h>, and once <features.h> is included, it is too late. */
#ifndef _GNU_SOURCE
# define _GNU_SOURCE 1
#endif /* !_GNU_SOURCE */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include <sys/types.h>

#ifndef PARAMS
# if __STDC__ || defined __GNUC__ || defined __SUNPRO_C || defined __cplusplus || __PROTOTYPES
#  define PARAMS(args) args
# else
#  define PARAMS(args) ()
# endif /* __STDC__ */
#endif /* !PARAMS */

#ifdef __GNUC__
# define alloca __builtin_alloca
# ifndef HAVE_ALLOCA
#  define HAVE_ALLOCA 1
# endif /* !HAVE_ALLOCA */
#else
# ifdef _MSC_VER
#  include <malloc.h>
#  define alloca _alloca
# else
#  if defined HAVE_ALLOCA_H || defined _LIBC
#   include <alloca.h>
#  else
#   ifdef _AIX
 #pragma alloca
#   else
#    ifndef alloca
char *alloca PARAMS((size_t size));
#    endif /* !alloca */
#   endif /* _AIX */
#  endif /* HAVE_ALLOCA_H || _LIBC */
# endif /* _MSC_VER */
#endif /* __GNUC__ */

#include <errno.h>
#ifndef errno
extern int errno;
#endif /* !errno */
#ifndef __set_errno
# define __set_errno(val) errno = (val)
#endif /* !__set_errno */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#if defined HAVE_UNISTD_H || defined _LIBC
# include <unistd.h>
#endif /* HAVE_UNISTD_H || _LIBC */

#include <locale.h>

#ifdef _LIBC
  /* Guess whether integer division by zero raises signal SIGFPE.
   * Set to 1 only if you know for sure. In case of doubt, set to 0: */
# if defined(__alpha__) || defined(__arm__) || defined(__i386__) \
     || defined(__m68k__) || defined(__s390__)
#  define INTDIV0_RAISES_SIGFPE 1
# else
#  define INTDIV0_RAISES_SIGFPE 0
# endif
#endif
#if (!defined(INTDIV0_RAISES_SIGFPE) || !INTDIV0_RAISES_SIGFPE)
# include <signal.h>
#endif /* !INTDIV0_RAISES_SIGFPE */

#if defined HAVE_SYS_PARAM_H || defined _LIBC
# include <sys/param.h>
#endif /* HAVE_SYS_PARAM_H || _LIBC */

#ifdef HAVE_ICONV_H
# include <iconv.h>
#else
# ifdef HAVE_PHP_ICONV_H
#  include <php_iconv.h>
# else
#  ifdef HAVE_UNICONV_H
#   include <uniconv.h>
#  else
#   if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#    warning "dcigettext.c expects an iconv-related header to be included."
#   endif /* __GNUC__ && !__STRICT_ANSI__ */
#  endif /* HAVE_UNICONV_H */
# endif /* HAVE_PHP_ICONV_H */
#endif /* HAVE_ICONV_H */

#include "gettextP.h"
#include "plural-exp.h"
#ifdef _LIBC
# include <libintl.h>
#else
# include "libgnuintl.h"
#endif /* _LIBC */
#include "hash-string.h"

/* Thread safetyness.  */
#ifdef _LIBC
# include <bits/libc-lock.h>
#else
/* Provide dummy implementation if this is outside glibc.  */
# define __libc_lock_define_initialized(CLASS, NAME)
# define __libc_lock_lock(NAME)
# define __libc_lock_unlock(NAME)
# define __libc_rwlock_define_initialized(CLASS, NAME)
# define __libc_rwlock_rdlock(NAME)
# define __libc_rwlock_unlock(NAME)
#endif /* _LIBC */

/* Alignment of types.  */
#if defined __GNUC__ && (__GNUC__ >= 2)
# define alignof(TYPE) __alignof__(TYPE)
#else
# define alignof(TYPE) \
    ((int) &((struct { char dummy1; TYPE dummy2; } *) 0)->dummy2)
#endif /* gcc 2+ */

/* The internal variables in the standalone libintl.a must have different
   names than the internal variables in GNU libc, otherwise programs
   using libintl.a cannot be linked statically.  */
#if !defined _LIBC
# define _nl_default_default_domain libintl_nl_default_default_domain
# define _nl_current_default_domain libintl_nl_current_default_domain
# define _nl_default_dirname libintl_nl_default_dirname
# define _nl_domain_bindings libintl_nl_domain_bindings
#endif /* !_LIBC */

/* Some compilers, like SunOS4 cc, do NOT have offsetof in <stddef.h>: */
#ifndef offsetof
# define offsetof(type,ident) ((size_t)&(((type*)0)->ident))
#endif /* !offsetof */

/* @@ end of prolog @@ */

#ifdef _LIBC
/* Rename the non ANSI C functions. This is required by the standard
 * because some ANSI C functions will require linking with this object
 * file and the name space must not be polluted: */
# define getcwd __getcwd
# ifndef stpcpy
#  define stpcpy __stpcpy
# endif /* !stpcpy */
# define tfind __tfind
#else
# if !defined HAVE_GETCWD
char *getwd PARAMS((char *buffer));
#  define getcwd(buf, max) getwd(buf)
# else
#  ifdef HAVE_IO_H
#   include <io.h>
#  else
char *getcwd PARAMS((char *buffer, size_t buflen));
#  endif /* HAVE_IO_H */
# endif /* !HAVE_GETCWD */
# if !defined(HAVE_STPCPY) || !HAVE_STPCPY || !defined(HAVE_DECL_STPCPY) || !HAVE_DECL_STPCPY
static char *stpcpy PARAMS((char *dest, const char *src));
# endif /* !HAVE_STPCPY */
# if !defined(HAVE_MEMPCPY) || !HAVE_MEMPCPY || !defined(HAVE_DECL_MEMPCPY) || !HAVE_DECL_MEMPCPY
#  if !defined(__STRICT_ANSI__)
static
#  endif /* !__STRICT_ANSI__ */
void *mempcpy PARAMS((void *dest, const void *src, size_t n));
# endif /* !HAVE_MEMPCPY */
#endif /* _LIBC */

/* Amount to increase buffer size by in each try: */
#define PATH_INCR 32

/* The following is from pathmax.h.  */
/* Non-POSIX BSD systems might have gcc's limits.h, which does NOT define
 * PATH_MAX but might cause redefinition warnings when sys/param.h is
 * later included (as on MORE/BSD 4.3): */
#if defined _POSIX_VERSION || (defined HAVE_LIMITS_H && !defined __GNUC__)
# include <limits.h>
#endif /* _POSIX_VERSION || (HAVE_LIMITS_H && !__GNUC__) */

#ifndef _POSIX_PATH_MAX
# define _POSIX_PATH_MAX 255
#endif /* !_POSIX_PATH_MAX */

#if !defined PATH_MAX && defined _PC_PATH_MAX
# define PATH_MAX ((pathconf("/", _PC_PATH_MAX) < 1) ? 1024 : pathconf("/", _PC_PATH_MAX))
#endif /* !PATH_MAX && _PC_PATH_MAX */

/* Do NOT include sys/param.h if it already has been: */
#if defined HAVE_SYS_PARAM_H && !defined PATH_MAX && !defined MAXPATHLEN
# include <sys/param.h>
#endif /* HAVE_SYS_PARAM_H && !PATH_MAX && !MAXPATHLEN */

#if !defined PATH_MAX && defined MAXPATHLEN
# define PATH_MAX MAXPATHLEN
#endif /* !PATH_MAX && MAXPATHLEN */

#ifndef PATH_MAX
# define PATH_MAX _POSIX_PATH_MAX
#endif /* !PATH_MAX */

/* Pathname support.
 * ISSLASH(C)           tests whether C is a directory separator character.
 * IS_ABSOLUTE_PATH(P)  tests whether P is an absolute path. If it is not,
 *                      it may be concatenated to a directory pathname.
 * IS_PATH_WITH_DIR(P)  tests whether P contains a directory specification.
 */
#if defined(_WIN32) || defined(__WIN32__) || defined(__EMX__) || \
    defined(__DJGPP__)
  /* Win32, OS/2, DOS */
# define ISSLASH(C) ((C) == '/' || (C) == '\\')
# define HAS_DEVICE(P) \
    ((((P)[0] >= 'A' && (P)[0] <= 'Z') || ((P)[0] >= 'a' && (P)[0] <= 'z')) \
     && (P)[1] == ':')
# define IS_ABSOLUTE_PATH(P) (ISSLASH((P)[0]) || HAS_DEVICE(P))
# define IS_PATH_WITH_DIR(P) \
    ((strchr(P, '/') != NULL) || (strchr(P, '\\') != NULL) || HAS_DEVICE(P))
#else
  /* Unix: */
# define ISSLASH(C) ((C) == '/')
# define IS_ABSOLUTE_PATH(P) ISSLASH((P)[0])
# define IS_PATH_WITH_DIR(P) (strchr(P, '/') != NULL)
#endif /* Windows vs. not */

#ifndef ZERO
# if defined(__STRICT_ANSI__)
/* '-pedantic' plus '-ansi' warns agains zero-length arrays: */
#  define ZERO 1
# else
#  define ZERO 0
# endif /* __STRICT_ANSI__ */
#endif /* !ZERO */

/* This is the type used for the search tree where known translations
 * are stored: */
struct known_translation_t
{
  /* Domain in which to search: */
  char *domainname;

  /* The category: */
  int category;

  /* State of the catalog counter at the point the string was found: */
  int counter;

  /* Catalog where the string was found: */
  struct loaded_l10nfile *domain;

  /* And finally the translation: */
  const char *translation;
  size_t translation_length;

  /* Pointer to the string in question: */
  char msgid[ZERO];
};

/* Root of the search tree with known translations.  We can use this
   only if the system provides the `tsearch' function family.  */
#if defined HAVE_TSEARCH || defined _LIBC
# include <search.h>

static void *root;

# ifdef _LIBC
#  define tsearch __tsearch
# endif /* _LIBC */

/* Function to compare two entries in the table of known translations: */
static int transcmp PARAMS((const void *p1, const void *p2));
static int transcmp(const void *p1, const void *p2)
{
  const struct known_translation_t *s1;
  const struct known_translation_t *s2;
  int result;

  s1 = (const struct known_translation_t *)p1;
  s2 = (const struct known_translation_t *)p2;

  result = strcmp(s1->msgid, s2->msgid);
  if (result == 0) {
      result = strcmp(s1->domainname, s2->domainname);
      if (result == 0) {
	/* We compare the category last (though this is the cheapest
	 * operation) since it is hopefully always the same (namely
	 * LC_MESSAGES): */
	result = (s1->category - s2->category);
      }
  }

  return result;
}
#endif /* HAVE_TSEARCH || _LIBC */

#ifndef INTVARDEF
# define INTVARDEF(name)
#endif /* !INTVARDEF */
#ifndef INTUSE
# define INTUSE(name) name
#endif /* !INTUSE */

/* Name of the default domain used for gettext(3) prior any call to
 * textdomain(3). The default value for this is "messages": */
const char _nl_default_default_domain[] attribute_hidden = "messages";

/* Value used as the default domain for gettext(3): */
const char *_nl_current_default_domain attribute_hidden
     = _nl_default_default_domain;

/* Contains the default location of the message catalogs: */
#if defined __EMX__
extern const char _nl_default_dirname[];
#else
const char _nl_default_dirname[] = LOCALEDIR;
INTVARDEF(_nl_default_dirname)
#endif /* __EMX__ */

/* List with bindings of specific domains created by bindtextdomain()
 * calls: */
struct binding *_nl_domain_bindings;

/* Prototypes for local functions: */
static char *plural_lookup PARAMS((struct loaded_l10nfile *domain,
                                   unsigned long int n,
                                   const char *translation,
                                   size_t translation_len))
     internal_function;
static const char *guess_category_value PARAMS((int category,
                                                const char *categoryname))
     internal_function;
#ifdef _LIBC
# include "../locale/localeinfo.h"
# define category_to_name(category) _nl_category_names[category]
#else
static const char *category_to_name PARAMS((int category)) internal_function;
#endif /* _LIBC */


/* For those loosing systems which do NOT have `alloca' we have to add
 * some additional code emulating it: */
#ifdef HAVE_ALLOCA
/* Nothing has to be done: */
# define freea(p) /* nothing */
# define ADD_BLOCK(list, address) /* nothing */
# define FREE_BLOCKS(list) /* nothing */
#else
struct block_list
{
  void *address;
  struct block_list *next;
};
# define ADD_BLOCK(list, addr)						  \
  do {									  \
    struct block_list *newp = (struct block_list *)malloc(sizeof(*newp)); \
    /* If we cannot get a free block, then we cannot add the new element  \
     * to the list. */							  \
    if (newp != NULL) {							  \
      newp->address = (addr);						  \
      newp->next = (list);						  \
      (list) = newp;							  \
    }									  \
  } while (0)
# define FREE_BLOCKS(list)						  \
  do {									  \
    while (list != NULL) {						  \
      struct block_list *old = list;					  \
      list = list->next;						  \
      free(old->address);						  \
      free(old);							  \
    }									  \
  } while (0)
# undef alloca
# define alloca(size) (malloc (size))
# define freea(p) free (p)
#endif	/* have alloca */


#ifdef _LIBC
/* List of blocks allocated for translations: */
typedef struct transmem_list
{
  struct transmem_list *next;
  char data[ZERO];
} transmem_block_t;
static struct transmem_list *transmem_list;
#else
typedef unsigned char transmem_block_t;
#endif /* _LIBC */


/* Names for the libintl functions are a problem. They must not clash
 * with existing names and they should follow ANSI C. But this source
 * code is also used in GNU C Library where the names have a __
 * prefix. So we have to make a difference here: */
#ifdef _LIBC
# define DCIGETTEXT __dcigettext
#else
# define DCIGETTEXT libintl_dcigettext
#endif /* _LIBC */

/* Lock variable to protect global data in the gettext implementation: */
#ifdef _LIBC
__libc_rwlock_define_initialized(, _nl_state_lock attribute_hidden)
#endif /* _LIBC */

/* Checking whether the binaries runs SUID must be done and glibc provides
 * easier methods therefore we make a difference here: */
#ifdef _LIBC
# define ENABLE_SECURE __libc_enable_secure
# define DETERMINE_SECURE
#else
# ifndef HAVE_GETUID
#  define getuid() 0
# endif /* !HAVE_GETUID */
# ifndef HAVE_GETGID
#  define getgid() 0
# endif /* !HAVE_GETGID */
# ifndef HAVE_GETEUID
#  define geteuid() getuid()
# endif /* !HAVE_GETEUID */
# ifndef HAVE_GETEGID
#  define getegid() getgid()
# endif /* !HAVE_GETEGID */
static int enable_secure;
# define ENABLE_SECURE (enable_secure == 1)
# define DETERMINE_SECURE \
  if (enable_secure == 0) {						  \
      if ((getuid() != geteuid()) || (getgid() != getegid())) {		  \
	enable_secure = 1;						  \
      } else {							          \
	enable_secure = -1;						  \
      }                                                                   \
  }
#endif /* _LIBC */

/* Get the function to evaluate the plural expression.  */
#include "eval-plural.h"

/* Same condition as where this was extracted from: */
#ifndef _LIBC
extern void _nl_log_untranslated PARAMS((const char *logfilename,
                                         const char *domainname,
                                         const char *msgid1,
                                         const char *msgid2,
                                         int plural));
#endif /* !_LIBC */

/* Look up MSGID in the DOMAINNAME message catalog for the current
   CATEGORY locale and, if PLURAL is nonzero, search over string
   depending on the plural form determined by N.  */
char *DCIGETTEXT(const char *domainname, const char *msgid1,
                 const char *msgid2, int plural, unsigned long int n,
                 int category)
{
#ifndef HAVE_ALLOCA
  struct block_list *block_list = NULL;
#endif /* !HAVE_ALLOCA */
  struct loaded_l10nfile *domain;
  struct binding *binding;
  const char *categoryname;
  const char *categoryvalue;
  char *dirname, *xdomainname;
  char *single_locale;
  char *retval;
  char *retstr;
  size_t retlen;
  int saved_errno;
#if defined HAVE_TSEARCH || defined _LIBC
  struct known_translation_t *search;
  struct known_translation_t **foundp = NULL;
  size_t msgid_len;
#endif /* HAVE_TSEARCH || _LIBC */
  size_t domainname_len;

  /* If no real MSGID is given, then return NULL: */
  if (msgid1 == NULL) {
    return NULL;
  }

#ifdef _LIBC
  if ((category < 0) || (category >= __LC_LAST) || (category == LC_ALL)) {
    /* Bogus: */
    return ((plural == 0)
	    ? (char *)msgid1
	    /* Use the Germanic plural rule: */
	    : ((n == 1) ? (char *)msgid1 : (char *)msgid2));
  }
#endif /* _LIBC */

  __libc_rwlock_rdlock(_nl_state_lock);

  /* If DOMAINNAME is NULL, we are interested in the default domain. If
   * CATEGORY is not LC_MESSAGES, then this might not make much sense,
   * but the definition left this undefined: */
  if (domainname == NULL) {
    domainname = _nl_current_default_domain;
  }

  /* OS/2 specific: backward compatibility with older libintl versions: */
#ifdef LC_MESSAGES_COMPAT
  if (category == LC_MESSAGES_COMPAT) {
    category = LC_MESSAGES;
  }
#endif /* LC_MESSAGES_COMPAT */

#if defined HAVE_TSEARCH || defined _LIBC
  msgid_len = (strlen(msgid1) + 1);

  /* Try to find the translation among those which we found at
   * some time: */
  search = (struct known_translation_t *)
	   alloca(offsetof(struct known_translation_t, msgid) + msgid_len);
  memcpy(search->msgid, msgid1, msgid_len);
  search->domainname = (char *)domainname;
  search->category = category;

  foundp = (struct known_translation_t **)tfind(search, &root, transcmp);
  freea (search);
  if ((foundp != NULL) && ((*foundp)->counter == _nl_msg_cat_cntr)) {
      /* Now deal with plural: */
      if (plural) {
	retval = plural_lookup((*foundp)->domain, n,
                               (*foundp)->translation,
                               (*foundp)->translation_length);
      } else {
	retval = (char *)(*foundp)->translation;
      }

      __libc_rwlock_unlock(_nl_state_lock);
      return retval;
  }
#endif /* HAVE_TSEARCH || _LIBC */

  /* Preserve the `errno' value: */
  saved_errno = errno;

  /* See whether this is a SUID binary or not: */
  DETERMINE_SECURE;

  /* First find matching binding: */
  for ((binding = _nl_domain_bindings); (binding != NULL); (binding = binding->next)) {
      int compare = strcmp(domainname, binding->domainname);
      if (compare == 0) {
	/* We found it! */
	break;
      }
      if (compare < 0) {
	  /* It is not in the list: */
	  binding = NULL;
	  break;
      }
  }

  if (binding == NULL) {
    dirname = (char *)INTUSE(_nl_default_dirname);
  } else if (IS_ABSOLUTE_PATH(binding->dirname)) {
    dirname = binding->dirname;
  } else {
      /* We have a relative path. Make it absolute now: */
      size_t dirname_len = (strlen(binding->dirname) + 1UL);
      size_t path_max;
      char *ret;

      path_max = (unsigned int)PATH_MAX;
      path_max += 2UL; /* The getcwd docs say to do this. */

      for (;;) {
	  dirname = (char *)alloca(path_max + dirname_len);
	  ADD_BLOCK(block_list, dirname);

	  __set_errno(0);
	  ret = getcwd(dirname, path_max);
	  if ((ret != NULL) || (errno != ERANGE)) {
	    break;
          }

	  path_max += (path_max / 2UL);
	  path_max += PATH_INCR;
      }

      if (ret == NULL) {
	/* We cannot get the current working directory. Do NOT signal an
	 * error, but simply return the default string: */
	goto return_untranslated;
      }

      stpcpy(stpcpy(strchr(dirname, '\0'), "/"), binding->dirname);
  }

  /* Now determine the symbolic name of CATEGORY and its value: */
  categoryname = category_to_name(category);
  categoryvalue = guess_category_value(category, categoryname);

  domainname_len = strlen(domainname);
  xdomainname = (char *)alloca(strlen(categoryname) + domainname_len + 5);
  ADD_BLOCK(block_list, xdomainname);

  retstr =
    (char *)stpcpy((char *)mempcpy(stpcpy(stpcpy(xdomainname,
                                                 categoryname), "/"),
                                   domainname, domainname_len), ".mo");

  if (retstr == NULL) {
    ; /* do something? */
  }

  /* Creating working area: */
  single_locale = (char *)alloca(strlen(categoryvalue) + 1);
  ADD_BLOCK(block_list, single_locale);


  /* Search for the given string. This is a loop because we perhaps
   * got an ordered list of languages to consider for the translation: */
  while (1) {
      /* Make CATEGORYVALUE point to the next element of the list: */
      while ((categoryvalue[0] != '\0') && (categoryvalue[0] == ':')) {
	++categoryvalue;
      }
      if (categoryvalue[0] == '\0') {
	  /* The whole contents of CATEGORYVALUE has been searched but
	   * no valid entry has been found. We solve this situation
	   * by implicitly appending a "C" entry, i.e. no translation
	   * will take place.  */
	  single_locale[0] = 'C';
	  single_locale[1] = '\0';
      } else {
	  char *cp = single_locale;
          while ((categoryvalue[0] != '\0') && (categoryvalue[0] != ':')) {
	    *cp++ = *categoryvalue++;
          }
	  *cp = '\0';

	  /* When this is a SUID binary we must not allow accessing files
	   * outside the dedicated directories: */
          if (ENABLE_SECURE && IS_PATH_WITH_DIR(single_locale)) {
	    /* Ingore this entry: */
	    continue;
          }
      }

      /* If the current locale value is C (or POSIX), then we do NOT load a
       * domain. Return the MSGID: */
      if ((strcmp(single_locale, "C") == 0)
	  || (strcmp(single_locale, "POSIX") == 0)) {
	break;
      }

      /* Find structure describing the message catalog matching the
       * DOMAINNAME and CATEGORY: */
#if 0
      /*FIXME: define a preprocessor macro based on how many arguments
       * this function takes, and then check for it: */
      domain = _nl_find_domain(dirname, single_locale, xdomainname,
                               binding);
#else
      domain = _nl_find_domain(dirname, single_locale, xdomainname);
#endif /* 0 */

      if (domain != NULL) {
	  retval = _nl_find_msg(domain, binding, msgid1, &retlen);

	  if (retval == NULL) {
	      int cnt;

	      for ((cnt = 0); (domain->successor[cnt] != NULL); ++cnt) {
		  retval = _nl_find_msg(domain->successor[cnt], binding,
                                        msgid1, &retlen);

		  if (retval != NULL) {
		      domain = domain->successor[cnt];
		      break;
                  }
              }
          }

	  if (retval != NULL) {
	      /* Found the translation of MSGID1 in domain DOMAIN:
               * starting at RETVAL, RETLEN bytes: */
	      FREE_BLOCKS(block_list);
#if defined HAVE_TSEARCH || defined _LIBC
	      if (foundp == NULL) {
		  /* Create a new entry and add it to the search tree: */
		  struct known_translation_t *newp;

		  newp = (struct known_translation_t *)
		    malloc(offsetof(struct known_translation_t, msgid)
                           + msgid_len + domainname_len + 1);
		  if (newp != NULL) {
		      newp->domainname =
			mempcpy(newp->msgid, msgid1, msgid_len);
		      memcpy(newp->domainname, domainname,
                             (domainname_len + 1));
		      newp->category = category;
		      newp->counter = _nl_msg_cat_cntr;
		      newp->domain = domain;
		      newp->translation = retval;
		      newp->translation_length = retlen;

		      /* Insert the entry in the search tree: */
		      foundp = (struct known_translation_t **)
			tsearch(newp, &root, transcmp);
		      if ((foundp == NULL)
			  || __builtin_expect(*foundp != newp, 0)) {
			/* The insert failed: */
			free(newp);
                      }
                  }
              } else {
		  /* We can update the existing entry: */
		  (*foundp)->counter = _nl_msg_cat_cntr;
		  (*foundp)->domain = domain;
		  (*foundp)->translation = retval;
		  (*foundp)->translation_length = retlen;
              }
#endif /* HAVE_TSEARCH || _LIBC */
	      __set_errno(saved_errno);

	      /* Now deal with plural: */
	      if (plural) {
		retval = plural_lookup(domain, n, retval, retlen);
              }

	      __libc_rwlock_unlock(_nl_state_lock);
	      return retval;
          }
      } /* end check "if" domain is null */
  } /* end while-loop */

 return_untranslated:
  /* Return the untranslated MSGID: */
  FREE_BLOCKS(block_list);
  __libc_rwlock_unlock(_nl_state_lock);
#ifndef _LIBC
  if (!ENABLE_SECURE) {
      const char *logfilename = getenv("GETTEXT_LOG_UNTRANSLATED");

      if ((logfilename != NULL) && (logfilename[0] != '\0')) {
	_nl_log_untranslated(logfilename, domainname, msgid1, msgid2,
                             plural);
      }
  }
#endif /* !_LIBC */
  __set_errno(saved_errno);
  return ((plural == 0)
	  ? (char *)msgid1
	  /* Use the Germanic plural rule: */
	  : ((n == 1) ? (char *)msgid1 : (char *)msgid2));
}


char *
internal_function
_nl_find_msg(struct loaded_l10nfile *domain_file,
             struct binding *domainbinding, const char *msgid,
             size_t *lengthp)
{
  struct loaded_domain *domain;
  nls_uint32 nstrings;
  size_t act;
  char *result;
  size_t resultlen;

  if (domain_file->decided == 0) {
#if 0
    _nl_load_domain(domain_file, domainbinding);
#else
    _nl_load_domain(domain_file);
    if (domainbinding != NULL) {
      ; /* do nothing; just silence a warning */
    }
#endif /* 0 */
  }

  if (domain_file->data == NULL) {
    return NULL;
  }

  domain = (struct loaded_domain *)domain_file->data;

  nstrings = domain->nstrings;

  /* Locate the MSGID and its translation: */
  if (domain->hash_tab != NULL) {
      /* Use the hashing table: */
      nls_uint32 len = (nls_uint32)strlen(msgid);
      nls_uint32 hash_val = (nls_uint32)hash_string(msgid);
      nls_uint32 idx = (hash_val % domain->hash_size);
      nls_uint32 incr = (1 + (hash_val % (domain->hash_size - 2)));

      while (1) {
	  nls_uint32 nstr =
	    W(domain->must_swap_hash_tab, domain->hash_tab[idx]);

	  if (nstr == 0) {
	    /* Hash table entry is empty: */
	    return NULL;
          }

	  nstr--;

	  /* Compare msgid with the original string at index nstr.
	   * We compare the lengths with >=, not ==, because plural entries
	   * are represented by strings with an embedded NUL: */
	  if ((nstr < nstrings)
	      ? (W(domain->must_swap,
                   domain->orig_tab[nstr].length) >= len)
		&& (strcmp(msgid,
                           domain->data + W(domain->must_swap,
                                            domain->orig_tab[nstr].offset))
		    == 0)
	      : (domain->orig_sysdep_tab[(nstr - nstrings)].length > len)
		&& (strcmp(msgid,
                           domain->orig_sysdep_tab[(nstr - nstrings)].pointer)
		    == 0)) {
	      act = nstr;
	      goto found;
          }

	  if (idx >= domain->hash_size - incr) {
	    idx -= (domain->hash_size - incr);
	  } else {
	    idx += incr;
          }
      } /* end while-loop */
      /* NOTREACHED */
  } else {
      /* Try the default method: binary search in the sorted array of
       * messages: */
      size_t top, bottom;

      bottom = 0;
      top = nstrings;
      while (bottom < top) {
	  int cmp_val;

	  act = ((bottom + top) / 2);
	  cmp_val = strcmp(msgid, (domain->data
                                   + W(domain->must_swap,
                                       domain->orig_tab[act].offset)));
	  if (cmp_val < 0) {
	    top = act;
	  } else if (cmp_val > 0) {
	    bottom = (act + 1);
	  } else {
	    goto found;
          }
      }
      /* No translation was found: */
      return NULL;
  }

 found:
  /* The translation was found at index ACT. If we have to convert the
     string to use a different character set, this is the time.  */
  if (act < nstrings) {
      result = (char *)(domain->data + W(domain->must_swap,
                                         domain->trans_tab[act].offset));
      resultlen = (W(domain->must_swap,
                     domain->trans_tab[act].length) + 1);
  } else {
      result = (char *)domain->trans_sysdep_tab[(act - nstrings)].pointer;
      resultlen = domain->trans_sysdep_tab[(act - nstrings)].length;
  }

#if defined(_LIBC) || (defined(HAVE_ICONV) && HAVE_ICONV)
  if (domain->codeset_cntr
      != ((domainbinding != NULL) ? domainbinding->codeset_cntr : 0)) {
      /* The domain's codeset has changed through bind_textdomain_codeset()
       * since the message catalog was initialized or last accessed. We
       * have to reinitialize the converter: */
      _nl_free_domain_conv(domain);
      _nl_init_domain_conv(domain_file, domain, domainbinding);
  }

# ifndef CONV_TYPE
#  ifdef _LIBC
#   define CONV_TYPE __gconv_t
#  else
#   if defined(HAVE_ICONV) && HAVE_ICONV
#    if defined(HAVE_ICONV_T) || defined(_ICONV_T)
#     define CONV_TYPE iconv_t
#    else
#     define CONV_TYPE void *
#    endif /* HAVE_ICONV_T || _ICONV_T */
#   else
#    define CONV_TYPE int
#    if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#     warning "using a conversion type that might be incorrect."
#    endif /* __GNUC__ && !__STRICT_ANSI__ */
#   endif /* HAVE_ICONV */
#  endif /* _LIBC */
# endif /* !CONV_TYPE */

  if (domain->conv != (CONV_TYPE)-1) {
      /* We are supposed to do a conversion.  First allocate an
	 appropriate table with the same structure as the table
	 of translations in the file, where we can put the pointers
	 to the converted strings in.
	 There is a slight complication with plural entries.  They
	 are represented by consecutive NUL terminated strings.  We
	 handle this case by converting RESULTLEN bytes, including
	 NULs.  */

      if ((domain->conv_tab == NULL)
	  && ((domain->conv_tab =
		 (char **)calloc((size_t)(nstrings + domain->n_sysdep_strings),
                                  sizeof(char *)))
	      == NULL)) {
	/* Mark that we did NOT succeed allocating a table: */
	domain->conv_tab = (char **)-1;
      }

      if (__builtin_expect(domain->conv_tab == (char **)-1, 0)) {
	/* Nothing we can do, no more memory: */
	goto converted;
      }

      if (domain->conv_tab[act] == NULL) {
	  /* We have NOT used this string so far, so it is not
	   * translated yet. Do this now. */
	  /* We use a bit more efficient memory handling.
	   * We allocate always larger blocks which get used over
	   * time. This is faster than many small allocations.  */
	  __libc_lock_define_initialized(static, lock)
# define INITIAL_BLOCK_SIZE 4080
	  static unsigned char *freemem;
	  static size_t freemem_size;

	  const unsigned char *inbuf;
	  unsigned char *outbuf;
	  int malloc_count;
# ifndef _LIBC
	  transmem_block_t *transmem_list = NULL;
# endif /* !_LIBC */

	  __libc_lock_lock(lock);

	  inbuf = (const unsigned char *)result;
	  outbuf = (freemem + sizeof(size_t));

	  malloc_count = 0;
	  while (1) {
	      transmem_block_t *newmem;
# ifdef _LIBC
	      size_t non_reversible;
	      int res;

	      if (freemem_size < sizeof(size_t)) {
		goto resize_freemem;
              }

	      res = __gconv(domain->conv, &inbuf,
                            (inbuf + resultlen), &outbuf,
                            (outbuf + freemem_size - sizeof(size_t)),
                            &non_reversible);

	      if ((res == __GCONV_OK) || (res == __GCONV_EMPTY_INPUT)) {
		break;
              }

	      if (res != __GCONV_FULL_OUTPUT) {
		  __libc_lock_unlock(lock);
		  goto converted;
              }

	      inbuf = result;
# else
#  if defined(HAVE_ICONV) && HAVE_ICONV
	      const char *inptr = (const char *)inbuf;
	      size_t inleft = resultlen;
	      char *outptr = (char *)outbuf;
	      size_t outleft;

	      if (freemem_size < sizeof(size_t)) {
		goto resize_freemem;
              }

	      outleft = (freemem_size - sizeof(size_t));
	      if (iconv(domain->conv,
                        (ICONV_CONST char **)&inptr, &inleft,
                        &outptr, &outleft) != (size_t)(-1)) {
		  outbuf = (unsigned char *)outptr;
		  break;
              }
	      if (errno != E2BIG) {
		  __libc_lock_unlock(lock);
		  goto converted;
              }
#  endif /* HAVE_ICONV */
# endif /* _LIBC */

	    resize_freemem:
	      /* We must allocate a new buffer or resize the old one: */
	      if (malloc_count > 0) {
		  ++malloc_count;
		  freemem_size = (size_t)(malloc_count * INITIAL_BLOCK_SIZE);
		  newmem = (transmem_block_t *)realloc(transmem_list,
                                                       freemem_size);
# ifdef _LIBC
		  if (newmem != NULL) {
		    transmem_list = transmem_list->next;
		  } else {
		      struct transmem_list *old = transmem_list;

		      transmem_list = transmem_list->next;
		      free(old);
                  }
# endif /* _LIBC */
              } else {
		  malloc_count = 1;
		  freemem_size = INITIAL_BLOCK_SIZE;
		  newmem = (transmem_block_t *)malloc(freemem_size);
              }
	      if (__builtin_expect(newmem == NULL, 0)) {
		  freemem = NULL;
		  freemem_size = 0;
		  __libc_lock_unlock(lock);
		  goto converted;
              }

# ifdef _LIBC
	      /* Add the block to the list of blocks we have to free
               * at some point.  */
	      newmem->next = transmem_list;
	      transmem_list = newmem;

	      freemem = newmem->data;
	      freemem_size -= offsetof(struct transmem_list, data);
# else
	      transmem_list = newmem;
	      freemem = newmem;
# endif /* _LIBC */

	      outbuf = freemem + sizeof(size_t);
          } /* end while-loop */

	  /* We have now in our buffer a converted string. Put this
	   * into the table of conversions. */
	  *(size_t *)freemem =
            (size_t)(outbuf - (size_t)freemem - sizeof(size_t));
	  domain->conv_tab[act] = (char *)freemem;
	  /* Shrink freemem, but keep it aligned: */
	  freemem_size -= (long unsigned int)(outbuf - freemem);
	  freemem = outbuf;
	  freemem += (freemem_size & (alignof(size_t) - 1));
	  freemem_size = (freemem_size & ~ (alignof(size_t) - 1));

	  __libc_lock_unlock(lock);
      } /* end "if" check for NULL */

      /* Now domain->conv_tab[act] contains the translation of all
	 the plural variants.  */
      result = (domain->conv_tab[act] + sizeof(size_t));
      resultlen = *(size_t *)domain->conv_tab[act];
  } /* end "if" */

 converted:
  /* The result string is converted. */

#endif /* _LIBC || HAVE_ICONV */

  *lengthp = resultlen;
  return result;
}


/* Look up a plural variant: */
static char *
internal_function
plural_lookup(struct loaded_l10nfile *domain, unsigned long int n,
              const char *translation, size_t translation_len)
{
  struct loaded_domain *domaindata = (struct loaded_domain *)domain->data;
  unsigned long int index_val;
  const char *p;

  index_val = plural_eval(domaindata->plural, n);
  if (index_val >= domaindata->nplurals) {
    /* This should never happen. It means the plural expression and the
     * given maximum value do not match: */
    index_val = 0;
  }

  /* Skip INDEX strings at TRANSLATION: */
  p = translation;
  while (index_val-- > 0) {
#ifdef _LIBC
      p = __rawmemchr(p, '\0');
#else
      p = strchr(p, '\0');
#endif /* _LIBC */
      /* And skip over the NUL byte.  */
      p++;

      if (p >= (translation + translation_len)) {
	/* This should never happen. It means the plural expression
	 * evaluated to a value larger than the number of variants
	 * available for MSGID1.  */
	return (char *)translation;
      }
  }
  return (char *)p;
}

#ifndef _LIBC
/* Return string representation of locale CATEGORY: */
static const char *
internal_function
category_to_name(int category)
{
  const char *retval;

  switch (category) {
#ifdef LC_COLLATE
  case LC_COLLATE:
    retval = "LC_COLLATE";
    break;
#endif /* LC_COLLATE */
#ifdef LC_CTYPE
  case LC_CTYPE:
    retval = "LC_CTYPE";
    break;
#endif /* LC_CTYPE */
#ifdef LC_MONETARY
  case LC_MONETARY:
    retval = "LC_MONETARY";
    break;
#endif /* LC_MONETARY */
#ifdef LC_NUMERIC
  case LC_NUMERIC:
    retval = "LC_NUMERIC";
    break;
#endif /* LC_NUMERIC */
#ifdef LC_TIME
  case LC_TIME:
    retval = "LC_TIME";
    break;
#endif /* LC_TIME */
#ifdef LC_MESSAGES
  case LC_MESSAGES:
    retval = "LC_MESSAGES";
    break;
#endif /* LC_MESSAGES */
#ifdef LC_RESPONSE
  case LC_RESPONSE:
    retval = "LC_RESPONSE";
    break;
#endif /* LC_RESPONSE */
#ifdef LC_ALL
  case LC_ALL:
    /* This might not make sense, but it is perhaps better than any other
     * value: */
    retval = "LC_ALL";
    break;
#endif /* LC_ALL */
  default:
    /* If you have a better idea for a default value, then let me know: */
    retval = "LC_XXX";
  }

  return retval;
}
#endif /* !_LIBC */

/* Guess value of current locale from value of environment variables: */
static const char *
internal_function
guess_category_value(int category, const char *categoryname)
{
  const char *language;
  const char *retval;

  /* The highest priority value is the `LANGUAGE' environment
     variable.  But we do NOT use the value if the currently selected
     locale is the C locale.  This is a GNU extension.  */
  language = getenv("LANGUAGE");
  if ((language != NULL) && (language[0] == '\0')) {
    language = NULL;
  }

  /* We have to proceed with the POSIX methods of looking to `LC_ALL',
     `LC_xxx', and `LANG'.  On some systems this can be done by the
     `setlocale' function itself.  */
#ifdef _LIBC
  retval = __current_locale_name(category);
#else
  retval = _nl_locale_name(category, categoryname);
#endif /* _LIBC */

  /* Ignore LANGUAGE if the locale is set to "C" because
     1. "C" locale usually uses the ASCII encoding, and most international
	messages use non-ASCII characters. These characters get displayed
	as question marks (if using glibc's iconv()) or as invalid 8-bit
	characters (because other iconv()s refuse to convert most non-ASCII
	characters to ASCII). In any case, the output is ugly.
     2. The precise output of some programs in the "C" locale is specified
	by POSIX and should not depend on environment variables like
	"LANGUAGE".  We allow such programs to use gettext().  */
  return (((language != NULL) && (strcmp(retval, "C") != 0))
          ? language : retval);
}

/* @@ begin of epilog @@ */

/* We do NOT want libintl.a to depend on any other library. So we
 * avoid the non-standard function stpcpy. In GNU C Library this
 * function is available, though. Also allow the symbol HAVE_STPCPY
 * to be defined.  */
#if (!defined(_LIBC) || !_LIBC) && !HAVE_STPCPY
static char *stpcpy(char *dest, const char *src)
{
  while ((*dest++ = *src++) != '\0') {
    /* Do nothing. */ ;
  }
  return (dest - 1);
}
#endif /* !_LIBC && !HAVE_STPCPY */

/* try to keep condition the same as the one used for its prototype: */
#if (!defined(_LIBC) || !_LIBC) && (!defined(HAVE_MEMPCPY) || !HAVE_MEMPCPY) || \
    (!defined(HAVE_DECL_MEMPCPY) || !HAVE_DECL_MEMPCPY)
# if !defined(__STRICT_ANSI__)
static
# endif /* !__STRICT_ANSI__ */
void *mempcpy(void *dest, const void *src, size_t n)
{
  return (void *)((char *)memcpy(dest, src, n) + n);
}
#endif /* !_LIBC && !HAVE_MEMPCPY */


#ifdef _LIBC
/* If we want to free all resources we have to do some work at
 * program's end: */
libc_freeres_fn(free_mem)
{
  void *old;

  while (_nl_domain_bindings != NULL) {
      struct binding *oldp = _nl_domain_bindings;
      _nl_domain_bindings = _nl_domain_bindings->next;
      if (oldp->dirname != INTUSE(_nl_default_dirname)) {
	/* Yes, this is a pointer comparison: */
	free(oldp->dirname);
      }
      free(oldp->codeset);
      free(oldp);
  }

  if (_nl_current_default_domain != _nl_default_default_domain) {
    /* Yes, again a pointer comparison: */
    free((char *)_nl_current_default_domain);
  }

  /* Remove the search tree with the known translations.  */
  __tdestroy(root, free);
  root = NULL;

  while (transmem_list != NULL) {
      old = transmem_list;
      transmem_list = transmem_list->next;
      free(old);
  }
}
#endif /* _LIBC */

#ifdef HAVE_ALLOCA
# undef HAVE_ALLOCA
#endif /* HAVE_ALLOCA */

/* EOF */
