##  -*- Mode: Makefile -*-
##
## Makefile.am -- process this file with automake to produce Makefile.in
##
## Author:          Bruce Korb <bkorb@gnu.org>
## Time-stamp:      "2010-07-18 08:47:05 bkorb"
##
## This file is part of AutoGen.
##  AutoGen Copyright (c) 1992-2013 by Bruce Korb - all rights reserved
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

TESTS      = convert.test
EXTRA_DIST = $(TESTS)

TESTS_ENVIRONMENT = \
	X2Aexe=`\cd $(top_builddir)/xml2ag ; pwd`/xml2ag$(EXEEXT) \
	srcdir=$(srcdir) top_srcdir=$(top_srcdir)

distclean-local:
	-rm -rf testdir FAILURES

check : perm-stamp

$(TESTS) : perm-stamp

perm-stamp :
	cd $(srcdir) ; chmod +x *.test

# Makefile.am ends here

 XML2AG=
