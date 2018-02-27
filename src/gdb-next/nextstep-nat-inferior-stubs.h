/* nextstep-nat-inferior-stubs.h -*- C -*- */
#ifndef NEXTSTEP_NAT_INFERIOR_STUBS_H
#define NEXTSTEP_NAT_INFERIOR_STUBS_H 1

#include "defs.h"
#include "target.h"

extern int attach(int) ATTRIBUTE_NORETURN;
extern void detach(int) ATTRIBUTE_NORETURN;
extern void kill_inferior(void) ATTRIBUTE_NORETURN;
extern void child_resume(int, int, enum target_signal) ATTRIBUTE_NORETURN;

#endif /* !NEXTSTEP_NAT_INFERIOR_STUBS_H */

/* EOF */
