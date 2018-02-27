/* -*- C -*-
 * nextstep-nat-dyld-section.c
 */

#include "nextstep-nat-dyld-process.h"

#include "nextstep-nat-dyld-info.h"
#include "nextstep-nat-dyld-path.h"
#include "nextstep-nat-inferior.h"
#include "nextstep-nat-mutils.h"

#include "defs.h"
#include "inferior.h"
#include "symfile.h"
#include "symtab.h"
#include "gdbcmd.h"
#include "objfiles.h"

#include <mach-o/nlist.h>
#include <mach-o/loader.h>
#ifdef HAVE_MACH_O_DYLD_DEBUG_H
# include <mach-o/dyld_debug.h>
#else
# ifdef HAVE_MACH_O_DYLD_H
#  include <mach-o/dyld.h>
# endif /* HAVE_MACH_O_DYLD_H */
# ifdef HAVE_MACH_O_DYLD_IMAGES_H
#  include <mach-o/dyld_images.h>
# endif /* HAVE_MACH_O_DYLD_IMAGES_H */
# ifdef HAVE_DLFCN_H
#  include <dlfcn.h>
# endif /* HAVE_DLFCN_H */
#endif /* HAVE_MACH_O_DYLD_DEBUG_H */

#include <string.h>

#include "mach-o.h"

#include "nextstep-nat-dyld-section.h"

extern bfd *exec_bfd;

void dyld_update_section_tables(struct dyld_objfile_info *result,
				struct target_ops *target)
{
  update_section_tables(result, target);
}

void dyld_merge_section_tables(struct dyld_objfile_info *result)
{
  (void)result;
}

/* EOF */
