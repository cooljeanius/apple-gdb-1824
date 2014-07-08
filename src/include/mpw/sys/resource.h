/* Imitation of sys/resource.h */
#ifndef __SYS_RESOURCE_H__
#define __SYS_RESOURCE_H__ 1

#ifndef __need_struct_timeval
# define __need_struct_timeval 1
#endif /* !__need_struct_timeval */

#if defined(HAVE_SYS_TIME_H) || defined(_POSIX_C_SOURCE)
# include <sys/time.h>
#else
# if defined(HAVE_SYS__STRUCTS_H) || defined(_DARWIN_C_SOURCE)
#  include <sys/_structs.h>
# else
#  include "time.h"
# endif /* HAVE_SYS__STRUCTS_H || _DARWIN_C_SOURCE */
#endif /* HAVE_SYS_TIME_H || _POSIX_C_SOURCE */

#ifndef _STRUCT_TIMEVAL
# define _STRUCT_TIMEVAL struct timeval
#endif /* !_STRUCT_TIMEVAL */

struct rusage {
  _STRUCT_TIMEVAL ru_utime;
  _STRUCT_TIMEVAL ru_stime;
};

#endif /* __SYS_RESOURCE_H__ */

/* EOF */
