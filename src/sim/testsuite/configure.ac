dnl#                                               -*- Autoconf -*-
dnl# Process this file file with autoconf to produce a configure script.
dnl# This file is a shell script fragment that supplies the information
dnl# necessary to tailor a template configure script into the configure
dnl# script appropriate for this directory.  For more information, check
dnl# any existing configure script.

AC_PREREQ([2.5])
AC_INIT([sim-testsuite],[0.0.1],[/dev/null])
AC_CONFIG_SRCDIR([common/bits-tst.c])

my_CC=${CC-cc}
AC_SUBST([my_CC])
AC_CONFIG_AUX_DIR([../..])
AC_CANONICAL_TARGET

# Checks for programs.
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL

# Checks for header files.
AC_DEFUN([AC_REQUIRE_HEADER_STDC],[
  AC_REQUIRE([AC_HEADER_STDC])
])
AC_REQUIRE_HEADER_STDC
AC_CHECK_HEADERS([fcntl.h limits.h netinet/in.h stddef.h stdio.h \
                  sys/socket.h sys/time.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_PROTOTYPES
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_LSTAT dnl# also checks whether it "_FOLLOWS_SLASHED_SYMLINK"
AC_FUNC_MALLOC
AC_FUNC_MMAP
AC_FUNC_REALLOC
AC_CHECK_FUNCS([ftruncate getcwd gettimeofday memset munmap pathconf \
                socket strchr strerror strrchr uname])

# Directories to use in all configurations.
configdirs=""

# add test sub-directory for appropriate targets
testdir=`echo ${target} | sed -e 's/-.*-/-/'`
if test -r ${srcdir}/${testdir}/configure ; then
  configdirs="${configdirs} $testdir"
fi

# add any extra subdirectories
case ${target} in
  *) ;;
esac

# Compute the target architecture.
# FIXME: Will need to canonicalize some values.
# FIXME: Should be in generally accessable place.
case ${target} in
  *) arch=${target_cpu} ;;
esac
AC_SUBST([arch])

# configure the subdirectories too
AC_CONFIG_SUBDIRS([$configdirs])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
