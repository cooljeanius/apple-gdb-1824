dnl#                                               -*- Autoconf -*-
dnl# Autoconf configure script for GDB server.
dnl# Copyright 2000, 2002, 2003, 2004, 2005 Free Software Foundation, Inc.
dnl#
dnl# This file is part of GDB.
dnl#
dnl# This program is free software; you can redistribute it and/or modify
dnl# it under the terms of the GNU General Public License as published by
dnl# the Free Software Foundation; either version 2 of the License, or
dnl# (at your option) any later version.
dnl#
dnl# This program is distributed in the hope that it will be useful,
dnl# but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl# GNU General Public License for more details.
dnl#
dnl# You should have received a copy of the GNU General Public License
dnl# along with this program; if not, write to:
dnl#  The Free Software Foundation, Inc.
dnl#  59 Temple Place - Suite 330
dnl#  Boston, MA 02111-1307, USA

dnl# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.59])dnl

AC_INIT([gdbserver],[1824],[gclayton@apple.com])
dnl# bug report address is email address of last person to touch this
dnl# directory, according to the Changelog-Apple in this directory
AC_CONFIG_SRCDIR([server.c])
AC_CONFIG_MACRO_DIR([../m4])
AC_CONFIG_AUX_DIR([..])
AC_REQUIRE_AUX_FILE([depcomp])
AC_REQUIRE_AUX_FILE([mkinstalldirs])
AC_REQUIRE_AUX_FILE([move-if-change])
AC_CONFIG_HEADER([config.h])
m4_ifdef([ACX_PKGVERSION],[
ACX_PKGVERSION([GDBSERVER])
if test "x${PKGVERSION}" != "x"; then
  test -n "${PKGVERSION}"
  AC_DEFINE_UNQUOTED([PKGVERSION],["${PKGVERSION}"],
                     [Additional package description])
fi
],[
test -z "${PKGVERSION}"
])
m4_ifdef([ACX_BUGURL],[
ACX_BUGURL([http://www.gnu.org/software/gdb/bugs/])
if test "x${REPORT_BUGS_TO}" != "x"; then
  test -n "${REPORT_BUGS_TO}"
  AC_DEFINE_UNQUOTED([REPORT_BUGS_TO],["${REPORT_BUGS_TO}"],
                     [Bug reporting address])
fi
],[
test -z "${REPORT_BUGS_TO}"
])

# automake-lite sub-checks:
AM_SANITY_CHECK
AM_MAINTAINER_MODE

# Checks for programs.
AC_MSG_CHECKING([programs])
AC_MSG_RESULT([ ])
AC_PROG_CC
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_GCC_TRADITIONAL
AC_EXEEXT
#TODO: improve this check for profiling support:
AC_MSG_CHECKING([if we can do profiling])
if test "x${ac_cv_c_compiler_gnu}" = "xyes" -a "x${CC}" != "xclang"; then
  export PROFILE_CFLAGS="-pg"
  AC_SUBST([PROFILE_CFLAGS])
  AC_MSG_RESULT([yes, with ${PROFILE_CFLAGS}])
else
  AC_MSG_RESULT([no, we are not using a GNU compiler])
fi

AC_SYS_LARGEFILE

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
AC_SUBST([target_alias])
])
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
AC_SUBST([host_alias])
])
if test -z "${HOST_ARCHITECTURE}" -a -n "${host_cpu}"; then
  export HOST_ARCHITECTURE="${host_cpu}"
  AC_SUBST([HOST_ARCHITECTURE])
fi
if test -z "${TARGET_ARCHITECTURE}" -a -n "${target_cpu}"; then
  export TARGET_ARCHITECTURE="${target_cpu}"
  AC_SUBST([TARGET_ARCHITECTURE])
fi

if test "x${HOST_ARCHITECTURE}" != "x"; then
  test -n "${HOST_ARCHITECTURE}" && test -z "${C_ARCHFLAGS}" && export C_ARCHFLAGS="-arch ${HOST_ARCHITECTURE}"
elif test "x${TARGET_ARCHITECTURE}" != "x"; then
  test -n "${TARGET_ARCHITECTURE}" && test -z "${C_ARCHFLAGS}" && export C_ARCHFLAGS="-arch ${TARGET_ARCHITECTURE}"
fi
AC_SUBST([C_ARCHFLAGS])

if test "x${CFLAGS}" = "x"; then
  test -z "${CFLAGS}" && test -n "${C_ARCHFLAGS}" && export CFLAGS="${C_ARCHFLAGS}"
else
  test -n "${CFLAGS}" && test -n "${C_ARCHFLAGS}" && export CFLAGS="${CFLAGS} ${C_ARCHFLAGS}"
fi

AC_PROG_INSTALL
AC_PROG_RANLIB
AC_PROG_SED

AC_ARG_PROGRAM

if test -e ${srcdir}/../development.sh; then
  # Set the 'development' global.
  AC_MSG_NOTICE([sourcing ${srcdir}/../development.sh])
  . ${srcdir}/../development.sh
fi

# Check for the 'make' the user wants to use.
AC_CHECK_PROGS([MAKE],[make])
MAKE_IS_GNU=""
case "`${MAKE} --version 2>&1 | sed 1q`" in
  *GNU*)
    MAKE_IS_GNU=yes
    ;;
esac
AM_CONDITIONAL([GMAKE],[test "x${MAKE_IS_GNU}" = "xyes"])
AC_PROG_MAKE_SET

# Check for a way of listing symbols:
GDB_PATH_NM
AC_CHECK_TOOL([NMEDIT],[nmedit],[:])
GDB_CMD_GLOBAL_SYMBOLS

# Checks for libraries.
AC_MSG_CHECKING([libraries])
AC_MSG_RESULT([ ])
if test -d ../../libiberty; then
  if test -z "${LDFLAGS}"; then
    export LDFLAGS="-L../../libiberty"
  else
    export LDFLAGS="${LDFLAGS} -L../../libiberty"
  fi
elif test -d ${srcdir}/../../libiberty; then
  if test -z "${LDFLAGS}"; then
    export LDFLAGS="-L${srcdir}/../../libiberty"
  else
    export LDFLAGS="${LDFLAGS} -L${srcdir}/../../libiberty"
  fi
fi
AC_SEARCH_LIBS([xmalloc],[iberty c gnu])
AC_SEARCH_LIBS([xsnprintf],[iberty c gnu])
AC_SEARCH_LIBS([xstrdup],[iberty c gnu])
dnl# libdl is checked lower below

# Checks for header files.
AC_MSG_CHECKING([headers])
AC_MSG_RESULT([ ])
AC_HEADER_ASSERT
AC_HEADER_DIRENT
AC_DEFUN([AC_REQUIRE_HEADER_STDC],[
  AC_REQUIRE([AC_HEADER_STDC])
])
AC_REQUIRE_HEADER_STDC
AC_SYS_POSIX_TERMIOS
AC_HEADER_TIOCGWINSZ
AC_HEADER_TIME
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS([AvailabilityMacros.h arpa/inet.h elf.h errno.h fcntl.h \
                  libgen.h libintl.h limits.h linux/elf.h locale.h \
                  mach/exception_types.h mach/mach.h mach/mach_error.h \
                  mach/mach_types.h mach/mach_vm.h \
                  mach/machine/thread_status.h mach/port.h \
                  mach-o/nlist.h malloc.h malloc/malloc.h netdb.h \
                  netinet/in.h netinet/tcp.h prctl.h proc_service.h \
                  pthread.h sgtty.h signal.h stddef.h stdio.h sys/file.h \
                  sys/ioctl.h sys/param.h sys/ptrace.h sys/signal.h \
                  sys/socket.h sys/procfs.h sys/reg.h sys/time.h \
                  termio.h termios.h thread_db.h])
dnl# "_ONCE" causes mis-ordering of checks, so just check the cache value
dnl# first to simulate "_ONCE":
if test "x${ac_cv_header_sys_wait_h}" = "x"; then
  test -z "${ac_cv_header_sys_wait_h}"
  AC_CHECK_HEADERS([sys/wait.h])
fi

case ${host} in
  *arm*-apple-darwin*)
    AC_CHECK_HEADERS([mach/arm/thread_act.h])
    ;;
  i386-apple-darwin*)
    AC_CHECK_HEADERS([mach/i386/thread_act.h])
    ;;
  i686-apple-darwin*)
    AC_CHECK_HEADERS([mach/i386/thread_act.h])
    AC_CHECK_HEADERS([mach/x86_64/thread_act.h])
    ;;
  powerpc-apple-darwin*)
    AC_CHECK_HEADERS([mach/ppc/thread_act.h])
    AC_CHECK_HEADERS([mach/ppc64/thread_act.h])
    ;;
  x86_64-apple-darwin*)
    AC_CHECK_HEADERS([mach/x86_64/thread_act.h])
    ;;
esac

# Checks for declarations:
AC_MSG_CHECKING([declarations])
AC_MSG_RESULT([ ])
AC_CHECK_DECLS([child_get_pagesize])
AC_CHECK_DECLS([strerror, strstr, perror, vasprintf, vsnprintf])
AC_CHECK_DECLS([NULL])
AC_CHECK_DECLS([basename],[],[],[
#ifdef HAVE_LIBGEN_H
# include <libgen.h>
#endif /* HAVE_LIBGEN_H */
])

# Checks for typedefs, structures, and compiler characteristics.
AC_MSG_CHECKING([C compiler characteristics])
AC_MSG_RESULT([ ])
AC_C_BIGENDIAN
AC_C_CHAR_UNSIGNED
AC_C_CONST
AC_C_INLINE
AC_C_PROTOTYPES
AC_C_VOLATILE
AC_MSG_CHECKING([types])
AC_MSG_RESULT([ ])
AC_TYPE_INT8_T
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INTMAX_T
AC_TYPE_OFF_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINTMAX_T
AC_TYPE_UINTPTR_T
AC_STRUCT_TIMEZONE dnl# also does the "_TM" one
AC_CHECK_TYPES([ptrdiff_t])
AC_CHECK_TYPES([ptid_t])
AC_CHECK_TYPES([exception_mask_t],[],[],[
#ifdef HAVE_MACH_EXCEPTION_TYPES_H
# include <mach/exception_types.h>
#else
# if defined(__APPLE__) && defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "This conftest expects <mach/exception_types.h> to be included."
# endif /* __APPLE__ && __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_MACH_EXCEPTION_TYPES_H */
])
AC_CHECK_TYPES([task_t],[],[],[
#ifdef HAVE_MACH_MACH_H
# include <mach/mach.h>
#else
# if defined(__APPLE__) && defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "This conftest for task_t expects <mach/mach.h> to be included."
# endif /* __APPLE__ && __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_MACH_MACH_H */
])
AC_CHECK_TYPES([socklen_t],[],[],[
#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "This conftest for socklen_t wants <sys/types.h> to be included"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_SYS_TYPES_H */
#ifdef HAVE_SYS_SOCKET_H
# include <sys/socket.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "This conftest for socklen_t wants to include <sys/socket.h>"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_SYS_SOCKET_H */
])
AC_CHECK_TYPES([mach_vm_size_t],[],[],[
#ifdef HAVE_MACH_MACH_VM_H
# include <mach/mach_vm.h>
#else
# if defined(__APPLE__) && defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "This conftest expects <mach/mach_vm.h> to be included."
# endif /* __APPLE__ && __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_MACH_MACH_VM_H */
])
AC_CHECK_TYPES([vm_size_t],[],[],[
#ifdef HAVE_MACH_MACH_VM_H
# include <mach/mach_vm.h>
#else
# if defined(__APPLE__) && defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "This conftest expects <mach/mach_vm.h> to be included."
# endif /* __APPLE__ && __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_MACH_MACH_VM_H */
])
AC_CHECK_TYPES([gregset_t])
AC_CHECK_TYPES([elf_gregset_t],[],[],[
#ifdef HAVE_LINUX_ELF_H
# include <linux/elf.h>
#endif /* HAVE_LINUX_ELF_H */
])
AC_CHECK_TYPES([fpregset_t])
AC_CHECK_TYPES([elf_fpregset_t],[],[],[
#ifdef HAVE_LINUX_ELF_H
# include <linux/elf.h>
#endif /* HAVE_LINUX_ELF_H */
])
AC_CHECK_TYPES([Elf32_auxv_t, Elf64_auxv_t],[],[],[
#ifdef HAVE_ELF_H
# include <elf.h>
#else
# ifdef HAVE_LINUX_ELF_H
#  include <linux/elf.h>
# else
#  if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#   warning "This conftest expects an elf header to be included."
#  endif /* __GNUC__ && !__STRICT_ANSI__ */
# endif /* HAVE_LINUX_ELF_H */
#endif /* HAVE_ELF_H */
])
AC_CHECK_TYPES([unsigned char])
AC_CHECK_TYPES([unsigned int])
AC_CHECK_TYPES([unsigned long])
AC_CHECK_TYPES([unsigned long long])
AC_CHECK_TYPES([long unsigned int])
AC_CHECK_TYPES([long long])
AC_CHECK_SIZEOF([int])
AC_CHECK_ALIGNOF([int])
AC_CHECK_SIZEOF([void *])
AC_CHECK_ALIGNOF([void *])
dnl# Begin what used to be AC\_TYPE\_SIGNAL
AC_CACHE_CHECK([return type of signal handlers],[ac_cv_type_signal],[
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <sys/types.h>
#include <signal.h>
]],[[
return *(signal (0, 0)) (0) == 1;
]])],[ac_cv_type_signal=int],[ac_cv_type_signal=void])
])
AC_DEFINE_UNQUOTED([RETSIGTYPE],[${ac_cv_type_signal}],
         [Define as the return type of signal handlers (`int' or `void').])
dnl# End what used to be AC\_TYPE\_SIGNAL

# Checks for library functions.
AC_MSG_CHECKING([functions])
AC_MSG_RESULT([ ])
AC_FUNC_ALLOCA
if test -n "${LIBOBJDIR}"; then
  if test -f ${LIBOBJDIR}/error.c; then
    test -z "${LIBOBJS}"
    AC_FUNC_ERROR_AT_LINE
  else
    test -z "${ac_cv_func_error_at_line}"
    AC_CHECK_FUNCS([error_at_line])
  fi
fi
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_FUNC_MEMCMP
AC_FUNC_MMAP
AC_FUNC_REALLOC
AC_FUNC_SELECT_ARGTYPES
AC_FUNC_SETPGRP
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([atexit basename child_get_pagesize gettimeofday inet_ntoa \
                memset minus_one_ptid pread pread64 pwrite readlink \
                select setenv socket strchr strdup strerror strlen \
                strncmp strrchr strstr strtol strtoul strtoull \
                xmalloc xsnprintf xstrdup])
dnl# the makefile gets the sources for these libobjs from libiberty:
AC_REPLACE_FUNCS([vasprintf vsnprintf])
dnl# dladdr is glibc-specific. It is used by thread-db.c but only for
dnl# debugging messages. It lives in -ldl, which is handled below,
dnl# so we do NOT use AC_CHECK_LIB (or AC_SEARCH_LIBS) here. Instead we
dnl# just temporarily augment LIBS:
old_LIBS="${LIBS}"
LIBS="${LIBS} -ldl"
AC_CHECK_FUNCS([dladdr])
LIBS="${old_LIBS}"

# Check for various supplementary target information (beyond the
# triplet) which might affect the choices in configure.srv.
case "${target}" in
changequote(,)dnl
  i[34567]86-*-linux*)
changequote([,])dnl
    AC_CACHE_CHECK([if building for x86-64],[gdb_cv_i386_is_x86_64],
      	           [save_CPPFLAGS="${CPPFLAGS}"
                    CPPFLAGS="${CPPFLAGS} ${CFLAGS}"
                    AC_EGREP_CPP([got_it],[
#if __x86_64__
got_it
#endif /* __x86_64__ */
                    ],[gdb_cv_i386_is_x86_64=yes],
                      [gdb_cv_i386_is_x86_64=no])
                    CPPFLAGS="${save_CPPFLAGS}"])
    ;;
  m68k-*-*)
    AC_CACHE_CHECK([if building for Coldfire],[gdb_cv_m68k_is_coldfire],
      	           [save_CPPFLAGS="${CPPFLAGS}"
                    CPPFLAGS="${CPPFLAGS} ${CFLAGS}"
                    AC_EGREP_CPP([got_it],[
#ifdef __mcoldfire__
got_it
#endif /* __mcoldfire__ */
                    ],[gdb_cv_m68k_is_coldfire=yes],
                      [gdb_cv_m68k_is_coldfire=no])
                    CPPFLAGS="${save_CPPFLAGS}"])
    ;;
esac

AC_MSG_NOTICE([sourcing ${srcdir}/configure.srv to check for ${target}])
. ${srcdir}/configure.srv

if test "x${srv_mingwce}" = "xyes"; then
  LIBS="${LIBS} -lws2"
elif test "x${srv_mingw}" = "xyes"; then
  LIBS="${LIBS} -lws2_32"
elif test "x${srv_qnx}" = "xyes"; then
  LIBS="${LIBS} -lsocket"
elif test "x${srv_lynxos}" = "xyes"; then
  LIBS="${LIBS} -lnetinet"
fi

if test "x${srv_mingw}" = "xyes"; then
  AC_DEFINE([USE_WIN32API],[1],
	    [Define if we should use the Windows API, instead of the
	     POSIX API.  On Windows, we use the Windows API when
	     building for MinGW, but the POSIX API when building
	     for Cygwin.])
fi

if test "x${srv_linux_usrregs}" = "xyes"; then
  AC_DEFINE([HAVE_LINUX_USRREGS],[1],
	    [Define to 1 if targ supports PTRACE_PEEKUSR for register ]
	    [access.])
fi

if test "x${srv_linux_regsets}" = "xyes"; then
  AC_DEFINE([HAVE_LINUX_REGSETS],[1],
	    [Define to 1 if targ supports register sets.])

  AC_MSG_CHECKING([for PTRACE_GETREGS])
  AC_CACHE_VAL([gdbsrv_cv_have_ptrace_getregs],
  [AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <sys/ptrace.h>
   ]],[[
PTRACE_GETREGS;
   ]])],[gdbsrv_cv_have_ptrace_getregs=yes],
        [gdbsrv_cv_have_ptrace_getregs=no])
  ])
  AC_MSG_RESULT([${gdbsrv_cv_have_ptrace_getregs}])
  if test "x${gdbsrv_cv_have_ptrace_getregs}" = "xyes"; then
    AC_DEFINE([HAVE_PTRACE_GETREGS],[1],
	      [Define to 1 if targ supports PTRACE_GETREGS for register ]
	      [access.])
  fi

  AC_MSG_CHECKING([for PTRACE_GETFPXREGS])
  AC_CACHE_VAL([gdbsrv_cv_have_ptrace_getfpxregs],
  [AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <sys/ptrace.h>
   ]],[[
PTRACE_GETFPXREGS;
   ]])],[gdbsrv_cv_have_ptrace_getfpxregs=yes],
        [gdbsrv_cv_have_ptrace_getfpxregs=no])
  ])
  AC_MSG_RESULT([${gdbsrv_cv_have_ptrace_getfpxregs}])
  if test "x${gdbsrv_cv_have_ptrace_getfpxregs}" = "xyes"; then
    AC_DEFINE([HAVE_PTRACE_GETFPXREGS],[1],
	      [Define to 1 if targ supports PTRACE_GETFPXREGS for extra ]
	      [register access.])
  fi
else
  AC_MSG_NOTICE([skipping Linux-specific regset tests])
fi

if test "x${srv_linux_btrace}" = "xyes"; then
  AC_DEFINE([HAVE_LINUX_BTRACE],[1],
	    [Define if the target supports branch tracing.])
fi

if test "x${ac_cv_header_sys_procfs_h}" = "xyes"; then
  BFD_HAVE_SYS_PROCFS_TYPE([lwpid_t])
  BFD_HAVE_SYS_PROCFS_TYPE([psaddr_t])
  BFD_HAVE_SYS_PROCFS_TYPE([prgregset_t])
  BFD_HAVE_SYS_PROCFS_TYPE([prfpregset_t])

  dnl# Check for broken prfpregset_t type

  dnl# For Linux/i386, glibc 2.1.3 was released with a bogus
  dnl# prfpregset_t type (it was a typedef for the pointer to a struct
  dnl# instead of the struct itself). We detect this here, and work
  dnl# around it in gdb_proc_service.h.

  if test "x${bfd_cv_have_sys_procfs_type_prfpregset_t}" = "xyes"; then
    AC_MSG_CHECKING([whether prfpregset_t type is broken])
    AC_CACHE_VAL([gdb_cv_prfpregset_t_broken],
      [AC_RUN_IFELSE([AC_LANG_SOURCE([[
       #include <sys/procfs.h>
       int main(void)
       {
         if (sizeof(prfpregset_t) == sizeof(void *)) {
           return 1;
         }
         return 0;
       }]])],[gdb_cv_prfpregset_t_broken=no],
             [gdb_cv_prfpregset_t_broken=yes],
             [gdb_cv_prfpregset_t_broken=yes])
    ])
    AC_MSG_RESULT([${gdb_cv_prfpregset_t_broken}])
    if test "x${gdb_cv_prfpregset_t_broken}" = "xyes"; then
      AC_DEFINE([PRFPREGSET_T_BROKEN],[1],
		[Define to 1 if the prfpregset_t type is broken.])
    fi
  fi

  BFD_HAVE_SYS_PROCFS_TYPE([elf_fpregset_t])
else
  AC_MSG_NOTICE([skipping tests that require a working system procfs])
fi

dnl# Check for libdl, but do not add it to LIBS as only gdbserver
dnl# needs it (and gdbreplay does NOT).
old_LIBS="${LIBS}"
AC_CHECK_LIB([dl],[dlopen])
LIBS="${old_LIBS}"

srv_thread_depfiles=""
srv_libs=""
USE_THREAD_DB=""

if test "x${srv_linux_thread_db}" = "xyes"; then
  SRV_CHECK_THREAD_DB
  if test "x${srv_cv_thread_db}" = "xno"; then
    AC_MSG_WARN([Could not find libthread_db.])
    AC_MSG_WARN([Disabling thread support in gdbserver.])
    srv_linux_thread_db=no
  else
    srv_libs="${srv_cv_thread_db}"
  fi
  if test "x${srv_libs}" = "x"; then
    test -z "${srv_libs}"
    if test "x${ac_cv_lib_dl_dlopen}" = "xyes"; then
      srv_libs="-ldl"
    else
      srv_libs="-lthread_db"
    fi
  fi
  AC_MSG_CHECKING([for -rdynamic linker flag])
  old_LDFLAGS="${LDFLAGS}"
  LDFLAGS="${LDFLAGS} -rdynamic"
  AC_LINK_IFELSE([AC_LANG_PROGRAM([[]],[[]])],
                 [RDYNAMIC="-rdynamic"],[RDYNAMIC=""])
  AC_SUBST([RDYNAMIC])
  LDFLAGS="${old_LDFLAGS}"
  AC_MSG_RESULT([${RDYNAMIC}])
else
  AC_MSG_NOTICE([skipping more Linux-specific tests (for thread_db here)])
fi

if test "x${srv_linux_thread_db}" = "xyes"; then
  srv_thread_depfiles="thread-db.o proc-service.o"
  USE_THREAD_DB="-DUSE_THREAD_DB"
  AC_CACHE_CHECK([for TD_VERSION],[gdbsrv_cv_have_td_version],
  [AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <thread_db.h>
   ]],[[
TD_VERSION;
   ]])],[gdbsrv_cv_have_td_version=yes],
        [gdbsrv_cv_have_td_version=no])
  ])
  if test "x${gdbsrv_cv_have_td_version}" = "xyes"; then
    AC_DEFINE([HAVE_TD_VERSION],[1],[Define if TD_VERSION is available.])
  fi
else
  AC_MSG_NOTICE([skipping even more Linux-specific thread_db checks])
fi

# APPLE LOCAL - This is to get me to macosx...
if test "x${srv_platform_include}" = "x"; then
  GDBSERVER_PLATFORM_INCLUDE=""
else
  GDBSERVER_PLATFORM_INCLUDE="${srv_platform_include}"
fi
AC_MSG_NOTICE([includes is "${srv_platform_include}"])
AC_MSG_NOTICE([PLATFORM is: "${GDBSERVER_PLATFORM_INCLUDE}"])
AC_SUBST([GDBSERVER_PLATFORM_INCLUDE])
# END APPLE LOCAL

if test "x${srv_libs}" != "x" -a "x${srv_libs}" != "x-ldl"; then
  AC_DEFINE([USE_LIBTHREAD_DB_DIRECTLY],[1],
            [Define if we should use libthread_db directly.])
fi

if test "x${srv_xmlfiles}" != "x"; then
  srv_xmlbuiltin="xml-builtin.o"
  AC_DEFINE([USE_XML],[1],
            [Define if an XML target description is available.])

  tmp_xmlfiles=${srv_xmlfiles}
  srv_xmlfiles=""
  for f in ${tmp_xmlfiles}; do
    srv_xmlfiles="${srv_xmlfiles} \$(XML_DIR)/${f}"
  done
fi

GDBSERVER_DEPFILES="${srv_regobj} ${srv_tgtobj} ${srv_hostio_err_objs} ${srv_thread_depfiles}"
AC_PATH_PROGS([UNIQ_BIN],[uniq guniq],[])
# uniq the depfiles.
f=""
for i in ${GDBSERVER_DEPFILES}; do
    case " ${f} " in
    *" ${i} "*) ;;
    *) f="${f} ${i}" ;;
    esac
done
GDBSERVER_DEPFILES="${f}"
AC_MSG_NOTICE([depfiles needed for gdbserver are: ${GDBSERVER_DEPFILES}])
GDBSERVER_LIBS="${srv_libs}"
if test "x${GDBSERVER_LIBS}" != "x"; then
  test -n "${GDBSERVER_LIBS}"
  AC_MSG_NOTICE([libs needed for gdbserver are '${GDBSERVER_LIBS}'])
fi

dnl# Check whether the target supports __sync_*_compare_and_swap.
AC_CACHE_CHECK([whether the target supports __sync_*_compare_and_swap],
	       [gdbsrv_cv_have_sync_builtins],[
AC_LINK_IFELSE([AC_LANG_PROGRAM([[
 ]],[[
int foo, bar; bar = __sync_val_compare_and_swap(&foo, 0, 1);
 ]])],[gdbsrv_cv_have_sync_builtins=yes],
      [gdbsrv_cv_have_sync_builtins=no])
])
if test "x${gdbsrv_cv_have_sync_builtins}" = "xyes"; then
  AC_DEFINE([HAVE_SYNC_BUILTINS],[1],
    [Define to 1 if the target supports __sync_*_compare_and_swap])
fi

dnl# Check for -fvisibility=hidden support in the compiler.
saved_cflags="${CFLAGS}"
CFLAGS="${CFLAGS} -fvisibility=hidden"
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[]],[[]])],
                  [gdbsrv_cv_have_visibility_hidden=yes],
                  [gdbsrv_cv_have_visibility_hidden=no])
CFLAGS="${saved_cflags}"

dnl# Check if we can disable the virtual address space randomization.
dnl# The functionality of setarch -R.
AC_CHECK_DECLS([ADDR_NO_RANDOMIZE],[],[],[#include <sys/personality.h>])
define([PERSONALITY_TEST],[AC_LANG_PROGRAM([#include <sys/personality.h>],[
#      if !HAVE_DECL_ADDR_NO_RANDOMIZE
#       define ADDR_NO_RANDOMIZE 0x0040000
#      endif /* !HAVE_DECL_ADDR_NO_RANDOMIZE */
       /* Test the flag could be set and stays set.  */
       personality (personality (0xffffffff) | ADDR_NO_RANDOMIZE);
       if (!(personality (personality (0xffffffff)) & ADDR_NO_RANDOMIZE))
           return 1])])
AC_RUN_IFELSE([PERSONALITY_TEST],
              [gdbsrv_cv_have_personality=true],
              [gdbsrv_cv_have_personality=false],
              [AC_LINK_IFELSE([PERSONALITY_TEST],
                              [gdbsrv_cv_have_personality=true],
                              [gdbsrv_cv_have_personality=false])])
if ${gdbsrv_cv_have_personality}
then
    AC_DEFINE([HAVE_PERSONALITY],[1],
              [Define if you support the personality syscall.])
fi


IPA_DEPFILES=""
extra_libraries=""

# check whether to enable the inprocess agent:
if test "x${ipa_obj}" != "x" \
   -a "x${gdbsrv_cv_have_sync_builtins}" = "xyes" \
   -a "x${gdbsrv_cv_have_visibility_hidden}" = "xyes"; then
   have_ipa=true
else
   have_ipa=false
fi

if ${have_ipa}; then
  IPA_DEPFILES="${ipa_obj}"
  extra_libraries="${extra_libraries} libinproctrace.so"
else
  AC_MSG_WARN([inprocess agent not supported for this target])
fi

dnl# GDBSERVER_DEPFILES and GDBSERVER_LIBS are set farther up now.

AC_SUBST([GDBSERVER_DEPFILES])
AC_SUBST([GDBSERVER_LIBS])
AC_SUBST([srv_xmlbuiltin])
AC_SUBST([srv_xmlfiles])
AC_SUBST([IPA_DEPFILES])
AC_SUBST([extra_libraries])
AC_SUBST([USE_THREAD_DB])
AC_SUBST([CPPFLAGS])

dnl# warning flags get set at the bottom, to try to prevent them from
dnl# messing with the other conftets:
build_warnings="-Wdeclaration-after-statement -Wmissing-declarations \
-Wstrict-prototypes -Wold-style-definition -Wmissing-prototypes \
-Wpointer-arith -Wformat-nonliteral -Wno-char-subscripts -Wempty-body \
-Wnested-externs -Wno-unused-parameter -Wunused-macros -Wundef \
-Wparentheses -Winline -Wvolatile-register-var"

case "${host}" in
  *-apple-darwin* | *-apple-macos*)
    # -Wmost is APPLE_LOCAL, as are -Wextra-tokens and -Wnewline-eof
    build_warnings="${build_warnings} -Wmost -Wextra-tokens -Wnewline-eof"
  ;;
esac

# GCC supports -Wuninitialized only with -O or -On, n != 0.
# and also -Winit-self only with -Wuninitialized
if test x${CFLAGS+set} = xset; then
  case "${CFLAGS}" in
    *"-O0"* ) ;;
    *"-O"* )
      build_warnings="${build_warnings} -Wuninitialized -Winit-self"
    ;;
  esac
else
  build_warnings="${build_warnings} -Wuninitialized -Winit-self"
fi

WARN_CFLAGS=""
if test "x${GCC}" = xyes
then
    AC_MSG_CHECKING([compiler warning flags])
    for w in ${build_warnings}; do
	case ${w} in
	-Werr*)
	    dnl# (just ignoring this completely for now)
	    ;;
	*) # Check that GCC accepts it:
	    saved_CFLAGS="${CFLAGS}"
	    CFLAGS="${CFLAGS} ${w}"
	    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[]],[[]])],
	                      [WARN_CFLAGS="${WARN_CFLAGS} ${w}"],[])
	    CFLAGS="${saved_CFLAGS}"
	    ;;
	esac
    done
    AC_MSG_RESULT([${WARN_CFLAGS} ${WERROR_CFLAGS}])
fi
AC_SUBST([WARN_CFLAGS])

case ${target} in
*86*-apple-darwin*)
 if test -e macosx-i386-low.c; then
  AC_MSG_CHECKING([length of macosx-i386-low.c])
changequote(,)
  export MACOSX_I386_LOW_C_LENGTH=`wc -l < macosx-i386-low.c | tr -d [:blank:]`
changequote([,])
  AC_DEFINE_UNQUOTED([MACOSX_I386_LOW_C_LENGTH_C_DEFINE],
                     [${MACOSX_I386_LOW_C_LENGTH}],
                     [Length of macosx-i386-low.c in lines])
  AC_MSG_RESULT([${MACOSX_I386_LOW_C_LENGTH} lines])
 fi
 ;;
esac

AC_CONFIG_FILES([Makefile])
AC_CONFIG_COMMANDS([default],[case x${CONFIG_HEADERS} in
xconfig.h:config.in)
 export CPPFLAGS="${CPPFLAGS} -DHAVE_CONFIG_H"
 echo "warning: using wrong config header" | tee stamp-h ;;
xconfig.h)
 export CPPFLAGS="${CPPFLAGS} -DHAVE_CONFIG_H"
 echo "" > stamp-h ;;
x*)
 touch stamp-h ;;
esac
],[])
AC_OUTPUT
