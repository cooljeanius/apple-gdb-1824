/* rlcompat.h -- backwards compatibility functions for readline. */

#if !defined(_RLCOMPAT_H_)
#define _RLCOMPAT_H_

#include "rlstdc.h" /* for PARAMS */
#include "rltypedefs.h" /* for rl_compentry_func_t */

extern void free_undo_list PARAMS((void));
extern int maybe_replace_line PARAMS((void));
extern int maybe_save_line PARAMS((void));
extern int maybe_unsave_line PARAMS((void));
extern int ding PARAMS((void));
extern int crlf PARAMS((void));
extern int alphabetic PARAMS((int));
extern char **completion_matches PARAMS((const char *, rl_compentry_func_t *));
extern char *username_completion_function PARAMS((const char *, int));
extern char *filename_completion_function PARAMS((const char *, int));

#endif /* !_RLCOMPAT_H_ */

/* EOF */
