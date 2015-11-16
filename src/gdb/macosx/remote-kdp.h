/* macosx/remote-kdp.h -*- C -*- */

#ifndef __GDB_REMOTE_KDP_H__
#define __GDB_REMOTE_KDP_H__

#include "defs.h"
#include "target.h"

extern int kdp_can_use_hw_breakpoint(int, int, int);

extern int kdp_stopped_by_watchpoint(void);

extern int kdp_stopped_data_address(struct target_ops *, CORE_ADDR *);

extern int kdp_insert_watchpoint(CORE_ADDR, int, int);

extern int kdp_remove_watchpoint(CORE_ADDR, int, int);

extern int kdp_insert_hw_breakpoint(CORE_ADDR, gdb_byte *);

extern int kdp_remove_hw_breakpoint(CORE_ADDR, gdb_byte *);

#endif /* !__GDB_REMOTE_KDP_H__ */

/* EOF */
