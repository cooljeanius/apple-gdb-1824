#!/bin/sh -ev
if { echo "${MAKEFLAGS}" | grep -e "-r" ;}
then echo "ALERT: some tests will break with MAKEFLAGS -r !"
	echo "I will kindly remove -r for you."
	MAKEFLAGS=`echo ${MAKEFLAGS} | sed 's|-r||'`
	echo "MAKEFLAGS=${MAKEFLAGS}"
	export MAKEFLAGS
fi
cd build
make -k check || exit 2
