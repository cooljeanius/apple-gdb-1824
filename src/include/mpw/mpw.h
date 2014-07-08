/* mpw.h
 * Mac MPW host-specific definitions.
 */

#ifndef	__INCLUDE_MPW_H
#define __INCLUDE_MPW_H

#ifndef MPW
# define MPW /* (nothing) */
#endif /* !MPW */

/* MPW C is basically ANSI, but does NOT actually enable __STDC__, nor does it
 * allow __STDC__ to be #defined. */

#ifndef ALMOST_STDC
# define ALMOST_STDC
#endif /* !ALMOST_STDC */

#include <stdio.h>
#include <ctype.h>
#include <errno.h>

#define HAVE_TIME_T_IN_TIME_H 1

#define HAVE_STDLIB_H 1

#define HAVE_ERRNO_H 1

#define HAVE_STDDEF_H 1

#define HAVE_STRING_H 1

#define HAVE_STDARG_H 1

#define HAVE_VPRINTF 1

#ifdef USE_MW_HEADERS
# include <unix.h>
#else
# include <fcntl.h>
# ifdef HAVE_IOCTL_H
#  include <ioctl.h>
# else
#  ifdef HAVE_SYS_IOCTL_H
#   include <sys/ioctl.h>
#  endif /* HAVE_SYS_IOCTL_H */
# endif /* HAVE_IOCTL_H */
# include <sys/stat.h>
# define HAVE_FCNTL_H 1
# ifndef O_ACCMODE
#  define O_ACCMODE (O_RDONLY | O_WRONLY | O_RDWR)
# endif /* !O_ACCMODE */
# ifndef fileno
#  define fileno(fp) ((fp)->_file)
# endif /* !fileno */
/* stdio.h does not define this if __STDC__, so define here. */
# ifdef __STDC__
FILE *fdopen(int fildes, const char *mode);
# endif /* __STDC__ */
#endif /* USE_MW_HEADERS */

/* Add ersatz definitions, for systems that lack them.  */

#ifndef EIO
# define EIO 96
#endif /* !EIO */
#ifndef ENOENT
# define ENOENT 97
#endif /* !ENOENT */
#ifndef EACCES
# define EACCES 98
#endif /* !EACCES */
#ifndef ENOSYS
# define ENOSYS 99
#endif /* !ENOSYS */

#ifndef R_OK
# define R_OK 4
# define W_OK 2
# define X_OK 1
#endif /* !R_OK */

/* Binary files have different characteristics; for instance, no cr/nl
 * translation: */
#define USE_BINARY_FOPEN /* (nothing) */

#include <spin.h>

#ifdef MPW_C
# undef  __PTR_TO_INT
# define __PTR_TO_INT(P) ((int)(P))
# undef __INT_TO_PTR
# define __INT_TO_PTR(P) ((char *)(P))
#endif /* MPW_C */

#define NO_FCNTL

#if !defined(_SYS_STAT_H_) && !defined(__SYS_STAT_H__)
int fstat(void);
#endif /* !_SYS_STAT_H_ && !__SYS_STAT_H__ */

FILE *mpw_fopen(void);
int mpw_fseek(void);
int mpw_fread(void);
int mpw_fwrite(void);
int mpw_access(void);
int mpw_open(void);
int mpw_creat(void);
void mpw_abort(void);

/* Map these standard functions to improved versions in libiberty: */
#define fopen mpw_fopen
#define fseek mpw_fseek
#define fread mpw_fread
#define fwrite mpw_fwrite
#define open mpw_open
#define access mpw_access
#define creat mpw_creat
#define abort mpw_abort

#define POSIX_UTIME /* (nothing) */

#define LOSING_TOTALLY /* (nothing) */

/* Define this so that files will be closed before being unlinked: */
#define CLOSE_BEFORE_UNLINK /* (nothing) */

#endif /* __INCLUDE_MPW_H */

/* EOF */
