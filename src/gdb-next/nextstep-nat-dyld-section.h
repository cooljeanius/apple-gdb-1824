/*
 *  nextstep-nat-dyld-section.h
 *  gdb
 *
 *  Created by Eric Gallager on 2/26/18.
 *
 */

#ifndef NEXTSTEP_NAT_DYLD_SECTION_H
#define NEXTSTEP_NAT_DYLD_SECTION_H 1

#include "nextstep-nat-dyld-info.h"

struct target_ops;

extern void dyld_update_section_tables(struct dyld_objfile_info *,
				       struct target_ops *);
void dyld_merge_section_tables(struct dyld_objfile_info *);

#endif /* !NEXTSTEP_NAT_DYLD_SECTION_H */

/* EOF */
