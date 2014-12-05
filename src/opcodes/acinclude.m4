sinclude(../bfd/acinclude.m4)

dnl# sinclude(../gettext.m4) already included in bfd/acinclude.m4
ifelse([yes],[no],[
AC_DEFUN([CY_WITH_NLS],[echo "old nls macro called"])dnl
AC_SUBST([INTLLIBS])dnl
])dnl

dnl# AM_INSTALL_LIBBFD already included in bfd/acinclude.m4
ifelse([yes],[no],[
AC_DEFUN([AM_INSTALL_LIBBFD],["echo old bfd macro called"])dnl
AC_SUBST([bfdlibdir])dnl
AC_SUBST([bfdincludedir])dnl
])dnl
