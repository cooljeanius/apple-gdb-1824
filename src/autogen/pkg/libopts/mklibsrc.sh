#! /bin/sh
##  -*- Mode: shell-script -*-
## mklibsrc.sh --   make the libopts tear-off library source tarball
##
## Time-stamp:      "2011-03-06 15:24:16 bkorb"
##
##  This file is part of AutoGen.
##  AutoGen Copyright (c) 1992-2012 by Bruce Korb - all rights reserved
##
##  AutoGen is free software: you can redistribute it and/or modify it
##  under the terms of the GNU General Public License as published by the
##  Free Software Foundation, either version 3 of the License, or
##  (at your option) any later version.
##
##  AutoGen is distributed in the hope that it will be useful, but
##  WITHOUT ANY WARRANTY; without even the implied warranty of
##  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
##  See the GNU General Public License for more details.
##
##  You should have received a copy of the GNU General Public License along
##  with this program.  If not, see <http://www.gnu.org/licenses/>.

set -e

top_builddir=`cd $top_builddir ; pwd`
top_srcdir=`cd $top_srcdir ; pwd`

test -x ${top_builddir}/agen5/autogen   || exit 0
test -x ${top_builddir}/columns/columns || exit 0

ao_rev=${AO_CURRENT}.${AO_REVISION}.${AO_AGE}
tag=libopts-${ao_rev}

cd ${top_builddir}/pkg
[ ! -d ${tag} ] || rm -rf ${tag}
mkdir ${tag} ${tag}/compat ${tag}/autoopts ${tag}/m4
tagd=`pwd`/${tag}

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#
#  WORKING IN SOURCE DIRECTORY
#
cd ${top_builddir}/autoopts
files=`fgrep '#include' libopts.c | \
       sed -e 's,"$,,;s,#.*",,' \
           -e '/^compat\/compat\.h$/d' `

egrep "#define HAVE_PARSE_DURATION" ${top_builddir}/config.h >/dev/null || \
  files="${files} parse-duration.c parse-duration.h"

for f in libopts.c ${files}
do
  test -f ${f} &&
    cp -f ${f} ${tagd}/${f} && continue

  test -f ${top_srcdir}/autoopts/${f} &&
    cp -f ${top_srcdir}/autoopts/${f} ${tagd}/${f} && continue

  test -f ${top_srcdir}/${f} &&
    cp -f ${top_srcdir}/${f} ${tagd}/${f} && continue
done

cp -f ${top_srcdir}/pkg/libopts/COPYING.* ${tagd}/.

cd ${top_srcdir}/compat
cp windows-config.h compat.h pathfind.c snprintf.c strdup.c strchr.c \
   ${tagd}/compat/.
#
#  END WORK IN SOURCE DIRECTORY
#
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

cd ${tagd}

cp ${top_srcdir}/config/libopts*.m4 m4/.
chmod u+w m4/libopts.m4
cat ${top_srcdir}/pkg/libopts/libopts-add.m4 >> m4/libopts.m4
test ! -f Makefile.am || rm -f Makefile.am

sed s,'\${tag}',"${tag}",g ${top_srcdir}/pkg/libopts/README > README

touch MakeDefs.inc

vers=${AO_CURRENT}:${AO_REVISION}:${AO_AGE}
exec 3> Makefile.am
cat >&3 <<- EOMakefile
	## LIBOPTS Makefile
	MAINTAINERCLEANFILES    = Makefile.in
	if INSTALL_LIBOPTS
	lib_LTLIBRARIES         = libopts.la
	else
	noinst_LTLIBRARIES      = libopts.la
	endif
	libopts_la_SOURCES      = libopts.c
	libopts_la_CPPFLAGS     = -I\$(top_srcdir)
	libopts_la_LDFLAGS      = -version-info ${AM_LDFLAGS} ${vers}
	EXTRA_DIST              = \\
	EOMakefile

find $(ls -A) -type f \
  | egrep -v '^(libopts\.c|Makefile\.am)$' \
  | ${CLexe} -I4 --spread=1 --line-sep="  \\" >&3
exec 3>&-

if gzip --version > /dev/null 2>&1
then
  gz='gzip --best'
  sfx=tar.gz
else
  gz=compress
  sfx=tar.Z
fi

cd ..
echo ! cd `pwd`
echo ! tar cvf ${tag}.${sfx} ${tag}
tar cvf - ${tag} | $gz > ${top_builddir}/autoopts/${tag}.${sfx}
rm -rf ${tag}

## Local Variables:
## mode: shell-script
## indent-tabs-mode: nil
## sh-indentation: 2
## sh-basic-offset: 2
## End:

# eval: (add-hook 'write-file-hooks 'time-stamp)
# time-stamp-start: "timestamp='"
# time-stamp-format: "%:y-%02m-%02d"
# time-stamp-end: "'"

## end of mklibsrc.sh
