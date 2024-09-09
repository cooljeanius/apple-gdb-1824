/* waitpid.c */

/*

@deftypefn Supplemental int waitpid (int @var{pid}, int *@var{status}, int)

This is a wrapper around the @code{wait} function.  Any ``special''
values of @var{pid} depend on your implementation of @code{wait}, as
does the return value.  The third argument is unused in @libib{}.

@end deftypefn

*/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */
#include "ansidecl.h"

/* On some systems (such as WindISS), you must include <sys/types.h>
   to get the definition of "pid_t" before you include <sys/wait.h>.  */
#include <sys/types.h>

#ifdef HAVE_SYS_WAIT_H
# include <sys/wait.h>
#else
# ifdef __MINGW32__
#  include <process.h>
#  define wait(s)  _cwait(s,pid,_WAIT_CHILD)
# else
#  if defined(__GNUC__) && !defined(__STRICT_ANSI__) && !defined(__PEDANTIC__)
#   warning "waitpid.c expects <sys/wait.h> to be included for wait()."
#  endif /* __GNUC__ && !__STRICT_ANSI__ && !__PEDANTIC__ */
# endif /* __MINGW32__ */
#endif /* HAVE_SYS_WAIT_H */

pid_t
waitpid(pid_t pid, int *stat_loc, int options ATTRIBUTE_UNUSED)
{
  for (;;)
    {
      int wpid = wait(stat_loc);
      if ((wpid == pid) || (wpid == -1))
	return wpid;
    }
}

/* EOF */
