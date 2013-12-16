/* xm-vaxult.h
 * Definitions to make GDB run on a vax under Ultrix.
 */

#include "vax/xm-vax.h"
/* This is required for Ultrix 3.1b, not for later versions. Ultrix
 * 3.1b cannot just use xm-vaxult2.h because Ultrix 3.1b does define
 * FD_SET. Sure, we could have separate configurations for vaxult2,
 * vaxult3, and vaxult, but why bother? Defining the ptrace constants
 * in infptrace.c is NOT going to do any harm; it is not like they are
 * going to change.  */
#define NO_PTRACE_H

/* EOF */
