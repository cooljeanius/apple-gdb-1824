dnl#                                               -*- Autoconf -*-
dnl# Process this file file with autoconf to produce a configure script.
dnl# This file is a shell script fragment that supplies the information
dnl# necessary to tailor a template configure script into the configure
dnl# script appropriate for this directory.  For more information, check
dnl# any existing configure script.

AC_PREREQ([2.70])
AC_INIT
AC_CONFIG_SRCDIR([asm-source.exp])

CC=${CC-cc}
AC_SUBST([CC])
AC_CONFIG_AUX_DIR([../../..])
AC_CANONICAL_TARGET

AC_PROG_INSTALL

dnl# In default case we need to link with some file so use common.inc.
archinc=common.inc
case ${target} in
d10v-*-*) archinc=d10v.inc ;;
esac
m4_warn([obsolete],
[It is technically impossible to `autoupdate' cleanly from AC\_LINK\_FILES
to AC\_CONFIG\_LINKS.  `autoupdate' provides a functional but inelegant
update, you should probably tune the result yourself.])dnl
ac_sources="${archinc}"
ac_dests="arch.inc"
while test -n "${ac_sources}"; do
  set ${ac_dests}; ac_dest=${1}; shift; ac_dests=$*
  set ${ac_sources}; ac_source=${1}; shift; ac_sources=$*
  export ac_config_links_1="${ac_config_links_1} ${ac_dest}:${ac_source}"
done
AC_CONFIG_LINKS([${ac_config_links_1}])dnl
  ## do like ../.. does:
echo "symlinks to be made should be \"${ac_config_links1}\""

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
