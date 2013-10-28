dnl#                                               -*- Autoconf -*-
dnl# Process this file file with autoconf to produce a configure script.
dnl# This file is a shell script fragment that supplies the information
dnl# necessary to tailor a template configure script into the configure
dnl# script appropriate for this directory. For more information, check
dnl# any existing configure script.

AC_PREREQ([2.5])
AC_INIT([mips64el-elf-sim-testsuite],[0.0.1],[/dev/null])
dnl# FIXME - think of a truly unique file to this directory
AC_CONFIG_SRCDIR([Makefile.in])

my_CC=${CC-cc}
AC_SUBST([my_CC])
AC_CONFIG_AUX_DIR([../../..])
AC_CANONICAL_TARGET
AC_SUBST([target_cpu])
AC_MSG_CHECKING([target_alias])
if test "x${target_alias}" = "x"; then
	test -z "${target_alias}"
	if test "x${target_canonical}" = "x"; then
		test -z "${target_canonical}"
		export target_canonical=mips64el-elf
	else
		test ! -z "${target_canonical}" && export target_canonical
		AC_SUBST([target_canonical])
	fi
	export target_alias="${target_canonical}"
	test ! -z "${target_alias}"
	AC_SUBST([target_alias])
	AC_MSG_RESULT([set to "${target_alias}"])
else
	test ! -z "${target_alias}"
	AC_MSG_RESULT([already set: "${target_alias}"])
fi

AM_PROG_AS

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
