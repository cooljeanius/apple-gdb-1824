/* Imitation of sys/types.h. */

#ifndef __SYS_TYPES_H__
#define __SYS_TYPES_H__

#include <Types.h>

#ifndef _DEV_T
typedef short dev_t;
# define _DEV_T 1
#endif /* !_DEV_T */
#ifndef	_INO_T
typedef short ino_t;
# define _INO_T 1
#endif /* !_INO_T */
#ifndef	_MODE_T
typedef unsigned short mode_t;
# define _MODE_T 1
#endif /* !_MODE_T */
#ifndef _UID_T
typedef unsigned short uid_t;
# define _UID_T 1
#endif /* !_UID_T */
#ifndef _GID_T
typedef unsigned short gid_t;
# define _GID_T 1
#endif /* !_GID_T */
#ifndef _OFF_T
typedef long off_t;
# define _OFF_T 1
#endif /* !_OFF_T */

#endif /* __SYS_TYPES_H__ */

/* EOF */
