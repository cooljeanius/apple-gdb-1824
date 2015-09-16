# Target: SPARC64
# solib.o and procfs.o taken out for now. We do NOT have shlibs yet,
# and the elf version requires procfs.o but the a.out version does NOT.
# Then again, having procfs.o in a target makefile fragment seems wrong.
TDEPFILES = sparc-tdep.o
TM_FILE= tm-sp64.h

# Need gcc for long long support.
CC = gcc
