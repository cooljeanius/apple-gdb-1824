# Top-level makefile for Apple's fork of gdb
# If this Makefile does not work, try the one in ./src

GDB_VERSION = 6.3.50-20050815
GDB_RC_VERSION = 1824

BINUTILS_VERSION = 2.13-20021117
BINUTILS_RC_VERSION = 46

# Keep line below uncommented for debugging shell commands
SHELL = /bin/sh -ex

.PHONY: all clean configure build install installsrc installhdrs headers \
	build-core build-binutils build-gdb \
	install-frameworks-headers\
	install-frameworks-macosx \
	install-binutils-macosx \
	install-gdb-fat \
	install-chmod-macosx install-chmod-macosx-noprocmod \
	install-clean install-source check \
	cross-installhdrs \
	cross-install-frameworks-headers \
	cross-install-frameworks-headers-finalize

SUBDIRS = src bin macsbug libcheckpoint


# Get the correct setting for SYSTEM_DEVELOPER_TOOLS_DOC_DIR if 
# the platform-variables.make file exists.

OS=MACOS
XCODE_APP_CONTENTS_DIR=/Applications/Xcode.app/Contents
SYSTEM_DEVELOPER_TOOLS_DOC_DIR=$(XCODE_APP_CONTENTS_DIR)/Developer/Documentation/DocSets/com.apple.ADC_Reference_Library.DeveloperTools.docset/Contents/Resources/Documents/documentation/DeveloperTools


ifndef RC_ARCHS
RC_ARCHS=$(shell /usr/bin/arch)
endif

ifndef SRCROOT
SRCROOT=.
endif

ifndef OBJROOT
OBJROOT=./obj
endif

ifndef SYMROOT
SYMROOT=./sym
endif

ifndef DSTROOT
DSTROOT=./dst
endif

INSTALL=$(SRCROOT)/src/install-sh

CANONICAL_ARCHS := $(foreach arch,$(RC_ARCHS),$(foreach os,$(RC_OS),$(foreach release,$(RC_RELEASE),$(os):$(arch):$(release))))

CANONICAL_ARCHS := $(subst teflon:ppc:Rhapsody,powerpc-apple-rhapsody,$(CANONICAL_ARCHS))
CANONICAL_ARCHS := $(subst teflon:i386:Rhapsody,i386-apple-rhapsody,$(CANONICAL_ARCHS))

CANONICAL_ARCHS := $(subst macos:i386:$(RC_RELEASE),i386-apple-darwin,$(CANONICAL_ARCHS))
CANONICAL_ARCHS := $(subst macos:x86_64:$(RC_RELEASE),x86_64-apple-darwin,$(CANONICAL_ARCHS))
CANONICAL_ARCHS := $(subst macos:ppc:$(RC_RELEASE),powerpc-apple-darwin,$(CANONICAL_ARCHS))
CANONICAL_ARCHS := $(subst macos:arm:$(RC_RELEASE),arm-apple-darwin,$(CANONICAL_ARCHS))
CANONICAL_ARCHS := $(subst macos:armv4:$(RC_RELEASE),arm-apple-darwin,$(CANONICAL_ARCHS))
CANONICAL_ARCHS := $(subst macos:armv4t:$(RC_RELEASE),arm-apple-darwin,$(CANONICAL_ARCHS))
CANONICAL_ARCHS := $(subst macos:armv5:$(RC_RELEASE),arm-apple-darwin,$(CANONICAL_ARCHS))
CANONICAL_ARCHS := $(subst macos:armv5t:$(RC_RELEASE),arm-apple-darwin,$(CANONICAL_ARCHS))
CANONICAL_ARCHS := $(subst macos:armv6:$(RC_RELEASE),arm-apple-darwin,$(CANONICAL_ARCHS))
CANONICAL_ARCHS := $(subst macos:armv6t:$(RC_RELEASE),arm-apple-darwin,$(CANONICAL_ARCHS))
CANONICAL_ARCHS := $(subst macos:armv6t2:$(RC_RELEASE),arm-apple-darwin,$(CANONICAL_ARCHS))
CANONICAL_ARCHS := $(subst macos:armv7:$(RC_RELEASE),arm-apple-darwin,$(CANONICAL_ARCHS))
CANONICAL_ARCHS := $(subst macos:armv7t:$(RC_RELEASE),arm-apple-darwin,$(CANONICAL_ARCHS))
CANONICAL_ARCHS := $(subst macos:armv7s:$(RC_RELEASE),arm-apple-darwin,$(CANONICAL_ARCHS))

CANONICAL_ARCHS := $(subst powerpc-apple-darwin i386-apple-darwin,i386-apple-darwin powerpc-apple-darwin,$(CANONICAL_ARCHS))

CANONICAL_ARCHS := $(sort $(CANONICAL_ARCHS))

SRCTOP = $(shell cd $(SRCROOT) && pwd)
OBJTOP = $(shell (test -d $(OBJROOT) || $(INSTALL) -c -d $(OBJROOT)) && cd $(OBJROOT) && pwd)
SYMTOP = $(shell (test -d $(SYMROOT) || $(INSTALL) -c -d $(SYMROOT)) && cd $(SYMROOT) && pwd)
DSTTOP = $(shell (test -d $(DSTROOT) || $(INSTALL) -c -d $(DSTROOT)) && cd $(DSTROOT) && pwd)

ARCH_SAYS := $(shell /usr/bin/arch)
ifeq (i386,$(ARCH_SAYS))
BUILD_ARCH := i386-apple-darwin
else
ifeq (ppc,$(ARCH_SAYS))
BUILD_ARCH := powerpc-apple-darwin
else
ifeq (arm,$(ARCH_SAYS))
BUILD_ARCH := arm-apple-darwin
else
BUILD_ARCH := $(ARCH_SAYS)
endif
endif
endif

GDB_VERSION_STRING = $(GDB_VERSION) (Apple version gdb-$(GDB_RC_VERSION))
BINUTILS_VERSION_STRING = "$(BINUTILS_VERSION) (Apple version binutils-$(BINUTILS_RC_VERSION))"

GDB_BINARIES = gdb
GDB_FRAMEWORKS = gdb
GDB_MANPAGES = 

BINUTILS_FRAMEWORKS = bfd binutils
BINUTILS_MANPAGES = 

FRAMEWORKS = $(GDB_FRAMEWORKS) $(BINUTILS_FRAMEWORKS)

ifndef BINUTILS_BUILD_ROOT
BINUTILS_BUILD_ROOT = $(SDKROOT)
endif

BINUTILS_FRAMEWORK_PATH = $(BINUTILS_BUILD_ROOT)/System/Library/PrivateFrameworks
BINUTILS_LIB_PATH = $(BINUTILS_BUILD_ROOT)/usr/lib

BFD_FRAMEWORK = $(BINUTILS_FRAMEWORK_PATH)/bfd.framework
BFD_HEADERS = $(BFD_FRAMEWORK)/Headers

LIBERTY_FRAMEWORK = $(BINUTILS_FRAMEWORK_PATH)/liberty.framework
LIBERTY_HEADERS = $(LIBERTY_FRAMEWORK)/Headers

OPCODES_FRAMEWORK = $(BINUTILS_FRAMEWORK_PATH)/opcodes.framework
OPCODES_HEADERS = $(OPCODES_FRAMEWORK)/Headers

BINUTILS_FRAMEWORK = $(BINUTILS_FRAMEWORK_PATH)/binutils.framework
BINUTILS_HEADERS = $(BINUTILS_FRAMEWORK)/Headers

INTL_FRAMEWORK = $(BINUTILS_BUILD_ROOT)/usr/lib/libintl.dylib
INTL_HEADERS = $(BINUTILS_BUILD_ROOT)/usr/include

ifndef SDKROOT_FOR_BUILD
export SDKROOT_FOR_BUILD = $(shell xcodebuild -version -sdk macosx Path 2>/dev/null | head -1)
endif

export AR       = $(shell xcrun -find ar)
export CC       = $(shell (xcrun -find clang 2>/dev/null || xcrun -find gcc))
export CPP      = $(shell xcrun -find cpp) -I$(SDKROOT_FOR_BUILD)/usr/include
export CXX      = $(shell (xcrun -find clang++ 2>/dev/null || xcrun -find g++))
export LD       = $(shell xcrun -find ld)
export LIBTOOL  = $(shell xcrun -find libtool)
export MAKE     = $(shell xcrun -find make)
export NM       = $(shell xcrun -find nm)
export RANLIB   = $(shell xcrun -find ranlib)
export TAR      = $(shell (xcrun -find gnutar 2>/dev/null || xcrun -find tar))
ifndef RM
export RM       = rm
endif

export CC_FOR_BUILD      = $(shell (xcrun -find clang 2>/dev/null || xcrun -find gcc))
export CCFLAGS_FOR_BUILD = -I$(SDKROOT_FOR_BUILD)/usr/include
export LDFLAGS_FOR_BUILD = -isysroot $(SDKROOT_FOR_BUILD)

ifndef CDEBUGFLAGS
CDEBUGFLAGS = -ggdb -Os -funwind-tables -fasynchronous-unwind-tables -D_DARWIN_UNLIMITED_STREAMS -Wno-format-security -Wno-format-nonliteral 
endif

CFLAGS = $(CDEBUGFLAGS) $(RC_CFLAGS)
HOST_ARCHITECTURE = UNKNOWN

RC_CFLAGS_NOARCH = $(strip $(shell echo $(RC_CFLAGS) | sed -e 's/-arch [a-z0-9_]*//g'))

SYSTEM_FRAMEWORK = -L../intl -L./intl -L../intl/.libs -L./intl/.libs -lintl -framework System
FRAMEWORK_PREFIX =
FRAMEWORK_SUFFIX =
FRAMEWORK_VERSION = A
FRAMEWORK_VERSION_SUFFIX =

DEVEXEC_DIR=UNKNOWN
LIBEXEC_BINUTILS_DIR=UNKNOWN
LIBEXEC_GDB_DIR=UNKNOWN
LIBEXEC_LIB_DIR=UNKNOWN
MAN_DIR=UNKNOWN
PRIVATE_FRAMEWORKS_DIR=UNKNOWN

NATIVE_TARGETS = $(foreach arch1,$(CANONICAL_ARCHS),$(arch1)--$(arch1))

CROSS_TARGETS = $(strip $(foreach hostarch, $(CANONICAL_ARCHS), $(foreach targarch, $(filter-out $(hostarch), $(CANONICAL_ARCHS)), $(hostarch)--$(targarch))))

# Unconditionally add a cross-arm debugger to every native host.

CROSS_TARGETS := $(strip $(CROSS_TARGETS) $(foreach hostarch, $(CANONICAL_ARCHS), $(hostarch)--arm-apple-darwin))

CROSS_TARGETS := $(filter-out x86_64-apple-darwin--i386-apple-darwin, $(CROSS_TARGETS))
CROSS_TARGETS := $(filter-out i386-apple-darwin--x86_64-apple-darwin, $(CROSS_TARGETS))

# We do NOT want to build a ppc cross anything gdb; Apple assumes that
# no one will need that these days.
CROSS_TARGETS := $(filter-out powerpc-apple-darwin--x86_64-apple-darwin, $(CROSS_TARGETS))
CROSS_TARGETS := $(filter-out powerpc-apple-darwin--i386-apple-darwin, $(CROSS_TARGETS))
CROSS_TARGETS := $(filter-out powerpc-apple-darwin--arm-apple-darwin, $(CROSS_TARGETS))

# Similarly, Apple assumes that no one needs an arm x x86-64 debugger,
# for instance.
CROSS_TARGETS := $(filter-out arm-apple-darwin--x86_64-apple-darwin, $(CROSS_TARGETS))
CROSS_TARGETS := $(filter-out arm-apple-darwin--arm-apple-darwin, $(CROSS_TARGETS))
CROSS_TARGETS := $(filter-out arm-apple-darwin--i386-apple-darwin, $(CROSS_TARGETS))
CROSS_TARGETS := $(filter-out arm-apple-darwin--powerpc-apple-darwin, $(CROSS_TARGETS))

CROSS_TARGETS := $(sort $(CROSS_TARGETS))

# These can be set to control the flags passed to the configure scripts
# in the various subdirectories:
CONFIG_VERBOSE=-v
CONFIG_ENABLE_GDBTK=--enable-gdbtk=no
CONFIG_ENABLE_GDBMI=--enable-gdbmi
CONFIG_ENABLE_BUILD_WARNINGS=--enable-build-warnings --disable-werror
CONFIG_ENABLE_TUI=--disable-tui
CONFIG_ALL_BFD_TARGETS=--enable-targets#=all
CONFIG_ALL_BFD_APPLE_TARGETS=--enable-targets=m68k-apple-aux,powerpc-apple-rhapsody,arm-apple-darwin,i386-apple-darwin,x86_64-apple-darwin,powerpc-apple-macos,powerpc64-apple-darwin,powerpc64-apple-macos
CONFIG_64_BIT_BFD=--enable-64-bit-bfd
CONFIG_WITH_MMAP=--with-mmap
CONFIG_ENABLE_SHARED=--disable-shared
CONFIG_MAINTAINER_MODE=#--enable-maintainer-mode
CONFIG_SILENT_RULES=
CONFIG_DEP_TRACK=--disable-dependency-tracking
CONFIG_BUILD=--build=$(BUILD_ARCH)
CONFIG_OTHER_OPTIONS?=--disable-serial-configure --with-x --enable-carbon-framework --enable-debug-symbols-framework

ifneq ($(findstring macosx,$(CANONICAL_ARCHS))$(findstring darwin,$(CANONICAL_ARCHS)),)
CC = clang -arch $(HOST_ARCHITECTURE)
CC_FOR_BUILD = clang

CDEBUGFLAGS = -ggdb -Os

# The -Wno-error=deprecated-declarations flag is not recognized by some 
# compilers; disable it on a per-release basis.

ifeq (Leopard,$(RC_RELEASE))
OS_DEP_CFLAGS = 
else
ifeq (SnowLeopard,$(RC_RELEASE))
OS_DEP_CFLAGS = -Wno-error=deprecated-declarations
else
OS_DEP_CFLAGS = 
endif
endif

CFLAGS = $(strip $(RC_NONARCH_CFLAGS) $(CDEBUGFLAGS) -Wall -Wimplicit $(OS_DEP_CFLAGS) -funwind-tables -fasynchronous-unwind-tables)
HOST_ARCHITECTURE = $(shell echo $* | sed -e 's/--.*//' -e 's/powerpc/ppc/' -e 's/-apple-macosx.*//' -e 's/-apple-macos.*//' -e 's/-apple-darwin.*//')
endif


MACOSX_FLAGS = \
	DEVEXEC_DIR=usr/bin \
	LIBEXEC_BINUTILS_DIR=usr/libexec/binutils \
	LIBEXEC_GDB_DIR=usr/libexec/gdb \
	LIB_DIR=usr/lib \
	MAN_DIR=usr/share/man \
	PRIVATE_FRAMEWORKS_DIR=System/Library/PrivateFrameworks \
	SOURCE_DIR=System/Developer/Source/Commands/gdb

CONFIGURE_OPTIONS = $(filter-out ,\
	$(CONFIG_VERBOSE) \
	$(CONFIG_ENABLE_GDBTK) \
	$(CONFIG_ENABLE_GDBMI) \
	$(CONFIG_ENABLE_BUILD_WARNINGS) \
	$(CONFIG_ENABLE_TUI) \
	$(CONFIG_ALL_BFD_TARGETS) \
	$(CONFIG_64_BIT_BFD) \
	$(CONFIG_WITH_MMAP) \
	$(CONFIG_ENABLE_SHARED) \
	$(CONFIG_MAINTAINER_MODE) \
	$(CONFIG_DEP_TRACK) \
	$(CONFIG_BUILD) \
	$(CONFIG_OTHER_OPTIONS))

MAKE_OPTIONS = \
	prefix='/usr'

EFLAGS = $(filter-out ,\
	CFLAGS='$(CFLAGS)' \
	CC='$(CC)' \
	CPP='$(CPP)' \
	CXX='$(CXX)' \
	LD='$(LD)' \
	AR='$(AR)' \
	RANLIB='$(RANLIB)' \
	NM='$(NM)' \
	CC_FOR_BUILD='$(CC_FOR_BUILD)' \
	HOST_ARCHITECTURE='$(HOST_ARCHITECTURE)' \
	SDKROOT='$(SDKROOT)' \
	BINUTILS_FRAMEWORK_PATH='$(BINUTILS_FRAMEWORK_PATH)' \
	SRCROOT='$(SRCROOT)' \
	$(MAKE_OPTIONS))

SFLAGS = $(EFLAGS)

FFLAGS = \
	$(SFLAGS) \
	SYSTEM_FRAMEWORK='$(SYSTEM_FRAMEWORK)' \
	FRAMEWORK_PREFIX='$(FRAMEWORK_PREFIX)' \
	FRAMEWORK_SUFFIX='$(FRAMEWORK_SUFFIX)' \
	FRAMEWORK_VERSION_SUFFIX='$(FRAMEWORK_VERSION_SUFFIX)'

FRAMEWORK_TARGET=stamp-framework-headers all
framework=-L../$(patsubst liberty,libiberty,$(1)) -l$(patsubst liberty,iberty,$(1))

FSFLAGS = $(SFLAGS)

CONFIGURE_ENV = $(EFLAGS)
MAKE_ENV = $(EFLAGS)

SUBMAKE = $(MAKE_ENV) $(MAKE)

_all: all


crossarm: LIBEXEC_GDB_DIR=usr/libexec/gdb
crossarm: CDEBUGFLAGS ?= -gdwarf-2 -D__DARWIN_UNIX03=0


crossarm:;
        
	echo BUILDING CROSSARM; \
	$(RM) -v $(SYMROOT)/$(LIBEXEC_GDB_DIR)/gdb-arm-apple-darwin
	for i in $(RC_ARCHS); do \
		$(RM) -r $(OBJROOT)/$${i}-apple-darwin--arm-apple-darwin; \
		$(INSTALL) -c -d $(OBJROOT)/$${i}-apple-darwin--arm-apple-darwin; \
		sdk_cc=`xcrun -sdk $(SDKROOT) -find clang`; \
		(cd $(OBJROOT)/$${i}-apple-darwin--arm-apple-darwin/ && \
			$(CONFIGURE_ENV) CC="$${sdk_cc} -arch $${i}" $(MACOSX_FLAGS) $(SRCTOP)/src/configure \
				--host=$${i}-apple-darwin \
				--target=arm-apple-darwin \
                                --build=$(BUILD_ARCH) \
				CFLAGS=" -isystem $(SDKROOT)/usr/include $(CDEBUGFLAGS)" \
				$(CONFIGURE_OPTIONS) \
			); \
		mkdir -p $(SYMROOT)/$(LIBEXEC_GDB_DIR); \
		mkdir -p $(DSTROOT)/$(LIBEXEC_GDB_DIR); \
		$(SUBMAKE) $(MACOSX_FLAGS) -C $(OBJROOT)/$${i}-apple-darwin--arm-apple-darwin configure-gdb; \
		$(SUBMAKE) $(MACOSX_FLAGS) -C $(OBJROOT)/$${i}-apple-darwin--arm-apple-darwin \
				-W version.in VERSION='$(GDB_VERSION_STRING)' GDB_RC_VERSION='$(GDB_RC_VERSION)' all-gdb; \
		if [ -e $(SYMROOT)/$(LIBEXEC_GDB_DIR)/gdb-arm-apple-darwin ] ; then \
			lipo -create $(SYMROOT)/$(LIBEXEC_GDB_DIR)/gdb-arm-apple-darwin $(OBJROOT)/$${i}-apple-darwin--arm-apple-darwin/gdb/gdb -output $(SYMROOT)/$(LIBEXEC_GDB_DIR)/gdb-arm-apple-darwin; \
		else \
			lipo -create $(OBJROOT)/$${i}-apple-darwin--arm-apple-darwin/gdb/gdb -output $(SYMROOT)/$(LIBEXEC_GDB_DIR)/gdb-arm-apple-darwin; \
		fi;\
	done;
	(cd $(SYMROOT)/$(LIBEXEC_GDB_DIR)/ ; dsymutil gdb-arm-apple-darwin)
	strip -S -o $(DSTROOT)/$(LIBEXEC_GDB_DIR)/gdb-arm-apple-darwin \
		$(SYMROOT)/$(LIBEXEC_GDB_DIR)/gdb-arm-apple-darwin
	chown root:wheel $(DSTROOT)/$(LIBEXEC_GDB_DIR)/gdb-arm-apple-darwin
	chmod 755 $(DSTROOT)/$(LIBEXEC_GDB_DIR)/gdb-arm-apple-darwin
	mkdir -p ${DSTROOT}/usr/bin
	sed -e 's/version=.*/version=$(GDB_VERSION)-$(GDB_RC_VERSION)/' \
                < $(SRCROOT)/gdb.sh > ${DSTROOT}/usr/bin/gdb
	chmod 755 ${DSTROOT}/usr/bin/gdb
.PHONY: crossarm

#
# cross
# 
# Build only a cross targets for host architectures. RC_ARCHS specifies
# all of the host architectures to build for, and RC_CROSS_ARCHS specifies
# all of the cross architectures to build for. This can save time when
# building a cross target as you do NOT have to build all permutations of
# RC_ARCHS. 
#
# SDKROOT can be used to specify a system root for cross builds, and
# the current macos is used will be used for others.
#
# The command below will build a cross armv6 gdb to be hosted on i386, ppc,
# and natively on armv6:
#
# sudo ~rc/bin/buildit `pwd` -arch ppc -arch i386 -arch armv5 \
#	-target cross -project gdb -release RELEASENAME \
#	SDKROOT=/path/to/sdk/some.sdk RC_CROSS_ARCHS=armv6
#


cross: 	LIBEXEC_GDB_DIR=usr/libexec/gdb
cross:;
	$(SUBMAKE) $(MACOSX_FLAGS) install-gdb-macosx-common
	echo BUILDING CROSS $(RC_CROSS_ARCHS) for HOST $(RC_ARCHS); \
	set -e; for cross_arch in $(RC_CROSS_ARCHS); do \
		cross_arch_full=$${cross_arch}; \
		if [[ "$${cross_arch}" =~ arm.* ]] ; then \
			cross_arch=arm; \
		fi; \
		target_arch_vendor_os="$${cross_arch}-apple-darwin"; \
		curr_symroot_output_file="$(SYMROOT)/$(LIBEXEC_GDB_DIR)/gdb-$${target_arch_vendor_os}" ; \
		for host_arch in $(RC_ARCHS); do \
			host_arch_full="$${host_arch}"; \
			if [[ "$${host_arch}" =~ arm.* ]] ; then \
				host_arch="arm"; \
			fi; \
			echo "BUILDING CROSS $${cross_arch_full} for HOST $${host_arch_full}"; \
			host_arch_vendor_os="$${host_arch}-apple-darwin"; \
			curr_objroot="$(OBJROOT)/$${host_arch_full}-apple-darwin--$${cross_arch_full}-apple-darwin"; \
			$(RM) -r "$${curr_objroot}"; \
			$(INSTALL) -c -d "$${curr_objroot}"; \
			if [[ "$${host_arch}" == "arm" ]] ; then \
				sdk_cc=`xcrun -sdk $(SDKROOT) -find clang`; \
				sdk_cxx=`xcrun -sdk $(SDKROOT) -find clang++`; \
				sdk_cflags=" -isysroot $(SDKROOT)"; \
				sdk_cxxflags=" -isysroot $(SDKROOT)"; \
			else \
				sdk_cc="clang"; \
				sdk_cxx="clang++"; \
				sdk_cflags=" -isysroot $(SDKROOT_FOR_BUILD)"; \
				sdk_cxxflags=" -isysroot $(SDKROOT_FOR_BUILD)"; \
			fi; \
			(cd "$${curr_objroot}"/ && \
				$(CONFIGURE_ENV) \
					CC="$${sdk_cc} -arch $${host_arch_full}$${sdk_cflags}" \
					CXX="$${sdk_cxx} -arch $${host_arch_full}$${sdk_cxxflags}" \
					$(MACOSX_FLAGS) $(SRCTOP)/src/configure \
					--host=$${host_arch_vendor_os} \
					--target=$${target_arch_vendor_os} \
					--build=$(BUILD_ARCH) \
					CFLAGS="$(CDEBUGFLAGS)" \
					$(CONFIGURE_OPTIONS) \
				); \
			mkdir -p $(SYMROOT)/$(LIBEXEC_GDB_DIR); \
			mkdir -p $(DSTROOT)/$(LIBEXEC_GDB_DIR); \
			$(SUBMAKE) $(MACOSX_FLAGS) -C "$${curr_objroot}" configure-gdb; \
			$(SUBMAKE) $(MACOSX_FLAGS) -C "$${curr_objroot}" \
				-W version.in VERSION='$(GDB_VERSION_STRING)' GDB_RC_VERSION='$(GDB_RC_VERSION)' all-gdb; \
			if [ -e "$${curr_symroot_output_file}" ] ; then \
				lipo -create "$${curr_symroot_output_file}" "$${curr_objroot}/gdb/gdb" -output "$${curr_symroot_output_file}"; \
			else \
				lipo -create "$${curr_objroot}/gdb/gdb" -output "$${curr_symroot_output_file}"; \
			fi; \
		done; \
                (cd $(SYMROOT)/$(LIBEXEC_GDB_DIR)/ ; dsymutil gdb-$${target_arch_vendor_os}); \
		strip -S -o $(DSTROOT)/$(LIBEXEC_GDB_DIR)/gdb-$${target_arch_vendor_os} "$${curr_symroot_output_file}"; \
	done; \
	mkdir -p ${DSTROOT}/usr/bin; \
	sed -e 's/version=.*/version=$(GDB_VERSION)-$(GDB_RC_VERSION)/' \
                < $(SRCROOT)/gdb.sh > ${DSTROOT}/usr/bin/gdb; \
	chmod 755 ${DSTROOT}/usr/bin/gdb; \
.PHONY: cross

cross-installhdrs:
	$(SUBMAKE) $(MACOSX_FLAGS) install-clean ; \
	echo BUILDING CROSS $(RC_CROSS_ARCHS) for HOST $(RC_ARCHS); \
		for cross_arch in $(RC_CROSS_ARCHS); do \
		cross_arch_full=$${cross_arch}; \
		if [[ "$${cross_arch}" =~ arm.* ]] ; then \
			cross_arch=arm; \
		fi; \
		target_arch_vendor_os="$${cross_arch}-apple-darwin"; \
		for host_arch in $(RC_ARCHS); do \
			host_arch_full="$${host_arch}"; \
			if [[ "$${host_arch}" =~ arm.* ]] ; then \
				host_arch="arm"; \
			fi; \
			echo "BUILDING CROSS $${cross_arch_full} for HOST $${host_arch_full}"; \
			host_arch_vendor_os="$${host_arch}-apple-darwin"; \
			curr_objroot="$(OBJROOT)/$${host_arch_full}-apple-darwin--$${cross_arch_full}-apple-darwin"; \
			$(RM) -r "$${curr_objroot}"; \
			$(INSTALL) -c -d "$${curr_objroot}"; \
			if [[ "$${host_arch}" == "arm" ]] ; then \
				sdk_cc=`xcrun -sdk $(SDKROOT) -find clang`; \
				sdk_cxx=`xcrun -sdk $(SDKROOT) -find clang++`; \
				sdk_cflags=" -isysroot $(SDKROOT)"; \
				sdk_cxxflags=" -isysroot $(SDKROOT)"; \
			else \
				sdk_cc="clang"; \
				sdk_cxx="clang++"; \
				sdk_cflags=" -isysroot $(SDKROOT_FOR_BUILD)"; \
				sdk_cxxflags=" -isysroot $(SDKROOT_FOR_BUILD)"; \
			fi; \
			(cd "$${curr_objroot}"/ && \
				$(CONFIGURE_ENV) \
					CC="$${sdk_cc} -arch $${host_arch_full}$${sdk_cflags}" \
					CXX="$${sdk_cxx} -arch $${host_arch_full}$${sdk_cxxflags}" \
					$(MACOSX_FLAGS) $(SRCTOP)/src/configure \
					--host=$${host_arch_vendor_os} \
					--target=$${target_arch_vendor_os} \
					--build=$(BUILD_ARCH) \
					CFLAGS="$(CDEBUGFLAGS)" \
					$(CONFIGURE_OPTIONS) \
				); \
			$(SUBMAKE) $${curr_objroot}/stamp-build-headers ; \
			$(SUBMAKE) $(MACOSX_FLAGS) CURRENT_ROOT=$(SYMROOT) CROSS_ARCH=$${cross_arch} CROSS_ARCH_FULL=$${cross_arch_full} HOST_ARCH=$${host_arch} HOST_ARCH_FULL=$${host_arch_full} cross-install-frameworks-headers ; \
			$(SUBMAKE) $(MACOSX_FLAGS) CURRENT_ROOT=$(DSTROOT) CROSS_ARCH=$${cross_arch} CROSS_ARCH_FULL=$${cross_arch_full} HOST_ARCH=$${host_arch} HOST_ARCH_FULL=$${host_arch_full} cross-install-frameworks-headers ; \
		done; \
	done; \
	$(SUBMAKE) CURRENT_ROOT=$(SYMROOT) $(MACOSX_FLAGS) cross-install-frameworks-headers-finalize ; \
	$(SUBMAKE) CURRENT_ROOT=$(DSTROOT) $(MACOSX_FLAGS) cross-install-frameworks-headers-finalize ; \


$(OBJROOT)/%/stamp-rc-configure:
	$(RM) -r $(OBJROOT)/$*
	$(INSTALL) -c -d $(OBJROOT)/$*
	(cd $(OBJROOT)/$* && \
		$(CONFIGURE_ENV) $(SRCTOP)/src/configure \
			--host=$(shell echo $* | sed -e 's/--.*//') \
			--target=$(shell echo $* | sed -e 's/.*--//') \
			$(CONFIGURE_OPTIONS) \
			)
	touch $@

$(OBJROOT)/%/stamp-rc-configure-cross:
	$(RM) -r $(OBJROOT)/$*
	$(INSTALL) -c -d $(OBJROOT)/$*
	(cd $(OBJROOT)/$* && \
		$(CONFIGURE_ENV) $(SRCTOP)/src/configure \
			--host=$(shell echo $* | sed -e 's/--.*//') \
			--target=$(shell echo $* | sed -e 's/.*--//') \
			$(CONFIGURE_OPTIONS) \
			)
	touch $@

$(OBJROOT)/%/stamp-build-headers:
	$(SUBMAKE) -C $(OBJROOT)/$* configure-intl configure-libiberty configure-bfd configure-opcodes configure-gdb
	$(SUBMAKE) -C $(OBJROOT)/$*/libiberty $(FFLAGS) stamp-framework-headers
	$(SUBMAKE) -C $(OBJROOT)/$*/bfd $(FFLAGS) headers stamp-framework-headers
	$(SUBMAKE) -C $(OBJROOT)/$*/opcodes $(FFLAGS) stamp-framework-headers
	$(SUBMAKE) -C $(OBJROOT)/$* $(FFLAGS) stamp-framework-headers-binutils
	$(SUBMAKE) -C $(OBJROOT)/$*/gdb/doc $(MFLAGS) VERSION='$(GDB_VERSION_STRING)'
	$(SUBMAKE) -C $(OBJROOT)/$* $(MFLAGS) stamp-framework-headers-gdb
	touch $@

$(OBJROOT)/%/stamp-build-core:
	$(SUBMAKE) -C $(OBJROOT)/$* configure-intl configure-libiberty configure-bfd configure-opcodes
	$(SUBMAKE) -C $(OBJROOT)/$*/intl $(SFLAGS) libintl.la
	$(SUBMAKE) -C $(OBJROOT)/$*/libiberty $(FFLAGS) $(FRAMEWORK_TARGET)
	$(SUBMAKE) -C $(OBJROOT)/$*/bfd $(FFLAGS) headers
	$(SUBMAKE) -C $(OBJROOT)/$*/bfd $(FFLAGS) $(FRAMEWORK_TARGET)
	$(SUBMAKE) -C $(OBJROOT)/$*/opcodes $(FFLAGS) $(FRAMEWORK_TARGET)
	$(SUBMAKE) -C $(OBJROOT)/$* configure-readline configure-intl
	$(SUBMAKE) -C $(OBJROOT)/$*/readline $(MFLAGS) all $(FRAMEWORK_TARGET)
	$(SUBMAKE) -C $(OBJROOT)/$*/intl $(MFLAGS)
	touch $@

$(OBJROOT)/%/stamp-build-binutils:
	$(SUBMAKE) -C $(OBJROOT)/$* configure-binutils
	$(SUBMAKE) -C $(OBJROOT)/$*/binutils $(FSFLAGS) VERSION='$(BINUTILS_VERSION)' VERSION_STRING='$(BINUTILS_VERSION_STRING)' all
	$(SUBMAKE) -C $(OBJROOT)/$* $(FFLAGS) stamp-framework-headers-binutils
	touch $@

$(OBJROOT)/%/stamp-build-gdb:
	$(SUBMAKE) -C $(OBJROOT)/$* configure-gdb
	$(SUBMAKE) -C $(OBJROOT)/$*/gdb -W version.in $(MFLAGS) $(FSFLAGS) VERSION='$(GDB_VERSION_STRING)' GDB_RC_VERSION='$(GDB_RC_VERSION)' gdb

$(OBJROOT)/%/stamp-build-gdb-framework:
	$(SUBMAKE) -C $(OBJROOT)/$* $(FFLAGS) stamp-framework-headers-gdb

$(OBJROOT)/%/stamp-build-gdb-docs:
	$(SUBMAKE) -C $(OBJROOT)/$*/gdb/doc $(MFLAGS) VERSION='$(GDB_VERSION_STRING)' gdb.info
	touch $@

TEMPLATE_HEADERS = config.h tm.h xm.h nm.h

install-frameworks-headers:
	$(INSTALL) -c -d $(CURRENT_ROOT)/$(PRIVATE_FRAMEWORKS_DIR)
	set -e;	for i in $(FRAMEWORKS); do \
		framedir=$(CURRENT_ROOT)/$(PRIVATE_FRAMEWORKS_DIR)/$${i}.framework; \
		$(INSTALL) -c -d $${framedir}/Versions/A/PrivateHeaders; \
		$(INSTALL) -c -d $${framedir}/Versions/A/Headers; \
		ln -sf A $${framedir}/Versions/Current; \
		ln -sf Versions/Current/PrivateHeaders $${framedir}/PrivateHeaders; \
		ln -sf Versions/Current/Headers $${framedir}/Headers; \
	done
	set -e; for i in $(FRAMEWORKS); do \
		l=`echo $${i} | sed -e 's/liberty/libiberty/;' -e 's/binutils/\./;' -e 's/gdb/\./;'`; \
		(cd $(OBJROOT)/$(firstword $(NATIVE_TARGETS))/$${l}/$${i}.framework/Versions/A \
		 && $(TAR) --exclude=CVS --exclude=.svn -cf - Headers) \
		| \
		(cd $(CURRENT_ROOT)/$(PRIVATE_FRAMEWORKS_DIR)/$${i}.framework/Versions/A \
		 && $(TAR) -xf -); \
	done
	set -e; for i in gdb; do \
		l=`echo $${i} | sed -e 's/liberty/libiberty/;' -e 's/binutils/\./;' -e 's/gdb/\./;'`; \
		rm -rf $(CURRENT_ROOT)/$(PRIVATE_FRAMEWORKS_DIR)/$${i}.framework/Versions/A/Headers/machine; \
		mkdir -p $(CURRENT_ROOT)/$(PRIVATE_FRAMEWORKS_DIR)/$${i}.framework/Versions/A/Headers/machine; \
		for j in $(NATIVE_TARGETS); do \
			mkdir -p $(CURRENT_ROOT)/$(PRIVATE_FRAMEWORKS_DIR)/$${i}.framework/Versions/A/Headers/machine/$${j}; \
			(cd $(OBJROOT)/$${j}/$${l}/$${i}.framework/Versions/A/Headers/machine \
			 && $(TAR) --exclude=CVS --exclude=.svn -cf - *) \
			| \
			(cd $(CURRENT_ROOT)/$(PRIVATE_FRAMEWORKS_DIR)/$${i}.framework/Versions/A/Headers/machine/$${j} \
			 && $(TAR) -xf -) \
		done; \
		for h in $(TEMPLATE_HEADERS); do \
			hg=`echo $${h} | sed -e 's/\.h//' -e 'y/abcdefghijklmnopqrstuvwxyz/ABCDEFGHIJKLMNOPQRSTUVWXYZ/'`; \
			rm -f $(CURRENT_ROOT)/$(PRIVATE_FRAMEWORKS_DIR)/$${i}.framework/Versions/A/Headers/$${h}; \
			ln -s machine/$${h} $(CURRENT_ROOT)/$(PRIVATE_FRAMEWORKS_DIR)/$${i}.framework/Versions/A/Headers/$${h}; \
			cat template.h | sed -e "s/@file@/$${h}/g" -e "s/@FILEGUARD@/_CONFIG_$${hg}_H_/" \
				> $(CURRENT_ROOT)/$(PRIVATE_FRAMEWORKS_DIR)/$${i}.framework/Versions/A/Headers/machine/$${h}; \
		done; \
	done

cross-install-frameworks-headers-finalize:

	cd "$(CURRENT_ROOT)/$(PRIVATE_FRAMEWORKS_DIR)/gdb.framework/Versions/A/Headers/machine" \
		&& cross_dirs=`find . -type d`; \
	set -e; for h in $(TEMPLATE_HEADERS); do \
		hg=`echo $${h} | sed -e 's/\.h//' -e 'y/abcdefghijklmnopqrstuvwxyz/ABCDEFGHIJKLMNOPQRSTUVWXYZ/'`; \
		aggregate_cross_file=$(CURRENT_ROOT)/$(PRIVATE_FRAMEWORKS_DIR)/gdb.framework/Versions/A/Headers/machine/$${h}; \
		echo "#ifndef _CONFIG_$${hg}_H_\n#define _CONFIG_$${hg}_H_\n#include <mach/mach.h>\n" > "$${aggregate_cross_file}" ; \
		for cross_dir in $${cross_dirs}; do \
			cross_dir=`echo $${cross_dir} | sed -e 's/^\.//' -e 's/\///'`; \
			if [[ -n "$${cross_dir}" && -e "$${cross_dir}/$${h}" ]]; then \
				host_arch=`echo "$${cross_dir}" | sed -e 's/\([^-]*\).*/\1/'`; \
				cross_arch=`echo "$${cross_dir}" | sed -e 's/.*--\([^-]*\).*/\1/'`; \
				if [[ "$${host_arch}" == "$${cross_arch}" ]]; then \
					echo "#if defined (NM_NEXTSTEP) && defined (TM_NEXTSTEP) && defined (__$${host_arch}__)" >> "$${aggregate_cross_file}" ; \
					echo "#include \"machine/$${cross_dir}/$${h}\"" >> "$${aggregate_cross_file}" ; \
				else \
					cross_arch_caps=`echo "$${cross_arch}" | sed -e 's/ppc/POWERPC/' -e 'y/abcdefghijklmnopqrstuvwxyz/ABCDEFGHIJKLMNOPQRSTUVWXYZ/'`; \
					echo "#if !defined (NM_NEXTSTEP) && defined (TM_NEXTSTEP) && defined (TARGET_$${cross_arch_caps}) && defined (__$${host_arch}__)" >> "$${aggregate_cross_file}" ; \
					echo "#include \"machine/$${cross_dir}/$${h}\"" >> "$${aggregate_cross_file}" ; \
				fi ; \
			fi ; \
		done ; \
		echo "#else\n#error unknown architecture\n#endif\n\n#endif\n" >> "$${aggregate_cross_file}" ; \
	done

cross-install-frameworks-headers:

	$(INSTALL) -c -d $(CURRENT_ROOT)/$(PRIVATE_FRAMEWORKS_DIR)

	set -e;	for i in $(FRAMEWORKS); do \
		framedir=$(CURRENT_ROOT)/$(PRIVATE_FRAMEWORKS_DIR)/$${i}.framework; \
		$(INSTALL) -c -d $${framedir}/Versions/A/PrivateHeaders; \
		$(INSTALL) -c -d $${framedir}/Versions/A/Headers; \
		ln -sf A $${framedir}/Versions/Current; \
		ln -sf Versions/Current/PrivateHeaders $${framedir}/PrivateHeaders; \
		ln -sf Versions/Current/Headers $${framedir}/Headers; \
	done

	set -e; for i in $(FRAMEWORKS); do \
		l=`echo $${i} | sed -e 's/liberty/libiberty/;' -e 's/binutils/\./;' -e 's/gdb/\./;'`; \
		(cd "$(OBJROOT)/$(HOST_ARCH_FULL)-apple-darwin--$(CROSS_ARCH_FULL)-apple-darwin/$${l}/$${i}.framework/Versions/A" \
		 && $(TAR) --exclude=CVS --exclude=.svn -cf - Headers) \
		| \
		(cd "$(CURRENT_ROOT)/$(PRIVATE_FRAMEWORKS_DIR)/$${i}.framework/Versions/A" \
		 && $(TAR) -xf -); \
	done

	rm -rf "$(CURRENT_ROOT)/$(PRIVATE_FRAMEWORKS_DIR)/gdb.framework/Versions/A/Headers/machine/$(HOST_ARCH)-apple-darwin--$(CROSS_ARCH)-apple-darwin"; \
	mkdir -p "$(CURRENT_ROOT)/$(PRIVATE_FRAMEWORKS_DIR)/gdb.framework/Versions/A/Headers/machine/$(HOST_ARCH)-apple-darwin--$(CROSS_ARCH)-apple-darwin"; \
	(cd "$(OBJROOT)/$(HOST_ARCH_FULL)-apple-darwin--$(CROSS_ARCH_FULL)-apple-darwin/gdb.framework/Versions/A/Headers/machine" && $(TAR) --exclude=CVS --exclude=.svn -cf - *) \
		| (cd "$(CURRENT_ROOT)/$(PRIVATE_FRAMEWORKS_DIR)/gdb.framework/Versions/A/Headers/machine/$(HOST_ARCH)-apple-darwin--$(CROSS_ARCH)-apple-darwin" && $(TAR) -xf -);

cross-install-frameworks-headers-remove:

	for h in $(TEMPLATE_HEADERS); do \
		hg=`echo $${h} | sed -e 's/\.h//' -e 'y/abcdefghijklmnopqrstuvwxyz/ABCDEFGHIJKLMNOPQRSTUVWXYZ/'`; \
		rm -f $(CURRENT_ROOT)/$(PRIVATE_FRAMEWORKS_DIR)/gdb.framework/Versions/A/Headers/$${h}; \
		ln -s machine/$${h} $(CURRENT_ROOT)/$(PRIVATE_FRAMEWORKS_DIR)/gdb.framework/Versions/A/Headers/$${h}; \
		cat template.h | sed -e "s/@file@/$${h}/g" -e "s/@FILEGUARD@/_CONFIG_$${hg}_H_/" \
			> $(CURRENT_ROOT)/$(PRIVATE_FRAMEWORKS_DIR)/gdb.framework/Versions/A/Headers/machine/$${h}; \
	done; 

install-frameworks-resources:

	$(INSTALL) -c -d $(CURRENT_ROOT)/$(PRIVATE_FRAMEWORKS_DIR)

	set -e;	for i in $(GDB_FRAMEWORKS); do \
		framedir=$(CURRENT_ROOT)/$(PRIVATE_FRAMEWORKS_DIR)/$${i}.framework; \
		$(INSTALL) -c -d $${framedir}/Versions/A/Resources; \
		ln -sf Versions/Current/Resources $${framedir}/Resources; \
		$(INSTALL) -c -d $${framedir}/Versions/A/Resources/English.lproj; \
		cat $(SRCROOT)/Info-macos.template | sed -e "s/@NAME@/$$i/g" -e 's/@VERSION@/$(GDB_RC_VERSION)/g' > \
			$${framedir}/Versions/A/Resources/Info-macos.plist; \
	done
	set -e;	for i in $(BINUTILS_FRAMEWORKS); do \
		framedir=$(CURRENT_ROOT)/$(PRIVATE_FRAMEWORKS_DIR)/$${i}.framework; \
		$(INSTALL) -c -d $${framedir}/Versions/A/Resources; \
		ln -sf Versions/Current/Resources $${framedir}/Resources; \
		$(INSTALL) -c -d $${framedir}/Versions/A/Resources/English.lproj; \
		cat $(SRCROOT)/Info-macos.template | sed -e "s/@NAME@/$$i/g" -e 's/@VERSION@/$(BINUTILS_RC_VERSION)/g' > \
			$${framedir}/Versions/A/Resources/Info-macos.plist; \
	done

install-frameworks-macosx:

	$(SUBMAKE) CURRENT_ROOT=$(SYMROOT) install-frameworks-headers
	$(SUBMAKE) CURRENT_ROOT=$(DSTROOT) install-frameworks-headers
	$(SUBMAKE) CURRENT_ROOT=$(SYMROOT) install-frameworks-resources
	$(SUBMAKE) CURRENT_ROOT=$(DSTROOT) install-frameworks-resources

install-gdb-common:

	set -e; for dstroot in $(SYMROOT) $(DSTROOT); do \
		\
		$(INSTALL) -c -d $${dstroot}/$(DEVEXEC_DIR); \
		$(INSTALL) -c -d $${dstroot}/$(MAN_DIR); \
		\
		docroot="$${dstroot}/$(SYSTEM_DEVELOPER_TOOLS_DOC_DIR)/gdb"; \
		\
		$(INSTALL) -c -d "$${docroot}"; \
		\
		$(INSTALL) -c -m 644 $(SRCROOT)/doc/refcard.pdf "$${docroot}/refcard.pdf"; \
		\
	done;

install-gdb-macosx-common: install-gdb-common

	set -e; for dstroot in $(SYMROOT) $(DSTROOT); do \
		\
		$(INSTALL) -c -d $${dstroot}/$(LIBEXEC_GDB_DIR); \
		\
		docroot="$${dstroot}/$(SYSTEM_DEVELOPER_TOOLS_DOC_DIR)/gdb"; \
		\
		for i in gdb gdbint stabs; do \
			$(INSTALL) -c -d "$${docroot}/$${i}"; \
			(cd "$${docroot}/$${i}" && \
				$(SRCROOT)/texi2html \
					-split_chapter \
					-I$(OBJROOT)/$(firstword $(NATIVE_TARGETS))/gdb/doc \
					-I$(SRCROOT)/src/readline/doc \
					-I$(SRCROOT)/src/gdb/mi \
					$(SRCROOT)/src/gdb/doc/$${i}.texinfo); \
		done; \
		\
		$(INSTALL) -c -d $${dstroot}/$(MAN_DIR)/man1; \
		$(INSTALL) -c -m 644 $(SRCROOT)/src/gdb/gdb.1 $${dstroot}/$(MAN_DIR)/man1/gdb.1; \
		perl -pi -e 's,GDB_DOCUMENTATION_DIRECTORY,$(SYSTEM_DEVELOPER_TOOLS_DOC_DIR)/gdb,' $${dstroot}/$(MAN_DIR)/man1/gdb.1; \
		\
		$(INSTALL) -c -d $${dstroot}/usr/local/OpenSourceLicenses; \
		$(INSTALL) -c -d $${dstroot}/usr/local/OpenSourceVersions; \
		$(INSTALL) -c -m 644 $(SRCROOT)/gdb.plist $${dstroot}/usr/local/OpenSourceVersions; \
		$(INSTALL) -c -m 644 $(SRCROOT)/gdb.txt $${dstroot}/usr/local/OpenSourceLicenses; \
		\
		sed -e 's/version=.*/version=$(GDB_VERSION)-$(GDB_RC_VERSION)/' \
			< $(SRCROOT)/gdb.sh > $${dstroot}/usr/bin/gdb; \
		chmod 755 $${dstroot}/usr/bin/gdb; \
		\
	done;

install-gdb-macosx: install-gdb-macosx-common

	set -e; for target in $(filter-out x86_64-apple-darwin, $(CANONICAL_ARCHS)); do \
		lipo -create $(OBJROOT)/$${target}--$${target}/gdb/gdb \
			-output $(SYMROOT)/$(LIBEXEC_GDB_DIR)/gdb-$${target}; \
		dsymutil -o $(SYMROOT)/$(LIBEXEC_GDB_DIR)/gdb-$${target}.dSYM \
                         $(SYMROOT)/$(LIBEXEC_GDB_DIR)/gdb-$${target}; \
	 	strip -S -o $(DSTROOT)/$(LIBEXEC_GDB_DIR)/gdb-$${target} \
			$(SYMROOT)/$(LIBEXEC_GDB_DIR)/gdb-$${target}; \
		cp $(DSTROOT)/$(LIBEXEC_GDB_DIR)/gdb-$${target} $(SYMROOT)/$(LIBEXEC_GDB_DIR)/gdb-$${target}; \
	done

# When this target is invoked, NATIVE is the binary that we will be
# outputting and HOSTCOMBOS are the binaries that will be combined into
# that. For instance,
#
# HOSTCOMBOS == i386-apple-darwin--i386-apple-darwin x86_64-apple-darwin--x86_64-apple-darwin powerpc-apple-darwin--i386-apple-darwin
# NATIVE == i386-apple-darwin

# NATIVE i386 is a special case where we add the x86_64-apple-darwin
# variant manually.

install-gdb-fat: install-gdb-macosx-common
	lipo -create $(patsubst %,$(OBJROOT)/%/gdb/gdb,$(HOSTCOMBOS)) \
	     -output $(SYMROOT)/$(LIBEXEC_GDB_DIR)/gdb-$(NATIVE)

dsym-and-strip-fat-gdbs:
	set -e; for target in $(filter-out x86_64-apple-darwin, $(ARCHS_TO_INSTALL)); do \
		dsymutil -o $(SYMROOT)/$(LIBEXEC_GDB_DIR)/gdb-$${target}.dSYM \
                         $(SYMROOT)/$(LIBEXEC_GDB_DIR)/gdb-$${target}; \
	 	strip -S -o $(DSTROOT)/$(LIBEXEC_GDB_DIR)/gdb-$${target} \
			$(SYMROOT)/$(LIBEXEC_GDB_DIR)/gdb-$${target}; \
		cp $(DSTROOT)/$(LIBEXEC_GDB_DIR)/gdb-$${target} \
                   $(SYMROOT)/$(LIBEXEC_GDB_DIR)/gdb-$${target}; \
	done

install-binutils-macosx:

	set -e; for i in $(BINUTILS_BINARIES); do \
		instname=`echo $${i} | sed -e 's/\\-new//'`; \
		lipo -create $(patsubst %,$(OBJROOT)/%/binutils/$${i},$(NATIVE_TARGETS)) \
			-output $(SYMROOT)/$(LIBEXEC_BINUTILS_DIR)/$${instname}; \
		strip -S -o $(DSTROOT)/$(LIBEXEC_BINUTILS_DIR)/$${instname} $(SYMROOT)/$(LIBEXEC_BINUTILS_DIR)/$${instname}; \
	done

# "procmod" was a new group (2005-09-27) which was not initially present on
# all the systems, so we use a '-' prefix on that loop for now so that the
# errors do NOT halt the build. 
install-chmod-macosx:
	set -e; if [ `whoami` = 'root' ]; then \
		for dstroot in $(SYMROOT) $(DSTROOT); do \
			chown -R root:wheel $${dstroot}; \
			chmod -R  u=rwX,g=rX,o=rX $${dstroot}; \
			chmod a+x $${dstroot}/$(LIBEXEC_GDB_DIR)/*; \
			chmod a+x $${dstroot}/$(DEVEXEC_DIR)/*; \
		done; \
	fi
	-set -e; if [ `whoami` = 'root' ]; then \
		for dstroot in $(SYMROOT) $(DSTROOT); do \
			chgrp procmod $${dstroot}/$(LIBEXEC_GDB_DIR)/gdb* && chmod g+s $${dstroot}/$(LIBEXEC_GDB_DIR)/gdb*; \
			chgrp procmod $${dstroot}/$(LIBEXEC_GDB_DIR)/plugins/MacsBug/MacsBug_plugin && chmod g+s $${dstroot}/$(LIBEXEC_GDB_DIR)/plugins/MacsBug/MacsBug_plugin; \
		done; \
	fi

install-chmod-macosx-noprocmod:
	set -e; for dstroot in $(SYMROOT) $(DSTROOT); do \
			chown -R root:wheel $${dstroot}; \
			chmod -R  u=rwX,g=rX,o=rX $${dstroot}; \
			chmod a+x $${dstroot}/$(LIBEXEC_GDB_DIR)/*; \
			chmod a+x $${dstroot}/$(DEVEXEC_DIR)/*; \
			chmod 755 $${dstroot}/$(LIBEXEC_GDB_DIR)/gdb-*-apple-darwin; \
		done

install-source:
	$(INSTALL) -c -d $(DSTROOT)/$(SOURCE_DIR)
	$(TAR) --exclude=CVS --exclude=.svn -C $(SRCROOT) -cf - . | $(TAR) -C $(DSTROOT)/$(SOURCE_DIR) -xf -

all: build
	if test -e src/Makefile; then unset CPP && $(MAKE) -C src; fi

mostlyclean:
	if test -e src/Makefile; then \
	  unset CPP && $(MAKE) -i -C src mostlyclean; fi
clean: mostlyclean
	if test -e src/Makefile; then \
	  unset CPP && $(MAKE) -i -C src clean; fi
	$(RM) -r $(OBJROOT)
	$(RM) -v *~ stamp-*
	$(RM) .DS_Store autoscan.log
	$(RM) -r autom4te.cache || rmdir autom4te.cache

distclean: clean
	if test -e src/Makefile; then \
	  unset CPP && $(MAKE) -i -C src distclean; fi
	$(RM) configure config.log config.h.*
.PHONY: mostlyclean distclean

check-args:
ifneq (,$(filter-out i386-apple-darwin, $(filter-out powerpc-apple-darwin, $(filter-out x86_64-apple-darwin, $(filter-out arm-apple-darwin, $(CANONICAL_ARCHS))))))
	echo "Unknown architecture string: \"$(CANONICAL_ARCHS)\""
	exit 1
endif
.PHONY: check-args

configure-headers:
ifneq ($(NATIVE_TARGETS),)
	$(SUBMAKE) $(patsubst %,$(OBJROOT)/%/stamp-rc-configure, $(NATIVE_TARGETS))
endif

configure: 
ifneq ($(NATIVE_TARGETS),)
	$(SUBMAKE) $(patsubst %,$(OBJROOT)/%/stamp-rc-configure, $(NATIVE_TARGETS))
endif
ifneq ($(CROSS_TARGETS),)
	$(SUBMAKE) $(patsubst %,$(OBJROOT)/%/stamp-rc-configure-cross, $(CROSS_TARGETS))
endif
	if test ! -e configure; then \
	  if test -e configure.sh; then \
	    cp -v configure.sh configure; \
	  elif test -d .git && test -x "`which git`"; then \
	    if test -n "`git status -s configure | grep D`"; then \
	      echo "configure appears to have been deleted; attempting to restore it..."; \
	      git restore configure; \
	    fi; \
	  elif test -e $(SRCROOT)/configure; then \
	    cp -v $(SRCROOT)/configure configure; \
	  elif test -e $(OBJROOT)/configure; then \
	    cp -v $(OBJROOT)/configure configure; \
	  elif test -e $(SYMROOT)/configure; then \
	    cp -v $(SYMROOT)/configure configure; \
	  elif test -e $(DSTROOT)/configure; then \
	    cp -v $(DSTROOT)/configure configure; \
	  else \
	    touch configure; \
	  fi; \
	else \
	  stat configure; \
	fi

build-headers: configure
	$(SUBMAKE) configure-headers
ifneq ($(NATIVE_TARGETS),)
	$(SUBMAKE) $(patsubst %,$(OBJROOT)/%/stamp-build-headers, $(NATIVE_TARGETS))
endif
# This CROSS_TARGETS thing had previously been commented out:
ifneq ($(CROSS_TARGETS),)
	$(SUBMAKE) $(patsubst %,$(OBJROOT)/%/stamp-build-headers, $(CROSS_TARGETS))
endif

build-core: configure
	if test "x$(SUBMAKE)" != "x"; then \
	  test -n "$(SUBMAKE)" && $(SUBMAKE) configure; \
	elif test "x$(MAKE)" != "x"; then \
	  test -n "$(MAKE)" && $(MAKE) configure; \
	elif test -x "`which make`"; then \
	  make configure; \
	else \
	  echo "unsure how to ensure that we have configure present..."; \
	fi
ifneq ($(NATIVE_TARGETS),)
	$(SUBMAKE) $(patsubst %,$(OBJROOT)/%/stamp-build-core, $(NATIVE_TARGETS)) 
endif
ifneq ($(CROSS_TARGETS),)
	$(SUBMAKE) $(patsubst %,$(OBJROOT)/%/stamp-build-core, $(CROSS_TARGETS))
endif

build-binutils: configure
	$(SUBMAKE) configure
ifneq ($(NATIVE_TARGETS),)
	$(SUBMAKE) $(patsubst %,$(OBJROOT)/%/stamp-build-binutils, $(NATIVE_TARGETS))
endif

build-gdb: configure
	$(SUBMAKE) configure
ifneq ($(NATIVE_TARGETS),)
	$(SUBMAKE) $(patsubst %,$(OBJROOT)/%/stamp-build-gdb, $(NATIVE_TARGETS))
	$(SUBMAKE) $(patsubst %,$(OBJROOT)/%/stamp-build-gdb-framework, $(NATIVE_TARGETS))
endif
ifneq ($(CROSS_TARGETS),)
	$(SUBMAKE) $(patsubst %,$(OBJROOT)/%/stamp-build-gdb, $(CROSS_TARGETS))
endif

build-gdb-docs: configure
	$(MAKE) configure
ifneq ($(NATIVE_TARGETS),)
	$(MAKE) $(patsubst %,$(OBJROOT)/%/stamp-build-gdb-docs, $(NATIVE_TARGETS))
endif
ifneq ($(CROSS_TARGETS),)
	$(MAKE) $(patsubst %,$(OBJROOT)/%/stamp-build-gdb-docs, $(CROSS_TARGETS))
endif

build: check-args build-core build-binutils build-gdb build-gdb-docs
	$(SUBMAKE) check-args
	$(SUBMAKE) build-core
	$(SUBMAKE) build-binutils
	$(SUBMAKE) build-gdb 
	$(SUBMAKE) build-gdb-docs 

install-clean:
	$(RM) -rf $(DSTROOT)

install-macosx: install-clean install-frameworks-macosx install-binutils-macosx install-gdb-fat dsym-and-strip-fat-gdbs install-chmod-macosx-noprocmod
	$(SUBMAKE) install-clean
	$(SUBMAKE) install-frameworks-macosx 
	$(SUBMAKE) install-binutils-macosx 
ifneq (,$(findstring i386-apple-darwin, $(CANONICAL_ARCHS)))
	$(SUBMAKE) install-gdb-fat HOSTCOMBOS="$(sort $(filter i386-apple-darwin--i386-apple-darwin, $(NATIVE_TARGETS)) $(filter %--i386-apple-darwin, $(CROSS_TARGETS)) $(filter x86_64-apple-darwin--x86_64-apple-darwin, $(NATIVE_TARGETS)) $(filter %--x86_64-apple-darwin, $(CROSS_TARGETS)))" NATIVE=i386-apple-darwin
endif
ifneq (,$(findstring powerpc-apple-darwin, $(CANONICAL_ARCHS)))
	$(SUBMAKE) install-gdb-fat HOSTCOMBOS="$(sort $(filter powerpc-apple-darwin--powerpc-apple-darwin, $(NATIVE_TARGETS)) $(filter %--powerpc-apple-darwin, $(CROSS_TARGETS)))" NATIVE=powerpc-apple-darwin
endif
ifneq (,$(findstring arm-apple-darwin, $(CANONICAL_ARCHS)))
	$(SUBMAKE) install-gdb-fat HOSTCOMBOS="$(sort $(filter arm-apple-darwin--arm-apple-darwin, $(NATIVE_TARGETS)) $(filter %--arm-apple-darwin, $(CROSS_TARGETS)))" NATIVE=arm-apple-darwin
else
	$(SUBMAKE) install-gdb-fat HOSTCOMBOS="$(sort $(filter %--arm-apple-darwin, $(CROSS_TARGETS)))" NATIVE=arm-apple-darwin
endif
	$(SUBMAKE) dsym-and-strip-fat-gdbs ARCHS_TO_INSTALL="$(CANONICAL_ARCHS)"
	$(SUBMAKE) dsym-and-strip-fat-gdbs ARCHS_TO_INSTALL="arm-apple-darwin"
	$(SUBMAKE) install-chmod-macosx-noprocmod

install-macsbug:
	$(SUBMAKE) -C $(SRCROOT)/macsbug GDB_BUILD_ROOT=$(DSTROOT) BINUTILS_BUILD_ROOT=$(DSTROOT) SRCROOT=$(SRCROOT)/macsbug OBJROOT=$(OBJROOT)/powerpc-apple-darwin--powerpc-apple-darwin/macsbug SYMROOT=$(SYMROOT) DSTROOT=$(DSTROOT) install
 
install: check-args build install-macosx install-chmod-macosx-noprocmod
	$(SUBMAKE) check-args
	$(SUBMAKE) build
	$(SUBMAKE) $(MACOSX_FLAGS) install-macosx
	$(SUBMAKE) $(MACOSX_FLAGS) install-chmod-macosx-noprocmod

installhdrs: check-args configure-headers build-headers install-clean install-frameworks-headers cross-installhdrs
ifeq ($(RC_CROSS_ARCHS),)
	$(SUBMAKE) check-args
	$(SUBMAKE) configure-headers
	$(SUBMAKE) build-headers
	$(SUBMAKE) install-clean
	$(SUBMAKE) $(MACOSX_FLAGS) CURRENT_ROOT=$(SYMROOT) install-frameworks-headers
	$(SUBMAKE) $(MACOSX_FLAGS) CURRENT_ROOT=$(DSTROOT) install-frameworks-headers
else
	$(SUBMAKE) $(MACOSX_FLAGS) cross-installhdrs
endif

installsrc: check
	$(SUBMAKE) check
	$(TAR) --dereference --exclude=CVS --exclude=.svn --exclude=src/contrib --exclude=src/dejagnu --exclude=src/etc --exclude=src/expect --exclude=src/sim --exclude=src/tcl --exclude=src/texinfo --exclude=src/utils -cf - . | $(TAR) -C $(SRCROOT) -xf -


check:
	@[ -z "`find . -name \*~ -o -name .\#\*`" ] || \
	   (echo; echo 'Emacs or CVS backup files present; not copying:'; \
           find . \( -name \*~ -o -name .#\* \) -print | sed 's,^[.]/,  ,'; \
           echo Suggest: ; \
           echo '    ' find . \\\( -name \\\*~ -o -name .#\\\* \\\) -exec rm -f \{\} \\\; -print ; \
           echo; \
           exit 1)
	$(MAKE) -C src check

test: # (unsure if it is okay to make this equivalent to check?)
.PHONY: test

ifndef srcdir
srcdir = .
endif

.PHONY: update-ChangeLog
update-ChangeLog:
	    if test -d $(srcdir)/.git; then                         \
	       $(srcdir)/build-aux/gitlog-to-changelog              \
	          --format='%s%n%n%b%n' --no-cluster                \
	          --strip-tab --strip-cherry-pick                   \
	          -- $$(cat $(srcdir)/.last-cl-gen)..               \
	        >ChangeLog.tmp                                      \
	      && git rev-list -n 1 HEAD >.last-cl-gen.tmp           \
	      && (echo; cat $(srcdir)/ChangeLog) >>ChangeLog.tmp    \
	      && mv -f ChangeLog.tmp $(srcdir)/ChangeLog            \
	      && mv -f .last-cl-gen.tmp $(srcdir)/.last-cl-gen      \
	      && rm -f ChangeLog.tmp;                               \
	    fi

EXTRA_DIST += .last-cl-gen

## so subdirs can use automake:
am--refresh:
	@:
