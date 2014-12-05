##  -*- Mode: Makefile -*-
##
## Makefile.am -- process this file with automake to produce Makefile.in
##
## Time-stamp:      "2011-12-30 11:40:17 bkorb"
##
##  This file is part of AutoGen.
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

SUBDIRS         = test
AM_CPPFLAGS     = @INCLIST@
FSMTPL          = fsm.tpl  fsm-macro.tlib fsm-trans.tlib
TEMPLATES       = directive.tpl snarf.tpl $(FSMTPL)
DEFINITIONS     = cgi.def opts.def pseudo.def defParse.def
CLEANFILES      = invoke-autogen.texi invoke-autogen.menu autogen.1
GEN_CSRC        = opts.h directive.h expr.ini expr.h proto.h pseudo-fsm.h
STAMPS          = stamp-opts stamp-directive stamp-exprini stamp-fsm \
                  stamp-man stamp-parse
pkgdata_DATA    = $(FSMTPL)
nodist_man_MANS = autogen.1

DISTCLEANFILES  = $(STAMPS) ag.c
MAINTAINERCLEANFILES = $(GEN_CSRC)

bin_PROGRAMS    = autogen

# The list of source files with AutoGen functions
#
FUNCLIST        = \
	funcCase.c funcDef.c funcEval.c funcFor.c funcIf.c functions.c

# The list of source files with Guile expression functions defined
#
EXPRLIST        = \
	expExtract.c expFormat.c expGperf.c expGuile.c  expMake.c \
	expOutput.c  expPrint.c  expState.c expString.c agShell.c \
	$(FUNCLIST)

FSMLIST         = cgi-fsm.c defParse-fsm.c
csrc = \
    autogen.c    ag-text.c    $(FSMLIST) $(EXPRLIST)  \
    agCgi.c      agDep.c      agInit.c   agUtils.c    \
    defDirect.c  defFind.c    defLex.c   defLoad.c    \
    fmemopen.c   loadPseudo.c opts.c     scmStrings.c \
    tpLoad.c     tpParse.c    tpProcess.c

hsrc = ag-text.h autogen.h defParse-fsm.h functions.h cgi-fsm.h guile-iface.h

EXTRA_DIST = $(csrc) $(hsrc) $(DEFINITIONS) $(TEMPLATES) \
             defParse.x bootstrap.dir schemedef.scm \
             guile-iface.def guile-iface.tpl

LO_LIB          = $(top_builddir)/autoopts/libopts.la
SNV_LIB         = $(top_builddir)/snprintfv/libsnprintfv.la

nodist_autogen_SOURCES = ag.c
autogen_SOURCES = $(GEN_CSRC)
EXTRA_autogen_DEPENDENCIES = opts.h expr.h directive.h
autogen_LDADD   = $(LO_LIB) $(SNV_LIB) $(LIBGUILE_LIBS)
autogen_LDFLAGS = $(DYNAMIC_AG)
autogen_CFLAGS  = $(LIBGUILE_CFLAGS)

AM_YFLAGS = -d

ag.c : Makefile
	exec > $@ ; \
	echo '#undef   PKGDATADIR' ; \
	echo '#define  PKGDATADIR "$(pkgdatadir)"' ; \
	mk=`set -- $(MAKE) ; which $$1` ; \
	echo 'static char const zMakeProg[] = "'$$mk'";' ; \
	echo ; echo '#define  DEFINING' ; \
	echo '#include "autoopts/project.h"' ; \
	echo '#include "autoopts/ag-char-map.h"' ; \
	echo '#include "autogen.h"' ; \
	for f in $(csrc) ; do echo "#include \"$$f\"" ; done

## Funny build rules for extracting definitions.  "Makefile.in"
## should never depend on these, so they must *not* be listed in the
## "BUILT_SOURCES" list.  They are to be rebuilt on explicit request:
## make gen.  ALSO:  keep the "GEN - RULE" comments - used by bootstrap.dir
##
BOOTENV  = top_srcdir=$(top_srcdir) top_builddir=$(top_builddir) \
	srcdir=$(srcdir) AGexe=$(AGexe) GDexe=$(GDexe) CLexe=$(CLexe)

## stamp-opts GEN-RULE
stamp-opts: opts.def $(top_srcdir)/VERSION $(CLexe)
	@$(BOOTENV) $(POSIX_SHELL) $(srcdir)/bootstrap.dir $@

## stamp-directive GEN-RULE
stamp-directive: defDirect.c
	@$(BOOTENV) $(POSIX_SHELL) $(srcdir)/bootstrap.dir $@

## stamp-exprini GEN-RULE
stamp-exprini : $(EXPRLIST) snarf.tpl
	@$(BOOTENV) $(POSIX_SHELL) $(srcdir)/bootstrap.dir $@

## stamp-fsm GEN-RULE
stamp-fsm : pseudo.def $(FSMTPL)
	@$(BOOTENV) $(POSIX_SHELL) $(srcdir)/bootstrap.dir $@

## stamp-parse GEN-RULE
stamp-parse : defParse.def $(FSMTPL)
	@$(BOOTENV) $(POSIX_SHELL) $(srcdir)/bootstrap.dir $@

## stamp-ag-text GEN-RULE
stamp-ag-text : ag-text.def
	@$(BOOTENV) $(POSIX_SHELL) $(srcdir)/bootstrap.dir $@

## proto.h GEN-RULE
proto.h :
	@$(BOOTENV) $(POSIX_SHELL) $(srcdir)/bootstrap.dir $@

## stamp-texi GEN-RULE
stamp-texi : $(AGnam)
	@test -x "$(CLexe)" || { cd ../columns && $(MAKE) $(CLnam) ; }
	@test -x "$(GDexe)" || { cd ../getdefs && $(MAKE) $(GDnam) ; }
	@$(BOOTENV) $(POSIX_SHELL) $(srcdir)/bootstrap.dir $@

## stamp-man GEN-RULE
stamp-man : opts.def $(AGnam)
	@test -x "$(CLexe)" || { cd ../columns && $(MAKE) $(CLnam) ; }
	@test -x "$(GDexe)" || { cd ../getdefs && $(MAKE) $(GDnam) ; }
	@$(BOOTENV) $(POSIX_SHELL) $(srcdir)/bootstrap.dir $@

autogen.1       : stamp-man
invoke-autogen.texi    : stamp-texi
gen      : stamp-texi

../snprintfv/snprintfv.h :
	@if [ ! -f ../snprintfv/snprintfv.h ] ; then cd ../snprintfv ; \
	ln -s $(top_srcdir)/snprintfv/snprintfv/snprintfv.h . ; fi

../snprintfv/libsnprintfv.la :
	cd ../snprintfv ; $(MAKE) libsnprintfv.la

.NOTPARALLEL:

# end of agen5/Makefile.am
