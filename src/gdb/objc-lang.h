/* Objective-C language support definitions for GDB, the GNU debugger.

   Copyright 1992, 2005 Free Software Foundation, Inc.

   Contributed by Apple Computer, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

#if !defined(OBJC_LANG_H)
#define OBJC_LANG_H 1

#if defined(HAVE_AVAILABILITY_H) || (defined(__APPLE__) && defined(NM_NEXTSTEP))
# include <Availability.h>
#else
# ifndef __OSX_AVAILABLE_BUT_DEPRECATED
#  define __OSX_AVAILABLE_BUT_DEPRECATED(_macIntro, _macDep, _iphoneIntro, _iphoneDep) /* nothing */
# endif /* !__OSX_AVAILABLE_BUT_DEPRECATED */
# ifndef __MAC_10_0
#  define __MAC_10_0      1000
# endif /* !__MAC_10_0 */
# ifndef __MAC_10_6
#  define __MAC_10_6      1060
# endif /* !__MAC_10_6 */
# ifndef __MAC_NA
#  define __MAC_NA        9999   /* not available */
# endif /* !__MAC_NA */
# ifndef __IPHONE_NA
#  define __IPHONE_NA     99999  /* not available */
# endif /* !__IPHONE_NA */
#endif /* HAVE_AVAILABILITY_H || (__APPLE__ && NM_NEXTSTEP) */

struct stoken;

struct value;
struct block;

extern int objc_parse(void);	/* Defined in c-exp.y */

extern void objc_error(char *);	/* Defined in c-exp.y */

extern CORE_ADDR lookup_objc_class(char *);
extern CORE_ADDR lookup_child_selector(char *);
extern CORE_ADDR lookup_child_selector_nocache(char *);

extern char *objc_demangle(const char *, int);
extern char *objcplus_demangle(const char *, int);

extern int find_objc_msgcall(CORE_ADDR, CORE_ADDR *);

void tell_objc_msgsend_cacher_objfile_changed(struct objfile *);

void objc_clear_caches(void);

CORE_ADDR find_implementation(CORE_ADDR, CORE_ADDR, int);

extern char *parse_selector(char *, char **);

extern char *parse_method(char *, char *, char **, char **, char **);

extern char *find_imps(struct symtab *, struct block *, char *,
                       struct symbol **, unsigned int *, unsigned int *);

extern struct value *value_nsstring(char *, int);

/* APPLE LOCAL: Need this bit to decorate the gc-roots command output: */
extern struct type *objc_target_type_from_object(CORE_ADDR, struct block *,
                                                 int, char **);

extern struct type *value_objc_target_type(struct value *, struct block *,
                                           char **);
int should_lookup_objc_class(void);

/* for parsing Objective C */
extern void start_msglist(void);
extern void add_msglist(struct stoken *str, int addcolon);
extern int end_msglist(void);

struct symbol *lookup_struct_typedef(char *name, struct block *block,
				     int noerr);

/* APPLE LOCAL: We need to control this down in macosx/ when debugging
   translated programs.  */
extern int lookup_objc_class_p;

extern int objc_handle_update(CORE_ADDR stop_addr);
void objc_init_trampoline_observer(void);
void objc_init_runtime_version(void);
int pc_in_objc_trampoline_p(CORE_ADDR pc, uint32_t *flags);

/* comment in source file says to use debugger "safe mode" instead: */
int objc_runtime_lock_taken_p(void) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0,__MAC_10_6,__IPHONE_NA,__IPHONE_NA);

void objc_invalidate_objc_class(struct type *type);

/* APPLE LOCAL Disable breakpoints while updating data formatters: */
extern int is_objc_exception_throw_breakpoint(struct breakpoint *);

enum objc_debugger_mode_result
  {
    objc_debugger_mode_unknown,
    objc_debugger_mode_success,
    objc_debugger_mode_fail_unable_to_enter_debug_mode,
    objc_debugger_mode_fail_spinlock_held,
    objc_debugger_mode_fail_malloc_lock_held,
    objc_debugger_mode_fail_objc_api_unavailable
  };

enum objc_debugger_mode_result make_cleanup_set_restore_debugger_mode(struct cleanup **cleanup, int level);

enum objc_handcall_fail_reasons
  {
    objc_no_fail,
    objc_exception_thrown,
    objc_debugger_mode_fail
  };

enum objc_handcall_fail_reasons objc_pc_at_fail_point(CORE_ADDR pc);
struct cleanup *make_cleanup_init_objc_exception_catcher(void);
void reinitialize_objc(void);
int objc_runtime_check_enabled_p(void);

/* Are we using the ObjC 2.0 runtime?  */
int new_objc_runtime_internals(void);

/* Goes with the following: */
int objc_check_safe_to_run_all_threads(void);

/* This it the ObjC "make printing safe for ObjC" method: */
int objc_setup_safe_print(struct cleanup **);
#endif /* !OBJC_LANG_H */

/* EOF */
