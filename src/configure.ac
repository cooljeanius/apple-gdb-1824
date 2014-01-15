#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

#   Copyright (C) 1992, 1993, 1994, 1995, 1996, 1997, 1998,
#   1999, 2000, 2001, 2002, 2003, 2004 Free Software Foundation, Inc.
#
# This file is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc, 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.

##########################################################################
# WARNING: this file contains embedded tabs. Do NOT run untabify on this
# file.

sinclude(config/acx.m4)
m4_include(config/elf.m4)
m4_include([libtool.m4])
m4_include([ltoptions.m4])
m4_include([ltsugar.m4])
m4_include([ltversion.m4])
m4_include([lt~obsolete.m4])

AC_PREREQ([2.69])
dnl# This is a hack because autoconf finds the AC\_INIT from
dnl# config/override.m4 first if it is present.
dnl# Feel free to re-define to whichever version of autoconf
dnl# that you are using:
m4_define([_GCC_AUTOCONF_VERSION],[2.69])
dnl# OK, now that we have that out of the way, we can actually get started:
AC_INIT([apple-gdb-src],[1824],[gclayton@apple.com])
dnl# bug report address is email address of last person to touch this
dnl# directory, according to the Changelog-Apple in this directory
AC_CONFIG_SRCDIR([move-if-change])
AC_DISABLE_OPTION_CHECKING
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])

# System-detecting stuff
AC_CANONICAL_TARGET
ACX_NONCANONICAL_BUILD
ACX_NONCANONICAL_HOST
ACX_NONCANONICAL_TARGET
AC_ARG_PROGRAM
AC_OBJEXT
AC_EXEEXT
AC_USE_SYSTEM_EXTENSIONS

# Autobuild
AB_INIT

# Check for an already-installed autogen
AG_PATH_AUTOOPTS([5.17.4])
# TODO: figure out how to pass the --disable-opts-test configure flag
# to the configure script in the "utils" subdirectory.

# Get 'install' or 'install-sh' and its variants.
AC_PROG_INSTALL
AM_PROG_INSTALL_STRIP
ACX_PROG_LN
AC_PROG_LN_S

### We might need to use some other shell than /bin/sh for running
### subshells. If we are on Windows, search for the shell. This will permit
### people to not have /bin/sh, but to be able to see /SOME/PATH/sh
### configure without also having to set CONFIG_SHELL. This code will work
### when using bash, which sets OSTYPE.
case "${OSTYPE}" in
*win32*)
  if test x${CONFIG_SHELL} = x ; then
    if test ! -f /bin/sh ; then
      if test x${SHELL} != x && test -f ${SHELL} ; then
	CONFIG_SHELL=${SHELL}
	export CONFIG_SHELL
      else
	for prog in sh sh.exe bash bash.exe; do
	  IFS="${IFS=	}"; save_ifs="${IFS}"; IFS="${IFS}:"
	  for dir in ${PATH}; do
	    test -z "${dir}" && dir=.
	    if test -f ${dir}/${prog}; then
	      CONFIG_SHELL=${dir}/${prog}
	      export CONFIG_SHELL
	      break
	    fi
	  done
	  IFS="${save_ifs}"
	  test -n "${CONFIG_SHELL}" && break
	done
      fi
    fi
  fi
  ;;
esac

config_shell=${CONFIG_SHELL-/bin/sh}

# Another shell check:
AC_SYS_INTERPRETER

progname=${0}
# if PWD already has a value, it is probably wrong.
if test -n "${PWD}" ; then PWD=`${PWDCMD-pwd}`; fi

# Test for sed
AC_PROG_SED
# ...and for grep
if test "x${GREP}" = "x"; then
  test -z "${GREP}"
  AC_PROG_GREP
else
  test ! -z "${GREP}"
fi

# Export original configure arguments for use by sub-configures. These
# will be expanded by make, so quote '$'.
tmp="${progname} $@"
sed -e 's,\$,$$,g' <<EOF_SED > conftestsed.out
${tmp}
EOF_SED
TOPLEVEL_CONFIGURE_ARGUMENTS=`cat conftestsed.out`
rm -f conftestsed.out
AC_SUBST([TOPLEVEL_CONFIGURE_ARGUMENTS])

moveifchange=${srcdir}/move-if-change

srcpwd=`cd ${srcdir} ; ${PWDCMD-pwd}`

# We pass INSTALL explicitly to sub-makes. Make sure that it is not
# a relative path.
if test "${INSTALL}" = "${srcdir}/install-sh -c"; then
  INSTALL="${srcpwd}/install-sh -c"
fi

AC_SUBST([INSTALLdir])
AC_SUBST([TEMPLATEdir])

# Check Make
AC_PROG_MAKE_SET

# Set srcdir to "." if that is what it is.
# This is important for multilib support.
pwd=`${PWDCMD-pwd}`
if test "${pwd}" = "${srcpwd}" ; then
  srcdir=.
fi

topsrcdir=${srcpwd}

extra_host_args=""

# Test for awk
AC_PROG_AWK

### To add a new directory to the tree, first choose whether it is a target
### or a host dependent tool. Then put it into the appropriate list
### (library or tools, host or target), doing a dependency sort.

# Subdirs will be configured in the order listed in build_configdirs, 
# configdirs, or target_configdirs; see the serialization section below.

# Dependency sorting is only needed when *configuration* must be done in 
# a particular order. In all cases a dependency should be specified in 
# the Makefile, whether or not it is implicitly specified here.

# Double entries in build_configdirs, configdirs, or target_configdirs may
# cause circular dependencies and break everything horribly.

# these library is used by various programs built for the build
# environment
#
build_libs="build-libiberty"

# these tools are built for the build environment
build_tools="build-autogen build-texinfo build-byacc build-flex build-bison build-m4 build-fixincludes"

# these libraries are used by various programs built for the host
# environment
#
host_libs="intl electric-fence mmalloc libiberty libdecnumber opcodes bfd readline tcl tk itcl libgui zlib libcpp libbacktrace libelf libiconv"

# These tools are built for the host environment
# Note, the powerpc-eabi build depends on sim occurring before gdb in order
# to know that we are building the simulator.
# binutils, gas and ld appear in that order because it makes sense to run
# "make check" in that particular order.
# If --enable-gold is used, "gold" may replace "ld".
host_tools="texinfo byacc flex bison binutils gas ld fixincludes gcc sid sim gdb make patch prms send-pr gprof etc expect dejagnu ash bash bzip2 m4 autoconf autogen automake libtool diff rcs fileutils shellutils time textutils wdiff find uudecode hello tar gzip indent recode release sed utils guile perl gawk findutils gettext zip fastjar gnattools cgen"

# libgcj represents the runtime libraries only used by gcj.
libgcj="target-libffi \
	target-boehm-gc \
	target-zlib \
	target-qthreads \
	target-libjava"

# These libraries are built for the target environment, and are built after
# the host libraries and the host tools (which may be a cross compiler)
#
target_libraries="target-libiberty \
		target-libgcc \
		target-libbacktrace \
		target-libgloss \
		target-newlib \
		target-libgomp \
		target-libatomic \
		target-libitm \
		target-libstdc++-v3 \
		target-libsanitizer \
		target-libmudflap \
		target-libvtv \
		target-libssp \
		target-libquadmath \
		target-libgfortran \
		target-boehm-gc \
		${libgcj} \
		target-libobjc \
		target-libada \
		target-libgo"

# these tools are built using the target libraries, and are intended to
# run only in the target environment
#
# note: any program that *uses* libraries that are in the
# "target_libraries" list belongs in this list. Those programs
# are also very likely candidates for the "native_only" list
# which follows
#
target_tools="target-examples target-groff target-gperf target-rda"

###########################################################################

## All tools belong in one of the four categories, and are assigned above
## We assign ${configdirs} this way to remove all embedded newlines. This
## is important because configure will choke if they ever get through.
## ${configdirs} is directories we build using the host tools.
## ${target_configdirs} is directories we build using the target tools.
configdirs=`echo ${host_libs} ${host_tools}`
target_configdirs=`echo ${target_libraries} ${target_tools}`
build_configdirs=`echo ${build_libs} ${build_tools}`

# Only make build modules if build != host.
# This should be done more generally, but at the moment it does NOT matter.
# APPLE LOCAL: Use "host" over "host_alias" - autoconf 2.13/2.57 diff?
if test ${host} != ${build} ; then
  # This is the only build module.
  build_modules=libiberty
else
  build_modules=
fi

###########################################################################

srcname="gnu development package"

# This gets set non-empty for some net releases of packages.
appdirs=""

# Define is_cross_compiler to save on calls to 'test'.
is_cross_compiler=
if test x"${host}" = x"${target}" ; then
  is_cross_compiler=no
else
  is_cross_compiler=yes
fi	

# Find the build and target subdir names.
GCC_TOPLEV_SUBDIRS
# Be sure to cover against remnants of an in-tree build.
if test ${srcdir} != .  && test -d ${srcdir}/host-${host_noncanonical}; then
  AC_MSG_WARN([building out of tree but ${srcdir} (aka srcdir) contains host-${host_noncanonical}.
Use a pristine source tree when building in a separate tree])
fi

# Skipdirs are removed silently.
skipdirs=""
# Noconfigdirs are removed loudly.
noconfigdirs=""

use_gnu_ld=""
# Make sure we do NOT let GNU ld be added if we did NOT want it.
if test x${with_gnu_ld} = xno ; then
  use_gnu_ld=no
  noconfigdirs="${noconfigdirs} ld gold"
fi

use_gnu_as=""
# Make sure we do NOT let GNU as be added if we did NOT want it.
if test x${with_gnu_as} = xno ; then
  use_gnu_as=no
  noconfigdirs="${noconfigdirs} gas"
fi

use_included_zlib="yes"
# Make sure we do NOT let ZLIB be added if we did NOT want it.
if test x${with_system_zlib} = xyes ; then
  use_included_zlib=no
  noconfigdirs="${noconfigdirs} zlib"
fi

# some tools are so dependent upon X11 that if we are not building with X, 
# it is not even worth trying to configure, much less build, that tool.

case ${with_x} in
  yes | "")
    AC_PATH_X
    AC_PATH_XTRA
    ;; # the default value for this tree is that X11 is available
  no)
    skipdirs="${skipdirs} tk itcl libgui"
    # We will not be able to build gdbtk without X.
    enable_gdbtk=no 
    ;;
  *)  echo "*** bad value \"${with_x}\" for -with-x flag; ignored" 1>&2 ;;
esac

AC_MSG_CHECKING([subdirectories to skip])
AC_MSG_RESULT([(will announce results later)])
# Some tools are only suitable for building in a "native" situation.
# Remove these if host!=target.  
native_only="autoconf autogen automake libtool fileutils find gawk gettext gzip hello indent m4 rcs recode sed shellutils tar textutils texinfo uudecode wdiff gprof target-groff guile perl time ash bash bzip2 prms gnuserv target-gperf"

# Similarly, some are only suitable for cross toolchains.
# Remove these if host=target.
cross_only="target-libgloss target-newlib target-opcodes"

case ${is_cross_compiler} in
  no) skipdirs="${skipdirs} ${cross_only}" ;;
  yes) skipdirs="${skipdirs} ${native_only}" ;;
esac

# If both --with-headers and --with-libs are specified, default to
# --without-newlib.
if test x"${with_headers}" != x && test x"${with_headers}" != xno \
   && test x"${with_libs}" != x && test x"${with_libs}" != xno ; then
  if test x"${with_newlib}" = x ; then
    with_newlib=no
  fi
fi

# Recognize --with-newlib/--without-newlib.
case ${with_newlib} in
  no) skipdirs="${skipdirs} target-newlib" ;;
  yes) skipdirs=`echo " ${skipdirs} " | sed -e 's/ target-newlib / /'` ;;
esac

# Handle --enable-gold, --enable-ld.
# --disable-gold [--enable-ld]
#     Build only ld.  Default option.
# --enable-gold [--enable-ld]
#     Build both gold and ld.  Install gold as "ld.gold", install ld
#     as "ld.bfd" and "ld".
# --enable-gold=default [--enable-ld]
#     Build both gold and ld.  Install gold as "ld.gold" and "ld",
#     install ld as "ld.bfd".
# --enable-gold[=default] --disable-ld
#     Build only gold, which is then installed as both "ld.gold" and "ld".
# --enable-gold --enable-ld=default
#     Build both gold (installed as "ld.gold") and ld (installed as "ld"
#     and ld.bfd).
#     In other words, ld is default
# --enable-gold=default --enable-ld=default
#     Error(?) (make warning instead here?).

default_ld=""
AC_ARG_ENABLE([gold],
[AS_HELP_STRING([[--enable-gold[=ARG]]],
		[build gold @<:@ARG={default,yes,no}@:>@])],
[ENABLE_GOLD=${enableval}],
[ENABLE_GOLD=no])
case "${ENABLE_GOLD}" in
  yes|default)
    # Check for ELF target.
    is_elf=no
    case "${target}" in
      *-*-android | *-*-elf* | *-*-sysv4* | *-*-unixware* | *-*-eabi* \
      | hppa*64*-*-hpux* | *-*-linux* | *-*-gnu* | frv-*-uclinux* \
      | *-*-irix5* | *-*-irix6* | *-*-netbsd* | *-*-openbsd* \
      | *-*-freebsd* | *-*-dragonfly* | *-*-solaris2* | *-*-nto* \
      | *-*-nacl*)
        case "${target}" in
          *-*-linux*aout* | *-*-linux*oldld*)
            ;;
          *)
            is_elf=yes
            ;;
        esac
    esac

    if test "${is_elf}" = "yes"; then
      # Check for target supported by gold.
      case "${target}" in
        i?86-*-* | x86_64-*-* | sparc*-*-* | powerpc*-*-* | arm*-*-* \
        | tilegx*-*-*)
	  configdirs="${configdirs} gold"
	  if test x${ENABLE_GOLD} = xdefault; then
	    default_ld=gold
	  fi
	  ENABLE_GOLD=yes
          ;;
      esac
    fi
    ;;
  no)
    ;;
  *)
    AC_MSG_ERROR([invalid --enable-gold argument])
    ;;
esac

AC_ARG_ENABLE([ld],
[AS_HELP_STRING([[--enable-ld[=ARG]]],
		[build ld @<:@ARG={default,yes,no}@:>@])],
[ENABLE_LD=${enableval}],
[ENABLE_LD=yes])

case "${ENABLE_LD}" in
  default)
    if test x${default_ld} != x; then
      AC_MSG_WARN([either gold or ld can be the default ld])
    fi
    ;;
  yes)
    ;;
  no)
    if test x${ENABLE_GOLD} != xyes; then
      AC_MSG_WARN([neither ld nor gold are enabled])
    fi
    configdirs=`echo " ${configdirs} " | sed -e 's/ ld / /'`
    ;;
  *)
    AC_MSG_ERROR([invalid --enable-ld argument])
    ;;
esac

# Configure extra directories which are host specific

case "${host}" in
  *-cygwin*)
    configdirs="${configdirs} libtermcap" ;;
esac

# A target can indicate whether a language is not supported for some
# reason.
# Only spaces may be used in this macro; not newlines or tabs.
unsupported_languages=

# Remove more programs from consideration, based on the host or 
# target this usually means that a port of the program does NOT
# exist yet.

case "${host}" in
  hppa*64*-*-*)
    noconfigdirs="${noconfigdirs} byacc"
    ;;
  i[[3456789]]86-*-vsta)
    noconfigdirs="${noconfigdirs} tcl expect dejagnu make texinfo bison patch flex byacc send-pr gprof uudecode dejagnu diff guile autogen perl itcl gnuserv gettext"
    ;;
  i[[3456789]]86-*-go32* | i[[3456789]]86-*-msdosdjgpp*)
    noconfigdirs="${noconfigdirs} tcl tk expect dejagnu send-pr uudecode guile autogen itcl gnuserv libffi"
    ;;
  i[[3456789]]86-*-mingw32*)
    noconfigdirs_commented_out="tcl tk expect dejagnu make texinfo bison patch flex byacc send-pr uudecode dejagnu diff guile autogen perl itcl gnuserv"
    noconfigdirs="${noconfigdirs} expect dejagnu autoconf autogen automake send-pr rcs guile perl texinfo libtool"
    ;;
  i[[3456789]]86-*-beos*)
    noconfigdirs="${noconfigdirs} tk itcl libgui gdb"
    ;;
  *-*-cygwin*)
    noconfigdirs="${noconfigdirs} autoconf autogen automake send-pr rcs guile perl"
    ;;
  *-*-netbsd*)
    noconfigdirs="${noconfigdirs} rcs"
    ;;
  ppc*-*-pe)
    noconfigdirs="${noconfigdirs} patch diff make tk tcl expect dejagnu autoconf autogen automake texinfo bison send-pr gprof rcs guile perl itcl gnuserv"
    ;;
  powerpc-*-beos*)
    noconfigdirs="${noconfigdirs} tk itcl libgui gdb dejagnu readline"
    ;;
esac


AC_ARG_ENABLE([libquadmath],
[AS_HELP_STRING([--disable-libquadmath],
  [do not build libquadmath directory])],
[ENABLE_LIBQUADMATH=${enableval}],
[ENABLE_LIBQUADMATH=yes])
if test "${ENABLE_LIBQUADMATH}" = "no" ; then
  noconfigdirs="${noconfigdirs} target-libquadmath"
fi


AC_ARG_ENABLE([libquadmath-support],
[AS_HELP_STRING([--disable-libquadmath-support],
  [disable libquadmath support for Fortran])],
[ENABLE_LIBQUADMATH_SUPPORT=${enableval}],
[ENABLE_LIBQUADMATH_SUPPORT=yes])
enable_libquadmath_support=
if test "${ENABLE_LIBQUADMATH_SUPPORT}" = "no" ; then
  enable_libquadmath_support=no
fi


AC_ARG_ENABLE([libada],
[AS_HELP_STRING([--enable-libada],[Builds libada directory])],
[ENABLE_LIBADA=${enableval}],
[ENABLE_LIBADA=yes])
if test "${ENABLE_LIBADA}" != "yes" ; then
  noconfigdirs="${noconfigdirs} target-libada"
  noconfigdirs="${noconfigdirs} gnattools"
fi

AC_ARG_ENABLE([libssp],
[AS_HELP_STRING([--enable-libssp],[Builds libssp directory])],
[ENABLE_LIBSSP=$enableval],
[ENABLE_LIBSSP=yes])
if test "${ENABLE_LIBSSP}" != "yes" ; then
  noconfigdirs="${noconfigdirs} target-libssp"
fi

AC_ARG_ENABLE([libstdcxx],
[AS_HELP_STRING([--disable-libstdcxx],
  [do not build libstdc++-v3 directory])],
[ENABLE_LIBSTDCXX=${enableval}],
[ENABLE_LIBSTDCXX=default])
[if test "${ENABLE_LIBSTDCXX}" = "no" ; then
  noconfigdirs="${noconfigdirs} libstdc++-v3"
fi]

# Save it here so that, even in case of --enable-libgcj, if the Java
# front-end is NOT enabled, we still get libgcj disabled.
libgcj_saved=${libgcj}
case ${enable_libgcj} in
yes)
  # If we reset it here, it will NOT get added to noconfigdirs in the
  # target-specific build rules, so it will be forcibly enabled
  # (unless the Java language itself is NOT enabled).
  libgcj=
  ;;
no)
  # Make sure we get it printed in the list of not supported target libs.
  # Do NOT disable libffi, though, other languages use it.
  noconfigdirs="${noconfigdirs} `echo ${libgcj} | sed -e 's/target-libffi//'`"
  # Clear libgcj_saved so that even if java is enabled libffi will NOT be
  # built.
  libgcj_saved=""
  ;;
esac

AC_ARG_ENABLE([static-libjava],
[AS_HELP_STRING([[--enable-static-libjava[=ARG]]],
		[build static libjava @<:@default=no@:>@])],
[ENABLE_STATIC_LIBJAVA=${enableval}],
[ENABLE_STATIC_LIBJAVA=no])
enable_static_libjava=""
if test "${ENABLE_STATIC_LIBJAVA}" = "yes" ; then
  enable_static_libjava=yes
fi

if test x${enable_static_libjava} != xyes ; then
  EXTRA_CONFIGARGS_LIBJAVA=--disable-static
fi
AC_SUBST([EXTRA_CONFIGARGS_LIBJAVA])

# Disable libgomp on non POSIX hosted systems.
if test x${enable_libgomp} = x ; then
    # Enable libgomp by default on hosted POSIX systems.
    case "${target}" in
    *-*-linux* | *-*-gnu* | *-*-k*bsd*-gnu | *-*-kopensolaris*-gnu)
	;;
    *-*-netbsd* | *-*-freebsd* | *-*-openbsd* | *-*-dragonfly*)
	;;
    *-*-solaris2* | *-*-hpux11*)
	;;
    *-*-darwin* | *-*-aix*)
	;;
    *)
	noconfigdirs="${noconfigdirs} target-libgomp"
	;;
    esac
fi

# Allow --disable-libmudflap to exclude target-libmudflap
case ${enable_libmudflap} in
yes)
    ;;
no)
    noconfigdirs="${noconfigdirs} target-libmudflap"
    ;;
"")
    case "${target}" in
    *-*-linux*-gnu | *-*-gnu* | *-*-k*bsd*-gnu)
        # Enable libmudflap by default in GNU and friends.
	;;
    *-*-freebsd*)
        # Enable libmudflap by default in FreeBSD.
	;;
    *)
        # Disable it by default everywhere else.
	noconfigdirs="${noconfigdirs} target-libmudflap"
	;;
    esac
esac

AC_ARG_ENABLE([64-bit-bfd],
[AS_HELP_STRING([--enable-64-bit-bfd],[64-bit bfd support (on hosts with narrower word sizes)])],
[case "${enableval}" in
  yes)  want64=true  ;;
  no)   want64=false ;;
  *)    AC_MSG_ERROR([bad value ${enableval} for 64-bit-bfd option]) ;;
esac],[want64=false])dnl
AC_SUBST([want64])

if test "x${want64}" = "xtrue"; then
  enable_64_bit_bfd=yes
fi

case "${target}" in
  *-*-chorusos)
    noconfigdirs="${noconfigdirs} target-newlib target-libgloss ${libgcj}"
    ;;
# APPLE LOCAL: Customize our platform configuration a bit.
  *-*-darwin*)
    # PR 46986
    noconfigdirs="${noconfigdirs} target-libgo"
# The Apple linker and assembler packages are part of the cctools and ld64
# packages respectively (see opensource.apple.com for more info)
# Thus we avoid building the GNU ones here:
    noconfigdirs="${noconfigdirs} ld gas gprof"
    noconfigdirs="${noconfigdirs} target-rda"
    noconfigdirs="${noconfigdirs} ${libgcj}"
# The "texinfo" subdir here does NOT actually contain the texinfo sources:
    if test ! -e texinfo/Makefile.am; then
      noconfigdirs="${noconfigdirs} build-texinfo texinfo"
    fi
# so far dejagnu works fine when configured on its own, but sometimes it
# fails as a subdirectory for the top-level Makefile...
    mynoconfigdirs="${noconfigdirs} dejagnu"
# likewise with expect
    if test ! -z "${build_alias}" -o ! -z "${host_alias}" -o ! -z "${target_alias}"; then
      noconfigdirs="${noconfigdirs} expect"
    fi
# I do not know why Apple had these put under noconfigdirs:
    applespecifiednoconfigdirs="${noconfigdirs} mmalloc sim"
    if test "${APPLE_INTERNAL_DIR}" = "/AppleInternal"; then
      test -d "${APPLE_INTERNAL_DIR}"
      AC_MSG_WARN([You appear to be at Apple, in that case disabling subdirectories that Apple says to disable (unset the APPLE_INTERNAL_DIR environment variable to avoid this)])
      noconfigdirs="${applespecifiednoconfigdirs}"
    fi
# building zlib from this level can mess it up; build it manually instead
    if test -z "${AR}" -o -z "${ARFLAGS}" -o "${AR}" = "libtool" -o "${AR}" = "ar" -o "${ARFLAGS}" = "-o"; then
      noconfigdirs="${noconfigdirs} zlib"
    fi
# autogen support in this tree is still tentative, so disable if guile is
# not also present in this tree
    if test ! -d guile; then
      noconfigdirs="${noconfigdirs} build-autogen autogen"
    fi
# I also do not know about a lot of the rest of these:
    enable_shared=no
    enable_64_bit_bfd=yes
    enable_softfloat=no
    host_configargs=" ${host_configargs} --disable-shared --enable-64-bit-bfd --disable-softfloat --disable-werror "

    AC_ARG_ENABLE([carbon-framework],
    [AS_HELP_STRING([--disable-carbon-framework],[disable Carbon framework use even when available])],
    [case ${enableval} in
      yes)
        host_configargs=" ${host_configargs} --enable-carbon-framework" ;;
      no) 
        host_configargs=" ${host_configargs} --disable-carbon-framework" ;;
      *)
        AC_MSG_ERROR([bad value '${enableval}' for --enable-carbon-framework]) ;;
    esac],[])

    AC_ARG_ENABLE([debug-symbols-framework],
    [AS_HELP_STRING([--disable-debug-symbols-framework],[disable DebugSymbols framework use even when available])],
    [case ${enableval} in
      yes)
        host_configargs=" ${host_configargs} --enable-debug-symbols-framework" ;;
      no) 
        host_configargs=" ${host_configargs} --disable-debug-symbols-framework" ;;
      *)
        AC_MSG_ERROR([bad value '${enableval}' for --enable-debug-symbols-framework]) ;;
    esac],[])

    AC_ARG_ENABLE([arch-for-exec],
      [AS_HELP_STRING([--disable-arch-for-exec],[disable use of arch for exec])],
      [case ${enableval} in
        yes) 
          host_configargs=" ${host_configargs} --enable-arch-for-exec" ;;
        no) 
          host_configargs=" ${host_configargs} --disable-arch-for-exec" ;;
        *)
          AC_MSG_ERROR([bad value '${enableval}' for --enable-arch-for-exec]) ;;
      esac],[])
    ;;
# END APPLE LOCAL
# Travis triplet:
  x86_64-unknown-linux-gnu*)
# Same as with Apple case above:
    if test ! -z "${build_alias}" -o ! -z "${host_alias}" -o ! -z "${target_alias}"; then
      noconfigdirs="${noconfigdirs} expect"
    fi
# Also same as with Apple case above:
    if test -z "${AR}" -o -z "${ARFLAGS}" -o "${ARFLAGS}" = "-o"; then
      noconfigdirs="${noconfigdirs} zlib"
    fi
# autogen support in this tree is still tentative, so disable if guile is
# not also present in this tree:
    if test ! -d guile; then
      noconfigdirs="${noconfigdirs} build-autogen autogen"
    fi
    ;;
  *-*-freebsd[[12]] | *-*-freebsd[[12]].* | *-*-freebsd*aout*)
    noconfigdirs="${noconfigdirs} target-newlib target-libgloss ${libgcj}"
    ;;
  *-*-freebsd*)
    noconfigdirs="${noconfigdirs} target-newlib target-libgloss"
    if test "x${with_gmp}" = x && test "x${with_gmp_dir}" = x \
	&& test -f /usr/local/include/gmp.h; then
      with_gmp=/usr/local
    fi

    # Skip some stuff that is unsupported on some FreeBSD configurations.
    case "${target}" in
      i*86-*-*) ;;
      alpha*-*-*) ;;
      *)
	noconfigdirs="${noconfigdirs} ${libgcj}"
	;;
    esac
    ;;
  *-*-kaos*)
    # Remove unsupported stuff on all kaOS configurations.
    skipdirs="target-libiberty ${libgcj} target-libstdc++-v3 target-librx"
    skipdirs="${skipdirs} target-libobjc target-examples target-groff target-gperf"
    skipdirs="${skipdirs} zlib fastjar target-libjava target-boehm-gc target-zlib"
    noconfigdirs="${noconfigdirs} target-libgloss"
    ;;
  *-*-netbsd*)
    # Skip some stuff on all NetBSD configurations.
    noconfigdirs="${noconfigdirs} target-newlib target-libiberty target-libgloss"

    # Skip some stuff that is unsupported on some NetBSD configurations.
    case "${target}" in
      i*86-*-netbsdelf*) ;;
      arm*-*-netbsdelf*) ;;
      *)
	noconfigdirs="${noconfigdirs} ${libgcj}"
	;;
    esac
    ;;
  *-*-netware*)
    noconfigdirs="${noconfigdirs} target-newlib target-libiberty target-libgloss ${libgcj} target-libmudflap"
    ;;
  *-*-rtems*)
    noconfigdirs="${noconfigdirs} target-libgloss ${libgcj}"
    ;;
    # The tpf target does NOT support gdb yet.
  *-*-tpf*)
    noconfigdirs="${noconfigdirs} target-newlib target-libgloss target-libiberty ${libgcj} target-libmudflap gdb tcl tk libgui itcl"
    ;;
  *-*-uclinux*)
    noconfigdirs="${noconfigdirs} target-newlib target-libgloss target-rda ${libgcj}"
    ;;
  *-*-vxworks*)
    noconfigdirs="${noconfigdirs} target-newlib target-libgloss target-libiberty ${libgcj}"
    ;;
  m32c-*-*)
    noconfigdirs="${noconfigdirs} target-libstdc++-v3"
    ;;
  alpha*-dec-osf*)
    # ld works, but does not support shared libraries.
    # newlib is not 64 bit ready. I am not sure about fileutils.
    # gas does NOT generate exception information.
    noconfigdirs="${noconfigdirs} gas ld fileutils target-newlib target-libgloss"
    ;;
  alpha*-*-*vms*)
    noconfigdirs="${noconfigdirs} gdb ld target-newlib target-libgloss ${libgcj}"
    ;;
  alpha*-*-linux*)
    # newlib is not 64 bit ready
    noconfigdirs="${noconfigdirs} target-newlib target-libgloss"
    ;;
  alpha*-*-*)
    # newlib is not 64 bit ready
    noconfigdirs="${noconfigdirs} target-newlib target-libgloss ${libgcj}"
    ;;
  am33_2.0-*-linux*)
    noconfigdirs="${noconfigdirs} ${libgcj} target-newlib target-libgloss"
    ;;
  sh-*-linux*)
    noconfigdirs="${noconfigdirs} ${libgcj} target-newlib target-libgloss"
    ;;    
  sh*-*-pe|mips*-*-pe|*arm-wince-pe)
    noconfigdirs="${noconfigdirs} ${libgcj}"
    noconfigdirs="${noconfigdirs} target-examples"
    noconfigdirs="${noconfigdirs} target-libiberty texinfo send-pr"
    noconfigdirs="${noconfigdirs} tcl tk itcl libgui sim"
    noconfigdirs="${noconfigdirs} expect dejagnu"
    # the C++ libraries do NOT build on top of CE's C libraries
    noconfigdirs="${noconfigdirs} target-libstdc++-v3"
    noconfigdirs="${noconfigdirs} target-newlib"
    case "${host}" in
      *-*-cygwin*) ;; # keep gdb and readline
      *) noconfigdirs="${noconfigdirs} gdb readline"
	 ;;
    esac
    ;;
  arc-*-*)
    noconfigdirs="${noconfigdirs} target-libgloss ${libgcj}"
    ;;
  arm-semi-aof )
    ;;
  arm-*-coff | strongarm-*-coff | xscale-*-coff)
    noconfigdirs="${noconfigdirs} ${libgcj}"
    ;;
  arm-*-elf* | strongarm-*-elf* | xscale-*-elf* | arm*-*-eabi* )
    noconfigdirs="${noconfigdirs} target-libffi target-qthreads"
    ;;
  arm*-*-symbianelf*)
    noconfigdirs="${noconfigdirs} ${libgcj} target-libiberty"
    ;;
  arm-*-pe*)
    noconfigdirs="${noconfigdirs} target-libgloss ${libgcj}"
    ;;
  thumb-*-coff)
    noconfigdirs="${noconfigdirs} target-libgloss ${libgcj}"
    ;;
  thumb-*-elf)
    noconfigdirs="${noconfigdirs} target-libgloss ${libgcj}"
    ;;
  thumb-*-pe)
    noconfigdirs="${noconfigdirs} target-libgloss ${libgcj}"
    ;;
  arm-*-riscix*)
    noconfigdirs="${noconfigdirs} ld target-libgloss ${libgcj}"
    ;;
  avr-*-*)
    noconfigdirs="${noconfigdirs} target-libiberty target-libstdc++-v3 ${libgcj}"
    ;;
  c4x-*-* | tic4x-*-*)
    noconfigdirs="${noconfigdirs} target-libstdc++-v3 target-libgloss ${libgcj}"
    ;;
  c54x*-*-* | tic54x-*-*)
    noconfigdirs="${noconfigdirs} target-libstdc++-v3 target-libgloss ${libgcj} gcc gdb newlib"
    ;;
  cris-*-* | crisv32-*-*)
    unsupported_languages="$unsupported_languages java"
    case "${target}" in
      *-*-aout)
	unsupported_languages="$unsupported_languages f95"
	noconfigdirs="${noconfigdirs} target-libffi target-boehm-gc";;
      *-*-elf)
	unsupported_languages="$unsupported_languages f95"
	noconfigdirs="${noconfigdirs} target-boehm-gc";;
      *-*-linux*)
	noconfigdirs="${noconfigdirs} target-newlib target-libgloss";;
      *)
	unsupported_languages="$unsupported_languages f95"
	noconfigdirs="${noconfigdirs} ${libgcj} target-newlib target-libgloss";;
    esac
    ;;
  crx-*-*)
    noconfigdirs="${noconfigdirs} target-libstdc++-v3 target-mudflap ${libgcj}"
    ;;
  d10v-*-*)
    noconfigdirs="${noconfigdirs} target-libstdc++-v3 target-libgloss ${libgcj}"
    ;;
  d30v-*-*)
    noconfigdirs="${noconfigdirs} ${libgcj} gdb"
    ;;
  fr30-*-elf*)
    noconfigdirs="${noconfigdirs} ${libgcj} gdb"
    ;;
  frv-*-*)
    noconfigdirs="${noconfigdirs} ${libgcj}"
    ;;
  h8300*-*-*)
    noconfigdirs="${noconfigdirs} target-libgloss ${libgcj}"
    ;;
  h8500-*-*)
    noconfigdirs="${noconfigdirs} target-libstdc++-v3 target-libgloss ${libgcj}"
    ;;
  hppa1.1-*-osf* | hppa1.1-*-bsd* )
    ;;
  hppa*64*-*-linux* | parisc*64*-*-linux*)
    # In this case, it is because the hppa64-linux target is for
    # the kernel only at this point and has no libc, and thus no
    # headers, crt*.o, etc., all of which are needed by these.
    noconfigdirs="${noconfigdirs} target-zlib"
    ;;
  parisc*-*-linux* | hppa*-*-linux*)
    ;;
  hppa*-*-*elf* | \
  hppa*-*-lites* | \
  hppa*-*-openbsd* | \
  hppa*64*-*-*)
    noconfigdirs="${noconfigdirs} ${libgcj}"
    ;;
  hppa*-*-*)
    # According to Alexandre Oliva <aoliva@redhat.com>, libjava will NOT
    # build on HP-UX 10.20.
    noconfigdirs="${noconfigdirs} ld shellutils ${libgcj}"
    ;;
  i960-*-*)
    noconfigdirs="${noconfigdirs} ${libgcj} gdb"
    ;;
  ia64*-*-elf*)
    # No gdb support yet.
    noconfigdirs="${noconfigdirs} readline mmalloc libgui itcl gdb"
    ;;
  ia64*-**-hpux*)
    # No gdb or ld support yet.
    noconfigdirs="${noconfigdirs} ${libgcj} readline mmalloc libgui itcl gdb ld"
    ;;
  i370-*-opened*)
    ;;
  i[[3456789]]86-*-coff | i[[3456789]]86-*-elf)
    noconfigdirs="${noconfigdirs} ${libgcj}"
    ;;
  i[[3456789]]86-*-linux*)
    # The GCC port for glibc1 has no MD_FALLBACK_FRAME_STATE_FOR, so let us
    # not build java stuff by default.
    case "${target}" in
      *-*-*libc1*)
	noconfigdirs="${noconfigdirs} ${libgcj}";;
    esac

    # This section makes it possible to build newlib natively on linux.
    # If we are using a cross compiler then do NOT configure newlib.
    if test x${is_cross_compiler} != xno ; then
      noconfigdirs="${noconfigdirs} target-newlib"
    fi
    noconfigdirs="${noconfigdirs} target-libgloss"
    # If we are not using a cross compiler, do configure newlib.
    # Note however, that newlib will only be configured in this situation
    # if the --with-newlib option has been given, because otherwise
    # 'target-newlib' will appear in skipdirs.
    ;;
  i[[3456789]]86-*-mingw32*)
    target_configdirs="${target_configdirs} target-mingw"
    noconfigdirs="${noconfigdirs} expect target-libgloss ${libgcj}"

    # Cannot build gdb for mingw32 if not native.
    case "${host}" in
      i[[3456789]]86-*-mingw32) ;; # keep gdb tcl tk expect etc.
      *) noconfigdirs="${noconfigdirs} gdb tcl tk expect itcl gnuserv"
         ;;
    esac
    ;;    
  *-*-cygwin*)
    target_configdirs="${target_configdirs} target-libtermcap target-winsup"
    noconfigdirs="${noconfigdirs} target-gperf target-libgloss ${libgcj}"
    # always build newlib if winsup directory is present.
    if test -d "${srcdir}/winsup"; then
      skipdirs=`echo " ${skipdirs} " | sed -e 's/ target-newlib / /'`
    elif test -d "${srcdir}/newlib"; then
      echo "Warning: winsup is missing so newlib cannot be built."
    fi

    # Cannot build gdb for Cygwin if not native.
    case "${host}" in
      *-*-cygwin*) ;; # keep gdb tcl tk expect etc.
      *) noconfigdirs="${noconfigdirs} gdb tcl tk expect itcl libgui gnuserv"
         ;;
    esac
    ;;    
  i[[3456789]]86-moss-msdos | i[[3456789]]86-*-moss* | \
  i[[3456789]]86-*-uwin* | i[[3456789]]86-*-interix* )
    ;;
  i[[3456789]]86-*-pe)
    noconfigdirs="${noconfigdirs} target-libstdc++-v3 target-libgloss ${libgcj}"
    ;;
  i[[3456789]]86-*-sco3.2v5*)
    # The linker does not yet know about weak symbols in COFF,
    # and is not configured to handle mixed ELF and COFF.
    noconfigdirs="${noconfigdirs} ld target-libgloss ${libgcj}"
    ;;
  i[[3456789]]86-*-sco*)
    noconfigdirs="${noconfigdirs} gprof target-libgloss ${libgcj}"
    ;;
  i[[3456789]]86-*-solaris2*)
    noconfigdirs="${noconfigdirs} target-libgloss"
    ;;
  i[[3456789]]86-*-sysv4*)
    noconfigdirs="${noconfigdirs} target-libgloss ${libgcj}"
    ;;
  i[[3456789]]86-*-beos*)
    noconfigdirs="${noconfigdirs} gdb target-newlib target-libgloss ${libgcj}"
    ;;
  m32r-*-*)
    noconfigdirs="${noconfigdirs} ${libgcj}"
    ;;
  m68hc11-*-*|m6811-*-*|m68hc12-*-*|m6812-*-*)
    noconfigdirs="${noconfigdirs} target-libiberty target-libstdc++-v3 ${libgcj}"
    ;;
  m68k-*-elf*)
    noconfigdirs="${noconfigdirs} ${libgcj}"
    ;;
  m68k-*-coff*)
    noconfigdirs="${noconfigdirs} ${libgcj}"
    ;;
  mcore-*-pe*)
  # The EPOC C++ environment does not support exceptions or rtti,
  # and so building libstdc++-v3 tends not to always work.
    noconfigdirs="${noconfigdirs} target-libstdc++-v3"
    ;;
  mmix-*-*)
    noconfigdirs="${noconfigdirs} target-libffi target-boehm-gc gdb libgloss"
    unsupported_languages="$unsupported_languages f95 java"
    ;;
  mn10200-*-*)
    noconfigdirs="${noconfigdirs} ${libgcj}"
    ;;
  mn10300-*-*)
    noconfigdirs="${noconfigdirs} ${libgcj}"
    ;;
  ms1-*-*)
    noconfigdirs="${noconfigdirs} sim"
    ;;
  powerpc-*-aix*)
    # copied from rs6000-*-* entry
    noconfigdirs="${noconfigdirs} gprof target-libgloss ${libgcj}"
    ;;
  powerpc*-*-winnt* | powerpc*-*-pe* | ppc*-*-pe)
    target_configdirs="${target_configdirs} target-winsup"
    noconfigdirs="${noconfigdirs} gdb tcl tk make expect target-libgloss itcl gnuserv ${libgcj}"
    # always build newlib.
    skipdirs=`echo " ${skipdirs} " | sed -e 's/ target-newlib / /'`
    ;;
    # This is temporary until we can link against shared libraries
  powerpcle-*-solaris*)
    noconfigdirs="${noconfigdirs} gdb sim make tcl tk expect itcl gnuserv ${libgcj}"
    ;;
  powerpc-*-beos*)
    noconfigdirs="${noconfigdirs} gdb target-newlib target-libgloss ${libgcj}"
    ;;
  powerpc-*-eabi)
    noconfigdirs="${noconfigdirs} ${libgcj}"
    ;;
  powerpc-*-eabi* | powerpcle-*-eabi* | powerpc-*-rtems* )
    ;;
  rs6000-*-lynxos*)
    noconfigdirs="${noconfigdirs} target-newlib gprof ${libgcj}"
    ;;
  rs6000-*-aix*)
    noconfigdirs="${noconfigdirs} gprof target-libgloss ${libgcj}"
    ;;
  rs6000-*-*)
    noconfigdirs="${noconfigdirs} gprof ${libgcj}"
    ;;
  m68k-apollo-*)
    noconfigdirs="${noconfigdirs} ld binutils gprof target-libgloss ${libgcj}"
    ;;
  mips*-*-irix5*)
    noconfigdirs="${noconfigdirs} gprof target-libgloss ${libgcj}"
    ;;
  mips*-*-irix6*)
    # Linking libjava exceeds command-line length limits on at least
    # IRIX 6.2, but not on IRIX 6.5.
    # Also, boehm-gc will NOT build on IRIX 6.5, according to Jeffrey Oldham
    # <oldham@codesourcery.com>
    noconfigdirs="${noconfigdirs} gprof target-libgloss ${libgcj}"
    ;;
  mips*-dec-bsd*)
    noconfigdirs="${noconfigdirs} gprof target-libgloss ${libgcj}"
    ;;
  mips*-*-bsd*)
    noconfigdirs="${noconfigdirs} gprof target-libgloss ${libgcj}"
    ;;
  mipstx39-*-*)
    noconfigdirs="${noconfigdirs} gprof ${libgcj}" # same as generic mips
    ;;
  mips64*-*-linux*)
    noconfigdirs="${noconfigdirs} target-newlib ${libgcj}"
    ;;
  mips*-*-linux*)
    noconfigdirs="${noconfigdirs} target-newlib target-libgloss"
    ;;
  mips*-*-*)
    noconfigdirs="${noconfigdirs} gprof ${libgcj}"
    ;;
  romp-*-*)
    noconfigdirs="${noconfigdirs} bfd binutils ld gas opcodes target-libgloss ${libgcj}"
    ;;
  sh-*-* | sh64-*-*)
    case "${host}" in
      i[[3456789]]86-*-vsta) ;; # do NOT add gprof back in
      i[[3456789]]86-*-go32*) ;; # do NOT add gprof back in
      i[[3456789]]86-*-msdosdjgpp*) ;; # do NOT add gprof back in
      *) skipdirs=`echo " ${skipdirs} " | sed -e 's/ gprof / /'` ;;
    esac
    noconfigdirs="${noconfigdirs} target-libgloss ${libgcj}"
    ;;
  sparc-*-elf*)
    noconfigdirs="${noconfigdirs} ${libgcj}"
    ;;
  sparc64-*-elf*)
    noconfigdirs="${noconfigdirs} ${libgcj}"
    ;;
  sparclite-*-*)
    noconfigdirs="${noconfigdirs} ${libgcj}"
    ;;
  sparc-*-sunos4*)
    noconfigdirs="${noconfigdirs} ${libgcj}"
    if test x${is_cross_compiler} != xno ; then
           noconfigdirs="${noconfigdirs} gdb target-newlib target-libgloss"
    else
           use_gnu_ld=no
    fi
    ;;
  sparc-*-solaris2.[[0-6]] | sparc-*-solaris2.[[0-6]].*)
    noconfigdirs="${noconfigdirs} ${libgcj}"
    ;;
  sparc-*-solaris* | sparc64-*-solaris* | sparcv9-*-solaris*)
    ;;
  v810-*-*)
    noconfigdirs="${noconfigdirs} bfd binutils gas gcc gdb ld target-libstdc++-v3 opcodes target-libgloss ${libgcj}"
    ;;
  v850-*-*)
    noconfigdirs="${noconfigdirs} target-libgloss ${libgcj}"
    ;;
  v850e-*-*)
    noconfigdirs="${noconfigdirs} target-libgloss ${libgcj}"
    ;;
  v850ea-*-*)
    noconfigdirs="${noconfigdirs} target-libgloss ${libgcj}"
    ;;
  vax-*-vms)
    noconfigdirs="${noconfigdirs} bfd binutils gdb ld target-newlib opcodes target-libgloss ${libgcj}"
    ;;
  vax-*-*)
    noconfigdirs="${noconfigdirs} target-newlib target-libgloss ${libgcj}"
    ;;
  xtensa-*-*)
    noconfigdirs="${noconfigdirs} ${libgcj}"
    ;;
  ip2k-*-*)
    noconfigdirs="${noconfigdirs} target-libiberty target-libstdc++-v3 ${libgcj}"
    ;;
  *-*-linux* | *-*-gnu* | *-*-k*bsd*-gnu)
    noconfigdirs="${noconfigdirs} target-newlib target-libgloss"
    ;;
  *-*-lynxos*)
    noconfigdirs="${noconfigdirs} target-newlib target-libgloss ${libgcj}"
    ;; 
  *-*-*)
    noconfigdirs="${noconfigdirs} ${libgcj}"
    ;;
esac

if test "${build}" != "${host}" ; then
    noconfigdirs="${noconfigdirs} expect"
fi

# If we are NOT building newlib, then do NOT build libgloss, since libgloss
# depends upon some newlib header files.
case "${noconfigdirs}" in
  *target-libgloss*) ;;
  *target-newlib*) noconfigdirs="${noconfigdirs} target-libgloss" ;;
esac

# Work in distributions that contain no compiler tools, like Autoconf.
tentative_cc=""
host_makefile_frag=/dev/null
if test -d ${srcdir}/config ; then
case "${host}" in
  m68k-hp-hpux*)
    # Avoid "too much defining" errors from HPUX compiler.
    tentative_cc="cc -Wp,-H256000"
    # If "ar" in $PATH is GNU ar, the symbol table may need rebuilding.
    # If it is HP/UX ar, this should be harmless.
    RANLIB="ar ts"
    ;;
  m68k-apollo-sysv*)
    tentative_cc="cc -A ansi -A runtype,any -A systype,any -U__STDC__ -DUSG"
    ;;
  m68k-apollo-bsd*)
    # None of the Apollo compilers can compile gas or binutils. The
    # preprocessor chokes on bfd, the compiler will not let you assign
    # integers to enums, and other problems. Defining CC to gcc is a
    # questionable way to say "do NOT use the apollo compiler"
    # (the preferred version of GCC could be called cc, or whatever), but
    # I am not sure leaving CC as cc is any better...
    CC_UNCOMMENTED=cc -A ansi -A runtype,any -A systype,any -U__STDC__ -DNO_STDARG
    # Used to have BISON=yacc.
    tentative_cc=gcc
    ;;
  m88k-dg-dgux*)
    tentative_cc="gcc -Wall -ansi -D__using_DGUX"
    ;;
  m88k-harris-cxux*)
    # Under CX/UX, we want to tell the compiler to use ANSI mode.
    tentative_cc="cc -Xa"
    host_makefile_frag="config/mh-cxux"
    ;;
  m88k-motorola-sysv*)
    ;;
  mips*-dec-ultrix*)
    tentative_cc="cc -Wf,-XNg1000"
    host_makefile_frag="config/mh-decstation"
    ;;
  mips*-nec-sysv4*)
    # The C compiler on NEC MIPS SVR4 needs bigger tables.
    tentative_cc="cc -ZXNd=5000 -ZXNg=1000"
    host_makefile_frag="config/mh-necv4"
    ;;
  mips*-sgi-irix4*)
    # Tell compiler to use K&R C. We cannot compile under the SGI Ansi
    # environment. Also bump switch table size so that cp-parse will
    # compile. Bump string length limit so linker builds.
    tentative_cc="cc -cckr -Wf,-XNg1500 -Wf,-XNk1000 -Wf,-XNh2000 -Wf,-XNl8192"
    ;;
  mips*-*-sysv4*)
    host_makefile_frag="config/mh-sysv4"
    ;;
  mips*-*-sysv*)
    # This is for a MIPS running RISC/os 4.52C.

    # This is needed for GDB, but needs to be in the top-level make because
    # if a library is compiled with BSD headers and gets linked with the
    # sysv system libs all hell can break loose (e.g. a jmp_buf might be
    # a different size).
    # ptrace(2) apparently has problems in the BSD environment. No
    # workaround is known except to select the sysv environment. Could we
    # use /proc instead? These "sysv environments" and "bsd environments"
    # often end up being a pain.
    #
    # This is not part of CFLAGS because perhaps not all C compilers have
    # this option.
    tentative_cc="cc -systype sysv"
    ;;
  i370-ibm-opened*)
    tentative_cc="c89"
    ;;
  i[[3456789]]86-*-sysv5*)
    host_makefile_frag="config/mh-sysv5"
    ;;
  i[[3456789]]86-*-dgux*)
    tentative_cc="gcc -Wall -ansi -D__using_DGUX"
    host_makefile_frag="config/mh-dgux386"
    ;;
  i[[3456789]]86-ncr-sysv4.3*)
    # The MetaWare compiler will generate a copyright message unless you
    # turn it off by adding the -Hnocopyr flag.
    tentative_cc="cc -Hnocopyr"
    ;;
  i[[3456789]]86-ncr-sysv4*)
    # for an NCR 3000 (i486/SVR4) system.
    # The NCR 3000 ships with a MetaWare compiler installed as /bin/cc.
    # This compiler not only emits obnoxious copyright messages every time
    # you run it, but it chokes and dies on a whole bunch of GNU source
    # files. Default to use the AT&T compiler installed in /usr/ccs/ATT/cc.
    tentative_cc="/usr/ccs/ATT/cc"
    host_makefile_frag="config/mh-ncr3000"
    ;;
  i[[3456789]]86-*-sco3.2v5*)
    ;;
  i[[3456789]]86-*-sco*)
    # The native C compiler botches some simple uses of const.
    # Unfortunately, it does NOT define anything like "__sco__" for us to
    # test for in ansidecl.h.
    AC_C_CONST
    tentative_cc="cc -Dconst="
    host_makefile_frag="config/mh-sco"
    ;;
  i[[3456789]]86-*-udk*)
    host_makefile_frag="config/mh-sysv5"
    ;;
  i[[3456789]]86-*-solaris2*)
    host_makefile_frag="config/mh-sysv4"
    ;;
  i[[3456789]]86-*-msdosdjgpp*)
    host_makefile_frag="config/mh-djgpp"
    ;;
  *-cygwin*)
    ACX_CHECK_CYGWIN_CAT_WORKS
    host_makefile_frag="config/mh-cygwin"
    ;;
  *-mingw32*)
    ;;
  *-interix*)
    host_makefile_frag="config/mh-interix"
    ;;
  vax-*-ultrix2*)
    # The old BSD pcc is NOT up to compiling parts of gdb so use gcc
    tentative_cc=gcc
    ;;
  *-*-solaris2*)
    host_makefile_frag="config/mh-solaris"
    ;;
  m68k-sun-sunos*)
    # Sun's C compiler needs the -J flag to be able to compile cp-parse.c
    # without overflowing the jump tables (-J says to use a 32 bit table)
    tentative_cc="cc -J"
    ;;
  *-hp-hpux*)
    tentative_cc="cc -Wp,-H256000"
    ;;
  *-*-hiux*)
    tentative_cc="cc -Wp,-H256000"
    ;;
  rs6000-*-lynxos*)
    # /bin/cc is less than useful for our purposes. Always use GCC
    tentative_cc="/usr/cygnus/progressive/bin/gcc"
    host_makefile_frag="config/mh-lynxrs6k"
    ;;
  powerpc-*-darwin*)
    host_makefile_frag="config/mh-ppc-darwin"
    ;;
  *-*-darwin*)
    host_makefile_frag="config/mh-darwin"
    ;;
  # Not actually a real system, but it does have a makefile_frag:
  *-*-darwinpic*)
    host_makefile_frag="config/mh-darwinpic"
    ;;
  *-*-lynxos*)
    # /bin/cc is less than useful for our purposes. Always use GCC
    tentative_cc="/bin/gcc"
    ;;
  *-*-sysv4*)
    host_makefile_frag="config/mh-sysv4"
    ;;
  # This is placed last to prevent interfering with the cases above.
  i[[3456789]]86-*-*)
    # Build the stage2 and stage3 compilers with -fomit-frame-pointer.
    host_makefile_frag="config/mh-x86omitfp"
    ;;
esac
fi

# If we are NOT going to be using gcc, see if we can extract a definition
# of CC from the fragment.
# Actually, use the 'pre-extracted' version above.
if test -z "${CC}" && test "${build}" = "${host}" ; then
  IFS="${IFS= 	}"; save_ifs="${IFS}"; IFS="${IFS}:"
  found=
  for dir in ${PATH}; do
    test -z "${dir}" && dir=.
    if test -f ${dir}/gcc; then
      found=yes
      break
    fi
  done
  IFS="${save_ifs}"
  if test -z "${found}" && test -n "${tentative_cc}" ; then
    CC=${tentative_cc}
  fi
fi

if test "${build}" != "${host}" ; then
  # If we are doing a Canadian Cross, in which the host and build systems
  # are not the same, we set reasonable default values for the tools.

  CC=${CC-${host_alias}-gcc}
  CFLAGS=${CFLAGS-"-g -O2"}
  CXX=${CXX-${host_alias}-c++}
  CXXFLAGS=${CXXFLAGS-"-g -O2"}
  CC_FOR_BUILD=${CC_FOR_BUILD-gcc}
  BUILD_PREFIX=${build_alias}-
  BUILD_PREFIX_1=${build_alias}-

else
  # Set reasonable default values for some tools even if not Canadian.
  # Of course, these are different reasonable default values, originally
  # specified directly in the Makefile.
  # We do NOT export, so that autoconf can do its job.
  # Note that all these settings are above the fragment inclusion point
  # in Makefile.in, so can still be overridden by fragments.
  # This is all going to change when we autoconfiscate...

  CC_FOR_BUILD="\$(CC)"
  BUILD_PREFIX=
  BUILD_PREFIX_1=loser-

  if test "x${CC}" = "x"; then
    test -z "${CC}"
    AC_PROG_CC
  else
    test ! -z "${CC}"
    AC_PROG_GCC_TRADITIONAL
  fi
  AC_PROG_CPP
  AC_PROG_CXX
  AC_PROG_OBJC
  AC_PROG_OBJCPP

  # We must set the default linker to the linker used by gcc for the
  # correct operation of libtool. If LD is not defined and we are using
  # gcc, try to set the LD default to the ld used by gcc.
  if test -z "${LD}"; then
    if test "${GCC}" = yes; then
      case ${build} in
        *-*-mingw*)
	      gcc_prog_ld=`${CC} -print-prog-name=ld 2>&1 | tr -d '\015'`
	      ;;
        *)
	      gcc_prog_ld=`${CC} -print-prog-name=ld 2>&1`
	      ;;
      esac
      case ${gcc_prog_ld} in
        # Accept absolute paths.
        [[\\/]* | [A-Za-z]:[\\/]*)]
          LD="${gcc_prog_ld}"
          ;;
      esac
    fi # GCC
  fi # LD

  CXX=${CXX-"c++"}
  CFLAGS=${CFLAGS-"-g"}
  CXXFLAGS=${CXXFLAGS-"-g -O2"}
fi

ACX_PROG_GNAT
ACX_PROG_CMP_IGNORE_INITIAL

# Used for setting ${lt_cv_objdir}
if test "x${lt_cv_objdir}" = "x"; then
  test -z "${lt_cv_objdir}"
  _LT_CHECK_OBJDIR
fi
dnl# lt_cv_objdir is used by some subdirs

# Check for GMP, MPFR and MPC
gmplibs="-lmpc -lmpfr -lgmp"
gmpinc=
have_gmp=yes

# Specify a location for mpc
# check for this first so it ends up on the link line before mpfr.
AC_ARG_WITH([mpc],
[AS_HELP_STRING([--with-mpc=PATH],
		[specify prefix directory for installed MPC package.
		 Equivalent to --with-mpc-include=PATH/include
		 plus --with-mpc-lib=PATH/lib])])
AC_ARG_WITH([mpc-include],
[AS_HELP_STRING([--with-mpc-include=PATH],
		[specify directory for installed MPC include files])])
AC_ARG_WITH([mpc-lib],
[AS_HELP_STRING([--with-mpc-lib=PATH],
		[specify directory for the installed MPC library])])

if test "x${with_mpc}" != x; then
  gmplibs="-L${with_mpc}/lib ${gmplibs}"
  gmpinc="-I${with_mpc}/include ${gmpinc}"
fi
if test "x${with_mpc_include}" != x; then
  gmpinc="-I${with_mpc_include} ${gmpinc}"
fi
if test "x${with_mpc_lib}" != x; then
  gmplibs="-L${with_mpc_lib} ${gmplibs}"
fi
if test "x${with_mpc}${with_mpc_include}${with_mpc_lib}" = x && test -d ${srcdir}/mpc; then
  gmplibs='-L$$r/$(HOST_SUBDIR)/mpc/src/'"${lt_cv_objdir} ${gmplibs}"
  gmpinc='-I$$s/mpc/src '"${gmpinc}"
  # Do not test the mpc version. Assume that it is sufficient, since
  # it is in the source tree, and the library has not been built yet
  # but it would be included on the link line in the version check below
  # hence making the test fail.
  have_gmp=yes
fi

# Specify a location for mpfr
# check for this first so it ends up on the link line before gmp.
AC_ARG_WITH([mpfr-dir],[AS_HELP_STRING([--with-mpfr-dir=PATH],[Specify source directory for MPFR library])])

if test "x${with_mpfr_dir}" != x; then
  gmpinc="-I${with_mpfr_dir}"
  gmplibs="${with_mpfr_dir}/libmpfr.a"
else
  gmplibs="-lmpfr"
fi

AC_ARG_WITH([mpfr],[AS_HELP_STRING([--with-mpfr=PATH],[Specify directory for installed MPFR library])])

if test "x${with_mpfr}" != x; then
  gmplibs="-L${with_mpfr}/lib ${gmplibs}"
  gmpinc="-I${with_mpfr}/include"
fi

# Specify a location for gmp
AC_ARG_WITH([gmp-dir],[AS_HELP_STRING([--with-gmp-dir=PATH],[Specify source directory for GMP library])])

if test "x${with_gmp_dir}" != x; then
  gmpinc="${gmpinc} -I${with_gmp_dir}"
  if test -f "${with_gmp_dir}/.libs/libgmp.a"; then
    gmplibs="${gmplibs} ${with_gmp_dir}/.libs/libgmp.a"
  elif test -f "${with_gmp_dir}/_libs/libgmp.a"; then
    gmplibs="${gmplibs} ${with_gmp_dir}/_libs/libgmp.a"
  fi
  # One of the later tests will catch the error if neither library is present.
else
  gmplibs="${gmplibs} -lgmp"
fi

AC_ARG_WITH([gmp],[AS_HELP_STRING([--with-gmp=PATH],[Specify directory for installed GMP library])])

if test "x${with_gmp}" != x; then
  gmplibs="-L${with_gmp}/lib ${gmplibs}"
  gmpinc="-I${with_gmp}/include ${gmpinc}"
fi

saved_CFLAGS="${CFLAGS}"
CFLAGS="${CFLAGS} ${gmpinc}"
# Check GMP actually works
AC_CHECK_HEADERS_ONCE([gmp.h])
AC_MSG_CHECKING([for correct version of gmp.h])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include "gmp.h"]],[[
#if __GNU_MP_VERSION < 3
choke me
#endif /* __GNU_MP_VERSION < 3 */
]])],[AC_MSG_RESULT([yes])],[AC_MSG_RESULT([no]); have_gmp=no])

if test x"${have_gmp}" = xyes; then
  AC_CHECK_HEADERS_ONCE([mpfr.h])
  AC_MSG_CHECKING([for MPFR])

  saved_LIBS="${LIBS}"
  LIBS="${LIBS} ${gmplibs}"
  AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <gmp.h>
#include <mpfr.h>]],[[mpfr_t n; mpfr_init(n);]])],[AC_MSG_RESULT([yes])],[AC_MSG_RESULT([no]); have_gmp=no])
  LIBS="${saved_LIBS}"
  CFLAGS="${saved_CFLAGS}"
fi

# Flags needed for both GMP, MPFR and/or MPC.
AC_SUBST([gmplibs])
AC_SUBST([gmpinc])

# Checks for header files.
AC_HEADER_STDBOOL dnl# calls AC_CHECK_HEADER_STDBOOL
unset ac_cv_header_netdb_h
unset ac_cv_header_sys_time_h
AC_CHECK_HEADERS([fcntl.h langinfo.h libintl.h limits.h locale.h \
                  mach/mach.h malloc.h malloc/malloc.h netdb.h \
                  netinet/in.h stddef.h stdio.h sys/file.h sys/ioctl.h \
                  sys/param.h sys/socket.h sys/time.h wchar.h wctype.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_BIGENDIAN
AC_C_INLINE
AC_C_PROTOTYPES
AC_C_RESTRICT
AC_TYPE_INT8_T
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_MODE_T
AC_TYPE_OFF_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UID_T
AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_CHECK_TYPES([ptrdiff_t])
AC_CHECK_MEMBERS([struct stat.st_blksize])
AC_STRUCT_ST_BLOCKS
AC_CHECK_MEMBERS([struct stat.st_rdev])

# Checks for library functions.
AC_FUNC_ALLOCA
AC_FUNC_ERROR_AT_LINE
AC_FUNC_MALLOC
AC_FUNC_MKTIME
AC_FUNC_OBSTACK
AC_FUNC_REALLOC
AC_FUNC_STRCOLL
AC_FUNC_STRTOD
unset ac_cv_func_gethostbyname
AC_CHECK_FUNCS([atexit floor getcwd gethostbyaddr gethostbyname \
                gettimeofday isascii localeconv mblen memchr memmove \
                memset modf pow printf putenv re_comp regcomp rint \
                select setlocale socket sqrt strcasecmp strchr strcspn \
                strerror strpbrk strrchr strspn strstr strtol strtoul \
                strverscmp system utime])
# Checks for tcl-related functions
AC_DEFUN([SC_TCL_NEW_MACROS],[
  AC_REQUIRE([SC_PROG_TCLSH])
  dnl# SC\_BUILD\_TCLSH never seems to return the right results...
  AC_REQUIRE([SC_TCL_GETHOSTBYADDR_R])
  AC_REQUIRE([SC_TCL_GETHOSTBYNAME_R])
  AC_REQUIRE([SC_TCL_GETPWUID_R])
  AC_REQUIRE([SC_TCL_GETPWNAM_R])
  AC_REQUIRE([SC_TCL_GETGRGID_R])
  AC_REQUIRE([SC_TCL_GETGRNAM_R])
])
SC_TCL_NEW_MACROS

# Allow host libstdc++ to be specified for static linking with PPL.
AC_ARG_WITH([host-libstdcxx],
[AS_HELP_STRING([--with-host-libstdcxx=L],
		[use linker arguments L to link with libstdc++
		 when linking with PPL])])

case ${with_host_libstdcxx} in
  no|yes)
    AC_MSG_ERROR([the --with-host-libstdcxx flag needs an argument to it])
    ;;
esac

# Libraries to use for stage1 or when not bootstrapping.
AC_ARG_WITH([stage1-libs],
[AS_HELP_STRING([--with-stage1-libs=LIBS],[libraries for stage1])],
[if test "${withval}" = "no" -o "${withval}" = "yes"; then
   stage1_libs=""
 else
   stage1_libs=${withval}
 fi],
[stage1_libs=${with_host_libstdcxx}])
AC_SUBST([stage1_libs])

# Linker flags to use for stage1 or when not bootstrapping.
AC_ARG_WITH([stage1-ldflags],
[AS_HELP_STRING([--with-stage1-ldflags=FLAGS],[linker flags for stage1])],
[if test "${withval}" = "no" -o "${withval}" = "yes"; then
   stage1_ldflags=""
 else
   stage1_ldflags=${withval}
 fi],
[stage1_ldflags=""
 # In stage 1, default to linking libstdc++ and libgcc statically with GCC
 # if supported. But if the user explicitly specified the libraries to use,
 # trust that they are doing what they want.
 if test "${stage1_libs}" = "" -a "${have_static_libs}" = yes; then
   stage1_ldflags="-static-libstdc++ -static-libgcc"
 fi])
AC_SUBST([stage1_ldflags])

# Libraries to use for stage2 and later builds.  This defaults to the
# argument passed to --with-host-libstdcxx.
AC_ARG_WITH([boot-libs],
[AS_HELP_STRING([--with-boot-libs=LIBS],[libraries for stage2 and later])],
[if test "${withval}" = "no" -o "${withval}" = "yes"; then
   poststage1_libs=""
 else
   poststage1_libs=${withval}
 fi],
[poststage1_libs=${with_host_libstdcxx}])
AC_SUBST([poststage1_libs])

# Linker flags to use for stage2 and later builds.
AC_ARG_WITH([boot-ldflags],
[AS_HELP_STRING([--with-boot-ldflags=FLAGS],
		[linker flags for stage2 and later])],
[if test "${withval}" = "no" -o "${withval}" = "yes"; then
   poststage1_ldflags=""
 else
   poststage1_ldflags=${withval}
 fi],
[poststage1_ldflags=""
 # In stages 2 and 3, default to linking libstdc++ and libgcc
 # statically. But if the user explicitly specified the libraries to
 # use, trust that they are doing what they want.
 if test "${poststage1_libs}" = ""; then
   poststage1_ldflags="-static-libstdc++ -static-libgcc"
 fi])
AC_SUBST([poststage1_ldflags])

# Check for LTO support.
AC_ARG_ENABLE([lto],
[AS_HELP_STRING([--enable-lto],[enable (building) link time optimization support])],
[enable_lto=${enableval}],
[enable_lto=yes; default_enable_lto=yes])

ACX_ELF_TARGET_IFELSE([# ELF platforms build the lto-plugin always.
  build_lto_plugin=yes
],[if test x"${default_enable_lto}" = x"yes" ; then
    case ${target} in
      *-apple-darwin9* | *-cygwin* | *-mingw*) ;;
      # On other non-ELF platforms, LTO has yet to be validated.
      *) enable_lto=no ;;
    esac
  else
  # Apart from ELF platforms, only Windows and Darwin support LTO so far.
  # It would also be nice to check the binutils support, but we do NOT
  # have gcc_GAS_CHECK_FEATURE available here. For now, we will just
  # warn during gcc/ subconfigure; unless you are bootstrapping with
  # -flto it will NOT be needed until after installation anyway.
    case ${target} in
      *-cygwin* | *-mingw* | *-apple-darwin*) ;;
      *) if test x"${enable_lto}" = x"yes"; then
	AC_MSG_ERROR([LTO support is not enabled for this target.])
        fi
      ;;
    esac
  fi
  # Among non-ELF, only Windows platforms support the lto-plugin so far.
  # Build it unless LTO was explicitly disabled.
  case ${target} in
    *-cygwin* | *-mingw*) build_lto_plugin=${enable_lto} ;;
    *) ;;
  esac
])


# By default, C and C++ are the only stage 1 languages.
stage1_languages=c
AC_SUBST([stage1_languages])

# Target libraries that we bootstrap.
bootstrap_target_libs=target-libgcc
AC_SUBST([bootstrap_target_libs])

# Figure out what language subdirectories are present.
# Look if the user specified --enable-languages="..."; if not, use
# the environment variable ${LANGUAGES} if defined. ${LANGUAGES} might
# go away some day.
# NB: embedded tabs in this IF block -- do not untabify
if test -d ${srcdir}/gcc; then
  if test x"${enable_languages+set}" != xset; then
    if test x"${LANGUAGES+set}" = xset; then
      enable_languages="${LANGUAGES}"
        echo "configure.ac: warning: setting LANGUAGES is deprecated, use --enable-languages instead" 1>&2
    else
      enable_languages=all
    fi
  else
    if test x"${enable_languages}" = x ||
       test x"${enable_languages}" = xyes;
       then
      echo "configure.ac: --enable-languages needs at least one language argument" 1>&2
      exit 1
    fi
  fi
  enable_languages=`echo "${enable_languages}" | sed -e 's/[[ 	,]][[ 	,]]*/,/g' -e 's/,$//'`

  # 'f95' is the old name for the 'fortran' language. We issue a warning
  # and make the substitution.
  case ,${enable_languages}, in
    *,f95,*)
      echo configure.ac: warning: 'f95' as language name is deprecated, use 'fortran' instead 1>&2
      enable_languages=`echo "${enable_languages}" | sed -e 's/f95/fortran/g'`
      ;;
  esac

  # If bootstrapping, C++ must be enabled.
  case ",${enable_languages},:${enable_bootstrap}" in
    *,c++,*:*) ;;
    *:yes)
      if test -f ${srcdir}/gcc/cp/config-lang.in; then
        enable_languages="${enable_languages},c++"
      else
        AC_MSG_ERROR([bootstrapping requires c++ sources])
      fi
      ;;
  esac

  # First scan to see if an enabled language requires some other language.
  # We assume that a given config-lang.in will list all the language
  # front ends it requires, even if some are required indirectly.
  for lang_frag in ${srcdir}/gcc/*/config-lang.in .. ; do
    case ${lang_frag} in
      ..) ;;
      # The odd quoting in the next line works around
      # an apparent bug in bash 1.12 on linux.
      ${srcdir}/gcc/[[*]]/config-lang.in) ;;
      *)
        # From the config-lang.in, get ${language}, ${lang_requires}, &
        # ${lang_requires_boot_languages}.
        language=""
        lang_requires=""
        lang_requires_boot_languages=""
        AC_MSG_NOTICE([sourcing ${lang_frag}])
        . ${lang_frag}
        for other in ${lang_requires} ${lang_requires_boot_languages}; do
          case ,${enable_languages}, in
	    *,${other},*) ;;
	    *,all,*) ;;
	    *,${language},*)
	      echo " \`${other}' language required by \`${language}'; enabling" 1>&2
	      enable_languages="${enable_languages},${other}"
	      ;;
	  esac
        done
	for other in ${lang_requires_boot_languages} ; do
	  if test "${other}" != "c"; then
	    case ,${enable_stage1_languages}, in
	      *,${other},*) ;;
	      *,all,*) ;;
	      *)
		case ,${enable_languages}, in
		  *,${language},*)
		    echo " '${other}' language required by '${language}' in stage 1; enabling" 1>&2
		    enable_stage1_languages="${enable_stage1_languages},${other}"
		    ;;
		esac
		;;
	    esac
          fi
        done
        ;;
    esac
  done

  new_enable_languages=c
  missing_languages=`echo ",${enable_languages}," | sed -e s/,all,/,/ -e s/,c,/,/ `
  potential_languages=c

  # If LTO is enabled, add the LTO front end.
  extra_host_libiberty_configure_flags=""
  if test "${enable_lto}" = "yes" ; then
    case ,${enable_languages}, in
      *,lto,*) ;;
      *) enable_languages="${enable_languages},lto" ;;
    esac
    if test "${build_lto_plugin}" = "yes" ; then
      configdirs="${configdirs} lto-plugin"
      extra_host_libiberty_configure_flags=--enable-shared
    fi
  fi
  AC_SUBST([extra_host_libiberty_configure_flags])

  missing_languages=`echo ",${enable_languages}," | sed -e s/,all,/,/ -e s/,c,/,/ `
  potential_languages=c

  enabled_target_libs=""
  disabled_target_libs=""

  for lang_frag in ${srcdir}/gcc/*/config-lang.in .. ; do
    case ${lang_frag} in
      ..) ;;
      # The odd quoting in the next line works around
      # an apparent bug in bash 1.12 on linux.
      ${srcdir}/gcc/[[*]]/config-lang.in) ;;
      *)
        # From the config-lang.in, get ${language}, ${target_libs}, 
        # ${lang_dirs}, ${boot_language}, and ${build_by_default}
        language=
        target_libs=
        lang_dirs=
        boot_language=
        build_by_default=
        need_gmp=
        AC_MSG_NOTICE([sourcing ${lang_frag}])
        . ${lang_frag}
        potential_languages="${potential_languages},${language}"
        # This is sensitive to the ordering of the case statement arms.
        case ,${enable_languages},:${language}:${have_gnat}:${build_by_default} in
          *::*:*)
            echo "${lang_frag} does not set \$language." 1>&2
            exit 1
            ;;
          *:ada:no:*)
            # Ada was requested with no preexisting GNAT. Disable
            # unconditionally.
            add_this_lang=no
            ;;
          *,${language},*:*:*:*)
            # Language was explicitly selected; include it.
            add_this_lang=yes
            ;;
          *,all,*:*:*:no)
            # 'all' was selected, but this is not a default language
            # so do not include it.
            add_this_lang=no
            ;;
          *,all,*:*:*:*)
            # 'all' was selected & this is a default language; include it.
            add_this_lang=yes
            ;;
          *)
            add_this_lang=no
            ;;
        esac

        # Disable languages that need GMP if it is NOT available.
        case ,${enable_languages},:${have_gmp}:${need_gmp} in
          *,${language},*:no:yes)
            # Specifically requested language; tell them.
            AC_MSG_ERROR([GMP with MPFR support is required to build $language])
            ;;
          *:no:yes)
            # Silently disable.
            add_this_lang=no
            ;;
        esac

	# Disable a language that is unsupported by the target.
	case " ${unsupported_languages} " in
	  *" ${language} "*)
	    add_this_lang=no
	    ;;
	esac

	case ${add_this_lang} in
	  no)
            # Remove language-dependent dirs.
            eval noconfigdirs='"${noconfigdirs} "'\"${target_libs} ${lang_dirs}\"
	    ;;
          *)
	    new_enable_languages="${new_enable_languages},${language}"
	    missing_languages=`echo "${missing_languages}" | sed "s/,${language},/,/"`
	    case ${boot_language} in
	      yes)
		# Add to (comma-separated) list of stage 1 languages.
		stage1_languages="${stage1_languages},${language}"
		;;
	    esac
	    ;;
        esac
        ;;
    esac
  done

  missing_languages=`echo "${missing_languages}" | sed -e "s/^,//" -e "s/,$//"`
  if test "x${missing_languages}" != x; then
    AC_MSG_ERROR([
The following requested languages could not be built: ${missing_languages}
Recognised languages are: ${potential_languages}])
  fi

  if test "x${new_enable_languages}" != "x${enable_languages}"; then
    echo "The following languages will be built: ${new_enable_languages}"
  fi
  enable_languages="${new_enable_languages}"
  ac_configure_args=`echo " ${ac_configure_args}" | sed -e 's/ --enable-languages=[[^ ]]*//' -e 's/$/ --enable-languages='"${enable_languages}"/ `
fi

# Remove the entries in ${skipdirs} and ${noconfigdirs} from ${configdirs},
# ${build_configdirs} and ${target_configdirs}.
# If we have source for ${noconfigdirs} entries, add them to ${notsupp}.

notsupp=""
for dir in . ${skipdirs} ${noconfigdirs} ; do
  my_dirname=`echo ${dir} | sed -e s/target-//g -e s/build-//g`
  if test ${dir} != .  && echo " ${configdirs} " | grep " ${dir} " >/dev/null 2>&1; then
    configdirs=`echo " ${configdirs} " | sed -e "s/ ${dir} / /"`
    if test -r ${srcdir}/${my_dirname}/configure ; then
      if echo " ${skipdirs} " | grep " ${dir} " >/dev/null 2>&1; then
	true
      else
	notsupp="${notsupp} ${dir}"
      fi
    fi
  fi
  if test $dir != .  && echo " ${build_configdirs} " | grep " ${dir} " >/dev/null 2>&1; then
    build_configdirs=`echo " ${build_configdirs} " | sed -e "s/ ${dir} / /"`
    if test -r ${srcdir}/${my_dirname}/configure ; then
      if echo " ${skipdirs} " | grep " ${dir} " >/dev/null 2>&1; then
	true
      else
	notsupp="${notsupp} ${dir}"
      fi
    fi
  fi
  if test $dir != . && echo " ${target_configdirs} " | grep " ${dir} " >/dev/null 2>&1; then
    target_configdirs=`echo " ${target_configdirs} " | sed -e "s/ ${dir} / /"`
    if test -r ${srcdir}/${my_dirname}/configure ; then
      if echo " ${skipdirs} " | grep " ${dir} " >/dev/null 2>&1; then
	true
      else
	notsupp="${notsupp} ${dir}"
      fi
    fi
  fi
done

# Sometimes the tools are distributed with libiberty but with no other
# libraries. In that case, we do NOT want to build target-libiberty.
if test -n "${target_configdirs}" ; then
  others=
  for i in `echo ${target_configdirs} | sed -e s/target-//g` ; do
    if test "${i}" != "libiberty" ; then
      if test -r ${srcdir}/${i}/configure ; then
	others=yes;
	break;
      fi
    fi
  done
  if test -z "${others}" ; then
    target_configdirs=""
  fi
fi

# Quietly strip out all directories which are NOT configurable in this
# tree. This relies on all configurable subdirectories being
# autoconfiscated, which is now the case.
build_configdirs_all="${build_configdirs}"
build_configdirs=
for i in ${build_configdirs_all} ; do
  j=`echo ${i} | sed -e s/build-//g` 
  if test -f ${srcdir}/${j}/configure ; then
    build_configdirs="${build_configdirs} ${i}"
  fi
done

configdirs_all="${configdirs}"
configdirs=
for i in ${configdirs_all} ; do
  if test -f ${srcdir}/${i}/configure ; then
    configdirs="${configdirs} ${i}"
  fi
done

target_configdirs_all="${target_configdirs}"
target_configdirs=
for i in ${target_configdirs_all} ; do
  j=`echo $i | sed -e s/target-//g` 
  if test -f ${srcdir}/$j/configure ; then
    target_configdirs="${target_configdirs} $i"
  fi
done

# Produce a warning message for the subdirs we cannot configure.
# This is NOT especially interesting in the Cygnus tree, but in the
# individual FSF releases, it is important to let people know when their
# machine is NOT supported by the one or two programs in a package.

if test -n "${notsupp}" && test -z "${norecursion}" ; then
  # If $appdirs is non-empty, at least one of those directories must still
  # be configured, or we error out. (E.g., if the gas release supports a
  # specified target in some subdirs but not the gas subdir, we should NOT
  # pretend that all is well.)
  if test -n "$appdirs" ; then
    for dir in $appdirs ; do
      if test -r $dir/Makefile.in ; then
	if echo " ${configdirs} " | grep " ${dir} " >/dev/null 2>&1; then
	  appdirs=""
	  break
	fi
	if echo " ${target_configdirs} " | grep " target-${dir} " >/dev/null 2>&1; then
	  appdirs=""
	  break
	fi
      fi
    done
    if test -n "$appdirs" ; then
      echo "*** This configuration is not supported by this package." 1>&2
      exit 1
    fi
  fi
  # Okay, some application will build, or we do not care to check. Still
  # notify of subdirs not getting built.
  echo "*** This configuration is not supported in the following subdirectories:" 1>&2
  echo "    ${notsupp}" 1>&2
  echo "    (Any other directories should still work fine.)" 1>&2
fi

case "${host}" in
  *msdosdjgpp*)
    enable_gdbtk=no ;;
esac

copy_dirs=

AC_ARG_WITH([build-sysroot], 
  [AS_HELP_STRING([--with-build-sysroot=sysroot],[use sysroot as the system root during the build])],
  [if test x"$withval" != x ; then
     SYSROOT_CFLAGS_FOR_TARGET="--sysroot=$withval"
   fi],
  [SYSROOT_CFLAGS_FOR_TARGET=])
AC_SUBST([SYSROOT_CFLAGS_FOR_TARGET])

# Handle --with-headers=XXX. If the value is not "yes", the contents of
# the named directory are copied to $(tooldir)/sys-include.
if test x"${with_headers}" != x && test x"${with_headers}" != xno ; then
  if test x${is_cross_compiler} = xno ; then
    echo 1>&2 '***' --with-headers is only supported when cross compiling
    exit 1
  fi
  if test x"${with_headers}" != xyes ; then
    case "${exec_prefixoption}" in
    "") x=${prefix} ;;
    *) x=${exec_prefix} ;;
    esac
    copy_dirs="${copy_dirs} ${with_headers} $x/${target}/sys-include"
  fi
fi

# Handle --with-libs=XXX. If the value is not "yes", the contents of
# the name directories are copied to $(tooldir)/lib. Multiple directories
# are permitted.
if test x"${with_libs}" != x && test x"${with_libs}" != xno ; then
  if test x${is_cross_compiler} = xno ; then
    echo 1>&2 '***' --with-libs is only supported when cross compiling
    exit 1
  fi
  if test x"${with_libs}" != xyes ; then
    # Copy the libraries in reverse order, so that files in the first named
    # library override files in subsequent libraries.
    case "${exec_prefixoption}" in
    "") x=${prefix} ;;
    *) x=${exec_prefix} ;;
    esac
    for l in ${with_libs}; do
      copy_dirs="$l $x/${target}/lib ${copy_dirs}"
    done
  fi
fi

# Handle ${copy_dirs}
set fnord ${copy_dirs}
shift
while test $# != 0 ; do
  if test -f $2/COPIED && test x"`cat $2/COPIED`" = x"$1" ; then
    :
  else
    echo Copying $1 to $2

    # Use the install script to create the directory and all required
    # parent directories.
    if test -d $2 ; then
      :
    else
      echo >config.temp
      ${srcdir}/install-sh -c -m 644 config.temp $2/COPIED
    fi

    # Copy the directory, assuming we have tar.
    # FIXME: Should we use B in the second tar? Not all systems support it.
    (cd $1; tar -cf - .) | (cd $2; tar -xpf -)

    # It is the responsibility of the user to correctly adjust all
    # symlinks. If somebody can figure out how to handle them correctly
    # here, feel free to add the code.

    echo $1 > $2/COPIED
  fi
  shift; shift
done

extra_arflags_for_target=
extra_nmflags_for_target=
extra_ranlibflags_for_target=
target_makefile_frag=/dev/null
case "${target}" in
  *-*-netware*)
    target_makefile_frag="config/mt-netware"
    ;;
  *-*-linux* | *-*-gnu* | *-*-k*bsd*-gnu)
    target_makefile_frag="config/mt-gnu"
    ;;
  *-*-linux*)
    target_makefile_frag="config/mt-linux"
    ;;
  *-*-aix4.[[3456789]]* | *-*-aix[[56789]].*)
    # nm and ar from AIX 4.3+ require -X32_64 flag to all ar and nm 
    # commands to handle both 32-bit and 64-bit objects. These flags are
    # harmless if we are using GNU nm or ar.
    extra_arflags_for_target=" -X32_64"
    extra_nmflags_for_target=" -B -X32_64"
    ;;
  *-*-darwin*)
    # ranlib from Darwin requires the -c flag to look at common symbols.
    extra_ranlibflags_for_target=" -c"
    ;;
  mips*-*-pe | sh*-*-pe | *arm-wince-pe)
    target_makefile_frag="config/mt-wince"
    ;;
esac

alphaieee_frag=/dev/null
case $target in
  alpha*-*-*)
    # This just makes sure to use the -mieee option to build target libs.
    # This should probably be set individually by each library.
    alphaieee_frag="config/mt-alphaieee"
    ;;
esac

# If --enable-target-optspace always use -Os instead of -O2 to build
# the target libraries, similarly if it is not specified, use -Os
# on selected platforms.
ospace_frag=/dev/null
case "${enable_target_optspace}:${target}" in
  yes:*)
    ospace_frag="config/mt-ospace"
    ;;
  :d30v-*)
    ospace_frag="config/mt-d30v"
    ;;
  :m32r-* | :d10v-* | :fr30-*)
    ospace_frag="config/mt-ospace"
    ;;
  no:* | :*)
    ;;
  *)
    echo "*** bad value \"${enable_target_optspace}\" for --enable-target-optspace flag; ignored" 1>&2
    ;;
esac

# Set with_gnu_as and with_gnu_ld as appropriate.
#
# This is done by determining whether or not the appropriate directory
# is available, and by checking whether or not specific configurations
# have requested that this magic not happen.
# 
# The command line options always override the explicit settings in 
# configure.ac, and the settings in configure.ac override this magic.
#
# If the default for a toolchain is to use GNU as and ld, and you do NOT 
# want to do that, then you should use the --without-gnu-as and
# --without-gnu-ld options for the configure script.

if test x${use_gnu_as} = x &&
   echo " ${configdirs} " | grep " gas " > /dev/null 2>&1 ; then
  with_gnu_as=yes
  extra_host_args="$extra_host_args --with-gnu-as"
fi

if test x${use_gnu_ld} = x &&
   echo " ${configdirs} " | grep " ld " > /dev/null 2>&1 ; then
  with_gnu_ld=yes
  extra_host_args="$extra_host_args --with-gnu-ld"
fi

# If using newlib, add --with-newlib to extra_host_args so gcc/configure
# can detect this case.

if test x${with_newlib} != xno && echo " ${target_configdirs} " | grep " target-newlib " > /dev/null 2>&1 ; then
  with_newlib=yes
  extra_host_args="$extra_host_args --with-newlib"
fi

# Default to using --with-stabs for certain targets.
if test x${with_stabs} = x ; then
  case "${target}" in
  mips*-*-irix[[56]]*)
    ;;
  mips*-*-* | alpha*-*-osf*)
    with_stabs=yes;
    extra_host_args="${extra_host_args} --with-stabs"
    ;;
  esac
fi

# hpux11 in 64bit mode has libraries in a weird place. Arrange to find
# them automatically.
case "${host}" in
  hppa*64*-*-hpux11*)	
    extra_host_args="$extra_host_args -x-libraries=/usr/lib/pa20_64 -x-includes=/usr/X11R6/include"
    ;;
esac

# Some systems (e.g., one of the i386-aix systems the gas testers are
# using) do NOT handle "\$" correctly, so do NOT use it here.
# APPLE LOCAL: Use "host" over "host_alias" - autoconf 2.13/2.57 diff?
tooldir='${exec_prefix}'/${target}
build_tooldir=${tooldir}

# Generate default definitions for YACC, M4, LEX and other programs to run
# on the build machine. These are used if the Makefile cannot locate these
# programs in objdir.
MISSING=`cd $ac_aux_dir && ${PWDCMD-pwd}`/missing
AC_CHECK_PROGS([CONFIGURED_BISON],[bison],[$MISSING bison])
AC_CHECK_PROGS([CONFIGURED_YACC],['bison -y' byacc yacc],[$MISSING bison -y])
AC_CHECK_PROGS([CONFIGURED_M4],[gm4 gnum4 m4],[$MISSING m4])
AC_CHECK_PROGS([CONFIGURED_FLEX],[flex],[$MISSING flex])
AC_CHECK_PROGS([CONFIGURED_LEX],[flex lex],[$MISSING flex])
AC_CHECK_PROGS([CONFIGURED_MAKEINFO],[makeinfo],[$MISSING makeinfo])

AM_PROG_LEX
AC_PROG_YACC

# Create a .gdbinit file which runs the one in srcdir
# and tells GDB to look there for source files.

if test -r ${srcdir}/.gdbinit ; then
  case ${srcdir} in
    .) ;;
    *) cat > ./.gdbinit <<EOF
# ${NO_EDIT}
dir ${srcdir}
dir .
source ${srcdir}/.gdbinit
EOF
    ;;
  esac
fi

# Make sure that the compiler is able to generate an executable. If it
# cannot, we are probably in trouble. We do NOT care whether we can run the
# executable--we might be using a cross compiler--we only care whether it
# can be created. At this point the main configure script should have set
# CC.
we_are_ok=no
echo "int main () { return 0; }" > conftest.c
${CC} -o conftest ${CFLAGS} ${CPPFLAGS} ${LDFLAGS} conftest.c
if test $? = 0 ; then
  if test -s conftest || test -s conftest.exe ; then
    we_are_ok=yes
  fi
fi 
case $we_are_ok in
  no)
    echo 1>&2 "*** The command '${CC} -o conftest ${CFLAGS} ${CPPFLAGS} ${LDFLAGS} conftest.c' failed."
    echo 1>&2 "*** You must set the environment variable CC to a working compiler."
    rm -rf conftest*
    exit 1
    ;;
esac
rm -rf conftest*

# The Solaris /usr/ucb/cc compiler does not appear to work.
case "${host}" in
  sparc-sun-solaris2*)
      CCBASE="`echo ${CC-cc} | sed 's/ .*$//'`"
      if test "`type $CCBASE | sed 's/^[[^/]]*//'`" = "/usr/ucb/cc" ; then
          could_use=
          test -d /opt/SUNWspro/bin && could_use="/opt/SUNWspro/bin"
          if test -d /opt/cygnus/bin ; then
              if test "$could_use" = "" ; then
                  could_use="/opt/cygnus/bin"
              else
                  could_use="$could_use or /opt/cygnus/bin"
              fi
          fi
        if test "${could_use}" = "" ; then
            echo "Warning: compilation may fail because you are using"
            echo "/usr/ucb/cc.  You should change your PATH or CC "
            echo "variable and rerun configure."
        else
            echo "Warning: compilation may fail because you are using"
            echo "/usr/ucb/cc, when you should use the C compiler from"
            echo "${could_use}. You should change your"
            echo "PATH or CC variable and rerun configure."
        fi
      fi
  ;;
esac

case "${host}" in
  *-*-hpux*) RPATH_ENVVAR=SHLIB_PATH ;;
  *-*-darwin* | *-*-rhapsody* ) RPATH_ENVVAR=DYLD_LIBRARY_PATH ;;
  *-*-mingw* | *-*-cygwin ) RPATH_ENVVAR=PATH ;;
  *) RPATH_ENVVAR=LD_LIBRARY_PATH ;;
esac

# On systems where the dynamic library environment variable is PATH,
# gcc/ will put dynamic libraries into a subdirectory to avoid adding
# built executables to PATH.
if test "${RPATH_ENVVAR}" = PATH; then
  GCC_SHLIB_SUBDIR=/shlib
else
  GCC_SHLIB_SUBDIR=""
fi

# Record target_configdirs and the configure arguments for target and
# build configuration in Makefile.
target_configdirs=`echo "${target_configdirs}" | sed -e 's/target-//g'`
build_configdirs=`echo "${build_configdirs}" | sed -e 's/build-//g'`

# If we are building libgomp, bootstrap it.
if echo " ${target_configdirs} " | grep " libgomp " > /dev/null 2>&1 ; then
  bootstrap_target_libs=${bootstrap_target_libs}target-libgomp,
fi

# If we are building libsanitizer, bootstrap it.
if echo " ${target_configdirs} " | grep " libsanitizer " > /dev/null 2>&1 ; then
  bootstrap_target_libs=${bootstrap_target_libs}target-libsanitizer,
fi

# If we are building libvtv, bootstrap it.
if echo " ${target_configdirs} " | grep " libvtv " > /dev/null 2>&1 ; then
  bootstrap_target_libs=${bootstrap_target_libs}target-libvtv,
fi

# Determine whether gdb needs tk/tcl or not.
# Use 'maybe' since enable_gdbtk might be true even if tk is NOT available
# and in that case we want gdb to be built without tk. Ugh!
# In fact I believe gdb is the *only* package directly dependent on tk,
# so we should be able to put the 'maybe's in unconditionally and
# leave out the maybe dependencies when enable_gdbtk is false. I am not
# 100% sure that that is safe though.

gdb_tk="maybe-all-tcl maybe-all-tk maybe-all-itcl maybe-all-libgui"
case "${enable_gdbtk}" in
  no)
    GDB_TK="" ;;
  yes)
    GDB_TK="${gdb_tk}" ;;
  *)
    # Only add the dependency on gdbtk when GDBtk is part of the gdb
    # distro. Eventually someone will fix this and move Insight, nee
    # gdbtk to a separate directory.
    if test -d ${srcdir}/gdb/gdbtk ; then
      GDB_TK="${gdb_tk}"
    else
      GDB_TK=""
    fi
    ;;
esac
CONFIGURE_GDB_TK=`echo ${GDB_TK} | sed s/-all-/-configure-/g`
INSTALL_GDB_TK=`echo ${GDB_TK} | sed s/-all-/-install-/g`

# Strip out unwanted targets.

# While at that, we remove Makefiles if we were started for recursive
# configuration, so that the top-level Makefile reconfigures them,
# like we used to do when configure itself was recursive.

# Loop over modules. ${extrasub} must be used with care, limiting as
# much as possible the usage of range addresses. That is because autoconf
# splits the sed script to overcome limits in the number of commands,
# and relying on carefully-timed sed passes may turn out to be very hard
# to maintain later. In this particular case, you just have to be careful
# not to nest @if/@endif pairs, because configure will not warn you at all.

dnl# newer versions want to put this bootstrapping block farther up...
AC_ARG_ENABLE([bootstrap],
[AS_HELP_STRING([--enable-bootstrap[=lean]],[Enable bootstrapping [no]])],[],
[enable_bootstrap=default])

# Issue errors and warnings for invalid/strange bootstrap combinations.
case "${configdirs}" in
  *gcc*) have_compiler=yes ;;
  *) have_compiler=no ;;
esac

case "${have_compiler}:${host}:${target}:${enable_bootstrap}" in
  *:*:*:no) ;;

  # Default behavior. (We will) enable bootstrap if we have a compiler
  # and we are in a native configuration.
  yes:${build}:${build}:default)
    # This will become 'yes'
    enable_bootstrap=no ;;

  *:*:*:default)
    enable_bootstrap=no ;;

  # We have a compiler & we are in a native configuration, bootstrap is ok
  yes:${build}:${build}:yes | yes:${build}:${build}:lean)
    ;;

  # Other configurations, but we have a compiler. Assume the user knows
  # what he is doing.
  yes:*:*:yes | yes:*:*:lean)
    AC_MSG_WARN([trying to bootstrap a cross compiler])
    ;;

  # No compiler: if they passed --enable-bootstrap explicitly, fail
  no:*:*:yes | no:*:*:lean)
    AC_MSG_ERROR([cannot bootstrap without a compiler]) ;;

  # Fail if wrong command line
  *)
    AC_MSG_ERROR([invalid option for --enable-bootstrap])
    ;;
esac

# Adjust the toplevel makefile according to whether bootstrap was selected.
case "${enable_bootstrap}" in
  yes)
    bootstrap_lean='#'
    default_target=bootstrap
    bootstrap_suffix=bootstrap ;;
  lean)
    bootstrap_lean=''
    default_target=bootstrap
    bootstrap_suffix=bootstrap ;;
  no)
    default_target=all
    bootstrap_suffix=no-bootstrap ;;
esac
AC_SUBST([bootstrap_lean])
AC_SUBST([default_target])

for module in ${build_configdirs} ; do
  if test -z "${no_recursion}" \
     && test -f ${build_subdir}/${module}/Makefile; then
    echo 1>&2 "*** removing ${build_subdir}/${module}/Makefile to force reconfigure"
    rm -f ${build_subdir}/${module}/Makefile
  fi
  extrasub="$extrasub
/^@if build-$module\$/d
/^@endif build-$module\$/d
/^@if build-$module-$bootstrap_suffix\$/d
/^@endif build-$module-$bootstrap_suffix\$/d"
done
for module in ${configdirs} ; do
  if test -z "${no_recursion}" \
     && test -f ${module}/Makefile; then
    echo 1>&2 "*** removing ${module}/Makefile to force reconfigure"
    rm -f ${module}/Makefile
  fi
  extrasub="$extrasub
/^@if $module\$/d
/^@endif $module\$/d
/^@if $module-$bootstrap_suffix\$/d
/^@endif $module-$bootstrap_suffix\$/d"
done
for module in ${target_configdirs} ; do
  if test -z "${no_recursion}" \
     && test -f ${target_subdir}/${module}/Makefile; then
    echo 1>&2 "*** removing ${target_subdir}/${module}/Makefile to force reconfigure"
    rm -f ${target_subdir}/${module}/Makefile
  fi
  extrasub="$extrasub
/^@if target-$module\$/d
/^@endif target-$module\$/d
/^@if target-$module-$bootstrap_suffix\$/d
/^@endif target-$module-$bootstrap_suffix\$/d"
done

extrasub="$extrasub
/^@if /,/^@endif /d"

# Create the serialization dependencies. This uses a temporary file.

AC_ARG_ENABLE([serial-configure],
[AS_HELP_STRING([--enable-serial-[{host,target,build}-]configure],[Force sequential configuration of sub-packages for the host, target or build machine, or all sub-packages])])

case ${enable_serial_configure} in
  yes)
    enable_serial_build_configure=yes
    enable_serial_host_configure=yes
    enable_serial_target_configure=yes
    ;;
esac

# These force 'configure's to be done one at a time, to avoid problems
# with contention over a shared config.cache.
rm -f serdep.tmp
echo '# serdep.tmp' > serdep.tmp
olditem=
test "x${enable_serial_build_configure}" = xyes &&
for item in ${build_configdirs} ; do
  case ${olditem} in
    "") ;;
    *) echo "configure-build-${item}: configure-build-${olditem}" >> serdep.tmp ;;
  esac
  olditem=${item}
done
olditem=
test "x${enable_serial_host_configure}" = xyes &&
for item in ${configdirs} ; do
  case ${olditem} in
    "") ;;
    *) echo "configure-${item}: configure-${olditem}" >> serdep.tmp ;;
  esac
  olditem=${item}
done
olditem=
test "x${enable_serial_target_configure}" = xyes &&
for item in ${target_configdirs} ; do
  case ${olditem} in
    "") ;;
    *) echo "configure-target-${item}: configure-target-${olditem}" >> serdep.tmp ;;
  esac
  olditem=${item}
done
serialization_dependencies=serdep.tmp
AC_SUBST_FILE([serialization_dependencies])

# Base args. Strip norecursion, cache-file, srcdir, host, build,
# target and nonopt. These are the ones we might not want to pass
# down to subconfigures. Also strip program-prefix, program-suffix,
# and program-transform-name, so that we can pass down a consistent
# program-transform-name. If autoconf has put single quotes around
# any of these arguments (because they contain shell metacharacters)
# then this will fail; in practice this only happens for
# --program-transform-name, so be sure to override --program-transform-name
# at the end of the argument list.
# These will be expanded by make, so quote '$'.
cat <<\EOF_SED > conftestsed
s/ --no[[^ ]]*/ /g
s/ --c[[a-z-]]*[[= ]][[^ ]]*//g
s/ --sr[[a-z-]]*[[= ]][[^ ]]*//g
s/ --ho[[a-z-]]*[[= ]][[^ ]]*//g
s/ --bu[[a-z-]]*[[= ]][[^ ]]*//g
s/ --t[[a-z-]]*[[= ]][[^ ]]*//g
s/ --program-[[pst]][[a-z-]]*[[= ]][[^ ]]*//g
s/ -cache-file[[= ]][[^ ]]*//g
s/ -srcdir[[= ]][[^ ]]*//g
s/ -host[[= ]][[^ ]]*//g
s/ -build[[= ]][[^ ]]*//g
s/ -target[[= ]][[^ ]]*//g
s/ -program-prefix[[= ]][[^ ]]*//g
s/ -program-suffix[[= ]][[^ ]]*//g
s/ -program-transform-name[[= ]][[^ ]]*//g
s/ -arch ([^ ]*)/-arch_\1/g
s/ [[^' -][^ ]*] / /
s/ -arch_([^ ]*)/-arch \1/g
s/^ *//;s/ *$//
s,\$,$$,g
EOF_SED
sed -f conftestsed <<EOF_SED > conftestsed.out
 ${ac_configure_args} 
EOF_SED
baseargs=`cat conftestsed.out`
rm -rf conftestsed conftestsed.out

# Add in --program-transform-name, after --program-prefix and
# --program-suffix have been applied to it. Autoconf has already
# doubled dollar signs and backslashes in program_transform_name; we want
# the backslashes un-doubled, and then the entire thing wrapped in single
# quotes, because this will be expanded first by make and then by the
# shell. Also, because we want to override the logic in subdir configure
# scripts to choose program_transform_name, replace any s,x,x, with s,y,y,.
sed -e "s,\\\\\\\\,\\\\,g; s,','\\\\'',g; s/s,x,x,/s,y,y,/" <<EOF_SED > conftestsed.out
${program_transform_name}
EOF_SED
gcc_transform_name=`cat conftestsed.out`
rm -f conftestsed.out
baseargs="${baseargs} --program-transform-name='${gcc_transform_name}'"

# For the build-side libraries, we just need to pretend we are native,
# and not use the same cache file.  Multilibs are neither needed nor
# desired.
# APPLE LOCAL: Use "host" over "host_alias" - autoconf 2.13/2.57 diff?
build_configargs="${build_configargs} --cache-file=../config.cache --build=${build} --host=${build} ${baseargs}"

# For host modules, accept cache file option, or specification as blank.
case "${cache_file}" in
"") # empty
  cache_file_option="" ;;
/* | [[A-Za-z]]:[[\\/]]* ) # absolute path
  cache_file_option="--cache-file=${cache_file}" ;;
*) # relative path
  cache_file_option="--cache-file=../${cache_file}" ;;
esac

# Host dirs do not like to share a cache file either, horribly enough.
# This seems to be due to autoconf 2.5x stupidity.
# APPLE LOCAL
host_configargs="$host_configargs --cache-file=./config.cache --build=${build} --host=${host} --target=${target} ${extra_host_args} ${baseargs}"

target_configargs=${baseargs}

# Passing a --with-cross-host argument lets the target libraries know
# whether they are being built with a cross-compiler or being built
# native. However, it would be better to use other mechanisms to make the
# sorts of decisions they want to make on this basis. Please consider
# this option to be deprecated. FIXME.
if test x${is_cross_compiler} = xyes ; then
# APPLE LOCAL: Use "host" instead of "host_alias" and such -
# autoconf 2.13/2.57 diff?
  target_configargs="--with-cross-host=${host} ${target_configargs}"
fi

# Default to --enable-multilib.
if test x${enable_multilib} = x ; then
  target_configargs="--enable-multilib ${target_configargs}"
fi

# Pass --with-newlib if appropriate. Note that target_configdirs has
# changed from the earlier setting of with_newlib.
if test x${with_newlib} != xno && echo " ${target_configdirs} " | grep " newlib " > /dev/null 2>&1 && test -d ${srcdir}/newlib ; then
  target_configargs="--with-newlib ${target_configargs}"
fi

# Different target subdirs use different values of certain variables
# (notably CXX). Worse, multilibs use *lots* of different values.
# Worse yet, autoconf 2.5x makes some of these 'precious', meaning that
# it does NOT automatically accept command-line overrides of them.
# This means it is not safe for target subdirs to share a cache file,
# which is disgusting, but there you have it. Hopefully this can be
# fixed in future. It is still worthwhile to use a cache file for each
# directory. I think.

# Pass the appropriate --build, --host, --target and --cache-file
# arguments. We need to pass --target, as newer autoconfes requires
# consistency for target_alias and gcc does NOT manage it consistently.
# APPLE LOCAL: Use "host" instead of "host_alias" and such -
# autoconf 2.13/2.57 diff?
old_target_configargs="--cache-file=./config.cache --host=${target} --build=${build} ${target_configargs}"
target_configargs="--cache-file=./config.cache --build=${build_alias} --host=${target_alias} --target=${target_alias} ${target_configargs}"

FLAGS_FOR_TARGET=
case " ${target_configdirs} " in
 *" newlib "*)
  case " ${target_configargs} " in
  *" --with-newlib "*)
   case "$target" in
   *-cygwin*)
     FLAGS_FOR_TARGET=${FLAGS_FOR_TARGET}' -L$$r/$(TARGET_SUBDIR)/winsup -L$$r/$(TARGET_SUBDIR)/winsup/cygwin -L$$r/$(TARGET_SUBDIR)/winsup/w32api/lib -isystem $$s/winsup/include -isystem $$s/winsup/cygwin/include -isystem $$s/winsup/w32api/include' ;;
   esac

   # If we are not building GCC, do NOT discard standard headers.
   if test -d ${srcdir}/gcc; then
     FLAGS_FOR_TARGET=${FLAGS_FOR_TARGET}' -nostdinc'

     if test "${build}" != "${host}"; then
       # On Canadian crosses, CC_FOR_TARGET will have already been set
       # by `configure', so we will NOT have an opportunity to add -Bgcc/
       # to it. This is right: we do NOT want to search that directory
       # for binaries, but we want the header files in there, so add
       # them explicitly.
       FLAGS_FOR_TARGET=${FLAGS_FOR_TARGET}' -isystem $$r/$(HOST_SUBDIR)/gcc/include'

       # Someone might think of using the pre-installed headers on
       # Canadian crosses, in case the installed compiler is not fully
       # compatible with the compiler being built. In this case, it
       # would be better to flag an error than risking having
       # incompatible object files being constructed. We cannot
       # guarantee that an error will be flagged, but let us hope the
       # compiler will do it, when presented with incompatible header
       # files.
     fi
   fi

   case "${target}-${is_cross_compiler}" in
   i[[3456789]]86-*-linux*-no)
      # Here host == target, so we do NOT need to build gcc,
      # so we do NOT want to discard standard headers.
      FLAGS_FOR_TARGET=`echo " ${FLAGS_FOR_TARGET} " | sed -e 's/ -nostdinc / /'`
      ;;
   *)
      # If we are building newlib, use its generic headers last, but search
      # for any libc-related directories first (so make it the last -B
      # switch).
      FLAGS_FOR_TARGET=${FLAGS_FOR_TARGET}' -B$$r/$(TARGET_SUBDIR)/newlib/ -isystem $$r/$(TARGET_SUBDIR)/newlib/targ-include -isystem $$s/newlib/libc/include'
      ;;
   esac
   ;;
  esac
  ;;
esac

# Allow the user to override the flags for
# our build compiler if desired.
CFLAGS_FOR_BUILD=${CFLAGS_FOR_BUILD-${CFLAGS}}
CXXFLAGS_FOR_BUILD=${CXXFLAGS_FOR_BUILD-${CXXFLAGS}}
LDFLAGS_FOR_BUILD=${LDFLAGS_FOR_BUILD-${LDFLAGS}}

# On Canadian crosses, we will be searching the right directories for
# the previously-installed cross compiler, so do not bother to add
# flags for directories within the install tree of the compiler
# being built; programs in there will not even run.
if test "${build}" = "${host}" && test -d ${srcdir}/gcc; then
  # Search for pre-installed headers if nothing else fits.
  FLAGS_FOR_TARGET=${FLAGS_FOR_TARGET}' -B$(build_tooldir)/bin/ -B$(build_tooldir)/lib/ -isystem $(build_tooldir)/include -isystem $(build_tooldir)/sys-include'
fi

if test "x${use_gnu_ld}" = x &&
   echo " ${configdirs} " | grep " ld " > /dev/null ; then
  # Arrange for us to find uninstalled linker scripts.
  FLAGS_FOR_TARGET=${FLAGS_FOR_TARGET}' -L$$r/$(HOST_SUBDIR)/ld'
fi

# Search for other target-specific linker scripts and such.
case "${target}" in
  mep*)
    FLAGS_FOR_TARGET="${FLAGS_FOR_TARGET} -mlibrary"
    ;;
esac

# Makefile fragments.
for frag in host_makefile_frag target_makefile_frag alphaieee_frag ospace_frag;
do
  eval fragval=\$$frag
  if test ${fragval} != /dev/null; then
    eval ${frag}=${srcdir}/${fragval}
  fi
done
AC_SUBST_FILE([host_makefile_frag])
AC_SUBST_FILE([target_makefile_frag])
AC_SUBST_FILE([alphaieee_frag])
AC_SUBST_FILE([ospace_frag])

# Miscellanea: directories, flags, etc.
AC_SUBST([RPATH_ENVVAR])
AC_SUBST([GCC_SHLIB_SUBDIR])
AC_SUBST([BUILD_PREFIX])
AC_SUBST([BUILD_PREFIX_1])
AC_SUBST([tooldir])
AC_SUBST([build_tooldir])
AC_SUBST([CONFIGURE_GDB_TK])
AC_SUBST([GDB_TK])
AC_SUBST([INSTALL_GDB_TK])

# Build module lists & subconfigure args.
AC_SUBST([build_configargs])
AC_SUBST([build_configdirs])

# Host module lists & subconfigure args.
AC_SUBST([host_configargs])
AC_SUBST([configdirs])

# Target module lists & subconfigure args.
AC_SUBST([target_configargs])
AC_SUBST([target_configdirs])

# Build tools.
AC_SUBST([AR_FOR_BUILD])
AC_SUBST([AS_FOR_BUILD])
AC_SUBST([CC_FOR_BUILD])
AC_SUBST([CFLAGS_FOR_BUILD])
AC_SUBST([CXXFLAGS_FOR_BUILD])
AC_SUBST([CXX_FOR_BUILD])
AC_SUBST([DLLTOOL_FOR_BUILD])
AC_SUBST([GCJ_FOR_BUILD])
AC_SUBST([GFORTRAN_FOR_BUILD])
AC_SUBST([GOC_FOR_BUILD])
AC_SUBST([LDFLAGS_FOR_BUILD])
AC_SUBST([LD_FOR_BUILD])
AC_SUBST([NM_FOR_BUILD])
AC_SUBST([RANLIB_FOR_BUILD])
AC_SUBST([WINDMC_FOR_BUILD])
AC_SUBST([WINDRES_FOR_BUILD])
AC_SUBST([config_shell])

# Generate default definitions for YACC, M4, LEX & other programs that run
# on the build machine. These are used if the Makefile cannot locate these
# programs in objdir.
if test -z "${MISSING}"; then
  MISSING=`cd ${ac_aux_dir} && ${PWDCMD-pwd}`/missing
fi

if test -z "${YACC}"; then
  AC_CHECK_PROGS([YACC],['bison -y' byacc yacc],[${MISSING} bison -y])
  case " ${build_configdirs} " in
    *" bison "*) YACC='$$r/$(BUILD_SUBDIR)/bison/tests/bison -y' ;;
    *" byacc "*)
      if test -e ${srcdir}/byacc/byacc; then
        YACC='$$r/$(BUILD_SUBDIR)/byacc/byacc'
      elif test -e ${srcdir}/byacc/yacc; then
        YACC='$$r/$(BUILD_SUBDIR)/byacc/yacc'
      fi
      ;;
  esac
fi

AC_CHECK_PROGS([BISON],[bison],[${MISSING} bison])
case " ${build_configdirs} " in
  *" bison "*) BISON='$$r/$(BUILD_SUBDIR)/bison/tests/bison' ;;
esac

AC_CHECK_PROGS([M4],[gm4 gnum4 m4],[${MISSING} m4])
case " ${build_configdirs} " in
  *" m4 "*) M4='$$r/$(BUILD_SUBDIR)/m4/m4' ;;
esac

if test -z "${LEX}"; then
  AC_CHECK_PROGS([LEX],[flex lex],[${MISSING} flex])
  case " ${build_configdirs} " in
    *" flex "*) LEX='$$r/$(BUILD_SUBDIR)/flex/flex' ;;
    *" lex "*) LEX='$$r/$(BUILD_SUBDIR)/lex/lex' ;;
  esac
fi

AC_CHECK_PROGS([FLEX],[flex],[${MISSING} flex])
case " ${build_configdirs} " in
  *" flex "*) FLEX='$$r/$(BUILD_SUBDIR)/flex/flex' ;;
esac

AC_CHECK_PROGS([MAKEINFO],[makeinfo],[${MISSING} makeinfo])
case " ${build_configdirs} " in
  *" texinfo "*) MAKEINFO='$$r/$(BUILD_SUBDIR)/texinfo/makeinfo/makeinfo' ;;
  *)
changequote(,)
    # For an installed makeinfo, we require it to be from texinfo 4.7 or
    # higher, else we use the "missing" dummy.
    if ${MAKEINFO} --version \
       | egrep 'texinfo[^0-9]*(4\.([7-9]|[1-9][0-9])|[5-9]|[1-9][0-9])' >/dev/null 2>&1; then
      :
    else
      MAKEINFO="${MISSING} makeinfo"
    fi
    ;;
changequote([,])
esac

# FIXME: expect and dejagnu may become build tools?

AC_CHECK_PROGS([EXPECT],[expect],[expect])
case " ${configdirs} " in
  *" expect "*)
    test ${host} = ${build} && EXPECT='$$r/$(HOST_SUBDIR)/expect/expect'
    ;;
esac

AC_CHECK_PROGS([RUNTEST],[runtest],[runtest])
case " ${configdirs} " in
  *" dejagnu "*)
    test ${host} = ${build} && RUNTEST='$$s/$(HOST_SUBDIR)/dejagnu/runtest'
    ;;
esac

# Host tools.
NCN_STRICT_CHECK_TOOLS([AR],[ar])
NCN_STRICT_CHECK_TOOLS([AS],[as])
NCN_STRICT_CHECK_TOOLS([DLLTOOL],[dlltool])
NCN_STRICT_CHECK_TOOLS([LD],[ld])
NCN_STRICT_CHECK_TOOLS([LIPO],[lipo])
NCN_STRICT_CHECK_TOOLS([NM],[nm])
NCN_STRICT_CHECK_TOOLS([RANLIB],[ranlib],[:])
NCN_STRICT_CHECK_TOOLS([STRIP],[strip],[true])
NCN_STRICT_CHECK_TOOLS([WINDRES],[windres])
NCN_STRICT_CHECK_TOOLS([WINDMC],[windmc])
NCN_STRICT_CHECK_TOOLS([OBJCOPY],[objcopy])
NCN_STRICT_CHECK_TOOLS([OBJDUMP],[objdump])
NCN_STRICT_CHECK_TOOLS([READELF],[readelf])

AM_PROG_AR
AM_PROG_AS
if test "x${RANLIB}" = "x"; then
	test -z "${RANLIB}"
	AC_PROG_RANLIB
else
	test ! -z "${RANLIB}"
fi
if test -x ${srcdir}/libtool; then
  test -r libtool
  if test "x${LIBTOOL_FLAGS}" = "x"; then
    test -z "${LIBTOOL_FLAGS}"
    export LIBTOOL_FLAGS=--debug
  else
    test ! -z "${LIBTOOL_FLAGS}"
    AC_MSG_NOTICE([LIBTOOL_FLAGS are set to "${LIBTOOL_FLAGS}"])
  fi
else
  test -e libtool
  AC_CHECK_HEADERS_ONCE([dlfcn.h])
fi

AC_SUBST([CC])
AC_SUBST([CXX])
AC_SUBST([CFLAGS])
AC_SUBST([CFLAGS_FOR_BUILD])
AC_SUBST([CXXFLAGS])

# Target tools.
AC_ARG_WITH([build-time-tools], 
  [AS_HELP_STRING([--with-build-time-tools=PATH],
		  [use given path to find target tools during the build])],
  [case x"${withval}" in
     x/*) ;;
     *)
       with_build_time_tools=""
       AC_MSG_WARN([argument to --with-build-time-tools must be an absolute path])
       ;;
   esac],
  [with_build_time_tools=])

# Target tools. Do the tests using the names they may have passed in
# the environment, then move it to CONFIGURED_*_FOR_TARGET.
NCN_STRICT_CHECK_TARGET_TOOLS([AR_FOR_TARGET],[ar])
NCN_STRICT_CHECK_TARGET_TOOLS([AS_FOR_TARGET],[as])
NCN_STRICT_CHECK_TARGET_TOOLS([CC_FOR_TARGET],[cc gcc])
NCN_STRICT_CHECK_TARGET_TOOLS([CXX_FOR_TARGET],[c++ g++ cxx gxx])
NCN_STRICT_CHECK_TARGET_TOOLS([DLLTOOL_FOR_TARGET],[dlltool])
NCN_STRICT_CHECK_TARGET_TOOLS([GCC_FOR_TARGET],[gcc],[${CC_FOR_TARGET}])
NCN_STRICT_CHECK_TARGET_TOOLS([GCJ_FOR_TARGET],[gcj])
NCN_STRICT_CHECK_TARGET_TOOLS([GFORTRAN_FOR_TARGET],[gfortran])
NCN_STRICT_CHECK_TARGET_TOOLS([GOC_FOR_TARGET],[gccgo])
NCN_STRICT_CHECK_TARGET_TOOLS([LD_FOR_TARGET],[ld])
NCN_STRICT_CHECK_TARGET_TOOLS([NM_FOR_TARGET],[nm])
NCN_STRICT_CHECK_TARGET_TOOLS([RANLIB_FOR_TARGET],[ranlib],[:])
NCN_STRICT_CHECK_TARGET_TOOLS([WINDRES_FOR_TARGET],[windres])

CONFIGURED_AR_FOR_TARGET="${AR_FOR_TARGET}"
CONFIGURED_AS_FOR_TARGET="${AS_FOR_TARGET}" # might need to be commented out for confusing autoconf
CONFIGURED_CC_FOR_TARGET="${CC_FOR_TARGET}"
CONFIGURED_CXX_FOR_TARGET="${CXX_FOR_TARGET}"
CONFIGURED_DLLTOOL_FOR_TARGET="${DLLTOOL_FOR_TARGET}"
CONFIGURED_GCC_FOR_TARGET="${GCC_FOR_TARGET}"
CONFIGURED_GCJ_FOR_TARGET="${GCJ_FOR_TARGET}"
CONFIGURED_GFORTRAN_FOR_TARGET="${GFORTRAN_FOR_TARGET}"
CONFIGURED_LD_FOR_TARGET="${LD_FOR_TARGET}"
CONFIGURED_NM_FOR_TARGET="${NM_FOR_TARGET}"
CONFIGURED_RANLIB_FOR_TARGET="${RANLIB_FOR_TARGET}"
CONFIGURED_WINDRES_FOR_TARGET="${WINDRES_FOR_TARGET}"

AC_SUBST([CONFIGURED_AR_FOR_TARGET])dnl
AC_SUBST([CONFIGURED_AS_FOR_TARGET])dnl
AC_SUBST([CONFIGURED_CC_FOR_TARGET])dnl
AC_SUBST([CONFIGURED_CXX_FOR_TARGET])dnl
AC_SUBST([CONFIGURED_DLLTOOL_FOR_TARGET])dnl
AC_SUBST([CONFIGURED_GCC_FOR_TARGET])dnl
AC_SUBST([CONFIGURED_GCJ_FOR_TARGET])dnl
AC_SUBST([CONFIGURED_GFORTRAN_FOR_TARGET])dnl
AC_SUBST([CONFIGURED_LD_FOR_TARGET])dnl
AC_SUBST([CONFIGURED_NM_FOR_TARGET])dnl
AC_SUBST([CONFIGURED_RANLIB_FOR_TARGET])dnl
AC_SUBST([CONFIGURED_WINDRES_FOR_TARGET])dnl

AC_SUBST([FLAGS_FOR_TARGET])

# Fix up target tools.
if test "x${build}" = "x${host}" ; then
  # In this case, the newly built tools can and should be used,
  # so we override the results of the autoconf tests.
  # This should really only happen when the tools are actually being built,
  # but that is a further refinement. The new build scheme, where
  # tools are built into a structure paralleling where they are installed,
  # should also eliminate all of this cleanly.
  AR_FOR_TARGET="\$(USUAL_AR_FOR_TARGET)"
  AS_FOR_TARGET="\$(USUAL_AS_FOR_TARGET)" # might need to be commented out for confusing autoconf
  CC_FOR_TARGET="\$(USUAL_CC_FOR_TARGET)"
  CXX_FOR_TARGET="\$(USUAL_CXX_FOR_TARGET)"
  RAW_CXX_FOR_TARGET="\$(USUAL_RAW_CXX_FOR_TARGET)"
  DLLTOOL_FOR_TARGET="\$(USUAL_DLLTOOL_FOR_TARGET)"
  GCC_FOR_TARGET="\$(USUAL_GCC_FOR_TARGET)"
  GCJ_FOR_TARGET="\$(USUAL_GCJ_FOR_TARGET)"
  GFORTRAN_FOR_TARGET="\$(USUAL_GFORTRAN_FOR_TARGET)"
  LD_FOR_TARGET="\$(USUAL_LD_FOR_TARGET)"
  NM_FOR_TARGET="\$(USUAL_NM_FOR_TARGET)"
  RANLIB_FOR_TARGET="\$(USUAL_RANLIB_FOR_TARGET)"
  WINDRES_FOR_TARGET="\$(USUAL_WINDRES_FOR_TARGET)"
else
  # Just use the ones we found.
  AR_FOR_TARGET="\$(CONFIGURED_AR_FOR_TARGET)"
  AS_FOR_TARGET="\$(CONFIGURED_AS_FOR_TARGET)" # might need to be commented out for confusing autoconf
  CC_FOR_TARGET="\$(CONFIGURED_CC_FOR_TARGET)"
  CXX_FOR_TARGET="\$(CONFIGURED_CXX_FOR_TARGET)"
  RAW_CXX_FOR_TARGET="\$(CONFIGURED_CXX_FOR_TARGET)"
  DLLTOOL_FOR_TARGET="\$(CONFIGURED_DLLTOOL_FOR_TARGET)"
  GCC_FOR_TARGET="\$(CONFIGURED_GCC_FOR_TARGET)"
  GCJ_FOR_TARGET="\$(CONFIGURED_GCJ_FOR_TARGET)"
  GFORTRAN_FOR_TARGET="\$(CONFIGURED_GFORTRAN_FOR_TARGET)"
  LD_FOR_TARGET="\$(CONFIGURED_LD_FOR_TARGET)"
  NM_FOR_TARGET="\$(CONFIGURED_NM_FOR_TARGET)"
  RANLIB_FOR_TARGET="\$(CONFIGURED_RANLIB_FOR_TARGET)"
  WINDRES_FOR_TARGET="\$(CONFIGURED_WINDRES_FOR_TARGET)"  
fi

AC_SUBST([RAW_CXX_FOR_TARGET])

# Certain tools may need extra flags.
AR_FOR_TARGET=${AR_FOR_TARGET}${extra_arflags_for_target}
RANLIB_FOR_TARGET=${RANLIB_FOR_TARGET}${extra_ranlibflags_for_target}
NM_FOR_TARGET=${NM_FOR_TARGET}${extra_nmflags_for_target}

# When building target libraries, except in a Canadian cross, we use
# the same toolchain as the compiler we just built.
COMPILER_AS_FOR_TARGET='$(AS_FOR_TARGET)'
COMPILER_LD_FOR_TARGET='$(LD_FOR_TARGET)'
COMPILER_NM_FOR_TARGET='$(NM_FOR_TARGET)'
if test ${host} = ${build}; then
  case " ${configdirs} " in
    *" gcc "*)
      COMPILER_AS_FOR_TARGET='$$r/$(HOST_SUBDIR)/gcc/as'
      COMPILER_LD_FOR_TARGET='$$r/$(HOST_SUBDIR)/gcc/collect-ld'
      COMPILER_NM_FOR_TARGET='$$r/$(HOST_SUBDIR)/gcc/nm'${extra_nmflags_for_target}
      ;;
  esac
fi

AC_SUBST([COMPILER_AS_FOR_TARGET])
AC_SUBST([COMPILER_LD_FOR_TARGET])
AC_SUBST([COMPILER_NM_FOR_TARGET])

AC_MSG_CHECKING([whether to enable maintainer-specific portions of Makefiles])
AC_ARG_ENABLE([maintainer-mode],
[AS_HELP_STRING([--enable-maintainer-mode],[enable make rules and
                          dependencies not useful (and sometimes confusing)
                          to the casual installer])],
      [USE_MAINTAINER_MODE=${enableval}],
      [USE_MAINTAINER_MODE=no])
AC_MSG_RESULT([$USE_MAINTAINER_MODE])
AC_SUBST([MAINTAINER_MODE_TRUE])
AC_SUBST([MAINTAINER_MODE_FALSE])
if test "${USE_MAINTAINER_MODE}" = yes; then
  MAINTAINER_MODE_TRUE=
  MAINTAINER_MODE_FALSE='#'
else
  MAINTAINER_MODE_TRUE='#'
  MAINTAINER_MODE_FALSE=
fi	
MAINT=${MAINTAINER_MODE_TRUE}
AC_SUBST([MAINT])dnl

# ---------------------
# GCC bootstrap support
# ---------------------

# Stage specific cflags for build.
stage1_cflags="-ggdb"
case ${build} in
  vax-*-*)
    case ${GCC} in
      yes) stage1_cflags="-g -Wa,-J" ;;
      *) stage1_cflags="-g -J" ;;
    esac ;;
  powerpc-*-darwin*)
    # The spiffy cpp-precomp chokes on some legitimate constructs in GCC
    # sources; use -no-cpp-precomp to get to GNU cpp.
    # Apple's GCC has bugs in designated initializer handling, so disable
    # that too.
    stage1_cflags="-ggdb -no-cpp-precomp -DHAVE_DESIGNATED_INITIALIZERS=0"
    ;;
esac
AC_SUBST([stage1_cflags])

# Enable --enable-checking in stage1 of the compiler.
AC_ARG_ENABLE([stage1-checking],
[AS_HELP_STRING([[--enable-stage1-checking[=all]]],
		[choose additional checking for stage1 of the compiler])],
[stage1_checking=--enable-checking=${enable_stage1_checking}],
[if test "x${enable_checking}" = xno || test "x${enable_checking}" = x; then
  stage1_checking=--enable-checking=yes,types
else
  stage1_checking=--enable-checking=${enable_checking},types
fi])
AC_SUBST([stage1_checking])

# It makes debugging easier if we create as symlinks the stage directories
# gcc for stageN-gcc and stagePREV-gcc for stage(N-1). In case this is not
# possible, however, we can resort to mv.
AC_CACHE_CHECK([if symbolic links between directories work],
[gcc_cv_prog_ln_s_dir],
[if test "${LN_S}" = "ln -s" \
   && mkdir confdir.s1 \
   && ln -s confdir.s1 confdir.s2 \
   && echo timestamp1 > confdir.s1/conftest.1 \
   && cmp confdir.s1/conftest.1 confdir.s2/conftest.1 \
   && echo timestamp2 > confdir.s2/conftest.2 \
   && cmp confdir.s1/conftest.2 confdir.s1/conftest.2 \
   && rm -f confdir.s2; then
  gcc_cv_prog_ln_s_dir=yes
else
  gcc_cv_prog_ln_s_dir=no
fi
rm -rf confdir.s1 confdir.s2])

case ${gcc_cv_prog_ln_s_dir} in
  yes)
    CREATE_LINK_TO_DIR='ln -s $$1 $$2'
    UNDO_LINK_TO_DIR='rm -f $$1' ;;
  *)
    CREATE_LINK_TO_DIR='mv $$1 $$2'
    UNDO_LINK_TO_DIR='mv $$1 $$2' ;;
esac
AC_SUBST([CREATE_LINK_TO_DIR])
AC_SUBST([UNDO_LINK_TO_DIR])

# Enable -Werror in bootstrap stage2 and later.
# Change the default to "no" on release branches.
AC_ARG_ENABLE([werror],
[AS_HELP_STRING([--enable-werror],[enable -Werror in bootstrap stage2 and later])],[],
[enable_werror=yes])
case ${enable_werror} in
  yes) stage2_werror_flag="--enable-werror-always" ;;
  *) stage2_werror_flag="" ;;
esac
AC_SUBST([stage2_werror_flag])

# Enable --enable-host-shared.
AC_ARG_ENABLE([host-shared],
[AS_HELP_STRING([--enable-host-shared],
		[build host code as shared libraries])],
[host_shared=${enableval}],[host_shared=no])
AC_SUBST(host_shared)

# Specify what files to not compare during bootstrap.

compare_exclusions="gcc/cc*-checksum\$(objext) | gcc/ada/*tools/*"
case "${target}" in
  hppa*64*-*-hpux*) ;;
  hppa*-*-hpux*) compare_exclusions="gcc/cc*-checksum\$(objext) | */libgcc/lib2funcs* | gcc/ada/*tools/*" ;;
  powerpc*-ibm-aix*) compare_exclusions="gcc/cc*-checksum\$(objext) | gcc/ada/*tools/* | *libgomp*\$(objext)" ;;
esac
AC_SUBST([compare_exclusions])

AC_CONFIG_FILES([Makefile \
                 Makefile.orig])
AC_OUTPUT
