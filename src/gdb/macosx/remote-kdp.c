/* gdb/macosx/remote-kdp.c
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

#if defined(TARGET_I386)
# define KDP_TARGET_I386 1
#else
# undef KDP_TARGET_I386
#endif /* TARGET_I386 */

#if defined(TARGET_POWERPC)
# define KDP_TARGET_POWERPC 1
#else
# undef KDP_TARGET_POWERPC
#endif /* TARGET_POWERPC */

#if defined(TARGET_ARM)
# define KDP_TARGET_ARM 1
#else
# undef KDP_TARGET_ARM
#endif /* TARGET_ARM */

#if !defined(NO_POISON) && defined(POISON_FREE_TOO)
/* included files can drag in "free": */
# define NO_POISON 1
#endif /* !NO_POISON && POISON_FREE_TOO */

#include "defs.h"
#include "value.h"
#include "infcall.h"  /* For inferior_function_calls_disabled_p.  */
#include "gdbarch.h"
#include "arch-utils.h"
#include "gdbcore.h"
#include "bfd.h"
#include "mach-o.h"
#include "objfiles.h"

#if defined(KDP_TARGET_POWERPC) && KDP_TARGET_POWERPC
# include "ppc-macosx-thread-status.h"
# include "ppc-macosx-regs.h"
# include "ppc-macosx-regnums.h"
# include "ppc-macosx-tdep.h"
#endif /* KDP_TARGET_POWERPC */

#if defined(KDP_TARGET_I386) && KDP_TARGET_I386
# include "i386-macosx-thread-status.h"
# include "i386-macosx-tdep.h"
#endif /* KDP_TARGET_I386 */

#if defined(KDP_TARGET_ARM) && KDP_TARGET_ARM
# include "arm-macosx-thread-status.h"
# include "arm-macosx-tdep.h"
# include "arm-tdep.h"
#endif /* KDP_TARGET_ARM */

#include "inferior.h"
#include "gdbcmd.h"
#include "event-loop.h"
#include "event-top.h"
#include "inf-loop.h"
#include "regcache.h"
#include "exceptions.h"
#include "osabi.h"

#include "kdp-udp.h"
#include "kdp-transactions.h"

#include "remote-kdp.h"

#ifndef S_SPLINT_S
# include <CoreFoundation/CoreFoundation.h>
# include <CoreFoundation/CFPropertyList.h>
#endif /* !S_SPLINT_S */

#ifndef CPU_TYPE_I386
# define CPU_TYPE_I386 (7)
#endif /* !CPU_TYPE_I386 */

#ifndef CPU_TYPE_X86_64
# define CPU_TYPE_X86_64 (16777223)
#endif /* !CPU_TYPE_X86_64 */

#ifndef CPU_TYPE_POWERPC
# define CPU_TYPE_POWERPC (18)
#endif /* !CPU_TYPE_POWERPC */

#ifndef CPU_TYPE_ARM
# define CPU_TYPE_ARM (12)
#endif /* !CPU_TYPE_ARM */

#ifndef KDP_REMOTE_ID
# define KDP_REMOTE_ID 3
#endif /* !KDP_REMOTE_ID */


#define KDP_MAX_BREAKPOINTS 100

/* When major changes are made to the level of KDP support (such as new
 * built-in commands, or new target support), please increment this so that
 * kgmacros can adopt the new support and keep old support for older gdb's:
 */
#define GDB_KDP_SUPPORT_LEVEL 1

#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <mach/mach.h>
#include <uuid/uuid.h>
#include <libintl.h>

extern int standard_is_async_p(void);
extern int standard_can_async_p(void);

static unsigned int kdp_debug_level = 3;
static unsigned int kdp_default_port = 41139;

static const char *kdp_default_host_type_str = "powerpc";
static int kdp_default_cpu_type = CPU_TYPE_POWERPC;

static kdp_connection c;
static int kdp_host_type = -1;
static int kdp_cpu_type = -1; /* CPU_TYPE_FOO */

static int kdp_stopped = 0;
static int kdp_timeout = 5000;
static int kdp_retries = 10;
static int remote_kdp_version = 0;
static int remote_kdp_feature = 0;

struct target_ops kdp_ops;

static void logger(kdp_log_level, const char *, ...)
  ATTR_FORMAT(gnu_printf, 2, 3);

static void kdp_mourn_inferior(void);

extern void _initialize_remote_kdp(void);

/* */
static void
set_timeouts(const char *args, int from_tty, struct cmd_list_element *cmd)
{
  kdp_set_timeouts(&c, kdp_timeout, kdp_retries);
}

static int
parse_host_type(const char *host)
{
  if ((strcasecmp(host, "powerpc") == 0) || (strcasecmp(host, "ppc") == 0))
    {
#if defined(KDP_TARGET_POWERPC) && KDP_TARGET_POWERPC
      return CPU_TYPE_POWERPC;
#else
      return -2;
#endif /* KDP_TARGET_POWERPC */
    }
  else if ((strcasecmp(host, "ia32") == 0)
           || (strcasecmp(host, "i386") == 0)
           || (strcasecmp(host, "i486") == 0)
           || (strcasecmp(host, "i586") == 0)
           || (strcasecmp(host, "pentium") == 0))
    {
#if defined(KDP_TARGET_I386) && KDP_TARGET_I386
      return CPU_TYPE_I386;
#else
      return -2;
#endif /* KDP_TARGET_I386 */
    }
  else if (strcasecmp(host, "arm") == 0)
    {
#if defined(KDP_TARGET_ARM) && KDP_TARGET_ARM
      return CPU_TYPE_ARM;
#else
      return -2;
#endif /* KDP_TARGET_ARM */
    }
  else
    {
      return -1;
    }
}

static void
logger(kdp_log_level l, const char *format, ...)
{
  va_list ap;

  if (l > kdp_debug_level)
    {
      return;
    }

  va_start(ap, format);
  vfprintf(stderr, format, ap);
  va_end(ap);
}

#if defined(KDP_TARGET_ARM) && KDP_TARGET_ARM
extern int set_arm_single_step_mode(struct gdbarch *, int);
#endif /* KDP_TARGET_ARM */

static void
kdp_open(const char *name, int from_tty)
{
  push_target(&kdp_ops);
  /* KDP cannot do inferior function calls: */
  inferior_function_calls_disabled_p = 1;
#if defined(KDP_TARGET_ARM) && KDP_TARGET_ARM
  set_arm_single_step_mode(current_gdbarch, arm_single_step_mode_software);
#endif /* KDP_TARGET_ARM */
}

static void
kdp_close(int quitting ATTRIBUTE_UNUSED)
{
  return;
}

static int
convert_host_type(unsigned int mach_type)
{
  switch (mach_type)
    {
    case CPU_TYPE_POWERPC:
      return bfd_arch_powerpc;
    case CPU_TYPE_I386:
    case CPU_TYPE_X86_64:
      return bfd_arch_i386;
#if defined(CPU_TYPE_ARM)
    case CPU_TYPE_ARM:
      return bfd_arch_arm;
#endif /* CPU_TYPE_ARM */
    default:
      return -1;
    }
}

static int
kdp_insert_breakpoint(CORE_ADDR addr, gdb_byte *contents_cache)
{
  kdp_return_t kdpret;

  if (!kdp_is_connected(&c))
    {
      logger(KDP_LOG_DEBUG,
             "kdp_insert_breakpoint: unable to set breakpoint - (not connected)");
      return -1;
    }

  if (remote_kdp_version >= 11) {
    c.request->breakpoint64_req.hdr.request = KDP_BREAKPOINT64_SET;
    c.request->breakpoint64_req.address = addr;
  } else {
    c.request->breakpoint_req.hdr.request = KDP_BREAKPOINT_SET;
    c.request->breakpoint_req.address = (unsigned int)addr;
  }

  kdpret =
    kdp_transaction(&c, c.request, c.response, "kdp_insert_breakpoint");
  logger(KDP_LOG_DEBUG,
         "kdp_insert_breakpoint: kdp_transaction returned %d\n", kdpret);
  if (kdpret == RR_SUCCESS) {
    if (remote_kdp_version >= 11) {
      kdpret = (kdp_return_t)c.response->breakpoint64_reply.error;
    } else {
      kdpret = (kdp_return_t)c.response->breakpoint_reply.error;
    }
  }

  if (kdpret != RR_SUCCESS)
    {
      /* We have reached the maximum number of breakpoints the kernel can support
         so revert to the old model of directly writing to memory */
      if (kdpret == KDP_MAX_BREAKPOINTS)
        {
          kdp_ops.to_insert_breakpoint = memory_insert_breakpoint;
          kdp_ops.to_remove_breakpoint = memory_remove_breakpoint;
          printf_unfiltered("Max number of kernel breakpoints reached,"
                            "Reverting to memory_insert_breakpoint.\n");
          memory_insert_breakpoint(addr, contents_cache);
          return 0;
        }

      logger(KDP_LOG_DEBUG,
             "kdp_insert_breakpoint: response contained error code %d\n",
             kdpret);
      return -1;
    }
  return 0;
}

static int
kdp_remove_breakpoint(CORE_ADDR addr, gdb_byte *contents_cache)
{
  kdp_return_t kdpret;

  if (!kdp_is_connected(&c))
    {
      logger(KDP_LOG_DEBUG,
             "kdp_remove_breakpoint: unable to remove breakpoint - (not connected)");
      return -1;
    }

    if (remote_kdp_version >= 11) {
      c.request->breakpoint64_req.hdr.request = KDP_BREAKPOINT64_REMOVE;
      c.request->breakpoint64_req.address = addr;
    } else {
      c.request->breakpoint_req.hdr.request = KDP_BREAKPOINT_REMOVE;
      c.request->breakpoint_req.address = (unsigned int)addr;
    }

  kdpret =
    kdp_transaction(&c, c.request, c.response, "kdp_remove_breakpoint");
  logger(KDP_LOG_DEBUG,
         "kdp_remove_breakpoint: kdp_transaction returned %d\n", kdpret);

  if (kdpret == RR_SUCCESS) {
    if (remote_kdp_version >= 11) {
      kdpret = (kdp_return_t)c.response->breakpoint64_reply.error;
    } else {
      kdpret = (kdp_return_t)c.response->breakpoint_reply.error;
    }
  }

  if (kdpret != RR_SUCCESS)
    {
      logger(KDP_LOG_DEBUG,
             "kdp_remove_breakpoint: response contained error code %d\n",
             kdpret);
      return -1;
    }
  return 0;
}

/* */
static void
kdp_kernelversion_command(const char *args, int from_tty)
{
  kdp_return_t kdpret;

  /* Ignore arguments */

  if (!kdp_is_connected(&c))
    {
      error("kdp: unable to get kernel version (not connected)");
    }

  c.request->kernelversion_req.hdr.request = KDP_KERNELVERSION;
  kdpret =
    kdp_transaction(&c, c.request, c.response, "kdp_kernelversion");
  logger(KDP_LOG_DEBUG,
         "kdp_kernelversion: kdp_transaction returned %d\n", kdpret);
  if (kdpret == RR_SUCCESS) {
    printf_unfiltered("%s\n", c.response->kernelversion_reply.version);
  }
}

/* Query the remote kernel to find out the host architecture
   (cpu type, cpu subtype) and set the KDP_CPU_TYPE and KDP_HOST_TYPE
   global variables as appropriate.  */

static void
kdp_hostinfo(void)
{
  kdp_return_t kdpret, kdpret2;

  c.request->readregs_req.hdr.request = KDP_HOSTINFO;

  kdpret = kdp_transaction(&c, c.request, c.response, "kdp_hostinfo");
  if (kdpret != RR_SUCCESS)
    {
      kdpret2 = kdp_disconnect (&c);
      if (kdpret2 != RR_SUCCESS)
        {
          warning(_("unable to disconnect from host after error determining cpu type: %s"),
		  kdp_return_string(kdpret2));
        }
      kdpret2 = kdp_destroy (&c);
      if (kdpret2 != RR_SUCCESS)
        {
          warning(_("unable to destroy host connection after error determining cpu type: %s"),
		  kdp_return_string(kdpret2));
        }
      error(_("kdp_attach: unable to determine host type: %s"),
	    kdp_return_string(kdpret));
    }

  if (c.response->hostinfo_reply.cpu_type == CPU_TYPE_X86_64)
    {
      struct gdbarch_info info;
      gdbarch_info_init (&info);
      gdbarch_info_fill (current_gdbarch, &info);
      info.byte_order = gdbarch_byte_order (current_gdbarch);
      info.osabi = GDB_OSABI_DARWIN64;
      info.bfd_arch_info = bfd_lookup_arch (bfd_arch_i386, bfd_mach_x86_64);
      gdbarch_update_p (info);
    }

  kdp_cpu_type = c.response->hostinfo_reply.cpu_type;
  kdp_host_type = convert_host_type (kdp_cpu_type);

  if (kdp_host_type == -1)
    {
      warning(_("kdp_attach: unknown host type 0x%lx; trying default (0x%lx)\n"),
	      (unsigned long)c.response->hostinfo_reply.cpu_type,
	      (unsigned long)kdp_default_cpu_type);
      kdp_cpu_type = kdp_default_cpu_type;
      kdp_host_type = convert_host_type(kdp_default_cpu_type);
    }

  if (kdp_host_type == -1)
    {
      kdpret2 = kdp_disconnect (&c);
      if (kdpret2 != RR_SUCCESS)
        {
          warning(_("unable to disconnect from host after error determining cpu type: %s"),
		  kdp_return_string(kdpret2));
        }
      kdpret2 = kdp_destroy (&c);
      if (kdpret2 != RR_SUCCESS)
        {
          warning(_("unable to destroy host connection after error determining cpu type: %s"),
		  kdp_return_string(kdpret2));
        }
      error("kdp_attach: unknown host type");
    }
}

/* Look at the KDP_VERSIONINFO string to see if the kernel's
 * Mach-O UUID and load address are provided: */
static void
kdp_uuid_and_load_addr(void)
{
  kdp_return_t kdpret, kdpret2;
  int uuids_matched;
  int got_remote_uuid;
  uuid_t remote_uuid;

  CORE_ADDR slide;
  CORE_ADDR actual_load_address;

  enum gdb_osabi mem_osabi;

  c.request->readregs_req.hdr.request = KDP_KERNELVERSION;

  kdpret = kdp_transaction(&c, c.request, c.response, "kdp_kernelversion");
  if (kdpret != RR_SUCCESS)
    {
      kdpret2 = kdp_disconnect(&c);
      if (kdpret2 != RR_SUCCESS)
        {
          warning(_("unable to disconnect from host after requesting kernel version string: %s"),
                  kdp_return_string(kdpret2));
        }
      kdpret2 = kdp_destroy(&c);
      if (kdpret2 != RR_SUCCESS)
        {
          warning(_("unable to destroy host connection after error requesting kernel version string: %s"),
                  kdp_return_string(kdpret2));
        }
      error(_("kdp_attach: unable to determine kernel version string: %s"),
            kdp_return_string(kdpret));
    }

  if ((c.response->kernelversion_reply.version == NULL)
      || (c.response->kernelversion_reply.version[0] == '\0'))
    {
      return;
    }

  /* The version string may look something like
   * "Darwin Kernel Version 11.0.0: Thu Nov 17 06:13:06 PST 2011; <blah blah>; UUID=A4B1973D-6A6A-3320-AEB9-585A4579C31A; stext=0xffffff8000400000"
   * The "UUID=" and "stext=" fields may not be present.  */

  uuids_matched = 0;
  got_remote_uuid = 0;
  if (strstr(c.response->kernelversion_reply.version, "; UUID="))
    {
      const char *j = (strstr(c.response->kernelversion_reply.version,
                              "; UUID=") + strlen("; UUID="));
      if ((strlen(j) > 36UL)
          && ((*(j + 36) == '\0') || (*(j + 36) == ';')))
        {
          char uuid_alone[40];
          uuid_t local_kernel_uuid;
          strncpy(uuid_alone, j, 36);
          uuid_alone[36] = '\0';
          if (uuid_parse(uuid_alone, remote_uuid) == 0)
            {
              logger(KDP_LOG_DEBUG,
                     "kdp_uuid_and_load_addr: remote kernel image has Mach-O UUID of %s\n",
                     puuid(remote_uuid));
              got_remote_uuid = 1;
            }
          if (got_remote_uuid && symfile_objfile && symfile_objfile->obfd
              && bfd_mach_o_get_uuid(symfile_objfile->obfd,
                                     local_kernel_uuid, sizeof(uuid_t)))
            {
              if (memcmp(remote_uuid, local_kernel_uuid, sizeof(uuid_t)) != 0)
                {
                  warning(_("Host-side kernel file has Mach-O UUID of %s but remote kernel has a UUID of %s "
			    "-- a mismatched kernel file will result in a poor debugger experience."),
                          puuid(local_kernel_uuid), puuid(remote_uuid));
                }
              else
                {
                  uuids_matched = 1;
                }
            }
        }
    }

  /* Slide the objfile even if the UUIDs do NOT match.
   * Maybe the user has given us a "pretty close" binary.
   * Sliding it to the correct address cannot make anything
   * worse than it would be otherwise... */

  slide = 0UL;
  actual_load_address = (INVALID_ADDRESS + slide);
  if (strstr(c.response->kernelversion_reply.version, "; stext="))
    {
      const char *j = (strstr(c.response->kernelversion_reply.version,
                              "; stext=") + strlen("; stext="));
      if ((*j == '0') && (tolower(*(j + 1)) == 'x'))
        j += 2;

      errno = 0;
      actual_load_address = strtoul(j, NULL, 16);
      if ((errno != 0) || (actual_load_address == 0)
          || (actual_load_address == INVALID_ADDRESS))
        return;
    }
  else
    {
      (void)actual_load_address;
      return;
    }

  mem_osabi = GDB_OSABI_UNKNOWN;
  get_information_about_macho(NULL, actual_load_address, NULL, 1, 1, NULL,
                              &mem_osabi, NULL, NULL, NULL, NULL);

  if (symfile_objfile)
    {
      CORE_ADDR file_load_addr = INVALID_ADDRESS;
      if (symfile_objfile
          && get_information_about_macho(NULL, INVALID_ADDRESS,
                                         symfile_objfile->obfd, 1, 0,
                                         NULL, NULL, NULL, &file_load_addr,
                                         NULL, NULL))
        {
          slide_kernel_objfile(symfile_objfile, actual_load_address,
                               remote_uuid, mem_osabi);
        }
      return;
    }

  /* If we have a UUID and load address, then see if there is a debug
   * symbols shell command defined, and if it might be a way to fetch the
   * correct kernel binary based on that UUID: */
  if (got_remote_uuid)
    {
      try_to_find_and_load_kernel_via_uuid(actual_load_address,
                                           remote_uuid, mem_osabi);
    }

  /* use an unused variable: */
  if (uuids_matched > 0) {
    return; /* else just fall off the end; it matters little */
  }
}

/* */
static void
kdp_attach(const char *args, int from_tty)
{
  kdp_return_t kdpret, kdpret2;
  unsigned int old_seqno, old_exc_seqno;

  if (args == NULL)
    {
      args = (char *)"";
    }

  {
    const char *s = args;
    while ((*s != '\0') && isspace(*s))
      {
        s++;
      }
    if (*s == '\0')
      {
        error("usage: attach <hostname>");
      }
    while ((*s != '\0') && !isspace(*s))
      {
        s++;
      }
    while ((*s != '\0') && isspace(*s))
      {
        s++;
      }
    if (*s != '\0')
      {
        error("usage: attach <hostname>");
      }
  }

  if (kdp_is_connected(&c))
    {
      kdpret = kdp_disconnect(&c);
      if (kdpret != RR_SUCCESS)
        {
          error("unable to disconnect from host: %s",
                kdp_return_string(kdpret));
        }
    }

  if (kdp_is_bound(&c))
    {
      kdpret = kdp_destroy(&c);
      if (kdpret != RR_SUCCESS)
        {
          error("unable to deallocate KDP connection: %s",
                kdp_return_string(kdpret));
        }
    }

  old_seqno = c.seqno;
  old_exc_seqno = c.exc_seqno;

  kdpret =
    kdp_create(&c, logger, args, kdp_default_port, kdp_timeout, kdp_retries);
  if (kdpret != RR_SUCCESS)
    {
      error("unable to create connection for host \"%s\": %s", args,
            kdp_return_string(kdpret));
    }

#if defined(KDP_TARGET_POWERPC) && KDP_TARGET_POWERPC
  kdp_set_little_endian(&c);
#elif defined(KDP_TARGET_I386) && KDP_TARGET_I386
  kdp_set_big_endian(&c);
#elif defined(KDP_TARGET_ARM) && KDP_TARGET_ARM
  kdp_set_big_endian(&c);
#elif defined(S_SPLINT_S)
  ; /* ??? */
#else
# error "unsupported architecture"
#endif /* KDP_TARGET_foo */

  c.seqno = old_seqno;
  c.exc_seqno = old_exc_seqno;

  kdpret = kdp_connect(&c);
  if (kdpret != RR_SUCCESS)
    {
      kdpret2 = kdp_destroy(&c);
      if (kdpret2 != RR_SUCCESS)
        {
          warning(_("unable to destroy host connection after error connecting: %s"),
		  kdp_return_string(kdpret2));
        }
      error(_("unable to connect to host \"%s\": %s"), args,
	    kdp_return_string(kdpret));
    }

  c.request->readregs_req.hdr.request = KDP_VERSION;

  kdpret = kdp_transaction (&c, c.request, c.response, "kdp_attach");
  if (kdpret != RR_SUCCESS)
    {
      kdpret2 = kdp_disconnect (&c);
      if (kdpret2 != RR_SUCCESS)
        {
          warning(_("unable to disconnect from host after error determining protocol version: %s"),
		  kdp_return_string(kdpret2));
        }
      kdpret2 = kdp_destroy (&c);
      if (kdpret2 != RR_SUCCESS)
        {
          warning(_("unable to destroy host connection after error determining protocol version: %s"),
		  kdp_return_string(kdpret2));
        }
      error(_("kdp_attach: unable to determine protocol version: %s"),
	    kdp_return_string(kdpret));
    }

  remote_kdp_version = c.response->version_reply.version;
  remote_kdp_feature = c.response->version_reply.feature;

  set_internalvar(lookup_internalvar("kdp_protocol_version"),
		  value_from_longest(builtin_type_int, (LONGEST)remote_kdp_version));

  set_internalvar(lookup_internalvar("gdb_kdp_support_level"),
		  value_from_longest(builtin_type_int, (LONGEST)GDB_KDP_SUPPORT_LEVEL));

  /* retrieve the cpu type & cpusubtype, set kdp_cpu_type kdp_host_type
   * globals: */
  kdp_hostinfo();

  /* See if the kdp_versioninfo tells us where the kernel is loaded in
   * memory: */
  kdp_uuid_and_load_addr();

  /* Use breakpoint packets only if the kernel supports them */
  if ((remote_kdp_version >= 10) && (remote_kdp_feature & KDP_FEATURE_BP))
    {
      kdp_ops.to_insert_breakpoint = kdp_insert_breakpoint;
      kdp_ops.to_remove_breakpoint = kdp_remove_breakpoint;
    }
  kdp_ops.to_has_all_memory = 1;
  kdp_ops.to_has_memory = 1;
  kdp_ops.to_has_stack = 1;
  kdp_ops.to_has_registers = 1;
  kdp_ops.to_has_execution = 1;

  update_current_target ();

  inferior_ptid = pid_to_ptid (KDP_REMOTE_ID);
  kdp_stopped = 1;

  printf_unfiltered ("Connected.\n");
}

/* */
static void
kdp_detach(const char *args, int from_tty)
{
  kdp_return_t kdpret;

  if (kdp_is_connected(&c))
    {
      kdpret = kdp_disconnect(&c);
      if (kdpret != RR_SUCCESS)
        {
          warning(_("unable to disconnect from host: %s"),
		  kdp_return_string(kdpret));
        }
    }

  kdp_ops.to_has_all_memory = 0;
  kdp_ops.to_has_memory = 0;
  kdp_ops.to_has_stack = 0;
  kdp_ops.to_has_registers = 0;
  kdp_ops.to_has_execution = 0;

  update_current_target ();

  if (kdp_is_bound (&c))
    {
      kdpret = kdp_destroy (&c);
      if (kdpret != RR_SUCCESS)
        {
          error("unable to deallocate KDP connection: %s",
                kdp_return_string(kdpret));
        }
    }
  kdp_mourn_inferior();

  printf_unfiltered("Disconnected.\n");
}

/* */
static void
kdp_reattach_command(const char *args, int from_tty)
{
  kdp_return_t kdpret;
  char **argv;
  char *host;

  argv = buildargv(args);

  if ((argv == NULL) || (argv[0] == NULL) || (argv[1] != NULL))
    error("usage: kdp-reattach <hostname>");

  host = argv[0];

  kdp_open(NULL, 0);

  kdp_reset(&c);

  kdpret =
    kdp_create(&c, logger, argv[0], kdp_default_port, kdp_timeout,
               kdp_retries);
  if (kdpret != RR_SUCCESS)
    error("unable to create connection for host \"%s\": %s", args,
          kdp_return_string(kdpret));

#if defined(KDP_TARGET_POWERPC) && KDP_TARGET_POWERPC
  kdp_set_little_endian(&c);
#elif defined(KDP_TARGET_I386) && KDP_TARGET_I386
  kdp_set_big_endian(&c);
#elif defined(KDP_TARGET_ARM) && KDP_TARGET_ARM
  kdp_set_big_endian(&c);
#elif defined(S_SPLINT_S)
  ; /* ??? */
#else
# error "unsupported architecture"
#endif /* KDP_TARGET_foo */

  kdpret = kdp_reattach (&c);

  if (kdpret != RR_SUCCESS)
    warning(_("unable to disconnect host: %s"), kdp_return_string(kdpret));
  if (kdp_is_bound(&c))
    {
      kdpret = kdp_destroy(&c);
      if (kdpret != RR_SUCCESS)
        error(_("unable to deallocate KDP connection: %s"),
	      kdp_return_string(kdpret));
    }

  kdp_ops.to_has_all_memory = 0;
  kdp_ops.to_has_memory = 0;
  kdp_ops.to_has_stack = 0;
  kdp_ops.to_has_registers = 0;
  kdp_ops.to_has_execution = 0;

  update_current_target ();

  kdp_mourn_inferior();

  kdp_open(NULL, 0);

  kdp_attach(host, 0);
}

/* */
static void
kdp_reboot_command(const char *args, int from_tty)
{
  kdp_return_t kdpret;
  char **argv;

  argv = buildargv(args);
  if (!c.connected)
    error(_("Must already be connected to the remote machine."));

  if (!((argv == NULL) || (argv[0] == NULL)))
    {
      error(_("usage: kdp-reboot"));
    }

  kdpret = kdp_hostreboot (&c);

  if (kdpret != RR_SUCCESS)
    warning(_("unable to disconnect host: %s"), kdp_return_string(kdpret));
  if (kdp_is_bound (&c))
    {
      kdpret = kdp_destroy (&c);
      if (kdpret != RR_SUCCESS)
        error(_("unable to deallocate KDP connection: %s"),
	      kdp_return_string(kdpret));
    }

  kdp_ops.to_has_all_memory = 0;
  kdp_ops.to_has_memory = 0;
  kdp_ops.to_has_stack = 0;
  kdp_ops.to_has_registers = 0;
  kdp_ops.to_has_execution = 0;

  update_current_target();

  kdp_mourn_inferior();
}

/* */
static void
kdp_detach_command(const char *args, int from_tty)
{
  kdp_connection c2;
  kdp_return_t kdpret;

  char **argv;
  char *host;
  unsigned int seqno;

  argv = buildargv(args);

  if ((argv == NULL) || (argv[0] == NULL) || (argv[1] == NULL)
      || (argv[2] != NULL))
    {
      error("usage: kdp-detach <hostname> <seqno>");
    }
  host = argv[0];
  seqno = atoi(argv[1]);

  kdp_reset(&c2);

  kdpret =
    kdp_create(&c2, logger, argv[0], kdp_default_port, kdp_timeout,
               kdp_retries);
  if (kdpret != RR_SUCCESS)
    {
      error("unable to create connection for host \"%s\": %s", args,
            kdp_return_string(kdpret));
    }

  c2.request->disconnect_req.hdr.request = KDP_DISCONNECT;
  c2.request->hdr.key = 0;
  c2.request->hdr.is_reply = 0;
  c2.request->hdr.seq = (unsigned char)seqno;

  kdp_set_big_endian(&c2);
  kdpret = kdp_transmit_debug(&c2, c2.request);
  if (kdpret != RR_SUCCESS)
    {
      error("unable to send reset request: %s", kdp_return_string(kdpret));
    }

  kdp_set_little_endian(&c2);
  kdpret = kdp_transmit_debug(&c2, c2.request);
  if (kdpret != RR_SUCCESS)
    {
      error("unable to send reset request: %s", kdp_return_string(kdpret));
    }

  kdpret = kdp_destroy(&c2);
  if (kdpret != RR_SUCCESS)
    {
      error("unable to destroy connection: %s", kdp_return_string(kdpret));
    }

  /* use an unused variable: */
  if (host == NULL) {
    return; /* else just fall off the end; it matters little either way */
  }
}

static void
kdp_set_trace_bit(int step)
{
  switch (kdp_cpu_type)
    {

    case CPU_TYPE_POWERPC:
      {
#if defined(KDP_TARGET_POWERPC) && KDP_TARGET_POWERPC
        LONGEST srr1 = read_register (PS_REGNUM);
        if (step)
          {
            srr1 |= 0x400UL;
          }
        else
          {
            srr1 &= ~0x400UL;
          }
        write_register (PS_REGNUM, srr1);
#else
        error ("kdp_set_trace_bit: not configured to support powerpc");
#endif /* KDP_TARGET_POWERPC */
      }
      break;

    case CPU_TYPE_I386:
      {
#if defined(KDP_TARGET_I386) && KDP_TARGET_I386
        LONGEST eflags = read_register (PS_REGNUM);
        if (step)
          {
            eflags |= 0x100UL;
          }
        else
          {
            eflags &= ~0x100UL;
          }
        write_register (PS_REGNUM, eflags);
#else
        error ("kdp_set_trace_bit: not configured to support i386");
#endif /* KDP_TARGET_I386 */
      }
      break;
    case CPU_TYPE_X86_64:
      {
#if defined(KDP_TARGET_I386) && KDP_TARGET_I386
        LONGEST rflags = read_register (PS_REGNUM);
        if (step)
          {
            rflags |= 0x100UL;
          }
        else
          {
            rflags &= ~0x100UL;
          }
        write_register (PS_REGNUM, rflags);
#else
        error ("kdp_set_trace_bit: not configured to support x86_64");
#endif /* KDP_TARGET_I386 */
      }
      break;
    case CPU_TYPE_ARM:
      /* We just ignore requests to set the trace bit on ARM.
       * For now, we are always using software single stepping.  */
      break;
    default:
      error ("kdp_set_trace_bit: unknown host type 0x%lx",
             (unsigned long) kdp_host_type);
    }
}

static void
kdp_resume (ptid_t pid, int step, enum target_signal sig)
{
  kdp_return_t kdpret;

  if (!kdp_is_connected (&c))
    {
      error ("kdp: unable to resume (not connected)");
    }

  if (step)
    {
      kdp_set_trace_bit (1);
    }
  else
    {
      kdp_set_trace_bit (0);
    }

  c.request->resumecpus_req.hdr.request = KDP_RESUMECPUS;

  if (proceed_to_finish || (step_over_calls != STEP_OVER_UNDEBUGGABLE))
    c.request->resumecpus_req.cpu_mask = 1L;
  else
    c.request->resumecpus_req.cpu_mask = ~0L;

  kdpret = kdp_transaction (&c, c.request, c.response, "kdp_resume");
  if (kdpret != RR_SUCCESS)
    {
      error ("unable to resume processing on host: %s",
             kdp_return_string (kdpret));
    }

  kdp_stopped = 0;

  if (target_can_async_p ())
    target_async (inferior_event_handler, 0);

  if (target_is_async_p ())
    target_executing = 1;
}

static ptid_t
kdp_wait (ptid_t pid,
          struct target_waitstatus *status, gdb_client_data client_data)
{
  kdp_return_t kdpret;

  if (PIDGET (pid) == -1)
    {
      pid = pid_to_ptid (KDP_REMOTE_ID);
    }
  if (PIDGET (pid) != KDP_REMOTE_ID)
    {
      error ("kdp: unable to switch to process-id %s",
             target_pid_to_str (pid));
    }

  if (!kdp_is_connected (&c))
    {
      error ("kdp: unable to wait for activity (not connected)");
    }

  if (kdp_stopped)
    {
      status->kind = TARGET_WAITKIND_STOPPED;
      status->value.sig = TARGET_SIGNAL_TRAP;
      return pid;
    }

  kdpret = kdp_exception_wait (&c, c.response, 0);

  if (kdpret != RR_SUCCESS)
    {
      error ("unable to wait for result from host: %s\n",
             kdp_return_string (kdpret));
    }

  kdp_set_trace_bit (0);

  kdp_stopped = 1;
  select_frame (get_current_frame ());

  status->kind = TARGET_WAITKIND_STOPPED;
  status->value.sig = TARGET_SIGNAL_TRAP;

  return pid;
}

#if defined(KDP_TARGET_POWERPC) && KDP_TARGET_POWERPC
static void
kdp_fetch_registers_ppc (int regno)
{
  unsigned int i;

  if (!kdp_is_connected (&c))
    {
      error ("kdp: unable to fetch registers (not connected)");
    }

  if ((regno == -1) || PPC_MACOSX_IS_GP_REGNUM (regno)
      || PPC_MACOSX_IS_GSP_REGNUM (regno))
    {
      kdp_return_t kdpret;
      gdb_ppc_thread_state_t gp_regs;

      c.request->readregs_req.hdr.request = KDP_READREGS;
      c.request->readregs_req.cpu = 0;
      c.request->readregs_req.flavor = GDB_PPC_THREAD_STATE;

      kdpret =
        kdp_transaction (&c, c.request, c.response,
                         "kdp_fetch_registers_ppc");
      if (kdpret != RR_SUCCESS)
        {
          error("kdp_fetch_registers_ppc: unable to fetch PPC_THREAD_STATE: %s",
		kdp_return_string(kdpret));
        }
      if (c.response->readregs_reply.nbytes !=
          (GDB_PPC_THREAD_STATE_COUNT * 4))
        {
          error("kdp_fetch_registers_ppc: kdp returned %lu bytes of register data (expected %lu)",
		c.response->readregs_reply.nbytes,
		(GDB_PPC_THREAD_STATE_COUNT * 4));
        }

      memcpy (&gp_regs, c.response->readregs_reply.data,
              (GDB_PPC_THREAD_STATE_COUNT * 4));
      ppc_macosx_fetch_gp_registers (&gp_regs);
    }

# if 0
  if ((regno == -1) || PPC_MACOSX_IS_FP_REGNUM (regno)
      || PPC_MACOSX_IS_FSP_REGNUM (regno))
    {
      kdp_return_t kdpret;
      gdb_ppc_thread_fpstate_t fp_regs;

      c.request->readregs_req.hdr.request = KDP_READREGS;
      c.request->readregs_req.cpu = 0;
      c.request->readregs_req.flavor = GDB_PPC_THREAD_FPSTATE;

      kdpret =
        kdp_transaction (&c, c.request, c.response,
                         "kdp_fetch_registers_ppc");
      if (kdpret != RR_SUCCESS)
        {
          error("kdp_fetch_registers_ppc: unable to fetch PPC_THREAD_FPSTATE: %s",
		kdp_return_string(kdpret));
        }
      if (c.response->readregs_reply.nbytes !=
          (GDB_PPC_THREAD_FPSTATE_COUNT * 4))
        {
          error("kdp_fetch_registers_ppc: kdp returned %lu bytes of register data (expected %lu)",
		c.response->readregs_reply.nbytes,
		(GDB_PPC_THREAD_FPSTATE_COUNT * 4));
        }

      memcpy (&fp_regs, c.response->readregs_reply.data,
              (GDB_PPC_THREAD_FPSTATE_COUNT * 4));
      ppc_macosx_fetch_fp_registers (&fp_regs);
    }
# else
  if ((regno == -1) || PPC_MACOSX_IS_FP_REGNUM (regno)
      || PPC_MACOSX_IS_FSP_REGNUM (regno))
    {
      /* Accesses to the fp registers are NOT currently supported in
         the kernel. */
      for (i = PPC_MACOSX_FIRST_FP_REGNUM; i <= PPC_MACOSX_LAST_FP_REGNUM;
           i++)
	set_register_cached (i, 1);
      for (i = PPC_MACOSX_FIRST_FSP_REGNUM; i <= PPC_MACOSX_LAST_FSP_REGNUM;
           i++)
	set_register_cached (i, 1);
    }
# endif /* 0 */

  if ((regno == -1) || (regno >= PPC_MACOSX_FIRST_VP_REGNUM)
      || PPC_MACOSX_IS_VSP_REGNUM (regno))
    {
      /* Accesses to the vector, fpscr and vrsave registers are NOT currently
         supported in the kernel. */
      for (i = PPC_MACOSX_FIRST_VP_REGNUM; i <= PPC_MACOSX_LAST_VP_REGNUM;
           i++)
	set_register_cached (i, 1);
      for (i = PPC_MACOSX_FIRST_VSP_REGNUM; i <= PPC_MACOSX_LAST_VSP_REGNUM;
           i++)
	set_register_cached (i, 1);
    }
}
#endif /* KDP_TARGET_POWERPC */

#if defined(KDP_TARGET_POWERPC) && KDP_TARGET_POWERPC
static void
kdp_store_registers_ppc (int regno)
{
  if (!kdp_is_connected (&c))
    {
      error ("kdp: unable to store registers (not connected)");
    }

  if ((regno == -1) || PPC_MACOSX_IS_GP_REGNUM (regno)
      || PPC_MACOSX_IS_GSP_REGNUM (regno))
    {

      gdb_ppc_thread_state_t gp_regs;
      kdp_return_t kdpret;

      ppc_macosx_store_gp_registers (&gp_regs);

      memcpy (c.request->writeregs_req.data, &gp_regs,
              (GDB_PPC_THREAD_STATE_COUNT * 4));

      c.request->writeregs_req.hdr.request = KDP_WRITEREGS;
      c.request->writeregs_req.cpu = 0;
      c.request->writeregs_req.flavor = GDB_PPC_THREAD_STATE;
      c.request->writeregs_req.nbytes = GDB_PPC_THREAD_STATE_COUNT * 4;

      kdpret =
        kdp_transaction (&c, c.request, c.response,
                         "kdp_store_registers_ppc");
      if (kdpret != RR_SUCCESS)
        {
          error("kdp_store_registers_ppc: unable to store PPC_THREAD_STATE: %s",
		kdp_return_string(kdpret));
        }
    }

  if ((regno == -1) || PPC_MACOSX_IS_FP_REGNUM (regno)
      || PPC_MACOSX_IS_FSP_REGNUM (regno))
    {

      gdb_ppc_thread_fpstate_t fp_regs;
      kdp_return_t kdpret;

      ppc_macosx_store_fp_registers (&fp_regs);

      memcpy (c.response->readregs_reply.data, &fp_regs,
              (GDB_PPC_THREAD_FPSTATE_COUNT * 4));

      c.request->writeregs_req.hdr.request = KDP_WRITEREGS;
      c.request->writeregs_req.cpu = 0;
      c.request->writeregs_req.flavor = GDB_PPC_THREAD_FPSTATE;
      c.request->writeregs_req.nbytes = GDB_PPC_THREAD_FPSTATE_COUNT * 4;

      kdpret =
        kdp_transaction (&c, c.request, c.response,
                         "kdp_store_registers_ppc");
      if (kdpret != RR_SUCCESS)
        {
          error("kdp_store_registers_ppc: unable to store PPC_THREAD_FPSTATE: %s",
		kdp_return_string(kdpret));
        }
    }
}
#endif /* KDP_TARGET_POWERPC */

#if defined(KDP_TARGET_I386) && KDP_TARGET_I386
static void
kdp_fetch_registers_i386(int regno)
{
  if (!kdp_is_connected(&c))
    {
      error("kdp: unable to fetch registers (not connected)");
    }

  if ((regno == -1) || IS_GP_REGNUM(regno))
    {
      kdp_return_t kdpret;
      gdb_i386_thread_state_t gp_regs;

      c.request->readregs_req.hdr.request = KDP_READREGS;
      c.request->readregs_req.cpu = 0;
      c.request->readregs_req.flavor = GDB_i386_THREAD_STATE;

      kdpret =
        kdp_transaction(&c, c.request, c.response,
                        "kdp_fetch_registers_i386");
      if (kdpret != RR_SUCCESS)
        {
          error("kdp_fetch_registers_i386: unable to fetch i386_THREAD_STATE: %s",
                kdp_return_string (kdpret));
        }
      if (c.response->readregs_reply.nbytes !=
          (GDB_i386_THREAD_STATE_COUNT * 4))
        {
          error("kdp_fetch_registers_i386: kdp returned %lu bytes of register data (expected %u)",
                c.response->readregs_reply.nbytes,
                (GDB_i386_THREAD_STATE_COUNT * 4));
        }

      memcpy(&gp_regs, c.response->readregs_reply.data,
             (GDB_i386_THREAD_STATE_COUNT * 4));
      i386_macosx_fetch_gp_registers(&gp_regs);
    }

  if ((regno == -1) || IS_FP_REGNUM(regno))
    {
      gdb_i386_thread_fpstate_t fp_regs = { 0U, 0U, { 0U }, 0U };

      /* FIXME: For now we hang the kdp stub asking for FP registers,
         so till the kernel can handle the request, do NOT send it.  */
# if 0
      kdp_return_t kdpret;
      c.request->readregs_req.hdr.request = KDP_READREGS;
      c.request->readregs_req.cpu = 0;
      c.request->readregs_req.flavor = GDB_i386_THREAD_FPSTATE;

      kdpret =
        kdp_transaction(&c, c.request, c.response,
                        "kdp_fetch_registers_i386");
      if (kdpret != RR_SUCCESS)
        {
          error("kdp_fetch_registers_i386: unable to fetch GDB_i386_THREAD_FPSTATE: %s",
                kdp_return_string(kdpret));
        }
      if (c.response->readregs_reply.nbytes !=
          (GDB_i386_THREAD_FPSTATE_COUNT * 4))
        {
          error("kdp_fetch_registers_i386: kdp returned %lu bytes of register data (expected %lu)",
                c.response->readregs_reply.nbytes,
                (GDB_i386_THREAD_FPSTATE_COUNT * 4));
        }

      memcpy(&fp_regs.hw_fu_state, c.response->readregs_reply.data,
             (GDB_i386_THREAD_FPSTATE_COUNT * 4));
# endif /* 0 */
      i386_macosx_fetch_fp_registers((gdb_i386_float_state_t *)&fp_regs);
    }
}
#endif /* KDP_TARGET_I386 */

#if defined(KDP_TARGET_I386) && KDP_TARGET_I386
static void
kdp_fetch_registers_x86_64(int regno)
{
  if (!kdp_is_connected(&c))
    {
      error("kdp: unable to fetch registers (not connected)");
    }

  if ((regno == -1) || IS_GP_REGNUM_64(regno))
    {
      kdp_return_t kdpret;
      gdb_x86_thread_state64_t gp_regs;

      c.request->readregs_req.hdr.request = KDP_READREGS;
      c.request->readregs_req.cpu = 0;
      c.request->readregs_req.flavor = GDB_x86_THREAD_STATE64;

      kdpret =
        kdp_transaction(&c, c.request, c.response,
                        "kdp_fetch_registers_x86_64");
      if (kdpret != RR_SUCCESS)
        {
          error("kdp_fetch_registers_x86_64: unable to fetch x86_THREAD_STATE64: %s",
                kdp_return_string(kdpret));
        }
      if (c.response->readregs_reply.nbytes !=
          (GDB_x86_THREAD_STATE64_COUNT * 4))
        {
          error("kdp_fetch_registers_x86_64: kdp returned %lu bytes of register data (expected %u)",
                c.response->readregs_reply.nbytes,
                (GDB_x86_THREAD_STATE64_COUNT * 4));
        }

      memcpy(&gp_regs, c.response->readregs_reply.data,
             (GDB_x86_THREAD_STATE64_COUNT * 4));
      x86_64_macosx_fetch_gp_registers(&gp_regs);
    }

  if ((regno == -1) || IS_FP_REGNUM(regno))
    {
      kdp_return_t kdpret;
      __extension__ gdb_x86_float_state64_t fp_regs = { }; /* FIXME: blah */

      /* FIXME: For now we hang the kdp stub asking for FP registers,
         so till the kernel can handle the request, do NOT send it.  */
# if 1
      c.request->readregs_req.hdr.request = KDP_READREGS;
      c.request->readregs_req.cpu = 0;
      c.request->readregs_req.flavor = GDB_x86_FLOAT_STATE64;

      kdpret =
        kdp_transaction(&c, c.request, c.response,
                        "kdp_fetch_registers_x86_64");
      if (kdpret != RR_SUCCESS)
        {
          error("kdp_fetch_registers_x86_64: unable to fetch x86_FLOAT_STATE64: %s",
                kdp_return_string(kdpret));
        }
      if (c.response->readregs_reply.nbytes !=
          (GDB_x86_FLOAT_STATE64_COUNT * 4))
        {
          error("kdp_fetch_registers_x86_64: kdp returned %lu bytes of register data (expected %u)",
                c.response->readregs_reply.nbytes,
                (GDB_x86_FLOAT_STATE64_COUNT * 4));
        }

      memcpy(&fp_regs, c.response->readregs_reply.data,
             (GDB_x86_FLOAT_STATE64_COUNT * 4));
# endif /* 1 */
      x86_64_macosx_fetch_fp_registers(&fp_regs);
    }
}
#endif /* KDP_TARGET_I386 */

#if defined(KDP_TARGET_I386) && KDP_TARGET_I386
static void
kdp_store_registers_i386(int regno)
{
  if (!kdp_is_connected(&c))
    {
      error("kdp: unable to store registers (not connected)");
    }

  if ((regno == -1) || IS_GP_REGNUM(regno))
    {
      gdb_i386_thread_state_t gp_regs;
      kdp_return_t kdpret;

      i386_macosx_store_gp_registers(&gp_regs);

      memcpy(c.request->writeregs_req.data, &gp_regs,
             (GDB_i386_THREAD_STATE_COUNT * 4));

      c.request->writeregs_req.hdr.request = KDP_WRITEREGS;
      c.request->writeregs_req.cpu = 0;
      c.request->writeregs_req.flavor = GDB_i386_THREAD_STATE;
      c.request->writeregs_req.nbytes = (GDB_i386_THREAD_STATE_COUNT * 4);

      kdpret =
        kdp_transaction(&c, c.request, c.response,
                        "kdp_store_registers_i386");
      if (kdpret != RR_SUCCESS)
        {
          error("kdp_store_registers_i386: unable to store i386_THREAD_STATE: %s",
                kdp_return_string(kdpret));
        }
    }

  if ((regno == -1) || IS_FP_REGNUM(regno))
    {
      gdb_i386_thread_fpstate_t fp_regs;
      kdp_return_t kdpret;

      if (i386_macosx_store_fp_registers((gdb_i386_float_state_t *)&fp_regs) == 0)
        return;

      /* Subtract 8 from size to avoid -Wstringop-overflow: */
      memcpy(c.response->readregs_reply.data, &fp_regs.hw_fu_state,
             ((GDB_i386_THREAD_FPSTATE_COUNT * 4) - 8));
      /* FIXME: ...but that leads to -Warray-bounds? */

      c.request->writeregs_req.hdr.request = KDP_WRITEREGS;
      c.request->writeregs_req.cpu = 0;
      c.request->writeregs_req.flavor = GDB_i386_THREAD_FPSTATE;
      c.request->writeregs_req.nbytes = GDB_i386_THREAD_FPSTATE_COUNT * 4;

      kdpret =
        kdp_transaction(&c, c.request, c.response,
                        "kdp_store_registers_i386");
      if (kdpret != RR_SUCCESS)
        {
          error("kdp_store_registers_i386: unable to store i386_THREAD_FPSTATE: %s",
                kdp_return_string(kdpret));
        }
    }
}
#endif /* KDP_TARGET_I386 */

#if defined(KDP_TARGET_ARM) && KDP_TARGET_ARM
static void
kdp_fetch_registers_arm(int regno)
{
  unsigned int i;

  if (!kdp_is_connected(&c))
    {
      error("kdp: unable to fetch registers (not connected)");
    }

  if ((regno == -1) || ARM_MACOSX_IS_GP_RELATED_REGNUM(regno))
    {
      kdp_return_t kdpret;
      gdb_arm_thread_state_t gp_regs;

      c.request->readregs_req.hdr.request = KDP_READREGS;
      c.request->readregs_req.cpu = 0;
      c.request->readregs_req.flavor = GDB_ARM_THREAD_STATE;

      kdpret =
        kdp_transaction(&c, c.request, c.response,
                        "kdp_fetch_registers_arm");
      if (kdpret != RR_SUCCESS)
        {
          error("kdp_fetch_registers_arm: unable to fetch ARM_THREAD_STATE: %s",
                kdp_return_string(kdpret));
        }
      if (c.response->readregs_reply.nbytes !=
          (GDB_ARM_THREAD_STATE_COUNT * 4))
        {
          error("kdp_fetch_registers_arm: kdp returned %lu bytes of register data (expected %lu)",
                c.response->readregs_reply.nbytes,
                (GDB_ARM_THREAD_STATE_COUNT * 4));
        }

      memcpy(&gp_regs, c.response->readregs_reply.data,
             (GDB_ARM_THREAD_STATE_COUNT * 4));
      arm_macosx_fetch_gp_registers(&gp_regs);
    }

#if KDP_TARGET_SUPPORTS_FP
  if ((regno == -1) || ARM_MACOSX_IS_VFP_RELATED_REGNUM (regno))
    {
      enum arm_vfp_version vfp_version;
      vfp_version = new_gdbarch_tdep(current_gdbarch)->vfp_version;

      switch (vfp_version)
	{
	  case ARM_VFP_UNSUPPORTED:
	    /* No VFP support, so nothing to do: */
	    break;
	  case ARM_VFP_VERSION_1:
	  case ARM_VFP_VERSION_3:
	    {
              kdp_return_t kdpret;
              c.request->readregs_req.hdr.request = KDP_READREGS;
              c.request->readregs_req.cpu = 0;
              c.request->readregs_req.flavor = GDB_ARM_THREAD_FPSTATE;

	      kdpret = kdp_transaction(&c, c.request, c.response,
                                       "kdp_fetch_registers_arm");
              if (kdpret != RR_SUCCESS)
                {
                  error("kdp_fetch_registers_arm: unable to fetch ARM_THREAD_FPSTATE: %s",
                        kdp_return_string(kdpret));
                }

	      if (vfp_version == ARM_VFP_VERSION_1)
                {
		  if (c.response->readregs_reply.nbytes ==
		      sizeof(gdb_arm_thread_vfpv1_state_t))
		    {
		      gdb_arm_thread_vfpv1_state_t fp_regs;
		      memcpy(&fp_regs, c.response->readregs_reply.data,
			     c.response->readregs_reply.nbytes);
		      arm_macosx_fetch_vfpv1_regs(&fp_regs);
		    }
		  else
		    {
                      error("kdp_fetch_registers_arm: kdp returned %lu bytes of "
                            "register data (expected %lu)",
                            c.response->readregs_reply.nbytes,
                            sizeof(gdb_arm_thread_vfpv1_state_t));
                    }
		}
	      else if (vfp_version == ARM_VFP_VERSION_3)
		{
		  if (c.response->readregs_reply.nbytes ==
		      sizeof(gdb_arm_thread_vfpv3_state_t))
		    {
		      gdb_arm_thread_vfpv3_state_t fp_regs;
                      memcpy(&fp_regs, c.response->readregs_reply.data,
                             c.response->readregs_reply.nbytes);
		      arm_macosx_fetch_vfpv3_regs(&fp_regs);
                    }
		  else
		    {
		      error("kdp_fetch_registers_arm: kdp returned %lu bytes of "
                            "register data (expected %lu)",
                            c.response->readregs_reply.nbytes,
                            sizeof(gdb_arm_thread_vfpv3_state_t));
		    }
		}
	    }
	    break;

	  default:
	    error("kdp_fetch_registers_arm: unable to fetch ARM_THREAD_FPSTATE: "
		  "unsupported vfp version: %d", (int)vfp_version);
	    break;
	}
    }
#else
  if ((regno == -1) || ARM_MACOSX_IS_VFP_RELATED_REGNUM(regno))
    {
      /* Accesses to the fp registers are NOT currently supported in
         the kernel. */
      if (new_gdbarch_tdep(current_gdbarch)->fp_model == ARM_FLOAT_VFP)
	{
	  for (i = 0; i <= ARM_MACOSX_NUM_VFP_REGS; i++)
	    set_register_cached(ARM_VFP_REGNUM_S0 + i, 1);
	  set_register_cached(ARM_VFP_REGNUM_FPSCR, 1);
	}
    }
#endif /* KDP_TARGET_SUPPORTS_FP */
  /* All other modes always include the F0-F7 and its FPS. */
  if ((regno == -1) || ARM_MACOSX_IS_FP_RELATED_REGNUM(regno))
    {
      for (i = ARM_F0_REGNUM; i <= ARM_F7_REGNUM; i++)
	set_register_cached(i, 1);
      set_register_cached(ARM_FPS_REGNUM, 1);
    }
}
#endif /* KDP_TARGET_ARM */

#if defined(KDP_TARGET_ARM) && KDP_TARGET_ARM
static void
kdp_store_registers_arm(int regno)
{
  if (!kdp_is_connected(&c))
    {
      error("kdp: unable to store registers (not connected)");
    }

  if ((regno == -1) || ARM_MACOSX_IS_GP_RELATED_REGNUM(regno))
    {
      gdb_arm_thread_state_t gp_regs;
      kdp_return_t kdpret;

      arm_macosx_store_gp_registers(&gp_regs);

      memcpy(c.request->writeregs_req.data, &gp_regs,
             (GDB_ARM_THREAD_STATE_COUNT * 4));

      c.request->writeregs_req.hdr.request = KDP_WRITEREGS;
      c.request->writeregs_req.cpu = 0;
      c.request->writeregs_req.flavor = GDB_ARM_THREAD_STATE;
      c.request->writeregs_req.nbytes = (GDB_ARM_THREAD_STATE_COUNT * 4);

      kdpret =
        kdp_transaction(&c, c.request, c.response,
                        "kdp_store_registers_arm");
      if (kdpret != RR_SUCCESS)
        {
          error("kdp_store_registers_arm: unable to store ARM_THREAD_STATE: %s",
                kdp_return_string(kdpret));
        }
    }
# if KDP_TARGET_SUPPORTS_FP
  if ((regno == -1) || ARM_MACOSX_IS_VFP_RELATED_REGNUM(regno))
    {
      kdp_return_t kdpret;
      enum arm_vfp_version vfp_version;
      vfp_version = new_gdbarch_tdep(current_gdbarch)->vfp_version;
      int fp_data_size = -1;
      union vfp_regs
      {
	gdb_arm_thread_vfpv1_state_t v1;
	gdb_arm_thread_vfpv3_state_t v3;
      } fp_regs;

      switch (vfp_version)
	{
	  case ARM_VFP_UNSUPPORTED:
	    /* No VFP support, so nothing to do: */
	    fp_data_size = 0;
	    break;
	  case ARM_VFP_VERSION_1:
	    {
	      arm_macosx_store_vfpv1_regs(&fp_regs.v1);
	      fp_data_size = sizeof(gdb_arm_thread_vfpv1_state_t);
	    }
	    break;
	  case ARM_VFP_VERSION_3:
	    {
	      arm_macosx_store_vfpv3_regs(&fp_regs.v3);
	      fp_data_size = sizeof(gdb_arm_thread_vfpv3_state_t);
	    }
	    break;

	  default:
	    break;
	}

      if (fp_data_size < 0)
	{
	    error("kdp_store_registers_arm: unable to store ARM_THREAD_FPSTATE: "
		  "unsupported vfp version: %d", (int)vfp_version);
	}
      else if (fp_data_size > 0)
	{
	  memcpy(c.response->readregs_reply.data, &fp_regs, fp_data_size);

          c.request->writeregs_req.hdr.request = KDP_WRITEREGS;
          c.request->writeregs_req.cpu = 0;
          c.request->writeregs_req.flavor = GDB_ARM_THREAD_FPSTATE;
	  c.request->writeregs_req.nbytes = fp_data_size;

	  kdpret = kdp_transaction(&c, c.request, c.response,
                                   "kdp_store_registers_arm");

          if (kdpret != RR_SUCCESS)
            {
              error("kdp_store_registers_arm: unable to store ARM_THREAD_FPSTATE: %s",
                    kdp_return_string(kdpret));
            }
        }
    }
# endif /* KDP_TARGET_SUPPORTS_FP */
}
#endif /* KDP_TARGET_ARM */

#if defined(KDP_TARGET_I386) && KDP_TARGET_I386
static void
kdp_store_registers_x86_64(int regno)
{
  if (!kdp_is_connected(&c))
    {
      error("kdp: unable to store registers (not connected)");
    }

  if ((regno == -1) || IS_GP_REGNUM_64(regno))
    {
      gdb_x86_thread_state64_t gp_regs;
      kdp_return_t kdpret;

      x86_64_macosx_store_gp_registers(&gp_regs);

      memcpy(c.request->writeregs_req.data, &gp_regs,
             (GDB_x86_THREAD_STATE64_COUNT * 4));

      c.request->writeregs_req.hdr.request = KDP_WRITEREGS;
      c.request->writeregs_req.cpu = 0;
      c.request->writeregs_req.flavor = GDB_x86_THREAD_STATE64;
      c.request->writeregs_req.nbytes = (GDB_x86_THREAD_STATE64_COUNT * 4);

      kdpret =
        kdp_transaction(&c, c.request, c.response,
                        "kdp_store_registers_x86_64");
      if (kdpret != RR_SUCCESS)
        {
          error("kdp_store_registers_x86_64: unable to store x86_THREAD_STATE64: %s",
                kdp_return_string(kdpret));
        }
    }
}
#endif /* KDP_TARGET_I386 */

static void
kdp_store_registers(int regno)
{
  if (!kdp_is_connected(&c))
    {
      error("kdp: unable to store registers (not connected)");
    }

  switch (kdp_cpu_type)
    {
    case CPU_TYPE_POWERPC:
#if defined(KDP_TARGET_POWERPC) && KDP_TARGET_POWERPC
      kdp_store_registers_ppc(regno);
#else
      error("kdp_store_registers: not configured to support powerpc");
#endif /* KDP_TARGET_POWERPC */
      break;

    case CPU_TYPE_X86_64:
#if defined(KDP_TARGET_I386) && KDP_TARGET_I386
      kdp_store_registers_x86_64(regno);
#else
      error("kdp_store_registers: not configured to support x86_64");
#endif /* KDP_TARGET_I386 */
      break;

    case CPU_TYPE_I386:
#if defined(KDP_TARGET_I386) && KDP_TARGET_I386
      kdp_store_registers_i386(regno);
#else
      error("kdp_store_registers: not configured to support i386");
#endif /* KDP_TARGET_I386 */
      break;

    case CPU_TYPE_ARM:
#if defined(KDP_TARGET_ARM) && KDP_TARGET_ARM
      kdp_store_registers_arm(regno);
#else
      error("kdp_store_registers: not configured to support arm");
#endif /* KDP_TARGET_ARM */
      break;

    default:
      error("kdp_store_registers: unknown host type 0x%lx",
            (unsigned long)kdp_host_type);
    }
}

static void
kdp_fetch_registers(int regno)
{
  if (!kdp_is_connected(&c))
    {
      error("kdp: unable to fetch registers (not connected)");
    }

  switch (kdp_cpu_type)
    {
    case CPU_TYPE_POWERPC:
#if defined(KDP_TARGET_POWERPC) && KDP_TARGET_POWERPC
      kdp_fetch_registers_ppc(regno);
#else
      error("kdp_fetch_registers: not configured to support powerpc");
#endif /* KDP_TARGET_POWERPC */
      break;

    case CPU_TYPE_I386:
#if defined(KDP_TARGET_I386) && KDP_TARGET_I386
      kdp_fetch_registers_i386(regno);
#else
      error("kdp_fetch_registers: not configured to support i386");
#endif /* KDP_TARGET_I386 */
      break;

    case CPU_TYPE_ARM:
#if defined(KDP_TARGET_ARM) && KDP_TARGET_ARM
      kdp_fetch_registers_arm(regno);
#else
      error("kdp_fetch_registers: not configured to support arm");
#endif /* KDP_TARGET_ARM */
      break;

    case CPU_TYPE_X86_64:
#if defined(KDP_TARGET_I386) && KDP_TARGET_I386
      kdp_fetch_registers_x86_64(regno);
#else
      error("kdp_fetch_registers: not configured to support i386");
#endif /* KDP_TARGET_I386 */
      break;

    default:
      error("kdp_fetch_registers: unknown host type 0x%lx",
            (unsigned long)kdp_host_type);
    }
}

static void
kdp_prepare_to_store(void)
{
  kdp_fetch_registers(-1);
}

static int
kdp_xfer_memory(CORE_ADDR memaddr, gdb_byte *myaddr, int len, int write,
                struct mem_attrib *attrib, struct target_ops *target)
{
  kdp_return_t kdpret;

  if (!kdp_is_connected(&c))
    {
      logger(KDP_LOG_DEBUG,
             "kdp: unable to transfer memory (not connected)");
      return 0;
    }

  if (len > KDP_MAX_DATA_SIZE)
    {
      len = KDP_MAX_DATA_SIZE;
    }

  if (write)
    {
      if (remote_kdp_version >= 11) {
	c.request->writemem64_req.hdr.request = KDP_WRITEMEM64;
	c.request->writemem64_req.address = memaddr;
	c.request->writemem64_req.nbytes = len;
	memcpy(c.request->writemem64_req.data, myaddr, len);
      } else {
        c.request->writemem_req.hdr.request = KDP_WRITEMEM;
        c.request->writemem_req.address = (unsigned int)memaddr;
        c.request->writemem_req.nbytes = len;
        memcpy(c.request->writemem_req.data, myaddr, len);
      }

      kdpret = kdp_transaction(&c, c.request, c.response, "kdp_xfer_memory");
      if (kdpret == RR_SUCCESS) {
	if (remote_kdp_version >= 11) {
	  kdpret = (kdp_return_t)c.response->writemem64_reply.error;
	} else {
          kdpret = (kdp_return_t)c.response->writemem_reply.error;
        }
      }
      if (kdpret != RR_SUCCESS)
        {
          logger(KDP_LOG_DEBUG,
                 "kdp_xfer_memory: unable to store %d bytes at 0x%s: %s\n",
                 len, paddr_nz(memaddr), kdp_return_string(kdpret));
          return 0;
        }
    }
  else
    {
      if (remote_kdp_version >= 11) {
	c.request->readmem64_req.hdr.request = KDP_READMEM64;
	c.request->readmem64_req.address = memaddr;
	c.request->readmem64_req.nbytes = len;
      } else {
        c.request->readmem_req.hdr.request = KDP_READMEM;
        c.request->readmem_req.address = (unsigned int)memaddr;
        c.request->readmem_req.nbytes = len;
      }

      kdpret = kdp_transaction(&c, c.request, c.response, "kdp_xfer_memory");
      if (kdpret == RR_SUCCESS) {
	if (remote_kdp_version >= 11) {
	  kdpret = (kdp_return_t)c.response->readmem64_reply.error;
	} else {
          kdpret = (kdp_return_t)c.response->readmem_reply.error;
        }
      }
      if (kdpret != RR_SUCCESS)
        {
          logger(KDP_LOG_DEBUG,
                 "kdp_xfer_memory: unable to fetch %d bytes from 0x%s: %s\n",
                 len, paddr_nz(memaddr), kdp_return_string(kdpret));
          return 0;
        }
	if (remote_kdp_version >= 11) {
	  if (c.response->readmem64_reply.nbytes != (size_t)len)
	    {
	      logger(KDP_LOG_DEBUG,
		     "kdp_xfer_memory: kdp read only %d bytes of data (expected %d)\n",
		     c.response->readmem64_reply.nbytes, len);
	      return 0;
	    }
	  memcpy(myaddr, c.response->readmem64_reply.data, len);
	}
      else
        {
          if (c.response->readmem_reply.nbytes != (size_t)len)
            {
              logger(KDP_LOG_DEBUG,
                     "kdp_xfer_memory: kdp read only %d bytes of data (expected %d)\n",
                     c.response->readmem_reply.nbytes, len);
              return 0;
            }
	  memcpy(myaddr, c.response->readmem_reply.data, len);
	}
    }

  return len;
}

static void
kdp_files_info(struct target_ops *ops)
{
  printf_unfiltered("\tNo connection information available.\n");
}

static void
kdp_kill(void)
{
  kdp_detach("", 0);
}

/* */
static void ATTR_NORETURN
kdp_load(const char *args ATTRIBUTE_UNUSED, int from_tty ATTRIBUTE_UNUSED)
{
  error(_("unsupported operation kdp_load"));
}

static void ATTR_NORETURN
kdp_create_inferior(char *execfile, char *args, char **env, int fromtty)
{
  error("unsupported operation kdp_create_inferior");
}

static void
kdp_mourn_inferior(void)
{
  unpush_target(&kdp_ops);
  generic_mourn_inferior();
}

static int remote_async_terminal_ours_p = 1;
static void (*ofunc)(int);
static PTR sigint_remote_twice_token;
static PTR sigint_remote_token;

static void remote_interrupt_twice(int signo);
static void remote_interrupt(int signo);
static void handle_remote_sigint_twice(int sig);
static void handle_remote_sigint(int sig);
static void async_remote_interrupt_twice(gdb_client_data arg);
static void async_remote_interrupt(gdb_client_data arg);

static void
interrupt_query(void)
{
  target_terminal_ours();

  if (query("Interrupted while waiting for the program.\n\
Give up (and stop debugging it)? "))
    {
      target_mourn_inferior();
      deprecated_throw_reason(RETURN_QUIT);
    }

  target_terminal_inferior();
}

static void
remote_interrupt_twice(int signo)
{
  signal(signo, ofunc);
  interrupt_query();
  signal(signo, remote_interrupt);
}

static void
remote_interrupt(int signo)
{
  signal(signo, remote_interrupt_twice);
  target_stop();
}

static void
handle_remote_sigint_twice(int sig)
{
  signal(sig, handle_sigint);
  sigint_remote_twice_token =
    create_async_signal_handler(inferior_event_handler_wrapper, NULL);
  mark_async_signal_handler_wrapper(sigint_remote_twice_token);
}

static void
handle_remote_sigint(int sig)
{
  signal(sig, handle_remote_sigint_twice);
  sigint_remote_twice_token =
    create_async_signal_handler(async_remote_interrupt_twice, NULL);
  mark_async_signal_handler_wrapper(sigint_remote_token);
}

static void
async_remote_interrupt_twice(gdb_client_data arg)
{
  if (target_executing)
    {
      signal(SIGINT, handle_remote_sigint);
      interrupt_query();
    }
}

static void
async_remote_interrupt(gdb_client_data arg)
{
  if (remote_debug)
    fprintf_unfiltered(gdb_stdlog, "%s called\n", __extension__ __FUNCTION__);

  if (current_target.to_stop == target_ignore)
    {
      target_terminal_ours();

      if (yquery("remote-kdp cannot stop a running kernel.\n\
Would you like to disconnect? "))
	{
	  kdp_return_t kdpret;
	  kdp_ops.to_has_all_memory = 0;
	  kdp_ops.to_has_memory = 0;
	  kdp_ops.to_has_stack = 0;
	  kdp_ops.to_has_registers = 0;
	  kdp_ops.to_has_execution = 0;

	  update_current_target();

	  if (kdp_is_bound(&c))
	    {
	      kdpret = kdp_destroy(&c);
	      if (kdpret != RR_SUCCESS)
		{
		  error("unable to deallocate KDP connection: %s",
                        kdp_return_string(kdpret));
		}
	    }
	  target_mourn_inferior();
	  target_executing = 0;
	  printf_unfiltered("Disconnected.\n");
	  deprecated_throw_reason(RETURN_QUIT);
	}
      else
	{
	  target_terminal_inferior();
	}
    }
  else
    {
      target_stop();
    }
}

static void
cleanup_sigint_signal_handler(void *dummy)
{
  signal(SIGINT, handle_sigint);
  if (sigint_remote_twice_token)
    delete_async_signal_handler((struct async_signal_handler **)
                                &sigint_remote_twice_token);
  if (sigint_remote_token)
    delete_async_signal_handler((struct async_signal_handler **)
                                &sigint_remote_token);
}

static void
initialize_sigint_signal_handler(void)
{
  sigint_remote_token =
    create_async_signal_handler(async_remote_interrupt, NULL);
  signal(SIGINT, handle_remote_sigint);
}

static void
kdp_terminal_inferior(void)
{
#if 0
  terminal_inferior();
#endif /* 0 */

  if (!sync_execution)
    return;
  if (!remote_async_terminal_ours_p)
    return;
  CHECK_FATAL(sync_execution);
  CHECK_FATAL(remote_async_terminal_ours_p);
  delete_file_handler(input_fd);
  remote_async_terminal_ours_p = 0;
  initialize_sigint_signal_handler();
}

static void
kdp_terminal_ours(void)
{
#if 0
  terminal_ours();
#endif /* 0 */

  if (!sync_execution)
    return;
  if (remote_async_terminal_ours_p)
    return;
  CHECK_FATAL(sync_execution);
  CHECK_FATAL(!remote_async_terminal_ours_p);
  cleanup_sigint_signal_handler(NULL);

  add_file_handler(input_fd, stdin_event_handler, 0);

  remote_async_terminal_ours_p = 1;
}

static void (*async_client_callback)(enum inferior_event_type event_type,
                                     void *context);
static void *async_client_context;

static void
kdp_file_handler(int error, gdb_client_data client_data)
{
  async_client_callback(INF_REG_EVENT, async_client_context);
}

static void
kdp_file_handler_callback(void *arg)
{
  async_client_callback(INF_REG_EVENT, async_client_context);
}

static void
kdp_async(void (*callback)(enum inferior_event_type event_type,
                           void *context), void *context)
{
  if (current_target.to_async_mask_value == 0)
    internal_error(__FILE__, __LINE__,
                   "Calling remote_async when async is masked");

  if (callback != NULL)
    {
      async_client_callback = callback;
      async_client_context = context;
      if (c.reqfd > 0)
        add_file_handler(c.reqfd, kdp_file_handler, NULL);
      if (c.excfd > 0)
        add_file_handler(c.excfd, kdp_file_handler, NULL);
    }
  else
    {
      if (c.reqfd > 0)
        delete_file_handler(c.reqfd);
      if (c.excfd > 0)
        delete_file_handler(c.excfd);
    }

  if ((callback != NULL) && (c.saved_exception_pending))
    {
      gdb_queue_event(kdp_file_handler_callback, (void *)0, TAIL);
    }
}

/* This set of hardware watchpoint stubs currently do nothing, but we
   have to be sure to set them, so we do NOT pick up the ones from the
   macosx "exec" target.  */

int ATTRIBUTE_CONST
kdp_can_use_hw_breakpoint(int unused1 ATTRIBUTE_UNUSED,
                          int unused2 ATTRIBUTE_UNUSED,
                          int unused3 ATTRIBUTE_UNUSED)
{
  return 0;
}

/* */
int ATTRIBUTE_CONST
kdp_stopped_by_watchpoint(void)
{
  return 0;
}

/* */
int ATTRIBUTE_CONST
kdp_stopped_data_address(struct target_ops *unused1 ATTRIBUTE_UNUSED,
                         CORE_ADDR *unused2 ATTRIBUTE_UNUSED)
{
  return 0;
}

/* */
int ATTRIBUTE_CONST
kdp_insert_watchpoint(CORE_ADDR unused1 ATTRIBUTE_UNUSED,
                      int unused2 ATTRIBUTE_UNUSED,
                      int unused3 ATTRIBUTE_UNUSED)
{
  return 0;
}

/* */
int ATTRIBUTE_CONST
kdp_remove_watchpoint(CORE_ADDR unused1 ATTRIBUTE_UNUSED,
                      int unused2 ATTRIBUTE_UNUSED,
                      int unused3 ATTRIBUTE_UNUSED)
{
  return 0;
}

/* */
int ATTRIBUTE_CONST
kdp_insert_hw_breakpoint(CORE_ADDR unused1 ATTRIBUTE_UNUSED,
                         gdb_byte *unused2 ATTRIBUTE_UNUSED)
{
  return 0;
}

/* */
int ATTRIBUTE_CONST
kdp_remove_hw_breakpoint(CORE_ADDR unused1 ATTRIBUTE_UNUSED,
                         gdb_byte *unused2 ATTRIBUTE_UNUSED)
{
  return 0;
}

static NORETURN CORE_ADDR ATTR_NORETURN
kdp_allocate_memory(int size ATTRIBUTE_UNUSED)
{
  error(_("KDP cannot allocate memory in the kernel being debugged."));
  /*NOTREACHED*/
}

static void
init_kdp_ops(void)
{
  kdp_ops.to_shortname = "remote-kdp";
  kdp_ops.to_longname = "Remote Mac OS X system via KDP";
  kdp_ops.to_doc = "Remotely debug a Mac OS X system using KDP\n\
    Use the attach <hostname> command subsequently to connect via KDP.";
  kdp_ops.to_open = kdp_open;
  kdp_ops.to_close = kdp_close;
  kdp_ops.to_attach = kdp_attach;
  kdp_ops.to_resume = kdp_resume;
  kdp_ops.to_wait = kdp_wait;
  kdp_ops.to_fetch_registers = kdp_fetch_registers;
  kdp_ops.to_store_registers = kdp_store_registers;
  kdp_ops.to_prepare_to_store = kdp_prepare_to_store;
  kdp_ops.deprecated_xfer_memory = kdp_xfer_memory;
  kdp_ops.to_files_info = kdp_files_info;
  kdp_ops.to_insert_breakpoint = memory_insert_breakpoint;
  kdp_ops.to_remove_breakpoint = memory_remove_breakpoint;
  kdp_ops.to_can_use_hw_breakpoint = kdp_can_use_hw_breakpoint;
  kdp_ops.to_stopped_by_watchpoint = kdp_stopped_by_watchpoint;
  kdp_ops.to_stopped_data_address = kdp_stopped_data_address;
  kdp_ops.to_insert_watchpoint = kdp_insert_watchpoint;
  kdp_ops.to_remove_watchpoint = kdp_remove_watchpoint;
  kdp_ops.to_insert_hw_breakpoint = kdp_insert_hw_breakpoint;
  kdp_ops.to_remove_hw_breakpoint = kdp_remove_hw_breakpoint;
  kdp_ops.to_have_continuable_watchpoint = 0;
  kdp_ops.to_detach = kdp_detach;
  kdp_ops.to_kill = kdp_kill;
  kdp_ops.to_load = kdp_load;
  kdp_ops.to_create_inferior = kdp_create_inferior;
  kdp_ops.to_mourn_inferior = kdp_mourn_inferior;
  kdp_ops.to_stratum = process_stratum;
  kdp_ops.to_can_async_p = standard_can_async_p;
  kdp_ops.to_is_async_p = standard_is_async_p;
  kdp_ops.to_terminal_inferior = kdp_terminal_inferior;
  kdp_ops.to_terminal_ours = kdp_terminal_ours;
  kdp_ops.to_async = kdp_async;
  kdp_ops.to_async_mask_value = 1;
  kdp_ops.to_allocate_memory = kdp_allocate_memory;
  kdp_ops.to_magic = OPS_MAGIC;
}

/* */
static void
update_kdp_default_host_type(const char *args, int from_tty,
                             struct cmd_list_element *c)
{
  int htype;

  if (args == NULL)
    {
      args = (char *)kdp_default_host_type_str;
    }
  htype = parse_host_type(args);
  if (htype < 0)
    {
      if (htype == -2)
        {
          error("Known but unsupported host type: \"%s\".", args);
        }
      else
        {
          error("Unknown host type: \"%s\".", args);
        }
    }

  kdp_default_cpu_type = htype;
}

void
_initialize_remote_kdp(void)
{
  static const char *archlist[] = { "powerpc", "ia32", NULL };

  init_kdp_ops();
  add_target(&kdp_ops);

  add_com("kdp-reattach", class_run, kdp_reattach_command,
          "Re-attach to a (possibly connected) remote Mac OS X kernel.\nThe kernel must support the reattach packet.");
  add_com("kdp-reboot", class_run, kdp_reboot_command,
          "Reboot a connected remote Mac OS X kernel.\nThe kernel must support the reboot packet.");
  add_com("kdp-detach", class_run, kdp_detach_command,
          "Reset a (possibly disconnected) remote Mac OS X kernel.\n");
  add_com("kdp-kernelversion", class_run, kdp_kernelversion_command,
          "Print the version of a remote Mac OS X kernel.\n");

  add_setshow_enum_cmd("kdp-default-host-type", class_obscure, archlist,
                       (const char **)&kdp_default_host_type_str, _("\
Set CPU type to be used for hosts providing incorrect information (powerpc/ia32)."), _("\
Show CPU type to be used for hosts providing incorrect information (powerpc/ia32)."), _("\
No additional help."), update_kdp_default_host_type, NULL,
                       &setlist, &showlist);

  add_setshow_zinteger_cmd("kdp-timeout",
                           class_obscure, &kdp_timeout, _("\
Set UDP timeout in milliseconds for (non-exception) KDP transactions."), _("\
Show UDP timeout in milliseconds for (non-exception) KDP transactions."), _("\
No additional help."), set_timeouts, NULL, &setlist, &showlist);

  add_setshow_zinteger_cmd("kdp-retries",
                           class_obscure, &kdp_retries, _("\
Set number of UDP retries for (non-exception) KDP transactions."), _("\
Show number of UDP retries for (non-exception) KDP transactions."), _("\
No additional help."), set_timeouts, NULL, &setlist, &showlist);

  add_setshow_uinteger_cmd("kdp-default-port",
                           class_obscure, &kdp_default_port, _("\
Set default UDP port on which to attempt to contact KDP."), _("\
Show default UDP port on which to attempt to contact KDP."), _("\
No additional help."), NULL, NULL, &setlist, &showlist);

  add_setshow_uinteger_cmd("kdp-debug-level",
                           class_obscure, &kdp_debug_level, _("\
Set level of verbosity for KDP debugging information."), _("\
Show level of verbosity for KDP debugging information."), _("\
No additional help."), NULL, NULL, &setlist, &showlist);

  add_setshow_uinteger_cmd("kdp-sequence-number",
                           class_obscure, &c.seqno, _("\
Set current sequence number for KDP transactions."), _("\
Show current sequence number for KDP transactions."), _("\
No additional help."), NULL, NULL, &setlist, &showlist);

  add_setshow_uinteger_cmd("kdp-exception-sequence-number",
                           class_obscure, &c.exc_seqno, _("\
Set current sequence number for KDP exception transactions."), _("\
Show current sequence number for KDP exception transactions."), _("\
No additional help."), NULL, NULL, &setlist, &showlist);

  kdp_reset(&c);
}

/* EOF */
