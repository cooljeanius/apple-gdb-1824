# Target: tic54x
TDEPFILES= tic54x-tdep.o
SIM_OBS = remote-sim.o
SIM = ../sim/tic54x/libsim.a
GDBSERVER_DEPFILES= low-sim.o
GDBSERVER_LIBS = ../../sim/tic54x/libsim.a ../../bfd/libbfd.a ../../libiberty/libiberty.a -lm

