##  -*- Mode: Makefile -*-
##
## Makefile.am -- process this file with automake to produce Makefile.in
##
## Time-stamp:      "2012-01-29 15:04:34 bkorb"
## Author:          Bruce Korb <bkorb@gnu.org>
##
## This file is part of AutoGen.
## AutoGen Copyright (c) 1992-2013 by Bruce Korb - all rights reserved
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
## ---------------------------------------------------------------------

## This variable is needed to subvert automake's info rules.
## They do NOT work for generated texi files:
##
INFO_DEPS       = autogen.info
##(ignore the warning, it says above we are overriding the rule on purpose)
MIexe           = $(MAKEINFO) --no-split
MAKEINFOFLAGS   = -I$(top_srcdir)/autoopts -I../autoopts
passenv         = MAKE=$(MAKE) srcdir="$(srcdir)" SHELL="$(POSIX_SHELL)" \
	top_builddir="$(top_builddir)" top_srcdir="$(top_srcdir)"

run_mktexi      = $(passenv) $(POSIX_SHELL) $${dashx} mk-agen-texi.sh
all : $(INFO_DEPS)

BUILT_SOURCES   = autogen.texi
info_TEXINFOS   = $(BUILT_SOURCES)
EXTRA_DIST	= autogen-texi.txt auto-opts.tpl autogen-intro.texi \
    gendocs_template      gendocs.sh \
    agdoc.texi            autogen-intro.texi    \
    fdl.texi              invoke-autogen.texi   invoke-columns.texi \
    invoke-getdefs.texi   invoke-snprintfv.texi invoke-xml2ag.texi \
    libopts.texi          snprintfv.texi

# MAINTAINERCLEANFILES  = MakeDep.inc
TEXI2DVI_FLAGS  =  --texinfo='@pagesizes 9.5in,7.0in'

agdoc.texi      : # self-depends upon all executables
	$(run_mktexi) $@

dependency_for_autogen.dvi: agdoc.texi
.PHONY: dependency_for_autogen.dvi
autogen.texi    : agdoc.texi mk-agen-texi.sh

# Special rule for generating all the GNU standard formats.
#
autogen.txt : autogen.texi
	$(MIexe) --fill-column=70 --paragraph-indent=0 --no-headers \
		--output=$@ autogen.texi

clobber : maintainer-clean

.NOTPARALLEL:

gnudocs             : $(srcdir)/gendocs_template agdoc.texi
	$(run_mktexi) $@

# doc/Makefile.am ends here
