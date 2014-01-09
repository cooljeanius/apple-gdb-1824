dnl# Check type of signal routines (posix, 4.2bsd, 4.1bsd or v7)
AC_DEFUN([BASH_SYS_SIGNAL_VINTAGE],
[AC_REQUIRE([AC_TYPE_SIGNAL])
AC_MSG_CHECKING([for type of signal functions])
AC_CACHE_VAL([bash_cv_signal_vintage],
[
  AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <signal.h>]],[[
    sigset_t ss;
    struct sigaction sa;
    sigemptyset(&ss); sigsuspend(&ss);
    sigaction(SIGINT, &sa, (struct sigaction *) 0);
    sigprocmask(SIG_BLOCK, &ss, (sigset_t *) 0);
  ]])],[bash_cv_signal_vintage=posix],[
    AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <signal.h>]],[[
	int mask = sigmask(SIGINT);
	sigsetmask(mask); sigblock(mask); sigpause(mask);
    ]])],[bash_cv_signal_vintage=4.2bsd],
    [
      AC_LINK_IFELSE([AC_LANG_PROGRAM([[
	#include <signal.h>
	RETSIGTYPE foo() { }]],[[
		int mask = sigmask(SIGINT);
		sigset(SIGINT, foo); sigrelse(SIGINT);
		sighold(SIGINT); sigpause(SIGINT);
        ]])],[bash_cv_signal_vintage=svr3],[bash_cv_signal_vintage=v7])
    ])
  ])
])
AC_MSG_RESULT([$bash_cv_signal_vintage])
if test "$bash_cv_signal_vintage" = posix; then
AC_DEFINE([HAVE_POSIX_SIGNALS],[1],[Define to 1 if we have POSIX signals])
elif test "$bash_cv_signal_vintage" = "4.2bsd"; then
AC_DEFINE([HAVE_BSD_SIGNALS],[1],[Define to 1 if we have BSD signals])
elif test "$bash_cv_signal_vintage" = svr3; then
AC_DEFINE([HAVE_USG_SIGHOLD],[1],[Define to 1 if we have some obscure third type of signal])
fi
])


AC_DEFUN([BASH_SYS_REINSTALL_SIGHANDLERS],
[AC_REQUIRE([AC_TYPE_SIGNAL])
AC_REQUIRE([AC_HEADER_STDC])
AC_REQUIRE([BASH_SYS_SIGNAL_VINTAGE])
AC_MSG_CHECKING([if signal handlers must be reinstalled when invoked])
AC_CACHE_VAL([bash_cv_must_reinstall_sighandlers],
[AC_RUN_IFELSE([AC_LANG_SOURCE([[
#include <signal.h>
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif /* HAVE_UNISTD_H */

typedef RETSIGTYPE sigfunc();

int nsigint;

#ifdef HAVE_POSIX_SIGNALS
sigfunc *
set_signal_handler(sig, handler)
     int sig;
     sigfunc *handler;
{
  struct sigaction act, oact;
  act.sa_handler = handler;
  act.sa_flags = 0;
  sigemptyset (&act.sa_mask);
  sigemptyset (&oact.sa_mask);
  sigaction (sig, &act, &oact);
  return (oact.sa_handler);
}
#else
#define set_signal_handler(s, h) signal(s, h)
#endif

RETSIGTYPE
sigint(s)
int s;
{
  nsigint++;
}

main()
{
	nsigint = 0;
	set_signal_handler(SIGINT, sigint);
	kill((int)getpid(), SIGINT);
	kill((int)getpid(), SIGINT);
	exit(nsigint != 2);
}
]])],[bash_cv_must_reinstall_sighandlers=no],[bash_cv_must_reinstall_sighandlers=yes],[AC_MSG_WARN([cannot check signal handling if cross compiling -- defaulting to no])
    bash_cv_must_reinstall_sighandlers=no
])])
AC_MSG_RESULT([$bash_cv_must_reinstall_sighandlers])
if test $bash_cv_must_reinstall_sighandlers = yes; then
AC_DEFINE([MUST_REINSTALL_SIGHANDLERS],[1],[Define to 1 if it is necessary to reinstall sighandlers])
fi
])

dnl# check that some necessary job control definitions are present
AC_DEFUN([BASH_SYS_JOB_CONTROL_MISSING],
[AC_REQUIRE([AC_HEADER_STDC])
AC_REQUIRE([BASH_SYS_SIGNAL_VINTAGE])
AC_MSG_CHECKING([for presence of necessary job control definitions])
AC_CACHE_VAL([bash_cv_job_control_missing],
[AC_RUN_IFELSE([AC_LANG_SOURCE([[
#include <sys/types.h>
#ifdef HAVE_SYS_WAIT_H
# include <sys/wait.h>
#endif /* HAVE_SYS_WAIT_H */
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif /* HAVE_UNISTD_H */
#include <signal.h>

/* Add more tests in here as appropriate. */
main()
{
/* signal type */
#if !defined (HAVE_POSIX_SIGNALS) && !defined (HAVE_BSD_SIGNALS)
exit(1);
#endif

/* signals and tty control. */
#if !defined (SIGTSTP) || !defined (SIGSTOP) || !defined (SIGCONT)
exit (1);
#endif

/* process control */
#if !defined (WNOHANG) || !defined (WUNTRACED) 
exit(1);
#endif

/* Posix systems have tcgetpgrp and waitpid. */
#if defined (_POSIX_VERSION) && !defined (HAVE_TCGETPGRP)
exit(1);
#endif

#if defined (_POSIX_VERSION) && !defined (HAVE_WAITPID)
exit(1);
#endif

/* Other systems have TIOCSPGRP/TIOCGPRGP and wait3. */
#if !defined (_POSIX_VERSION) && !defined (HAVE_WAIT3)
exit(1);
#endif

exit(0);
}]])],[bash_cv_job_control_missing=present],[bash_cv_job_control_missing=missing],[AC_MSG_WARN([cannot check job control if cross-compiling -- defaulting to missing])
     bash_cv_job_control_missing=missing
])])
AC_MSG_RESULT([$bash_cv_job_control_missing])
if test $bash_cv_job_control_missing = missing; then
AC_DEFINE([JOB_CONTROL_MISSING],[1],[Define to 1 if job control is missing])
fi
])
AC_DEFUN([BASH_HAVE_TIOCGWINSZ],
[AC_MSG_CHECKING([for TIOCGWINSZ in sys/ioctl.h])
AC_CACHE_VAL([bash_cv_tiocgwinsz_in_ioctl],
[AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <sys/types.h>
#include <sys/ioctl.h>]],[[int x = TIOCGWINSZ;]])],[bash_cv_tiocgwinsz_in_ioctl=yes],[bash_cv_tiocgwinsz_in_ioctl=no])])
AC_MSG_RESULT([$bash_cv_tiocgwinsz_in_ioctl])
if test $bash_cv_tiocgwinsz_in_ioctl = yes; then   
AC_DEFINE([GWINSZ_IN_SYS_IOCTL],[1],[Define to 1 if TIOCGWINSZ is in sys/ioctl.h])
fi
])

AC_DEFUN([BASH_HAVE_TIOCSTAT],
[AC_MSG_CHECKING([for TIOCSTAT in sys/ioctl.h])
AC_CACHE_VAL([bash_cv_tiocstat_in_ioctl],
[AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <sys/types.h>
#include <sys/ioctl.h>]],[[int x = TIOCSTAT;]])],[bash_cv_tiocstat_in_ioctl=yes],[bash_cv_tiocstat_in_ioctl=no])])
AC_MSG_RESULT([$bash_cv_tiocstat_in_ioctl])
if test $bash_cv_tiocstat_in_ioctl = yes; then   
AC_DEFINE([TIOCSTAT_IN_SYS_IOCTL],[1],[Define to 1 if TIOCSTAT is in sys/ioctl.h])
fi
])

AC_DEFUN([BASH_HAVE_FIONREAD],
[AC_MSG_CHECKING([for FIONREAD in sys/ioctl.h])
AC_CACHE_VAL([bash_cv_fionread_in_ioctl],
[AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <sys/types.h>
#include <sys/ioctl.h>]],[[int x = FIONREAD;]])],[bash_cv_fionread_in_ioctl=yes],[bash_cv_fionread_in_ioctl=no])])
AC_MSG_RESULT([$bash_cv_fionread_in_ioctl])
if test $bash_cv_fionread_in_ioctl = yes; then   
AC_DEFINE([FIONREAD_IN_SYS_IOCTL],[1],[Define to 1 if FIONREAD sys/ioctl.h])
fi
])

AC_DEFUN([BASH_STRUCT_TERMIOS_LDISC],
[
AC_CHECK_MEMBER([struct termios.c_line],[AC_DEFINE([TERMIOS_LDISC],[1],[Define to 1 if the termios struct has the c_line member])],[],[
#include <sys/types.h>
#include <termios.h>
])
])

AC_DEFUN([BASH_STRUCT_TERMIO_LDISC],
[
AC_CHECK_MEMBER([struct termio.c_line],[AC_DEFINE([TERMIO_LDISC],[1],[Define to 1 if the termio struct has the c_line member])],[],[
#include <sys/types.h>
#include <termio.h>
])
])

AC_DEFUN([BASH_STRUCT_WINSIZE],
[AC_MSG_CHECKING([for struct winsize in sys/ioctl.h and termios.h])
AC_CACHE_VAL([bash_cv_struct_winsize_header],
[AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <sys/types.h>
#include <sys/ioctl.h>]],[[struct winsize x;]])],[bash_cv_struct_winsize_header=ioctl_h],[
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <sys/types.h>
#include <termios.h>]],[[struct winsize x;]])],
  [bash_cv_struct_winsize_header=termios_h],[bash_cv_struct_winsize_header=other])
])])
if test $bash_cv_struct_winsize_header = ioctl_h; then
  AC_MSG_RESULT([sys/ioctl.h])
  AC_DEFINE([STRUCT_WINSIZE_IN_SYS_IOCTL],[1],[Define to 1 if the winsize struct is in sys/ioctl.h])
elif test $bash_cv_struct_winsize_header = termios_h; then
  AC_MSG_RESULT([termios.h])
  AC_DEFINE([STRUCT_WINSIZE_IN_TERMIOS],[1],[Define to 1 if the winsize struct is in termios.h])
else
  AC_MSG_RESULT([not found])
fi
])


dnl#
dnl# See if speed_t is declared in <sys/types.h>. Some versions of linux
dnl# require a definition of speed_t each time <termcap.h> is included,
dnl# but you can only get speed_t if you include <termios.h> (on some
dnl# versions) or <sys/types.h> (on others).
dnl#
AC_DEFUN([BASH_CHECK_SPEED_T],
[AC_REQUIRE([AC_HEADER_STDC])
AC_REQUIRE([AC_PROG_CC])
AC_MSG_CHECKING([for speed_t in sys/types.h])
AC_CACHE_VAL(bash_cv_speed_t_in_sys_types,
[AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <sys/types.h>]],[[speed_t x;]])],[bash_cv_speed_t_in_sys_types=yes],[bash_cv_speed_t_in_sys_types=no])])
AC_MSG_RESULT([$bash_cv_speed_t_in_sys_types])
if test $bash_cv_speed_t_in_sys_types = yes; then   
AC_DEFINE([SPEED_T_IN_SYS_TYPES],[1],[Define to 1 if speed_t is in sys/types.h])
fi
])


#--------------------------------------------------------------------
# SC_SERIAL_PORT
#
#	Determine which interface to use to talk to the serial port.
#	Note that #include lines must begin in leftmost column for
#	some compilers to recognize them as preprocessor directives,
#	and some build environments have stdin not pointing at a
#	pseudo-terminal (usually /dev/null instead.)
#
# Arguments:
#	none
#	
# Results:
#
#	Defines only one of the following vars:
#		HAVE_SYS_MODEM_H
#		USE_TERMIOS
#		USE_TERMIO
#		USE_SGTTY
#
#--------------------------------------------------------------------

AC_DEFUN([SC_SERIAL_PORT],[
    AC_CHECK_HEADERS([sys/modem.h])
    AC_MSG_CHECKING([termios vs. termio vs. sgtty])
    AC_CACHE_VAL([tcl_cv_api_serial],[
    AC_RUN_IFELSE([AC_LANG_SOURCE([[
#include <termios.h>

int main() {
    struct termios t;
    if (tcgetattr(0, &t) == 0) {
	cfsetospeed(&t, 0);
	t.c_cflag |= PARENB | PARODD | CSIZE | CSTOPB;
	return 0;
    }
    return 1;
}]])],[tcl_cv_api_serial=termios],[tcl_cv_api_serial=no],[tcl_cv_api_serial=no])
    if test $tcl_cv_api_serial = no ; then
	AC_RUN_IFELSE([AC_LANG_SOURCE([[
#include <termio.h>

int main() {
    struct termio t;
    if (ioctl(0, TCGETA, &t) == 0) {
	t.c_cflag |= CBAUD | PARENB | PARODD | CSIZE | CSTOPB;
	return 0;
    }
    return 1;
}]])],[tcl_cv_api_serial=termio],[tcl_cv_api_serial=no],[tcl_cv_api_serial=no])
    fi
    if test $tcl_cv_api_serial = no ; then
	AC_RUN_IFELSE([AC_LANG_SOURCE([[
#include <sgtty.h>

int main() {
    struct sgttyb t;
    if (ioctl(0, TIOCGETP, &t) == 0) {
	t.sg_ospeed = 0;
	t.sg_flags |= ODDP | EVENP | RAW;
	return 0;
    }
    return 1;
}]])],[tcl_cv_api_serial=sgtty],[tcl_cv_api_serial=no],[tcl_cv_api_serial=no])
    fi
    if test $tcl_cv_api_serial = no ; then
	AC_RUN_IFELSE([AC_LANG_SOURCE([[
#include <termios.h>
#include <errno.h>

int main() {
    struct termios t;
    if (tcgetattr(0, &t) == 0
	|| errno == ENOTTY || errno == ENXIO || errno == EINVAL) {
	cfsetospeed(&t, 0);
	t.c_cflag |= PARENB | PARODD | CSIZE | CSTOPB;
	return 0;
    }
    return 1;
}]])],[tcl_cv_api_serial=termios],[tcl_cv_api_serial=no],[tcl_cv_api_serial=no])
    fi
    if test $tcl_cv_api_serial = no; then
	AC_RUN_IFELSE([AC_LANG_SOURCE([[
#include <termio.h>
#include <errno.h>

int main() {
    struct termio t;
    if (ioctl(0, TCGETA, &t) == 0
	|| errno == ENOTTY || errno == ENXIO || errno == EINVAL) {
	t.c_cflag |= CBAUD | PARENB | PARODD | CSIZE | CSTOPB;
	return 0;
    }
    return 1;
    }]])],[tcl_cv_api_serial=termio],[tcl_cv_api_serial=no],[tcl_cv_api_serial=no])
    fi
    if test $tcl_cv_api_serial = no; then
	AC_RUN_IFELSE([AC_LANG_SOURCE([[
#include <sgtty.h>
#include <errno.h>

int main() {
    struct sgttyb t;
    if (ioctl(0, TIOCGETP, &t) == 0
	|| errno == ENOTTY || errno == ENXIO || errno == EINVAL) {
	t.sg_ospeed = 0;
	t.sg_flags |= ODDP | EVENP | RAW;
	return 0;
    }
    return 1;
}]])],[tcl_cv_api_serial=sgtty],[tcl_cv_api_serial=none],[tcl_cv_api_serial=none])
    fi])
    case $tcl_cv_api_serial in
	termios) AC_DEFINE([USE_TERMIOS],[1],[Define to 1 to use termios]);;
	termio)  AC_DEFINE([USE_TERMIO],[1],[Define to 1 to use termio]);;
	sgtty)   AC_DEFINE([USE_SGTTY],[1],[Define to 1 to use sgtty]);;
    esac
    AC_MSG_RESULT([$tcl_cv_api_serial])
])
