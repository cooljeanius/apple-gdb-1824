## Makefile.am -- Backtrace Automake Makefile; turns into:
# Backtrace Makefile
# Copyright (C) 2012-2014 Free Software Foundation, Inc.

# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:

#     (1) Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer. 

#     (2) Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in
#     the documentation and/or other materials provided with the
#     distribution.  

#     (3) The name of the author may not be used to
#     endorse or promote products derived from this software without
#     specific prior written permission.

# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
# INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
# STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
# IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

ACLOCAL_AMFLAGS = -I.. -I../config

AM_CPPFLAGS = -I$(top_srcdir)/../include -I$(top_srcdir)/../libgcc \
	-I../libgcc

AM_CFLAGS = $(EXTRA_FLAGS) $(WARN_FLAGS) $(PIC_FLAG)

AUTOMAKE_OPTIONS = 1.11.6 foreign no-dist no-define no-dependencies -Wall

noinst_LTLIBRARIES = libbacktrace.la

BUILT_SOURCES = config.h backtrace-supported.h

libbacktrace_la_SOURCES = \
	backtrace.h \
	atomic.c \
	dwarf.c \
	fileline.c \
	internal.h \
	posix.c \
	print.c \
	sort.c \
	state.c

BACKTRACE_FILES = \
	backtrace.c \
	simple.c \
	nounwind.c

FORMAT_FILES = \
	elf.c \
	macho.c \
	unknown.c

VIEW_FILES = \
	read.c \
	mmapio.c

ALLOC_FILES = \
	alloc.c \
	mmap.c

EXTRA_libbacktrace_la_SOURCES = \
	$(BACKTRACE_FILES) \
	$(FORMAT_FILES) \
	$(VIEW_FILES) \
	$(ALLOC_FILES)

libbacktrace_la_LIBADD = \
	$(BACKTRACE_FILE) \
	$(FORMAT_FILE) \
	$(VIEW_FILE) \
	$(ALLOC_FILE)

builddir_LTOBJECTS = \
	$(builddir)/atomic.lo \
	$(builddir)/dwarf.lo \
	$(builddir)/fileline.lo \
	$(builddir)/posix.lo \
	$(builddir)/print.lo \
	$(builddir)/sort.lo \
	$(builddir)/state.lo

## only do this for variables that can only ever refer to a single file:
builddir_FORMAT_FILE = $(builddir)/$(FORMAT_FILE)
builddir_VIEW_FILE = $(builddir)/$(VIEW_FILE)
builddir_ALLOC_FILE = $(builddir)/$(ALLOC_FILE)
EXTRA_builddir_LTOBJECTS = \
	$(builddir_FORMAT_FILE) \
	$(builddir_VIEW_FILE) \
	$(builddir_ALLOC_FILE)

libbacktrace_la_DEPENDENCIES = $(libbacktrace_la_LIBADD)

EXTRA_libbacktrace_la_DEPENDENCIES = \
	$(builddir_LTOBJECTS) \
	$(EXTRA_builddir_LTOBJECTS)

MULTISRCTOP =
MULTIBUILDTOP =
MULTIDIRS =
MULTISUBDIR =
MULTIDO = true
MULTICLEAN = true

## extend the automake verbosity control:
AM_V_MFLAG = $(am__v_MFLAG_@AM_V@)
am__v_MFLAG_ = $(am__v_MFLAG_@AM_DEFAULT_V@)
am__v_MFLAG_0 = -s
am__v_MFLAG_1 = -w

all: $(BUILT_SOURCES) $(libbacktrace_la_OBJECTS) $(noinst_LTLIBRARIES)
all-local: Makefile $(builddir_LTOBJECTS) $(builddir)/libbacktrace.la
	$(AM_V_at)for mytarg in $(libbacktrace_la_OBJECTS) $(noinst_LTLIBRARIES); do \
	  if test ! -e $${mytarg} || test $${mytarg} -ot Makefile; then \
	    $(MAKE) $(AM_V_MFLAG) $${mytarg}; \
	  elif test "x$(AM_V_P)" != "xfalse"; then \
	    if test -r $${mytarg} && test -w /dev/stdout; then \
	      stat $${mytarg}; \
	    else \
	      echo "dunno about $${mytarg}..."; \
	    fi; \
	  else true; fi; \
	done 
default: all-local @MAINT@ $(builddir)/Makefile $(builddir)/config.h # all
	$(AM_V_at)for mydeftarg in $(builddir_LTOBJECTS) $(builddir)/libbacktrace.la; do \
	  if test ! -e $${mydeftarg} || test $${mydeftarg} -ot Makefile; then \
	    $(MAKE) $(AM_V_MFLAG) $${mydeftarg}; \
	  elif test "x$(AM_V_P)" != "xfalse"; then \
	    if test -r $${mydeftarg} && test -w /dev/stdout; then \
	      stat $${mydeftarg}; \
	    else \
	      echo "dunno about $${mydeftarg}..."; \
	    fi; \
	  else true; fi; \
	done
all-am: default all-multi @MAINT@ Makefile.am
	$(AM_V_at)for tg in $(libbacktrace_la_DEPENDENCIES) $(EXTRA_builddir_LTOBJECTS); do \
	  if test ! -e $${tg} || test $${tg} -ot Makefile; then \
	    $(MAKE) $(AM_V_MFLAG) $${tg}; \
	  elif test "x$(AM_V_P)" != "xfalse"; then \
	    if test -r $${tg} && test -w /dev/stdout; then \
	      stat $${tg}; \
	    else \
	      echo "dunno about $${tg}..."; \
	    fi; \
	  else true; fi; \
	done
install-exec-local: install-multi

# Testsuite.

# Add a test to this variable if you want it to be built.
check_PROGRAMS =

# Add a test to this variable if you want it to be run.
TESTS = $(check_PROGRAMS)

# Add a test to this variable if you want it to be built and run.
BUILDTESTS =

# Add a file to this variable if you want it to be built for testing.
check_DATA =

# Flags to use when compiling test programs.
libbacktrace_TEST_CFLAGS = $(EXTRA_FLAGS) $(WARN_FLAGS) -g

if HAVE_DSYMUTIL

%.dSYM: %
	$(DSYMUTIL) $<

else HAVE_DSYMUTIL_FALSE

%.dSYM: %
	@echo "dsymutil is missing"

endif HAVE_DSYMUTIL_FALSE

if NATIVE

btest_SOURCES = btest.c
btest_OPTFLAG = -O1
btest_CFLAGS = $(AM_CFLAGS) -ggdb $(btest_OPTFLAG) -Wno-unused-function
btest_DEPENDENCIES = backtrace.h backtrace-supported.h
btest_LDADD = libbacktrace.la
btest_LDFLAGS = $(AM_LDFLAGS) $(CFLAGS)

check_PROGRAMS += btest

if HAVE_DSYMUTIL
check_DATA += btest.dSYM
endif HAVE_DSYMUTIL

check-local: $(check_DATA)
	for thingy in $(check_DATA) libbacktrace.la; do \
	  if test -e $${thingy} && test -r $${thingy}; then \
	    echo "ok: $${thingy}"; \
	  else \
	    echo "missing: $${thingy}"; \
	  fi; \
	done

stest_SOURCES = stest.c
stest_LDADD = libbacktrace.la

check_PROGRAMS += stest

if HAVE_DSYMUTIL
check_DATA += stest.dSYM
endif HAVE_DSYMUTIL

else NATIVE_FALSE

check-local:
	@echo "check-local target requires a native build"

endif NATIVE_FALSE

# Cleaning.
DISTCLEANFILES = *~ autoscan*.log .DS_Store config_status_backup.sh
MAINTAINERCLEANFILES = configure*.scan

mostlyclean-local: mostlyclean-multi
clean-local: clean-multi
distclean-local: distclean-multi
	rm -rf autom4te.cache || rmdir autom4te.cache
	rm -rf *.dSYM || rmdir *.dSYM
maintainer-clean-local: maintainer-clean-multi
.PHONY: mostlyclean-local clean-local distclean-local
.PHONY: maintainer-clean-local

configure.scan:
	autoscan || touch $@

# We cannot use automake's automatic dependency tracking, because it
# breaks when using bootstrap-lean.  Automatic dependency tracking
# with GCC bootstrap will cause some of the objects to depend on
# header files in prev-gcc/include, e.g., stddef.h and stdarg.h.  When
# using bootstrap-lean, prev-gcc is removed after each stage.  When
# running "make install", those header files will be gone, causing the
# library to be rebuilt at install time.  That may not succeed.

# These manual dependencies do not include dependencies on unwind.h,
# even though that is part of GCC, because where to find it depends on
# whether we are being built as a host library or a target library.

INCDIR = $(top_srcdir)/../include
alloc.lo: config.h backtrace.h internal.h
backtrace.lo: config.h backtrace.h
btest.lo: (INCDIR)/filenames.h backtrace.h backtrace-supported.h
dwarf.lo: config.h $(INCDIR)/dwarf2.h $(INCDIR)/dwarf2.def \
	$(INCDIR)/filenames.h backtrace.h internal.h
elf.lo: config.h backtrace.h internal.h
fileline.lo: config.h backtrace.h internal.h
macho.lo: config.h backtrace.h internal.h
mmap.lo: config.h backtrace.h internal.h
mmapio.lo: config.h backtrace.h internal.h
nounwind.lo: config.h internal.h
posix.lo: config.h backtrace.h internal.h
print.lo: config.h backtrace.h internal.h
read.lo: config.h backtrace.h internal.h
simple.lo: config.h backtrace.h internal.h
state.lo: config.h backtrace.h backtrace-supported.h internal.h
unknown.lo: config.h backtrace.h internal.h

# GNU Make needs to see an explicit $(MAKE) variable in the command it
# runs to enable its job server during parallel builds.  Hence the
# comments below.
all-multi:
	$(MULTIDO) $(AM_MAKEFLAGS) DO=all multi-do # $(MAKE)
install-multi:
	$(MULTIDO) $(AM_MAKEFLAGS) DO=install multi-do # $(MAKE)
.PHONY: all-multi install-multi

mostlyclean-multi:
	$(MULTICLEAN) $(AM_MAKEFLAGS) DO=mostlyclean multi-clean # $(MAKE)
clean-multi:
	$(MULTICLEAN) $(AM_MAKEFLAGS) DO=clean multi-clean # $(MAKE)
distclean-multi:
	$(MULTICLEAN) $(AM_MAKEFLAGS) DO=distclean multi-clean # $(MAKE)
maintainer-clean-multi:
	$(MULTICLEAN) $(AM_MAKEFLAGS) DO=maintainer-clean multi-clean # $(MAKE)
.PHONY: mostlyclean-multi clean-multi distclean-multi
.PHONY: maintainer-clean-multi

# <https://gcc.gnu.org/bugzilla/show_bug.cgi?id=43171>:
am__depfiles_maybe = multilib-default-1
