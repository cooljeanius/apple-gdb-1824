dnl#                                               -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.12.1])dnl
AC_INIT([gdbserve-nlm],[4.12],[])
AC_CONFIG_SRCDIR([gdbserve.c])

AC_CONFIG_AUX_DIR([../..])
AC_CANONICAL_TARGET
AC_ARG_PROGRAM

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_DEFUN([AC_REQUIRE_PROG_SED],[
  AC_REQUIRE([AC_PROG_SED])
])
AC_REQUIRE_PROG_SED

# Checks for header files.
AC_DEFUN([AC_REQUIRE_HEADER_STDC],[
  AC_REQUIRE([AC_HEADER_STDC])
])
AC_REQUIRE_HEADER_STDC
AC_CHECK_HEADERS([stddef.h])

# Checks for library functions.
AC_CHECK_FUNCS([memset strtol])

# Map target cpu into the config cpu subdirectory name.
# The default is $target_cpu.
changequote(,)dnl
case "${target_cpu}" in
alpha)			gdb_target_cpu=alpha ;;
c[12])			gdb_target_cpu=convex ;;
hppa*)			gdb_target_cpu=pa ;;
i[34567]86)		gdb_target_cpu=i386 ;;
m68*)			gdb_target_cpu=m68k ;;
np1)			gdb_target_cpu=gould ;;
pn)			gdb_target_cpu=gould ;;
pyramid)		gdb_target_cpu=pyr ;;
sparc*)			gdb_target_cpu=sparc ;;
*)			gdb_target_cpu=$target_cpu ;;
esac
changequote([,])dnl

target_makefile_frag=${srcdir}/../config/${gdb_target_cpu}/gdbserve.mt
if test ! -f ${target_makefile_frag}; then
	AC_MSG_WARN([*** GDBSERVE does not support target ${target}. This should be a hard error, but we are ignoring it to facilitate work on this configure script.])
fi

dnl# We have to assign the same value to other variables because autoconf
dnl# does NOT provide a mechanism to substitute a replacement keyword with
dnl# arbitrary data or pathnames.
dnl#

if test "${target_makefile_frag}" != ""; then
target_makefile_frag_path=$target_makefile_frag
AC_SUBST([target_makefile_frag_path])
AC_SUBST_FILE([target_makefile_frag])
test -f ${target_makefile_frag} && export cpufile=`sed -n '
s/CPU_FILE[ 	]*=[ 	]*\([^ 	]*\)/\1/p
' ${target_makefile_frag}`
fi

files=
links=
rm -f cpu.h
if test "${cpufile}" != ""; then
	files="${files} ${cpufile}.h"
	links="${links} cpu.h"
fi

AC_DIAGNOSE([obsolete],[It is technically impossible to `autoupdate' cleanly from AC_LINK_FILES
to AC_CONFIG_LINKS. `autoupdate' provides a functional but inelegant
update, you should probably tune the result yourself.])dnl
ac_sources="$files"
ac_dests="$links"
while test -n "$ac_sources"; do
  set $ac_dests; ac_dest=$1; shift; ac_dests=$*
  set $ac_sources; ac_source=$1; shift; ac_sources=$*
  ac_config_links_1="$ac_config_links_1 $ac_dest:$ac_source"
done
AC_CONFIG_LINKS([$ac_config_links_1])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT

