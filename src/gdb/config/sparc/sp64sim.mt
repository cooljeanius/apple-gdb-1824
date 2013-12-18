# Target: SPARC64 (with simulator)
# solib.o and procfs.o taken out for now. We do NOT have shlibs yet,
# and the elf version requires procfs.o but the a.out version does NOT.
# Then again, having procfs.o in a target makefile fragment seems wrong.
TDEPFILES = sparc-tdep.o
TM_FILE= tm-sp64.h

# Need gcc for long long support.
CC = gcc

MH_CFLAGS = -I${srcdir}/../sim/sp64
SIM_OBS = remote-sim.o
SIM = ../sim/sp64/libsim.a
