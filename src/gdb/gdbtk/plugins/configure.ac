dnl#                                               -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.13])dnl
AC_INIT([insight-plugins],[1.0],[keiths@redhat.com])
dnl# bug report address is last person to touch this directory
dnl# according to the ChangeLog in this directory
AC_CONFIG_SRCDIR([Make-rules])
AC_CONFIG_AUX_DIR([../../..])
AC_CONFIG_MACRO_DIR([../../m4])
AC_CANONICAL_TARGET
AC_OBJEXT
AC_EXEEXT

# Checks for programs.
AC_PROG_CC
AC_PROG_MAKE_SET

# Automake:
AM_INIT_AUTOMAKE([foreign dejagnu -Wall -Wno-obsolete])
AM_MAINTAINER_MODE

# Find Tcl, Tk, and things related to them:
CYG_AC_PATH_TCLCONFIG
CYG_AC_LOAD_TCLCONFIG
CYG_AC_PATH_TKCONFIG
CYG_AC_LOAD_TKCONFIG
CYG_AC_PATH_TCLH
CYG_AC_PATH_TKH
AC_SUBST([TCL_DBGX])dnl

# Make sure TCL_SHLIB_SUFFIX is set:
if test "x${TCL_SHLIB_SUFFIX}" = "x"; then
  case "${host}" in
    *cygwin*) TCL_SHLIB_SUFFIX=".dll" ;;
    *darwin*) TCL_SHLIB_SUFFIX=".dylib" ;;
    *) TCL_SHILB_SUFFIX=".so" ;;
  esac
fi
AC_SUBST([TCL_SHLIB_SUFFIX])dnl

dnl# Since we are not(?) (read as "*were* not") using autoconf > 2.1x,
dnl# we cannot ("*could* not") use AC_CONFIG_FILES here (yet):
make_subdirs=""
dnl# (uh, actually, we are now, and we can now... prereq is now set
dnl# explicitly above to make sure...)

# See if -mwin32 works for cygwin:
case "${host}" in
  *cygwin*)
    ocflags=${CFLAGS}
    CFLAGS="${CFLAGS} -mwin32"
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[]],[[]])],
                      [],[CFLAGS="${ocflags}"])
    ;;
  *) ;;
esac

# Note the toplevel plugin build directory:
plugin_builddir=`pwd`
AC_SUBST([plugin_builddir])dnl

#############
## Plugins ##
#############

# Check the supported hosts.

# Only supported/tested on linux, solaris, cygwin:
supported=yes
case "${host}" in
  *cygwin*) ;;
  *solaris*) ;;
  *linux*) ;;
  *) supported=no ;;
esac

# Host-specific configury:
case "${host}" in
  *cygwin*)
    AC_CHECK_TOOLS([NM],[nm gnm],[nm])
    AC_CHECK_TOOLS([AS],[as gas],[as])
    AC_CHECK_TOOLS([LD],[ld gld ld.bfd ld.gold gold],[ld])
    AC_MSG_CHECKING(["for libcygwin.a"])
    LIBCYGWIN_A=`${CC} -print-file-name=libcygwin.a`
    AC_MSG_RESULT([${LIBCYGWIN_A}])
    AC_SUBST([LIBCYGWIN_A])dnl
    AC_CHECK_TOOLS([DLLTOOL],[dlltool gdlltool])
    ;;
  *)
    AM_PROG_AS
    ;;
esac

# Plugins supported by this configuration:
if test "x${supported}" = "xyes"; then
  plugins="rhabout"
  case "${target}" in
    *cygwin*) ;;
    *linux*) ;;
    *i?86*)
      plugins="${plugins} intel-pentium"
      ;;
  esac

  for i in ${plugins}; do
    make_subdirs="${make_subdirs} ${i}"
  done
fi
AC_SUBST([make_subdirs])dnl

# Output:
AC_CONFIG_FILES([Makefile \
  rhabout/Makefile:rhabout/Makefile.in:Make-rules \
  rhabout/rhabout.tcl \
  intel-pentium/Makefile:intel-pentium/Makefile.in:Make-rules \
  intel-pentium/intel-pentium.tcl])dnl
AC_OUTPUT
