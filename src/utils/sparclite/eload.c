/* eload.c: Program to load image into a SPARClite monitor board w/Ethernet
 * Copyright 1993 Free Software Foundation, Inc.  */
/*
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
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* Call with:

   eload PROG HOSTNAME

(HOSTNAME is the name (or IP address) of your eval board)

ie: eload hello sparky

*/

#ifdef HAVE_CONFIG_H
# include "config.h"
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning eload.c expects "config.h" to be included.
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_STDIO_H
# include <stdio.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "eload.c expects <stdio.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_STDIO_H */

#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "eload.c expects <stdlib.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_STDLIB_H */

#ifdef HAVE_STRING_H
# include <string.h>
#else
# ifdef HAVE_STRINGS_H
#  include <strings.h>
# else
#  if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#   warning "eload.c expects a string-related header to be included."
#  endif /* __GNUC__ && !__STRICT_ANSI__ */
# endif /* HAVE_STRINGS_H */
#endif /* HAVE_STRING_H */

#include "ansidecl.h"

#if (defined(ANSI_PROTOTYPES) || defined(__PROTOTYPES)) && defined(HAVE_STDARG_H)
# include <stdarg.h>
#else
# if defined(HAVE_VARARGS_H)
#  include <varargs.h>
# else
#  if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#   warning "eload.c expects <stdarg.h> or <varargs.h> to be included."
#  endif /* __GNUC__ && !__STRICT_ANSI__ */
# endif /* HAVE_VARARGS_H */
#endif /* (ANSI_PROTOTYPES || __PROTOTYPES) && HAVE_STDARG_H */

#include "libiberty.h"
#include "bfd.h"

#ifdef HAVE_ERRNO_H
# include <errno.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "eload.c expects <errno.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_ERRNO_H */
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "eload.c expects <unistd.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_UNISTD_H */
#ifdef HAVE_FCNTL_H
# include <fcntl.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "eload.c expects <fcntl.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_FCNTL_H */

#ifndef HAVE_TERMIOS
# error "You lose for not having termios."
#endif /* !HAVE_TERMIOS */

#if defined(HAVE_TERMIOS)
# include <termios.h>
#else
# if defined(HAVE_TERMIO)
#  include <termio.h>
# else
#  if defined(HAVE_SGTTY)
#   include <sgtty.h>
#  else
#   if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#    warning "Not including a serial-port-related header."
#   endif /* __GNUC__ && !__STRICT_ANSI__ */
#  endif /* HAVE_SGTTY */
# endif /* HAVE_TERMIO */
#endif /* HAVE_TERMIOS */

#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "eload.c expects <sys/types.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_SYS_TYPES_H */
#ifdef HAVE_SYS_SOCKET_H
# include <sys/socket.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "eload.c expects <sys/socket.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_SYS_SOCKET_H */
#ifdef HAVE_NETDB_H
# include <netdb.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "eload.c expects <netdb.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_NETDB_H */
#ifdef HAVE_NETINET_IN_H
# include <netinet/in.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "eload.c expects <netinet/in.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_NETINET_IN_H */
#ifdef HAVE_SYS_TIME_H
# include <sys/time.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "eload.c expects <sys/time.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_SYS_TIME_H */

#define min(A, B) (((A) < (B)) ? (A) : (B))

/* Where the code goes by default: */
#ifndef LOAD_ADDRESS
# define LOAD_ADDRESS 0x40000000
#endif /* !LOAD_ADDRESS */

static void
usage(void)
{
  fprintf(stderr, "usage: eload executable-file network-name\n");
  exit(1);
}

static void
#if defined(ANSI_PROTOTYPES) || defined(__PROTOTYPES)
sys_error(char *msg, ...)
#else
sys_error(va_alist)
     va_dcl
#endif /* ANSI_PROTOTYPES || __PROTOTYPES */
{
  int e = errno;
  va_list args;

#if defined(ANSI_PROTOTYPES) || defined(__PROTOTYPES)
  va_start(args, msg);
#else
  va_start(args);
#endif /* ANSI_PROTOTYPES || __PROTOTYPES */

#if defined(ANSI_PROTOTYPES) || defined(__PROTOTYPES)
  vfprintf(stderr, msg, args);
#else
  {
    char *msg1;

    msg1 = va_arg(args, char *);
    vfprintf(stderr, msg1, args);
  }
#endif /* ANSI_PROTOTYPES || __PROTOTYPES */
  va_end(args);

  fprintf(stderr, ": %s\n", strerror(e));
  exit(1);
}

static void
#if defined(ANSI_PROTOTYPES) || defined(__PROTOTYPES)
error(char *msg, ...)
#else
error(va_alist)
     va_dcl
#endif /* ANSI_PROTOTYPES || __PROTOTYPES */
{
  va_list args;

#if defined(ANSI_PROTOTYPES) || defined(__PROTOTYPES)
  va_start(args, msg);
#else
  va_start(args);
#endif /* ANSI_PROTOTYPES || __PROTOTYPES */

#if defined(ANSI_PROTOTYPES) || defined(__PROTOTYPES)
  vfprintf(stderr, msg, args);
#else
  {
    char *msg1;

    msg1 = va_arg(args, char *);
    vfprintf(stderr, msg1, args);
  }
#endif /* ANSI_PROTOTYPES || __PROTOTYPES */
  va_end(args);

  fputc('\n', stderr);
  exit(1);
}

int netfd;

static ssize_t
#if defined(ANSI_PROTOTYPES) || defined(__PROTOTYPES)
recv_buf(int fd, unsigned char *buf, int len, int timeout)
#else
recv_buf(fd, buf, len, timeout)
     int fd, len;
     unsigned char *buf;
     int timeout;
#endif /* ANSI_PROTOTYPES || __PROTOTYPES */
{
  ssize_t cc;
  fd_set readfds;

  FD_ZERO(&readfds);
  FD_SET(fd, &readfds);

  if (timeout >= 0)
    {
      struct timeval timebuf;

      timebuf.tv_sec = timeout;
      timebuf.tv_usec = 0;
      cc = select((fd + 1), &readfds, 0, 0, &timebuf);
    }
  else
    cc = select((fd + 1), &readfds, 0, 0, 0);

  if (cc == 0)
    return 0;

  if (cc != 1)
    sys_error("recv_buf: Bad return value from select:");

  cc = recv(fd, buf, (size_t)len, 0);

  if (cc < 0)
    sys_error("Got an error from recv: ");

  return cc;
}

static void
#if defined(ANSI_PROTOTYPES) || defined(__PROTOTYPES)
send_buf(int fd, unsigned char *buf, int len)
#else
send_buf(fd, buf, len)
     int fd, len;
     unsigned char *buf;
#endif /* ANSI_PROTOTYPES || __PROTOTYPES */
{
  ssize_t cc;

  cc = send(fd, buf, (size_t)len, 0);

  if (cc == len)
    return;

  if (cc < 0)
    sys_error("Got an error from send: ");

  printf("Short count in send: tried %d, sent %d\n", len, cc);
}

static unsigned short
#if defined(ANSI_PROTOTYPES) || defined(__PROTOTYPES)
calc_checksum(unsigned char *buffer, int count)
#else
calc_checksum(buffer, count)
     unsigned char *buffer;
     int count;
#endif /* ANSI_PROTOTYPES || __PROTOTYPES */
{
  unsigned short checksum;
  unsigned short *s;

  s = (unsigned short *)buffer;

  checksum = 0;
  for (; count > 0; count -= 2)
    checksum += (unsigned short)*s++;

  if (count != 0)
    checksum += (unsigned short)(*s & 0xff00);

  return checksum;
}

static void
#if defined(ANSI_PROTOTYPES) || defined(__PROTOTYPES)
send_data(unsigned char *buffer, int fd, unsigned long addr, int count)
#else
send_data(buffer, fd, addr, count)
     unsigned char *buffer;
     int fd;
     unsigned long addr;
     int count;
#endif /* ANSI_PROTOTYPES || __PROTOTYPES */
{
  ssize_t cc;
  int i;
  static int pkt_num = 0;
  unsigned char snd_buf[2000];
  unsigned short checksum;
  static unsigned long old_addr = (unsigned long)-1L;

  while (1)
    {
      if (addr != old_addr)
	{
	  snd_buf[0] = 0x1;	/* Load command */
	  snd_buf[1] = 0x1;	/* Loading address */
	  snd_buf[2] = (unsigned char)(addr >> 24);
	  snd_buf[3] = (unsigned char)(addr >> 16);
	  snd_buf[4] = (unsigned char)(addr >> 8);
	  snd_buf[5] = (unsigned char)addr;

	  checksum = 0;
	  for (i = 0; i < 6; i++)
	    checksum += (unsigned short)snd_buf[i];
	  checksum &= 0xff;

	  send_buf(fd, snd_buf, 6);
	  cc = recv_buf(fd, snd_buf, sizeof(snd_buf), -1);

	  if (cc < 1)
	    {
	      fprintf(stderr, "Got back short checksum for load addr\n");
	      exit(1);
	    }

	  if (checksum != snd_buf[0])
	    {
	      fprintf(stderr, "Got back bad checksum for load addr\n");
	      exit(1);
	    }
	  pkt_num = 0;		/* Load addr resets packet seq # */
	  old_addr = addr;
	}

      memcpy((snd_buf + 6), buffer, (size_t)count);

      checksum = calc_checksum(buffer, count);

      snd_buf[0] = 0x1;		/* Load command */
      snd_buf[1] = 0x2;		/* Loading data */
      snd_buf[2] = (unsigned char)(pkt_num >> 8);
      snd_buf[3] = (unsigned char)pkt_num;
      snd_buf[4] = (unsigned char)(checksum >> 8);
      snd_buf[5] = (unsigned char)checksum;

      send_buf(fd, snd_buf, (count + 6));
      cc = recv_buf(fd, snd_buf, sizeof(snd_buf), 3);

      if (cc == 0)
	{
	  fprintf(stderr,
                  "send_data: timeout sending %d bytes to addr 0x%lx, retrying\n",
                  count, addr);
	  continue;
	}

      if (cc < 1)
	{
	  fprintf(stderr, "Got back short response for load data\n");
	  exit(1);
	}

      if (snd_buf[0] != 0xff)
	{
	  fprintf(stderr, "Got back bad response for load data\n");
	  exit(1);
	}

      old_addr += (unsigned long)count;
      pkt_num++;

      return;
    }
}

extern int optind;

int
#if defined(ANSI_PROTOTYPES) || defined(__PROTOTYPES)
main(int argc, char **argv)
#else
main(argc, argv)
     int argc;
     char **argv;
#endif /* ANSI_PROTOTYPES || __PROTOTYPES */
{
  ssize_t cc;
  int c;
  unsigned char buf[10];
  asection *section;
  bfd *pbfd;
  unsigned long entry;
  struct hostent *he;
  struct sockaddr_in sockaddr;

  while ((c = getopt(argc, argv, "")) != EOF)
    {
      switch (c)
	{
	default:
	  usage();
	}
    }
  argc -= optind;
  argv += optind;

  if (argc != 2)
    usage();

  pbfd = bfd_openr(argv[1], 0);

  if (pbfd == NULL)
    sys_error("Open of PROG failed");

  /* Setup the socket.  Must be raw UDP: */
  he = gethostbyname(argv[2]);

  if (!he)
    sys_error("No such host");

  netfd = socket(PF_INET, SOCK_DGRAM, 0);

  sockaddr.sin_family = PF_INET;
  sockaddr.sin_port = htons(7000);
  memcpy(&sockaddr.sin_addr.s_addr, he->h_addr, sizeof(struct in_addr));

  if (connect(netfd, (const struct sockaddr *)&sockaddr, sizeof(sockaddr)))
    sys_error("Connect failed");

  buf[0] = 0x5;
  buf[1] = 0;

  send_buf(netfd, buf, 2);	/* Request version */
  cc = recv_buf(netfd, buf, sizeof(buf), -1); /* Get response */

  if (cc < 3)
    {
      fprintf(stderr, "SPARClite appears to be ill\n");
      exit(1);
    }

  printf("[SPARClite appears to be alive]\n");

  if (!bfd_check_format(pbfd, bfd_object))
    error("It does NOT seem to be an object file");

  for (section = pbfd->sections; section; section = section->next)
    {
      if (bfd_get_section_flags(pbfd, section) & SEC_ALLOC)
	{
	  bfd_vma section_address;
	  size_t section_size;
	  const char *section_name;

	  section_name = bfd_get_section_name(pbfd, section);

	  section_address = bfd_get_section_vma(pbfd, section);
	  /* Adjust sections from a.out files, since they do NOT
	   * carry their addresses with: */
	  if (bfd_get_flavour(pbfd) == bfd_target_aout_flavour)
	    section_address += LOAD_ADDRESS;
	  section_size = bfd_section_size (pbfd, section);

	  printf("[Loading section %s at %lx (%ld bytes)]\n",
		 section_name, section_address, section_size);

	  /* Text, data, or lit: */
	  if (bfd_get_section_flags(pbfd, section) & SEC_LOAD)
	    {
	      file_ptr fptr;

	      fptr = 0;

	      while (section_size > 0)
		{
		  char buffer[1024];
		  int count;
		  static char inds[] = "|/-\\";
		  static int k = 0;

		  count = min((int)section_size, 1024);

		  bfd_get_section_contents(pbfd, section, buffer, fptr,
					   (bfd_size_type)count);

#if defined(DO_EXTRA_CHECKSUMMING)
		  {
		    int i;
		    unsigned char checksum;

		    checksum = 0;
		    for (i = 0; i < count; i++)
		      checksum += buffer[i];
		  }
#endif /* DO_EXTRA_CHECKSUMMING */

		  printf("\r%c", inds[k++ % 4]);
		  fflush(stdout);

		  send_data((unsigned char *)buffer, netfd,
                            section_address, count);

		  section_address += (bfd_vma)count;
		  fptr += count;
		  section_size -= (size_t)count;
		}
	    }
	  else			/* BSS */
	    printf("Not loading BSS \n");
	}
    }

  entry = bfd_get_start_address(pbfd);

  printf("[Starting %s at 0x%lx]\n", argv[1], entry);

  buf[0] = 0x3;
  buf[1] = 0;
  buf[2] = (unsigned char)(entry >> 24);
  buf[3] = (unsigned char)(entry >> 16);
  buf[4] = (unsigned char)(entry >> 8);
  buf[5] = (unsigned char)entry;

  send_buf(netfd, buf, 6);	/* Send start addr */
  cc = recv_buf(netfd, buf, sizeof(buf), -1); /* Get response */

  if ((cc < 1) || (buf[0] != 0x55))
    {
      fprintf(stderr, "Failed to take start address\n");
      exit(1);
    }

  exit(0);
}

/* EOF */
