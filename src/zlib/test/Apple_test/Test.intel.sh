#!/bin/sh

test -e Build.intel.ksh && ksh ./Build.intel.ksh

mkfile 10k tmpfile
echo "#" >> tmpfile

test -x compress && ./compress tmpfile tmpfile.gz
test -x decompress && ./decompress tmpfile.gz tmpfile_new

set -ex

cmp tmpfile tmpfile_new

rm -f tmpfile*
