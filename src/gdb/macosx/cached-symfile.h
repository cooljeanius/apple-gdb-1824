#ifndef __GDB_MACOSX_CACHED_SYMFILE_H__
#define __GDB_MACOSX_CACHED_SYMFILE_H__

#include "defs.h"

/* Declarations for functions defined in objfiles.c */

extern struct objfile *allocate_objfile (bfd *, int, int, CORE_ADDR);

extern int build_objfile_section_table (struct objfile *);

extern struct objfile *open_objfile_from_mmalloc_pool (char *name, bfd *abfd, PTR md, int fd);

extern struct objfile *create_objfile_from_mmalloc_pool (bfd *abfd, PTR md, int fd, CORE_ADDR mapaddr);

extern struct objfile *open_mapped_objfile (bfd *abfd, CORE_ADDR mapaddr);

struct objfile *create_mapped_objfile (bfd *abfd, CORE_ADDR mapaddr);

struct objfile *create_objfile (bfd *abfd);

#endif /* __GDB_MACOSX_CACHED_SYMFILE_H__ */
