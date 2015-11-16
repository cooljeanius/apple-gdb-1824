/* gdbserver/inferiors.h
   Inferior process information for the remote server for GDB.
   Copyright (C) 1993-2013 Free Software Foundation, Inc.

   This file is part of GDB.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#ifndef INFERIORS_H
#define INFERIORS_H

#ifndef _STRUCT_INFERIOR_LIST
# define _STRUCT_INFERIOR_LIST 1
/* Generic information for tracking a list of ``inferiors'' - threads,
 * processes, etc. */
struct inferior_list
{
  struct inferior_list_entry *head;
  struct inferior_list_entry *tail;
};
#endif /* !_STRUCT_INFERIOR_LIST*/
#ifndef _STRUCT_INFERIOR_LIST_ENTRY
# define _STRUCT_INFERIOR_LIST_ENTRY 1
struct inferior_list_entry
{
  /* "server.h" has this as "unsigned long": */
  ptid_t id;
# ifndef _INFERIOR_LIST_ENTRY_ID_IS_PTID_T
#  define _INFERIOR_LIST_ENTRY_ID_IS_PTID_T 1
# endif /* !_INFERIOR_LIST_ENTRY_ID_IS_PTID_T */
  struct inferior_list_entry *next;
};
#endif /* !_STRUCT_INFERIOR_LIST_ENTRY */

struct thread_info;
struct target_desc;
struct sym_cache;
struct breakpoint;
struct raw_breakpoint;
struct fast_tracepoint_jump;
struct process_info_private;

#ifndef _STRUCT_PROCESS_INFO
# define _STRUCT_PROCESS_INFO 1
struct process_info
{
  /* This must appear first.
   * The list iterator functions assume it.  */
  struct inferior_list_entry entry;

  /* might have been the old name for the previous, idk... */
  struct inferior_list_entry head;

  /* Nonzero if this child process was attached rather than spawned: */
  int attached;

  /* True if GDB asked us to detach from this process, but we remained
   * attached anyway: */
  int gdb_detached;

  /* The symbol cache: */
  struct sym_cache *symbol_cache;

  /* The list of memory breakpoints: */
  struct breakpoint *breakpoints;

  /* The list of raw memory breakpoints: */
  struct raw_breakpoint *raw_breakpoints;

  /* The list of installed fast tracepoints: */
  struct fast_tracepoint_jump *fast_tracepoint_jumps;

  const struct target_desc *tdesc;

  /* Private target data: */
  struct process_info_private *private;
};
#endif /* !_STRUCT_PROCESS_INFO */

#ifndef ptid_of
# define ptid_of(inf) ((inf)->entry.id)
#endif /* !ptid_of */
#ifndef pid_of
# define pid_of(inf) ptid_get_pid((inf)->entry.id)
#endif /* !pid_of */
#ifndef lwpid_of
# define lwpid_of(inf) ptid_get_lwp((inf)->entry.id)
#endif /* !lwpid_of */

/* Return a pointer to the process that corresponds to the current thread
 * (current_inferior). It is an error to call this if there is no current
 * thread selected: */
struct process_info *current_process(void);
struct process_info *get_thread_process(struct thread_info *);

extern struct inferior_list all_processes;

void add_inferior_to_list(struct inferior_list *list,
						  struct inferior_list_entry *new_inferior);
void for_each_inferior(struct inferior_list *list,
					   void (*action)(struct inferior_list_entry *));
void for_each_inferior_with_data
 (struct inferior_list *list,
  void (*action)(struct inferior_list_entry *, void *),
  void *data);

void clear_inferior_list(struct inferior_list *list);

int one_inferior_p(struct inferior_list *list);

#ifndef A_I_NEXT
/* Helper for ALL_INFERIORS_TYPE. Gets the next element starting at
 * CUR, if CUR is not NULL.  */
# define A_I_NEXT(type, list, cur)						\
   ((cur) != NULL										\
   ? (type *)((struct inferior_list_entry *)cur)->next	\
   : NULL)
#endif /* !A_I_NEXT */

#if !defined(ALL_INFERIORS_TYPE) && defined(A_I_NEXT)
/* Iterate over all inferiors of type TYPE in LIST, open loop style: */
# define ALL_INFERIORS_TYPE(type, list, cur, tmp)				\
  for ((cur) = (type *)(list)->head, (tmp) = A_I_NEXT(type, list, cur); \
	   (cur) != NULL;							\
       (cur) = (tmp), (tmp) = A_I_NEXT(type, list, cur))
#endif /* !ALL_INFERIORS_TYPE && A_I_NEXT */

#if !defined(ALL_INFERIORS) && defined(ALL_INFERIORS_TYPE)
/* Iterate over all inferiors in LIST, open loop style: */
# define ALL_INFERIORS(list, cur, tmp)				\
  ALL_INFERIORS_TYPE(struct inferior_list_entry, list, cur, tmp)
#endif /* !ALL_INFERIORS && ALL_INFERIORS_TYPE */

#if !defined(ALL_PROCESSES) && defined(ALL_INFERIORS_TYPE)
/* Iterate over all processes, open loop style: */
# define ALL_PROCESSES(cur, tmp)					\
  ALL_INFERIORS_TYPE(struct process_info, &all_processes, cur, tmp)
#endif /* !ALL_PROCESSES && ALL_INFERIORS_TYPE */


extern struct thread_info *current_inferior;
void remove_inferior(struct inferior_list *list,
					 struct inferior_list_entry *entry);

struct inferior_list_entry *get_first_inferior(struct inferior_list *list);

struct process_info *add_process(int pid, int attached);
void remove_process(struct process_info *process);
struct process_info *find_process_pid(int pid);
int have_started_inferiors_p(void);
int have_attached_inferiors_p(void);

struct inferior_list_entry *
find_inferior(struct inferior_list *,
			  int (*func)(struct inferior_list_entry *, void *),
			  void *arg);
#ifndef _HAVE_ID_CONVERSION_FUNCS
# define _HAVE_ID_CONVERSION_FUNCS 1
struct inferior_list_entry *find_inferior_id(struct inferior_list *list,
											 ptid_t id);
ptid_t thread_id_to_gdb_id(ptid_t);
ptid_t thread_to_gdb_id(struct thread_info *);
ptid_t gdb_id_to_thread_id(ptid_t);
#endif /* !_HAVE_ID_CONVERSION_FUNCS */

void clear_inferiors(void);
void new_way_to_clear_inferiors(void);
void *inferior_target_data(struct thread_info *);
void set_inferior_target_data(struct thread_info *, void *);
void *inferior_regcache_data(struct thread_info *);
void set_inferior_regcache_data(struct thread_info *, void *);

#endif /* INFERIORS_H */

/* EOF */
