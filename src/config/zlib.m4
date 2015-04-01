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

  # for the in-tree one:
  if test "x${zlibdir}" = "x"; then
    test -z "${zlibdir}" && zlibdir=-L../zlib
  fi
  if test "x${zlibinc}" = "x"; then
    test -z "${zlibinc}" && zlibinc="-I\$(srcdir)/../zlib"
  fi
  AC_SUBST([zlibdir])dnl
  AC_SUBST([zlibinc])dnl
])dnl

