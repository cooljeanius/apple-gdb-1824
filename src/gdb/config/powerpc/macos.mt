# macos.mt
# Target: PowerMac (PowerPC running MacOS Classic)

MT_CFLAGS = -DTARGET_POWERPC

TDEPFILES= rs6000-tdep.o xcoffread.o ppc-linux-tdep.o solib.o solib-svr4.o
TM_FILE= tm-macos.h

SIM_OBS = remote-sim.o
SIM = ../sim/ppc/libsim.a
