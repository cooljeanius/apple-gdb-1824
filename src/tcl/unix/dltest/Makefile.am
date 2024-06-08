## automake Makefile for Tcl's dltest testsuite
# This Makefile is used to create several test cases for Tcl's load
# command.  It also illustrates how to take advantage of configuration
# exported by Tcl to set up Makefiles for shared libraries.
# RCS: @(#) Makefile.in,v 1.5 2003/01/21 19:40:21 hunt Exp

ACLOCAL_AMFLAGS = -I../m4 --install

AUTOMAKE_OPTIONS = 1.14 foreign dejagnu no-dist info-in-builddir -Wno-obsolete

DISTCLEANFILES = *~

TCL_DBGX = @TCL_DBGX@
CC = @CC@
LIBS = @TCL_BUILD_STUB_LIB_SPEC@ @DL_LIBS@ @LIBS@ @MATH_LIBS@
AC_FLAGS = @EXTRA_CFLAGS@
SHLIB_CFLAGS = @SHLIB_CFLAGS@
SHLIB_LD = @SHLIB_LD@
SHLIB_LD_LIBS = @SHLIB_LD_LIBS@
SHLIB_SUFFIX = @SHLIB_SUFFIX@
SRC_DIR = @srcdir@
TCL_VERSION = @TCL_VERSION@

# Architectures:
BUILD_ARCHITECTURE = @BUILD_ARCHITECTURE@
HOST_ARCHITECTURE = @HOST_ARCHITECTURE@
TARGET_ARCHITECTURE = @TARGET_ARCHITECTURE@
C_ARCHFLAGS = @C_ARCHFLAGS@

CFLAGS_DEBUG = @CFLAGS_DEBUG@
CFLAGS_OPTIMIZE = @CFLAGS_OPTIMIZE@

## this one should be override-able:
CFLAGS = @CFLAGS@ @CFLAGS_DEFAULT@
## Put our own flags here:
CC_SWITCHES = $(CFLAGS) -I${SRC_DIR}/../../generic -DTCL_MEM_DEBUG \
  ${SHLIB_CFLAGS} -DUSE_TCL_STUBS ${AC_FLAGS} $(C_ARCHFLAGS)

## Likewise with linker flags:
LDFLAGS = @LDFLAGS@
LD_SWITCHES = $(LDFLAGS) $(CFLAGS) -L${SRC_DIR}/../../generic \
  ${SHLIB_CFLAGS} ${AC_FLAGS} $(C_ARCHFLAGS)

all: pkga${SHLIB_SUFFIX} pkgb${SHLIB_SUFFIX} pkgc${SHLIB_SUFFIX} \
  pkgd${SHLIB_SUFFIX} pkge${SHLIB_SUFFIX}
	@touch ../dltest.marker

pkga${SHLIB_SUFFIX}: $(SRC_DIR)/pkga.c
	$(CC) -c $(CC_SWITCHES) $(SRC_DIR)/pkga.c
	${SHLIB_LD} $(LD_SWITCHES) -o pkga${SHLIB_SUFFIX} pkga.o \
	  ${SHLIB_LD_LIBS}

pkgb${SHLIB_SUFFIX}: $(SRC_DIR)/pkgb.c
	$(CC) -c $(CC_SWITCHES) $(SRC_DIR)/pkgb.c
	${SHLIB_LD} $(LD_SWITCHES) -o pkgb${SHLIB_SUFFIX} pkgb.o \
	  ${SHLIB_LD_LIBS}

pkgc${SHLIB_SUFFIX}: $(SRC_DIR)/pkgc.c
	$(CC) -c $(CC_SWITCHES) $(SRC_DIR)/pkgc.c
	${SHLIB_LD} $(LD_SWITCHES) -o pkgc${SHLIB_SUFFIX} pkgc.o \
	  ${SHLIB_LD_LIBS}

pkgd${SHLIB_SUFFIX}: $(SRC_DIR)/pkgd.c
	$(CC) -c $(CC_SWITCHES) $(SRC_DIR)/pkgd.c
	${SHLIB_LD} $(LD_SWITCHES) -o pkgd${SHLIB_SUFFIX} pkgd.o \
	  ${SHLIB_LD_LIBS}

pkge${SHLIB_SUFFIX}: $(SRC_DIR)/pkge.c
	$(CC) -c $(CC_SWITCHES) $(SRC_DIR)/pkge.c
	${SHLIB_LD} $(LD_SWITCHES) -o pkge${SHLIB_SUFFIX} pkge.o \
	  ${SHLIB_LD_LIBS}

clean-local:
	rm -f *.o *${SHLIB_SUFFIX} config.cache config.log config.status
	rm -f lib.exp ../dltest.marker

distclean-local: @MAINT@ clean-local
	rm -f Makefile Makefile*orig
	-rm -rf autom4te.cache || rmdir autom4te.cache

## automake will only generate this rule depending on the directory from
## which it was run:
am--refresh:
	@if test "x$(subdir)" != "xdltest"; then \
	  echo "warning: autoreconf screwed up..." >&2; \
	else \
	  echo "making '$@'"; \
	fi
.PHONY: am--refresh
## (a case study in "recursive automake considered harmful")
