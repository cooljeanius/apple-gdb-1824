/*
 * nextstep-nat-dyld.h
 */

#ifndef _NEXTSTEP_NAT_DYLD_H_
#define _NEXTSTEP_NAT_DYLD_H_

#include "defs.h"
#include "nextstep-nat-mutils.h"
#include "nextstep-nat-threads.h"

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

struct objfile;
struct target_waitstatus;

struct next_inferior_status;

struct dyld_objfile_entry;

#include "nextstep-nat-dyld-info.h"
#include "nextstep-nat-dyld-path.h"

enum next_dyld_thread_state
{
  dyld_clear,
  dyld_initialized,
  dyld_started
};
typedef enum next_dyld_thread_state next_dyld_thread_state;

struct next_dyld_thread_status {

  CORE_ADDR object_images;
  CORE_ADDR library_images;
  CORE_ADDR state_changed_hook;

  unsigned int dyld_version;

  unsigned int nobject_images;
  unsigned int nlibrary_images;
  unsigned int object_image_size;
  unsigned int library_image_size;

  enum next_dyld_thread_state state;

  struct dyld_objfile_info current_info;
  struct dyld_path_info path_info;
};
typedef struct next_dyld_thread_status next_dyld_thread_status;

void dyld_debug(const char *fmt, ...) ATTRIBUTE_PRINTF_1;

enum dyld_debug_return;
const char *dyld_debug_error_string (enum dyld_debug_return ret);
void dyld_print_status_info (struct next_dyld_thread_status *s, unsigned int mask);

void next_init_dyld (struct next_dyld_thread_status *s, struct objfile *o);

void next_clear_start_breakpoint(void);
void next_set_start_breakpoint (bfd *exec_bfd);

int next_mach_try_start_dyld(void);
int next_mach_start_dyld (struct next_inferior_status *s);

void next_mach_add_shared_symbol_files(void);

void next_init_dyld_symfile (struct objfile *o);

int next_dyld_update (int dyldonly);

#endif /* _NEXTSTEP_NAT_DYLD_H_ */

/* EOF */
