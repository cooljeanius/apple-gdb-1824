dnl#                                               -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.12.1])dnl
AC_INIT([gdbserve-nlm],[4.12],[martin@hinner.info])
dnl# bug report address is from the pdf in nlm-doc
AC_CONFIG_SRCDIR([gdbserve.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_AUX_DIR([../..])dnl

dnl# get target/host set up:
AC_CANONICAL_TARGET
m4_ifdef([ACX_NONCANONICAL_TARGET],[
  ACX_NONCANONICAL_TARGET
],[
if test "x${target_alias}" = "x"; then
  test -z "${target_alias}"
  if test "x${target_noncanonical}" != "x"; then
    test -n "${target_noncanonical}" && export target_alias="${target_noncanonical}"
  else
    test -n "${target}" && export target_alias="${target}"
  fi
fi
AC_SUBST([target_alias])dnl
])dnl
m4_ifdef([ACX_NONCANONICAL_HOST],[
  ACX_NONCANONICAL_HOST
],[
if test "x${host_alias}" = "x"; then
  test -z "${host_alias}"
  if test "x${host_noncanonical}" != "x"; then
    test -n "${host_noncanonical}" && export host_alias="${host_noncanonical}"
  else
    test -n "${host}" && export host_alias="${host}"
  fi
fi
AC_SUBST([host_alias])dnl
])dnl

# some checks stolen from automake:
AC_ARG_PROGRAM
AM_SANITY_CHECK
AM_MAINTAINER_MODE

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LN_S
AC_DEFUN([AC_REQUIRE_PROG_SED],[
  AC_REQUIRE([AC_PROG_SED])dnl
])dnl
AC_REQUIRE_PROG_SED

# Checks for header files.
AC_DEFUN([AC_REQUIRE_HEADER_STDC],[
  AC_REQUIRE([AC_HEADER_STDC])dnl
])dnl
AC_REQUIRE_HEADER_STDC
AC_HEADER_TIME
AC_CHECK_HEADERS_ONCE([advanced.h aio.h conio.h debugapi.h dfs.h \
                       nwadv.h nwconio.h nwdbgapi.h nwdfs.h nwpre.h \
                       nwthread.h nwtypes.h process.h])dnl
AC_CHECK_HEADERS([ctype.h errno.h stddef.h stdio.h time.h])dnl

# Checks for typedefs, structures, and compiler characteristics.
AC_C_BIGENDIAN
AC_C_CONST
AC_C_PROTOTYPES
AC_C_VOLATILE

# Checks for library functions.
AC_FUNC_MEMCMP
AC_CHECK_FUNCS([exit fprintf isspace memcpy memset \
                sprintf strcmp strcpy strnicmp strtol])dnl

# Checks for declarations.
AC_CHECK_DECLS([TERMINATE_BY_UNLOAD])
AC_CHECK_DECLS([LONG])
AC_CHECK_DECLS([BYTE])
AC_CHECK_DECLS([REGISTER_BYTES])
AC_CHECK_DECLS([BREAKPOINT])
AC_CHECK_DECLS([BREAKPOINT_SIZE])dnl

# Map target cpu into the config cpu subdirectory name.
# The default is ${target_cpu}.
changequote(,)dnl
case "${target_cpu}" in
alpha)		gdb_target_cpu=alpha ;;
amd64)		gdb_target_cpu=i386 ;;
c[12])		gdb_target_cpu=convex ;;
hppa*)		gdb_target_cpu=pa ;;
i[34567]86)	gdb_target_cpu=i386 ;;
m68*)		gdb_target_cpu=m68k ;;
np1)		gdb_target_cpu=gould ;;
powerpc)	gdb_target_cpu=ppc ;;
pn)		gdb_target_cpu=gould ;;
ppc)		gdb_target_cpu=ppc ;;
pyramid)	gdb_target_cpu=pyr ;;
sparc*)		gdb_target_cpu=sparc ;;
x86_64)		gdb_target_cpu=i386 ;;
*)		gdb_target_cpu=${target_cpu} ;;
esac
changequote([,])dnl

AC_SUBST([gdb_target_cpu])dnl

target_makefile_frag=${srcdir}/../config/${gdb_target_cpu}/gdbserve.mt
if test ! -f "${target_makefile_frag}"; then
  AC_MSG_WARN([*** GDBSERVE does not support target ${target}. This should be a hard error, but we are ignoring it to facilitate work on this configure script.])
else
  test -e "${target_makefile_frag}"
  AC_MSG_NOTICE([using '${target_makefile_frag}' for your Makefile frag])
fi

dnl# We have to assign the same value to other variables because autoconf
dnl# does NOT provide a mechanism to substitute a replacement keyword with
dnl# arbitrary data or pathnames.
  ## begin complicatedness:
if test "x${target_makefile_frag}" != "x"; then
  target_makefile_frag_path=${target_makefile_frag}
  AC_SUBST([target_makefile_frag_path])
  AC_SUBST_FILE([target_makefile_frag])
  unset cpufile
  (test -f ${target_makefile_frag} && export cpufile=`sed -n '
s/CPU_FILE[ 	]*=[ 	]*\([^ 	]*\)/\1/p
' ${target_makefile_frag}`) || (test -z "${cpufile}" && test -n "${gdb_target_cpu}" && export cpufile="${gdb_target_cpu}")
  if test "x${cpufile}" = "x"; then
    test -z "${cpufile}"
    if test "x${gdb_target_cpu}" != "x"; then
      test -n "${gdb_target_cpu}" && export cpufile="${gdb_target_cpu}"
    elif test "x${target_cpu}" != "x"; then
      test -n "${target_cpu}" && export cpufile="${target_cpu}"
    elif test "x${host_cpu}" != "x"; then
      test -n "${host_cpu}" && export cpufile="${host_cpu}"
    elif test "x${build_cpu}" != "x"; then
      test -n "${build_cpu}" && export cpufile="${build_cpu}"
    fi
  fi
  AC_SUBST([cpufile])
else
  test -z "${target_makefile_frag_path}" && target_makefile_frag_path=/dev/null
  test -z "${target_makefile_frag}" && target_makefile_frag=/dev/null
  test -z "${cpufile}" && test -n "${gdb_target_cpu}" && export cpufile="${gdb_target_cpu}"
  if test "x${cpufile}" = "x"; then
    test -z "${cpufile}"
    if test "x${target_cpu}" != "x"; then
      test -n "${target_cpu}" && export cpufile="${target_cpu}"
    elif test "x${host_cpu}" != "x"; then
      test -n "${host_cpu}" && export cpufile="${host_cpu}"
    elif test "x${build_cpu}" != "x"; then
      test -n "${build_cpu}" && export cpufile="${build_cpu}"
    fi
  fi
fi

files=""
links=""
rm -f cpu.h
if test "x${cpufile}" != "x"; then
  files="${files} ${cpufile}.h"
  links="${links} cpu.h"
else
  files="${files}"
  links="${links}"
fi

if test "x${files}" = "x"; then
  test -z "${files}"
  AC_MSG_WARN([the files variable is empty; not making symlinks.])
else
  test ! -z "${files}"
  AC_MSG_NOTICE([files should be "${files}"])
fi

if test "x${links}" = "x"; then
  test -z "${links}"
  AC_MSG_WARN([the links variable is empty; not making symlinks.])
else
  test ! -z "${links}"
  AC_MSG_NOTICE([links should be "${links}"])
fi

AC_DIAGNOSE([obsolete],[It is technically impossible to `autoupdate'
cleanly from AC\_LINK\_FILES to AC_CONFIG_LINKS. `autoupdate' provides a
functional but inelegant update, you should probably tune the result
yourself.])dnl
ac_sources="${files}"
ac_dests="${links}"
while test -n "${ac_sources}"; do
  set ${ac_dests}; ac_dest=${1}; shift; ac_dests=$*
  set ${ac_sources}; ac_source=${1}; shift; ac_sources=$*
  ac_config_links_1="${ac_config_links_1} ${ac_dest}:${ac_source}"
done
AC_CONFIG_LINKS([${ac_config_links_1}])
AC_SUBST([ac_config_links1])dnl
  ## print to verify the result is correct:
echo "symlinks to be made should be \"${ac_config_links1}\""

# Output:
AC_CONFIG_FILES([Makefile])dnl
AC_OUTPUT

