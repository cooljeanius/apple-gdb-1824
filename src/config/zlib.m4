dnl# zlib.m4                                               -*- Autoconf -*-
dnl# A function to check for zlib availability. zlib is used by default
dnl# unless the user configured with --without-zlib.

#serial 2

AC_DEFUN([AM_ZLIB],[
  # See if the user specified whether he or she wants zlib support or not:
  AC_ARG_WITH([zlib],
    [AS_HELP_STRING([--with-zlib],
                    [include zlib support (auto/yes/no) [default=auto]])],
    [],[with_zlib=auto])dnl

  if test "x${with_zlib}" != "xno"; then
    AC_SEARCH_LIBS([zlibVersion],[z],[AC_CHECK_HEADERS([zlib.h])])
    if test "x${with_zlib}" = "yes" -a "x${ac_cv_header_zlib_h}" != "xyes"; then
      AC_MSG_ERROR([zlib (libz) library was explicitly requested but not found])
    fi
  fi

  AC_MSG_CHECKING([if there is a zlib sister directory in our source tree])
  if test -d ../zlib -o -d ${srcdir}/../zlib; then
    if test "x${zlibdir}" = "x"; then
      if test -d ../zlib; then
        test -z "${zlibdir}" && zlibdir=-L../zlib
      elif test -d ${srcdir}/../zlib; then
        test -z "${zlibdir}" && zlibdir=-L${srcdir}/../zlib
      fi
    fi
    if test "x${zlibinc}" = "x"; then
      test -z "${zlibinc}" && zlibinc="-I\$(srcdir)/../zlib"
    fi
    export HAVE_ZLIB_SOURCE=1
    AC_SUBST([HAVE_ZLIB_SOURCE])dnl
    ## print:
    AC_MSG_RESULT([yes])
  else
    zlibdir=""
    zlibinc=""
    AC_MSG_RESULT([no])
  fi
  AC_SUBST([zlibdir])dnl
  AC_SUBST([zlibinc])dnl
])dnl

