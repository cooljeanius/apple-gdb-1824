dnl#                                               -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script.
AC_PREREQ([2.13])dnl
AC_INIT([sim],[???],[/dev/null])
AC_CONFIG_SRCDIR([Makefile.in])

AC_PROG_CC
AC_PROG_INSTALL
AC_CHECK_TOOL([AR],[ar])
AC_CHECK_TOOL([RANLIB],[ranlib],[:])

AC_CONFIG_AUX_DIR([..])
AC_CANONICAL_TARGET
AC_ARG_PROGRAM
AC_PROG_CXX
AC_SUBST([CFLAGS])
AC_SUBST([HDEFINES])
AR=${AR-ar}
AC_SUBST([AR])
if test "x$RANLIB" = "x"; then
	test -z "$RANLIB"
	AC_PROG_RANLIB
else
	test ! -z "$RANLIB" && export RANLIB
	AC_SUBST([RANLIB])
fi

# Put a plausible default for CC_FOR_BUILD in Makefile.
if test "x$cross_compiling" = "xno"; then
  CC_FOR_BUILD='$(CC)'
else
  CC_FOR_BUILD=gcc
fi
AC_SUBST([CC_FOR_BUILD])

# If a cpu ever has more than one simulator to choose from, use
# --enable-sim=... to choose.
AC_ARG_ENABLE([sim],
[AS_HELP_STRING([--enable-sim],[If a cpu ever has more than one simulator to choose from, use this flag.])],
[case "${enableval}" in
yes | no) ;;
*)	AC_MSG_ERROR([bad value ${enableval} given for --enable-sim option]) ;;
esac])

# Assume simulator can be built with cc.
# If the user passes --enable-sim built it regardless of $(CC).
only_if_gcc=no
only_if_enabled=no
extra_subdirs=common

dnl# Macro for adding sim directory to subdirectories, assuming it exists:
dnl# Usage: $1 = target subdir
AC_DEFUN([AC_SIM_TARGET_SUBDIR],[
	if test -d $1; then
		sim_target=$1
	else
		AC_MSG_WARN([the $1 target was requested but the subdirectory for it is not present])
	fi
])

# WHEN ADDING ENTRIES TO THIS MATRIX:
#  Make sure that the left side always has two dashes. Otherwise you
#  can get spurious matches. Even for unambiguous cases, do this as a
#  convention, else the table becomes a mess to understand and maintain.

case "${target}" in
  arm*-*-* | thumb*-*-*)
	AC_SIM_TARGET_SUBDIR([arm])
	extra_subdirs="${extra_subdirs} testsuite"
	;;
  strongarm*-*-*)
	AC_SIM_TARGET_SUBDIR([arm])
	extra_subdirs="${extra_subdirs} testsuite"
	;;
  xscale-*-*)
	AC_SIM_TARGET_SUBDIR([arm])
	extra_subdirs="${extra_subdirs} testsuite"
	;;
  d10v-*-*)		AC_SIM_TARGET_SUBDIR([d10v]) ;;
  h8300*-*-*)		AC_SIM_TARGET_SUBDIR([h8300]) ;;
  h8500-*-*)		AC_SIM_TARGET_SUBDIR([h8500]) ;;
  i960-*-*)		AC_SIM_TARGET_SUBDIR([i960]) ;;
  m32r-*-*)		AC_SIM_TARGET_SUBDIR([m32r]) ;;
  m68hc11-*-*|m6811-*-*) AC_SIM_TARGET_SUBDIR([m68hc11]) ;;
  mcore-*-*)		AC_SIM_TARGET_SUBDIR([mcore]) ;;
  mips*-*-*)
	# The MIPS simulator can only be compiled by gcc.
	AC_SIM_TARGET_SUBDIR([mips])
	only_if_gcc=yes
	extra_subdirs="${extra_subdirs} igen"
	;;
  mn10300*-*-*)
	# The mn10300 simulator can only be compiled by gcc.
	AC_SIM_TARGET_SUBDIR([mn10300])
	only_if_gcc=yes
	extra_subdirs="${extra_subdirs} igen"
	;;
  mn10200*-*-*)
	AC_SIM_TARGET_SUBDIR([mn10200])
	;;
  sh*-*-*)		AC_SIM_TARGET_SUBDIR([sh]) ;;
  powerpc*-*-eabi* | powerpc*-*-solaris* | powerpc*-*-sysv4* | \
  powerpc*-*-elf* | powerpc*-*-linux* | powerpc*-*-netbsd* | powerpc*-apple-darwin*)
	# The PowerPC simulator uses the GCC extension long long as well as
	# ANSI prototypes, so do NOT enable it for random host compilers
	# unless asked to.
	AC_C_PROTOTYPES
	AC_TYPE_INTMAX_T
	AC_TYPE_UINTMAX_T
	AC_TYPE_INTPTR_T
	AC_TYPE_UINTPTR_T
	AC_SIM_TARGET_SUBDIR([ppc])
	only_if_gcc=yes
	extra_subdirs="${extra_subdirs} igen"
	;;
  tic80-*-*)
	AC_SIM_TARGET_SUBDIR([tic80])
	only_if_gcc=yes
	extra_subdirs="${extra_subdirs} igen"
	;;
  v850-*-*)
	# The V850 simulator can only be compiled by gcc.
	AC_SIM_TARGET_SUBDIR([v850])
	extra_subdirs="${extra_subdirs} igen"
	only_if_gcc=yes
	;;
  v850e-*-*)
	# The V850 simulator can only be compiled by gcc.
	AC_SIM_TARGET_SUBDIR([v850])
	extra_subdirs="${extra_subdirs} igen"
	only_if_gcc=yes
	;;
  v850ea-*-*)
	# The V850 simulator can only be compiled by gcc.
	AC_SIM_TARGET_SUBDIR([v850])
	extra_subdirs="${extra_subdirs} igen"
	only_if_gcc=yes
	;;
  w65-*-*)
	AC_SIM_TARGET_SUBDIR([w65])
	# The w65 is suffering from gradual decay.
	only_if_enabled=yes
	;;
  z8k*-*-*)		AC_SIM_TARGET_SUBDIR([z8k]) ;;
  sparc64-*-*)
	only_if_gcc=yes
	sim_target=none # Do NOT build erc32 if sparc64.
	;;
  sparclite*-*-* | sparc86x*-*-*)
	# The SPARC simulator can only be compiled by gcc.
	only_if_gcc=yes
	AC_SIM_TARGET_SUBDIR([erc32])
	;;
  sparc*-*-*)
	# The SPARC simulator can only be compiled by gcc.
	only_if_gcc=yes
	# Unfortunately erc32 will NOT build on many hosts, so only enable
	# it if the user really really wants it.
	only_if_enabled=yes
	AC_SIM_TARGET_SUBDIR([erc32])
	;;
  *)
	# The "igen" subdir always successfully configures.
	# So does the "testsuite" subdir, but that is checked farther below.
	# Also the subdirs are useful to have regardless of which simulator
	# you want.
	extra_subdirs="${extra_subdirs} igen"
	sim_target=none
	;;
esac


# Is there a testsuite directory for the target?
testdir=`echo ${target} | sed -e 's/-.*-/-/'`
if test -r ${srcdir}/testsuite/${testdir}/configure ; then
  extra_subdirs="${extra_subdirs} testsuite"
fi


case "${enable_sim}" in
no)	sim_target=none ;;
yes)
	if test ${only_if_gcc} = yes ; then
		if test "${GCC}" != yes ; then
			echo "Cannot enable simulator since not compiling with GCC."
			sim_target=none
		fi
	fi
	;;
*)
	if test ${only_if_enabled} = yes ; then
		echo "The simulator was not explicitly enabled, so disabling."
		sim_target=none
	else
		if test ${only_if_gcc} = yes ; then
			if test "${GCC}" != yes ; then
				echo "The requested simulator requires gcc, but you are not using gcc, so disabling."
				sim_target=none
			fi
		fi
	fi
	;;
esac

if test x"${sim_target}" != xnone ; then
	configdirs="${extra_subdirs} ${sim_target}"
	AC_MSG_NOTICE([config subdirs are "${configdirs}"])
	AC_CONFIG_SUBDIRS([${configdirs}])
else
	AC_MSG_NOTICE([config subdirs are just "${extra_subdirs}"])
	AC_CONFIG_SUBDIRS([${extra_subdirs}])
fi

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

exit 0
