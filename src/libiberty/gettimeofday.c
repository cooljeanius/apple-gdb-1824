/* gettimeofday.c */

#include "config.h"
#include "libiberty.h"

#ifdef HAVE_TIME_H
# include <time.h>
#endif /* HAVE_TIME_H */
#ifdef HAVE_SYS_TIME_H
# include <sys/time.h>
#endif /* HAVE_SYS_TIME_H */

#if !defined(_STDLIB_H_)
# if defined(HAVE_STDLIB_H) || defined(STDC_HEADERS) || defined(__STDC__)
#  include <stdlib.h> /* for abort() */
# endif /* HAVE_STDLIB_H || STDC_HEADERS || __STDC__ */
#endif /* !_STDLIB_H_ */

/*

@deftypefn Supplemental int gettimeofday (struct timeval *@var{tp}, void *@var{tz})

Writes the current time to @var{tp}.  This implementation requires
that @var{tz} be NULL.  Returns 0 on success, -1 on failure.

@end deftypefn

*/

int
gettimeofday (struct timeval *tp, void *tz)
{
  if (tz) {
    abort();
  }
  tp->tv_usec = 0;
  if (time(&tp->tv_sec) == (time_t)-1) {
    return -1;
  }
  return 0;
}

/* EOF */
