/* xm-vaxbsd.h
 * Definitions to make GDB run on a vax under BSD, 4.3 or 4.4.
 */

/* This should exist on either 4.3 or 4.4.  4.3 does NOT have limits.h
 * or machine/limits.h.  */
#include <sys/param.h>

#include "vax/xm-vax.h"

/* EOF */
