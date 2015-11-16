/* gdbserver/remote-utils.h
   Remote utility routines for the remote server for GDB.
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

#ifndef REMOTE_UTILS_H
#define REMOTE_UTILS_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#else
# define REMOTE_UTILS_H_NON_AUTOTOOLS_BUILD 1
#endif /* HAVE_CONFIG_H */

/* pretty sure about this being '&&' instead of '||', but not entirely: */
#if !defined(gdb_byte) && !defined(SERVER_H)
# include "bfd.h"
# ifdef bfd_byte
typedef bfd_byte gdb_byte;
# else
typedef unsigned char gdb_byte;
# endif /* bfd_byte */
#endif /* !gdb_byte && !SERVER_H */

#ifndef ULONGEST
# define ULONGEST unsigned long long
#endif /* !ULONGEST */

extern int remote_debug;
extern int noack_mode;
extern int transport_is_reliable;

int gdb_connected (void);

#define STDIO_CONNECTION_NAME "stdio"
int remote_connection_is_stdio (void);

ptid_t read_ptid (char *buf, char **obuf);
char *write_ptid (char *buf, ptid_t ptid);

int putpkt (char *buf);
int putpkt_binary (char *buf, int len);
int putpkt_notif (char *buf);
int getpkt (char *buf);
void remote_prepare (char *name);
void remote_open (char *name);
void remote_close (void);
void write_ok (char *buf);
void write_enn (char *buf);
void initialize_async_io (void);
void enable_async_io (void);
void disable_async_io (void);
void check_remote_input_interrupt_request (void);
#ifndef SERVER_H
void convert_ascii_to_int (const char *from, unsigned char *to, int n);
void convert_int_to_ascii (const unsigned char *from, char *to, int n);
#endif /* !SERVER_H */
void new_thread_notify (int id);
void dead_thread_notify (int id);
#ifndef SERVER_H
void prepare_resume_reply (char *buf, ptid_t ptid,
			   struct target_waitstatus *status);
#endif /* !SERVER_H */

const char *decode_address_to_semicolon (CORE_ADDR *addrp, const char *start);
void decode_address (CORE_ADDR *addrp, const char *start, int len);
void decode_m_packet (char *from, CORE_ADDR * mem_addr_ptr,
		      unsigned int *len_ptr);
#ifndef SERVER_H
void decode_M_packet (char *from, CORE_ADDR * mem_addr_ptr,
		      unsigned int *len_ptr, unsigned char **to_p);
#endif /* !SERVER_H */
int decode_X_packet (char *from, int packet_len, CORE_ADDR * mem_addr_ptr,
		     unsigned int *len_ptr, unsigned char **to_p);
int decode_xfer_write (char *buf, int packet_len,
		       CORE_ADDR *offset, unsigned int *len,
		       unsigned char *data);
int decode_search_memory_packet (const char *buf, int packet_len,
				 CORE_ADDR *start_addrp,
				 CORE_ADDR *search_space_lenp,
				 gdb_byte *pattern,
				 unsigned int *pattern_lenp);

int unhexify(char *bin, const char *hex, int count);
int hexify(char *hex, const char *bin, int count);
int remote_escape_output(const gdb_byte *buffer, int len, gdb_byte *out_buf,
						 int *out_len, int out_maxlen);
char *unpack_varlen_hex(char *buff,  ULONGEST *result);

void clear_symbol_cache(struct sym_cache **symcache_p);
#ifndef SERVER_H
int look_up_one_symbol(const char *name, CORE_ADDR *addrp, int may_ask_gdb);
#endif /* !SERVER_H */

int relocate_instruction(CORE_ADDR *to, CORE_ADDR oldloc);

void monitor_output(const char *msg);

#endif /* REMOTE_UTILS_H */

/* EOF */
