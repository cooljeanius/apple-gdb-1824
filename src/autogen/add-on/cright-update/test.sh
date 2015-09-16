#! /bin/sh

#  cright-update Copyright (C) 1992-2013 by Bruce Korb - all rights reserved

TMP=`mktemp -d ${TMPDIR:-/tmp}/test.XXXXXX`
trap "rm -rf $TMP 2>/dev/null" EXIT
sample=${TMP}/sample.c
save=${TMP}/save.c
sav2=${TMP}/sav2.c

ty=$(date +%Y)
ly=$(( ty - 1 ))

die()
{
    exec >&2
    echo "test failure: $1"
    shift
    test $# -gt 0 && echo "$*"
    trap "" EXIT
    echo not removing $TMP
    exit 1
}

cat > ${sample} <<- _EOF_
	/**
	 *  cright-update Copyright (C) 1992$ly by Bruce Korb - all rights reserved
	 */

	/* Copyright (C) 1990, 1993, 1996, 1997, 1998, 1999, 2000, 2002, 2003,
	 * 2004-2005, 2006, 2007, 2008,
	 * 2009-$ly by Bruce Korb -- Free Software Foundation, Inc.
	 * Contributed by John Carr (jfc@mit.edu).
	 */
	_EOF_

rm -f .crightrc

cat > ${save} <<_EOF_
/**
 *  cright-update Copyright (C) 1992$ty by Bruce Korb - all rights reserved
 */

/* Copyright (C) 1990, 1993, 1996, 1997, 1998, 1999, 2000, 2002, 2003,
 * 2004-2005, 2006, 2007, 2008, 2009-$ty by Bruce Korb -- Free Software Foundation, Inc.
 * Contributed by John Carr (jfc@mit.edu).
 */
_EOF_

./cright-update --all ${sample}
cmp -s ${TMP}/sa* || {
    df=$(diff -up ${save} ${sample})
    die "differences found:" "$df"
}

sed -i 's/2000,/2000,\
 *    /'";s/$ty/$ly/g" ${sample}

cmp -s ${TMP}/sa* && die "sed failed"
cat > .crightrc <<- _EOF_
	all
	join-years
	owner      = by Bruce Korb
	_EOF_

./cright-update ${sample}

cat > ${sav2} <<- _EOF_
	/**
	 *  cright-update Copyright (C) 1992$ty by Bruce Korb - all rights reserved
	 */

	/* Copyright (C) 1990, 1993, 1996-2000, 2002$ty
	 *  by Bruce Korb -- Free Software Foundation, Inc.
	 * Contributed by John Carr (jfc@mit.edu).
	 */
	_EOF_
cmp -s ${sav2} ${sample} || {
    df=$(diff -up ${sav2} ${sample})
    die "secondary differences found:" "$df"
}

rm -f .crightrc
exit 0
