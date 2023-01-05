/* exp_win.c - window support

Written by: Don Libes, NIST, 10/25/93

This file is in the public domain. However, the author and NIST
would appreciate credit if you use this file or parts of it.

*/

#include "expect_cf.h"
#include "tcl.h"

#ifdef NO_STDLIB_H
# include "../compat/stdlib.h"
#else
# ifdef HAVE_STDLIB_H
#  include <stdlib.h>		/* for malloc */
# else
#  ifdef HAVE_MALLOC_H
#   include <malloc.h>
#  else
#   ifdef HAVE_MALLOC_MALLOC_H
#    include <malloc/malloc.h>
#   else
#    if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#     warning "exp_win.c expects a header that provides malloc() to be included."
#    endif /* __GNUC__ && !__STRICT_ANSI__ */
#   endif /* HAVE_MALLOC_MALLOC_H */
#  endif /* HAVE_MALLOC_H */
# endif /* HAVE_STDLIB_H */
#endif /*NO_STDLIB_H*/

#if defined(HAVE_STRING_H) || defined(STDC_HEADERS) || defined(__STDC__)
# include <string.h>
#endif /* HAVE_STRING_H || STDC_HEADERS || __STDC__ */

#ifndef errno
# ifdef HAVE_ERRNO_H
#  include <errno.h>
# else
#  ifdef HAVE_SYS_ERRNO_H
#   include <sys/errno.h>
#  endif /* HAVE_SYS_ERRNO_H */
# endif /* HAVE_ERRNO_H */
#endif /* !errno */

/* _IBCS2 required on some Intel platforms to allow the include files */
/* to produce a definition for winsize. */
#define _IBCS2 1

/*
 * get everyone's window size definitions
 *
note that this is tricky because (of course) everyone puts them in
different places. Worse, on some systems, some .h files conflict
and cannot both be included even though both exist. This is the
case, for example, on SunOS 4.1.3 using gcc where termios.h
conflicts with sys/ioctl.h
 */

#ifdef HAVE_TERMIOS
# include <termios.h>
#else
# include <sys/ioctl.h>
#endif /* HAVE_TERMIOS */

/* Sigh. On AIX 2.3, termios.h exists but does not define TIOCGWINSZ */
/* Instead, it has to come from ioctl.h. However, As I said above, this */
/* cannot be cavalierly included on all machines, even when it exists. */
#if defined(HAVE_TERMIOS) && !defined(HAVE_TIOCGWINSZ_IN_TERMIOS_H)
# include <sys/ioctl.h>
#endif /* HAVE_TERMIOS && !HAVE_TIOCGWINSZ_IN_TERMIOS_H */

/* Who cares about AIX 2.3 anymore? I think we can be cavalier now: */
#ifdef HAVE_SYS_IOCTL_H
# include <sys/ioctl.h>
#else
# ifdef HAVE_IOCTL_H
#  include <ioctl.h>
# else
#  if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#   warning "exp_win.c wants to include either <sys/ioctl.h> or <ioctl.h>."
#  endif /* __GNUC__ && !__STRICT_ANSI__ */
# endif /* HAVE_IOCTL_H */
#endif /* HAVE_SYS_IOCTL_H */

/* SCO defines window size structure in PTEM and TIOCGWINSZ in termio.h */
/* Sigh... */
#if defined(HAVE_SYS_PTEM_H)
# include <sys/types.h>   /* for stream.h's caddr_t */
# include <sys/stream.h>  /* for ptem.h's mblk_t */
# include <sys/ptem.h>
#endif /* HAVE_SYS_PTEM_H */

#include "exp_tty.h"
#include "exp_win.h"

#ifdef TIOCGWINSZ
typedef struct winsize exp_winsize;
# define columns ws_col
# define rows ws_row
# define EXP_WIN
#endif /* TIOCGWINSZ */

#if !defined(EXP_WIN) && defined(TIOCGSIZE)
typedef struct ttysize exp_winsize;
# define columns ts_cols
# define rows ts_lines
# define EXP_WIN
#endif /* !EXP_WIN && TIOCGSIZE */

#if !defined(EXP_WIN)
typedef struct {
	int columns;
	int rows;
} exp_winsize;
#endif /* !EXP_WIN */

static exp_winsize winsize = {0, 0};
static exp_winsize win2size = {0, 0};

int exp_window_size_set(fd)
int fd;
{
	int ret = 0;
#ifdef TIOCSWINSZ
	ret = ioctl(fd,TIOCSWINSZ,&winsize);
#endif /* TIOCSWINSZ */
	if (ret == -1) {
		fprintf(stderr, "ioctl failed with errno %d: %s.\n", errno,
				strerror(errno));
	}
#if defined(TIOCSSIZE) && !defined(TIOCSWINSZ)
	ret = ioctl(fd,TIOCSSIZE,&winsize);
#endif /* TIOCSSIZE && !TIOCSWINSZ */
	return ret;
}

int exp_window_size_get(fd)
int fd;
{
	int ret = 0;
#ifdef TIOCGWINSZ
	ret = ioctl(fd,TIOCGWINSZ,&winsize);
#endif /* TIOCGWINSZ */
	if (ret == -1) {
		fprintf(stderr, "ioctl failed with errno %d: %s.\n", errno,
				strerror(errno));
	}
#if defined(TIOCGSIZE) && !defined(TIOCGWINSZ)
	ret = ioctl(fd,TIOCGSIZE,&winsize);
#endif /* TIOCGSIZE && !TIOCGWINSZ */
#if !defined(EXP_WIN)
	winsize.rows = 0;
	winsize.columns = 0;
#endif /* !EXP_WIN */
	return ret;
}

void
exp_win_rows_set(rows)
char *rows;
{
	winsize.rows = atoi(rows);
	exp_window_size_set(exp_dev_tty);
}

void
exp_win_rows_get(rows)
char *rows;
{
	exp_window_size_get(exp_dev_tty);
	sprintf(rows,"%d",winsize.rows);
}

void
exp_win_columns_set(columns)
char *columns;
{
	winsize.columns = atoi(columns);
	exp_window_size_set(exp_dev_tty);
}

void
exp_win_columns_get(columns)
char *columns;
{
	exp_window_size_get(exp_dev_tty);
	sprintf(columns,"%d",winsize.columns);
}

/*
 * separate copy of everything above - used for handling user stty requests
 */

int exp_win2_size_set(fd)
int fd;
{
	int ret = 0;
#ifdef TIOCSWINSZ
	ret = ioctl(fd,TIOCSWINSZ,&win2size);
#endif /* TIOCSWINSZ */
	if (ret == -1) {
		fprintf(stderr, "ioctl failed with errno %d: %s.\n", errno,
				strerror(errno));
	}
#if defined(TIOCSSIZE) && !defined(TIOCSWINSZ)
	ret = ioctl(fd,TIOCSSIZE,&win2size);
#endif /* TIOCSSIZE && !TIOCSWINSZ */
	return ret;
}

int exp_win2_size_get(fd)
int fd;
{
	int ret = 0;
#ifdef TIOCGWINSZ
	ret = ioctl(fd,TIOCGWINSZ,&win2size);
#endif /* TIOCGWINSZ */
	if (ret == -1) {
		fprintf(stderr, "ioctl failed with errno %d: %s.\n", errno,
				strerror(errno));
	}
#if defined(TIOCGSIZE) && !defined(TIOCGWINSZ)
	ret = ioctl(fd,TIOCGSIZE,&win2size);
#endif /* TIOCGSIZE && !TIOCGWINSZ */
	return ret;
}

void
exp_win2_rows_set(fd,rows)
int fd;
char *rows;
{
	exp_win2_size_get(fd);
	win2size.rows = atoi(rows);
	exp_win2_size_set(fd);
}

void
exp_win2_rows_get(fd,rows)
int fd;
char *rows;
{
	exp_win2_size_get(fd);
	sprintf(rows,"%d",win2size.rows);
#if !defined(EXP_WIN)
	win2size.rows = 0;
	win2size.columns = 0;
#endif /* !EXP_WIN */
}

void
exp_win2_columns_set(fd,columns)
int fd;
char *columns;
{
	exp_win2_size_get(fd);
	win2size.columns = atoi(columns);
	exp_win2_size_set(fd);
}

void
exp_win2_columns_get(fd,columns)
int fd;
char *columns;
{
	exp_win2_size_get(fd);
	sprintf(columns,"%d",win2size.columns);
}

/* EOF */
