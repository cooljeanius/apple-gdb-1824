#!/bin/sh
# This script is deprecated.

TMPDIR='c:\temp'; export TMPDIR

SHELL='c:/apple/library/executables/sh.exe'; export SHELL
CONFIG_SHELL='c:/apple/library/executables/sh.exe'; export CONFIG_SHELL

if test -e src/zlib/contrib/dotzlib/DotZLib/DotZLib.csproj && test -x "$(which msbuild)"; then \
    cd src/zlib/contrib/dotzlib/DotZLib && msbuild; \
else \
make \
    SHELL='c:/apple/library/executables/sh.exe' \
    CONFIG_SHELL='c:/apple/library/executables/sh.exe' \
    YACC=bison \
    YFLAGS=-y \
    AWK=gawk \
    CC=gcc \
    PICFLAG= \
    "$*"; \
fi
