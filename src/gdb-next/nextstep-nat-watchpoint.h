/*
 *  nextstep-nat-watchpoint.h
 *  gdb
 *
 *  Created by Eric Gallager on 2/25/18.
 *
 */

#ifndef __NEXTSTEP_NAT_WATCHPOINT_H__
#define __NEXTSTEP_NAT_WATCHPOINT_H__ 1

enum bptype;

struct target_waitstatus;

extern int next_mach_stopped_by_watchpoint(struct target_waitstatus *, int,
					   int);
extern int next_mach_can_use_hw_watchpoint(enum bptype, int, enum bptype);

#endif /* !__NEXTSTEP_NAT_WATCHPOINT_H__ */

/* EOF */
