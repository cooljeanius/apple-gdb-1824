dnl#                                               -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script.
AC_PREREQ([2.3])dnl
AC_INIT([wince],[???],[/dev/null])
AC_CONFIG_SRCDIR([cesetup.c])
AC_CONFIG_HEADERS([config.h])dnl

my_CC=${CC-cc}
AC_SUBST([my_CC])dnl
AC_PROG_CC
AC_EXEEXT

AC_PROG_INSTALL

AC_CONFIG_AUX_DIR([../..])
AC_CANONICAL_TARGET
AC_ARG_PROGRAM

if test -z "${HOST_ARCHITECTURE}" -a -n "${host_cpu}"; then
  export HOST_ARCHITECTURE="${host_cpu}"
  AC_SUBST([HOST_ARCHITECTURE])dnl
fi
if test -z "${TARGET_ARCHITECTURE}" -a -n "${target_cpu}"; then
  export TARGET_ARCHITECTURE="${target_cpu}"
  AC_SUBST([TARGET_ARCHITECTURE])dnl
  ## TODO: might also want to do likewise with the build arch...
fi

PROGS='cesetup$(EXEEXT)'
AC_SUBST([PROGS])dnl

# Checks for libraries.
AC_MSG_CHECKING([for linker flag to verify linked libraries have the correct architecture])
# Make sure this is done here:
if test "x${acl_cv_wl}" = "x"; then
  test -z "${acl_cv_wl}" && export acl_cv_wl='-Wl,'
fi
pre_saved_LDFLAGS="${LDFLAGS}"
LDFLAGS="${LDFLAGS} ${acl_cv_wl}-arch_errors_fatal"
AC_LINK_IFELSE([AC_LANG_PROGRAM([[]],[[]])],
               [LD_ARCHFLAGS="${acl_cv_wl}-arch_errors_fatal"],[])
LDFLAGS="${pre_saved_LDFLAGS}"
if test "x${LD_ARCHFLAGS}" != "x"; then
  test -n "${LD_ARCHFLAGS}"
  AC_MSG_RESULT([${LD_ARCHFLAGS}])
  AC_SUBST([LD_ARCHFLAGS])dnl
  ## that should be all we need to do with them for now...
else
  test -z "${LD_ARCHFLAGS}"
  AC_MSG_RESULT([none])
fi
# check arch when "main()" is the only function for which we check:
ARCHLESS_LDFLAGS="${LDFLAGS}"
LDFLAGS="${LDFLAGS} ${LD_ARCHFLAGS}"
AC_CHECK_LIB([c],[printf])dnl
  ## reset these:
LDFLAGS="${ARCHLESS_LDFLAGS}"

# Checks for header files.
AC_HEADER_DIRENT
AC_CHECK_HEADERS([ctype.h stdarg.h stdio.h])dnl

# Checks for typedefs, structures, and compiler characteristics.
AC_C_BIGENDIAN
AC_C_INLINE
AC_C_PROTOTYPES
AC_CHECK_SIZEOF([int])dnl
AC_CHECK_ALIGNOF([int])dnl

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_STAT
AC_CHECK_FUNCS([exit fclose fopen fprintf fputs isupper memcmp memset \
                memcpy strcasecmp strchr strncasecmp strstr tolower])dnl
AC_CHECK_DECLS_ONCE([basename])dnl

# Output.
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
