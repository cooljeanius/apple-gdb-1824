# gcc-version.m4 serial 1 file starts here
dnl# This was originally in the emacs configure.ac file.   -*- Autoconf -*-

dnl# gl_GCC_VERSION_IFELSE([major],[minor],
dnl#                       [run-if-found],[run-if-not-found])
dnl# ------------------------------------------------
dnl# If ${CPP} is gcc-MAJOR.MINOR or newer, then run RUN-IF-FOUND.
dnl# Otherwise, run RUN-IF-NOT-FOUND.
AC_DEFUN([gl_GCC_VERSION_IFELSE],[
   m4_ifdef([AX_GCC_OPTION],[
     m4_ifdef([AX_GCC_VERSION],[
       AC_REQUIRE([AX_GCC_VERSION])dnl
     ])dnl
   ])dnl
   AC_PREPROC_IFELSE([AC_LANG_PROGRAM([[
#if ($1) < __GNUC__ || (($1) == __GNUC__ && ($2) <= __GNUC_MINOR__)
/* ok */
#else
# error "your version of gcc is older than $1.$2"
#endif /* GCC version check */
      ]]),
    ],[$3],[$4])dnl# (the bracketing at this end seems wrong but whatever)
])dnl

dnl#FIXME: are there still places for me to use the above more often?

# gcc-version.m4 ends here
