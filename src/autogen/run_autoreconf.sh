#!/bin/sh

set -ex

# TODO: make it possible to remove the -Wno-obsolete flag
autoreconf -fvi -Wall -Wno-obsolete

test -d autom4te.cache && rm -rf autom4te.cache
test -e config.h.in~ && rm -f config.h.in~
