/*
 * unistd.c
 */

#include <config.h>
#if !defined(_GL_MATH_INLINE) && defined(_GL_EXTERN_INLINE)
# define _GL_MATH_INLINE _GL_EXTERN_INLINE
#endif /* !_GL_MATH_INLINE && _GL_EXTERN_INLINE */
#include "unistd.h"

/* prototype: */
extern int unistd_dummy_func(void);

/* function: */
int unistd_dummy_func() {
	return 0;
}

/* EOF */
