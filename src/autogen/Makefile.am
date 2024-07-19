##  -*- Mode: Makefile -*-
## Makefile.am --- process this file with automake to produce Makefile.in
##
## Time-stamp:      "2011-05-05 10:04:15 bkorb"
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

ACLOCAL_AMFLAGS = -I config --install

SUBDIRS        = compat snprintfv autoopts agen5

if DO_SHELL_CMDS
if  HAVE_XML_LIB
SUBDIRS       += columns getdefs xml2ag doc pkg
else
SUBDIRS       += columns getdefs pkg
endif
else

## Without shell commands, you cannot build docs or packages and the
## columns and getdefs commands are unworkable.
##
if  HAVE_XML_LIB
SUBDIRS       += xml2ag
else
SUBDIRS       += po
endif
endif

pkgdata_DATA   = config/conftest.tpl config/confmacs.tlib config/liboptschk.m4
misc_extra     = \
    VERSION                   NOTES \
    config/bootstrap          config/bootstrap.local \
    config/bootstrap.shlib    config/ag_macros.m4 \
    config/extensions.m4      config/libopts.m4 \
    config/liboptschk.m4      config/snprintfv.m4 \
    config/unlocked-io.m4     autoopts/parse-duration.c \
    autoopts/parse-duration.h doc/gendocs.sh \
    doc/gendocs_template
EXTRA_DIST     = $(misc_extra) $(pkgdata_DATA)
DISTCLEANFILES = stamp-h *~ config/*~ po/*~ autoscan.log config.log .DS_Store

distclean-local:
	rm -fv *.log
	rm -rfv autom4te.cache || rmdir autom4te.cache

my_distcleancheck_listfiles_local = \
     find -type f -exec 'test -f $(srcdir)/{} || echo {} ;'

configure   : VERSION

pkg         : package
package     :
	cd pkg && $(MAKE) $@ pkgtype="$(pkgtype)"

docs        : gnudocs
gnudoc      : gnudocs
gnudocs     :
	cd doc && $(MAKE) gnudocs

doxydocs    :
	test -f Doxyfile || cp ~/.Doxyfile ./Doxyfile ; \
	doxygen

usage-txt.po: gettext
gettext     :
	cd autoopts && $(MAKE) usage-txt.po

all         : config/shdefs
config/shdefs: config/mk-shdefs
	$(SHELL) $< $@

if HAVE_XML_LIB
release     : distcheck
else
release     :
	echo "Distributions cannot be made with a partial build" >&2
	exit 1
endif

.NOTPARALLEL:
.PHONY: pkg package docs gnudoc gnudocs

## Makefile.am ends here
