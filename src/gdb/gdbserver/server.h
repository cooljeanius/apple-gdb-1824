/* gdbserver/server.h
   Common definitions for remote server for GDB.
   Copyright 1993, 1995, 1997, 1998, 1999, 2000, 2002, 2003, 2004, 2005
   Free Software Foundation, Inc.

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

#ifndef SERVER_H
#define SERVER_H

#include "config.h"

#ifdef __MINGW32CE__
# include "wincecompat.h"
#endif /* __MINGW32CE__ */

/* FIXME: for -Wundef; should already be in "config.h": */
#ifndef HAVE_DECL_BASENAME
# define HAVE_DECL_BASENAME 0
#endif /* !HAVE_DECL_BASENAME */
#include "libiberty.h"
#include "ansidecl.h"
#include "version.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef HAVE_STDINT_H
# include <stdint.h>
#else
# ifdef HAVE_INTTYPES_H
#  include <inttypes.h>
# else
#  ifdef HAVE_SYS_TYPES_H
#   include <sys/types.h>
#  endif /* HAVE_SYS_TYPES_H */
# endif /* HAVE_INTTYPES_H */
#endif /* HAVE_STDINT_H */
#ifdef HAVE_ERRNO_H
# include <errno.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
 #  warning "server.h expects <errno.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_ERRNO_H */
#include <setjmp.h>

#ifdef HAVE_STRING_H
# include <string.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
 #  warning "server.h expects <string.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_STRING_H */

#ifdef HAVE_ALLOCA_H
# include <alloca.h>
#endif /* HAVE_ALLOCA_H */
/* On some systems such as MinGW, alloca is declared in malloc.h
 * (there is no alloca.h).  */
#ifdef HAVE_MALLOC_H
# include <malloc.h>
#else
# ifdef HAVE_MALLOC_MALLOC_H
#  include <malloc/malloc.h>
# else
#  if defined(__GNUC__) && !defined(__STRICT_ANSI__)
 #   warning "server.h expects a malloc-related header to be included."
#  endif /* __GNUC__ && !__STRICT_ANSI__ */
# endif /* HAVE_MALLOC_MALLOC_H */
#endif /* HAVE_MALLOC_H */

#if !HAVE_DECL_STRERROR
# ifndef strerror
extern char *strerror(int);	/* X3.159-1989  4.11.6.2 */
# endif /* !strerror */
#endif /* !HAVE_DECL_STRERROR */

#if !HAVE_DECL_PERROR
# ifndef perror
extern void perror(const char *);
# endif /* !perror */
#endif /* !HAVE_DECL_PERROR */

#if !HAVE_DECL_VASPRINTF
# ifndef vasprintf
extern int vasprintf(char **strp, const char *fmt, va_list ap);
# endif /* !vasprintf */
#endif /* !HAVE_DECL_VASPRINTF */
#if !HAVE_DECL_VSNPRINTF
# ifndef vsnprintf
int vsnprintf(char *str, size_t size, const char *format, va_list ap);
# endif /* !vsnprintf */
#endif /* !HAVE_DECL_VSNPRINTF */

#ifdef IN_PROCESS_AGENT
#  define PROG "ipa"
#else
#  define PROG "gdbserver"
#endif /* IN_PROCESS_AGENT */

/* A type used for binary buffers: */
typedef unsigned char gdb_byte;

#include "ptid.h"
#include "buffer.h"
#include "xml-utils.h"

#ifndef ATTR_NORETURN
# if defined(__GNUC__) && ((__GNUC__ > 2) || \
    ((__GNUC__ == 2) && defined(__GNUC_MINOR__) && (__GNUC_MINOR__ >= 7)))
#  define ATTR_NORETURN __attribute__ ((noreturn))
# else
#  define ATTR_NORETURN           /* nothing */
# endif /* GCC 2.7+ */
#endif /* !ATTR_NORETURN */

#ifndef ATTR_FORMAT
# if defined(__GNUC__) && ((__GNUC__ > 2) || \
    ((__GNUC__ == 2) && defined(__GNUC_MINOR__) && (__GNUC_MINOR__ >= 4)))
#  define ATTR_FORMAT(type, x, y) __attribute__ ((format(type, x, y)))
# else
#  define ATTR_FORMAT(type, x, y) /* nothing */
# endif /* GCC 2.4+ */
#endif /* !ATTR_FORMAT */

/* FIXME: This should probably be autoconf'd for. It is an integer type at
   least the size of a (void *).  */
#if defined(HAVE_INTPTR_T)
typedef intptr_t CORE_ADDR;
#else
typedef long long CORE_ADDR;
#endif /* HAVE_INTPTR_T */

typedef long long LONGEST;
typedef unsigned long long ULONGEST;

#include "regcache.h"
#include "gdb/signals.h"
#include "target.h"
#include "mem-break.h"

#ifndef _STRUCT_INFERIOR_LIST
# define _STRUCT_INFERIOR_LIST 1
/* Generic information for tracking a list of ``inferiors'' - threads,
 * processes, etc.  */
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
  /* in newer versions of gdb this changes type to ptid_t, and moves to
   * "inferiors.h": */
  unsigned long id;
# ifndef _INFERIOR_LIST_ENTRY_ID_IS_UNSIGNED_LONG
#  define _INFERIOR_LIST_ENTRY_ID_IS_UNSIGNED_LONG 1
# endif /* !_INFERIOR_LIST_ENTRY_ID_IS_UNSIGNED_LONG */
  struct inferior_list_entry *next;
};
#endif /* !_STRUCT_INFERIOR_LIST_ENTRY */

/* Opaque type for user-visible threads.  */
struct thread_info;

#include "regcache.h"
#include "gdb/signals.h"

#include "target.h"
#include "mem-break.h"

/* Target-specific functions */

void initialize_low(void);

/* From inferiors.c. */

extern struct inferior_list all_threads;
void add_inferior_to_list(struct inferior_list *list,
			  struct inferior_list_entry *new_inferior);
void for_each_inferior(struct inferior_list *list,
		       void (*action)(struct inferior_list_entry *));
/* APPLE LOCAL: Need a version of this that takes a client data.
 * It is really annoying to have to pass everything through globals... */
void for_each_inferior_data(struct inferior_list *list, void *data,
			    void (*action)(struct inferior_list_entry *,
					   void *));
extern struct thread_info *current_inferior;
void remove_inferior(struct inferior_list *list,
		     struct inferior_list_entry *entry);
void remove_thread(struct thread_info *thread);
void add_thread(unsigned long thread_id, void *target_data, unsigned int);
struct inferior_list_entry *
find_inferior(struct inferior_list *,
	      int (*func)(struct inferior_list_entry *, void *),
	      void *arg);
#ifndef _HAVE_ID_CONVERSION_FUNCS
# define _HAVE_ID_CONVERSION_FUNCS 1
struct inferior_list_entry *find_inferior_id(struct inferior_list *list,
					     unsigned long id);
unsigned int thread_id_to_gdb_id(unsigned long);
unsigned int thread_to_gdb_id(struct thread_info *);
unsigned long gdb_id_to_thread_id(unsigned int);
#endif /* !_HAVE_ID_CONVERSION_FUNCS */
void clear_inferiors(void);
void *inferior_target_data(struct thread_info *);
void set_inferior_target_data(struct thread_info *, void *);
void *inferior_regcache_data(struct thread_info *);
void set_inferior_regcache_data(struct thread_info *, void *);
void change_inferior_id(struct inferior_list *list, unsigned long new_id);

/* Public variables in server.c */

extern unsigned long cont_thread;
extern unsigned long general_thread;
extern unsigned long step_thread;
extern unsigned long thread_from_wait;
extern unsigned long old_thread_from_wait;
extern int server_waiting;

extern jmp_buf toplevel;

/* Functions from remote-utils.c */

#ifndef REMOTE_UTILS_H
int putpkt(char *buf);
int getpkt(char *buf);
void remote_open(char *name);
void remote_close(void);
void write_ok(char *buf);
void write_enn(char *buf);
void enable_async_io(void);
void disable_async_io(void);
void unblock_async_io(void);
void block_async_io(void);
void convert_ascii_to_int(char *from, unsigned char *to, int n);
void convert_int_to_ascii(unsigned char *from, char *to, int n);
void new_thread_notify(int id);
void dead_thread_notify(int id);
void prepare_resume_reply(char *buf, char status, unsigned char sig);

void decode_m_packet(char *from, CORE_ADDR *mem_addr_ptr,
		     unsigned int *len_ptr);
void decode_M_packet(char *from, CORE_ADDR *mem_addr_ptr,
		     unsigned int *len_ptr, unsigned char *to);

int unhexify(char *bin, const char *hex, int count);
int hexify(char *hex, const char *bin, int count);

int look_up_one_symbol(const char *name, CORE_ADDR *addrp);
#endif /* !REMOTE_UTILS_H */

/* Functions from ``signals.c''.  */
enum target_signal target_signal_from_host(int hostsig);
int target_signal_to_host_p(enum target_signal oursig);
int target_signal_to_host(enum target_signal oursig);
const char *target_signal_to_name(enum target_signal oursig);

/* Functions from utils.c */

void perror_with_name(const char *string);
void error(const char *string,...) ATTR_NORETURN ATTR_FORMAT(printf, 1, 2);
void fatal(const char *string,...) ATTR_NORETURN ATTR_FORMAT(printf, 1, 2);
void warning(const char *string,...) ATTR_FORMAT(printf, 1, 2);

/* Functions from the register cache definition: */
void init_registers(void);

#if defined(USE_WIN32API) && USE_WIN32API
# include <winsock2.h>
#endif /* USE_WIN32API */

#include "common-utils.h"
#include "debug.h"

#include "gdb_assert.h"

/* Maximum number of bytes to read/write at once. The value here
 * is chosen to fill up a packet (the headers account for the 32).  */
#define MAXBUFBYTES(N) (((N)-32)/2)

/* Buffer sizes for transferring memory, registers, etc. Round up PBUFSIZ to
 * hold all the registers, at least.  */
#define	PBUFSIZ (((registers_length() + 32) > 2000) \
		 ? (registers_length() + 32) \
		 : 2000)

#if defined(ENABLE_NLS) && ENABLE_NLS
# ifdef HAVE_LIBINTL_H
#  include <libintl.h>
# else
#  if defined(__GNUC__) && !defined(__STRICT_ANSI__)
 #   warning "server.h expects <libintl.h> to be included."
#  endif /* __GNUC__ && !__STRICT_ANSI__ */
# endif /* HAVE_LIBINTL_H */
# if !defined(_)
#  define _(String) gettext(String)
# endif /* !_ */
# if !defined(N_)
#  ifdef gettext_noop
#   define N_(String) gettext_noop(String)
#  else
#   define N_(String) (String)
#  endif /* gettext_noop */
# endif /* !N_ */
#else
# if !defined(_)
#  define _(String) (String)
# endif /* !_ */
# if !defined(N_)
#  define N_(String) (String)
# endif /* !N_ */
#endif /* ENABLE_NLS */

#endif /* SERVER_H */

/* EOF */
