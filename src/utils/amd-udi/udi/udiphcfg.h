/* udiphcfg.h */
/* This file just picks the correct udiphxxx.h depending on the host */
/* The three hosts that are now defined are HPUX and SUN and MSDOS */

#ifndef _UDIPHCFG_H
#define _UDIPHCFG_H

#ifdef __hpux
# include "udiphsun.h"
#endif /* __hpux */

#ifdef sun
# include "udiphsun.h"
#else /* sun */
# ifdef MSDOS
#  include "udiphdos.h"
# endif /* MSDOS */
#endif /* sun */

#endif /* _UDIPHCFG_H */

/* EOF */
