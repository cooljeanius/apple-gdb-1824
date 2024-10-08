/* macosx/machoread.c
   Mac OS X support for mach-o executables for GDB, the GNU debugger.
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

#if !defined(NO_POISON) && defined(POISON_FREE_TOO)
/* included files can drag in "free": */
# define NO_POISON 1
#endif /* !NO_POISON && POISON_FREE_TOO */

#include "defs.h"
#include "symtab.h"
#include "gdbtypes.h"
#include "breakpoint.h"
#include "bfd.h"
#include "symfile.h"
#include "objfiles.h"
#include "buildsym.h"
#include "obstack.h"
#include "gdb-stabs.h"
#include "stabsread.h"
#include "gdbcmd.h"
#include "completer.h"
#include "dwarf2read.h"
#include "macosx-tdep.h"
/* "gdb_assert.h" is included later */

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
#elif defined(TARGET_M68K)
# include "m68k-tdep.h"
#elif defined(TARGET_SPARC)
# include "sparc-tdep.h"
#else
# ifndef S_SPLINT_S
#  error "Unrecognized target architecture."
# endif /* !S_SPLINT_S */
#endif /* TARGET_foo */
#include "gdbarch.h"

#include "mach-o.h"
#include "gdb_assert.h"
#include "macosx-nat-dyld-io.h"
#include "macosx-nat-inferior.h"

#include <string.h>
#include <libintl.h>

#ifndef DEBUG
# define DEBUG 2
#endif /* !DEBUG */
#ifndef _DEBUG
# define _DEBUG 1
#endif /* !_DEBUG */

#if HAVE_MMAP
static int mmap_strtabflag = 1;
#endif /* HAVE_MMAP */

static int use_eh_frames_info = 0;

static int mach_o_process_exports_flag = 1;

struct macho_symfile_info
{
  asymbol **syms;
  long nsyms;
};

static int macho_read_indirect_symbols(bfd *abfd,
                                       struct bfd_mach_o_dysymtab_command
                                       *dysymtab,
                                       struct bfd_mach_o_symtab_command
                                       *symtab, struct objfile *objfile)
  ATTRIBUTE_W_U_R;

extern void macho_build_psymtabs(struct objfile *objfile, int mainline,
                                 const char *stab_name,
				 const char *stabstr_name,
                                 const char *text_name,
				 const char *local_stab_name,
                                 const char *nonlocal_stab_name,
                                 const char *coalesced_text_name,
                                 const char *data_name, const char *bss_name);

extern void _initialize_machoread(void);

/* */
static void
macho_new_init(struct objfile *objfile)
{
  if (objfile == NULL) {
    warning(_("NULL objfile passed to macho_new_init()."));
  }
  return;
}

/* */
static void
macho_symfile_init(struct objfile *objfile)
{
  objfile->deprecated_sym_stab_info =
    (struct dbx_symfile_info *)xmmalloc(objfile->md,
                                        sizeof(struct dbx_symfile_info));

  memset((PTR)objfile->deprecated_sym_stab_info, 0, sizeof(struct dbx_symfile_info));

  objfile->deprecated_sym_private =
    xmmalloc(objfile->md, sizeof(struct macho_symfile_info));

  memset(objfile->deprecated_sym_private, 0, sizeof(struct macho_symfile_info));

  objfile->flags |= OBJF_REORDERED;
}

/* Scan and build partial symbols for a file with special sections for stabs
   and stabstrings. The file has already been processed to get its minimal
   symbols, and any other symbols that might be necessary to resolve GSYMs.

   This routine is the equivalent of dbx_symfile_init and dbx_symfile_read
   rolled into one.

   OBJFILE is the object file we are reading symbols from.
   ADDR is the address relative to which the symbols are (e.g. the base address
   of the text segment).
   MAINLINE is true if we are reading the main symbol table (as opposed to a
   shared lib or dynamically loaded file).
   STAB_NAME is the name of the section that contains the stabs.
   STABSTR_NAME is the name of the section that contains the stab strings.

   This routine is mostly copied from dbx_symfile_init and dbx_symfile_read. */

void dbx_symfile_read(struct objfile *objfile, int mainline);

void
macho_build_psymtabs(struct objfile *objfile, int mainline,
                     const char *stab_name, const char *stabstr_name,
                     const char *text_name, const char *local_stab_name,
		     const char *nonlocal_stab_name,
                     const char *coalesced_text_name,
                     const char *data_name, const char *bss_name)
{
  int val;
  bfd *sym_bfd = objfile->obfd;
  char *name = bfd_get_filename(sym_bfd);
  asection *stabsect;
  struct obj_section *os;
  asection *stabstrsect;
  asection *local_stabsect, *nonlocal_stabsect;
  int bfd_in_memory;
  struct cleanup *mycleanup;

  init_minimal_symbol_collection();
  mycleanup = make_cleanup_discard_minimal_symbols();

  gdb_assert(mycleanup != NULL);

  stabsect = bfd_get_section_by_name(sym_bfd, stab_name);
  stabstrsect = bfd_get_section_by_name(sym_bfd, stabstr_name);

  bfd_in_memory = macosx_bfd_is_in_memory(sym_bfd);
  if (!stabsect)
    return;

  if (!stabstrsect)
    error("macho_build_psymtabs:  Found stabs (%s), but not string section (%s)",
          stab_name, stabstr_name);

  objfile->deprecated_sym_stab_info =
    ((struct dbx_symfile_info *)
     xmmalloc(objfile->md, sizeof(struct dbx_symfile_info)));
  memset(objfile->deprecated_sym_stab_info, 0, sizeof(struct dbx_symfile_info));

  gdb_assert(text_name != NULL);
  gdb_assert(data_name != NULL);

  /* For text, data, coalesced text and bss we want to get the
     struct obj_section's instead of BFD asections. The asection will have
     the intended load address, but if the file slid at load-time those
     addresses will not be reliable. For other sections, e.g. DBX_STRINGTAB,
     it is fine to refer to the file's asection.  */

  DBX_TEXT_SECTION(objfile) = NULL;
  ALL_OBJFILE_OSECTIONS(objfile, os)
    if (os->the_bfd_section && os->the_bfd_section->name
        && (strcmp(os->the_bfd_section->name, text_name) == 0))
      {
        DBX_TEXT_SECTION(objfile) = os;
        break;
      }

  DBX_DATA_SECTION(objfile) = NULL;
  ALL_OBJFILE_OSECTIONS(objfile, os)
    if (os->the_bfd_section && os->the_bfd_section->name
        && (strcmp(os->the_bfd_section->name, data_name) == 0))
      {
        DBX_DATA_SECTION(objfile) = os;
        break;
      }

  /* If there is no __DATA, __data section we still need to come up
     with something for any symbols in the __DATA segment (e.g. something
     in __DATA, __common) so let us point to the segment itself.  */
  if (DBX_DATA_SECTION(objfile) == NULL)
    ALL_OBJFILE_OSECTIONS(objfile, os)
      if (os->the_bfd_section && os->the_bfd_section->name
          && (strcmp(os->the_bfd_section->name, "LC_SEGMENT.__DATA") == 0))
        {
          DBX_DATA_SECTION(objfile) = os;
          break;
        }

  DBX_COALESCED_TEXT_SECTION(objfile) = NULL;
  if (coalesced_text_name != NULL)
    ALL_OBJFILE_OSECTIONS(objfile, os)
      if (os->the_bfd_section && os->the_bfd_section->name
          && (strcmp(os->the_bfd_section->name, coalesced_text_name) == 0))
        {
          DBX_COALESCED_TEXT_SECTION(objfile) = os;
          break;
        }

  DBX_BSS_SECTION(objfile) = NULL;
  if (bss_name != NULL)
    ALL_OBJFILE_OSECTIONS(objfile, os)
      if (os->the_bfd_section && os->the_bfd_section->name
          && (strcmp(os->the_bfd_section->name, bss_name) == 0))
        {
          DBX_BSS_SECTION(objfile) = os;
          break;
        }

  /* Zero length sections will have a BFD asection but not a struct
     obj_section.  */
  if (!DBX_TEXT_SECTION(objfile))
    {
      asection *text_sect = bfd_get_section_by_name(sym_bfd, text_name);
      if (text_sect)
        {
          DBX_TEXT_SECTION(objfile) = (struct obj_section *)
                            obstack_alloc(&objfile->objfile_obstack,
					  sizeof(struct obj_section));
          DBX_TEXT_SECTION(objfile)->addr =
                                 bfd_section_vma(sym_bfd, text_sect);
          DBX_TEXT_SECTION(objfile)->endaddr =
                                 (bfd_section_vma(sym_bfd, text_sect)
				  + bfd_section_size(sym_bfd, text_sect));
          DBX_TEXT_SECTION(objfile)->objfile = objfile;
          DBX_TEXT_SECTION(objfile)->the_bfd_section = text_sect;
        }
      else
	{
	  /* Finally, if we cannot find a text section at all, then
	     make up an invalid one. Code in dbxread.c looks up the
	     DBX_TEXT_SECTION()->the_bfd_section without checking that
	     DBX_TEXT_SECTION exists. It is easier to just fake up one
	     here.  */
          DBX_TEXT_SECTION(objfile) = (struct obj_section *)
                            obstack_alloc(&objfile->objfile_obstack,
                                    sizeof(struct obj_section));
          DBX_TEXT_SECTION(objfile)->addr = INVALID_ADDRESS;
          DBX_TEXT_SECTION(objfile)->endaddr = INVALID_ADDRESS;
          DBX_TEXT_SECTION(objfile)->objfile = objfile;
          DBX_TEXT_SECTION(objfile)->the_bfd_section = NULL;
	}
    }

  /* Zero length sections will have a BFD asection but not a struct
     obj_section.  */
  if (!DBX_DATA_SECTION(objfile))
    {
      asection *data_sect = bfd_get_section_by_name(sym_bfd, data_name);
      if (data_sect)
        {
          DBX_DATA_SECTION(objfile) =
	    ((struct obj_section *)
	     obstack_alloc(&objfile->objfile_obstack,
			   sizeof(struct obj_section)));
	  gdb_assert(DBX_DATA_SECTION(objfile) != NULL);
          DBX_DATA_SECTION(objfile)->addr =
                                 bfd_section_vma(sym_bfd, data_sect);
          DBX_DATA_SECTION(objfile)->endaddr =
                                 (bfd_section_vma(sym_bfd, data_sect)
				  + bfd_section_size(sym_bfd, data_sect));
          DBX_DATA_SECTION(objfile)->objfile = objfile;
          DBX_DATA_SECTION(objfile)->the_bfd_section = data_sect;
        }
      else
	{
	  /* Finally, if we cannot find a data section at all, then
	     make up an invalid one. Code in dbxread.c looks up the
	     DBX_DATA_SECTION()->the_bfd_section without checking that
	     DBX_DATA_SECTION exists. It is easier to just fake up one
	     here.  */
          DBX_DATA_SECTION(objfile) =
	    ((struct obj_section *)
	     obstack_alloc(&objfile->objfile_obstack,
			   sizeof(struct obj_section)));
	  gdb_assert(DBX_DATA_SECTION(objfile) != NULL);
          DBX_DATA_SECTION(objfile)->addr = INVALID_ADDRESS;
          DBX_DATA_SECTION(objfile)->endaddr = INVALID_ADDRESS;
          DBX_DATA_SECTION(objfile)->objfile = objfile;
          DBX_DATA_SECTION(objfile)->the_bfd_section = NULL;
	}
    }

  /* Zero length sections will have a BFD asection but not a struct
     obj_section.  */
  if (!DBX_COALESCED_TEXT_SECTION(objfile))
    {
      asection *textcoal_sect = bfd_get_section_by_name(sym_bfd,
                                                        coalesced_text_name);
      if (textcoal_sect)
        {
          DBX_COALESCED_TEXT_SECTION(objfile) =
	    ((struct obj_section *)
	     obstack_alloc(&objfile->objfile_obstack,
			   sizeof(struct obj_section)));
	  gdb_assert(DBX_COALESCED_TEXT_SECTION(objfile) != NULL);
          DBX_COALESCED_TEXT_SECTION(objfile)->addr =
                                  bfd_section_vma(sym_bfd, textcoal_sect);
          DBX_COALESCED_TEXT_SECTION(objfile)->endaddr =
                                  (bfd_section_vma(sym_bfd, textcoal_sect)
				   + bfd_section_size(sym_bfd, textcoal_sect));
          DBX_COALESCED_TEXT_SECTION(objfile)->objfile = objfile;
          DBX_COALESCED_TEXT_SECTION(objfile)->the_bfd_section = textcoal_sect;
        }
      else
	{
	  /* Finally, if we cannot find a coalesced text section at
	     all, then make up an invalid one. Code in dbxread.c
	     looks up the
	     DBX_COALESCED_TEXT_SECTION()->the_bfd_section without
	     checking that DBX_COALESCED_TEXT_SECTION exists. It is
	     easier to just fake up one here.  */
          DBX_COALESCED_TEXT_SECTION(objfile) =
	    ((struct obj_section *)
	     obstack_alloc(&objfile->objfile_obstack,
			   sizeof(struct obj_section)));
	  gdb_assert(DBX_COALESCED_TEXT_SECTION(objfile) != NULL);
          DBX_COALESCED_TEXT_SECTION(objfile)->addr = INVALID_ADDRESS;
          DBX_COALESCED_TEXT_SECTION(objfile)->endaddr = INVALID_ADDRESS;
          DBX_COALESCED_TEXT_SECTION(objfile)->objfile = objfile;
          DBX_COALESCED_TEXT_SECTION(objfile)->the_bfd_section = NULL;
	}
    }

  if (DBX_TEXT_SECTION(objfile))
    {
      DBX_TEXT_ADDR(objfile) = DBX_TEXT_SECTION(objfile)->addr;
      DBX_TEXT_SIZE(objfile) = (int)(DBX_TEXT_SECTION(objfile)->endaddr
                                     - DBX_TEXT_SECTION(objfile)->addr);
    }
  else
    {
      DBX_TEXT_ADDR(objfile) = 0;
      DBX_TEXT_SIZE(objfile) = 0;
    }

  /* APPLE LOCAL: Pre-fetch the addresses for the coalesced section as well.
     Note: It is not an error not to have a coalesced section...  */

  if (DBX_COALESCED_TEXT_SECTION(objfile))
    {
      DBX_COALESCED_TEXT_ADDR(objfile) =
                                DBX_COALESCED_TEXT_SECTION(objfile)->addr;
      DBX_COALESCED_TEXT_SIZE(objfile) =
                      (int)(DBX_COALESCED_TEXT_SECTION(objfile)->endaddr
                            - DBX_COALESCED_TEXT_SECTION(objfile)->addr);
    }
  else
    {
      DBX_COALESCED_TEXT_ADDR(objfile) = 0;
      DBX_COALESCED_TEXT_SIZE(objfile) = 0;
    }
  /* END APPLE LOCAL */

  DBX_SYMBOL_SIZE(objfile) =
    ((bfd_mach_o_version(objfile->obfd) > 1) ? 16 : 12);
  DBX_SYMCOUNT(objfile) =
    ((int)bfd_section_size(sym_bfd, stabsect) / DBX_SYMBOL_SIZE(objfile));
  DBX_STRINGTAB_SIZE(objfile) = (int)bfd_section_size(sym_bfd,
                                                      stabstrsect);

  /* XXX - FIXME: POKING INSIDE BFD DATA STRUCTURES */
  DBX_SYMTAB_OFFSET(objfile) = stabsect->filepos;

#if HAVE_MMAP
  if (mmap_strtabflag && (bfd_in_memory == 0))
    {
      /* currently breaks mapped symbol files (string table does NOT end up in objfile) */

      bfd_window w;
      bfd_init_window(&w);

      /* APPLE LOCAL: Open the string table read only if possible. Should
         be more efficient.  */

      val =
        bfd_get_section_contents_in_window_with_mode(sym_bfd, stabstrsect, &w, 0,
                                                     DBX_STRINGTAB_SIZE(objfile), 0);

      if (!val)
        perror_with_name(name);

      DBX_STRINGTAB(objfile) = (char *)w.data;
    }
  else
    {
#endif /* HAVE_MMAP */
#if defined(TARGET_ARM) && defined(NM_NEXTSTEP)
      get_dyld_shared_cache_local_syms();
      /* Hack for ARM native MacOSX targets where we can rely on anything
       * in the shared cache being mapped in our process at the same
       * address. This can save us 10MB - 11MB which is a about a tenth
       * of our available memory.   */
      if (bfd_mach_o_in_shared_cached_memory(sym_bfd))
	{
          static char *g_shared_cache_stringtab = NULL;

	  /* Map a single copy of the string table and cache the result: */
	  if (g_shared_cache_stringtab == NULL)
	    {
              g_shared_cache_stringtab = xmalloc(DBX_STRINGTAB_SIZE(objfile) + 1);
	      OBJSTAT(objfile, sz_strtab += (DBX_STRINGTAB_SIZE(objfile) + 1));

	      /* Now read in the string table in one big gulp: */
	      val = bfd_get_section_contents(sym_bfd, stabstrsect,
					     g_shared_cache_stringtab, 0,
	         			     DBX_STRINGTAB_SIZE(objfile));
	      if (!val)
	        perror_with_name(name);
	    }
	  DBX_STRINGTAB(objfile) = g_shared_cache_stringtab;

          /* Pre-seed the minsyms for this objfile with the nlist records in
             the separate dyld_shared_cache file on iOS devices -- the in-memory
             copy will NOT have the symbols.  */
          struct gdb_copy_dyld_cache_local_symbols_entry *dsc_locsyms_entry = NULL;
          CORE_ADDR slide = 0UL;
          if (objfile->sections &&
              objfile->sections[objfile->sect_index_text].the_bfd_section)
            {
              dsc_locsyms_entry =
                get_dyld_shared_cache_entry(objfile->sections[objfile->sect_index_text].the_bfd_section->lma);
              slide = (objfile->sections[objfile->sect_index_text].addr
                       - objfile->sections[objfile->sect_index_text].the_bfd_section->lma);
            }
          if (dsc_locsyms_entry && dsc_locsyms_entry->nlistCount > 0)
            {
              int nlist_size;
              uint8_t *base;
              if (new_gdbarch_tdep(current_gdbarch)->wordsize == 4)
                nlist_size = 12;
              else
                nlist_size = 16;
              base = (dyld_shared_cache_local_nlists
                      + (dsc_locsyms_entry->nlistStartIndex * nlist_size));
              add_dyld_shared_cache_local_symbols(objfile, base, dsc_locsyms_entry->nlistCount,
                                                  nlist_size, dyld_shared_cache_strings,
                                                  slide, mainline);
            }
	}
      else
	{
#endif /* TARGET_ARM && NM_NEXTSTEP */
      /* Only check the length if our bfd is not in memory since the bfd
       * read iovec functions we define in macosx-nat-dyld-info.c do not
       * always have a length as our in memory executable images can now
       * be scattered about memory with any segment data appearing at a
       * lower address than our mach header.  */
      if (bfd_in_memory == 0)
	{
	  if (DBX_STRINGTAB_SIZE(objfile) > bfd_get_size(sym_bfd))
	    error("error parsing symbol file: invalid string table size (%d bytes)",
                  DBX_STRINGTAB_SIZE(objfile));
	}
      DBX_STRINGTAB(objfile) =
        (char *)obstack_alloc(&objfile->objfile_obstack,
                              (DBX_STRINGTAB_SIZE(objfile) + 1));
      OBJSTAT(objfile, sz_strtab += (DBX_STRINGTAB_SIZE(objfile) + 1));

      /* Now read in the string table in one big gulp: */
      val =
        bfd_get_section_contents(sym_bfd, stabstrsect,
                                 DBX_STRINGTAB(objfile), 0,
                                 DBX_STRINGTAB_SIZE(objfile));

      if (!val)
        perror_with_name(name);
#if defined(TARGET_ARM) && defined(NM_NEXTSTEP)
	}
#endif /* TARGET_ARM && NM_NEXTSTEP */
#if HAVE_MMAP
    }
#endif /* HAVE_MMAP */

  /* APPLE LOCAL: Get the "local" vs "nonlocal" nlist record locations
     from the LC_DYSYMTAB load command if it was provided. */
  local_stabsect = bfd_get_section_by_name(sym_bfd, local_stab_name);
  nonlocal_stabsect = bfd_get_section_by_name(sym_bfd, nonlocal_stab_name);
  if ((local_stabsect == NULL) || (nonlocal_stabsect == NULL))
    local_stabsect = nonlocal_stabsect = NULL;

  /* APPLE LOCAL: Initialize the local/non-local stab nlist record pointers
     Set everything to 0 if there is no information provided by the static link
     editor -- users of these values should fall back to using the standard
     DBX_SYMTAB_OFFSET et al values for all stab records. */
  if (local_stabsect == NULL)
    {
      DBX_LOCAL_STAB_OFFSET(objfile) = 0;
      DBX_LOCAL_STAB_COUNT(objfile) = 0;
      DBX_NONLOCAL_STAB_OFFSET(objfile) = 0;
      DBX_NONLOCAL_STAB_COUNT(objfile) = 0;
    }
  else
    {
      /* XXX - FIXME: POKING INSIDE BFD DATA STRUCTURES */
      DBX_LOCAL_STAB_OFFSET(objfile) = local_stabsect->filepos;
      DBX_LOCAL_STAB_COUNT(objfile) =
        ((int)bfd_section_size(sym_bfd, local_stabsect)
         / DBX_SYMBOL_SIZE(objfile));
      /* XXX - FIXME: POKING INSIDE BFD DATA STRUCTURES */
      DBX_NONLOCAL_STAB_OFFSET(objfile) = nonlocal_stabsect->filepos;
      DBX_NONLOCAL_STAB_COUNT(objfile) =
        ((int)bfd_section_size(sym_bfd, nonlocal_stabsect)
         / DBX_SYMBOL_SIZE(objfile));
    }

  stabsread_new_init();
  buildsym_new_init();
  free_header_files();
  init_header_files();

#if 0 || 1
  install_minimal_symbols(objfile);
#endif /* 0 || 1 */

  processing_acc_compilation = 1;
  dbx_symfile_read(objfile, mainline);
}

/* FIXME: needs comment: */
static void
macho_symfile_read(struct objfile *objfile, int mainline)
{
  bfd *abfd = objfile->obfd;
  struct cleanup *minsym_cleanup;

  struct bfd_mach_o_load_command *gsymtab, *gdysymtab;

  struct bfd_mach_o_symtab_command *symtab = NULL;
  struct bfd_mach_o_dysymtab_command *dysymtab = NULL;

  int ret;

  CHECK_FATAL(objfile != NULL);
  CHECK_FATAL(abfd != NULL);
  CHECK_FATAL(abfd->filename != NULL);

  /* If this objfile is pointing to a stub library -- a library whose text
     and data have been stripped -- stop processing right now. gdb will
     try to examine the text or data and does not handle it gracefully when
     they are not present.  */
  if (bfd_mach_o_stub_library(abfd))
    {
      printf_filtered(_("%s: objfile is a stub library; skipping.\n"),
		      __FILE__);
      return;
    }

  /* Also, if the binary is encrypted, then it will only confuse us. We will
     skip reading this in, and gdb will read it from memory later on.  */
  if (bfd_mach_o_encrypted_binary(abfd))
    {
      printf_filtered(_("%s: binary is encrypted; skipping reading now to avoid confusion.\n"),
		      __FILE__);
      return;
    }

  init_minimal_symbol_collection();
  minsym_cleanup = make_cleanup_discard_minimal_symbols();

  /* If we are reinitializing, or if we have never loaded syms yet,
     set table to empty. MAINLINE is cleared so that *_read_psymtab
     functions do not all also re-initialize the psymbol table. */
  if (mainline)
    {
      printf_filtered(_("%s: setting table to empty; mainline: %d.\n"),
		      __FILE__, mainline);
      init_psymbol_list(objfile, 0);
      mainline = 0;
    }

  if (info_verbose && macosx_bfd_is_in_memory(abfd) && target_is_remote()
      && !target_is_kdp_remote())
    {
      warning(_("Copying %s from device memory..."), abfd->filename);
    }
  macho_build_psymtabs(objfile, mainline,
                       "LC_SYMTAB.stabs", "LC_SYMTAB.stabstr",
                       "LC_SEGMENT.__TEXT.__text",
                       "LC_DYSYMTAB.localstabs",
                       "LC_DYSYMTAB.nonlocalstabs",
                       "LC_SEGMENT.__TEXT.__textcoal_nt",
                       "LC_SEGMENT.__DATA.__data",
                       "LC_SEGMENT.__DATA.__bss");

  if (dwarf2_has_info(objfile))
    {
      dwarf2_build_psymtabs(objfile, mainline);
      if (use_eh_frames_info)
        dwarf2_build_frame_info(objfile);
    }
  else if ((dwarf_eh_frame_section != NULL) && use_eh_frames_info)
    {
      dwarf2_build_frame_info(objfile);
    }

  if (mach_o_process_exports_flag)
    {
      ret = bfd_mach_o_lookup_command(abfd, BFD_MACH_O_LC_SYMTAB, &gsymtab);
      if (ret != 1)
        {
#if (defined(DEBUG) || defined(_DEBUG) || defined(GDB_DEBUG)) && defined(LC_SYMTAB)
          warning("Error %d fetching LC_SYMTAB load command from object file \"%s\"",
                  ret, abfd->filename);
#endif /* (DEBUG || _DEBUG || GDB_DEBUG) && LC_SYMTAB */
          install_minimal_symbols(objfile);
          return;
        }

      ret =
        bfd_mach_o_lookup_command(abfd, BFD_MACH_O_LC_DYSYMTAB,
                                  &gdysymtab);
      if (ret != 1)
        {
#if (defined(DEBUG) || defined(_DEBUG) || defined(GDB_DEBUG)) && defined(LC_DYSYMTAB)
          warning("Error %d fetching LC_DYSYMTAB load command from object file \"%s\"",
                  ret, abfd->filename);
#endif /* (DEBUG || _DEBUG || GDB_DEBUG) && LC_DYSYMTAB */
          install_minimal_symbols(objfile);
          return;
        }

      CHECK_FATAL(gsymtab->type == BFD_MACH_O_LC_SYMTAB);
      CHECK_FATAL(gdysymtab->type == BFD_MACH_O_LC_DYSYMTAB);

      symtab = &gsymtab->command.symtab;
      dysymtab = &gdysymtab->command.dysymtab;

      if (symtab->strtab == NULL)
        {
          symtab->strtab = DBX_STRINGTAB(objfile);
        }

      if (symtab->strtab == NULL)
        {
          ret = bfd_mach_o_scan_read_symtab_strtab(abfd, symtab);
          if (ret != 0)
            {
              warning("Unable to read symbol table for \"%s\": %s (ret is %d)",
                      abfd->filename, bfd_errmsg(bfd_get_error()), ret);
              install_minimal_symbols(objfile);
              return;
            }
        }

      CHECK_FATAL(abfd != NULL);
      CHECK_FATAL(dysymtab != NULL);
      CHECK_FATAL(symtab != NULL);
      CHECK_FATAL(objfile != NULL);

#if (defined(DEBUG) || defined(_DEBUG) || defined(GDB_DEBUG))
      printf_filtered(_("%s: will to try to read indirect symbols from %s next,"
			" with abfd %p, dysymtab %p, symtab %p,"
			" and objfile %p.\n"),
		      __FILE__, abfd->filename, (void *)abfd, (void *)dysymtab,
		      (void *)symtab, (void *)objfile);
#endif /* (DEBUG || _DEBUG || GDB_DEBUG) */

      if (!macho_read_indirect_symbols(abfd, dysymtab, symtab, objfile))
        {
          install_minimal_symbols(objfile);
          return;
        }
    }

  install_minimal_symbols(objfile);
  do_cleanups(minsym_cleanup);
}

/* Record minsyms for the dyld stub trampolines, and prefix them with
 * "dyld_stub_": */
ATTRIBUTE_W_U_R int
macho_read_indirect_symbols(bfd *abfd,
                            struct bfd_mach_o_dysymtab_command *dysymtab,
                            struct bfd_mach_o_symtab_command *symtab,
                            struct objfile *objfile)
{
  /* FIXME: ld: warning: could not create compact unwind for
   * _macho_read_indirect_symbols: stack subq instruction is too different from
   * dwarf stack size */
  unsigned int i, nsyms, ret;
  asymbol sym;
  asection *bfdsec = (asection *)NULL;
  int section_count;
  struct bfd_mach_o_section *section = NULL;
  struct bfd_mach_o_load_command *lcommand = NULL;

  for (section_count = abfd->section_count, bfdsec = abfd->sections;
       section_count > 0; section_count--, bfdsec = bfdsec->next)
    {
      struct obj_section *osect;
      int osect_idx;
      int found_it;

      ret = bfd_mach_o_lookup_section(abfd, bfdsec, &lcommand, &section);
      if (ret != 1)
        {
#if (defined(DEBUG) || defined(_DEBUG) || defined(GDB_DEBUG)) && defined(bfd_section_name)
          warning("Error fetching section %s from object file \"%s\"; %d such sections found.",
                  bfd_section_name(abfd, bfdsec), abfd->filename, ret);
#endif /* (DEBUG || _DEBUG || GDB_DEBUG) && bfd_section_name */
          continue;
        }
      if (section == NULL)
        continue;
      if ((section->flags & BFD_MACH_O_SECTION_TYPE_MASK)
          != BFD_MACH_O_S_SYMBOL_STUBS)
        continue;
      if (section->reserved2 == 0)
        {
          warning("section %s has S_SYMBOL_STUBS flag set, but not reserved2",
                  bfd_section_name(abfd, bfdsec));
          continue;
        }

      /* We need to find the correct section to get the offset
       * for these symbols. In the dyld shared cache, the IMPORT
       * segments slides independently of the TEXT & DATA segments.
       * so we cannot just apply the TEXT offset.
       * It is kind of annoying that the bfd index is NOT the same as
       * the objfile section_offsets index, so we have to search
       * for it like this, but...  */

      osect_idx = 0;
      found_it = 0;

      ALL_OBJFILE_OSECTIONS(objfile, osect)
	{
	  if (osect->the_bfd_section == bfdsec)
	    {
	      found_it = 1;
	      break;
	    }
	  else
	    osect_idx++;
	}
      if (!found_it)
	osect_idx = SECT_OFF_TEXT(objfile);


      nsyms = (unsigned int)(section->size / section->reserved2);

      for (i = 0U; i < nsyms; i++)
        {
          CORE_ADDR cursym = (section->reserved1 + i);
          CORE_ADDR stubaddr = (section->addr + (i * section->reserved2));
          const char *sname = (const char *)NULL;
          char nname[4096];
	  char sname_arr[4096];
	  int i_i;
	  int printed;
	  void *nameptr;
	  struct minimal_symbol *myminsym = (struct minimal_symbol *)NULL;

	  nameptr = memset((void *)&nname, 0, sizeof(nname));

	  CHECK_FATAL(nameptr != NULL);

          if (cursym >= dysymtab->nindirectsyms)
            {
              warning("Indirect symbol entry out of range in \"%s\" (0x%s >= 0x%s)",
                      abfd->filename, paddr_nz(cursym),
                      paddr_nz(dysymtab->nindirectsyms));
              return 0;
            }
          ret =
            bfd_mach_o_scan_read_dysymtab_symbol(abfd, dysymtab, symtab,
                                                 &sym, (unsigned long)cursym);
          if (ret != 0)
            {
#if (defined(DEBUG) || defined(_DEBUG) || defined(GDB_DEBUG))
	      warning(_("Issue reading dysymtab symbol; cursym: %lu, ret: %d."),
		      (unsigned long)cursym, ret);
#endif /* DEBUG || _DEBUG || GDB_DEBUG */
              return 0;
            }

          sname = sym.name;
          CHECK_FATAL(sname != NULL);
          if (sname[0] == bfd_get_symbol_leading_char(abfd))
            {
              sname++;
            }

          CHECK_FATAL((strlen(sname) + sizeof("dyld_stub_") + 1UL) < 4096UL);
	  /* Try to pacify -Wformat-length by putting sname into an equivalent
	   * array: */
	  for (i_i = 0; i_i < 4096; i_i++) {
	    sname_arr[i_i] = *sname;
	    sname++;
	    if (sname_arr[i_i] == '\0') {
	      break;
	    }
	  }
	  /* The ".4085" in the format string is for -Wformat-truncation: */
          printed = snprintf(nname, sizeof(nname), "dyld_stub_%.4085s",
			     sname_arr);
	  CHECK_FATAL((size_t)printed == strlen(nname));
#if (defined(DEBUG) || defined(_DEBUG) || defined(GDB_DEBUG))
	  if (i == 0) {
	    printf_filtered(_("\n"));
	  }
	  printf_filtered(_("%s: In loop iteration %d, recording minsym %s \n"
			    "\twith address 0x%s, osect_idx %d,"
			    " bfdsec %p, and objfile %p...\n"),
			  __FILE__, i, nname, paddr(stubaddr), osect_idx,
			  (void *)bfdsec, (void *)objfile);
#endif /* DEBUG || _DEBUG || GDB_DEBUG */
	  CHECK_FATAL(printed < 4096);

          stubaddr += objfile_section_offset(objfile, osect_idx);
	  CHECK_FATAL(stubaddr != INVALID_ADDRESS);
          myminsym = prim_record_minimal_symbol_and_info(nname, stubaddr,
							 mst_solib_trampoline,
							 NULL, osect_idx,
							 bfdsec, objfile);
	  CHECK_FATAL(myminsym != (struct minimal_symbol *)NULL);
        }
    }

  return 1;
}

/* FIXME: add comment: */
static void
macho_symfile_finish(struct objfile *objfile)
{
  if (objfile == NULL) {
    warning(_("NULL objfile passed to macho_symfile_finish()."));
  }
  return;
}

/* */
static void
macho_symfile_offsets(struct objfile *objfile,
                      struct section_addr_info *addrs)
{
  unsigned int i;
  size_t num_sections;
  struct obj_section *osect;

  objfile->num_sections = (int)(objfile->sections_end - objfile->sections);
  objfile->section_offsets =
    ((struct section_offsets *)
     obstack_alloc(&objfile->objfile_obstack,
		   (int)SIZEOF_N_SECTION_OFFSETS(objfile->num_sections)));
  memset(objfile->section_offsets, 0,
         SIZEOF_N_SECTION_OFFSETS(objfile->num_sections));

  /* This code is run when we first add the objfile with
     symfile_add_with_addrs_or_offsets, when "addrs" not "offsets" are passed
     in. The place in symfile.c where the addrs are applied depends on the
     addrs having section names. But in the dyld code we build an anonymous
     array of addrs, so that code is a no-op. Because of that, we have to
     apply the addrs to the sections here.  N.B. if an objfile slides after
     we have already created it, then it goes through objfile_relocate.  */

  if (addrs->other[0].addr != 0)
    {
      num_sections = (objfile->sections_end - objfile->sections);
      for (i = 0U; i < num_sections; i++)
        {
          objfile->sections[i].addr += addrs->other[0].addr;
          objfile->sections[i].endaddr += addrs->other[0].addr;
        }
    }

  for (i = 0U; i < (unsigned int)objfile->num_sections; i++)
    {
      objfile->section_offsets->offsets[i] = addrs->other[0].addr;
    }

  /* You might think you could use the index from the bfd_section here,
     but you would be wrong. We do NOT make objfile sections from ALL the
     bfd_sections (the ones with 0 length get dropped.) The sect_index_*
     are used to index into the objfile's section_offsets, which in turn
     is supposed to map to the objfile sections. So we have to do it this
     way instead.  */

  /* NB: The code below is (mostly) a reimplementation of
     symfile.c:init_objfile_sect_indices() */

  i = 0;
  objfile->sect_index_text = 0;
  objfile->sect_index_data = 0;
  objfile->sect_index_bss = 0;
  objfile->sect_index_rodata = 0;

  ALL_OBJFILE_OSECTIONS(objfile, osect)
    {
      const char *bfd_sect_name = osect->the_bfd_section->name;

#if defined(TEXT_SEGMENT_NAME) && defined(DATA_SECTION_NAME) && defined(BSS_SECTION_NAME)
      if (strcmp(bfd_sect_name, TEXT_SEGMENT_NAME) == 0)
	objfile->sect_index_text = i;
      else if (strcmp(bfd_sect_name, DATA_SECTION_NAME) == 0)
	objfile->sect_index_data = i;
      else if (strcmp(bfd_sect_name, BSS_SECTION_NAME) == 0)
	objfile->sect_index_bss = i;
#else
      gdb_assert(bfd_sect_name != NULL);
#endif /* TEXT_SEGMENT_NAME && DATA_SECTION_NAME && BSS_SECTION_NAME */
      i++;
    }
}


/* This function calculates the offset between the dsym file and
   the binary it was made from, if both were loaded at their
   set addresses. This may be different if the binary was rebased
   after the dsym file was made.  */
static ATTRIBUTE_W_U_R CORE_ADDR
macho_calculate_dsym_offset(bfd *exe_bfd, bfd *sym_bfd)
{
  asection *sym_text_sect = (asection *)NULL;
  asection *exe_text_sect = (asection *)NULL;
  CORE_ADDR exe_text_addr;
  CORE_ADDR sym_text_addr;

#ifdef TEXT_SEGMENT_NAME
  /* Extract the sym file BFD section for the __TEXT segment: */
  sym_text_sect = bfd_get_section_by_name(sym_bfd, TEXT_SEGMENT_NAME);
#endif /* TEXT_SEGMENT_NAME */
  if (!sym_text_sect)
    return 0;

#ifdef TEXT_SEGMENT_NAME
  /* Extract the exe file BFD section for the __TEXT segment: */
  exe_text_sect = bfd_get_section_by_name(exe_bfd, TEXT_SEGMENT_NAME);
#endif /* TEXT_SEGMENT_NAME */

  /* FIXME: Is this warning correct? */
  if (!exe_text_sect)
    {
      warning(_("Failed to extract exe file BFD section for the __TEXT segment.\n"));
      return 0;
    }

  /* Get the virtual address for each segment: */
  exe_text_addr = bfd_get_section_vma(exe_bfd, exe_text_sect);
  sym_text_addr = bfd_get_section_vma(sym_bfd, sym_text_sect);
  /* Return the difference: */
  return (exe_text_addr - sym_text_addr);
}

/* This function takes either the addrs or the offsets array that the
   actual objfile for a separate_debug_file would use, and conses up
   a section_offsets array for the separate debug objfile. In the case
   of macho it is just a copy of the objfile's section array, plus the
   potential offset between the dSYM and the objfile's load addresses.  */
void
macho_calculate_offsets_for_dsym(struct objfile *main_objfile,
				 bfd *sym_bfd,
				 struct section_addr_info *addrs,
				 struct section_offsets *in_offsets,
				 int in_num_offsets,
				 struct section_offsets **sym_offsets,
				 int *sym_num_offsets)
{
  bfd_boolean in_mem_shared_cache;
  int i = 0;
  CORE_ADDR dsym_offset;
  in_mem_shared_cache = bfd_mach_o_in_shared_cached_memory(main_objfile->obfd);

  if (in_mem_shared_cache)
    {
      gdb_assert(in_offsets);
      if (in_offsets)
	{
	 /* When we have a main_objfile that is in the shared cache and is
	  * also memory based, we need to figure out the differences of each
	  * section compared to the mach segment map that is found in modern
	  * dSYM files. dSYM files contain all of the segment load commands
	  * from the original executable and we can figure out the offsets
	  * or addresses accordingly.  */
	  struct bfd_section *sym_sect = NULL;
	  struct bfd_section *exe_sect = NULL;
	  bfd *exe_bfd = main_objfile->obfd;

	  *sym_offsets = ((struct section_offsets *)
			  xmalloc(SIZEOF_N_SECTION_OFFSETS(in_num_offsets)));
	  memset(*sym_offsets, 0,
		 SIZEOF_N_SECTION_OFFSETS(in_num_offsets));

	  exe_sect = exe_bfd->sections;
	  sym_sect = sym_bfd->sections;
	  for (i = 0;
	       (exe_sect != NULL) && (i < in_num_offsets);
	       exe_sect = exe_sect->next, i++)
	    {
              struct bfd_section *sect;
	      if ((i > 0) && (sym_sect != NULL))
		sym_sect = sym_sect->next;

	      sect = NULL;
	      if (sym_sect && (strcmp(exe_sect->name, sym_sect->name) == 0))
		sect = sym_sect;
	      else
		{
		  /* Sections were out of order, let us search linearly for the
		     section with the same name.  */
		  for (sect = sym_bfd->sections; sect != NULL; sect = sect->next)
		    {
		      if (strcmp(exe_sect->name, sect->name) == 0)
			break;
		    }
		}

	      (*sym_offsets)->offsets[i] = ANOFFSET(in_offsets, i);
	      if (sect)
		(*sym_offsets)->offsets[i] += (exe_sect->vma - sect->vma);
	    }
	  *sym_num_offsets = in_num_offsets;
	  return;
	}
    }

  dsym_offset = macho_calculate_dsym_offset(main_objfile->obfd,
                                            sym_bfd);
  if (in_offsets)
    {
      *sym_offsets = ((struct section_offsets *)
		      xmalloc(SIZEOF_N_SECTION_OFFSETS(in_num_offsets)));
      for (i = 0; i < in_num_offsets; i++)
	(*sym_offsets)->offsets[i] = (ANOFFSET(in_offsets, i) + dsym_offset);

      *sym_num_offsets = in_num_offsets;
    }
  else if (addrs)
    {
#ifdef TEXT_SEGMENT_NAME
      /* This is kind of gross, but this is how add-symbol-file passes
       * the addr down if the user just supplied a single address.  But they
       * did NOT really intend for us JUST to offset the TEXT_SEGMENT, then
       * meant this is a constant slide.  So do that:  */
      if ((addrs->num_sections == 1)
	  && (strcmp(addrs->other[0].name, TEXT_SEGMENT_NAME) == 0))
	{
	  CORE_ADDR adjustment = dsym_offset;
	  if (!addrs->addrs_are_offsets)
	    {
	      asection *text_segment =
		bfd_get_section_by_name(sym_bfd, TEXT_SEGMENT_NAME);
	      if (text_segment != NULL)
		adjustment -= bfd_get_section_vma(sym_bfd, text_segment);
	    }
	  *sym_num_offsets = bfd_count_sections(sym_bfd);
	  *sym_offsets = ((struct section_offsets *)
			  xmalloc(SIZEOF_N_SECTION_OFFSETS(*sym_num_offsets)));
	  for (i = 0; i < *sym_num_offsets; i++)
	    {
	      (*sym_offsets)->offsets[i] = (dsym_offset + addrs->other[0].addr
					    + adjustment);
	    }
	}
      else
	{
	  /* This branch assumes the addrs are in the same order as
	     the offsets for this objfile. I actually do NOT think
	     that is right, since these addrs are generally right for
	     the main objfile, and the dsym objfile has different
	     sections.

	     This works by because we usually only have a rigid slide,
	     and we only care about the offsets for the text segment
	     anyway (using that for baseaddr.)

	     It also does NOT properly handle the
	     !addrs->addrs_are_offsets case properly. Then we should
	     look up the load address of each section and subtract
	     that from the section addr.
	     I am not going to fix this right now. */

	  *sym_offsets =
	    ((struct section_offsets *)
	     xmalloc(SIZEOF_N_SECTION_OFFSETS(addrs->num_sections)));
	  for (i = 0; i < (int)addrs->num_sections; i++)
	    {
	      (*sym_offsets)->offsets[i] = dsym_offset;
	      if (addrs->addrs_are_offsets)
		(*sym_offsets)->offsets[i] += addrs->other[0].addr;
              else
                {
                  /* This clause will be hit when we are doing add-kext -
                     the kernel provides us the actual load addresses of
                     the sections (so ADDRS is filled in) instead of
                     offsets (so ADDRS_IS_OFFSETS is 0).  */
                  if (addrs->other[i].name)
                    {
                      asection *exe_sect;
                      exe_sect = bfd_get_section_by_name(main_objfile->obfd,
							 addrs->other[i].name);
                      (*sym_offsets)->offsets[i] = (addrs->other[i].addr
						    - exe_sect->vma);
                    }
                }
	    }
	  *sym_num_offsets = (int)addrs->num_sections;
	}
#endif /* TEXT_SEGMENT_NAME */
    }
  else if (dsym_offset != 0)
    {
      *sym_offsets = ((struct section_offsets *)
		      xmalloc(SIZEOF_N_SECTION_OFFSETS(in_num_offsets)));
      for (i = 0; i < in_num_offsets; i++)
	(*sym_offsets)->offsets[i] = dsym_offset;

      *sym_num_offsets = in_num_offsets;
    }
  else
    {
      *sym_offsets = NULL;
      *sym_num_offsets = 0;
    }
}

/* */
static struct sym_fns macho_sym_fns = {
  bfd_target_mach_o_flavour,

  macho_new_init,        /* sym_new_init: init anything gbl to entire symtab */
  macho_symfile_init,    /* sym_init: read initial info, setup for sym_read() */
  macho_symfile_read,    /* sym_read: read a symbol file into symtab */
  macho_symfile_finish,  /* sym_finish: finished with file, cleanup */
  macho_symfile_offsets, /* sym_offsets:  xlate external to internal form */
  NULL                   /* next: pointer to next struct sym_fns */
};

/* Usual gdb initialization hook: */
void
_initialize_machoread(void)
{
  add_symtab_fns(&macho_sym_fns);

#if defined(HAVE_MMAP) && HAVE_MMAP
  add_setshow_boolean_cmd("mmap-string-tables", class_obscure,
			  &mmap_strtabflag, _("\
Set if GDB should use mmap() to read STABS info."), _("\
Show if GDB should use mmap() to read STABS info."), NULL,
			  NULL, NULL, &setlist, &showlist);
#endif /* HAVE_MMAP */

  add_setshow_boolean_cmd("use-eh-frame-info", class_obscure,
			  &use_eh_frames_info, _("\
Set if GDB should use the EH frame/DWARF CFI information to backtrace."), _("\
Show if GDB should use the EH frame/DWARF CFI information to backtrace."), NULL,
			  NULL, NULL, &setlist, &showlist);

  add_setshow_boolean_cmd("mach-o-process-exports", class_obscure,
			  &mach_o_process_exports_flag, _("\
Set if GDB should process indirect function stub symbols from object files."), _("\
Show if GDB should process indirect function stub symbols from object files."), NULL,
			  NULL, NULL, &setlist, &showlist);
}

/* EOF */
