/* Remote utilities header file for multi-ice server for GDB.
 * Copyright (C) 1999 Free Software Foundation, Inc.  */
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
Foundation, Inc., 59 Temple Pl., Suite 330, Boston, MA 02111-1307, USA. */

#ifndef GDB_MULTI_ICE_REMOTE_UTILS_H
#define GDB_MULTI_ICE_REMOTE_UTILS_H 1

void close_connection(void);
void close_listener(void);
void check_for_SIGIO(void);
int getpkt(char *buf, int *buf_length);
int open_listener(char *listner_port);
int putpkt(char *buf);
int wait_for_connection(void);

/* remote-utils.c used to claim that it was the only one that used these,
 * but actually at least server-main.c also uses them: */
void enable_async_io(void);
void disable_async_io(void);
void close_and_exit(void);

#endif /* !GDB_MULTI_ICE_REMOTE_UTILS_H */

/* EOF */
