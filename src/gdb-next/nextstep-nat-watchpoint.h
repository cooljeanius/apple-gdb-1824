/*
 *  nextstep-nat-watchpoint.h
 *  gdb
 *
 *  Created by Eric Gallager on 2/25/18.
 *
 */

#ifndef __NEXTSTEP_NAT_WATCHPOINT_H__
#define __NEXTSTEP_NAT_WATCHPOINT_H__ 1

struct target_waitstatus;

extern int next_mach_stopped_by_watchpoint(struct target_waitstatus *, int,
					   int);

#endif /* !__NEXTSTEP_NAT_WATCHPOINT_H__ */

/* EOF */
