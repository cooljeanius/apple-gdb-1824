dnl#                                               -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.61])
AC_INIT([libgui],[0.0],[/dev/null])
AC_CONFIG_SRCDIR([src/subcommand.h])
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE([1.11.6 gnits no-dependencies no-dist dejagnu])
AC_CONFIG_HEADERS([config.h])
AM_MAINTAINER_MODE

# Checks for programs.
AC_PROG_CC
AC_EXEEXT
AC_OBJEXT
AC_ARG_ENABLE([install-libgui],
  [AS_HELP_STRING([--enable-install-libgui],
                  [Install libgui.a and library header files])])
AM_CONDITIONAL([CROSS_COMPILING],[test "x${cross_compiling}" = "xyes"])
AM_CONDITIONAL([INSTALL_LIBGUI],
               [test "x${enable_install_libgui}" = "xyes"])
AC_CHECK_TOOLS([RANLIB],[ranlib granlib],[:],[${PATH}:./../binutils])
AC_CHECK_TOOLS([AR],[ar gar],[:],[${PATH}:./../binutils])
AC_CHECK_PROG([ITCL_SH],[itcl_sh],[itcl_sh],
	[\$\$here/\$(top_builddir)/../itcl/itcl/unix/itcl_sh\$(EXEEXT)])
AC_PROG_SED

# Checks for libraries.
AC_PATH_X

# Checks for header files.
AC_DEFUN([AC_REQUIRE_HEADER_STDC],[
  AC_REQUIRE([AC_HEADER_STDC])dnl
])dnl
AC_REQUIRE_HEADER_STDC
AC_HEADER_STDBOOL dnl# also "_CHECK"s it
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS([ctype.h stdio.h getopt.h fcntl.h sys/file.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T
AC_STRUCT_TIMEZONE dnl# also does the "_TM" one
AC_TYPE_INTPTR_T
AC_TYPE_UINTPTR_T

# Checks for library functions.
AC_FUNC_ALLOCA
AC_FUNC_FORK
AC_FUNC_STRTOD
AC_CHECK_FUNCS([memset raise strchr strerror strncasecmp strtol strtoul])

AC_ARG_ENABLE([ide],[AS_HELP_STRING([--enable-ide],[Enable IDE support])],
[case "${enableval}" in
  yes) ide=yes ;;
  no)  ide=no ;;
  *)   AC_MSG_ERROR([bad value ${enableval} for enable-ide option]) ;;
esac],
[ide=no])
AM_CONDITIONAL([IDE_ENABLED],[test "x${ide}" = "xyes"])
if test "x${ide}" = "xyes"; then
  AC_DEFINE([IDE_ENABLED])
fi

# If we have random, assume we have srandom.  If we have drand48,
# assume we have srand48.  If we have rand, assume we have srand.
AC_CHECK_FUNCS([random drand48 rand],[break])

dnl# Tcl ensures that strncasecmp is provided everywhere.  But in some
dnl# situations we might need to declare it.  We check for that case
dnl# here.
AC_EGREP_CPP([strncasecmp],[#include <string.h>],
  [AC_DEFINE([HAVE_STRNCASECMP_DECL])])dnl


AC_CACHE_CHECK([for cygwin32],[ide_cv_os_cygwin32],
[AC_EGREP_CPP([you_lose],[
#ifdef __CYGWIN32__
you_lose
#endif],[ide_cv_os_cygwin32=yes],[ide_cv_os_cygwin32=no])])
ac_win_build="no"
if test "x${ide_cv_os_cygwin32}" = "xyes"; then
  ac_win_build="yes"
fi

case "${host}" in
*-*-cygwin*)
        touch ac$$.c
        if ${CC} -c -mwin32 ac$$.c >/dev/null 2>&1; then
            case "${LIBGUI_CFLAGS}" in
                *-mwin32*) ;;
                *) LIBGUI_CFLAGS="-mwin32 ${LIBGUI_CFLAGS}" ;;
            esac
        fi
        rm -f ac$$.o ac$$.c
        ;;
esac
case "${host}" in
*-*-cygwin*)
	LIBGUI_CFLAGS="-DWIN32 ${LIBGUI_CFLAGS}" ;;
esac
AC_SUBST([LIBGUI_CFLAGS])dnl 

tmp="`cd ${srcdir}/library; pwd`"
if test x"${ac_cv_prog_CC}" = x"cl"; then
  tmp2="`cygpath --windows ${tmp}`"
  LIBGUI_LIBRARY_DIR="`echo ${tmp2} | sed -e s#\\\\\\\\#/#g`"
else
  LIBGUI_LIBRARY_DIR="${tmp}"
fi
AC_SUBST([LIBGUI_LIBRARY_DIR])dnl

# Find the init.tcl file.

AC_MSG_CHECKING([for init.tcl])
AC_CACHE_VAL([ac_cv_c_tcl_libdir],[
dnl# currently, only check the source tree:
if test -f ${srcdir}/../tcl/library/init.tcl ; then
  ac_cv_c_tcl_libdir=`cd ${srcdir}/../tcl/library; pwd`
elif test -f ${srcdir}/../tcl8.1/library/init.tcl; then
  ac_cv_c_tcl_libdir=`cd ${srcdir}/../tcl8.1/library; pwd`
fi
])
AC_MSG_RESULT([${ac_cv_c_tcl_libdir}])

if test x"${ac_cv_prog_CC}" = x"cl"; then
  tmp2="`cygpath --windows ${ac_cv_c_tcl_libdir}`"
  TCL_LIBRARY="`echo ${tmp2} | sed -e s#\\\\\\\\#/#g`"
else
  TCL_LIBRARY="${ac_cv_c_tcl_libdir}"
fi
AC_SUBST([TCL_LIBRARY])dnl

# Check for Tcl and Tk.
CYG_AC_PATH_TCLCONFIG
CYG_AC_LOAD_TCLCONFIG
CYG_AC_PATH_TKCONFIG
CYG_AC_LOAD_TKCONFIG
CYG_AC_PATH_TCLH
# FIXME: consider only doing this if --with-x given.
CYG_AC_PATH_TKH

AC_SUBST([TCL_DEFS])dnl
AC_SUBST([TK_DEFS])dnl
AC_SUBST([TCLHDIR])dnl
AC_SUBST([TKHDIR])dnl
AC_SUBST([TK_XINCLUDES])dnl
AC_SUBST([TCL_LIBS])dnl
AC_SUBST([TK_BUILD_LIB_SPEC])dnl
AC_SUBST([TCL_BUILD_LIB_SPEC])dnl
AC_SUBST([TK_LIBS])dnl

AM_CONDITIONAL([TCL_SHARED],[test x${TCL_SHARED_BUILD} = x1])dnl

AC_SUBST([RPATH_ENVVAR])dnl

# Output.
AC_CONFIG_FILES([Makefile library/Makefile src/Makefile])
AC_OUTPUT
