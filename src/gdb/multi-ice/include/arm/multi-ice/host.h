/* host.h - adapted from the one for rdi-share
 * Copyright (C) 1995 Advanced RISC Machines Limited. All rights reserved.
 *
 * This software may be freely used, copied, modified, and distributed
 * provided that the above copyright notice is preserved in all copies
 * of the software.
 */


#ifndef _ARM_RDI_SHARE_HOST_H
#define _ARM_RDI_SHARE_HOST_H 1

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#ifdef GDB_NM_FILE
# include "nm.h"
#endif /* GDB_NM_FILE */
#ifdef GDB_XM_FILE
# include "xm.h"
#endif /* GDB_XM_FILE */

#ifndef SEEK_SET
#  define SEEK_SET  0
#endif /* !SEEK_SET */
#ifndef SEEK_CUR
#  define SEEK_CUR  1
#endif /* !SEEK_CUR */
#ifndef SEEK_END
#  define SEEK_END  2
#endif /* !SEEK_END */

/* The following for the benefit of compiling on SunOS: */
#ifndef offsetof
#  define offsetof(T, member)  ((char *)&(((T *)0)->member) - (char *)0)
#endif /* !offsetof */

/* A temporary sop to older compilers */
#if defined (__NetBSD__) || defined (unix) || defined (__APPLE__)
#  ifndef __unix              /* (good for long-term portability?)  */
#    define __unix 1
#  endif /* !__unix */
#endif /* __NetBSD__ || unix || __APPLE__ */

#if defined(__unix)
/* Generic unix -- hopefully a split into other variants will not be    */
/* needed.  However, beware the 'bsd' test above and safe_toupper etc.  */
/* which cope with backwards (pre-posix/X/open) unix compatility.       */
#  define COMPILING_ON_UNIX 1
#endif /* __unix */
#if defined(_WIN32)
#  define COMPILING_ON_WIN32 1
#  if !defined(__CYGWIN__)
#    define COMPILING_ON_WINDOWS 1
#  endif /* !__CYGWIN__ */
#endif /* _WIN32 */
#if defined(_CONSOLE)
#  define COMPILING_ON_WINDOWS_CONSOLE 1
#  define COMPILING_ON_WINDOWS 1
#endif /* _CONSOLE */
/* The '(defined(__sparc) && defined(P_tmpdir)                     */
/* && !defined(__svr4__))' is to detect gcc on SunOS.              */
/* C++ compilers do NOT have to define __STDC__                    */
#if (defined(__sparc) && defined(P_tmpdir))
#  if defined(__svr4__)
#    define COMPILING_ON_SOLARIS
#  else
#    define COMPILING_ON_SUNOS
#  endif /* __svr4__ */
#endif /* (__sparc && P_tmpdir) */
#ifdef __hppa
#  define COMPILING_ON_HPUX
#endif /* __hppa */

/*
 * The following typedefs may need alteration for obscure host machines.
 */
#if defined(__alpha) && defined(__osf__) /* =========================== */
/* Under OSF the alpha has 64-bit pointers and 64-bit longs.            */
# ifndef _INT32_DEFINED
#  define _INT32_DEFINED 1
typedef                int   int32;
# endif /* _INT32_DEFINED */
# ifndef _UNSIGNED32_DEFINED
#  define _UNSIGNED32_DEFINED 1
typedef unsigned       int   unsigned32;
# endif /* _UNSIGNED32_DEFINED */
/* IPtr and UPtr are 'ints of same size as pointer'.  Do not use in     */
/* new code.  Currently only used within 'ncc'.                         */
typedef          long  int   IPtr;
typedef unsigned long  int   UPtr;

#else /* all hosts except alpha under OSF ============================= */
# ifndef _INT32_DEFINED
#  define _INT32_DEFINED 1
typedef          long  int   int32;
# endif /* _INT32_DEFINED */
# ifndef _UNSIGNED32_DEFINED
#  define _UNSIGNED32_DEFINED 1
typedef unsigned long  int   unsigned32;
# endif /* _UNSIGNED32_DEFINED */
/* IPtr and UPtr are 'ints of same size as pointer'.  Do not use in     */
/* new code.  Currently only used within 'ncc'.                         */
# ifndef IPtr
#  define IPtr int32
# endif /* !IPtr */
# ifndef UPtr
#  define UPtr unsigned32
# endif /* UPtr */
#endif /* ============================================================= */

typedef          short int   int16;
typedef unsigned short int   unsigned16;
typedef   signed       char  int8;
typedef unsigned       char  unsigned8;

/* The following code defines the 'bool' type to be 'int' under C       */
/* and real 'bool' under C++.  It also avoids warnings such as          */
/* C++ keyword 'bool' used as identifier.  It can be overridden by      */
/* defining IMPLEMENT_BOOL_AS_ENUM or IMPLEMENT_BOOL_AS_INT.            */
#undef _bool

#ifdef IMPLEMENT_BOOL_AS_ENUM
   enum _bool { _false, _true };
#  ifndef _bool
#    define _bool enum _bool
#  endif /* !_bool */
#elif defined(IMPLEMENT_BOOL_AS_INT) || !defined(__cplusplus)
#  ifndef _bool
#    define _bool int
#  endif /* !_bool */
#  ifndef _false
#    define _false 0
#  endif /* _false */
#  ifndef _true
#    define _true 1
#  endif /* !_true */
#endif /* bool implementations */

#ifdef _bool
#  if defined(_MFC_VER) || defined(__CC_NORCROFT) /* When using MS Visual C/C++ v4.2 */
#    ifndef bool
#      define bool _bool /* avoids "'bool' is reserved word" warning    */
#    endif /* !bool */
#    ifndef BOOL
#      define BOOL _bool /* case sensitivity... */
#    endif /* !BOOL */
#  else
#    ifndef bool
       typedef _bool bool;
#    endif /* !bool */
#  endif /* _MFC_VER || __CC_NORCROFT */
#  if !defined(true) && defined(_true)
#    define true _true
#  endif /* !true && _true */
#  if !defined(false) && defined(_false)
#    define false _false
#  endif /* !false && _false */
#endif /* _bool */

#if !defined(YES) && !defined(__OBJC__) && defined(true)
# define YES true
#endif /* !YES && !__OBJC__ && true */
#if !defined(NO) && !defined(__OBJC__) && defined(false)
# define NO false
#endif /* !NO && !__OBJC__ && false */

#if !defined(__OBJC__)
# undef TRUE             /* some OSF headers define as 1                 */
# define TRUE true
# undef FALSE            /* some OSF headers define as 1                 */
# define FALSE false
#endif /* !__OBJC__ */

/* 'uint' conflicts with some Unixen <sys/types.h>,
 * so we now do NOT define it here */
typedef unsigned8  uint8;
typedef unsigned16 uint16;
typedef unsigned32 uint32;

typedef unsigned   Uint;
typedef unsigned8  Uint8;
typedef unsigned16 Uint16;
typedef unsigned32 Uint32;

#ifdef ALPHA_TASO_SHORT_ON_OSF /* was __alpha && __osf.                 */
/* The following sets ArgvType for 64-bit pointers so that              */
/* DEC Unix (OSF) cc can be used with the -xtaso_short compiler option  */
/* to force pointers to be 32-bit.  Not currently used since most/all   */
/* ARM tools accept 32 or 64 bit pointers transparently.  See IPtr.     */
#pragma pointer_size (save)
#pragma pointer_size (long)
typedef char *ArgvType;
#pragma pointer_size (restore)
#else
typedef char *ArgvType;
#endif /* ALPHA_TASO_SHORT_ON_OSF */

/*
 * Rotate macros:
 */
#define ROL_32(val, n) \
((((unsigned32)(val) << (n)) | ((unsigned32)(val) >> (32-(n)))) & 0xFFFFFFFFL)
#define ROR_32(val, n) \
((((unsigned32)(val) >> (n)) | ((unsigned32)(val) << (32-(n)))) & 0xFFFFFFFFL)

#if 0
# ifdef COMPILING_ON_UNIX
#  define FOPEN_WB     "w"
#  define FOPEN_RB     "r"
#  define FOPEN_RWB    "r+"
#  ifndef __STDC__                     /* caveat RISCiX... */
#    define remove(file) unlink(file)  /* a horrid hack, but probably best? */
#  endif /* !__STDC__ */
# else
#  define FOPEN_WB     "wb"
#  define FOPEN_RB     "rb"
#  define FOPEN_RWB    "rb+"
# endif /* COMPILING_ON_UNIX */
#endif /* 0 */

#ifndef FILENAME_MAX
#  define FILENAME_MAX 256
#endif /* !FILENAME_MAX */

#if (!defined(__STDC__) && !defined(__cplusplus)) || defined(COMPILING_ON_SUNOS)
/* Use bcopy rather than memmove, as memmove is not available.     */
/* There does not seem to be a header for bcopy.                   */
void bcopy(const char *src, char *dst, int length);
#  define memmove(d,s,l) bcopy(s,d,l)

/* BSD/SUN do not have strtoul(), but then strtol() does NOT barf on */
/* overflow as required by ANSI... This bodge is horrid.           */
#  define strtoul(s, ptr, base) strtol(s, ptr, base)

/* strtod is present in the C-library but is not in stdlib.h       */
extern double strtod(const char *str, char **ptr);
#endif /* (!__STDC__ && !__cplusplus) || COMPILING_ON_SUNOS */

/* For systems that do not define EXIT_SUCCESS and EXIT_FAILURE */
#ifndef EXIT_SUCCESS
#  define EXIT_SUCCESS           0
#endif /* !EXIT_SUCCESS */
#ifndef EXIT_FAILURE
#  define EXIT_FAILURE           1
#endif /* !EXIT_FAILURE */

#ifndef IGNORE
# define IGNORE(x) (x = x)  /* Silence compiler warnings for unused arguments */
#endif /* !IGNORE */

/* Define endian-ness of host: */
#if defined(__acorn) || defined(__mvs) || defined(_WIN32) || \
  (defined(__alpha) && defined(__osf__))
#  define HOST_ENDIAN_LITTLE
#elif defined(__sparc) || defined(macintosh)
#  define HOST_ENDIAN_BIG
#else
#  define HOST_ENDIAN_UNKNOWN
#endif /* different host checks */

#endif /* !_ARM_RDI_SHARE_HOST_H */

/* Needs to be supplied by the host: */
extern void Fail(const char *, ...);

/* end of host.h */
