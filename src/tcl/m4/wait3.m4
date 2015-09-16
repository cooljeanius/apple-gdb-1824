# wait3.m4 serial 1                                        -*- Autoconf -*-
dnl# taken from autoconf to un-obsolete it
dnl########################################################################

dnl# AC_FUNC_WAIT3
dnl# -------------
dnl# Do NOT bother too hard maintaining this macro, as it is obsoleted.
dnl# We do NOT AU define it, since we do NOT have any alternative to it
dnl# to propose; any invocation should be removed, and the code adjusted.
AN_FUNCTION([wait3],[AC_FUNC_WAIT3])dnl
AC_DEFUN([AC_FUNC_WAIT3],[
AC_CACHE_CHECK([for wait3 that fills in rusage],
	       [ac_cv_func_wait3_rusage],
[AC_RUN_IFELSE([AC_LANG_SOURCE(
[AC_INCLUDES_DEFAULT[
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
/* HP-UX has wait3 but does not fill in rusage at all: */
int
main()
{
  struct rusage r;
  int i;
  /* Use a field that we can force nonzero --
     voluntary context switches.
     For systems like NeXT and OSF/1 that do NOT set it,
     also use the system CPU time.  And page faults (I/O) for Linux: */
  r.ru_nvcsw = 0;
  r.ru_stime.tv_sec = 0;
  r.ru_stime.tv_usec = 0;
  r.ru_majflt = r.ru_minflt = 0;
  switch (fork())
    {
    case 0: /* Child: */
      sleep(1); /* Give up the CPU. */
      _exit(0);
      break;
    case -1: /* What can we do? */
      _exit(0);
      break;
    default: /* Parent: */
      wait3(&i, 0, &r);
      /* Avoid "text file busy" from rm on fast HP-UX machines: */
      sleep(2);
      return ((r.ru_nvcsw == 0) && (r.ru_majflt == 0) && (r.ru_minflt == 0)
	      && (r.ru_stime.tv_sec == 0) && (r.ru_stime.tv_usec == 0));
    }
}]])],
	       [ac_cv_func_wait3_rusage=yes],
	       [ac_cv_func_wait3_rusage=no],
	       [ac_cv_func_wait3_rusage=no])])dnl
  ## corresponding define:
if test "x${ac_cv_func_wait3_rusage}" = "xyes"; then
  AC_DEFINE([HAVE_WAIT3],[1],
	    [Define to 1 if you have the `wait3' system call.
	     Deprecated, you should no longer depend upon `wait3'.])
fi
])dnl# AC_FUNC_WAIT3


dnl# AU::AC_WAIT3
dnl# ------------
AU_ALIAS([AC_WAIT3],[AC_FUNC_WAIT3])dnl
