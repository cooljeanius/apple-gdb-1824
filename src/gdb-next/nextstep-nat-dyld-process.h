/*
 * nextstep-nat-dyld-process.h
 */

#ifndef _NEXTSTEP_NAT_DYLD_PROCESS_H_
#define _NEXTSTEP_NAT_DYLD_PROCESS_H_

#include "defs.h"

struct next_inferior_status;
struct next_dyld_thread_status;

struct dyld_objfile_info;
struct dyld_path_info;

struct dyld_event;

struct target_ops;

void dyld_add_inserted_libraries
PARAMS ((struct dyld_objfile_info *info, const struct dyld_path_info *d));

void dyld_add_image_libraries
PARAMS ((struct dyld_objfile_info *info, bfd *abfd));

void dyld_resolve_shlibs_internal
PARAMS((const struct next_inferior_status *s, struct dyld_objfile_info *));

void dyld_resolve_shlibs_dyld
PARAMS((const struct next_inferior_status *s, struct dyld_objfile_info *));

void dyld_load_libraries
PARAMS ((const struct dyld_path_info *d, struct dyld_objfile_info *result));

void dyld_merge_libraries
PARAMS((struct dyld_objfile_info *old,
	struct dyld_objfile_info *newone,
	struct dyld_objfile_info *result));

void dyld_remove_objfiles
PARAMS ((const struct dyld_path_info *d, struct dyld_objfile_info *result));

void dyld_remove_duplicates
PARAMS ((struct dyld_path_info *d, struct dyld_objfile_info *result));

void dyld_check_discarded
PARAMS ((struct dyld_objfile_info *info));

void dyld_purge_cached_libraries
PARAMS ((struct dyld_objfile_info *info));

void dyld_update_shlibs
PARAMS((const struct next_dyld_thread_status *s,
	struct dyld_path_info *d,
	struct dyld_objfile_info *old,
	struct dyld_objfile_info *newone,
	struct dyld_objfile_info *result));

extern void dyld_resolve_filename_image(const struct next_dyld_thread_status *,
					struct dyld_objfile_entry *);
extern void dyld_resolve_filenames(const struct next_dyld_thread_status *,
				   struct dyld_objfile_info *);
extern unsigned int dyld_parse_load_level(const char *);
extern int dyld_resolve_load_flag(const struct dyld_path_info *,
				  struct dyld_objfile_entry *, const char *);
extern int dyld_minimal_load_flag(const struct dyld_path_info *,
				  struct dyld_objfile_entry *);
extern int dyld_default_load_flag(const struct dyld_path_info *,
				  struct dyld_objfile_entry *);
extern void dyld_load_library(const struct dyld_path_info *,
			      struct dyld_objfile_entry *);
extern void dyld_load_symfile(struct dyld_objfile_entry *);
extern void dyld_load_symfiles(struct dyld_objfile_info *);
extern void dyld_remove_objfile(struct dyld_objfile_entry *);
extern void dyld_objfile_move_load_data(struct dyld_objfile_entry *,
					struct dyld_objfile_entry *);
extern void dyld_merge_shlibs(const struct next_dyld_thread_status *,
			      struct dyld_path_info *,
			      struct dyld_objfile_info *,
			      struct dyld_objfile_info *,
			      struct dyld_objfile_info *);
extern void dyld_process_image_event(struct dyld_objfile_info *,
				     const struct dyld_event *);

#endif /* _NEXTSTEP_NAT_DYLD_PROCESS_H_ */

/* EOF */
