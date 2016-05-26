## automake Makefile for bfd documentation
## Process this file with automake to generate Makefile.in
#
#   Copyright (C) 2012-2014 Free Software Foundation, Inc.
#
# This file is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; see the file COPYING3.  If not see
# <http://www.gnu.org/licenses/>.
#

## no-dependencies is because of how we override the rule for chew.o:
AUTOMAKE_OPTIONS = 1.9 gnits -Wno-override no-dependencies subdir-objects
## "cygnus" == "foreign no-dist" (no room for all that though)
AUTOMAKE_DEPRECATED_OPTIONS = cygnus

## hacks to extend the verbosity control:
AM_V_redir = $(am__v_redir_@AM_V@)
am__v_redir_ = $(am__v_redir_@AM_DEFAULT_V@)
am__v_redir_0 = >
am__v_redir_1 = | tee

AM_V_redir_a = $(am__v_redir_a_@AM_V@)
am__v_redir_a_ = $(am__v_redir_a_@AM_DEFAULT_V@)
am__v_redir_a_0 = >>
am__v_redir_a_1 = | tee -a

AM_V_v = $(am__v_v_@AM_V@)
am__v_v_ = $(am__v_v_@AM_DEFAULT_V@)
am__v_v_0 =
am__v_v_1 = -v

docdir = .

DOCFILES = aoutx.texi archive.texi archures.texi \
	bfdt.texi cache.texi coffcode.texi \
	core.texi elf.texi elfcode.texi format.texi \
	libbfd.texi bfdwin.texi bfdio.texi \
	opncls.texi reloc.texi section.texi  \
	syms.texi targets.texi init.texi hash.texi linker.texi \
	mmo.texi \
	bfdver.texi

PROTOS = archive.p archures.p bfd.p \
	 core.p format.p \
	bfdio.p bfdwin.p \
	libbfd.p opncls.p reloc.p \
	section.p syms.p targets.p  \
	format.p core.p init.p

IPROTOS = cache.ip libbfd.ip reloc.ip init.ip archures.ip coffcode.ip

srcdir_up = $(srcdir)/..
srcdir_high = $(srcdir_up)/..

# SRCDOC, SRCPROT, SRCIPROT only used to sidestep Sun Make bug in
# interaction between VPATH and suffix rules. If you use GNU Make, perhaps
# other Makes, you do NOT need these three:
SRCDOC = $(srcdir_up)/aoutx.h $(srcdir_up)/archive.c \
	$(srcdir_up)/archures.c $(srcdir_up)/bfd.c \
	$(srcdir_up)/bfdio.c $(srcdir_up)/bfdwin.c \
	$(srcdir_up)/cache.c $(srcdir_up)/coffcode.h \
	$(srcdir_up)/corefile.c $(srcdir_up)/elf.c \
	$(srcdir_up)/elfcode.h $(srcdir_up)/format.c \
	$(srcdir_up)/libbfd.c $(srcdir_up)/opncls.c \
	$(srcdir_up)/reloc.c $(srcdir_up)/section.c \
	$(srcdir_up)/syms.c $(srcdir_up)/targets.c \
	$(srcdir_up)/hash.c $(srcdir_up)/linker.c \
	$(srcdir_up)/mmo.c

SRCPROT = $(srcdir_up)/archive.c $(srcdir_up)/archures.c \
	$(srcdir_up)/bfd.c $(srcdir_up)/coffcode.h $(srcdir_up)/corefile.c \
	$(srcdir_up)/format.c $(srcdir_up)/libbfd.c \
	$(srcdir_up)/bfdio.c $(srcdir_up)/bfdwin.c \
	$(srcdir_up)/opncls.c $(srcdir_up)/reloc.c \
	$(srcdir_up)/section.c $(srcdir_up)/syms.c \
	$(srcdir_up)/targets.c $(srcdir_up)/init.c \
	$(srcdir_up)/cache.c

SRCIPROT = $(srcdir_up)/cache.c $(srcdir_up)/libbfd.c \
	$(srcdir_up)/bfdio.c $(srcdir_up)/bfdwin.c \
	$(srcdir_up)/reloc.c $(srcdir_up)/cpu-h8300.c \
	$(srcdir_up)/cpu-i960.c $(srcdir_up)/archures.c \
	$(srcdir_up)/init.c

TEXIDIR = $(srcdir_high)/texinfo/fsf

info_TEXINFOS = bfd.texi
bfd_TEXINFOS = $(DOCFILES) bfdsumm.texi

MKDOC = chew$(EXEEXT_FOR_BUILD)
src_MKDOC = $(srcdir)/$(MKDOC)
build_MKDOC = $(builddir)/$(MKDOC)

## also build chew with automake to have automake generate rules for
## compiling C code for us, but rename it to try to avoid conflicts:
AM_CFLAGS = $(WARN_WRITE_STRINGS) $(C_ARCHFLAGS)
noinst_PROGRAMS = chew_am
chew_am_SOURCES = $(srcdir)/chew.c $(srcdir_high)/include/ansidecl.h
chew_am_CFLAGS = $(AM_CFLAGS) $(CFLAGS_FOR_BUILD) $(H_CFLAGS)
chew_am_LDFLAGS = -L$(srcdir_high)/intl -L../../intl $(LDFLAGS_FOR_BUILD) \
  $(WARN_LDFLAGS)
EXTRA_chew_am_DEPENDENCIES = Makefile $(srcdir_up)/config.h

chew_am-chew.o: chew.c $(srcdir_high)/include/ansidecl.h Makefile
	if test -e $(srcdir)/chew_am-chew.o -a -w $(srcdir); then \
	  rm -f $(AM_V_v) $(srcdir)/chew_am-chew.o; \
	fi
	$(AM_V_CC)$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(chew_am_CFLAGS) $(CFLAGS) -c -o chew_am-chew.o `test -f '$(srcdir)/chew.c' || echo '$(srcdir)/'`$(srcdir)/chew.c

## yes, I do mean to override this here, to avoid an error with libtool:
LDFLAGS =
## (automake should really shut up about it...)

PATH_TO_MOVEIFCHANGE = $(srcdir_high)/move-if-change
RUN_MOVEIFCHANGE = $(SHELL) $(PATH_TO_MOVEIFCHANGE)

AM_CPPFLAGS = -I.. -I$(srcdir)/.. -I$(srcdir_high)/include \
	-I$(srcdir_high)/intl -I../../intl

BUILD_HFILES = libbfd.h libcoff.h bfd.h
noinst_HEADERS = $(BUILD_HFILES)
BUILT_SOURCES = $(BUILD_HFILES)

$(MKDOC): $(srcdir)/chew.c $(PATH_TO_MOVEIFCHANGE)
	$(AM_V_CCLD)$(CC_FOR_BUILD) -o chew.$$$$ $(srcdir)/chew.c \
	  $(CFLAGS_FOR_BUILD) $(LDFLAGS_FOR_BUILD) $(H_CFLAGS) \
	  $(AM_CPPFLAGS); \
	$(RUN_MOVEIFCHANGE) chew.$$$$ $(MKDOC)

chew.o: chew.c $(srcdir_high)/include/ansidecl.h @MAINT@ Makefile
	$(AM_V_CC)$(CC_FOR_BUILD) -c $(AM_CPPFLAGS) $(H_CFLAGS) \
	  $(CFLAGS_FOR_BUILD) $(srcdir)/chew.c

protos: $(BUILD_HFILES)
.PHONY: protos

$(srcdir)/bfd.info bfd.dvi: $(DOCFILES) bfdsumm.texi bfd.texinfo $(MKDOC)

# We cannot replace these rules with an implicit rule, because
# makes without VPATH support could NOT find the .h files in `..'.

# We do not depend on chew directly so that we can distribute the info
# files, and permit people to rebuild them, without requiring the makeinfo
# program. If somebody tries to rebuild info, but none of the .texi files
# have changed, then nothing will be rebuilt.

aoutx.texi: $(srcdir_up)/aoutx.h $(srcdir)/doc.str @MAINT@ chew.c $(PATH_TO_MOVEIFCHANGE) $(src_MKDOC)
	if test ! -e $(MKDOC); then $(MAKE) $(MKDOC); fi
	if test $(srcdir) != "."; then \
	  if test ! -e $(src_MKDOC); then $(MAKE) $(src_MKDOC); fi; \
	fi
	$(src_MKDOC) -f $(srcdir)/doc.str <$(srcdir_up)/aoutx.h > aoutx.tmp
	$(RUN_MOVEIFCHANGE) aoutx.tmp $@
	$(AM_V_at)touch $@

archive.texi: $(srcdir_up)/archive.c $(srcdir)/doc.str @MAINT@ chew.c $(PATH_TO_MOVEIFCHANGE)
	if test ! -e $(MKDOC); then $(MAKE) $(MKDOC); fi
	if test $(srcdir) != "."; then \
	  if test ! -e $(src_MKDOC); then $(MAKE) $(src_MKDOC); fi; \
	fi
	$(src_MKDOC) -f $(srcdir)/doc.str <$(srcdir_up)/archive.c >archive.tmp
	$(RUN_MOVEIFCHANGE) archive.tmp $@
	$(AM_V_at)touch $@

archures.texi: $(srcdir_up)/archures.c $(srcdir)/doc.str @MAINT@ chew.c $(PATH_TO_MOVEIFCHANGE) $(src_MKDOC)
	if test ! -e $(MKDOC); then $(MAKE) $(MKDOC); fi
	if test $(srcdir) != "."; then \
	  if test ! -e $(src_MKDOC); then $(MAKE) $(src_MKDOC); fi; \
	fi
	$(src_MKDOC) -f $(srcdir)/doc.str < $(srcdir_up)/archures.c >archures.tmp
	$(RUN_MOVEIFCHANGE) archures.tmp $@
	$(AM_V_at)touch $@

# We use bfdt.texi, rather than bfd.texi, to avoid conflicting with
# bfd.texinfo on an 8.3 filesystem.
bfdt.texi: $(srcdir_up)/bfd.c $(srcdir)/doc.str @MAINT@ chew.c $(PATH_TO_MOVEIFCHANGE) $(src_MKDOC)
	if test ! -e $(MKDOC); then $(MAKE) $(MKDOC); fi
	if test $(srcdir) != "."; then \
	  if test ! -e $(src_MKDOC); then $(MAKE) $(src_MKDOC); fi; \
	fi
	$(src_MKDOC) -f $(srcdir)/doc.str < $(srcdir_up)/bfd.c > bfd.tmp
	$(RUN_MOVEIFCHANGE) bfd.tmp $@
	$(AM_V_at)touch $@

cache.texi: $(srcdir_up)/cache.c $(srcdir)/doc.str @MAINT@ chew.c $(PATH_TO_MOVEIFCHANGE) $(src_MKDOC)
	if test ! -e $(MKDOC); then $(MAKE) $(MKDOC); fi
	if test $(srcdir) != "."; then \
	  if test ! -e $(src_MKDOC); then $(MAKE) $(src_MKDOC); fi; \
	fi
	$(src_MKDOC) -f $(srcdir)/doc.str < $(srcdir_up)/cache.c >cache.tmp
	$(RUN_MOVEIFCHANGE) cache.tmp $@
	$(AM_V_at)touch $@

coffcode.texi: $(srcdir_up)/coffcode.h $(srcdir)/doc.str @MAINT@ chew.c $(PATH_TO_MOVEIFCHANGE) $(src_MKDOC)
	if test ! -e $(MKDOC); then $(MAKE) $(MKDOC); fi
	if test $(srcdir) != "."; then \
	  if test ! -e $(src_MKDOC); then $(MAKE) $(src_MKDOC); fi; \
	fi
	$(src_MKDOC) -f $(srcdir)/doc.str <$(srcdir_up)/coffcode.h >coffcode.tmp
	$(RUN_MOVEIFCHANGE) coffcode.tmp $@
	$(AM_V_at)touch $@

core.texi: $(srcdir_up)/corefile.c $(srcdir)/doc.str @MAINT@ chew.c $(PATH_TO_MOVEIFCHANGE) $(src_MKDOC)
	if test ! -e $(MKDOC); then $(MAKE) $(MKDOC); fi
	if test $(srcdir) != "."; then \
	  if test ! -e $(src_MKDOC); then $(MAKE) $(src_MKDOC); fi; \
	fi
	$(src_MKDOC) -f $(srcdir)/doc.str <$(srcdir_up)/corefile.c >core.tmp
	$(RUN_MOVEIFCHANGE) core.tmp $@
	$(AM_V_at)touch $@

elf.texi: $(srcdir_up)/elf.c $(srcdir)/doc.str @MAINT@ chew.c $(PATH_TO_MOVEIFCHANGE) $(src_MKDOC)
	if test ! -e $(MKDOC); then $(MAKE) $(MKDOC); fi
	if test $(srcdir) != "."; then \
	  if test ! -e $(src_MKDOC); then $(MAKE) $(src_MKDOC); fi; \
	fi
	$(src_MKDOC) -f $(srcdir)/doc.str <$(srcdir_up)/elf.c > elf.tmp
	$(RUN_MOVEIFCHANGE) elf.tmp $@
	$(AM_V_at)touch $@

elfcode.texi: $(srcdir_up)/elfcode.h $(srcdir)/doc.str @MAINT@ chew.c $(PATH_TO_MOVEIFCHANGE) $(src_MKDOC)
	if test ! -e $(MKDOC); then $(MAKE) $(MKDOC); fi
	if test $(srcdir) != "."; then \
	  if test ! -e $(src_MKDOC); then $(MAKE) $(src_MKDOC); fi; \
	fi
	$(src_MKDOC) -f $(srcdir)/doc.str <$(srcdir_up)/elfcode.h >elfcode.tmp
	$(RUN_MOVEIFCHANGE) elfcode.tmp $@
	$(AM_V_at)touch $@

mmo.texi: $(srcdir_up)/mmo.c $(srcdir)/doc.str @MAINT@ chew.c $(PATH_TO_MOVEIFCHANGE) $(src_MKDOC)
	if test ! -e $(MKDOC); then $(MAKE) $(MKDOC); fi
	if test $(srcdir) != "."; then \
	  if test ! -e $(src_MKDOC); then $(MAKE) $(src_MKDOC); fi; \
	fi
	$(src_MKDOC) -f $(srcdir)/doc.str <$(srcdir_up)/mmo.c > mmo.tmp
	$(RUN_MOVEIFCHANGE) mmo.tmp $@
	$(AM_V_at)touch $@

format.texi: $(srcdir_up)/format.c $(srcdir)/doc.str @MAINT@ chew.c $(PATH_TO_MOVEIFCHANGE) $(src_MKDOC)
	if test ! -e $(MKDOC); then $(MAKE) $(MKDOC); fi
	if test $(srcdir) != "."; then \
	  if test ! -e $(src_MKDOC); then $(MAKE) $(src_MKDOC); fi; \
	fi
	$(src_MKDOC) -f $(srcdir)/doc.str <$(srcdir_up)/format.c >format.tmp
	$(RUN_MOVEIFCHANGE) format.tmp $@
	$(AM_V_at)touch $@

libbfd.texi: $(srcdir_up)/libbfd.c $(srcdir)/doc.str @MAINT@ chew.c $(PATH_TO_MOVEIFCHANGE) $(src_MKDOC)
	if test ! -e $(MKDOC); then $(MAKE) $(MKDOC); fi
	if test $(srcdir) != "."; then \
	  if test ! -e $(src_MKDOC); then $(MAKE) $(src_MKDOC); fi; \
	fi
	$(src_MKDOC) -f $(srcdir)/doc.str < $(srcdir_up)/libbfd.c >libbfd.tmp
	$(RUN_MOVEIFCHANGE) libbfd.tmp $@
	$(AM_V_at)touch $@

bfdio.texi: $(srcdir_up)/bfdio.c $(srcdir)/doc.str @MAINT@ chew.c $(PATH_TO_MOVEIFCHANGE) $(src_MKDOC)
	if test ! -e $(MKDOC); then $(MAKE) $(MKDOC); fi
	if test $(srcdir) != "."; then \
	  if test ! -e $(src_MKDOC); then $(MAKE) $(src_MKDOC); fi; \
	fi
	$(src_MKDOC) -f $(srcdir)/doc.str < $(srcdir_up)/bfdio.c >bfdio.tmp
	$(RUN_MOVEIFCHANGE) bfdio.tmp $@
	$(AM_V_at)touch $@

bfdwin.texi: $(srcdir_up)/bfdwin.c $(srcdir)/doc.str @MAINT@ chew.c $(PATH_TO_MOVEIFCHANGE) $(src_MKDOC)
	if test ! -e $(MKDOC); then $(MAKE) $(MKDOC); fi
	if test $(srcdir) != "."; then \
	  if test ! -e $(src_MKDOC); then $(MAKE) $(src_MKDOC); fi; \
	fi
	$(src_MKDOC) -f $(srcdir)/doc.str < $(srcdir_up)/bfdwin.c >bfdwin.tmp
	$(RUN_MOVEIFCHANGE) bfdwin.tmp $@
	$(AM_V_at)touch $@

opncls.texi: $(srcdir_up)/opncls.c $(srcdir)/doc.str @MAINT@ chew.c $(PATH_TO_MOVEIFCHANGE) $(src_MKDOC)
	if test ! -e $(MKDOC); then $(MAKE) $(MKDOC); fi
	if test $(srcdir) != "."; then \
	  if test ! -e $(src_MKDOC); then $(MAKE) $(src_MKDOC); fi; \
	fi
	$(src_MKDOC) -f $(srcdir)/doc.str <$(srcdir_up)/opncls.c >opncls.tmp
	$(RUN_MOVEIFCHANGE) opncls.tmp $@
	$(AM_V_at)touch $@

reloc.texi: $(srcdir_up)/reloc.c $(srcdir)/doc.str @MAINT@ chew.c $(PATH_TO_MOVEIFCHANGE) $(src_MKDOC)
	if test ! -e $(MKDOC); then $(MAKE) $(MKDOC); fi
	if test $(srcdir) != "."; then \
	  if test ! -e $(src_MKDOC); then $(MAKE) $(src_MKDOC); fi; \
	fi
	$(src_MKDOC) -f $(srcdir)/doc.str <$(srcdir_up)/reloc.c >reloc.tmp
	$(RUN_MOVEIFCHANGE) reloc.tmp $@
	$(AM_V_at)touch $@

section.texi: $(srcdir_up)/section.c $(srcdir)/doc.str @MAINT@ chew.c $(PATH_TO_MOVEIFCHANGE) $(src_MKDOC)
	if test ! -e $(MKDOC); then $(MAKE) $(MKDOC); fi
	if test $(srcdir) != "."; then \
	  if test ! -e $(src_MKDOC); then $(MAKE) $(src_MKDOC); fi; \
	fi
	$(src_MKDOC) -f $(srcdir)/doc.str <$(srcdir_up)/section.c >section.tmp
	$(RUN_MOVEIFCHANGE) section.tmp $@
	$(AM_V_at)touch $@

syms.texi: $(srcdir_up)/syms.c $(srcdir)/doc.str @MAINT@ chew.c $(PATH_TO_MOVEIFCHANGE) $(src_MKDOC)
	if test ! -e $(MKDOC); then $(MAKE) $(MKDOC); fi
	if test $(srcdir) != "."; then \
	  if test ! -e $(src_MKDOC); then $(MAKE) $(src_MKDOC); fi; \
	fi
	$(src_MKDOC) -f $(srcdir)/doc.str <$(srcdir_up)/syms.c > syms.tmp
	$(RUN_MOVEIFCHANGE) syms.tmp $@
	$(AM_V_at)touch $@

targets.texi: $(srcdir_up)/targets.c $(srcdir)/doc.str @MAINT@ chew.c $(PATH_TO_MOVEIFCHANGE) $(src_MKDOC)
	if test ! -e $(MKDOC); then $(MAKE) $(MKDOC); fi
	if test $(srcdir) != "."; then \
	  if test ! -e $(src_MKDOC); then $(MAKE) $(src_MKDOC); fi; \
	fi
	$(src_MKDOC) -f $(srcdir)/doc.str <$(srcdir_up)/targets.c >targets.tmp
	$(RUN_MOVEIFCHANGE) targets.tmp $@
	$(AM_V_at)touch $@

init.texi: $(srcdir_up)/init.c $(srcdir)/doc.str @MAINT@ chew.c $(PATH_TO_MOVEIFCHANGE) $(src_MKDOC)
	if test ! -e $(MKDOC); then $(MAKE) $(MKDOC); fi
	if test $(srcdir) != "."; then \
	  if test ! -e $(src_MKDOC); then $(MAKE) $(src_MKDOC); fi; \
	fi
	$(src_MKDOC) -f $(srcdir)/doc.str <$(srcdir_up)/init.c > init.tmp
	$(RUN_MOVEIFCHANGE) init.tmp $@
	$(AM_V_at)touch $@

hash.texi: $(srcdir_up)/hash.c $(srcdir)/doc.str @MAINT@ chew.c $(PATH_TO_MOVEIFCHANGE) $(src_MKDOC)
	if test ! -e $(MKDOC); then $(MAKE) $(MKDOC); fi
	if test $(srcdir) != "."; then \
	  if test ! -e $(src_MKDOC); then $(MAKE) $(src_MKDOC); fi; \
	fi
	$(src_MKDOC) -f $(srcdir)/doc.str <$(srcdir_up)/hash.c > hash.tmp
	$(RUN_MOVEIFCHANGE) hash.tmp $@
	$(AM_V_at)touch $@

linker.texi: $(srcdir_up)/linker.c $(srcdir)/doc.str @MAINT@ chew.c $(PATH_TO_MOVEIFCHANGE) $(src_MKDOC)
	if test ! -e $(MKDOC); then $(MAKE) $(MKDOC); fi
	if test $(srcdir) != "."; then \
	  if test ! -e $(src_MKDOC); then $(MAKE) $(src_MKDOC); fi; \
	fi
	$(src_MKDOC) -f $(srcdir)/doc.str <$(srcdir_up)/linker.c >linker.tmp
	$(RUN_MOVEIFCHANGE) linker.tmp $@
	$(AM_V_at)touch $@

LIBBFD_H_DEP = \
	$(srcdir_up)/libbfd-in.h	\
	$(srcdir_up)/init.c		\
	$(srcdir_up)/libbfd.c		\
	$(srcdir_up)/bfdio.c		\
	$(srcdir_up)/bfdwin.c		\
	$(srcdir_up)/cache.c		\
	$(srcdir_up)/reloc.c		\
	$(srcdir_up)/archures.c		\
	$(srcdir_up)/elf.c		\
	$(srcdir)/header.sed		\
	$(srcdir)/proto.str		\
	$(MKDOC) $(src_MKDOC)

##FIXME: use libbfd-post.h?
libbfd.h: $(LIBBFD_H_DEP) @MAINT@ $(src_MKDOC) Makefile.am
	echo "$(LIBBFD_H_DEP)" | $(SED) -f $(srcdir)/header.sed \
	  $(AM_V_redir) $@
	for file in $(LIBBFD_H_DEP); do \
	  case $${file} in \
	    *-in.h) cat $${file} >> $@ ;; \
	    */header.sed) break ;; \
	    *)	echo $${file} | $(SED) -e 's,.*/,,' -e 's,^,/* Extracted from ,' \
				-e 's,$$,.  */,' $(AM_V_redir_a) $@ ; \
		$(src_MKDOC) -i -f $(srcdir)/proto.str < $${file} >>$@ ;; \
	  esac; \
	done
	echo "#ifdef __cplusplus" >> $@
	echo "}" >> $@
	echo "#endif /* __cplusplus */" >> $@
	echo "" >> $@
	echo "#endif /* __LIBBFD_H__ */" >> $@
	echo "" >> $@
	echo "/* End of libbfd.h */" >> $@

LIBCOFF_H_DEP = \
	$(srcdir_up)/libcoff-in.h	\
	$(srcdir_up)/coffcode.h		\
	$(srcdir)/header.sed		\
	$(srcdir)/proto.str		\
	$(MKDOC) $(src_MKDOC)

libcoff.h: $(LIBCOFF_H_DEP) @MAINT@ $(src_MKDOC)
	echo "$(LIBCOFF_H_DEP)" | $(SED) -f $(srcdir)/header.sed \
	  $(AM_V_redir) $@
	for file in $(LIBCOFF_H_DEP); do \
	  case $${file} in \
	    *-in.h) cat $${file} >> $@ ;; \
	    */header.sed) break ;; \
	    *)	echo $${file} | $(SED) -e 's,.*/,,' -e 's,^,/* Extracted from ,' \
				-e 's,$$,.  */,' $(AM_V_redir_a) $@ ; \
		$(src_MKDOC) -i -f $(srcdir)/proto.str < $${file} >>$@ ;; \
	  esac; \
	done

BFD_H_DEP = \
	$(srcdir_up)/bfd-in.h 		\
	$(srcdir_up)/init.c 		\
	$(srcdir_up)/opncls.c 		\
	$(srcdir_up)/libbfd.c 		\
	$(srcdir_up)/bfdio.c		\
	$(srcdir_up)/bfdwin.c		\
	$(srcdir_up)/section.c 		\
	$(srcdir_up)/cache.c 		\
	$(srcdir_up)/archures.c		\
	$(srcdir_up)/reloc.c		\
	$(srcdir_up)/syms.c		\
	$(srcdir_up)/bfd.c		\
	$(srcdir_up)/archive.c		\
	$(srcdir_up)/corefile.c		\
	$(srcdir_up)/targets.c		\
	$(srcdir_up)/format.c		\
	$(srcdir_up)/linker.c		\
	$(srcdir_up)/simple.c		\
	$(srcdir_up)/compress.c		\
	$(srcdir)/header.sed		\
	$(srcdir)/proto.str		\
	$(srcdir_up)/version.h		\
	$(MKDOC) $(src_MKDOC)

am--refresh: Makefile $(srcdir_up)/Makefile
	@pwd || echo "am--refresh"

$(srcdir_up)/Makefile: $(srcdir_up)/Makefile.in $(top_builddir)/config.status
	if test $(srcdir) != "."; then \
	  if test -e ../Makefile -a ! -e $(srcdir_up)/Makefile; then \
	    cp $(AM_V_v) ../Makefile $(srcdir_up)/Makefile; \
	  elif test -e $(srcdir_up)/config.status; then \
	    echo ' $(SHELL) $(srcdir_up)/config.status'; \
	    $(SHELL) $(srcdir_up)/config.status; \
	  fi; \
	else \
	  if test -e $(top_builddir)/config.status; then \
	    echo ' $(SHELL) $(top_builddir)/config.status'; \
	    $(SHELL) $(top_builddir)/config.status; \
	  fi; \
	fi

bfd.h: $(BFD_H_DEP) @MAINT@ $(src_MKDOC)
	echo "$(BFD_H_DEP)" | $(SED) -f $(srcdir)/header.sed \
	  $(AM_V_redir) $@
	for file in $(BFD_H_DEP); do \
	  case $${file} in \
	    *-in.h) cat $${file} >> $@ ;; \
	    */header.sed) break ;; \
	    *)	echo $${file} | $(SED) -e 's,.*/,,' -e 's,^,/* Extracted from ,' \
				-e 's,$$,.  */,' $(AM_V_redir_a) $@ ; \
		$(src_MKDOC) -f $(srcdir)/proto.str < $${file} >> $@ ;; \
	  esac; \
	done
	echo "#ifdef __cplusplus" >> $@
	echo "}" >> $@
	echo "#endif /* __cplusplus */" >> $@
	echo "#endif" >> $@

bfdver.texi: @MAINT@ $(srcdir)/Makefile.in
	@echo "creating $@"; \
	echo "@set VERSION $(VERSION)" $(AM_V_redir) bfdver.texi; \
	if [ -n "$(PKGVERSION)" ]; then \
	  echo "@set VERSION_PACKAGE $(PKGVERSION)" $(AM_V_redir_a) bfdver.texi; \
	fi; \
	echo "@set UPDATED `date '+%B %Y'`" $(AM_V_redir_a) bfdver.texi; \
	if [ -n "$(REPORT_BUGS_TEXI)" ]; then \
	  echo "@set BUGURL $(REPORT_BUGS_TEXI)" $(AM_V_redir_a) bfdver.texi; \
	fi

noinst_TEXINFOS = bfdint.texi

.PHONY: info info-am

MOSTLYCLEANFILES = $(MKDOC) *.o *.stamp

CLEANFILES = *.p *.ip *.dSYM

apple-clean:
	-rm -rf $(AM_V_v) *.dSYM || rmdir *.dSYM
	-if test -w $(srcdir); then \
	  rm -rf $(AM_V_v) $(srcdir)/*.dSYM || rmdir $(srcdir)/*.dSYM; \
	fi
.PHONY: apple-clean

clean-local: Makefile @MAINT@ apple-clean
	-if test -w $(srcdir); then rm -f $(AM_V_v) $(srcdir)/*.o; fi
.PHONY: clean-local

all-local: clean-local $(build_MKDOC) protos
.PHONY: all-local

DISTCLEANFILES = bfd.?? bfd.??? bfd.h libbfd.h libcoff.h texput.log

MAINTAINERCLEANFILES = $(DOCFILES) .DS_Store

# We want install to imply install-info as per GNU standards, despite the
# (deprecated) cygnus option, hence this (renamed) target:
install_my_texinfos: install-info

MAINTAINERCLEANFILES += bfd.info

## Automake 1.9 will only build info files in the objdir if they are
## mentioned in DISTCLEANFILES. It does NOT have to be unconditional,
## though, so we use a bogus condition.
if GENINSRC_NEVER
DISTCLEANFILES += bfd.info
endif
## end "if GENINSRC_NEVER"
