/* Imitation of sys/time.h. */

#ifndef __SYS_TIME_H__
#define __SYS_TIME_H__

#include <time.h>

#ifdef __need_struct_timeval
# undef __need_struct_timeval
# ifndef _STRUCT_TIMEVAL
#  define _STRUCT_TIMEVAL struct timeval
_STRUCT_TIMEVAL
{
  long tv_sec; /* seconds */
  long tv_usec; /* and microseconds */
};
# endif /* !_STRUCT_TIMEVAL */
#endif /* __need_struct_timeval */

#endif /* __SYS_TIME_H__ */

/* EOF */
