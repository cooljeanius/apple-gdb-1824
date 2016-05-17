dnl# zlib/acinclude.m4                                     -*- Autoconf -*-

sinclude(../config/no-executables.m4)
sinclude(../config/warnings.m4)

dnl# as in ../bfd, make sure the libtool.m4 included here is the same
dnl# version as the one in the ./m4 directory; if they differ, it can lead
dnl# to strange build failures due to missing variables introduced in the
dnl# newer of the two versions:
sinclude(../libtool.m4)

dnl# The lines below arrange for aclocal not to bring libtool.m4
dnl# AM_PROG_LIBTOOL into aclocal.m4, while still arranging for automake
dnl# to add a definition of LIBTOOL to Makefile.in.
dnl# (is this really a good idea?)
ifelse([yes],[no],[
AC_DEFUN([AM_PROG_LIBTOOL],
         [echo "old version of libtool initialization macro called"])dnl
AC_SUBST([LIBTOOL])dnl
])dnl
