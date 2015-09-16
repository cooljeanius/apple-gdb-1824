AC_DEFUN([GDB_FUNC_SETPGRP],[
AC_REQUIRE([AC_CANONICAL_TARGET])dnl
AC_REQUIRE([AC_HEADER_STDC])dnl

dnl# AC_FUNC_SETPGRP does not work when cross compiling
dnl# Instead, assume we will have a prototype for setpgrp
dnl# if we are cross-compiling:
if test "x${cross_compiling}" = "xno"; then
  AC_FUNC_SETPGRP
else
  AC_CACHE_CHECK([whether setpgrp takes no argument],
                 [ac_cv_func_setpgrp_void],
    [AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <unistd.h>
     ]],[[
if (setpgrp(1, 1) == -1) {
    exit(0);
} else {
    exit(1);
}
     ]])],[ac_cv_func_setpgrp_void=no],[ac_cv_func_setpgrp_void=yes])])
  if test "x${ac_cv_func_setpgrp_void}" = "xyes"; then
    AC_DEFINE([SETPGRP_VOID],[1],
              [Define to 1 if setgrp takes no argument (i.e. void)])
  fi
fi
])dnl
