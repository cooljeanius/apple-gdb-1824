/* acconfig_obsolete.h */
/* used to be just acconfig.h, but autoheader considers files like that
 * to be obsolete, so moved aside for now... */

#ifndef __ACCONFIG_OBSOLETE_H
#define __ACCONFIG_OBSOLETE_H 1

/* Define to 1 if NLS is requested: */
#undef ENABLE_NLS

/* Define as 1 if you have catgets and do NOT want to use GNU gettext: */
#undef HAVE_CATGETS

/* Define as 1 if you have gettext and do NOT want to use GNU gettext: */
#undef HAVE_GETTEXT

/* Define as 1 if you have the stpcpy function: */
#undef HAVE_STPCPY

/* Define if your locale.h file contains LC_MESSAGES: */
#undef HAVE_LC_MESSAGES

#ifdef HAVE_CONFIG_H
# ifndef _INCLUDING_CONFIG_H
#  define _INCLUDING_CONFIG_H
#  include "config.h"
# endif /* _INCLUDING_CONFIG_H */
#endif /* HAVE_CONFIG_H */

#endif /* !__ACCONFIG_OBSOLETE_H */

/* EOF */
