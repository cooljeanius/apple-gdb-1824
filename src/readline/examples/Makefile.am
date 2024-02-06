#
# This is the Makefile for the readline examples subdirectory.
#
# Copyright (C) 1994 Free Software Foundation, Inc.

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111 USA.

AUTOMAKE_OPTIONS = dejagnu gnits no-dist

RL_LIBRARY_VERSION = @LIBVERSION@

SHELL = @MAKE_SHELL@
RM = rm -f

srcdir = @srcdir@
VPATH = .:@srcdir@
top_srcdir = @top_srcdir@
BUILD_DIR = .

# Support an alternate destination root directory for package building
DESTDIR =

DEFS = @DEFS@
CC = @CC@
MY_CFLAGS = @CFLAGS@
LOCAL_CFLAGS = @LOCAL_CFLAGS@ -DREADLINE_LIBRARY -DRL_LIBRARY_VERSION='"$(RL_LIBRARY_VERSION)"'
MY_CPPFLAGS = @CPPFLAGS@

AM_CPPFLAGS = -I$(srcdir) -I$(top_srcdir) -I..

## architecture flags:
C_ARCHFLAGS = @C_ARCHFLAGS@

CCFLAGS = $(DEFS) $(LOCAL_CFLAGS) $(CPPFLAGS) $(MY_CPPFLAGS) $(INCLUDES) $(AM_CPPFLAGS) $(CFLAGS) $(MY_CFLAGS) $(C_ARCHFLAGS)
AM_CFLAGS = $(CCFLAGS)
AM_LDFLAGS = -ggdb -L.. -L../.. -L../../libiberty @LDFLAGS@
LIBS = @LIBS@

READLINE_LIB = ../libreadline.a
HISTORY_LIB = ../libhistory.a
LIBIBERTY_LIB = ../../libiberty/libiberty.a

TERMCAP_LIB = @TERMCAP_LIB@

## extend the automake verbosity control:
AM_V_v = $(am__v_v_@AM_V@)
am__v_v_ = $(am__v_v_@AM_DEFAULT_V@)
am__v_v_0 =
am__v_v_1 = -v

AM_V_MFLAG = $(am__v_MFLAG_@AM_V@)
am__v_MFLAG_ = $(am__v_MFLAG_@AM_DEFAULT_V@)
am__v_MFLAG_0 = -s
am__v_MFLAG_1 = -w

@ifGNUmake@ifneq (.@OBJEXT@,.o)
@ifGNUmake@.c.@OBJEXT@:
@ifGNUmake@	$(AM_V_at)${RM} $@
@ifGNUmake@	$(AM_V_CC)$(CC) $(CCFLAGS) -c $<
@ifGNUmake@endif

EXECUTABLES = fileman rltest rl rlcat rlversion histexamp 
OBJECTS = fileman.o rltest.o rl.o rlcat.o rlversion.o histexamp.o

check_PROGRAMS = fileman_am rltest_am rl_am rlcat_am rlversion_am histexamp_am

fileman_am_SOURCES = fileman.c
fileman_am_CFLAGS = $(AM_CFLAGS) @MAINT@ $(GTOGGLE) $(GCC_LINT_FLAGS)
fileman_am_LDADD = $(READLINE_LIB) $(HISTORY_LIB) $(TERMCAP_LIB)
fileman_am_DEPENDENCIES = fileman.o $(READLINE_LIB) $(HISTORY_LIB)

rltest_am_SOURCES = rltest.c
rltest_am_CFLAGS = $(AM_CFLAGS) @MAINT@ $(GTOGGLE) $(GCC_LINT_FLAGS)
rltest_am_LDADD = $(READLINE_LIB) $(HISTORY_LIB) $(TERMCAP_LIB)
rltest_am_DEPENDENCIES = rltest.o $(READLINE_LIB) $(HISTORY_LIB)

rl_am_SOURCES = rl.c
rl_am_CFLAGS = $(AM_CFLAGS) @MAINT@ $(GTOGGLE) $(GCC_LINT_FLAGS)
rl_am_LDADD = $(READLINE_LIB) $(HISTORY_LIB) $(TERMCAP_LIB)
rl_am_DEPENDENCIES = rl.o $(READLINE_LIB) $(HISTORY_LIB)

rlcat_am_SOURCES = rlcat.c
rlcat_am_CFLAGS = $(AM_CFLAGS) @MAINT@ $(GTOGGLE) $(GCC_LINT_FLAGS)
rlcat_am_LDADD = $(READLINE_LIB) $(TERMCAP_LIB)
rlcat_am_DEPENDENCIES = rlcat.o $(READLINE_LIB)

rlversion_am_SOURCES = rlversion.c
rlversion_am_CFLAGS = $(AM_CFLAGS) @MAINT@ $(GTOGGLE) $(GCC_LINT_FLAGS)
rlversion_am_LDADD = $(READLINE_LIB) $(TERMCAP_LIB)
rlversion_am_DEPENDENCIES = rlversion.o $(READLINE_LIB)

histexamp_am_SOURCES = histexamp.c
histexamp_am_CFLAGS = $(AM_CFLAGS) @MAINT@ $(GTOGGLE) $(GCC_LINT_FLAGS)
histexamp_am_LDADD = $(HISTORY_LIB) $(TERMCAP_LIB)
histexamp_am_DEPENDENCIES = histexamp.o $(HISTORY_LIB)

all: $(EXECUTABLES)
everything: all rlfe

$(READLINE_LIB): ../Makefile
	$(MAKE) -C .. $(AM_V_MFLAG) libreadline.a
$(HISTORY_LIB): ../Makefile
	$(MAKE) -C .. $(AM_V_MFLAG) libhistory.a

rl$(EXEEXT): rl.o $(READLINE_LIB) $(HISTORY_LIB)
	$(AM_V_at)test -e ./Makefile && test -w . && $(MAKE) $(AM_V_MFLAG) ./rl.o
	$(AM_V_at)if test -e $(srcdir)/rl.o -a ! -e rl.o; then \
	  cp $(AM_V_v) $(srcdir)/rl.o rl.o; \
	fi
	$(AM_V_CCLD)$(CC) $(CFLAGS) $(C_ARCHFLAGS) $(LDFLAGS) $(AM_LDFLAGS) -o $@ rl.o -lreadline -lhistory $(READLINE_LIB) $(TERMCAP_LIB)

rlcat$(EXEEXT): rlcat.o $(READLINE_LIB)
	$(AM_V_at)test -e ./Makefile && test -w . && $(MAKE) $(AM_V_MFLAG) ./rlcat.o
	$(AM_V_at)if test -e $(srcdir)/rlcat.o -a ! -e rlcat.o; then \
	  cp $(AM_V_v) $(srcdir)/rlcat.o rlcat.o; \
	fi
	$(AM_V_CCLD)$(CC) $(CFLAGS) $(C_ARCHFLAGS) $(LDFLAGS) $(AM_LDFLAGS) -o $@ rlcat.o -lreadline $(READLINE_LIB) $(TERMCAP_LIB)

fileman$(EXEEXT): fileman.o $(READLINE_LIB) $(HISTORY_LIB)
	$(AM_V_at)test -e ./Makefile && test -w . && $(MAKE) $(AM_V_MFLAG) ./fileman.o
	$(AM_V_at)if test -e $(srcdir)/fileman.o -a ! -e fileman.o; then \
	  cp $(AM_V_v) $(srcdir)/fileman.o fileman.o; \
	fi
	$(AM_V_CCLD)$(CC) $(CFLAGS) $(C_ARCHFLAGS) $(LDFLAGS) $(AM_LDFLAGS) -o $@ fileman.o $(LIBS) $(READLINE_LIB) $(HISTORY_LIB) $(TERMCAP_LIB)

rltest$(EXEEXT): rltest.o $(READLINE_LIB) $(HISTORY_LIB)
	$(AM_V_at)test -e ./Makefile && test -w . && $(MAKE) $(AM_V_MFLAG) ./rltest.o
	$(AM_V_at)if test -e $(srcdir)/rltest.o -a ! -e rltest.o; then \
	  cp $(AM_V_v) $(srcdir)/rltest.o rltest.o; \
	fi
	$(AM_V_CCLD)$(CC) $(CFLAGS) $(C_ARCHFLAGS) $(LDFLAGS) $(AM_LDFLAGS) -o $@ rltest.o -lreadline -lhistory $(TERMCAP_LIB)

rlversion$(EXEEXT): rlversion.o $(READLINE_LIB)
	$(AM_V_at)test -e ./Makefile && test -w . && $(MAKE) $(AM_V_MFLAG) ./rlversion.o
	$(AM_V_at)if test -e $(srcdir)/rlversion.o -a ! -e rlversion.o; then \
	  cp $(AM_V_v) $(srcdir)/rlversion.o rlversion.o; \
	fi
	$(AM_V_CCLD)$(CC) $(CFLAGS) $(C_ARCHFLAGS) $(LDFLAGS) $(AM_LDFLAGS) -o $@ rlversion.o -lreadline $(TERMCAP_LIB)

histexamp$(EXEEXT): histexamp.o $(HISTORY_LIB)
	$(AM_V_at)test -e ./Makefile && test -w . && $(MAKE) $(AM_V_MFLAG) ./histexamp.o
	$(AM_V_at)if test -e $(srcdir)/histexamp.o -a ! -e histexamp.o; then \
	  cp $(AM_V_v) $(srcdir)/histexamp.o histexamp.o; \
	fi
	$(AM_V_CCLD)$(CC) $(CFLAGS) $(C_ARCHFLAGS) $(LDFLAGS) $(AM_LDFLAGS) -o $@ histexamp.o -lhistory $(TERMCAP_LIB)

clean-local mostlyclean-local:
	$(RM) $(OBJECTS)
	$(RM) $(EXECUTABLES) *.exe
	$(RM) -r $(AM_V_v) *.dSYM || rmdir *.dSYM
	$(RM) rlfe.o rlfe

distclean-local: clean-local
	if test -e Makefile_orig; then $(RM) Makefile_orig; fi

maintainer-clean-local: distclean-local
	if test -e Makefile.in; then $(RM) Makefile.in; \
	elif test -e Makefile; then $(RM) Makefile; fi

fileman.o: fileman.c
rltest.o: rltest.c
rl.o: rl.c
rlversion.o: rlversion.c
histexamp.o: histexamp.c

fileman.o: $(top_srcdir)/readline.h
rltest.o: $(top_srcdir)/readline.h
rl.o: $(top_srcdir)/readline.h
rlversion.o: $(top_srcdir)/readline.h
histexamp.o: $(top_srcdir)/history.h

# Stuff for Per Bothner's `rlfe' program
 
rlfe: rlfe.o $(READLINE_LIB) $(HISTORY_LIB)
	$(AM_V_at)test -e ./Makefile && test -w . && $(MAKE) $(AM_V_MFLAG) ./rlfe.o
	$(AM_V_at)if test -e $(srcdir)/rlfe.o -a ! -e rlfe.o; then \
	  cp $(AM_V_v) $(srcdir)/rlfe.o rlfe.o; \
	fi
	$(AM_V_CCLD)$(CC) $(CFLAGS) $(C_ARCHFLAGS) $(LDFLAGS) $(AM_LDFLAGS) -o $@ rlfe.o -lreadline -lhistory ${TERMCAP_LIB}

rlfe.o: rlfe.c

rlfe.o: $(top_srcdir)/readline.h
rlfe.o: $(top_srcdir)/history.h
