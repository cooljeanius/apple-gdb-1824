/* macosx-mutils.c
   Mac OS X support for GDB, the GNU debugger.
   Copyright 1997, 1998, 1999, 2000, 2001, 2002
   Free Software Foundation, Inc.

   Contributed by Apple Computer, Inc.

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

#ifdef HAVE_CONFIG_H
# include "config.h"
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning not including "config.h"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
# define MACOSX_MUTILS_C_NON_AUTOTOOLS_BUILD 1
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_MACH_MACH_H
# include <mach/mach.h>
#else
# if defined(__APPLE__) && defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "macosx-mutils.c expects <mach/mach.h> to be included."
# endif /* __APPLE__ && __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_MACH_MACH_H */

#ifdef HAVE_MACH_O_NLIST_H
# include <mach-o/nlist.h>
#else
# if defined(__APPLE__) && defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "macosx-mutils.c expects <mach-o/nlist.h> to be included."
# endif /* __APPLE__ && __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_MACH_O_NLIST_H */

#ifdef HAVE_MACH_MACH_ERROR_H
# include <mach/mach_error.h>
#else
# if defined(__APPLE__) && defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "macosx-mutils.c expects <mach/mach_error.h> to be included."
# endif /* __APPLE__ && __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_MACH_MACH_ERROR_H */

#ifdef HAVE_SYS_SIGNAL_H
# include <sys/signal.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "macosx-mutils.c expects <sys/signal.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_SYS_SIGNAL_H */
#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "macosx-mutils.c expects <sys/types.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_SYS_TYPES_H */
#ifdef HAVE_SYS_WAIT_H
# include <sys/wait.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "macosx-mutils.c expects <sys/wait.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_SYS_WAIT_H */
#ifdef HAVE_LIMITS_H
# include <limits.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "macosx-mutils.c expects <limits.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_LIMITS_H */

#ifdef HAVE_UNISTD_H
# include <unistd.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "macosx-mutils.c expects <unistd.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_UNISTD_H */

#ifdef HAVE_AVAILABILITYMACROS_H
# include <AvailabilityMacros.h>
#else
# if defined(__APPLE__) && defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "macosx-mutils.c expects <AvailabilityMacros.h> to be included."
# endif /* __APPLE__ && __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_AVAILABILITYMACROS_H */

#include "server.h"
#include "macosx-low.h"
#include "macosx-mutils.h"

#ifndef MACH64
# ifdef MAC_OS_X_VERSION_MAX_ALLOWED
#  define MACH64 (MAC_OS_X_VERSION_MAX_ALLOWED >= 1040)
# else
#  if defined(__APPLE__) && defined(__GNUC__) && !defined(__STRICT_ANSI__)
#   warning "need to define MAC_OS_X_VERSION_MAX_ALLOWED to be able to define MACH64"
#  endif /* __APPLE__ && __GNUC__ && !__STRICT_ANSI__ */
# endif /* MAC_OS_X_VERSION_MAX_ALLOWED */
#endif /* !MACH64 */

#if defined(MACH64) && MACH64
# ifdef HAVE_MACH_MACH_VM_H
#  include <mach/mach_vm.h>
#  ifdef HAVE_UNSIGNED_INT
#   ifndef HAVE_VM_SIZE_T
#    ifndef vm_size_t
#     if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#      warning "We do not have the vm_size_t type, which this file needs."
#     endif /* __GNUC__ && !__STRICT_ANSI__ */
/* Try defining it as an "unsigned int". */
#    else
#     define VM_SIZE_T_PRESENT_BUT_NOT_DETECTED_BY_CONFIGURE 1
#    endif /* !vm_size_t */
#   else
#    ifdef vm_size_t
#     if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#      warning "Your pre-existing definition for vm_size_t might conflict with the type that this file expects."
#     endif /* __GNUC__ && !__STRICT_ANSI__ */
#    else
#     define VM_SIZE_T_NOT_PRESENT_BUT_DETECTED_BY_CONFIGURE 1
#    endif /* vm_size_t */
#   endif /* !HAVE_VM_SIZE_T */
#  else
#   ifdef __GDBSERVER_MACOSX_MUTILS_H__
#    if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#     warning "macosx-mutils.h" (which this file includes) will want to use the "unsigned int" type.
#    endif /* __GNUC__ && !__STRICT_ANSI__ */
#   else
#    define MACOSX_MUTILS_H_NOT_INCLUDED 1
#   endif /* __GDBSERVER_MACOSX_MUTILS_H__ */
#  endif /* HAVE_UNSIGNED_INT */
# else
#  if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#   warning "macosx-mutils.c expects <mach/mach_vm.h> to be included."
#  endif /* __GNUC__ && !__STRICT_ANSI__ */
# endif /* HAVE_MACH_MACH_VM_H */
#else /* ! MACH64 */
# define mach_vm_size_t vm_size_t
# define mach_vm_address_t vm_address_t
# define mach_vm_read vm_read
# define mach_vm_write vm_write
# define mach_vm_region vm_region
# define mach_vm_protect vm_protect
# define VM_REGION_BASIC_INFO_COUNT_64 VM_REGION_BASIC_INFO_COUNT
# define VM_REGION_BASIC_INFO_64 VM_REGION_BASIC_INFO
#endif /* MACH64 */

#ifndef MAX_INSTRUCTION_CACHE_WARNINGS
# define MAX_INSTRUCTION_CACHE_WARNINGS 0
#endif /* !MAX_INSTRUCTION_CACHE_WARNINGS */

/* MINUS_INT_MIN is the absolute value of the minimum value that can be stored
 * in a int. We cannot just use -INT_MIN, as that would implicitly be a int,
 * not an unsigned int, and would overflow on 2's complement machines. */

#define MINUS_INT_MIN (((unsigned int)(-(INT_MAX + INT_MIN))) + INT_MAX)

#ifdef DEBUG_MACOSX_MUTILS
static FILE *mutils_stderr = NULL;
static const char* g_macosx_protection_strs [8] =
{ "---", "--r", "-w-", "-wr", "x--", "x-r", "xw-", "xwr" };

static void mutils_debug(const char *fmt, ...)
{
  va_list ap;

  if (mutils_stderr == NULL) {
    mutils_stderr = fdopen(fileno(stderr), "w");
  }

  va_start(ap, fmt);
  fprintf(mutils_stderr, "[%d mutils]: ", getpid());
  vfprintf(mutils_stderr, fmt, ap);
  va_end(ap);
  fflush(mutils_stderr);
}
#endif /* DEBUG_MACOSX_MUTILS */

#define CHECK_FATAL(expression) \
  ((void)((expression) ? (void)0 : fatal("on %s:%d - assert: %s", __FILE__, __LINE__, #expression)))

/* macosx-mutils.h has child_get_pagesize as being of type unsigned int */
vm_size_t child_get_pagesize(void)
{
  kern_return_t status;
  static vm_size_t g_cached_child_page_size = 0;

  if (g_cached_child_page_size == (vm_size_t)-1)
    {
      status = host_page_size(mach_host_self(), &g_cached_child_page_size);
      /* This is probably being over-careful, since if we
         cannot call host_page_size on ourselves, we probably
         are not going to get much further.  */
      if (status != KERN_SUCCESS) {
        g_cached_child_page_size = 0;
      }
      MACH_CHECK_ERROR (status);
    }

  return g_cached_child_page_size;
}

/* Copy LEN bytes to or from inferior's memory starting at MEMADDR
   to debugger memory starting at MYADDR. Copy to inferior if
   WRITE is nonzero.

   Returns the length copied. */

static int
mach_xfer_memory_remainder(CORE_ADDR memaddr, char *myaddr,
                           size_t len, int write, task_t task)
{
  vm_size_t pagesize = child_get_pagesize();

  vm_offset_t mempointer;       /* local copy of inferior's memory */
  mach_msg_type_number_t memcopied;     /* for vm_read to use */

  CORE_ADDR pageaddr = (memaddr - (memaddr % pagesize));

  kern_return_t kret;

  CHECK_FATAL((CORE_ADDR)((memaddr + len - 1UL)
                          - ((memaddr + len - 1UL)
                             % pagesize)) == pageaddr);

  if (!write)
    {
      kret = mach_vm_read (task, pageaddr, pagesize, &mempointer, &memcopied);

      if (kret != KERN_SUCCESS)
	{
#ifdef DEBUG_MACOSX_MUTILS
	  mutils_debug
	    ("Unable to read page for region at 0x%8.8llx with length %lu from inferior: %s (0x%lx)\n",
	     (uint64_t) pageaddr, (unsigned long) len,
	     MACH_ERROR_STRING (kret), kret);
#endif /* DEBUG_MACOSX_MUTILS */
	  return 0;
	}
      if (memcopied != pagesize)
	{
	  kret = vm_deallocate (mach_task_self (), mempointer, memcopied);
	  if (kret != KERN_SUCCESS)
	    {
	      warning
		("Unable to deallocate memory used by failed read from inferior: %s (0x%lx)",
		 MACH_ERROR_STRING (kret), (unsigned long) kret);
	    }
#ifdef DEBUG_MACOSX_MUTILS
	  mutils_debug
	    ("Unable to read region at 0x%8.8llx with length %lu from inferior: "
	     "vm_read returned %lu bytes instead of %lu\n",
	     (uint64_t) pageaddr, (unsigned long) pagesize,
	     (unsigned long) memcopied, (unsigned long) pagesize);
#endif /* DEBUG_MACOSX_MUTILS */
	  return 0;
	}

      memcpy (myaddr, ((unsigned char *) 0) + mempointer
              + (memaddr - pageaddr), len);
      kret = vm_deallocate(mach_task_self(), mempointer, memcopied);
      if (kret != KERN_SUCCESS)
	{
	  warning
	    ("Unable to deallocate memory used to read from inferior: %s (0x%ulx)",
	     MACH_ERROR_STRING(kret), kret);
	  return 0;
	}
    }
  else
    {
      /* We used to read in a whole page, then modify the page
	 contents, then write that page back out. I bet we did that
	 so we did not break up page maps or something like that.
	 However, in Leopard there is a bug in the shared cache
	 implementation, such that if we write into it with whole
	 pages the maximum page protections do NOT get set properly and
	 we can no longer reset the execute bit. In 64 bit Leopard
	 apps, the execute bit has to be set or we cannot run code from
	 there.

	 If we figure out that not writing whole pages causes problems
	 of its own, then we will have to revisit this.  */

#if defined (TARGET_POWERPC)
      vm_machine_attribute_val_t flush = MATTR_VAL_CACHE_FLUSH;
      /* This vm_machine_attribute only works on PPC, so no reason
	 to keep failing on x86... */

      kret = vm_machine_attribute(mach_task_self(), mempointer,
                                  pagesize, MATTR_CACHE, &flush);
# ifdef DEBUG_MACOSX_MUTILS
      if (kret != KERN_SUCCESS)
        {
          mutils_debug
            ("Unable to flush GDB's address space after memcpy prior to vm_write: %s (0x%lx)\n",
             MACH_ERROR_STRING(kret), kret);
        }
# endif /* DEBUG_MACOSX_MUTILS */
#endif /* TARGET_POWERPC */
      kret =
        mach_vm_write(task, memaddr, (pointer_t)myaddr, len);
      if (kret != KERN_SUCCESS)
        {
#ifdef DEBUG_MACOSX_MUTILS
          mutils_debug
            ("Unable to write region at 0x%8.8llx with length %lu to inferior: %s (0x%lx)\n",
             (uint64_t)memaddr, (unsigned long)len,
             MACH_ERROR_STRING(kret), kret);
#endif /* DEBUG_MACOSX_MUTILS */
          return 0;
        }
    }

  return len;
}

static int
mach_xfer_memory_block (CORE_ADDR memaddr, char *myaddr,
                        int len, int write, task_t task)
{
  vm_size_t pagesize = child_get_pagesize ();

  vm_offset_t mempointer;       /* local copy of inferior's memory */
  mach_msg_type_number_t memcopied;     /* for vm_read to use */

  kern_return_t kret;

  CHECK_FATAL ((memaddr % pagesize) == 0);
  CHECK_FATAL ((len % pagesize) == 0);

  if (!write)
    {
      kret =
        mach_vm_read (task, memaddr, len, &mempointer,
                      &memcopied);
      if (kret != KERN_SUCCESS)
        {
#ifdef DEBUG_MACOSX_MUTILS
          mutils_debug
            ("Unable to read region at 0x%8.8llx with length %lu from inferior: %s (0x%lx)\n",
             (uint64_t) memaddr, (unsigned long) len,
             MACH_ERROR_STRING (kret), kret);
#endif /* DEBUG_MACOSX_MUTILS */
          return 0;
        }
      if (memcopied != (mach_msg_type_number_t)len)
        {
          kret = vm_deallocate(mach_task_self(), mempointer, memcopied);
          if (kret != KERN_SUCCESS)
            {
              warning
                ("Unable to deallocate memory used by failed read from inferior: %s (0x%ux)",
                 MACH_ERROR_STRING (kret), kret);
            }
#ifdef DEBUG_MACOSX_MUTILS
          mutils_debug
            ("Unable to read region at 0x%8.8llx with length %lu from inferior: "
             "vm_read returned %lu bytes instead of %lu\n",
             (uint64_t) memaddr, (unsigned long) len,
             (unsigned long) memcopied, (unsigned long) len);
#endif /* DEBUG_MACOSX_MUTILS */
          return 0;
        }
      memcpy (myaddr, ((unsigned char *) 0) + mempointer, len);
      kret = vm_deallocate (mach_task_self (), mempointer, memcopied);
      if (kret != KERN_SUCCESS)
        {
          warning
            ("Unable to deallocate memory used by read from inferior: %s (0x%ulx)",
             MACH_ERROR_STRING (kret), kret);
          return 0;
        }
    }
  else
    {
      kret =
        mach_vm_write (task, memaddr, (pointer_t) myaddr, len);
      if (kret != KERN_SUCCESS)
        {
#ifdef DEBUG_MACOSX_MUTILS
          mutils_debug
            ("Unable to write region at 0x%8.8llx with length %lu from inferior: %s (0x%lx)\n",
             (uint64_t) memaddr, (unsigned long) len,
             MACH_ERROR_STRING (kret), kret);
#endif /* DEBUG_MACOSX_MUTILS */
          return 0;
        }
    }

  return len;
}

#if defined (VM_REGION_SUBMAP_SHORT_INFO_COUNT_64)

/* We probably have a Leopard based build since
   VM_REGION_SUBMAP_SHORT_INFO_COUNT_64 was defined, so we will assign
   the functions to call for MACOSX_GET_REGION_INFO and MACOSX_VM_PROTECT.  */

#define macosx_get_region_info macosx_vm_region_recurse_short
#define macosx_vm_protect macosx_vm_protect_range

#else /* #if defined (VM_REGION_SUBMAP_SHORT_INFO_COUNT_64)  */

/* We need to build Salt on Tiger, but we want to try to run it on Leopard.
 * On Leopard, there is both the SHORT and regular versions of the
 * mach_vm_region_recurse call.  BUT the latter is MUCH slower on Leopard
 * than on Tiger, whereas the short form is pretty much the same speed.
 * Sadly, the defines for the short version are missing on Tiger.
 * So in that case, I supply the defines here, copied over.  Then we try
 * the call, and if we get KERN_INVALID_ARGUMENT, we know that we are on
 * Tiger, and switch over to the long form.  */

  struct vm_region_submap_short_info_64 {
    vm_prot_t		protection;     /* present access protection */
    vm_prot_t		max_protection; /* max avail through vm_prot */
    vm_inherit_t		inheritance;/* behavior of map/obj on fork */
    memory_object_offset_t	offset;		/* offset into object/map */
    unsigned int            user_tag;	/* user tag on map entry */
    unsigned int            ref_count;	 /* obj/map mappers, etc */
    unsigned short          shadow_depth; 	/* only for obj */
    unsigned char           external_pager;  /* only for obj */
    unsigned char           share_mode;	/* see enumeration */
    boolean_t		is_submap;	/* submap vs obj */
    vm_behavior_t		behavior;	/* access behavior hint */
    vm_offset_t		object_id;	/* obj/map name, not a handle */
    unsigned short		user_wired_count;
  };

  typedef struct vm_region_submap_short_info_64	*vm_region_submap_short_info_64_t;
  typedef struct vm_region_submap_short_info_64	 vm_region_submap_short_info_data_64_t;

#define VM_REGION_SUBMAP_SHORT_INFO_COUNT_64 ((mach_msg_type_number_t) \
                                              (sizeof(vm_region_submap_short_info_data_64_t) / sizeof(int)))

/* We probably have a Tiger-based build since
   VM_REGION_SUBMAP_SHORT_INFO_COUNT_64 was not defined, so we will assign
   the functions to call for MACOSX_GET_REGION_INFO and MACOSX_VM_PROTECT.
   Use the get_region_info variant call that tries both the long and short
   and region subrange calls.  */
#define macosx_get_region_info macosx_get_region_info_both

/* Use the vm_protect call that protect by the current region address.  */
#define macosx_vm_protect macosx_vm_protect_region

#endif   /* #else defined (VM_REGION_SUBMAP_SHORT_INFO_COUNT_64)  */

static kern_return_t
macosx_vm_region_recurse_long (task_t task,
			       mach_vm_address_t addr,
			       mach_vm_address_t *r_start,
			       mach_vm_size_t *r_size,
			       vm_prot_t *prot,
			       vm_prot_t *max_prot)
{
  vm_region_submap_info_data_64_t r_long_data;
  mach_msg_type_number_t r_info_size;
  natural_t r_depth;
  kern_return_t kret;

  r_info_size = VM_REGION_SUBMAP_INFO_COUNT_64;
  r_depth = 1000;
  *r_start = addr;

  kret = mach_vm_region_recurse (task,
				 r_start, r_size,
				 & r_depth,
				 (vm_region_recurse_info_t) &r_long_data,
				 &r_info_size);
  if (kret == KERN_SUCCESS)
    {
      *prot = r_long_data.protection;
      *max_prot = r_long_data.max_protection;
#ifdef DEBUG_MACOSX_MUTILS
      mutils_debug ("macosx_vm_region_recurse_long ( 0x%8.8llx ): [ 0x%8.8llx - 0x%8.8llx ) "
		    "depth = %d, prot = %c%c%s max_prot = %c%c%s\n",
		    (uint64_t) addr,
		    (uint64_t) (*r_start),
		    (uint64_t) (*r_start + *r_size),
		    r_depth,
		    *prot & VM_PROT_COPY ? 'c' : '-',
		    *prot & VM_PROT_NO_CHANGE ? '!' : '-',
		    g_macosx_protection_strs[*prot & 7],
		    *max_prot & VM_PROT_COPY ? 'c' : '-',
		    *max_prot & VM_PROT_NO_CHANGE ? '!' : '-',
		    g_macosx_protection_strs[*max_prot & 7]);
#endif /* DEBUG_MACOSX_MUTILS */
    }
  else
    {
#ifdef DEBUG_MACOSX_MUTILS
      mutils_debug ("macosx_vm_region_recurse_long ( 0x%8.8llx ): ERROR %s\n",
		    (uint64_t) addr, MACH_ERROR_STRING (kret));
#endif /* DEBUG_MACOSX_MUTILS */
      *r_start = 0;
      *r_size = 0;
      *prot = VM_PROT_NONE;
      *max_prot = VM_PROT_NONE;
    }

  return kret;
}


static kern_return_t
macosx_vm_region_recurse_short (task_t task,
				mach_vm_address_t addr,
				mach_vm_address_t *r_start,
				mach_vm_size_t *r_size,
				vm_prot_t *prot,
				vm_prot_t *max_prot)
{
  vm_region_submap_short_info_data_64_t r_short_data;
  mach_msg_type_number_t r_info_size;
  natural_t r_depth;
  kern_return_t kret;

  r_info_size = VM_REGION_SUBMAP_SHORT_INFO_COUNT_64;
  r_depth = 1000;
  *r_start = addr;

  kret = mach_vm_region_recurse (task,
				 r_start, r_size,
				 & r_depth,
				 (vm_region_recurse_info_t) &r_short_data,
				 &r_info_size);
  if (kret == KERN_SUCCESS)
    {
      *prot = r_short_data.protection;
      *max_prot = r_short_data.max_protection;
#ifdef DEBUG_MACOSX_MUTILS
      mutils_debug ("macosx_vm_region_recurse_short ( 0x%8.8llx ): [ 0x%8.8llx - 0x%8.8llx ) "
		    "depth = %d, prot = %c%c%s max_prot = %c%c%s\n",
		    (uint64_t) addr,
		    (uint64_t) (*r_start),
		    (uint64_t) (*r_start + *r_size),
		    r_depth,
		    *prot & VM_PROT_COPY ? 'c' : '-',
		    *prot & VM_PROT_NO_CHANGE ? '!' : '-',
		    g_macosx_protection_strs[*prot & 7],
		    *max_prot & VM_PROT_COPY ? 'c' : '-',
		    *max_prot & VM_PROT_NO_CHANGE ? '!' : '-',
		    g_macosx_protection_strs[*max_prot & 7]);
#endif /* DEBUG_MACOSX_MUTILS */
    }
  else
    {
#ifdef DEBUG_MACOSX_MUTILS
      mutils_debug ("macosx_vm_region_recurse_short ( 0x%8.8llx ): ERROR %s\n",
		    (uint64_t) addr, MACH_ERROR_STRING (kret));
#endif /* DEBUG_MACOSX_MUTILS */
      *r_start = 0;
      *r_size = 0;
      *prot = VM_PROT_NONE;
      *max_prot = VM_PROT_NONE;
    }
  return kret;
}




static kern_return_t
macosx_vm_protect_range (task_t task,
			 mach_vm_address_t region_start,
			 mach_vm_size_t region_size,
			 mach_vm_address_t addr,
			 mach_vm_size_t size,
			 vm_prot_t prot,
			 boolean_t set_max)
{
  kern_return_t kret;
  mach_vm_address_t protect_addr;
  mach_vm_size_t protect_size;

  /* On Leopard we want to protect the smallest range possible.  */
  protect_addr = addr;
  protect_size = size;

  kret = mach_vm_protect (task, protect_addr, protect_size, set_max, prot);
#ifdef DEBUG_MACOSX_MUTILS
  mutils_debug ("macosx_vm_protect_range ( 0x%8.8llx ):  [ 0x%8.8llx - 0x%8.8llx ) %s = %c%c%s => %s\n",
		(uint64_t) addr,
		(uint64_t) protect_addr,
		(uint64_t) (protect_addr + protect_size),
		set_max ? "max_prot" : "prot",
		prot & VM_PROT_COPY ? 'c' : '-',
		prot & VM_PROT_NO_CHANGE ? '!' : '-',
		g_macosx_protection_strs[prot & 7],
		kret ? MACH_ERROR_STRING (kret) : "0");
#endif /* DEBUG_MACOSX_MUTILS */
  return kret;
}

static kern_return_t
macosx_vm_protect_region(task_t task, mach_vm_address_t region_start,
			 mach_vm_size_t region_size,
			 mach_vm_address_t addr, mach_vm_size_t size,
			 vm_prot_t prot, boolean_t set_max)
{
  kern_return_t kret;
  mach_vm_address_t protect_addr;
  mach_vm_size_t protect_size;

  /* On Tiger we want to set protections at the region level.  */
  protect_addr = region_start;
  protect_size = region_size;

  kret = mach_vm_protect(task, protect_addr, protect_size, set_max, prot);

#ifdef DEBUG_MACOSX_MUTILS
  mutils_debug("macosx_vm_protect_region ( 0x%8.8llx ):  [ 0x%8.8llx - 0x%8.8llx ) %s = %c%c%s => %s\n",
               (uint64_t)addr, (uint64_t)protect_addr,
               (uint64_t)(protect_addr + protect_size),
               (set_max ? "max_prot" : "prot"),
               ((prot & VM_PROT_COPY) ? 'c' : '-'),
               ((prot & VM_PROT_NO_CHANGE) ? '!' : '-'),
               g_macosx_protection_strs[prot & 7],
               ((kret ? MACH_ERROR_STRING(kret)) : "0"));
#endif /* DEBUG_MACOSX_MUTILS */
  return kret;
}

static kern_return_t
macosx_get_region_info_both(task_t task, mach_vm_address_t addr,
			    mach_vm_address_t *r_start,
                            mach_vm_size_t *r_size, vm_prot_t *prot,
			    vm_prot_t *max_prot)
{
  static int use_short_info = 1;

  kern_return_t kret;

  if (use_short_info)
    {
      kret = macosx_vm_region_recurse_short (task, addr, r_start, r_size,
					     prot, max_prot);

      if (kret == KERN_INVALID_ARGUMENT)
	{
	  use_short_info = 0;
#ifdef DEBUG_MACOSX_MUTILS
	  mutils_debug ("vm_region_submap_short_info not supported, switching"
			" to long info.\n");
#endif /* DEBUG_MACOSX_MUTILS */
	  kret = macosx_vm_region_recurse_long (task, addr, r_start, r_size,
						prot, max_prot);
	}
    }
  else
    {
      kret = macosx_vm_region_recurse_long (task, addr, r_start, r_size,
					    prot, max_prot);
    }

  return kret;
}


int
mach_xfer_memory(CORE_ADDR memaddr, const char *myaddr, int len, int write,
                 task_t task)
{
  mach_vm_address_t r_start = 0;
  mach_vm_address_t r_end = 0;
  mach_vm_size_t r_size = 0;

  vm_prot_t orig_protection = 0;
  vm_prot_t max_orig_protection = 0;

  CORE_ADDR cur_memaddr;
  char *cur_myaddr;
  int cur_len;

  vm_size_t pagesize = child_get_pagesize();
  kern_return_t kret;
  int ret;

  /* check for out-of-range address: */
  r_start = memaddr;
  /* FIXME: did I cast the correct one? */
  if (r_start != (mach_vm_address_t)memaddr)
    {
      errno = EINVAL;
      return 0;
    }

  if (len == 0)
    {
      return 0;
    }

  CHECK_FATAL(myaddr != NULL);
  errno = 0;

  /* check for case where memory available only at address greater than
   * address specified: */
  {
    kret = macosx_get_region_info(task, memaddr, &r_start, &r_size,
                                  &orig_protection, &max_orig_protection);
    if (kret != KERN_SUCCESS)
      {
        return 0;
      }

    if (r_start > (mach_vm_address_t)memaddr)
      {
        if ((r_start - memaddr) <= MINUS_INT_MIN)
          {
#ifdef DEBUG_MACOSX_MUTILS
            mutils_debug("First available address near 0x%8.8llx is at 0x%8.8llx; returning\n",
                         (uint64_t)memaddr, (uint64_t)r_start);
#endif /* DEBUG_MACOSX_MUTILS */
            return (int)(-(r_start - memaddr));
          }
        else
          {
#ifdef DEBUG_MACOSX_MUTILS
            mutils_debug("First available address near 0x%8.8llx is at 0x%8.8llx"
                         "(too far; returning 0)\n",
                         (uint64_t)memaddr, (uint64_t)r_start);
#endif /* DEBUG_MACOSX_MUTILS */
            return 0;
          }
      }
  }

  cur_memaddr = memaddr;
  cur_myaddr = (char *)myaddr;
  cur_len = len;

  while (cur_len > 0)
    {
      int changed_protections = 0;

      /* We want the inner-most map containing our address, so set
	 the recurse depth to some high value, and call mach_vm_region_recurse.  */
      kret = macosx_get_region_info(task, cur_memaddr, &r_start, &r_size,
				    &orig_protection, &max_orig_protection);

      if (r_start > (mach_vm_address_t)cur_memaddr)
        {
#ifdef DEBUG_MACOSX_MUTILS
          mutils_debug
            ("Next available region for address at 0x%8.8llx is 0x%8.8llx\n",
             (uint64_t)cur_memaddr, (uint64_t)r_start);
#endif /* DEBUG_MACOSX_MUTILS */
          break;
        }

      if (write)
        {
	  /* Keep the execute permission if we modify protections: */
	  vm_prot_t new_prot = (VM_PROT_READ | VM_PROT_WRITE);

	  /* Do we need to modify our protections?  */
	  if (orig_protection & VM_PROT_WRITE)
	    {
	      /* We do NOT need to modify our protections.  */
	      kret = KERN_SUCCESS;
#ifdef DEBUG_MACOSX_MUTILS
	      mutils_debug("We already have write access to the region "
                           "containing: 0x%8.8llx, skipping permission modification.\n",
                           (uint64_t)cur_memaddr);
#endif /* DEBUG_MACOSX_MUTILS */
	    }
	  else
	    {
              mach_vm_size_t prot_size;
	      changed_protections = 1;

	      if (cur_len < (int)(r_size - (cur_memaddr - r_start)))
		prot_size = cur_len;
	      else
		prot_size = (cur_memaddr - r_start);

	      kret = macosx_vm_protect(task, r_start, r_size,
                                       cur_memaddr, prot_size, new_prot, 0);

	      if (kret != KERN_SUCCESS)
		{
#ifdef DEBUG_MACOSX_MUTILS
		  mutils_debug("Without COPY failed: %s (0x%lx)\n",
                               MACH_ERROR_STRING(kret), kret);
#endif /* DEBUG_MACOSX_MUTILS */
		  kret = macosx_vm_protect(task, r_start, r_size,
                                           cur_memaddr, prot_size,
                                           (VM_PROT_COPY | new_prot), 0);
		}

	      if (kret != KERN_SUCCESS)
		{
#ifdef DEBUG_MACOSX_MUTILS
		  mutils_debug
		    ("Unable to add write access to region at 0x8.8llx: %s (0x%lx)\n",
		     (uint64_t)r_start, MACH_ERROR_STRING(kret), kret);
#endif /* DEBUG_MACOSX_MUTILS */
		  break;
		}
	    }
        }

      r_end = (r_start + r_size);

      CHECK_FATAL(r_start <= (mach_vm_address_t)cur_memaddr);
      CHECK_FATAL(r_end >= (mach_vm_address_t)cur_memaddr);
      CHECK_FATAL((r_start % pagesize) == 0);
      CHECK_FATAL((r_end % pagesize) == 0);
      CHECK_FATAL(r_end >= (r_start + pagesize));

      if ((cur_memaddr % pagesize) != 0)
        {
          int max_len = (pagesize - (cur_memaddr % pagesize));
          int op_len = cur_len;
          if (op_len > max_len)
            {
              op_len = max_len;
            }
          ret = mach_xfer_memory_remainder(cur_memaddr, cur_myaddr, op_len,
                                           write, task);
        }
      else if (cur_len >= (int)pagesize)
        {
          size_t max_len = (size_t)(r_end - cur_memaddr);
          size_t op_len = cur_len;
          if (op_len > max_len)
            {
              op_len = max_len;
            }
          op_len -= (op_len % pagesize);
          ret = mach_xfer_memory_block(cur_memaddr, cur_myaddr, op_len,
                                       write, task);
        }
      else
        {
          ret = mach_xfer_memory_remainder(cur_memaddr, cur_myaddr, cur_len,
                                           write, task);
        }

      if (write)
        {
	  /* This vm_machine_attribute is NOT supported on i386,
	   * so we will not try: */
#if defined (TARGET_POWERPC)
	  vm_machine_attribute_val_t flush = MATTR_VAL_CACHE_FLUSH;
          kret = vm_machine_attribute (task, r_start, r_size,
                                       MATTR_CACHE, &flush);
          if (kret != KERN_SUCCESS)
            {
              static int nwarn = 0;
              nwarn++;
              if (nwarn <= MAX_INSTRUCTION_CACHE_WARNINGS)
                {
                  warning
                    ("Unable to flush data/instruction cache for region at 0x%8.8llx: %s",
                     (uint64_t) r_start, MACH_ERROR_STRING (ret));
                }
              if (nwarn == MAX_INSTRUCTION_CACHE_WARNINGS)
                {
                  warning
                    ("Support for flushing the data/instruction cache on this "
		     "machine appears broken");
                  warning ("No further warning messages will be given.");
                }
            }
#endif /* TARGET_POWERPC */
	  /* Try and restore permissions on the minimal address range.  */
	  if (changed_protections)
	    {
	      mach_vm_size_t prot_size;
	      if (cur_len < (int)(r_size - (cur_memaddr - r_start)))
		prot_size = cur_len;
	      else
		prot_size = (cur_memaddr - r_start);

	      kret = macosx_vm_protect(task, r_start, r_size,
                                       cur_memaddr, prot_size,
                                       orig_protection, 0);
	      if (kret != KERN_SUCCESS)
		{
		  warning
		    ("Unable to restore original permissions for region at 0x%8.8llx",
		     (uint64_t)r_start);
		}
	    }
        }


      cur_memaddr += ret;
      cur_myaddr += ret;
      cur_len -= ret;

      if (ret == 0)
        {
          break;
        }
    }

  return (len - cur_len);
}


int macosx_thread_valid(task_t task, thread_t thread)
{
  thread_array_t thread_list;
  unsigned int thread_count = 0;
  kern_return_t kret;

  unsigned int found = 0;
  unsigned int i;

  CHECK_FATAL(task != TASK_NULL);

  kret = task_threads (task, &thread_list, &thread_count);
#ifdef DEBUG_MACOSX_MUTILS
  mutils_debug("macosx_thread_valid - task_threads (%d, %p, %d) returned 0x%lx\n", task, &thread_list, thread_count, kret);
#endif /* DEBUG_MACOSX_MUTILS */
  if ((kret == KERN_INVALID_ARGUMENT)
      || (kret == MACH_SEND_INVALID_RIGHT) || (kret == MACH_RCV_INVALID_NAME)) {
      return 0;
  }
  MACH_CHECK_ERROR(kret);

  for ((i = 0); (i < thread_count); i++) {
      if (thread_list[i] == thread) {
          found = 1;
      }
  }

  kret = vm_deallocate(mach_task_self(), (vm_address_t)thread_list,
					   (vm_size_t)(thread_count * sizeof(thread_t)));
  MACH_CHECK_ERROR(kret);

#ifdef DEBUG_MACOSX_MUTILS
  if (!found) {
      mutils_debug("thread 0x%lx no longer valid for task 0x%lx\n",
                   (unsigned long)thread, (unsigned long)task);
  }
#endif /* DEBUG_MACOSX_MUTILS */
  return found;
}


/* Silence warnings from '-Wunused-macros': */
#ifdef VM_SIZE_T_NOT_PRESENT_BUT_DETECTED_BY_CONFIGURE
# undef VM_SIZE_T_NOT_PRESENT_BUT_DETECTED_BY_CONFIGURE
#endif /* VM_SIZE_T_NOT_PRESENT_BUT_DETECTED_BY_CONFIGURE */
#ifdef MAX_INSTRUCTION_CACHE_WARNINGS
# undef MAX_INSTRUCTION_CACHE_WARNINGS
#endif /* MAX_INSTRUCTION_CACHE_WARNINGS */

/* EOF */
