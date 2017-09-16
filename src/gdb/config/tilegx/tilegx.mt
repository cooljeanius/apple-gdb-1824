# Target: tilegx
TDEPFILES= tilegx-tdep.o
SIM_OBS = remote-sim.o
SIM = ../sim/tilegx/libsim.a
GDBSERVER_DEPFILES= low-sim.o
GDBSERVER_LIBS = ../../sim/tilegx/libsim.a ../../bfd/libbfd.a ../../libiberty/libiberty.a -lm
