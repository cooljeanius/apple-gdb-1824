#!/bin/bash

set -ex

autoreconf -fvi -Wall -Wno-obsolete

if test macosx/configure -ot macosx/configure.ac && test -n "$(grep AC_INIT macosx/configure.ac)"; then
  pushd macosx
  autoreconf -fvi -Wall -Wno-obsolete || touch configure
  popd
fi

# need to do this separately for the files it installs, because autoreconf
# fails to do this for us, because we do not actually use automake yet:
automake --gnits --add-missing --copy --force-missing --verbose -Wall 2>/dev/null || pwd
# (ignore its messages to stderr, because they are irrelevant for us here)

if test -z "${PWD}"; then PWD="$(pwd)"; fi
if test -e "${PWD}"/config.cache; then
  echo "hacking ${PWD}/config.cache"
  sed -i.bak -e "s|ac_cv_env_CPP_set|bad_CPP_set|g" "${PWD}"/config.cache
  sed -i.bak -e "s|ac_cv_env_CPP_value|bad_CPP_value|g" "${PWD}"/config.cache
  sed -i.bak -e "s|ac_cv_prog_CPP|bad_CPP|g" "${PWD}"/config.cache
elif test -e ./config.cache; then
  echo "hacking ./config.cache"
  sed -i.bak -e "s|ac_cv_env_CPP_set|bad_CPP_set|g" ./config.cache
  sed -i.bak -e "s|ac_cv_env_CPP_value|bad_CPP_value|g" ./config.cache
  sed -i.bak -e "s|ac_cv_prog_CPP|bad_CPP|g" ./config.cache
else
  echo "config.cache is missing, which means we can skip attempts to hack it"
fi

(test -e config.h.in~ && rm -f config.h.in~) || (test -e config-h.in~ && rm -f config-h.in~)
(test -e configure~ && rm -f configure~) || (test -w . && rm -f config*~)
for mysubdir in cygwin macosx unix win; do
  if test -d ${mysubdir} && test -r ${mysubdir} && test -w ${mysubdir}; then
    rm -fv ${mysubdir}/*~
    mysubcache="${mysubdir}/autom4te.cache"
    if test -d ${mysubcache} && test -s ${mysubcache}; then
      rm -rf ${mysubcache} || rmdir ${mysubcache} || stat ${mysubcache}
    elif test -e ${mysubcache}; then
	  stat ${mysubcache} || du ${mysubcache}
    fi
    unset mysubcache
  fi
done

exit 0
