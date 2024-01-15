#!/bin/sh
### quick sanity test for the binutils.
###
### This file was written and is maintained by K. Richard Pixley,
### <rich@cygnus.com>.

### fail on errors
set -e

if test -n "${V}" && test "${V}" -ge 1; then
	set -ex
fi

### first arg is directory in which binaries to be tested reside.
case "$1" in
"") BIN=. ;;
*)  BIN="$1" ;;
esac

if test -x "${BIN}"/nm-new; then
	MY_NM="nm-new"
else
	MY_NM="nm"
fi

if test -x "${BIN}"/strip-new; then
	MY_STRIP="strip-new"
else
	MY_STRIP="strip"
fi

### size
for i in size objdump ${MY_NM} ar ${MY_STRIP} ranlib ; do
	"${BIN}"/size "${BIN}"/"${i}" > /dev/null
done

### objdump
for i in size objdump ${MY_NM} ar ${MY_STRIP} ranlib ; do
	"${BIN}"/objdump -ahifdrtxsl "${BIN}"/"${i}" > /dev/null
done

### nm
for i in size objdump ${MY_NM} ar ${MY_STRIP} ranlib ; do
	"${BIN}"/${MY_NM} "${BIN}"/"${i}" > /dev/null
done

### strip
TMPDIR=./binutils-$$
mkdir ${TMPDIR}

cp "${BIN}"/${MY_STRIP} "${TMPDIR}"/strip

for i in size objdump nm ar ranlib ; do
	cp "${BIN}"/${i} ${TMPDIR}/${i}
	"${BIN}"/${MY_STRIP} ${TMPDIR}/${i} || ${TMPDIR}/strip ${TMPDIR}/${i}
	cp -f "${BIN}"/${i} ${TMPDIR}/${i}
	${TMPDIR}/strip ${TMPDIR}/${i}
done

### ar

### ranlib

rm -rf ${TMPDIR}

exit 0
