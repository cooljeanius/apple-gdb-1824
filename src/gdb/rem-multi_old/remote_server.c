/* remote_server.c
 * Main code for remote server for GDB, the GNU Debugger.
 * Copyright (C) 1989 Free Software Foundation, Inc. */
/*
This file is part of GDB.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "param_old.h" /* renamed from "param.h" */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* Prototypes for functions used in this file: */
void read_inferior_memory(unsigned int memaddr, char *myaddr,
                          unsigned int len);
void fetch_inferior_registers(void);
unsigned char resume(int arg1, int arg2, char **status);
void kill_inferior(void);
void initialize(void);
void try_writing_regs_command(void);
int create_inferior(char *buf, char **environ);
int read_register(void); /* unneeded? */
int store_inferior_registers(int regno);
int write_inferior_memory(CORE_ADDR memaddr, char *myaddr, int len);

/* global variables: */
extern char registers[];
int inferior_pid;
extern char **environ;

/* Descriptor for I/O to remote machine: */
int remote_desc;
int kiodebug = 0;
int remote_debugging;

/* More prototypes: */
void remote_send(char *buf); /* unneeded? */
void putpkt(char *buf);
void getpkt(char *buf);
void remote_open(const char *name, int from_tty);
void write_ok(char *buf);
void write_enn(char *buf);
void convert_ascii_to_int(char *from, char *to, int n);
void convert_int_to_ascii(char *from, char *to, int n);
void prepare_resume_reply(char *buf, char *status, unsigned char signal);
void decode_m_packet(char *from, unsigned int *mem_addr_ptr,
                     unsigned int *len_ptr);
void decode_M_packet(char *from, unsigned int *mem_addr_ptr,
                     unsigned int *len_ptr, char *to);

/* (some) definition(s): */
#if !defined(REGISTER_BYTES) && !defined(HAVE_DECL_REGISTER_BYTES)
/* arbitrarily made-up default: */
# define REGISTER_BYTES 0
#endif /* !REGISTER_BYTES && !HAVE_DECL_REGISTER_BYTES */

/* main function: */
int main(int argc, char *argv[])
{
    char ch;
    char *status;
    char own_buf[2000], mem_buf[2000];
	int i = 0;
	unsigned char signal;
	unsigned int mem_addr, len;

	initialize();
    printf("\nwill open serial link\n");
    remote_open("/dev/ttya", 0);

	if (argc < 2) {
		printf("Enter name of program to be run with command line args\n");
#if defined(HAVE_FGETS) || defined(__STDC__)
        fgets(own_buf, 2000, stdin);
#else
		gets(own_buf);
#endif /* HAVE_FGETS */
		inferior_pid = create_inferior(own_buf, environ);
		printf("\nProcess %s created; pid = %d\n", own_buf, inferior_pid);
	} else {
		inferior_pid = create_inferior(argv[1], environ);
		printf("\nProcess %s created; pid = %d\n", argv[1], inferior_pid);
	}

    do {
        getpkt(own_buf);
        printf("\nPacket received is>:%s\n", own_buf);
		i = 0;
		ch = own_buf[i++];
	    switch (ch) {
			case 'h':
                /****This is only for tweaking the gdb+ program */
                signal = resume(1, 0, &status);
				prepare_resume_reply(own_buf, status, signal);
				break;
				/********end tweak********************************/

			case 'g':
                fetch_inferior_registers();
                convert_int_to_ascii(registers, own_buf, REGISTER_BYTES);
                break;
			case 'G':
                convert_ascii_to_int(&own_buf[1], registers,
                                     REGISTER_BYTES);
                if (store_inferior_registers(-1) == 0) {
                    write_ok(own_buf);
                } else {
                    write_enn(own_buf);
                }
                break;
			case 'm':
                decode_m_packet(&own_buf[1], &mem_addr, &len);
                read_inferior_memory(mem_addr, mem_buf, len);
                convert_int_to_ascii(mem_buf, own_buf, len);
                break;
			case 'M':
                decode_M_packet(&own_buf[1], &mem_addr, &len, mem_buf);
                if (write_inferior_memory(mem_addr, mem_buf, len) == 0) {
                    write_ok(own_buf);
                } else {
                    write_enn(own_buf);
                }
                break;
			case 'c':
                signal = resume(0, 0, &status);
                printf("\nSignal received is >: %0x \n", signal);
                prepare_resume_reply(own_buf, status, signal);
                break;
			case 's':
                signal = resume(1, 0, &status);
                prepare_resume_reply(own_buf, status, signal);
                break;
			case 'k':
                kill_inferior();
                sprintf(own_buf,"q");
                putpkt(own_buf);
                printf("\nObtained kill request...terminating\n");
                close(remote_desc);
                exit(0);
			case 't':
                try_writing_regs_command();
                own_buf[0] = '\0';
                break;
			default:
                printf("\nUnknown option chosen by master\n");
                write_enn(own_buf);
                break;
        }

        putpkt(own_buf);
     } while(1);

    close(remote_desc);
    /** now get out of here**/
    printf("\nFinished reading data from serial link - Bye!\n");
    exit(0);
}

/* EOF */
