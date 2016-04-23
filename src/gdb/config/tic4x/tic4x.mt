# Target: tic4x
TDEPFILES= tic4x-tdep.o
SIM_OBS = remote-sim.o
SIM = ../sim/tic4x/libsim.a
GDBSERVER_DEPFILES= low-sim.o
GDBSERVER_LIBS = ../../sim/tic4x/libsim.a ../../bfd/libbfd.a ../../libiberty/libiberty.a -lm

