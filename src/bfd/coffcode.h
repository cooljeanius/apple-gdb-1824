/* coffcode.h
   Support for the generic parts of most COFF variants, for BFD.
   Copyright 1990, 1991, 1992, 1993, 1994, 1995, 1996, 1997, 1998, 1999,
   2000, 2001, 2002, 2003, 2004, 2005
   Free Software Foundation, Inc.
   Written by Cygnus Support.

   This file is part of BFD, the Binary File Descriptor library.

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
   Foundation, Inc., 51 Franklin St., 5th Floor, Boston, MA 02110-1301, USA */

/* Most of this hacked by  Steve Chamberlain,
			sac@cygnus.com.  */
/*
SECTION
	coff backends

	BFD supports a number of different flavours of coff format.
	The major differences between formats are the sizes and
	alignments of fields in structures on disk, and the occasional
	extra field.

	Coff in all its varieties is implemented with a few common
	files and a number of implementation specific files. For
	example, The 88k bcs coff format is implemented in the file
	@file{coff-m88k.c}. This file @code{#include}s
	@file{coff/m88k.h} which defines the external structure of the
	coff format for the 88k, and @file{coff/internal.h} which
	defines the internal structure. @file{coff-m88k.c} also
	defines the relocations used by the 88k format
	@xref{Relocations}.

	The Intel i960 processor version of coff is implemented in
	@file{coff-i960.c}. This file has the same structure as
	@file{coff-m88k.c}, except that it includes @file{coff/i960.h}
	rather than @file{coff-m88k.h}.

SUBSECTION
	Porting to a new version of coff

	The recommended method is to select from the existing
	implementations the version of coff which is most like the one
	you want to use. For example, we will say that i386 coff is
	the one you select, and that your coff flavour is called foo.
	Copy @file{i386coff.c} to @file{foocoff.c}, copy
	@file{../include/coff/i386.h} to @file{../include/coff/foo.h},
	and add the lines to @file{targets.c} and @file{Makefile.in}
	so that your new back end is used. Alter the shapes of the
	structures in @file{../include/coff/foo.h} so that they match
	what you need. You will probably also have to add
	@code{#ifdef}s to the code in @file{coff/internal.h} and
	@file{coffcode.h} if your version of coff is too wild.

	You can verify that your new BFD backend works quite simply by
	building @file{objdump} from the @file{binutils} directory,
	and making sure that its version of what is going on and your
	host system's idea (assuming it has the pretty standard coff
	dump utility, usually called @code{att-dump} or just
	@code{dump}) are the same. Then clean up your code, and send
	what you have done to Cygnus. Then your stuff will be in the
	next release, and you will NOT have to keep integrating it.

SUBSECTION
	How the coff backend works

SUBSUBSECTION
	File layout

	The Coff backend is split into generic routines that are
	applicable to any Coff target and routines that are specific
	to a particular target.  The target-specific routines are
	further split into ones which are basically the same for all
	Coff targets except that they use the external symbol format
	or use different values for certain constants.

	The generic routines are in @file{coffgen.c}.  These routines
	work for any Coff target.  They use some hooks into the target
	specific code; the hooks are in a @code{bfd_coff_backend_data}
	structure, one of which exists for each target.

	The essentially similar target-specific routines are in
	@file{coffcode.h}.  This header file includes executable C code.
	The various Coff targets first include the appropriate Coff
	header file, make any special defines that are needed, and
	then include @file{coffcode.h}.

	Some of the Coff targets then also have additional routines in
	the target source file itself.

	For example, @file{coff-i960.c} includes
	@file{coff/internal.h} and @file{coff/i960.h}.  It then
	defines a few constants, such as @code{I960}, and includes
	@file{coffcode.h}.  Since the i960 has complex relocation
	types, @file{coff-i960.c} also includes some code to
	manipulate the i960 relocs.  This code is not in
	@file{coffcode.h} because it would not be used by any other
	target.

SUBSUBSECTION
	Bit twiddling

	Each flavour of coff supported in BFD has its own header file
	describing the external layout of the structures. There is also
	an internal description of the coff layout, in
	@file{coff/internal.h}. A major function of the
	coff backend is swapping the bytes and twiddling the bits to
	translate the external form of the structures into the normal
	internal form. This is all performed in the
	@code{bfd_swap}_@i{thing}_@i{direction} routines. Some
	elements are different sizes between different versions of
	coff; it is the duty of the coff version specific include file
	to override the definitions of various packing routines in
	@file{coffcode.h}. E.g., the size of line number entry in coff is
	sometimes 16 bits, and sometimes 32 bits. @code{#define}ing
	@code{PUT_LNSZ_LNNO} and @code{GET_LNSZ_LNNO} will select the
	correct one. No doubt, some day someone will find a version of
	coff which has a varying field size not catered to at the
	moment. To port BFD, that person will have to add more @code{#defines}.
	Three of the bit twiddling routines are exported to
	@code{gdb}; @code{coff_swap_aux_in}, @code{coff_swap_sym_in}
	and @code{coff_swap_lineno_in}. @code{GDB} reads the symbol
	table on its own, but uses BFD to fix things up.  More of the
	bit twiddlers are exported for @code{gas};
	@code{coff_swap_aux_out}, @code{coff_swap_sym_out},
	@code{coff_swap_lineno_out}, @code{coff_swap_reloc_out},
	@code{coff_swap_filehdr_out}, @code{coff_swap_aouthdr_out},
	@code{coff_swap_scnhdr_out}. @code{Gas} currently keeps track
	of all the symbol table and reloc drudgery itself, thereby
	saving the internal BFD overhead, but uses BFD to swap things
	on the way out, making cross ports much safer.  Doing so also
	allows BFD (and thus the linker) to use the same header files
	as @code{gas}, which makes one avenue to disaster disappear.

SUBSUBSECTION
	Symbol reading

	The simple canonical form for symbols used by BFD is not rich
	enough to keep all the information available in a coff symbol
	table. The back end gets around this problem by keeping the original
	symbol table around, "behind the scenes".

	When a symbol table is requested (through a call to
	@code{bfd_canonicalize_symtab}), a request gets through to
	@code{coff_get_normalized_symtab}. This reads the symbol table from
	the coff file and swaps all the structures inside into the
	internal form. It also fixes up all the pointers in the table
	(represented in the file by offsets from the first symbol in
	the table) into physical pointers to elements in the new
	internal table. This involves some work since the meanings of
	fields change depending upon context: a field that is a
	pointer to another structure in the symbol table at one moment
	may be the size in bytes of a structure at the next.  Another
	pass is made over the table. All symbols which mark file names
	(<<C_FILE>> symbols) are modified so that the internal
	string points to the value in the auxent (the real filename)
	rather than the normal text associated with the symbol
	(@code{".file"}).

	At this time the symbol names are moved around. Coff stores
	all symbols less than nine characters long physically
	within the symbol table; longer strings are kept at the end of
	the file in the string 	table. This pass moves all strings
	into memory and replaces them with pointers to the strings.

	The symbol table is massaged once again, this time to create
	the canonical table used by the BFD application. Each symbol
	is inspected in turn, and a decision made (using the
	@code{sclass} field) about the various flags to set in the
	@code{asymbol}.  @xref{Symbols}. The generated canonical table
	shares strings with the hidden internal symbol table.

	Any linenumbers are read from the coff file too, and attached
	to the symbols which own the functions the linenumbers belong to.

SUBSUBSECTION
	Symbol writing

	Writing a symbol to a coff file which did NOT come from a coff
	file will lose any debugging information. The @code{asymbol}
	structure remembers the BFD from which the symbol was taken, and on
	output the back end makes sure that the same destination target as
	source target is present.

	When the symbols have come from a coff file then all the
	debugging information is preserved.

	Symbol tables are provided for writing to the back end in a
	vector of pointers to pointers. This allows applications like
	the linker to accumulate and output large symbol tables
	without having to do too much byte copying.

	This function runs through the provided symbol table and
	patches each symbol marked as a file place holder
	(@code{C_FILE}) to point to the next file place holder in the
	list. It also marks each @code{offset} field in the list with
	the offset from the first symbol of the current symbol.

	Another function of this procedure is to turn the canonical
	value form of BFD into the form used by coff. Internally, BFD
	expects symbol values to be offsets from a section base; so a
	symbol physically at 0x120, but in a section starting at
	0x100, would have the value 0x20. Coff expects symbols to
	contain their final value, so symbols have their values
	changed at this point to reflect their sum with their owning
	section.  This transformation uses the
	<<output_section>> field of the @code{asymbol}'s
	@code{asection} @xref{Sections}.

	o <<coff_mangle_symbols>>

	This routine runs though the provided symbol table and uses
	the offsets generated by the previous pass and the pointers
	generated when the symbol table was read in to create the
	structured hierarchy required by coff. It changes each pointer
	to a symbol into the index into the symbol table of the asymbol.

	o <<coff_write_symbols>>

	This routine runs through the symbol table and patches up the
	symbols from their internal form into the coff way, calls the
	bit twiddlers, and writes out the table to the file.

*/

/*
INTERNAL_DEFINITION
	coff_symbol_type

DESCRIPTION
	The hidden information for an <<asymbol>> is described in a
	<<combined_entry_type>>:

CODE_FRAGMENT
.
.typedef struct coff_ptr_struct
.{
.  {* Remembers the offset from the first symbol in the file for
.     this symbol. Generated by coff_renumber_symbols. *}
.  unsigned int offset;
.
.  {* Should the value of this symbol be renumbered.  Used for
.     XCOFF C_BSTAT symbols.  Set by coff_slurp_symbol_table.  *}
.  unsigned int fix_value : 1;
.
.  {* Should the tag field of this symbol be renumbered.
.     Created by coff_pointerize_aux. *}
.  unsigned int fix_tag : 1;
.
.  {* Should the endidx field of this symbol be renumbered.
.     Created by coff_pointerize_aux. *}
.  unsigned int fix_end : 1;
.
.  {* Should the x_csect.x_scnlen field be renumbered.
.     Created by coff_pointerize_aux. *}
.  unsigned int fix_scnlen : 1;
.
.  {* Fix up an XCOFF C_BINCL/C_EINCL symbol.  The value is the
.     index into the line number entries.  Set by coff_slurp_symbol_table.  *}
.  unsigned int fix_line : 1;
.
.  {* The container for the symbol structure as read and translated
.     from the file. *}
.  union
.  {
.    union internal_auxent auxent;
.    struct internal_syment syment;
.  } u;
.} combined_entry_type;
.
.
.{* Each canonical asymbol really looks like this: *}
.
.typedef struct coff_symbol_struct
.{
.  {* The actual symbol which the rest of BFD works with *}
.  asymbol symbol;
.
.  {* A pointer to the hidden information for this symbol *}
.  combined_entry_type *native;
.
.  {* A pointer to the linenumber information for this symbol *}
.  struct lineno_cache_entry *lineno;
.
.  {* Have the line numbers been relocated yet ? *}
.  bfd_boolean done_lineno;
.} coff_symbol_type;

*/

#ifndef __BFD_COFFCODE_H__
#define __BFD_COFFCODE_H__ 1

#ifdef COFF_WITH_PE
# include "peicode.h"
#else
# include "coffswap.h"
#endif /* COFF_WITH_PE */

#define STRING_SIZE_SIZE 4

#define DOT_DEBUG	".debug"
#define GNU_LINKONCE_WI ".gnu.linkonce.wi."

static long sec_to_styp_flags(const char *, flagword);
static bfd_boolean styp_to_sec_flags
  (bfd *, void *, const char *, asection *, flagword *);
static bfd_boolean coff_bad_format_hook(bfd *, void *);
static void coff_set_custom_section_alignment
  (bfd *, asection *, const struct coff_section_alignment_entry *,
   const unsigned int);
static bfd_boolean coff_new_section_hook(bfd *, asection *);
static bfd_boolean coff_set_arch_mach_hook(bfd *, void *);
static bfd_boolean coff_write_relocs(bfd *, int);
static bfd_boolean coff_set_flags(bfd *, unsigned int *, unsigned short *);
static bfd_boolean coff_set_arch_mach
  (bfd *, enum bfd_architecture, unsigned long);
static bfd_boolean coff_compute_section_file_positions(bfd *);
static bfd_boolean coff_write_object_contents(bfd *) ATTRIBUTE_USED;
static bfd_boolean coff_set_section_contents
  (bfd *, asection *, const void *, file_ptr, bfd_size_type);
static void * buy_and_read(bfd *, file_ptr, bfd_size_type);
static bfd_boolean coff_slurp_line_table(bfd *, asection *);
static bfd_boolean coff_slurp_symbol_table(bfd *);
static enum coff_symbol_classification coff_classify_symbol
  (bfd *, struct internal_syment *);
static bfd_boolean coff_slurp_reloc_table(bfd *, asection *, asymbol **);
static long coff_canonicalize_reloc
  (bfd *, asection *, arelent **, asymbol **);
#ifndef coff_mkobject_hook
static void * coff_mkobject_hook
  (bfd *, void *,  void *);
#endif /* !coff_mkobject_hook */
#ifdef COFF_WITH_PE
static flagword handle_COMDAT
  (bfd *, flagword, void *, const char *, asection *);
#endif /* COFF_WITH_PE */
#ifdef COFF_IMAGE_WITH_PE
static bfd_boolean coff_read_word
  (bfd *, unsigned int *);
static unsigned int coff_compute_checksum
  (bfd *);
static bfd_boolean coff_apply_checksum
  (bfd *);
#endif /* COFF_IMAGE_WITH_PE */
#ifdef TICOFF
static bfd_boolean ticoff0_bad_format_hook
  (bfd *, void * );
static bfd_boolean ticoff1_bad_format_hook
  (bfd *, void * );
#endif /* TICOFF */


/* void warning(); */

/* Return a word with STYP_* (scnhdr.s_flags) flags set to represent
 * the incoming SEC_* flags. The inverse of this function is
 * styp_to_sec_flags(). NOTE: If you add to/change this routine, you should
 * probably mirror the changes in styp_to_sec_flags(). */
#ifndef COFF_WITH_PE
/* Macros for setting debugging flags: */
#ifdef STYP_DEBUG
# define STYP_XCOFF_DEBUG STYP_DEBUG
#else
# define STYP_XCOFF_DEBUG STYP_INFO
#endif /* STYP_DEBUG */

#ifdef COFF_ALIGN_IN_S_FLAGS
# define STYP_DEBUG_INFO STYP_DSECT
#else
# define STYP_DEBUG_INFO STYP_INFO
#endif /* COFF_ALIGN_IN_S_FLAGS */

#ifndef _TEXT
# define _TEXT	".text"
#endif /* !_TEXT */
#ifndef _DATA
# define _DATA   ".data"
#endif /* !_DATA */
#ifndef _BSS
# define _BSS    ".bss"
#endif /* !_BSS */
#ifndef _PAD
# define _PAD	".pad"
#endif /* !_PAD */
#ifndef _LOADER
# define _LOADER ".loader"
#endif /* !_LOADER */
#ifndef _EXCEPT
# define _EXCEPT ".except"
#endif /* !_EXCEPT */
#ifndef _TYPCHK
# define _TYPCHK ".typchk"
#endif /* !_TYPCHK */
#ifndef STYP_LOADER
/* XCOFF uses a special .loader section with type STYP_LOADER.  */
# define STYP_LOADER 0x1000
#endif /* !STYP_LOADER */
#ifndef STYP_EXCEPT
/* Specifies an exception section.  A section of this type provides
 information to identify the reason that a trap or ececptin occured within
 and executable object program */
# define STYP_EXCEPT 0x0100
#endif /* !STYP_EXCEPT */
#ifndef STYP_TYPCHK
/* Specifies a type check section.  A section of this type contains parameter
 argument type check strings used by the AIX binder.  */
# define STYP_TYPCHK 0x4000
#endif /* !STYP_TYPCHK */

static long
sec_to_styp_flags(const char *sec_name, flagword sec_flags)
{
  long styp_flags = 0L;

  if (!strcmp(sec_name, _TEXT)) {
      styp_flags = STYP_TEXT;
  } else if (!strcmp(sec_name, _DATA)) {
      styp_flags = STYP_DATA;
  } else if (!strcmp(sec_name, _BSS)) {
      styp_flags = STYP_BSS;
#ifdef _COMMENT
  } else if (!strcmp(sec_name, _COMMENT)) {
      styp_flags = STYP_INFO;
#endif /* _COMMENT */
#ifdef _LIB
  } else if (!strcmp(sec_name, _LIB)) {
      styp_flags = STYP_LIB;
#endif /* _LIB */
#ifdef _LIT
  } else if (!strcmp(sec_name, _LIT)) {
      styp_flags = STYP_LIT;
#endif /* _LIT */
  } else if (!strncmp(sec_name, DOT_DEBUG, (sizeof(DOT_DEBUG) - 1))) {
      /* Handle the XCOFF debug section and DWARF2 debug sections.  */
      if (!sec_name[6]) {
	  styp_flags = STYP_XCOFF_DEBUG;
      } else {
	  styp_flags = STYP_DEBUG_INFO;
      }
  } else if (!strncmp(sec_name, ".stab", 5UL)) {
      styp_flags = STYP_DEBUG_INFO;
#ifdef COFF_LONG_SECTION_NAMES
  } else if (!strncmp(sec_name, GNU_LINKONCE_WI,
		      (sizeof(GNU_LINKONCE_WI) - 1))) {
      styp_flags = STYP_DEBUG_INFO;
#endif /* COFF_LONG_SECTION_NAMES */
#ifdef RS6000COFF_C
  } else if (!strcmp(sec_name, _PAD)) {
      styp_flags = STYP_PAD;
  } else if (!strcmp(sec_name, _LOADER)) {
      styp_flags = STYP_LOADER;
  } else if (!strcmp(sec_name, _EXCEPT)) {
      styp_flags = STYP_EXCEPT;
  } else if (!strcmp(sec_name, _TYPCHK)) {
      styp_flags = STYP_TYPCHK;
#endif /* RS6000COFF_C */
  /* Try and figure out what it should be: */
  } else if (sec_flags & SEC_CODE) {
      styp_flags = STYP_TEXT;
  } else if (sec_flags & SEC_DATA) {
      styp_flags = STYP_DATA;
  } else if (sec_flags & SEC_READONLY) {
#ifdef STYP_LIT			/* 29k readonly text/data section */
      styp_flags = STYP_LIT;
#else
      styp_flags = STYP_TEXT;
#endif /* STYP_LIT */
  } else if (sec_flags & SEC_LOAD) {
      styp_flags = STYP_TEXT;
  } else if (sec_flags & SEC_ALLOC) {
      styp_flags = STYP_BSS;
  }

#ifdef STYP_CLINK
  if (sec_flags & SEC_TIC54X_CLINK) {
    styp_flags |= STYP_CLINK;
  }
#endif /* STYP_CLINK */

#ifdef STYP_BLOCK
  if (sec_flags & SEC_TIC54X_BLOCK) {
      styp_flags |= STYP_BLOCK;
  }
#endif /* STYP_BLOCK */

#ifdef STYP_NOLOAD
  if ((sec_flags & (SEC_NEVER_LOAD | SEC_COFF_SHARED_LIBRARY)) != 0) {
      styp_flags |= STYP_NOLOAD;
  }
#endif /* STYP_NOLOAD */

  return styp_flags;
}

#else /* COFF_WITH_PE */

/* The PE version; see above for the general comments. The non-PE
 * case seems to be more guessing, and breaks PE format; specifically,
 * .rdata is readonly, but it surely is NOT text. Really, all this
 * should be set up properly in gas (or whatever assembler is in use),
 * and honor whatever objcopy/strip, etc. sent us as input.  */
static long
sec_to_styp_flags(const char *sec_name, flagword sec_flags)
{
  long styp_flags = 0L;

  /* caution: there are at least three groups of symbols that have
     very similar bits and meanings: IMAGE_SCN*, SEC_*, and STYP_*.
     SEC_* are the BFD internal flags, used for generic BFD
     information.  STYP_* are the COFF section flags which appear in
     COFF files.  IMAGE_SCN_* are the PE section flags which appear in
     PE files.  The STYP_* flags and the IMAGE_SCN_* flags overlap,
     but there are more IMAGE_SCN_* flags.  */

  /* FIXME: There is no gas syntax to specify the debug section flag: */
  if ((strncmp(sec_name, DOT_DEBUG, (sizeof(DOT_DEBUG) - 1)) == 0)
      || (strncmp(sec_name, GNU_LINKONCE_WI, (sizeof(GNU_LINKONCE_WI) - 1)) == 0))
    sec_flags = SEC_DEBUGGING;

  /* skip LOAD */
  /* READONLY later */
  /* skip RELOC */
  if ((sec_flags & SEC_CODE) != 0) {
    styp_flags |= IMAGE_SCN_CNT_CODE;
  }
  if ((sec_flags & SEC_DATA) != 0) {
    styp_flags |= IMAGE_SCN_CNT_INITIALIZED_DATA;
  }
  if (((sec_flags & SEC_ALLOC) != 0) && ((sec_flags & SEC_LOAD) == 0)) {
    styp_flags |= IMAGE_SCN_CNT_UNINITIALIZED_DATA;  /* ==STYP_BSS */
  }
  /* skip ROM */
  /* skip constRUCTOR */
  /* skip CONTENTS */
  if ((sec_flags & SEC_IS_COMMON) != 0) {
    styp_flags |= IMAGE_SCN_LNK_COMDAT;
  }
  if ((sec_flags & SEC_DEBUGGING) != 0) {
    styp_flags |= IMAGE_SCN_MEM_DISCARDABLE;
  }
  if ((sec_flags & SEC_EXCLUDE) != 0) {
    styp_flags |= IMAGE_SCN_LNK_REMOVE;
  }
  if ((sec_flags & SEC_NEVER_LOAD) != 0) {
    styp_flags |= IMAGE_SCN_LNK_REMOVE;
  }
  /* skip IN_MEMORY */
  /* skip SORT */
  if (sec_flags & SEC_LINK_ONCE) {
    styp_flags |= IMAGE_SCN_LNK_COMDAT;
  }
  /* skip LINK_DUPLICATES */
  /* skip LINKER_CREATED */

  if (sec_flags & (SEC_ALLOC | SEC_LOAD))
    {
      /* For now, the read/write bits are mapped onto SEC_READONLY, even
       * though the semantics do NOT quite match. The bits from the input
       * are retained in pei_section_data(abfd, section)->pe_flags.  */
      styp_flags |= IMAGE_SCN_MEM_READ;       /* Always readable.  */
      if ((sec_flags & SEC_READONLY) == 0) {
        styp_flags |= (long)IMAGE_SCN_MEM_WRITE; /* Invert READONLY for write. */
      }
      if (sec_flags & SEC_CODE) {
        styp_flags |= IMAGE_SCN_MEM_EXECUTE;  /* CODE->EXECUTE. */
      }
      if (sec_flags & SEC_COFF_SHARED) {
        styp_flags |= IMAGE_SCN_MEM_SHARED; /* Shared remains meaningful */
      }
    }

  return styp_flags;
}

#endif /* COFF_WITH_PE */

/* Return a word with SEC_* flags set to represent the incoming STYP_*
   flags (from scnhdr.s_flags).  The inverse of this function is
   sec_to_styp_flags().  NOTE: If you add to/change this routine, you
   should probably mirror the changes in sec_to_styp_flags().  */

#ifndef COFF_WITH_PE

static bfd_boolean
styp_to_sec_flags(bfd *abfd ATTRIBUTE_UNUSED, void *hdr, const char *name,
		  asection *section ATTRIBUTE_UNUSED, flagword *flags_ptr)
{
  struct internal_scnhdr *internal_s = (struct internal_scnhdr *)hdr;
  long styp_flags = internal_s->s_flags;
  flagword sec_flags = 0;

#ifdef STYP_BLOCK
  if (styp_flags & STYP_BLOCK) {
    sec_flags |= SEC_TIC54X_BLOCK;
  }
#endif /* STYP_BLOCK */

#ifdef STYP_CLINK
  if (styp_flags & STYP_CLINK) {
    sec_flags |= SEC_TIC54X_CLINK;
  }
#endif /* STYP_CLINK */

#ifdef STYP_NOLOAD
  if (styp_flags & STYP_NOLOAD) {
    sec_flags |= SEC_NEVER_LOAD;
  }
#endif /* STYP_NOLOAD */

  /* For 386 COFF, at least, an unloadable text or data section is
     actually a shared library section.  */
  if (styp_flags & STYP_TEXT)
    {
      if (sec_flags & SEC_NEVER_LOAD) {
        sec_flags |= (SEC_CODE | SEC_COFF_SHARED_LIBRARY);
      } else {
        sec_flags |= (SEC_CODE | SEC_LOAD | SEC_ALLOC);
      }
    }
  else if (styp_flags & STYP_DATA)
    {
      if (sec_flags & SEC_NEVER_LOAD) {
        sec_flags |= (SEC_DATA | SEC_COFF_SHARED_LIBRARY);
      } else {
        sec_flags |= (SEC_DATA | SEC_LOAD | SEC_ALLOC);
      }
    }
  else if (styp_flags & STYP_BSS)
    {
#ifdef BSS_NOLOAD_IS_SHARED_LIBRARY
      if (sec_flags & SEC_NEVER_LOAD) {
        sec_flags |= (SEC_ALLOC | SEC_COFF_SHARED_LIBRARY);
      } else
#endif /* BSS_NOLOAD_IS_SHARED_LIBRARY */
	sec_flags |= SEC_ALLOC;
    }
  else if (styp_flags & STYP_INFO)
    {
      /* We mark these as SEC_DEBUGGING, but only if COFF_PAGE_SIZE is
       * defined.  coff_compute_section_file_positions uses
       * COFF_PAGE_SIZE to ensure that the low order bits of the
       * section VMA and the file offset match. If we do NOT know
       * COFF_PAGE_SIZE, we cannot ensure the correct correspondence,
       * and demand page loading of the file will fail.  */
#if defined (COFF_PAGE_SIZE) && !defined (COFF_ALIGN_IN_S_FLAGS)
      sec_flags |= SEC_DEBUGGING;
#endif /* COFF_PAGE_SIZE && !COFF_ALIGN_IN_S_FLAGS */
    } else if (styp_flags & STYP_PAD) {
      sec_flags = 0;
    } else if (strcmp(name, _TEXT) == 0) {
      if (sec_flags & SEC_NEVER_LOAD) {
        sec_flags |= (SEC_CODE | SEC_COFF_SHARED_LIBRARY);
      } else {
        sec_flags |= (SEC_CODE | SEC_LOAD | SEC_ALLOC);
      }
    }
  else if (strcmp(name, _DATA) == 0)
    {
      if (sec_flags & SEC_NEVER_LOAD) {
        sec_flags |= (SEC_DATA | SEC_COFF_SHARED_LIBRARY);
      } else {
        sec_flags |= (SEC_DATA | SEC_LOAD | SEC_ALLOC);
      }
    }
  else if (strcmp(name, _BSS) == 0)
    {
#ifdef BSS_NOLOAD_IS_SHARED_LIBRARY
      if (sec_flags & SEC_NEVER_LOAD) {
        sec_flags |= (SEC_ALLOC | SEC_COFF_SHARED_LIBRARY);
      } else
#endif /* BSS_NOLOAD_IS_SHARED_LIBRARY */
	sec_flags |= SEC_ALLOC;
    }
  else if ((strncmp(name, DOT_DEBUG, (sizeof(DOT_DEBUG) - 1UL)) == 0)
#ifdef _COMMENT
	   || (strcmp(name, _COMMENT) == 0)
#endif /* _COMMENT */
#ifdef COFF_LONG_SECTION_NAMES
	   || (strncmp(name, GNU_LINKONCE_WI, (sizeof(GNU_LINKONCE_WI) - 1UL)) == 0)
#endif /* COFF_LONG_SECTION_NAMES */
	   || (strncmp(name, ".stab", 5UL) == 0))
    {
#ifdef COFF_PAGE_SIZE
      sec_flags |= SEC_DEBUGGING;
#endif /* COFF_PAGE_SIZE */
    }
#ifdef _LIB
  else if (strcmp (name, _LIB) == 0)
    ;
#endif /* _LIB */
#ifdef _LIT
  else if (strcmp (name, _LIT) == 0) {
    sec_flags = SEC_LOAD | SEC_ALLOC | SEC_READONLY;
  }
#endif /* _LIT */
  else {
    sec_flags |= SEC_ALLOC | SEC_LOAD;
  }

#ifdef STYP_LIT			/* A29k readonly text/data section type: */
  if ((styp_flags & STYP_LIT) == STYP_LIT) {
    sec_flags = (SEC_LOAD | SEC_ALLOC | SEC_READONLY);
  }
#endif /* STYP_LIT */

#ifdef STYP_OTHER_LOAD		/* Other loaded sections: */
  if (styp_flags & STYP_OTHER_LOAD) {
    sec_flags = (SEC_LOAD | SEC_ALLOC);
  }
#endif /* STYP_SDATA */

#if defined(COFF_LONG_SECTION_NAMES) && defined(COFF_SUPPORT_GNU_LINKONCE)
  /* As a GNU extension, if the name begins with .gnu.linkonce, we
     only link a single copy of the section.  This is used to support
     g++.  g++ will emit each template expansion in its own section.
     The symbols will be defined as weak, so that multiple definitions
     are permitted.  The GNU linker extension is to actually discard
     all but one of the sections.  */
  if (strncmp(name, ".gnu.linkonce", sizeof(".gnu.linkonce") - 1UL) == 0) {
    sec_flags |= (SEC_LINK_ONCE | SEC_LINK_DUPLICATES_DISCARD);
  }
#endif /* COFF_LONG_SECTION_NAMES && COFF_SUPPORT_GNU_LINKONCE */

  if (flags_ptr == NULL) {
    return FALSE;
  }

  *flags_ptr = sec_flags;
  return TRUE;
}

#else /* COFF_WITH_PE */

static flagword
handle_COMDAT(bfd *abfd, flagword sec_flags, void *hdr, const char *name,
	      asection *section)
{
  struct internal_scnhdr *internal_s = (struct internal_scnhdr *)hdr;
  bfd_byte *esymstart, *esym, *esymend;
  int seen_state = 0;
  char *target_name = NULL;

  sec_flags |= SEC_LINK_ONCE;

  /* Unfortunately, the PE format stores essential information in
     the symbol table, of all places. We need to extract that
     information now, so that objdump and the linker will know how
     to handle the section without worrying about the symbols. We
     cannot call slurp_symtab, because the linker does NOT want the
     swapped symbols.  */

  /* COMDAT sections are special.  The first symbol is the section
     symbol, which tells what kind of COMDAT section it is.  The
     second symbol is the "comdat symbol" - the one with the
     unique name.  GNU uses the section symbol for the unique
     name; MS uses ".text" for every comdat section.  Sigh.  - DJ */

  /* This is not mirrored in sec_to_styp_flags(), but there
     does NOT seem to be a need to, either, and it would at best be
     rather messy.  */

  if (! _bfd_coff_get_external_symbols(abfd))
    return sec_flags;

  esymstart = esym = (bfd_byte *)obj_coff_external_syms(abfd);
  esymend = (esym + obj_raw_syment_count(abfd) * bfd_coff_symesz(abfd));

  while (esym < esymend) {
      struct internal_syment isym;
      char buf[(SYMNMLEN + 1)];
      const char *symname;

      bfd_coff_swap_sym_in(abfd, esym, & isym);

      if (sizeof(internal_s->s_name) > SYMNMLEN) {
	  /* This case implies that the matching
	   * symbol name will be in the string table.  */
	  abort(); /*NOTREACHED*/
      }

      if (isym.n_scnum == section->target_index) {
	  /* According to the MSVC documentation, the first
	     TWO entries with the section # are both of
	     interest to us.  The first one is the "section
	     symbol" (section name). The second is the comdat
	     symbol name. Here, we have found the first
	     qualifying entry; we distinguish it from the
	     second with a state flag.

	     In the case of gas-generated (at least until that
	     is fixed) .o files, it is NOT necessarily the
	     second one. It may be some other later symbol.

	     Since gas also does NOT follow MS conventions and
	     emits the section similar to .text$<name>, where
	     <something> is the name we are looking for, we
	     distinguish the two as follows:

	     If the section name is simply a section name (no
	     $) we presume it is/was MS-generated, and look at
	     precisely the second symbol for the comdat name.
	     If the section name has a $, we assume it is/was
	     gas-generated, and look for <something> (whatever
	     follows the $) as the comdat symbol.  */

	  /* All 3 branches use this: */
	  symname = _bfd_coff_internal_syment_name(abfd, &isym, buf);

	  if (symname == NULL) {
	    abort();
	  }

	  switch (seen_state) {
	    case 0: {
		/* The first time we have seen the symbol: */
		union internal_auxent aux;

		/* If it is NOT the stuff we are/were expecting, die;
		   The MS documentation is vague, but it
		   appears that the second entry serves BOTH
		   as the comdat symbol and the defining
		   symbol record (either C_STAT or C_EXT,
		   possibly with an aux entry with debug
		   information if it is/was a function.) It
		   appears the only way to find the second one
		   is to count. (On Intel, they appear to be
		   adjacent, but on Alpha, they have been
		   found separated.)

		   Here, we think we have found the first one,
		   but there is some checking we can do to be
		   sure.  */

		if (!((isym.n_sclass == C_STAT)
		      && (isym.n_type == T_NULL)
		      && (isym.n_value == 0))) {
		    abort();
		}

		/* FIXME LATER: MSVC generates section names
		   like .text for comdats.  Gas generates
		   names like .text$foo__Fv (in the case of a
		   function).  See comment above for more.  */

		if (strcmp (name, symname) != 0)
		  _bfd_error_handler (_("%B: warning: COMDAT symbol '%s' does not match section name '%s'"),
				      abfd, symname, name);

		seen_state = 1;

		/* This is the section symbol.  */
		bfd_coff_swap_aux_in (abfd, (esym + bfd_coff_symesz (abfd)),
				      isym.n_type, isym.n_sclass,
				      0, isym.n_numaux, & aux);

		target_name = strchr (name, '$');
		if (target_name != NULL)
		  {
		    /* Gas mode.  */
		    seen_state = 2;
		    /* Skip the `$'.  */
		    target_name += 1;
		  }

		/* FIXME: Microsoft uses NODUPLICATES and
		   ASSOCIATIVE, but gnu uses ANY and
		   SAME_SIZE.  Unfortunately, gnu does NOT do
		   the comdat symbols right. So, until we can
		   fix it to do the right thing, we are
		   temporarily disabling comdats for the MS
		   types (they are/were used in DLLs and C++, but we
		   do NOT support *their* C++ libraries anyway
		   - DJ.  */

		/* Cygwin does not follow the MS style, and
		   uses ANY and SAME_SIZE where NODUPLICATES
		   and ASSOCIATIVE should be used.  For
		   Interix, we just do the right thing up
		   front.  */

		switch (aux.x_scn.x_comdat)
		  {
		  case IMAGE_COMDAT_SELECT_NODUPLICATES:
#ifdef STRICT_PE_FORMAT
		    sec_flags |= SEC_LINK_DUPLICATES_ONE_ONLY;
#else
		    sec_flags &= (flagword)(~SEC_LINK_ONCE);
#endif /* STRICT_PE_FORMAT */
		    break;

		  case IMAGE_COMDAT_SELECT_ANY:
		    sec_flags |= SEC_LINK_DUPLICATES_DISCARD;
		    break;

		  case IMAGE_COMDAT_SELECT_SAME_SIZE:
		    sec_flags |= SEC_LINK_DUPLICATES_SAME_SIZE;
		    break;

		  case IMAGE_COMDAT_SELECT_EXACT_MATCH:
		    /* Not yet fully implemented ??? */
		    sec_flags |= SEC_LINK_DUPLICATES_SAME_CONTENTS;
		    break;

		    /* debug$S gets this case; other
		       implications ??? */

		    /* There may be no symbol... we shall search
		       the whole table... Is this the right
		       place to play this game? Or should we do
		       it when reading it in.  */
		  case IMAGE_COMDAT_SELECT_ASSOCIATIVE:
#ifdef STRICT_PE_FORMAT
		    /* FIXME: This is not currently implemented: */
		    sec_flags |= SEC_LINK_DUPLICATES_DISCARD;
#else
		    sec_flags &= (flagword)(~SEC_LINK_ONCE);
#endif /* STRICT_PE_FORMAT */
		    break;

		  default:  /* 0 means "no symbol" */
		    /* debug$F gets this case; other
		       implications ??? */
		    sec_flags |= SEC_LINK_DUPLICATES_DISCARD;
		    break;
		  }
	      }
	      break;

	    case 2:
	      /* Gas mode: the first matching on partial name.  */

#ifndef TARGET_UNDERSCORE
# define TARGET_UNDERSCORE 0
#endif /* !TARGET_UNDERSCORE */
	      /* Is this the name we are/were looking for ?  */
	      if (strcmp(target_name,
			 (symname + (TARGET_UNDERSCORE ? 1 : 0))) != 0)
		{
		  /* Not the name we are/were looking for */
		  esym += ((isym.n_numaux + 1U) * bfd_coff_symesz(abfd));
		  continue;
		}
	      /* Fall through.  */
              ATTRIBUTE_FALLTHROUGH;
	    case 1:
	      /* MSVC mode: the lexically second symbol (or drop through
	       * from the above). */
	      {
		char *newname;
		bfd_size_type amt;

		/* This must the second symbol with the section #.
		 * It is the actual symbol name. Intel puts the two adjacent,
		 * but Alpha (at least) spreads them out: */
		amt = sizeof(struct coff_comdat_info);
		coff_section_data(abfd, section)->comdat
		  = (struct coff_comdat_info *)bfd_alloc(abfd, amt);
		if (coff_section_data(abfd, section)->comdat == NULL) {
		  abort();
		}

		coff_section_data(abfd, section)->comdat->symbol =
		  (long)((esym - esymstart)
                         / (ptrdiff_t)bfd_coff_symesz(abfd));

		amt = (strlen(symname) + 1UL);
		newname = (char *)bfd_alloc(abfd, amt);
		if (newname == NULL) {
		  abort();
		}

		strncpy(newname, symname, amt);
		coff_section_data(abfd, section)->comdat->name
		  = newname;
	      }

	      goto breakloop;
	    default:
	      break; /* (?) */
	  } /* end "switch (seen_state)" */
      }

      esym += ((isym.n_numaux + 1U) * bfd_coff_symesz(abfd));
  } /* end while-loop */

 breakloop:
  return sec_flags;
}


/* The PE version; see above for the general comments.

   Since to set the SEC_LINK_ONCE and associated flags, we have to
   look at the symbol table anyway, we return the symbol table index
   of the symbol being used as the COMDAT symbol.  This is admittedly
   ugly, but there is really nowhere else that we have access to the
   required information.  FIXME: Is the COMDAT symbol index used for
   any purpose other than objdump?  */

static bfd_boolean
styp_to_sec_flags(bfd *abfd, void *hdr, const char *name,
                  asection *section, flagword *flags_ptr)
{
  struct internal_scnhdr *internal_s = (struct internal_scnhdr *)hdr;
  long styp_flags = internal_s->s_flags;
  flagword sec_flags;
  bfd_boolean result = TRUE;

  /* Assume read only unless IMAGE_SCN_MEM_WRITE is specified.  */
  sec_flags = SEC_READONLY;

  /* Process each flag bit in styp_flags in turn.  */
  while (styp_flags)
    {
      long flag = (styp_flags & -styp_flags);
      const char *unhandled = (const char *)NULL;

      styp_flags &= ~flag;

      /* We infer from the distinct read/write/execute bits the settings
       * of some of the bfd flags; the actual values, should we need them,
       * are also in pei_section_data (abfd, section)->pe_flags.  */

      switch (flag) {
	case STYP_DSECT:
	  unhandled = "STYP_DSECT";
	  break;
	case STYP_GROUP:
	  unhandled = "STYP_GROUP";
	  break;
	case STYP_COPY:
	  unhandled = "STYP_COPY";
	  break;
	case STYP_OVER:
	  unhandled = "STYP_OVER";
	  break;
#ifdef SEC_NEVER_LOAD
	case STYP_NOLOAD:
	  sec_flags |= SEC_NEVER_LOAD;
	  break;
#endif /* SEC_NEVER_LOAD */
	case IMAGE_SCN_MEM_READ:
	  /* Ignored, assume it always to be true.  */
	  break;
	case IMAGE_SCN_TYPE_NO_PAD:
	  /* Skip.  */
	  break;
	case IMAGE_SCN_LNK_OTHER:
	  unhandled = "IMAGE_SCN_LNK_OTHER";
	  break;
	case IMAGE_SCN_MEM_NOT_CACHED:
	  unhandled = "IMAGE_SCN_MEM_NOT_CACHED";
	  break;
	case IMAGE_SCN_MEM_NOT_PAGED:
	  /* Generate a warning message rather using the 'unhandled'
	   * variable as this will allow some .sys files generate by
	   * other toolchains to be processed. See bugzilla issue 196.  */
	  _bfd_error_handler(_("%B: Warning: Ignoring section flag IMAGE_SCN_MEM_NOT_PAGED in section %s"),
			     abfd, name);
	  break;
	case IMAGE_SCN_MEM_EXECUTE:
	  sec_flags |= SEC_CODE;
	  break;
	case (long)IMAGE_SCN_MEM_WRITE:
	  sec_flags &= (flagword)~SEC_READONLY;
	  break;
	case IMAGE_SCN_MEM_DISCARDABLE:
	  /* The MS PE spec sets the DISCARDABLE flag on .reloc sections
	     but we do not want them to be labelled as debug section, since
	     then strip would remove them.  */
	  if (strncmp(name, ".reloc", (sizeof(".reloc") - 1UL)) != 0) {
	    sec_flags |= SEC_DEBUGGING;
	  }
	  break;
	case IMAGE_SCN_MEM_SHARED:
	  sec_flags |= SEC_COFF_SHARED;
	  break;
	case IMAGE_SCN_LNK_REMOVE:
	  sec_flags |= SEC_EXCLUDE;
	  break;
	case IMAGE_SCN_CNT_CODE:
	  sec_flags |= (SEC_CODE | SEC_ALLOC | SEC_LOAD);
	  break;
	case IMAGE_SCN_CNT_INITIALIZED_DATA:
	  sec_flags |= (SEC_DATA | SEC_ALLOC | SEC_LOAD);
	  break;
	case IMAGE_SCN_CNT_UNINITIALIZED_DATA:
	  sec_flags |= SEC_ALLOC;
	  break;
	case IMAGE_SCN_LNK_INFO:
	  /* We mark these as SEC_DEBUGGING, but only if COFF_PAGE_SIZE is
	   * defined.  coff_compute_section_file_positions uses
	   * COFF_PAGE_SIZE to ensure that the low order bits of the
	   * section VMA and the file offset match.  If we do NOT know
	   * COFF_PAGE_SIZE, we cannot ensure the correct correspondence,
	   * and demand page loading of the file will fail.  */
#ifdef COFF_PAGE_SIZE
	  sec_flags |= SEC_DEBUGGING;
#endif /* COFF_PAGE_SIZE */
	  break;
	case IMAGE_SCN_LNK_COMDAT:
	  /* COMDAT gets very special treatment: */
	  sec_flags = handle_COMDAT(abfd, sec_flags, hdr, name, section);
	  break;
	default:
	  /* Silently ignore for now: */
	  break;
	}

      /* If the section flag was not handled, report it here: */
      if (unhandled != NULL) {
	  (*_bfd_error_handler)
	    (_("%B (%s): Section flag %s (0x%x) ignored"),
	     abfd, name, unhandled, flag);
	  result = FALSE;
      }
    }

#if defined(COFF_LONG_SECTION_NAMES) && defined(COFF_SUPPORT_GNU_LINKONCE)
  /* As a GNU extension, if the name begins with .gnu.linkonce, we
     only link a single copy of the section. This is used to support
     g++.  g++ will emit each template expansion in its own section.
     The symbols will be defined as weak, so that multiple definitions
     are permitted. The GNU linker extension is to actually discard
     all but one of the sections.  */
  if (strncmp(name, ".gnu.linkonce", sizeof(".gnu.linkonce") - 1UL) == 0) {
    sec_flags |= (SEC_LINK_ONCE | SEC_LINK_DUPLICATES_DISCARD);
  }
#endif /* COFF_LONG_SECTION_NAMES && COFF_SUPPORT_GNU_LINKONCE */

  if (flags_ptr) {
    *flags_ptr = sec_flags;
  }

  return result;
}

#endif /* COFF_WITH_PE */

#define	get_index(symbol)	((symbol)->udata.i)

/*
INTERNAL_DEFINITION
	bfd_coff_backend_data

CODE_FRAGMENT

.{* COFF symbol classifications.  *}
.
.enum coff_symbol_classification
.{
.  {* Global symbol.  *}
.  COFF_SYMBOL_GLOBAL,
.  {* Common symbol.  *}
.  COFF_SYMBOL_COMMON,
.  {* Undefined symbol.  *}
.  COFF_SYMBOL_UNDEFINED,
.  {* Local symbol.  *}
.  COFF_SYMBOL_LOCAL,
.  {* PE section symbol.  *}
.  COFF_SYMBOL_PE_SECTION
.};
.
Special entry points for gdb to swap in coff symbol table parts:
.typedef struct
.{
.  void (*_bfd_coff_swap_aux_in)
.    (bfd *, void *, int, int, int, int, void *);
.
.  void (*_bfd_coff_swap_sym_in)
.    (bfd *, void *, void *);
.
.  void (*_bfd_coff_swap_lineno_in)
.    (bfd *, void *, void *);
.
.  unsigned int (*_bfd_coff_swap_aux_out)
.    (bfd *, void *, int, int, int, int, void *);
.
.  unsigned int (*_bfd_coff_swap_sym_out)
.    (bfd *, void *, void *);
.
.  unsigned int (*_bfd_coff_swap_lineno_out)
.    (bfd *, void *, void *);
.
.  unsigned int (*_bfd_coff_swap_reloc_out)
.    (bfd *, void *, void *);
.
.  unsigned int (*_bfd_coff_swap_filehdr_out)
.    (bfd *, void *, void *);
.
.  unsigned int (*_bfd_coff_swap_aouthdr_out)
.    (bfd *, void *, void *);
.
.  unsigned int (*_bfd_coff_swap_scnhdr_out)
.    (bfd *, void *, void *);
.
.  unsigned int _bfd_filhsz;
.  unsigned int _bfd_aoutsz;
.  unsigned int _bfd_scnhsz;
.  unsigned int _bfd_symesz;
.  unsigned int _bfd_auxesz;
.  unsigned int _bfd_relsz;
.  unsigned int _bfd_linesz;
.  unsigned int _bfd_filnmlen;
.  bfd_boolean _bfd_coff_long_filenames;
.  bfd_boolean _bfd_coff_long_section_names;
.  unsigned int _bfd_coff_default_section_alignment_power;
.  bfd_boolean _bfd_coff_force_symnames_in_strings;
.  unsigned int _bfd_coff_debug_string_prefix_length;
.
.  void (*_bfd_coff_swap_filehdr_in)
.    (bfd *, void *, void *);
.
.  void (*_bfd_coff_swap_aouthdr_in)
.    (bfd *, void *, void *);
.
.  void (*_bfd_coff_swap_scnhdr_in)
.    (bfd *, void *, void *);
.
.  void (*_bfd_coff_swap_reloc_in)
.    (bfd *abfd, void *, void *);
.
.  bfd_boolean (*_bfd_coff_bad_format_hook)
.    (bfd *, void *);
.
.  bfd_boolean (*_bfd_coff_set_arch_mach_hook)
.    (bfd *, void *);
.
.  void * (*_bfd_coff_mkobject_hook)
.    (bfd *, void *, void *);
.
.  bfd_boolean (*_bfd_styp_to_sec_flags_hook)
.    (bfd *, void *, const char *, asection *, flagword *);
.
.  void (*_bfd_set_alignment_hook)
.    (bfd *, asection *, void *);
.
.  bfd_boolean (*_bfd_coff_slurp_symbol_table)
.    (bfd *);
.
.  bfd_boolean (*_bfd_coff_symname_in_debug)
.    (bfd *, struct internal_syment *);
.
.  bfd_boolean (*_bfd_coff_pointerize_aux_hook)
.    (bfd *, combined_entry_type *, combined_entry_type *,
.	     unsigned int, combined_entry_type *);
.
.  bfd_boolean (*_bfd_coff_print_aux)
.    (bfd *, FILE *, combined_entry_type *, combined_entry_type *,
.	     combined_entry_type *, unsigned int);
.
.  void (*_bfd_coff_reloc16_extra_cases)
.    (bfd *, struct bfd_link_info *, struct bfd_link_order *, arelent *,
.	    bfd_byte *, unsigned int *, unsigned int *);
.
.  int (*_bfd_coff_reloc16_estimate)
.    (bfd *, asection *, arelent *, unsigned int,
.	     struct bfd_link_info *);
.
.  enum coff_symbol_classification (*_bfd_coff_classify_symbol)
.    (bfd *, struct internal_syment *);
.
.  bfd_boolean (*_bfd_coff_compute_section_file_positions)
.    (bfd *);
.
.  bfd_boolean (*_bfd_coff_start_final_link)
.    (bfd *, struct bfd_link_info *);
.
.  bfd_boolean (*_bfd_coff_relocate_section)
.    (bfd *, struct bfd_link_info *, bfd *, asection *, bfd_byte *,
.	     struct internal_reloc *, struct internal_syment *, asection **);
.
.  reloc_howto_type *(*_bfd_coff_rtype_to_howto)
.    (bfd *, asection *, struct internal_reloc *,
.	     struct coff_link_hash_entry *, struct internal_syment *,
.	     bfd_vma *);
.
.  bfd_boolean (*_bfd_coff_adjust_symndx)
.    (bfd *, struct bfd_link_info *, bfd *, asection *,
.	     struct internal_reloc *, bfd_boolean *);
.
.  bfd_boolean (*_bfd_coff_link_add_one_symbol)
.    (struct bfd_link_info *, bfd *, const char *, flagword,
.	     asection *, bfd_vma, const char *, bfd_boolean, bfd_boolean,
.	     struct bfd_link_hash_entry **);
.
.  bfd_boolean (*_bfd_coff_link_output_has_begun)
.    (bfd *, struct coff_final_link_info *);
.
.  bfd_boolean (*_bfd_coff_final_link_postscript)
.    (bfd *, struct coff_final_link_info *);
.
.} bfd_coff_backend_data;
.
.#define coff_backend_info(abfd) \
.  ((bfd_coff_backend_data *) (abfd)->xvec->backend_data)
.
.#define bfd_coff_swap_aux_in(a,e,t,c,ind,num,i) \
.  ((coff_backend_info (a)->_bfd_coff_swap_aux_in) (a,e,t,c,ind,num,i))
.
.#define bfd_coff_swap_sym_in(a,e,i) \
.  ((coff_backend_info (a)->_bfd_coff_swap_sym_in) (a,e,i))
.
.#define bfd_coff_swap_lineno_in(a,e,i) \
.  ((coff_backend_info ( a)->_bfd_coff_swap_lineno_in) (a,e,i))
.
.#define bfd_coff_swap_reloc_out(abfd, i, o) \
.  ((coff_backend_info (abfd)->_bfd_coff_swap_reloc_out) (abfd, i, o))
.
.#define bfd_coff_swap_lineno_out(abfd, i, o) \
.  ((coff_backend_info (abfd)->_bfd_coff_swap_lineno_out) (abfd, i, o))
.
.#define bfd_coff_swap_aux_out(a,i,t,c,ind,num,o) \
.  ((coff_backend_info (a)->_bfd_coff_swap_aux_out) (a,i,t,c,ind,num,o))
.
.#define bfd_coff_swap_sym_out(abfd, i,o) \
.  ((coff_backend_info (abfd)->_bfd_coff_swap_sym_out) (abfd, i, o))
.
.#define bfd_coff_swap_scnhdr_out(abfd, i,o) \
.  ((coff_backend_info (abfd)->_bfd_coff_swap_scnhdr_out) (abfd, i, o))
.
.#define bfd_coff_swap_filehdr_out(abfd, i,o) \
.  ((coff_backend_info (abfd)->_bfd_coff_swap_filehdr_out) (abfd, i, o))
.
.#define bfd_coff_swap_aouthdr_out(abfd, i,o) \
.  ((coff_backend_info (abfd)->_bfd_coff_swap_aouthdr_out) (abfd, i, o))
.
.#define bfd_coff_filhsz(abfd) (coff_backend_info (abfd)->_bfd_filhsz)
.#define bfd_coff_aoutsz(abfd) (coff_backend_info (abfd)->_bfd_aoutsz)
.#define bfd_coff_scnhsz(abfd) (coff_backend_info (abfd)->_bfd_scnhsz)
.#define bfd_coff_symesz(abfd) (coff_backend_info (abfd)->_bfd_symesz)
.#define bfd_coff_auxesz(abfd) (coff_backend_info (abfd)->_bfd_auxesz)
.#define bfd_coff_relsz(abfd)  (coff_backend_info (abfd)->_bfd_relsz)
.#define bfd_coff_linesz(abfd) (coff_backend_info (abfd)->_bfd_linesz)
.#define bfd_coff_filnmlen(abfd) (coff_backend_info (abfd)->_bfd_filnmlen)
.#define bfd_coff_long_filenames(abfd) \
.  (coff_backend_info (abfd)->_bfd_coff_long_filenames)
.#define bfd_coff_long_section_names(abfd) \
.  (coff_backend_info (abfd)->_bfd_coff_long_section_names)
.#define bfd_coff_default_section_alignment_power(abfd) \
.  (coff_backend_info (abfd)->_bfd_coff_default_section_alignment_power)
.#define bfd_coff_swap_filehdr_in(abfd, i,o) \
.  ((coff_backend_info (abfd)->_bfd_coff_swap_filehdr_in) (abfd, i, o))
.
.#define bfd_coff_swap_aouthdr_in(abfd, i,o) \
.  ((coff_backend_info (abfd)->_bfd_coff_swap_aouthdr_in) (abfd, i, o))
.
.#define bfd_coff_swap_scnhdr_in(abfd, i,o) \
.  ((coff_backend_info (abfd)->_bfd_coff_swap_scnhdr_in) (abfd, i, o))
.
.#define bfd_coff_swap_reloc_in(abfd, i, o) \
.  ((coff_backend_info (abfd)->_bfd_coff_swap_reloc_in) (abfd, i, o))
.
.#define bfd_coff_bad_format_hook(abfd, filehdr) \
.  ((coff_backend_info (abfd)->_bfd_coff_bad_format_hook) (abfd, filehdr))
.
.#define bfd_coff_set_arch_mach_hook(abfd, filehdr)\
.  ((coff_backend_info (abfd)->_bfd_coff_set_arch_mach_hook) (abfd, filehdr))
.#define bfd_coff_mkobject_hook(abfd, filehdr, aouthdr)\
.  ((coff_backend_info (abfd)->_bfd_coff_mkobject_hook)\
.   (abfd, filehdr, aouthdr))
.
.#define bfd_coff_styp_to_sec_flags_hook(abfd, scnhdr, name, section, flags_ptr)\
.  ((coff_backend_info (abfd)->_bfd_styp_to_sec_flags_hook)\
.   (abfd, scnhdr, name, section, flags_ptr))
.
.#define bfd_coff_set_alignment_hook(abfd, sec, scnhdr)\
.  ((coff_backend_info (abfd)->_bfd_set_alignment_hook) (abfd, sec, scnhdr))
.
.#define bfd_coff_slurp_symbol_table(abfd)\
.  ((coff_backend_info (abfd)->_bfd_coff_slurp_symbol_table) (abfd))
.
.#define bfd_coff_symname_in_debug(abfd, sym)\
.  ((coff_backend_info (abfd)->_bfd_coff_symname_in_debug) (abfd, sym))
.
.#define bfd_coff_force_symnames_in_strings(abfd)\
.  (coff_backend_info (abfd)->_bfd_coff_force_symnames_in_strings)
.
.#define bfd_coff_debug_string_prefix_length(abfd)\
.  (coff_backend_info (abfd)->_bfd_coff_debug_string_prefix_length)
.
.#define bfd_coff_print_aux(abfd, file, base, symbol, aux, indaux)\
.  ((coff_backend_info (abfd)->_bfd_coff_print_aux)\
.   (abfd, file, base, symbol, aux, indaux))
.
.#define bfd_coff_reloc16_extra_cases(abfd, link_info, link_order,\
.                                     reloc, data, src_ptr, dst_ptr)\
.  ((coff_backend_info (abfd)->_bfd_coff_reloc16_extra_cases)\
.   (abfd, link_info, link_order, reloc, data, src_ptr, dst_ptr))
.
.#define bfd_coff_reloc16_estimate(abfd, section, reloc, shrink, link_info)\
.  ((coff_backend_info (abfd)->_bfd_coff_reloc16_estimate)\
.   (abfd, section, reloc, shrink, link_info))
.
.#define bfd_coff_classify_symbol(abfd, sym)\
.  ((coff_backend_info (abfd)->_bfd_coff_classify_symbol)\
.   (abfd, sym))
.
.#define bfd_coff_compute_section_file_positions(abfd)\
.  ((coff_backend_info (abfd)->_bfd_coff_compute_section_file_positions)\
.   (abfd))
.
.#define bfd_coff_start_final_link(obfd, info)\
.  ((coff_backend_info (obfd)->_bfd_coff_start_final_link)\
.   (obfd, info))
.#define bfd_coff_relocate_section(obfd,info,ibfd,o,con,rel,isyms,secs)\
.  ((coff_backend_info (ibfd)->_bfd_coff_relocate_section)\
.   (obfd, info, ibfd, o, con, rel, isyms, secs))
.#define bfd_coff_rtype_to_howto(abfd, sec, rel, h, sym, addendp)\
.  ((coff_backend_info (abfd)->_bfd_coff_rtype_to_howto)\
.   (abfd, sec, rel, h, sym, addendp))
.#define bfd_coff_adjust_symndx(obfd, info, ibfd, sec, rel, adjustedp)\
.  ((coff_backend_info (abfd)->_bfd_coff_adjust_symndx)\
.   (obfd, info, ibfd, sec, rel, adjustedp))
.#define bfd_coff_link_add_one_symbol(info, abfd, name, flags, section,\
.                                     value, string, cp, coll, hashp)\
.  ((coff_backend_info (abfd)->_bfd_coff_link_add_one_symbol)\
.   (info, abfd, name, flags, section, value, string, cp, coll, hashp))
.
.#define bfd_coff_link_output_has_begun(a,p) \
.  ((coff_backend_info (a)->_bfd_coff_link_output_has_begun) (a, p))
.#define bfd_coff_final_link_postscript(a,p) \
.  ((coff_backend_info (a)->_bfd_coff_final_link_postscript) (a, p))
.
*/

/* See whether the magic number matches: */
static bfd_boolean
coff_bad_format_hook(bfd *abfd, void *filehdr)
{
  struct internal_filehdr *internal_f = (struct internal_filehdr *)filehdr;

  if (BADMAG(*internal_f)) {
    return FALSE;
  }

  /* If the optional header is NULL or not the correct size then
     quit; the only difference I can see between m88k dgux headers (MC88DMAGIC)
     and Intel 960 readwrite headers (I960WRMAGIC) is that the
     optional header is of a different size.

     But the mips keeps extra stuff in its opthdr, so do NOT check
     when doing that.  */

#if defined(M88) || defined(I960)
  if ((internal_f->f_opthdr != 0)
      && (bfd_coff_aoutsz(abfd) != internal_f->f_opthdr)) {
    return FALSE;
  }
#else
  if (abfd == (bfd *)NULL) {
    ; /* ??? */
  }
#endif /* M88 || I960 */

  return TRUE;
}

#ifdef TICOFF
static bfd_boolean
ticoff0_bad_format_hook(bfd *abfd ATTRIBUTE_UNUSED, void *filehdr)
{
  struct internal_filehdr *internal_f = (struct internal_filehdr *)filehdr;

  if (COFF0_BADMAG(*internal_f)) {
    return FALSE;
  }

  return TRUE;
}
#endif /* TICOFF */

#ifdef TICOFF
static bfd_boolean
ticoff1_bad_format_hook(bfd *abfd ATTRIBUTE_UNUSED, void *filehdr)
{
  struct internal_filehdr *internal_f = (struct internal_filehdr *)filehdr;

  if (COFF1_BADMAG(*internal_f)) {
    return FALSE;
  }

  return TRUE;
}
#endif /* TICOFF */

/* Check whether this section uses an alignment other than the
   default.  */

static void
coff_set_custom_section_alignment(bfd *abfd ATTRIBUTE_UNUSED,
				  asection *section,
				  const struct coff_section_alignment_entry *alignment_table,
				  const unsigned int table_size)
{
  const unsigned int default_alignment = COFF_DEFAULT_SECTION_ALIGNMENT_POWER;
  unsigned int i;

  for ((i = 0); (i < table_size); ++i)
    {
      const char *secname = bfd_get_section_name(abfd, section);

      if ((alignment_table[i].comparison_length == (unsigned int)-1)
	  ? (strcmp(alignment_table[i].name, secname) == 0)
	  : (strncmp(alignment_table[i].name, secname,
		     (size_t)alignment_table[i].comparison_length) == 0))
	break;
    }
  if (i >= table_size)
    return;

  if (alignment_table[i].default_alignment_min != COFF_ALIGNMENT_FIELD_EMPTY
      && (default_alignment < alignment_table[i].default_alignment_min))
    return;

  if (alignment_table[i].default_alignment_max != COFF_ALIGNMENT_FIELD_EMPTY
#if (COFF_DEFAULT_SECTION_ALIGNMENT_POWER != 0)
      && (default_alignment > alignment_table[i].default_alignment_max)
#endif /* COFF_DEFAULT_SECTION_ALIGNMENT_POWER != 0 */
      )
    return;

  section->alignment_power = alignment_table[i].alignment_power;
}

/* Custom section alignment records: */
static const struct coff_section_alignment_entry
coff_section_alignment_table[] =
{
#ifdef COFF_SECTION_ALIGNMENT_ENTRIES
  COFF_SECTION_ALIGNMENT_ENTRIES,
#endif /* COFF_SECTION_ALIGNMENT_ENTRIES */
  /* There must not be any gaps between .stabstr sections.  */
  { COFF_SECTION_NAME_PARTIAL_MATCH(".stabstr"),
    1, COFF_ALIGNMENT_FIELD_EMPTY, 0 },
  /* The .stab section must be aligned to 2**2 at most, to avoid gaps.  */
  { COFF_SECTION_NAME_PARTIAL_MATCH(".stab"),
    3, COFF_ALIGNMENT_FIELD_EMPTY, 2 },
  /* Similarly for the .ctors and .dtors sections.  */
  { COFF_SECTION_NAME_EXACT_MATCH(".ctors"),
    3, COFF_ALIGNMENT_FIELD_EMPTY, 2 },
  { COFF_SECTION_NAME_EXACT_MATCH(".dtors"),
    3, COFF_ALIGNMENT_FIELD_EMPTY, 2 }
};

static const unsigned int coff_section_alignment_table_size =
  (sizeof(coff_section_alignment_table) / sizeof(coff_section_alignment_table[0]));

/* Initialize a section structure with information peculiar to this
 * particular implementation of COFF: */
static ATTRIBUTE_USED bfd_boolean
coff_new_section_hook(bfd *abfd, asection *section)
{
  combined_entry_type *native;
  bfd_size_type amt;

  section->alignment_power = COFF_DEFAULT_SECTION_ALIGNMENT_POWER;

#ifdef RS6000COFF_C
  if ((bfd_xcoff_text_align_power(abfd) != 0)
      && (strcmp(bfd_get_section_name(abfd, section), ".text") == 0)) {
      section->alignment_power = (unsigned int)bfd_xcoff_text_align_power(abfd);
  }
  if ((bfd_xcoff_data_align_power(abfd) != 0)
      && (strcmp(bfd_get_section_name(abfd, section), ".data") == 0)) {
      section->alignment_power = (unsigned int)bfd_xcoff_data_align_power(abfd);
  }
#endif /* RS6000COFF_C */

  /* Allocate aux records for section symbols, to store size and
   * related info.
   *
   * @@ The 10 is a guess at a plausible maximum number of aux entries
   * (but should NOT be a constant).  */
  amt = (sizeof(combined_entry_type) * 10);
  native = (combined_entry_type *)bfd_zalloc(abfd, amt);
  if (native == NULL) {
      return FALSE;
  }

  /* We do NOT need to set up n_name, n_value, or n_scnum in the native
   * symbol info, since they will be overridden by the BFD symbol anyhow.
   * However, we do need to set the type and storage class, in case this
   * symbol winds up getting written out.  The value 0 for n_numaux is
   * already correct: */
  native->u.syment.n_type = T_NULL;
  native->u.syment.n_sclass = C_STAT;

  coffsymbol(section->symbol)->native = native;

  coff_set_custom_section_alignment(abfd, section, coff_section_alignment_table,
				    coff_section_alignment_table_size);

  return TRUE;
}

#ifdef COFF_ALIGN_IN_SECTION_HEADER
/* Set the alignment of a BFD section: */
static void
coff_set_alignment_hook(bfd *abfd ATTRIBUTE_UNUSED, asection *section,
			void *scnhdr)
{
  struct internal_scnhdr *hdr = (struct internal_scnhdr *)scnhdr;
  unsigned int i;

#ifdef I960
  /* Extract ALIGN from 2**ALIGN stored in section header: */
  for (i = 0; i < 32; i++) {
    if ((1 << i) >= hdr->s_align) {
      break;
    }
  }
#endif /* I960 */
#ifdef TIC80COFF
  /* TI tools puts the alignment power in bits 8-11: */
  i = ((hdr->s_flags >> 8) & 0xF);
#endif /* TIC80COFF */
#ifdef COFF_DECODE_ALIGNMENT
  i = COFF_DECODE_ALIGNMENT(hdr->s_flags);
#endif /* COFF_DECODE_ALIGNMENT */
  section->alignment_power = i;

#ifdef coff_set_section_load_page
  coff_set_section_load_page(section, hdr->s_page);
#endif /* coff_set_section_load_page */
}

#else /* ! COFF_ALIGN_IN_SECTION_HEADER */
#ifdef COFF_WITH_PE

/* A couple of macros to help setting the alignment power field.  */
#define ALIGN_SET(field, x, y) \
  if (((field) & IMAGE_SCN_ALIGN_64BYTES) == x)\
    {\
      section->alignment_power = y;\
    }

#define ELIFALIGN_SET(field, x, y) \
  else if (((field) & IMAGE_SCN_ALIGN_64BYTES) == x) \
    {\
      section->alignment_power = y;\
    }

static void
coff_set_alignment_hook(bfd *abfd, asection *section, void *scnhdr)
{
  struct internal_scnhdr *hdr = (struct internal_scnhdr *)scnhdr;
  bfd_size_type amt;

  ALIGN_SET(hdr->s_flags, IMAGE_SCN_ALIGN_64BYTES, 6)
  ELIFALIGN_SET(hdr->s_flags, IMAGE_SCN_ALIGN_32BYTES, 5)
  ELIFALIGN_SET(hdr->s_flags, IMAGE_SCN_ALIGN_16BYTES, 4)
  ELIFALIGN_SET(hdr->s_flags, IMAGE_SCN_ALIGN_8BYTES, 3)
  ELIFALIGN_SET(hdr->s_flags, IMAGE_SCN_ALIGN_4BYTES, 2)
  ELIFALIGN_SET(hdr->s_flags, IMAGE_SCN_ALIGN_2BYTES, 1)
  ELIFALIGN_SET(hdr->s_flags, IMAGE_SCN_ALIGN_1BYTES, 0)

  /* In a PE image file, the s_paddr field holds the virtual size of a
     section, while the s_size field holds the raw size.  We also keep
     the original section flag value, since not every bit can be
     mapped onto a generic BFD section bit.  */
  if (coff_section_data(abfd, section) == NULL)
    {
      amt = sizeof(struct coff_section_tdata);
      section->used_by_bfd = bfd_zalloc(abfd, amt);
      if (section->used_by_bfd == NULL)
	/* FIXME: Return error: */
	abort();
    }

  if (pei_section_data(abfd, section) == NULL)
    {
      amt = sizeof(struct pei_section_tdata);
      coff_section_data(abfd, section)->tdata = bfd_zalloc(abfd, amt);
      if (coff_section_data(abfd, section)->tdata == NULL)
	/* FIXME: Return error: */
	abort();
    }
  pei_section_data(abfd, section)->virt_size = hdr->s_paddr;
  pei_section_data(abfd, section)->pe_flags = hdr->s_flags;

  section->lma = hdr->s_vaddr;

  /* Check for extended relocs: */
  if (hdr->s_flags & IMAGE_SCN_LNK_NRELOC_OVFL)
    {
      struct external_reloc dst;
      struct internal_reloc n;
      file_ptr oldpos = bfd_tell(abfd);
      bfd_size_type relsz = bfd_coff_relsz(abfd);

      bfd_seek(abfd, (file_ptr)hdr->s_relptr, 0);
      if (bfd_bread(& dst, relsz, abfd) != relsz)
	return;

      coff_swap_reloc_in(abfd, &dst, &n);
      bfd_seek(abfd, oldpos, 0);
      hdr->s_nreloc = (unsigned long)(n.r_vaddr - 1UL);
      section->reloc_count = (unsigned)hdr->s_nreloc;
      section->rel_filepos += (file_ptr)relsz;
    }
  else if (hdr->s_nreloc == 0xffff)
    (*_bfd_error_handler)
      ("%s: warning: claims to have 0xffff relocs, without overflow",
       bfd_get_filename(abfd));
}
#undef ALIGN_SET
#undef ELIFALIGN_SET

#else /* ! COFF_WITH_PE */
# ifdef RS6000COFF_C
#  ifndef STYP_OVRFLO
/* XCOFF handles line number or relocation overflow by creating
 another section header with STYP_OVRFLO set.  */
#   define STYP_OVRFLO 0x8000
#  endif /* !STYP_OVRFLO */
/* We grossly abuse this function to handle XCOFF overflow headers.
   When we see one, we correct the reloc and line number counts in the
   real header, and remove the section we just created.  */
static void
coff_set_alignment_hook(bfd *abfd, asection *section, void *scnhdr)
{
  struct internal_scnhdr *hdr = (struct internal_scnhdr *)scnhdr;
  asection *real_sec;

  if ((hdr->s_flags & STYP_OVRFLO) == 0)
    return;

  real_sec = coff_section_from_bfd_index(abfd, (int)hdr->s_nreloc);
  if (real_sec == NULL)
    return;

  real_sec->reloc_count = (unsigned)hdr->s_paddr;
  real_sec->lineno_count = (unsigned int)hdr->s_vaddr;

  if (!bfd_section_removed_from_list(abfd, section))
    {
      bfd_section_list_remove(abfd, section);
      --abfd->section_count;
    }
}

# else /* !RS6000COFF_C: */
#  define coff_set_alignment_hook \
    ((void (*)(bfd *, asection *, void *))bfd_void)
# endif /* ! RS6000COFF_C */
#endif /* ! COFF_WITH_PE */
#endif /* ! COFF_ALIGN_IN_SECTION_HEADER */

#ifndef coff_mkobject
static bfd_boolean
coff_mkobject(bfd *abfd)
{
  coff_data_type *coff;
  bfd_size_type amt = sizeof(coff_data_type);

  abfd->tdata.coff_obj_data = (coff_data_type *)bfd_zalloc(abfd, amt);
  if (abfd->tdata.coff_obj_data == NULL) {
      return FALSE;
  }
  coff = coff_data(abfd);
  coff->symbols = NULL;
  coff->conversion_table = NULL;
  coff->raw_syments = NULL;
  coff->relocbase = 0;
  coff->local_toc_sym_map = 0;

# if 0
  make_abs_section(abfd);
# endif /* 0 */

  return TRUE;
}
#endif /* !coff_mkobject */

/* Create the COFF backend specific information: */
#ifndef coff_mkobject_hook
static void *
coff_mkobject_hook(bfd *abfd, void *filehdr, void *aouthdr)
{
  struct internal_filehdr *internal_f = (struct internal_filehdr *)filehdr;
  coff_data_type *coff;

  if (!coff_mkobject(abfd)) {
    return NULL;
  }

  coff = coff_data(abfd);

  coff->sym_filepos = (file_ptr)internal_f->f_symptr;

  /* These members communicate important constants about the symbol
     table to GDB's symbol-reading code.  These `constants'
     unfortunately vary among coff implementations...  */
  coff->local_n_btmask = N_BTMASK;
  coff->local_n_btshft = N_BTSHFT;
  coff->local_n_tmask = N_TMASK;
  coff->local_n_tshift = N_TSHIFT;
  coff->local_symesz = bfd_coff_symesz(abfd);
  coff->local_auxesz = bfd_coff_auxesz(abfd);
  coff->local_linesz = bfd_coff_linesz(abfd);

  coff->timestamp = internal_f->f_timdat;

  obj_conv_table_size(abfd) = (int)internal_f->f_nsyms;
  obj_raw_syment_count(abfd) = (unsigned long)obj_conv_table_size(abfd);

#ifdef RS6000COFF_C
  if ((internal_f->f_flags & F_SHROBJ) != 0) {
    abfd->flags |= DYNAMIC;
  }
  if ((aouthdr != NULL) && (internal_f->f_opthdr >= bfd_coff_aoutsz(abfd)))
    {
      struct internal_aouthdr *internal_a =
	(struct internal_aouthdr *)aouthdr;
      struct xcoff_tdata *xcoff;

      xcoff = xcoff_data(abfd);
# ifdef U803XTOCMAGIC
      xcoff->xcoff64 = internal_f->f_magic == U803XTOCMAGIC;
# else
      xcoff->xcoff64 = 0;
# endif /* U803XTOCMAGIC */
      xcoff->full_aouthdr = TRUE;
      xcoff->toc = internal_a->o_toc;
      xcoff->sntoc = internal_a->o_sntoc;
      xcoff->snentry = internal_a->o_snentry;
      /* Accessor macros for tdata. */
# ifndef bfd_xcoff_text_align_power
#  define bfd_xcoff_text_align_power(a) ((xcoff_data (a)->text_align_power))
# endif /* !bfd_xcoff_text_align_power */
# ifndef bfd_xcoff_data_align_power
#  define bfd_xcoff_data_align_power(a) ((xcoff_data (a)->data_align_power))
# endif /* !bfd_xcoff_data_align_power */
      bfd_xcoff_text_align_power(abfd) = internal_a->o_algntext;
      bfd_xcoff_data_align_power(abfd) = internal_a->o_algndata;
      xcoff->modtype = internal_a->o_modtype;
      xcoff->cputype = internal_a->o_cputype;
      xcoff->maxdata = internal_a->o_maxdata;
      xcoff->maxstack = internal_a->o_maxstack;
    }
#else
  (void)aouthdr;
#endif /* RS6000COFF_C */

#ifdef ARM
  /* Set the flags field from the COFF header read in: */
  if (!_bfd_coff_arm_set_private_flags(abfd, internal_f->f_flags)) {
    coff->flags = 0;
  }
#endif /* ARM */

#ifdef COFF_WITH_PE
  /* FIXME: I am not sure this is ever executed, since "peicode.h"
   * defines coff_mkobject_hook: */
  if ((internal_f->f_flags & IMAGE_FILE_DEBUG_STRIPPED) == 0) {
    abfd->flags |= HAS_DEBUG;
  }
#endif /* COFF_WITH_PE */

  return coff;
}
#endif /* !coff_mkobject_hook (?) */

/* Determine the machine architecture and type.  FIXME: This is target
   dependent because the magic numbers are defined in the target
   dependent header files.  But there is no particular need for this.
   If the magic numbers were moved to a separate file, this function
   would be target independent and would also be much more successful
   at linking together COFF files for different architectures.  */

static bfd_boolean
coff_set_arch_mach_hook(bfd *abfd, void *filehdr)
{
  unsigned long machine;
  enum bfd_architecture arch;
  struct internal_filehdr *internal_f = (struct internal_filehdr *)filehdr;

  /* Zero selects the default machine for an arch: */
  machine = 0;
  switch (internal_f->f_magic)
    {
#ifdef OR32_MAGIC_BIG
    case OR32_MAGIC_BIG:
    case OR32_MAGIC_LITTLE:
      arch = bfd_arch_or32;
      break;
#endif /* OR32_MAGIC_BIG */
#ifdef PPCMAGIC
    case PPCMAGIC:
      arch = bfd_arch_powerpc;
      break;
#endif /* PPCMAGIC */
#ifdef I386MAGIC
    case I386MAGIC:
    case I386PTXMAGIC:
    case I386AIXMAGIC:		/* Danbury PS/2 AIX C Compiler */
    case LYNXCOFFMAGIC:	/* shadows the m68k Lynx number below, sigh */
      arch = bfd_arch_i386;
      break;
#endif /* I386MAGIC */
#ifdef IA64MAGIC
    case IA64MAGIC:
      arch = bfd_arch_ia64;
      break;
#endif /* IA64MAGIC */
#ifdef A29K_MAGIC_BIG
    case A29K_MAGIC_BIG:
    case A29K_MAGIC_LITTLE:
      arch = bfd_arch_a29k;
      break;
#endif /* A29K_MAGIC_BIG */
#ifdef ARMMAGIC
    case ARMMAGIC:
    case ARMPEMAGIC:
    case THUMBPEMAGIC:
      arch = bfd_arch_arm;
      machine = bfd_arm_get_mach_from_notes(abfd, ARM_NOTE_SECTION);
      if (machine == bfd_mach_arm_unknown)
	{
	  switch (internal_f->f_flags & F_ARM_ARCHITECTURE_MASK)
	    {
	    case F_ARM_2:  machine = bfd_mach_arm_2;  break;
	    case F_ARM_2a: machine = bfd_mach_arm_2a; break;
	    case F_ARM_3:  machine = bfd_mach_arm_3;  break;
	    default:
	    case F_ARM_3M: machine = bfd_mach_arm_3M; break;
	    case F_ARM_4:  machine = bfd_mach_arm_4;  break;
	    case F_ARM_4T: machine = bfd_mach_arm_4T; break;
              /* The COFF header does not have enough bits available
               * to cover all the different ARM architectures.  So we
               * interpret F_ARM_5, the highest flag value to mean
               * "the highest ARM architecture known to BFD" which is
               * currently the XScale.  */
	    case F_ARM_5:  machine = bfd_mach_arm_XScale;  break;
	    }
	}
      break;
#endif /* ARM */
#ifdef MC68MAGIC
    case MC68MAGIC:
    case M68MAGIC:
# ifdef MC68KBCSMAGIC
    case MC68KBCSMAGIC:
# endif /* MC68KBCSMAGIC */
# ifdef APOLLOM68KMAGIC
    case APOLLOM68KMAGIC:
# endif /* APOLLOM68KMAGIC */
# ifdef LYNXCOFFMAGIC
    case LYNXCOFFMAGIC:
# endif /* LYNXCOFFMAGIC */
      arch = bfd_arch_m68k;
      machine = bfd_mach_m68020;
      break;
#endif /* MC68MAGIC */
#ifdef MAXQ20MAGIC
    case MAXQ20MAGIC:
      arch = bfd_arch_maxq;
      switch (internal_f->f_flags & F_MACHMASK)
	{
        case F_MAXQ10:
          machine = bfd_mach_maxq10;
          break;
        case F_MAXQ20:
          machine = bfd_mach_maxq20;
          break;
        default:
          return FALSE;
	}
      break;
#endif /* MAXQ20MAGIC */
#ifdef MC88MAGIC
    case MC88MAGIC:
    case MC88DMAGIC:
    case MC88OMAGIC:
      arch = bfd_arch_m88k;
      machine = 88100;
      break;
#endif /* MC88MAGIC */
#ifdef Z8KMAGIC
    case Z8KMAGIC:
      arch = bfd_arch_z8k;
      switch (internal_f->f_flags & F_MACHMASK)
	{
	case F_Z8001:
	  machine = bfd_mach_z8001;
	  break;
	case F_Z8002:
	  machine = bfd_mach_z8002;
	  break;
	default:
	  return FALSE;
	}
      break;
#endif /* Z8KMAGIC */
#ifdef I860
    case I860MAGIC:
      arch = bfd_arch_i860;
      break;
#endif /* I860 */
#ifdef I960
# ifdef I960ROMAGIC
    case I960ROMAGIC:
    case I960RWMAGIC:
      arch = bfd_arch_i960;
      switch (F_I960TYPE & internal_f->f_flags)
	{
	default:
	case F_I960CORE:
	  machine = bfd_mach_i960_core;
	  break;
	case F_I960KB:
	  machine = bfd_mach_i960_kb_sb;
	  break;
	case F_I960MC:
	  machine = bfd_mach_i960_mc;
	  break;
	case F_I960XA:
	  machine = bfd_mach_i960_xa;
	  break;
	case F_I960CA:
	  machine = bfd_mach_i960_ca;
	  break;
	case F_I960KA:
	  machine = bfd_mach_i960_ka_sa;
	  break;
	case F_I960JX:
	  machine = bfd_mach_i960_jx;
	  break;
	case F_I960HX:
	  machine = bfd_mach_i960_hx;
	  break;
	}
      break;
# endif /* I960ROMAGIC */
#endif /* I960 */

#ifdef RS6000COFF_C
# ifdef XCOFF64
    case U64_TOCMAGIC:
    case U803XTOCMAGIC:
# else
    case U802ROMAGIC:
    case U802WRMAGIC:
    case U802TOCMAGIC:
# endif /* XCOFF64 */
      {
	int cputype;

	if (xcoff_data(abfd)->cputype != -1) {
          cputype = (xcoff_data(abfd)->cputype & 0xff);
	} else {
          /* We did not get a value from the a.out header.  If the file
           * has not been stripped, then we may be able to get the
           * architecture information from the 1st symbol, if it is a
           * .file symbo:  */
          if (obj_raw_syment_count(abfd) == 0) {
            cputype = 0;
          } else {
            bfd_byte *buf;
            struct internal_syment sym;
            bfd_size_type amt = bfd_coff_symesz(abfd);

            buf = (bfd_byte *)bfd_malloc(amt);
            if ((bfd_seek(abfd, obj_sym_filepos(abfd), SEEK_SET) != 0)
                || (bfd_bread(buf, amt, abfd) != amt))
              {
                free(buf);
                return FALSE;
              }
            bfd_coff_swap_sym_in(abfd, buf, & sym);
            if (sym.n_sclass == C_FILE)
              cputype = (sym.n_type & 0xff);
            else
              cputype = 0;
            free(buf);
          }
        }

	/* FIXME: We do NOT handle all cases here: */
	switch (cputype)
	  {
	  default:
	  case 0:
	    arch = bfd_xcoff_architecture(abfd);
	    machine = (unsigned long)bfd_xcoff_machine(abfd);
	    break;

	  case 1:
	    arch = bfd_arch_powerpc;
	    machine = bfd_mach_ppc_601;
	    break;
	  case 2: /* 64 bit PowerPC */
	    arch = bfd_arch_powerpc;
	    machine = bfd_mach_ppc_620;
	    break;
	  case 3:
	    arch = bfd_arch_powerpc;
	    machine = bfd_mach_ppc;
	    break;
	  case 4:
	    arch = bfd_arch_rs6000;
	    machine = bfd_mach_rs6k;
	    break;
	  }
      }
      break;
#endif /* RS6000COFF_C */

#ifdef WE32KMAGIC
    case WE32KMAGIC:
      arch = bfd_arch_we32k;
      break;
#endif /* WE32KMAGIC */

#ifdef H8300MAGIC
    case H8300MAGIC:
      arch = bfd_arch_h8300;
      machine = bfd_mach_h8300;
      /* !! FIXME this probably is NOT the right place for this.  */
      abfd->flags |= BFD_IS_RELAXABLE;
      break;
#endif /* H8300MAGIC */

#ifdef H8300HMAGIC
    case H8300HMAGIC:
      arch = bfd_arch_h8300;
      machine = bfd_mach_h8300h;
      /* !! FIXME this probably is NOT the right place for this.  */
      abfd->flags |= BFD_IS_RELAXABLE;
      break;
#endif /* H8300HMAGIC */

#ifdef H8300SMAGIC
    case H8300SMAGIC:
      arch = bfd_arch_h8300;
      machine = bfd_mach_h8300s;
      /* !! FIXME this probably is NOT the right place for this.  */
      abfd->flags |= BFD_IS_RELAXABLE;
      break;
#endif /* H8300SMAGIC */

#ifdef H8300HNMAGIC
    case H8300HNMAGIC:
      arch = bfd_arch_h8300;
      machine = bfd_mach_h8300hn;
      /* !! FIXME this probably is NOT the right place for this.  */
      abfd->flags |= BFD_IS_RELAXABLE;
      break;
#endif /* H8300HNMAGIC */

#ifdef H8300SNMAGIC
    case H8300SNMAGIC:
      arch = bfd_arch_h8300;
      machine = bfd_mach_h8300sn;
      /* !! FIXME this probably is NOT the right place for this.  */
      abfd->flags |= BFD_IS_RELAXABLE;
      break;
#endif /* H8300SNMAGIC */

#ifdef SH_ARCH_MAGIC_BIG
    case SH_ARCH_MAGIC_BIG:
    case SH_ARCH_MAGIC_LITTLE:
# ifdef COFF_WITH_PE
    case SH_ARCH_MAGIC_WINCE:
# endif /* COFF_WITH_PE */
      arch = bfd_arch_sh;
      break;
#endif /* SH_ARCH_MAGIC_BIG */

#ifdef MIPS_ARCH_MAGIC_WINCE
    case MIPS_ARCH_MAGIC_WINCE:
      arch = bfd_arch_mips;
      break;
#endif /* MIPS_ARCH_MAGIC_WINCE */

#ifdef H8500MAGIC
    case H8500MAGIC:
      arch = bfd_arch_h8500;
      break;
#endif /* H8500MAGIC */

#ifdef SPARCMAGIC
    case SPARCMAGIC:
# ifdef LYNXCOFFMAGIC
    case LYNXCOFFMAGIC:
# endif /* LYNXCOFFMAGIC */
      arch = bfd_arch_sparc;
      break;
#endif /* SPARCMAGIC */

#ifdef TIC30MAGIC
    case TIC30MAGIC:
      arch = bfd_arch_tic30;
      break;
#endif /* TIC30MAGIC */

#ifdef TICOFF0MAGIC
# ifdef TICOFF_TARGET_ARCH
      /* This TI COFF section should be used by all new TI COFF v0 targets.  */
    case TICOFF0MAGIC:
      arch = TICOFF_TARGET_ARCH;
      machine = TICOFF_TARGET_MACHINE_GET (internal_f->f_flags);
      break;
# endif /* TICOFF_TARGET_ARCH */
#endif /* TICOFF0MAGIC */

#ifdef TICOFF1MAGIC
      /* This TI COFF section should be used by all new TI COFF v1/2 targets.  */
      /* TI COFF1 and COFF2 use the target_id field to specify which arch.  */
    case TICOFF1MAGIC:
    case TICOFF2MAGIC:
      switch (internal_f->f_target_id)
        {
# ifdef TI_TARGET_ID
        case TI_TARGET_ID:
          arch = TICOFF_TARGET_ARCH;
	  machine = TICOFF_TARGET_MACHINE_GET (internal_f->f_flags);
          break;
# endif /* TI_TARGET_ID */
        default:
          arch = bfd_arch_obscure;
          (*_bfd_error_handler)
            (_("Unrecognized TI COFF target id '0x%x'"),
             internal_f->f_target_id);
          break;
        }
      break;
#endif /* TICOFF1MAGIC */

#ifdef TIC80_ARCH_MAGIC
    case TIC80_ARCH_MAGIC:
      arch = bfd_arch_tic80;
      break;
#endif /* TIC80_ARCH_MAGIC */

#ifdef MCOREMAGIC
    case MCOREMAGIC:
      arch = bfd_arch_mcore;
      break;
#endif /* MCOREMAGIC */

#ifdef W65MAGIC
    case W65MAGIC:
      arch = bfd_arch_w65;
      break;
#endif /* W65MAGIC */

    default:			/* Unreadable input file type.  */
      arch = bfd_arch_obscure;
      break;
    }

  bfd_default_set_arch_mach (abfd, arch, machine);
  return TRUE;
}

#ifdef SYMNAME_IN_DEBUG

static bfd_boolean
symname_in_debug_hook (bfd * abfd ATTRIBUTE_UNUSED, struct internal_syment *sym)
{
  return SYMNAME_IN_DEBUG (sym) != 0;
}

#else

# define symname_in_debug_hook \
  (bfd_boolean (*) (bfd *, struct internal_syment *)) bfd_false

#endif /* SYMNAME_IN_DEBUG */

#ifdef RS6000COFF_C
# ifdef XCOFF64
#  define FORCE_SYMNAMES_IN_STRINGS
# endif /* XCOFF64 */
/* Handle the csect auxent of a C_EXT or C_HIDEXT symbol.  */
static bfd_boolean
coff_pointerize_aux_hook(bfd *abfd ATTRIBUTE_UNUSED,
                         combined_entry_type *table_base,
                         combined_entry_type *symbol,
                         unsigned int indaux,
                         combined_entry_type *aux)
{
  int coffclass = symbol->u.syment.n_sclass;

  if (((coffclass == C_EXT) || (coffclass == C_HIDEXT))
      && ((indaux + 1) == symbol->u.syment.n_numaux))
    {
      if (SMTYP_SMTYP(aux->u.auxent.x_csect.x_smtyp) == XTY_LD)
	{
	  aux->u.auxent.x_csect.x_scnlen.p =
	    (table_base + aux->u.auxent.x_csect.x_scnlen.l);
	  aux->fix_scnlen = 1;
	}

      /* Return TRUE to indicate that the caller should not do any
         further work on this auxent.  */
      return TRUE;
    }

  /* Return FALSE to indicate that this auxent should be handled by
     the caller.  */
  return FALSE;
}

#else
# ifdef I960
/* We do NOT want to pointerize bal entries.  */
static bfd_boolean
coff_pointerize_aux_hook(bfd *abfd ATTRIBUTE_UNUSED,
			 combined_entry_type *table_base ATTRIBUTE_UNUSED,
			 combined_entry_type *symbol, unsigned int indaux,
			 combined_entry_type *aux ATTRIBUTE_UNUSED)
{
  /* Return TRUE if we do NOT want to pointerize this aux entry, which
     is the case for the lastfirst aux entry for a C_LEAFPROC symbol.  */
  return ((indaux == 1)
	  && ((symbol->u.syment.n_sclass == C_LEAFPROC)
	      || (symbol->u.syment.n_sclass == C_LEAFSTAT)
	      || (symbol->u.syment.n_sclass == C_LEAFEXT)));
  /* FIXME: what if C_LEAFSTAT == C_LEAFEXT? */
}

# else /* ! I960 */
#  define coff_pointerize_aux_hook 0
# endif /* ! I960 */
#endif /* ! RS6000COFF_C */

/* Print an aux entry.  This returns TRUE if it has printed it: */
static bfd_boolean
coff_print_aux(bfd *abfd ATTRIBUTE_UNUSED, FILE *file,
               combined_entry_type *table_base, combined_entry_type *symbol,
               combined_entry_type *aux, unsigned int indaux)
{
#ifdef RS6000COFF_C
  if (((symbol->u.syment.n_sclass == C_EXT)
       || (symbol->u.syment.n_sclass == C_HIDEXT))
      && ((indaux + 1) == symbol->u.syment.n_numaux))
    {
      /* This is a csect entry: */
      fprintf(file, "AUX ");
      if (SMTYP_SMTYP(aux->u.auxent.x_csect.x_smtyp) != XTY_LD)
	{
	  BFD_ASSERT(! aux->fix_scnlen);
# ifdef XCOFF64
	  fprintf(file, "val %5lld",
                  (long long)aux->u.auxent.x_csect.x_scnlen.l);
# else
	  fprintf(file, "val %5ld", (long)aux->u.auxent.x_csect.x_scnlen.l);
# endif /* XCOFF64 */
	}
      else
	{
	  fprintf(file, "indx ");
	  if (!aux->fix_scnlen)
# ifdef XCOFF64
	    fprintf(file, "%4lld",
                    (long long)aux->u.auxent.x_csect.x_scnlen.l);
# else
	    fprintf(file, "%4ld", (long)aux->u.auxent.x_csect.x_scnlen.l);
# endif /* XCOFF64 */
	  else
	    fprintf(file, "%4ld",
                    (long)(aux->u.auxent.x_csect.x_scnlen.p - table_base));
	}
      fprintf(file,
	      " prmhsh %ld snhsh %u typ %d algn %d clss %u stb %ld snstb %u",
	      aux->u.auxent.x_csect.x_parmhash,
	      (unsigned int)aux->u.auxent.x_csect.x_snhash,
	      SMTYP_SMTYP(aux->u.auxent.x_csect.x_smtyp),
	      SMTYP_ALIGN(aux->u.auxent.x_csect.x_smtyp),
	      (unsigned int)aux->u.auxent.x_csect.x_smclas,
	      aux->u.auxent.x_csect.x_stab,
	      (unsigned int)aux->u.auxent.x_csect.x_snstab);
      return TRUE;
    }
#else
  (void)symbol;
  (void)indaux;
  (void)file;
  (void)aux;
  (void)table_base;
#endif /* RS6000COFF_C */

  /* Return FALSE to indicate that no special action was taken: */
  return FALSE;
}

/*
SUBSUBSECTION
	Writing relocations

	To write relocations, the back end steps though the
	canonical relocation table and create an
	@code{internal_reloc}. The symbol index to use is removed from
	the @code{offset} field in the symbol table supplied.  The
	address comes directly from the sum of the section base
	address and the relocation offset; the type is dug directly
	from the howto field.  Then the @code{internal_reloc} is
	swapped into the shape of an @code{external_reloc} and written
	out to disk.

*/

#ifdef TARG_AUX
/* AUX's ld wants relocations to be sorted.  */
static int compare_arelent_ptr(const void * x, const void * y)
{
  const arelent **a = (const arelent **)x;
  const arelent **b = (const arelent **)y;
  bfd_size_type aadr = (*a)->address;
  bfd_size_type badr = (*b)->address;

  return ((aadr < badr) ? -1 : ((badr < aadr) ? 1 : 0));
}
#endif /* TARG_AUX */

static bfd_boolean coff_write_relocs(bfd *abfd, int first_undef)
{
  asection *s;

  for (s = abfd->sections; s != NULL; s = s->next)
    {
      unsigned int i;
      struct external_reloc dst;
      arelent **p;

#ifndef TARG_AUX
      p = s->orelocation;
#else
      {
	/* Sort relocations before we write them out.  */
	bfd_size_type amt;

	amt = s->reloc_count;
	amt *= sizeof(arelent *);
	p = (arelent **)bfd_malloc(amt);
	if ((p == NULL) && (s->reloc_count > 0)) {
	    return FALSE;
	}
	memcpy(p, s->orelocation, (size_t)amt);
	qsort(p, (size_t)s->reloc_count, sizeof(arelent *),
              compare_arelent_ptr);
      }
#endif /* TARG_AUX */

      if (bfd_seek(abfd, s->rel_filepos, SEEK_SET) != 0) {
        return FALSE;
      }

#ifdef COFF_WITH_PE
      if (obj_pe(abfd) && (s->reloc_count >= 0xffff))
	{
	  /* Encode real count here as first reloc: */
	  struct internal_reloc n;

	  memset(&n, 0, sizeof(n));
	  /* Add one to count *this* reloc (grr).  */
	  n.r_vaddr = (s->reloc_count + 1);
	  coff_swap_reloc_out(abfd, &n, &dst);
	  if (bfd_bwrite(&dst, (bfd_size_type)bfd_coff_relsz(abfd),
			 abfd) != bfd_coff_relsz(abfd))
	    return FALSE;
	}
#endif /* COFF_WITH_PE */

      for (i = 0; i < s->reloc_count; i++)
	{
	  struct internal_reloc n;
	  arelent *q = p[i];

	  memset(&n, 0, sizeof(n));

	  /* Now we have renumbered the symbols we know where the
	     undefined symbols live in the table.  Check the reloc
	     entries for symbols whose output bfd is NOT the right one.
	     This is because the symbol was undefined (which means
	     that all the pointers are never made to point to the same
	     place). This is a bad thing, because the symbols attached
	     to the output bfd are indexed, so that the relocation
	     entries know which symbol index they point to.  So we
	     have to look up the output symbol here.  */

	  if (q->sym_ptr_ptr[0]->the_bfd != abfd)
	    {
	      int j;
	      const char *sname = q->sym_ptr_ptr[0]->name;
	      asymbol **outsyms = abfd->outsymbols;

	      for (j = first_undef; outsyms[j]; j++)
		{
		  const char *intable = outsyms[j]->name;

		  if (strcmp(intable, sname) == 0)
		    {
		      /* Got a hit, so repoint the reloc: */
		      q->sym_ptr_ptr = (outsyms + j);
		      break;
		    }
		}
	    }

	  n.r_vaddr = (q->address + s->vma);

#ifdef R_IHCONST
	  /* The 29k const/consth reloc pair is a real kludge. The consth
	     part does NOT have a symbol; it has an offset. So rebuilt
	     that here.  */
          if (q->howto->type == R_IHCONST) {
            n.r_symndx = (long)q->addend;
          } else
#endif /* R_IHCONST */
	    if (q->sym_ptr_ptr)
	      {
#ifdef SECTION_RELATIVE_ABSOLUTE_SYMBOL_P
                if (SECTION_RELATIVE_ABSOLUTE_SYMBOL_P(q, s))
#else
		if (((*q->sym_ptr_ptr)->section == bfd_abs_section_ptr)
		    && (((*q->sym_ptr_ptr)->flags & BSF_SECTION_SYM) != 0))
#endif /* SECTION_RELATIVE_ABSOLUTE_SYMBOL_P */
		  /* This is a relocation relative to the absolute symbol: */
		  n.r_symndx = -1L;
		else
		  {
		    n.r_symndx = (long)get_index((*(q->sym_ptr_ptr)));
		    /* Take notice if the symbol reloc points to a symbol
		       we do NOT have in our symbol table. What should we
		       do for this??  */
		    if (n.r_symndx > obj_conv_table_size(abfd))
		      abort();
		  }
	      }

#ifdef SWAP_OUT_RELOC_OFFSET
	  n.r_offset = (unsigned long)q->addend;
#endif /* SWAP_OUT_RELOC_OFFSET */

#ifdef SELECT_RELOC
	  /* Work out reloc type from what is required: */
	  SELECT_RELOC(n, q->howto);
#else
	  n.r_type = (unsigned short)q->howto->type;
#endif /* SELECT_RELOC */
	  coff_swap_reloc_out(abfd, &n, &dst);

	  if (bfd_bwrite(&dst, (bfd_size_type)bfd_coff_relsz(abfd),
			 abfd) != bfd_coff_relsz(abfd))
	    return FALSE;
	}

#ifdef TARG_AUX
      if (p != NULL) {
        free(p);
      }
#endif /* TARG_AUX */
    }

  return TRUE;
}

/* Set flags and magic number of a coff file from architecture and machine
 * type.  Result is TRUE if we can represent the arch&type, FALSE if not: */
static bfd_boolean
coff_set_flags(bfd *abfd, unsigned int *magicp, unsigned short *flagsp)
{
  switch (bfd_get_arch(abfd))
    {
#ifdef Z8KMAGIC
    case bfd_arch_z8k:
      *magicp = Z8KMAGIC;

      switch (bfd_get_mach(abfd))
	{
	case bfd_mach_z8001: *flagsp = F_Z8001;	break;
	case bfd_mach_z8002: *flagsp = F_Z8002;	break;
	default:	     return FALSE;
	}
      return TRUE;
#endif /* Z8KMAGIC */

#ifdef I960ROMAGIC
    case bfd_arch_i960:
      {
	unsigned int flags;

	*magicp = I960ROMAGIC;

	switch (bfd_get_mach(abfd))
	  {
	  case bfd_mach_i960_core:  flags = F_I960CORE; break;
	  case bfd_mach_i960_kb_sb: flags = F_I960KB;	break;
	  case bfd_mach_i960_mc:    flags = F_I960MC;	break;
	  case bfd_mach_i960_xa:    flags = F_I960XA;	break;
	  case bfd_mach_i960_ca:    flags = F_I960CA;	break;
	  case bfd_mach_i960_ka_sa: flags = F_I960KA;	break;
	  case bfd_mach_i960_jx:    flags = F_I960JX;	break;
	  case bfd_mach_i960_hx:    flags = F_I960HX;	break;
	  default:	            return FALSE;
	  }
	*flagsp = (unsigned short)flags;
	return TRUE;
      }
      break;
#endif /* I960ROMAGIC */

#ifdef TIC30MAGIC
    case bfd_arch_tic30:
      *magicp = TIC30MAGIC;
      return TRUE;
#endif /* TIC30MAGIC */

#ifdef TICOFF_DEFAULT_MAGIC
    case TICOFF_TARGET_ARCH:
      /* If there is no indication of which version we want, then use the
       * default: */
      if (!abfd->xvec)
        *magicp = TICOFF_DEFAULT_MAGIC;
      else
        {
          /* We may want to output in a different COFF version: */
          switch (abfd->xvec->name[4])
            {
            case '0':
              *magicp = TICOFF0MAGIC;
              break;
            case '1':
              *magicp = TICOFF1MAGIC;
              break;
            case '2':
              *magicp = TICOFF2MAGIC;
              break;
            default:
              return FALSE;
            }
        }
      TICOFF_TARGET_MACHINE_SET(flagsp, bfd_get_mach(abfd));
      return TRUE;
#endif /* TICOFF_DEFAULT_MAGIC */

#ifdef TIC80_ARCH_MAGIC
    case bfd_arch_tic80:
      *magicp = TIC80_ARCH_MAGIC;
      return TRUE;
#endif /* TIC80_ARCH_MAGIC */

#ifdef ARMMAGIC
    case bfd_arch_arm:
# ifdef ARM_WINCE
      *magicp = ARMPEMAGIC;
# else
      *magicp = ARMMAGIC;
# endif /* ARM_WINCE */
      *flagsp = 0;
      if (APCS_SET(abfd))
	{
          if (APCS_26_FLAG(abfd)) {
            *flagsp |= F_APCS26;
          }

          if (APCS_FLOAT_FLAG(abfd)) {
            *flagsp |= F_APCS_FLOAT;
          }

          if (PIC_FLAG(abfd)) {
            *flagsp |= F_PIC;
          }
	}
      if (INTERWORK_SET (abfd) && INTERWORK_FLAG (abfd)) {
        *flagsp |= F_INTERWORK;
      }
      switch (bfd_get_mach(abfd)) {
	case bfd_mach_arm_2:  *flagsp |= F_ARM_2;  break;
	case bfd_mach_arm_2a: *flagsp |= F_ARM_2a; break;
	case bfd_mach_arm_3:  *flagsp |= F_ARM_3;  break;
	case bfd_mach_arm_3M: *flagsp |= F_ARM_3M; break;
	case bfd_mach_arm_4:  *flagsp |= F_ARM_4;  break;
	case bfd_mach_arm_4T: *flagsp |= F_ARM_4T; break;
	case bfd_mach_arm_5:  *flagsp |= F_ARM_5;  break;
	  /* FIXME: we do not have F_ARM vaues greater than F_ARM_5.
	   * See also the comment in coff_set_arch_mach_hook().  */
	case bfd_mach_arm_5T: *flagsp |= F_ARM_5;  break;
	case bfd_mach_arm_5TE: *flagsp |= F_ARM_5; break;
	case bfd_mach_arm_XScale: *flagsp |= F_ARM_5; break;
	default: break;
      }
      return TRUE;
#endif /* ARMMAGIC */

#ifdef PPCMAGIC
    case bfd_arch_powerpc:
      *magicp = PPCMAGIC;
      return TRUE;
#endif /* PPCMAGIC */

#ifdef I386MAGIC
    case bfd_arch_i386:
      *magicp = I386MAGIC;
# ifdef LYNXOS
      /* Just overwrite the usual value if we are doing Lynx.  */
      *magicp = LYNXCOFFMAGIC;
# endif /* LYNXOS */
      return TRUE;
#endif /* I386MAGIC */

#ifdef I860MAGIC
    case bfd_arch_i860:
      *magicp = I860MAGIC;
      return TRUE;
#endif /* I860MAGIC */

#ifdef IA64MAGIC
    case bfd_arch_ia64:
      *magicp = IA64MAGIC;
      return TRUE;
#endif /* IA64MAGIC */

#ifdef MC68MAGIC
    case bfd_arch_m68k:
# ifdef APOLLOM68KMAGIC
      *magicp = APOLLO_COFF_VERSION_NUMBER;
# else
      /* NAMES_HAVE_UNDERSCORE may be defined by coff-u68k.c.  */
#  ifdef NAMES_HAVE_UNDERSCORE
      *magicp = MC68KBCSMAGIC;
#  else
      *magicp = MC68MAGIC;
#  endif /* NAMES_HAVE_UNDERSCORE */
# endif /* APOLLOM68KMAGIC */
# ifdef LYNXOS
      /* Just overwrite the usual value if we are doing Lynx.  */
      *magicp = LYNXCOFFMAGIC;
# endif /* LYNXOS */
      return TRUE;
#endif /* MC68MAGIC */

#ifdef MC88MAGIC
    case bfd_arch_m88k:
      *magicp = MC88OMAGIC;
      return TRUE;
#endif /* MC88MAGIC */

#ifdef H8300MAGIC
    case bfd_arch_h8300:
      switch (bfd_get_mach(abfd))
	{
	case bfd_mach_h8300: *magicp = H8300MAGIC; return TRUE;
	case bfd_mach_h8300h: *magicp = H8300HMAGIC; return TRUE;
	case bfd_mach_h8300s: *magicp = H8300SMAGIC; return TRUE;
	case bfd_mach_h8300hn: *magicp = H8300HNMAGIC; return TRUE;
	case bfd_mach_h8300sn: *magicp = H8300SNMAGIC; return TRUE;
	default: break;
	}
      break;
#endif /* H8300MAGIC */

#ifdef SH_ARCH_MAGIC_BIG
    case bfd_arch_sh:
# ifdef COFF_IMAGE_WITH_PE
      *magicp = SH_ARCH_MAGIC_WINCE;
# else
      if (bfd_big_endian(abfd)) {
        *magicp = SH_ARCH_MAGIC_BIG;
      } else {
        *magicp = SH_ARCH_MAGIC_LITTLE;
      }
# endif /* COFF_IMAGE_WITH_PE */
      return TRUE;
#endif

#ifdef MIPS_ARCH_MAGIC_WINCE
    case bfd_arch_mips:
      *magicp = MIPS_ARCH_MAGIC_WINCE;
      return TRUE;
#endif /* MIPS_ARCH_MAGIC_WINCE */

#ifdef SPARCMAGIC
    case bfd_arch_sparc:
      *magicp = SPARCMAGIC;
# ifdef LYNXOS
      /* Just overwrite the usual value if we are doing Lynx.  */
      *magicp = LYNXCOFFMAGIC;
# endif /* LYNXOS */
      return TRUE;
#endif /* SPARCMAGIC */

#ifdef H8500MAGIC
    case bfd_arch_h8500:
      *magicp = H8500MAGIC;
      return TRUE;
      break;
#endif /* H8500MAGIC */

#ifdef A29K_MAGIC_BIG
    case bfd_arch_a29k:
      if (bfd_big_endian(abfd)) {
        *magicp = A29K_MAGIC_BIG;
      } else {
        *magicp = A29K_MAGIC_LITTLE;
      }
      return TRUE;
#endif /* A29K_MAGIC_BIG */

#ifdef WE32KMAGIC
    case bfd_arch_we32k:
      *magicp = WE32KMAGIC;
      return TRUE;
#endif /* WE32KMAGIC */

#ifdef RS6000COFF_C
    case bfd_arch_rs6000:
# ifndef PPCMAGIC
    case bfd_arch_powerpc:
# endif /* !PPCMAGIC */
      BFD_ASSERT(bfd_get_flavour(abfd) == bfd_target_xcoff_flavour);
      *magicp = bfd_xcoff_magic_number(abfd);
      return TRUE;
#endif /* RS6000COFF_C */

#ifdef MCOREMAGIC
    case bfd_arch_mcore:
      *magicp = MCOREMAGIC;
      return TRUE;
#endif /* MCOREMAGIC */

#ifdef W65MAGIC
    case bfd_arch_w65:
      *magicp = W65MAGIC;
      return TRUE;
#endif /* W65MAGIC */

#ifdef OR32_MAGIC_BIG
    case bfd_arch_or32:
      if (bfd_big_endian(abfd)) {
        *magicp = OR32_MAGIC_BIG;
      } else {
        *magicp = OR32_MAGIC_LITTLE;
      }
      return TRUE;
#endif /* OR32_MAGIC_BIG */

#ifdef MAXQ20MAGIC
    case bfd_arch_maxq:
      *magicp = MAXQ20MAGIC;
      switch (bfd_get_mach(abfd))
	{
	case bfd_mach_maxq10: *flagsp = F_MAXQ10; return TRUE;
	case bfd_mach_maxq20: *flagsp = F_MAXQ20; return TRUE;
	default:	      return FALSE;
	}
#endif /* MAXQ20MAGIC */

    default:			/* Unknown architecture.  */
      *magicp = 0x0;
      *flagsp = 0x0;
      /* Fall through to "return FALSE" below, to avoid
       * "statement never reached" errors on the one below: */
      break;
    }

  return FALSE;
}

/* */
static ATTRIBUTE_USED bfd_boolean
coff_set_arch_mach(bfd *abfd, enum bfd_architecture arch,
                   unsigned long machine)
{
  unsigned dummy1;
  unsigned short dummy2;

  if (! bfd_default_set_arch_mach(abfd, arch, machine))
    return FALSE;

  if ((arch != bfd_arch_unknown)
      && ! coff_set_flags(abfd, &dummy1, &dummy2))
    return FALSE;		/* We cannot represent this type.  */

  return TRUE;			/* We are easy...  */
}

#ifdef COFF_IMAGE_WITH_PE

/* This is used to sort sections by VMA, as required by PE image files: */
static int
sort_by_secaddr(const void * arg1, const void * arg2)
{
  const asection *a = *(const asection **)arg1;
  const asection *b = *(const asection **)arg2;

  if (a->vma < b->vma)
    return -1;
  else if (a->vma > b->vma)
    return 1;

  return 0;
}

#endif /* COFF_IMAGE_WITH_PE */

/* Calculate the file position for each section: */
#ifndef I960
# define ALIGN_SECTIONS_IN_FILE
#endif /* !I960 */
#if defined(TIC80COFF) || defined(TICOFF)
# undef ALIGN_SECTIONS_IN_FILE
#endif /* TIC80COFF || TICOFF */

static bfd_boolean
coff_compute_section_file_positions(bfd *abfd)
{
  asection *current;
  asection *previous = (asection *)NULL;
  file_ptr sofar = bfd_coff_filhsz(abfd);
  bfd_boolean align_adjust;
#ifdef ALIGN_SECTIONS_IN_FILE
  file_ptr old_sofar;
#endif /* ALIGN_SECTIONS_IN_FILE */

#ifdef RS6000COFF_C
  /* On XCOFF, if we have symbols, set up the .debug section: */
  if (bfd_get_symcount(abfd) > 0)
    {
      bfd_size_type sz;
      bfd_size_type i, symcount;
      asymbol **symp;

      sz = 0UL;
      symcount = bfd_get_symcount(abfd);
      for (symp = abfd->outsymbols, i = 0; i < symcount; symp++, i++)
	{
	  coff_symbol_type *cf;

	  cf = coff_symbol_from(abfd, *symp);
	  if ((cf != NULL)
	      && (cf->native != NULL)
	      && SYMNAME_IN_DEBUG(&cf->native->u.syment))
	    {
	      size_t len;

	      len = strlen(bfd_asymbol_name(*symp));
              if ((len > SYMNMLEN) || bfd_coff_force_symnames_in_strings(abfd)) {
                sz += (len + 1UL + bfd_coff_debug_string_prefix_length(abfd));
              }
	    }
	}
      if (sz > 0)
	{
	  asection *dsec;

	  dsec = bfd_make_section_old_way(abfd, DOT_DEBUG);
          if (dsec == NULL) {
            abort();
          }
	  dsec->size = sz;
	  dsec->flags |= SEC_HAS_CONTENTS;
	}
    }
#endif /* RS6000COFF_C */

#ifdef COFF_IMAGE_WITH_PE
  int page_size;

  if (coff_data(abfd)->link_info)
    {
      page_size = (int)pe_data(abfd)->pe_opthdr.FileAlignment;

      /* If no file alignment has been set, default to one.
       * This repairs 'ld -r' for arm-wince-pe target.  */
      if (page_size == 0)
        page_size = 1;
    }
  else
    page_size = PE_DEF_FILE_ALIGNMENT;
#else
# ifdef COFF_PAGE_SIZE
  int page_size = COFF_PAGE_SIZE;
# endif /* COFF_PAGE_SIZE */
#endif /* COFF_IMAGE_WITH_PE */

  if (bfd_get_start_address(abfd)) {
    /* A start address may have been added to the original file. In this
     * case it will need an optional header to record it.  */
    abfd->flags |= EXEC_P;
  }

  if (abfd->flags & EXEC_P) {
    sofar += bfd_coff_aoutsz(abfd);
#ifdef RS6000COFF_C
  } else if (xcoff_data(abfd)->full_aouthdr) {
    sofar += bfd_coff_aoutsz(abfd);
  } else {
    sofar += SMALL_AOUTSZ;
#endif /* RS6000COFF_C */
  }

  sofar += (abfd->section_count * bfd_coff_scnhsz(abfd));

#ifdef RS6000COFF_C
  /* XCOFF handles overflows in the reloc and line number count fields
     by allocating a new section header to hold the correct counts. */
  for (current = abfd->sections; current != NULL; current = current->next)
    {
      if ((current->reloc_count >= 0xffff) || (current->lineno_count >= 0xffff)) {
        sofar += bfd_coff_scnhsz(abfd);
      }
    }
#endif /* RS6000COFF_C */

#ifdef COFF_IMAGE_WITH_PE
  {
    /* PE requires the sections to be in memory order when listed in
       the section headers.  It also does not like empty loadable
       sections.  The sections apparently do not have to be in the
       right order in the image file itself, but we do need to get the
       target_index values right.  */

    unsigned int count;
    asection **section_list;
    unsigned int i;
    int target_index;
    bfd_size_type amt;

    count = 0;
    for (current = abfd->sections; current != NULL; current = current->next) {
      ++count;
    }

    /* We allocate an extra cell to simplify the final loop: */
    amt = (sizeof(struct asection *) * (count + 1));
    section_list = (asection **)bfd_malloc(amt);
    if (section_list == NULL) {
      return FALSE;
    }

    i = 0;
    for (current = abfd->sections; current != NULL; current = current->next)
      {
	section_list[i] = current;
	++i;
      }
    section_list[i] = NULL;

    qsort(section_list, (size_t)count, sizeof(asection *),
          sort_by_secaddr);

    /* Rethread the linked list into sorted order; at the same time,
       assign target_index values.  */
    target_index = 1;
    abfd->sections = NULL;
    abfd->section_last = NULL;
    for (i = 0; i < count; i++)
      {
	current = section_list[i];
	bfd_section_list_append (abfd, current);

	/* Later, if the section has zero size, we will be throwing it
	   away, so we do NOT want to number it now.  Note that having
	   a zero size and having real contents are different
	   concepts: .bss has no contents, but (usually) non-zero
	   size.  */
	if (current->size == 0)
	  {
	    /* Discard.  However, it still might have (valid) symbols
	       in it, so arbitrarily set it to section 1 (indexing is
	       1-based here; usually .text).  __end__ and other
	       contents of .endsection really have this happen.
	       FIXME: This seems somewhat dubious.  */
	    current->target_index = 1;
	  }
	else
	  current->target_index = target_index++;
      }

    free(section_list);
  }
#else /* ! COFF_IMAGE_WITH_PE */
  {
    /* Set the target_index field: */
    int target_index;

    target_index = 1;
    for (current = abfd->sections; current != NULL; current = current->next)
      current->target_index = target_index++;
  }
#endif /* ! COFF_IMAGE_WITH_PE */

  align_adjust = FALSE;
  for (current = abfd->sections;
       current != NULL;
       current = current->next)
    {
#ifdef COFF_IMAGE_WITH_PE
      /* With PE we have to pad each section to be a multiple of its
       * page size too, and remember both sizes.  */
      if (coff_section_data (abfd, current) == NULL)
	{
	  bfd_size_type amt = sizeof(struct coff_section_tdata);

	  current->used_by_bfd = bfd_zalloc(abfd, amt);
          if (current->used_by_bfd == NULL) {
            return FALSE;
          }
	}
      if (pei_section_data (abfd, current) == NULL)
	{
	  bfd_size_type amt = sizeof(struct pei_section_tdata);

	  coff_section_data(abfd, current)->tdata = bfd_zalloc(abfd, amt);
          if (coff_section_data(abfd, current)->tdata == NULL) {
            return FALSE;
          }
	}
      if (pei_section_data(abfd, current)->virt_size == 0) {
        pei_section_data(abfd, current)->virt_size = current->size;
      }
#endif /* COFF_IMAGE_WITH_PE */

      /* Only deal with sections which have contents: */
      if (!(current->flags & SEC_HAS_CONTENTS)) {
        continue;
      }

#ifdef COFF_IMAGE_WITH_PE
      /* Make sure we skip empty sections in a PE image: */
      if (current->size == 0) {
        continue;
      }
#endif /* COFF_IMAGE_WITH_PE */

      /* Align the sections in the file to the same boundary on
       * which they are aligned in virtual memory.  I960 does NOT
       * do this (FIXME) so we can stay in sync with Intel.  960
       * does NOT yet page from files...  */
#ifdef ALIGN_SECTIONS_IN_FILE
      if ((abfd->flags & EXEC_P) != 0)
	{
	  /* Make sure this section is aligned on the right boundary - by
	     padding the previous section up if necessary.  */
	  old_sofar = sofar;

# ifdef RS6000COFF_C
	  /* AIX loader checks the text section alignment of (vma - filepos)
	     So even though the filepos may be aligned wrt the o_algntext, for
	     AIX executables, this check fails. This shows up when a native
	     AIX executable is stripped with gnu strip because the default vma
	     of native is 0x10000150 but default for gnu is 0x10000140.  Gnu
	     stripped gnu excutable passes this check because the filepos is
	     0x0140.  This problem also show up with 64 bit shared objects. The
	     data section must also be aligned.  */
	  if (!strcmp(current->name, _TEXT)
	      || !strcmp(current->name, _DATA))
	    {
	      bfd_vma pad;
	      bfd_vma align;

	      sofar =
                (file_ptr)BFD_ALIGN(sofar,
                                    (1UL << current->alignment_power));

	      align = (bfd_vma)(1UL << current->alignment_power);
	      pad = (bfd_vma)(abs((int)((file_ptr)current->vma - sofar))
                              % (int)align);

	      if (pad)
		{
		  pad = (align - pad);
		  sofar += (file_ptr)pad;
		}
	    }
	  else
# else
	    {
	      sofar = ((file_ptr)
                       BFD_ALIGN(sofar,
                                 (1UL << current->alignment_power)));
	    }
# endif /* RS6000COFF_C */
	  if (previous != NULL)
	    previous->size += (bfd_size_type)(sofar - old_sofar);
	}

#endif /* ALIGN_SECTIONS_IN_FILE */

      /* In demand paged files the low order bits of the file offset
       * must match the low order bits of the virtual address.  */
#ifdef COFF_PAGE_SIZE
      if (((abfd->flags & D_PAGED) != 0)
          && ((current->flags & SEC_ALLOC) != 0)) {
        sofar += (file_ptr)((current->vma - (bfd_vma)sofar)
                            % (bfd_vma)page_size);
      }
#endif /* COFF_PAGE_SIZE */
      current->filepos = sofar;

#ifdef COFF_IMAGE_WITH_PE
      /* Set the padded size: */
      current->size = ((current->size + (bfd_size_type)page_size - 1UL)
                       & (bfd_size_type)-page_size);
#endif /* COFF_IMAGE_WITH_PE */

      sofar += (file_ptr)current->size;

#ifdef ALIGN_SECTIONS_IN_FILE
      /* Make sure that this section is of the right size too.  */
      if ((abfd->flags & EXEC_P) == 0)
	{
	  bfd_size_type old_size;

	  old_size = current->size;
	  current->size = BFD_ALIGN(current->size,
				    (1UL << current->alignment_power));
	  align_adjust = (current->size != old_size);
	  sofar += (file_ptr)(current->size - old_size);
	}
      else
	{
	  old_sofar = sofar;
	  sofar = (file_ptr)BFD_ALIGN(sofar,
                                      (1UL << current->alignment_power));
	  align_adjust = (sofar != old_sofar);
	  current->size += (bfd_size_type)(sofar - old_sofar);
	}
#endif /* ALIGN_SECTIONS_IN_FILE */

#ifdef COFF_IMAGE_WITH_PE
      /* For PE we need to make sure we pad out to the aligned
         size, in case the caller only writes out data to the
         unaligned size.  */
      if (pei_section_data(abfd, current)->virt_size < current->size) {
        align_adjust = TRUE;
      }
#endif /* COFF_IMAGE_WITH_PE */

#ifdef _LIB
      /* Force .lib sections to start at zero.  The vma is then
       * incremented in coff_set_section_contents.  This is right for
       * SVR3.2.  */
      if (strcmp(current->name, _LIB) == 0) {
        if (bfd_set_section_vma(abfd, current, 0)) {
          ; /* do nothing; just silence '-Wunused-value' */
        }
      }
#endif /* _LIB */

      previous = current;
    }
  
  if (previous == (asection *)NULL) {
    ; /* ??? */
  }

  /* It is now safe to write to the output file. If we needed an
     alignment adjustment for the last section, then make sure that
     there is a byte at offset sofar. If there are no symbols and no
     relocs, then nothing follows the last section. If we do NOT force
     the last byte out, then the file may appear to be truncated.  */
  if (align_adjust)
    {
      bfd_byte b;

      b = 0;
      if ((bfd_seek(abfd, (sofar - 1L), SEEK_SET) != 0)
          || (bfd_bwrite(&b, (bfd_size_type)1UL, abfd) != 1)) {
        return FALSE;
      }
    }

  /* Make sure the relocations are aligned.  We do NOT need to make
     sure that this byte exists, because it will only matter if there
     really are relocs.  */
  sofar = (file_ptr)BFD_ALIGN(sofar,
                              (1 << COFF_DEFAULT_SECTION_ALIGNMENT_POWER));

  obj_relocbase(abfd) = (long int)sofar;
  abfd->output_has_begun = TRUE;

  return TRUE;
}

#ifdef COFF_IMAGE_WITH_PE

static unsigned int pelength;
static unsigned int peheader;

/* */
static bfd_boolean
coff_read_word(bfd *abfd, unsigned int *value)
{
  unsigned char b[2];
  int status;

  status = (int)bfd_bread(b, (bfd_size_type)2UL, abfd);
  if (status < 1)
    {
      *value = 0U;
      return FALSE;
    }

  if (status == 1)
    *value = (unsigned int)b[0];
  else
    *value = (unsigned int)(b[0] + (b[1] << 8U));

  pelength += (unsigned int)status;

  return TRUE;
}

/* */
static unsigned int
coff_compute_checksum(bfd *abfd)
{
  bfd_boolean more_data;
  file_ptr filepos;
  unsigned int value;
  unsigned int total;

  total = 0;
  pelength = 0;
  filepos = (file_ptr)0L;

  do {
    if (bfd_seek(abfd, filepos, SEEK_SET) != 0)
      return 0;

    more_data = coff_read_word(abfd, &value);
    total += value;
    total = (0xffff & (total + (total >> 0x10)));
    filepos += 2;
  } while (more_data);

  return (0xffff & (total + (total >> 0x10)));
}

/* */
static bfd_boolean
coff_apply_checksum(bfd *abfd)
{
  unsigned int computed;
  unsigned int checksum = 0U;

  if (bfd_seek(abfd, (file_ptr)0x3c, SEEK_SET) != 0) {
    return FALSE;
  }

  if (!coff_read_word(abfd, &peheader)) {
    return FALSE;
  }

  if (bfd_seek(abfd, (file_ptr)(peheader + 0x58), SEEK_SET) != 0) {
    return FALSE;
  }

  checksum = 0;
  bfd_bwrite(&checksum, (bfd_size_type)4UL, abfd);

  if (bfd_seek(abfd, (file_ptr)peheader, SEEK_SET) != 0) {
    return FALSE;
  }

  computed = coff_compute_checksum(abfd);

  checksum = (computed + pelength);

  if (bfd_seek(abfd, (file_ptr)(peheader + 0x58), SEEK_SET) != 0) {
    return FALSE;
  }

  bfd_bwrite(&checksum, (bfd_size_type)4UL, abfd);

  return TRUE;
}
#endif /* COFF_IMAGE_WITH_PE */

/* */
static bfd_boolean
coff_write_object_contents(bfd *abfd)
{
  asection *current;
  bfd_boolean hasrelocs = FALSE;
  bfd_boolean haslinno = FALSE;
  bfd_boolean hasdebug = FALSE;
  file_ptr scn_base;
  file_ptr reloc_base;
  file_ptr lineno_base;
  file_ptr sym_base;
  unsigned long reloc_size = 0UL, reloc_count = 0UL;
  unsigned long lnno_size = 0UL;
  bfd_boolean long_section_names;
  asection *text_sec = NULL;
  asection *data_sec = NULL;
  asection *bss_sec = NULL;
  struct internal_filehdr internal_f;
  struct internal_aouthdr internal_a;
#ifdef COFF_LONG_SECTION_NAMES
  size_t string_size = STRING_SIZE_SIZE;
#endif /* COFF_LONG_SECTION_NAMES */

  bfd_set_error(bfd_error_system_call);

  /* Make a pass through the symbol table to count line number entries and
     put them into the correct asections.  */
  lnno_size = ((unsigned int)coff_count_linenumbers(abfd)
               * bfd_coff_linesz(abfd));

  if (! abfd->output_has_begun)
    {
	if (! coff_compute_section_file_positions(abfd)) {
            return FALSE;
        }
    }

  reloc_base = obj_relocbase(abfd);

  /* Work out the size of the reloc and linno areas: */
  for (current = abfd->sections; current != NULL; current =
       current->next)
    {
#ifdef COFF_WITH_PE
      /* We store the actual reloc count in the first reloc's addr: */
      if (obj_pe(abfd) && (current->reloc_count >= 0xffff)) {
        reloc_count++;
      }
#endif /* COFF_WITH_PE */
      reloc_count += current->reloc_count;
    }

  reloc_size = (reloc_count * bfd_coff_relsz(abfd));

  lineno_base = (reloc_base + (file_ptr)reloc_size);
  sym_base = (lineno_base + (file_ptr)lnno_size);

  /* Indicate in each section->line_filepos its actual file address: */
  for (current = abfd->sections; current != NULL; current =
       current->next)
    {
      if (current->lineno_count)
	{
	  current->line_filepos = lineno_base;
	  current->moving_line_filepos = lineno_base;
	  lineno_base += (current->lineno_count * bfd_coff_linesz(abfd));
	} else {
          current->line_filepos = 0;
        }

      if (current->reloc_count)
	{
	  current->rel_filepos = reloc_base;
	  reloc_base += (current->reloc_count * bfd_coff_relsz(abfd));
#ifdef COFF_WITH_PE
	  /* Extra reloc to hold real count.  */
          if (obj_pe (abfd) && current->reloc_count >= 0xffff) {
            reloc_base += bfd_coff_relsz(abfd);
          }
#endif /* COFF_WITH_PE */
	} else {
          current->rel_filepos = 0;
	}
    }

  /* Write section headers to the file: */
  internal_f.f_nscns = 0;

  if ((abfd->flags & EXEC_P) != 0)
    scn_base = (bfd_coff_filhsz(abfd) + bfd_coff_aoutsz(abfd));
  else
    {
      scn_base = bfd_coff_filhsz(abfd);
#ifdef RS6000COFF_C
# ifndef XCOFF64
      if (xcoff_data(abfd)->full_aouthdr) {
        scn_base += bfd_coff_aoutsz(abfd);
      } else {
        scn_base += SMALL_AOUTSZ;
      }
# endif /* XCOFF64 */
#endif /* RS6000COFF_C */
    }

  if (bfd_seek(abfd, scn_base, SEEK_SET) != 0) {
    return FALSE;
  }

  long_section_names = FALSE;
  for (current = abfd->sections;
       current != NULL;
       current = current->next)
    {
      struct internal_scnhdr section;
      bfd_boolean is_reloc_section = FALSE;

#ifdef COFF_IMAGE_WITH_PE
      if (strcmp(current->name, ".reloc") == 0)
	{
	  is_reloc_section = TRUE;
	  hasrelocs = TRUE;
	  pe_data(abfd)->has_reloc_section = 1;
	}
#endif /* COFF_IMAGE_WITH_PE */

      internal_f.f_nscns++;

      strncpy(section.s_name, current->name, ((size_t)SCNNMLEN - 1UL));

#ifdef COFF_LONG_SECTION_NAMES
      /* Handle long section names as in PE. This must be compatible
       * with the code in coff_write_symbols and _bfd_coff_final_link. */
      {
	size_t len;

	len = strlen(current->name);
	if (len > SCNNMLEN)
	  {
	    memset(section.s_name, 0, (size_t)SCNNMLEN);
	    /* FIXME: -Wformat-overflow... increase SNNMLEN? */
	    snprintf(section.s_name, (size_t)SCNNMLEN, "/%zu", string_size);
	    string_size += len + 1;
	    long_section_names = TRUE;
	  }
      }
#endif /* COFF_LONG_SECTION_NAMES */

#ifdef _LIB
      /* Always set s_vaddr of .lib to 0. This is right for SVR3.2
       * Ian Taylor <ian@cygnus.com>.  */
      if (strcmp (current->name, _LIB) == 0) {
        section.s_vaddr = 0;
      } else
#endif /* _LIB */
      section.s_vaddr = current->vma;
      section.s_paddr = current->lma;
      section.s_size =  current->size;
#ifdef coff_get_section_load_page
      section.s_page = (unsigned char)coff_get_section_load_page(current);
#endif /* coff_get_section_load_page */

#ifdef COFF_WITH_PE
      section.s_paddr = 0;
#endif /* COFF_WITH_PE */
#ifdef COFF_IMAGE_WITH_PE
      /* Reminder: s_paddr holds the virtual size of the section: */
      if ((coff_section_data(abfd, current) != NULL)
          && (pei_section_data(abfd, current) != NULL)) {
        section.s_paddr = pei_section_data(abfd, current)->virt_size;
      } else {
        section.s_paddr = 0;
      }
#endif /* COFF_IMAGE_WITH_PE */

      /* If this section has no size or is unloadable then the scnptr
       * will be 0 too.  */
      if ((current->size == 0)
	  || ((current->flags & (SEC_LOAD | SEC_HAS_CONTENTS)) == 0)) {
	section.s_scnptr = 0;
      } else {
	section.s_scnptr = (bfd_vma)current->filepos;
      }

      section.s_relptr = (bfd_vma)current->rel_filepos;
      section.s_lnnoptr = (bfd_vma)current->line_filepos;
      section.s_nreloc = current->reloc_count;
      section.s_nlnno = current->lineno_count;
#ifndef COFF_IMAGE_WITH_PE
      /* In PEI, relocs come in the .reloc section.  */
      if (current->reloc_count != 0) {
        hasrelocs = TRUE;
      }
#endif /* !COFF_IMAGE_WITH_PE */
      if (current->lineno_count != 0) {
        haslinno = TRUE;
      }
      if (((current->flags & SEC_DEBUGGING) != 0)
          && ! is_reloc_section) {
        hasdebug = TRUE;
      }

#ifdef RS6000COFF_C
# ifndef XCOFF64
      /* Indicate the use of an XCOFF overflow section header: */
      if ((current->reloc_count >= 0xffff) || (current->lineno_count >= 0xffff))
	{
	  section.s_nreloc = 0xffff;
	  section.s_nlnno = 0xffff;
	}
# endif /* XCOFF64 */
#endif /* RS6000COFF_C */

      section.s_flags = sec_to_styp_flags(current->name, current->flags);

      if (!strcmp(current->name, _TEXT)) {
        text_sec = current;
      } else if (!strcmp(current->name, _DATA)) {
        data_sec = current;
      } else if (!strcmp(current->name, _BSS)) {
        bss_sec = current;
      }

#ifdef I960
      section.s_align = (current->alignment_power
			 ? (1 << current->alignment_power) : 0);
#endif /* I960 */
#ifdef TIC80COFF
      /* TI COFF puts the alignment power in bits 8-11 of the flags: */
      section.s_flags |= (long)((current->alignment_power & 0xF) << 8);
#endif /* TIC80COFF */
#ifdef COFF_ENCODE_ALIGNMENT
      COFF_ENCODE_ALIGNMENT(section, current->alignment_power);
#endif /* COFF_ENCODE_ALIGNMENT */

#ifdef COFF_IMAGE_WITH_PE
      /* Suppress output of the sections if they are null. ld
       * includes the bss and data sections even if there is no size
       * assigned to them. NT loader does NOT like it if these section
       * headers are included if the sections themselves are not
       * needed. See also coff_compute_section_file_positions. */
      if (section.s_size == 0) {
        internal_f.f_nscns--;
      } else
#endif /* COFF_IMAGE_WITH_PE */
	{
	  SCNHDR buff;
	  bfd_size_type amt = bfd_coff_scnhsz(abfd);

	  if ((coff_swap_scnhdr_out(abfd, &section, &buff) == 0)
	      || (bfd_bwrite(& buff, amt, abfd) != amt)) {
            return FALSE;
	  }
	}

#ifdef COFF_WITH_PE
      /* PE stores COMDAT section information in the symbol table.  If
         this section is supposed to have some COMDAT info, track down
         the symbol in the symbol table and modify it.  */
      if ((current->flags & SEC_LINK_ONCE) != 0)
	{
	  unsigned int i, count;
	  asymbol **psym;
	  coff_symbol_type *csym = NULL;
	  asymbol **psymsec;

	  psymsec = NULL;
	  count = bfd_get_symcount (abfd);
	  for (i = 0, psym = abfd->outsymbols; i < count; i++, psym++)
	    {
              if ((*psym)->section != current) {
                  continue;
              }

              /* Remember the location of the first symbol in this
               * section.  */
              if (psymsec == NULL) {
                  psymsec = psym;
              }

	      /* See if this is the section symbol: */
	      if (strcmp((*psym)->name, current->name) == 0)
		{
		  csym = coff_symbol_from(abfd, *psym);
		  if ((csym == NULL) || (csym->native == NULL)
		      || (csym->native->u.syment.n_numaux < 1)
		      || (csym->native->u.syment.n_sclass != C_STAT)
		      || (csym->native->u.syment.n_type != T_NULL)) {
                    continue;
		  }

		  /* Here *PSYM is the section symbol for CURRENT.  */

		  break;
		}
	    }

	  /* Did we find it?
	     Note that we might not if we are converting the file from
	     some other object file format.  */
	  if (i < count)
	    {
	      combined_entry_type *aux;

	      /* We do NOT touch the x_checksum field. The
	       * x_associated field is not currently supported.  */

	      aux = (csym->native + 1);
	      switch (current->flags & SEC_LINK_DUPLICATES) {
		case SEC_LINK_DUPLICATES_DISCARD:
		  aux->u.auxent.x_scn.x_comdat = IMAGE_COMDAT_SELECT_ANY;
		  break;

		case SEC_LINK_DUPLICATES_ONE_ONLY:
		  aux->u.auxent.x_scn.x_comdat =
		    IMAGE_COMDAT_SELECT_NODUPLICATES;
		  break;

		case SEC_LINK_DUPLICATES_SAME_SIZE:
		  aux->u.auxent.x_scn.x_comdat =
		    IMAGE_COMDAT_SELECT_SAME_SIZE;
		  break;

		case SEC_LINK_DUPLICATES_SAME_CONTENTS:
		  aux->u.auxent.x_scn.x_comdat =
		    IMAGE_COMDAT_SELECT_EXACT_MATCH;
		  break;
		default:
		  /* The "pe.h" header says that IMAGE_COMDAT_SELECT_ANY means
		   * "[n]o warning", so use that as the default: */
		  aux->u.auxent.x_scn.x_comdat = IMAGE_COMDAT_SELECT_ANY;
		  break;
	      }

              /* The COMDAT symbol must be the first symbol from this
               * section in the symbol table. In order to make this
               * work, we move the COMDAT symbol before the first
               * symbol we found in the search above. It is OK to
               * rearrange the symbol table at this point, because
               * coff_renumber_symbols is going to rearrange it
               * further and fix up all the aux entries.  */
	      if (psym != psymsec)
		{
		  asymbol *hold;
		  asymbol **pcopy;

		  hold = *psym;
                  for (pcopy = psym; pcopy > psymsec; pcopy--) {
                    pcopy[0] = pcopy[-1];
                  }
		  *psymsec = hold;
		}
	    }
	}
#endif /* COFF_WITH_PE */
    }

#ifdef RS6000COFF_C
# ifndef XCOFF64
  /* XCOFF handles overflows in the reloc and line number count fields
   * by creating a new section header to hold the correct values: */
  for (current = abfd->sections; current != NULL; current = current->next)
    {
      if ((current->reloc_count >= 0xffff) || (current->lineno_count >= 0xffff))
	{
	  struct internal_scnhdr scnhdr;
	  SCNHDR buff;
	  bfd_size_type amt;

	  internal_f.f_nscns++;
	  strncpy(&(scnhdr.s_name[0]), current->name, (8UL - 1UL));
	  scnhdr.s_paddr = current->reloc_count;
	  scnhdr.s_vaddr = current->lineno_count;
	  scnhdr.s_size = 0;
	  scnhdr.s_scnptr = 0;
	  scnhdr.s_relptr = (bfd_vma)current->rel_filepos;
	  scnhdr.s_lnnoptr = (bfd_vma)current->line_filepos;
	  scnhdr.s_nreloc = (unsigned long)current->target_index;
	  scnhdr.s_nlnno = (unsigned long)current->target_index;
	  scnhdr.s_flags = STYP_OVRFLO;
	  amt = bfd_coff_scnhsz (abfd);
	  if ((coff_swap_scnhdr_out(abfd, &scnhdr, &buff) == 0)
	      || (bfd_bwrite(&buff, amt, abfd) != amt)) {
            return FALSE;
	  }
	}
    }
# endif /* !XCOFF64 */
#endif /* RS6000COFF_C */

  /* OK, now set up the filehdr...  */

  /* Do NOT include the internal abs section in the section count */

  /* We will NOT put a fucking timestamp in the header here. Every time you
   * put it back, I will come in and take it out again. I am sorry. This
   * field does not belong here. We fill it with a 0 so it compares the
   * same but is not a reasonable time. -- <gnu@cygnus.com>  */
  internal_f.f_timdat = 0;
  internal_f.f_flags = 0;

  if (abfd->flags & EXEC_P) {
    internal_f.f_opthdr = (unsigned short)bfd_coff_aoutsz(abfd);
  } else {
    internal_f.f_opthdr = 0;
#ifdef RS6000COFF_C
# ifndef XCOFF64
    if (xcoff_data(abfd)->full_aouthdr) {
        internal_f.f_opthdr = (unsigned short)bfd_coff_aoutsz(abfd);
    } else {
        internal_f.f_opthdr = SMALL_AOUTSZ;
    }
# endif /* !XCOFF64 */
#endif /* RS6000COFF_C */
  }

  if (!hasrelocs) {
      internal_f.f_flags |= F_RELFLG;
  }
  if (!haslinno) {
      internal_f.f_flags |= F_LNNO;
  }
  if (abfd->flags & EXEC_P) {
      internal_f.f_flags |= F_EXEC;
  }
#ifdef COFF_IMAGE_WITH_PE
  if (! hasdebug) {
      internal_f.f_flags |= IMAGE_FILE_DEBUG_STRIPPED;
  }
  if (pe_data(abfd)->real_flags & IMAGE_FILE_LARGE_ADDRESS_AWARE) {
      internal_f.f_flags |= IMAGE_FILE_LARGE_ADDRESS_AWARE;
  }
#else
  if (! hasdebug) {
     ;
  }
#endif /* COFF_IMAGE_WITH_PE */

#ifdef COFF_WITH_PE
  internal_f.f_flags |= IMAGE_FILE_32BIT_MACHINE;
#else
  if (bfd_little_endian (abfd)) {
      internal_f.f_flags |= F_AR32WR;
  } else {
      internal_f.f_flags |= F_AR32W;
  }
#endif /* COFF_WITH_PE */

#ifdef TI_TARGET_ID
  /* Target id is used in TI COFF v1 and later; COFF0 will NOT use this
   * field, but it does NOT hurt to set it internally: */
  internal_f.f_target_id = TI_TARGET_ID;
#endif /* TI_TARGET_ID */
#ifdef TIC80_TARGET_ID
  internal_f.f_target_id = TIC80_TARGET_ID;
#endif /* TIC80_TARGET_ID */

  /* FIXME, should do something about the other byte orders and
   * architectures.  */

#ifdef RS6000COFF_C
  if ((abfd->flags & DYNAMIC) != 0) {
      internal_f.f_flags |= F_SHROBJ;
  }
  if (bfd_get_section_by_name (abfd, _LOADER) != NULL) {
      internal_f.f_flags |= F_DYNLOAD;
  }
#endif /* RS6000COFF_C */

  memset(&internal_a, 0, sizeof(internal_a));

  /* Set up architecture-dependent stuff: */
  {
    unsigned int magic = 0U;
    unsigned short flags = 0U;

    coff_set_flags(abfd, &magic, &flags);
    internal_f.f_magic = (unsigned short)magic;
    internal_f.f_flags |= flags;
    /* ...and the "opt"hdr...  */

#if defined(A29K) || (defined(NMAGIC) && (NMAGIC == 0410) && defined(A29K_MAGIC_BIG))
# if defined(ULTRA3) || (defined(SHMAGIC) && (SHMAGIC == 0406)) /* NYU's machine */
    /* FIXME: This is a bogus check. I really want to see if there
     * is a .shbss or a .shdata section, if so then set the magic
     * number to indicate a shared data executable.  */
    if (internal_f.f_nscns >= 7) {
	internal_a.magic = SHMAGIC; /* Shared magic.  */
    } else
# endif /* ULTRA3 */
    internal_a.magic = NMAGIC; /* Assume separate i/d.  */
# define __A_MAGIC_SET__ 01
#endif /* A29K */

#ifdef TICOFF_AOUT_MAGIC
    internal_a.magic = TICOFF_AOUT_MAGIC;
# define __A_MAGIC_SET__ 02
#endif /* TICOFF_AOUT_MAGIC */

#if defined(TIC80COFF) || defined(TIC80_ARCH_MAGIC)
    internal_a.magic = TIC80_ARCH_MAGIC;
# define __A_MAGIC_SET__ 03
#endif /* TIC80 */

#ifdef I860
    /* FIXME: What are the a.out magic numbers for the i860?  */
    internal_a.magic = 0;
# define __A_MAGIC_SET__ 04
#endif /* I860 */

#if defined(I960) || (defined(I960ROMAGIC) && defined(NMAGIC) && defined(OMAGIC))
    internal_a.magic = ((magic == I960ROMAGIC) ? NMAGIC : OMAGIC);
# define __A_MAGIC_SET__ 05
#endif /* I960 */

#if (defined(M88) && M88) || (defined(PAGEMAGICBCS) && PAGEMAGICBCS)
# define __A_MAGIC_SET__ 06
    internal_a.magic = PAGEMAGICBCS;
#endif /* M88 */

#if (defined(APOLLO_M68) && APOLLO_M68) || (defined(APOLLO_COFF_VERSION_NUMBER) && APOLLO_COFF_VERSION_NUMBER)
# define __A_MAGIC_SET__ 07
    internal_a.magic = APOLLO_COFF_VERSION_NUMBER;
#endif /* APOLLO_M68 */

#if defined(M68) || defined(WE32K) || defined(M68K)
# define __A_MAGIC_SET__ 08
# if defined(LYNXOS) || (defined(LYNXCOFFMAGIC) && (LYNXCOFFMAGIC == 0415))
    internal_a.magic = LYNXCOFFMAGIC;
# else
#  if defined(TARG_AUX) || \
      (defined(D_PAGED) && defined(PAGEMAGICPEXECPAGED) && defined(WP_TEXT) && defined(PAGEMAGICPEXECSWAPPED))
    internal_a.magic = ((abfd->flags & D_PAGED) ? PAGEMAGICPEXECPAGED :
			((abfd->flags & WP_TEXT) ? PAGEMAGICPEXECSWAPPED :
                         PAGEMAGICEXECSWAPPED));
#  else
#   if defined(PAGEMAGICPEXECPAGED)
    internal_a.magic = PAGEMAGICPEXECPAGED;
#   endif /* PAGEMAGICPEXECPAGED */
#  endif /* TARG_AUX */
# endif /* LYNXOS */
#endif /* M68 || WE32K || M68K */

#if defined(ARM) || (defined(ZMAGIC) && (ZMAGIC == 0413) && defined(ARMMAGIC))
# define __A_MAGIC_SET__ 09
    internal_a.magic = ZMAGIC;
#endif /* ARM */

#if defined(PPC_PE) || (defined(PPCMAGIC) && defined(IMAGE_NT_OPTIONAL_HDR_MAGIC) && (IMAGE_NT_OPTIONAL_HDR_MAGIC == 0x10b))
# define __A_MAGIC_SET__ 10
    internal_a.magic = IMAGE_NT_OPTIONAL_HDR_MAGIC;
#endif /* PPC_PE */

#if defined(MCORE_PE) || (defined(PEMCORE) && defined(IMAGE_NT_OPTIONAL_HDR_MAGIC) && (IMAGE_NT_OPTIONAL_HDR_MAGIC == 0x10b))
# define __A_MAGIC_SET__ 11
    internal_a.magic = IMAGE_NT_OPTIONAL_HDR_MAGIC;
#endif /* MCORE_PE */

#if (defined(__i386) || defined(__i386__)) && \
    ((defined(LYNXCOFFMAGIC) && defined(I386BADMAG)) || \
     (defined(ZMAGIC) && (ZMAGIC == 0413) && defined(I386MAGIC))) \
    && !defined(I386)
# define I386 1
#endif /* (__i386 || __i386__) && (LYNXCOFFMAGIC || ZMAGIC) && !I386 */

#if defined(I386)
# define __A_MAGIC_SET__ 12
# if defined(LYNXOS) || defined(LYNXCOFFMAGIC)
    internal_a.magic = LYNXCOFFMAGIC;
# else  /* LYNXOS */
    internal_a.magic = ZMAGIC;
# endif /* LYNXOS */
#endif /* I386 */

#if defined(IA64) || (defined(ZMAGIC) && (ZMAGIC == 0413) && defined(IA64MAGIC))
# define __A_MAGIC_SET__ 13
    internal_a.magic = ZMAGIC;
#endif /* IA64 */

#if defined(SPARC)
# define __A_MAGIC_SET__ 14
# if defined(LYNXOS) || (defined(LYNXCOFFMAGIC) && (LYNXCOFFMAGIC == 0415))
    internal_a.magic = LYNXCOFFMAGIC;
# endif /* LYNXOS */
#endif /* SPARC */

#ifdef RS6000COFF_C
# define __A_MAGIC_SET__ 15
# ifndef RS6K_AOUTHDR_ZMAGIC
#  define RS6K_AOUTHDR_ZMAGIC 0x010B /* paged: text r/o, both page-aligned */
# endif /* !RS6K_AOUTHDR_ZMAGIC */
# ifndef RS6K_AOUTHDR_NMAGIC
#  define RS6K_AOUTHDR_NMAGIC 0x0108 /* new: text r/o, data r/w */
# endif /* !RS6K_AOUTHDR_NMAGIC */
# ifndef RS6K_AOUTHDR_OMAGIC
#  define RS6K_AOUTHDR_OMAGIC 0x0107 /* old: text & data writeable */
# endif /* !RS6K_AOUTHDR_OMAGIC */
    internal_a.magic = ((abfd->flags & D_PAGED) ? RS6K_AOUTHDR_ZMAGIC :
                        ((abfd->flags & WP_TEXT) ? RS6K_AOUTHDR_NMAGIC :
                         RS6K_AOUTHDR_OMAGIC));
#endif /* RS6000COFF_C */

#if (defined(SH) && defined(COFF_WITH_PE)) || (defined(SH_PE_MAGIC) && (SH_PE_MAGIC == 0x010b))
# define __A_MAGIC_SET__ 16
    internal_a.magic = SH_PE_MAGIC;
#endif /* SH && COFF_WITH_PE */

#if (defined(MIPS) && defined(COFF_WITH_PE)) || (defined(MIPS_PE_MAGIC) && (MIPS_PE_MAGIC == 0x010b))
# define __A_MAGIC_SET__ 17
    internal_a.magic = MIPS_PE_MAGIC;
#endif /* MIPS && COFF_WITH_PE */

#if defined(OR32) || (defined(NMAGIC) && (NMAGIC == 0410) && defined(OR32_MAGIC_BIG))
# define __A_MAGIC_SET__ 18
    internal_a.magic = NMAGIC; /* Assume separate i/d.  */
#endif /* OR32 */

#ifdef MAXQ20MAGIC
# define __A_MAGIC_SET__ 19
      internal_a.magic = MAXQ20MAGIC;
#endif /* MAXQ20MAGIC */

#ifndef __A_MAGIC_SET__
/* Is it correct to use "#include" instead of "#error" or "#warning" here? */
# include "Your aouthdr magic number is not being set!"
#else
# undef __A_MAGIC_SET__
#endif /* !__A_MAGIC_SET__ */
  }

  /* FIXME: Does anybody ever set this to another value?  */
  internal_a.vstamp = 0;

  /* Now should write relocs, strings, syms: */
  obj_sym_filepos(abfd) = sym_base;

  if (bfd_get_symcount(abfd) != 0) {
      int firstundef;

      if (!coff_renumber_symbols(abfd, &firstundef)) {
          return FALSE;
      }
      coff_mangle_symbols(abfd);
      if (! coff_write_symbols(abfd)) {
          return FALSE;
      }
      if (! coff_write_linenumbers(abfd)) {
          return FALSE;
      }
      if (! coff_write_relocs(abfd, firstundef)) {
          return FALSE;
      }
  }
#ifdef COFF_LONG_SECTION_NAMES
  else if (long_section_names && ! obj_coff_strings_written(abfd)) {
      /* If we have long section names, then we have to write out the string
       * table even if there are no symbols: */
      if (! coff_write_symbols(abfd)) {
          return FALSE;
      }
  }
#endif /* COFF_LONG_SECTION_NAMES */
#ifdef COFF_IMAGE_WITH_PE
# ifdef PPC_PE
  else if ((abfd->flags & EXEC_P) != 0)
    {
      bfd_byte b;

      /* PowerPC PE appears to require that all executable files be
       * rounded up to the page size: */
      b = 0;
      if ((bfd_seek(abfd,
                   ((file_ptr)BFD_ALIGN(sym_base, COFF_PAGE_SIZE) - 1),
                    SEEK_SET) != 0)
	  || (bfd_bwrite(&b, (bfd_size_type)1, abfd) != 1))
	return FALSE;
    }
# endif /* PPC_PE */
#endif /* COFF_IMAGE_WITH_PE */

  /* If bfd_get_symcount (abfd) != 0, then we are not using the COFF
   * backend linker, and obj_raw_syment_count is not valid until after
   * coff_write_symbols is called: */
  if (obj_raw_syment_count(abfd) != 0)
    {
      internal_f.f_symptr = (bfd_vma)sym_base;
#ifdef RS6000COFF_C
      /* AIX appears to require that F_RELFLG not be set if there are
       * local symbols but no relocations: */
      internal_f.f_flags &= (unsigned short)~F_RELFLG;
#endif /* RS6000COFF_C */
    }
  else
    {
      if (long_section_names) {
          internal_f.f_symptr = (bfd_vma)sym_base;
      } else {
          internal_f.f_symptr = 0;
      }
      internal_f.f_flags |= F_LSYMS;
    }

  if (text_sec)
    {
      internal_a.tsize = text_sec->size;
      internal_a.text_start = (internal_a.tsize ? text_sec->vma : 0);
    }
  if (data_sec)
    {
      internal_a.dsize = data_sec->size;
      internal_a.data_start = (internal_a.dsize ? data_sec->vma : 0);
    }
  if (bss_sec)
    {
      internal_a.bsize = bss_sec->size;
      if (internal_a.bsize && (bss_sec->vma < internal_a.data_start))
	internal_a.data_start = bss_sec->vma;
    }

  internal_a.entry = bfd_get_start_address(abfd);
  internal_f.f_nsyms = (long)obj_raw_syment_count(abfd);

#ifdef RS6000COFF_C
  if (xcoff_data(abfd)->full_aouthdr)
    {
      bfd_vma toc;
      asection *loader_sec;

      internal_a.vstamp = 1;

      internal_a.o_snentry = (short)xcoff_data(abfd)->snentry;
      if (internal_a.o_snentry == 0)
	internal_a.entry = (bfd_vma)-1L;

      if (text_sec != NULL)
	{
	  internal_a.o_sntext = (short)text_sec->target_index;
	  internal_a.o_algntext =
            (short)bfd_get_section_alignment(abfd, text_sec);
	}
      else
	{
	  internal_a.o_sntext = 0;
	  internal_a.o_algntext = 0;
	}
      if (data_sec != NULL)
	{
	  internal_a.o_sndata = (short)data_sec->target_index;
	  internal_a.o_algndata =
            (short)bfd_get_section_alignment(abfd, data_sec);
	}
      else
	{
	  internal_a.o_sndata = 0;
	  internal_a.o_algndata = 0;
	}
      loader_sec = bfd_get_section_by_name(abfd, ".loader");
      if (loader_sec != NULL) {
          internal_a.o_snloader = (short)loader_sec->target_index;
      } else {
          internal_a.o_snloader = 0;
      }
      if (bss_sec != NULL) {
          internal_a.o_snbss = (short)bss_sec->target_index;
      } else {
          internal_a.o_snbss = 0;
      }

      toc = xcoff_data(abfd)->toc;
      internal_a.o_toc = toc;
      internal_a.o_sntoc = (short)xcoff_data(abfd)->sntoc;

      internal_a.o_modtype = xcoff_data (abfd)->modtype;
      if (xcoff_data (abfd)->cputype != -1) {
          internal_a.o_cputype = xcoff_data(abfd)->cputype;
      } else {
	  switch (bfd_get_arch(abfd))
	    {
	    case bfd_arch_rs6000:
	      internal_a.o_cputype = 4;
	      break;
	    case bfd_arch_powerpc:
              if (bfd_get_mach(abfd) == bfd_mach_ppc) {
                internal_a.o_cputype = 3;
              } else {
                internal_a.o_cputype = 1;
              }
	      break;
	    default:
	      abort();
	    }
	}
      internal_a.o_maxstack = xcoff_data(abfd)->maxstack;
      internal_a.o_maxdata = xcoff_data(abfd)->maxdata;
    }
#endif /* RS6000COFF_C */

  /* Now write them: */
  if (bfd_seek(abfd, (file_ptr)0, SEEK_SET) != 0) {
      return FALSE;
  }

  {
    char * buff;
    bfd_size_type amount = bfd_coff_filhsz(abfd);

    buff = (char *)bfd_malloc(amount);
    if (buff == NULL) {
	return FALSE;
    }

    bfd_coff_swap_filehdr_out(abfd, & internal_f, buff);
    amount = bfd_bwrite(buff, amount, abfd);

    free(buff);

    if (amount != bfd_coff_filhsz(abfd)) {
        return FALSE;
    }
  }

  if (abfd->flags & EXEC_P)
    {
      /* Note that peicode.h fills in a PEAOUTHDR, not an AOUTHDR.
       * include/coff/pe.h sets AOUTSZ == sizeof (PEAOUTHDR)).  */
      char * buff;
      bfd_size_type amount = bfd_coff_aoutsz (abfd);

      buff = (char *)bfd_malloc(amount);
      if (buff == NULL) {
	  return FALSE;
      }

      coff_swap_aouthdr_out(abfd, & internal_a, buff);
      amount = bfd_bwrite(buff, amount, abfd);

      free(buff);

      if (amount != bfd_coff_aoutsz(abfd)) {
          return FALSE;
      }

#ifdef COFF_IMAGE_WITH_PE
      if (! coff_apply_checksum(abfd)) {
          return FALSE;
      }
#endif /* COFF_IMAGE_WITH_PE */
    }
#ifdef RS6000COFF_C
  else
    {
      AOUTHDR buff;
      size_t size;

      /* XCOFF seems to always write at least a small a.out header: */
      coff_swap_aouthdr_out(abfd, & internal_a, & buff);
      if (xcoff_data (abfd)->full_aouthdr) {
        size = bfd_coff_aoutsz(abfd);
      } else {
        size = SMALL_AOUTSZ;
      }
      if (bfd_bwrite(& buff, (bfd_size_type)size, abfd) != size) {
        return FALSE;
      }
    }
#endif /* RS6000COFF_C */

  return TRUE;
}
/* end of coff_write_object_contents() */

/* */
static ATTRIBUTE_USED bfd_boolean
coff_set_section_contents(bfd *abfd, sec_ptr section,
                          const void *location, file_ptr offset,
                          bfd_size_type count)
{
  if (! abfd->output_has_begun)	/* (Set by bfd.c handler.) */
    {
	if (! coff_compute_section_file_positions(abfd)) {
            return FALSE;
	}
    }

#if defined(_LIB) && !defined(TARG_AUX)
   /* The physical address field of a .lib section is used to hold the
      number of shared libraries in the section.  This code counts the
      number of sections being written, and increments the lma field
      with the number.

      I have found no documentation on the contents of this section.
      Experimentation indicates that the section contains zero or more
      records, each of which has the following structure:

      - a (four byte) word holding the length of this record, in words,
      - a word that always seems to be set to "2",
      - the path to a shared library, null-terminated and then padded
        to a whole word boundary.

      bfd_assert calls have been added to alert if an attempt is made
      to write a section which does NOT follow these assumptions. The
      code has been tested on ISC 4.1 by me, and on SCO by Robert Lipe
      <robertl@arnet.com> (Thanks!).

      Gvran Uddeborg <gvran@uddeborg.pp.se>.  */
    if (strcmp(section->name, _LIB) == 0)
      {
	bfd_byte *rec, *recend;

	rec = (bfd_byte *)location;
	recend = (rec + count);
	while (rec < recend)
	  {
	    ++section->lma;
	    rec += (bfd_get_32(abfd, rec) * 4);
	  }

	BFD_ASSERT(rec == recend);
      }
#endif /* _LIB && !TARG_AUX */

  /* Do NOT write out bss sections - one way to do this is to
   * see if the filepos has not been set: */
  if (section->filepos == 0) {
    return TRUE;
  }

  if (bfd_seek(abfd, (section->filepos + offset), SEEK_SET) != 0) {
    return FALSE;
  }

  if (count == 0) {
    return TRUE;
  }

  return (bfd_bwrite(location, count, abfd) == count);
}

static void *
buy_and_read(bfd *abfd, file_ptr where, bfd_size_type size)
{
  void *area = bfd_alloc(abfd, size);

  if (!area) {
    return (NULL);
  }
  if ((bfd_seek(abfd, where, SEEK_SET) != 0)
      || (bfd_bread(area, size, abfd) != size)) {
    return (NULL);
  }
  return(area);
}

/*
SUBSUBSECTION
	Reading linenumbers

	Creating the linenumber table is done by reading in the entire
	coff linenumber table, and creating another table for internal use.

	A coff linenumber table is structured so that each function
	is marked as having a line number of 0. Each line within the
	function is an offset from the first line in the function. The
	base of the line number information for the table is stored in
	the symbol associated with the function.

	Note: The PE format uses line number 0 for a flag indicating a
	new source file.

	The information is copied from the external to the internal
	table, and each symbol which marks a function is marked by
	pointing its...

	How does this work ?
*/

static bfd_boolean
coff_slurp_line_table(bfd *abfd, asection *asect)
{
  LINENO *native_lineno;
  alent *lineno_cache;
  bfd_size_type amt;

  BFD_ASSERT(asect->lineno == NULL);

  amt = ((bfd_size_type)bfd_coff_linesz(abfd) * asect->lineno_count);
  native_lineno = (LINENO *)buy_and_read(abfd, asect->line_filepos, amt);
  if (native_lineno == NULL) {
      (*_bfd_error_handler)
        (_("%B: warning: line number table read failed"), abfd);
      return FALSE;
  }
  amt = (((bfd_size_type)asect->lineno_count + 1) * sizeof(alent));
  lineno_cache = (alent *)bfd_alloc(abfd, amt);
  if (lineno_cache == NULL) {
      return FALSE;
  } else {
      unsigned int counter = 0U;
      alent *cache_ptr = lineno_cache;
      LINENO *src = native_lineno;

      while (counter < asect->lineno_count)
	{
	  struct internal_lineno dst;

	  bfd_coff_swap_lineno_in(abfd, src, &dst);
	  cache_ptr->line_number = (unsigned int)dst.l_lnno;

	  if (cache_ptr->line_number == 0)
	    {
	      bfd_boolean warned;
	      bfd_signed_vma symndx;
	      coff_symbol_type *sym;

	      warned = FALSE;
	      symndx = dst.l_addr.l_symndx;
	      if ((symndx < 0L)
		  || ((bfd_vma)symndx >= obj_raw_syment_count(abfd)))
		{
		  (*_bfd_error_handler)
		    (_("%B: warning: illegal symbol index %ld in line numbers"),
		     abfd, dst.l_addr.l_symndx);
		  symndx = 0;
		  warned = TRUE;
		}
	      /* FIXME: We should not be casting between ints and
               * pointers like this: */
	      sym =
                ((coff_symbol_type *)
                 ((symndx + obj_raw_syments(abfd))->u.syment._n._n_n._n_zeroes));
	      cache_ptr->u.sym = (asymbol *)sym;
	      if ((sym->lineno != NULL) && ! warned)
		{
		  (*_bfd_error_handler)
		    (_("%B: warning: duplicate line number information for `%s'"),
		     abfd, bfd_asymbol_name(&sym->symbol));
		}
	      sym->lineno = cache_ptr;
	    }
	  else
	    cache_ptr->u.offset = ((bfd_vma)dst.l_addr.l_paddr
                                   - bfd_section_vma(abfd, asect));

	  cache_ptr++;
	  src++;
	  counter++;
	}
      cache_ptr->line_number = 0;

    }
  asect->lineno = lineno_cache;
  /* FIXME: free native_lineno here, or use alloca or something: */
  return TRUE;
}

/* Slurp in the symbol table, converting it to generic form.  Note that
 * if coff_relocate_section is defined, then the linker will read symbols
 * via coff_link_add_symbols, rather than via this routine: */
static bfd_boolean
coff_slurp_symbol_table(bfd *abfd)
{
  combined_entry_type *native_symbols;
  coff_symbol_type *cached_area;
  unsigned int *table_ptr;
  bfd_size_type amt;
  unsigned int number_of_symbols = 0;

  if (obj_symbols(abfd)) {
      return TRUE;
  }

  /* Read in the symbol table: */
  if ((native_symbols = coff_get_normalized_symtab(abfd)) == NULL) {
      return FALSE;
  }

  /* Allocate enough room for all the symbols in cached form: */
  amt = obj_raw_syment_count(abfd);
  amt *= sizeof(coff_symbol_type);
  cached_area = (coff_symbol_type *)bfd_alloc(abfd, amt);
  if (cached_area == NULL) {
      return FALSE;
  }

  amt = obj_raw_syment_count(abfd);
  amt *= sizeof(unsigned int);
  table_ptr = (unsigned int *)bfd_alloc(abfd, amt);

  if (table_ptr == NULL) {
      return FALSE;
  } else {
      coff_symbol_type *dst = cached_area;
      unsigned int last_native_index = (unsigned int)obj_raw_syment_count(abfd);
      unsigned int this_index = 0U;

      while (this_index < last_native_index) {
	  combined_entry_type *src = (native_symbols + this_index);
	  table_ptr[this_index] = number_of_symbols;
	  dst->symbol.the_bfd = abfd;

	  dst->symbol.name = (char *)(src->u.syment._n._n_n._n_offset);
	  /* We use the native name field to point to the cached field.: */
	  src->u.syment._n._n_n._n_zeroes = (long)dst;
	  dst->symbol.section = coff_section_from_bfd_index(abfd,
                                                            src->u.syment.n_scnum);
	  dst->symbol.flags = 0;
	  dst->done_lineno = FALSE;

	  switch (src->u.syment.n_sclass)
	    {
#ifdef I960
	    case C_LEAFEXT:
	      /* Fall through to next case.  */
#endif /* I960 */

	    case C_EXT:
	    case C_WEAKEXT:
#if defined ARM
            case C_THUMBEXT:
            case C_THUMBEXTFUNC:
#endif /* ARM */
#ifdef RS6000COFF_C
	    case C_HIDEXT:
#endif /* RS6000COFF_C */
#ifdef C_SYSTEM
	    case C_SYSTEM:	/* System Wide variable.  */
#endif /* C_SYSTEM */
#ifdef COFF_WITH_PE
            /* In PE, 0x68 (104) denotes a section symbol.  */
            case C_SECTION:
	    /* In PE, 0x69 (105) denotes a weak external symbol.  */
	    case C_NT_WEAK:
#endif /* COFF_WITH_PE */
	      switch (coff_classify_symbol(abfd, &src->u.syment)) {
		case COFF_SYMBOL_GLOBAL:
		  dst->symbol.flags = (BSF_EXPORT | BSF_GLOBAL);
#if defined COFF_WITH_PE
		  /* PE sets the symbol to a value relative to the start
		   * of the section: */
		  dst->symbol.value = src->u.syment.n_value;
#else
		  dst->symbol.value = (src->u.syment.n_value
				       - dst->symbol.section->vma);
#endif /* COFF_WITH_PE */
		  if (ISFCN ((src->u.syment.n_type))) {
		      /* A function ext does not go at the end of a
		       * file.  */
		      dst->symbol.flags |= (BSF_NOT_AT_END | BSF_FUNCTION);
		  }
		  break;

		case COFF_SYMBOL_COMMON:
		  dst->symbol.section = bfd_com_section_ptr;
		  dst->symbol.value = src->u.syment.n_value;
		  break;

		case COFF_SYMBOL_UNDEFINED:
		  dst->symbol.section = bfd_und_section_ptr;
		  dst->symbol.value = 0;
		  break;

		case COFF_SYMBOL_PE_SECTION:
		  dst->symbol.flags |= (BSF_EXPORT | BSF_SECTION_SYM);
		  dst->symbol.value = 0;
		  break;

		case COFF_SYMBOL_LOCAL:
		  dst->symbol.flags = BSF_LOCAL;
#if defined COFF_WITH_PE
		  /* PE sets the symbol to a value relative to the
		   * start of the section.  */
		  dst->symbol.value = src->u.syment.n_value;
#else
		  dst->symbol.value = (src->u.syment.n_value
				       - dst->symbol.section->vma);
#endif /* COFF_WITH_PE */
		  if (ISFCN((src->u.syment.n_type))) {
		      dst->symbol.flags |= (BSF_NOT_AT_END | BSF_FUNCTION);
		  }
		  break;
		default:
		  dst->symbol.value = 0;
		  break;
	      } /* end switch */

#ifdef RS6000COFF_C
	      /* A symbol with a csect entry should not go at the end: */
              if (src->u.syment.n_numaux > 0) {
                dst->symbol.flags |= BSF_NOT_AT_END;
              }
#endif /* RS6000COFF_C */

#ifdef COFF_WITH_PE
              if (src->u.syment.n_sclass == C_NT_WEAK) {
                dst->symbol.flags |= BSF_WEAK;
              }

	      if ((src->u.syment.n_sclass == C_SECTION)
                  && (src->u.syment.n_scnum > 0)) {
                dst->symbol.flags = BSF_LOCAL;
              }
#endif /* COFF_WITH_PE */
              if (src->u.syment.n_sclass == C_WEAKEXT) {
		dst->symbol.flags |= BSF_WEAK;
              }

	      break;

	    case C_STAT:	 /* Static.  */
#ifdef I960
	    case C_LEAFSTAT:	 /* Static leaf procedure.  */
#endif /* I960 */
#if defined ARM
            case C_THUMBSTAT:    /* Thumb static.  */
            case C_THUMBLABEL:   /* Thumb label.  */
            case C_THUMBSTATFUNC: /* Thumb static function.  */
#endif /* ARM */
	    case C_LABEL:	 /* Label.  */
              if (src->u.syment.n_scnum == N_DEBUG) {
                dst->symbol.flags = BSF_DEBUGGING;
              } else {
                dst->symbol.flags = BSF_LOCAL;
              }

              /* Base the value as an index from the base of the
               * section, if there is one.  */
	      if (dst->symbol.section)
		{
#if defined COFF_WITH_PE
		  /* PE sets the symbol to a value relative to the
                     start of the section.  */
		  dst->symbol.value = src->u.syment.n_value;
#else
		  dst->symbol.value = (src->u.syment.n_value
				       - dst->symbol.section->vma);
#endif /* COFF_WITH_PE */
		} else {
                  dst->symbol.value = src->u.syment.n_value;
		}
	      break;

	    case C_MOS:		/* Member of structure.  */
	    case C_EOS:		/* End of structure.  */
#ifdef NOTDEF			/* C_AUTOARG has the same value.  */
# ifdef C_GLBLREG
	    case C_GLBLREG:	/* A29k-specific storage class.  */
# endif /* C_GLBLREG */
#endif /* NOTDEF */
	    case C_REGPARM:	/* Register parameter.  */
	    case C_REG:		/* register variable.  */
              /* C_AUTOARG conflicts with TI COFF C_UEXT.  */
#if !defined (TIC80COFF) && !defined (TICOFF)
# ifdef C_AUTOARG
	    case C_AUTOARG:	/* 960-specific storage class.  */
# endif /* C_AUTOARG */
#endif /* !TIC80COFF && !TICOFF */
	    case C_TPDEF:	/* Type definition.  */
	    case C_ARG:
	    case C_AUTO:	/* Automatic variable.  */
	    case C_FIELD:	/* Bit field.  */
	    case C_ENTAG:	/* Enumeration tag.  */
	    case C_MOE:		/* Member of enumeration.  */
	    case C_MOU:		/* Member of union.  */
	    case C_UNTAG:	/* Union tag.  */
	      dst->symbol.flags = BSF_DEBUGGING;
	      dst->symbol.value = (src->u.syment.n_value);
	      break;

	    case C_FILE:	/* File name.  */
	    case C_STRTAG:	/* Structure tag.  */
#ifdef RS6000COFF_C
	    case C_GSYM:
	    case C_LSYM:
	    case C_PSYM:
	    case C_RSYM:
	    case C_RPSYM:
	    case C_STSYM:
	    case C_TCSYM:
	    case C_BCOMM:
	    case C_ECOML:
	    case C_ECOMM:
	    case C_DECL:
	    case C_ENTRY:
	    case C_FUN:
	    case C_ESTAT:
#endif /* RS6000COFF_C */
	      dst->symbol.flags = BSF_DEBUGGING;
	      dst->symbol.value = (src->u.syment.n_value);
	      break;

#ifdef RS6000COFF_C
	    case C_BINCL:	/* Beginning of include file.  */
	    case C_EINCL:	/* Ending of include file.  */
              /* The value is actually a pointer into the line numbers
               * of the file. We locate the line number entry, and
               * set the section to the section which contains it, and
               * the value to the index in that section. */
	      {
		asection *sec;

		dst->symbol.flags = BSF_DEBUGGING;
		for (sec = abfd->sections; sec != NULL; sec = sec->next) {
		  if ((sec->line_filepos <= (file_ptr)src->u.syment.n_value)
		      && ((file_ptr)(sec->line_filepos
                                     + (sec->lineno_count * bfd_coff_linesz(abfd)))
                          > (file_ptr)src->u.syment.n_value))
		    break;

                  if (sec == NULL) {
                    dst->symbol.value = 0;
                  } else {
                    dst->symbol.section = sec;
                    dst->symbol.value = ((src->u.syment.n_value
                                          - (bfd_vma)sec->line_filepos)
                                         / bfd_coff_linesz(abfd));
                    src->fix_line = 1;
                  }
                }
	      }
	      break;

	    case C_BSTAT:
	      dst->symbol.flags = BSF_DEBUGGING;

              /* The value is actually a symbol index.  Save a pointer
               * to the symbol instead of the index.
               * FIXME: This should use a union: */
	      src->u.syment.n_value =
		(bfd_vma)(long)(native_symbols + src->u.syment.n_value);
              /* The double-cast is unfortunate, but necessary in order to
               * properly silence warnings... */
	      dst->symbol.value = src->u.syment.n_value;
	      src->fix_value = 1;
	      break;
#endif /* RS6000COFF_C */

	    case C_BLOCK:	/* ".bb" or ".eb".  */
	    case C_FCN:		/* ".bf" or ".ef" (or PE ".lf").  */
	    case C_EFCN:	/* Physical end of function.  */
#if defined COFF_WITH_PE
              /* PE sets the symbol to a value relative to the start
               * of the section.  */
	      dst->symbol.value = src->u.syment.n_value;
	      if (strcmp(dst->symbol.name, ".bf") != 0)
		{
		  /* PE uses funny values for .ef and .lf; do NOT
		   * relocate them.  */
		  dst->symbol.flags = BSF_DEBUGGING;
		} else {
                  dst->symbol.flags = (BSF_DEBUGGING | BSF_DEBUGGING_RELOC);
		}
#else
              /* Base the value as an index from the base of the
               * section. */
	      dst->symbol.flags = BSF_LOCAL;
	      dst->symbol.value = (src->u.syment.n_value
				   - dst->symbol.section->vma);
#endif /* COFF_WITH_PE */
	      break;

	    case C_STATLAB:	/* Static load time label.  */
              dst->symbol.value = src->u.syment.n_value;
              dst->symbol.flags = BSF_GLOBAL;
              break;

	    case C_NULL:
              /* PE DLLs sometimes have zeroed out symbols for some
               * reason.  Just ignore them without a warning: */
	      if ((src->u.syment.n_type == 0)
                  && (src->u.syment.n_value == 0)
                  && (src->u.syment.n_scnum == 0)) {
                break;
              }
	      ATTRIBUTE_FALLTHROUGH;
              /* Fall through to: */
	    case C_EXTDEF:    /* External definition.  */
	    case C_ULABEL:    /* Undefined label.  */
	    case C_USTATIC:   /* Undefined static.  */
#ifndef COFF_WITH_PE
            /* C_LINE in regular coff is 0x68. NT has taken over this storage
             * class to represent a section symbol.  */
	    case C_LINE:   /* line # reformatted as symbol table entry.  */
	      /* NT uses 0x67 for a weak symbol, not C_ALIAS.  */
	    case C_ALIAS:	/* Duplicate tag.  */
#endif /* COFF_WITH_PE */
	      /* New storage classes for TI COFF.  */
#if defined(TIC80COFF) || defined(TICOFF)
	    case C_UEXT:	/* Tentative external definition.  */
#endif /* TIC80COFF || TICOFF */
	    case C_EXTLAB:	/* External load time label.  */
	    case C_HIDDEN:	/* Ext symbol in dmert public lib.  */
	    default:
	      (*_bfd_error_handler)
		(_("%B: Unrecognized storage class %d for %s symbol `%s'"),
		 abfd, src->u.syment.n_sclass,
		 dst->symbol.section->name, dst->symbol.name);
	      dst->symbol.flags = BSF_DEBUGGING;
	      dst->symbol.value = (src->u.syment.n_value);
	      break;
	    }

	  dst->native = src;

	  dst->symbol.udata.i = 0;
	  dst->lineno = NULL;
	  this_index += (unsigned int)((src->u.syment.n_numaux) + 1U);
	  dst++;
	  number_of_symbols++;
	}
    }

  obj_symbols(abfd) = cached_area;
  obj_raw_syments(abfd) = native_symbols;

  bfd_get_symcount(abfd) = number_of_symbols;
  obj_convert(abfd) = table_ptr;
  /* Slurp the line tables for each section, too: */
  {
    asection *p;

    p = abfd->sections;
    while (p)
      {
	coff_slurp_line_table(abfd, p);
	p = p->next;
      }
  }

  return TRUE;
}

/* Classify a COFF symbol.  A couple of targets have globally visible
 * symbols which are not class C_EXT, and this handles those.  It also
 * recognizes some special PE cases: */
static enum coff_symbol_classification
coff_classify_symbol(bfd *abfd, struct internal_syment *syment)
{
  /* FIXME: This partially duplicates the switch in
   * coff_slurp_symbol_table: */
  switch (syment->n_sclass)
    {
    case C_EXT:
    case C_WEAKEXT:
#ifdef I960
    case C_LEAFEXT:
#endif /* I960 */
#ifdef ARM
    case C_THUMBEXT:
    case C_THUMBEXTFUNC:
#endif /* ARM */
#ifdef C_SYSTEM
    case C_SYSTEM:
#endif /* C_SYSTEM */
#ifdef COFF_WITH_PE
    case C_NT_WEAK:
#endif /* COFF_WITH_PE */
      if (syment->n_scnum == 0)
	{
          if (syment->n_value == 0) {
            return COFF_SYMBOL_UNDEFINED;
          } else {
            return COFF_SYMBOL_COMMON;
          }
	}
      return COFF_SYMBOL_GLOBAL;

    default:
      break;
    }

#ifdef COFF_WITH_PE
  if (syment->n_sclass == C_STAT)
    {
      if (syment->n_scnum == 0)
	/* The Microsoft compiler sometimes generates these if a
	 * small static function is inlined every time it is used.
	 * The function is discarded, but the symbol table entry
	 * remains.  */
	return COFF_SYMBOL_LOCAL;

# ifdef STRICT_PE_FORMAT
      /* This is correct for Microsoft generated objects, but it
       * breaks gas generated objects.  */
      if (syment->n_value == 0)
	{
	  asection *sec;
	  char buf[SYMNMLEN + 1];

	  sec = coff_section_from_bfd_index(abfd, syment->n_scnum);
	  if ((sec != NULL)
	      && (strcmp(bfd_get_section_name(abfd, sec),
                         _bfd_coff_internal_syment_name(abfd, syment, buf))
		  == 0))
	    return COFF_SYMBOL_PE_SECTION;
	}
# endif /* STRICT_PE_FORMAT */

      return COFF_SYMBOL_LOCAL;
    }

  if (syment->n_sclass == C_SECTION)
    {
      /* In some cases in a DLL generated by the Microsoft linker, the
         n_value field will contain garbage.  FIXME: This should
         probably be handled by the swapping function instead.  */
      syment->n_value = 0;
      if (syment->n_scnum == 0) {
        return COFF_SYMBOL_UNDEFINED;
      }
      return COFF_SYMBOL_PE_SECTION;
    }
#endif /* COFF_WITH_PE */

  /* If it is not a global symbol, we presume it is a local symbol: */
  if (syment->n_scnum == 0)
    {
      char buf[SYMNMLEN + 1];

      (*_bfd_error_handler)
	(_("warning: %B: local symbol `%s' has no section"),
	 abfd, _bfd_coff_internal_syment_name(abfd, syment, buf));
    }

  return COFF_SYMBOL_LOCAL;
}

/*
SUBSUBSECTION
	Reading relocations

	Coff relocations are easily transformed into the internal BFD form
	(@code{arelent}).

	Reading a coff relocation table is done in the following stages:

	o Read the entire coff relocation table into memory.

	o Process each relocation in turn; first swap it from the
	external to the internal form.

	o Turn the symbol referenced in the relocation's symbol index
	into a pointer into the canonical symbol table.
	This table is the same as the one returned by a call to
	@code{bfd_canonicalize_symtab}. The back end will call that
	routine and save the result if a canonicalization has NOT been done.

	o The reloc index is turned into a pointer to a howto
	structure, in a back end specific way. For instance, the 386
	and 960 use the @code{r_type} to directly produce an index
	into a howto table vector; the 88k subtracts a number from the
	@code{r_type} field and creates an addend field.
*/

#ifndef CALC_ADDEND
# define CALC_ADDEND(abfd, ptr, reloc, cache_ptr)                \
  {                                                             \
    coff_symbol_type *coffsym = NULL;      			\
                                                                \
    if (ptr && bfd_asymbol_bfd (ptr) != abfd)                   \
      coffsym = (obj_symbols (abfd)                             \
                 + (cache_ptr->sym_ptr_ptr - symbols));         \
    else if (ptr)                                               \
      coffsym = coff_symbol_from (abfd, ptr);                   \
    if (coffsym != NULL				                \
        && coffsym->native->u.syment.n_scnum == 0)              \
      cache_ptr->addend = 0;                                    \
    else if (ptr && bfd_asymbol_bfd (ptr) == abfd               \
             && ptr->section != NULL)		                \
      cache_ptr->addend = - (ptr->section->vma + ptr->value);   \
    else                                                        \
      cache_ptr->addend = 0;                                    \
  }
#endif /* !CALC_ADDEND */

static bfd_boolean
coff_slurp_reloc_table(bfd * abfd, sec_ptr asect, asymbol ** symbols)
{
  RELOC *native_relocs;
  arelent *reloc_cache;
  arelent *cache_ptr;
  unsigned int idx;
  bfd_size_type amt;

  if (asect->relocation) {
    return TRUE;
  }
  if (asect->reloc_count == 0) {
    return TRUE;
  }
  if (asect->flags & SEC_CONSTRUCTOR) {
    return TRUE;
  }
  if (!coff_slurp_symbol_table (abfd)) {
    return FALSE;
  }

  amt = ((bfd_size_type)bfd_coff_relsz(abfd) * asect->reloc_count);
  native_relocs = (RELOC *)buy_and_read(abfd, asect->rel_filepos, amt);
  amt = ((bfd_size_type)asect->reloc_count * sizeof(arelent));
  reloc_cache = (arelent *)bfd_alloc(abfd, amt);

  if ((reloc_cache == NULL) || (native_relocs == NULL)) {
    return FALSE;
  }

  for (idx = 0; idx < asect->reloc_count; idx++)
    {
      struct internal_reloc dst;
      struct external_reloc *src;
#ifndef RELOC_PROCESSING
      asymbol *ptr;
#endif /* RELOC_PROCESSING */

      cache_ptr = (reloc_cache + idx);
      src = (native_relocs + idx);

      coff_swap_reloc_in(abfd, src, &dst);

#ifdef RELOC_PROCESSING
      RELOC_PROCESSING(cache_ptr, &dst, symbols, abfd, asect);
#else
      cache_ptr->address = dst.r_vaddr;

      if (dst.r_symndx != -1)
	{
	  if ((dst.r_symndx < 0) || (dst.r_symndx >= obj_conv_table_size(abfd)))
	    {
	      (*_bfd_error_handler)
		(_("%B: warning: illegal symbol index %ld in relocs"),
		 abfd, dst.r_symndx);
	      cache_ptr->sym_ptr_ptr = bfd_abs_section_ptr->symbol_ptr_ptr;
	      ptr = NULL;
	    }
	  else
	    {
	      cache_ptr->sym_ptr_ptr = (symbols
					+ obj_convert(abfd)[dst.r_symndx]);
	      ptr = *(cache_ptr->sym_ptr_ptr);
	    }
	}
      else
	{
	  cache_ptr->sym_ptr_ptr = bfd_abs_section_ptr->symbol_ptr_ptr;
	  ptr = NULL;
	}

      /* The symbols definitions that we have read in have been
       * relocated as if their sections started at 0. But the offsets
       * refering to the symbols in the raw data have not been
       * modified, so we have to have a negative addend to compensate.
       *
       * Note that symbols which used to be common must be left alone. */

      /* Calculate any reloc addend by looking at the symbol: */
      CALC_ADDEND(abfd, ptr, dst, cache_ptr);

      cache_ptr->address -= asect->vma;
      /* !! cache_ptr->section = NULL;*/

      /* Fill in the cache_ptr->howto field from dst.r_type: */
      RTYPE2HOWTO(cache_ptr, &dst);
      
      if (ptr == NULL) {
	; /* ??? */
      }
#endif	/* RELOC_PROCESSING */

      if (cache_ptr->howto == NULL)
	{
	  (*_bfd_error_handler)
	    (_("%B: illegal relocation type %d at address 0x%lx"),
	     abfd, dst.r_type, (long)dst.r_vaddr);
	  bfd_set_error(bfd_error_bad_value);
	  return FALSE;
	}
    }

  asect->relocation = reloc_cache;
  return TRUE;
}

#ifndef coff_rtype_to_howto
# ifdef RTYPE2HOWTO
/* Get the howto structure for a reloc.  This is only used if the file
 * including this one defines coff_relocate_section to be
 * _bfd_coff_generic_relocate_section, so it is OK if it does not
 * always work.  It is the responsibility of the including file to
 * make sure it is reasonable if it is needed: */
static reloc_howto_type *
coff_rtype_to_howto(bfd *abfd ATTRIBUTE_UNUSED,
                    asection *sec ATTRIBUTE_UNUSED,
                    struct internal_reloc *rel,
                    struct coff_link_hash_entry *h ATTRIBUTE_UNUSED,
                    struct internal_syment *sym ATTRIBUTE_UNUSED,
                    bfd_vma *addendp ATTRIBUTE_UNUSED)
{
  arelent genrel = {
    (struct bfd_symbol **)NULL, 0UL, 0UL, (reloc_howto_type *)NULL
  };

  RTYPE2HOWTO(&genrel, rel);
  return genrel.howto;
}
# else /* ! defined (RTYPE2HOWTO) */
#  define coff_rtype_to_howto NULL
# endif /* ! defined (RTYPE2HOWTO) */
#endif /* ! defined (coff_rtype_to_howto) */

/* This is stupid. This function should be a boolean predicate: */
static ATTRIBUTE_USED long
coff_canonicalize_reloc(bfd *abfd, sec_ptr section, arelent **relptr,
			asymbol **symbols)
{
  arelent *tblptr = section->relocation;
  unsigned int count = 0U;

  if (tblptr == NULL) {
      (void)tblptr;
  }
  if (section->flags & SEC_CONSTRUCTOR) {
      /* This section has relocs made up by us, they are not in the file,
       * so take them out of their chain and place them
       * into the data area provided: */
      arelent_chain *chain = section->constructor_chain;

      for ((count = 0); (count < section->reloc_count); count++) {
	  *relptr++ = &chain->relent;
	  chain = chain->next;
      }
  } else {
      if (! coff_slurp_reloc_table(abfd, section, symbols)) {
	  return -1;
      }

      tblptr = section->relocation;

      for (; (count++ < section->reloc_count);) {
	  *relptr++ = tblptr++;
      }
  }
  *relptr = 0;
  return (long)section->reloc_count;
}

#ifndef coff_reloc16_estimate
# define coff_reloc16_estimate dummy_reloc16_estimate
static int ATTRIBUTE_NORETURN
dummy_reloc16_estimate(bfd *abfd ATTRIBUTE_UNUSED,
		       asection *input_section ATTRIBUTE_UNUSED,
		       arelent *reloc ATTRIBUTE_UNUSED,
		       unsigned int shrink ATTRIBUTE_UNUSED,
		       struct bfd_link_info *link_info ATTRIBUTE_UNUSED)
{
# ifdef ATTRIBUTE_NORETURN
  abort();
# else
  return 0;
# endif /* ATTRIBUTE_NORETURN */
}
#endif /* !coff_reloc16_estimate */

#ifndef coff_reloc16_extra_cases
# define coff_reloc16_extra_cases dummy_reloc16_extra_cases
/* This works even if abort is not declared in any header file: */
static void ATTRIBUTE_NORETURN
dummy_reloc16_extra_cases(bfd *abfd ATTRIBUTE_UNUSED,
			  struct bfd_link_info *link_info ATTRIBUTE_UNUSED,
			  struct bfd_link_order *link_order ATTRIBUTE_UNUSED,
			  arelent *reloc ATTRIBUTE_UNUSED,
			  bfd_byte *data ATTRIBUTE_UNUSED,
			  unsigned int *src_ptr ATTRIBUTE_UNUSED,
			  unsigned int *dst_ptr ATTRIBUTE_UNUSED)
{
  abort();
}
#endif /* !coff_reloc16_extra_cases */

#ifndef coff_bfd_link_hash_table_free
# define coff_bfd_link_hash_table_free _bfd_generic_link_hash_table_free
#endif /* !coff_bfd_link_hash_table_free */

/* If coff_relocate_section is defined, we can use the optimized COFF
 * backend linker. Otherwise we must continue to use the old linker.  */
#ifdef coff_relocate_section
# ifndef coff_bfd_link_hash_table_create
#  define coff_bfd_link_hash_table_create _bfd_coff_link_hash_table_create
# endif /* !coff_bfd_link_hash_table_create */
# ifndef coff_bfd_link_add_symbols
#  define coff_bfd_link_add_symbols _bfd_coff_link_add_symbols
# endif /* !coff_bfd_link_add_symbols */
# ifndef coff_bfd_final_link
#  define coff_bfd_final_link _bfd_coff_final_link
# endif /* !coff_bfd_final_link */
#else /* ! defined (coff_relocate_section) */
# define coff_relocate_section NULL
# ifndef coff_bfd_link_hash_table_create
#  define coff_bfd_link_hash_table_create _bfd_generic_link_hash_table_create
# endif /* !coff_bfd_link_hash_table_create */
# ifndef coff_bfd_link_add_symbols
#  define coff_bfd_link_add_symbols _bfd_generic_link_add_symbols
# endif /* !coff_bfd_link_add_symbols */
# define coff_bfd_final_link _bfd_generic_final_link
#endif /* ! defined (coff_relocate_section) */

#define coff_bfd_link_just_syms      _bfd_generic_link_just_syms
#define coff_bfd_link_split_section  _bfd_generic_link_split_section

#ifndef coff_start_final_link
# define coff_start_final_link NULL
#endif /* !coff_start_final_link */

#ifndef coff_adjust_symndx
# define coff_adjust_symndx NULL
#endif /* !coff_adjust_symndx */

#ifndef coff_link_add_one_symbol
# define coff_link_add_one_symbol _bfd_generic_link_add_one_symbol
#endif /* !coff_link_add_one_symbol */

#ifndef coff_link_output_has_begun
static bfd_boolean
coff_link_output_has_begun(bfd *abfd,
			   struct coff_final_link_info *info ATTRIBUTE_UNUSED)
{
  return abfd->output_has_begun;
}
#endif /* !coff_link_output_has_begun */

#ifndef coff_final_link_postscript
static bfd_boolean
coff_final_link_postscript(bfd *abfd ATTRIBUTE_UNUSED,
			   struct coff_final_link_info *pfinfo ATTRIBUTE_UNUSED)
{
  return TRUE;
}
#endif /* !coff_final_link_postscript */

#ifndef coff_SWAP_aux_in
# define coff_SWAP_aux_in coff_swap_aux_in
#endif /* !coff_SWAP_aux_in */
#ifndef coff_SWAP_sym_in
# define coff_SWAP_sym_in coff_swap_sym_in
#endif /* !coff_SWAP_sym_in */
#ifndef coff_SWAP_lineno_in
# define coff_SWAP_lineno_in coff_swap_lineno_in
#endif /* !coff_SWAP_lineno_in */
#ifndef coff_SWAP_aux_out
# define coff_SWAP_aux_out coff_swap_aux_out
#endif /* !coff_SWAP_aux_out */
#ifndef coff_SWAP_sym_out
# define coff_SWAP_sym_out coff_swap_sym_out
#endif /* !coff_SWAP_sym_out */
#ifndef coff_SWAP_lineno_out
# define coff_SWAP_lineno_out coff_swap_lineno_out
#endif /* !coff_SWAP_lineno_out */
#ifndef coff_SWAP_reloc_out
# define coff_SWAP_reloc_out coff_swap_reloc_out
#endif /* !coff_SWAP_reloc_out */
#ifndef coff_SWAP_filehdr_out
# define coff_SWAP_filehdr_out coff_swap_filehdr_out
#endif /* !coff_SWAP_filehdr_out */
#ifndef coff_SWAP_aouthdr_out
# define coff_SWAP_aouthdr_out coff_swap_aouthdr_out
#endif /* !coff_SWAP_aouthdr_out */
#ifndef coff_SWAP_scnhdr_out
# define coff_SWAP_scnhdr_out coff_swap_scnhdr_out
#endif /* !coff_SWAP_scnhdr_out */
#ifndef coff_SWAP_reloc_in
# define coff_SWAP_reloc_in coff_swap_reloc_in
#endif /* !coff_SWAP_reloc_in */
#ifndef coff_SWAP_filehdr_in
# define coff_SWAP_filehdr_in coff_swap_filehdr_in
#endif /* !coff_SWAP_filehdr_in */
#ifndef coff_SWAP_aouthdr_in
# define coff_SWAP_aouthdr_in coff_swap_aouthdr_in
#endif /* !coff_SWAP_aouthdr_in */
#ifndef coff_SWAP_scnhdr_in
# define coff_SWAP_scnhdr_in coff_swap_scnhdr_in
#endif /* !coff_SWAP_scnhdr_in */

static const bfd_coff_backend_data bfd_coff_std_swap_table ATTRIBUTE_USED =
{
  coff_SWAP_aux_in, coff_SWAP_sym_in, coff_SWAP_lineno_in,
  coff_SWAP_aux_out, coff_SWAP_sym_out,
  coff_SWAP_lineno_out, coff_SWAP_reloc_out,
  coff_SWAP_filehdr_out, coff_SWAP_aouthdr_out,
  coff_SWAP_scnhdr_out,
  FILHSZ, AOUTSZ, SCNHSZ, SYMESZ, AUXESZ, RELSZ, LINESZ, FILNMLEN,
#ifdef COFF_LONG_FILENAMES
  TRUE,
#else
  FALSE,
#endif /* COFF_LONG_FILENAMES */
#ifdef COFF_LONG_SECTION_NAMES
  TRUE,
#else
  FALSE,
#endif /* COFF_LONG_SECTION_NAMES */
  COFF_DEFAULT_SECTION_ALIGNMENT_POWER,
#ifdef COFF_FORCE_SYMBOLS_IN_STRINGS
  TRUE,
#else
  FALSE,
#endif /* COFF_FORCE_SYMBOLS_IN_STRINGS */
#ifdef COFF_DEBUG_STRING_WIDE_PREFIX
  4,
#else
  2,
#endif /* COFF_DEBUG_STRING_WIDE_PREFIX */
  coff_SWAP_filehdr_in, coff_SWAP_aouthdr_in, coff_SWAP_scnhdr_in,
  coff_SWAP_reloc_in, coff_bad_format_hook, coff_set_arch_mach_hook,
  coff_mkobject_hook, styp_to_sec_flags, coff_set_alignment_hook,
  coff_slurp_symbol_table, symname_in_debug_hook, coff_pointerize_aux_hook,
  coff_print_aux, coff_reloc16_extra_cases, coff_reloc16_estimate,
  coff_classify_symbol, coff_compute_section_file_positions,
  coff_start_final_link, coff_relocate_section, coff_rtype_to_howto,
  coff_adjust_symndx, coff_link_add_one_symbol,
  coff_link_output_has_begun, coff_final_link_postscript
};

#ifdef TICOFF
/* COFF0 differs in file/section header size and relocation entry size: */
static const bfd_coff_backend_data ticoff0_swap_table =
{
  coff_SWAP_aux_in, coff_SWAP_sym_in, coff_SWAP_lineno_in,
  coff_SWAP_aux_out, coff_SWAP_sym_out,
  coff_SWAP_lineno_out, coff_SWAP_reloc_out,
  coff_SWAP_filehdr_out, coff_SWAP_aouthdr_out,
  coff_SWAP_scnhdr_out,
  FILHSZ_V0, AOUTSZ, SCNHSZ_V01, SYMESZ, AUXESZ, RELSZ_V0, LINESZ, FILNMLEN,
# ifdef COFF_LONG_FILENAMES
  TRUE,
# else
  FALSE,
# endif /* COFF_LONG_FILENAMES */
# ifdef COFF_LONG_SECTION_NAMES
  TRUE,
# else
  FALSE,
# endif /* COFF_LONG_SECTION_NAMES */
  COFF_DEFAULT_SECTION_ALIGNMENT_POWER,
# ifdef COFF_FORCE_SYMBOLS_IN_STRINGS
  TRUE,
# else
  FALSE,
# endif /* COFF_FORCE_SYMBOLS_IN_STRINGS */
# ifdef COFF_DEBUG_STRING_WIDE_PREFIX
  4,
# else
  2,
# endif /* COFF_DEBUG_STRING_WIDE_PREFIX */
  coff_SWAP_filehdr_in, coff_SWAP_aouthdr_in, coff_SWAP_scnhdr_in,
  coff_SWAP_reloc_in, ticoff0_bad_format_hook, coff_set_arch_mach_hook,
  coff_mkobject_hook, styp_to_sec_flags, coff_set_alignment_hook,
  coff_slurp_symbol_table, symname_in_debug_hook, coff_pointerize_aux_hook,
  coff_print_aux, coff_reloc16_extra_cases, coff_reloc16_estimate,
  coff_classify_symbol, coff_compute_section_file_positions,
  coff_start_final_link, coff_relocate_section, coff_rtype_to_howto,
  coff_adjust_symndx, coff_link_add_one_symbol,
  coff_link_output_has_begun, coff_final_link_postscript
};
#endif /* TICOFF */

#ifdef TICOFF
/* COFF1 differs in section header size: */
static const bfd_coff_backend_data ticoff1_swap_table =
{
  coff_SWAP_aux_in, coff_SWAP_sym_in, coff_SWAP_lineno_in,
  coff_SWAP_aux_out, coff_SWAP_sym_out,
  coff_SWAP_lineno_out, coff_SWAP_reloc_out,
  coff_SWAP_filehdr_out, coff_SWAP_aouthdr_out,
  coff_SWAP_scnhdr_out,
  FILHSZ, AOUTSZ, SCNHSZ_V01, SYMESZ, AUXESZ, RELSZ, LINESZ, FILNMLEN,
# ifdef COFF_LONG_FILENAMES
  TRUE,
# else
  FALSE,
# endif /* COFF_LONG_FILENAMES */
# ifdef COFF_LONG_SECTION_NAMES
  TRUE,
# else
  FALSE,
# endif /* COFF_LONG_SECTION_NAMES */
  COFF_DEFAULT_SECTION_ALIGNMENT_POWER,
# ifdef COFF_FORCE_SYMBOLS_IN_STRINGS
  TRUE,
# else
  FALSE,
# endif /* COFF_FORCE_SYMBOLS_IN_STRINGS */
# ifdef COFF_DEBUG_STRING_WIDE_PREFIX
  4,
# else
  2,
# endif /* COFF_DEBUG_STRING_WIDE_PREFIX */
  coff_SWAP_filehdr_in, coff_SWAP_aouthdr_in, coff_SWAP_scnhdr_in,
  coff_SWAP_reloc_in, ticoff1_bad_format_hook, coff_set_arch_mach_hook,
  coff_mkobject_hook, styp_to_sec_flags, coff_set_alignment_hook,
  coff_slurp_symbol_table, symname_in_debug_hook, coff_pointerize_aux_hook,
  coff_print_aux, coff_reloc16_extra_cases, coff_reloc16_estimate,
  coff_classify_symbol, coff_compute_section_file_positions,
  coff_start_final_link, coff_relocate_section, coff_rtype_to_howto,
  coff_adjust_symndx, coff_link_add_one_symbol,
  coff_link_output_has_begun, coff_final_link_postscript
};
#endif /* TICOFF */

#ifndef coff_close_and_cleanup
# define coff_close_and_cleanup _bfd_generic_close_and_cleanup
#endif /* !coff_close_and_cleanup */

#ifndef coff_bfd_free_cached_info
# define coff_bfd_free_cached_info _bfd_generic_bfd_free_cached_info
#endif /* !coff_bfd_free_cached_info */

#ifndef coff_get_section_contents
# define coff_get_section_contents _bfd_generic_get_section_contents
#endif /* !coff_get_section_contents */

#ifndef coff_bfd_copy_private_symbol_data
# define coff_bfd_copy_private_symbol_data _bfd_generic_bfd_copy_private_symbol_data
#endif /* !coff_bfd_copy_private_symbol_data */

#ifndef coff_bfd_copy_private_header_data
# define coff_bfd_copy_private_header_data _bfd_generic_bfd_copy_private_header_data
#endif /* !coff_bfd_copy_private_header_data */

#ifndef coff_bfd_copy_private_section_data
# define coff_bfd_copy_private_section_data _bfd_generic_bfd_copy_private_section_data
#endif /* !coff_bfd_copy_private_section_data */

#ifndef coff_bfd_copy_private_bfd_data
# define coff_bfd_copy_private_bfd_data _bfd_generic_bfd_copy_private_bfd_data
#endif /* !coff_bfd_copy_private_bfd_data */

#ifndef coff_bfd_merge_private_bfd_data
# define coff_bfd_merge_private_bfd_data _bfd_generic_bfd_merge_private_bfd_data
#endif /* !coff_bfd_merge_private_bfd_data */

#ifndef coff_bfd_set_private_flags
# define coff_bfd_set_private_flags _bfd_generic_bfd_set_private_flags
#endif /* !coff_bfd_set_private_flags */

#ifndef coff_bfd_print_private_bfd_data
# define coff_bfd_print_private_bfd_data _bfd_generic_bfd_print_private_bfd_data
#endif /* !coff_bfd_print_private_bfd_data */

#ifndef coff_bfd_is_local_label_name
# define coff_bfd_is_local_label_name _bfd_coff_is_local_label_name
#endif /* !coff_bfd_is_local_label_name */

#ifndef coff_bfd_is_target_special_symbol
# define coff_bfd_is_target_special_symbol ((bfd_boolean (*)(bfd *, asymbol *))bfd_false)
#endif /* !coff_bfd_is_target_special_symbol */

#ifndef coff_read_minisymbols
# define coff_read_minisymbols _bfd_generic_read_minisymbols
#endif /* !coff_read_minisymbols */

#ifndef coff_minisymbol_to_symbol
# define coff_minisymbol_to_symbol _bfd_generic_minisymbol_to_symbol
#endif /* !coff_minisymbol_to_symbol */

/* The reloc lookup routine must be supplied by each individual COFF
 * backend.  */
#ifndef coff_bfd_reloc_type_lookup
# define coff_bfd_reloc_type_lookup _bfd_norelocs_bfd_reloc_type_lookup
#endif /* !coff_bfd_reloc_type_lookup */

#ifndef coff_bfd_get_relocated_section_contents
# define coff_bfd_get_relocated_section_contents \
         bfd_generic_get_relocated_section_contents
#endif /* !coff_bfd_get_relocated_section_contents */

#ifndef coff_bfd_relax_section
# define coff_bfd_relax_section bfd_generic_relax_section
#endif /* !coff_bfd_relax_section */

#ifndef coff_bfd_gc_sections
# define coff_bfd_gc_sections bfd_generic_gc_sections
#endif /* !coff_bfd_gc_sections */

#ifndef coff_bfd_merge_sections
# define coff_bfd_merge_sections bfd_generic_merge_sections
#endif /* !coff_bfd_merge_sections */

#ifndef coff_bfd_is_group_section
# define coff_bfd_is_group_section bfd_generic_is_group_section
#endif /* !coff_bfd_is_group_section */

#ifndef coff_bfd_discard_group
# define coff_bfd_discard_group bfd_generic_discard_group
#endif /* !coff_bfd_discard_group */

#ifndef coff_section_already_linked
# define coff_section_already_linked \
         _bfd_generic_section_already_linked
#endif /* !coff_section_already_linked */

#ifndef coff_get_section_contents_in_window_with_mode
# define coff_get_section_contents_in_window_with_mode \
   _bfd_generic_get_section_contents_in_window_with_mode
#endif /* !coff_get_section_contents_in_window_with_mode */

#define CREATE_BIG_COFF_TARGET_VEC(VAR, NAME, EXTRA_O_FLAGS, EXTRA_S_FLAGS, UNDER, ALTERNATIVE, SWAP_TABLE)	\
const bfd_target VAR =							\
{									\
  (char *)NAME,                                                         \
  bfd_target_coff_flavour,						\
  BFD_ENDIAN_BIG,		/* Data byte order is big.  */		\
  BFD_ENDIAN_BIG,		/* Header byte order is big.  */	\
  /* object flags */							\
  (HAS_RELOC | EXEC_P | HAS_LINENO | HAS_DEBUG |			\
   HAS_SYMS | HAS_LOCALS | WP_TEXT | EXTRA_O_FLAGS),			\
  /* section flags */							\
  (SEC_HAS_CONTENTS | SEC_ALLOC | SEC_LOAD | SEC_RELOC | EXTRA_S_FLAGS),\
  UNDER,			/* Leading symbol underscore.  */	\
  '/',				/* AR_pad_char.  */			\
  15,				/* AR_max_namelen.  */			\
  									\
  /* Data conversion functions.  */					\
  bfd_getb64, bfd_getb_signed_64, bfd_putb64,				\
  bfd_getb32, bfd_getb_signed_32, bfd_putb32,				\
  bfd_getb16, bfd_getb_signed_16, bfd_putb16,				\
  									\
  /* Header conversion functions.  */					\
  bfd_getb64, bfd_getb_signed_64, bfd_putb64,				\
  bfd_getb32, bfd_getb_signed_32, bfd_putb32,				\
  bfd_getb16, bfd_getb_signed_16, bfd_putb16,				\
									\
	/* bfd_check_format.  */					\
  { _bfd_dummy_target, coff_object_p, bfd_generic_archive_p,		\
    _bfd_dummy_target },						\
	/* bfd_set_format.  */						\
  { bfd_false, coff_mkobject, _bfd_generic_mkarchive, bfd_false },	\
	/* bfd_write_contents.  */					\
  { bfd_false, coff_write_object_contents, _bfd_write_archive_contents,	\
    bfd_false },							\
									\
  BFD_JUMP_TABLE_GENERIC(coff),					\
  BFD_JUMP_TABLE_COPY(coff),						\
  BFD_JUMP_TABLE_CORE(_bfd_nocore),					\
  BFD_JUMP_TABLE_ARCHIVE(_bfd_archive_coff),				\
  BFD_JUMP_TABLE_SYMBOLS(coff),                                         \
  BFD_JUMP_TABLE_RELOCS(coff),						\
  BFD_JUMP_TABLE_WRITE(coff),						\
  BFD_JUMP_TABLE_LINK(coff),						\
  BFD_JUMP_TABLE_DYNAMIC(_bfd_nodynamic),				\
  									\
  ALTERNATIVE,								\
  									\
  SWAP_TABLE								\
};

#define CREATE_BIGHDR_COFF_TARGET_VEC(VAR, NAME, EXTRA_O_FLAGS, EXTRA_S_FLAGS, UNDER, ALTERNATIVE, SWAP_TABLE)	\
const bfd_target VAR =							\
{									\
  NAME,                                                                 \
  bfd_target_coff_flavour,						\
  BFD_ENDIAN_LITTLE,		/* Data byte order is little.  */	\
  BFD_ENDIAN_BIG,		/* Header byte order is big.  */	\
  /* object flags */							\
  (HAS_RELOC | EXEC_P | HAS_LINENO | HAS_DEBUG |			\
   HAS_SYMS | HAS_LOCALS | WP_TEXT | EXTRA_O_FLAGS),			\
  /* section flags */							\
  (SEC_HAS_CONTENTS | SEC_ALLOC | SEC_LOAD | SEC_RELOC | EXTRA_S_FLAGS),\
  UNDER,			/* Leading symbol underscore.  */	\
  '/',				/* AR_pad_char.  */			\
  15,				/* AR_max_namelen.  */			\
  									\
  /* Data conversion functions.  */					\
  bfd_getb64, bfd_getb_signed_64, bfd_putb64,				\
  bfd_getb32, bfd_getb_signed_32, bfd_putb32,				\
  bfd_getb16, bfd_getb_signed_16, bfd_putb16,				\
  									\
  /* Header conversion functions.  */					\
  bfd_getb64, bfd_getb_signed_64, bfd_putb64,				\
  bfd_getb32, bfd_getb_signed_32, bfd_putb32,				\
  bfd_getb16, bfd_getb_signed_16, bfd_putb16,				\
									\
	/* bfd_check_format.  */					\
  { _bfd_dummy_target, coff_object_p, bfd_generic_archive_p,		\
    _bfd_dummy_target },						\
	/* bfd_set_format.  */						\
  { bfd_false, coff_mkobject, _bfd_generic_mkarchive, bfd_false },	\
	/* bfd_write_contents.  */					\
  { bfd_false, coff_write_object_contents, _bfd_write_archive_contents,	\
    bfd_false },							\
									\
  BFD_JUMP_TABLE_GENERIC(coff),                                         \
  BFD_JUMP_TABLE_COPY(coff),						\
  BFD_JUMP_TABLE_CORE(_bfd_nocore),					\
  BFD_JUMP_TABLE_ARCHIVE(_bfd_archive_coff),				\
  BFD_JUMP_TABLE_SYMBOLS(coff),                                         \
  BFD_JUMP_TABLE_RELOCS(coff),						\
  BFD_JUMP_TABLE_WRITE(coff),						\
  BFD_JUMP_TABLE_LINK(coff),						\
  BFD_JUMP_TABLE_DYNAMIC(_bfd_nodynamic),				\
  									\
  ALTERNATIVE,								\
  									\
  SWAP_TABLE								\
};

#define CREATE_LITTLE_COFF_TARGET_VEC(VAR, NAME, EXTRA_O_FLAGS, EXTRA_S_FLAGS, UNDER, ALTERNATIVE, SWAP_TABLE)	\
const bfd_target VAR =							\
{									\
  (char *)NAME,                                                         \
  bfd_target_coff_flavour,						\
  BFD_ENDIAN_LITTLE,		/* Data byte order is little.  */	\
  BFD_ENDIAN_LITTLE,		/* Header byte order is little.  */	\
	/* object flags */						\
  (HAS_RELOC | EXEC_P | HAS_LINENO | HAS_DEBUG |			\
   HAS_SYMS | HAS_LOCALS | WP_TEXT | EXTRA_O_FLAGS),			\
	/* section flags */						\
  (SEC_HAS_CONTENTS | SEC_ALLOC | SEC_LOAD | SEC_RELOC | EXTRA_S_FLAGS),\
  UNDER,			/* Leading symbol underscore.  */	\
  '/',				/* AR_pad_char.  */			\
  15,				/* AR_max_namelen.  */			\
									\
  /* Data conversion functions.  */					\
  bfd_getl64, bfd_getl_signed_64, bfd_putl64,				\
  bfd_getl32, bfd_getl_signed_32, bfd_putl32,				\
  bfd_getl16, bfd_getl_signed_16, bfd_putl16,				\
  /* Header conversion functions.  */					\
  bfd_getl64, bfd_getl_signed_64, bfd_putl64,				\
  bfd_getl32, bfd_getl_signed_32, bfd_putl32,				\
  bfd_getl16, bfd_getl_signed_16, bfd_putl16,				\
	/* bfd_check_format.  */					\
  { _bfd_dummy_target, coff_object_p, bfd_generic_archive_p,		\
    _bfd_dummy_target },						\
       /* bfd_set_format.  */						\
  { bfd_false, coff_mkobject, _bfd_generic_mkarchive, bfd_false },	\
	/* bfd_write_contents.  */					\
  { bfd_false, coff_write_object_contents, _bfd_write_archive_contents,	\
    bfd_false },							\
									\
  BFD_JUMP_TABLE_GENERIC(coff),                                         \
  BFD_JUMP_TABLE_COPY(coff),						\
  BFD_JUMP_TABLE_CORE(_bfd_nocore),					\
  BFD_JUMP_TABLE_ARCHIVE(_bfd_archive_coff),				\
  BFD_JUMP_TABLE_SYMBOLS(coff),                                         \
  BFD_JUMP_TABLE_RELOCS(coff),						\
  BFD_JUMP_TABLE_WRITE(coff),						\
  BFD_JUMP_TABLE_LINK(coff),						\
  BFD_JUMP_TABLE_DYNAMIC(_bfd_nodynamic),				\
									\
  ALTERNATIVE,								\
  									\
  SWAP_TABLE								\
};

#endif /* !__BFD_COFFCODE_H__ */

/* End of coffcode.h */
