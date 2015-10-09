dnl# configure.ac -- Backtrace configure script.           -*- Autoconf -*-
# Copyright (C) 2012-2014 Free Software Foundation, Inc.

# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:

#     (1) Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer. 

#     (2) Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in
#     the documentation and/or other materials provided with the
#     distribution.  
    
#     (3) The name of the author may not be used to
#     endorse or promote products derived from this software without
#     specific prior written permission.

# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
# INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
# STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
# IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

dnl# in case autoupdate changes this, this is supposed to say "2.64":
AC_PREREQ([2.64])
dnl# This is a hack because autoconf finds the AC\_INIT from
dnl# ../config/override.m4 first if it is present, which is annoying.
dnl# Feel free to re-define to whichever version of autoconf
dnl# that you are using:
m4_define([_GCC_AUTOCONF_VERSION],[2.69])
AC_INIT([libbacktrace],[version-unused],[],[libbacktrace])
AC_CONFIG_SRCDIR([backtrace.h])dnl
AC_CONFIG_AUX_DIR([..])dnl
AC_CONFIG_MACRO_DIR([..])dnl
AC_CONFIG_HEADER([config.h])dnl

if test -n "${with_target_subdir}" && test -d "${with_target_subdir}"; then
  AM_ENABLE_MULTILIB([],[..])
elif test "x${multi_basedir}" = "x"; then
  test -z "${multi_basedir}"
  if test "x${srcdir}" != "x"; then
    test -n "${srcdir}"
    if test "x${srcdir}" = "x."; then
      if test -n "${with_target_subdir}" && test "${with_target_subdir}" != "."; then
        multi_basedir="${srcdir}/${with_multisrctop}../.."
      else
        multi_basedir="${srcdir}/${with_multisrctop}.."
      fi
    else
      multi_basedir="${srcdir}/.."
    fi
  else
    test -z "${srcdir}"
    multi_basedir="./.."
  fi
else
  test -n "${multi_basedir}"
fi

AC_CANONICAL_TARGET

target_alias=${target_alias-${host_alias}}

AC_USE_SYSTEM_EXTENSIONS

libtool_VERSION=1:0:0
AC_SUBST([libtool_VERSION])dnl

# hack to do before automake is initialized:
if test -f ${srcdir}/config.status; then
  if test -w "${srcdir}"; then
    AC_MSG_NOTICE([config.status is already present, moving it aside.])
    mv -vf ${srcdir}/config.status ${srcdir}/config_status_backup.sh
    if test -e ${srcdir}/config_status_backup.sh; then
      chmod +x ${srcdir}/config_status_backup.sh 2>&1
    fi
  else
    AC_MSG_WARN([srcdir is unwritable])
  fi
fi
# 1.11.6: Require that version of automake.
# foreign: Do NOT require README, INSTALL, NEWS, etc.
# no-define: Do NOT define PACKAGE and VERSION.
# no-dependencies: Do NOT generate automatic dependencies.
#    (because it breaks when using bootstrap-lean, since some of the
#    headers are gone at "make install" time).
# -Wall: Issue all automake warnings.
AM_INIT_AUTOMAKE([1.11.6 foreign no-dist no-define no-dependencies -Wall])

AM_MAINTAINER_MODE([disable])dnl

AC_ARG_WITH([target-subdir],
[AS_HELP_STRING([--with-target-subdir=SUBDIR],
                [Configuring in a subdirectory for target])])dnl

unset ac_cv_c_compiler_gnu
unset ac_cv_prog_ac_ct_CC
unset ac_cv_prog_cc_c89
unset ac_cv_prog_cc_g

# We must force CC to /not/ be precious variables; otherwise
# the wrong, non-multilib-adjusted value will be used in multilibs.
# As a side effect, we have to subst CFLAGS ourselves.
m4_rename([_AC_ARG_VAR_PRECIOUS],[backtrace_PRECIOUS])
m4_define([_AC_ARG_VAR_PRECIOUS],[])
AC_PROG_CC
m4_rename_force([backtrace_PRECIOUS],[_AC_ARG_VAR_PRECIOUS])

AC_SUBST([CFLAGS])dnl

AC_LANG([C])dnl

m4_ifdef([AM_PROG_CC_C_O],[
  AM_PROG_CC_C_O
])dnl

m4_ifdef([AC_REQUIRE_AUX_FILE],[
  AC_REQUIRE_AUX_FILE([compile])dnl
  AC_REQUIRE_AUX_FILE([depcomp])dnl
  AC_REQUIRE_AUX_FILE([missing])dnl
])dnl

AC_PROG_CPP

if test "x${AR}" = "x" && test "x${am_cv_ar_interface}" = "x"; then
  test -z "${AR}" && test -z "${am_cv_ar_interface}"
  AM_PROG_AR
fi
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_RANLIB

unset ac_cv_prog_AWK

AC_PROG_AWK
case "${AWK}" in
"") AC_MSG_ERROR([cannot build without awk]) ;;
esac

AC_PROG_CXX
LT_INIT([disable-shared dlopen win32-dll])dnl
LT_LANG([C])dnl
LT_PREREQ([2.4.6])dnl

AC_C_INLINE

backtrace_supported=yes

if test -n "${with_target_subdir}" && test -d "${with_target_subdir}"; then
  # We are compiling a GCC library.  We can assume that the unwind
  # library exists.
  AC_MSG_NOTICE([assuming unwind library exists])
  BACKTRACE_FILE="backtrace.lo simple.lo"
else
  AC_CHECK_HEADER([unwind.h],
    [AC_CHECK_FUNC([_Unwind_Backtrace],
	           [BACKTRACE_FILE="backtrace.lo simple.lo"],
		   [BACKTRACE_FILE="nounwind.lo"
                    backtrace_supported=no])],
    [BACKTRACE_FILE="nounwind.lo"
     backtrace_supported=no])
fi
AC_SUBST([BACKTRACE_FILE])dnl

EXTRA_FLAGS=""
if test -n "${with_target_subdir}" && test -d "${with_target_subdir}"; then
  AC_MSG_NOTICE([assuming we have the -funwind-tables and -frandom-seed=string options])
  EXTRA_FLAGS="-funwind-tables -frandom-seed=\$@"
else
  AC_CACHE_CHECK([for -funwind-tables option],
    [libbacktrace_cv_c_unwind_tables],
    [CFLAGS_hold="${CFLAGS}"
     CFLAGS="${CFLAGS} -funwind-tables"
     AC_COMPILE_IFELSE(
       [AC_LANG_PROGRAM([[static int f() { return 0; }]],[[return f();]])],
       [libbacktrace_cv_c_unwind_tables=yes],
       [libbacktrace_cv_c_unwind_tables=no])
     CFLAGS="${CFLAGS_hold}"])
  if test "x${libbacktrace_cv_c_unwind_tables}" = "xyes"; then
    EXTRA_FLAGS=-funwind-tables
  fi
  AC_CACHE_CHECK([for -frandom-seed=string option],
    [libbacktrace_cv_c_random_seed_string],
    [CFLAGS_hold="${CFLAGS}"
     CFLAGS="${CFLAGS} -frandom-seed=conftest.lo"
     AC_COMPILE_IFELSE(
       [AC_LANG_PROGRAM([[]],[[return 0;]])],
       [libbacktrace_cv_c_random_seed_string=yes],
       [libbacktrace_cv_c_random_seed_string=no])
     CFLAGS="${CFLAGS_hold}"])
  if test "x${libbacktrace_cv_c_random_seed_string}" = "xyes"; then
    EXTRA_FLAGS="${EXTRA_FLAGS} -frandom-seed=\$@"
  fi
fi
AC_SUBST([EXTRA_FLAGS])dnl

dnl# usability of 'const' could affect '-Wwrite-strings':
AC_C_CONST

dnl# '-W' is redundant with '-Wextra'
ACX_PROG_CC_WARNING_OPTS([-Wall -Wextra -Wwrite-strings -Wnested-externs \
-Wstrict-prototypes -Wmissing-prototypes -Wmissing-parameter-type -Wodr \
-Wold-style-definition -Wold-style-declaration -Wc++-compat -Wswitch \
-Wmissing-format-attribute -Wcast-qual -Wmisleading-indentation -Wabi \
-Wnull-dereference],[WARN_FLAGS])dnl

# deal with potential name mismatch:
if test "x${WARN_FLAGS}" = "x" && test "x${WARN_CFLAGS}" != "x"; then
  test -z "${WARN_FLAGS}" && test -n "${WARN_CFLAGS}" && export WARN_FLAGS="${WARN_CFLAGS}"
fi

if test -n "${with_target_subdir}" && test -d "${with_target_subdir}"; then
  AC_MSG_WARN([adding -Werror to WARN_FLAGS])
  WARN_FLAGS="${WARN_FLAGS} -Werror"
fi

AC_SUBST([WARN_FLAGS])dnl

if test -n "${with_target_subdir}" && test -d "${with_target_subdir}"; then
  GCC_CHECK_UNWIND_GETIPINFO
else
  ac_save_CFFLAGS="${CFLAGS}"
  CFLAGS="${CFLAGS} -Werror-implicit-function-declaration"
  AC_MSG_CHECKING([for _Unwind_GetIPInfo])
  AC_LINK_IFELSE(
    [AC_LANG_PROGRAM(
       [[#include "unwind.h"
	 struct _Unwind_Context *context;
	 int ip_before_insn = 0;]],
       [[return _Unwind_GetIPInfo (context, &ip_before_insn);]])],
    [have_unwind_getipinfo=yes],[have_unwind_getipinfo=no])
  CFLAGS="${ac_save_CFLAGS}"
  AC_MSG_RESULT([${have_unwind_getipinfo}])
  if test "x${have_unwind_getipinfo}" = "xyes"; then
    AC_DEFINE([HAVE_GETIPINFO],[1],
              [Define to 1 if _Unwind_GetIPInfo is available.])
  fi
fi

# When building as a target library, shared libraries may want to link
# this in.  We do NOT want to provide another shared library to
# complicate dependencies.  Instead, we just compile with -fPIC.
PIC_FLAG=""
if test -n "${with_target_subdir}" && test -d "${with_target_subdir}"; then
  case "${host}" in
  *-apple-darwin*) PIC_FLAG="-DPIC -fPIC -fno-common" ;;
  *) PIC_FLAG=-fPIC ;;
  esac
fi
# Similarly, use -fPIC with --enable-host-shared:
AC_ARG_ENABLE([host-shared],
[AS_HELP_STRING([--enable-host-shared],
		[build host code as shared libraries])],[
case "${host}" in
*-apple-darwin*) PIC_FLAG="-DPIC -fPIC -fno-common" ;;
*) PIC_FLAG=-fPIC ;;
esac
],[])dnl
AC_SUBST([PIC_FLAG])dnl

# Test for __sync support.
AC_CACHE_CHECK([__sync extensions],
[libbacktrace_cv_sys_sync],[
if test -n "${with_target_subdir}" && test -d "${with_target_subdir}"; then
   libbacktrace_cv_sys_sync=yes
else
   AC_LINK_IFELSE(
     [AC_LANG_PROGRAM([[int i;]],
                      [[__sync_bool_compare_and_swap (&i, i, i);
                        __sync_lock_test_and_set (&i, 1);
                        __sync_lock_release (&i);]])],
     [libbacktrace_cv_sys_sync=yes],
     [libbacktrace_cv_sys_sync=no])
fi])
BACKTRACE_SUPPORTS_THREADS=0
if test "x${libbacktrace_cv_sys_sync}" = "xyes"; then
  BACKTRACE_SUPPORTS_THREADS=1
  AC_DEFINE([HAVE_SYNC_FUNCTIONS],[1],
	    [Define to 1 if you have the __sync functions])
fi
AC_SUBST([BACKTRACE_SUPPORTS_THREADS])dnl

# Test for __atomic support.
AC_CACHE_CHECK([__atomic extensions],
[libbacktrace_cv_sys_atomic],[
if test -n "${with_target_subdir}" && test -d "${with_target_subdir}"; then
   libbacktrace_cv_sys_atomic=yes
else
   AC_LINK_IFELSE(
     [AC_LANG_PROGRAM([[int i;]],
     		      [[__atomic_load_n(&i, __ATOMIC_ACQUIRE);
		        __atomic_store_n(&i, 1, __ATOMIC_RELEASE);]])],
     [libbacktrace_cv_sys_atomic=yes],
     [libbacktrace_cv_sys_atomic=no])
fi])
if test "x${libbacktrace_cv_sys_atomic}" = "xyes"; then
  AC_DEFINE([HAVE_ATOMIC_FUNCTIONS],[1],
	    [Define to 1 if you have the __atomic functions])
fi

# The library needs to be able to read the executable itself.  Compile
# a file to determine the executable format.  The awk script
# filetype.awk prints out the file type.
AC_CACHE_CHECK([output filetype],
[libbacktrace_cv_sys_filetype],
[filetype=""
AC_COMPILE_IFELSE(
  [AC_LANG_PROGRAM([[int i;]],[[int j;]])],
  [filetype=`${AWK} -f ${srcdir}/filetype.awk conftest.${ac_objext}`],
  [AC_MSG_FAILURE([compiler failed])])
libbacktrace_cv_sys_filetype=${filetype}])dnl

# Match the file type to decide what files to compile.
FORMAT_FILE=""
backtrace_supports_data=yes
case "${libbacktrace_cv_sys_filetype}" in
elf*) FORMAT_FILE="elf.lo" ;;
mach-o*) AC_MSG_WARN([mach-o support is incomplete; treating as unknown])
         FORMAT_FILE="unknown.lo"
         backtrace_supports_data=no
         backtrace_supported=no
         ;;
*) AC_MSG_WARN([could not determine output file type])
   FORMAT_FILE="unknown.lo"
   backtrace_supported=no
   ;;
esac
AC_SUBST([FORMAT_FILE])dnl

# ELF defines.
elfsize=""
case "${libbacktrace_cv_sys_filetype}" in
elf32) elfsize=32 ;;
elf64) elfsize=64 ;;
esac
AC_DEFINE_UNQUOTED([BACKTRACE_ELF_SIZE],[${elfsize}],[ELF size: 32 or 64])

BACKTRACE_SUPPORTED=0
if test "x${backtrace_supported}" = "xyes"; then
  BACKTRACE_SUPPORTED=1
fi
AC_SUBST([BACKTRACE_SUPPORTED])dnl

BACKTRACE_SUPPORTS_DATA=0
if test "x${backtrace_supports_data}" = "xyes"; then
  BACKTRACE_SUPPORTS_DATA=1
fi
AC_SUBST([BACKTRACE_SUPPORTS_DATA])dnl

AC_CHECK_HEADERS([stddef.h])dnl

# check integer types before doing stdint header check:
AC_TYPE_INT8_T
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INTMAX_T
AC_TYPE_INTPTR_T
AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINTMAX_T
AC_TYPE_UINTPTR_T
GCC_HEADER_STDINT([gstdint.h])dnl

# Other types:
AC_TYPE_OFF_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

AC_CHECK_HEADERS([sys/mman.h])
if test "x${ac_cv_header_sys_mman_h}" = "xno"; then
  have_mmap=no
else
  if test -n "${with_target_subdir}" && test -d "${with_target_subdir}"; then
    # When built as a GCC target library, we cannot do a link test.  We
    # simply assume that if we have mman.h, we have mmap.
    AC_MSG_NOTICE([assuming we have mmap based on presence of mman.h])
    have_mmap=yes
  else
    AC_CHECK_FUNC([mmap],[have_mmap=yes],[have_mmap=no])dnl
    ## also related:
    AC_CHECK_FUNCS([munmap])
  fi
fi
if test "x${have_mmap}" = "xno"; then
  VIEW_FILE=read.lo
  ALLOC_FILE=alloc.lo
elif test "x${have_mmap}" = "x"; then
  AC_FUNC_MMAP dnl# calls some other things, as well...
  ## odd cache value:
  if test "x${ac_cv_func_mmap_fixed_mapped}" = "xyes"; then
    have_mmap=yes
  else
    have_mmap=no
  fi
fi

if test "x${have_mmap}" = "xyes"; then
  VIEW_FILE=mmapio.lo
  AC_PREPROC_IFELSE([AC_LANG_SOURCE([[
#include <sys/mman.h>
#if !defined(MAP_ANONYMOUS) && !defined(MAP_ANON)
  #error no MAP_ANONYMOUS
#endif /* !MAP_ANONYMOUS && !MAP_ANON */
]])],[ALLOC_FILE=mmap.lo],[ALLOC_FILE=alloc.lo])
fi
AC_SUBST([VIEW_FILE])dnl
AC_SUBST([ALLOC_FILE])dnl

AC_FUNC_MALLOC

BACKTRACE_USES_MALLOC=0
if test x"${ALLOC_FILE}" = x"alloc.lo"; then
  if test "x${ac_cv_func_malloc_0_nonnull}" = "xyes"; then
    BACKTRACE_USES_MALLOC=1
  else
    AC_MSG_WARN([libbacktrace wants to use malloc, but malloc is bad])
  fi
fi
AC_SUBST([BACKTRACE_USES_MALLOC])dnl

# Other memory functions:
AC_FUNC_MEMCMP
AC_CHECK_FUNCS([memset])dnl

AC_FUNC_REALLOC

# Check for dl_iterate_phdr.
AC_CHECK_HEADERS([link.h])
if test "x${ac_cv_header_link_h}" = "xno"; then
  have_dl_iterate_phdr=no
else
  if test -n "${with_target_subdir}"; then
    # When built as a GCC target library, we cannot do a link test.
    AC_EGREP_HEADER([dl_iterate_phdr],[link.h],[have_dl_iterate_phdr=yes],
		    [have_dl_iterate_phdr=no])
    case "${host}" in
    *-*-solaris2.10*)
        # Avoid dl_iterate_phdr on Solaris 10, where it is in the
    	# header file but is only in -ldl.
	have_dl_iterate_phdr=no ;;
    esac
  else
    AC_CHECK_FUNC([dl_iterate_phdr],[have_dl_iterate_phdr=yes],
		  [have_dl_iterate_phdr=no])
  fi
fi
if test "x${have_dl_iterate_phdr}" = "xyes"; then
  AC_DEFINE([HAVE_DL_ITERATE_PHDR],[1],
            [Define to 1 if dl_iterate_phdr is available.])
fi

# Check for the fcntl function.
if test -n "${with_target_subdir}" && test -d "${with_target_subdir}"; then
   case "${host}" in
   *-*-mingw*) have_fcntl=no ;;
   *) have_fcntl=yes ;;
   esac
else
  AC_CHECK_HEADERS([fcntl.h])dnl
  ## the function for real now:
  AC_CHECK_FUNC([fcntl],[have_fcntl=yes],[have_fcntl=no])
fi
if test "x${have_fcntl}" = "xyes"; then
  AC_DEFINE([HAVE_FCNTL],[1],
	    [Define to 1 if you have the fcntl function])
fi

AC_FUNC_STRNLEN

AC_CHECK_DECLS([strnlen])dnl

# Other string functions:
AC_CHECK_FUNCS([strdup strerror])dnl

# Check for getexecname function.
if test -n "${with_target_subdir}" && test -d "${with_target_subdir}"; then
   case "${host}" in
   *-*-solaris2*) have_getexecname=yes ;;
   *) have_getexecname=no ;;
   esac
else
  AC_CHECK_FUNC([getexecname],[have_getexecname=yes],[have_getexecname=no])
fi
if test "x${have_getexecname}" = "xyes"; then
  AC_DEFINE([HAVE_GETEXECNAME],[1],[Define if getexecname is available.])
fi

AC_CACHE_CHECK([whether tests can run],
  [libbacktrace_cv_sys_native],
  [AC_RUN_IFELSE([AC_LANG_PROGRAM([[]],[[return 0;]])],
     [libbacktrace_cv_sys_native=yes],
     [libbacktrace_cv_sys_native=no],
     [libbacktrace_cv_sys_native=no])])
AM_CONDITIONAL([NATIVE],
               [test "x${libbacktrace_cv_sys_native}" = "xyes"])dnl

## this multilib stuff is annoying; why do I have to put up with it?
if test "x${multilib}" = "xyes"; then
  multilib_arg="--enable-multilib"
else
  multilib_arg=""
fi

# Output.
AC_CONFIG_FILES([Makefile backtrace-supported.h])dnl

# We need multilib support, but only if configuring for the target.
AC_CONFIG_COMMANDS([default],
[if test -n "${CONFIG_FILES}"; then
   if test -n "${with_target_subdir}" && test -d "${with_target_subdir}"; then
     # Multilibs need MULTISUBDIR defined correctly in certain makefiles so
     # that multilib installs will end up installed in the correct place.
     # The testsuite needs it for multilib-aware ABI baseline files.
     # To work around this not being passed down from config-ml.in ->
     # srcdir/Makefile.am -> srcdir/{src,libsupc++,...}/Makefile.am,
     # manually append it here.  Only modify Makefiles that have just
     # been created.
     #
     # Also, get rid of this simulated-VPATH thing that automake does.
     cat > vpsed << \_EOF
  s!`test -f '$<' || echo '$(srcdir)/'`!!
_EOF
     for i in ${SUBDIRS}; do
      case ${CONFIG_FILES} in
       *${i}/Makefile*)
	 echo "Adding MULTISUBDIR to ${i}/Makefile"
	 sed -f vpsed ${i}/Makefile | tee tmp
	 grep '^MULTISUBDIR =' Makefile | tee -a tmp
	 mv -v tmp ${i}/Makefile
	 ;;
      esac
     done
     rm vpsed
   else
     echo "no target subdir, so no need to bother with MULTISUBDIR"
   fi
 else
   test -z "${CONFIG_FILES}" && echo "CONFIG_FILES is empty..."
 fi
],[
# Variables needed in config.status (file generation) which are NOT already
# passed by autoconf.
SUBDIRS="${SUBDIRS}"
])dnl

AC_CONFIG_COMMANDS([config_status_vpath_build_fix],[
if test "${srcdir}" != "."; then
  if test -d "${srcdir}"; then
    if test ! -e `pwd`/config.status; then
      if test -e ${srcdir}/config.status; then
        cp -v ${srcdir}/config.status `pwd`/config.status || echo ""
      elif test -e ${srcdir}/config_status_backup.sh; then
        cp -v ${srcdir}/config_status_backup.sh `pwd`/config.status || true
      else
        echo "`pwd`/config.status is missing" >&2
      fi
    elif test -w "${srcdir}" -a ! -e ${srcdir}/config.status; then
      if test -e `pwd`/config.status; then
        cp -v `pwd`/config.status ${srcdir}/config.status || echo ""
      elif test -e `pwd`/config_status_backup.sh; then
        cp -v `pwd`/config_status_backup.sh ${srcdir}/config.status || true
      else
        echo "${srcdir}/config.status is missing" >&2
      fi
    else
      echo "config.status should be fine"
    fi
  else
    echo "uh oh, something happened to srcdir (i.e. '${srcdir}')..." >&2
  fi
fi
],[])dnl

AC_OUTPUT
