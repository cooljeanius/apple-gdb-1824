/* macosx/macosx-nat-inferior-stubs.h -*- C -*- */

#ifndef __GDB_MACOSX_NAT_INFERIOR_STUBS_H__
#define __GDB_MACOSX_NAT_INFERIOR_STUBS_H__ 1

#include "defs.h"
#include "target.h"

extern int attach(int);
extern void detach(int);
extern void kill_inferior(void);
extern void child_resume(int, int, enum target_signal);

#endif /* !__GDB_MACOSX_NAT_INFERIOR_STUBS_H__ */

/* EOF */
