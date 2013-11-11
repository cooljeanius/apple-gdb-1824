# Target: AVR
TDEPFILES= avr-tdep.o

#
# There is no simulator provided with gdb (yet).
#
# See <http://savannah.gnu.org/projects/simulavr/> for the simulator
# used during development of avr support for gdb.
#
# Simulator: AVR
FUTURE_SIM_OBS = remote-sim.o
FUTURE_SIM = ../sim/avr/libsim.a
