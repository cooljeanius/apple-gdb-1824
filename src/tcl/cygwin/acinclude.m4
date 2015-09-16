AC_DEFUN([AC_PROG_CC_GNU],
[AC_CACHE_CHECK([whether we are using GNU C],[ac_cv_prog_gcc_var],
[dnl# The semicolon is to pacify NeXT's syntax-checking cpp.
cat > conftest.c <<EOF
#ifdef __GNUC__
  yes;
#endif /* __GNUC__ */
EOF
AC_REQUIRE([AC_PROG_EGREP])
if AC_TRY_COMMAND([${CC-cc} -E conftest.c]) | egrep yes >/dev/null 2>&1; then
  ac_cv_prog_gcc_var=yes
else
  ac_cv_prog_gcc_var=no
fi])
])

AC_DEFUN([AC_PROG_CC_G],
[AC_CACHE_CHECK([whether ${CC-cc} accepts -g],[ac_cv_prog_cc_g_var],
  [echo 'void f(){}' > conftest.c
   if test -z "`${CC-cc} -g -c conftest.c 2>&1`"; then
     export ac_cv_prog_cc_g_var=yes
   else
     export ac_cv_prog_cc_g_var=no
   fi
   rm -rf conftest*
  ])
])
