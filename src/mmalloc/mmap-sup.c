/* mmap-sup.c: Support for an sbrk-like function that uses mmap.
 * Copyright 1992, 2000 Free Software Foundation, Inc.
 *
 * Contributed by Fred Fish at Cygnus Support.  <fnf@cygnus.com>  */
/*
This file is part of the GNU C Library.

The GNU C Library is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public License as
published by the Free Software Foundation; either version 2 of the
License, or (at your option) any later version.

The GNU C Library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with the GNU C Library; see the file COPYING.LIB.  If
not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

#ifdef HAVE_CONFIG_H
# include "config.h"
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning mmap-sup.c expects "config.h" to be included.
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_CONFIG_H */

#if defined(HAVE_MMAP)

#include "mmprivate.h"

#ifdef HAVE_UNISTD_H
# include <unistd.h>	/* Prototypes for lseek */
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "mmap-sup.c expects <unistd.h> to be included for lseek()."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_UNISTD_H */
#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "mmap-sup.c expects <stdlib.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_STDLIB_H */
#ifdef HAVE_STDIO_H
# include <stdio.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "mmap-sup.c expects <stdio.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_STDIO_H */
#ifdef HAVE_STRING_H
# include <string.h>
#else
# ifdef HAVE_STRINGS_H
#  include <strings.h>
# else
#  if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#   warning "mmap-sup.c expects either <string.h> or <strings.h> to be included."
#  endif /* __GNUC__ && !__STRICT_ANSI__ */
# endif /* HAVE_STRINGS_H */
#endif /* HAVE_STRING_H */
#ifdef HAVE_FCNTL_H
# include <fcntl.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "mmap-sup.c expects <fcntl.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_FCNTL_H */
#ifdef HAVE_SYS_MMAN_H
# include <sys/mman.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "mmap-sup.c expects <sys/mman.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_SYS_MMAN_H */
#ifdef HAVE_ERRNO_H
# include <errno.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "mmap-sup.c expects <errno.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_ERRNO_H */
/* APPLE LOCAL: Needed sys/types.h for caddr_t  */
#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "mmap-sup.c expects <sys/types.h> to be included for caddr_t."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_SYS_TYPES_H */
#ifdef HAVE_INTTYPES_H
# include <inttypes.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "mmap-sup.c expects <inttypes.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_INTTYPES_H */
#ifdef HAVE_STDINT_H
# include <stdint.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "mmap-sup.c expects <stdint.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_STDINT_H */

#ifdef HAVE_MACH_MACH_H
# include <mach/mach.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__) && defined(__MACH__)
#  warning "mmap-sup.c expects <mach/mach.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ && __MACH__ */
#endif /* HAVE_MACH_MACH_H */

#ifdef HAVE_MACH_PORT_H
# include <mach/port.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__) && defined(__MACH__)
#  warning "mmap-sup.c expects <mach/port.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ && __MACH__ */
#endif /* HAVE_MACH_PORT_H */

#ifdef HAVE_AVAILABILITYMACROS_H
# include <AvailabilityMacros.h>
# define MACH64 (MAC_OS_X_VERSION_MAX_ALLOWED >= 1040)
# if MACH64
#  ifdef HAVE_MACH_MACH_VM_H
#   include <mach/mach_vm.h>
#  else
#   if defined(__GNUC__) && !defined(__STRICT_ANSI__) && defined(__MACH__)
#    warning "mmap-sup.c expects <mach/mach_vm.h> to be included."
#   endif /* __GNUC__ && !__STRICT_ANSI__ && __MACH__ */
#  endif /* HAVE_MACH_MACH_VM_H */
# else /* !MACH64 */
#  ifdef HAVE_MACH_VM_REGION_H
#   include <mach/vm_region.h>
#  else
#   if defined(__GNUC__) && !defined(__STRICT_ANSI__) && defined(__MACH__)
#    warning "mmap-sup.c expects <mach/vm_region.h> to be included."
#   endif /* __GNUC__ && !__STRICT_ANSI__ && __MACH__ */
#  endif /* HAVE_MACH_VM_REGION_H */
#  ifndef mach_vm_size_t
#   define mach_vm_size_t vm_size_t
#  endif /* !mach_vm_size_t */
#  ifndef mach_vm_address_t
#   define mach_vm_address_t vm_address_t
#  endif /* !mach_vm_address_t */
#  ifndef mach_vm_read
#   define mach_vm_read vm_read
#  endif /* !mach_vm_read */
#  ifndef mach_vm_write
#   define mach_vm_write vm_write
#  endif /* !mach_vm_write */
#  ifndef mach_vm_read
#   define mach_vm_region vm_region
#  endif /* !mach_vm_region */
#  ifndef VM_REGION_BASIC_INFO_COUNT_64
#   define VM_REGION_BASIC_INFO_COUNT_64 VM_REGION_BASIC_INFO_COUNT
#  endif /* !VM_REGION_BASIC_INFO_COUNT_64 */
#  ifndef VM_REGION_BASIC_INFO_64
#   define VM_REGION_BASIC_INFO_64 VM_REGION_BASIC_INFO
#  endif /* !VM_REGION_BASIC_INFO_64 */
# endif /* MACH64 */
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__) && defined(__APPLE__)
#  warning "mmap-sup.c expects <AvailabilityMacros.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ && __APPLE__ */
#endif /* HAVE_AVAILABILITYMACROS_H */

#ifdef HAVE_IPC_IPC_TYPES_H
# include <ipc/ipc_types.h>
#endif /* HAVE_IPC_IPC_TYPES_H */

#if defined(HAVE_MACH_PORT_T) && !defined(HAVE_PORT_T) && !defined(port_t)
/* Could just rename all instaces of port_t to mach_port_t unconditionally,
 * as some versions do, but whatever: */
# define port_t mach_port_t
#endif /* HAVE_MACH_PORT_T && !HAVE_PORT_T && !port_t */

#ifndef HAVE_MSYNC
# define HAVE_MSYNC
#endif /* !HAVE_MSYNC */

#if !defined(MAP_ANONYMOUS) && defined(MAP_ANON)
# define MAP_ANONYMOUS MAP_ANON
#endif /* !MAP_ANONYMOUS && MAP_ANON */

#ifndef SEEK_SET
# define SEEK_SET 0
#endif /* !SEEK_SET */

/* Cache the pagesize for the current host machine.  Note that if the host
   does not readily provide a getpagesize() function, we need to emulate it
   elsewhere, not clutter up this file with lots of kluges to try to figure
   it out. */

static size_t pagesize;
#if defined(NEED_DECLARATION_GETPAGESIZE) && NEED_DECLARATION_GETPAGESIZE
extern int getpagesize PARAMS((void));
#endif /* NEED_DECLARATION_GETPAGESIZE */

#define PAGE_ALIGN(addr) (caddr_t)(((size_t)(addr) + pagesize - 1UL) & ~(pagesize - 1UL))

/* Return MAP_PRIVATE if MDP represents /dev/zero.  Otherwise, return
   MAP_SHARED.  */

#define MAP_PRIVATE_OR_SHARED(MDP) ((MDP->flags & MMALLOC_SHARED) \
                                    ? MAP_PRIVATE \
                                    : MAP_SHARED)

/* Get core for the memory region specified by MDP, using SIZE as the
 * amount to either add to or subtract from the existing region.  Works
 * like sbrk(), but using mmap(). */
PTR
__mmalloc_mmap_morecore(struct mdesc *mdp, int size)
{
  PTR result = NULL;
  off_t foffset;        /* File offset at which new mapping will start */
  size_t mapbytes;      /* Number of bytes to map */
  caddr_t moveto;       /* Address which will become the new top */
  caddr_t mapto;        /* Address we actually mapped to */
  char buf = 0;         /* Single byte to write to extend mapped file */
  int flags = 0;        /* Flags for mmap() */

  if (pagesize == (size_t)0UL)
    {
      pagesize = (size_t)getpagesize();
    }

  if (size == 0)
    {
      /* Just return the current "break" value: */
      return (mdp->breakval);
    }

  if (size < 0)
    {
      /* We are deallocating memory.  If the amount requested would cause
       * us to try to deallocate back past the base of the mmap'd region
       * then do nothing, and return NULL.  Otherwise, deallocate the
       * memory and return the old break value: */
      if ((mdp->breakval + size) >= mdp->base)
        {
          result = (PTR)mdp->breakval;
          moveto = PAGE_ALIGN(mdp->breakval + size);
#ifdef HAVE_MSYNC
# ifdef MS_SYNC
          msync(moveto, (size_t)(mdp->top - moveto),
                (MS_SYNC | MS_INVALIDATE));
# else
          msync(moveto, (size_t)(mdp->top - moveto));
# endif /* MS_SYNC */
#endif /* HAVE_MSYNC */
          munmap(moveto, (size_t)(mdp->top - moveto));
          mdp->breakval += size;
          mdp->top = moveto;
          return result;
        }
      else
        {
          return NULL;
        }
    }

  /* We are allocating memory.  Make sure we have an open file
   * descriptor and then go on to get the memory: */
#ifndef MAP_ANONYMOUS
  if (mdp->fd < 0)
    {
      char buf[64];
      snprintf(buf, sizeof(buf), "/tmp/mmalloc.XXXXXX");

      mdp->fd = mkstemp(buf);
      if (mdp->fd < 0)
        {
          fprintf(stderr, "unable to create default mmalloc allocator: %s",
                  strerror(errno));
          return NULL;
        }

      if (unlink(buf) != 0)
        {
          fprintf(stderr, "unable to unlink map file for default mmalloc allocator: %s\n",
                  strerror(errno));
        }
    }
#endif /* MAP_ANONYMOUS */

  if ((mdp->breakval + size) <= mdp->top)
    {
      result = (PTR)mdp->breakval;
      mdp->breakval += size;
      return result;
    }

  /* The request would move us past the end of the currently
   * mapped memory, so map in enough more memory to satisfy
   * the request.  This means we also have to grow the mapped-to
   * file by an appropriate amount, since mmap cannot be used
   * to extend a file: */
  moveto = PAGE_ALIGN(mdp->breakval + size);
  mapbytes = (size_t)(moveto - mdp->top);
  foffset = (mdp->top - mdp->base);

#ifdef MAP_ANONYMOUS
  if (mdp->fd < 0)
    {
      flags = (MAP_PRIVATE | MAP_ANONYMOUS);
    }
#endif /* MAP_ANONYMOUS */

  if (mdp->fd > 0)
    {
      /* FIXME:  Test results of lseek() and write() here: */
      lseek(mdp->fd, (foffset + (off_t)mapbytes - 1L), SEEK_SET);
      write(mdp->fd, &buf, (size_t)1UL);

#ifdef MAP_FILE
      flags = (MAP_PRIVATE_OR_SHARED(mdp) | MAP_FILE);
#else
      flags = MAP_PRIVATE_OR_SHARED(mdp)
#endif /* MAP_FILE */
    }

  if (mdp->base == 0)
    {
      /* Let mmap pick the map start address: */
      mapto = (caddr_t)mmap(0, mapbytes, (PROT_READ | PROT_WRITE),
                            flags, mdp->fd, foffset);
      if (mapto != (caddr_t)-1)
        {
          mdp->base = mdp->breakval = mapto;
          mdp->top = (mdp->base + mapbytes);
          result = (PTR)mdp->breakval;
          mdp->breakval += size;
        }
    }
  else
    {
#ifdef __MACH__
      vm_address_t r_start;
      vm_size_t r_size;
      vm_region_basic_info_data_t r_data;
      mach_msg_type_number_t r_info_size;
      port_t r_object_name; /* TODO: find out: where is port_t declared? */
      /* (replaced by mach_port_t in newer versions of OS X) */
      kern_return_t kret;

      r_start = (vm_address_t)mdp->top;
# ifdef VM_REGION_BASIC_INFO_COUNT
      r_info_size = VM_REGION_BASIC_INFO_COUNT;
# else
      r_info_size = 0;
# endif /* VM_REGION_BASIC_INFO_COUNT */
      kret = mach_vm_region(mach_task_self(),
                            (mach_vm_address_t *)&r_start,
                            (mach_vm_address_t *)&r_size,
                            VM_REGION_BASIC_INFO,
                            (vm_region_info_t)&r_data, &r_info_size,
                            &r_object_name);
      if ((kret != KERN_SUCCESS) && (kret != KERN_INVALID_ADDRESS))
	{
	  return NULL;
	}
      if ((kret == KERN_SUCCESS) && (r_start < ((uintptr_t)mdp->top + mapbytes)))
	{
	  return NULL;
	}
#endif /* __MACH__ */

      mapto = (caddr_t)mmap(mdp->top, mapbytes, (PROT_READ | PROT_WRITE),
                            (flags | MAP_FIXED), mdp->fd, foffset);
      if (mapto == mdp->top)
        {
          mdp->top = moveto;
          result = (PTR)mdp->breakval;
          mdp->breakval += size;
        }
      else
	{
	  return NULL;
	}
    }

  return (result);
}

PTR
mmalloc_findbase(size_t size, void *base)
{
#ifdef __MACH__
  vm_address_t last = (vm_address_t)base;

  if (last == 0) {
    last = 0xd0000000;
  }

  for (;;) {
    vm_address_t r_start;
    vm_size_t r_size;
    vm_region_basic_info_data_t r_data;
    mach_msg_type_number_t r_info_size;
    port_t r_object_name;
    kern_return_t kret;

    r_start = last;
#ifdef VM_REGION_BASIC_INFO_COUNT
    r_info_size = VM_REGION_BASIC_INFO_COUNT;
#else
    r_info_size = 0;
#endif /* VM_REGION_BASIC_INFO_COUNT */

    kret = mach_vm_region(mach_task_self(), (mach_vm_address_t *)&r_start,
                          (mach_vm_address_t *)&r_size,
                          VM_REGION_BASIC_INFO, (vm_region_info_t)&r_data,
                          &r_info_size, &r_object_name);
    if ((kret == KERN_INVALID_ADDRESS) && ((last + size) > last))
      {
	return (void *)last;
      }
    if (kret != KERN_SUCCESS)
      {
	return NULL;
      }
    if ((r_start - last) >= size)
      {
	return (void *)last;
      }

    last = (r_start + r_size);
  }
#else
  (void)size;
  (void)base;
  return NULL;
#endif /* __MACH__ */
}

PTR
mmalloc_findbase_hidden(size_t size)
{
  int fd;
  int flags;
  caddr_t base = (caddr_t)NULL;

  if (pagesize == (size_t)0UL)
    {
      pagesize = (size_t)getpagesize();
    }

#ifdef MAP_ANONYMOUS
  flags = (MAP_PRIVATE | MAP_ANONYMOUS);
  fd = -1;
#else
# ifdef MAP_FILE
  flags = (MAP_PRIVATE | MAP_FILE);
# else
  flags = MAP_PRIVATE;
# endif /* MAP_FILE */
#endif /* MAP_ANONYMOUS */

#ifndef MAP_ANONYMOUS
  {
    char buf[64];
    snprintf(buf, sizeof(buf), "/tmp/mmalloc.XXXXXX");

    fd = mkstemp(buf);
    if (fd < 0)
      {
	fprintf(stderr, "unable to create default mmalloc allocator: %s",
                strerror(errno));
	return NULL;
      }

    if (unlink(buf) != 0)
      {
	fprintf(stderr, "unable to unlink map file for default mmalloc allocator: %s",
                strerror(errno));
      }
  }
#endif /* !MAP_ANONYMOUS */

  size = (size_t)PAGE_ALIGN((PTR) size);
  base = (caddr_t)mmap(0, size, (PROT_READ | PROT_WRITE), flags, fd,
                       (off_t)0L);
  if (base != (caddr_t)-1)
    {
#ifdef MS_SYNC
      msync(base, (size_t)size, (MS_SYNC | MS_INVALIDATE));
#else
      msync(base, (size_t)size);
#endif /* MS_SYNC */
      munmap(base, (size_t)size);
    }
  if (fd != -1)
    {
      close(fd);
    }
  if (base == 0)
    {
      /* Do NOT allow mapping at address zero.  We use that value
       * to signal an error return, and besides, it is useful to
       * catch NULL pointers if it is unmapped.  Instead start
       * at the next page boundary: */
      base = (caddr_t)(intptr_t)getpagesize();
    }
  else if (base == (caddr_t)-1)
    {
      base = NULL;
    }
  return ((PTR)base);
}

void mmalloc_endpoints(PTR md, size_t *start, size_t *end)
{
  struct mdesc *mdp = MD_TO_MDP(md);

  if (pagesize == (size_t)0UL)
    {
      pagesize = (size_t)getpagesize();
    }

  if (mdp->child)
    {
      /* returns void anyways, so no point in combining it with the return
       * statement for this function: */
      mmalloc_endpoints(mdp->child, start, end);
      return;
    }

  *start = (size_t)mdp->base;
  *end = (size_t)PAGE_ALIGN(mdp->breakval);
}

#endif	/* defined(HAVE_MMAP) */

/* EOF */
