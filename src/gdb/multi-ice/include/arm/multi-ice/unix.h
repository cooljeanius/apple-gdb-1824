/* unix.h */
/* based on the windows equivalent, i.e. "windows.h" */

#ifndef _ARM_MULTI_ICE_UNIX_H
#define _ARM_MULTI_ICE_UNIX_H 1

#if !defined(_host_LOADED) && !defined(_ARM_RDI_SHARE_HOST_H)
# include "host.h"
#endif /* !_host_LOADED && !_ARM_RDI_SHARE_HOST_H */

#if defined(HAVE_STDBOOL_H)
# include <stdbool.h>
#endif /* HAVE_STDBOOL_H */
#if defined(HAVE_SYS_TYPES_H)
# include <sys/types.h>
#endif /* HAVE_SYS_TYPES_H */
#if defined(HAVE_UNISTD_H)
# include <unistd.h>
#endif /* HAVE_UNISTD_H */

#endif /* !_ARM_MULTI_ICE_UNIX_H */

/* EOF */
