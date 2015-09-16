#!/bin/sh

set -ex

autoreconf -fvi -Wall

# need to do this separately for the files it installs, because autoreconf
# fails to do this for us, because we do not actually use automake yet:
automake --gnits --add-missing --copy --force-missing --verbose -Wall 2>/dev/null || pwd
# (ignore its messages to stderr, because they are irrelevant for us here)

exit 0
