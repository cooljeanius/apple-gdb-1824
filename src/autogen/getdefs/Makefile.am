##  -*- Mode: Makefile -*-
## ---------------------------------------------------------------------
## Makefile.am -- process this file with automake to produce Makefile.in
##
## Time-stamp:      "2011-04-20 09:52:36 bkorb"
## Author:	    Bruce Korb <bkorb@gnu.org>
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
## ---------------------------------------------------------------------
TARG = getdefs

bin_PROGRAMS    = getdefs
gdsrcs          = getdefs.h proto.h getdefs.c gdemit.c gdinit.c
getdefs_SOURCES = proto.h
BUILT_SOURCES   = gd.c
nodist_getdefs_SOURCES = $(BUILT_SOURCES)

SUBDIRS         = test
EXTRA_DIST      = opts.def $(gdsrcs)
RUNAG           = $(AGexe) -L$(top_srcdir)/autoopts/tpl \
	 -L$(top_builddir)/autoopts/tpl

if AMDEP
DEP_ARG         = -MF$(DEPDIR)/opts-dep -MT$@
INF_DEP         = -b$(TARG) -MF$(DEPDIR)/info-dep -MT$@
MAN_DEP         = -b$(TARG) -MF$(DEPDIR)/man-dep  -MT$@
include $(DEPDIR)/opts-dep
include $(DEPDIR)/info-dep
include $(DEPDIR)/man-dep
else
DEP_ARG         =
INF_DEP         =
MAN_DEP         =
endif

getdefs_LDADD   = $(top_builddir)/autoopts/libopts.la
man_MANS        = $(TARG).1
DOCFILES        = $(TARG).texi $(TARG).menu $(TARG).1
DISTCLEANFILES  = $(DOCFILES) $(nodist_getdefs_SOURCES) *-stamp
AM_CPPFLAGS     = @INCLIST@
AG_ENV          = top_builddir=$(top_builddir) \
  top_srcdir=$(top_srcdir) PATH=`cd ../columns >/dev/null && pwd`:$$PATH ; \
  export top_builddir top_srcdir PATH

all : gen
gen : $(BUILT_SOURCES) $(DOCFILES)

gd.c : $(gdsrcs)
	exec > $@ ; \
	echo '#undef   PKGDATADIR' ; \
	echo '#define  PKGDATADIR "$(pkgdatadir)"' ; \
	echo ; echo '#define  DEFINING 1' ; \
	echo '#include "autoopts/project.h"' ; \
	echo '// #define AUTOOPTS_INTERNAL 1' ; \
	echo '#include "autoopts/ag-char-map.h"' ; \
	for f in opts.c $(gdsrcs) ; \
	do echo "#include \"$$f\"" ; done

$(getdefs_OBJECTS) opts.h opts.c : stamp-opts
stamp-opts : opts.def
	@test -x $(AGexe) || { cd ../agen5   && $(MAKE) $(AGnam) ; }
	@test -x $(CLexe) || { cd ../columns && $(MAKE) $(CLnam) ; }
	$(AG_ENV) ; $(RUNAG) $(DEP_ARG) $(srcdir)/opts.def

$(DOCFILES) : stamp-doc
stamp-doc :
	@test -x $(TARG)$(EXEEXT) || $(MAKE) $(TARG)$(EXEEXT)
	@test -x $(AGexe) || { cd ../agen5   && $(MAKE) $(AGnam) ; }
	@test -x $(CLexe) || { cd ../columns && $(MAKE) $(CLnam) ; }
	$(AG_ENV) ; \
	$(RUNAG) $(MAN_DEP) -Tagman-cmd $(srcdir)/opts.def ; \
	$(RUNAG) $(INF_DEP) -Tagtexi-cmd -DLEVEL=section $(srcdir)/opts.def

.NOTPARALLEL:

# getdefs/Makefile.am ends here
