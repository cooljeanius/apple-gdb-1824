# Target: Intel 386 running NeXTStep

MT_CFLAGS = -DTARGET_I386 -D__NeXT__ ${acl_cv_wp}-I$(srcdir)/../gdb-next

VPATH += $(srcdir)/../gdb-next

TDEPFILES= i386-tdep.o i387-tdep.o i386-next-tdep.o
TM_FILE= tm-i386-next.h
DEPRECATED_TM_FILE= tm-i386-next.h
