/* libiberty.h: Deprecated function declarations for libiberty. */

#ifndef LIBIBERTY_DEPRECATED_H
#define LIBIBERTY_DEPRECATED_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef _ANSIDECL_H
# include "ansidecl.h"
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__) && defined(DEBUG)
 #  warning "we seem to have already included ansidecl.h once"
# endif /* __GNUC__ && !__STRICT_ANSI__ && DEBUG */
#endif /* !_ANSIDECL_H */

/* Get a definition for size_t: */
#include <stddef.h>

extern void bcopy(const void *, void *, size_t) ATTRIBUTE_DEPRECATED;

extern void bzero(void *, size_t) ATTRIBUTE_DEPRECATED_FOR(memset);

extern char *index(const char *, int) ATTRIBUTE_DEPRECATED_FOR(strchr);

extern char *rindex(const char *s, int c) ATTRIBUTE_DEPRECATED_FOR(strrchr);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ! defined(LIBIBERTY_DEPRECATED_H) */

/* EOF */
