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
AC_CONFIG_HEADER([config.h])
AM_SANITY_CHECK
AM_MAINTAINER_MODE

# Checks for programs.
AC_PROG_CC
AC_PROG_CPP
#TODO: improve this check for profiling support:
AC_MSG_CHECKING([if we can do profiling])
if test "x${ac_cv_c_compiler_gnu}" = "xyes" -a "x${CC}" != "xclang"; then
  export PROFILE_CFLAGS="-pg"
  AC_SUBST([PROFILE_CFLAGS])
  AC_MSG_RESULT([yes, with ${PROFILE_CFLAGS}])
else
  AC_MSG_RESULT([no, we are not using a GNU compiler])
fi

AC_CANONICAL_TARGET

AC_PROG_INSTALL

AC_ARG_PROGRAM

# Checks for libraries.
AC_SEARCH_LIBS([xmalloc],[iberty c gnu])
AC_SEARCH_LIBS([xsnprintf],[iberty c gnu])
AC_SEARCH_LIBS([xstrdup],[iberty c gnu])

# Checks for header files.
AC_DEFUN([AC_REQUIRE_HEADER_STDC],[
  AC_REQUIRE([AC_HEADER_STDC])
])
AC_REQUIRE_HEADER_STDC
AC_SYS_POSIX_TERMIOS
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS([AvailabilityMacros.h arpa/inet.h errno.h fcntl.h \
                  libintl.h limits.h linux/elf.h mach/exception_types.h \
                  mach/mach.h mach/mach_error.h mach/mach_types.h \
                  mach/mach_vm.h mach/machine/thread_status.h mach/port.h \
                  mach-o/nlist.h malloc.h malloc/malloc.h netdb.h \
                  netinet/in.h netinet/tcp.h prctl.h proc_service.h \
                  pthread.h sgtty.h signal.h stddef.h stdio.h sys/file.h \
                  sys/ioctl.h sys/param.h sys/ptrace.h sys/signal.h \
                  sys/socket.h sys/procfs.h sys/reg.h sys/time.h \
                  termio.h termios.h thread_db.h])
AC_CHECK_HEADERS_ONCE([sys/wait.h])

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

# Checks for declarations
AC_CHECK_DECLS([child_get_pagesize])
AC_CHECK_DECLS([strerror])
AC_CHECK_DECLS([NULL])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_BIGENDIAN
AC_C_CHAR_UNSIGNED
AC_C_INLINE
AC_C_PROTOTYPES
AC_C_VOLATILE
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
AC_CHECK_TYPES([ptrdiff_t])
AC_CHECK_TYPES([ptid_t])
AC_CHECK_TYPES([exception_mask_t],[],[],[
#ifdef HAVE_MACH_EXCEPTION_TYPES_H
# include <mach/exception_types.h>
#else
# ifdef __APPLE__
#  warning this conftest expects <mach/exception_types.h> to be included.
# endif /* __APPLE__ */
#endif /* HAVE_MACH_EXCEPTION_TYPES_H */
])
AC_CHECK_TYPES([task_t],[],[],[
#ifdef HAVE_MACH_MACH_H
# include <mach/mach.h>
#else
# ifdef __APPLE__
#  warning this conftest expects <mach/mach.h> to be included.
# endif /* __APPLE__ */
#endif /* HAVE_MACH_MACH_H */
])
AC_CHECK_TYPES([socklen_t],[],[],[
#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#else
# warning this conftest expects <sys/types.h> to be included.
#endif /* HAVE_SYS_TYPES_H */
#ifdef HAVE_SYS_SOCKET_H
# include <sys/socket.h>
#else
# warning this conftest expects <sys/socket.h> to be included.
#endif /* HAVE_SYS_SOCKET_H */
])
AC_CHECK_TYPES([mach_vm_size_t],[],[],[
#ifdef HAVE_MACH_MACH_VM_H
# include <mach/mach_vm.h>
#else
# ifdef __APPLE__
#  warning this conftest expects <mach/mach_vm.h> to be included.
# endif /* __APPLE__ */
#endif /* HAVE_MACH_MACH_VM_H */
])
AC_CHECK_TYPES([vm_size_t],[],[],[
#ifdef HAVE_MACH_MACH_VM_H
# include <mach/mach_vm.h>
#else
# ifdef __APPLE__
#  warning this conftest expects <mach/mach_vm.h> to be included.
# endif /* __APPLE__ */
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

# Checks for library functions.
AC_FUNC_ALLOCA
AC_FUNC_ERROR_AT_LINE
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_FUNC_MMAP
AC_FUNC_REALLOC
AC_CHECK_FUNCS([atexit child_get_pagesize gettimeofday inet_ntoa memset \
                minus_one_ptid select setenv socket strchr strdup \
                strerror strlen strncmp strrchr strtol strtoul strtoull \
                xmalloc xsnprintf xstrdup])

AC_MSG_NOTICE([sourcing ${srcdir}/configure.srv to check for ${target}])
. ${srcdir}/configure.srv

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
  [AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <sys/ptrace.h>]], [[PTRACE_GETREGS;]])],[gdbsrv_cv_have_ptrace_getregs=yes],[gdbsrv_cv_have_ptrace_getregs=no])])
  AC_MSG_RESULT([${gdbsrv_cv_have_ptrace_getregs}])
  if test "x${gdbsrv_cv_have_ptrace_getregs}" = "xyes"; then
    AC_DEFINE([HAVE_PTRACE_GETREGS],[1],
	      [Define to 1 if targ supports PTRACE_GETREGS for register ]
	      [access.])
  fi

  AC_MSG_CHECKING([for PTRACE_GETFPXREGS])
  AC_CACHE_VAL([gdbsrv_cv_have_ptrace_getfpxregs],
  [AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <sys/ptrace.h>]], [[PTRACE_GETFPXREGS;]])],[gdbsrv_cv_have_ptrace_getfpxregs=yes],[gdbsrv_cv_have_ptrace_getfpxregs=no])])
  AC_MSG_RESULT([${gdbsrv_cv_have_ptrace_getfpxregs}])
  if test "x${gdbsrv_cv_have_ptrace_getfpxregs}" = "xyes"; then
    AC_DEFINE([HAVE_PTRACE_GETFPXREGS],[1],
	      [Define to 1 if targ supports PTRACE_GETFPXREGS for extra ]
	      [register access.])
  fi
else
  AC_MSG_NOTICE([skipping Linux-specific tests])
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
      [AC_RUN_IFELSE([AC_LANG_SOURCE([[#include <sys/procfs.h>
       int main ()
       {
         if (sizeof (prfpregset_t) == sizeof (void *))
           return 1;
         return 0;
       }]])],[gdb_cv_prfpregset_t_broken=no],[gdb_cv_prfpregset_t_broken=yes],[gdb_cv_prfpregset_t_broken=yes])])
    AC_MSG_RESULT([$gdb_cv_prfpregset_t_broken])
    if test "x${gdb_cv_prfpregset_t_broken}" = "xyes"; then
      AC_DEFINE([PRFPREGSET_T_BROKEN],[1],
		[Define to 1 if the prfpregset_t type is broken.])
    fi
  fi

  BFD_HAVE_SYS_PROCFS_TYPE([elf_fpregset_t])
else
  AC_MSG_NOTICE([skipping tests that require a working system procfs])
fi

srv_thread_depfiles=""
srv_libs=""
USE_THREAD_DB=""

if test "x${srv_linux_thread_db}" = "xyes"; then
  SRV_CHECK_THREAD_DB
  if test "${srv_cv_thread_db}" = no; then
    AC_MSG_WARN([Could not find libthread_db.])
    AC_MSG_WARN([Disabling thread support in gdbserver.])
    srv_linux_thread_db=no
  else
    srv_libs="${srv_cv_thread_db}"
  fi
  AC_MSG_CHECKING([for -rdynamic linker flag])
  old_LDFLAGS="${LDFLAGS}"
  LDFLAGS="${LDFLAGS} -rdynamic"
  AC_LINK_IFELSE([AC_LANG_PROGRAM([[]],[[]])],[RDYNAMIC=-rdynamic],[RDYNAMIC=])
  AC_SUBST([RDYNAMIC])
  LDFLAGS="${old_LDFLAGS}"
  AC_MSG_RESULT([${RDYNAMIC}])
else
  AC_MSG_NOTICE([skipping more Linux-specific tests])
fi

if test "x${srv_linux_thread_db}" = "xyes"; then
  srv_thread_depfiles="thread-db.o proc-service.o"
  USE_THREAD_DB="-DUSE_THREAD_DB"
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

GDBSERVER_DEPFILES="${srv_regobj} ${srv_tgtobj} ${srv_thread_depfiles}"
GDBSERVER_LIBS="${srv_libs}"

AC_SUBST([GDBSERVER_DEPFILES])
AC_SUBST([GDBSERVER_LIBS])
AC_SUBST([USE_THREAD_DB])
AC_SUBST([CPPFLAGS])

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
