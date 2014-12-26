#signal.m4 serial 1                                        -*- Autoconf -*-
dnl# taken from autoconf2.61
dnl# obsolete in newer versions

dnl# AC_TYPE_SIGNAL
dnl# --------------
dnl# Note that identifiers starting with SIG are reserved by ANSI C.
AN_FUNCTION([signal],[AC_TYPE_SIGNAL])dnl
AC_DEFUN([AC_TYPE_SIGNAL],[
AC_CACHE_CHECK([return type of signal handlers],[ac_cv_type_signal],[
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <sys/types.h>
#include <signal.h>
]],[[return *(signal(0, 0)) (0) == 1;]])],
		   [ac_cv_type_signal=int],
		   [ac_cv_type_signal=void])])
AC_DEFINE_UNQUOTED([RETSIGTYPE],[${ac_cv_type_signal}],
		   [Define as the return type of signal handlers
		    (`int' or `void').])
])dnl
