#!/bin/sh

set -ex

# disable obsoletion warnings, because of the way some macros are overriden:
autoreconf -fvi -Wall -Wno-obsolete

# need to do this separately for the files it installs, because autoreconf
# fails to do this for us, because we do not actually use automake yet:
automake --gnits --add-missing --copy --force-missing --verbose -Wall 2>/dev/null || ls config/*
# (ignore its messages to stderr, because they are irrelevant for us here)

exit 0
