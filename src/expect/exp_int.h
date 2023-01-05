/* exp_int.h - private symbols common to both expect program and library

Written by: Don Libes, libes@cme.nist.gov, NIST, 12/3/90

Design and implementation of this program was paid for by U.S. tax
dollars. Therefore it is public domain. However, the author and NIST
would appreciate credit if this program or parts of it are used.
*/

#ifndef _EXPECT_INT_H
#define _EXPECT_INT_H

#ifndef TRUE
# define FALSE 0
# define TRUE 1
#endif /* !TRUE */

#if !defined(HAVE_MEMCPY) && !defined(memcpy)
# define memcpy(x,y,len) bcopy(y,x,len)
#endif /* !HAVE_MEMCPY && !memcpy */

#include <errno.h>

int		Exp_StringMatch();
int		Exp_StringMatch2();
void		exp_console_set _ANSI_ARGS_((void));

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
#    warning exp_int.h expects a header that provides malloc() to be included.
#   endif /* HAVE_MALLOC_MALLOC_H */
#  endif /* HAVE_MALLOC_H */
# endif /* HAVE_STDLIB_H */
#endif /*NO_STDLIB_H*/

#endif /* _EXPECT_INT_H */
