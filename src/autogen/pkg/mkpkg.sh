#! /bin/sh

## mkpkg.sh --      create a native package
## Time-stamp:      "2011-10-07 16:22:09 bkorb"
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

test -f pkg-env && . pkg-env

if test -z "${pkgtype}"
then
  pkgtype=`sh ${top_srcdir}/config/config.guess | \
                sed 's,-[^-]*$,,;s,.*-,,'`
fi

if test ! -f ${srcdir}/mkpkg.${pkgtype}
then
  pkgtype=`uname -s | tr '[A-Z]' '[a-z]'`
  test -f ${srcdir}/mkpkg.${pkgtype} || {
    echo "No mkpkg script for ${pkgtype}" >&2
    exit 1
  }
fi

. ${srcdir}/mkpkg.${pkgtype}

## Local Variables:
## mode: shell-script
## indent-tabs-mode: nil
## sh-indentation: 2
## sh-basic-offset: 2
## End:
## mkpkg.sh ends here
