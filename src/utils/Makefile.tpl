[+ AutoGen5 template
# Makefile.tpl
# Run 'autogen Makefile.def' in the same directory to regenerate this
# Makefile.in
#
# This file is mostly an ordinary Makefile.in.
# Certain repetitive sections are generated by autogen;
# the parts with [+ and +] around them are the macros.
#
# Use .in as the suffix for the generated file:
in
# make sure we do NOT use csh:
(setenv "SHELL" "/bin/sh") 
+]

#
# Makefile.tpl, Makefile.in, Makefile
#   Copyright 2002 Free Software Foundation
#
# This file is part of GDB.
#
# This file is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

SHELL = /bin/sh

CC = @CC@
CFLAGS = @CFLAGS@

CPP = @CPP@
CPPFLAGS = @CPPFLAGS@

AR = @AR@
AS = @CCAS@

AUTOCONF = @AUTOCONF@
AUTOGEN = @AUTOGEN@

M4 = @M4@
M4_GNU = @M4_GNU@
M4_DEBUGFILE = @M4_DEBUGFILE@

prefix = @prefix@
exec_prefix = @exec_prefix@
bindir = @bindir@
libdir = @libdir@
datadir = @datadir@
datarootdir = @datarootdir@
mandir = @mandir@
infodir = @infodir@
includedir = @includedir@
srcdir = @srcdir@
abs_srcdir = @abs_srcdir@

man1dir = $(mandir)/man1
man2dir = $(mandir)/man2
man3dir = $(mandir)/man3
man4dir = $(mandir)/man4
man5dir = $(mandir)/man5
man6dir = $(mandir)/man6
man7dir = $(mandir)/man7
man8dir = $(mandir)/man8
man9dir = $(mandir)/man9
tooldir = $(exec_prefix)

subconfigure_args = @subconfigure_args@

# Should it be INSTALL = @INSTALL@ -c ?
# The old makefile passed the -c argument.
INSTALL = @INSTALL@
INSTALL_PROGRAM = $(INSTALL)
INSTALL_DATA = $(INSTALL)

## variable for automake-style verbosity control in subdirs:
V = @V@

# public targets
all: @all_subdirs@
install: @install_subdirs@

mostlyclean: @mostlyclean_subdirs@
clean: @clean_subdirs@

# These should always clean all sudirectories...
distclean: [+ FOR subdirs +]distclean-[+subdir+] [+ ENDFOR subdirs +]
maintainer-clean: [+ FOR subdirs +]maintainer-clean-[+subdir+] [+ ENDFOR subdirs +]
clean mostlyclean:
	-rm -f *~ core *.o a.out 
maintainer-clean distclean:
	-rm -rfv autom4te.cache || rmdir autom4te.cache
	-rm -f Makefile config.status *.log config.cache *~ core *.o a.out
	-find . -name .DS_Store -delete 
	-if test -n "$(srcdir)" && test -d "$(srcdir)" && test -w "$(srcdir)" && test -r "$(srcdir)"; then \
	  rm -rfv $(srcdir)/*/autom4te.cache || rmdir $(srcdir)/*/autom4te.cache; \
	fi

info:
install-info:
clean-info:
dvi:

# This should run the testsuite for each subdirectory:
check: [+ FOR subdirs +]check-[+subdir+] [+ ENDFOR subdirs +]

.PHONY: all install clean mostlyclean distclean maintainer-clean
.PHONY: info install-info clean-info dvi check
.NOEXPORT:

# Rules for regenerating this Makefile
Makefile: $(srcdir)/Makefile.in config.status
	if test -x "$(M4)"; then \
	  export M4="$(M4)"; \
	elif test -n "$(M4)"; then \
	  echo "cannot set M4=$(M4), because something is wrong with it"; \
	else \
	  echo "something is wrong with M4; check on it"; \
	fi; \
	if test -n "$${CPP}" && test "x$${CPP}" != "x$${ac_cv_env_CPP_value}"; then unset CPP; fi; \
	./config.status || (test -s $@ && touch $@)

$(srcdir)/Makefile.in: $(srcdir)/Makefile.tpl $(srcdir)/Makefile.def
	cd $(srcdir) && $(AUTOGEN) Makefile.def

config.status: $(srcdir)/configure
	if test -x "$(M4)"; then \
	  export M4="$(M4)"; \
	elif test -n "$(M4)"; then \
	  echo "cannot set M4=$(M4), because something is wrong with it"; \
	else \
	  echo "something is wrong with M4; check on it"; \
	fi; \
	if test -n "$${CPP}" && test "x$${CPP}" != "x$${ac_cv_env_CPP_value}"; then unset CPP; fi; \
	./config.status --recheck || (test -x $@ && test -s $@ && touch $@)

$(srcdir)/configure: $(srcdir)/configure.ac
	if test -x "$(M4)"; then \
	  export M4="$(M4)"; \
	elif test -n "$(M4)"; then \
	  echo "cannot set M4=$(M4), because something is wrong with it"; \
	else \
	  echo "something is wrong with M4; check on it"; \
	fi; \
	cd $(srcdir) && ($(AUTOCONF) || (test -x configure && touch configure))

## so subdirs can use automake if they want:
am--refresh:
	@:

# Miscellaneous variable setting
AR_FLAGS = qv
FLAGS_TO_PASS = \
	"CC=$(CC)" \
	"CFLAGS=$(CFLAGS)" \
	"CPP=$(CPP)" \
	"CPPFLAGS=$(CPPFLAGS)" \
	"AR=$(AR)" \
	"AR_FLAGS=$(AR_FLAGS)" \
	"AS=$(AS)" \
	"CROSS_CFLAGS=$(CROSS_CFLAGS)" \
	"TARGET_CFLAGS=$(TARGET_CFLAGS)" \
	"INCLUDES=$(INCLUDES)" \
	"V=$(V)" \
	"M4=$(M4)"

# Everything from here on down is generated by FOR loops in autogen.
[+ FOR targets +]
# Glue for [+target+].
[+ FOR subdirs +]
[+target+]-[+subdir+]: configure-[+subdir+]
	[+makeline_flag+]cd [+subdir+] && $(MAKE) [+target_flags+] [+target+]
.PHONY: [+target+]-[+subdir+]
[+ ENDFOR subdirs +][+ ENDFOR targets +]
# Glue for configure.
[+ FOR subdirs +]
configure-[+subdir+]: [+subdir+]/Makefile
[+subdir+]/Makefile: config.status
	test -d [+subdir+] || mkdir [+subdir+] ; \
	unset CPP && cd [+subdir+] && $(abs_srcdir)/[+subdir+]/configure $(subconfigure_args)/[+subdir+]
.PHONY: configure-[+subdir+]
[+ ENDFOR subdirs +]
