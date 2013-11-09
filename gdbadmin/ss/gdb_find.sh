#!/bin/sh

# A find that prunes files that GDB users shouldn't be interested in.
# Use sort to order files alphabetically.

find "$@" \
    -name testsuite -prune -o \
    -name gdbserver -prune -o \
    -name gnulib -prune -o \
    -name osf-share -prune -o \
    -name '*-stub.c' -prune -o \
    -name '*-exp.c' -prune -o \
    -name ada-lex.c -prune -o \
    -name cp-name-parser.c -prune -o \
    -type f -name '*.[lyhc]' -print | sort
