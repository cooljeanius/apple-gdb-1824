/* macosx/symread.h -*- C -*- */

#ifndef __GDB_SYMREAD_H__
#define __GDB_SYMREAD_H__ 1

#include "objfiles.h"

extern int sym_parse_type(struct objfile *, struct type **, size_t,
                          unsigned char *, size_t, size_t, size_t *,
                          struct type **, const char **, unsigned long *vptr);

extern void sym_dump_command(const char *, int);

#endif /* !__GDB_SYMREAD_H__ */

/* EOF */
