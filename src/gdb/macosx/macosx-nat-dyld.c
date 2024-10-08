/* macosx/macosx-nat-dyld.c: Mac OS X support for GDB, the GNU debugger.
   Copyright 1997, 1998, 1999, 2000, 2001, 2002, 2004, 2005
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

#if !defined(NO_POISON) && defined(POISON_FREE_TOO)
/* included files can drag in "free": */
# define NO_POISON 1
#endif /* !NO_POISON && POISON_FREE_TOO */

#include "defs.h"

#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <mach/mach_types.h>
#include <mach/vm_types.h>
#include <mach/vm_region.h>
#include <mach/machine/vm_param.h>
#include <mach-o/loader.h>
#include <mach/task_info.h>
#include <stdio.h>
#include <sys/time.h>

#include "inferior.h"
#include "target.h"
#include "gdbcmd.h"
#include "annotate.h"
#include "mach-o.h"
#include "gdbcore.h"            /* for core_ops */
#include "symfile.h"
#include "objfiles.h"
#include "gdb_assert.h"
#include "gdb_stat.h"
#include "regcache.h"
#include "bfd.h"
#include "gdb-stabs.h"
#include "objc-lang.h"
#include "gdbarch.h"
#include "arch-utils.h"
#include "ui-out.h"
#include "osabi.h"
#include "exceptions.h"
#include "remote.h"

#ifdef USE_MMALLOC
# include <mmalloc.h>
#endif /* USE_MMALLOC */

#include "macosx-nat-dyld.h"
#include "macosx-nat-dyld-path.h"
#include "macosx-nat-inferior.h"
#include "macosx-nat-inferior-debug.h"
#include "macosx-nat-mutils.h"
/* we already included "macosx-nat-dyld.h" once above */
#include "macosx-nat-dyld-io.h"
#include "macosx-nat-dyld-info.h"
/* we already included "macosx-nat-dyld-path.h" once above */
#include "macosx-nat-dyld-process.h"

#include "macosx-tdep.h"

#if defined(HAVE_AVAILABILITY_H) || (defined(__APPLE__) && defined(NM_NEXTSTEP))
# include <Availability.h>
#else
# ifndef __MAC_10_5
#  define __MAC_10_5      1050
# endif /* !__MAC_10_5 */
#endif /* HAVE_AVAILABILITY_H || (__APPLE__ && NM_NEXTSTEP) */
#include <AvailabilityMacros.h>
#ifndef MAC_OS_X_VERSION_MIN_REQUIRED
# define MAC_OS_X_VERSION_MIN_REQUIRED 1010
#endif /* !MAC_OS_X_VERSION_MIN_REQUIRED */

#include <mach/mach_vm.h>

#include <libintl.h>

/* For the gdbarch_tdep structure so we can get the wordsize. */
#if defined(TARGET_POWERPC)
# include "ppc-tdep.h"
#elif defined(TARGET_I386)
# include "target/amd64-tdep.h"
# include "target/i386-tdep.h"
#elif defined(TARGET_ARM)
# include "arm-tdep.h"
# if !defined(_TM_NEXTSTEP_H_)
#  include "tm-macosx.h"
# endif /* !_TM_NEXTSTEP_H_ */
#elif defined(TARGET_AARCH64)
# include "aarch64-tdep.h"
#else
# ifndef S_SPLINT_S
#  error "Unrecognized target architecture."
# endif /* !S_SPLINT_S */
#endif /* TARGET_foo */

#if WITH_CFM
# include "macosx-nat-cfm.h"
#endif /* WITH_CFM */

#ifndef INVALID_ADDRESS
# define INVALID_ADDRESS ((CORE_ADDR)(-1L))
#endif /* !INVALID_ADDRESS */

#if defined(NM_NEXTSTEP)
/* MACOSX_STATUS is only defined for native builds, not cross builds.  */
/* (so then why does the ARM cross build complain about this file missing
 * the _macosx_status symbol?) */
extern macosx_inferior_status *macosx_status;
#else
# define MACOSX_NAT_DYLD_C_NOT_NATIVE 1
#endif /* NM_NEXTSTEP */

extern struct target_ops exec_ops;

/* The dyld status is now in a separate structure to support remote
   debugging using gdbserver.  */
macosx_dyld_thread_status macosx_dyld_status;
static struct dyld_objfile_entry *macosx_solib_add_dyld_objfile_entries = NULL;
static int num_macosx_solib_add_dyld_objfile_entries = 0;

int dyld_preload_libraries_flag = 1;
int dyld_filter_events_flag = 1;
int dyld_always_read_from_memory_flag = 0;
const char *dyld_symbols_prefix = "__dyld_";
int dyld_load_dyld_symbols_flag = 1;
int dyld_load_dyld_shlib_symbols_flag = 1;
int dyld_load_cfm_shlib_symbols_flag = 1;
int dyld_print_basenames_flag = 0;
int dyld_reload_on_downgrade_flag = 0;
static int pre_slide_libraries_flag = 1;
char *dyld_load_rules = NULL;
char *dyld_minimal_load_rules = NULL;
static char *shlib_path_subst_cmd_args = NULL;
static char **shlib_path_substitutions = NULL;

FILE *dyld_stderr = NULL;
int dyld_debug_flag = 0;

/* Do NOT enclose this in a #if WITH_CFM block because Xcode tries
   to set this even on x86 systems and we do NOT want to give them
   an error from that command.  */
int inferior_auto_start_cfm_flag = 0;

int inferior_auto_start_dyld_flag = 1;

int dyld_stop_on_shlibs_added = 1;
int dyld_stop_on_shlibs_updated = 1;
int dyld_combine_shlibs_added = 1;

/* This is the function that libSystem calls to tell dyld that
   the malloc system has been initialized.
   This function name can be mangled in one of two ways. We must check
   for both.  */

const char *malloc_inited_callback = "_Z21registerThreadHelpersPKN4dyld16LibSystemHelpersE";
const char *malloc_inited_callback_alt = "_ZL21registerThreadHelpersPKN4dyld16LibSystemHelpersE";

/* A structure filled in by dyld in the inferior process.
   There is only one of these in the inferior.
   This is an internal representation of the 'struct dyld_all_image_infos'
   defined in <mach-o/dyld_images.h> */

struct dyld_raw_infos
{
  uint32_t version;       /* MacOS X 10.4 == 1; 2 == after Mac OS X 10.5 */
  uint32_t num_info;        /* Number of elements in the following array */

  /* Array of images (struct dyld_raw_info here in gdb) that are loaded
     in the inferior process.
     Note that this address may change over the lifetime of a process;
     as the array grows, dyld may need to realloc() the array. So do NOT
     cache the value of info_array except while the inferior is stopped.
     This is either 4 or 8 bytes in the inferior, depending on wordsize.
     This value can be 0 (NULL) if dyld is in the middle of updating the
     array. Currently, we will just fail in that (unlikely) circumstance. */

  CORE_ADDR info_array;

  /* Function called by dyld after a new dylib/bundle (or group of
     dylib/bundles) has been loaded, but before those images have had
     their initializer functions run.  This function has a prototype of

     void dyld_image_notifier (enum dyld_image_mode mode, uint32_t infoCount,
     const struct dyld_image_info info[]);

     Where mode is either dyld_image_adding (0) or dyld_image_removing (1).
     This is either 4 or 8 bytes in the inferior, depending on wordsize. */

  CORE_ADDR dyld_notify;

  int process_detached_from_shared_region;

  /* The following fields are only present when version is higher than 1.  */

  /* This field indicates that the malloc library in libSystem is ready
     to use.  It is set by dyld.  */
  int libsystem_initialized;

  /* The actual load address of dyld in memory (the Mach-O header is here) */
  CORE_ADDR dyld_actual_load_address;

  /* The address where dyld intended to load (its __TEXT seg addr in the
     exectutable file on disk/in memory) */
  CORE_ADDR dyld_intended_load_address;
};

/* A structure filled in by dyld in the inferior process.
   Each dylib/bundle loaded has one of these structures allocated
   for it.
   Each field is either 4 or 8 bytes, depending on the wordsize of
   the inferior process.  (including the modtime field - size_t goes to
   64 bits in the 64 bit ABIs).  */

struct dyld_raw_info
{
  CORE_ADDR addr;               /* struct mach_header *imageLoadAddress */
  CORE_ADDR name;               /* const char *imageFilePath */
  uint64_t modtime;             /* time_t imageFileModDate */
};

static void info_sharedlibrary_address(CORE_ADDR);
static void set_load_state_1(struct dyld_objfile_entry *e,
                             const struct dyld_path_info *d,
                             int index, int load_state);

static void dyld_read_raw_infos(CORE_ADDR addr,
                                struct dyld_raw_infos *info);

static void dyld_info_read_raw(struct macosx_dyld_thread_status *status,
                               struct dyld_raw_info **rinfo,
                               int *rninfo);

static void dyld_info_read_raw_data (CORE_ADDR addr, int num,
                                     struct dyld_raw_info *rinfo);

static int dyld_starts_here_p(mach_vm_address_t addr);

static int dyld_info_process_raw(struct macosx_dyld_thread_status *status,
				 struct dyld_objfile_entry *entry,
                                 CORE_ADDR name, uint64_t modtime,
                                 CORE_ADDR header_addr);

static void macosx_set_auto_start_dyld(const char *args, int from_tty,
                                       struct cmd_list_element *c);

static int target_read_dylib_command(CORE_ADDR addr,
				     struct dylib_command *dcmd);

static CORE_ADDR find_text_segment_load_address(CORE_ADDR mem_addr);

extern void _initialize_macosx_nat_dyld(void);

void
dyld_debug(const char *fmt, ...)
{
  va_list ap;
  if (dyld_debug_flag >= 1)
    {
      va_start(ap, fmt);
      fprintf(dyld_stderr, "[%d dyld]: ", (int)getpid());
      vfprintf(dyld_stderr, fmt, ap);
      va_end(ap);
      fflush(dyld_stderr);
    }
}

/* */
void
dyld_print_status_info(struct macosx_dyld_thread_status *s,
                       unsigned int mask, const char *args)
{
  switch (s->state)
    {
    case dyld_clear:
      ui_out_text(uiout,
               "The DYLD shared library state has not yet been initialized.\n");
      break;
    case dyld_initialized:
      ui_out_text(uiout,
         "The DYLD shared library state has been initialized from the "
         "executable's shared library information.  All symbols should be "
         "present, but the addresses of some symbols may move when the program "
         "is executed, as DYLD may relocate library load addresses if "
         "necessary.\n");
      break;
    case dyld_started:
      ui_out_text(uiout,
                  "DYLD shared library information has been read from "
                  "the DYLD debugging thread.\n");
      break;
    default:
      internal_error(__FILE__, __LINE__, "invalid value for s->dyld_state");
      gdb_unreachable();
    }

  dyld_print_shlib_info(&s->current_info, mask, 1, args);
}

/* */
void
macosx_clear_start_breakpoint(void)
{
  struct macosx_dyld_thread_status *status = &macosx_dyld_status;
  if (status && status->dyld_breakpoint)
    {
      delete_breakpoint(status->dyld_breakpoint);
      status->dyld_breakpoint = NULL;
    }
  if (status && status->malloc_inited_breakpoint)
    {
      delete_breakpoint(status->malloc_inited_breakpoint);
      status->malloc_inited_breakpoint = NULL;
    }
  remove_solib_event_breakpoints();
}

/* Check if we are debugging via gdb remote protocol (Rosetta debugging,
   gdbserver debugging, remote nub debugging) - in which case we cannot
   do simple mach vm mapping calls on the inferior process.  */

int
target_is_remote(void)
{
  if ((strstr(current_target.to_shortname, "remote") == NULL)
      && (strstr(current_target.to_shortname, "async") == NULL))
    return 0;
  if (strcmp(current_target.to_shortname, "remote"))
    return 1;
  if (strcmp(current_target.to_shortname, "remote-mobile"))
    return 1;
  if (strcmp(current_target.to_shortname, "remote-macosx"))
    return 1;
  if (strcmp(current_target.to_shortname, "extended-remote"))
    return 1;
  if (strcmp(current_target.to_shortname, "extended-async"))
    return 1;
  if (strcmp(current_target.to_shortname, "async"))
    return 1;

  return 0;
}

int
target_is_kdp_remote(void)
{
  if (strcmp(current_target.to_shortname, "remote-kdp") == 0)
    return 1;
  return 0;
}


static CORE_ADDR
lookup_dyld_address(macosx_dyld_thread_status *status, const char *s)
{
  struct minimal_symbol *msym = NULL;
  CORE_ADDR sym_addr;
  char *ns = NULL;

  xasprintf(&ns, "%s%s", dyld_symbols_prefix, s);
  msym = lookup_minimal_symbol(ns, NULL, NULL);
  xfree(ns);

  if (msym == NULL)
    return INVALID_ADDRESS;

  sym_addr = SYMBOL_VALUE_ADDRESS(msym);

  /* The minsyms normally have the correct, relocated address of the symbol.
     dyld is special because we may have loaded the objfile without yet knowing
     its actual slide value.  We will unload & reload it in a bit but until then
     we need to adjust any minsyms manually by dyld_slide.
     This comes up in debugging translated apps when the ppc dyld is always
     slid from its declared load addr.  */

  if ((status->dyld_slide != INVALID_ADDRESS)
      && (status->dyld_minsyms_have_been_relocated == 0))
    {
      sym_addr += status->dyld_slide;
    }

  return sym_addr;
}

/* Find the dyld_all_image_infos structure in the inferior's dyld.
   This is the top-level data structure that we use to find all the
   loaded dylibs in the inferior and the hook to get notifications
   about new ones.  */

void
macosx_init_addresses(macosx_dyld_thread_status *s)
{
  struct dyld_raw_infos infos;

  struct objfile *dyld_objfile = find_objfile_by_name("dyld", 0);

  if (dyld_objfile && dyld_objfile->obfd)
    {
      if (gdbarch_lookup_osabi_from_bfd(dyld_objfile->obfd)
          != osabi_seen_in_attached_dyld)
	{
	  const char *osabi_name = gdbarch_osabi_name(osabi_seen_in_attached_dyld);
	  set_osabi_option(osabi_name);
	  reread_symbols_for_objfile(dyld_objfile, 0,
				     osabi_seen_in_attached_dyld, NULL);
	}
    }

  /* Check if we have the all image infos address already from a call to
     task_info on the host or remote target.  */
  if (s->dyld_image_infos == INVALID_ADDRESS)
    {
      s->dyld_image_infos = lookup_dyld_address(s, "dyld_all_image_infos");

      if (s->dyld_image_infos == INVALID_ADDRESS)
        return;
    }

  dyld_read_raw_infos(s->dyld_image_infos, &infos);

  s->libsystem_initialized = infos.libsystem_initialized;
  macosx_set_malloc_inited(s->libsystem_initialized);

  s->dyld_notify = infos.dyld_notify;
  s->dyld_slide = (infos.dyld_actual_load_address
                   - infos.dyld_intended_load_address);
}

static void
macosx_init_dyld_cache_ranges(macosx_dyld_thread_status *s)
{
  /* If we are looking at an uninitialized dyld, do NOT try to read
     anything yet.  We'll apply an invalid slide to the location
     of the dyld_shared_cache_ranges and read random memory.  */
  if (s->dyld_addr == INVALID_ADDRESS)
    return;

  s->dyld_shared_cache_ranges =
    lookup_dyld_address(s, "dyld_shared_cache_ranges");
  if (s->dyld_shared_cache_ranges != INVALID_ADDRESS)
    {
      int i;
      int wordsize = new_gdbarch_tdep(current_gdbarch)->wordsize;
      gdb_byte *buf = (gdb_byte *)alloca(2UL * wordsize);
      CORE_ADDR addr = s->dyld_shared_cache_ranges;

      if (target_read_memory(addr, buf, wordsize) == 0)
        s->dyld_num_shared_cache_ranges =
          extract_unsigned_integer(buf, wordsize);
      else
        s->dyld_num_shared_cache_ranges = -1;

      if (s->dyld_num_shared_cache_ranges > 0)
	{
	  addr += wordsize;

	  s->dyld_shared_cache_array =
            ((struct dyld_cache_range *)
             xmalloc(s->dyld_num_shared_cache_ranges
                     * sizeof(struct dyld_cache_range)));

	  for (i = 0; i < s->dyld_num_shared_cache_ranges; i++)
	    {
	      if (target_read_memory(addr, buf, (2 * wordsize)) != 0)
                {
                  warning("Error while reading dyld shared cache region "
                          "%d of %d\n", i, s->dyld_num_shared_cache_ranges);
                  s->dyld_num_shared_cache_ranges = 0;
                  xfree(s->dyld_shared_cache_array);
                  s->dyld_shared_cache_array = NULL;
                  return;
                }
	      addr += (2 * wordsize);

	      s->dyld_shared_cache_array[i].start =
                extract_unsigned_integer(buf, wordsize);
	      s->dyld_shared_cache_array[i].length =
                extract_unsigned_integer((buf + wordsize), wordsize);
	    }
	}
    }
}


#define EXTRACT_INT_MEMBER(type, struct_ptr, member, member_type) \
  struct_ptr->member = \
    (member_type)extract_unsigned_integer(((gdb_byte *)struct_ptr \
                                           + offsetof(type, member)), \
                                          sizeof(struct_ptr->member))

#define EXTRACT_INT_MEMBER_FROM_BUF(buf, type, struct_ptr, member, member_type) \
  struct_ptr->member = \
    (member_type)extract_unsigned_integer(((gdb_byte *)buf \
                                           + offsetof(type, member)), \
                                          sizeof(struct_ptr->member))

/* This is a little confusing.  We only use target_read_mach_header
   when we are reading from memory, and need to figure out how big the
   whole header is so we can mmap it in, or do some very minimal
   parsing (like find the UUID).  We generally make a bfd out of it,
   and use the bfd to actually parse the header.

   As of SL, there is a mach_header struct which is the header for 32
   bit programs, and a mach_header_64 which is used by 64 bit
   programs.  The 32 bit & 64 bit headers are laid out the same way as
   far as the 32 bit one extends, and we do NOT care about the extra
   field in the 64 bit version. So for now, we shall just read in a
   mach_header in both cases.

   But then when you want to compute the header size, you need to use
   target_get_mach_header_size.  */

int
target_read_mach_header(CORE_ADDR addr, struct mach_header *s)
{
  int error;
  gdb_assert(addr != INVALID_ADDRESS);
  error = target_read_memory(addr, (gdb_byte *)s, sizeof(*s));

  if (error == 0)
    {
      EXTRACT_INT_MEMBER(struct mach_header, s, magic, uint32_t);
      EXTRACT_INT_MEMBER(struct mach_header, s, cputype, cpu_type_t);
      EXTRACT_INT_MEMBER(struct mach_header, s, cpusubtype, cpu_subtype_t);
      EXTRACT_INT_MEMBER(struct mach_header, s, filetype, uint32_t);
      EXTRACT_INT_MEMBER(struct mach_header, s, ncmds, uint32_t);
      EXTRACT_INT_MEMBER(struct mach_header, s, sizeofcmds, uint32_t);
      EXTRACT_INT_MEMBER(struct mach_header, s, flags, uint32_t);
    }
  return error;
}

int
target_read_minimal_segment_32(CORE_ADDR addr, struct segment_command *s)
{
  int error;
  gdb_assert(addr != INVALID_ADDRESS);
  /* Read only up to the file offset as we only need the info up to the
   * vmaddr and vmsize: */
  memset(s, 0, sizeof(*s));
  error = target_read_memory(addr, (gdb_byte *)s,
                             offsetof(struct segment_command, fileoff));

  if (error == 0)
    {
      EXTRACT_INT_MEMBER(struct segment_command, s, cmd, uint32_t);
      EXTRACT_INT_MEMBER(struct segment_command, s, cmdsize, uint32_t);
      EXTRACT_INT_MEMBER(struct segment_command, s, vmaddr, uint32_t);
      EXTRACT_INT_MEMBER(struct segment_command, s, vmsize, uint32_t);
    }
  return error;
}

/* FIXME: needs comment */
int
target_read_minimal_segment_64(CORE_ADDR addr, struct segment_command_64 *s)
{
  int error;
  gdb_assert(addr != INVALID_ADDRESS);
  /* Read only up to the file offset as we only need the info up to the
   * vmaddr and vmsize: */
  memset(s, 0, sizeof(*s));
  error = target_read_memory(addr, (gdb_byte *)s,
                             offsetof(struct segment_command_64, fileoff));

  if (error == 0)
    {
      EXTRACT_INT_MEMBER(struct segment_command_64, s, cmd, uint32_t);
      EXTRACT_INT_MEMBER(struct segment_command_64, s, cmdsize, uint32_t);
      EXTRACT_INT_MEMBER(struct segment_command_64, s, vmaddr, uint64_t);
      EXTRACT_INT_MEMBER(struct segment_command_64, s, vmsize, uint64_t);
    }
  return error;
}

int
target_get_mach_header_size(struct mach_header *header)
{
  if (header->magic == 0xfeedfacf)
    return sizeof(struct mach_header_64);
  else
    return sizeof(struct mach_header);
}

int
target_read_load_command(CORE_ADDR addr, struct load_command *s)
{
  int error;
  gdb_assert(addr != INVALID_ADDRESS);
  error = target_read_memory(addr, (gdb_byte *)s, sizeof(*s));

  if (error == 0)
    {
      EXTRACT_INT_MEMBER(struct load_command, s, cmd, uint32_t);
      EXTRACT_INT_MEMBER(struct load_command, s, cmdsize, uint32_t);
    }
  return error;
}

int
target_read_uuid(CORE_ADDR addr, unsigned char *uuid)
{
  int error;
  struct uuid_command s;
  gdb_assert(addr != INVALID_ADDRESS);
  error = target_read_memory(addr, (gdb_byte *)&s, sizeof(s));

  if (error == 0)
    {
      memcpy(uuid, s.uuid, 16);
    }
  return error;
}

int
target_read_dylib_command(CORE_ADDR addr, struct dylib_command *s)
{
  int error;
  gdb_assert(addr != INVALID_ADDRESS);
  error = target_read_memory(addr, (gdb_byte *)s, sizeof(*s));

  if (error == 0)
    {
      EXTRACT_INT_MEMBER(struct dylib_command, s, cmd, uint32_t);
      EXTRACT_INT_MEMBER(struct dylib_command, s, cmdsize, uint32_t);
      EXTRACT_INT_MEMBER(struct dylib_command, s, dylib.name.offset,
                         uint32_t);
      EXTRACT_INT_MEMBER(struct dylib_command, s, dylib.timestamp,
                         uint32_t);
      EXTRACT_INT_MEMBER(struct dylib_command, s, dylib.current_version,
                         uint32_t);
      EXTRACT_INT_MEMBER(struct dylib_command, s,
                         dylib.compatibility_version, uint32_t);
    }
  return error;
}

static int
dyld_starts_here_in_memory(CORE_ADDR addr)
{
  struct mach_header mh;
  if (target_read_mach_header(addr, &mh) == 0)
    return (((mh.magic == MH_MAGIC) || (mh.magic == MH_MAGIC_64))
            && (mh.filetype == MH_DYLINKER));
  return 0;
}


static int
dyld_starts_here_p(mach_vm_address_t addr)
{
  /* keep this condition the same as the one where they are used below: */
#if defined(NM_NEXTSTEP)
  mach_vm_address_t address;
  mach_vm_size_t size;
  vm_region_basic_info_data_64_t info;
  mach_msg_type_number_t info_cnt;
  kern_return_t ret;
  mach_port_t object_name;
  vm_offset_t data;
  mach_msg_type_number_t data_count;

  struct mach_header *mh;
#endif /* NM_NEXTSTEP */

  /* If we are using the remote protocol, we will NOT be able to do
      mach vm region calls unless we add something special to gdbserver.
      We will try just reading memory  */
  if (target_is_remote())
    return dyld_starts_here_in_memory((CORE_ADDR)addr);

#if defined(NM_NEXTSTEP)
  address = addr;
  size = 0;
  info_cnt = sizeof(vm_region_basic_info_data_64_t);

  info_cnt = VM_REGION_BASIC_INFO_COUNT_64;
  ret = mach_vm_region(macosx_status->task, &address, &size,
                       VM_REGION_BASIC_INFO_64, (vm_region_info_t)&info,
                       &info_cnt, &object_name);

  if (ret != KERN_SUCCESS)
    return 0;

  /* If it is not readable, then it is not dyld: */
  if ((info.protection & VM_PROT_READ) == 0)
    return 0;

  ret = mach_vm_read(macosx_status->task, address, size, &data, &data_count);

  if (ret != KERN_SUCCESS)
    {
      /* Do NOT vm_deallocate the memory here, you did NOT successfully get
         it, and deallocating it will cause a crash. */
      return 0;
    }

  /* If the vm region is too small to contain a mach_header, it also cannot be
     where dyld is loaded */

  if (data_count < sizeof(struct mach_header))
    {
      ret = vm_deallocate(mach_task_self(), data, data_count);
      if (ret != KERN_SUCCESS) {
	warning(_("possible issue with vm_deallocate()"));
      }
      return 0;
    }

  mh = (struct mach_header *)data;

  /* If the magic number is right and the size of this region is big
     enough to cover the mach header and load commands, assume it is
     correct. */
  if (((mh->magic != MH_MAGIC) && (mh->magic != MH_MAGIC_64)) ||
      (mh->filetype != MH_DYLINKER) ||
      (data_count < (sizeof(struct mach_header) + mh->sizeofcmds)))
    {
      ret = vm_deallocate(mach_task_self(), data, data_count);
      if (ret != KERN_SUCCESS) {
	warning(_("possible issue with vm_deallocate()"));
      }
      return 0;
    }

  /* Looks like dyld, smells like dyld -- must be dyld! */

  /* This is the first chance we have to detect whether the inferior is
     a 32- or 64-bit process on ppc so we need to set the osabi here.
     On x86 systems fetch_inferior_registers() will have already set the
     osabi because the results of thread_get_state include a self-describing
     flavor field. We have no such field on ppc systems - thread_get_state
     gives us the full 64 bits of registers regardless of the inferior's
     ABI.  */

# ifdef TARGET_POWERPC
  if (mh->magic == MH_MAGIC_64)
    {
      struct gdbarch_info info;
      gdbarch_info_init(&info);
      gdbarch_info_fill(current_gdbarch, &info);
      info.byte_order = gdbarch_byte_order(current_gdbarch);
      info.osabi = GDB_OSABI_DARWIN64;
      info.bfd_arch_info = bfd_lookup_arch(bfd_arch_powerpc, bfd_mach_ppc64);
      gdbarch_update_p(info);
    }
# endif /* TARGET_POWERPC */

  /* This is pretty crude, but here is where we first figure out the
     architecture of the dyld we are attaching to. So if we have been
     given an executable file, and it is of the wrong architecture,
     then we just bag out.  */

  if (mh->magic == MH_MAGIC_64)
    {
      if (exec_bfd
	  && gdbarch_lookup_osabi_from_bfd(exec_bfd) != GDB_OSABI_DARWIN64)
	  {
	    error("The exec file is 32 bits but the attach target is 64 bits.\n"
	      "Quit gdb & restart, using \"--arch\" to select the 64 bit fork of the executable.");
          }
    }
  else if (mh->magic == MH_MAGIC)
    {
      if (exec_bfd
	  && gdbarch_lookup_osabi_from_bfd(exec_bfd) != GDB_OSABI_DARWIN
	  && gdbarch_lookup_osabi_from_bfd(exec_bfd) != GDB_OSABI_DARWINV6
	  && gdbarch_lookup_osabi_from_bfd(exec_bfd) != GDB_OSABI_DARWINV7
	  && gdbarch_lookup_osabi_from_bfd(exec_bfd) != GDB_OSABI_DARWINV7F
	  && gdbarch_lookup_osabi_from_bfd(exec_bfd) != GDB_OSABI_DARWINV7K
	  && gdbarch_lookup_osabi_from_bfd(exec_bfd) != GDB_OSABI_DARWINV7S)
	  {
	    error("The exec file is 64 bits but the attach target is 32 bits.\n"
	      "Quit gdb & restart, using \"--arch\" to select the 32 bit fork of the executable.");
	  }
    }

  ret = vm_deallocate(mach_task_self(), data, data_count);

  if (ret != KERN_SUCCESS) {
    warning(_("possible issue with vm_deallocate()"));
  }

  return 1;
#else
  return 0;
#endif /* NM_NEXTSTEP */
}

/* Tries to find the name string for the dynamic linker passed as
   ABFD.  Returns 1 if it found a name (and returns xmalloc'ed name in *RNAME).
   Return 0 if no name was found; -1 if there was an error. */

static int
macosx_lookup_dyld_name(bfd *abfd, const char **rname)
{
  struct mach_o_data_struct *mdata = NULL;
  char *name = NULL;
  char buf[PATH_MAX];
  unsigned int i;

  if (abfd == NULL)
    return -1;
  if (!bfd_mach_o_valid(abfd))
    return -1;

  mdata = abfd->tdata.mach_o_data;

  CHECK_FATAL(mdata != NULL);
  CHECK_FATAL(rname != NULL);

  for (i = 0U; i < mdata->header.ncmds; i++)
    {
      struct bfd_mach_o_load_command *cmd = &mdata->commands[i];

      if (cmd->type == BFD_MACH_O_LC_LOAD_DYLINKER)
        {
          bfd_mach_o_dylinker_command *dcmd = &cmd->command.dylinker;

          name = (char *)xmalloc(dcmd->name_len + 1);

          bfd_seek(abfd, dcmd->name_offset, SEEK_SET);
          if (bfd_bread(name, dcmd->name_len, abfd) != dcmd->name_len)
            {
              warning("Unable to find library name for LC_LOAD_DYLINKER "
                      "or LD_ID_DYLINKER command; ignoring");
              xfree(name);
              name = NULL;
              continue;
            }
          else
            {
	      char *fixed_name = dyld_fix_path (name);
              xfree (name);
              name = fixed_name;
              break;
            }
        }
    }

  /* The DYLINKER is often set to a soft-link when the developer
     is the person working on dyld -- they do NOT want to splat a debug
     version of dyld into /usr/lib/dyld and hose their system. So we will
     need to realpath to get to the actual name of the dyld binary for
     consistency.  */

  if ((name != NULL) && (realpath(name, buf) != NULL))
    {
      xfree(name);
      name = xstrdup(buf);
    }

  *rname = name;
  return ((name == NULL) ? 0 : 1);
}

/* Find the /usr/lib/dyld objfile and set LOADADDR to be its intended
   (on-disk) load address. */

static int
macosx_lookup_dyld_file_load_address(const char *name, CORE_ADDR *loadaddr)
{
  struct objfile *o;
  if ((name == NULL) || (name[0] == '\0') || (loadaddr == NULL))
    return 0;
  o = find_objfile_by_name(name, 1);
  if (o == NULL)
    return 0;

  *loadaddr = o->sections[o->sect_index_text].the_bfd_section->vma;

  return 1;
}

/* NAME is the on-disk filename of dyld, e.g. /usr/lib/dyld
   VALUE is set to the load address of dyld if we can find it.
   SLIDE is set to the amount dyld was slid from its on-disk addr.
   1 is returned if we found a dyld.
   If 0 is returned, S->DYLD_ADDR and S->DYLD_SLIDE are not set to anything.  */

static int
macosx_locate_dyld_via_taskinfo(macosx_dyld_thread_status *s)
{
  if (s == NULL)
    return 0;


  if (s->dyld_image_infos == INVALID_ADDRESS)
    {
      CORE_ADDR all_image_info_addr = INVALID_ADDRESS;

     /* there is no dyld if this is kernel kdp-mode debugging: */
     if (target_is_kdp_remote())
       return 0;

     if (target_is_remote())
       {
         all_image_info_addr = remote_macosx_get_all_image_infos_addr();
       }
     else
       {
#if defined(NM_NEXTSTEP) && defined(TASK_DYLD_INFO) && defined(TASK_DYLD_INFO_COUNT)
         struct task_dyld_info task_dyld_info;
         mach_msg_type_number_t count = TASK_DYLD_INFO_COUNT;
         kern_return_t kret;

          if (macosx_status->task == TASK_NULL)
            return 0;

          count = TASK_DYLD_INFO_COUNT;

          kret = task_info(macosx_status->task, TASK_DYLD_INFO,
                           (task_info_t)&task_dyld_info, &count);

          /* Sometimes task_info returns success, but the addr & size are still 0.
             Presumably when a task is just starting?  Anyway, protect against that
             case here.  */
          if ((kret != KERN_SUCCESS)
              || (task_dyld_info.all_image_info_addr == 0)
              || (task_dyld_info.all_image_info_size == 0))
            return 0;
          all_image_info_addr = (CORE_ADDR)task_dyld_info.all_image_info_addr;
#else
           return 0;
#endif /* NM_NEXTSTEP && TASK_DYLD_INFO && TASK_DYLD_INFO_COUNT */
        }

#if defined(TARGET_ARM)
      /* Old debugserver implementations do NOT support the qShlibInfoAddr
       * packet -- try the old fixed load address of dyld: */
      if (target_is_remote() && (all_image_info_addr == INVALID_ADDRESS)
          && dyld_starts_here_p(0x2fe00000))
        {
          s->dyld_addr = 0x2fe00000;
          s->dyld_slide = 0;
          return 1;
        }
#endif /* TARGET_ARM */

      if (all_image_info_addr == INVALID_ADDRESS)
        return 0;
      s->dyld_image_infos = all_image_info_addr;
    }

  if (s->dyld_image_infos != INVALID_ADDRESS)
    {
      struct dyld_raw_infos raw_infos = {
	((MAC_OS_X_VERSION_MIN_REQUIRED < __MAC_10_5) ? 1U : 2U),
	0U, 0UL, 0UL, 0, 0, 0UL, 0UL
      };
      struct gdb_exception e;
      TRY_CATCH(e, RETURN_MASK_ERROR)
        {
          dyld_read_raw_infos(s->dyld_image_infos, &raw_infos);
        }
      if (e.reason != (int)NO_ERROR)
        return 0;

      if ((raw_infos.dyld_actual_load_address != INVALID_ADDRESS)
          && (raw_infos.dyld_actual_load_address != 0)
          && dyld_starts_here_p(raw_infos.dyld_actual_load_address))
        {
          if ((raw_infos.dyld_intended_load_address != INVALID_ADDRESS)
              && (raw_infos.dyld_intended_load_address != 0)
              && (raw_infos.dyld_actual_load_address != INVALID_ADDRESS)
              && dyld_starts_here_p(raw_infos.dyld_actual_load_address))
            {
              s->dyld_addr = raw_infos.dyld_actual_load_address;
              s->dyld_slide = (raw_infos.dyld_actual_load_address
                               - raw_infos.dyld_intended_load_address);
              return 1;
            }
        }

      if ((raw_infos.dyld_intended_load_address == INVALID_ADDRESS)
          && (raw_infos.dyld_actual_load_address != INVALID_ADDRESS)
          && (raw_infos.version >= 9)
          && (s->dyld_name != NULL)
          && (s->dyld_name[0] != '\0')
          && dyld_starts_here_p(raw_infos.dyld_actual_load_address))
        {
          CORE_ADDR dyld_file_vma;
          if (macosx_lookup_dyld_file_load_address(s->dyld_name, &dyld_file_vma) == 0)
            return 0;
          s->dyld_addr = raw_infos.dyld_actual_load_address;
          s->dyld_slide = (raw_infos.dyld_actual_load_address - dyld_file_vma);
          return 1;
        }
    }
  return 0;
}

/* Searches the target address space for dyld itself, returning it in
   VALUE. Returns 1 if it found dyld, 0 if it did not find it, and -1 if there
   was an error. If HINT is not INVALID_ADDRESS, check there first
   as a hint for where to find dyld. */

static int
macosx_locate_dyld(CORE_ADDR *value, CORE_ADDR hint)
{
  /* If we have remote Mac OS X target, we do NOT support alternate
     dyld versions when remote debugging. If we ever need to
     support alternate dyld versions remotely, we will need to
     add the region-searching functionality to gdbserver and
     create a custom command to access the results.  */
  if (target_is_remote())
    {
      if ((hint != INVALID_ADDRESS) && dyld_starts_here_p(hint))
        {
          *value = hint;
          return 1;
        }
      return 0;
    }

#if defined(NM_NEXTSTEP)
  {
    kern_return_t kret = KERN_SUCCESS;
    vm_region_basic_info_data_64_t info;
    mach_msg_type_number_t info_cnt = sizeof(vm_region_basic_info_data_64_t);
    mach_vm_address_t test_addr = VM_MIN_ADDRESS;
    mach_vm_size_t size = 0;
    mach_port_t object_name = MACH_PORT_NULL;
    task_t target_task = macosx_status->task;

    info_cnt = VM_REGION_BASIC_INFO_COUNT_64;

    do {
      kret = mach_vm_region(target_task, &test_addr,
                            &size, VM_REGION_BASIC_INFO_64,
                            (vm_region_info_t)&info, &info_cnt,
                            &object_name);

      /* This function is initially called before the inferior
         process has started executing; we will get back a KERN_NO_SPACE.
         When we try again once running the inferior we will try again.  */
      if (kret != KERN_SUCCESS)
        return -1;

      if (dyld_starts_here_p(test_addr))
        {
          *value = (CORE_ADDR)test_addr;
          return 1;
        }

      test_addr += size;
    } while (size != 0);
  }
#endif /* NM_NEXTSTEP */
  return 0;
}

/* Determine the address where the current target expected dyld to be
   loaded.  This is used when first guessing the address of dyld for
   symbol loading on dynamic executables, and also to compute the
   proper slide to be applied to values found in the dyld memory
   itself.  Returns 1 if it found the address, 0 if it did not. */

int
macosx_locate_dyld_static(macosx_dyld_thread_status *s, CORE_ADDR *value)
{
  struct objfile *objfile = NULL;
  const char *dyld_name = s->dyld_name;

  if (dyld_name == NULL)
    dyld_name = "/usr/lib/dyld";

  ALL_OBJFILES(objfile)
  {
    asection *text_section = NULL;
    if (core_bfd)
      {
	const char *bfd_filename = bfd_get_filename(objfile->obfd);
	if (strstr(bfd_filename, "[memory object \"/usr/lib/dyld\"") == bfd_filename)
	  {
	    text_section = bfd_get_section_by_name(objfile->obfd,
						   "LC_SEGMENT.__TEXT");
	    *value = bfd_section_vma(objfile->obfd, text_section);
	    return 1;
	  }
      }
    if (strcmp(dyld_name, bfd_get_filename(objfile->obfd)) == 0)
      {
        text_section = bfd_get_section_by_name(objfile->obfd,
                                               "LC_SEGMENT.__TEXT");
        *value = bfd_section_vma(objfile->obfd, text_section);
        return 1;
      }
  }
  return 0;
}

/* A parallel function to macosx_dyld_init. Initialize the CFM parser
   and breakpoint, if possible; take no action if not, or if it has
   already been initialized. Return '1' if we end up initializing the
   CFM code; '0' if we do not, or if it has already been
   initialized. */

#if WITH_CFM
int
macosx_cfm_init(macosx_cfm_thread_status *s)
{
  if (inferior_auto_start_cfm_flag == 0)
    return 0;

  if (s->cfm_breakpoint != NULL)
    return 0;

  macosx_cfm_thread_create(s);
  if (s->cfm_breakpoint == NULL)
    return 0;

  return 1;
}
#endif /* WITH_CFM */

/* Step through the load commands of an in-memory MachO file
   at MEM_ADDR, find the __TEXT segment load command and return
   the __TEXT segment load address.
   If the MachO file was slid when loaded into memory (by dyld
   or whatever), MEM_ADDR and the return address will differ.
   If no __TEXT segment is found, INVALID_ADDRESS is returned.  */

static CORE_ADDR
find_text_segment_load_address(CORE_ADDR mem_addr)
{
  struct bfd_section *mem_sect;
  struct cleanup *bfd_cleanups = NULL;
  bfd *mem_bfd = NULL;
  struct mach_header h;
  int header_size;
  gdb_byte *buf;
  target_read_mach_header(mem_addr, &h);
  header_size = target_get_mach_header_size(&h);
  buf = (gdb_byte *)xmalloc(h.sizeofcmds + header_size);
  bfd_cleanups = make_cleanup(xfree, buf);
  if (target_read_memory(mem_addr, buf, h.sizeofcmds + header_size) == 0)
    {
      mem_bfd = bfd_memopenr("tempbfd", NULL, buf, h.sizeofcmds + header_size);
      if (mem_bfd && bfd_check_format(mem_bfd, bfd_object))
        {
          make_cleanup_bfd_close(mem_bfd);
          for (mem_sect = mem_bfd->sections;
               mem_sect != NULL;
               mem_sect = mem_sect->next)
            {
              if (mem_sect->name && strcmp (mem_sect->name, "LC_SEGMENT.__TEXT") == 0)
                {
                  do_cleanups (bfd_cleanups);
                  return mem_sect->vma;
                }
            }
        }
    }
  if (bfd_cleanups)
    do_cleanups (bfd_cleanups);
  return INVALID_ADDRESS;
}

void
macosx_dyld_create_inferior_hook(CORE_ADDR all_image_info_addr)
{
  macosx_clear_start_breakpoint();
  dyld_init_paths(&macosx_dyld_status.path_info);
  macosx_dyld_thread_init(&macosx_dyld_status);

  macosx_dyld_status.dyld_image_infos = all_image_info_addr;
  if (inferior_auto_start_dyld_flag)
    {
      macosx_dyld_init (&macosx_dyld_status, exec_bfd);
    }
}

#if defined(__GNUC__) && defined(__GNUC_MINOR__)
# if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6))
 #  pragma GCC diagnostic push
 #  pragma GCC diagnostic ignored "-Woverflow"
# endif /* gcc 4.6+ */
#endif /* any gcc */

/* Locates the dylinker in the executable, and updates the dyld part
   of our data structures.

   This function should be able to be called at any time, and should
   always do as much of the right thing as possible based on the
   information available at the time.

   Return '1' if we end up initializing the dyld code; '0' if we do
   not, or if it has already been initialized. */

int
macosx_dyld_init(macosx_dyld_thread_status *s, bfd *exec_bfd)
{
  CORE_ADDR static_dyld_address = INVALID_ADDRESS;
  CORE_ADDR dyld_address = INVALID_ADDRESS;
  CORE_ADDR prev_dyld_address = INVALID_ADDRESS;
  int ret = 0;

  /* Once dyld_slide is set, we have already done everything that needs
   * to be done.  At least for the moment, it is not possible for dyld to
   * be moved once the program is executing: */
  if (s->dyld_slide != INVALID_ADDRESS)
    return 0;

  /* Now find dyld's name.  Get it from the exec_bfd if we have it; if we
   * do NOT find it here, then we will look again in memory once we have
   * found dyld: */
  if (s->dyld_name == NULL)
    macosx_lookup_dyld_name(exec_bfd, &s->dyld_name);

  /* Find the location of the dyld, if it has not been found already. */
  /* Store the old value, so we will know if it was changed: */
  prev_dyld_address = s->dyld_addr;

  if (prev_dyld_address == INVALID_ADDRESS) {
    ; /* this just silences '-Wunused-but-set-variable' for now... */
  }

  /* Ask the kernel, if possible: */
  ret = macosx_locate_dyld_via_taskinfo(s);
  /* If dyld loaded someplace other than where it normally would, slide
     the minsyms in the objfile right away: we will NOT get any dyld notification
     about it sliding as we do with other images.  */
  if ((ret == 1)  && (s->dyld_addr != 0)
      && (s->dyld_addr != INVALID_ADDRESS) && (s->dyld_slide != 0)
      && (s->dyld_slide != INVALID_ADDRESS) && (s->dyld_name != NULL))
    {
      struct objfile *o = find_objfile_by_name(s->dyld_name, 1);
      if (o)
        {
          slide_objfile(o, s->dyld_slide, NULL);
          s->dyld_minsyms_have_been_relocated = 1;
        }
    }

  if (ret == 1)
    {
      ret = macosx_locate_dyld(&dyld_address, s->dyld_addr);
    }

  if (ret != 1)
    {
      macosx_locate_dyld_static(s, &static_dyld_address);
      if (s->dyld_addr != INVALID_ADDRESS)
        ret = macosx_locate_dyld(&dyld_address, s->dyld_addr);
      else if (static_dyld_address != INVALID_ADDRESS)
        ret = macosx_locate_dyld(&dyld_address, static_dyld_address);
      else
        ret = macosx_locate_dyld(&dyld_address, INVALID_ADDRESS);
    }

  if ((ret != 1) && target_is_remote())
    {
      /* On an iPhone we can find dyld at 0x2fe00000.
         On a PPC app running under Rosetta, we will find dyld at 0x8fc00000.
         We need to add a remote protocol packet which will iterate over
         allocated memory regions and look for a dyld on the remote side
         instead of hardcoding these.  */
      ret = macosx_locate_dyld(&dyld_address, 0x2fe00000ull);
      if (ret != 1)
        ret = macosx_locate_dyld(&dyld_address, 0x8fc00000ull);
    }

  if (ret != 1)
    {
      /* If we have a core file we should get our information from what is
         in the core memory.  */
      if (core_bfd)
	{
	  /* Check the two usual dyld addresses for 32 and 64 bit first: */
	  if (dyld_starts_here_in_memory(0x8fe00000ull))
	    {
	      /* We found dyld at the default 32 bit location: */
	      dyld_address = 0x8fe00000ull;
	      ret = 1;
	    }
	  else if (dyld_starts_here_in_memory(0x00007fff5fc00000ull))
	    {
	      /* We found dyld at the default 64 bit location: */
	      dyld_address = 0x00007fff5fc00000ull;
	      ret = 1;
	    }
	  else
	    {
              /* DYLD did not seem to start at the standard location, do an
               * exhaustive search through each of the core segments.  Nick
               * said that dyld will start at the beginning of one of the
               * segments and it should be good enough to scan the first
               * address in each segment of the core.  */
	      unsigned int i = 0U;
	      struct bfd_section *core_segment;
	      for (i = 0U, core_segment = core_bfd->sections;
		   (core_segment != NULL) && (i < core_bfd->section_count);
		   i++, core_segment = core_segment->next)
		{
		  if (dyld_starts_here_in_memory(core_segment->vma))
		    {
		      /* We found dyld in our core file: */
		      dyld_address = core_segment->vma;
		      ret = 1;
		      break;
		    }
		}
	    }
	}
    }
  /* If we did NOT find dyld, then there is no point continuing. */

  if (ret != 1)
    return 0;
  CHECK_FATAL(dyld_address != INVALID_ADDRESS);

  /* Now grub for dyld's name in memory if we have NOT found it already: */
  if (dyld_address != INVALID_ADDRESS)
    {
      target_read_mach_header(dyld_address, &s->dyld_mem_header);

      if (s->dyld_name == NULL)
        s->dyld_name = dyld_find_dylib_name(dyld_address,
                                            s->dyld_mem_header.cputype,
					    s->dyld_mem_header.ncmds);

      /* Store the osabi of the found-dyld in these global variables because
         we need to use this over in the tdep.c file where we do NOT always
         have access to the macosx_dyld_thread_status or macosx_status
         structures.  */
      if ((s->dyld_mem_header.cputype == BFD_MACH_O_CPU_TYPE_POWERPC)
          || (s->dyld_mem_header.cputype == BFD_MACH_O_CPU_TYPE_I386))
        osabi_seen_in_attached_dyld = GDB_OSABI_DARWIN;
      else if (s->dyld_mem_header.cputype == BFD_MACH_O_CPU_TYPE_POWERPC_64
               || s->dyld_mem_header.cputype == BFD_MACH_O_CPU_TYPE_X86_64)
        osabi_seen_in_attached_dyld = GDB_OSABI_DARWIN64;
#if defined(TARGET_ARM)
      else if (s->dyld_mem_header.cputype ==  BFD_MACH_O_CPU_TYPE_ARM)
	{
	  if (s->dyld_mem_header.cpusubtype == BFD_MACH_O_CPU_SUBTYPE_ARM_6)
	    osabi_seen_in_attached_dyld = GDB_OSABI_DARWINV6;
	  else if (s->dyld_mem_header.cpusubtype == BFD_MACH_O_CPU_SUBTYPE_ARM_7)
	    osabi_seen_in_attached_dyld = GDB_OSABI_DARWINV7;
	  else if (s->dyld_mem_header.cpusubtype == BFD_MACH_O_CPU_SUBTYPE_ARM_7F)
	    osabi_seen_in_attached_dyld = GDB_OSABI_DARWINV7F;
	  else if (s->dyld_mem_header.cpusubtype == BFD_MACH_O_CPU_SUBTYPE_ARM_7S)
	    osabi_seen_in_attached_dyld = GDB_OSABI_DARWINV7S;
	  else if (s->dyld_mem_header.cpusubtype == BFD_MACH_O_CPU_SUBTYPE_ARM_7K)
	    osabi_seen_in_attached_dyld = GDB_OSABI_DARWINV7K;
	  else
	    osabi_seen_in_attached_dyld = GDB_OSABI_DARWIN;
	}
#endif /* TARGET_ARM */
    }

  /* Once we know the address at which dyld was loaded, we can try to
     read in the symbols for it, and then hopefully find the static
     address and use it to compute the slide. */

  s->dyld_addr = dyld_address;
  macosx_dyld_update(1);
  if ((s->dyld_addr == INVALID_ADDRESS)
      || (s->dyld_slide == INVALID_ADDRESS))
    {
      macosx_locate_dyld_static(s, &static_dyld_address);

      if (static_dyld_address == INVALID_ADDRESS)
        return 0;

      /* At this point we have the actual load address of dyld
         and the intended load address (the 'static_address' gotten
         out of the dyld objfile).  */

      s->dyld_addr = dyld_address;
      s->dyld_minsyms_have_been_relocated = 1;

      if (s->dyld_slide == INVALID_ADDRESS)
        {
          s->dyld_slide = (dyld_address - static_dyld_address);
          if (s->dyld_slide != 0)
            {
              s->dyld_minsyms_have_been_relocated = 0;
            }
        }
    }
  else
    {
      if (s->dyld_slide == 0)
        s->dyld_minsyms_have_been_relocated = 1;
      else
        s->dyld_minsyms_have_been_relocated = 0;
    }

  /* We only need to initialize the shared cache ranges in the case of a core
   * file.  We actually do NOT want to initialize them in macosx_dyld_init
   * when we are running a child process because this gets called BEFORE dyld
   * has a chance to run and fill in the correct values.  So we defer calling
   * macosx_init_dyld_cache_ranges to macosx_solib_add.  Note that when we do an
   * attach, macosx_child_attach calls macosx_solib_add to set up the shared
   * library state, so we are okay in that case as well: */
  if (core_bfd)
    macosx_init_dyld_cache_ranges(s);

  macosx_init_addresses(s);
  macosx_set_start_breakpoint(s, exec_bfd);

  breakpoints_changed();
  return 1;
}

/* keep this condition the same as where we push: */
#if defined(__GNUC__) && defined(__GNUC_MINOR__)
# if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6))
 #  pragma GCC diagnostic pop
# endif /* gcc 4.6+ */
#endif /* any gcc */

/* Put a breakpoint in the dyld function (in the inferior) which is
   called every time an image/a group of images (bundles, dylibs, etc.) are
   added/removed from the inferior process.  */

void
macosx_set_start_breakpoint (macosx_dyld_thread_status *s, bfd *exec_bfd)
{
  struct macosx_dyld_thread_status *status = &macosx_dyld_status;
  struct minimal_symbol *malloc_inited;
  int changed_breakpoints = 0;

  if (status->dyld_breakpoint == NULL)
    {
      status->dyld_breakpoint = create_solib_event_breakpoint
                                         (status->dyld_notify);
      changed_breakpoints = 1;
    }

  /* We only have to bother with the malloc initialized breakpoint in
     version 1 dyld.  With version 2 there is an explicit field in the
     dyld_all_image_infos structure telling us whether it is
     initialized or not.  */
  if (s->dyld_version == 1)
    {
      if (status->malloc_inited_breakpoint == NULL)
	{
	  struct cleanup *free_adorned;

          /* The malloc_inited_callback function has two possible mangled
             forms.  We have to check for both.  */

	  char *adorned_name = ((char *)
                                xmalloc(strlen(malloc_inited_callback) +
                                        strlen(dyld_symbols_prefix) + 1U));
          char *adorned_name_alt;
	  free_adorned = make_cleanup(xfree, adorned_name);
	  strcpy(adorned_name, dyld_symbols_prefix);
	  strcat(adorned_name, malloc_inited_callback);

	  adorned_name_alt = ((char *)
                              xmalloc(strlen(malloc_inited_callback_alt)
                                      + strlen(dyld_symbols_prefix) + 1U));
	  make_cleanup(xfree, adorned_name_alt);
	  strcpy(adorned_name_alt, dyld_symbols_prefix);
	  strcat(adorned_name_alt, malloc_inited_callback_alt);

	  malloc_inited = lookup_minimal_symbol(adorned_name, NULL, NULL);
          if (malloc_inited == NULL)
	    malloc_inited = lookup_minimal_symbol(adorned_name_alt, NULL, NULL);

	  do_cleanups(free_adorned);
	  if (malloc_inited != NULL)
	    {
	      status->malloc_inited_breakpoint =
                create_solib_event_breakpoint(SYMBOL_VALUE_ADDRESS(malloc_inited));
	      changed_breakpoints = 1;
	    }
	  else
	    {
              /* If I do not find the malloc symbol, then I had better
               * turn on malloc inited now, since there will NOT be another
               * good chance to do so.  This could happen with a totally
               * static binary, or if somebody set the dyld load state to
               * "container": */
	      warning("Could not find malloc init callback function.  "
		      "\nMake sure malloc is initialized before calling functions.");
	      macosx_set_malloc_inited(1);
	    }
	}
    }

  if (changed_breakpoints)
    breakpoints_changed();
}

#if defined(TARGET_POWERPC)
static ULONGEST
FETCH_ARGUMENT(int i)
{
  return read_register(3 + i);
}
#elif defined (TARGET_I386)
static ULONGEST
FETCH_ARGUMENT(int i)
{
  if (gdbarch_osabi(current_gdbarch) == GDB_OSABI_DARWIN64)
    {
      int reg = 0;
      switch (i)
        {
          case 0:
            reg = AMD64_RDI_REGNUM;
            break;
          case 1:
            reg = AMD64_RSI_REGNUM;
            break;
          case 2:
            reg = AMD64_RDX_REGNUM;
            break;
	  default:
	    break;
        }
      return read_register(reg);
    }
  else
    {
      CORE_ADDR stack = read_register(SP_REGNUM);
      return read_memory_unsigned_integer(stack + (4 * (i + 1)), 4);
    }
}
#elif defined(TARGET_ARM) /* NGK XXX hack */
static ULONGEST
FETCH_ARGUMENT(int i)
{
  return read_register(i);
}
#else
# ifdef S_SPLINT_S
static ULONGEST
FETCH_ARGUMENT(int i)
{
  if (i > 0) {
    return (ULONGEST)i;
  } else {
    return ULONG_MAX;
  }
}
# else
#  error "unknown architecture"
# endif /* S_SPLINT_S */
#endif /* TARGET_foo */

/* Remove NUM dyld_objfile_entries listed in ENTRIES from DYLD_STATUS: */
void
macosx_dyld_remove_libraries(struct macosx_dyld_thread_status *dyld_status,
                             struct dyld_objfile_entry *entries,
                             int num)
{
  int i;
  for (i = 0; i < num; i++)
    {
      struct dyld_objfile_entry *e = NULL;
      int found_it = 0;

      int k;

      DYLD_ALL_OBJFILE_INFO_ENTRIES(&dyld_status->current_info, e, k)
	{
	  if (dyld_libraries_compatible (&dyld_status->path_info, e, &entries[i]))
	    {
	      if (e->objfile)
		{
		  tell_breakpoints_objfile_removed(e->objfile);
		  tell_objc_msgsend_cacher_objfile_changed(e->objfile);
		}
	      if (ui_out_is_mi_like_p(uiout))
		{
		  struct cleanup *notify_cleanup;
		  notify_cleanup = make_cleanup_ui_out_notify_begin_end(uiout,
                                                                        "shlibs-removed");
		  dyld_print_entry_info(e, k, 0);
		  do_cleanups(notify_cleanup);
		}

	      dyld_remove_objfile(e);
	      dyld_objfile_entry_clear(e);
	      found_it = 1;
	      break;
	    }
	}
      if (!found_it)
	warning("Tried to remove a non-existent library: %s",
                entries[i].dyld_name ? entries[i].dyld_name : "<unknown>");
    }
}

/* Add the dyld_objfile_entry ENTRIES (an array of N of them) to the
   inferior process' DYLD_OBJFILE_INFO list of known images.

   We may already have dyld_objfile_entry records for some of the ENTRIES,
   e.g. because we looked at the program's load commands before starting it
   and now we're getting dylib-loaded notifications from dyld. So we want
   to replace the old entries with these newer, shinier ones.  */

void
macosx_dyld_add_libraries(struct macosx_dyld_thread_status *dyld_status,
                          struct dyld_objfile_entry *entries,
                          int num)
{
  int i;
  int shlibnum;
  static int timer_id = -1;
  struct cleanup *timer_cleanup = NULL;

  if (maint_use_timers)
   timer_cleanup = start_timer(&timer_id, "macosx_dyld_add_libraries", "");

  for (i = 0; i < num; i++)
    {
      struct dyld_objfile_entry *pentry;
      dyld_merge_shlib (dyld_status, &dyld_status->path_info,
                        &dyld_status->current_info, &entries[i]);
      dyld_prune_shlib (&dyld_status->path_info,
			&dyld_status->current_info, &entries[i]);

      pentry = dyld_objfile_entry_alloc (&dyld_status->current_info);
      *pentry = entries[i];
    }

  dyld_update_shlibs (&dyld_status->path_info, &dyld_status->current_info);

  if (! ui_out_is_mi_like_p (uiout))
    {
      if (maint_use_timers)
	do_cleanups (timer_cleanup);
      return;
    }

  for (i = 0; i < num; i++)
    {
      struct dyld_objfile_entry *entry = NULL;
      int j;
      DYLD_ALL_OBJFILE_INFO_ENTRIES(&dyld_status->current_info, entry, j)
        if (dyld_libraries_compatible(&dyld_status->path_info, entry,
                                      &entries[i]))
          {
            struct cleanup *notify_cleanup;
            /* Get the shlib number of this entry */
            CHECK_FATAL(dyld_entry_shlib_num(&dyld_status->current_info,
                                             entry, &shlibnum) == 0);
            notify_cleanup = make_cleanup_ui_out_notify_begin_end(uiout,
                                                              "shlibs-added");
            dyld_print_entry_info(entry, shlibnum, 0);
            if (ui_out_is_mi_like_p(uiout))
              {
                struct timeval t;
                make_cleanup_ui_out_tuple_begin_end(uiout, "time");
                gettimeofday(&t, NULL);
                ui_out_field_fmt(uiout, "now", "%u.%06u",
                             (unsigned int)t.tv_sec, (unsigned int)t.tv_usec);
              }
            do_cleanups(notify_cleanup);
	    /* We have to add objfiles to the list of objfiles which
	       should be scanned for new breakpoints when we see them
	       get loaded.

	       This is necessary because if we try to set a breakpoint
	       before the objfile gets loaded into memory we might get
	       it wrong - mostly because we try to move past the
	       prologue, but the memory for the breakpoint's not
	       mapped yet, so we cannot read the instructions.  When
	       that happens - for now just in breakpoint_re_set - we
	       mark the breakpoint as unset.  So we need to try it
	       again here.  */
	    if (entry->objfile != NULL)
	      breakpoint_re_set (entry->objfile);
          }
    }
  if (maint_use_timers)
    do_cleanups (timer_cleanup);

}

/* */
int
macosx_solib_add(const char *filename, int from_tty,
                 struct target_ops *targ, int loadsyms)
{
  struct macosx_dyld_thread_status *dyld_status = NULL;
#if WITH_CFM
  struct macosx_cfm_thread_status *cfm_status = NULL;
#endif /* WITH_CFM */
  int libraries_changed = 0;
  int notify = 0;
  int started_dyld = 0;
  int started_cfm = 0;

  dyld_status = &macosx_dyld_status;
#if (WITH_CFM && defined(NM_NEXTSTEP))
  CHECK_FATAL(macosx_status != NULL);
  cfm_status = &macosx_status->cfm_status;
#endif /* WITH_CFM && NM_NEXTSTEP */

  started_dyld = macosx_dyld_init(dyld_status, exec_bfd);
#if WITH_CFM
  started_cfm = macosx_cfm_init(cfm_status);
#endif /* WITH_CFM */

  /* macosx_dyld_init() can fail to set up the dyld addresses when
     we are re-setting the inferior process and this func is called
     from exec.c before the new process is up and running.  e.g. this
     sequence of commands:
        b main
        run
        file /path/to/some/file
        y
     will result in this function being called from solib_add_stub()
     without an inferior process yet initialized -- in which case, none of
     the below checks are going to accomplish anything. So we will just close
     our eyes for the nonce and wait to be called again when there is an
     inferior to inspect.  */

  if (dyld_status->dyld_addr == INVALID_ADDRESS)
    return 0;

  /* We need to initialize the shared ranges here.  We cannot do it in
     macosx_init_dyld, since that also gets called when we first stop
     the target, before the dyld code has had a chance to run and populate
     the num_shared_cache_ranges with the correct value.  */
  if (dyld_status->dyld_num_shared_cache_ranges == -1)
    macosx_init_dyld_cache_ranges(dyld_status);

  /* If we have version 2 of the dyld_all_image_infos structure,
     and we have NOT seen the libsystem_initialized set yet, check
     again here.  */
  if ((dyld_status->dyld_version >= 2)
      && (macosx_get_malloc_inited() != 1)
      && (dyld_status->dyld_image_infos != INVALID_ADDRESS))
    {
      struct dyld_raw_infos info;
      dyld_read_raw_infos(dyld_status->dyld_image_infos, &info);
      dyld_status->libsystem_initialized = info.libsystem_initialized;
      macosx_set_malloc_inited(dyld_status->libsystem_initialized);
    }

  /* If the inferior stopped at the dyld notification function,
     some file images have been loaded or removed.  */

  if (started_dyld || started_cfm)
    {
      libraries_changed = macosx_dyld_update(0);
      notify = (libraries_changed && dyld_stop_on_shlibs_updated);
    }
  else if (read_pc() == 0)
    {
      /* initial update -- $pc == 0x0 means we are not executing */
      libraries_changed = macosx_dyld_update(0);
      notify = (libraries_changed && dyld_stop_on_shlibs_updated);
    }
#if WITH_CFM
  else if ((cfm_status->cfm_breakpoint != NULL)
	   && (cfm_status->cfm_breakpoint->loc->address == read_pc()))
    {
      /* no cfm support for incremental update yet: */
      libraries_changed = macosx_dyld_update(0);
      notify = (libraries_changed && dyld_stop_on_shlibs_updated);
    }
#endif /* WITH_CFM */
  else if ((dyld_status->malloc_inited_breakpoint != NULL)
	   && (dyld_status->malloc_inited_breakpoint->loc->address == read_pc()))
    {
      macosx_set_malloc_inited(1);
    }
  else if ((dyld_status->dyld_breakpoint != NULL)
	   && (dyld_status->dyld_breakpoint->loc->address == read_pc()))
    {
      static struct dyld_raw_info *rinfo = NULL;
      static struct dyld_objfile_entry *tinfo = NULL;
      static unsigned int maxent = 0U;
      unsigned int i;
      unsigned int j;

      int mode = (int)FETCH_ARGUMENT(0);           /* Adding or removing */
      int count = (int)FETCH_ARGUMENT(1);          /* How many */
      CORE_ADDR addr = FETCH_ARGUMENT(2);     /* ptr to array of structs */
      static int breakpoint_timer = -1;
      struct cleanup *timer_cleanup = NULL;

      num_macosx_solib_add_dyld_objfile_entries = 0;
      macosx_solib_add_dyld_objfile_entries = NULL;
      if (count > (int)maxent)
        {
          maxent = ((count >= 16) ? count : 16);
          rinfo = ((struct dyld_raw_info *)
                   xrealloc(rinfo,
                            (maxent * sizeof(struct dyld_raw_info))));
          tinfo = ((struct dyld_objfile_entry *)
                   xrealloc(tinfo,
                            (maxent * sizeof(struct dyld_objfile_entry))));
        }
      macosx_solib_add_dyld_objfile_entries = tinfo;

      dyld_info_read_raw_data(addr, count, rinfo);

      j = 0;
      for (i = 0U; (int)i < count; i++)
        {
          dyld_objfile_entry_clear(&tinfo[j]);
          tinfo[j].allocated = 1;
          tinfo[j].reason = dyld_reason_dyld;
          tinfo[j].dyld_valid = 1;
	  num_macosx_solib_add_dyld_objfile_entries = (j + 1);
          if (dyld_info_process_raw(dyld_status, &tinfo[j], rinfo[i].name,
				    rinfo[i].modtime, rinfo[i].addr) == 1)
            j++;
        }

      /* MODE == 0 is shared libraries added.
	 MODE == 1 is shared libraries removed.  */
      if (mode == 0)
	{
	  macosx_dyld_add_libraries(&macosx_dyld_status, tinfo, j);
	}
      else if (mode == 1)
	{
	  macosx_dyld_remove_libraries(dyld_status, tinfo, j);
	}
      else
	{
	  internal_error(__FILE__, __LINE__,
                         "Wrong mode value %d from dyld notification.",
                         mode);
	}

      /* Maybe CarbonCore just got loaded -- let us see if we cannot insert the
         CFM dylib-loaded breakpoint now.  */
#if WITH_CFM
      macosx_cfm_init(cfm_status);
#endif /* WITH_CFM */

      libraries_changed = 1;
      if (maint_use_timers)
	timer_cleanup = start_timer(&breakpoint_timer, "shlib-bkpt-reset", "");

      /* Since we want to re-check breakpoints in libraries that get
       * loaded (to catch cases where we originally did not insert
       * a breakpoint because it was not loaded and we could not read
       * the memory for the breakpoint) we need to update all the
       * breakpoints here after we have added all the libraries.  */

      breakpoint_update();
      objc_clear_caches();

      if (maint_use_timers)
	do_cleanups(timer_cleanup);
      notify = (libraries_changed && dyld_stop_on_shlibs_updated);
    }
  else if (objc_handle_update(read_pc()) == 1)
    {
      /* Do not need to do anything special here, just need to call this.  */
    }
  else if ((dyld_status->dyld_breakpoint != NULL) || core_bfd)
    {
      /* looks like an attach -- we are not at the dyld_breakpoint, but
         the dyld_breakpoint has already been set.  */
      libraries_changed = macosx_dyld_update(0);
      notify = (libraries_changed && dyld_stop_on_shlibs_updated);
    }
  else
    {
      if (target_is_remote())
        {
          printf_filtered("Communication packet log leading up to internal error:\n");
          dump_packets_command("", 0);
          printf_filtered("\n\nPlease include all of the above log output in your bug report.\n");
          printf_filtered("\n");
        }
      internal_error(__FILE__, __LINE__,
                     "unrecognized shared library breakpoint, stopped at 0x%s",
                     paddr_nz(read_pc()));
      notify = 0;
    }

  return notify;
}

/* */
void
macosx_dyld_thread_init(macosx_dyld_thread_status *s)
{
  macosx_clear_start_breakpoint();
  s->dyld_name = NULL;
  s->dyld_addr = INVALID_ADDRESS;
  s->dyld_slide = INVALID_ADDRESS;
  s->dyld_minsyms_have_been_relocated = 0;
  s->dyld_image_infos = INVALID_ADDRESS;
  s->dyld_shared_cache_ranges = INVALID_ADDRESS;
  s->dyld_num_shared_cache_ranges = -1;
  s->dyld_version = 0;
  s->dyld_breakpoint = NULL;
  s->malloc_inited_breakpoint = NULL;
  s->dyld_shared_cache_array = NULL;
  dyld_zero_path_info(&s->path_info);
}

/* */
void
macosx_dyld_thread_clear(macosx_dyld_thread_status *s)
{
  if (s->dyld_name)
    xfree((char *)s->dyld_name);

  if (s->path_info.framework_path != NULL)
    xfree(s->path_info.framework_path);
  if (s->path_info.library_path != NULL)
    xfree(s->path_info.library_path);
  if (s->path_info.fallback_framework_path != NULL)
    xfree(s->path_info.fallback_framework_path);
  if (s->path_info.fallback_library_path != NULL)
    xfree(s->path_info.fallback_library_path);
  if (s->path_info.image_suffix != NULL)
    xfree(s->path_info.image_suffix);
  if (s->path_info.insert_libraries != NULL)
    xfree(s->path_info.insert_libraries);
  if (s->dyld_shared_cache_array)
    xfree(s->dyld_shared_cache_array);
  macosx_clear_start_breakpoint();
  macosx_dyld_thread_init(s);
}

/* */
void
macosx_add_shared_symbol_files(void)
{
  macosx_dyld_update(0);
}

/* */
void
macosx_init_dyld_from_core(void)
{
  /* Clear out any objfiles from the preloading of the shared libraries
     so that we take everything we possibly can from the core file. Core
     files can be transfered to a different computer with completely
     different binaries, we also could be cross debugging, so we need to
     take everything we can from the core image.  */
  struct dyld_objfile_info *info;
  struct dyld_objfile_entry *e = NULL;
  struct target_ops *target;
  int i;

  if (!inferior_auto_start_dyld_flag)
    return;

  info = &macosx_dyld_status.current_info;
  target = &exec_ops;

  DYLD_ALL_OBJFILE_INFO_ENTRIES(info, e, i)
    {
      dyld_remove_objfile(e);
      dyld_objfile_entry_clear(e);
    }

  macosx_dyld_mourn_inferior();

  macosx_dyld_thread_clear(&macosx_dyld_status);

  reinitialize_objc();

  /* The macosx-exec target keeps a list of address ranges and which
     bfd and bfd_sections they correspond to, so we need to blow away
     this cache.
     TODO: We really need a better way to track when a bfd_close is
     going to happen and call "remove_target_sections (bfd *abfd)". We
     are not going to make this change just before we ship, but this is
     something we need to keep an eye one. Perhaps a target_bfd_close
     would be a good way to control these things.  */
  if (target->to_sections != NULL)
    {
      xfree (target->to_sections);
      target->to_sections = target->to_sections_end = NULL;
    }

  macosx_solib_add (NULL, 0, NULL, 0);
}

/* */
void
macosx_init_dyld(struct macosx_dyld_thread_status *s,
		 struct objfile *o, bfd *abfd)
{
  struct dyld_objfile_entry *e0 = NULL;
  int i;
  struct dyld_objfile_info previous_info;
  static int timer_id = -1;
  struct cleanup *timer_cleanup = NULL;

  if (maint_use_timers)
    timer_cleanup = start_timer(&timer_id, "macosx_init_dyld", "");

  DYLD_ALL_OBJFILE_INFO_ENTRIES(&s->current_info, e0, i)
    if (e0->reason & dyld_reason_executable_mask)
      dyld_objfile_entry_clear(e0);

  dyld_init_paths(&s->path_info);

  dyld_objfile_info_init(&previous_info);

  if (pre_slide_libraries_flag && (s->pre_run_memory_map == NULL)
      && (abfd != NULL))
    {
      printf_filtered("\n");
      s->pre_run_memory_map = create_pre_run_memory_map(abfd);
    }

  dyld_objfile_info_copy(&previous_info, &s->current_info);
  dyld_objfile_info_free(&s->current_info);

  if (o != NULL)
    {
      const char *objfile_name = NULL;
      struct dyld_objfile_entry *e1;

      /* Canonicalize the name: */
      if (bfd_get_filename(o->obfd) != NULL)
        {
          char buf[PATH_MAX];
          if (realpath(bfd_get_filename(o->obfd), buf) != NULL)
            {
              objfile_name = xstrdup(buf);
            }
          else
            objfile_name = bfd_get_filename(o->obfd);
        }

      e1 = dyld_objfile_entry_alloc(&s->current_info);
      e1->text_name_valid = 1;
      e1->reason = dyld_reason_executable;
      e1->objfile = o;
      /* No need to set e1->abfd, since e1->objfile is present. */
      e1->load_flag = o->symflags;
      e1->text_name = objfile_name;
      e1->loaded_from_memory = 0;
      e1->loaded_name = e1->text_name;
      e1->loaded_addr = 0;
      e1->loaded_addrisoffset = 1;
    }

  if (dyld_preload_libraries_flag)
    {
      dyld_add_inserted_libraries(&s->current_info, &s->path_info);
      if (abfd != NULL)
        {
          dyld_add_image_libraries(&s->current_info, abfd);
        }
    }

  dyld_merge_shlibs(s, &s->path_info, &previous_info, &s->current_info);
  dyld_update_shlibs(&s->path_info, &s->current_info);

  dyld_objfile_info_free(&previous_info);

  s->state = dyld_initialized;
  if (maint_use_timers)
    do_cleanups(timer_cleanup);
}


/* We now store the in memory version of the mach header in the
   dyld_objfile_entry structures so we can access CPUTYPE and CPUSUBTYPE as
   dyld has loaded them. When a universal file gets loaded, we need to grab the
   correct slice. To accomplish this we check the cpu type and subtype and
   return the corresponding OSABI for each bfd.  */
enum gdb_osabi
macosx_get_osabi_from_dyld_entry(bfd *abfd)
{
  const char *bfd_name;
  struct macosx_dyld_thread_status *status;
  const struct dyld_path_info *d;
  struct dyld_objfile_entry *e = NULL;
  int i;
  enum gdb_osabi bfd_osabi;
  const char *entry_name;

  if (abfd == NULL)
    return GDB_OSABI_UNKNOWN;

  bfd_name = bfd_get_filename(abfd);
  if (bfd_name == NULL)
    return GDB_OSABI_UNKNOWN;

  status = &macosx_dyld_status;
  d = &status->path_info;
  bfd_osabi = GDB_OSABI_UNKNOWN;
  entry_name = NULL;

  for (i = (num_macosx_solib_add_dyld_objfile_entries - 1); i >= 0; i--)
    {
      e = &macosx_solib_add_dyld_objfile_entries[i];
      entry_name = dyld_entry_filename(e, d, DYLD_ENTRY_FILENAME_LOADED);
      if ((entry_name != NULL) && (strcmp(entry_name, bfd_name) == 0))
	{
	  bfd_osabi = dyld_objfile_entry_osabi(e);
	  break;
	}
    }

  if (bfd_osabi <= GDB_OSABI_UNKNOWN)
    {
      /* Check the current list of dyld objfile entries: */
      DYLD_ALL_OBJFILE_INFO_ENTRIES(&status->current_info, e, i)
	{
	  entry_name = dyld_entry_filename(e, d, DYLD_ENTRY_FILENAME_LOADED);
	  if (entry_name != NULL)
            {
              const char *alt_entry_name = dyld_fix_path(entry_name);
              if ((strcmp(entry_name, bfd_name) == 0)
                  || (strcmp(alt_entry_name, bfd_name) == 0))
                {
                  bfd_osabi = dyld_objfile_entry_osabi(e);
                  xfree((void *)alt_entry_name);
                  break;
                }
              xfree((void *)alt_entry_name);
            }
	}
    }

  if (bfd_osabi >= GDB_OSABI_UNKNOWN)
    dyld_debug("macosx_get_osabi_from_dyld_entry(%s) returned osabi = %s\n",
               bfd_name, gdbarch_osabi_name(bfd_osabi));
  return bfd_osabi;
}

/* */
void
macosx_init_dyld_symfile(struct objfile *o, bfd *abfd)
{
  macosx_init_dyld(&macosx_dyld_status, o, abfd);
}

/* */
static void
dyld_cache_purge_command(const char *exp ATTRIBUTE_UNUSED,
			 int from_tty ATTRIBUTE_UNUSED)
{
  dyld_purge_cached_libraries(&macosx_dyld_status.current_info);
}

/* This function returns an xmalloc'ed array of section offsets (slides)
   for a dylib in the shared cache.
   Dylibs included in the shared cache will have different offsets
   (slides) for different sections -- the only way to determine what those
   offsets are is to step through the in-memory section addresses and
   the on-disk section addresses and compute them individually.

   It is the caller's responsbility to xfree the returned offsets once
   they are done being used.  */

struct section_offsets *
get_sectoffs_for_shared_cache_dylib(struct dyld_objfile_entry *entry,
                                    CORE_ADDR header_addr)
{
  int cur_section;
  struct cleanup *cleanup;
  bfd *this_bfd = entry->abfd;
  struct section_offsets *sect_offsets;
  struct bfd_section *this_sect;
  unsigned int i;

  struct section_addr_info *sect_addrs;
  CORE_ADDR slide_within_shared_cache;

  sect_addrs = get_section_addresses_for_macho_in_memory(header_addr);
  if (sect_addrs == NULL)
    return NULL;
  cleanup = make_cleanup_free_section_addr_info(sect_addrs);

  /* The libraries in the shared cache are slid an additional
     amount within the shared cahce -- and that slide is not
     correctly recorded in the in-memory macho load commands
     that get_section_addresses_for_macho_in_memory() returned.  */

  slide_within_shared_cache = 0;
  for (i = 0U; i < sect_addrs->num_sections; i++)
    {
      if (strcmp("LC_SEGMENT.__TEXT", sect_addrs->other[i].name) == 0)
        {
          slide_within_shared_cache = (header_addr - sect_addrs->other[i].addr);
          break;
        }
    }
  for (i = 0U; i < sect_addrs->num_sections; i++)
    sect_addrs->other[i].addr += slide_within_shared_cache;

  /* The sect_addrs array now represents the actual load address of each
     section for this dylib in the shared cache.  */

  sect_offsets = (struct section_offsets *)
    xmalloc(SIZEOF_N_SECTION_OFFSETS(this_bfd->section_count));
  memset(sect_offsets, 0,
         SIZEOF_N_SECTION_OFFSETS(this_bfd->section_count));

  for (i = 0U; i < sect_addrs->num_sections; i++)
    {
      if ((sect_addrs->other[i].name == NULL)
          || (sect_addrs->other[i].name[0] == '\0')
          || (sect_addrs->other[i].addr == 0))
        continue;

      for (this_sect = this_bfd->sections, cur_section = 0;
           this_sect != NULL;
           this_sect = this_sect->next, ++cur_section)
        {
          if ((this_sect->name == NULL) || (this_sect->name[0] == '\0'))
            continue;

          if (strcmp(sect_addrs->other[i].name, this_sect->name) == 0)
            {
              sect_offsets->offsets[cur_section] =
                          (sect_addrs->other[i].addr - this_sect->vma);
            }
        }
    }

  do_cleanups(cleanup);
  return sect_offsets;
}


/* Process the information about a just-discovered file image from dyld
   and fill in a dyld_objfile_entry with the preliminary information.  */

static int
dyld_info_process_raw(struct macosx_dyld_thread_status *s,
		      struct dyld_objfile_entry *entry,
                      CORE_ADDR name, uint64_t modtime, CORE_ADDR header_addr)
{
  char *namebuf = NULL;
#ifndef errno
  /* FIXME: does this local variable really work as a replacement for the
   * global one? */
  int errno;
#endif /* !errno */
  unsigned int i;
  CORE_ADDR curpos;
  CORE_ADDR intended_loadaddr;
  struct load_command cmd;
  struct dylib_command dcmd;
  bfd *this_bfd = NULL;
  struct cleanup *override_trust_readonly;
  int old_trust_readonly;

  /* Determine whether the image is in the new "shared cache" region: */
  if (s->dyld_num_shared_cache_ranges != -1)
    {
      int j;
      for (j = 0; j < s->dyld_num_shared_cache_ranges; j++)
	{
	  if ((header_addr >= s->dyld_shared_cache_array[j].start)
              && (header_addr < (s->dyld_shared_cache_array[j].start
              			 + s->dyld_shared_cache_array[j].length)))
	    {
	      entry->in_shared_cache = 1;
	      break;
	    }
	}
    }

  /* Even if the library is loading in the same place as a directly linked
     shared library that we have read in already, we still want to read the raw
     information from memory.  */
  old_trust_readonly = set_trust_readonly(0);
  override_trust_readonly =
    make_cleanup(set_trust_readonly_cleanup,
                 (void *)(intptr_t)old_trust_readonly);

  /* Read the mach header for this entry and store it. The in memory version
     of the mach header will detail the exact slice and whether the image is
     in the new "shared cache" region.  */
  target_read_mach_header(header_addr, &entry->mem_header);

  switch (entry->mem_header.filetype)
    {
    case 0:
      return 0;
    case MH_EXECUTE:
    case MH_DYLIB:
    case MH_DYLINKER:
    case MH_BUNDLE:
#ifdef MH_KEXT_BUNDLE
    case MH_KEXT_BUNDLE:
#endif /* MH_KEXT_BUNDLE */
      break;
    case MH_FVMLIB:
    case MH_PRELOAD:
      return 0;
    default:
      warning("Ignored unknown object module at 0x%s with type 0x%x\n",
              paddr_nz(header_addr), entry->mem_header.filetype);
      do_cleanups(override_trust_readonly);
      return 0;
    }

  target_read_string(name, &namebuf, 1024, &errno);

  if (errno != 0)
    {
      xfree(namebuf);
      namebuf = NULL;
    }

  /* Look through the load commands for the image filename.
     Also read out the UUID if we are going to check it later.  */

  if (namebuf == NULL)
    {
      namebuf = (char *)xmalloc(PATH_MAX);

      curpos = (header_addr + target_get_mach_header_size(&entry->mem_header));

      for (i = 0U; i < entry->mem_header.ncmds; i++)
        {
	  if (target_read_load_command(curpos, &cmd) != 0)
	    break;
	  if (cmd.cmd == LC_ID_DYLIB)
	    {
	      if (target_read_dylib_command(curpos, &dcmd) == 0)
		target_read_memory(curpos + dcmd.dylib.name.offset,
				   (gdb_byte *)namebuf,
				   PATH_MAX);
	      break;
	    }
          curpos += cmd.cmdsize;
        }
    }

  /* realpath the image name -- we can get many different paths to the
     same file handed to us, so try to canonicalize them via realpath.  */

  if (namebuf != NULL)
    {
      char buf[PATH_MAX];
      char *resolved = NULL;

      /* Fix dyld path in case we have dyld search/replace paths for
         cross targets.  */
      resolved = dyld_fix_path (namebuf);
      xfree (namebuf);
      namebuf = resolved;

      resolved = realpath (namebuf, buf);
      if (resolved == NULL)
        resolved = namebuf;

      entry->dyld_name = xstrdup (resolved);
      entry->dyld_name_valid = 1;
    }
    /* Now that the name has been resolved print it out if dyld logging is
       enabled so we can see we are loading this shlib. This is handy to track
       when we are loading libraries again as on remote targets we just fixed
       a slow down issue related to reloading all libraries every time a shlid
       got loaded.  */
    dyld_debug ("dyld_info_process_raw() called for '%s'\n", entry->dyld_name);
    /* Set the loaded image address since we know where the mach header is.  */
    entry->dyld_addr = header_addr;
    if (core_bfd)
      {
	entry->dyld_valid = 1;
	entry->loaded_addr = header_addr;
	entry->loaded_addrisoffset = 0;
      }

  /* Next compute the intended load address of the library.  */
  intended_loadaddr = INVALID_ADDRESS;

  /* dyld_objfile_entry_in_shared_cache() needs to know whether this entry
     is actually in memory or not.  So I set dyld_valid early in this
     function - given the nature of this function, we know we got the
     information about the entry from dyld.  */
  if (entry->dyld_name)
    entry->dyld_valid = 1;

  if (dyld_objfile_entry_in_shared_cache (entry))
    {
      /* If we are in the shared cache, then the version in memory WILL NOT be
       * slid, it will be rebound to its correct address. So we need to
       * figure out the slide by looking at the disk version, figuring out
       * where that was supposed to load and then computing the difference
       * between that and where it really ended up.  */

      if (entry->dyld_name != NULL)
	{
	  struct bfd_section *this_sect = NULL;
	  struct section_offsets *sect_offsets = NULL;

	  /* We need to look at the load commands for the on-disk
	     file, so we might as well open the bfd here, and put it in the
	     dyld_objfile_entry. That way it will get reused most of the
	     time.  If we make a bfd for a directly linked file, that will
	     get thrown away in dyld_objfile_merge_load_data, but all the
	     indirectly linked ones will actually get reused.  I use
	     dyld_load_library_from_file since that is the code path that
	     dyld_load_library would use for directly linked files, and what
	     would happen if I did not open it here.  */

#ifdef ALWAYS_READ_FROM_MEMORY
          int old_always_read = dyld_always_read_from_memory_flag;
          dyld_always_read_from_memory_flag = 1;
	  entry->dyld_valid = 1;
	  entry->loaded_addr = header_addr;
	  entry->loaded_addrisoffset = 0;
#endif /* ALWAYS_READ_FROM_MEMORY */
	  dyld_load_library(&(s->path_info), entry);

#ifdef ALWAYS_READ_FROM_MEMORY
          dyld_always_read_from_memory_flag = old_always_read;
#endif /* ALWAYS_READ_FROM_MEMORY */
	  this_bfd = entry->abfd;

	  if (this_bfd == NULL)
	    {
	      entry->dyld_valid = 1;
	      entry->loaded_addr = header_addr;
	      entry->loaded_addrisoffset = 0;
	      dyld_load_library_from_memory (&(s->path_info), entry, 1);
	      this_bfd = entry->abfd;

	      if (this_bfd == NULL)
		error("Could not read in on disk or the memory version of library \"%s\".",
		      entry->dyld_name);
	    }

  	  if (macosx_bfd_is_in_memory (this_bfd))
  	    {
  	      int cur_section;
              CORE_ADDR process_shared_cache_slide;
  	      sect_offsets = (struct section_offsets *)
  		xmalloc(SIZEOF_N_SECTION_OFFSETS(this_bfd->section_count));
  	      memset(sect_offsets, 0,
  		     SIZEOF_N_SECTION_OFFSETS(this_bfd->section_count));

              process_shared_cache_slide = 0;
  	      intended_loadaddr = header_addr;
  	      for (this_sect = this_bfd->sections;
                   this_sect != NULL;
                   this_sect = this_sect->next)
                  {
                    if (this_sect->name && (strcmp(this_sect->name, "LC_SEGMENT.__TEXT") == 0))
                      {
                        process_shared_cache_slide = (header_addr - this_sect->vma);
                        intended_loadaddr = this_sect->vma;
                        break;
                      }
                  }

  	      cur_section = 0;
  	      for (this_sect = this_bfd->sections; this_sect != NULL;
  		   this_sect = this_sect->next)
  		{
  		  /* The offsets map to the sections that get added to the
  		     bfd, so do NOT add bfd sections that are not going to get
  		     added to the objfile later on.  */

  		  if (!objfile_keeps_section (this_bfd, this_sect))
  		      continue;
  		  sect_offsets->offsets[cur_section++] = process_shared_cache_slide;
  		}
  	      entry->dyld_section_offsets = sect_offsets;
  	    }
	  else
	    {
              entry->dyld_section_offsets =
                  get_sectoffs_for_shared_cache_dylib (entry, header_addr);
	    }


	  if (this_bfd != NULL)
	    {
	      asection *text_sect;

	      text_sect = bfd_get_section_by_name(this_bfd,
                                                  "LC_SEGMENT.__TEXT");
	      if (text_sect != NULL)
		intended_loadaddr = bfd_section_vma(this_bfd, text_sect);
	      else
		warning("Could not find __TEXT segment in disk version of "
                        " library \"%s\" for library in shared cache at %s",
                        entry->dyld_name, paddr_nz(header_addr));
	    }
	  else
	    {
	      error("Could not find the on disk version of library \"%s\" "
                    "for library in shared cache at %s", entry->dyld_name,
                    paddr_nz(header_addr));
	    }
	}
      else
	{
	  warning("Cannot read symbols for shared cache image at address %s",
		  paddr_nz(header_addr));
	}
    }
  else
    {
      /* This is the case where the library is NOT in the shared cache.
       * Find what address the image intended to load at, and compute the
       * slide (the difference between the intended load addr & the actual
       * load addr.) We use the bfd for the on disk image. If there ends up
       * being a problem locating the on-disk image, we can copy the code
       * above that uses the in memory bfd.  */
      struct bfd_section *this_sect;
      struct section_offsets *sect_offsets;
      int cur_section;

      /* See the comments above about reusing the bfd.  */
      /* We do NOT know what the slide is until we read the load commands
       * for the binary.  Do NOT pretend like we do, or it will fool the
       * layers below us: */
      entry->dyld_slide = INVALID_ADDRESS;
      dyld_load_library(&(s->path_info), entry);


      /* A bundle that only exists in memory (e.g. was loaded with
       * mmap + NSCreateObjectFileImageFromMemory + NSLinkModule)
       * may not have an on-disk file that we can find.  We should
       * map the entire image from the inferior into our address
       * space and do bfd_memopenr() on it, similar to what is
       * done above.  But at the very least, do NOT try to dereference
       * the bfd struct if we failed to open the file.  */

      /* It is a little tricky trying to calculate the slide of each
       * segment independently.  Since at present all libraries that are
       * NOT in the shared cache just get slid rigidly, we will figure out
       * the rigid slide using the slide of the text segment, then apply
       * that to all the sections.  */

      if (!entry->loaded_error && entry->abfd)
        {
	  CORE_ADDR rigid_slide;
	  this_bfd = entry->abfd;
	  sect_offsets = (struct section_offsets *)
            xmalloc (SIZEOF_N_SECTION_OFFSETS (this_bfd->section_count));
	  memset (sect_offsets, 0,
                  SIZEOF_N_SECTION_OFFSETS (this_bfd->section_count));

	  for (this_sect = this_bfd->sections;
               this_sect != NULL;
               this_sect = this_sect->next)
            {
              if (!objfile_keeps_section (this_bfd, this_sect))
                continue;
              if (strcmp (this_sect->name, TEXT_SEGMENT_NAME) == 0)
		{
		  intended_loadaddr = this_sect->vma;
		  break;
		}
	    }

	  rigid_slide = header_addr - intended_loadaddr;
          cur_section = 0;
          for (this_sect = this_bfd->sections;
	       this_sect != NULL;
	       this_sect = this_sect->next)
	    {
	      if (!objfile_keeps_section (this_bfd, this_sect))
		continue;
	      sect_offsets->offsets[cur_section++] = rigid_slide;
	    }
          entry->dyld_section_offsets = sect_offsets;
        }
    }

  /* The slide is the difference between the actual load address and
     the intended load address.  So if a library was supposed to load
     at 0x1f00 and it actually loads at 0x2f00 because another library
     was there, we have a slide of 0x1000.

     If it was supposed to load at 0x2f00 but it slides *down* to
     0x1f00, the slide will be 0xfffffffffffff000 in a 64-bit CORE_ADDR or
     0xfffff000 in a 32-bit CORE_ADDR.
     In short the slide is added to the intended load address and it "wraps
     around" to give you the actual load address. if you have a so-called
     "negative slide".
     e.g  0x2f00 + 0xfffffffffffff000 == 0x1f00 in 64-bit unsigned math.  */

  if (intended_loadaddr == INVALID_ADDRESS)
    entry->dyld_slide = 0;
  else
    entry->dyld_slide = header_addr - intended_loadaddr;
  entry->dyld_valid = 1;

  switch (entry->mem_header.filetype)
    {
    case MH_EXECUTE:
      {
        entry->reason = dyld_reason_executable;
        if (symfile_objfile != NULL)
          {
            entry->objfile = symfile_objfile;
            /* No need to set e->abfd, since e->objfile is present. */
	    if (core_bfd == NULL)
	      {
		entry->loaded_from_memory = 0;
		entry->loaded_addr = 0;
		entry->loaded_addrisoffset = 1;
	      }
            entry->loaded_name = symfile_objfile->name;
          }
        break;
      }
    case MH_DYLIB:
      entry->reason = dyld_reason_dyld;
      break;
    case MH_DYLINKER:
    case MH_BUNDLE:
#ifdef MH_KEXT_BUNDLE
    case MH_KEXT_BUNDLE:
#endif /* MH_KEXT_BUNDLE */
      entry->reason = dyld_reason_dyld;
      break;
    default:
      internal_error(__FILE__, __LINE__,
                     "Unknown object module at 0x%s (offset 0x%s) with type 0x%x\n",
                     paddr_nz(header_addr), paddr_nz(entry->dyld_slide),
                     entry->mem_header.filetype);
    }

  do_cleanups(override_trust_readonly);
  return 1;
}


struct dyld_all_image_infos_offsets
{
  int version;                          /* verison 1 */
  int infoArrayCount;                   /* version 1 */
  int infoArray;                        /* version 1 */
  int notification;                     /* version 1 */
  int processDetachedFromSharedRegion;  /* version 1 */
  int libSystemInitialized;             /* version 2 */
  int dyldImageLoadAddress;             /* version 2 */
  int jitInfo;                          /* version 3 */
  int dyldVersion;                      /* version 5 */
  int errorMessage;                     /* version 5 */
  int terminationFlags;                 /* version 5 */
  int coreSymbolicationShmPage;         /* version 6 */
  int systemOrderFlag;                  /* version 7 */
  int uuidArrayCount;                   /* version 8 */
  int uuidArray;                        /* version 8 */
  int dyldAllImageInfosAddress;         /* version 9 */
  int initialImageCount;                /* version 10 */
  int errorKind;                        /* version 11 */
  int errorClientOfDylibPath;           /* version 11 */
  int errorTargetDylibPath;             /* version 11 */
  int errorSymbol;                      /* version 11 */
  int sharedCacheSlide;                 /* version 12 */
};

/* Given the ADDR of the struct dyld_all_image_infos in the inferior,
   copy it from the inferior's memory into INFO.

   NB: the value for info->info_array may change as dyld grows the array
   of image files that are loaded (it will realloc() the array), so we cannot
   assume this value is constant across inferior running.
*/

static void
dyld_read_raw_infos(CORE_ADDR addr, struct dyld_raw_infos *info)
{
  int wordsize;
  int p;  /* pointers, uintptr_t */
  const int b = 1;         /* bools */
  const int i = 4;         /* ints */
  uint8_t version_buf[4];
  int version;
  size_t image_infos_size;
  gdb_byte *buf;
  CORE_ADDR adjustment;
  /* keep this conditional the inverse of below: */
#ifdef S_SPLINT_S
  struct dyld_all_image_infos_offsets offsets =
  {
    0, i, i + i, i + i + 1, i + i + 2, i + i + 2 + b, (2 * i) + (2 * b),
    (2 * i) + (2 * b) + 1, (2 * i) + (2 * b) + 2, (2 * i) + (2 * b) + 3,
    (2 * i) + (2 * b) + 4, (2 * i) + (2 * b) + 5, (2 * i) + (2 * b) + 6,
    (2 * i) + (2 * b) + 7, (2 * i) + (2 * b) + 8, (2 * i) + (2 * b) + 9,
    (2 * i) + (2 * b) + 10, (2 * i) + (2 * b) + 11, (2 * i) + (2 * b) + 12,
    (2 * i) + (2 * b) + 13, (2 * i) + (2 * b) + 14, (2 * i) + (2 * b) + 15,
  };
#endif /* S_SPLINT_S */

  gdb_assert(addr != INVALID_ADDRESS);

  wordsize = new_gdbarch_tdep(current_gdbarch)->wordsize;
  p = wordsize;

  /* ignore '-Wdeclaration-after-statement' here, because trying to hack
   * around the use of the C99 designated initializers is more trouble
   * than it seems like it would be worth... */
#if defined(__GNUC__) && defined(__GNUC_MINOR__) && !defined(__cplusplus)
# if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6))
 #  pragma GCC diagnostic push
 #  pragma GCC diagnostic ignored "-Wdeclaration-after-statement"
# else
#  if defined(__clang__) && (__clang__ >= 1)
 #   pragma clang diagnostic push
 #   pragma clang diagnostic ignored "-Wdeclaration-after-statement"
#  endif /* clang */
# endif /* gcc 4.6+ */
#endif /* any gcc, but not g++ */
#ifndef S_SPLINT_S
  struct dyld_all_image_infos_offsets offsets =
    {
      .version                         = 0,
      .infoArrayCount                  = i,
      .infoArray                       = i + i,
      .notification                    = i + i + p,
      .processDetachedFromSharedRegion = i + i + p + p,
      .libSystemInitialized            = i + i + p + p + b,
      /* there is padding inserted before the next word-aligned field */
      .dyldImageLoadAddress            = i + i + p + p + b + b + (p - 2 * b),
      .jitInfo                         = i + i + p + p + b + b + (p - 2 * b) + p,
      .dyldVersion                     = i + i + p + p + b + b + (p - 2 * b) + p + p,
      .errorMessage                    = i + i + p + p + b + b + (p - 2 * b) + p + p + p,
      .terminationFlags                = i + i + p + p + b + b + (p - 2 * b) + p + p + p + p,
      .coreSymbolicationShmPage        = i + i + p + p + b + b + (p - 2 * b) + p + p + p + p + p,
      .systemOrderFlag                 = i + i + p + p + b + b + (p - 2 * b) + p + p + p + p + p + p,
      .uuidArrayCount                  = i + i + p + p + b + b + (p - 2 * b) + p + p + p + p + p + p + p,
      .uuidArray                       = i + i + p + p + b + b + (p - 2 * b) + p + p + p + p + p + p + p + p,
      .dyldAllImageInfosAddress        = i + i + p + p + b + b + (p - 2 * b) + p + p + p + p + p + p + p + p + p,
      .initialImageCount               = i + i + p + p + b + b + (p - 2 * b) + p + p + p + p + p + p + p + p + p + p,
      .errorKind                       = i + i + p + p + b + b + (p - 2 * b) + p + p + p + p + p + p + p + p + p + p + p,
      .errorClientOfDylibPath          = i + i + p + p + b + b + (p - 2 * b) + p + p + p + p + p + p + p + p + p + p + p + p,
      .errorTargetDylibPath            = i + i + p + p + b + b + (p - 2 * b) + p + p + p + p + p + p + p + p + p + p + p + p + p,
      .errorSymbol                     = i + i + p + p + b + b + (p - 2 * b) + p + p + p + p + p + p + p + p + p + p + p + p + p + p,
      .sharedCacheSlide                = i + i + p + p + b + b + (p - 2 * b) + p + p + p + p + p + p + p + p + p + p + p + p + p + p + p,
    };
#endif /* !S_SPLINT_S */
/* keep the condition the same as where we push: */
#if defined(__GNUC__) && defined(__GNUC_MINOR__) && !defined(__cplusplus)
# if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6))
 #  pragma GCC diagnostic pop
# else
#  if defined(__clang__) && (__clang__ >= 1)
 #   pragma clang diagnostic pop
#  endif /* clang */
# endif /* gcc 4.6+ */
#endif /* any gcc, but not g++ */

  target_read_memory(addr, version_buf, 4);
  version = (int)extract_unsigned_integer(version_buf, 4);

  /* we are only interested in a v9 version of the struct
     or a version 2 copy of the struct.  */
  if (version >= 9)
    image_infos_size = (offsets.dyldAllImageInfosAddress + p);
  else if (version >= 2)
    image_infos_size = offsets.jitInfo;
  else
    image_infos_size = offsets.libSystemInitialized;  /* version 1 */

  buf = (gdb_byte *)alloca(image_infos_size);
  target_read_memory(addr, buf, (int)image_infos_size);

  info->version = (uint32_t)extract_unsigned_integer(buf, 4);
  info->num_info =
    (uint32_t)extract_unsigned_integer((buf + offsets.infoArrayCount), 4);
  info->info_array =
    extract_unsigned_integer((buf + offsets.infoArray), wordsize);
  info->dyld_notify =
    extract_unsigned_integer((buf + offsets.notification), wordsize);

#if defined(TARGET_ARM)
  /* Clear bit zero of for ARM in case the dyld_notify routine is in Thumb.  */
  info->dyld_notify = (info->dyld_notify & 0xfffffffffffffffeull);
#endif /* TARGET_ARM */

  info->process_detached_from_shared_region =
    (int)extract_unsigned_integer((buf + offsets.processDetachedFromSharedRegion), 1);

  info->libsystem_initialized =
    (int)extract_unsigned_integer((buf + offsets.libSystemInitialized), 1);
  info->dyld_actual_load_address =
    extract_unsigned_integer((buf + offsets.dyldImageLoadAddress),
                             wordsize);

  /* If we are attaching to a process very early in its startup -- before
   * dyld has had a chance to update the addresses of these fields -- we
   * can tell what the adjustment will be that will be applied to them soon
   * and we can apply it by hand now before we return the addresses: */
  adjustment = INVALID_ADDRESS;
  if (info->version >= 9)
    {
      CORE_ADDR dyld_image_infos_addr;
      dyld_image_infos_addr =
        extract_unsigned_integer(buf + offsets.dyldAllImageInfosAddress,
                                 wordsize);

      /* The dyld_all_image_infos structure contains a pointer to itself.
         If dyld is loaded pie by the kernel, at initial startup it needs
         to relocate all of its addresses to their slid values.  This includes
         the dyld_notify address in the all_image_infos structure and the
         all_image_infos address itself.  If the address of the structure
         (passed in to this function) and the address of the structure itself
         do NOT agree, we need to apply the slide ourselves.  */
      if (dyld_image_infos_addr != addr)
        {
          adjustment = (addr - dyld_image_infos_addr);
        }
    }

  /* We are very early in program startup; the addresses in the
   * dyld_all_image_infos struct need to be updated by hand: */
  if ((adjustment != INVALID_ADDRESS)
      && dyld_starts_here_p(info->dyld_actual_load_address + adjustment))
    {
      if ((info->info_array != 0) && (info->info_array != INVALID_ADDRESS))
        info->info_array += adjustment;
      if ((info->dyld_notify != 0) && info->dyld_notify != INVALID_ADDRESS)
        info->dyld_notify += adjustment;
      if ((info->dyld_actual_load_address != 0)
          && (info->dyld_actual_load_address != INVALID_ADDRESS))
        info->dyld_actual_load_address += adjustment;
    }

  if (dyld_starts_here_p(info->dyld_actual_load_address))
    {
      info->dyld_intended_load_address =
        find_text_segment_load_address(info->dyld_actual_load_address);
    }
  else
    {
      info->dyld_intended_load_address = INVALID_ADDRESS;
    }
}

/* Read an array of NUM dyld_raw_info structures out of the inferior's
   dyld at ADDR and store the data in RINFO.
   In gdb, struct dyld_raw_info is the maximal size of the fields.
   In the inferior, some of the fields will be 4 or 8 bytes depending on
   the wordsize of the inferior.  */

static void
dyld_info_read_raw_data(CORE_ADDR addr, int num, struct dyld_raw_info *rinfo)
{
  gdb_byte *buf;
  int i;
  int size_of_dyld_raw_info_in_inferior;
  int wordsize = new_gdbarch_tdep(current_gdbarch)->wordsize;

  size_of_dyld_raw_info_in_inferior = (wordsize * 3);  /* three fields */

  if (num == 0)
    return;

  buf = (gdb_byte *)xmalloc(sizeof(struct dyld_raw_info) * num);

  target_read_memory(addr, buf, num * size_of_dyld_raw_info_in_inferior);

  for (i = 0; i < num; i++)
    {
      gdb_byte *ebuf = (buf + (i * size_of_dyld_raw_info_in_inferior));
      rinfo[i].addr = extract_unsigned_integer(ebuf, wordsize);
      ebuf += wordsize;
      rinfo[i].name = extract_unsigned_integer(ebuf, wordsize);
      ebuf += wordsize;
      rinfo[i].modtime = extract_unsigned_integer(ebuf, wordsize);
    }
  xfree(buf);
}

/* Given the address of the struct dyld_all_image_infos in the
   inferior, read the struct dyld_all_image_infos and the entire
   array of struct dyld_image_info's out of the inferior's dyld.
   Sets RINFO to point to this (xmalloc'ed) array of dyld_raw_info's
   and RNINFO to the number of elements in the array. */

static void
dyld_info_read_raw(struct macosx_dyld_thread_status *status,
                   struct dyld_raw_info **rinfo, int *rninfo)
{
  struct dyld_raw_infos info;
  struct dyld_raw_info *ninfo;

  dyld_read_raw_infos(status->dyld_image_infos, &info);
  /* For version 2 of the all_image_infos structure we should check
     to see if libsystem is initialized...  In truth, we only need to
     do this if it is not already initialized, but this does NOT hurt.  */
  if (info.version >= 2)
    {
      status->libsystem_initialized = info.libsystem_initialized;
      macosx_set_malloc_inited(status->libsystem_initialized);
    }

  ninfo = ((struct dyld_raw_info *)
           xmalloc(info.num_info * sizeof(struct dyld_raw_info)));

  dyld_info_read_raw_data(info.info_array, info.num_info, ninfo);

  *rinfo = ninfo;
  *rninfo = info.num_info;
}

/* */
static void
dyld_info_read(struct macosx_dyld_thread_status *status,
               struct dyld_objfile_info *info, int dyldonly)
{
  struct dyld_raw_info *rinfo = NULL;
  ptrdiff_t reserved = -1;
  int nrinfo = 0;
  int i = 0;

  if (!dyldonly)
    {
      struct dyld_objfile_entry *entry = dyld_objfile_entry_alloc(info);
      reserved = (entry - info->entries);
    }

  /* FIXME: The following code only makes sense for "dyldonly" mode, i.e.
     the very first time we are initializing the dyld structures. Why is
     it being run every time we macosx_dyld_update?  Is dyld_addr set to
     INVALID_ADDRESS after we have finished initializing dyld somehow?
     Why are we not leaking dyld_objfile_entry records? We probably compact
     them with the merge/update_shlib calls up in macosx_dyld_update.
     jmolenda/2005-05-11 */

  if (status->dyld_addr != INVALID_ADDRESS)
    {
      const char *dyld_name = NULL;
      struct dyld_objfile_entry *entry = NULL;
      char buf[PATH_MAX];
      char *new_dyld_name;

      entry = dyld_objfile_entry_alloc(info);
      dyld_name =
        (status->dyld_name != NULL) ? status->dyld_name : "/usr/lib/dyld";

      /* normal dyld-notified libraries get their names realpath'ed over
         in dyld_info_process_raw(), but dyld is a special case that never
         heads over that way, so do it here.  */

      if (realpath(dyld_name, buf) != NULL)
        dyld_name = buf;

      /* run the dyld name through the shlib pathname substitutions: */
      new_dyld_name = dyld_fix_path(dyld_name);
      if (file_exists_p(new_dyld_name))
        {
          entry->dyld_name = new_dyld_name;
        }
      else
        {
          xfree (new_dyld_name);
          entry->dyld_name = xstrdup(dyld_name);
        }

      entry->dyld_name_valid = 1;
      entry->prefix = dyld_symbols_prefix;

      entry->dyld_addr = status->dyld_addr;
      if (status->dyld_slide != INVALID_ADDRESS)
        entry->dyld_slide = status->dyld_slide;
      else
        entry->dyld_slide = 0;

      entry->dyld_valid = 1;
      entry->reason = dyld_reason_dyld;
    }

  if (dyldonly)
    return;

  if (status->dyld_image_infos != INVALID_ADDRESS)
    dyld_info_read_raw(status, &rinfo, &nrinfo);
  else
    {
      rinfo = NULL;
      nrinfo = 0;
    }

  for (i = 0; i < nrinfo; i++)
    {
      struct dyld_objfile_entry tentry;
      struct dyld_objfile_entry *pentry = NULL;
      dyld_objfile_entry_clear(&tentry);
      tentry.allocated = 1;
      tentry.reason = dyld_reason_dyld;
      if (dyld_info_process_raw(status, &tentry, rinfo[i].name,
                                rinfo[i].modtime, rinfo[i].addr) == 1)
        {
          if ((tentry.reason & dyld_reason_executable_mask)
              && (reserved >= 0))
            {
              pentry = &info->entries[reserved];
              reserved = -1;
            }
          else
            {
              pentry = dyld_objfile_entry_alloc(info);
            }
          *pentry = tentry;
        }
    }

  xfree(rinfo);

  if (reserved >= 0)
    dyld_objfile_entry_clear(&info->entries[reserved]);
}

/* */
int
macosx_dyld_update(int dyldonly)
{
#if WITH_CFM
  int ret;
#endif /* WITH_CFM */
  int libraries_changed;
  struct dyld_objfile_info previous_info, saved_info, *current_info;
  struct macosx_dyld_thread_status *status;

  static int timer_id = -1;
  struct cleanup *timer_cleanup = NULL;

  if (maint_use_timers)
    timer_cleanup = start_timer(&timer_id, "macosx_dyld_update", "");

  status = &macosx_dyld_status;
  current_info = &status->current_info;

  dyld_objfile_info_init(&previous_info);
  dyld_objfile_info_init(&saved_info);

  dyld_objfile_info_copy(&previous_info, current_info);
  dyld_objfile_info_copy(&saved_info, &previous_info);

  dyld_objfile_info_free(current_info);
  dyld_objfile_info_init(current_info);

  dyld_info_read(status, current_info, dyldonly);
#if (WITH_CFM && defined(NM_NEXTSTEP))
  CHECK_FATAL(macosx_status != NULL);
  if (inferior_auto_start_cfm_flag)
    ret = cfm_update(macosx_status->task, current_info);
  if (ret == -1) {
    ; /* ??? */
  }
#endif /* WITH_CFM && NM_NEXTSTEP */

  dyld_merge_shlibs(status, &status->path_info, &previous_info, current_info);
  dyld_update_shlibs(&status->path_info, current_info);

  if (dyld_filter_events_flag)
    {
      libraries_changed = dyld_objfile_info_compare(&saved_info, current_info);
    }
  else
    {
      libraries_changed = 1;
    }

  dyld_objfile_info_free(&saved_info);
  dyld_objfile_info_free(&previous_info);

  if (ui_out_is_mi_like_p(uiout) && libraries_changed)
    {
      struct cleanup *notify_cleanup;
      notify_cleanup =
        make_cleanup_ui_out_notify_begin_end(uiout, "shlibs-updated");
      do_cleanups(notify_cleanup);
    }

  /* Try to insert the CFM shared library breakpoint, if necessary.
     This used to be handled by dyld_symfile_loaded_hook, but that
     hook was only called when symbol files were read, not when the
     library image was added. We should probably make a hook that
     gets called on all image loads, and use that to trigger the CFM
     breakpoint. But this is expedient in the meantime, and
     inexpensive (one symbol lookup per shared library event, until
     the CFM code gets loaded). */

#if (WITH_CFM && defined(NM_NEXTSTEP))
  macosx_cfm_init(&macosx_status->cfm_status);
#endif /* WITH_CFM && NM_NEXTSTEP */

  /* OBJC_VTABLE: Init the objc trampoline stuff. This is also done in the
     dyld_symfile_loaded hook. I am not convinced by the comment above that
     this needs to be done here, and if you do NOT do it in the symfile hook
     a change in the library will NOT get noticed on run (for instance if you
     use DYLD_LIBRARY_PATH AFTER you have read in the executable.) But it will NOT
     hurt to do it here.  */

  objc_init_trampoline_observer();
  objc_init_runtime_version();

  if (maint_use_timers)
    do_cleanups(timer_cleanup);
  return libraries_changed;
}

/* The inferior has gone away. Let uss keep some state around so that
   we can still respond usefully to a "info shared", but none of the
   images, or their data, are actually in memory any longer.  */

void
macosx_dyld_mourn_inferior(void)
{
  struct dyld_objfile_entry *e = NULL;
  int i;
  struct macosx_dyld_thread_status *status = &macosx_dyld_status;
  int removed_memory_objfiles;
  DYLD_ALL_OBJFILE_INFO_ENTRIES(&status->current_info, e, i)
    {
      e->dyld_addr = 0;
      e->dyld_slide = 0;
      e->dyld_length = 0;
      e->dyld_valid = 0;
      e->pre_run_slide_addr_valid = 0;
#if WITH_CFM
      e->cfm_container = 0;
#endif /* WITH_CFM */
      /* This is NOT really right - the executable is actually
       * dyld_reason_executable - but I do NOT think it will actually cause
       * any problems: */
      if (e->reason == dyld_reason_dyld)
	e->reason = dyld_reason_init;

      /* God as my witness I do NOT know what all these names are doing in
       * a dyld_objfile_entry.  */
      /* FIXME: xfree dyld_name if I am not assigning it to text_name? */
      if (e->text_name == NULL && e->dyld_name != NULL)
	e->text_name = e->dyld_name;
      e->dyld_name = NULL;
    }

  /* Remove any objfiles that have are only in-memory (do NOT exist as
   * an on-disk file) and clear the dyld_objfile_entry struct as well.
   * We will pick them up again on re-exec if the user does that: */
  removed_memory_objfiles = 0;
  DYLD_ALL_OBJFILE_INFO_ENTRIES (&status->current_info, e, i)
    {
      /* Anything in the shared cache, even if it is in-memory-only,
       * will be present on re-launch so just leave it be: */
      if (e->objfile && e->objfile->obfd
          && bfd_mach_o_in_shared_cached_memory(e->objfile->obfd))
        {
          continue;
        }

      if (e->objfile && macosx_bfd_is_in_memory(e->objfile->obfd))
        {
          dyld_remove_objfile(e);
          dyld_objfile_entry_clear(e);
          removed_memory_objfiles = 1;
        }
    }
  if (removed_memory_objfiles)
    dyld_objfile_info_pack(&status->current_info);

  free_pre_run_memory_map(status->pre_run_memory_map);
  status->pre_run_memory_map = NULL;
}

/* */
static void
macosx_dyld_update_command(const char *args, int from_tty)
{
  macosx_dyld_update(0);
  macosx_init_dyld(&macosx_dyld_status, symfile_objfile, exec_bfd);
}

/* */
static void
map_shlib_numbers(const char *args,
                  void (*function)(struct dyld_path_info *,
                                   struct dyld_objfile_entry *,
                                   struct objfile *,
                                   int index,
                                   const char *param),
                  struct dyld_path_info *d, struct dyld_objfile_info *info)
{
  char *p, *p1, *val;
  char **argv;
  int num, match = 0;
  struct cleanup *cleanups;

  if (args == 0)
    error_no_arg(_("one or more shlib numbers"));

  p = (char *)args;
  while (isspace(*p) && (*p != '\0'))
    p++;

  if (strncmp(p, "all ", 4) == 0)
    p += 4;
  else
    {
      for (;;)
        {
          while (isspace(*p) && (*p != '\0'))
            p++;
          if (!isdigit(*p))
            break;
          while ((!isspace(*p)) && (*p != '\0'))
            p++;
        }
    }
  val = p;

  argv = buildargv(val);
  if (argv == NULL)
    error("no argument provided");
  cleanups = make_cleanup_freeargv(argv);
  if ((argv[0] == NULL) || (argv[1] != NULL))
    error("exactly one argument must be provided");
  gdb_assert(strlen(argv[0]) <= strlen(val));
  strcpy(val, argv[0]);

  if ((*p != '\0') && (p > args))
    {
      p[-1] = '\0';
    }

  p = (char *)args;

  if (strcmp(p, "all") == 0)
    {
      struct dyld_objfile_entry *e = NULL;
      unsigned int n;

      /* Hacked this macro to fix -Wsign-compare: */
      DYLD_ALL_OBJFILE_INFO_ENTRIES(info, e, n)
        (*function)(d, e, e->objfile, (n + 1U), val);

      do_cleanups(cleanups);
      return;
    }

  while (*p)
    {
      struct dyld_objfile_entry *e;
      struct objfile *o;
      int ret;

      match = 0;
      p1 = p;
      num = get_number_or_range((const char **)&p);

      if (num == 0)
        {
          warning("bad shlib number at or near '%s'", p1);
          do_cleanups(cleanups);
          return;
        }

      ret = dyld_resolve_shlib_num(info, num, &e, &o);

      if (ret < 0)
        {
          warning("no shlib %d", num);
          do_cleanups(cleanups);
          return;
        }

      (*function)(d, e, o, num, val);
    }

  do_cleanups(cleanups);

  /* this is just to use the value stored to 'match': */
  if (match != 0) {
    return;
  }
}

/* */
static void
dyld_generic_command_with_helper(const char *args,
                                 int from_tty,
                                 void (*function)(struct dyld_path_info *,
                                                  struct dyld_objfile_entry *,
                                                  struct objfile *,
                                                  int, const char *param))
{
  struct dyld_objfile_info original_info, modified_info;
  static int timer_id  = -1;
  struct cleanup *timer_cleanup = NULL;

  if (maint_use_timers)
    timer_cleanup = start_timer(&timer_id,  "dyld_generic_command_with_helper",
				"");

  dyld_objfile_info_init(&original_info);
  dyld_objfile_info_init(&modified_info);

  dyld_objfile_info_copy(&original_info,
                         &macosx_dyld_status.current_info);
  dyld_objfile_info_copy(&modified_info,
                         &macosx_dyld_status.current_info);
  dyld_objfile_info_clear_objfiles(&modified_info);

  map_shlib_numbers(args, function, &macosx_dyld_status.path_info,
                    &modified_info);

  dyld_merge_shlibs(&macosx_dyld_status,
                    &macosx_dyld_status.path_info, &original_info,
                    &modified_info);
  dyld_update_shlibs(&macosx_dyld_status.path_info, &modified_info);

  dyld_objfile_info_copy(&macosx_dyld_status.current_info,
                         &modified_info);

  dyld_objfile_info_free(&original_info);
  dyld_objfile_info_free(&modified_info);
  if (maint_use_timers)
    do_cleanups(timer_cleanup);
}

/* */
static void
add_helper(struct dyld_path_info *d, struct dyld_objfile_entry *e,
	   struct objfile *o, int index, const char *arg)
{
  if (e != NULL)
    e->load_flag = OBJF_SYM_ALL;
}

/* */
static void
dyld_add_symbol_file_command(const char *args, int from_tty)
{
  dyld_generic_command_with_helper(args, from_tty, add_helper);
}

/* */
static void
remove_helper(struct dyld_path_info *d, struct dyld_objfile_entry *e,
              struct objfile *o, int index, const char *arg)
{
  if (e != NULL)
    e->load_flag = (OBJF_SYM_NONE | dyld_minimal_load_flag(d, e));
}

/* */
static void
dyld_remove_symbol_file_command(const char *args, int from_tty)
{
  dyld_generic_command_with_helper(args, from_tty, remove_helper);
}

/* objfile_set_load_state

   Given objfile O, this changes the load state to LOAD_STATE.  This
   will cause the current objfile to get tossed, and a new version
   read in.  If LOAD_STATE matches the objfile's current load state,
   this is a no-op, however.  Returns the original load state, or
   -1 if there was an error.  */

int
dyld_objfile_set_load_state(struct objfile *o, int load_state)
{
  struct dyld_objfile_entry *e = NULL;
  int i, found_it = -1;

  DYLD_ALL_OBJFILE_INFO_ENTRIES(&macosx_dyld_status.current_info, e, i)
    if (e->objfile == o)
      {
	found_it = e->load_flag;
        set_load_state_1(e, &macosx_dyld_status.path_info, i + 1, load_state);
        break;
      }
  return found_it;
}

static void
apply_load_rules_helper(struct dyld_path_info *d,
                        struct dyld_objfile_entry *e, struct objfile *o,
                        int index, const char *arg)
{
  int load_state;

  if (e == NULL)
    return;

  load_state =
    (dyld_default_load_flag(d, e) | dyld_minimal_load_flag(d, e));
  set_load_state_1(e, d, index, load_state);
}

/* */
static void
dyld_apply_load_rules_command(const char *args, int from_tty)
{
  map_shlib_numbers(args, apply_load_rules_helper,
                    &macosx_dyld_status.path_info,
                    &macosx_dyld_status.current_info);

 /* Since we have change the load state of some libraries, we should
     see if any of our pending breakpoints will now take.  */
  re_enable_breakpoints_in_shlibs(0);

}

static void
set_load_state_1(struct dyld_objfile_entry *e,
                 const struct dyld_path_info *d,
                 int index, int load_state)
{
  struct bfd *tmp_bfd;
  enum dyld_reload_result state_change;

  int old_pre_run_slide_addr_valid = 0;
  CORE_ADDR old_pre_run_slide_addr = 0UL;
  int old_image_addr_valid = 0;
  CORE_ADDR old_image_addr = 0UL;

  e->load_flag = load_state;

  /* If there is no existing objfile, load it (if appropriate) & return: */
  if (e->objfile == NULL)
    {
      dyld_load_library(d, e);
      if (e->abfd)
        dyld_load_symfile(e);
      return;
    }

  state_change = dyld_should_reload_objfile_for_flags(e);
  if (state_change == DYLD_NO_CHANGE)
    return;

  /* This is a bit of a hack, but I do NOT want to have to throw away and
     reconstitute the bfd.  So I am hiding it from dyld_remove_objfile
     in TMP_BFD.  I may give up on this!  */
  tmp_bfd = e->objfile->obfd;
  e->objfile->obfd = NULL;

  /* Turns out the bfd's strtab for the fake stabstr section is
     actually a pointer to the version allocated on the objfile's
     objstack!!!  So I need to null this out so it gets reset.  */

  /* Only hide away the string table if this IS a mach_o file.  It might be a CFM binary. */

  if (bfd_get_flavour(tmp_bfd) == bfd_target_mach_o_flavour)
    {
      int ret;
      struct bfd_mach_o_load_command *gsymtab;

      ret = bfd_mach_o_lookup_command(tmp_bfd, BFD_MACH_O_LC_SYMTAB, &gsymtab);
      if (ret != 1)
	{
	  warning("Error fetching LC_SYMTAB load command from object file \"%s\"",
                  tmp_bfd->filename);
	}
      else if (gsymtab->command.symtab.strtab == DBX_STRINGTAB(e->objfile))
	gsymtab->command.symtab.strtab = NULL;
    }

  tell_breakpoints_objfile_changed(e->objfile);
  tell_objc_msgsend_cacher_objfile_changed(e->objfile);

  /* FIXME: check state_change, and remove the varobj's that depend
     on the objfile now as well.  */

  /* FIXME: This hiding & restoring the pre_run_slide & image_addr
     state is a hack.  The pre_run_slide is only set in load
     library, not load_symfile, so they will get lost if we allow
     dyld_clear_objfile to zero them here.  You also have to set the
     image_addr before doing load_symfile_internal, or the library
     address will not get reported properly (more specifically,
     loaded_addrisoffset will get set to 1 here, which is wrong.)

     I suspect that the pre run sliding is not getting done in quite
     the correct place because we should NOT need to do this. But for
     now...  */

  if (e->reason == dyld_reason_init)
    {
      old_pre_run_slide_addr_valid = e->pre_run_slide_addr_valid;
      old_pre_run_slide_addr = e->pre_run_slide_addr;
      old_image_addr = e->image_addr;
      old_image_addr_valid = e->image_addr_valid;
    }

  dyld_clear_objfile(e);

  e->abfd = tmp_bfd;

  if (e->reason == dyld_reason_init)
    {
      asection *text_sect;
      e->pre_run_slide_addr_valid = old_pre_run_slide_addr_valid;
      e->pre_run_slide_addr = old_pre_run_slide_addr;
      text_sect = bfd_get_section_by_name(e->abfd, TEXT_SEGMENT_NAME);
      if (text_sect != NULL)
        {
          e->image_addr = bfd_section_vma(e->abfd, text_sect);
          e->image_addr_valid = 1;
        }

    }

  dyld_load_symfile_preserving_objfile(e);

  /* Now if we modified the image_addr & image_addr_valid to get
     dyld_load_symfile to work right, reset them here.  */

  if (e->reason == dyld_reason_init)
    {
      e->image_addr = old_image_addr;
      e->image_addr_valid = old_image_addr_valid;
    }

  if (ui_out_is_mi_like_p(uiout))
    {
      struct cleanup *notify_cleanup;
      notify_cleanup =
        make_cleanup_ui_out_notify_begin_end(uiout,
                                             "shlib-state-modified");
      dyld_print_entry_info(e, index, 0);
      do_cleanups(notify_cleanup);
    }

}

static void
set_load_state_helper(struct dyld_path_info *d, struct dyld_objfile_entry *e,
                      struct objfile *o, int index, const char *arg)
{
  int load_state;

  if (e == NULL)
    return;

  load_state = dyld_parse_load_level(arg);
  load_state |= dyld_minimal_load_flag(d, e);
  set_load_state_1(e, d, index, load_state);
}

/* */
static void
dyld_set_load_state_command(const char *args, int from_tty)
{
  map_shlib_numbers(args, set_load_state_helper,
                    &macosx_dyld_status.path_info,
                    &macosx_dyld_status.current_info);
  /* Since we have change the load state of some libraries, we should
     see if any of our pending breakpoints will now take.  */
  re_enable_breakpoints_in_shlibs(0);

}

static void
section_info_helper(struct dyld_path_info *d, struct dyld_objfile_entry *e,
                    struct objfile *o, int index, const char *arg)
{
  if (o != NULL)
    {
      print_section_info_objfile(o);
    }
  else
    {
      ui_out_begin(uiout, ui_out_type_list, "section-info");
      ui_out_end(uiout, ui_out_type_list);
    }

  if (e != NULL)
    {
#if (WITH_CFM && defined(NM_NEXTSTEP))
      if (inferior_auto_start_cfm_flag && (e->cfm_container != 0))
        {
          NCFragContainerInfo container;
          struct cfm_parser *parser;
          unsigned long section_index;
          int ret;
          parser = &macosx_status->cfm_status.parser;

          ret =
            cfm_fetch_container_info(parser, e->cfm_container, &container);
          if (ret != 0)
            return;

          ui_out_begin(uiout, ui_out_type_list, "cfm-section-info");

          for (section_index = 0; section_index < container.sectionCount;
               section_index++)
            {
              NCFragSectionInfo section;
              NCFragInstanceInfo instance;

              /* What is this supposed to be initialized to, anyway?  */
              instance.address = 0;

              ret =
                cfm_fetch_container_section_info(parser, e->cfm_container,
                                                 section_index, &section);
              if (ret != 0)
                break;

              ui_out_begin(uiout, ui_out_type_list, "section");

              ui_out_text(uiout, "\t");
              ui_out_field_core_addr(uiout, "addr", instance.address);
              ui_out_text(uiout, " - ");
              ui_out_field_core_addr(uiout, "endaddr",
                                     instance.address + section.length);
              if (info_verbose)
                {
                  ui_out_text(uiout, " @ ");
                  ui_out_field_core_addr(uiout, "filepos", 0);
                }
              ui_out_text(uiout, " is ");
              ui_out_field_string(uiout, "name", "unknown");
# if 0
              if (p->objfile->obfd != abfd)
                {
                  ui_out_text(uiout, " in ");
                  ui_out_field_string(uiout, "filename",
                                      bfd_get_filename(p->bfd));
                }
# endif /* 0 */
              ui_out_text(uiout, "\n");

              ui_out_end(uiout, ui_out_type_list);     /* "section" */
            }

          ui_out_end(uiout, ui_out_type_list); /* "cfm-section-info" */
        }
#endif /* WITH_CFM && NM_NEXTSTEP */
    }
}

/* */
static void
dyld_section_info_command(const char *args, int from_tty)
{
  map_shlib_numbers(args, section_info_helper,
                    &macosx_dyld_status.path_info,
                    &macosx_dyld_status.current_info);
}


/* The "info sharedlibrary" command is overloaded a bit much.

   "info sharedlibrary" by itself will print some help text, followed
   by all your shared libraries.

   "info sharedlibrary [all|dyld|cfm|raw-cfm|raw-dyld]" list certain types of
   shared libraries, sans the help text.

   "info sharedlibrary <address>" show the shared library that contains
   that address.

   The gdb command system will run seperate routines for the
   "info sharedlibrary [all|dyld|cfm|raw-cfm|raw-dyld]" case, but we need
   to handle the other two here.  */

static void
info_sharedlibrary_command(const char *args, int from_tty)
{
  char **argv = (char **)NULL;
  struct cleanup *wipe;

  wipe = make_cleanup(null_cleanup, NULL);

  if (args != NULL)
    if ((argv = buildargv(args)) != NULL)
      make_cleanup_freeargv(argv);

  if ((args == NULL) || (argv == NULL) || (argv[0] == NULL)
      || !strcmp(argv[0], ""))
    {
      dyld_print_status_info(&macosx_dyld_status,
                             (dyld_reason_all_mask | dyld_reason_user),
                             args);
    }
  else
    {
      CORE_ADDR address;

      address = parse_and_eval_address(argv[0]);
      info_sharedlibrary_address(address);
    }

  do_cleanups(wipe);
}

/* Given an address, find the shared library (or executable) that contains
   this address.  Akin to the old metrowerks-address-to-name command.  */

static void
info_sharedlibrary_address(CORE_ADDR address)
{
  struct dyld_objfile_info *s = &macosx_dyld_status.current_info;
  int shlibnum = 1;
  int found_dylib = 0;
  int baselen;
  int i = 0;
  struct obj_section *osection;

  baselen = dyld_shlib_info_basename_length(s, dyld_reason_all_mask);
  if (baselen < 12)
    baselen = 12;

  osection = find_pc_sect_section(address, NULL);
  if (osection != NULL)
    {
      for (i = 0; i < s->nents; i++)
        {
          if (osection->objfile == s->entries[i].objfile)
            {
              found_dylib = 1;
              break;
            }
          shlibnum++;
        }
    }

  if (found_dylib)
    dyld_print_entry_info(&s->entries[i], shlibnum, baselen);
  else
    error("[unknown]");
}

/* */
static void
info_sharedlibrary_all_command(const char *args, int from_tty)
{
  dyld_print_shlib_info(&macosx_dyld_status.current_info,
                        (dyld_reason_all_mask | dyld_reason_user), 1, args);
}

/* */
static void
info_sharedlibrary_dyld_command(const char *args, int from_tty)
{
  dyld_print_shlib_info(&macosx_dyld_status.current_info,
                        dyld_reason_dyld, 1, args);
}

#if (WITH_CFM && defined(NM_NEXTSTEP))
static void
info_sharedlibrary_cfm_command(const char *args, int from_tty)
{
  dyld_print_shlib_info(&macosx_dyld_status.current_info,
                        dyld_reason_cfm, 1, args);
}

static void
info_sharedlibrary_raw_cfm_command(const char *args, int from_tty)
{
  task_t task = macosx_status->task;
  struct dyld_objfile_info info;

  dyld_objfile_info_init(&info);
  cfm_update(task, &info);

  dyld_print_shlib_info(&info, dyld_reason_cfm, 1, args);
}
#endif /* WITH_CFM && NM_NEXTSTEP */

/* */
static void
info_sharedlibrary_raw_dyld_command(const char *args, int from_tty)
{
  struct dyld_objfile_info info;

  dyld_objfile_info_init(&info);
  dyld_info_read(&macosx_dyld_status, &info, 0);

  dyld_print_shlib_info(&info, dyld_reason_dyld, 1, args);
}

int
dyld_lookup_and_bind_function(char *name)
{
  if (!target_has_execution)
    return 0;

  /* APPLE LOCAL: dyld USED to lazily bind modules within a shared
     library, so just because a library had been loaded and initialized,
     that did NOT mean you could actually call any given function. You
     had to make sure that function was already bound.  Since Tiger,
     that is NOT done any more, all the functions get bound up when
     the library loads, and NSLookupAndBindSymbol does not actually
     do anything anymore. So we just return 1 here.
     FIXME: Really should just rip out the whole bind_function target
     vector stuff I had to add for this, but I do not have time to
     do that right now.  */

  return 1;
}

/* */
static void ATTR_NORETURN
dyld_cache_symfiles_command(const char *args ATTRIBUTE_UNUSED,
			    int from_tty ATTRIBUTE_UNUSED)
{
  error(_("Cached symfiles are not supported on this configuration of GDB."));
}

/* */
static void ATTR_NORETURN
dyld_cache_symfile_command(const char *args ATTRIBUTE_UNUSED,
			   int from_tty ATTRIBUTE_UNUSED)
{
  error(_("Cached symfiles are not supported on this configuration of GDB."));
}

/* */
void
update_section_tables(void)
{
  update_section_tables_dyld(&macosx_dyld_status.current_info);
}

/* */
void
update_section_tables_dyld(struct dyld_objfile_info *s)
{
  struct target_ops *target;
  struct objfile *o;
  struct obj_section *osection;
  int nsections, csection;
  ptrdiff_t osections;
  int i;
  struct dyld_objfile_entry *j = NULL;

  target = &exec_ops;

  /* Count the total # of sections. */

  nsections = 0;
  ALL_OBJFILES(o)
    ALL_OBJFILE_OSECTIONS(o, osection)
      nsections++;

  osections = (target->to_sections_end - target->to_sections);
  target_resize_to_sections(target, (int)(nsections - osections));
  gdb_assert((target->to_sections + nsections) == target->to_sections_end);

#define ADD_SECTION(osection) \
        { \
        gdb_assert(osection != NULL); \
        gdb_assert(osection->objfile != NULL); \
        target->to_sections[csection].addr = osection->addr; \
        target->to_sections[csection].endaddr = osection->endaddr; \
        target->to_sections[csection].the_bfd_section = osection->the_bfd_section; \
        target->to_sections[csection].abfd = osection->objfile->obfd; \
        csection++; \
        }

  csection = 0;

  /* First, add all the shared libraries brought in from the
     dyld_objfile_entry code, in the order that they are printed by
     dyld_print_shlib_info (that way, the exec file will always go
     first, among other things).  When there are overlapping sections,
     GDB just uses the first one. */

  DYLD_ALL_OBJFILE_INFO_ENTRIES(s, j, i)
    {
      if (j->objfile != NULL)
        ALL_OBJFILE_OSECTIONS(j->objfile, osection)
          ADD_SECTION(osection);

      if (j->commpage_objfile != NULL)
        ALL_OBJFILE_OSECTIONS(j->commpage_objfile, osection)
          ADD_SECTION(osection);
    }

  /* Then, go through and match all the objfiles not managed by the
     dyld_objfile_entry code ... again, matching the order used by
     dyld_print_shlib_info. */

  ALL_OBJFILES(o)
    {
      int found = 0;

      DYLD_ALL_OBJFILE_INFO_ENTRIES(s, j, i)
        {
          if ((j->objfile == o) || (j->commpage_objfile == o))
            found = 1;
        }

      if (!found)
        ALL_OBJFILE_OSECTIONS(o, osection)
          ADD_SECTION(osection);
    }

#undef ADD_SECTION

  gdb_assert(csection == nsections);
}

/* FIXME: This is used in the test to determine whether to re-enable
 * a shlib breakpoint in re_enable_breakpoints_in_shlibs.  I had to
 * disable that test.  Re-enable it when this function works: */
const char * ATTRIBUTE_CONST
macosx_pc_solib(CORE_ADDR addr ATTRIBUTE_UNUSED)
{
  return (const char *)NULL;
}


/* Fixup dyld paths by seaching our list of search/replace path pairs.
   This is used for remote targets that may have executables whose dyld
   paths do not match those on the host machine.

   The returned char* always points to newly xmalloc'ed memory even if
   there was no change to the string.  It is the caller's responsibility
   to free it.  */

char *
dyld_fix_path(const char *path)
{
  int i;
  /* Check if we have any dyld path substitutions to perform: */
  if (shlib_path_substitutions)
    {
      /* First check to make sure we are not double-substituting a path */
      for (i = 0; shlib_path_substitutions[i] != NULL; i += 2)
        {
          const char *prefix = shlib_path_substitutions[i];
          const char *new_prefix = shlib_path_substitutions[i + 1];
          if (strncmp(path, new_prefix, strlen(new_prefix)) == 0)
            {
              return xstrdup(path);
            }
          else if (prefix == NULL)
            {
              ; /* do nothing; just use value stored to prefix */
            }
        }

      /* Step through all path substitution pairs: */
      for (i = 0; shlib_path_substitutions[i] != NULL; i += 2)
	{
          const char *prefix = shlib_path_substitutions[i];
          const char *new_prefix = shlib_path_substitutions[i + 1];
	  /* See if the path passed in starts with one of our search paths.  */
	  if (strncmp(path, prefix, strlen(prefix)) == 0)
	    {
	      /* We have a match, now we will substitute the new value into
	         the existing path.  */
	      size_t old_prefix_length = strlen(prefix);
	      size_t new_path_length = (strlen(new_prefix) + strlen(path)
                                        - old_prefix_length + 1UL);
              char *new_path = (char *)xmalloc(new_path_length);
	      if (new_path)
		{
		  strcpy(new_path, new_prefix);
		  strcat(new_path, (path + old_prefix_length));
                  return new_path;
		}
	      else
		{
		  /* FIXME: verify that the warning here is appropriate
                   * for being out of memory: */
                  warning("ran out of memory while fixing path for dyld");
		}
	    }
	}
    }
    return xstrdup(path);
}

/* APPLE LOCAL: This boolean tells us whether the malloc library is
   initialized.  Turns out on Leopard we get the first dyld notification
   before this has happened, and calling malloc at this point will
   cause the target to fall over.
   We will use this value in macosx_check_safe_call to override any of
   the other checks we might do.  */

static int malloc_inited_p;

void
macosx_set_malloc_inited(int new_val)
{
  malloc_inited_p = new_val;
}

int
macosx_get_malloc_inited(void)
{
  /* If it is uninitialized, use this as an opportunity to double-check...
     Otherwise we only re-examine this boolean from dyld when we have a
     shlib load notification -- if we have a trivial program that only directly
     links to dylibs, we may only get the initial libsystem et al load
     notification at which point libsystem has not yet been initialized.
     e.g. in the testsuite's "ivars.exp" test case.  */
  if (malloc_inited_p == 0) {
    macosx_init_addresses(&macosx_dyld_status);
  }

  return malloc_inited_p;
}

struct cmd_list_element *dyldlist = NULL;
struct cmd_list_element *setshliblist = NULL;
struct cmd_list_element *showshliblist = NULL;
struct cmd_list_element *infoshliblist = NULL;
struct cmd_list_element *shliblist = NULL;
struct cmd_list_element *maintenanceshliblist = NULL;

/* */
static void
maintenance_sharedlibrary_command(const char *arg, int from_tty)
{
  printf_unfiltered("\"maintenance sharedlibrary\" must be followed by the name of a sharedlibrary command.\n");
  help_list(maintenanceshliblist, "maintenance sharedlibrary ",
            (enum command_class)-1, gdb_stdout);
}

/* */
static void
sharedlibrary_command(const char *arg, int from_tty)
{
  printf_unfiltered("\"sharedlibrary\" must be followed by the name of a sharedlibrary command.\n");
  help_list(shliblist, "sharedlibrary ", (enum command_class)-1,
            gdb_stdout);
}

/* */
static void
macosx_set_auto_start_dyld(const char *args, int from_tty,
                           struct cmd_list_element *c)
{
  /* Do NOT want to bother with stopping the target to set this... */
  if (target_executing)
    return;

  /* If we are so early on that the macosx_status has NOT gotten allocated
     yet, this will fail, but we also will NOT have needed to do anything,
     so we can safely just exit. */
#if defined(NM_NEXTSTEP)
  if (macosx_status == NULL) {
    return;
  }
#endif /* NM_NEXTSTEP */

  /* If we are turning off watching dyld, we need to remove
     the breakpoint... */

  if (!inferior_auto_start_dyld_flag)
    {
      macosx_clear_start_breakpoint();
      return;
    }

  /* If the inferior is not running, then all we have to do
     is set the flag, which is done in generic code. */

  if (ptid_equal(inferior_ptid, null_ptid))
    return;

  macosx_solib_add(NULL, 0, NULL, 0);
}

/* Set the pairs of pathname substitutions to use when loading
   shared libraries. This is used for remote targets where you
   may have the dependant shared libraries installed at a path
   that does NOT match the currently shared libraries on your
   host machine. Mach executables contain paths to shared
   libraries in LC_LOAD_DYLINKER and LC_LOAD_DYLIB load commands.
   Also when hitting the dyld breakpoint, the paths reported by
   dyld will be paths that match the LC_LOAD_DYLIB load commands
   of any used shared libraries.  */

static void
set_shlib_path_substitutions_cmd(const char *args, int from_tty,
                                 struct cmd_list_element * c)
{
  int success = 0;

  /* Free our old path array if we had one: */
  if (shlib_path_substitutions != NULL)
    {
      freeargv(shlib_path_substitutions);
      shlib_path_substitutions = NULL;
    }

  /* Create our new path array and check it: */
  if (shlib_path_subst_cmd_args && shlib_path_subst_cmd_args[0])
    {
      shlib_path_substitutions = buildargv(shlib_path_subst_cmd_args);
      if (shlib_path_substitutions && shlib_path_substitutions[0] != NULL)
	{
	  /* If we got some valid path substitutions, the we must have an
           * even number of them: */
	  int i;
	  success = 1;
	  for (i = 0; success && (shlib_path_substitutions[i] != NULL);
               i += 2)
	    {
	      success = shlib_path_substitutions[i + 1] != NULL;
	    }
	}

      if (!success)
	{
	  warning("An even number of paths must be given, surround paths "
		  "constaining spaces with single or double quotes.");
	  freeargv(shlib_path_substitutions);
	  shlib_path_substitutions = NULL;
	}
    }
}

/* */
static void
show_shlib_path_substitutions_cmd(struct ui_file *file, int from_tty,
                                  struct cmd_list_element *c,
                                  const char *value ATTRIBUTE_UNUSED)
{
  char *from;
  char *to;
  int i = 0;

  if (shlib_path_substitutions)
    {
      fprintf_filtered(file, _("Current shared library path substitions:\n"));
      for (i = 0; shlib_path_substitutions[i] != NULL; i += 2)
	{
	  from = shlib_path_substitutions[i];
	  to = shlib_path_substitutions[i+1];
	  fprintf_filtered(file, _("  [%i] '%s' -> '%s'\n"),
                           ((i / 2) + 1), from, to);
	}
    }
  else
    fprintf_filtered(file, _("No shared library path substitions are currently set.\n"));

  if (shlib_path_subst_cmd_args)
    fprintf_filtered(file, _("\nLast shared library pathname substition command was:\n"
                             "set shlib-path-substitutions%s%s\n"),
		     (shlib_path_subst_cmd_args[0] ? " " : ""),
		     shlib_path_subst_cmd_args);
}

void
_initialize_macosx_nat_dyld(void)
{
  macosx_dyld_thread_init(&macosx_dyld_status);

  dyld_stderr = fdopen(fileno(stderr), "w");

  add_prefix_cmd("sharedlibrary", class_run, sharedlibrary_command,
                 "Commands for shared library manipulation.",
                 &shliblist, "sharedlibrary ", 0, &cmdlist);

  add_prefix_cmd("sharedlibrary", class_maintenance,
                 maintenance_sharedlibrary_command,
                 "Commands for internal sharedlibrary manipulation.",
                 &maintenanceshliblist, "maintenance sharedlibrary ", 0,
                 &maintenancelist);

  add_cmd("apply-load-rules", class_run, dyld_apply_load_rules_command,
          "Apply the current load-rules to the existing shared library state.",
          &shliblist);

  add_cmd("add-symbol-file", class_run, dyld_add_symbol_file_command,
          "Add a symbol file.", &shliblist);

  add_cmd("remove-symbol-file", class_run, dyld_remove_symbol_file_command,
          "Remove a symbol file.", &shliblist);

  add_cmd("set-load-state", class_run, dyld_set_load_state_command,
          "Set the load level of a library (given by the index from \"info sharedlibrary\").",
          &shliblist);

  add_cmd("section-info", class_run, dyld_section_info_command,
          "Get the section info for a library (given by the index from \"info sharedlibrary\").",
          &shliblist);

  add_cmd("cache-purge", class_obscure, dyld_cache_purge_command,
          "Purge all symbols for DYLD images cached by GDB.",
          &maintenanceshliblist);

  add_cmd("update", class_run, macosx_dyld_update_command,
          "Process all pending DYLD events.", &shliblist);

  add_prefix_cmd("sharedlibrary", no_class, info_sharedlibrary_command,
                 "Generic command for shlib information.\n`info sharedlibrary ADDRESS' will show the dylib containing ADDRESS.",
                 &infoshliblist, "info sharedlibrary ", 1, &infolist);

  add_cmd("all", no_class, info_sharedlibrary_all_command,
          "Show current DYLD state.", &infoshliblist);
  add_cmd("dyld", no_class, info_sharedlibrary_dyld_command,
          "Show current DYLD state.", &infoshliblist);
#if defined(WITH_CFM) && WITH_CFM
  add_cmd("cfm", no_class, info_sharedlibrary_cfm_command,
          "Show current CFM state.", &infoshliblist);
  add_cmd("raw-cfm", no_class, info_sharedlibrary_raw_cfm_command,
          "Show current CFM state.", &infoshliblist);
#endif /* WITH_CFM */
  add_cmd("raw-dyld", no_class, info_sharedlibrary_raw_dyld_command,
          "Show current CFM state.", &infoshliblist);

  add_prefix_cmd("sharedlibrary", no_class, not_just_help_class_command,
                 "Generic command for setting shlib settings.",
                 &setshliblist, "set sharedlibrary ", 0, &setlist);

  add_prefix_cmd("sharedlibrary", no_class, not_just_help_class_command,
                 "Generic command for showing shlib settings.",
                 &showshliblist, "show sharedlibrary ", 0, &showlist);

  add_setshow_boolean_cmd("filter-events", class_obscure,
			  &dyld_filter_events_flag, _("\
Set if GDB should filter shared library events to a minimal set."), _("\
Show if GDB should filter shared library events to a minimal set."), NULL,
			  NULL, NULL,
			  &setshliblist, &showshliblist);

  add_setshow_boolean_cmd("preload-libraries", class_obscure,
			  &dyld_preload_libraries_flag, _("\
Set if GDB should pre-load symbols for DYLD libraries."), _("\
Show if GDB should pre-load symbols for DYLD libraries."), NULL,
			  NULL, NULL,
			  &setshliblist, &showshliblist);

  add_setshow_boolean_cmd("load-dyld-symbols", class_obscure,
			  &dyld_load_dyld_symbols_flag, _("\
Set if GDB should load symbol information for the dynamic linker."), _("\
Show if GDB should load symbol information for the dynamic linker."), NULL,
			  NULL, NULL,
			  &setshliblist, &showshliblist);

  add_setshow_boolean_cmd("load-dyld-shlib-symbols", class_obscure,
			  &dyld_load_dyld_shlib_symbols_flag, _("\
Set if GDB should load symbol information for DYLD-based shared libraries."), _("\
Show if GDB should load symbol information for DYLD-based shared libraries."), NULL,
			  NULL, NULL,
			  &setshliblist, &showshliblist);
#if defined(__APPLE__) && defined(APPLE_MERGE)
  deprecate_cmd(cmd, "set sharedlibrary load-rules");
#endif /* __APPLE__ && APPLE_MERGE */

  add_setshow_boolean_cmd("load-cfm-shlib-symbols", class_obscure,
			  &dyld_load_cfm_shlib_symbols_flag, _("\
Set if GDB should load symbol information for CFM-based shared libraries."), _("\
Show if GDB should load symbol information for CFM-based shared libraries."), NULL,
			  NULL, NULL,
			  &setshliblist, &showshliblist);
#if defined(__APPLE__) && defined(APPLE_MERGE)
  deprecate_cmd(cmd, "set sharedlibrary load-rules");
#endif /* __APPLE__ && APPLE_MERGE */

  add_setshow_string_cmd("dyld-symbols-prefix", class_obscure,
                         (char **)&dyld_symbols_prefix, _("\
Set the prefix that GDB should prepend to all symbols for the dynamic linker."), _("\
Show the prefix that GDB should prepend to all symbols for the dynamic linker."), NULL,
                         NULL, NULL,
                         &setshliblist, &showshliblist);

  dyld_symbols_prefix = xstrdup(dyld_symbols_prefix);

  add_setshow_boolean_cmd("always-read-from-memory", class_obscure,
			  &dyld_always_read_from_memory_flag, _("\
Set if GDB should always read loaded images from the inferior's memory."), _("\
Show if GDB should always read loaded images from the inferior's memory."), NULL,
			  NULL, NULL,
			  &setshliblist, &showshliblist);

  add_setshow_boolean_cmd("print-basenames", class_obscure,
			  &dyld_print_basenames_flag, _("\
Set if GDB should print the basenames of loaded files when printing progress messages."), _("\
Show if GDB should print the basenames of loaded files when printing progress messages."), NULL,
			  NULL, NULL,
			  &setshliblist, &showshliblist);

  add_setshow_string_cmd("load-rules", class_support,
			 &dyld_load_rules, _("\
Set the rules governing the level of symbol loading for shared libraries.\n\
 * Each load rule is a triple.\n\
 * The command takes a flattened list of triples.\n\
 * The first two elements of the triple specify the library, by giving \n\
      - \"who loaded it\" (i.e. dyld, cfm or all) in the first element, \n\
      - and a regexp to match the library name in the second. \n\
 * The last element specifies the level of loading for that library\n\
      - The options are:  all, extern, container or none.\n\
\n\
Example: To load only external symbols from all dyld-based system libraries, use: \n\
    set sharedlibrary load-rules dyld ^/System/Library.* extern\n"),
			 "XYZ", NULL, NULL, NULL,
			 &setshliblist, &showshliblist);

  add_setshow_string_cmd("minimal-load-rules", class_support,
			 &dyld_minimal_load_rules, _("\
Set the minimal DYLD load rules.  These prime the main list.\n\
gdb relies on some of these for proper functioning, so do NOT remove elements from it\n\
unless you know what you are doing."),
			 "xyz", NULL, NULL, NULL,
			 &setshliblist, &showshliblist);

  add_setshow_zinteger_cmd("dyld", class_obscure,
			   &dyld_debug_flag, _("\
Set if printing dyld communication debugging statements."), _("\
Show if printing dyld communication debugging statements."), NULL,
			   NULL, NULL,
			   &setdebuglist, &showdebuglist);

  dyld_minimal_load_rules =
    xstrdup("\"dyld\" \"CarbonCore$\\\\|CarbonCore_[^/]*$\" all \".*\" \"dyld$\" extern \".*\" \".*\" none");
  dyld_load_rules = xstrdup("\".*\" \".*\" all");

  add_setshow_zinteger_cmd("stop-on-shlibs-added", class_support,
			   &dyld_stop_on_shlibs_added, _("\
Set if a shlib event should be reported on a shlibs-added event."), _("\
Show if a shlib event should be reported on a shlibs-added event."), NULL,
			   NULL, NULL,
			   &setshliblist, &showshliblist);

  add_setshow_zinteger_cmd("stop-on-shlibs-updated", class_support,
			   &dyld_stop_on_shlibs_updated, _("\
Set if a shlib event should be reported on a shlibs-updated event."), _("\
Show if a shlib event should be reported on a shlibs-updated event."), NULL,
			   NULL, NULL,
			   &setshliblist, &showshliblist);

  add_setshow_zinteger_cmd("combine-shlibs-added", class_support,
			   &dyld_combine_shlibs_added, _("\
Set if GDB should combine shlibs-added events from the same image into a single event."), _("\
Show if GDB should combine shlibs-added events from the same image into a single event."), NULL,
			   NULL, NULL,
			   &setshliblist, &showshliblist);

  add_setshow_zinteger_cmd("reload-on-downgrade", class_support,
			   &dyld_reload_on_downgrade_flag, _("\
Set if GDB should re-read symbol files in order to remove symbol information."), _("\
Show if GDB should re-read symbol files in order to remove symbol information."), NULL,
			   NULL, NULL,
			   &setshliblist, &showshliblist);


 add_setshow_boolean_cmd("inferior-auto-start-dyld", class_obscure,
                         &inferior_auto_start_dyld_flag, _("\
Set if GDB should enable debugging of dyld shared libraries."), _("\
Show if GDB should enable debugging of dyld shared libraries."), NULL,
                         macosx_set_auto_start_dyld, NULL,
                         &setlist, &showlist);

/* Do NOT add #if WITH_CFM around this or xcode will get an error when
   it tries to set this setting at startup.  */
  add_setshow_boolean_cmd("inferior-auto-start-cfm", class_obscure,
			  &inferior_auto_start_cfm_flag, _("\
Set if GDB should enable debugging of CFM shared libraries."), _("\
Show if GDB should enable debugging of CFM shared libraries."), NULL,
			  NULL, NULL,
			  &setlist, &showlist);

  add_cmd("cache-symfiles", class_run, dyld_cache_symfiles_command,
          "Generate persistent caches of symbol files for the current executable state.",
          &shliblist);

  add_cmd("cache-symfile", class_run, dyld_cache_symfile_command,
          "Generate persistent caches of symbol files for a specified executable.\n"
          "usage: cache-symfile <source> <target> [prefix]", &shliblist);

  add_setshow_string_cmd("shlib-path-substitutions", class_support,
			 &shlib_path_subst_cmd_args, _("\
Set path substitutions to be used when loading shared libraries."), _("\
Show path substitutions to be used when loading shared libraries."), _("\
Commonly used for remote debugging a target that was built with shared\n\
libraries that do not match those currently installed on the host machine.\n\
Any needed shared libraries can be copied from the remote machine to a local\n\
directory, or the root of the remote machine could be mounted over a network.\n\
\n\
The string substitutions are space separated pairs of paths where each\n\
string can be surrounded by double quotes if a path contains spaces.\n\
\n\
For example, if you are running gdb on a 10.4 machine, and you are remote\n\
debugging to a machine with 10.3 installed, you could copy the files to a\n\
local directory (/tmp/remote) and use the following command:\n\
\n\
shlib-path-substitutions /usr /tmp/remote/usr /System /tmp/remote/System"),
			 set_shlib_path_substitutions_cmd,
			 show_shlib_path_substitutions_cmd,
			 &setlist, &showlist);

  add_setshow_boolean_cmd("pre-slide-libraries", class_obscure,
			  &pre_slide_libraries_flag, _("\
Set if GDB should pre-slide libraries to unique addresses before running."), _("\
Show if GDB should pre-slide libraries to unique addresses before running."),
                          _("\
When multiple libraries have a load address of 0x0, they will overlap before\n\
execution has started.  When execution starts, dyld will slide them all to\n\
unique locations.  Before execution, when you try to disassemble or examine\n\
the contents of these libraries, you may have unexpected results from gdb as\n\
it gets the different overlapping libraries confused.  At this time, the \n\
pre-sliding feature still has some rough edges and is not recommended for\n\
use by default."),
                          NULL, NULL,
			  &setshliblist, &showshliblist);
}

/* EOF */
