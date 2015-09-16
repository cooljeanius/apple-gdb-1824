dnl# libbacktrace/acinclude.m4                             -*- Autoconf -*-
dnl# based on snippets from the one from bfd

sinclude(../config/acx.m4)
sinclude(../config/no-executables.m4)

dnl# for libtool stuff:
sinclude(../libtool.m4)  
dnl# The lines below arrange for aclocal not to bring libtool.m4
dnl# AM_PROG_LIBTOOL into aclocal.m4, while still arranging for automake
dnl# to add a definition of LIBTOOL to Makefile.in.
dnl# (is this really a good idea?)
ifelse([yes],[no],[
AC_DEFUN([AM_PROG_LIBTOOL],
         [echo "old version of libtool initialization macro called"])dnl
AC_DEFUN([AM_DISABLE_SHARED],
         [echo "old version of libtool macro to disable shared called"])dnl
AC_SUBST([LIBTOOL])dnl
])dnl

sinclude(../gettext.m4)
ifelse([yes],[no],[
AC_DEFUN([CY_WITH_NLS],[])dnl
AC_SUBST([INTLLIBS])dnl
])dnl

