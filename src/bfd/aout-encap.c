/* aout-encap.c
 * BFD back-end for a.out files encapsulated with COFF headers.
 */
/* Copyright 1990, 1991, 1994, 1995, 2000, 2001, 2002, 2003
 * Free Software Foundation, Inc. */
/*
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
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

/* THIS MODULE IS NOT FINISHED. IT PROBABLY DOES NOT EVEN COMPILE. */

#if 0 || (!defined(TARGET_PAGE_SIZE) && !defined(SEGMENT_SIZE) && !defined(TEXT_START_ADDR))
# define TARGET_PAGE_SIZE 4096
# if !defined(SEGMENT_SIZE)
#  if defined(TARGET_PAGE_SIZE)
#   define SEGMENT_SIZE TARGET_PAGE_SIZE
#  else
#   define SEGMENT_SIZE 4096
#  endif /* TARGET_PAGE_SIZE */
# endif /* !SEGMENT_SIZE */
# define TEXT_START_ADDR 0
#endif /* 0 && (!TARGET_PAGE_SIZE && !SEGMENT_SIZE && !TEXT_START_ADDR) */

#include "bfd.h"
#include "sysdep.h"
#include "libbfd.h"
#include "aout/aout64.h"
#include "aout/stab_gnu.h"
#include "aout/ar.h"
#include "aout/encap.h"
/* all the targets (i386, m68k, a29k, sparc) that are used elsewhere
 * in this file (aout-encap.c) have headers that define L_LNNO_SIZE as 2,
 * so use that value here, too: */
#ifndef L_LNNO_SIZE
# define L_LNNO_SIZE 2
#endif /* !L_LNNO_SIZE */
#include "coff/external.h"
#include "libaout.h" /* BFD a.out internal data structures */


#ifndef __STRUCT_EXEC_OVERRIDE__
/* This is the layout on disk of a Unix V7, Berkeley, SunOS, Vax Ultrix
 * "struct exec".  Do NOT assume that on this machine, the "struct exec"
 * will lay out the same sizes or alignments.  */
struct exec_bytes {
  unsigned char a_info[4];
  unsigned char a_text[4];
  unsigned char a_data[4];
  unsigned char a_bss[4];
  unsigned char a_syms[4];
  unsigned char a_entry[4];
  unsigned char a_trsize[4];
  unsigned char a_drsize[4];
};

/* How big the "struct exec" is on disk */
#ifndef EXEC_BYTES_SIZE
# define EXEC_BYTES_SIZE (8 * 4)
#endif /* !EXEC_BYTES_SIZE */

/* This is the layout in memory of a "struct exec" while we process it: */
struct exec
{
  unsigned long a_info; /* Use macros N_MAGIC, etc for access */
  unsigned a_text; /* length of text, in bytes */
  unsigned a_data; /* length of data, in bytes */
  unsigned a_bss; /* length of uninitialized data area for file, in bytes */
  unsigned a_syms; /* length of symbol table data in file, in bytes */
  unsigned a_entry; /* start address */
  unsigned a_trsize; /* length of relocation info for text, in bytes */
  unsigned a_drsize; /* length of relocation info for data, in bytes */
};
#endif /* __STRUCT_EXEC_OVERRIDE__ */


#ifndef N_MAGIC
# define N_MAGIC(exec) ((exec).a_info & 0xffff)
#endif /* !N_MAGIC */
#ifndef N_FLAGS
# define N_FLAGS(exec) (((exec).a_info >> 24) & 0xff)
#endif /* !N_FLAGS */
#ifndef N_SET_MAGIC
# define N_SET_MAGIC(exec, magic) \
  ((exec).a_info = (((exec).a_info & 0xffff0000) | ((magic) & 0xffff)))
#endif /* !N_SET_MAGIC */


/* Virtual Address of text segment from the a.out file. For OMAGIC,
 * (almost always "unlinked .o's" these days), should be zero.
 * For linked files, should reflect reality if we know it. */
#ifndef N_TXTADDR
# define N_TXTADDR(x) ((N_MAGIC(x)==OMAGIC) ? 0 : TEXT_START_ADDR)
#endif /* !N_TXTADDR */

#ifndef N_BADMAG
# define N_BADMAG(x) ((N_MAGIC(x) != OMAGIC)     \
		      && (N_MAGIC(x) != NMAGIC)  \
		      && (N_MAGIC(x) != ZMAGIC))
#endif /* !N_BADMAG */

/* This complexity is for encapsulated COFF support: */
#ifndef _N_HDROFF
# define _N_HDROFF(x) (N_SEGSIZE(x) - sizeof(struct exec))
#endif /* !_N_HDROFF */

#ifndef N_TXTOFF
# define N_TXTOFF(x) ((N_MAGIC(x) == ZMAGIC) ?		  \
		      (_N_HDROFF((x)) + sizeof(struct exec)) : \
		      sizeof(struct exec))
#endif /* !N_TXTOFF */

#ifndef N_DATOFF
# define N_DATOFF(x) (N_TXTOFF(x) + (x).a_text)
#endif /* !N_DATOFF */

#ifndef N_TRELOFF
# define N_TRELOFF(x) (N_DATOFF(x) + (x).a_data)
#endif /* !N_TRELOFF */

#ifndef N_DRELOFF
# define N_DRELOFF(x) (N_TRELOFF(x) + (x).a_trsize)
#endif /* !N_DRELOFF */

#ifndef N_SYMOFF
# define N_SYMOFF(x) (N_DRELOFF(x) + (x).a_drsize)
#endif /* !N_SYMOFF */

#ifndef N_STROFF
# define N_STROFF(x)	(N_SYMOFF(x) + (x).a_syms)
#endif /* !N_STROFF */

#ifndef N_DATADDR
# define N_DATADDR(x) \
    ((N_MAGIC(x)==OMAGIC) ? (N_TXTADDR(x)+(x).a_text) \
     :  (N_SEGSIZE(x) + ((N_TXTADDR(x)+(x).a_text-1) & ~(N_SEGSIZE(x)-1))))
#endif /* !N_DATADDR */

/* Address of bss segment in memory after it is loaded: */
#ifndef N_BSSADDR
# define N_BSSADDR(x) (N_DATADDR(x) + (x).a_data)
#endif /* !N_BSSADDR */

struct nlist {
  union {
    char *n_name;
    struct nlist *n_next;
    long n_strx;
  } n_un;
  unsigned char n_type;
  char n_other;
  short n_desc;
  unsigned long n_value;
};

struct reloc_ext_bytes {
  unsigned char	r_address[4];
  unsigned char r_index[3];
  unsigned char r_bits[1];
  unsigned char r_addend[4];
};

struct reloc_info_extended
{
  unsigned long r_address;
  unsigned int  r_index:24;
/* r_index is a struct member, so it is unnecessary to check if it is defined
 * here: */
#if !defined(r_symbolnum)
# define r_symbolnum r_index
#endif /* !r_symbolnum */
  unsigned	r_extern:1;
  unsigned	:2;
  enum reloc_type r_type:5;
  long int	r_addend;
};

/* The standard, old-fashioned, Berkeley compatible relocation struct: */
struct reloc_std_bytes {
  unsigned char	r_address[4];
  unsigned char r_index[3];
  unsigned char r_bits[1];
};

struct relocation_info
{
  /* Address (within segment) to be relocated: */
  int r_address;
  /* The meaning of r_symbolnum depends on r_extern: */
  unsigned int r_symbolnum:24;
  /* Nonzero means value is a pc-relative offset, and it should be relocated
   * for changes in its own address as well as for changes
   * in the symbol or section specified.: */
  unsigned int r_pcrel:1;
  /* Length (as exponent of 2) of the field to be relocated.
   * Thus, a value of 2 indicates 1<<2 bytes: */
  unsigned int r_length:2;
  /* 1 => relocate with value of symbol.
   * r_symbolnum is the index of the symbol in the symbol table for the file.
   * 0 => relocate with the address of a segment.
   * r_symbolnum is N_TEXT, N_DATA, N_BSS or N_ABS
   * (the N_EXT bit may be set also, but signifies nothing): */
  unsigned int r_extern:1;
  /* The next three bits are for SunOS shared libraries, and seem to be
   * undocumented: */
  unsigned int r_baserel:1;	/* Linkage table relative */
  unsigned int r_jmptable:1;	/* pc-relative to jump table */
#ifdef TC_NS32K
/* r_baserel and r_jmptable are struct members, so it should be unnecessary
 * to check if they are defined here: */
# if !defined(r_bsr)
#  define r_bsr r_baserel
# endif /* !r_bsr */
# if !defined(r_disp)
#  define r_disp r_jmptable
# endif /* !r_disp */
#endif /* TC_NS32K */
  unsigned int r_relative:1;	/* "relative relocation" */
  /* unused: */
  unsigned int r_pad:1;		/* Padding -- set to zero */
};

/* Address of text segment in memory after it is loaded. */
#ifndef N_TXTADDR
# define N_TXTADDR(x) \
  (TEXT_START_ADDR + \
  ((N_FLAGS(x) & N_FLAGS_COFF_ENCAPSULATE) ? \
   sizeof (struct coffheader) + sizeof (struct exec) : 0))
#endif /* !N_TXTADDR */

#define N_DATADDR(x) \
  ((N_FLAGS(x) & N_FLAGS_COFF_ENCAPSULATE) ? \
   (SEGMENT_SIZE + ((N_TXTADDR(x)+(x).a_text-1) & ~(SEGMENT_SIZE-1))) : \
   (N_TXTADDR(x)+(x).a_text))

#ifndef COFF_MAGIC_I386
# define COFF_MAGIC_I386 0514 /* I386MAGIC */
#endif /* !COFF_MAGIC_I386 */
#ifndef COFF_MAGIC_M68K
# define COFF_MAGIC_M68K 0520 /* MC68MAGIC */
#endif /* !COFF_MAGIC_M68K */
#ifndef COFF_MAGIC_A29K
# define COFF_MAGIC_A29K 0x17A /* Used by asm29k cross-tools */
#endif /* !COFF_MAGIC_A29K */

#if !defined(COFF_MAGIC)
# if defined(TARGET)
/* Describe some of the parameters of the encapsulation: */
#  if defined(TARGET_I386) && (TARGET == TARGET_I386)
#   ifdef COFF_MAGIC_I386
#    define COFF_MAGIC COFF_MAGIC_I386
#   else
#    define COFF_MAGIC 0514 /* I386MAGIC */
#   endif /* COFF_MAGIC_I386 */
#  endif /* TARGET_I386 */
#  if defined(TARGET_M68K) && (TARGET == TARGET_M68K)
#   ifdef COFF_MAGIC_M68K
#    define COFF_MAGIC COFF_MAGIC_M68K
#   else
#    define COFF_MAGIC 0520 /* MC68MAGIC */
#   endif /* COFF_MAGIC_M68K */
#  endif /* TARGET_M68K */
#  if defined(TARGET_SPARC) && (TARGET == TARGET_SPARC)
#   define COFF_MAGIC UNKNOWN!!! /* Used by TTI */
#  endif /* TARGET_SPARC */
#  if defined(TARGET_AM29K) && (TARGET == TARGET_AM29K)
#   ifdef COFF_MAGIC_A29K
#    define COFF_MAGIC COFF_MAGIC_A29K
#   else
#    define COFF_MAGIC 0x17A /* Used by asm29k cross-tools */
#   endif /* COFF_MAGIC_A29K */
#  endif /* TARGET_AM29K */
# else
#  define COFF_MAGIC 0000 /* Should really be "UNKNOWN!!!" like TTI... */
/* silence some '-Wunused-macros' warnings: */
#  ifdef COFF_MAGIC_I386
#   undef COFF_MAGIC_I386
#  endif /* COFF_MAGIC_I386 */
#  ifdef COFF_MAGIC_M68K
#   undef COFF_MAGIC_M68K
#  endif /* COFF_MAGIC_M68K */
#  ifdef COFF_MAGIC_A29K
#   undef COFF_MAGIC_A29K
#  endif /* COFF_MAGIC_A29K */
# endif /* TARGET */
#endif /* !COFF_MAGIC */


#ifdef COFF_MAGIC
short __header_offset_temp;

/* FIXME, this is dumb. The same tools cannot handle a.outs for different archs,
 * just because COFF_MAGIC is different; so you need a separate GNU nm for each
 * different arch!!? Also note that for expediency, this macros accepts
 * COFF_MAGIC in either byte order. The right thing to do is to call
 * read_aout_header to handle all this. */
# ifndef HEADER_OFFSET
#  define HEADER_OFFSET(f) \
    (__header_offset_temp = 0, \
     fread((char *)&__header_offset_temp, sizeof(short), 1, (f)), \
     fseek((f), -(sizeof(short)), 1), \
     ((__header_offset_temp==COFF_MAGIC) || __header_offset_temp == \
        ((COFF_MAGIC >> 8)|((COFF_MAGIC&0xFF)<<8)) \
            ? sizeof(struct coffheader) : 0))
# endif /* !HEADER_OFFSET */

# ifndef HEADER_OFFSET_FD
#  define HEADER_OFFSET_FD(fd) \
    (__header_offset_temp = 0, \
     read(fd, (char *)&__header_offset_temp, sizeof(short)), \
     lseek((fd), -(sizeof(short)), 1), \
     ((__header_offset_temp==COFF_MAGIC) || __header_offset_temp == \
        ((COFF_MAGIC >> 8)|((COFF_MAGIC&0xFF)<<8)) \
            ? sizeof(struct coffheader) : 0))
# endif /* !HEADER_OFFSET_FD */

#else
# ifndef HEADER_OFFSET
#  define HEADER_OFFSET(f) 0
# endif /* !HEADER_OFFSET */
# ifndef HEADER_OFFSET_FD
#  define HEADER_OFFSET_FD(fd) 0
# endif /* !HEADER_OFFSET_FD */
#endif /* COFF_MAGIC */

#ifndef N_FLAGS_COFF_ENCAPSULATE
# define N_FLAGS_COFF_ENCAPSULATE 0x20 /* coff header precedes bsd header */
#endif /* !N_FLAGS_COFF_ENCAPSULATE */

#ifndef EXEC_BYTES_SIZE
/* How big the "struct exec" is on disk: */
# define EXEC_BYTES_SIZE (8 * 4)
#endif /* !EXEC_BYTES_SIZE */

#ifndef OMAGIC
/* Code indicating object file or impure executable: */
# define OMAGIC 0407
#endif /* !OMAGIC */
#ifndef NMAGIC
/* Code indicating pure executable: */
# define NMAGIC 0410
#endif /* !NMAGIC */
#ifndef ZMAGIC
/* Code indicating demand-paged executable: */
# define ZMAGIC 0413
#endif /* !ZMAGIC */
/* By default, segment size is constant. But on some machines, it can
 * be a function of the a.out header (e.g. machine type). */
#if !defined(N_SEGSIZE) && defined(SEGMENT_SIZE)
# define N_SEGSIZE(x) SEGMENT_SIZE
#endif /* !N_SEGSIZE && SEGMENT_SIZE */
/* Address of text segment in memory after it is loaded. */
#ifndef N_TXTADDR
# define N_TXTADDR(x) 0
#endif /* !N_TXTADDR */

#ifndef N_UNDF
# define N_UNDF 0
#endif /* !N_UNDF */
#ifndef N_ABS
# define N_ABS 2
#endif /* !N_ABS */
#ifndef N_TEXT
# define N_TEXT 4
#endif /* !N_TEXT */
#ifndef N_DATA
# define N_DATA 6
#endif /* !N_DATA */
#ifndef N_BSS
# define N_BSS 8
#endif /* !N_BSS */
#ifndef N_FN
# define N_FN 0x1e
#endif /* !N_FN */

#ifndef N_EXT
# define N_EXT 1
#endif /* !N_EXT */
#ifndef N_TYPE
# define N_TYPE 036
#endif /* !N_TYPE */
#ifndef N_STAB
# define N_STAB 0340
#endif /* !N_STAB */

#ifndef N_INDR
/* The following type indicates the definition of a symbol as being an indirect
 * reference to another symbol. The other symbol appears as an undefined
 * reference, immediately following this symbol.
 *
 * Indirection is asymmetrical.  The other symbol's value will be used
 * to satisfy requests for the indirect symbol, but not vice versa.
 * If the other symbol does not have a definition, libraries will be searched
 * to find a definition. */
# define N_INDR 0xa
#endif /* !N_INDR */

/* The following symbols refer to set elements. All the N_SET[ATDB] symbols
 * with the same name form one set. Space is allocated for the set in the
 * text section, and the value of each set element is stored into 1 word of the
 * space. The 1st word of the space is the length of the set (number of elements).
 *
 * The address of the set is made into an N_SETV symbol whose name is the same
 * as the name of the set. This symbol acts like a N_DATA global symbol in that
 * it can satisfy undefined external references. */

/* These appear as input to LD, in a .o file. */
#ifndef N_SETA
# define N_SETA 0x14 /* Absolute set element symbol */
#endif /* !N_SETA */
#ifndef N_SETT
# define N_SETT 0x16 /* Text set element symbol */
#endif /* !N_SETT */
#ifndef N_SETD
# define N_SETD 0x18 /* Data set element symbol */
#endif /* !N_SETD */
#ifndef N_SETB
# define N_SETB 0x1A /* Bss set element symbol */
#endif /* !N_SETB */

/* This is output from LD. */
#ifndef N_SETV
# define N_SETV	0x1C /* Pointer to set vector in data area. */
#endif /* !N_SETV */

#ifndef RELOC_EXT_BITS_EXTERN_BIG
# define RELOC_EXT_BITS_EXTERN_BIG 0x80
#endif /* !RELOC_EXT_BITS_EXTERN_BIG */
#ifndef RELOC_EXT_BITS_EXTERN_LITTLE
# define RELOC_EXT_BITS_EXTERN_LITTLE 0x01
#endif /* !RELOC_EXT_BITS_EXTERN_LITTLE */

#ifndef RELOC_EXT_BITS_TYPE_BIG
# define RELOC_EXT_BITS_TYPE_BIG 0x1F
#endif /* !RELOC_EXT_BITS_TYPE_BIG */
#ifndef RELOC_EXT_BITS_TYPE_SH_BIG
# define RELOC_EXT_BITS_TYPE_SH_BIG 0
#endif /* !RELOC_EXT_BITS_TYPE_SH_BIG */
#ifndef RELOC_EXT_BITS_TYPE_LITTLE
# define RELOC_EXT_BITS_TYPE_LITTLE 0xF8
#endif /* !RELOC_EXT_BITS_TYPE_LITTLE */
#ifndef RELOC_EXT_BITS_TYPE_SH_LITTLE
# define RELOC_EXT_BITS_TYPE_SH_LITTLE 3
#endif /* !RELOC_EXT_BITS_TYPE_SH_LITTLE */

#ifndef RELOC_EXT_SIZE
# define RELOC_EXT_SIZE 12 /* Bytes per relocation entry */
#endif /* !RELOC_EXT_SIZE */

#ifndef RELOC_STD_BITS_PCREL_BIG
# define RELOC_STD_BITS_PCREL_BIG 0x80
#endif /* !RELOC_STD_BITS_PCREL_BIG */
#ifndef RELOC_STD_BITS_PCREL_LITTLE
# define RELOC_STD_BITS_PCREL_LITTLE 0x01
#endif /* !RELOC_STD_BITS_PCREL_LITTLE */

#ifndef RELOC_STD_BITS_LENGTH_BIG
# define RELOC_STD_BITS_LENGTH_BIG 0x60
#endif /* !RELOC_STD_BITS_LENGTH_BIG */
#ifndef RELOC_STD_BITS_LENGTH_SH_BIG
# define RELOC_STD_BITS_LENGTH_SH_BIG 5 /* To shift to units place */
#endif /* !RELOC_STD_BITS_LENGTH_SH_BIG */
#ifndef RELOC_STD_BITS_LENGTH_LITTLE
# define RELOC_STD_BITS_LENGTH_LITTLE 0x06
#endif /* !RELOC_STD_BITS_LENGTH_LITTLE */
#ifndef RELOC_STD_BITS_LENGTH_SH_LITTLE
# define RELOC_STD_BITS_LENGTH_SH_LITTLE 1
#endif /* !RELOC_STD_BITS_LENGTH_SH_LITTLE */

#ifndef RELOC_STD_BITS_EXTERN_BIG
# define RELOC_STD_BITS_EXTERN_BIG 0x10
#endif /* !RELOC_STD_BITS_EXTERN_BIG */
#ifndef RELOC_STD_BITS_EXTERN_LITTLE
# define RELOC_STD_BITS_EXTERN_LITTLE 0x08
#endif /* !RELOC_STD_BITS_EXTERN_LITTLE */

#ifndef RELOC_STD_BITS_BASEREL_BIG
# define RELOC_STD_BITS_BASEREL_BIG 0x08
#endif /* !RELOC_STD_BITS_BASEREL_BIG */
#ifndef RELOC_STD_BITS_BASEREL_LITTLE
# define RELOC_STD_BITS_BASEREL_LITTLE 0x08
#endif /* !RELOC_STD_BITS_BASEREL_LITTLE */

#ifndef RELOC_STD_BITS_JMPTABLE_BIG
# define RELOC_STD_BITS_JMPTABLE_BIG 0x04
#endif /* !RELOC_STD_BITS_JMPTABLE_BIG */
#ifndef RELOC_STD_BITS_JMPTABLE_LITTLE
# define RELOC_STD_BITS_JMPTABLE_LITTLE 0x04
#endif /* !RELOC_STD_BITS_JMPTABLE_LITTLE */

#ifndef RELOC_STD_BITS_RELATIVE_BIG
# define RELOC_STD_BITS_RELATIVE_BIG 0x02
#endif /* !RELOC_STD_BITS_RELATIVE_BIG */
#ifndef RELOC_STD_BITS_RELATIVE_LITTLE
# define RELOC_STD_BITS_RELATIVE_LITTLE 0x02
#endif /* !RELOC_STD_BITS_RELATIVE_LITTLE */

#ifndef RELOC_STD_SIZE
# define RELOC_STD_SIZE	8 /* Bytes per relocation entry */
#endif /* !RELOC_STD_SIZE */

#ifndef SEGMENT_SIZE
# define SEGMENT_SIZE 0x400000
#endif /* !SEGMENT_SIZE */

#ifndef MY
/* Do not "beautify" the CONCAT* macro args. Traditional C will not remove
 * whitespace added here, and thus will fail to concatenate the tokens. */
# define MY(OP) CONCAT2(encap_,OP)
#endif /* !MY */
#define TARGETNAME "a.out-encap"
#ifndef NAME
# define NAME(x,y) CONCAT3(aout_encap,_32_,y)
#endif /* !NAME */

#if !defined(MY_mkobject) && defined(NAME) && defined(__BFD_AOUT_TARGET_H__)
# define MY_mkobject NAME(aout_encap,mkobject)
#endif /* !MY_mkobject && NAME */
#if !defined(MY_write_object_contents) && defined(MY)
# define MY_write_object_contents MY(write_object_contents)
#endif /* !MY_write_object_contents && MY */
#if !defined(MY_set_sizes) && defined(MY) && defined(__BFD_AOUT_TARGET_H__)
# define MY_set_sizes MY(set_sizes)
#endif /* !MY_set_sizes && MY && __BFD_AOUT_TARGET_H__ */


#ifndef MY_zmagic_contiguous
#define MY_zmagic_contiguous 0
#endif
#ifndef MY_text_includes_header
#define MY_text_includes_header 0
#endif
#ifndef MY_entry_is_text_address
#define MY_entry_is_text_address 0
#endif
#ifndef MY_exec_header_not_counted
#define MY_exec_header_not_counted 1
#endif
#ifndef MY_add_dynamic_symbols
#define MY_add_dynamic_symbols 0
#endif
#ifndef MY_add_one_symbol
#define MY_add_one_symbol 0
#endif
#ifndef MY_link_dynamic_object
#define MY_link_dynamic_object 0
#endif
#ifndef MY_write_dynamic_symbol
#define MY_write_dynamic_symbol 0
#endif
#ifndef MY_check_dynamic_reloc
#define MY_check_dynamic_reloc 0
#endif
#ifndef MY_finish_dynamic_link
#define MY_finish_dynamic_link 0
#endif

/* prototypes from this file: */
#if defined(__STDC__) && __STDC__
const bfd_target *encap_real_callback(bfd *abfd);
const bfd_target *encap_object_p(bfd *abfd);
bfd_boolean encap_write_object_contents(bfd *abfd);
#else
const bfd_target *encap_real_callback();
const bfd_target *encap_object_p();
bfd_boolean encap_write_object_contents();
#endif /* __STDC__ */

#if !defined(MY_write_object_content) && defined(encap_write_object_contents)
# define MY_write_object_content encap_write_object_contents
/* no "else" condition because this define might not even be necessary... */
#endif /* !MY_write_object_content */
/* 'encap_object_p' should be a prototype, and NOT a define, so there is no need
 * to also check if it is defined here: */
#if !defined(MY_object_p)
# define MY_object_p encap_object_p
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "MY_object_p should be defined here but is not"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* !MY_object_p */
#if !defined(MY_exec_hdr_flags) && defined(N_FLAGS_COFF_ENCAPSULATE)
# define MY_exec_hdr_flags N_FLAGS_COFF_ENCAPSULATE
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "MY_exec_hdr_flags should be defined here but is not"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* !MY_exec_hdr_flags && N_FLAGS_COFF_ENCAPSULATE */

#include "aout-target.h"

#if defined(__STDC__) && __STDC__
/* could not find a header that had these prototypes... just copy them here: */
extern void perror_with_name(char *string);
extern int myread(int desc, char *addr, int len);
# ifndef __BFD_AOUT_TARGET_H__
static const bfd_target *MY(callback)(bfd *abfd, struct internal_exec *);
# endif /* !__BFD_AOUT_TARGET_H__ */
# ifndef IS_OBJECT_FILE
#  define IS_OBJECT_FILE(hdr) (hdr.a_trsize != 0)
# endif /* !IS_OBJECT_FILE */
#else
extern void perror_with_name();
extern int myread();
# ifndef __BFD_AOUT_TARGET_H__
static const bfd_target *MY(callback)();
# endif /* !__BFD_AOUT_TARGET_H__ */
# ifndef IS_OBJECT_FILE
#  define IS_OBJECT_FILE(hdr) (0)
# endif /* !IS_OBJECT_FILE */
#endif /* __STDC__ */
/* Finish up the reading of an encapsulated-coff a.out file header: */
const bfd_target *encap_real_callback(abfd)
     bfd *abfd;
{
  long text_start;
  long exec_data_start;
  struct internal_exec exec_aouthdr;
  struct internal_exec *execp = exec_hdr(abfd);

#ifdef __BFD_AOUT_TARGET_H__
  MY(callback)(abfd);
  /* there is no argument for execp to go into in this preprocessor condition,
   * so contrive a dummy code condition to make sure that it is used: */
  if (execp != NULL) {
    ;
  }
#else /* no header: */
  MY(callback)(abfd, execp); /* (# of args differs) */
#endif /* __BFD_AOUT_TARGET_H__ */

  /* If we have a coff header, it can give us better values for
   * text_start and exec_data_start. This is particularly useful
   * for remote debugging of embedded systems.  */
  if (N_FLAGS(exec_aouthdr) & N_FLAGS_COFF_ENCAPSULATE)
    {
      struct coffheader ch;
      int val;
      int execchan;
      char *filename;
      val = lseek(execchan, -(sizeof(AOUTHDR) + sizeof(ch)), 1);
      if (val == -1) {
	perror_with_name(filename);
      }
      val = myread(execchan, (char *)&ch, sizeof(ch));
      if (val < 0) {
	  perror_with_name(filename);
      }
      text_start = ch.text_start;
      exec_data_start = ch.data_start;
  } else {
      text_start =
	(IS_OBJECT_FILE(exec_aouthdr) ? 0 : N_TXTADDR(exec_aouthdr));
      exec_data_start = (IS_OBJECT_FILE(exec_aouthdr)
			 ? exec_aouthdr.a_text
			 : N_DATADDR(exec_aouthdr));
    }

  /* Determine the architecture and machine type of the object file: */
  bfd_default_set_arch_mach(abfd, bfd_arch_m68k, 0); /* FIXME */

  return abfd->xvec;
}

const bfd_target *encap_object_p(abfd)
bfd *abfd;
{
  unsigned char magicbuf[4]; /* Raw bytes of magic number from file */
  unsigned long magic; /* Swapped magic number */
  short coff_magic;
  struct external_exec exec_bytes;
  struct internal_exec exec;
  bfd_size_type amt = sizeof(magicbuf);

  if (bfd_bread((PTR)magicbuf, amt, abfd) != amt) {
    if (bfd_get_error() != bfd_error_system_call) {
      bfd_set_error (bfd_error_wrong_format);
    }
    return 0;
  }

  coff_magic = H_GET_16(abfd, magicbuf);
  if (coff_magic != COFF_MAGIC) {
    return 0; /* Not an encap coff file */
  }

  magic = H_GET_32(abfd, magicbuf);

  if (N_BADMAG(*((struct internal_exec *)&magic))) {
    return 0;
  }

  if (bfd_seek(abfd, (file_ptr)sizeof(struct coffheader), SEEK_SET) != 0) {
    return 0;
  }

  amt = EXEC_BYTES_SIZE;
  if (bfd_bread((PTR)&exec_bytes, amt, abfd) != amt) {
    if (bfd_get_error() != bfd_error_system_call) {
      bfd_set_error(bfd_error_wrong_format);
    }
    return 0;
  }
  NAME(aout,swap_exec_header_in) (abfd, &exec_bytes, &exec);

  return aout_32_some_aout_object_p(abfd, &exec, encap_real_callback);
}

/* Write an object file in Encapsulated COFF format. Section contents have
 * already been written. We write the file header, symbols, & relocation. */
bfd_boolean encap_write_object_contents(abfd)
     bfd *abfd;
{
  short magic;
  bfd_size_type data_pad = 0;
  struct external_exec exec_bytes;
  struct internal_exec outheader;
  struct internal_exec *execp = exec_hdr(abfd);
#if !defined(local_text_size) && !defined(GDBSTABS_H)
  int local_text_size = 0;
#endif /* !local_text_size && !GDBSTABS_H */

  /* FIXME: Fragments from the old GNU LD program for dealing w/encap-coff: */
  struct coffheader mycoffheader;
  int need_coff_header;
  mycoffheader.magic = 0;
  need_coff_header = 0;

  /* dummy conditions to use unused variables: */
  if ((data_pad == 0) || (mycoffheader.magic == 0) || (need_coff_header == 0)) {
    ;
  }
  magic = mycoffheader.magic;
#ifdef N_SET_MAGIC
  /* Determine whether to count the header as part of
   * the text size, and initialize the text size accordingly.
   * This depends on the kind of system and on the output format selected: */
  N_SET_MAGIC(outheader, magic);
#endif /* N_SET_MAGIC */
#ifdef INITIALIZE_HEADER
  INITIALIZE_HEADER;
#endif /* INITIALIZE_HEADER */

  local_text_size = sizeof(struct exec);
#ifdef COFF_ENCAPSULATE
  if ((relocatable_output == 0) && (file_table[0].just_syms_flag == 0)) {
      need_coff_header = 1;
      /* set this flag now, since it will change the values of N_TXTOFF, etc: */
      N_SET_FLAGS(outheader, aout_backend_info(abfd)->exec_hdr_flags);
      local_text_size += sizeof(struct coffheader);
  }
#endif /* COFF_ENCAPSULATE */

#ifdef COFF_ENCAPSULATE
  if (need_coff_header) {
      /* We are encapsulating BSD format within COFF format. */
      struct coffscn *tp, *dp, *bp;

      tp = &mycoffheader.scns[0];
      dp = &mycoffheader.scns[1];
      bp = &mycoffheader.scns[2];

      strcpy(tp->s_name, ".text");
      tp->s_paddr = text_start;
      tp->s_vaddr = text_start;
      tp->s_size = local_text_size;
      tp->s_scnptr = (sizeof(struct coffheader) + sizeof(struct exec));
      tp->s_relptr = 0;
      tp->s_lnnoptr = 0;
      tp->s_nreloc = 0;
      tp->s_nlnno = 0;
      tp->s_flags = 0x20;
      strcpy (dp->s_name, ".data");
      dp->s_paddr = data_start;
      dp->s_vaddr = data_start;
      dp->s_size = data_size;
      dp->s_scnptr = tp->s_scnptr + tp->s_size;
      dp->s_relptr = 0;
      dp->s_lnnoptr = 0;
      dp->s_nreloc = 0;
      dp->s_nlnno = 0;
      dp->s_flags = 0x40;
      strcpy (bp->s_name, ".bss");
      bp->s_paddr = dp->s_vaddr + dp->s_size;
      bp->s_vaddr = bp->s_paddr;
      bp->s_size = bss_size;
      bp->s_scnptr = 0;
      bp->s_relptr = 0;
      bp->s_lnnoptr = 0;
      bp->s_nreloc = 0;
      bp->s_nlnno = 0;
      bp->s_flags = 0x80;

      mycoffheader.f_magic = COFF_MAGIC;
      mycoffheader.f_nscns = 3;
      /* store an unlikely time so programs can
       * tell that there is a bsd header
       */
      mycoffheader.f_timdat = 1;
      mycoffheader.f_symptr = 0;
      mycoffheader.f_nsyms = 0;
      mycoffheader.f_opthdr = 28;
      mycoffheader.f_flags = 0x103;
      /* aouthdr */
      mycoffheader.magic = ZMAGIC;
      mycoffheader.vstamp = 0;
      mycoffheader.tsize = tp->s_size;
      mycoffheader.dsize = dp->s_size;
      mycoffheader.bsize = bp->s_size;
      mycoffheader.entry = outheader.a_entry;
      mycoffheader.text_start = tp->s_vaddr;
      mycoffheader.data_start = dp->s_vaddr;
  }
#endif /* COFF_ENCAPSULATE */

#ifdef COFF_ENCAPSULATE
  if (need_coff_header) {
      mywrite(&mycoffheader, sizeof(mycoffheader), 1, outdesc);
  }
#endif /* COFF_ENCAPSULATE */

#if !defined(COFF_ENCAPSULATE) && defined(N_TXTOFF) && defined(padfile)
  padfile((N_TXTOFF(outheader) - sizeof(outheader)), outdesc);
#endif /* !COFF_ENCAPSULATE && N_TXTOFF && padfile */

  local_text_size -= N_TXTOFF(outheader);
  WRITE_HEADERS(abfd, execp);
  return TRUE;
}

/* silence '-Wunused-macros' warnings
 * (for ones that it does NOT make sense to just delete): */
#ifdef HEADER_OFFSET_FD
# undef HEADER_OFFSET_FD
#endif /* HEADER_OFFSET_FD */

/* EOF */
