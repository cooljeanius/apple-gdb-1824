/* msdos.c */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include <ansidecl.h>

/* various places 'size_t' could live: */
#if defined(HAVE_STDDEF_H) || defined(STDC_HEADERS)
# include <stddef.h>
#else
# if defined(HAVE_STDIO_H) || defined(STDC_HEADERS)
#  include <stdio.h>
# else
#  if defined(HAVE_SYS_TYPES_H)
#   include <sys/types.h>
#  endif /* HAVE_SYS_TYPES_H */
# endif /* HAVE_STDIO_H || STDC_HEADERS */
#endif /* HAVE_STDDEF_H || STDC_HEADERS */

#if defined(HAVE_UNISTD_H) && \
    (!defined(write) && !defined(_UNISTD_H_) && !defined(HAVE_VFORK))
# include <unistd.h>
#else
extern
# ifdef HAVE_SSIZE_T
ssize_t
# else
void
# endif /* HAVE_SSIZE_T */
write PARAMS((int, const PTR, size_t));
#endif /* HAVE_UNISTD_H && !write && !_UNISTD_H_ && !HAVE_VFORK */

char msg[] = "No vfork available - aborting\n";
void vfork PARAMS((void))
{
  write(1, msg, sizeof(msg));
}

void sigsetmask PARAMS((void))
{
  /* no signals support in go32 (yet) */
  return;
}

int waitpid PARAMS((void))
{
  return -1;
}

/* EOF */
