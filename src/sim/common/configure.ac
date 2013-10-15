dnl#                                               -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script.

sinclude(acinlude.m4)

AC_PREREQ([2.5])dnl#

AC_INIT([sim-common],[0.0.1],[/dev/null])
AC_CONFIG_SRCDIR([Makefile.in])
AC_CONFIG_AUX_DIR([../..])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])
AC_COPYRIGHT([GPL-2])
AC_CANONICAL_TARGET

# This is intended for use by the target specific directories, and by us.
AC_DEFUN([AC_REQUIRE_SIM_AC_COMMON],[
  AC_REQUIRE([SIM_AC_COMMON])
])
AC_REQUIRE_SIM_AC_COMMON

# Put a useful copy of CPP_FOR_TARGET in Makefile.
# This is only used to build the target values header files, which are
# shipped with distributions so that CPP_FOR_TARGET only needs to work in
# developer's trees. This value is borrowed from ../../Makefile.in.
CPP_FOR_TARGET="\` \
  if test -f \$\${rootme}/../../gcc/Makefile ; then \
    if test -f \$\${rootme}/../../\$(TARGET_SUBDIR)/newlib/Makefile ; then \
      echo \$\${rootme}/../../gcc/xgcc -B\$\${rootme}/../../gcc/ -idirafter \$\${rootme}/../../\$(TARGET_SUBDIR)/newlib/targ-include -idirafter \$(srcroot)/newlib/libc/include -nostdinc; \
    else \
      echo \$\${rootme}/../../gcc/xgcc -B\$\${rootme}/../../gcc/; \
    fi; \
  else \
    if test '\$(host_canonical)' = '\$(target_canonical)' ; then \
      echo \$(CC); \
    else \
      t='\$(program_transform_name)'; echo gcc | sed -e 's/x/x/' \$\$t; \
    fi; \
  fi\` -E"
AC_SUBST([CPP_FOR_TARGET])

# Set TARGET_SUBDIR, needed by CPP_FOR_TARGET.
if test x"${host}" = x"${target}" ; then
	TARGET_SUBDIR="."
else
	TARGET_SUBDIR=${target_alias}
fi
AC_SUBST([TARGET_SUBDIR])

# These are NOT all needed yet, but will be eventually.
AC_DEFUN([AC_REQUIRE_FUNC_MMAP],[
  AC_REQUIRE([AC_FUNC_MMAP])
  AC_CHECK_HEADERS([sys/mman.h])
])
AC_REQUIRE_FUNC_MMAP
AC_CHECK_HEADERS([sys/times.h])

AC_CONFIG_FILES([Makefile])
AC_CONFIG_COMMANDS([default],[case x$CONFIG_HEADERS in xcconfig.h:config.in) echo > stamp-h ;; esac],[])
AC_OUTPUT
