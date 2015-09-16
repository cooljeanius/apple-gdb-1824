##  -*- Mode: Makefile -*-
## Makefile.am --- process this file with automake to produce Makefile.in
##
## Time-stamp:      "2011-06-11 12:02:49 bkorb"
##
## This file is part of AutoGen.
## AutoGen Copyright (c) 1992-2012 by Bruce Korb - all rights reserved
##
## AutoGen is free software: you can redistribute it and/or modify it
## under the terms of the GNU General Public License as published by the
## Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## AutoGen is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
## See the GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License along
## with this program.  If not, see <http://www.gnu.org/licenses/>.

DISTCLEANFILES  = autogen.lsm
LIBOPTS_FILES   = \
    libopts/COPYING.gplv3  libopts/COPYING.lgplv3 libopts/COPYING.mbsd \
    libopts/README         libopts/libopts-add.m4 libopts/mklibsrc.sh

TPL_FILES       = lsm.tpl spec.tpl gnudir.tpl gnudoc.tpl
PKG_FILES       = mkpkg.linux mkpkg.sh mkpkg.sun pkg-env.in
EXTRA_DIST      = $(TPL_FILES) $(LIBOPTS_FILES) $(PKG_FILES)
DOCENV          = MAKE="$(MAKE)"

all : $(DISTCLEANFILES)

autogen.lsm : $(top_srcdir)/VERSION lsm.tpl
	top_builddir=$(top_builddir) top_srcdir=$(top_srcdir) \
	$(AGexe) -L$(top_srcdir)/autoopts -T $(srcdir)/lsm.tpl \
	    $(top_srcdir)/agen5/opts.def

autogen.spec : $(top_srcdir)/VERSION spec.tpl
	top_builddir=$(top_builddir) top_srcdir=$(top_srcdir) \
	$(AGexe) -T $(srcdir)/spec.tpl $(top_srcdir)/agen5/opts.def

package :
	pkgtype=$(pkgtype) \
	DESTDIR=`cd $(top_builddir) > /dev/null ; pwd`/stage-pkg \
	$(DOCENV) $(POSIX_SHELL) $(srcdir)/mkpkg.sh

pkg     : package

.NOTPARALLEL:

## pkg/Makefile.am ends here
