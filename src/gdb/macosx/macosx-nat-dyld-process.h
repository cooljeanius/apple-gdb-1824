/*
 * macosx/macosx-nat-dyld-process.h
 */

#ifndef __GDB_MACOSX_NAT_DYLD_PROCESS_H__
#define __GDB_MACOSX_NAT_DYLD_PROCESS_H__

#include "defs.h"

struct macosx_inferior_status;
struct macosx_dyld_thread_status;

struct dyld_objfile_info;
struct dyld_path_info;
struct dyld_objfile_entry;

struct dyld_event;

struct target_ops;

void dyld_purge_objfiles(struct dyld_objfile_info *info);

struct dyld_objfile_entry *dyld_lookup_objfile_entry(struct dyld_objfile_info *,
						     struct objfile *)
  ATTRIBUTE_W_U_R;

enum dyld_reload_result
{
  DYLD_NO_CHANGE = 0,
  DYLD_UPGRADE,
  DYLD_DOWNGRADE
};

enum dyld_reload_result dyld_should_reload_objfile_for_flags(struct dyld_objfile_entry *e)
  ATTRIBUTE_W_U_R;

void dyld_add_inserted_libraries(struct dyld_objfile_info *info,
                                 const struct dyld_path_info *d);

void dyld_add_image_libraries(struct dyld_objfile_info *info, bfd *abfd);

void dyld_resolve_shlibs_internal(const struct macosx_inferior_status *s,
                                  struct dyld_objfile_info *);

void dyld_resolve_shlibs_dyld(const struct macosx_inferior_status *s,
                              struct dyld_objfile_info *);

void dyld_load_library(const struct dyld_path_info *d,
                       struct dyld_objfile_entry *e);

void dyld_load_libraries(const struct dyld_path_info *d,
                         struct dyld_objfile_info *result);

void dyld_load_library_from_file(const struct dyld_path_info *d,
                                 struct dyld_objfile_entry *e,
                                 int print_errors);

void dyld_load_library_from_memory(const struct dyld_path_info *d,
                                   struct dyld_objfile_entry *e,
                                   int print_errors);

void dyld_symfile_loaded_hook(struct objfile *o);

void dyld_merge_libraries(struct dyld_objfile_info *,
                          struct dyld_objfile_info *,
                          struct dyld_objfile_info *);

void dyld_prune_shlib(struct dyld_path_info *d,
		      struct dyld_objfile_info *,
                      struct dyld_objfile_entry *n);

void dyld_merge_shlibs(const struct macosx_dyld_thread_status *s,
                       struct dyld_path_info *d,
                       struct dyld_objfile_info *,
                       struct dyld_objfile_info *);

void remove_objfile_from_dyld_records(struct objfile *);

int dyld_is_objfile_loaded(struct objfile *obj)
  ATTRIBUTE_W_U_R;

void dyld_remove_objfile_internal(struct dyld_objfile_entry *e,
                                  int delete_p);

void dyld_remove_objfile(struct dyld_objfile_entry *);

void dyld_clear_objfile(struct dyld_objfile_entry *);

void dyld_remove_objfiles(const struct dyld_path_info *d,
                          struct dyld_objfile_info *result);

void dyld_remove_duplicates(struct dyld_path_info *d,
                            struct dyld_objfile_info *result);

void dyld_objfile_move_load_data(struct dyld_objfile_entry *src,
                                 struct dyld_objfile_entry *dst);

void dyld_check_discarded(struct dyld_objfile_info *info);

void dyld_purge_cached_libraries(struct dyld_objfile_info *info);

void dyld_update_shlibs(struct dyld_path_info *,
                        struct dyld_objfile_info *);

void dyld_merge_shlib(const struct macosx_dyld_thread_status *,
                      struct dyld_path_info *,
                      struct dyld_objfile_info *,
                      struct dyld_objfile_entry *);

int dyld_libraries_compatible(struct dyld_path_info *d,
                              struct dyld_objfile_entry *f,
                              struct dyld_objfile_entry *l)
  ATTRIBUTE_W_U_R;

struct objfile *dyld_lookup_objfile_safe(struct dyld_objfile_entry *e)
  ATTRIBUTE_W_U_R;

int dyld_objfile_allocated(struct objfile *o)
  ATTRIBUTE_W_U_R;

int dyld_parse_load_level(const char *s)
  ATTRIBUTE_W_U_R;

int dyld_resolve_load_flag(const struct dyld_path_info *d,
                           struct dyld_objfile_entry *e,
                           const char *rules)
  ATTRIBUTE_W_U_R;

int dyld_minimal_load_flag(const struct dyld_path_info *d,
                           struct dyld_objfile_entry *e)
  ATTRIBUTE_W_U_R;

int dyld_default_load_flag(const struct dyld_path_info *d,
                           struct dyld_objfile_entry *e)
  ATTRIBUTE_W_U_R;

void dyld_resolve_filename_image(const struct macosx_dyld_thread_status *s,
                                 struct dyld_objfile_entry *e);

char *dyld_find_dylib_name(CORE_ADDR addr, int cpu_type, int ncmds)
  ATTRIBUTE_W_U_R;

void dyld_resolve_filenames(const struct macosx_dyld_thread_status *s,
                            struct dyld_objfile_info *newinfo);

void dyld_load_symfile(struct dyld_objfile_entry *e);

void dyld_load_symfile_preserving_objfile(struct dyld_objfile_entry *e);

void dyld_load_symfiles(struct dyld_objfile_info *result);

struct pre_run_memory_map *create_pre_run_memory_map(struct bfd *abfd)
  ATTRIBUTE_W_U_R;

void free_pre_run_memory_map(struct pre_run_memory_map *map);

#ifndef DYLD_TRACE_NUM_EXTERNALLY_DECLARED
# define DYLD_TRACE_NUM_EXTERNALLY_DECLARED 1
extern int dyld_trace_num;
#endif /* !DYLD_TRACE_NUM_EXTERNALLY_DECLARED */

#endif /* __GDB_MACOSX_NAT_DYLD_PROCESS_H__ */

/* EOF */
