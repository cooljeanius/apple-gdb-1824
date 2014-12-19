#!/bin/sh
# for libbacktrace

echo "This script assumes that your copy of autoconf264 has suffixed its commands with \"264\" like the MacPorts autoconf264 package does."
echo "If your autoconf264 is not like this, then this script will probably fail."

set -x

export AUTOCONF=autoconf264
export AUTOHEADER=autoheader264
export AUTOM4TE=autom4te264
export AUTORECONF=autorecon264
export AUTOSCAN=autoscan264
export AUTOUPDATE=autoupdate264
export IFNAMES=ifnames264
export ACLOCAL="aclocal 2>&1" # attempt at workaround

test -x `which autoreconf264` && autoreconf264 -fvi --warnings=all
if [ $? -eq 1 -o $? -eq 63 ]; then
  test -x `which autoconf264` && autoconf264 -f --warnings=all
  test -x `which autoheader264` && autoheader264 -f --warnings=all
fi

unset AUTOCONF
unset AUTOHEADER
unset AUTOM4TE
unset AUTORECONF
unset AUTOSCAN
unset AUTOUPDATE
unset IFNAMES
unset ACLOCAL
