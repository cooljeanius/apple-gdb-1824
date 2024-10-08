dnl#                                               -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script.
sinclude([../aclocal.m4])

AC_PREREQ([2.69])
AC_INIT([expect-testsuite],[5.26],[libes@nist.gov])
AC_CONFIG_AUX_DIR([../..])
AC_CONFIG_SRCDIR([exp_test.c])dnl

m4_ifdef([AM_SILENT_RULES],[
AM_SILENT_RULES
])dnl

AC_PROG_SED

if test -e ./../config.cache; then
  sed -i "s|ac_cv_env_CFLAGS_value|bad_CFLAGS|g" ./../config.cache
  sed -i "s|ac_cv_prog_CC|bad_CC|g" ./../config.cache
  sed -i "s|ac_cv_prog_CPP|bad_CPP|g" ./../config.cache
  sed -i "s|ac_cv_env_build_alias|bad_build_alias|g" ./../config.cache
  sed -i "s|ac_cv_env_host_alias|bad_host_alias|g" ./../config.cache
  sed -i "s|ac_cv_env_target_alias|bad_target_alias|g" ./../config.cache
fi

# architecture stuff:
AC_CANONICAL_TARGET
if test -z "${BUILD_ARCHITECTURE}" -a -n "${build_cpu}"; then
  export BUILD_ARCHITECTURE="${build_cpu}"
  AC_SUBST([BUILD_ARCHITECTURE])
fi
if test -z "${HOST_ARCHITECTURE}" -a -n "${host_cpu}"; then
  export HOST_ARCHITECTURE="${host_cpu}"
  AC_SUBST([HOST_ARCHITECTURE])
fi
if test -z "${TARGET_ARCHITECTURE}" -a -n "${target_cpu}"; then
  export TARGET_ARCHITECTURE="${target_cpu}"
  AC_SUBST([TARGET_ARCHITECTURE])
fi

CY_AC_PATH_TCLCONFIG
CY_AC_LOAD_TCLCONFIG
MY_CC=${TCL_CC}

# Save CFLAGS and friends first
CFLAGSOLD=${CFLAGS}
CXXFLAGSOLD=${CXXFLAGS}
CPPFLAGSOLD=${CPPFLAGS}
unset CFLAGS
unset CXXFLAGS
unset CPPFLAGS
# CC might already have been checked...
if test "x${CC}" = "x"; then
  test -z "${CC}"
  AC_PROG_CC
else
  test -n "${CC}"
  # The conditional is to try to avoid breakage:
  if test "x${ac_cv_c_compiler_gnu}" != "x:"; then
    test -n "${GCC}" && echo "GCC? '${GCC}'"
  fi
  # Check preprocessor:
  AC_PROG_CPP
  # This is in a conditional because otherwise it might not work
  if test "x${CYGNUS}" != "x"; then
    test -n "${CYGNUS}" && echo "CYGNUS? '${CYGNUS}'"
    # Check to make sure compiler actually works:
    CY_AC_C_WORKS
  fi
fi
# Now reset CFLAGS and friends:
CFLAGS=${CFLAGSOLD}
CXXFLAGS=${CXXFLAGSOLD}
CPPFLAGS=${CPPFLAGSOLD}

# Checks for functions.
AC_CHECK_FUNCS([memset])dnl
 
# This is for LynxOS, which needs a flag to force true POSIX when
# building. It is weirder than that, because the flag varies depending
# how old the compiler is. So...
# -X is for the old "cc" and "gcc" (based on 1.42)
# -mposix is for the new gcc (at least 2.5.8)
# This modifies the value of ${CC} to have the POSIX flag added
# so it will configure correctly.
CY_AC_TCL_LYNX_POSIX

# Check for Tcl headers:
CY_AC_PATH_TCLH

dnl# Substitutions.
AC_SUBST([MY_CC])dnl
AC_SUBST([TCLHDIR])dnl
AC_SUBST([host])dnl

# Output.
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
