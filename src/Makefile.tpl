[+ AutoGen5 template -*- Mode: Makefile -*-
in
+]

# Makefile.in is generated from Makefile.tpl by 'autogen Makefile.def'.
#
# Makefile for directory with subdirs to build.
#   Copyright (C) 1990, 1991, 1992, 1993, 1994, 1995, 1996, 1997, 1998,
#   1999, 2000, 2001, 2002, 2003, 2004 Free Software Foundation
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
# Foundation, Inc., 51 Franklin St., 5th Floor, Boston, MA 02110-1301, USA.
#

# -------------------------------
# Standard Autoconf-set variables
# -------------------------------
VPATH=@srcdir@

build_alias=@build_alias@
build_noncanonical=@build_noncanonical@
build_vendor=@build_vendor@
build_os=@build_os@
build=@build@
host_alias=@host_alias@
host_noncanonical=@host_noncanonical@
host_vendor=@host_vendor@
host_os=@host_os@
host=@host@
target_alias=@target_alias@
target_noncanonical=@target_noncanonical@
target_vendor=@target_vendor@
target_os=@target_os@
target=@target@

program_transform_name = @program_transform_name@

prefix = @prefix@
exec_prefix = @exec_prefix@

srcdir = @srcdir@

bindir = @bindir@
sbindir = @sbindir@
libexecdir = @libexecdir@
datadir = @datadir@
sysconfdir = @sysconfdir@
sharedstatedir = @sharedstatedir@
localstatedir = @localstatedir@
libdir = @libdir@
includedir = @includedir@
oldincludedir = @oldincludedir@
infodir = @infodir@
datarootdir = @datarootdir@
docdir = @docdir@
pdfdir = @pdfdir@
htmldir = @htmldir@
mandir = @mandir@
man1dir = $(mandir)/man1
man2dir = $(mandir)/man2
man3dir = $(mandir)/man3
man4dir = $(mandir)/man4
man5dir = $(mandir)/man5
man6dir = $(mandir)/man6
man7dir = $(mandir)/man7
man8dir = $(mandir)/man8
man9dir = $(mandir)/man9

INSTALL = @INSTALL@
INSTALL_PROGRAM = @INSTALL_PROGRAM@
INSTALL_SCRIPT = @INSTALL_SCRIPT@
INSTALL_DATA = @INSTALL_DATA@
LN = @LN@
LN_S = @LN_S@
MKDIR_P = @MKDIR_P@
SED = @SED@

# ------------------------------
# automake-style maintainer mode
# ------------------------------
MAINT = @MAINT@
MAINTAINER_MODE_FALSE = @MAINTAINER_MODE_FALSE@
MAINTAINER_MODE_TRUE = @MAINTAINER_MODE_TRUE@
ERROR_ON_WARNING = @ERROR_ON_WARNING@

# --------------------------------
# automake-style verbosity control
# --------------------------------

AM_DEFAULT_V = @AM_DEFAULT_V@
AM_DEFAULT_VERBOSITY = @AM_DEFAULT_VERBOSITY@
V_TO_PASS = @V@
V_VAR = @AM_V@

AM_V_at = $(am__v_at_@AM_V@)
am__v_at_ = $(am__v_at_@AM_DEFAULT_V@)
am__v_at_0 = @
am__v_at_1 =

AM_V_v = $(am__v_v_@AM_V@)
am__v_v_ = $(am__v_v_@AM_DEFAULT_V@)
am__v_v_0 =
am__v_v_1 = -v

AM_V_MFLAG = $(am__v_MFLAG_@AM_V@)
am__v_MFLAG_ = $(am__v_MFLAG_@AM_DEFAULT_V@)
am__v_MFLAG_0 = @MAINT@ --no-print-directory
am__v_MFLAG_1 = -w

# -------------------------------------------------
# Miscellaneous non-standard autoconf-set variables
# -------------------------------------------------

# The gcc driver likes to know the arguments it was configured with.
TOPLEVEL_CONFIGURE_ARGUMENTS=@TOPLEVEL_CONFIGURE_ARGUMENTS@

tooldir = @tooldir@
build_tooldir = @build_tooldir@

GDB_NLM_DEPS = 

## list all three archs, starting with build:
BUILD_ARCHITECTURE = @BUILD_ARCHITECTURE@
## now host:
HOST_ARCHITECTURE = @HOST_ARCHITECTURE@
## finally target:
TARGET_ARCHITECTURE = @TARGET_ARCHITECTURE@
## and their equivalent archflags, starting with build:
C_BUILD_ARCHFLAGS = @C_BUILD_ARCHFLAGS@
CXX_BUILD_ARCHFLAGS = $(C_BUILD_ARCHFLAGS)
## and host:
C_HOST_ARCHFLAGS = @C_HOST_ARCHFLAGS@
CXX_HOST_ARCHFLAGS = $(C_HOST_ARCHFLAGS)
## and finally target:
C_TARG_ARCHFLAGS = @C_TARG_ARCHFLAGS@
CXX_TARG_ARCHFLAGS = $(C_TARG_ARCHFLAGS)

# This is the name of the environment variable used for the path to
# the libraries.
RPATH_ENVVAR = @RPATH_ENVVAR@

# This is the list of directories to be built for the build system.
BUILD_CONFIGDIRS = libiberty
# Build programs are put under this directory.
BUILD_SUBDIR = @build_subdir@
# This is set by the configure script to the arguments to use when configuring
# directories built for the build system.
BUILD_CONFIGARGS = @build_configargs@ --with-build-subdir="$(BUILD_SUBDIR)"

# Linker flags to use on the host, for stage1 or when not
# bootstrapping.
STAGE1_LDFLAGS = @stage1_ldflags@

# Libraries to use on the host, for stage1 or when not bootstrapping.
STAGE1_LIBS = @stage1_libs@

# Linker flags to use for stage2 and later.
POSTSTAGE1_LDFLAGS = @poststage1_ldflags@

# Libraries to use for stage2 and later.
POSTSTAGE1_LIBS = @poststage1_libs@

# This is the list of variables to export in the environment when
# configuring any subdirectory.  It must also be exported whenever
# recursing into a build directory in case that directory's Makefile
# re-runs configure.
BASE_EXPORTS = \
	FLEX="$(FLEX)"; export FLEX; \
	LEX="$(LEX)"; export LEX; \
	BISON="$(BISON)"; export BISON; \
	YACC="$(YACC)"; export YACC; \
	M4="$(M4)"; export M4; \
	MAKEINFO="$(MAKEINFO)"; export MAKEINFO; \
	GUILE="$(GUILE)"; export GUILE;

# This is the list of variables to export in the environment when
# configuring subdirectories for the build system.
BUILD_EXPORTS = \
	$(BASE_EXPORTS) \
	AR="$(AR_FOR_BUILD)"; export AR; \
	AS="$(AS_FOR_BUILD)"; export AS; \
	CC="$(CC_FOR_BUILD)"; export CC; \
	CFLAGS="$(CFLAGS_FOR_BUILD) $(C_BUILD_ARCHFLAGS)"; export CFLAGS; \
	CONFIG_SHELL="$(SHELL)"; export CONFIG_SHELL; \
	CXX="$(CXX_FOR_BUILD)"; export CXX; \
	CXXFLAGS="$(CXXFLAGS_FOR_BUILD) $(CXX_BUILD_ARCHFLAGS)"; export CXXFLAGS; \
	GCJ="$(GCJ_FOR_BUILD)"; export GCJ; \
	GFORTRAN="$(GFORTRAN_FOR_BUILD)"; export GFORTRAN; \
	GOC="$(GOC_FOR_BUILD)"; export GOC; \
	GOCFLAGS="$(GOCFLAGS_FOR_BUILD)"; export GOCFLAGS; \
	DLLTOOL="$(DLLTOOL_FOR_BUILD)"; export DLLTOOL; \
	LD="$(LD_FOR_BUILD)"; export LD; \
	LDFLAGS="$(LDFLAGS_FOR_BUILD)"; export LDFLAGS; \
	NM="$(NM_FOR_BUILD)"; export NM; \
	OBJC="$(OBJC_FOR_BUILD)"; export OBJC; \
	OBJCPP="$(OBJCPP_FOR_BUILD)"; export OBJCPP; \
	RANLIB="$(RANLIB_FOR_BUILD)"; export RANLIB; \
	WINDRES="$(WINDRES_FOR_BUILD)"; export WINDRES; \
	WINDMC="$(WINDMC_FOR_BUILD)"; export WINDMC;

# These variables must be set on the make command line for directories
# built for the build system to override those in BASE_FLAGS_TO_PASS.
EXTRA_BUILD_FLAGS = \
	CFLAGS="$(CFLAGS_FOR_BUILD)" \
	LDFLAGS="$(LDFLAGS_FOR_BUILD)"

# This is the list of directories to built for the host system.
SUBDIRS = @configdirs@
# This is set by the configure script to the arguments to use when configuring
# directories built for the host system.
HOST_CONFIGARGS = @host_configargs@
# Host programs are put under this directory, which is . except if building
# with srcdir=..
HOST_SUBDIR = @host_subdir@
# This is the list of variables to export in the environment when
# configuring subdirectories for the host system.
HOST_EXPORTS = \
	$(BASE_EXPORTS) \
	CC="$(CC)"; export CC; \
	ADA_CFLAGS="$(ADA_CFLAGS)"; export ADA_CFLAGS; \
	CFLAGS="$(CFLAGS) $(C_HOST_ARCHFLAGS)"; export CFLAGS; \
	CONFIG_SHELL="$(SHELL)"; export CONFIG_SHELL; \
	CPPFLAGS="$(CPPFLAGS)"; export CPPFLAGS; \
	CXX="$(CXX)"; export CXX; \
	CXXFLAGS="$(CXXFLAGS) $(CXX_HOST_ARCHFLAGS)"; export CXXFLAGS; \
	GCJ="$(GCJ)"; export GCJ; \
	GFORTRAN="$(GFORTRAN)"; export GFORTRAN; \
	GOC="$(GOC)"; export GOC; \
	AR="$(AR)"; export AR; \
	AS="$(AS)"; export AS; \
	CC_FOR_BUILD="$(CC_FOR_BUILD)"; export CC_FOR_BUILD; \
	DLLTOOL="$(DLLTOOL)"; export DLLTOOL; \
	LD="$(LD)"; export LD; \
	LDFLAGS="$(LDFLAGS)"; export LDFLAGS; \
	NM="$(NM)"; export NM; \
	RANLIB="$(RANLIB)"; export RANLIB; \
	WINDRES="$(WINDRES)"; export WINDRES; \
	WINDMC="$(WINDMC)"; export WINDMC; \
	OBJC="$(OBJC)"; export OBJC; \
	OBJCOPY="$(OBJCOPY)"; export OBJCOPY; \
	OBJCPP="$(OBJCPP)"; export OBJCPP; \
	OBJCXX="$(OBJCXX)"; export OBJCXX; \
	OBJCXXCPP="$(OBJCXXCPP)"; export OBJCXXCPP; \
	OBJDUMP="$(OBJDUMP)"; export OBJDUMP; \
	READELF="$(READELF)"; export READELF; \
	AR_FOR_TARGET="$(AR_FOR_TARGET)"; export AR_FOR_TARGET; \
	AS_FOR_TARGET="$(AS_FOR_TARGET)"; export AS_FOR_TARGET; \
	GCC_FOR_TARGET="$(GCC_FOR_TARGET)"; export GCC_FOR_TARGET; \
	LD_FOR_TARGET="$(LD_FOR_TARGET)"; export LD_FOR_TARGET; \
	NM_FOR_TARGET="$(NM_FOR_TARGET)"; export NM_FOR_TARGET; \
	OBJC_FOR_TARGET="$(OBJC_FOR_TARGET)"; export OBJC_FOR_TARGET; \
	OBJCOPY_FOR_TARGET="$(OBJCOPY_FOR_TARGET)"; export OBJCOPY_FOR_TARGET; \
	OBJCPP_FOR_TARGET="$(OBJCPP_FOR_TARGET)"; export OBJCPP_FOR_TARGET; \
	OBJCXX_FOR_TARGET="$(OBJCXX_FOR_TARGET)"; export OBJCXX_FOR_TARGET; \
	OBJDUMP_FOR_TARGET="$(OBJDUMP_FOR_TARGET)"; export OBJDUMP_FOR_TARGET; \
	RANLIB_FOR_TARGET="$(RANLIB_FOR_TARGET)"; export RANLIB_FOR_TARGET; \
	READELF_FOR_TARGET="$(READELF_FOR_TARGET)"; export READELF_FOR_TARGET; \
	TOPLEVEL_CONFIGURE_ARGUMENTS="$(TOPLEVEL_CONFIGURE_ARGUMENTS)"; export TOPLEVEL_CONFIGURE_ARGUMENTS; \
	HOST_LIBS="$(STAGE1_LIBS)"; export HOST_LIBS; \
	GMPLIBS="$(HOST_GMPLIBS)"; export GMPLIBS; \
	GMPINC="$(HOST_GMPINC)"; export GMPINC; \
	ISLLIBS="$(HOST_ISLLIBS)"; export ISLLIBS; \
	ISLINC="$(HOST_ISLINC)"; export ISLINC; \
	LIBELFLIBS="$(HOST_LIBELFLIBS)"; export LIBELFLIBS; \
	LIBELFINC="$(HOST_LIBELFINC)"; export LIBELFINC; \
	$(RPATH_ENVVAR)=`echo "$(HOST_LIB_PATH)$$$(RPATH_ENVVAR)" | $(SED) 's,::*,:,g;s,^:*,,;s,:*$$,,'`; export $(RPATH_ENVVAR);

POSTSTAGE1_CXX_EXPORT = \
	CXX='$(CXX)'; export CXX; \
	CXX_FOR_BUILD='$(CXX_FOR_BUILD)'; export CXX_FOR_BUILD;

# Similar, for later GCC stages.
POSTSTAGE1_HOST_EXPORTS = \
	$(RPATH_ENVVAR)=`echo "$(TARGET_LIB_PATH)$$$(RPATH_ENVVAR)" | $(SED) 's,::*,:,g;s,^:*,,;s,:*$$,,'`; export $(RPATH_ENVVAR); \
	$(HOST_EXPORTS) \
	CC="$(STAGE_CC_WRAPPER) $$r/$(HOST_SUBDIR)/prev-gcc/xgcc$(exeext) \
	  -B$${r}/$(HOST_SUBDIR)/prev-gcc/ -B$(build_tooldir)/bin/"; export CC; \
	CC_FOR_BUILD="$(STAGE_CC_WRAPPER) \
	  $${r}/$(HOST_SUBDIR)/prev-gcc/xgcc$(exeext) \
	  -B$${r}/$(HOST_SUBDIR)/prev-gcc/ \
	  -B$(build_tooldir)/bin/"; export CC_FOR_BUILD; \
	$(POSTSTAGE1_CXX_EXPORT) \
	LDFLAGS="$(POSTSTAGE1_LDFLAGS) $(BOOT_LDFLAGS)"; export LDFLAGS; \
	HOST_LIBS="$(POSTSTAGE1_LIBS)"; export HOST_LIBS;

# This is set by the configure script to the list of directories which
# should be built using the target tools.
TARGET_CONFIGDIRS = @target_configdirs@
# Target libraries are put under this directory:
TARGET_SUBDIR = @target_subdir@
# This is set by the configure script to the arguments to use when configuring
# directories built for the target.
TARGET_CONFIGARGS = @target_configargs@ --with-target-subdir="$(TARGET_SUBDIR)"
# This is the list of variables to export in the environment when
# configuring subdirectories for the host system.
BASE_TARGET_EXPORTS = \
	$(BASE_EXPORTS) \
	AR="$(AR_FOR_TARGET)"; export AR; \
	AS="$(AS_FOR_TARGET)"; export AS; \
	CC="$(CC_FOR_TARGET)"; export CC; \
	CFLAGS="$(CFLAGS_FOR_TARGET) $(C_TARG_ARCHFLAGS)"; export CFLAGS; \
	CONFIG_SHELL="$(SHELL)"; export CONFIG_SHELL; \
	CPPFLAGS="$(CPPFLAGS_FOR_TARGET)"; export CPPFLAGS; \
	CXXFLAGS="$(CXXFLAGS_FOR_TARGET) $(CXX_TARG_ARCHFLAGS)"; export CXXFLAGS; \
	GCJ="$(GCJ_FOR_TARGET)"; export GCJ; \
	GFORTRAN="$(GFORTRAN_FOR_TARGET)"; export GFORTRAN; \
	GOC="$(GOC_FOR_TARGET) $(XGCC_FLAGS_FOR_TARGET) $$TFLAGS"; export GOC; \
	DLLTOOL="$(DLLTOOL_FOR_TARGET)"; export DLLTOOL; \
	LD="$(LD_FOR_TARGET)"; export LD; \
	LDFLAGS="$(LDFLAGS_FOR_TARGET)"; export LDFLAGS; \
	LIPO="$(LIPO_FOR_TARGET)"; export LIPO; \
	NM="$(NM_FOR_TARGET)"; export NM; \
	OBJC="$(OBJC_FOR_TARGET)"; export OBJC; \
	OBJCOPY="$(OBJCOPY_FOR_TARGET)"; export OBJCOPY; \
	OBJCPP="$(OBJCPP_FOR_TARGET)"; export OBJCPP; \
	OBJCXX="$(OBJCXX_FOR_TARGET)"; export OBJCXX; \
	OBJDUMP="$(OBJDUMP_FOR_TARGET)"; export OBJDUMP; \
	RANLIB="$(RANLIB_FOR_TARGET)"; export RANLIB; \
	READELF="$(READELF_FOR_TARGET)"; export READELF; \
	STRIP="$(STRIP_FOR_TARGET)"; export STRIP; \
	WINDRES="$(WINDRES_FOR_TARGET)"; export WINDRES; \
	WINDMC="$(WINDMC_FOR_TARGET)"; export WINDMC; \
	$(RPATH_ENVVAR)=`echo "$(HOST_LIB_PATH)$(TARGET_LIB_PATH)$$$(RPATH_ENVVAR)" | $(SED) 's,::*,:,g;s,^:*,,;s,:*$$,,'`; export $(RPATH_ENVVAR);

RAW_CXX_TARGET_EXPORTS = \
	$(BASE_TARGET_EXPORTS) \
	CXX_FOR_TARGET="$(RAW_CXX_FOR_TARGET)"; export CXX_FOR_TARGET; \
	CXX="$(RAW_CXX_FOR_TARGET)"; export CXX;

NORMAL_TARGET_EXPORTS = \
	$(BASE_TARGET_EXPORTS) \
	CXX="$(CXX_FOR_TARGET)"; export CXX;

# Where to find GMP
HOST_GMPLIBS = @gmplibs@
HOST_GMPINC = @gmpinc@

# Where to find ISL
HOST_ISLLIBS = @isllibs@
HOST_ISLINC = @islinc@

# Where to find libelf
HOST_LIBELFLIBS = @libelflibs@
HOST_LIBELFINC = @libelfinc@

EXTRA_CONFIGARGS_LIBJAVA = @EXTRA_CONFIGARGS_LIBJAVA@

# ----------------------------------------------
# Programs producing files for the BUILD machine
# ----------------------------------------------

SHELL = @config_shell@

# pwd command to use. Allow user to override default by setting PWDCMD in
# the environment to account for automounters. The make variable must not
# be called PWDCMD, otherwise the value set here is passed to make
# subprocesses and overrides the setting from the user's environment.
# Do NOT use PWD since it is a common shell environment variable and we
# do NOT want to corrupt it.
PWD_COMMAND = $${PWDCMD-pwd}

# compilers to use to create programs which must be run in the build
# environment.
AR_FOR_BUILD = @AR_FOR_BUILD@
AS_FOR_BUILD = @AS_FOR_BUILD@
CC_FOR_BUILD = @CC_FOR_BUILD@
CFLAGS_FOR_BUILD = @CFLAGS_FOR_BUILD@
LIBCFLAGS_FOR_BUILD = $(CFLAGS_FOR_BUILD) $(C_BUILD_ARCHFLAGS)

CPP_FOR_BUILD = @CPP_FOR_BUILD@

CXX_FOR_BUILD = $(CXX)
CXXFLAGS_FOR_BUILD = @CXXFLAGS_FOR_BUILD@
LIBCXXFLAGS_FOR_BUILD = $(CXXFLAGS_FOR_BUILD) $(CXX_BUILD_ARCHFLAGS)

# ...continued:
DLLTOOL_FOR_BUILD = @DLLTOOL_FOR_BUILD@
GCJ_FOR_BUILD = @GCJ_FOR_BUILD@
GFORTRAN_FOR_BUILD = @GFORTRAN_FOR_BUILD@
GOC_FOR_BUILD = @GOC_FOR_BUILD@
LDFLAGS_FOR_BUILD = @LDFLAGS_FOR_BUILD@
LD_FOR_BUILD = @LD_FOR_BUILD@
NM_FOR_BUILD = @NM_FOR_BUILD@
OBJC_FOR_BUILD = @OBJC_FOR_BUILD@
OBJCPP_FOR_BUILD = @OBJCPP_FOR_BUILD@
RANLIB_FOR_BUILD = @RANLIB_FOR_BUILD@
WINDMC_FOR_BUILD = @WINDMC_FOR_BUILD@
WINDRES_FOR_BUILD = @WINDRES_FOR_BUILD@

# Special variables passed down in EXTRA_GCC_FLAGS. They are defined
# here so that they can be overridden by Makefile fragments.
BUILD_PREFIX = @BUILD_PREFIX@
BUILD_PREFIX_1 = @BUILD_PREFIX_1@

# Flags to pass to stage2 and later makes. They are defined
# here so that they can be overridden by Makefile fragments.
BOOT_CFLAGS= -ggdb -O1
BOOT_LDFLAGS=
BOOT_ADAFLAGS= -gnatpg

AWK = @AWK@

CONFIGURED_BISON = @CONFIGURED_BISON@
BISON = `if [ -f $${r}/$(BUILD_SUBDIR)/bison/tests/bison ]; then \
	    echo $${r}/$(BUILD_SUBDIR)/bison/tests/bison; \
	 else \
	    echo ${CONFIGURED_BISON}; \
	 fi`

CONFIGURED_YACC = @CONFIGURED_YACC@
YACC = `if [ -f $${r}/$(BUILD_SUBDIR)/bison/tests/bison ]; then \
	    echo "$${r}/$(BUILD_SUBDIR)/bison/tests/bison -y"; \
	elif [ -f $${r}/$(BUILD_SUBDIR)/byacc/byacc ]; then \
	    echo $${r}/$(BUILD_SUBDIR)/byacc/byacc; \
	else \
	    echo ${CONFIGURED_YACC}; \
	fi`

CONFIGURED_FLEX = @CONFIGURED_FLEX@
FLEX = `if [ -f $${r}/$(BUILD_SUBDIR)/flex/flex ]; \
	then echo $${r}/$(BUILD_SUBDIR)/flex/flex; \
	else echo ${CONFIGURED_FLEX}; fi`

CONFIGURED_LEX = @CONFIGURED_LEX@
LEX = `if [ -f $${r}/$(BUILD_SUBDIR)/flex/flex ]; \
	then echo $${r}/$(BUILD_SUBDIR)/flex/flex; \
	else echo ${CONFIGURED_LEX}; fi`

CONFIGURED_M4 = @CONFIGURED_M4@
M4 = `if [ -f $${r}/$(BUILD_SUBDIR)/m4/m4 ]; \
	then echo $${r}/$(BUILD_SUBDIR)/m4/m4; \
	else echo ${CONFIGURED_M4}; fi`

# For an installed makeinfo, we require it to be from texinfo 4.2 or
# higher, else we use the "missing" dummy.  We also pass the subdirectory
# makeinfo even if only the Makefile is there, because Texinfo builds its
# manual when made, and it requires its own version.
CONFIGURED_MAKEINFO = @CONFIGURED_MAKEINFO@
MAKEINFO = `if [ -f $${r}/$(BUILD_SUBDIR)/texinfo/makeinfo/Makefile ]; \
	then echo $${r}/$(BUILD_SUBDIR)/texinfo/makeinfo/makeinfo; \
	else if (${CONFIGURED_MAKEINFO} --version \
	  | egrep 'texinfo[^0-9]*([1-3][0-9]|4\.[2-9]|[5-9])') >/dev/null 2>&1; \
        then echo ${CONFIGURED_MAKEINFO}; else echo "$${s}/missing makeinfo"; fi; fi`

# This just becomes part of the MAKEINFO definition passed down to
# sub-makes.  It lets flags be given on the command line while still
# using the makeinfo from the object tree.
# (Default to avoid splitting info files by setting the threshold high.)
MAKEINFOFLAGS = --split-size=5000000

# FIXME: expect may become a build tool?
EXPECT = `if [ -f $${r}/$(HOST_SUBDIR)/expect/expect ]; \
	then echo $${r}/$(HOST_SUBDIR)/expect/expect; \
	else echo expect; fi`

RUNTEST = `if [ -f $${s}/dejagnu/runtest ]; \
	then echo $${s}/dejagnu/runtest; \
	else echo runtest; fi`

# ---------------------------------------------
# Programs producing files for the HOST machine
# ---------------------------------------------

AS = @AS@

AR = @AR@
AR_FLAGS = rc

CC = @CC@
CFLAGS = @CFLAGS@
LIBCFLAGS = $(CFLAGS) $(C_HOST_ARCHFLAGS)
GOCFLAGS = $(CFLAGS)
GM2FLAGS = $(CFLAGS)

CCAS = @CCAS@
CCASFLAGS = @CCASFLAGS@

CPP = @CPP@
CPPFLAGS = @CPPFLAGS@

CXX = @CXX@
CXXFLAGS = @CXXFLAGS@
LIBCXXFLAGS = $(CXXFLAGS) $(CXX_HOST_ARCHFLAGS) -fno-implicit-templates

CXXCPP = @CXXCPP@

OBJC = @OBJC@
OBJCFLAGS = @OBJCFLAGS@
OBJCPP = @OBJCPP@

OBJCXX = @OBJCXX@
OBJCXXFLAGS = @OBJCXXFLAGS@
OBJCXXCPP = @OBJCXXCPP@

DLLTOOL = @DLLTOOL@

NM = @NM@

LD = @LD@
LDFLAGS = 
CONFIGURED_LDFLAGS = @LDFLAGS@

LIPO = @LIPO@
OBJDUMP = @OBJDUMP@
RANLIB = @RANLIB@
READELF = @READELF@
STRIP = @STRIP@

WINDRES = @WINDRES@
WINDMC = @WINDMC@

GNATBIND = @GNATBIND@
GNATMAKE = @GNATMAKE@

GUILE = guile

YFLAGS = @YFLAGS@

PICFLAG = 

TFLAGS =

# Only build the C compiler for stage1, because that is the only one that
# we can guarantee will build with the native compiler, and also it is the
# only thing useful for building stage2. STAGE1_CFLAGS (via CFLAGS),
# MAKEINFO and MAKEINFOFLAGS are explicitly passed here to make them
# overrideable (for a bootstrap build stage1 also builds gcc.info).

STAGE1_CFLAGS = @stage1_cflags@
STAGE1_CHECKING = @stage1_checking@
STAGE1_LANGUAGES = @stage1_languages@

do-compare = @do_compare@
do-compare3 = $(do-compare)

# -----------------------------------------------
# Programs producing files for the TARGET machine
# -----------------------------------------------

FLAGS_FOR_TARGET = @FLAGS_FOR_TARGET@

AR_FOR_TARGET=@AR_FOR_TARGET@
CONFIGURED_AR_FOR_TARGET=@CONFIGURED_AR_FOR_TARGET@
USUAL_AR_FOR_TARGET = ` \
  if [ -f $${r}/$(HOST_SUBDIR)/binutils/ar ]; then \
    echo $${r}/$(HOST_SUBDIR)/binutils/ar; \
  else \
    if [ 'x$(host)' = 'x$(target)' ]; then \
      echo $(AR); \
    else \
      echo $(CONFIGURED_AR_FOR_TARGET); \
    fi; \
  fi`

# APPLE LOCAL: autoconf 2.57 barfs if use use the name "AS_FOR_TARGET".
BAD_AS_TO_USE_FOR_OUR_TARGET=@GAS_FOR_TARGET@
AS_FOR_TARGET=@AS_FOR_TARGET@
CONFIGURED_AS_FOR_TARGET=@CONFIGURED_AS_FOR_TARGET@
USUAL_AS_FOR_TARGET = ` \
  if [ -f $${r}/$(HOST_SUBDIR)/gas/as-new ]; then \
    echo $${r}/$(HOST_SUBDIR)/gas/as-new; \
  elif [ -f $${r}/$(HOST_SUBDIR)/gcc/as ]; then \
    echo $${r}/$(HOST_SUBDIR)/gcc/as; \
  else \
    if [ 'x$(host)' = 'x$(target)' ]; then \
      echo $(AS); \
    else \
      echo $(CONFIGURED_AS_FOR_TARGET); \
    fi; \
  fi`

CC_FOR_TARGET=$(STAGE_CC_WRAPPER) @CC_FOR_TARGET@ $(FLAGS_FOR_TARGET)
CONFIGURED_CC_FOR_TARGET=@CONFIGURED_CC_FOR_TARGET@
USUAL_CC_FOR_TARGET = ` \
  if [ -f $${r}/$(HOST_SUBDIR)/gcc/xgcc ]; then \
    echo "$${r}/$(HOST_SUBDIR)/gcc/xgcc -B$${r}/$(HOST_SUBDIR)/gcc"; \
  else \
    if [ 'x$(host)' = 'x$(target)' ]; then \
      echo "$(CC)"; \
    else \
      echo "$(CONFIGURED_CC_FOR_TARGET)"; \
    fi; \
  fi`

# During gcc bootstrap, if we use some random cc for stage1 then
# CFLAGS will be just -g. We want to ensure that TARGET libraries
# (which we know are built with gcc) are built with optimizations so
# prepend -O1 when setting CFLAGS_FOR_TARGET.
CFLAGS_FOR_TARGET = -O1 $(CFLAGS) $(SYSROOT_CFLAGS_FOR_TARGET)
SYSROOT_CFLAGS_FOR_TARGET = @SYSROOT_CFLAGS_FOR_TARGET@

# If GCC_FOR_TARGET is not overriden on the command line, then this
# variable is passed down to the gcc Makefile, where it is used to
# build libgcc2.a.  We define it here so that it can itself be
# overridden on the command line.
GCC_FOR_TARGET=@GCC_FOR_TARGET@
CONFIGURED_GCC_FOR_TARGET=@CONFIGURED_GCC_FOR_TARGET@
USUAL_GCC_FOR_TARGET = $(STAGE_CC_WRAPPER) \
  $$r/$(HOST_SUBDIR)/gcc/xgcc -B$$r/$(HOST_SUBDIR)/gcc/ $(FLAGS_FOR_TARGET)
LIBCFLAGS_FOR_TARGET = $(CFLAGS_FOR_TARGET) $(C_TARG_ARCHFLAGS)

CXX_FOR_TARGET=$(STAGE_CC_WRAPPER) @CXX_FOR_TARGET@ $(FLAGS_FOR_TARGET)
CONFIGURED_CXX_FOR_TARGET=@CONFIGURED_CXX_FOR_TARGET@
USUAL_CXX_FOR_TARGET = ` \
  if [ -f $${r}/$(HOST_SUBDIR)/gcc/g++ ]; then \
    (echo "$${r}/$(HOST_SUBDIR)/gcc/g++ -B$${r}/$(HOST_SUBDIR)/gcc -nostdinc++"; \
    test ! -f $${r}/$(TARGET_SUBDIR)/libstdc++-v3/scripts/testsuite_flags || $(SHELL) $${r}/$(TARGET_SUBDIR)/libstdc++-v3/scripts/testsuite_flags --build-includes; \
    echo -L$${r}/$(TARGET_SUBDIR)/libstdc++-v3/src -L$${r}/$(TARGET_SUBDIR)/libstdc++-v3/src/.libs) | tr '\015\012' '  '; \
  else \
    if [ 'x$(host)' = 'x$(target)' ]; then \
      echo $(CXX); \
    else \
      echo $(CONFIGURED_CXX_FOR_TARGET); \
    fi; \
  fi`

RAW_CXX_FOR_TARGET=$(STAGE_CC_WRAPPER) @RAW_CXX_FOR_TARGET@ $(FLAGS_FOR_TARGET)
USUAL_RAW_CXX_FOR_TARGET = ` \
  if [ -f $${r}/$(HOST_SUBDIR)/gcc/xgcc ]; then \
    echo "$${r}/$(HOST_SUBDIR)/gcc/xgcc -shared-libgcc -B$${r}/$(HOST_SUBDIR)/gcc -nostdinc++ -L$${r}/$(TARGET_SUBDIR)/libstdc++-v3/src -L$${r}/$(TARGET_SUBDIR)/libstdc++-v3/src/.libs"; \
  else \
    if [ 'x$(host)' = 'x$(target)' ]; then \
      echo $(CXX); \
    else \
      echo $(CONFIGURED_CXX_FOR_TARGET); \
    fi; \
  fi`

CXXFLAGS_FOR_TARGET = $(CXXFLAGS) $(SYSROOT_CFLAGS_FOR_TARGET)
LIBCXXFLAGS_FOR_TARGET = $(CXXFLAGS_FOR_TARGET) $(CXX_TARG_ARCHFLAGS) -fno-implicit-templates

GCJ_FOR_TARGET=$(STAGE_CC_WRAPPER) @GCJ_FOR_TARGET@ $(FLAGS_FOR_TARGET)
CONFIGURED_GCJ_FOR_TARGET=@CONFIGURED_GCJ_FOR_TARGET@
USUAL_GCJ_FOR_TARGET = ` \
  if [ -f $${r}/$(HOST_SUBDIR)/gcc/gcj ]; then \
    echo "$${r}/$(HOST_SUBDIR)/gcc/gcj -B$${r}/$(HOST_SUBDIR)/gcc"; \
  else \
    if [ 'x$(host)' = 'x$(target)' ]; then \
      echo $(GCJ); \
    else \
      echo $(CONFIGURED_GCJ_FOR_TARGET); \
    fi; \
  fi`

GFORTRAN_FOR_TARGET=$(STAGE_CC_WRAPPER) @GFORTRAN_FOR_TARGET@ $(FLAGS_FOR_TARGET)
CONFIGURED_GFORTRAN_FOR_TARGET=@CONFIGURED_GFORTRAN_FOR_TARGET@
USUAL_GFORTRAN_FOR_TARGET = ` \
  if [ -f $${r}/$(HOST_SUBDIR)/gcc/gfortran ]; then \
    echo $${r}/$(HOST_SUBDIR)/gcc/gfortran -B$${r}/$(HOST_SUBDIR)/gcc; \
  else \
    if [ 'x$(host)' = 'x$(target)' ]; then \
      echo $(GFORTRAN); \
    else \
      echo $(CONFIGURED_GFORTRAN_FOR_TARGET); \
    fi; \
  fi`

GOC_FOR_TARGET=$(STAGE_CC_WRAPPER) @GOC_FOR_TARGET@

DLLTOOL_FOR_TARGET=@DLLTOOL_FOR_TARGET@
CONFIGURED_DLLTOOL_FOR_TARGET=@CONFIGURED_DLLTOOL_FOR_TARGET@
USUAL_DLLTOOL_FOR_TARGET = ` \
  if [ -f $${r}/$(HOST_SUBDIR)/binutils/dlltool ]; then \
    echo $${r}/$(HOST_SUBDIR)/binutils/dlltool; \
  else \
    if [ 'x$(host)' = 'x$(target)' ]; then \
      echo $(DLLTOOL); \
    else \
      echo $(CONFIGURED_DLLTOOL_FOR_TARGET); \
    fi; \
  fi`

LD_FOR_TARGET=@LD_FOR_TARGET@
CONFIGURED_LD_FOR_TARGET=@CONFIGURED_LD_FOR_TARGET@
USUAL_LD_FOR_TARGET = ` \
  if [ -f $${r}/$(HOST_SUBDIR)/ld/ld-new ]; then \
    echo $${r}/$(HOST_SUBDIR)/ld/ld-new; \
  elif [ -f $${r}/$(HOST_SUBDIR)/gcc/collect-ld ]; then \
    echo $${r}/$(HOST_SUBDIR)/gcc/collect-ld; \
  else \
    if [ 'x$(host)' = 'x$(target)' ]; then \
      echo $(LD); \
    else \
      echo $(CONFIGURED_LD_FOR_TARGET); \
    fi; \
  fi`

LDFLAGS_FOR_TARGET = 
CONFIGURED_LDFLAGS_FOR_TARGET = @LDFLAGS_FOR_TARGET@

LIPO_FOR_TARGET=@LIPO_FOR_TARGET@

NM_FOR_TARGET=@NM_FOR_TARGET@
CONFIGURED_NM_FOR_TARGET=@CONFIGURED_NM_FOR_TARGET@
USUAL_NM_FOR_TARGET = ` \
  if [ -f $${r}/$(HOST_SUBDIR)/binutils/nm-new ]; then \
    echo $${r}/$(HOST_SUBDIR)/binutils/nm-new; \
  elif [ -f $${r}/$(HOST_SUBDIR)/gcc/nm ]; then \
    echo $${r}/$(HOST_SUBDIR)/gcc/nm; \
  else \
    if [ 'x$(host)' = 'x$(target)' ]; then \
      echo $(NM); \
    else \
      echo $(CONFIGURED_NM_FOR_TARGET); \
    fi; \
  fi`

OBJC_FOR_TARGET=@OBJC_FOR_TARGET@
CONFIGURED_OBJC_FOR_TARGET=@CONFIGURED_OBJC_FOR_TARGET@
OBJCPP_FOR_TARGET=@OBJCPP_FOR_TARGET@
CONFIGURED_OBJCPP_FOR_TARGET=@CONFIGURED_OBJCPP_FOR_TARGET@
OBJCXX_FOR_TARGET=@OBJCXX_FOR_TARGET@
CONFIGURED_OBJCXX_FOR_TARGET=@CONFIGURED_OBJCXX_FOR_TARGET@

OBJCOPY_FOR_TARGET=@OBJCOPY_FOR_TARGET@
OBJDUMP_FOR_TARGET=@OBJDUMP_FOR_TARGET@

RANLIB_FOR_TARGET=@RANLIB_FOR_TARGET@
CONFIGURED_RANLIB_FOR_TARGET=@CONFIGURED_RANLIB_FOR_TARGET@
USUAL_RANLIB_FOR_TARGET = ` \
  if [ -f $${r}/$(HOST_SUBDIR)/binutils/ranlib ]; then \
    echo $${r}/$(HOST_SUBDIR)/binutils/ranlib; \
  else \
    if [ 'x$(host)' = 'x$(target)' ]; then \
      if [ x'$(RANLIB)' != x'' ]; then \
         echo $(RANLIB); \
      else \
         echo ranlib; \
      fi; \
    else \
      echo $(CONFIGURED_RANLIB_FOR_TARGET); \
    fi; \
  fi`

READELF_FOR_TARGET=@READELF_FOR_TARGET@

WINDRES_FOR_TARGET=@WINDRES_FOR_TARGET@
CONFIGURED_WINDRES_FOR_TARGET=@CONFIGURED_WINDRES_FOR_TARGET@
USUAL_WINDRES_FOR_TARGET = ` \
  if [ -f $${r}/$(HOST_SUBDIR)/binutils/windres ]; then \
    echo $${r}/$(HOST_SUBDIR)/binutils/windres; \
  else \
    if [ 'x$(host)' = 'x$(target)' ]; then \
      echo $(WINDRES); \
    else \
      echo $(CONFIGURED_WINDRES_FOR_TARGET); \
    fi; \
  fi`

WINDMC_FOR_TARGET=@WINDMC_FOR_TARGET@

COMPILER_AS_FOR_TARGET=@COMPILER_AS_FOR_TARGET@
COMPILER_LD_FOR_TARGET=@COMPILER_LD_FOR_TARGET@
COMPILER_NM_FOR_TARGET=@COMPILER_NM_FOR_TARGET@

GOCFLAGS_FOR_TARGET = -O1 -ggdb

PICFLAG_FOR_TARGET = 

# ------------------------------------
# Miscellaneous targets and flag lists
# ------------------------------------

# The 1st rule in the file had better be this one; do NOT put any above it.
# This lives here to allow makefile fragments to contain dependencies.
@default_target@:

#### host and target specific makefile fragments come in here.
@target_makefile_frag@
@alphaieee_frag@
@ospace_frag@
@host_makefile_frag@
###

# This is the list of directories that may be needed in RPATH_ENVVAR
# so that prorgams built for the target machine work.
TARGET_LIB_PATH = [+ FOR target_modules +][+
  IF lib_path +]$(TARGET_LIB_PATH_[+module+])[+ ENDIF lib_path +][+
  ENDFOR target_modules +]$(HOST_LIB_PATH_gcc)
[+ FOR target_modules +][+ IF lib_path +]
@if target-[+module+]
TARGET_LIB_PATH_[+module+] = $$r/$(TARGET_SUBDIR)/[+module+]/[+lib_path+]:
@endif target-[+module+]
[+ ENDIF lib_path +][+ ENDFOR target_modules +]


# This is the list of directories that may be needed in RPATH_ENVVAR
# so that programs built for the host machine work.
HOST_LIB_PATH = [+ FOR host_modules +][+
  IF lib_path +]$(HOST_LIB_PATH_[+module+])[+ ENDIF lib_path +][+
  ENDFOR host_modules +]

# Define HOST_LIB_PATH_gcc here, for the sake of TARGET_LIB_PATH, ouch
@if gcc
HOST_LIB_PATH_gcc = $$r/$(HOST_SUBDIR)/gcc:$$r/$(HOST_SUBDIR)/prev-gcc:
@endif gcc

[+ FOR host_modules +][+ IF lib_path +]
@if [+module+]
HOST_LIB_PATH_[+module+] = \
  $${r}/$(HOST_SUBDIR)/[+module+]/[+lib_path+]:[+ IF bootstrap
  +]$${r}/$(HOST_SUBDIR)/prev-[+module+]/[+lib_path+]:[+ ENDIF bootstrap +]
@endif [+module+]
[+ ENDIF lib_path +][+ ENDFOR host_modules +]

# Flags to pass down to all sub-makes.
BASE_FLAGS_TO_PASS = [+ FOR flags_to_pass +]\
	"[+flag+]=$([+flag+])" [+ ENDFOR flags_to_pass +]\
	"CONFIG_SHELL=$(SHELL)" \
	"MAKEINFO=$(MAKEINFO) $(MAKEINFOFLAGS)"

# We leave this in just in case, but it is not needed anymore.
RECURSE_FLAGS_TO_PASS = $(BASE_FLAGS_TO_PASS)

UNUSED_FLAGS_TO_PASS_WITH_VERBOSITY = $(RECURSE_FLAGS_TO_PASS) \
	"V=$(V_TO_PASS)"

# Flags to pass down to most sub-makes, in which we are building with
# the host environment.
EXTRA_HOST_FLAGS = \
	'AR=$(AR)' \
	'AS=$(AS)' \
	'CC=$(CC)' \
	'CPP=$(CPP)' \
	'CXX=$(CXX)' \
	'DLLTOOL=$(DLLTOOL)' \
	'GCJ=$(GCJ)' \
	'GFORTRAN=$(GFORTRAN)' \
	'GOC=$(GOC)' \
	'LD=$(LD)' \
	'LIPO=$(LIPO)' \
	'NM=$(NM)' \
	'OBJC=$(OBJC)' \
	'OBJCPP=$(OBJCPP)' \
	'OBJCXX=$(OBJCXX)' \
	'OBJCXXCPP=$(OBJCXXCPP)' \
	'OBJDUMP=$(OBJDUMP)' \
	'RANLIB=$(RANLIB)' \
	'READELF=$(READELF)' \
	'STRIP=$(STRIP)' \
	'WINDRES=$(WINDRES)' \
	'WINDMC=$(WINDMC)'

FLAGS_TO_PASS = $(BASE_FLAGS_TO_PASS) $(EXTRA_HOST_FLAGS)

# Flags that are concerned with the location of the X11 include files
# and library files
#
# NOTE: until the top-level is getting the values via autoconf, it only
# causes problems to have this top-level Makefile overriding the
# autoconf-set values in child directories.  Only variables that do NOT
# conflict with autoconf-ed ones should be passed by X11_FLAGS_TO_PASS
# for now:
X11_FLAGS_TO_PASS = \
	'X11_EXTRA_CFLAGS=$(X11_EXTRA_CFLAGS)' \
	'X11_EXTRA_LIBS=$(X11_EXTRA_LIBS)'

# Flags to pass down to makes which are built with the target environment.
# The double $ decreases the length of the command line; the variables
# are set in BASE_FLAGS_TO_PASS, and the sub-make will expand them.
EXTRA_TARGET_FLAGS = \
	'AR=$$(AR_FOR_TARGET)' \
	'AS=$$(AS_FOR_TARGET)' \
	'CC=$$(CC_FOR_TARGET)' \
	'CFLAGS=$$(CFLAGS_FOR_TARGET) $$(C_TARG_ARCHFLAGS)' \
	'CXX=$$(CXX_FOR_TARGET)' \
	'CXXFLAGS=$$(CXXFLAGS_FOR_TARGET) $$(CXX_TARG_ARCHFLAGS)' \
	'DLLTOOL=$$(DLLTOOL_FOR_TARGET)' \
	'GCJ=$$(GCJ_FOR_TARGET) $$(XGCC_FLAGS_FOR_TARGET) $$(TFLAGS)' \
	'GFORTRAN=$$(GFORTRAN_FOR_TARGET) $$(XGCC_FLAGS_FOR_TARGET) $$(TFLAGS)' \
	'GOC=$$(GOC_FOR_TARGET) $$(XGCC_FLAGS_FOR_TARGET) $$(TFLAGS)' \
	'GOCFLAGS=$$(GOCFLAGS_FOR_TARGET)' \
	'LD=$$(LD_FOR_TARGET)' \
	'LDFLAGS=$$(LDFLAGS_FOR_TARGET)' \
	'LIBCFLAGS=$$(LIBCFLAGS_FOR_TARGET)' \
	'LIBCXXFLAGS=$$(LIBCXXFLAGS_FOR_TARGET)' \
	'NM=$$(NM_FOR_TARGET)' \
	'OBJC=$$(OBJC_FOR_TARGET)' \
	'OBJCOPY=$$(OBJCOPY_FOR_TARGET)' \
	'OBJCPP=$$(OBJCPP_FOR_TARGET)' \
	'OBJCXX=$$(OBJCXX_FOR_TARGET)' \
	'OBJDUMP=$$(OBJDUMP_FOR_TARGET)' \
	'RANLIB=$$(RANLIB_FOR_TARGET)' \
	'READELF=$$(READELF_FOR_TARGET)' \
	'WINDRES=$$(WINDRES_FOR_TARGET)' \
	'WINDMC=$$(WINDMC_FOR_TARGET)'

TARGET_FLAGS_TO_PASS = $(BASE_FLAGS_TO_PASS) $(EXTRA_TARGET_FLAGS)

# Flags to pass down to gcc.  gcc builds a library, libgcc.a, so it
# unfortunately needs the native compiler and the target ar and
# ranlib.
# If any variables are added here, they must be added to do-*, below.
# The BUILD_* variables are a special case, which are used for the gcc
# cross-building scheme.
EXTRA_GCC_FLAGS = \
	'BUILD_PREFIX=$(BUILD_PREFIX)' \
	'BUILD_PREFIX_1=$(BUILD_PREFIX_1)' \
	"GCC_FOR_TARGET=$(GCC_FOR_TARGET)" \
	"`echo 'LANGUAGES=$(LANGUAGES)' | $(SED) -e s'/[^=][^=]*=$$/XFOO=/'`" \
	"`echo 'STMP_FIXPROTO=$(STMP_FIXPROTO)' | $(SED) -e s'/[^=][^=]*=$$/XFOO=/'`" \
	"`echo 'LIMITS_H_TEST=$(LIMITS_H_TEST)' | $(SED) -e s'/[^=][^=]*=$$/XFOO=/'`" \
	"`echo 'LIBGCC2_CFLAGS=$(LIBGCC2_CFLAGS)' | $(SED) -e s'/[^=][^=]*=$$/XFOO=/'`" \
	"`echo 'LIBGCC2_DEBUG_CFLAGS=$(LIBGCC2_DEBUG_CFLAGS)' | $(SED) -e s'/[^=][^=]*=$$/XFOO=/'`" \
	"`echo 'LIBGCC2_INCLUDES=$(LIBGCC2_INCLUDES)' | $(SED) -e s'/[^=][^=]*=$$/XFOO=/'`" \
	"`echo 'STAGE1_CFLAGS=$(STAGE1_CFLAGS)' | $(SED) -e s'/[^=][^=]*=$$/XFOO=/'`" \
	"`echo 'BOOT_CFLAGS=$(BOOT_CFLAGS)' | $(SED) -e s'/[^=][^=]*=$$/XFOO=/'`" \
	"`echo 'BOOT_ADAFLAGS=$(BOOT_ADAFLAGS)' | $(SED) -e s'/[^=][^=]*=$$/XFOO=/'`"

GCC_FLAGS_TO_PASS = $(BASE_FLAGS_TO_PASS) $(EXTRA_HOST_FLAGS) $(EXTRA_GCC_FLAGS)

.PHONY: configure-host
configure-host: [+
  FOR host_modules +] \
    maybe-configure-[+module+][+
  ENDFOR host_modules +]
.PHONY: configure-target
configure-target: [+
  FOR target_modules +] \
    maybe-configure-target-[+module+][+
  ENDFOR target_modules +]

# The target built for a native non-bootstrap build.
.PHONY: all
all: unstage all-host all-target stage

.PHONY: all-build
all-build: [+
  FOR build_modules +] \
    maybe-all-build-[+module+][+
  ENDFOR build_modules +]
.PHONY: all-host
all-host: [+
  FOR host_modules +] \
    maybe-all-[+module+][+
  ENDFOR host_modules +]
.PHONY: all-target
all-target: [+
  FOR target_modules +] \
    maybe-all-target-[+module+][+
  ENDFOR target_modules +]

# Do a target for all the subdirectories.  A ``make do-X'' will do a
# ``make X'' in all subdirectories (because, in general, there is a
# dependency (below) of X upon do-X, a ``make X'' will also do this,
# but it may do additional work as well).
[+ FOR recursive_targets +]
.PHONY: do-[+make_target+]
do-[+make_target+]: unstage [+make_target+]-host [+make_target+]-target stage

.PHONY: [+make_target+]-host
[+make_target+]-host: [+
  FOR host_modules +] \
    maybe-[+make_target+]-[+module+][+
  ENDFOR host_modules +]

.PHONY: [+make_target+]-target
[+make_target+]-target: [+
  FOR target_modules +] \
    maybe-[+make_target+]-target-[+module+][+
  ENDFOR target_modules +]
[+ ENDFOR recursive_targets +]

# Here are the targets which correspond to the do-X targets.

.PHONY: info installcheck dvi pdf html
.PHONY: install-info install-pdf install-html
.PHONY: clean distclean mostlyclean maintainer-clean realclean
.PHONY: local-clean local-distclean local-maintainer-clean
.PHONY: texinfo_js all-texinfo_js check-texinfo_js
info: do-info
installcheck: do-installcheck
dvi: do-dvi
pdf: do-pdf
html: do-html

doc docs: info dvi pdf html 
.PHONY: doc docs

# Make sure makeinfo is built before we do a `make info', if we're
# in fact building texinfo.
do-info: maybe-all-texinfo

texinfo/js/Makefile: texinfo/js/Makefile.in config.status
	./config.status texinfo/js/Makefile
texinfo_js: texinfo/js/Makefile
	$(MAKE) -C texinfo/js $(AM_V_MFLAG) $(FLAGS_TO_PASS)

all-texinfo_js: texinfo/js/Makefile
	$(MAKE) -C texinfo/js $(AM_V_MFLAG) $(FLAGS_TO_PASS) all
check-texinfo_js: texinfo/js/Makefile
	pushd texinfo/js; \
	$(MAKE) $(AM_V_MFLAG) $(FLAGS_TO_PASS) check || \
	$(MAKE) $(AM_V_MFLAG) $(FLAGS_TO_PASS) lint || \
	$(MAKE) $(AM_V_MFLAG) $(FLAGS_TO_PASS) check-types || \
	$(MAKE) $(AM_V_MFLAG) $(FLAGS_TO_PASS) uglify || \
	$(MAKE) $(AM_V_MFLAG) $(FLAGS_TO_PASS) modernizr || \
	$(MAKE) $(AM_V_MFLAG) $(FLAGS_TO_PASS) check-local || \
	stat Makefile; \
	popd

install-info: do-install-info dir.info
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	if [ -f dir.info ]; then \
	  $(INSTALL_DATA) dir.info $(DESTDIR)$(infodir)/dir.info; \
	else echo "dir.info is missing, so skipping installing it"; fi

install-pdf: do-install-pdf

install-html: do-install-html

local-clean:
	-rm -f *.a TEMP errs core *.o *~ \#* TAGS *.E *.log

distclean-local: local-distclean
.PHONY: distclean-local
local-distclean:
	-rm -f Makefile Makefile*orig
	-rm -f config.status config.cache ./*/config.cache ./*/config.cache.bak
	-rm -fv *~ config/*~
	-rm -f mh-frag mt-frag
	-rm -f multilib.out multilib.tmp maybedep.tmp serdep.tmp
	-if [ "$(TARGET_SUBDIR)" != "." ]; then \
	  rm -rf $(TARGET_SUBDIR); \
	else echo "vpath is irrelevant for target"; fi
	-rm -rf $(BUILD_SUBDIR)
	-if [ "$(HOST_SUBDIR)" != "." ]; then \
	  rm -rf $(HOST_SUBDIR); \
	else echo "vpath is irrelevant for host"; fi
	-rm -f texinfo/po/Makefile texinfo/po/Makefile.in texinfo/info/Makefile
	-rm -f texinfo/doc/Makefile texinfo/po/POTFILES
	-rmdir texinfo/doc texinfo/info texinfo/intl texinfo/lib 2>/dev/null
	-rmdir texinfo/makeinfo texinfo/po texinfo/util 2>/dev/null
	-rmdir fastjar gcc libiberty texinfo zlib 2>/dev/null
	-find . -name config.cache -exec rm -f {} \; \; 2>/dev/null
	-rm -rf autom4te.cache || rmdir autom4te.cache
	-rm -f .DS_Store
	-for dir in @build_configdirs@ @configdirs@ @target_configdirs@ @notsupp@; do \
	  if test -d $${dir} && test -e $${dir}/Makefile; then \
	    cd $${dir} && $(MAKE) $(AM_V_MFLAG) $(FLAGS_TO_PASS) distclean; \
	  elif test ! -d $${dir}; then \
	    echo "Skipping making nonexistent $${dir} be distclean"; \
	  elif test ! -e $${dir}/Makefile; then \
	    echo "Skipping making $${dir} be distclean (missing Makefile)"; \
	  else \
	    echo "Skipping making $${dir} be distclean for unknown reason"; \
	  fi; \
	done

local-maintainer-clean:
	@echo "This command is intended for maintainers to use;"
	@echo "it deletes files that may require special tools to rebuild."
	-rm -f configure.scan

clean: do-clean local-clean
mostlyclean: do-mostlyclean local-clean
distclean: do-distclean local-clean local-distclean
maintainer-clean: local-maintainer-clean do-maintainer-clean local-clean 
maintainer-clean: local-distclean
realclean: maintainer-clean

reconfigure_with_new_args_clean: clean
	-rm -fv ./*/config.cache

# Extra dependency for clean-target, owing to the mixed nature of gcc
clean-target: clean-target-libgcc
clean-target-libgcc:
	test ! -d gcc/libgcc || \
	(cd gcc/libgcc && find . -type d -print) | \
	while read d; do rm -f gcc/$${d}/libgcc.a || echo "nope"; done
	-rm -rf gcc/libgcc
	-rm -f gcc/stmp-dirs

# Check target.

.PHONY: check do-check
check: do-check

# Only include modules actually being configured and built.
do-check: unstage [+
  FOR host_modules +] \
    maybe-check-[+module+][+
  ENDFOR host_modules +][+
  FOR target_modules +] \
    maybe-check-target-[+module+][+
  ENDFOR target_modules +] stage

# Automated reporting of test results.

warning.log: build.log
	$(srcdir)/contrib/warn_summary build.log > $@

mail-report.log:
	if test x'$(BOOT_CFLAGS)' != x''; then \
	    BOOT_CFLAGS='$(BOOT_CFLAGS)'; export BOOT_CFLAGS; \
	fi; \
	$(srcdir)/contrib/test_summary -t >$@
	chmod +x $@
	echo If you really want to send e-mail, run ./$@ now

mail-report-with-warnings.log: warning.log
	if test x'$(BOOT_CFLAGS)' != x''; then \
	    BOOT_CFLAGS='$(BOOT_CFLAGS)'; export BOOT_CFLAGS; \
	fi; \
	$(srcdir)/contrib/test_summary -t -i warning.log >$@
	chmod +x $@
	echo If you really want to send e-mail, run ./$@ now

# Local Vim config

$(srcdir)/.local.vimrc: $(srcdir)/contrib/vimrc
	$(LN_S) -fv $(srcdir)/contrib/vimrc $@

$(srcdir)/.lvimrc: $(srcdir)/contrib/vimrc
	$(LN_S) -fv $(srcdir)/contrib/vimrc $@

vimrc: $(srcdir)/.local.vimrc $(srcdir)/.lvimrc

.PHONY: vimrc

# Installation targets.

.PHONY: install uninstall
install: installdirs install-host install-target

.PHONY: install-host-nogcc
install-host-nogcc: [+
  FOR host_modules +][+ IF (not (= (get "module") "gcc")) +] \
    maybe-install-[+module+][+ ENDIF +][+
  ENDFOR host_modules +]

.PHONY: install-host
install-host: [+
  FOR host_modules +] \
    maybe-install-[+module+][+
  ENDFOR host_modules +]

.PHONY: install-target
install-target: [+
  FOR target_modules +] \
    maybe-install-target-[+module+][+
  ENDFOR target_modules +]

uninstall:
	@echo "the uninstall target is not supported in this tree"

.PHONY: install.all
install.all: install-no-fixedincludes
	@if [ -f ./gcc/Makefile ]; then \
		r=`${PWD_COMMAND}`; export r; \
		$(HOST_EXPORTS) \
		(cd ./gcc && \
		$(MAKE) $(AM_V_MFLAG) $(FLAGS_TO_PASS) install-headers); \
	else \
		echo "no gcc subdir present for install.all"; \
	fi

# install-no-fixedincludes is used because Cygnus can not distribute
# the fixed header files.
.PHONY: install-no-fixedincludes
install-no-fixedincludes: installdirs install-host-nogcc \
	install-target gcc-no-fixedincludes

### other supporting targets

MAKEDIRS= \
	$(DESTDIR)$(prefix) \
	$(DESTDIR)$(exec_prefix)
.PHONY: installdirs
installdirs: mkinstalldirs
	$(SHELL) $(srcdir)/mkinstalldirs $(MAKEDIRS)

dir.info: do-install-info
	if [ -x $(srcdir)/texinfo/gen-info-dir ]; then \
	  $(srcdir)/texinfo/gen-info-dir $(DESTDIR)$(infodir) $(srcdir)/texinfo/dir.info-template > dir.info.new; \
	  mv -f dir.info.new dir.info; \
	else echo "missing script to generate texinfo info dir"; \
	fi

dist_target:
	@echo "Building a full distribution of this tree is NOT done"
	@echo "via 'make dist'. Check out the etc/ subdirectory" 
.PHONY: dist_target

etags tags: TAGS

# Right now this just builds TAGS in each subdirectory.  emacs19 has the
# ability to use several tags files at once, so there is probably no need
# to combine them into one big TAGS file (like CVS 1.3 does).  We could
# (if we felt like it) have this Makefile write a piece of elisp which
# the user could load to tell emacs19 where all the TAGS files we just
# built are.
TAGS: do-TAGS
.PHONY: TAGS

# ------------------------------------
# Macros for configure and all targets
# ------------------------------------

[+ DEFINE configure +]
.PHONY: configure-[+prefix+][+module+] maybe-configure-[+prefix+][+module+]
maybe-configure-[+prefix+][+module+]:
@if [+prefix+][+module+]
maybe-configure-[+prefix+][+module+]: configure-[+prefix+][+module+]
configure-[+prefix+][+module+]: [+deps+]
	$(AM_V_at)[+ IF bootstrap +]test -f stage_last && exit 0; \
	[+ ENDIF bootstrap +]test ! -f [+subdir+]/[+module+]/Makefile || exit 0; \
	$(SHELL) $(srcdir)/mkinstalldirs [+subdir+]/[+module+]; \
	r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	[+exports+] \
	echo Configuring in [+subdir+]/[+module+]; \
	cd "[+subdir+]/[+module+]" || exit 1; \
	case $(srcdir) in \
	  /* | [A-Za-z]:[\\/]*) topdir=$(srcdir) ;; \
	  *) topdir=`echo [+subdir+]/[+module+]/ | \
		$(SED) -e 's,\./,,g' -e 's,[^/]*/,../,g' `$(srcdir) ;; \
	esac; \
	srcdiroption="--srcdir=$${topdir}/[+module+]"; \
	libsrcdir="$${s}/[+module+]"; \
	[+ IF no-config-site +]rm -f no-such-file || echo "ok"; \
	if test -n "$${CPP}"; then unset CPP; else echo "CPP is empty"; fi; \
	CONFIG_SITE=no-such-file [+ ENDIF +]$(SHELL) $${libsrcdir}/configure \
	  [+args+] $${srcdiroption} [+extra_configure_flags+] \
	  || (if test -x $${libsrcdir}/configure && test -s $${libsrcdir}/configure && test -r $${libsrcdir}/configure; then \
	        (stat $${libsrcdir}/configure && wc -l $${libsrcdir}/configure); \
	        if test -r $${libsrcdir}/Makefile; then \
	          (stat $${libsrcdir}/Makefile && wc -l $${libsrcdir}/Makefile); \
	        else \
	          echo "$@: no readable Makefile available in $${libsrcdir}; cannot continue!" >&2; \
	          exit 1; \
	        fi;\
	      else \
	        (echo "$${libsrcdir}/configure missing/unusable!" >&2; \
	         echo "SHELL is $(SHELL)"; \
	         echo "args are [+args+]"; \
	         echo "srcdiroption is $${srcdiroption}"; \
	         echo "extra_configure_flags are '[+extra_configure_flags+]'"; \
	         exit 1); \
	      fi)
@endif [+prefix+][+module+]

[+ IF bootstrap +]
[+ FOR bootstrap_stage +]
.PHONY: configure-stage[+id+]-[+prefix+][+module+] maybe-configure-stage[+id+]-[+prefix+][+module+]
maybe-configure-stage[+id+]-[+prefix+][+module+]:
@if [+module+]-bootstrap
maybe-configure-stage[+id+]-[+prefix+][+module+]: configure-stage[+id+]-[+prefix+][+module+]
configure-stage[+id+]-[+prefix+][+module+]: [+deps+]
	$(AM_V_at)$(MAKE) $(AM_V_MFLAG) stage[+id+]-start
	$(AM_V_at)[ -f [+subdir+]/[+module+]/Makefile ] && exit 0 || echo "nvm"; \
	r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	[+exports+][+ IF prev +] \
	[+poststage1_exports+][+ ENDIF prev +] \
	echo Configuring stage [+id+] in [+subdir+]/[+module+]; \
	cd [+subdir+]/[+module+] || exit 1; \
	case $(srcdir) in \
	  /* | [A-Za-z]:[\\/]*) topdir=$(srcdir) ;; \
	  *) topdir=`echo [+subdir+]/[+module+]/ | \
		$(SED) -e 's,\./,,g' -e 's,[^/]*/,../,g' `$(srcdir) ;; \
	esac; \
	srcdiroption="--srcdir=$${topdir}/[+module+]"; \
	libsrcdir="$${s}/[+module+]"; \
	unset CPP && $(SHELL) $${libsrcdir}/configure \
	  [+args+] $${srcdiroption} \
	  [+stage_configure_flags+] [+extra_configure_flags+]
@endif [+module+]-bootstrap
[+ ENDFOR bootstrap_stage +]
[+ ENDIF bootstrap +]
[+ ENDDEF +]

[+ DEFINE all +]
.PHONY: all-[+prefix+][+module+] maybe-all-[+prefix+][+module+]
maybe-all-[+prefix+][+module+]:
@if [+prefix+][+module+]
TARGET-[+prefix+][+module+]=[+
  IF target +][+target+][+ ELSE +]all[+ ENDIF target +]
maybe-all-[+prefix+][+module+]: all-[+prefix+][+module+]
all-[+prefix+][+module+]: configure-[+prefix+][+module+]
	$(AM_V_at)[+ IF bootstrap +]test -f stage_last && exit 0; \
	[+ ENDIF bootstrap +]r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	[+exports+] \
	(cd [+subdir+]/[+module+] && unset CPP && \
	  $(MAKE) $(AM_V_MFLAG) [+args+] [+extra_make_flags+] $(TARGET-[+prefix+][+module+]))
@endif [+prefix+][+module+]

[+ IF bootstrap +]
[+ FOR bootstrap_stage +]
.PHONY: all-stage[+id+]-[+prefix+][+module+] maybe-all-stage[+id+]-[+prefix+][+module+]
.PHONY: clean-stage[+id+]-[+prefix+][+module+] maybe-clean-stage[+id+]-[+prefix+][+module+]
maybe-all-stage[+id+]-[+prefix+][+module+]:
maybe-clean-stage[+id+]-[+prefix+][+module+]:
@if [+module+]-bootstrap
maybe-all-stage[+id+]-[+prefix+][+module+]: all-stage[+id+]-[+prefix+][+module+]
all-stage[+id+]: all-stage[+id+]-[+prefix+][+module+]
all-stage[+id+]-[+prefix+][+module+]: configure-stage[+id+]-[+prefix+][+module+]
	$(AM_V_at)$(MAKE) $(AM_V_MFLAG) stage[+id+]-start
	$(AM_V_at)r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	[+exports+][+ IF prev +] \
	[+poststage1_exports+][+ ENDIF prev +] \
	cd [+subdir+]/[+module+] && unset CPP && \
	$(MAKE) $(AM_V_MFLAG) [+args+] [+ IF prev
		+][+poststage1_args+][+ ENDIF prev
		+] [+stage_make_flags+] [+extra_make_flags+]

maybe-clean-stage[+id+]-[+prefix+][+module+]: clean-stage[+id+]-[+prefix+][+module+]
clean-stage[+id+]: clean-stage[+id+]-[+prefix+][+module+]
clean-stage[+id+]-[+prefix+][+module+]:
	$(AM_V_at)[ -f [+subdir+]/[+module+]/Makefile ] || [ -f [+subdir+]/stage[+id+]-[+module+]/Makefile ] \
	  || exit 0; \
	[ -f [+subdir+]/[+module+]/Makefile ] || $(MAKE) $(AM_V_MFLAG) stage[+id+]-start; \
	cd [+subdir+]/[+module+] && \
	$(MAKE) $(AM_V_MFLAG) [+args+] [+ IF prev +] \
		[+poststage1_args+] [+ ENDIF prev +] \
		[+stage_make_flags+] [+extra_make_flags+] clean
@endif [+module+]-bootstrap

[+ ENDFOR bootstrap_stage +]
[+ ENDIF bootstrap +]
[+ ENDDEF +]

# --------------------------------------
# Modules which run on the build machine
# --------------------------------------
[+ FOR build_modules +]
[+ configure prefix="build-" subdir="$(BUILD_SUBDIR)" exports="$(BUILD_EXPORTS)"
	     args="$(BUILD_CONFIGARGS)" no-config-site=true +]

[+ all prefix="build-" subdir="$(BUILD_SUBDIR)" exports="$(BUILD_EXPORTS)" +]
[+ ENDFOR build_module +]

# --------------------------------------
# Modules which run on the host machine
# --------------------------------------
[+ FOR host_modules +]
[+ configure prefix="" subdir="$(HOST_SUBDIR)"
	     exports="$(HOST_EXPORTS)"
	     poststage1_exports="$(POSTSTAGE1_HOST_EXPORTS)"
	     args="$(HOST_CONFIGARGS)" +]

[+ all prefix="" subdir="$(HOST_SUBDIR)"
       exports="$(HOST_EXPORTS)"
       poststage1_exports="$(POSTSTAGE1_HOST_EXPORTS)"
       args="$(FLAGS_TO_PASS)"
       stage1_args="$(STAGE1_FLAGS_TO_PASS)"
       poststage1_args="$(POSTSTAGE1_FLAGS_TO_PASS)" +]

.PHONY: check-[+module+] maybe-check-[+module+]
maybe-check-[+module+]:
@if [+module+]
maybe-check-[+module+]: check-[+module+]
[+ IF no_check +]
check-[+module+]:
[+ ELIF no_check_cross +]
# This module is only tested in a native toolchain.
check-[+module+]:
	$(AM_V_at)if [ 'x$(host)' = 'x$(target)' ]; then \
	  r=`${PWD_COMMAND}`; export r; \
	  s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	  $(HOST_EXPORTS) [+ IF bootstrap +]$(EXTRA_HOST_EXPORTS)[+
	  ENDIF bootstrap +] \
	  (cd $(HOST_SUBDIR)/[+module+] && \
	    $(MAKE) $(AM_V_MFLAG) $(FLAGS_TO_PASS) [+extra_make_flags+][+
	    IF bootstrap +] $(EXTRA_BOOTSTRAP_FLAGS)[+ ENDIF bootstrap +] check); \
	else \
	  echo "$@ is only tested in a native toolchain."; \
	fi
[+ ELSE check +]
check-[+module+]:
	$(AM_V_at)r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(HOST_EXPORTS) \
	(cd $(HOST_SUBDIR)/[+module+] && \
	  $(MAKE) $(AM_V_MFLAG) $(FLAGS_TO_PASS) [+extra_make_flags+] check)
[+ ENDIF no_check +]
@endif [+module+]

.PHONY: install-[+module+] maybe-install-[+module+]
maybe-install-[+module+]:
@if [+module+]
maybe-install-[+module+]: install-[+module+]
[+ IF no_install +]
install-[+module+]:
[+ ELSE install +]
install-[+module+]: installdirs
	$(AM_V_at)r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(HOST_EXPORTS) \
	(cd $(HOST_SUBDIR)/[+module+] && \
	  $(MAKE) $(AM_V_MFLAG) $(FLAGS_TO_PASS) [+extra_make_flags+] install)
[+ ENDIF no_install +]
@endif [+module+]

# Other targets (info, dvi, etc.)
[+ FOR recursive_targets +]
.PHONY: maybe-[+make_target+]-[+module+] [+make_target+]-[+module+]
maybe-[+make_target+]-[+module+]:
@if [+module+]
maybe-[+make_target+]-[+module+]: [+make_target+]-[+module+]
[+ IF (match-value? = "missing" (get "make_target") ) +]
# [+module+] doesn't support [+make_target+].
[+make_target+]-[+module+]:
[+ ELSE +]
[+make_target+]-[+module+]: [+
  FOR depend +]\
    [+depend+]-[+module+] [+
  ENDFOR depend +]
	$(AM_V_at)[ -f ./[+module+]/Makefile ] || exit 0; \
	r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(HOST_EXPORTS) \
	for flag in $(EXTRA_HOST_FLAGS) [+extra_make_flags+]; do \
	  eval `echo "$${flag}" | $(SED) -e "s|^\([^=]*\)=\(.*\)|\1='\2'; export \1|"`; \
	done; \
	echo "Doing [+make_target+] in [+module+]"; \
	(cd $(HOST_SUBDIR)/[+module+] && \
	  $(MAKE) $(AM_V_MFLAG) $(BASE_FLAGS_TO_PASS) "AR=$${AR}" "AS=$${AS}" \
	          "CC=$${CC}" "CXX=$${CXX}" "LD=$${LD}" "NM=$${NM}" \
	          "RANLIB=$${RANLIB}" \
	          "DLLTOOL=$${DLLTOOL}" "WINDRES=$${WINDRES}" \
	          [+make_target+]) \
	  || exit 1
[+ ENDIF +]
@endif [+module+]
[+ ENDFOR recursive_targets +]
[+ ENDFOR host_modules +]

# ---------------------------------------
# Modules which run on the target machine
# ---------------------------------------
[+ FOR target_modules +]

# There is only one multilib.out. Cleverer subdirs should NOT need it copied.
@if target-[+module+]
$(TARGET_SUBDIR)/[+module+]/multilib.out: multilib.out
	$(SHELL) $(srcdir)/mkinstalldirs $(TARGET_SUBDIR)/[+module+]; \
	rm -f $(TARGET_SUBDIR)/[+module+]/Makefile || echo "fine"; \
	cp multilib.out $(TARGET_SUBDIR)/[+module+]/multilib.out
@endif target-[+module+]

[+ IF raw_cxx +]
[+ configure prefix="target-" subdir="$(TARGET_SUBDIR)"
	     deps=(string-append "$(TARGET_SUBDIR)/" (get "module") "/multilib.out")
	     exports="$(RAW_CXX_TARGET_EXPORTS)"
	     args="$(TARGET_CONFIGARGS)" no-config-site=true +]

[+ all prefix="target-" subdir="$(TARGET_SUBDIR)"
       exports="$(RAW_CXX_TARGET_EXPORTS)"
       args="$(TARGET_FLAGS_TO_PASS) 'CXX=$$(RAW_CXX_FOR_TARGET)' 'CXX_FOR_TARGET=$$(RAW_CXX_FOR_TARGET)'" +]
[+ ELSE +]
[+ configure prefix="target-" subdir="$(TARGET_SUBDIR)"
	     deps=(string-append "$(TARGET_SUBDIR)/" (get "module") "/multilib.out")
	     exports="$(NORMAL_TARGET_EXPORTS)"
	     args="$(TARGET_CONFIGARGS)" no-config-site=true +]

[+ all prefix="target-" subdir="$(TARGET_SUBDIR)"
       exports="$(NORMAL_TARGET_EXPORTS)"
       args="$(TARGET_FLAGS_TO_PASS)" +]
[+ ENDIF +]

.PHONY: check-target-[+module+] maybe-check-target-[+module+]
maybe-check-target-[+module+]:
@if target-[+module+]
maybe-check-target-[+module+]: check-target-[+module+]
[+ IF no_check +]
# Dummy target for uncheckable module.
check-target-[+module+]:
[+ ELSE check +]
check-target-[+module+]:
	$(AM_V_at)r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \[+
IF raw_cxx +]
	$(RAW_CXX_TARGET_EXPORTS) \[+
ELSE normal_cxx +]
	$(NORMAL_TARGET_EXPORTS) \[+
ENDIF raw_cxx +]
	(cd $(TARGET_SUBDIR)/[+module+] && \
	  $(MAKE) $(AM_V_MFLAG) $(TARGET_FLAGS_TO_PASS) [+
	    IF raw_cxx 
	      +] 'CXX=$$(RAW_CXX_FOR_TARGET)' 'CXX_FOR_TARGET=$$(RAW_CXX_FOR_TARGET)' [+ 
	    ENDIF raw_cxx 
	  +] [+extra_make_flags+] check)
[+ ENDIF no_check +]
@endif target-[+module+]

.PHONY: install-target-[+module+] maybe-install-target-[+module+]
maybe-install-target-[+module+]:
@if target-[+module+]
maybe-install-target-[+module+]: install-target-[+module+]
[+ IF no_install +]
# Dummy target for uninstallable.
install-target-[+module+]:
[+ ELSE install +]
install-target-[+module+]: installdirs
	$(AM_V_at)r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \[+
IF raw_cxx +]
	$(RAW_CXX_TARGET_EXPORTS) \[+
ELSE normal_cxx +]
	$(NORMAL_TARGET_EXPORTS) \[+
ENDIF raw_cxx +]
	(cd $(TARGET_SUBDIR)/[+module+] && \
	  $(MAKE) $(AM_V_MFLAG) $(TARGET_FLAGS_TO_PASS) [+extra_make_flags+] install)
[+ ENDIF no_install +]
@endif target-[+module+]

# Other targets (info, dvi, etc.)
[+ FOR recursive_targets +]
.PHONY: maybe-[+make_target+]-target-[+module+] [+make_target+]-target-[+module+]
maybe-[+make_target+]-target-[+module+]:
@if target-[+module+]
maybe-[+make_target+]-target-[+module+]: [+make_target+]-target-[+module+]
[+ IF (match-value? = "missing" (get "make_target") ) +]
# [+module+] does NOT support [+make_target+].
[+make_target+]-target-[+module+]:
[+ ELSE +]
[+make_target+]-target-[+module+]: [+
  FOR depend +]\
    [+depend+]-target-[+module+] [+
  ENDFOR depend +]
	$(AM_V_at)[ -f $(TARGET_SUBDIR)/[+module+]/Makefile ] || exit 0; \
	r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \[+
IF raw_cxx +]
	$(RAW_CXX_TARGET_EXPORTS) \[+
ELSE normal_cxx +]
	$(NORMAL_TARGET_EXPORTS) \[+
ENDIF raw_cxx +]
	echo "Doing [+make_target+] in $(TARGET_SUBDIR)/[+module+]"; \
	for flag in $(EXTRA_TARGET_FLAGS); do \
	  eval `echo "$${flag}" | $(SED) -e "s|^\([^=]*\)=\(.*\)|\1='\2'; export \1|"`; \
	done; \
	(cd $(TARGET_SUBDIR)/[+module+] && \
	  $(MAKE) $(AM_V_MFLAG) $(BASE_FLAGS_TO_PASS) "AR=$${AR}" "AS=$${AS}" \
	          "CC=$${CC}" "CXX=$${CXX}" "LD=$${LD}" "NM=$${NM}" \
	          "RANLIB=$${RANLIB}" \
	          "DLLTOOL=$${DLLTOOL}" "WINDRES=$${WINDRES}" \
	          [+extra_make_flags+] [+make_target+]) \
	  || exit 1
[+ ENDIF +]
@endif target-[+module+]
[+ ENDFOR recursive_targets +]
[+ ENDFOR target_modules +]

# ----------
# GCC module
# ----------

@if gcc-no-bootstrap
# GCC has some more recursive targets, which trigger the old
# (but still current, until the toplevel bootstrap project
# is finished) compiler bootstrapping rules.

GCC_STRAP_TARGETS = bootstrap bootstrap-lean bootstrap2 bootstrap2-lean bootstrap3 bootstrap3-lean bootstrap4 bootstrap4-lean bubblestrap quickstrap cleanstrap restrap
.PHONY: $(GCC_STRAP_TARGETS)
$(GCC_STRAP_TARGETS): all-prebootstrap configure-gcc
	$(AM_V_at)r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(HOST_EXPORTS) \
	echo "Bootstrapping the compiler"; \
	$(RPATH_ENVVAR)=`echo "$(TARGET_LIB_PATH)$$$(RPATH_ENVVAR)" | $(SED) 's,:[ :]*,:,g;s,^[ :]*,,;s,:*$$,,'`; export $(RPATH_ENVVAR); \
	cd gcc && $(MAKE) $(AM_V_MFLAG) $(GCC_FLAGS_TO_PASS) $@
	$(AM_V_at)r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	case "$@" in \
	  *bootstrap4-lean ) \
	    msg="Comparing stage3 and stage4 of the compiler"; \
	    compare=compare3-lean ;; \
	  *bootstrap4 ) \
	    msg="Comparing stage3 and stage4 of the compiler"; \
	    compare=compare3 ;; \
	  *-lean ) \
	    msg="Comparing stage2 and stage3 of the compiler"; \
	    compare=compare-lean ;; \
	  * ) \
	    msg="Comparing stage2 and stage3 of the compiler"; \
	    compare=compare ;; \
	esac; \
	$(HOST_EXPORTS) \
	echo "$${msg}"; \
	cd gcc && $(MAKE) $(AM_V_MFLAG) $(GCC_FLAGS_TO_PASS) $${compare}
	$(AM_V_at)r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	echo "Building runtime libraries"; \
	$(MAKE) $(AM_V_MFLAG) $(RECURSE_FLAGS_TO_PASS) all

profiledbootstrap: all-prebootstrap configure-gcc
	$(AM_V_at)r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(HOST_EXPORTS) \
	$(RPATH_ENVVAR)=`echo "$(TARGET_LIB_PATH)$$$(RPATH_ENVVAR)" | $(SED) 's,:[ :]*,:,g;s,^[ :]*,,;s,:*$$,,'`; export $(RPATH_ENVVAR); \
	echo "Bootstrapping training compiler"; \
	cd gcc && $(MAKE) $(AM_V_MFLAG) $(GCC_FLAGS_TO_PASS) stageprofile_build
	$(AM_V_at)r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(HOST_EXPORTS) \
	echo "Building feedback based compiler"; \
	cd gcc && $(MAKE) $(AM_V_MFLAG) $(GCC_FLAGS_TO_PASS) stagefeedback_build
	$(AM_V_at)r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	echo "Building runtime libraries"; \
	$(MAKE) $(AM_V_MFLAG) $(RECURSE_FLAGS_TO_PASS) all

.PHONY: cross
cross: all-build all-gas all-ld
	$(AM_V_at)r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	$(HOST_EXPORTS) \
	echo "Building the C and C++ compiler"; \
	cd gcc && $(MAKE) $(AM_V_MFLAG) $(GCC_FLAGS_TO_PASS) LANGUAGES="c c++"
	$(AM_V_at)r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	echo "Building runtime libraries"; \
	$(MAKE) $(AM_V_MFLAG) $(RECURSE_FLAGS_TO_PASS) LANGUAGES="c c++" all
@endif gcc-no-bootstrap

@if gcc
.PHONY: check-gcc-c++
check-gcc-c++:
	$(AM_V_at)if [ -f ./gcc/Makefile ]; then \
	  r=`${PWD_COMMAND}`; export r; \
	  s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	  $(HOST_EXPORTS) \
	  (cd gcc && $(MAKE) $(AM_V_MFLAG) $(GCC_FLAGS_TO_PASS) check-c++); \
	else \
	  echo "gcc Makefile is missing, so skipping checking it"; \
	fi

.PHONY: check-c++
check-c++: check-target-libstdc++-v3 check-gcc-c++

# Install the gcc headers files, but not the fixed include files,
# which Cygnus is not allowed to distribute.  This rule is very
# dependent on the workings of the gcc Makefile.in.
.PHONY: gcc-no-fixedincludes
gcc-no-fixedincludes:
	@if [ -f ./gcc/Makefile ]; then \
	  rm -rf gcc/tmp-include; \
	  mv gcc/include gcc/tmp-include 2>/dev/null; \
	  mkdir gcc/include; \
	  cp $(srcdir)/gcc/gsyslimits.h gcc/include/syslimits.h; \
	  touch gcc/stmp-fixinc gcc/include/fixed; \
	  rm -f gcc/stmp-headers gcc/stmp-int-hdrs; \
	  r=`${PWD_COMMAND}`; export r; \
	  s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	  $(HOST_EXPORTS) \
	  (cd ./gcc && \
	   $(MAKE) $(AM_V_MFLAG) $(GCC_FLAGS_TO_PASS) install); \
	  rm -rf gcc/include; \
	  mv gcc/tmp-include gcc/include 2>/dev/null; \
	else echo "gcc Makefile is missing, so skipping it"; fi
@endif gcc

# ---------------------
# GCC bootstrap support
# ---------------------

# We track the current stage (the one in 'gcc') in the stage_current file.
# stage_last instead tracks the stage that was built last.  These targets
# are dummy when toplevel bootstrap is not active.

.PHONY: unstage
unstage:
@if gcc-bootstrap
	$(AM_V_at)[ -f stage_current ] || $(MAKE) $(AM_V_MFLAG) `cat stage_last`-start
@endif gcc-bootstrap

.PHONY: stage
stage:
@if gcc-bootstrap
	$(AM_V_at)$(MAKE) $(AM_V_MFLAG) `cat stage_current`-end
@endif gcc-bootstrap

# We name the build directories for the various stages "stage1-gcc",
# "stage2-gcc","stage3-gcc", etc.

# Since the 'compare' process will fail (on debugging information) if any
# directory names are different, we need to link the gcc directory for
# the previous stage to a constant name ('gcc-prev'), and to make the name of
# the build directories constant as well. For the latter, we use naked names
# like 'gcc', because the scripts in that directory assume it.  We use
# mv on platforms where symlinks to directories do not work or are not
# reliable.

# At the end of the bootstrap, a symlink to 'stage3-gcc' named 'gcc' must
# be kept, so that libraries can find it. Ick!

# It would be best to preinstall gcc into a staging area (and in the
# future, gather there all prebootstrap packages).  This would allow
# assemblers and linkers can be bootstrapped as well as the compiler
# (both in a combined tree, or separately). This however requires some
# change to the gcc driver, again in order to avoid comparison failures.

# Bugs: This is crippled when doing parallel make, the `make all-host'
# and `make all-target' phases can be parallelized.


# 'touch' does NOT work right on some platforms.
STAMP = echo timestamp > 

# Only build the C compiler for stage1, because that is the only one that
# we can guarantee will build with the native compiler, and also it is the
# only thing useful for building stage2. STAGE1_CFLAGS (via CFLAGS),
# MAKEINFO and MAKEINFOFLAGS are explicitly passed here to make them
# overrideable (for a bootstrap build stage1 also builds gcc.info).

STAGE1_CFLAGS=@stage1_cflags@
STAGE1_LANGUAGES=@stage1_languages@

# We only want to compare .o files, so set this!
objext = .o

# Flags to pass to stage1 or when not bootstrapping:
STAGE1_FLAGS_TO_PASS = \
	LDFLAGS="$${LDFLAGS}" \
	HOST_LIBS="$${HOST_LIBS}"

# Flags to pass to stage2 and later makes:
POSTSTAGE1_FLAGS_TO_PASS = \
	CC="$${CC}" CC_FOR_BUILD="$${CC_FOR_BUILD}" \
	CPP="$${CPP}" CPP_FOR_BUILD="$${CPP_FOR_BUILD}" \
	CXX="$${CXX}" CXX_FOR_BUILD="$${CXX_FOR_BUILD}" \
	STAGE_PREFIX=$${r}/prev-gcc/ \
	GNATBIND="$${GNATBIND}" \
	CFLAGS="$(BOOT_CFLAGS)" \
	LDFLAGS="$${LDFLAGS}" \
	HOST_LIBS="$${HOST_LIBS}" \
	$(LTO_FLAGS_TO_PASS) \
	ADAC="\$$(CC)" \
	"`echo 'ADAFLAGS=$(BOOT_ADAFLAGS)' | sed -e s'/[^=][^=]*=$$/XFOO=/'`"

@if gcc-bootstrap
EXTRA_HOST_EXPORTS = if [ "x$(current_stage)" != "xstage1" ]; then \
		       $(POSTSTAGE1_HOST_EXPORTS) \
		     fi;

EXTRA_BOOTSTRAP_FLAGS = CC="$${CC}" CXX="$${CXX}" LDFLAGS="$${LDFLAGS}"
@endif gcc-bootstrap

# For stage 1:
# * We force-disable intermodule optimizations, even if
#   --enable-intermodule was passed, since the installed compiler probably
#   cannot handle them. Luckily, autoconf always respects
#   the last argument when conflicting --enable arguments are passed.
# * Likewise, we force-disable coverage flags, since the installed compiler
#   probably has never heard of them.
# * We build only C (and possibly Ada).

[+ FOR bootstrap-stage +]
.PHONY: stage[+id+]-start stage[+id+]-end

stage[+id+]-start::
	$(AM_V_at)[ -f stage_current ] && $(MAKE) $(AM_V_MFLAG) `cat stage_current`-end || echo "nope"; \
	echo stage[+id+] > stage_current; \
	echo stage[+id+] > stage_last; \
	$(SHELL) $(srcdir)/mkinstalldirs $(HOST_SUBDIR) $(TARGET_SUBDIR)[+
   FOR host_modules +][+ IF bootstrap +]
@if [+ module +]
	$(AM_V_at)cd $(HOST_SUBDIR); [ -d stage[+id+]-[+module+] ] || \
	  mkdir stage[+id+]-[+module+]; \
	set stage[+id+]-[+module+] [+module+]; \
	@CREATE_LINK_TO_DIR@ [+ IF prev +]; \
	set stage[+prev+]-[+module+] prev-[+module+]; \
	@CREATE_LINK_TO_DIR@ [+ ENDIF prev +]
@endif [+ module +][+ ENDIF bootstrap +][+ ENDFOR host_modules +][+
   FOR target_modules +][+ IF bootstrap +]
@if target-[+ module +]
	$(AM_V_at)cd $(TARGET_SUBDIR); [ -d stage[+id+]-[+module+] ] || \
	  mkdir stage[+id+]-[+module+]; \
	set stage[+id+]-[+module+] [+module+]; \
	@CREATE_LINK_TO_DIR@ [+ IF prev +]; \
	set stage[+prev+]-[+module+] prev-[+module+]; \
	@CREATE_LINK_TO_DIR@ [+ ENDIF prev +]
@endif target-[+ module +][+ ENDIF bootstrap +][+ ENDFOR target_modules +]

stage[+id+]-end::
	$(AM_V_at)rm -f stage_current[+ FOR host_modules +][+ IF bootstrap +]
@if [+ module +]
	$(AM_V_at)cd $(HOST_SUBDIR); set [+module+] stage[+id+]-[+module+]; \
	@UNDO_LINK_TO_DIR@ [+ IF prev +]; \
	set prev-[+module+] stage[+prev+]-[+module+]; \
	@UNDO_LINK_TO_DIR@ [+ ENDIF prev +]
@endif [+ module +][+ ENDIF bootstrap +][+ ENDFOR host_modules +][+
   FOR target_modules +][+ IF bootstrap +]
@if target-[+ module +]
	$(AM_V_at)cd $(HOST_SUBDIR); set [+module+] stage[+id+]-[+module+]; \
	@UNDO_LINK_TO_DIR@ [+ IF prev +]; \
	set prev-[+module+] stage[+prev+]-[+module+]; \
	@UNDO_LINK_TO_DIR@ [+ ENDIF prev +]
@endif [+ module +][+ ENDIF bootstrap +][+ ENDFOR target_modules +]

# Bubble a bugfix through all the stages up to stage [+id+].  They
# are remade, but not reconfigured.  The next stage (if any) will not
# be reconfigured as well.
.PHONY: stage[+id+]-bubble
stage[+id+]-bubble:: [+ IF prev +]stage[+prev+]-bubble[+ ENDIF +][+IF lean +]
	@bootstrap_lean@-rm -rf stage[+lean+]-*; $(STAMP) stage[+lean+]-lean[+ ENDIF lean +]
	$(AM_V_at)if test -f stage[+id+]-lean [+
	  IF prev +]|| test -f stage[+prev+]-lean [+ ENDIF prev +]; then \
	  echo "Skipping rebuild of stage[+id+]"; \
	else \
	  $(MAKE) $(AM_V_MFLAG) $(RECURSE_FLAGS_TO_PASS) NOTPARALLEL= all-stage[+id+]; \
	fi

.PHONY: all-stage[+id+] clean-stage[+id+]
do-clean: clean-stage[+id+]

# FIXME: Will not need to be conditional when toplevel bootstrap is the
# only possibility, but now it conflicts with no-bootstrap rules
@if gcc-bootstrap
[+ IF compare-target +]
[+compare-target+]:
	$(AM_V_at)if test -f stage[+prev+]-lean; then \
	  echo "Cannot compare object files as stage [+prev+] was deleted."; \
	  exit 0; \
	fi; \
	[ -f stage_current ] && $(MAKE) $(AM_V_MFLAG) `cat stage_current`-end || echo "whatever"; \
	$(AM_V_at)r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	rm -f .bad_compare; \
	cd stage[+id+]-gcc; \
	files=`find . -name "*$(objext)" -print`; \
	cd ..; \
	for file in $${files}; do \
	  f1=$${r}/stage[+prev+]-gcc/$${file}; f2=$${r}/stage[+id+]-gcc/$${file}; \
	  @do_compare@ > /dev/null 2>&1; \
	  test $$? -eq 1 && echo "$${file} differs" >> .bad_compare || true; \
	done; \
	if [ -f .bad_compare ]; then \
	  echo "Bootstrap comparison failure!"; \
	  cat .bad_compare; \
	  exit 1; \
	else \
	  echo "Bootstrap comparison succeeded!"; \
	fi; \
	$(STAMP) [+compare-target+][+ IF prev +]
	@bootstrap_lean@-rm -rf stage[+prev+]-*; $(STAMP) stage[+prev+]-lean[+ ENDIF prev +]
[+ ENDIF compare-target +]

[+ IF bootstrap-target +]
.PHONY: [+bootstrap-target+]
[+bootstrap-target+]: stage[+id+]-bubble [+compare-target+] all
[+ ENDIF bootstrap-target +]

# Rules to wipe a stage and all the following ones, also used for cleanstrap
[+ IF prev +]distclean-stage[+prev+]:: distclean-stage[+id+] [+ ENDIF prev +]
.PHONY: distclean-stage[+id+]
distclean-stage[+id+]::
	[ -f stage_current ] && $(MAKE) $(AM_V_MFLAG) `cat stage_current`-end || echo "k"
	rm -rf stage[+id+]-* [+
	  IF compare-target +][+compare-target+] [+ ENDIF compare-target +]

[+ IF cleanstrap-target +]
.PHONY: [+cleanstrap-target+]
[+cleanstrap-target+]: distclean [+bootstrap-target+]
[+ ENDIF cleanstrap-target +]
@endif gcc-bootstrap

[+ ENDFOR bootstrap-stage +]

stageprofile-end::
	$(MAKE) $(AM_V_MFLAG) distclean-stagefeedback

stagefeedback-start::
	$(AM_V_at)r=`${PWD_COMMAND}`; export r; \
	s=`cd $(srcdir); ${PWD_COMMAND}`; export s; \
	for i in prev-*; do \
	  j=`echo $${i} | $(SED) s/^prev-//`; \
	  cd $${r}/$${i} && \
	  { find . -type d | sort | $(SED) 's,.*,$(SHELL) '"$${s}"'/mkinstalldirs "../'$${j}'/&",' | $(SHELL); } && \
	  { find . -name '*.*da' | $(SED) 's,.*,$(LN) -f "&" "../'$${j}'/&",' | $(SHELL); }; \
	done

@if gcc-bootstrap
NOTPARALLEL = .NOTPARALLEL
$(NOTPARALLEL):
do-distclean: distclean-stage1
@endif gcc-bootstrap

# --------------------------------------
# Dependencies between different modules
# --------------------------------------

# Generic dependencies for target modules on host stuff, especially gcc
[+ FOR target_modules +][+ IF bootstrap +]
@if gcc-bootstrap[+ FOR bootstrap_stage +]
configure-stage[+id+]-target-[+module+]: maybe-all-stage[+id+]-gcc[+ ENDFOR +]
@endif gcc-bootstrap[+ ENDIF bootstrap +]
configure-target-[+module+]: maybe-all-gcc
[+ ENDFOR target_modules +]

[+ FOR lang_env_dependencies +]
configure-target-[+module+]: maybe-all-target-newlib maybe-all-target-libgloss
[+ IF cxx +]configure-target-[+module+]: maybe-all-target-libstdc++-v3
[+ ENDIF cxx +][+ ENDFOR lang_env_dependencies +]

# There are two types of dependencies here: 'hard' dependencies, where one
# module simply will NOT build without the other; and 'soft' dependencies, where
# if the depended-on module is missing, the depending module will do without
# or find a substitute somewhere (perhaps installed). Soft dependencies
# are made here to depend on a 'maybe-' target. If you are not sure,
# it is safer to use a soft dependency.

[+ ;; These Scheme functions build the bulk of the dependencies.
   ;; dep-target builds a string like "maybe-all-MODULE_KIND-gcc",
   ;; where "maybe-" is only included if HARD is true, and all-gcc
   ;; is taken from VAR-NAME.
   (define dep-target (lambda (module-kind var-name hard)
      (string-append
         (if hard "" "maybe-")
         (dep-subtarget var-name)
         module-kind
         (dep-module var-name)
      )))

   ;; make-dep builds a dependency from the MODULE and ON AutoGen vars.
   (define make-dep (lambda (module-kind on-kind)
      (string-append
         (dep-target module-kind "module" #t) ": "
         (dep-target on-kind "on" (exist? "hard")))))

   ;; dep-subtarget extracts everything up to the first dash in the given
   ;; AutoGen variable, for example it extracts "all-" out of "all-gcc".
   (define dep-subtarget (lambda (var-name)
      (substring (get var-name) 0 (+ 1 (string-index (get var-name) #\-)))))

   ;; dep-module extracts everything up to the first dash in the given
   ;; AutoGen variable, for example it extracts "gcc" out of "all-gcc".
   (define dep-module (lambda (var-name)
      (substring (get var-name) (+ 1 (string-index (get var-name) #\-)))))

   ;; dep-stage builds a string for the prefix of a bootstrap stage.
   (define dep-stage (lambda ()
      (string-append
	 "stage"
	 (get "id")
	 "-")))

   ;; dep-maybe is the same as the AutoGen expression "- hard 'maybe-'"
   ;; but is written in Scheme.
   (define dep-maybe (lambda ()
      (if (exist? "hard") "" "maybe-")))

   ;; dep-kind returns "normal" if the dependency is on an "install" target,
   ;; or if either module is not bootstrapped.  It returns "bootstrap" for
   ;; configure or build dependencies between bootstrapped modules; it returns
   ;; "prebootstrap" for configure or build dependencies of bootstrapped
   ;; modules on a build module (e.g. all-gcc on all-build-bison).  All this
   ;; is only necessary for host modules.
   (define dep-kind (lambda ()
      (if (and (hash-ref boot-modules (dep-module "module"))
	       (=* (dep-module "on") "build-"))
	  "prebootstrap"

	  (if (or (= (dep-subtarget "on") "install-")
		  (not (hash-ref boot-modules (dep-module "module")))
		  (not (hash-ref boot-modules (dep-module "on"))))
              "normal"
	      "bootstrap"))))

   ;; We now build the hash table that is used by dep-kind.
   (define boot-modules (make-hash-table 113))
+]

[+ FOR host_modules +][+
   (if (exist? "bootstrap")
       (hash-create-handle! boot-modules (get "module") #t))
   "" +][+ ENDFOR host_modules +]
[+ FOR target_modules +][+
   (if (exist? "bootstrap")
       (hash-create-handle! boot-modules (string-append "target-" (get "module")) #t))
   "" +][+ ENDFOR target_modules +]

# With all the machinery above in place, it is pretty easy to generate
# dependencies.  Host dependencies are a bit more complex because we have
# to check for bootstrap/prebootstrap dependencies.  To resolve
# prebootstrap dependencies, prebootstrap modules are gathered in
# a hash table.
[+ FOR dependencies +][+ (make-dep "" "") +]
[+ CASE (dep-kind) +]
[+ == "prebootstrap"
     +][+ FOR bootstrap_stage +]
[+ (make-dep (dep-stage) "") +][+
       ENDFOR bootstrap_stage +]
all-prebootstrap: [+ (dep-target "" "on" (exist? "hard")) +]
[+ == "bootstrap"
     +][+ FOR bootstrap_stage +]
[+ (make-dep (dep-stage) (dep-stage)) +][+
       ENDFOR bootstrap_stage +]
[+ ESAC +][+
ENDFOR dependencies +]

# Non-toplevel bootstrap rules must depend on several packages, to be built
# before gcc.  Another wart that will go away, hopefully soon.
@if gcc-no-bootstrap
[+ FOR host_modules +][+
   IF (and (not (= (get "module") "gcc"))
	   (hash-ref boot-modules (get "module"))) +]
all-prebootstrap: maybe-all-[+module+][+
   ENDIF +][+
ENDFOR host_modules +]
@endif gcc-no-bootstrap

CONFIGURE_GDB_TK = @CONFIGURE_GDB_TK@
GDB_TK = @GDB_TK@
INSTALL_GDB_TK = @INSTALL_GDB_TK@
configure-gdb: $(CONFIGURE_GDB_TK)
all-gdb: $(gdbnlmrequirements) $(GDB_TK)
install-gdb: $(INSTALL_GDB_TK)

# Special-case for this one annoying target:
bfd-headers: configure-bfd
	$(AM_V_at)$(MAKE) $(AM_V_MFLAG) -C bfd headers
	$(AM_V_at)if test ! -e bfd/bfd.h; then \
	  $(MAKE) $(AM_V_MFLAG) -C bfd bfd.h; else stat bfd/bfd.h; fi
.PHONY: bfd-headers
all-bfd: bfd-headers

# Another special-cased target; may possibly want to do this for all subdirectories
# in the future, but for now, just handle this one separately:
check-binutils-local: all-binutils
	$(MAKE) $(AM_V_MFLAG) $(FLAGS_TO_PASS) -C binutils check-local

# Serialization dependencies. Host configures do NOT work well in parallel
# to each other, due to contention over config.cache. Target configures and 
# build configures are similar.
@serialization_dependencies@

# --------------------------------
# Regenerating top level configury
# --------------------------------

# Multilib.out tells target dirs what multilibs they should build.
# There is really only one copy. We use the 'timestamp' method to
# work around various timestamp bugs on some systems.
# We use move-if-change so that it is only considered updated when it
# actually changes, because it has to depend on a phony target.
multilib.out: maybe-all-gcc
	$(AM_V_at)r=`${PWD_COMMAND}`; export r; \
	echo "Checking multilib configuration..."; \
	if test ! -e multilib.out; then \
	  touch multilib.out && echo "" > multilib.out; \
	fi; \
	$(CC_FOR_TARGET) --print-multi-lib > multilib.tmp 2>/dev/null; \
	$(SHELL) $(srcdir)/move-if-change multilib.tmp multilib.out; \
	cat multilib.out

# Rebuilding Makefile.in, using autogen.
AUTOGEN = `which autogen`
$(srcdir)/Makefile.in: @MAINT@ $(srcdir)/Makefile.tpl $(srcdir)/Makefile.def
	cd $(srcdir) && $(AUTOGEN) Makefile.def
.PHONY: $(srcdir)/Makefile.in

# Rebuilding Makefile.
Makefile_target: $(srcdir)/Makefile.in config.status
	CONFIG_FILES=Makefile CONFIG_HEADERS="" $(SHELL) ./config.status
.PHONY: Makefile_target

# Normally it would be a mistake to have config.status depend upon itself,
# but since we have renamed the target here, that avoids infinite recursion:
config_dot_status_target: configure config.status
	if test -n "$${CPP}"; then unset CPP; else echo "CPP is empty"; fi; \
	CONFIG_SHELL="$(SHELL)" $(SHELL) ./config.status --recheck
.PHONY: config_dot_status_target

# Rebuilding configure.
AUTOCONF = autoconf
AUTOCONF_FLAGS = -Wno-obsolete
CONFIGURED_AUTOCONF = @AUTOCONF@
M4CONFDIR = $(srcdir)/config
MACRO_DEPS = $(M4CONFDIR)/acx.m4 $(M4CONFDIR)/override.m4 \
  $(M4CONFDIR)/proginstall.m4
$(srcdir)/configure: @MAINT@ $(srcdir)/configure.ac $(MACRO_DEPS)
	cd $(srcdir) && $(AUTOCONF) $(AUTOCONF_FLAGS)
.PHONY: $(srcdir)/configure

## so subdirs can use automake:
am--refresh:
	@:

# ------------------------------
# Special directives to GNU Make
# ------------------------------

# Do NOT pass command-line variables to submakes.
.NOEXPORT:
MAKEOVERRIDES=

# end of Makefile.in

NSLIBTOOL = cc -dynamiclib

SYSTEM_FRAMEWORK = -framework System

BINUTILS = binutils
BINUTILS_ADDRESS = 0x0
BINUTILS_PREFIX =
BINUTILS_SUFFIX =
BINUTILS_VERSION_SUFFIX =
BINUTILS_VERSION = A
BINUTILS_OFILES = binutils/version.o
BINUTILS_LIB = $(SYSTEM_FRAMEWORK)

stamp-framework-headers-binutils:
	$(AM_V_at)$(RM) -f stamp-framework-binutils stamp-framework-headers-binutils
	$(AM_V_at)$(RM) -rf $(AM_V_v) $(BINUTILS).framework
	$(MKDIR_P) $(AM_V_v) $(BINUTILS).framework/Versions/$(BINUTILS_VERSION)/Headers
	$(MKDIR_P) $(AM_V_v) $(BINUTILS).framework/Versions/$(BINUTILS_VERSION)/PrivateHeaders
	$(LN_S) $(AM_V_v) Versions/Current/Headers $(BINUTILS).framework/Headers
	$(LN_S) $(AM_V_v) Versions/Current/PrivateHeaders $(BINUTILS).framework/PrivateHeaders
	$(LN_S) $(AM_V_v) A $(BINUTILS).framework/Versions/Current
	cp -rp $(AM_V_v) $(srcdir)/include/* $(BINUTILS).framework/Versions/Current/Headers/
	cp -rp $(AM_V_v) $(srcdir)/intl/*.h $(BINUTILS).framework/Versions/Current/Headers/
	cp -rp $(AM_V_v) intl/*.h $(BINUTILS).framework/Versions/Current/Headers/
	$(AM_V_at)touch stamp-framework-headers-binutils

stamp-framework-binutils: $(BINUTILS_OFILES)
	$(AM_V_at)$(RM) -f stamp-framework-headers-binutils
	$(MAKE) $(AM_V_MFLAG) stamp-framework-headers-binutils
	$(MKDIR_P) $(AM_V_v) $(BINUTILS).framework/Versions/$(BINUTILS_VERSION)/Resources
	$(LN_S) $(AM_V_v) Versions/Current/Resources $(BINUTILS).framework/Resources
	$(MKDIR_P) $(AM_V_v) $(BINUTILS).framework/Versions/Current/Resources/English.lproj
	set -ex; \
	if [ "x$(BINUTILS_SUFFIX)" != "x" ]; then \
	    $(NSLIBTOOL) -arch $(HOST_ARCHITECTURE) -seg1addr $(BINUTILS_ADDRESS) -compatibility_version 1 -current_version 1 -install_name /System/Library/PrivateFrameworks/$(BINUTILS).framework/Versions/$(BINUTILS_VERSION)/$(BINUTILS_PREFIX)$(BINUTILS)$(BINUTILS_VERSION_SUFFIX) -o $(BINUTILS).framework/Versions/Current/$(BINUTILS_PREFIX)$(BINUTILS)$(BINUTILS_VERSION_SUFFIX) $(BINUTILS_OFILES) $(BINUTILS_LIB); \
	    $(LN_S) $(AM_V_v) $(BINUTILS_PREFIX)$(BINUTILS)$(BINUTILS_VERSION_SUFFIX) $(BINUTILS).framework/Versions/Current/$(BINUTILS_PREFIX)$(BINUTILS)$(BINUTILS_SUFFIX); \
	    $(LN_S) $(AM_V_v) Versions/Current/$(BINUTILS_PREFIX)$(BINUTILS)$(BINUTILS_VERSION_SUFFIX) $(BINUTILS).framework/$(BINUTILS_PREFIX)$(BINUTILS)$(BINUTILS_VERSION_SUFFIX); \
	    $(LN_S) $(AM_V_v) Versions/Current/$(BINUTILS_PREFIX)$(BINUTILS)$(BINUTILS_SUFFIX) $(BINUTILS).framework/$(BINUTILS_PREFIX)$(BINUTILS)$(BINUTILS_SUFFIX); \
	    $(LN_S) $(AM_V_v) $(BINUTILS_PREFIX)$(BINUTILS)$(BINUTILS_SUFFIX) $(BINUTILS).framework/$(BINUTILS); \
	else \
	    $(NSLIBTOOL) -arch $(HOST_ARCHITECTURE) -seg1addr $(BINUTILS_ADDRESS) -compatibility_version 1 -current_version 1 -install_name /System/Library/PrivateFrameworks/$(BINUTILS).framework/Versions/$(BINUTILS_VERSION)/$(BINUTILS_PREFIX)$(BINUTILS) -o $(BINUTILS).framework/Versions/Current/$(BINUTILS_PREFIX)$(BINUTILS) $(BINUTILS_OFILES) $(BINUTILS_LIB); \
	    $(LN_S) $(AM_V_v) Versions/Current/$(BINUTILS_PREFIX)$(BINUTILS) $(BINUTILS).framework/$(BINUTILS_PREFIX)$(BINUTILS); \
	fi
	$(AM_V_at)touch stamp-framework-binutils

GDB = gdb
GDB_ADDRESS = 0x0
GDB_PREFIX =
GDB_SUFFIX =
GDB_VERSION_SUFFIX =
GDB_VERSION = A
GDB_OFILES = gdb/version.o
GDB_LIB = $(SYSTEM_FRAMEWORK)

TEMPLATE_HEADERS = config.h tm.h xm.h nm.h
TEMPLATEdir = @TEMPLATEdir@

stamp-framework-headers-gdb:
	$(AM_V_at)$(RM) -f stamp-framework-gdb stamp-framework-headers-gdb
	$(AM_V_at)$(RM) -rf $(AM_V_v) $(GDB).framework
	$(MKDIR_P) $(AM_V_v) $(GDB).framework/Versions/$(GDB_VERSION)/Headers
	$(MKDIR_P) $(AM_V_v) $(GDB).framework/Versions/$(GDB_VERSION)/PrivateHeaders
	$(LN_S) $(AM_V_v) Versions/Current/Headers $(GDB).framework/Headers
	$(LN_S) $(AM_V_v) Versions/Current/PrivateHeaders $(GDB).framework/PrivateHeaders
	$(LN_S) $(AM_V_v) A $(GDB).framework/Versions/Current
	cp -p $(AM_V_v) $(srcdir)/gdb/*.h $(GDB).framework/Versions/Current/Headers/
	cp -p $(AM_V_v) $(srcdir)/gdb/macosx/*.h $(GDB).framework/Versions/Current/Headers/
	$(MKDIR_P) $(AM_V_v) $(GDB).framework/Versions/Current/Headers/tui
	cp -p $(AM_V_v) $(srcdir)/gdb/tui/*.h $(GDB).framework/Versions/Current/Headers/tui
	$(MKDIR_P) $(AM_V_v) $(GDB).framework/Versions/Current/Headers/cli
	cp -p $(AM_V_v) $(srcdir)/gdb/cli/*.h $(GDB).framework/Versions/Current/Headers/cli
	$(MKDIR_P) $(AM_V_v) $(GDB).framework/Versions/Current/Headers/mi
	cp -p $(AM_V_v) $(srcdir)/gdb/mi/*.h $(GDB).framework/Versions/Current/Headers/mi
	$(MKDIR_P) $(AM_V_v) $(GDB).framework/Versions/Current/Headers/machine
	cp -p $(AM_V_v) gdb/*.h $(GDB).framework/Versions/Current/Headers/machine/
	cp -rp $(AM_V_v) $(srcdir)/gdb/config $(GDB).framework/Versions/Current/Headers/config
	set -ex; \
	for h in $(TEMPLATE_HEADERS); do \
	    rm -f $(AM_V_v) $(GDB).framework/Versions/A/Headers/$${h}; \
	    $(LN_S) $(AM_V_v) machine/$${h} $(GDB).framework/Versions/A/Headers/$${h}; \
	done
	$(AM_V_at)touch stamp-framework-headers-gdb

stamp-framework-gdb: $(GDB_OFILES)
	$(AM_V_at)$(RM) -f stamp-framework-headers-gdb
	$(MAKE) $(AM_V_MFLAG) stamp-framework-headers-gdb
	$(MKDIR_P) $(AM_V_v) $(GDB).framework/Versions/$(GDB_VERSION)/Resources
	$(LN_S) $(AM_V_v) Versions/Current/Resources $(GDB).framework/Resources
	$(MKDIR_P) $(AM_V_v) $(GDB).framework/Versions/Current/Resources/English.lproj
	set -ex; \
	if [ "x$(GDB_SUFFIX)" != x"" ]; then \
	    $(NSLIBTOOL) -arch $(HOST_ARCHITECTURE) -seg1addr $(GDB_ADDRESS) -compatibility_version 1 -current_version 1 -install_name /System/Library/PrivateFrameworks/$(GDB).framework/Versions/$(GDB_VERSION)/$(GDB_PREFIX)$(GDB)$(GDB_VERSION_SUFFIX) -o $(GDB).framework/Versions/Current/$(GDB_PREFIX)$(GDB)$(GDB_VERSION_SUFFIX) $(GDB_OFILES) $(GDB_LIB); \
	    $(LN_S) $(AM_V_v) $(GDB_PREFIX)$(GDB)$(GDB_VERSION_SUFFIX) $(GDB).framework/Versions/Current/$(GDB_PREFIX)$(GDB)$(GDB_SUFFIX); \
	    $(LN_S) $(AM_V_v) Versions/Current/$(GDB_PREFIX)$(GDB)$(GDB_VERSION_SUFFIX) $(GDB).framework/$(GDB_PREFIX)$(GDB)$(GDB_VERSION_SUFFIX); \
	    $(LN_S) $(AM_V_v) Versions/Current/$(GDB_PREFIX)$(GDB)$(GDB_SUFFIX) $(GDB).framework/$(GDB_PREFIX)$(GDB)$(GDB_SUFFIX); \
	    $(LN_S) $(AM_V_v) $(GDB_PREFIX)$(GDB)$(GDB_SUFFIX) $(GDB).framework/$(GDB); \
	else \
	    $(NSLIBTOOL) -arch $(HOST_ARCHITECTURE) -seg1addr $(GDB_ADDRESS) -compatibility_version 1 -current_version 1 -install_name /System/Library/PrivateFrameworks/$(GDB).framework/Versions/$(GDB_VERSION)/$(GDB_PREFIX)$(GDB) -o $(GDB).framework/Versions/Current/$(GDB_PREFIX)$(GDB) $(GDB_OFILES) $(GDB_LIB); \
	    $(LN_S) $(AM_V_v) Versions/Current/$(GDB_PREFIX)$(GDB) $(GDB).framework/$(GDB_PREFIX)$(GDB); \
	fi
	$(AM_V_at)touch stamp-framework-gdb

# real end of Makefile.in and Makefile.tpl
