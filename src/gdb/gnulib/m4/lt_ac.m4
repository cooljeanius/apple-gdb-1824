# lt_ac.m4 serial 1                                        -*- Autoconf -*-
dnl# certain macros taken from libtool.m4 that were submitted to autoconf

m4_ifndef([AC_PROG_GO],[
############################################################
# NOTE: This macro has been submitted for inclusion into   #
#  GNU Autoconf as AC_PROG_GO.  When it is available in    #
#  a released version of Autoconf we should remove this    #
#  macro and use it instead.                               #
############################################################
m4_defun([AC_PROG_GO],
[AC_LANG_PUSH([Go])dnl
AC_ARG_VAR([GOC],[Go compiler command])dnl
AC_ARG_VAR([GOFLAGS],[Go compiler flags])dnl
_AC_ARG_VAR_LDFLAGS([])dnl
AC_CHECK_TOOL([GOC],[gccgo])
if test -z "${GOC}"; then
  if test -n "${ac_tool_prefix}"; then
    AC_CHECK_PROG([GOC],[${ac_tool_prefix}gccgo],[${ac_tool_prefix}gccgo])
  fi
fi
if test -z "${GOC}"; then
  AC_CHECK_PROG([GOC],[gccgo],[gccgo],[false])
fi
])dnl#m4_defun
])dnl#m4_ifndef

AC_PROVIDE_IFELSE([AC_PROG_GO],
  [LT_LANG(GO)],
  [m4_define([AC_PROG_GO],defn([AC_PROG_GO])[LT_LANG(GO)])])

m4_ifndef([AC_PROG_SED],[
############################################################
# NOTE: This macro has been submitted for inclusion into   #
#  GNU Autoconf as AC_PROG_SED.  When it is available in   #
#  a released version of Autoconf we should remove this    #
#  macro and use it instead.                               #
############################################################

m4_defun([AC_PROG_SED],
[AC_MSG_CHECKING([for a sed that does not truncate output])
AC_CACHE_VAL([lt_cv_path_SED],
[# Loop through the user's path and test for sed and gsed.
# Then use that list of sed's as ones to test for truncation.
as_save_IFS=${IFS}; IFS=${PATH_SEPARATOR}
for as_dir in ${PATH}
do
  IFS=${as_save_IFS}
  test -z "${as_dir}" && as_dir=.
  for lt_ac_prog in sed gsed; do
    for ac_exec_ext in '' ${ac_executable_extensions}; do
      if ${as_executable_p} "${as_dir}/${lt_ac_prog}${ac_exec_ext}"; then
        lt_ac_sed_list="${lt_ac_sed_list} ${as_dir}/${lt_ac_prog}${ac_exec_ext}"
      fi
    done
  done
done
IFS=${as_save_IFS}
lt_ac_max=0
lt_ac_count=0
# Add /usr/xpg4/bin/sed as it is typically found on Solaris
# along with /bin/sed that truncates output.
for lt_ac_sed in ${lt_ac_sed_list} /usr/xpg4/bin/sed; do
  test ! -f "${lt_ac_sed}" && continue
  cat /dev/null > conftest.in
  lt_ac_count=0
  echo ${ECHO_N} "0123456789${ECHO_C}" >conftest.in
  # Check for GNU sed and select it if it is found.
  if "${lt_ac_sed}" --version 2>&1 < /dev/null | grep 'GNU' > /dev/null; then
    lt_cv_path_SED=${lt_ac_sed}
    break
  fi
  while true; do
    cat conftest.in conftest.in >conftest.tmp
    mv conftest.tmp conftest.in
    cp conftest.in conftest.nl
    echo >>conftest.nl
    ${lt_ac_sed} -e 's/a$//' < conftest.nl >conftest.out || break
    cmp -s conftest.out conftest.nl || break
    # 10000 chars as input seems more than enough
    test 10 -lt "${lt_ac_count}" && break
    lt_ac_count=`expr ${lt_ac_count} + 1`
    if test "${lt_ac_count}" -gt "${lt_ac_max}"; then
      lt_ac_max=${lt_ac_count}
      lt_cv_path_SED=${lt_ac_sed}
    fi
  done
done
])
SED=${lt_cv_path_SED}
AC_SUBST([SED])
AC_MSG_RESULT([${SED}])
])dnl#AC_PROG_SED
])dnl#m4_ifndef

dnl# Old name:
AU_ALIAS([LT_AC_PROG_SED],[AC_PROG_SED])
dnl# aclocal-1.4 backwards compatibility:
dnl# AC_DEFUN([LT_AC_PROG_SED],[])

m4_ifndef([AC_REQUIRE_PROG_SED],[
AC_DEFUN([AC_REQUIRE_PROG_SED],[
  AC_REQUIRE([AC_PROG_SED])dnl
])dnl#AC_REQUIRE_PROG_SED
])dnl#m4_ifndef

dnl# similarly:
m4_ifndef([AC_REQUIRE_GREPS],[
AC_DEFUN([AC_REQUIRE_GREPS],[
  AC_REQUIRE([AC_PROG_GREP])dnl
  AC_REQUIRE([AC_PROG_EGREP])dnl
  AC_REQUIRE([AC_PROG_FGREP])dnl
])dnl#AC_REQUIRE_GREPS
])dnl#m4_ifndef
