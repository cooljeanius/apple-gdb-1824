/* macosx/macosx-nat-dyld-process.c: Mac OS X support for GDB, the GNU debugger.
   Copyright 1997, 1998, 1999, 2000, 2001, 2002, 2004
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

/* define this because of a header for mmalloc that needs fixing: */
#if !defined(NO_POISON) && defined(USE_MMALLOC)
# define NO_POISON 1
#endif /* !NO_POISON && USE_MMALLOC */
#include "defs.h"
#include "inferior.h"
#include "symtab.h"
#include "gdbcmd.h"
#include "objfiles.h"
#include "mach-o.h"
#include "gdbcore.h"
#include "gdb_regex.h"
#include "gdb-stabs.h"
#include "gdb_assert.h"
#include "interps.h"
#include "objc-lang.h"
#include "gdb_stat.h"
#include "arch-utils.h"
#include "gdbarch.h"
#include "symfile.h"

#ifndef _I386_LIMITS_H_
# include <limits.h>
#endif /* !_I386_LIMITS_H_ */

/* we already included "gdb_stat.h" once above */

#include <mach-o/nlist.h>
#include <mach-o/loader.h>

#ifdef USE_MMALLOC
# if !defined(__MMPRIVATE_H)
#  include "mmprivate.h"
# endif /* !__MMPRIVATE_H */
#endif /* USE_MMALLOC */

#if defined(TARGET_POWERPC)
# include "ppc-macosx-tdep.h"
#elif defined(TARGET_I386)
# include "target/i386-tdep.h"
#elif defined(TARGET_ARM)
# include "arm-tdep.h"
# if !defined(_TM_NEXTSTEP_H_)
#  include "tm-macosx.h"
# endif /* !_TM_NEXTSTEP_H_ */
#elif defined(TARGET_AARCH64)
# include "aarch64-tdep.h"
#endif /* TARGET_POWERPC || TARGET_I386 || TARGET_ARM || TARGET_AARCH64 */

#include <sys/mman.h>
#include <string.h>
#include <libintl.h>

#include "macosx-nat-dyld-info.h"
#include "macosx-nat-dyld-path.h"
#include "macosx-nat-dyld-io.h"
#include "macosx-nat-dyld.h"
#include "macosx-nat-inferior.h"
#include "macosx-nat-mutils.h"
#include "macosx-nat-dyld-process.h"

#ifndef INVALID_ADDRESS
# define INVALID_ADDRESS ((CORE_ADDR)(-1L))
#endif /* !INVALID_ADDRESS */

extern int dyld_preload_libraries_flag;
extern int dyld_filter_events_flag;
extern int dyld_always_read_from_memory_flag;
/* declaration for dyld_symbols_prefix is now in macosx-nat-dyld.h */
extern int dyld_load_dyld_symbols_flag;
extern int dyld_load_dyld_shlib_symbols_flag;
extern int dyld_load_cfm_shlib_symbols_flag;
extern int dyld_print_basenames_flag;
extern int dyld_reload_on_downgrade_flag;
extern char *dyld_load_rules;
extern char *dyld_minimal_load_rules;

static int dyld_check_uuids_flag = 0;

/* For the gdbarch_tdep structure so we can get the wordsize. */
#if defined(TARGET_POWERPC)
# include "ppc-tdep.h"
#elif defined(TARGET_I386)
# include "target/amd64-tdep.h"
# include "target/i386-tdep.h"
#elif defined(TARGET_ARM)
# include "arm-tdep.h"
#elif defined(TARGET_AARCH64)
# include "aarch64-tdep.h"
#else
# ifdef S_SPLINT_S
#  include "macosx-tdep.h"
# else
#  error "Unrecognized target architecture."
# endif /* S_SPLINT_S */
#endif /* TARGET */

#if WITH_CFM
extern int inferior_auto_start_cfm_flag;
#endif /* WITH_CFM */

extern macosx_inferior_status *macosx_status;
extern macosx_dyld_thread_status macosx_dyld_status;

/* call tracing */
#if defined(DEBUG) || defined(_DEBUG) || defined(GDB_DEBUG) || defined(stderr)
# ifndef DYLD_TRACE_NUM_DECLARED
#  define DYLD_TRACE_NUM_DECLARED 1
int dyld_trace_num = 0;
# endif /* !DYLD_TRACE_NUM_DECLARED */
# define NSTRACE(x)        fprintf(stderr, "%s:%d: [%d] " #x "\n",        \
				   __FILE__, __LINE__, ++dyld_trace_num)
#else
# define NSTRACE(x)
#endif /* DEBUG || _DEBUG || GLYPH_DEBUG || stderr */

/* We describe the memory that a bfd will occupy when loaded as a
   series of memory groups.  If a file's text+data loads together, there
   will be one group.  If its text and data are separate, there are two
   groups.  There is no limit on how many groups are supported but there
   are never more than two today.  */

struct bfd_memory_footprint {
  const char *filename;         /* filename of this bfd */
  CORE_ADDR seg1addr;           /* Address of the __TEXT segment */
  int num;                      /* Number of bucket groupings in use */
  int num_allocated;            /* Number of bucket grouping slots allocated */
  struct bfd_memory_footprint_group *groups;
};

struct bfd_memory_footprint_group {
  int offset;    /* Offset from this file's seg1addr in terms of buckets */
  int length;    /* Length in buckets */
};

static void mark_buckets_as_used(struct pre_run_memory_map *map, int i,
                                 struct bfd_memory_footprint *fp);

extern void _initialize_macosx_nat_dyld_process(void);

/* */
static int
dyld_print_status(void)
{
  NSTRACE(dyld_print_status);
  /* do not print status dots when executing MI: */
  return !ui_out_is_mi_like_p(uiout);
}

/* */
void
dyld_add_inserted_libraries(struct dyld_objfile_info *info,
                            const struct dyld_path_info *d)
{
  const char *s1, *s2;

  NSTRACE(dyld_add_inserted_libraries);

  CHECK_FATAL(info != NULL);
  CHECK_FATAL(d != NULL);

  s1 = d->insert_libraries;
  if (s1 == NULL)
    {
      return;
    }

  while (*s1 != '\0')
    {
      struct dyld_objfile_entry *e = NULL;
      char *tmp_name, *real_name, *fixed_name;

      s2 = strchr(s1, ':');
      if (s2 == NULL)
        {
          s2 = strchr(s1, '\0');
        }
      CHECK_FATAL(s2 != NULL);

      tmp_name = savestring(s1, (s2 - s1));
      fixed_name = dyld_fix_path(tmp_name);
      xfree(tmp_name);
      tmp_name = fixed_name;

      real_name = (char *)xmalloc(PATH_MAX + 1);
      if (realpath(tmp_name, real_name) != NULL)
	{
	  e = dyld_objfile_entry_alloc(info);
	  e->user_name = real_name;
	  e->reason = dyld_reason_init;
	}
      else
	warning("Could NOT get real path for inserted library %s\n",
                tmp_name);

      xfree (tmp_name);
      s1 = s2;
      while (*s1 == ':')
        {
          s1++;
        }
    }
}

/* Look through the main executable for directly linked
   dylibs/framework load commands; create dyld_objfile_entries for those.  */
void
dyld_add_image_libraries(struct dyld_objfile_info *info, bfd *abfd)
{
  struct mach_o_data_struct *mdata = NULL;
  unsigned int i;

  NSTRACE(dyld_add_image_libraries);

  CHECK_FATAL(info != NULL);

  if (abfd == NULL)
    {
      return;
    }

  if (!bfd_mach_o_valid(abfd))
    {
      return;
    }

  mdata = abfd->tdata.mach_o_data;

  if (mdata == NULL)
    {
      dyld_debug(_("dyld_add_image_libraries: mdata == NULL\n"));
      return;
    }

  for (i = 0U; i < mdata->header.ncmds; i++)
    {
      struct bfd_mach_o_load_command *cmd = &mdata->commands[i];
      switch (cmd->type)
        {
        case BFD_MACH_O_LC_REEXPORT_DYLIB:
        case BFD_MACH_O_LC_LOAD_WEAK_DYLIB:
        case BFD_MACH_O_LC_LOAD_DYLINKER:
        case BFD_MACH_O_LC_LOAD_DYLIB:
        case BFD_MACH_O_LC_LOAD_UPWARD_DYLIB:
          {
            struct dyld_objfile_entry *e = NULL;
            char *name = NULL;
	    char *fixed_name = NULL;

            switch (cmd->type)
              {
              case BFD_MACH_O_LC_LOAD_DYLINKER:
                {
                  bfd_mach_o_dylinker_command *dcmd = &cmd->command.dylinker;

                  name = (char *)xmalloc(dcmd->name_len + 1);

                  bfd_seek(abfd, dcmd->name_offset, SEEK_SET);
                  if (bfd_bread(name, dcmd->name_len, abfd) != dcmd->name_len)
                    {
                      warning(_("Unable to find library name for LC_LOAD_DYLINKER command; ignoring"));
                      xfree(name);
                      continue;
                    }
		  name[dcmd->name_len] = '\0';
		  fixed_name = dyld_fix_path(name);
                  xfree(name);
                  name = fixed_name;
                  break;
                }
              case BFD_MACH_O_LC_LOAD_DYLIB:
              case BFD_MACH_O_LC_LOAD_UPWARD_DYLIB:
              case BFD_MACH_O_LC_LOAD_WEAK_DYLIB:
              case BFD_MACH_O_LC_REEXPORT_DYLIB:
                {
                  bfd_mach_o_dylib_command *dcmd = &cmd->command.dylib;

                  name = (char *)xmalloc(dcmd->name_len + 1);

                  bfd_seek(abfd, dcmd->name_offset, SEEK_SET);
                  if (bfd_bread(name, dcmd->name_len, abfd) != dcmd->name_len)
                    {
		      const char *lcs_str =
			"LC_LOAD_DYLIB, LC_LOAD_UPWARD_DYLIB, LC_LOAD_WEAK_DYLIB or LC_REEXPORT_DYLIB command";
                      warning("Unable to find library name for %s; ignoring",
			      lcs_str);
                      xfree(name);
                      name = NULL;
                      continue;
                    }
		  name[dcmd->name_len] = '\0';

                  /* Ignore dylibs starting with "@rpath" because we do NOT
                     know how to resolve them correctly yet. This means
                     people who want to put breakpoints in one of these
                     dylibs will have to use a future-breakpoint instead;
                     not the end of the world.  Ditto for @loader_path.  */
                  if ((name[0] == '@')
		      && ((strncmp(name, "@rpath", 6) == 0)
			  || (strncmp(name, "@loader_path", 12) == 0)))
                    {
                      xfree(name);
                      name = NULL;
                      break;
                    }

		  fixed_name = dyld_fix_path(name);
                  xfree(name);
                  name = fixed_name;
                  break;
                }
              default:
                abort();
              }

            /* If NAME is null, this is an @rpath dylib that
               we want to skip over.  */
            if (name == NULL)
              continue;

            if (name[0] == '\0')
              {
                warning("No image name specified by LC_LOAD command; ignoring");
                xfree(name);
                name = NULL;
              }

            /* We have to run realpath on the text name here because
               some makefiles build the library with one name, then
               copy it to another.  For instance, they will build
               libfoo.2.dylib, but install the actual binary as
               libfoo.2.1.dylib, and then link libfoo.2.dylib back
               to this.  That means the load command refers to a
               file that exists (as a link) but is NOT the same as
               what gets loaded.  If we canonicalize everything to
               the real file, then we will NOT get fooled by this. */

            {
              char buf[PATH_MAX];

              if (realpath(name, buf) != NULL)
                {
                  xfree(name);
                  name = xstrdup(buf);
                }
            }

            /* If a dylib is mentioned more than once, only read it once.
               This can come up if we have a soft-link where a dylib used
               to be.  e.g. if libgcc_s is incorporated into libsystem and
               an executable links against libgcc_s and libsystem, we realpath
               these and end up with two entries for libsystem.  Which means
               double the breakpoints double the fun for any breakpoints on
               libsystem functions.  */
            if (name)
              {
                int j, skip_this_dylib = 0;
                DYLD_ALL_OBJFILE_INFO_ENTRIES(info, e, j)
                  {
                    if ((e->reason == dyld_reason_init)
                        && (e->text_name_valid == 1)
                        && e->text_name
                        && (strcmp(e->text_name, name) == 0))
                      skip_this_dylib = 1;
                  }
                if (skip_this_dylib)
                  {
                    xfree(name);
                    name = NULL;
                    continue;
                  }
              }

            e = dyld_objfile_entry_alloc(info);
            e->text_name = name;
            e->text_name_valid = 1;
            e->reason = dyld_reason_init;

            /* Do NOT accidentally xfree() this now that e->text_name
             * is holding a pointer to it: */
            name = NULL;

            switch (cmd->type)
              {
              case BFD_MACH_O_LC_LOAD_DYLINKER:
                e->prefix = dyld_symbols_prefix;
                break;
              case BFD_MACH_O_LC_LOAD_DYLIB:
              case BFD_MACH_O_LC_LOAD_UPWARD_DYLIB:
              case BFD_MACH_O_LC_REEXPORT_DYLIB:
                break;
              case BFD_MACH_O_LC_LOAD_WEAK_DYLIB:
                e->reason =
		  ((enum dyld_objfile_reason)
		   (e->reason
		    | (enum dyld_objfile_reason)dyld_reason_weak_mask));
                break;
              default:
                abort();
              };
          }
        default:
          break;
        }
    }
}

/* */
void
dyld_resolve_filename_image(const struct macosx_dyld_thread_status *s,
                            struct dyld_objfile_entry *e)
{
  struct mach_header header;
  NSTRACE(dyld_resolve_filename_image);
  CHECK_FATAL(e->allocated);
  if (e->image_name_valid)
    {
      return;
    }

  if (!e->dyld_valid)
    {
      return;
    }

  target_read_memory(e->dyld_addr, (gdb_byte *)&header,
                     sizeof(struct mach_header));

  switch (header.filetype)
    {
    case MH_DYLINKER:
    case MH_DYLIB:
      break;
    case MH_BUNDLE:
#ifdef MH_KEXT_BUNDLE
    case MH_KEXT_BUNDLE:
#endif /* MH_KEXT_BUNDLE */
      break;
    default:
      return;
    }
  e->image_name = dyld_find_dylib_name(header.ncmds, header.cputype,
                                       (int)e->dyld_addr);

  if (e->image_name == NULL)
    {
      dyld_debug("Unable to determine filename for loaded object "
                 "(no LC_ID load command)\n");
    }
  else
    {
      dyld_debug("Determined filename for loaded object from image\n");
      e->image_name_valid = 1;
    }
}

/* Assuming a Mach header starts at ADDR, and has NCMDS, look for the
   dylib name, and return a malloc'ed string containing the name.
   CPUTYPE is the cpu_type_t of the binary image.  */
char *
dyld_find_dylib_name(CORE_ADDR addr, int cputype, int ncmds)
{
  CORE_ADDR curpos;
  int i;
  char *image_name = NULL;

  NSTRACE(dyld_find_dylib_name);

  if ((cputype == CPU_TYPE_X86_64) || (cputype == CPU_TYPE_POWERPC64))
    curpos = (addr + sizeof(struct mach_header_64));
  else
    curpos = (addr + sizeof(struct mach_header));

  for (i = 0; i < ncmds; i++)
    {
      struct load_command cmd;
      struct dylib_command dcmd;
      struct dylinker_command dlcmd;
      char name[256];

      target_read_memory(curpos, (gdb_byte *)&cmd,
                         sizeof(struct load_command));
      if (cmd.cmd == LC_ID_DYLIB)
        {
          target_read_memory(curpos, (gdb_byte *)&dcmd,
			     sizeof(struct dylib_command));
          target_read_memory((curpos + dcmd.dylib.name.offset),
			     (gdb_byte *)name, 256);
          image_name = savestring(name, strlen(name));
          break;
        }
      else if (cmd.cmd == LC_ID_DYLINKER)
        {
          target_read_memory(curpos, (gdb_byte *)&dlcmd,
                             sizeof(struct dylinker_command));
          target_read_memory((curpos + dlcmd.name.offset), (gdb_byte *)name,
			     256);
          image_name = savestring(name, strlen(name));
          break;
        }

      curpos += cmd.cmdsize;
    }
  return image_name;
}

/* */
void
dyld_resolve_filenames(const struct macosx_dyld_thread_status *s,
                       struct dyld_objfile_info *newinfo)
{
  int i;
  struct dyld_objfile_entry *e = NULL;

  NSTRACE(dyld_resolve_filenames);

  CHECK_FATAL(s != NULL);
  CHECK_FATAL(newinfo != NULL);

  DYLD_ALL_OBJFILE_INFO_ENTRIES(newinfo, e, i)
    {
      if (e->dyld_name_valid)
        continue;
      dyld_resolve_filename_image(s, e);
    }
}

/* */
static CORE_ADDR
library_offset(struct dyld_objfile_entry *e)
{
  int wordsize = new_gdbarch_tdep(current_gdbarch)->wordsize;
  NSTRACE(library_offset);
  CHECK_FATAL(e != NULL);
  if (e->image_addr_valid && e->dyld_valid)
    {
      if (wordsize == 4)
        CHECK_FATAL(e->dyld_addr ==
		    ((e->image_addr + e->dyld_slide) & 0xffffffff));
      else
        CHECK_FATAL(e->dyld_addr == (e->image_addr + e->dyld_slide));
    }

  if (e->dyld_valid)
    return e->dyld_addr;
  else if (e->loaded_from_memory)
    return e->loaded_addr;
  else if (e->image_addr_valid)
    if (e->pre_run_slide_addr_valid)
      if (wordsize == 4)
        return ((e->image_addr + e->pre_run_slide_addr) & 0xffffffff);
      else
        return (e->image_addr + e->pre_run_slide_addr);
    else
      return e->image_addr;
  else
    return 0;
}

/* */
int
dyld_parse_load_level(const char *s)
{
  NSTRACE(dyld_parse_load_level);
  if (strcmp(s, "all") == 0)
    {
      return OBJF_SYM_ALL;
    }
  else if (strcmp(s, "container") == 0)
    {
      return OBJF_SYM_CONTAINER;
    }
  else if (strcmp(s, "extern") == 0)
    {
      return OBJF_SYM_EXTERN;
    }
  else if (strcmp(s, "none") == 0)
    {
      return OBJF_SYM_NONE;
    }
  else
    {
      warning("unknown setting \"%s\"; using \"none\"\n", s);
      return OBJF_SYM_NONE;
    }
}

/* */
int
dyld_resolve_load_flag(const struct dyld_path_info *d,
                       struct dyld_objfile_entry *e, const char *rules)
{
  const char *name0 = NULL;
  const char *leaf = NULL;

  char **prules = NULL;
  char **trule = NULL;
  int nrules = 0;
  int crule = 0;

  NSTRACE(dyld_resolve_load_flag);

  name0 = dyld_entry_string(e, 1);

  if (name0 == NULL)
    return OBJF_SYM_NONE;

  leaf = strrchr(name0, '/');
  leaf = ((leaf != NULL) ? leaf : name0);

  if (leaf == NULL) {
    warning(_("possible issue with leaf"));
  }

  if (rules != NULL)
    {
      prules = buildargv(rules);
      if (prules == NULL)
        {
          warning(_("unable to parse load rules"));
          return OBJF_SYM_NONE;
        }
    }

  nrules = 0;

  if (prules != NULL)
    {
      for (trule = prules; *trule != NULL; trule++)
        {
          nrules++;
        }
    }

  if ((nrules % 3) != 0)
    {
      warning("unable to parse load-rules (number of rule clauses must be a "
	      "multiple of 3)");
      return OBJF_SYM_NONE;
    }
  nrules /= 3;

  for (crule = 0; crule < nrules; crule++)
    {
      char *matchreason = prules[crule * 3];
      char *matchname = prules[(crule * 3) + 1];
      char *setting = prules[(crule * 3) + 2];

      const char *reason = NULL;
      const char *name1 = NULL;

      regex_t reasonbuf;
      regex_t namebuf;

      int ret;

      switch (e->reason & dyld_reason_type_mask)
        {
        case dyld_reason_user:
          reason = "user";
          break;
        case dyld_reason_init:
          reason = "dyld";
          break;
        case dyld_reason_executable:
          reason = "exec";
          break;
        case dyld_reason_dyld:
          reason = "dyld";
          break;
        case dyld_reason_cfm:
          reason = "cfm";
          break;
        default:
          reason = "INVALID";
          break;
        }

      if (e->objfile)
        {
          if (e->loaded_from_memory)
            {
              name1 = "memory";
            }
          else
            {
              name1 = e->loaded_name;
            }
        }
      else
        {
          name1 = dyld_entry_filename(e, d, DYLD_ENTRY_FILENAME_LOADED);
          if (name1 == NULL)
            {
              if (!(e->reason & dyld_reason_weak_mask))
                {
                  warning(_("Unable to resolve \"%s\"; not loading."),
			  ((name1 != NULL) ? name1 : "name1"));
                }
              return OBJF_SYM_NONE;
            }
        }

      ret = regcomp(&reasonbuf, matchreason, REG_NOSUB);
      if (ret != 0)
        {
          warning("unable to compile regular expression \"%s\"",
                  matchreason);
          continue;
        }

      ret = regcomp(&namebuf, matchname, REG_NOSUB);
      if (ret != 0)
        {
          warning("unable to compile regular expression \"%s\"",
                  matchreason);
          continue;
        }

      ret = regexec(&reasonbuf, reason, 0, 0, 0);
      if (ret != 0)
        continue;

      ret = regexec(&namebuf, name1, 0, 0, 0);
      if (ret != 0)
        continue;

      return dyld_parse_load_level(setting);
    }

  return -1;
}

/* */
int
dyld_minimal_load_flag(const struct dyld_path_info *d,
                       struct dyld_objfile_entry *e)
{
  int ret = dyld_resolve_load_flag(d, e, dyld_minimal_load_rules);
  NSTRACE(dyld_minimal_load_flag);
  return ((ret >= 0) ? ret : OBJF_SYM_NONE);
}

/* */
int
dyld_default_load_flag(const struct dyld_path_info *d,
                       struct dyld_objfile_entry *e)
{
  int ret = dyld_resolve_load_flag(d, e, dyld_load_rules);
  NSTRACE(dyld_default_load_flag);
  if (ret >= 0)
    return ret;

  if (e->reason != dyld_reason_cfm)
    {
      if (dyld_load_dyld_shlib_symbols_flag)
        return OBJF_SYM_ALL;
    }
  else
    {
      if (dyld_load_cfm_shlib_symbols_flag)
        return OBJF_SYM_ALL;
    }

  return OBJF_SYM_NONE;
}

#define IS_ALIGNED_TO_4096_P(num) (((num) & ~(4096 - 1)) == (num))
#define ALIGN_TO_4096(num) (IS_ALIGNED_TO_4096_P((num)) ? \
                              (num) : \
                              ((num) + 4096) & ~(4096 - 1))

/* Iterate over ABFD's sections to establish the groups of memory that it
   will require to load.
   Does not modify MAP.
   Returns the memory footprint representation of this ABFD which must be
   xfreed by the caller.  */
static struct bfd_memory_footprint *
scan_bfd_for_memory_groups(struct bfd *abfd, struct pre_run_memory_map *map)
{
  asection *asect;
  int seg1addr_set = 0;
  CORE_ADDR current_bucket_start_addr = (CORE_ADDR)(-1L);
  CORE_ADDR current_bucket_end_addr = (CORE_ADDR)(-1L);
  struct bfd_memory_footprint *fp;
  NSTRACE(scan_bfd_for_memory_groups);
  fp = ((struct bfd_memory_footprint *)
        xmalloc(sizeof(struct bfd_memory_footprint)));
  fp->groups = ((struct bfd_memory_footprint_group *)
                xmalloc(sizeof(struct bfd_memory_footprint_group) * 2UL));
  fp->num = 0;
  fp->num_allocated = 2;
  fp->filename = abfd->filename;

  for (asect = abfd->sections; asect != NULL; asect = asect->next)
    {
      CORE_ADDR this_seg_start, this_seg_end, this_seg_len;

      /* I am only interested in segments like __TEXT and __DATA.  */
      if (asect->segment_mark == 0)
        continue;

      /* PAGEZERO does NOT interest me: */
      if (strcmp(asect->name, "LC_SEGMENT.__PAGEZERO") == 0) {
        continue;
      }

      this_seg_start = bfd_section_vma(abfd, asect);
      this_seg_len = bfd_section_size(abfd, asect);
      this_seg_end = (this_seg_start + this_seg_len);
      this_seg_end = ALIGN_TO_4096(this_seg_end);

      /* Do NOT record a segment (or a load command) which does NOT load
         into the address space at all.  */
      if (this_seg_start == 0)
        continue;

      if (seg1addr_set == 0)
       {
          seg1addr_set = 1;
          fp->seg1addr = this_seg_start;
          fp->groups[0].offset = 0;
          /* New bucket so update these two:  */
          current_bucket_start_addr = this_seg_start;
          current_bucket_end_addr =
            (current_bucket_start_addr +
             (((this_seg_len / map->bucket_size) + 1) * map->bucket_size));
          /* Did we overflow?  We do NOT round current_bucket_start_addr down
             to a bucket boundary so if it is right near the top of memory,
             current_bucket_start_addr + bucket_size may overflow.  */
          if (current_bucket_end_addr < current_bucket_start_addr)
            current_bucket_end_addr = (CORE_ADDR)(-1L);
          continue;
        }

      /* This segment fits within the current bucket: */
      if (this_seg_end < current_bucket_end_addr)
        {
          continue;
        }

      /* This segment grows the current bucket.
       * NB: If this segment's start addr is within a single bucket range of
       * the current group's end addr, we will have two memory groups right
       * after one another - which is pointless. So if it is within a single
       * bucket length of the current group's end address, combine it with this
       * one. */
      if ((this_seg_start <= (current_bucket_end_addr + map->bucket_size))
          && (this_seg_end > current_bucket_end_addr))
        {
          /* Expand current_bucket_end_addr by bucket_size increments.  */
          current_bucket_end_addr =
            (current_bucket_start_addr +
             ((((this_seg_end - current_bucket_start_addr)
                / map->bucket_size) + 1) * map->bucket_size));
          /* Did we overflow?  We do NOT round current_bucket_start_addr down
             to a bucket boundary so if it is right near the top of memory,
             current_bucket_start_addr + bucket_size may overflow.  */
          if (current_bucket_end_addr < current_bucket_start_addr)
            current_bucket_end_addr = (CORE_ADDR)(-1L);
          continue;
        }

      /* This is a new grouping.  */

      /* Record how many buckets the last address range grouping occupies.  */
      /* Note that current_bucket_{start,end}_addr are aligned to bucket_size
         boundaries so it is evenly divisible by bucket_size and will be at
         least bucket_size in size.  */
      fp->groups[fp->num].length =
        (int)((current_bucket_end_addr - current_bucket_start_addr)
              / map->bucket_size);

      fp->num++;

      if (fp->num == fp->num_allocated)
        {
          fp->num_allocated *= 2;
          fp->groups =
            ((struct bfd_memory_footprint_group *)
             xrealloc(fp->groups,
                      (sizeof(struct bfd_memory_footprint_group)
                       * fp->num_allocated)));
        }

      /* How many buckets between the last memory grouping and this one?  */
      fp->groups[fp->num].offset =
        (int)((((this_seg_start - current_bucket_end_addr)
                / map->bucket_size) + 1) + fp->groups[fp->num - 1].length);
      current_bucket_start_addr = this_seg_start;
      current_bucket_end_addr = (current_bucket_start_addr +
                                 ((this_seg_len / map->bucket_size) + 1)
                                 * map->bucket_size);
      /* Did we overflow?  We do NOT round current_bucket_start_addr down
         to a bucket boundary so if it is right near the top of memory,
         current_bucket_start_addr + bucket_size may overflow.  */
      if (current_bucket_end_addr < current_bucket_start_addr)
        current_bucket_end_addr = (CORE_ADDR)(-1L);
    }

  /* Update the number of buckets used by the last memory group we saw.  */
  /* Note that current_bucket_{start,end}_addr are aligned to bucket_size
     boundaries so it is evenly divisible by bucket_size and will be at
     least bucket_size in size.  */
  if (seg1addr_set != 0)
    {
      CORE_ADDR recalculated_endaddr;
      fp->groups[fp->num].length =
        (int)((current_bucket_end_addr - current_bucket_start_addr)
              / map->bucket_size);

      /* Does this grouping overflow?  We do NOT round current_bucket_start_addr
         down  to a bucket boundary so if it is right near the top of memory,
         current_bucket_start_addr + bucket_size may overflow.  If so, remember
         to add one to the bucket calculation.  */
      recalculated_endaddr = (current_bucket_start_addr
                              + (map->bucket_size * fp->groups[fp->num].length));
      if (recalculated_endaddr < current_bucket_end_addr)
        fp->groups[fp->num].length += 1;

      fp->num++;
    }
  else
    {
      /* We saw no segments with load addresses in this bfd.  */
      fp->groups[0].offset = 0;
      fp->groups[0].length = 0;
    }

  return fp;
}

#ifdef S_SPLINT_S
# ifndef ULLONG_MAX
#  define ULLONG_MAX 0xffffffffffffffffULL
# endif /* !ULLONG_MAX */
#endif /* S_SPLINT_S */

/* Allocate and initialize a new pre-run representation of the inferior's
   address space.  ABFD is the main executable; its memory regions will be
   marked as used.
   The allocated & initialized pre-run memory map is returned.  */

struct pre_run_memory_map *
create_pre_run_memory_map(struct bfd *abfd)
{
  struct bfd_memory_footprint *fp;
  int i;
  struct pre_run_memory_map *map;
  NSTRACE(create_pre_run_memory_map);
  map = ((struct pre_run_memory_map *)
         xmalloc(sizeof(struct pre_run_memory_map)));
  if (new_gdbarch_tdep(current_gdbarch)->wordsize == 4)
    {
      map->number_of_buckets = 400;
      map->bucket_size = (UINT_MAX / map->number_of_buckets);
      map->bucket_size &= ~(4096 - 1);
    }
  else
    {
      map->number_of_buckets = 4000;
      map->bucket_size = (CORE_ADDR)(ULLONG_MAX / map->number_of_buckets);
      map->bucket_size &= ~(4096 - 1);
    }

  map->buckets = (int *)xmalloc(sizeof(int) * map->number_of_buckets);
  memset(map->buckets, 0, (map->number_of_buckets * sizeof(int)));

  /* Figure out which bucket this executable will start loading at;
     mark its memory areas as used.  */
  fp = scan_bfd_for_memory_groups(abfd, map);
  i = (int)(fp->seg1addr / map->bucket_size);
  mark_buckets_as_used(map, i, fp);

  return map;
}

/* Search the address map for the first range of free buckets that will fit
   a given request.
   STARTING_BUCKET specifies where to start our search; this helps to reduce
   redundant searching.
   BUCKETS specifies the number of unused buckets in a row that we need to
   find for a match.
   Returns the bucket index that can contain the requested group or returns
   -1 if no matching spot could be found.  */
static int
find_next_hole(struct pre_run_memory_map *map, int starting_bucket,
               int buckets)
{
  int i;
  i = starting_bucket;

  NSTRACE(find_next_hole);

  while (i < map->number_of_buckets)
    {
      int j;
      if (i + buckets >= map->number_of_buckets)
        return -1;

      /* Skip over used buckets.  */
      if (map->buckets[i] == 1)
        {
          i++;
          continue;
        }

      /* Found one open bucket; see if we have enough sequential buckets for
         this memory group.  */
      j = i;
      while (j - i < buckets)
        {
          if (map->buckets[j] == 0)
            j++;
          else
            break;
        }

      /* Found an empty region big enough.  */
      if (j - i == buckets)
        {
          return i;
        }

      /* Skip past the end of this region we were just examining.  */
      i = j + 1;
    }

  return -1;
}

/* Determine if the address map at STARTING_BUCKET has BUCKETS unused
   buckets in a row.  Returns 1 if there are BUCKETS unused buckets
   in a row; 0 if not.  */
static int
hole_at_p(struct pre_run_memory_map *map, int starting_bucket,
          int buckets)
{
  int i = starting_bucket;

  NSTRACE(hole_at_p);

  if (starting_bucket >= map->number_of_buckets)
    return 0;

  while (((i - starting_bucket) < buckets)
         && (i < map->number_of_buckets)
         && map->buckets[i] == 0)
    {
      i++;
    }

  if ((i - starting_bucket) == buckets)
    return 1;
  else
    return 0;
}

/* Given a list of memory buckets required for this bfd in FP, and a
   starting bucket number in MAP, mark the appropriate buckets in
   MAP as used.  */

static void
mark_buckets_as_used(struct pre_run_memory_map *map, int startingbucket,
                     struct bfd_memory_footprint *fp)
{
  int memgrp, k;
  NSTRACE(mark_buckets_as_used);
  for (memgrp = 0; memgrp < fp->num; memgrp++)
    {
      struct bfd_memory_footprint_group *group = &fp->groups[memgrp];
      int initial_bkt = startingbucket + group->offset;

      if (initial_bkt + group->length > map->number_of_buckets)
        warning ("sharedlibrary preload-libraries exceeded map array while "
                 "processing '%s'", fp->filename);
      for (k = 0; k < group->length; k++)
        if (initial_bkt + k < map->number_of_buckets)
          map->buckets[initial_bkt + k] = 1;
    }
}

/* */
void
free_pre_run_memory_map(struct pre_run_memory_map *map)
{
  NSTRACE(free_pre_run_memory_map);
  if (map)
    xfree(map->buckets);
  xfree(map);
}

/* */
static void
free_memory_footprint(struct bfd_memory_footprint *fp)
{
  NSTRACE(free_memory_footprint);
  if (fp)
    xfree(fp->groups);
  xfree(fp);
}

/* Given an ABFD and a representation of the inferior's pre-run address space
   MAP, find an empty space big enough for all of ABFD's memory groupings,
   mark those regions as used in MAP, and return slide that should be applied
   to ABFD's load address to keep it from overlapping with other dylibs.  */

static CORE_ADDR
slide_bfd_in_pre_run_memory_map(struct bfd *abfd,
                                struct pre_run_memory_map *map)
{
  int i = 0;
  int j, k;
  int intended_loadaddr_bucket;
  struct bfd_memory_footprint *fp;
  volatile CORE_ADDR intended_loadaddr;
  int can_load_at_preferred_addr;

  NSTRACE(slide_bfd_in_pre_run_memory_map);

  fp = scan_bfd_for_memory_groups(abfd, map);
  intended_loadaddr = fp->seg1addr;

  /* See if we can load the dylib at its preferred address: */
  intended_loadaddr_bucket = (int)(fp->seg1addr / map->bucket_size);
  can_load_at_preferred_addr = 1;
  for (k = 0; k < fp->num; k++)
    if (!hole_at_p(map, intended_loadaddr_bucket, fp->groups[k].length))
      {
        can_load_at_preferred_addr = 0;
        break;
      }

  if (can_load_at_preferred_addr)
    {
      mark_buckets_as_used(map, intended_loadaddr_bucket, fp);
      free_memory_footprint(fp);
      return 0;
    }

  /* Nope - now try to fit it somewhere in memory starting at the
     beginning.  */

  while (i < map->number_of_buckets)
    {
      j = find_next_hole(map, i, fp->groups[0].length);

      /* Could NOT find a hole; report no slide value at all.  */
      if (j == -1)
        {
          free_memory_footprint(fp);
          return 0;
        }

      /* If there are more than one memory groups, check to ensure that
         they can all fit.  */
      for (k = 1; k < fp->num; k++)
        {
          if (!hole_at_p(map, (j + fp->groups[k].offset), fp->groups[k].length))
            break;
        }

      /* Did all the groupings fit?  */
      if (k == fp->num)
        {
          CORE_ADDR loadaddr;
          mark_buckets_as_used(map, j, fp);
          free_memory_footprint(fp);
          loadaddr = (map->bucket_size * j);
          return (loadaddr - intended_loadaddr);
        }
      i += fp->groups[0].length;
    }


  /* No open slot found, so return the originally intended loadaddr of the
   * dylib: */
  free_memory_footprint(fp);
  return 0;
}

/* */
void
dyld_load_library_from_file(const struct dyld_path_info *d,
			    struct dyld_objfile_entry *e,
			    int print_errors)
{
  const char *name = NULL;
  struct ui_file *prev_stderr;

  NSTRACE(dyld_load_library_from_file);

  name = dyld_entry_filename(e, d, DYLD_ENTRY_FILENAME_LOADED);
  if (name == NULL)
    {
      if (print_errors)
	{
	  char *s = dyld_entry_string(e, 1);
	  warning(_("No image filename available for %s."), s);
	  xfree(s);
	}
      return;
    }

  /* We could just go straight to symfile_bfd_open_safe, but since
     GDB's error-handler resets bfd_errno, it is difficult to tell why
     the call has failed.  So instead, check explicitly if the file
     exists, and avoid printing a warning if a weak file is not
     found.  */
  if (!file_exists_p(name))
    {
      int issue_warning = 0;

      if (print_errors) {
        issue_warning = 1;
      }

      /* Do NOT warn about missing libraries if they are only linked
       * against weakly: */
      if (e->reason & dyld_reason_weak_mask) {
        issue_warning = 0;
      }

      /* An MH_BUNDLE with file mod time 0 (cannot check that here) with the
       * name cl_kernels is something we should NOT warn about not finding on
       * disk; it is just noise to the developer and there will be many of them
       * with certain programs, e.g. Mail.app on Lion and later. */
      if ((e->mem_header.filetype == MH_BUNDLE)
          && (strcmp(name, "cl_kernels") == 0))
        {
          issue_warning = 0;
        }

      /* Running natively on the phone we read everything from the
         shared cached and need not worry about when we are NOT able to read
         files from disk.  */
#if defined(TARGET_ARM) && defined(NM_NEXTSTEP)
       issue_warning = 0;
#endif /* TARGET_ARM && NM_NEXTSTEP */

      /* OpenCL jitted dylibs will not have a backing file; do NOT
         warn about them.  */
      if (strstr(name, "com.apple.opencl/clprogram") != NULL)
        issue_warning = 0;

      if (issue_warning)
        warning(_("Unable to read symbols for %s (file not found)."), name);

      return;
    }

  prev_stderr = gdb_stderr;

  gdb_stderr = gdb_null;
  CHECK_FATAL(e->abfd == NULL);

  e->abfd = symfile_bfd_open_safe(name, 0, GDB_OSABI_UNKNOWN);

  gdb_stderr = prev_stderr;

  if (e->abfd == NULL)
    {
      xfree((void *)name);
      return;
    }

  e->loaded_name = bfd_get_filename(e->abfd);
  e->loaded_from_memory = 0;
  e->loaded_error = 0;
}

/* */
void
dyld_load_library_from_memory(const struct dyld_path_info *d,
			      struct dyld_objfile_entry *e,
			      int print_errors)
{
  const char *name = NULL;
  CORE_ADDR slide;
  CORE_ADDR length;

  NSTRACE(dyld_load_library_from_memory);

  if (!e->dyld_valid && !core_bfd)
    {
      if (print_errors)
	{
#if !(defined(TARGET_ARM) && defined(NM_NEXTSTEP))
	  char *s = dyld_entry_string(e, dyld_print_basenames_flag);
	  warning("Unable to read symbols from %s (not yet mapped into memory).", s);
	  xfree(s);
#endif /* !(TARGET_ARM && NM_NEXTSTEP */
	}
      return;
    }

  name = dyld_entry_filename(e, d, DYLD_ENTRY_FILENAME_LOADED);

  CHECK_FATAL(e->abfd == NULL);
  slide = e->dyld_slide;
  length = e->dyld_length;
  if (core_bfd)
    {
      /* If we are reading an image from core memory, pass in an invalid
         slide amount so we the slide can be automatically determined.  */
       slide = INVALID_ADDRESS;
    }
  if (e->dyld_length == 0)
    {
      /* If the length is zero, do not try and limit what can be read from
       * memory. Entries in the shared cache can have mach headers that
       * contain segment load commands whose data is scattered over quite a
       * large area of memory and we do NOT alway know the bounds.  */
      length = INVALID_ADDRESS;
    }
  e->abfd = inferior_bfd(name, e->dyld_addr, slide, length);
  if (e->abfd == NULL)
    {
      warning("Could not read dyld entry: %s from inferior memory at 0x%s "
	      "with slide 0x%s and length %lu.", name, paddr_nz(e->dyld_addr),
	      paddr_nz(slide), (unsigned long)length);
      return;
    }

  e->loaded_memaddr = e->dyld_addr;
  e->loaded_from_memory = 1;
  e->loaded_error = 0;
}

/* dyld_load_library opens the bfd for the library in E, and
   sets some state in E.  */
void
dyld_load_library(const struct dyld_path_info *d,
                  struct dyld_objfile_entry *e)
{
  int read_from_memory = 0;
  int print_errors = 1;

  NSTRACE(dyld_load_library);

  CHECK_FATAL(e->allocated);

  if ((e->abfd != NULL) || (e->objfile != NULL))
    return;

  if (e->reason & dyld_reason_executable_mask)
    CHECK_FATAL(e->objfile == symfile_objfile);

 try_again_please:
  /* For now, we only print any error messages the first time we try
     to load a bfd.  It would be nice to use a more subtle mechanism
     here, that would avoid repeating the error messages when we retry
     a load, but would print them if we progressed further and hit
     some new error. */

  print_errors = !e->loaded_error;

  /* This would be a good candidate for load-rules similar to those
     for shared library load-levels.  For now, though, just hard-code
     some basic logic. */

  if (e->reason == dyld_reason_cfm)
    read_from_memory = 1;

  if (dyld_always_read_from_memory_flag)
    read_from_memory = 1;

  /* Always read from memory when we are using a core file: */
  if (core_bfd)
    read_from_memory = 1;

  if (read_from_memory)
    dyld_load_library_from_memory(d, e, print_errors);
  else if (e->abfd == NULL)
    {
      dyld_load_library_from_file(d, e, print_errors);
      if (e->abfd == NULL)
	{
	  read_from_memory = 1;
	  /* If the target is "remote" we want to lower the load level
	     on libraries that we are going to have to read out of memory.  */
	  if (target_is_remote())
	    {
              const char *n;
	      e->load_flag = OBJF_SYM_CONTAINER;
              n = (const char *)NULL;
              if (e->dyld_name)
                n = e->dyld_name;
              else if (e->image_name)
                n = e->image_name;
              else if (e->user_name)
                n = e->user_name;
              /* Print the bundle name */
              /* (or if not a bundle, print the basename). */
              if (n)
                {
                  const char *m;
                  m = bundle_basename(n);
                  if (m && (m != n))
                    {
                      n = m;
                    }
                  else
                    {
                      m = lbasename(n);
                      if (m)
                        n = m;
                    }
                }
              else
                {
                  n = "<No file name>";
                }
              /* For OpenCL jitted dylibs on a remote system do NOT report the
                 fact that we could NOT find them on the local system. Should we
                 ignore them altogether? This will still silently pull them down
                 over the wire which is slow... but ignoring them altogether would
                 be a drag if we crash in one or if one of the OpenCL folks are
                 trying to debug a problem with one.  */
              if (strncmp(n, "com.apple.opencl/clprogram", strlen("com.apple.opencl/clprogram")) != 0)
                warning("No copy of %s found locally, reading from memory on remote device.  This may slow down the debug session.", n);
	    }
	  goto try_again_please;
	}
      else if (bfd_mach_o_stub_library(e->abfd) || bfd_mach_o_encrypted_binary(e->abfd))
	{
	  /* If we find a stub library as the backing file,
	     then switch to reading from memory.  */
	  e->load_flag = (OBJF_SYM_CONTAINER | OBJF_SYM_DONT_CHANGE);
	  bfd_close(e->abfd);
	  e->abfd = NULL;
	  e->loaded_name = NULL;
	  read_from_memory = 1;
	  goto try_again_please;
	}
      else if (dyld_check_uuids_flag && e->dyld_valid)
	{
	  /* To speed things up in the common case where the UUID's
	     match, we find the offset of the UUID load command in the
	     on disk binary, and go directly to that offset in the
	     in memory copy.  If that is a load command, then we compare
	     those.  If we do NOT find one there, then we have to search
	     through the load commands in memory.
	     Note: Do NOT check if e->dyld_valid is not true, since then we ONLY
	     have a file we have read from load commands for this bfd, and do NOT
	     have an in memory copy yet.  */

	  struct mach_o_data_struct *mdata = NULL;
	  unsigned long i;
	  volatile bfd_vma uuid_addr = (bfd_vma)-1L;
	  unsigned char mem_uuid[16];
	  volatile int matches = 0;
	  unsigned char file_uuid[16];

	  CHECK_FATAL(bfd_mach_o_valid(e->abfd));
	  mdata = e->abfd->tdata.mach_o_data;
	  for (i = 0UL; i < mdata->header.ncmds; i++)
	    {
	      /* Find the UUID command in the on disk copy of the
		 binary.  */
	      if (mdata->commands[i].type == BFD_MACH_O_LC_UUID)
		{
		  uuid_addr = mdata->commands[i].offset;
		  bfd_mach_o_get_uuid(e->abfd, file_uuid, sizeof(file_uuid));
		  break;
		}
	    }

	  /* If there is no UUID cmd, we obviously cannot do any checking: */
	  if (uuid_addr != (bfd_vma)-1L)
	    {
	      struct gdb_exception exc;
	      volatile int error = 0;
	      volatile int found_uuid = 0;

	      uuid_addr += e->dyld_addr;
	      TRY_CATCH(exc, RETURN_MASK_ALL)
		{
		  error = target_read_uuid(uuid_addr, mem_uuid);
		}

	      /* Do the check with the UUID we found in the spot where it
               * would be if the binaries were the same.  If that works,
               * then we are done.  Otherwise, look through the in-memory
               * version directly for the LC_UUID.  */
	      if ((exc.reason == (int)NO_ERROR) && (error == 0))
		matches = (memcmp(mem_uuid, file_uuid, sizeof(file_uuid)) == 0);

	      if (!matches)
		{
		  struct mach_header header;
		  volatile bfd_vma curpos;
		  struct load_command cmd;
		  target_read_mach_header(e->dyld_addr, &header);
		  curpos = (e->dyld_addr
                            + target_get_mach_header_size(&header));

		  for (i = 0; i < header.ncmds; i++)
		    {
		      if (target_read_load_command(curpos, &cmd) != 0)
			break;
		      if (cmd.cmd == BFD_MACH_O_LC_UUID)
			{
			  if (target_read_uuid(curpos, mem_uuid) == 0)
			    found_uuid = 1;
			  break;
			}
		      curpos += cmd.cmdsize;
		    }
		  matches = (memcmp(mem_uuid, file_uuid, sizeof(file_uuid)) == 0);
		}

	      if (!matches)
		{
                  if (!info_verbose)
		    warning(_("UUID mismatch detected with the loaded library "
			      "- on disk is:\n\t%s"),
                            e->abfd->filename);
                  else
		    warning(_("UUID mismatch detected with the loaded library "
			      "- on host side:\n\t%s (UUID %s)\n"
			      "- on device side:\n\tUUID %s"),
                            e->abfd->filename, puuid(file_uuid),
                            puuid(mem_uuid));

		  if (ui_out_is_mi_like_p(uiout))
		    {
		      struct cleanup *notify_cleanup =
			make_cleanup_ui_out_notify_begin_end(uiout,
							     "uuid-mismatch-with-loaded-file");
		      ui_out_field_string(uiout, "file", e->abfd->filename);
		      do_cleanups(notify_cleanup);
		    }
		  bfd_close(e->abfd);
		  e->abfd = NULL;
		  e->loaded_name = NULL;
		  read_from_memory = 1;
		  goto try_again_please;
		}

              if (found_uuid == 1)
                {
                  dyld_debug("Found uuid.\n");
                }
	    }
	}
    }

  /* If we were NOT able to load the bfd, there must have been an error
     somewhere. Flag it, so we do NOT print error messages the next
     time around (see comment above). */

  if (e->abfd == NULL)
    {
      e->loaded_error = 1;
      return;
    }

  if (e->reason & dyld_reason_init && macosx_dyld_status.pre_run_memory_map)
    {
      CORE_ADDR addr =
	slide_bfd_in_pre_run_memory_map(e->abfd,
					macosx_dyld_status.pre_run_memory_map);
      e->pre_run_slide_addr_valid = 1;
      e->pre_run_slide_addr = addr;
    }

  if (e->reason & dyld_reason_image_mask)
    {
      asection *text_sect = bfd_get_section_by_name(e->abfd,
						    TEXT_SEGMENT_NAME);
      if (text_sect != NULL)
        {
          e->image_addr = bfd_section_vma(e->abfd, text_sect);
          e->image_addr_valid = 1;
        }
    }

  if (e->reason & dyld_reason_executable_mask)
    symfile_objfile = e->objfile;
}

/* */
void
dyld_load_libraries(const struct dyld_path_info *d,
                    struct dyld_objfile_info *result)
{
  int i;
  struct dyld_objfile_entry *e = NULL;
  NSTRACE(dyld_load_libraries);
  CHECK_FATAL(result != NULL);

  DYLD_ALL_OBJFILE_INFO_ENTRIES(result, e, i)
    {
      if (e->load_flag < 0)
        {
          e->load_flag =
            (dyld_default_load_flag(d, e) | dyld_minimal_load_flag(d, e));
        }
      if (e->load_flag)
        {
          dyld_load_library(d, e);
        }
    }
}

/* */
void
dyld_symfile_loaded_hook(struct objfile *o)
{
  NSTRACE(dyld_symfile_loaded_hook);
  /* I have to do this here as well as in macosx_dyld_update or
     this will NOT get re-initialized if you originally saw
     /usr/lib/libobjc.A.dylib, THEN set DYLD_LIBRARY_PATH to point to an
     independent copy, and THEN reran...  */
  if (o == find_libobjc_objfile())
    {
      objc_init_trampoline_observer();
      objc_init_runtime_version();
    }
}

/* dyld_slide_objfile applies the slide in NEW_OFFSETS, or in
   DYLD_SLIDE if NEW_OFFSETS is NULL, to OBJFILE, and to the
   separate_debug_objfile, if it exists.  */
void
dyld_slide_objfile(struct objfile *objfile, CORE_ADDR dyld_slide,
		   struct section_offsets *new_offsets)
{
  int i;
  if (objfile)
    {
      struct cleanup *offset_cleanup;
      if (new_offsets == NULL)
	{
	  new_offsets =
	    ((struct section_offsets *)
	     xmalloc(SIZEOF_N_SECTION_OFFSETS(objfile->num_sections)));
	  for (i = 0; i < objfile->num_sections; i++)
	    {
	      new_offsets->offsets[i] = dyld_slide;
	    }
	  offset_cleanup = make_cleanup(xfree, new_offsets);
	}
      else
	offset_cleanup = make_cleanup(null_cleanup, NULL);

      /* Note, I am not calling tell_breakpoints_objfile_changed here, but
	 instead relying on objfile_relocate to relocate the breakpoints
	 in this objfile.  */
      objfile_relocate(objfile, new_offsets);

      tell_objc_msgsend_cacher_objfile_changed(objfile);
      if (info_verbose)
        printf_filtered("Relocating symbols from %s...", objfile->name);
      gdb_flush(gdb_stdout);
      if (objfile->separate_debug_objfile != NULL)
	{
	  struct section_offsets *dsym_offsets;
	  int num_dsym_offsets;

	  /* The offsets we apply are supposed to be the TOTAL offset,
	     so we have to add the dsym offset to the one passed in
	     for the objfile.  */
	  macho_calculate_offsets_for_dsym(objfile,
					   objfile->separate_debug_objfile->obfd,
					   NULL,
					   new_offsets,
					   objfile->num_sections,
					   &dsym_offsets,
					   &num_dsym_offsets);
	  make_cleanup(xfree, dsym_offsets);
	  objfile_relocate(objfile->separate_debug_objfile, dsym_offsets);
	}


      do_cleanups(offset_cleanup);
      if (info_verbose)
        printf_filtered("done\n");
    }
}

/* */
static void
dyld_load_symfile_internal(struct dyld_objfile_entry *e,
                           int preserving_objfile_p)
{
  struct section_addr_info *volatile addrs;
  size_t zi0;
  volatile int using_orig_objfile = 0;

  NSTRACE(dyld_load_symfile_internal);

  if (e->loaded_error)
    return;

  CHECK_FATAL(e->allocated);

  if (e->reason & dyld_reason_executable_mask)
    CHECK_FATAL(e->objfile == symfile_objfile);

  if (e->dyld_valid)
    {
      e->loaded_addr = e->dyld_addr;
      e->loaded_addrisoffset = 0;
    }
  else if (e->image_addr_valid)
    {
      e->loaded_addr = e->image_addr;
      e->loaded_addrisoffset = 0;
    }
  else
    {
      e->loaded_addr = e->dyld_slide;
      e->loaded_addrisoffset = 1;
    }

  if (e->loaded_from_memory == 1)
    {
      e->loaded_memaddr = e->loaded_addr;
    }

  /* This is a little weird, because dyld_load_symfile is used both
     to load a symfile, and to slide it.  I want a third use, which
     is reload it but reuse the objfile structures, so we can
     reload both the objfile and the commpage objfile "in place".
     This is the only way I can think of to get ALL_OBJFILES_SAFE
     to work when we end up deleting an objfile AND its commpage
     objfile simultaneously.  */

  if ((e->objfile != NULL) && !preserving_objfile_p)
    {
      slide_objfile(e->objfile, e->dyld_slide, e->dyld_section_offsets);
    }
  else
    {
      volatile struct gdb_exception exc;
      const char *segname;
      asection *commsec;

      CHECK_FATAL(e->abfd != NULL);

      /* Is this in the shared cache?  If so, and we do NOT already
         have section offsets, we need to call a special routine to
         compute the offsets -- dylibs in the shared cache have
         different slide values for different sections.  */

      if (dyld_objfile_entry_in_shared_cache(e)
	  && (e->dyld_section_offsets == NULL)
          && e->dyld_valid)
        e->dyld_section_offsets =
                  get_sectoffs_for_shared_cache_dylib(e, e->loaded_addr);

      /* If we just have a slide then that means that the whole objfile is
       * sliding by the same amount.  So we make up a section_addr_info
       * struct, and fill each element with the slide value.  Otherwise, we
       * use the dyld_section_offsets.  */

      if (e->dyld_section_offsets == NULL)
	{
	  addrs = alloc_section_addr_info(bfd_count_sections(e->abfd));

	  for (zi0 = 0UL; zi0 < addrs->num_sections; zi0++)
	    {
	      addrs->other[zi0].name = NULL;
              if ((e->dyld_valid == 0)
                  && (e->pre_run_slide_addr_valid == 1))
	        addrs->other[zi0].addr = e->pre_run_slide_addr;
              else
	        addrs->other[zi0].addr = e->dyld_slide;
	      addrs->other[zi0].sectindex = 0;
	    }

	  addrs->addrs_are_offsets = 1;
	}
      else
	addrs = NULL;

      if (e->objfile == NULL)
	{
	  e->objfile = symbol_file_add_bfd_safe(e->abfd, 0, addrs,
                                                e->dyld_section_offsets,
                                                0, 0, e->load_flag, 0,
                                                e->prefix, NULL);
	}
      else
	{
          size_t zi1;
          volatile int num_offsets;
	  struct bfd_section *this_sect;

	  using_orig_objfile = 1;

          /* bfd sections are not the same as objfile sections: */
	  num_offsets = 0;
	  if (e->dyld_section_offsets != NULL)
	    {
	      this_sect = e->abfd->sections;
	      for (zi1 = 0UL;
                   zi1 < bfd_count_sections(e->abfd);
                   zi1++, this_sect = this_sect->next)
		{
		  if (objfile_keeps_section(e->abfd, this_sect))
		    num_offsets++;
		}
	    }

	  TRY_CATCH(exc, RETURN_MASK_ALL)
	  {
	    e->objfile =
	      symbol_file_add_with_addrs_or_offsets_using_objfile(e->objfile,
								  e->abfd, 0,
								  addrs,
							e->dyld_section_offsets,
								  num_offsets,
								  0, 0,
								  e->load_flag,
								  0, e->prefix,
								  NULL);
	  }
	  if (exc.reason == RETURN_ERROR)
	    e->objfile = NULL;
	}
      e->abfd = NULL;

      if (e->objfile == NULL)
        {
          e->loaded_error = 1;
          return;
        }

      e->loaded_name = e->objfile->name;
#if defined(CHECK_FATAL) && (defined(DEBUG) || defined(_DEBUG))
      CHECK_FATAL(e->objfile->obfd == e->abfd);
#endif /* CHECK_FATAL && (DEBUG || _DEBUG) */


      /* If we are loading the library for the first time, check to see
         if it has a __DATA.__commpage section, and if so, process the
         contents of that section as if it were a separate objfile.
         This objfile will not get relocated along with the parent
         library, which is appropriate since the commpage never moves in
         memory. */

      segname = "LC_SEGMENT.__DATA.__commpage";

      commsec = bfd_get_section_by_name(e->objfile->obfd, segname);
      if (commsec != NULL)
        {
          char *bufstr;
          bfd_size_type len;
          char *bfdname;
	  size_t buf_len;
	  size_t bfdnamelen;

          len = bfd_section_size(e->objfile->obfd, commsec);
	  buf_len = ((size_t)len * sizeof(char));
          bufstr = (char *)xmalloc(buf_len);
	  gdb_assert(e->objfile->obfd != NULL);
	  bfdnamelen = (strlen(e->objfile->obfd->filename) + 128UL);
          bfdname = (char *)xmalloc(bfdnamelen);

          snprintf(bfdname, bfdnamelen, "%s[%s]", e->objfile->obfd->filename,
		   segname);

#if (defined(DEBUG) || defined(_DEBUG) || defined(GDB_DEBUG))
	  printf_filtered(_("bfdname: %s.\n"), bfdname);
#endif /* DEBUG || _DEBUG || GDB_DEBUG */

          if (bfd_get_section_contents(e->objfile->obfd, commsec, bufstr,
                                       0, len) != TRUE)
            warning(_("unable to read commpage data"));

          e->commpage_bfd = bfd_memopenr(bfdname, NULL, (bfd_byte *)bufstr,
					 len);

          if (!bfd_check_format(e->commpage_bfd, bfd_object))
            {
              bfd_close(e->commpage_bfd);
              e->commpage_bfd = NULL;
            }

          if (e->commpage_bfd != NULL)
	    {
	      if (!using_orig_objfile)
		{
                  /* Pass a NULL value instead of ADDRS -- we do NOT want to
                     adjust/slide any of the comm page symbols. Their
                     addresses are absolute and are already correct.  */
		  e->commpage_objfile =
		    symbol_file_add_bfd_safe(e->commpage_bfd, 0, NULL, 0, 0, 0,
					     e->load_flag, 0, e->prefix, NULL);
		}
	      else
		{
		  TRY_CATCH (exc, RETURN_MASK_ALL)
		    {
                      /* Pass a NULL value instead of ADDRS - we do NOT want to
                         adjust/slide any of the comm page symbols. Their
                         addresses are absolute and are already correct.  */
		      e->commpage_objfile =
			symbol_file_add_bfd_using_objfile(e->commpage_objfile,
							  e->commpage_bfd, 0,
							  NULL, 0, 0, 0,
							  e->load_flag, 0,
							  e->prefix);
		    }
		  if (exc.reason == RETURN_ERROR)
		    e->commpage_objfile = NULL;
		}
	    }
          e->commpage_bfd = NULL;
        }
    }

  dyld_symfile_loaded_hook(e->objfile);

  if (e->reason & dyld_reason_executable_mask)
    {
      CHECK_FATAL((symfile_objfile == NULL)
                  || (symfile_objfile == e->objfile));
      symfile_objfile = e->objfile;
      return;
    }
}

/* */
void
dyld_load_symfile(struct dyld_objfile_entry *e)
{
  NSTRACE(dyld_load_symfile);
  dyld_load_symfile_internal(e, 0);
}

/* */
void
dyld_load_symfile_preserving_objfile(struct dyld_objfile_entry *e)
{
  NSTRACE(dyld_load_symfile_preserving_objfile);
  dyld_load_symfile_internal(e, 1);
}

/* */
void
dyld_load_symfiles(struct dyld_objfile_info *result)
{
  int i;
  int first = 1;
  struct dyld_objfile_entry *e = (struct dyld_objfile_entry *)NULL;

  NSTRACE(dyld_load_symfiles);
  CHECK_FATAL(result != NULL);

  DYLD_ALL_OBJFILE_INFO_ENTRIES(result, e, i)
    {
      char load_char;

      if (e->loaded_error)
        continue;
      if (e->abfd == NULL && e->objfile == NULL)
        continue;

      if (e->objfile != NULL)
        {
          if (e->dyld_valid && e->loaded_addr == e->dyld_addr
              && !e->loaded_addrisoffset)
            continue;
          if (e->dyld_valid && e->loaded_addr == e->dyld_slide
              && e->loaded_addrisoffset)
            continue;
          if (e->image_addr_valid && e->loaded_addr == e->image_addr
              && !e->loaded_addrisoffset)
            continue;
          if (!e->dyld_valid && !e->image_addr_valid)
            continue;
        }

      load_char = ((e->objfile != NULL) ? '+' : '.');
      if (first && !info_verbose && dyld_print_status())
        {
          first = 0;
          printf_filtered(_("Reading symbols for shared libraries \n"));
          gdb_flush(gdb_stdout);
        }
      dyld_load_symfile(e);
      if (!info_verbose && dyld_print_status())
        {
          printf_filtered("%c", load_char);
          gdb_flush(gdb_stdout);
        }
    }

  if (!first && !info_verbose && dyld_print_status())
    {
      printf_filtered(" done\n");
      gdb_flush(gdb_stdout);
    }
}

/* Look up the objfile for a given shared library entry.  If no
   objfile is currently allocated, or if the objfile has been removed
   by a lower level of GDB, return NULL. */
struct objfile *
dyld_lookup_objfile_safe(struct dyld_objfile_entry *e)
{
  struct objfile *o;
  struct objfile *temp = NULL;

  NSTRACE(dyld_lookup_objfile_safe);

  ALL_OBJFILES_SAFE(o, temp)
    if (e->objfile == o)
      return o;

  return NULL;
}

/* */
int
dyld_objfile_allocated(struct objfile *o)
{
  struct objfile *objfile;
  struct objfile *temp = NULL;

  NSTRACE(dyld_objfile_allocated);

  ALL_OBJFILES_SAFE(objfile, temp)
  {
    if (o == objfile)
      {
        return 1;
      }
  }
  return 0;
}

/* */
void
dyld_purge_objfiles(struct dyld_objfile_info *info)
{
  struct dyld_objfile_entry *e = NULL;
  int i;

  NSTRACE(dyld_purge_objfiles);

  DYLD_ALL_OBJFILE_INFO_ENTRIES(info, e, i)
    if (e->objfile != NULL)
      if (dyld_lookup_objfile_safe(e) == NULL)
        dyld_remove_objfile(e);
}

/* Return the dyld_objfile_entry for a given objfile.  If no
   dyld_objfile_entry claims the specified objfile, return NULL. */
struct dyld_objfile_entry *
dyld_lookup_objfile_entry(struct dyld_objfile_info *info, struct objfile *o)
{
  int i;
  struct dyld_objfile_entry *e = NULL;

  NSTRACE(dyld_lookup_objfile_entry);

  DYLD_ALL_OBJFILE_INFO_ENTRIES(info, e, i)
    if (e->objfile == o)
      return e;

  return NULL;
}

/* dyld_should_reload_objfile_for_flags: Checks whether the
   load level of the objfile in the dyld_objfile_entry E needs
   to be reloaded.  We always say a upgrade should be done.  But
   if it is a cached symfile, or dyld_reload_on_downgrade_flag
   is true, then reject downgrades.  */
enum dyld_reload_result
dyld_should_reload_objfile_for_flags(struct dyld_objfile_entry *e)
{
  NSTRACE(dyld_should_reload_objfile_for_flags);

  /* For regular symbol files, reload if there is any difference
     in the requested symbols at all if dyld_reload_on_downgrade_flag
     is set.  Otherwise, only reload on upgrade. */
  if (e->load_flag == e->objfile->symflags)
    return DYLD_NO_CHANGE;
  else if (e->objfile->symflags & OBJF_SYM_DONT_CHANGE)
    {
      e->load_flag = e->objfile->symflags;
      return DYLD_NO_CHANGE;
    }
  else if (e->load_flag & ~e->objfile->symflags)
    return DYLD_UPGRADE;
  else
    {
      if (dyld_reload_on_downgrade_flag) {
	return DYLD_DOWNGRADE;
      } else {
	return DYLD_NO_CHANGE;
      }
    }
}

/* Called when an objfile is to be freed.
   If a corresponding dyld_objfile_entry exists, we must free that
   as well so the dyld-level structures and the high-level objfile
   structures stay in sync.

   It would be nice if we got a notification from dyld about a dylib/bundle
   being unloaded and handled that correctly. But as of today, we are not,
   so Fix and Continue is reduced to doing it by hand.  */
void
remove_objfile_from_dyld_records(struct objfile *obj)
{
  struct dyld_objfile_info *info = &macosx_dyld_status.current_info;
  struct dyld_objfile_entry *e = NULL;
  int i;

  NSTRACE(remove_objfile_from_dyld_records);

  DYLD_ALL_OBJFILE_INFO_ENTRIES(info, e, i)
    if (e->objfile == obj)
      dyld_remove_objfile(e);
}

/* Returns 1 if the objfile OBJ is actually in memory right now.
   A better name might be "dyld_is_objfile_resident".

   If we the OBJ objfile record exists because of a program's load command,
   or it was loaded when the program was running but the inferior has exited
   (has been target_mourn_inferior'ed), we return 0.  */
extern int inferior_auto_start_dyld_flag;
ATTRIBUTE_W_U_R int
dyld_is_objfile_loaded(struct objfile *obj)
{
  struct dyld_objfile_entry *e = NULL;
  int i;
  struct macosx_dyld_thread_status *status = &macosx_dyld_status;

  NSTRACE(dyld_is_objfile_loaded);

  if (inferior_auto_start_dyld_flag == 0)
    return 1;

  if (obj == NULL)
    return 0;

  /* If we have a debug-only object file that is to be used for another
     objfile, return the fact that it is backlinked file is loaded instead
     of itself.  */
  if (obj->separate_debug_objfile_backlink != NULL)
    return dyld_is_objfile_loaded(obj->separate_debug_objfile_backlink);

  /* A SYMS_ONLY_OBJFILE is an objfile added by the user with
     add-symbol-file; it shadows an actually loaded & resident objfile,
     but breakpoints will be associated with the syms-only-objfile.
     So under the theory that users do NOT add-symbol-file things which
     have NOT been loaded yet, set breakpoints in this unconditionally.  */
  if (obj->syms_only_objfile == 1)
    return 1;

  /* Another problem is that since we do NOT consider anything mapped
     till we get the first dyld notification, we will NOT set any
     breakpoints in dyld and so and we will NOT hit any breakpoints in
     the early parts of the dyld code.
     To work around this, we ALWAYS consider dyld loaded. */

  if (bfd_hash_lookup(&obj->obfd->section_htab, "LC_ID_DYLINKER", 0, 0) != NULL)
    return 1;

  DYLD_ALL_OBJFILE_INFO_ENTRIES(&status->current_info, e, i)
    if ((e->objfile == obj) || (e->commpage_objfile == obj))
      if ((e->dyld_name != NULL)
          || ((e->dyld_addr != 0) && (e->dyld_addr != INVALID_ADDRESS)))
        return 1;

  return 0;
}

/* */
void
dyld_remove_objfile_internal(struct dyld_objfile_entry *e,
                             int delete_p)
{
  char *s = NULL;

  NSTRACE(dyld_remove_objfile_internal);

  CHECK_FATAL(e->allocated);

  if (e->reason & dyld_reason_executable_mask)
    CHECK_FATAL(e->objfile == symfile_objfile);

  if (e->objfile == NULL)
    {
      return;
    }

  CHECK_FATAL(dyld_objfile_allocated(e->objfile));

  s = dyld_entry_string(e, dyld_print_basenames_flag);
  if (info_verbose)
    {
      printf_filtered("Removing symbols for %s\n", s);
    }
  xfree(s);
  gdb_flush(gdb_stdout);

  if (delete_p)
    {
      free_objfile(e->objfile);
      e->objfile = NULL;
    }
  else
    clear_objfile(e->objfile);

  if (e->abfd != NULL)
    bfd_close(e->abfd);
  e->abfd = NULL;

  if (e->dyld_section_offsets != NULL)
    {
      xfree(e->dyld_section_offsets);
      e->dyld_section_offsets = NULL;
    }

  if (e->commpage_objfile != NULL)
    {
      /* The commpage objfile is read when symbols for the main library
         are ready for the first time; it needs to be freed along with
         the main objfile. */
      if (delete_p)
	{
	  free_objfile(e->commpage_objfile);
	  e->commpage_objfile = NULL;
	}
      else
	clear_objfile(e->commpage_objfile);

      if (e->commpage_bfd != NULL)
        bfd_close(e->commpage_bfd);
      e->commpage_bfd = NULL;
    }

  e->loaded_name = NULL;
  e->loaded_memaddr = 0;
  e->pre_run_slide_addr_valid = 0;
  e->pre_run_slide_addr = 0;
  gdb_flush(gdb_stdout);

  if (e->reason & dyld_reason_executable_mask)
    {
      symfile_objfile = e->objfile;
    }
}

/* dyld_remove_objfile deletes the objfile contained
   in the objfile entry E.  */
void
dyld_remove_objfile(struct dyld_objfile_entry *e)
{
  NSTRACE(dyld_remove_objfile);
  dyld_remove_objfile_internal(e, 1);
}

/* dyld_clear_objfile clears the current contents of
   the objfile contained in objfile entry E.  You
   can thereby edit it "in place".  */
void
dyld_clear_objfile(struct dyld_objfile_entry *e)
{
  NSTRACE(dyld_clear_objfile);
  dyld_remove_objfile_internal(e, 0);
}

/* */
void
dyld_remove_objfiles(const struct dyld_path_info *d,
		     struct dyld_objfile_info *result)
{
  int i;
  int first = 1;
  struct dyld_objfile_entry *e = NULL;
  NSTRACE(dyld_remove_objfiles);
  CHECK_FATAL(result != NULL);

  DYLD_ALL_OBJFILE_INFO_ENTRIES(result, e, i)
    {
      int should_reload = 0;

      if (e->load_flag < 0)
        {
          e->load_flag =
            (dyld_default_load_flag(d, e) | dyld_minimal_load_flag(d, e));
        }

      if (e->reason & dyld_reason_executable_mask)
        CHECK_FATAL(e->objfile == symfile_objfile);

      if (e->objfile != NULL)
        {
          if ((e->user_name != NULL)
              && (strcmp(e->user_name, e->objfile->name) != 0))
            should_reload = 1;

          if (dyld_should_reload_objfile_for_flags(e))
            should_reload = 1;
        }

      if (should_reload)
        {
          dyld_remove_objfile(e);
          if (first && !info_verbose && dyld_print_status())
            {
              first = 0;
              printf_filtered("Removing symbols for unused shared libraries ");
              gdb_flush(gdb_stdout);
            }
          if (!info_verbose && dyld_print_status())
            {
              printf_filtered(".");
              gdb_flush(gdb_stdout);
            }
        }
    }
  if (!first && !info_verbose && dyld_print_status())
    {
      printf_filtered(" done\n");
      gdb_flush(gdb_stdout);
    }
}

/* */
static ATTRIBUTE_W_U_R int
dyld_libraries_similar(struct dyld_path_info *d,
		       struct dyld_objfile_entry *f,
		       struct dyld_objfile_entry *l)
{
  const char *fname = NULL;
  const char *lname = NULL;

  const char *fbase = NULL;
  const char *lbase = NULL;
  size_t flen = 0UL;
  size_t llen = 0UL;

  NSTRACE(dyld_libraries_similar);

  CHECK_FATAL(f != NULL);
  CHECK_FATAL(l != NULL);

  if ((library_offset(f) != 0) && (library_offset(l) != 0))
    {
      return (library_offset(f) == library_offset(l));
    }

  fname = dyld_entry_filename(f, d, DYLD_ENTRY_FILENAME_LOADED);
  lname = dyld_entry_filename(l, d, DYLD_ENTRY_FILENAME_LOADED);

  if ((lname != NULL) && (fname != NULL))
    {
      int f_is_framework, f_is_bundle;
      int l_is_framework, l_is_bundle;

      dyld_library_basename(fname, &fbase, &flen, &f_is_framework,
                            &f_is_bundle);
      dyld_library_basename(lname, &lbase, &llen, &l_is_framework,
                            &l_is_bundle);

      if ((flen != llen) || (strncmp(fbase, lbase, llen) != 0))
        {
          xfree((char *)fbase);
          xfree((char *)lbase);
          return 0;
        }

      xfree((char *)fbase);
      xfree((char *)lbase);

      if (f_is_framework != l_is_framework)
        return 0;

      if (f_is_bundle != l_is_bundle)
        return 0;

      return 1;
    }

  return 0;
}

 /* Do dyld_objfile_entry OLDENT and NEWENT have the same filename?  In
    other words, are they the same dylib/bundle/executable/etc ?  */
ATTRIBUTE_W_U_R int
dyld_libraries_compatible(struct dyld_path_info *d,
                          struct dyld_objfile_entry *newent,
                          struct dyld_objfile_entry *oldent)
{
  const char *newname = NULL;
  const char *oldname = NULL;

  NSTRACE(dyld_libraries_compatible);

  CHECK_FATAL(oldent != NULL);
  CHECK_FATAL(newent != NULL);

  /* If either prefix is non-NULL, then they must both be the same string: */
  if ((oldent->prefix != NULL) || (newent->prefix != NULL))
    {
      if ((oldent->prefix == NULL) || (newent->prefix == NULL)) {
	return 0;
      }
      if (strcmp(oldent->prefix, newent->prefix) != 0) {
	return 0;
      }
    }

  newname = dyld_entry_filename(newent, d, DYLD_ENTRY_FILENAME_LOADED);
  oldname = dyld_entry_filename(oldent, d, DYLD_ENTRY_FILENAME_LOADED);

  /* If we have already loaded the objfile from memory, and from the
   * same address, then we can go ahead and re-use it.
   *
   * FIXME: What if dyld has moved libraries around, or plug-ins have
   * been unloaded and re-loaded for whatever reason, and we now have
   * some other library loaded at this address?  We should probably
   * store some token in the loaded_* information to provide for more
   * reliable matching.
   */

  if (oldent->loaded_from_memory && (oldent->loaded_addr == newent->dyld_addr))
    return dyld_libraries_similar(d, newent, oldent);

  /* If either filename is non-NULL, then they must both be the same string. */
  /* Be careful, if we are loaded from memory, then the original entry just has
   * the filename from the load commands, but the made entry says "memory object...".
   */
  if ((oldname != NULL) || (newname != NULL))
    {
      if ((oldname == NULL) || (newname == NULL))
	return 0;
      if (oldent->loaded_from_memory && newent->loaded_from_memory)
	{
	  const char *mem_obj_str = "[memory object \"";
	  size_t offset = strlen(mem_obj_str);
	  if (((strstr(newname, mem_obj_str) == newname)
	       && (strstr(newname, oldname) == (newname + offset)))
	      || ((strstr(oldname, mem_obj_str) == oldname)
		  && (strstr(oldname, newname) == (oldname + offset))))
	    return dyld_libraries_similar(d, newent, oldent);
	}
      else
	{
	  if (strcmp(oldname, newname) != 0)
	    return 0;
	}
    }

  if (oldent->loaded_from_memory != newent->loaded_from_memory)
    {
      return 0;
    }

   /* The same bundle can be loaded more than once under certain
      circumstances. Both dyld_objfile_entries will be dyld_reason_dyld,
      both will have the same filename, but they will have different dyld_addr's
      (different load addresses).
      It is not entirely clear to me whether this is technically legal, but
      it happens in real world use.  cf <rdar://problem/4308315>

      When this comes up, we should say that the two libraries are not
      "compatible". This is not an error condition. Yes, that means
      there will be two dyld_objfile_entry's with the same filename and
      two struct objfile's with the same filename, but that is how we roll. */
  return dyld_libraries_similar(d, newent, oldent);
}

/* Move the load data (whatever distinction that is -- not all the
   fields are moved) from the SRC dyld_objfile_entry into DEST.
   Upon completion, SRC will NOT have any of its load data fields set.  */
void
dyld_objfile_move_load_data(struct dyld_objfile_entry *src,
                            struct dyld_objfile_entry *dst)
{
  bfd *dst_bfd = NULL;
  bfd *src_bfd = NULL;
  int reload = 0;
  enum gdb_osabi reload_osabi = GDB_OSABI_UNKNOWN;
  NSTRACE(dyld_objfile_move_load_data);
  gdb_assert(dst->commpage_bfd == NULL);

  /* dyld_info_process_raw will open the bfd for the objfile and it
     so we try to reuse it.  But when the library was read at startup,
     then the old dyld_objfile_entry already has a bfd.  We will use the
     original one in SRC unless the one in DST is valid the 32/64 bit-ness
     differs, or the cputype/subtypes do NOT match.  */
  if (dst->abfd)
    dst_bfd = dst->abfd;
  else if (dst->objfile && dst->objfile->obfd)
    dst_bfd = dst->objfile->obfd;

  if (src->abfd)
    src_bfd = src->abfd;
  else if (src->objfile && src->objfile->obfd)
    src_bfd = src->objfile->obfd;

  if (dst_bfd && src_bfd)
    {
      struct mach_o_data_struct *dst_mdata = dst_bfd->tdata.mach_o_data;
      struct mach_o_data_struct *src_mdata = src_bfd->tdata.mach_o_data;
      if (dst_mdata && src_mdata && (dst_mdata != src_mdata))
	{
	  if ((dst_mdata->header.magic != src_mdata->header.magic) ||
	      (dst_mdata->header.cputype != src_mdata->header.cputype) ||
	      (dst_mdata->header.cpusubtype != src_mdata->header.cpusubtype))
	    {
	      reload = 1;
	      reload_osabi = macosx_get_osabi_from_dyld_entry(dst_bfd);
	    }
	}
    }

  if ((dst->abfd != NULL) && (src->abfd != dst->abfd))
      bfd_close(dst->abfd);

  gdb_assert((src->objfile == dst->objfile) || (dst->objfile == NULL));
  gdb_assert(dst->commpage_objfile == NULL);

  dst->abfd = src->abfd;
  dst->objfile = src->objfile;

  /* Check to see if we have a slice of an objfile, but not the right slice.
   * In this case, reload the symbols with the different OSABI
   * will differ.  */
  if (reload)
    reread_symbols_for_objfile(dst->objfile, 0, reload_osabi, NULL);

  if ((src->dyld_valid == 0) && (dst->dyld_valid == 1)
      && (src->pre_run_slide_addr_valid == 1)
      && (src->pre_run_slide_addr != 0))
    {
      /* FIXME: Why do we have to relocate differently if we are
       * using the pre_run_slide_addr? This should just be another
       * step in the relocating process.  */
      slide_objfile(dst->objfile, dst->dyld_slide, 0);
    }

  dst->commpage_bfd = src->commpage_bfd;
  dst->commpage_objfile = src->commpage_objfile;

  /* If we are re-running, and have NOT resolved the new load data
     flags, go ahead and pick them up from the previous run. */
  if ((src->load_flag > 0) && (dst->load_flag < 0))
    {
      dst->load_flag = src->load_flag;
    }

  dst->prefix = src->prefix;
  dst->loaded_name = src->loaded_name;
  dst->loaded_memaddr = src->loaded_memaddr;
  dst->loaded_addr = src->loaded_addr;
  dst->loaded_offset = src->loaded_offset;
  dst->loaded_addrisoffset = src->loaded_addrisoffset;
  dst->loaded_from_memory = src->loaded_from_memory;
  dst->loaded_error = src->loaded_error;
  dst->pre_run_slide_addr_valid = src->pre_run_slide_addr_valid;
  dst->pre_run_slide_addr = src->pre_run_slide_addr;

  src->objfile = NULL;
  src->abfd = NULL;
  src->dyld_section_offsets = NULL;

  src->commpage_bfd = NULL;
  src->commpage_objfile = NULL;

  src->load_flag = -1;

  src->loaded_name = NULL;
  src->loaded_memaddr = 0;
  src->loaded_addr = 0;
  src->loaded_offset = 0;
  src->loaded_addrisoffset = 0;
  src->loaded_from_memory = 0;
  src->loaded_error = 0;
  src->pre_run_slide_addr = 0;
  src->pre_run_slide_addr_valid = 0;

  dyld_objfile_entry_clear(src);
}

/* */
void
dyld_check_discarded(struct dyld_objfile_info *info)
{
  int j;
  NSTRACE(dyld_check_discarded);
  for (j = 0; j < info->nents; j++)
    {
      struct dyld_objfile_entry *e = &info->entries[j];
      if ((e->abfd == NULL) && (e->objfile == NULL) && !e->loaded_error)
        {
          dyld_objfile_entry_clear(e);
        }
    }
}

/* We are adding NEWENT to the list of dylib/bundle/etcs loaded in the
   inferior in a little while.  Look through the existing entries
   in OLDINFOS and see if we had one for this dylib/bundle/etc
   already.  If so, copy over the load data into NEWENT and clear
   them from the old entry in OLDINFOS.  (I think the old entry is
   marked as obsolete over in the purge_shlib function or something.)  */
void
dyld_merge_shlib(const struct macosx_dyld_thread_status *s,
                 struct dyld_path_info *d,
                 struct dyld_objfile_info *oldinfos,
                 struct dyld_objfile_entry *newent)
{
  int i;
  struct dyld_objfile_entry *oldent = NULL;

  NSTRACE(dyld_merge_shlib);

  DYLD_ALL_OBJFILE_INFO_ENTRIES(oldinfos, oldent, i)
    if (dyld_libraries_compatible(d, newent, oldent))
      {
        dyld_objfile_move_load_data(oldent, newent);
        if (newent->reason & dyld_reason_executable_mask)
          symfile_objfile = newent->objfile;
        return;
      }

  if (newent->reason & dyld_reason_image_mask)
    DYLD_ALL_OBJFILE_INFO_ENTRIES(oldinfos, oldent, i)
      if ((oldent->objfile != NULL)
	  && dyld_libraries_similar(d, newent, oldent))
        {
          dyld_objfile_move_load_data(oldent, newent);
          if (newent->reason & dyld_reason_executable_mask)
            symfile_objfile = newent->objfile;
          return;
        }
}

/* Go through all the dyld_objfile_entry's in OBJINFO, looking for
 * other entries that are for the same file image as NEW.
 *
 * When gdb starts with an app before launching, it creates
 * dyld_objfile_entry's for all the dylibs it finds in the app's
 * load commands. Then when the app executes, we get notifications
 * from dyld when the dylibs/bundles/etc actually load. So at that
 * point we want to toss the pre-execution speculative dyld_objfile_entry
 * and standardize on the actually-seen image file.
 *
 * That is one instance where we will be using this function: */
void
dyld_prune_shlib(struct dyld_path_info *d,
		 struct dyld_objfile_info *obj_info,
                 struct dyld_objfile_entry *newobj)
{
  struct dyld_objfile_entry *o = NULL;
  int i;

  NSTRACE(dyld_prune_shlib);

  DYLD_ALL_OBJFILE_INFO_ENTRIES(obj_info, o, i)
    {
      if ((o->reason & dyld_reason_executable_mask)
          && (newobj->reason & dyld_reason_executable_mask))
        {
          if ((o->objfile != NULL) && (o->objfile != newobj->objfile))
            {
              tell_breakpoints_objfile_changed(o->objfile);
              tell_objc_msgsend_cacher_objfile_changed(o->objfile);
            }
          dyld_objfile_entry_clear(o);
          continue;
        }

      if (dyld_libraries_similar(d, o, newobj))
        {
          if (o->objfile != NULL)
            {
              tell_breakpoints_objfile_changed(o->objfile);
              tell_objc_msgsend_cacher_objfile_changed(o->objfile);
            }
          dyld_remove_objfile(o);
          dyld_objfile_entry_clear(o);
        }
    }
}

/* */
void
dyld_merge_shlibs(const struct macosx_dyld_thread_status *s,
                  struct dyld_path_info *d,
                  struct dyld_objfile_info *oldobj,
                  struct dyld_objfile_info *newobj)
{
  struct dyld_objfile_entry *n = NULL;
  struct dyld_objfile_entry *o = NULL;
  int i;

  NSTRACE(dyld_merge_shlibs);

  CHECK_FATAL(oldobj != NULL);
  CHECK_FATAL(newobj != NULL);
  CHECK_FATAL(oldobj != newobj);

  dyld_resolve_filenames(s, newobj);

  DYLD_ALL_OBJFILE_INFO_ENTRIES(newobj, n, i)
    if (n->objfile == NULL)
      dyld_merge_shlib(s, d, oldobj, n);

  DYLD_ALL_OBJFILE_INFO_ENTRIES(newobj, n, i)
    dyld_prune_shlib(d, oldobj, n);

  DYLD_ALL_OBJFILE_INFO_ENTRIES(oldobj, o, i)
    {
      struct dyld_objfile_entry *e = NULL;
      e = dyld_objfile_entry_alloc(newobj);
      *e = *o;

      e->reason =
	((enum dyld_objfile_reason)
	 (e->reason | (enum dyld_objfile_reason)dyld_reason_cached_mask));

      dyld_objfile_entry_clear(o);
    }
}

/* */
static void
dyld_shlibs_updated(struct dyld_objfile_info *info)
{
  NSTRACE(dyld_shlibs_updated);
  dyld_objfile_info_pack(info);
  update_section_tables_dyld(info);
  update_current_target();
  reread_symbols();
  breakpoint_update();
  re_enable_breakpoints_in_shlibs(0);
}

/* */
void
dyld_update_shlibs(struct dyld_path_info *d, struct dyld_objfile_info *result)
{
  struct cleanup *timer_cleanup = NULL;
  static int dyld_timer = -1;
  NSTRACE(dyld_update_shlibs);
  CHECK_FATAL(result != NULL);

  if (maint_use_timers)
    timer_cleanup = start_timer(&dyld_timer, "dyld_update_shlibs", "");

  dyld_debug(_("dyld_update_shlibs: updating shared library information\n"));

  dyld_remove_objfiles(d, result);
  dyld_load_libraries(d, result);
  dyld_load_symfiles(result);

  dyld_shlibs_updated(result);
  if (maint_use_timers) {
    do_cleanups(timer_cleanup);
  }
}

/* FIXME: add comment: */
void
dyld_purge_cached_libraries(struct dyld_objfile_info *info)
{
  int i;
  struct dyld_objfile_entry *e = NULL;
  NSTRACE(dyld_purge_cached_libraries);
  CHECK_FATAL(info != NULL);

  DYLD_ALL_OBJFILE_INFO_ENTRIES(info, e, i)
    if (e->reason & dyld_reason_cached_mask)
      {
        dyld_remove_objfile(e);
        dyld_objfile_entry_clear(e);
      }

  dyld_shlibs_updated(info);
}

/* Usual gdb initialization hook: */
void
_initialize_macosx_nat_dyld_process(void)
{
  NSTRACE(_initialize_macosx_nat_dyld_process);
  add_setshow_boolean_cmd("check-uuids", class_obscure,
                          &dyld_check_uuids_flag, _("\
Set if GDB should check the binary UUID between the file on disk and the one loaded in memory."), _("\
Set if GDB should check the binary UUID between the file on disk and the one loaded in memory."), NULL,
                          NULL, NULL, &setshliblist, &showshliblist);
}

/* EOF */
