/* rem-mulit_old/remote_utils.c
 * Remote utility routines for the remote server for GDB, the GNU debugger.
 * Copyright (C) 1986, 1989 Free Software Foundation, Inc. */
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
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "param_old.h" /* renamed from "param.h" */
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#ifdef HAVE_A_OUT_H
# include <a.out.h>
#else
# ifdef __GNUC__
#  include "a.out.gnu_old.h" /* renamed from "a.out.gnu.h" */
# else
#  ifdef sun
#   include "a.out.sun4_old.h" /* renamed from "a.out.sun4.h" */
#  else
#   ifdef vax
#    include "a.out.vax_old.h" /* renamed from "a.out.vax.h" */
#   else
#    warning remote_utils.c expects a version of "a.out.h" to be included.
#   endif /* vax */
#  endif /* sun */
# endif /* __GNUC__ */
#endif /* HAVE_A_OUT_H */
#include <sys/file.h>
#include <sgtty.h>
#include <errno.h>

extern int remote_desc;
extern int remote_debugging;
extern int kiodebug;

/* Prototypes for functions from this file: */
void remote_open(char *name, int from_tty);
void remote_send(char *buf);
void putpkt(char *buf);
void getpkt(char *buf);

void write_ok(char *buf);
void write_enn(char *buf);
void convert_ascii_to_int(char *from, char *to, int n);
void convert_int_to_ascii(char *from, char *to, int n);
void prepare_resume_reply(char *buf, char *status, unsigned char signal);
void decode_m_packet(char *from, unsigned int *mem_addr_ptr,
                     unsigned int *len_ptr);
void decode_M_packet(char *from, unsigned int *mem_addr_ptr,
                     unsigned int *len_ptr, char *to);

/* Open a connection to a remote debugger.
 * NAME is the filename used for communication. */
void remote_open(char *name, int from_tty)
{
  struct sgttyb sg;

  remote_debugging = 0;

  remote_desc = open(name, O_RDWR);
  if (remote_desc < 0) {
      printf("\ncould not open remote device\n");
  }

  ioctl(remote_desc, TIOCGETP, &sg);
  sg.sg_flags = RAW;
  ioctl(remote_desc, TIOCSETP, &sg);

  if (from_tty) {
      printf("Remote debugging using %s\n", name);
  }
  remote_debugging = 1;
}

/* Convert hex digit A to a number: */
static int fromhex(int a)
{
  if ((a >= '0') && (a <= '9')) {
    return (a - '0');
  } else if ((a >= 'a') && (a <= 'f')) {
    return (a - 'a' + 10);
  } else {
    /*p*/error("Reply contains invalid hex digit");
  }
}

/* Convert number NIB to a hex digit: */
static char tohex(int nib)
{
  if (nib < 10) {
    return (char)('0' + (char)nib);
  } else {
    return (char)('a' + (char)nib - 10);
  }
}

/* Send the command in BUF to the remote machine & read the reply into BUF.
 * Report an error if we get an error reply. */
void remote_send(char *buf)
{
  putpkt(buf);
  getpkt(buf);

  if (buf[0] == 'E') {
      /*p*/error("Remote failure reply: %s", buf);
  }
}

/* Send a packet to the remote machine, with error checking.
 * The data of the packet is in BUF. */
void putpkt(char *buf)
{
  int i;
  unsigned char csum = 0U;
  char buf2[500];
  char buf3[1];
  size_t cnt = strlen(buf);
  char *p;

  if (kiodebug) {
      fprintf(stderr, "Sending packet: %s\n", buf);
  }

  /* Copy the packet into buffer BUF2, encapsulating it & giving it
   * a checksum: */
  p = buf2;
  *p++ = '$';

  for (i = 0; i < (int)cnt; i++) {
      csum += (unsigned char)buf[i];
      *p++ = buf[i];
  }
  *p++ = '#';
  *p++ = tohex((csum >> 4) & 0xf);
  *p++ = tohex(csum & 0xf);

  /* Send it over and over until we get a positive ack: */
  do {
      write(remote_desc, buf2, (size_t)(p - buf2));
      read(remote_desc, buf3, 1);
  } while (buf3[0] != '+');
}

static int readchar(void)
{
  char buf[1];
  while (read(remote_desc, buf, 1) != 1) {
    /* (do nothing) */ ;
  }
  return (buf[0] & 0x7f);
}

extern int kiodebug;

/* Read a packet from the remote machine, with error checking,
 * and store it in BUF: */
void getpkt(char *buf)
{
  char *bp;
  unsigned char csum, c, c1, c2;

  while (1) {
      csum = 0;
      while ((c = (unsigned char)readchar()) != '$') {
        /* (do nothing) */ ;
      }

      bp = buf;
      while (1) {
	  c = (unsigned char)readchar();
          if (c == '#') {
              break;
          }
	  *bp++ = (char)c;
	  csum += (unsigned char)c;
      }
      *bp = 0;

      c1 = (unsigned char)fromhex(readchar());
      c2 = (unsigned char)fromhex(readchar());
      if (csum == ((c1 << 4) + c2)) {
        break;
      }

      printf("Bad checksum, sentsum=0x%x, csum=0x%x, buf=%s\n",
             ((c1 << 4) + c2), csum, buf);
      write(remote_desc, "-", 1);
  }

  write(remote_desc, "+", 1);

  if (kiodebug) {
    fprintf(stderr,"Packet received :%s\n", buf);
  }
}


void write_ok(char *buf)
{
  buf[0] = 'O';
  buf[1] = 'k';
  buf[2] = '\0';
}

void write_enn(char *buf)
{
  buf[0] = 'E';
  buf[1] = 'N';
  buf[2] = 'N';
  buf[3] = '\0';
}

void convert_int_to_ascii(char *from, char *to, int n)
{
  int nib;
  char ch;
  while (n--) {
    ch = *from++;
    nib = (((ch & 0xf0) >> 4) & 0x0f);
    *to++ = tohex(nib);
    nib = ch & 0x0f;
    *to++ = tohex(nib);
  }
  *to++ = 0;
}


void convert_ascii_to_int(char *from, char *to, int n)
{
  int nib1, nib2;
  while (n--) {
    nib1 = fromhex(*from++);
    nib2 = fromhex(*from++);
    *to++ = (char)((((nib1 & 0x0f) << 4) & 0xf0) | (nib2 & 0x0f));
  }
}

void prepare_resume_reply(char *buf, char *status, unsigned char signal)
{
  int nib;
  char ch = '0';

  *buf++ = 'S';
  *buf++ = *status; /* FIXME: there might have been pointer misuse here? */
  nib = ((signal & 0xf0) >> 4);
  *buf++ = tohex(nib);
  nib = (signal & 0x0f);
  *buf++ = tohex(nib);
  *buf++ = 0;

  /* dummy condition to use 'ch': */
  if (ch == 0) {
    ;
  }
}

void decode_m_packet(char *from, unsigned int *mem_addr_ptr,
                     unsigned int *len_ptr)
{
    int i = 0;
    int j = 0;
    char ch;
    *mem_addr_ptr = *len_ptr = 0;
    /************debugging begin************/
    printf("\nIn decode_m_packet");
    /************debugging end************/

    while ((ch = from[i++]) != ',') {
        *mem_addr_ptr = (*mem_addr_ptr << 4);
        *mem_addr_ptr |= (fromhex(ch) & 0x0f);
    }
    /************debugging begin************/
    printf("\nFinished mem_addr part");
    /************debugging end************/

    for ((j = 0); (j < 4); j++) {
        if ((ch = from[i++]) == 0) {
            break;
        }
        *len_ptr = (*len_ptr << 4);
        *len_ptr |= (fromhex(ch) & 0x0f);
    }
    /************debugging begin************/
    printf("\nFinished len_ptr part");
    /************debugging end************/
}

void decode_M_packet(char *from, unsigned int *mem_addr_ptr,
                     unsigned int *len_ptr, char *to)
{
    int i = 0;
    int j = 0;
    char ch;
    *mem_addr_ptr = *len_ptr = 0;
    /************debugging begin************/
    printf("\nIn decode_M_packet");
    /************debugging end************/

    while ((ch = from[i++]) != ',') {
        *mem_addr_ptr = (*mem_addr_ptr << 4);
        *mem_addr_ptr |= (fromhex(ch) & 0x0f);
    }
    /************debugging begin************/
    printf("\nFinished mem_addr part: memaddr = %x", *mem_addr_ptr);
    /************debugging end************/

    while ((ch = from[i++]) != ':') {
        *len_ptr = (*len_ptr << 4);
        *len_ptr |= (fromhex(ch) & 0x0f);
    }
    /* copy & paste loop from previous function to use 'j' here: */
    for ((j = 0); (j < 4); j++) {
        if ((ch = from[i++]) == 0) {
            break;
        }
        /* (no need to actually mess with 'len_ptr' this time around) */
    }
    /************debugging begin************/
    printf("\nFinished len_ptr part: len = %d", *len_ptr);
    /************debugging end************/

    convert_ascii_to_int(&from[i++], to, (int)(*len_ptr));

    /************debugging begin************/
    printf("\nmembuf : %x", *(int *)to);
    /************debugging end************/
}

/* EOF */
