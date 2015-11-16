/*
 * macosx/macosx-nat-cfm-io.h
 */

#ifndef __GDB_MACOSX_NAT_CFM_IO_H__
#define __GDB_MACOSX_NAT_CFM_IO_H__

#include "symtab.h"

#include "macosx-nat-dyld-info.h"
#include "macosx-nat-dyld-path.h"

void
pef_load_library(const struct dyld_path_info *d,
                 struct dyld_objfile_entry *e);

#endif /* __GDB_MACOSX_NAT_CFM_IO_H__ */

/* EOF */
