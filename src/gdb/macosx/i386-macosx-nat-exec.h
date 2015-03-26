/* i386-macosx-nat-exec.h -*- C -*- */

#ifndef __GDB_I386_MACOSX_NAT_EXEC_H__
#define __GDB_I386_MACOSX_NAT_EXEC_H__ 1

#include "defs.h" /* for ptid_t */
#include "target.h" /* for struct target_ops */

extern int i386_macosx_target_stopped_data_address(struct target_ops *targ,
                                                   CORE_ADDR *addr);

extern int i386_macosx_can_use_hw_breakpoint(int uu1, int uu2, int uu3);

extern void i386_macosx_child_post_startup_inferior(ptid_t pid);

#endif /* !__GDB_I386_MACOSX_NAT_EXEC_H__ */

/* EOF */
