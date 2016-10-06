#!/bin/sh
# for libdecnumber
# using this script in this subdir should probably be deprecated...

echo "This script is out-of-date; just run autoreconf normally instead."

exit 0

echo "This script assumes that your copy of autoconf264 has suffixed its commands with \"264\" like the MacPorts autoconf264 package does."
echo "If your autoconf264 is not like this, then this script will probably fail."

set -ex

export AUTOCONF=autoconf264
export AUTOHEADER=autoheader264
export AUTOM4TE=autom4te264
export AUTORECONF=autoreconf264
export AUTOSCAN=autoscan264
export AUTOUPDATE=autoupdate264
export IFNAMES=ifnames264

test -x `which autoreconf264` && autoreconf264 -fvi --warnings=all

unset AUTOCONF
unset AUTOHEADER
unset AUTOM4TE
unset AUTORECONF
unset AUTOSCAN
unset AUTOUPDATE
unset IFNAMES
