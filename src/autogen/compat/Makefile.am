##  -*- Mode: Makefile -*-
##
## Makefile.am -- process this file with automake to produce Makefile.in
##
## Time-stamp:      "2012-01-29 13:14:52 bkorb"
## Copyright (c)    1997-2012 by Bruce Korb
## Author:          Bruce Korb <bkorb@gnu.org>
##
## This file is part of AutoGen.
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

COMPAT_CSRC = pathfind.c snprintf.c strchr.c strdup.c strftime.c strsignal.c
COMPAT_HDRS = compat.h strsignal.h windows-config.h unlocked-io.h
COMPAT_GEN  = bootstrap.dir strsignal.def strsignal.tpl
EXTRA_DIST  = $(COMPAT_CSRC) $(COMPAT_HDRS) $(COMPAT_GEN)

MAINTAINERCLEANFILES = strsignal.h

all:
	:

strsignal.h : strsignal.def strsignal.tpl
	@if $(AGexe) --version >&- 2>&- ; then \
	  echo $(AGexe) -L $(srcdir) $(srcdir)/strsignal.def ; \
	  top_srcdir=$(top_srcdir) top_builddir=$(top_builddir) \
	    $(AGexe) -L $(srcdir) $(srcdir)/strsignal.def ; \
	elif $(AGnam) --version >&- 2>&- ; then \
	  echo $(AGnam) -L $(srcdir) $(srcdir)/strsignal.def ; \
	  top_srcdir=$(top_srcdir) top_builddir=$(top_builddir) \
	    $(AGnam) -L $(srcdir) $(srcdir)/strsignal.def ; \
	elif test -s $(srcdir)/$@; then \
	  echo "WARNING: $(AGnam) not available, current $(srcdir)/$@ used"; \
	else \
	  echo "ERROR: $(srcdir)/$@ has been corrupted"; exit 1;\
	fi

.NOTPARALLEL:

# compat/Makefile.am ends here
