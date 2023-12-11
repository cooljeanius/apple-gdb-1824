dnl#                                               -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.64])
AC_INIT([rlfe],[0.4],[/dev/null])
AC_CONFIG_SRCDIR([rlfe.c])
AC_CONFIG_HEADERS([config.h])
VERSION=0.4
AC_SUBST([VERSION])dnl
AC_CANONICAL_TARGET

dnl#
dnl# Define some useful macros
dnl#
AC_DEFUN([AC_PROGRAM_SOURCE],
[AC_REQUIRE([AC_PROG_CPP])dnl
AC_REQUIRE([AC_PROG_SED])dnl
AC_PROVIDE([$0])
cat > conftest.c <<EOF
#include "confdefs.h"
[$1]
_CUT_HERE_
[$2]
EOF
eval "${ac_cpp} conftest.c 2>&5 | sed -e '1,/_CUT_HERE_/d' -e 's/ //g' > conftest.out"
. ./conftest.out
rm -f conftest*
])dnl#
dnl#
define([AC_NOTE],
[echo "$1" 1>&AS_MESSAGE_FD
])dnl#

# Checks for programs.
old_CFLAGS="${CFLAGS}"
AC_PROG_CC
AC_PROG_CPP
AC_PROG_GCC_TRADITIONAL
AC_PROG_LN_S
AC_PROG_INSTALL
AC_SEARCH_LIBS([strerror],[cposix])dnl

AS_MESSAGE([checking if your ${CC} actually works])
AC_RUN_IFELSE([AC_LANG_SOURCE([[
#include <stdlib.h>
int main(){exit(0);}
]])],[
AC_NOTE([- Your ${CC} works])
],[
if test "x${CC}" != "xcc"; then
AC_NOTE([- Your ${CC} failed - restarting with CC=cc])
AC_NOTE([])
CC=cc
export CC
exec $0 ${configure_args}
fi
],[
AC_NOTE([- cross-compiling, not checking])
])dnl

AS_MESSAGE([checking if the compiler actually works (i.e. runs)])
AC_RUN_IFELSE([AC_LANG_SOURCE([[
#include <stdlib.h>
int main(){exit(0);}
]])],[
AC_NOTE([- Your compiler works])
],[
exec 5>&2
eval ${ac_link}
AC_NOTE([CC=${CC}];[CFLAGS=${CFLAGS}];[LIBS=${LIBS};])
AC_NOTE([${ac_compile}])
AC_MSG_ERROR([Cannot run the compiler - sorry])
],[
AC_NOTE([- cross-compiling, not checking])
])dnl

AS_MESSAGE([checking if your compiler sets the exit status])
AC_RUN_IFELSE([AC_LANG_SOURCE([[
int main(void)
{
  int __something_strange_();
  __something_strange_(0);
}
]])],[AC_MSG_ERROR([Your compiler does not set the exit status - sorry])
],[
AC_NOTE([- Your compiler sets the exit status])
],[
AC_NOTE([- cross-compiling, not checking])
])dnl

AC_PROG_AWK

if test -f etc/toolcheck; then
AS_MESSAGE([checking for buggy tools...])
sh etc/toolcheck 1>&AS_MESSAGE_FD
fi

# Checks for libraries.
AC_MSG_CHECKING([for linker flag to verify linked libraries have the correct architecture])
# Make sure this is done up here first:
if test "x${acl_cv_wl}" = "x"; then
  test -z "${acl_cv_wl}" && export acl_cv_wl='-Wl,'
fi
pre_saved_LDFLAGS="${LDFLAGS}"
LDFLAGS="${LDFLAGS} ${acl_cv_wl}-arch_errors_fatal"
AC_LINK_IFELSE([AC_LANG_PROGRAM([[]],[[]])],
               [LD_ARCHFLAGS="${acl_cv_wl}-arch_errors_fatal"],[])
LDFLAGS="${pre_saved_LDFLAGS}"
if test "x${LD_ARCHFLAGS}" != "x"; then
  test -n "${LD_ARCHFLAGS}"
  AC_MSG_RESULT([${LD_ARCHFLAGS}])
  AC_SUBST([LD_ARCHFLAGS])dnl
  ## that should be all we need to do with them for now...
else
  test -z "${LD_ARCHFLAGS}"
  AC_MSG_RESULT([none])
fi
# check arch when "main()" is the only function for which we check:
ARCHLESS_LDFLAGS="${LDFLAGS}"
LDFLAGS="${LDFLAGS} ${LD_ARCHFLAGS}"
# FIXME: Replace `main' with a function in `-lhistory':
AC_CHECK_LIB([history],[main])dnl
# FIXME: Replace `main' with a function in `-lncurses':
AC_CHECK_LIB([ncurses],[main])dnl
# FIXME: Replace `main' with a function in `-lreadline':
AC_CHECK_LIB([readline],[main])dnl
  ## reset these:
LDFLAGS="${ARCHLESS_LDFLAGS}"

# Checks for header files.
AC_HEADER_MAJOR
AC_HEADER_STAT
AC_CHECK_HEADERS([arpa/inet.h curses.h errno.h fcntl.h grp.h netdb.h \
                  netinet/in.h secure/_string.h sgtty.h signal.h \
                  stdio.h sys/ioctl.h sys/param.h sys/pty.h \
                  sys/select.h sys/sioctl.h sys/socket.h sys/stropts.h \
                  sys/sysmacros.h sys/time.h sys/ttold.h sys/tty.h \
                  sys/un.h term.h termio.h termios.h utmp.h utmpx.h])dnl
# Force use of our own headers:
AH_BOTTOM([
#define READLINE_LIBRARY
])dnl

# Checks for typedefs, structures, and compiler characteristics.
AC_C_BIGENDIAN
AC_C_PROTOTYPES
AC_TYPE_UID_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_CHECK_MEMBERS([struct stat.st_rdev])dnl

# Checks for library functions.
if test "x${GITHUB_ACTIONS}" = "xtrue" && test "x${CODEQL_ACTION_FEATURE_WILL_UPLOAD}" = "xtrue"; then
  export ac_cv_func_chown_works=yes
fi
AC_FUNC_CHOWN
if test "x${GITHUB_ACTIONS}" = "xtrue" && test "x${CODEQL_ACTION_FEATURE_WILL_UPLOAD}" = "xtrue"; then
  export ac_cv_func_fork_works=yes
  export ac_cv_func_vfork_works=yes
fi
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_FUNC_STAT
AC_CHECK_FUNCS([alarm dup2 exit memcpy memmove mkfifo mknod \
                printf rl_getc select strcmp system])dnl

dnl#
dnl#   ****  special unix variants  ****
dnl#

AS_MESSAGE([checking for System V...])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>]],[[int x = SIGCHLD | FNDELAY;]])],[
AC_NOTE([- Not defining SYSV])
],[AC_NOTE([- Defining SYSV])
AC_DEFINE([SYSV],[1],[Define to 1 for System V])
])dnl

AS_MESSAGE([checking for Solaris 2.x...])
AC_EGREP_CPP([this_is_solaris],
[#if defined(SVR4) && defined(sun)
  this_is_solaris
#endif /* SVR4 && sun */
],[AC_NOTE([- using Solaris libs])
LIBS="${LIBS} -lsocket -lnsl -lkstat"
],[
AC_NOTE([- not on Solaris])
])dnl

dnl#
dnl#    ****  select()  ****
dnl#

AS_MESSAGE([checking select...])
AC_LINK_IFELSE([AC_LANG_PROGRAM([[
#include <sys/select.h>
]],[[select(0, 0, 0, 0, 0);]])],[
AC_NOTE([- found select by itself])
],[ 
LIBS="${LIBS} -lnet -lnsl"
AS_MESSAGE([checking select with ${LIBS}])
AC_LINK_IFELSE([AC_LANG_PROGRAM([[
#include <sys/select.h>
]],[[select(0, 0, 0, 0, 0);]])],[
AC_NOTE([- found select with ${LIBS}])
],[ 
AC_MSG_ERROR([!!! no select - no screen])])
])
dnl#
dnl#   ****  check the select implementation ****
dnl#

AS_MESSAGE([checking select return value...])
sleep 1
AC_RUN_IFELSE([AC_LANG_SOURCE([[
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#line 225 "configure.ac"

char *nam = "/tmp/conftest$$";

#ifdef NAMEDPIPE

#ifndef O_NONBLOCK
# define O_NONBLOCK O_NDELAY
#endif /* !O_NONBLOCK */
#ifndef S_IFIFO
# define S_IFIFO 0010000
#endif /* !S_IFIFO */

int main(void)
{
#ifdef FD_SET
  fd_set f;
#else
  int f;
#endif /* FD_SET */

#ifdef __FreeBSD__
/* From Andrew A. Chernov (ache@astral.msk.su):
 * opening RDWR fifo fails in BSD 4.4, but select return values are
 * right.
 */
  exit(0);
#endif /* __FreeBSD__ */
  (void)alarm(5);
#ifdef POSIX
  if (mkfifo(nam, 0777))
#else
  if (mknod(nam, S_IFIFO|0777, 0))
#endif /* POSIX */
    exit(1);
  close(0);
  if (open(nam, O_RDWR | O_NONBLOCK))
    exit(1);
  if (write(0, "TEST", 4) == -1)
    exit(1);

#else

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <signal.h>

int main(void)
{
  int s1, s2, l;
  struct sockaddr_un a;
#ifdef FD_SET
  fd_set f;
#else
  int f;
#endif /* FD_SET */

  (void)alarm(5);
  if ((s1 = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
    exit(1);
  a.sun_family = AF_UNIX;
  strcpy(a.sun_path, nam);
  (void) unlink(nam);
  if (bind(s1, (struct sockaddr *) &a, strlen(nam)+2) == -1)
    exit(1);
  if (listen(s1, 2))
    exit(1);
  if (fork() == 0)
    {
      if ((s2 = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
	kill(getppid(), 3);
      (void)connect(s2, (struct sockaddr *)&a, strlen(nam) + 2);
      if (write(s2, "HELLO", 5) == -1)
	kill(getppid(), 3);
      exit(0);
    }
  l = sizeof(a);
  close(0);
  if (accept(s1, (struct sockaddr *)&a, &l))
    exit(1);
#endif /* NAMEDPIPE */


#ifdef FD_SET
  FD_SET(0, &f);
#else
  f = 1;
#endif /* FD_SET */
  if (select(1, &f, 0, 0, 0) == -1)
    exit(1);
  if (select(1, &f, &f, 0, 0) != 2)
    exit(1);
  exit(0);
}
]])],[
AC_NOTE([- select is ok])
],[
AC_NOTE([- select cannot count])
AC_DEFINE([SELECT_BROKEN],[1],[Define to 1 if select is broken])
],[
AC_NOTE([- cross-compiling, not checking])
])
sleep 1

dnl#
dnl#   ****  termcap or terminfo  ****
dnl#
AS_MESSAGE([checking for tgetent...])
AC_LINK_IFELSE([AC_LANG_PROGRAM([[
#ifdef HAVE_CURSES_H
# include <curses.h>
#endif /* HAVE_CURSES_H */
#ifdef HAVE_TERM_H
# include <term.h>
#endif /* HAVE_TERM_H */
]],[[tgetent((char *)0, (char *)0);]])],[
  AC_NOTE([- found tgetent by itself])
  ],[
  olibs="${LIBS}"
  LIBS="-lcurses ${olibs}"
  AS_MESSAGE([checking libcurses])
  AC_LINK_IFELSE([AC_LANG_PROGRAM([[
#ifdef HAVE_CURSES_H
# include <curses.h>
#endif /* HAVE_CURSES_H */
#ifdef HAVE_TERM_H
# include <term.h>
#endif /* HAVE_TERM_H */
  ]],[[
#ifdef __hpux
__sorry_hpux_libcurses_is_totally_broken_in_10_10();
#else
tgetent((char *)0, (char *)0);
#endif /* __hpux */
  ]])],[
    AC_NOTE([- found tgetent with libcurses])
    ],[
    LIBS="-ltermcap ${olibs}"
    AS_MESSAGE([checking libtermcap])
    AC_LINK_IFELSE([AC_LANG_PROGRAM([[
#ifdef HAVE_CURSES_H
# include <curses.h>
#endif /* HAVE_CURSES_H */
#ifdef HAVE_TERM_H
# include <term.h>
#endif /* HAVE_TERM_H */
]],[[tgetent((char *)0, (char *)0);]])],[
      AC_NOTE([- found tgetent with libtermcap])
      ],[
      LIBS="-ltermlib ${olibs}"
      AS_MESSAGE([checking libtermlib])
      AC_LINK_IFELSE([AC_LANG_PROGRAM([[
#ifdef HAVE_CURSES_H
# include <curses.h>
#endif /* HAVE_CURSES_H */
#ifdef HAVE_TERM_H
# include <term.h>
#endif /* HAVE_TERM_H */
]],[[tgetent((char *)0, (char *)0);]])],[
        AC_NOTE([- found tgetent with libtermlib])
        ],[
        LIBS="-lncurses ${olibs}"
        AS_MESSAGE([checking libncurses])
        AC_LINK_IFELSE([AC_LANG_PROGRAM([[
#ifdef HAVE_CURSES_H
# include <curses.h>
#endif /* HAVE_CURSES_H */
#ifdef HAVE_TERM_H
# include <term.h>
#endif /* HAVE_TERM_H */
]],[[tgetent((char *)0, (char *)0);]])],[
          AC_NOTE([- found tgetent with libncurses])
          ],[
          AC_MSG_ERROR([!!! no tgetent - no screen])
        ])dnl
      ])dnl
    ])dnl
  ])dnl
])dnl

AS_MESSAGE([checking terminfo database])
sleep 1
AC_RUN_IFELSE([AC_LANG_SOURCE([[
#ifdef __APPLE__
# error "Erroring out early (while compiling) to prevent runtime errors"
#endif /* __APPLE__ */
#include <stdlib.h>
#include <string.h>
#ifdef HAVE_CURSES_H
# include <curses.h>
#endif /* HAVE_CURSES_H */
#ifdef HAVE_TERM_H
# include <term.h>
#endif /* HAVE_TERM_H */
#line 420 "configure.ac"
int main(void)
{
 exit(strcmp(tgoto("%p1%d", 0, 1), "1") ? 0 : 1);
}
]])],[
AC_NOTE([- you use the termcap database])
],[
AC_NOTE([- you use the terminfo database])
AC_DEFINE([TERMINFO],[1],[Define to 1 if you use the terminfo database])
],[
AC_NOTE([- cross-compiling, not checking])
])
sleep 1

AS_MESSAGE([checking ospeed...])
AC_LINK_IFELSE([AC_LANG_PROGRAM([[extern short ospeed;]],[[ospeed=5;]])],[
AC_NOTE([- found ospeed])
],[
AC_NOTE([- need ospeed])
AC_DEFINE([NEED_OSPEED],[1],[Define to 1 if you need ospeed])dnl
])dnl

dnl#
dnl#   ****  PTY specific things  ****
dnl#
AS_MESSAGE([checking for /dev/ptc...])
if test -r /dev/ptc; then
  AC_NOTE([- yes, we have /dev/ptc])
  AC_DEFINE([HAVE_DEV_PTC],[1],[Define to 1 if you have /dev/ptc])
else
  AC_NOTE([- no, /dev/ptc is not present])
fi

AS_MESSAGE([checking for SVR4 ptys...])
sysvr4ptys=""
if test -c /dev/ptmx ; then
AC_LINK_IFELSE([AC_LANG_PROGRAM([[
#include <stdlib.h>
]],[[ptsname(0);grantpt(0);unlockpt(0);]])],[
AC_NOTE([- you have SVR4 ptys])
AC_DEFINE([HAVE_SVR4_PTYS],[1],[Define to 1 if you have SVR4 ptys])
sysvr4ptys=1
],[
AC_NOTE([- you are missing SVR4 ptys])
])
fi

AC_CHECK_FUNCS([getpt ptsname])dnl

AC_CHECK_DECLS([ptsname],[],[],[
#include <stdlib.h>
])dnl

dnl# check for openpty()
if test -z "${sysvr4ptys}"; then
AC_CHECK_FUNCS([openpty],[],
[AC_CHECK_LIB([util],[openpty],[
  AC_DEFINE([HAVE_OPENPTY],[1],[Define to 1 if we have the openpty func])
  LIBS="${LIBS} -lutil"])
])
fi

AS_MESSAGE([checking for ptyranges...])
if test -d /dev/ptym ; then
  pdir='/dev/ptym'
else
  pdir='/dev'
fi
dnl# SCO uses ptyp%d
AC_EGREP_CPP([hello_m_unix],
[#ifdef M_UNIX
   hello_m_unix;
#endif /* M_UNIX */
],[ptys=`echo /dev/ptyp??`
AC_NOTE([- ptys end in "p"])
],[ptys=`echo ${pdir}/pty??`
AC_NOTE([- ptys are not required to end in "p"])
])
dnl# if test -c /dev/ptyp19; then
dnl# ptys=`echo /dev/ptyp??`
dnl# else
dnl# ptys=`echo ${pdir}/pty??`
dnl# fi
if test "${ptys}" != "${pdir}/pty??" ; then
p0=`echo ${ptys} | tr ' ' '\012' | sed -e 's/^.*\(.\).$/\1/g' | sort -u | tr -d '\012'`
p1=`echo ${ptys} | tr ' ' '\012' | sed -e 's/^.*\(.\)$/\1/g'  | sort -u | tr -d '\012'`
AC_DEFINE_UNQUOTED([PTYRANGE0],["${p0}"],[ptyrange0])
AC_DEFINE_UNQUOTED([PTYRANGE1],["${p1}"],[ptyrange1])
fi

dnl#   ****  pty mode/group handling ****
dnl#
dnl# support provided by Luke Mewburn <lm@rmit.edu.au>, 931222
AC_ARG_WITH([pty-mode],
            [AS_HELP_STRING([--with-pty-mode=mode],
                            [default mode for ptys])],
            [ptymode="${withval}"])dnl
AC_ARG_WITH([pty-group],
            [AS_HELP_STRING([--with-pty-group=group],
                            [default group for ptys])],
            [ptygrp="${withval}"])
test -n "${ptymode}" || ptymode=0620
if test -n "${ptygrp}" ; then
	AC_DEFINE_UNQUOTED([PTYMODE],[${ptymode}],[ptymode])
	AC_DEFINE_UNQUOTED([PTYGROUP],[${ptygrp}],[ptygroup])
else

AS_MESSAGE([checking default tty permissions/group...])
rm -f conftest_grp
sleep 1
AC_RUN_IFELSE([AC_LANG_SOURCE([[
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#line 537 "configure.ac"
int main(void)
{
  struct stat sb;
  char *x,*ttyname();
  int om, m;
  FILE *fp;

  if (!(x = ttyname(0))) exit(1);
  if (stat(x, &sb)) exit(1);
  om = sb.st_mode;
  if (om & 002) exit(0);
  m = system("mesg y");
  if (m == -1 || m == 127) exit(1);
  if (stat(x, &sb)) exit(1);
  m = sb.st_mode;
  if (chmod(x, om)) exit(1);
  if (m & 002) exit(0);
  if (sb.st_gid == getgid()) exit(1);
  if (!(fp=fopen("conftest_grp", "w")))
    exit(1);
  fprintf(fp, "%d\n", sb.st_gid);
  fclose(fp);
  exit(0);
}
]])],[
    if test -f conftest_grp; then
	ptygrp=`cat conftest_grp`
	    AC_NOTE([- pty mode: ${ptymode}, group: ${ptygrp}])
	    AC_DEFINE_UNQUOTED([PTYMODE],[${ptymode}],[ptymode])
	    AC_DEFINE_UNQUOTED([PTYGROUP],[${ptygrp}],[ptygroup])
    else
	    AC_NOTE([- ptys are world accessable])
    fi
],[
    AC_NOTE([- we need to check some other things first before we can check default tty permissions/group])
    WRITEPATH=''
    XTERMPATH=''
    AC_PATH_PROG([WRITEPATH],[write])
    AC_PATH_PROG([XTERMPATH],[xterm])
    found=""
    if test -n "${WRITEPATH}${XTERMPATH}"; then
      findfollow=""
      lsfollow=""
      found=`find $WRITEPATH ${XTERMPATH} -follow -print 2>/dev/null`
      if test -n "${found}"; then
	    findfollow=-follow
	    lsfollow=L
      fi
      if test -n "${XTERMPATH}"; then
	    ptygrpn=`ls -l${lsfollow} ${XTERMPATH} | sed -n -e 1p | ${AWK} '{print $4}'`
	    if test "xtty" != "x${ptygrpn}"; then
	      XTERMPATH=""
	    fi
      fi
    fi
    if test -n "${WRITEPATH}${XTERMPATH}"; then
      found=`find ${WRITEPATH} ${XTERMPATH} ${findfollow} -perm -2000 -print` 
      if test -n "${found}"; then
	    ptygrp=`ls -ln${lsfollow} ${found} | sed -n -e 1p | ${AWK} '{print $4}'`
	    AC_NOTE([- pty mode: ${ptymode}, group: ${ptygrp}])
	    AC_DEFINE_UNQUOTED([PTYMODE],[${ptymode}],[ptymode])
	    AC_DEFINE_UNQUOTED([PTYGROUP],[${ptygrp}],[ptygroup])
      else
	    AC_NOTE([- ptys are world accessable])
      fi
    else
      AC_NOTE([- cannot determine - assume ptys are world accessable])
    fi
],[
    AC_NOTE([- cross-compiling, not checking])
])
rm -f conftest_grp
fi

sleep 1

dnl#
dnl#   ****  signal handling  ****
dnl#
if test -n "${posix}"; then
  dnl# POSIX has reliable signals with void return type.
  AC_NOTE([assuming posix signal definition])
  AC_DEFINE([SIGVOID],[1],
            [Define to 1 if signals have a void return type])dnl
  ## nothing else to do here...
else

AS_MESSAGE([checking return type of signal handlers...])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <sys/types.h>
#include <signal.h>
#ifdef signal
# undef signal
#endif /* signal */
#line 631 "configure.ac"
extern void (*signal ()) ();]],[[int i;]])],[
AC_NOTE([- signals return void])
AC_DEFINE([SIGVOID],[1],[Define to 1 if signals have a void return type])
],[
AC_NOTE([- signals return something other than void])
])
AS_MESSAGE([checking sigset...])
AC_LINK_IFELSE([AC_LANG_PROGRAM([[
#include <sys/types.h>
#include <signal.h>
]],[[
#line 643 "configure.ac"
#ifdef SIGVOID
sigset(0, (void (*)())0);
#else
sigset(0, (int (*)())0);
#endif /* SIGVOID */
]])],[
AC_NOTE([- using sigset])
AC_DEFINE([USESIGSET],[1],[Define to 1 to use sigset])
],[
AC_NOTE([- not using sigset])
])
AS_MESSAGE([checking signal implementation...])
AC_RUN_IFELSE([AC_LANG_SOURCE([[
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>

#line 661 "configure.ac"

#ifndef SIGCLD
# define SIGCLD SIGCHLD
#endif /* !SIGCLD */
#ifdef USESIGSET
# define signal sigset
#endif /* USESIGSET */

int got;

#ifdef SIGVOID
void
#endif /* SIGVOID */
hand(void)
{
  got++;
}

int main(void)
{
  /* on hpux we use sigvec to get bsd signals */
#ifdef __hpux
  (void)signal(SIGCLD, hand);
  kill(getpid(), SIGCLD);
  kill(getpid(), SIGCLD);
  if (got < 2)
    exit(1);
#endif /* __hpux */
  exit(0);
}
]])],[
AC_NOTE([- signals that pass the conftest])
],[
AC_NOTE([- SYSV signals])
AC_DEFINE([SYSVSIGS],[1],[Define to 1 for SYSV signals])dnl
],[
AC_NOTE([- cross-compiling, not checking])
])dnl

fi

# Output.
AC_CONFIG_FILES([Makefile])dnl
AC_OUTPUT
