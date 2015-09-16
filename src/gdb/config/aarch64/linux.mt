# Target: AArch64 based machine running GNU/Linux
# Based off of the ARM equivalent

DEPRECATED_TM_FILE= tm-linux.h
TDEPFILES= aarch64-tdep.o aarch64-linux-tdep.o glibc-tdep.o \
	solib.o solib-svr4.o solib-legacy.o symfile-mem.o corelow.o \
	linux-tdep.o
