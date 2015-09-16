# macos.mt
# Target: really old Mac (MacOS Classic running on an old m68k Mac)

TDEPFILES= m68k-tdep.o xcoffread.o m68k-linux-tdep.o solib.o solib-svr4.o
# mac-nat.o and mac-xdep.o go in the ".mh" version
TM_FILE= tm-mac.h
