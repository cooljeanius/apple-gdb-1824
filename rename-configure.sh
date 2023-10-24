#!/bin/sh

# This is a separate script because I do not know how safe
# it is for a running script to rename itself
# (or if that is even possible)
set -e

echo "=== entering $(basename "$(pwd)") ($(pwd))"

if [ -e ./configure ]; then
	echo "Actually renaming the configure script now..."
	mv -vf ./configure ./configure.sh
	echo "Remember to run autoreconf if you want to re-configure."
else
	echo "No configure script exists to rename."
	exit 1
fi

exit 0
