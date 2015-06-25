/*
 * server.h
 *
 * Main header file for multi-ice server for GDB.
 *
 * Copyright (C) 1999 Free Software Foundation, Inc. */
/*
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
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef __GDB_MULTI_ICE_SERVER_H__
# define __GDB_MULTI_ICE_SERVER_H__ 1

#ifndef LONGEST
# define LONGEST long long
#endif /* !LONGEST */

#ifndef ULONGEST
# define ULONGEST unsigned long long
#endif /* !ULONGEST */

#ifndef PTR
# ifdef gdb_byte
#  define PTR gdb_byte *
# else
#  define PTR void *
# endif /* gdb_byte */
#endif /* !PTR */

#if !defined(REGISTER_BYTES) && (!defined(HAVE_DECL_REGISTER_BYTES) || !HAVE_DECL_REGISTER_BYTES)
/* arbitrarily made-up default: */
# define REGISTER_BYTES 4
#endif /* !REGISTER_BYTES && !HAVE_DECL_REGISTER_BYTES */

/* If DEBUG_ON is 1, then emit debugging infomation: */
extern int debug_on;

/* If RDI_LOGGING is 1, then turn on RDI logging for the target_arm_core */
extern int rdi_logging;

/* CLOSE_CONNECTION_NOW signals the main loop to abort the
 * current connection. */
extern int close_connection_now;

/* EXIT_NOW signals the main loop to close down completely: */
extern int exit_now;

/* REGISTERS holds the array of registers.
 * It should be defined in *-low.c */
extern char *aregisters;
extern int registers_up_to_date;
extern int registers_are_dirty;

/* Does the target support single stepping? */
extern int target_step;

/* Does a resume stop other modules in the debug target? */
extern int target_stop_others;

/* Multi-ICE driver [module] name */
extern char *target_driver_name;

/* TTHREAD_MODE enumerates the various thread operations: */
enum thread_mode {THREAD_SET, THREAD_ALIVE, THREAD_QUERY};
enum resume_mode {RESUME_CONTINUE, RESUME_STEP};
enum bp_action_type {BREAKPOINT_SET, BREAKPOINT_DELETE};
enum watch_type {WATCHPOINT_WRITE, WATCHPOINT_READ, WATCHPOINT_ACCESS};

int dispatch(char *input_buffer, int message_length);

/*
 * These are functions that are host-platform specific.
 * They will be defined in server-host.c
 */

int platform_init(void);
int handle_system_events(void);

/*
 * These are the various reporting functions:
 */
void output_error(char *format, ... );
void output(char *format, ... );
void print_help(void);
void print_version(int banner_only);


/* These are the functions that handle the remote protocol packets.
 * In general, there is one per packet type.
 * For now, these are just functions that need to be provided by the
 * appropriate "low-" file. If there are ever two of these, then someone
 * can get ambitious and multi-arch, target vector, etc-ize this setup. */
void enable_extended_ops(void);

char *remove_signal(char *input_buffer, int *signo);
int handle_breakpoint(enum bp_action_type action, char *input_buffer);
int handle_detach(char *input_buffer);
int handle_general_query(char *input_buffer);
int handle_general_set(char *input_buffer);
int handle_kill_target(char *input_buffer);
int handle_last_signal(char *input_buffer);
int handle_read_memory(char *input_buffer);
int handle_read_registers(char *input_buffer);
int handle_reset(char *input_buffer);
int handle_restart(char *input_buffer);
int handle_resume(char *input_buffer, enum resume_mode mode, int signo);
int handle_search_memory(char *input_buffer);
int handle_toggle_debug(char *input_buffer);
int handle_write_memory(char *input_buffer);
int handle_thread(char *input_buffer, enum thread_mode mode);
int handle_watchpoint(enum bp_action_type action, enum watch_type type,
                      char *input_buffer);
int handle_write_registers(char *input_buffer);
int handle_write_a_register(char *input_buffer);

/*
 * These are various useful conversion routines...
 */
int tohex(int nib);
int fromhex(int a);

void convert_bytes_to_ascii(char *from, char * to, int n, int swap);
void convert_ascii_to_bytes(char *from, char *to, int n, int swap);

void decode_m_packet(char *from, CORE_ADDR *mem_addr_ptr,
                     unsigned int *len_ptr);
void decode_M_packet(char *from, char *to, CORE_ADDR *mem_addr_ptr,
                     unsigned int *len_ptr);
/* these had to be renamed due to conflicting types in ../defs.h: */
extern void store_my_unsigned_integer(PTR addr, int len, ULONGEST val);
extern ULONGEST extract_my_unsigned_integer(PTR addr, int len);
extern LONGEST extract_my_signed_integer(PTR addr, int len);

#endif /* __GDB_MULTI_ICE_SERVER_H__ */

/* EOF */
