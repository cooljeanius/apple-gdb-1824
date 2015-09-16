
dnl @synopsis  LIBOPTS_CHECK
dnl
dnl Time-stamp:        "2012-02-25 12:54:32 bkorb"
dnl
dnl If autoopts-config works, add the linking information to LIBS.
dnl Otherwise, add ``libopts-${ao_rev}'' to SUBDIRS and run all
dnl the config tests that the library needs.  Invoke the
dnl "INVOKE_LIBOPTS_MACROS" macro iff we are building libopts.
dnl
dnl  This file is part of AutoGen.
dnl  AutoGen Copyright (c) 1992-2012 by Bruce Korb - all rights reserved
dnl
dnl  AutoGen is free software: you can redistribute it and/or modify it
dnl  under the terms of the GNU General Public License as published by the
dnl  Free Software Foundation, either version 3 of the License, or
dnl  (at your option) any later version.
dnl
dnl  AutoGen is distributed in the hope that it will be useful, but
dnl  WITHOUT ANY WARRANTY; without even the implied warranty of
dnl  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
dnl  See the GNU General Public License for more details.
dnl
dnl  You should have received a copy of the GNU General Public License along
dnl  with this program.  If not, see <http://www.gnu.org/licenses/>.
dnl
dnl Default to system libopts
dnl
AC_DEFUN([LIBOPTS_CHECK_COMMON],[
  AC_REQUIRE([INVOKE_LIBOPTS_MACROS_FIRST])
  [NEED_LIBOPTS_DIR='']
  m4_pushdef([AO_Libopts_Dir],
	    [ifelse($1, , [libopts], [$1])])
  AC_ARG_ENABLE([local-libopts],
    AC_HELP_STRING([--enable-local-libopts],
       [Use the supplied libopts tearoff code]),[
    if test x$enableval = xyes ; then
       AC_MSG_NOTICE([Using supplied libopts tearoff])
       LIBOPTS_CFLAGS='-I$(top_srcdir)/AO_Libopts_Dir'
       NEED_LIBOPTS_DIR=true
       LIBOPTS_LDADD='$(top_builddir)/AO_Libopts_Dir/libopts.la'
    fi])

  AC_ARG_ENABLE([libopts-install],
    AC_HELP_STRING([--enable-libopts-install],
       [Install libopts with client installation]))
  AM_CONDITIONAL([INSTALL_LIBOPTS],[test "X${enable_libopts_install}" = Xyes])

  [if test -z "${NEED_LIBOPTS_DIR}" ; then]
     AC_MSG_CHECKING([whether autoopts-config can be found])
     AC_ARG_WITH([autoopts-config],
        AC_HELP_STRING([--with-autoopts-config],
             [specify the config-info script]),
        [lo_cv_with_autoopts_config=${with_autoopts_config}],
        AC_CACHE_CHECK([whether autoopts-config is specified],
             [lo_cv_with_autoopts_config],
             [if autoopts-config --help 2>/dev/null 1>&2
        then lo_cv_with_autoopts_config=autoopts-config
        elif libopts-config --help 2>/dev/null 1>&2
        then lo_cv_with_autoopts_config=libopts-config
        else lo_cv_with_autoopts_config=no ; fi])
     ) # end of AC_ARG_WITH

     AC_CACHE_VAL([lo_cv_test_autoopts],[
        if test -z "${lo_cv_with_autoopts_config}" \
                -o X"${lo_cv_with_autoopts_config}" = Xno
        then
           if autoopts-config --help 2>/dev/null 1>&2
           then lo_cv_with_autoopts_config=autoopts-config
           elif libopts-config --help 2>/dev/null 1>&2
           then lo_cv_with_autoopts_config=libopts-config
           else lo_cv_with_autoopts_config=false ; fi
        fi
        lo_cv_test_autoopts=`
            ${lo_cv_with_autoopts_config} --libs` 2> /dev/null
        if test $? -ne 0 -o -z "${lo_cv_test_autoopts}"
        then lo_cv_test_autoopts=no ; fi
     ]) # end of CACHE_VAL
     AC_MSG_RESULT([${lo_cv_test_autoopts}])

     [if test "X${lo_cv_test_autoopts}" != Xno
     then
        LIBOPTS_LDADD="${lo_cv_test_autoopts}"
        LIBOPTS_CFLAGS="`${lo_cv_with_autoopts_config} --cflags`"
     else
        LIBOPTS_LDADD='$(top_builddir)/]AO_Libopts_Dir[/libopts.la'
        LIBOPTS_CFLAGS='-I$(top_srcdir)/]AO_Libopts_Dir['
        NEED_LIBOPTS_DIR=true
     fi
  fi # end of if test -z "${NEED_LIBOPTS_DIR}"
  if test -n "${LIBOPTS_BUILD_BLOCKED}" ; then
    NEED_LIBOPTS_DIR=''
  fi]
  AM_CONDITIONAL([NEED_LIBOPTS], [test -n "${NEED_LIBOPTS_DIR}"])
  AC_SUBST(LIBOPTS_LDADD)
  AC_SUBST(LIBOPTS_CFLAGS)
  AC_SUBST(LIBOPTS_DIR, AO_Libopts_Dir)
  m4_popdef([AO_Libopts_Dir])
[# end of AC_DEFUN of LIBOPTS_CHECK_COMMON]
])
dnl
dnl AC_CONFIG_FILES conditionalization requires using AM_COND_IF, however
dnl AM_COND_IF is new to Automake 1.11.  To use it on new Automake without
dnl requiring same, a fallback implementation for older Automake is provided.
dnl Note that disabling of AC_CONFIG_FILES requires Automake 1.11, this code
dnl is correct only in terms of m4sh generated script.
dnl
m4_ifndef([AM_COND_IF],
  [AC_DEFUN([AM_COND_IF], [
    if test -z "$$1_TRUE"; then :
      m4_n([$2])[]dnl
      m4_ifval([$3],[
    else
      $3
    ])dnl
    fi[]dnl
  ])dnl
])
dnl
AC_DEFUN([LIBOPTS_CHECK_NOBUILD], [
  m4_pushdef([AO_Libopts_Dir],
	      [ifelse($1, , [libopts], [$1])])
  LIBOPTS_BUILD_BLOCKED=true
  LIBOPTS_CHECK_COMMON(AO_Libopts_Dir)
  m4_popdef([AO_Libopts_Dir])dnl
# end of AC_DEFUN of LIBOPTS_CHECK_NOBUILD
])
dnl
AC_DEFUN([LIBOPTS_CHECK], [
  m4_pushdef([AO_Libopts_Dir],
	      [ifelse($1, , [libopts], [$1])])
  LIBOPTS_BUILD_BLOCKED=''
  LIBOPTS_CHECK_COMMON(AO_Libopts_Dir)
  AM_COND_IF([NEED_LIBOPTS], [
    INVOKE_LIBOPTS_MACROS
    AC_CONFIG_FILES(AO_Libopts_Dir/Makefile)
  ])dnl
  m4_popdef([AO_Libopts_Dir])dnl
# end of AC_DEFUN of LIBOPTS_CHECK
])

