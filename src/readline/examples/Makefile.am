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
CFLAGS = @CFLAGS@
LOCAL_CFLAGS = @LOCAL_CFLAGS@ -DREADLINE_LIBRARY -DRL_LIBRARY_VERSION='"$(RL_LIBRARY_VERSION)"'
CPPFLAGS = @CPPFLAGS@

AM_CPPFLAGS = -I$(srcdir) -I$(top_srcdir) -I..

## architecture flags:
C_ARCHFLAGS = @C_ARCHFLAGS@

CCFLAGS = $(DEFS) $(LOCAL_CFLAGS) $(CPPFLAGS) $(INCLUDES) $(AM_CPPFLAGS) $(CFLAGS) $(C_ARCHFLAGS)
AM_LDFLAGS = -ggdb -L.. -L../.. -L../../libiberty @LDFLAGS@
LIBS = @LIBS@

READLINE_LIB = ../libreadline.a
HISTORY_LIB = ../libhistory.a
LIBIBERTY_LIB = ../../libiberty/libiberty.a

TERMCAP_LIB = @TERMCAP_LIB@

## extend the automake verbosity control:
AM_V_MFLAG = $(am__v_MFLAG_@AM_V@)
am__v_MFLAG_ = $(am__v_MFLAG_@AM_DEFAULT_V@)
am__v_MFLAG_0 = -s
am__v_MFLAG_1 = -w

.c.o:
	${RM} $@
	$(CC) $(CCFLAGS) -c $<

EXECUTABLES = fileman rltest rl rlcat rlversion histexamp 
OBJECTS = fileman.o rltest.o rl.o rlcat.o rlversion.o histexamp.o

all: $(EXECUTABLES)
everything: all rlfe

rl: rl.o $(READLINE_LIB) $(HISTORY_LIB)
	test -e ./Makefile && test -w . && $(MAKE) $(AM_V_MFLAG) ./rl.o
	$(AM_V_at)if test -e $(srcdir)/rl.o -a ! -e rl.o; then \
	  cp -v $(srcdir)/rl.o rl.o; \
	fi
	$(CC) $(CFLAGS) $(C_ARCHFLAGS) $(LDFLAGS) $(AM_LDFLAGS) -o $@ rl.o -lreadline -lhistory $(READLINE_LIB) $(TERMCAP_LIB)

rlcat: rlcat.o $(READLINE_LIB)
	test -e ./Makefile && test -w . && $(MAKE) $(AM_V_MFLAG) ./rlcat.o
	$(AM_V_at)if test -e $(srcdir)/rlcat.o -a ! -e rlcat.o; then \
	  cp -v $(srcdir)/rlcat.o rlcat.o; \
	fi
	$(CC) $(CFLAGS) $(C_ARCHFLAGS) $(LDFLAGS) $(AM_LDFLAGS) -o $@ rlcat.o -lreadline $(READLINE_LIB) $(TERMCAP_LIB)

fileman: fileman.o $(READLINE_LIB) $(HISTORY_LIB)
	test -e ./Makefile && test -w . && $(MAKE) $(AM_V_MFLAG) ./fileman.o
	$(AM_V_at)if test -e $(srcdir)/fileman.o -a ! -e fileman.o; then \
	  cp -v $(srcdir)/fileman.o fileman.o; \
	fi
	$(CC) $(CFLAGS) $(C_ARCHFLAGS) $(LDFLAGS) $(AM_LDFLAGS) -o $@ fileman.o $(LIBS) $(READLINE_LIB) $(HISTORY_LIB) $(TERMCAP_LIB)

rltest: rltest.o $(READLINE_LIB) $(HISTORY_LIB)
	test -e ./Makefile && test -w . && $(MAKE) $(AM_V_MFLAG) ./rltest.o
	$(AM_V_at)if test -e $(srcdir)/rltest.o -a ! -e rltest.o; then \
	  cp -v $(srcdir)/rltest.o rltest.o; \
	fi
	$(CC) $(CFLAGS) $(C_ARCHFLAGS) $(LDFLAGS) $(AM_LDFLAGS) -o $@ rltest.o -lreadline -lhistory $(TERMCAP_LIB)

rlversion: rlversion.o $(READLINE_LIB)
	test -e ./Makefile && test -w . && $(MAKE) $(AM_V_MFLAG) ./rlversion.o
	$(AM_V_at)if test -e $(srcdir)/rlversion.o -a ! -e rlversion.o; then \
	  cp -v $(srcdir)/rlversion.o rlversion.o; \
	fi
	$(CC) $(CFLAGS) $(C_ARCHFLAGS) $(LDFLAGS) $(AM_LDFLAGS) -o $@ rlversion.o -lreadline $(TERMCAP_LIB)

histexamp: histexamp.o $(HISTORY_LIB)
	test -e ./Makefile && test -w . && $(MAKE) $(AM_V_MFLAG) ./histexamp.o
	$(AM_V_at)if test -e $(srcdir)/histexamp.o -a ! -e histexamp.o; then \
	  cp -v $(srcdir)/histexamp.o histexamp.o; \
	fi
	$(CC) $(CFLAGS) $(C_ARCHFLAGS) $(LDFLAGS) $(AM_LDFLAGS) -o $@ histexamp.o -lhistory $(TERMCAP_LIB)

clean-local mostlyclean-local:
	$(RM) $(OBJECTS)
	$(RM) $(EXECUTABLES) *.exe
	$(RM) -rv *.dSYM || rmdir *.dSYM
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
	test -e ./Makefile && test -w . && $(MAKE) $(AM_V_MFLAG) ./rlfe.o
	$(AM_V_at)if test -e $(srcdir)/rlfe.o -a ! -e rlfe.o; then \
	  cp -v $(srcdir)/rlfe.o rlfe.o; \
	fi
	$(CC) $(CFLAGS) $(C_ARCHFLAGS) $(LDFLAGS) $(AM_LDFLAGS) -o $@ rlfe.o -lreadline -lhistory ${TERMCAP_LIB}

rlfe.o: rlfe.c

rlfe.o: $(top_srcdir)/readline.h
rlfe.o: $(top_srcdir)/history.h
