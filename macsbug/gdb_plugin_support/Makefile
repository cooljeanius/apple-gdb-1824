################################################################
#                                                              #
#                           Makefile                           #
#                                                              #
#         Makefile to build the gdb plugin support API         #
#                                                              #
#                         Ira L. Ruben                         #
#          Copyright Apple Computer, Inc. 2000-2005            #
#                                                              #
################################################################

# This Makefile can be used explicitly with make, or it can be used to
# install the plugin API with buildit.

# When used explicitly with make the following targets are available:

# all		Build the entire MacsBug plugin tree and its gdb plugin
#               API.
# clean		Clean both the MacsBug and API objects and plugin.

# The following defines may be set on the make line:

# SRCROOT	Which directory the API sources are in (default is .).

# SYMROOT	Target directory to contain API LIBRARY
#               (gdb_plugin_support.o, default is .).

# OBJROOT	Directory to contain the Objects/ directory, i.e.,
#               where .o's go, default is ./Objects).

# When this Makefile is used with buildit BUILDIT_DIR should be set to
# where buildit is to create its output. Buildit expects and uses the
# following targets:

# installsrc	Installs all the source and additional files for the build
#               into SRCROOT. Use buildit's -noinstallsrc to suppress the
#               source copy.

# clean		Clean API objects.  Use buildit's -noclean to suppress the
#               clean step.

# installhdrs	Installs the API header (gdb.h) into DSTROOT which is
#               defined by buildit. Currently the following layout
#               describes where the default install places the header:
#
#		/usr/include/gdb/gdb.h

# install	Installs the API static library into DSTROOT which is
#               defined by buildit. Currently the following layout
#               describes where the default install places the files:
#
# 		/usr/lib/gdb/gdb_plugin_support.o

# As a reminder here some other possibly "useful" buildit options: -noverify, -nosum.

#----------------------------------------------------------------#

# NOTE: THE API IS NOT YET RELEASED SEPARATELY. THUS THE BUILDIT TARGETS
#       ARE NOT BEING USED. CURRENTLY THIS MAKEFILE IS USED SOLELY BY THE
#       MACSBUG MAKEFILE EXPLICITLY. THUS THE LIBRARY IS PLACED WHERE THE
#       MACSBUG MAKEFILE WANTS IT, AND THE HEADER IS KEPT IN THE API'S
#       SOURCE DIRECTORY AND NOT INSTALLED ANYWHERE.

#----------------------------------------------------------------#

SRCROOT				=	.
SYMROOT				=	.
OBJROOT				=	.
OFILE_DIR			=	$(OBJROOT)/Objects

IDIR				=	/usr/lib/gdb
DSTDIR				=	$(DSTROOT)$(IDIR)

IDIR_H				=	/usr/include/gdb
DSTDIR_H			=	$(DSTROOT)$(IDIR_H)

CC				=	cc

VPATH				=	$(OFILE_DIR)

CFILES				=	gdb.c			\
					gdb_io_redirection.c	\
					gdb_complete.c		\
					gdb_set.c		\
					gdb_special_events.c

HFILES				=	gdb.h			\
					gdb_private_interfaces.h

HEADERS				=	$(HFILES:%=$(SRCROOT)/%)

ifndef BINUTILS_BUILD_ROOT
BINUTILS_BUILD_ROOT = $(SDKROOT)
endif

ifndef GDB_BUILD_ROOT
GDB_BUILD_ROOT = $(SDKROOT)
endif

BINUTILS_FRAMEWORKS = $(BINUTILS_BUILD_ROOT)/System/Library/PrivateFrameworks

GDB_FRAMEWORKS = $(GDB_BUILD_ROOT)/System/Library/PrivateFrameworks

INCLUDES 			= 	\
					-I$(BINUTILS_FRAMEWORKS)/bfd.framework/Headers 	 \
					-I$(BINUTILS_FRAMEWORKS)/binutils.framework/Headers	 \
					-I$(GDB_FRAMEWORKS)/gdb.framework/Headers 	 \
					-I$(GDB_FRAMEWORKS)/gdb.framework/Headers/config \
					-I$(GDB_FRAMEWORKS)/gdb.framework/Headers/machine \
					-I. -I.. -I../../src -I../../src/include \
					-I$(IDIR_H) -I$(DSTDIR_H) \
					-I../../src/gdb -I../../src/bfd \
					-I../../src/gdb/target \
					-I../../src/readline/ \
					-I$(SRCROOT)/include

OPT				=	-Os

CFLAGS 				= 	-gdwarf-2		\
					-DTARGET_POWERPC        \
					$(OPT)			\
					-c			\
					-w			\
					-Wall			\
					-Wimplicit		\
					-Wformat		\
					-Wno-error		\
					-Wno-unused		\
					-Wno-deprecated-declarations \
					$(INCLUDES)

UNIX03_UGH_CFLAGS		=	-D__DARWIN_UNIX03=0

TARGET_ARCHITECTURE		=	ppc

OBJS				=	$(CFILES:%.c=$(OFILE_DIR)/%.o)

INSTALLSRC_FILES		=	$(CFILES)		\
					gdb_testing.i		\
					$(HFILES)		\
					Makefile

#----------------------------------------------------------------#
#                       *----------------*
#                       | Default Target |
#                       *----------------*

all				:	$(OFILE_DIR) 		\
					$(SYMROOT)		\
					$(SYMROOT)/gdb_plugin_support.o

#----------------------------------------------------------------#
#                       *-----------------*
#                       | Buildit Targets |
#                       *-----------------*

installsrc			:
	mkdir -p  $(SRCROOT)
	chmod 755 $(SRCROOT)
	gnutar cf - $(INSTALLSRC_FILES) | (cd $(SRCROOT) && gnutar xf -)
	chmod 444 $(SRCROOT)/*

clean				:
	rm -rf $(OFILE_DIR) $(SYMROOT)/gdb_plugin_support.o

installhdrs			:	
	cp $(SYMROOT)/gdb.h  $(DSTDIR_H)
	chmod 444 $(DSTDIR_H)/gdb.h

install				:	all $(DSTDIR)
	strip -ru $(SYMROOT)/gdb_plugin_support.o -o $(DSTDIR)/gdb_plugin_support.o
	chmod 444 $(DSTDIR)/gdb_plugin_support.o

#----------------------------------------------------------------#
#                        *-------------*
#                        | Build Rules |
#                        *-------------*

$(SYMROOT)/gdb_plugin_support.o	: 	$(OBJS)
	ld -r $(OBJS) -arch $(TARGET_ARCHITECTURE) -o $(SYMROOT)/gdb_plugin_support.o
	if test -d $(SYMROOT) && test -e $(SYMROOT)/gdb_plugin_support.o; then \
	  stat $(SYMROOT)/gdb_plugin_support.o; \
	  du $(SYMROOT)/gdb_plugin_support.o; \
	  if test -x "`which sync`"; then sync; fi; \
	  if test -x "`which nm`"; then \
	    echo "-----------------------------------"; \
	    echo "`nm $@ | wc -l | tr -d [:blank:]` symbols in $@"; \
	  fi; \
	  if test -x ../../src/binutils/size; then \
	    ../../src/binutils/size $(SYMROOT)/gdb_plugin_support.o; \
	  elif test -x "`which size`"; then \
	    size $(SYMROOT)/gdb_plugin_support.o; \
	  fi; \
	  if test -x "`which strings`"; then \
	    echo "----------------------------------"; \
	    echo "`strings $@ | wc -l | tr -d [:blank:]` strings in $@"; \
	  fi; \
	elif test -n "$(OBJS)"; then \
	  echo "OBJS are $(OBJS)"; \
	fi

$(OFILE_DIR)/%.o		:	%.c $(HFILES)
	sync && $(CC) $(CFLAGS) $(RC_CFLAGS) -o $(OFILE_DIR)/$(notdir $*).o $<

$(OFILE_DIR)/gdb.o		:	gdb.c gdb_testing.i $(HFILES)
	sync && $(CC) $(CFLAGS) $(RC_CFLAGS) -o $(OFILE_DIR)/gdb.o gdb.c

$(OFILE_DIR) $(SYMROOT) $(DSTDIR):
	mkdir -p $@
