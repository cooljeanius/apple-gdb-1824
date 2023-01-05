/* gdbserver/inferiors.c
   Inferior process information for the remote server for GDB.
   Copyright 2002, 2005
   Free Software Foundation, Inc.

   Contributed by MontaVista Software.

   This file is part of GDB.

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

#ifndef __STDC_WANT_LIB_EXT1__
# define __STDC_WANT_LIB_EXT1__ 1
#endif
#include <stdlib.h>

#include "config.h"
#include "server.h"
#include "dll.h"

#ifndef INFERIORS_H
# include "inferiors.h"
#endif /* !INFERIORS_H */

#ifndef __has_include
# define __has_include(x) 0
#endif /* !__has_include */

#if defined(HAVE_STRING_H) || __has_include(<string.h>)
# include <string.h>
#else
# if (defined(__GNUC__) && !defined(__STRICT_ANSI__)) || defined(__clang__)
 #  warning "inferiors.c expects <string.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_STRING_H */

#ifdef HAVE_STRINGS_H
# include <strings.h>
#endif /* HAVE_STRINGS_H */

/* my "gdbthread.h" is currently broken, so copy some parts of it here: */
#ifndef GDB_THREAD_H
# define GDB_THREAD_H 1
/* struct declarations: */
struct btrace_target_info;
extern struct inferior_list all_threads;
/* prototypes: */
struct thread_info *find_thread_ptid(ptid_t ptid);
struct thread_info *gdb_id_to_thread(unsigned int);
#endif /* GDB_THREAD_H */

struct thread_info
{
  struct inferior_list_entry entry;
  void *target_data;
  void *regcache_data;
  unsigned int gdb_id;
};

struct inferior_list all_processes;
struct inferior_list all_threads;

struct thread_info *current_inferior;

#define get_thread(inf) ((struct thread_info *)(inf))

void add_inferior_to_list(struct inferior_list *list,
			  struct inferior_list_entry *new_inferior)
{
  new_inferior->next = NULL;
  if (list->tail != NULL) {
      list->tail->next = new_inferior;
  } else {
      list->head = new_inferior;
  }
  list->tail = new_inferior;
}

/* Invoke ACTION for each inferior in LIST: */
void for_each_inferior(struct inferior_list *list,
		       void (*action)(struct inferior_list_entry *))
{
  struct inferior_list_entry *cur = list->head, *next;

  while (cur != NULL) {
      next = cur->next;
      (*action)(cur);
      cur = next;
  }
}

/* Invoke ACTION for each inferior in LIST, passing DATA to ACTION: */
void
for_each_inferior_data(struct inferior_list *list, void *data,
		       void (*action)(struct inferior_list_entry *, void *))
{
  struct inferior_list_entry *cur = list->head, *next;

  while (cur != NULL) {
      next = cur->next;
      (*action)(cur, data);
      cur = next;
  }
}

void change_inferior_id(struct inferior_list *list, unsigned long new_id)
{
  if (list->head != list->tail) {
      error("tried to change thread ID after multiple threads are created");
  }

  list->head->id = new_id;
}

void remove_inferior(struct inferior_list *list,
		     struct inferior_list_entry *entry)
{
  struct inferior_list_entry **cur;

  if (list->head == entry) {
      list->head = entry->next;
      if (list->tail == entry) {
	  list->tail = list->head;
      }
      return;
  }

  cur = &list->head;
  while (*cur && ((*cur)->next != entry)) {
      cur = &(*cur)->next;
  }

  if (*cur == NULL) {
      return;
  }

  (*cur)->next = entry->next;

  if (list->tail == entry) {
      list->tail = *cur;
  }
}

void
add_thread(unsigned long thread_id, void *target_data, unsigned int gdb_id)
{
  struct thread_info *new_thread
    = (struct thread_info *)malloc(sizeof(*new_thread));

#ifdef HAVE_BZERO
  bzero(new_thread, sizeof(*new_thread));
#else
  memset(new_thread, 0, sizeof(*new_thread));
#endif

  new_thread->entry.id = thread_id;

  add_inferior_to_list(&all_threads, & new_thread->entry);

  if (current_inferior == NULL) {
      current_inferior = new_thread;
  }

  new_thread->target_data = target_data;
  set_inferior_regcache_data(new_thread, new_register_cache());
  new_thread->gdb_id = gdb_id;
}

unsigned int thread_id_to_gdb_id(unsigned long thread_id)
{
  struct inferior_list_entry *inf = all_threads.head;

  while (inf != NULL) {
      struct thread_info *thread = get_thread(inf);
      if (inf->id == thread_id) {
	  return thread->gdb_id;
      }
      inf = inf->next;
  }

  return 0;
}

unsigned int thread_to_gdb_id(struct thread_info *thread)
{
  return thread->gdb_id;
}

unsigned long gdb_id_to_thread_id(unsigned int gdb_id)
{
  struct inferior_list_entry *inf = all_threads.head;

  while (inf != NULL) {
      struct thread_info *thread = get_thread(inf);
      if (thread->gdb_id == gdb_id) {
	  return inf->id;
      }
      inf = inf->next;
  }

  return 0;
}

static void free_one_thread(struct inferior_list_entry *inf)
{
  struct thread_info *thread = get_thread(inf);
  free_register_cache(inferior_regcache_data(thread));
  free(thread);
}

void remove_thread(struct thread_info *thread)
{
  remove_inferior(&all_threads, (struct inferior_list_entry *)thread);
  free_one_thread(&thread->entry);
}

void clear_inferiors(void)
{
  for_each_inferior(&all_threads, free_one_thread);

  all_threads.head = all_threads.tail = NULL;
}

/* Return a pointer to the first inferior in LIST, or NULL if there is NOT one.
 * This is for cases where the caller needs a thread, but does NOT care
 * which one: */
struct inferior_list_entry *get_first_inferior(struct inferior_list *list)
{
  if (list->head != NULL) {
      return list->head;
  }
  return NULL;
}

/* Find the first inferior_list_entry E in LIST for which FUNC (E, ARG)
 * returns non-zero. If no entry is found, then return NULL. */
struct inferior_list_entry *
find_inferior(struct inferior_list *list,
	      int (*func)(struct inferior_list_entry *, void *), void *arg)
{
  struct inferior_list_entry *inf = list->head;

  while (inf != NULL) {
      if ((*func)(inf, arg)) {
	  return inf;
      }
      inf = inf->next;
  }

  return NULL;
}

struct inferior_list_entry *
find_inferior_id(struct inferior_list *list, unsigned long id)
{
  struct inferior_list_entry *inf = list->head;

  while (inf != NULL) {
      if (inf->id == id) {
	  return inf;
      }
      inf = inf->next;
  }

  return NULL;
}

void *inferior_target_data(struct thread_info *inferior)
{
  return inferior->target_data;
}

void set_inferior_target_data(struct thread_info *inferior, void *data)
{
  inferior->target_data = data;
}

void *inferior_regcache_data(struct thread_info *inferior)
{
  return inferior->regcache_data;
}

void set_inferior_regcache_data(struct thread_info *inferior, void *data)
{
  inferior->regcache_data = data;
}

/* new since previous version: */

/* Return true if LIST has exactly one entry: */
int one_inferior_p(struct inferior_list *list)
{
  return ((list->head != NULL) && (list->head == list->tail));
}

/* Reset head/tail of LIST, assuming all entries have already been freed: */
void clear_inferior_list(struct inferior_list *list)
{
  list->head = NULL;
  list->tail = NULL;
}

void new_way_to_clear_inferiors(void)
{
  for_each_inferior(&all_threads, free_one_thread);
  clear_inferior_list(&all_threads);

  clear_dlls();

  current_inferior = NULL;
}

struct process_info *add_process(int pid, int attached)
{
  struct process_info *process;
  ptid_t temp_ptid;

  process = (struct process_info *)xcalloc(1, sizeof(*process));

#ifdef _INFERIOR_LIST_ENTRY_ID_IS_UNSIGNED_LONG
  /* as in dll.c, need to choose a member... a little trickier here though,
   * so use a temp variable: */
  temp_ptid = pid_to_ptid(pid);
  /* use same arbitrarily-chosen member as in dll.c for consistency: */
  process->entry.id = (unsigned long)temp_ptid.lwp;
#else
  process->entry.id = pid_to_ptid(pid);
#endif /* _INFERIOR_LIST_ENTRY_ID_IS_UNSIGNED_LONG */
  process->attached = attached;

  add_inferior_to_list(&all_processes, &process->entry);

  return process;
}

#include "remote-utils.h"

/* Remove a process from the common process list & free the memory allocated
 * for it. The caller is responsible for freeing private data first. */
void remove_process(struct process_info *process)
{
  clear_symbol_cache(&process->symbol_cache);
  /* not sure where this comes from: */
#ifdef free_all_breakpoints
  free_all_breakpoints(process);
#endif /* free_all_breakpoints */
  remove_inferior(&all_processes, &process->entry);
  free(process);
}

struct process_info *find_process_pid(int pid)
{
#ifdef _INFERIOR_LIST_ENTRY_ID_IS_UNSIGNED_LONG
  /* as above, old way is a little trickier: */
  ptid_t temp_ptid;
  temp_ptid = pid_to_ptid(pid);
  /* use same arbitrarily-chosen member as before for consistency: */
  return (struct process_info *)find_inferior_id(&all_processes,
						 (unsigned long)temp_ptid.lwp);
#else
  return (struct process_info *)find_inferior_id(&all_processes,
						 pid_to_ptid(pid));
#endif /* _INFERIOR_LIST_ENTRY_ID_IS_UNSIGNED_LONG */
}

/* Return non-zero if INF, a struct process_info, was started by us,
 * i.e. not attached to.  */
static int
started_inferior_callback(struct inferior_list_entry *entry, void *args)
{
  struct process_info *process = (struct process_info *)entry;

  return ! process->attached;
}

/* Return non-zero if there are any inferiors that we have created
 * (as opposed to attached-to): */
int have_started_inferiors_p(void)
{
  return (find_inferior(&all_processes, started_inferior_callback, NULL)
	  != NULL);
}

/* Return non-zero if INF, a struct process_info, was attached to: */
static int
attached_inferior_callback(struct inferior_list_entry *entry, void *args)
{
  struct process_info *process = (struct process_info *) entry;

  return process->attached;
}

/* Return non-zero if there are any inferiors that we have attached to: */
int have_attached_inferiors_p(void)
{
  return (find_inferior(&all_processes, attached_inferior_callback, NULL)
	  != NULL);
}

struct process_info *get_thread_process(struct thread_info *thread)
{
  int pid;
#if 0
  /* FIXME: this is another case of "unsigned long has become ptid_t",
   * but with a different structure member this time, so it will need
   * a different ifdef and stuff... */
  pid = ptid_get_pid(thread->entry.id);
#else
  /* in the meantime just do this: */
  pid = 0;
  /* (will probably always correspond to the 'kernel_task' on darwin/OSX,
   * which will most likely usually be wrong, but whatever...) */
#endif /* 0 */
  return find_process_pid(pid);
}

struct process_info *current_process(void)
{
  if (current_inferior == NULL) {
      fatal("Current inferior requested, but current_inferior is NULL\n");
  }

  return get_thread_process(current_inferior);
}

/* Silence '-Wunused-macros' warnings: */
#ifdef GDB_THREAD_H
# undef GDB_THREAD_H
#endif /* GDB_THREAD_H */

/* EOF */
