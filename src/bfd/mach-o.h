/* mach-o.h: Mach-O support for BFD.
 * Copyright 1999, 2000, 2001, 2002, 2003, 2005
 * Free Software Foundation, Inc.
 *
 * This file is part of BFD, the Binary File Descriptor library.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St. - 5th Floor, Boston, MA 02110-1301, USA.
 */

#ifndef _BFD_MACH_O_H_
#define _BFD_MACH_O_H_ 1

#include "ansidecl.h"
#include "bfd.h"

  /* We use __extension__ here to suppress -pedantic warnings about GCC
   * extensions.  This feature did NOT work properly before gcc 2.8: */
#if !defined(__extension__) && defined(GCC_VERSION)
# if (GCC_VERSION < 2008)
#  define __extension__
# endif /* gcc pre-2.8 */
#endif /* !__extension__ && GCC_VERSION */

#if defined(__GNUC__) && defined(__GNUC_MINOR__)
# if ((__GNUC__ == 4) && (__GNUC_MINOR__ == 2)) && !defined(__clang__)
#  pragma GCC diagnostic ignored "-pedantic"
# else
#  if defined(__clang__) && (__clang__ >= 1)
#   pragma clang diagnostic ignored "-Wpedantic"
#  endif /* __clang__ */
# endif /* gcc 4.2 (non-clang) */
#endif /* any gcc */

#if !defined(__has_include)
# define __has_include(foo) 0
#endif /* !__has_include */
#if defined(HAVE_MACH_O_LOADER_H) || __has_include(<mach-o/loader.h>)
# include <mach-o/loader.h>
# ifndef _ENUM_BFD_MACH_O_MACH_HEADER_MAGIC_DEFINED
#  define _ENUM_BFD_MACH_O_MACH_HEADER_MAGIC_DEFINED 1
#   if !defined(__STRICT_ANSI__) && !defined(PEDANTIC_WARNINGS)
/* Constants for header: */
typedef enum bfd_mach_o_mach_header_magic
{
  BFD_MACH_O_MH_MAGIC = __extension__ 0xfeedface,
  BFD_MACH_O_MH_CIGAM = __extension__ 0xcefaedfe,
  BFD_MACH_O_MH_MAGIC_64 = __extension__ 0xfeedfacf,
  BFD_MACH_O_MH_CIGAM_64 = __extension__ 0xcffaedfe
}
bfd_mach_o_mach_header_magic;
#  endif /* !__STRICT_ANSI__ && !PEDANTIC_WARNINGS */
# endif /* !_ENUM_BFD_MACH_O_MACH_HEADER_MAGIC_DEFINED */
# ifndef _ENUM_BFD_MACH_O_SECTION_TYPE_DEFINED
#  define _ENUM_BFD_MACH_O_SECTION_TYPE_DEFINED 1
typedef enum bfd_mach_o_section_type
{
  BFD_MACH_O_S_REGULAR = 0x0,
  BFD_MACH_O_S_ZEROFILL = 0x1,
  BFD_MACH_O_S_CSTRING_LITERALS = 0x2,
  BFD_MACH_O_S_4BYTE_LITERALS = 0x3,
  BFD_MACH_O_S_8BYTE_LITERALS = 0x4,
  BFD_MACH_O_S_LITERAL_POINTERS = 0x5,
  BFD_MACH_O_S_NON_LAZY_SYMBOL_POINTERS = 0x6,
  BFD_MACH_O_S_LAZY_SYMBOL_POINTERS = 0x7,
  BFD_MACH_O_S_SYMBOL_STUBS = 0x8,
  BFD_MACH_O_S_MOD_INIT_FUNC_POINTERS = 0x9,
  BFD_MACH_O_S_MOD_FINI_FUNC_POINTERS = 0xa,
  BFD_MACH_O_S_COALESCED = 0xb,
  BFD_MACH_O_S_GB_ZEROFILL = 0xc,
  BFD_MACH_O_S_INTERPOSING = 0xd,
  BFD_MACH_O_S_16BYTE_LITERALS = 0xe,
  BFD_MACH_O_S_DTRACE_DOF = 0xf,
  BFD_MACH_O_S_LAZY_DYLIB_SYMBOL_POINTERS = 0x10
}
bfd_mach_o_section_type;
# endif /* !_ENUM_BFD_MACH_O_SECTION_TYPE_DEFINED */
# ifndef _ENUM_BFD_MACH_O_SECTION_ATTRIBUTE_DEFINED
#  define _ENUM_BFD_MACH_O_SECTION_ATTRIBUTE_DEFINED 1
typedef enum bfd_mach_o_section_attribute
{
  BFD_MACH_O_S_ATTR_NONE = 0,
  BFD_MACH_O_S_ATTR_LOC_RELOC = 0x00000100,
  BFD_MACH_O_S_ATTR_EXT_RELOC = 0x00000200,
  BFD_MACH_O_S_ATTR_SOME_INSTRUCTIONS = 0x00000400,
  BFD_MACH_O_S_ATTR_DEBUG = 0x02000000,
  BFD_MACH_O_S_SELF_MODIFYING_CODE = 0x04000000,
  BFD_MACH_O_S_ATTR_LIVE_SUPPORT = 0x08000000,
  BFD_MACH_O_S_ATTR_NO_DEAD_STRIP = 0x10000000,
  BFD_MACH_O_S_ATTR_STRIP_STATIC_SYMS = 0x20000000,
  BFD_MACH_O_S_ATTR_NO_TOC = 0x40000000
#  if !defined(__STRICT_ANSI__) && !defined(PEDANTIC_WARNINGS)
  , BFD_MACH_O_S_ATTR_PURE_INSTRUCTIONS = __extension__ 0x80000000
#  endif /* !__STRICT_ANSI__ && !PEDANTIC_WARNINGS */
}
bfd_mach_o_section_attribute;
# endif /* !_ENUM_BFD_MACH_O_SECTION_ATTRIBUTE_DEFINED */
#else
# if (defined(__APPLE__) && defined(__MACH__)) || __has_include("mach-o-gnu/loader.h")
/* this should only be a temporary renaming: */
#  include "mach-o-gnu/loader.h"
# else
#  include "mach-o/loader.h"
# endif /* (__APPLE__ && __MACH__) || (renamed) */
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
 #  warning "mach-o.h expects a proper <mach-o/loader.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_MACH_O_LOADER_H */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define BFD_MACH_O_N_STAB  0xe0	/* If any of these bits set, a symbolic debugging entry.  */
#define BFD_MACH_O_N_PEXT  0x10	/* Private external symbol bit.  */
#define BFD_MACH_O_N_TYPE  0x0e	/* Mask for the type bits.  */
#define BFD_MACH_O_N_EXT   0x01	/* External symbol bit, set for external symbols.  */
#define BFD_MACH_O_N_UNDF  0x00	/* Undefined, n_sect == NO_SECT.  */
#define BFD_MACH_O_N_ABS   0x02	/* Absolute, n_sect == NO_SECT.  */
#define BFD_MACH_O_N_SECT  0x0e	/* Defined in section number n_sect.  */
#define BFD_MACH_O_N_PBUD  0x0c /* Prebound undefined (defined in a dylib).  */
#define BFD_MACH_O_N_INDR  0x0a	/* Indirect.  */

typedef enum bfd_mach_o_ppc_thread_flavour
{
  BFD_MACH_O_PPC_THREAD_STATE = 1,
  BFD_MACH_O_PPC_FLOAT_STATE = 2,
  BFD_MACH_O_PPC_EXCEPTION_STATE = 3,
  BFD_MACH_O_PPC_VECTOR_STATE = 4
  /* APPLE LOCAL 64-bit */
  , BFD_MACH_O_PPC_THREAD_STATE_64 = 5
}
bfd_mach_o_ppc_thread_flavour;

typedef enum bfd_mach_o_i386_thread_flavour
{
    BFD_MACH_O_i386_THREAD_STATE = 1,
    BFD_MACH_O_i386_FLOAT_STATE = 2,
    BFD_MACH_O_i386_EXCEPTION_STATE = 3,
    /* APPLE LOCAL begin x86_64 */
    BFD_MACH_O_x86_THREAD_STATE64 = 4,
    BFD_MACH_O_x86_FLOAT_STATE64 = 5,
    BFD_MACH_O_x86_EXCEPTION_STATE64 = 6,
    BFD_MACH_O_x86_THREAD_STATE = 7,
    BFD_MACH_O_x86_FLOAT_STATE = 8,
    BFD_MACH_O_x86_EXCEPTION_STATE = 9,
    BFD_MACH_O_i386_THREAD_STATE_NONE = 10/*,*/
    /* APPLE LOCAL end x86_64 */
}
bfd_mach_o_i386_thread_flavour;

typedef enum bfd_mach_o_arm_thread_flavour
  {
    BFD_MACH_O_ARM_THREAD_STATE = 1,
    BFD_MACH_O_ARM_VFP_STATE = 2,
    BFD_MACH_O_ARM_EXCEPTION_STATE = 3,
    BFD_MACH_O_ARM_THREAD_STATE_NONE = 4
  }
bfd_mach_o_arm_thread_flavour;

#ifndef BFD_MACH_O_LC_REQ_DYLD
# if defined(__STRICT_ANSI__) || defined(__clang__) || \
     (!defined(__GNUC__) || !defined(__GNUC_MINOR__)) || \
     (!defined(__STDC_VERSION__) || (__STDC_VERSION__ < 199901L)) || \
     defined(__extension__)
#  define BFD_MACH_O_LC_REQ_DYLD __extension__ (int)0x80000000
# else
#  define BFD_MACH_O_LC_REQ_DYLD 0x80000000
# endif /* __STRICT_ANSI__ || __clang__ || !gcc || pre-c99 */
#endif /* !BFD_MACH_O_LC_REQ_DYLD */

#ifndef _ENUM_BFD_MACH_O_LOAD_COMMAND_TYPE_DEFINED
# define _ENUM_BFD_MACH_O_LOAD_COMMAND_TYPE_DEFINED 1
typedef enum bfd_mach_o_load_command_type
{
  BFD_MACH_O_LC_SEGMENT = 0x1,		/* File segment to be mapped.  */
  BFD_MACH_O_LC_SYMTAB = 0x2, /* Link-edit stab symbol table info (obsolete).  */
  BFD_MACH_O_LC_SYMSEG = 0x3,	/* Link-edit gdb symbol table info.  */
  BFD_MACH_O_LC_THREAD = 0x4,		/* Thread.  */
  BFD_MACH_O_LC_UNIXTHREAD = 0x5, /* UNIX thread (includes a stack).  */
  BFD_MACH_O_LC_LOADFVMLIB = 0x6, /* Load a fixed VM shared library.  */
  BFD_MACH_O_LC_IDFVMLIB = 0x7,		/* Fixed VM shared library id.  */
  BFD_MACH_O_LC_IDENT = 0x8, /* Object identification information (obsolete).  */
  BFD_MACH_O_LC_FVMFILE = 0x9,		/* Fixed VM file inclusion.  */
  BFD_MACH_O_LC_PREPAGE = 0xa, /* Prepage command (internal use).  */
  BFD_MACH_O_LC_DYSYMTAB = 0xb, /* Dynamic link-edit symbol table info.  */
  BFD_MACH_O_LC_LOAD_DYLIB = 0xc, /* Load a dynamically linked shared library.  */
  BFD_MACH_O_LC_ID_DYLIB = 0xd, /* Dynamically linked shared lib identification.  */
  BFD_MACH_O_LC_LOAD_DYLINKER = 0xe,	/* Load a dynamic linker.  */
  BFD_MACH_O_LC_ID_DYLINKER = 0xf,    /* Dynamic linker identification.  */
  BFD_MACH_O_LC_PREBOUND_DYLIB = 0x10, /* Modules prebound for a dynamically.  */
  BFD_MACH_O_LC_ROUTINES = 0x11,	/* Image routines.  */
  BFD_MACH_O_LC_SUB_FRAMEWORK = 0x12,	/* Sub framework.  */
  BFD_MACH_O_LC_SUB_UMBRELLA = 0x13,	/* Sub umbrella.  */
  BFD_MACH_O_LC_SUB_CLIENT = 0x14,	/* Sub client.  */
  BFD_MACH_O_LC_SUB_LIBRARY = 0x15,   	/* Sub library.  */
  BFD_MACH_O_LC_TWOLEVEL_HINTS = 0x16, /* Two-level namespace lookup hints.  */
  BFD_MACH_O_LC_PREBIND_CKSUM = 0x17, /* Prebind checksum.  */
  /* Load a dynamically linked shared library that is allowed to be
       missing (weak).  */
  BFD_MACH_O_LC_LOAD_WEAK_DYLIB =
    __extension__ (0x18 | BFD_MACH_O_LC_REQ_DYLD),
  /* APPLE LOCAL 64-bit */
  BFD_MACH_O_LC_SEGMENT_64 = 0x19,  /* 64-bit segment of this file to be
                                     * mapped.  */
  BFD_MACH_O_LC_ROUTINES_64 = 0x1a, /* Address of the dyld init routine
                                     * in a dylib.  */
  BFD_MACH_O_LC_UUID = 0x1b,        /* 128-bit UUID of the executable.  */
  BFD_MACH_O_LC_RPATH =
    __extension__ (0x1c | BFD_MACH_O_LC_REQ_DYLD), /* runpath additions */
  BFD_MACH_O_LC_CODE_SIGNATURE = 0x1d,
  BFD_MACH_O_LC_SEGMENT_SPLIT_INFO = 0x1e,
  BFD_MACH_O_LC_REEXPORT_DYLIB =
    __extension__ (0x1f | BFD_MACH_O_LC_REQ_DYLD),
  BFD_MACH_O_LC_LAZY_LOAD_DYLIB = 0x20, /* delay load of dylib until first use */
  BFD_MACH_O_LC_ENCRYPTION_INFO = 0x21, /* encrypted segment information */
  BFD_MACH_O_LC_DYLD_INFO = 0x22,         /* compressed dyld information */
  BFD_MACH_O_LC_DYLD_INFO_ONLY =     /* compressed dyld information only */
    __extension__ (0x22 | BFD_MACH_O_LC_REQ_DYLD),
  BFD_MACH_O_LC_LOAD_UPWARD_DYLIB =             /* Same as LC_LOAD_DYLIB */
    __extension__ (0x23 | BFD_MACH_O_LC_REQ_DYLD),
  BFD_MACH_O_LC_VERSION_MIN_MACOSX = 0x24, /* build for MacOSX min OS version */
  BFD_MACH_O_LC_VERSION_MIN_IPHONEOS = 0x25, /* build for iPhoneOS min OS version */
  BFD_MACH_O_LC_FUNCTION_STARTS = 0x26, /* table of function start addresses. */
  BFD_MACH_O_LC_DYLD_ENVIRONMENT = 0x27, /* string for dyld to treat like environment variable */
  BFD_MACH_O_LC_MAIN =                  /* replacement for LC_UNIXTHREAD */
    __extension__ (0x28 | BFD_MACH_O_LC_REQ_DYLD),
  BFD_MACH_O_LC_DATA_IN_CODE = 0x29, /* table of non-instructions in __text */
  BFD_MACH_O_LC_SOURCE_VERSION = 0x2a, /* source version used to build binary */
  BFD_MACH_O_LC_DYLIB_CODE_SIGN_DRS = 0x2b/*,*/ /* Code signing DRs copied from linked dylibs */
}
bfd_mach_o_load_command_type;
#endif /* !_ENUM_BFD_MACH_O_LOAD_COMMAND_TYPE_DEFINED */

#define BFD_MACH_O_CPU_IS64BIT 0x1000000

#ifndef _ENUM_BFD_MACH_O_CPU_TYPE_DEFINED
# define _ENUM_BFD_MACH_O_CPU_TYPE_DEFINED 1
typedef enum bfd_mach_o_cpu_type
{
  BFD_MACH_O_CPU_TYPE_VAX = 1,
  BFD_MACH_O_CPU_TYPE_MC680x0 = 6,
  BFD_MACH_O_CPU_TYPE_I386 = 7,
  BFD_MACH_O_CPU_TYPE_MIPS = 8,
  BFD_MACH_O_CPU_TYPE_MC98000 = 10,
  BFD_MACH_O_CPU_TYPE_HPPA = 11,
  BFD_MACH_O_CPU_TYPE_ARM = 12,
  BFD_MACH_O_CPU_TYPE_MC88000 = 13,
  BFD_MACH_O_CPU_TYPE_SPARC = 14,
  BFD_MACH_O_CPU_TYPE_I860 = 15,
  BFD_MACH_O_CPU_TYPE_ALPHA = 16,
  BFD_MACH_O_CPU_TYPE_POWERPC = 18
  /* APPLE LOCAL 64-bit */
  , BFD_MACH_O_CPU_TYPE_POWERPC_64 = (18 | BFD_MACH_O_CPU_IS64BIT)
  /* APPLE LOCAL x86_64 */
  , BFD_MACH_O_CPU_TYPE_X86_64 = (BFD_MACH_O_CPU_TYPE_I386 | BFD_MACH_O_CPU_IS64BIT)
}
bfd_mach_o_cpu_type;
#endif /* !_ENUM_BFD_MACH_O_CPU_TYPE_DEFINED */

#ifndef _ENUM_BFD_MACH_O_CPU_SUBTYPE_DEFINED
# define _ENUM_BFD_MACH_O_CPU_SUBTYPE_DEFINED 1
typedef enum bfd_mach_o_cpu_subtype
  {
    BFD_MACH_O_CPU_SUBTYPE_POWERPC_ALL = 0,
    BFD_MACH_O_CPU_SUBTYPE_X86_ALL = 3,
    BFD_MACH_O_CPU_SUBTYPE_ARM_4T = 5,
    BFD_MACH_O_CPU_SUBTYPE_ARM_6 = 6,
    BFD_MACH_O_CPU_SUBTYPE_ARM_7 = 9,
    BFD_MACH_O_CPU_SUBTYPE_ARM_7F = 10,
    BFD_MACH_O_CPU_SUBTYPE_ARM_7S = 11,
    BFD_MACH_O_CPU_SUBTYPE_ARM_7K = 12,
    BFD_MACH_O_CPU_SUBTYPE_POWERPC_970 = 100
  }
bfd_mach_o_cpu_subtype;
#endif /* !_ENUM_BFD_MACH_O_CPU_SUBTYPE_DEFINED */

#ifndef _ENUM_BFD_MACH_O_FILETYPE_DEFINED
# define _ENUM_BFD_MACH_O_FILETYPE_DEFINED 1
typedef enum bfd_mach_o_filetype
{
  BFD_MACH_O_MH_OBJECT = 1,
  BFD_MACH_O_MH_EXECUTE = 2,
  BFD_MACH_O_MH_FVMLIB = 3,
  BFD_MACH_O_MH_CORE = 4,
  BFD_MACH_O_MH_PRELOAD = 5,
  BFD_MACH_O_MH_DYLIB = 6,
  BFD_MACH_O_MH_DYLINKER = 7,
  BFD_MACH_O_MH_BUNDLE = 8,
  BFD_MACH_O_MH_DYLIB_STUB = 9,
  BFD_MACH_O_MH_DSYM = 10,
  BFD_MACH_O_MH_BUNDLE_KEXT = 11
# if !defined(BFD_MACH_O_MH_KEXT_BUNDLE) && !defined(_MACH_O_LOADER_H)
#  define BFD_MACH_O_MH_KEXT_BUNDLE BFD_MACH_O_MH_BUNDLE_KEXT
# endif /* !BFD_MACH_O_MH_KEXT_BUNDLE && !_MACH_O_LOADER_H */
}
bfd_mach_o_filetype;
#endif /* !_ENUM_BFD_MACH_O_FILETYPE_DEFINED */

#ifndef _ENUM_BFD_MACH_O_HEADER_FLAGS_DEFINED
# define _ENUM_BFD_MACH_O_HEADER_FLAGS_DEFINED 1
typedef enum bfd_mach_o_header_flags
{
  BFD_MACH_O_MH_NOUNDEFS	= 0x1,
  BFD_MACH_O_MH_INCRLINK	= 0x2,
  BFD_MACH_O_MH_DYLDLINK	= 0x4,
  BFD_MACH_O_MH_BINDATLOAD	= 0x8,
  BFD_MACH_O_MH_PREBOUND	= 0x10,
  BFD_MACH_O_MH_SPLIT_SEGS	= 0x20,
  BFD_MACH_O_MH_LAZY_INIT	= 0x40,
  BFD_MACH_O_MH_TWOLEVEL	= 0x80,
  BFD_MACH_O_MH_FORCE_FLAT	= 0x100,
  BFD_MACH_O_MH_NOMULTIDEFS	= 0x200,
  BFD_MACH_O_MH_NOFIXPREBINDING = 0x400,
  BFD_MACH_O_MH_PREBINDABLE     = 0x800,
  BFD_MACH_O_MH_ALLMODSBOUND    = 0x1000,
  BFD_MACH_O_MH_SUBSECTIONS_VIA_SYMBOLS = 0x2000,
  BFD_MACH_O_MH_CANONICAL    = 0x4000,
  BFD_MACH_O_MH_WEAK_DEFINES	= 0x8000,
  BFD_MACH_O_MH_BINDS_TO_WEAK = 0x10000,
  BFD_MACH_O_MH_ALLOW_STACK_EXECUTION = 0x20000,
  BFD_MACH_O_MH_ROOT_SAFE = 0x40000,
  BFD_MACH_O_MH_SETUID_SAFE = 0x80000,
  BFD_MACH_O_MH_NO_REEXPORTED_DYLIBS = 0x100000,
  BFD_MACH_O_MH_PIE = 0x200000,
  BFD_MACH_O_MH_DEAD_STRIPPABLE_DYLIB = 0x40000,
  BFD_MACH_O_MH_HAS_TLV_DESCRIPTORS = 0x800000,
  BFD_MACH_O_MH_NO_HEAP_EXECUTION = 0x1000000
}
bfd_mach_o_header_flags;
#endif /* !_ENUM_BFD_MACH_O_HEADER_FLAGS_DEFINED */

/* Constants for the type of a section.  */

#ifndef _ENUM_BFD_MACH_O_SECTION_TYPE_DEFINED
# define _ENUM_BFD_MACH_O_SECTION_TYPE_DEFINED 1
typedef enum bfd_mach_o_section_type
{
  /* Regular section: */
  BFD_MACH_O_S_REGULAR = 0x0,

  /* Zero fill on demand section: */
  BFD_MACH_O_S_ZEROFILL = 0x1,

  /* Section with only literal C strings: */
  BFD_MACH_O_S_CSTRING_LITERALS = 0x2,

  /* Section with only 4 byte literals: */
  BFD_MACH_O_S_4BYTE_LITERALS = 0x3,

  /* Section with only 8 byte literals: */
  BFD_MACH_O_S_8BYTE_LITERALS = 0x4,

  /* Section with only pointers to literals: */
  BFD_MACH_O_S_LITERAL_POINTERS = 0x5,

  /* For the two types of symbol pointers sections and the symbol stubs
     section they have indirect symbol table entries.  For each of the
     entries in the section the indirect symbol table entries, in
     corresponding order in the indirect symbol table, start at the index
     stored in the reserved1 field of the section structure.  Since the
     indirect symbol table entries correspond to the entries in the
     section the number of indirect symbol table entries is inferred from
     the size of the section divided by the size of the entries in the
     section.  For symbol pointers sections the size of the entries in
     the section is 4 bytes and for symbol stubs sections the byte size
     of the stubs is stored in the reserved2 field of the section
     structure.  */

  /* Section with only non-lazy symbol pointers: */
  BFD_MACH_O_S_NON_LAZY_SYMBOL_POINTERS = 0x6,

  /* Section with only lazy symbol pointers: */
  BFD_MACH_O_S_LAZY_SYMBOL_POINTERS = 0x7,

  /* Section with only symbol stubs, byte size of stub in the reserved2 field: */
  BFD_MACH_O_S_SYMBOL_STUBS = 0x8,

  /* Section with only function pointers for initializatio:  */
  BFD_MACH_O_S_MOD_INIT_FUNC_POINTERS = 0x9
  /* APPLE LOCAL begin Mach-O */
  /* Section with only function pointers for termination: */
  , BFD_MACH_O_S_MOD_TERM_FUNC_POINTERS = 0xa,
  /* Section contains symbols that are to be coalesced: */
  BFD_MACH_O_S_COALESCED = 0xb,
  /* zero fill on demand section (that can be larger than 4 gigabytes) */
  BFD_MACH_O_S_GB_ZEROFILL = 0xd,
  /* a debug section: */
  BFD_MACH_O_S_ATTR_DEBUG = 0x02000000/*,*/
  /* APPLE LOCAL end Mach-O */
}
bfd_mach_o_section_type;
#endif /* !_ENUM_BFD_MACH_O_SECTION_TYPE_DEFINED */

#define BFD_MACH_O_SECTION_TYPE_MASK 0x000000ff
#define BFD_MACH_O_SECTION_ATTRIBUTES_MASK 0xffffff00

typedef struct bfd_mach_o_header
{
  unsigned long magic;
  unsigned long cputype;
  unsigned long cpusubtype;
  unsigned long filetype;
  unsigned long ncmds;
  unsigned long sizeofcmds;
  unsigned long flags;
  unsigned int reserved;
  /* Version 1: 32 bits, version 2: 64 bits: */
  unsigned int version;
  enum bfd_endian byteorder;
}
bfd_mach_o_header;

typedef struct bfd_mach_o_asymbol
{
  /* The actual symbol with which the rest of BFD works: */
  asymbol symbol;

  /* Mach-O symbol fields: */
  unsigned char n_type;
  unsigned char n_sect;
  unsigned short n_desc;
}
bfd_mach_o_asymbol;

#define BFD_MACH_O_SEGNAME_SIZE 16
#define BFD_MACH_O_SECTNAME_SIZE 16

typedef struct bfd_mach_o_section
{
  /* Fields present in the file: */
  asection *bfdsection;
  char sectname[BFD_MACH_O_SECTNAME_SIZE + 1]; 	/* Always NUL padded.  */
  char segname[BFD_MACH_O_SEGNAME_SIZE + 1];
  bfd_vma addr;
  bfd_vma size;
  bfd_vma offset;
  unsigned long align;
  bfd_vma reloff;
  unsigned long nreloc;
  unsigned long flags;
  unsigned long reserved1;
  unsigned long reserved2;
  unsigned long reserved3;
#if 0
  /* Corresponding bfd section: */
  asection *bfdsection;

  /* An array holding the indirect symbols for this section.
   * NULL values indicate local symbols.
   * The number of symbols is determined from the section size and type: */
  bfd_mach_o_asymbol **indirect_syms;

  /* Simply linked list: */
  struct bfd_mach_o_section *next;
#endif /* 0 */
}
bfd_mach_o_section;

typedef struct bfd_mach_o_segment_command
{
  char segname[BFD_MACH_O_SEGNAME_SIZE + 1];
  bfd_vma vmaddr;
  bfd_vma vmsize;
  bfd_vma fileoff;
  bfd_vma filesize;
  unsigned long maxprot;	/* Maximum permitted protection.  */
  unsigned long initprot;	/* Initial protection.  */
  unsigned long nsects;
  unsigned long flags;

  /* Linked list of sections: */
  bfd_mach_o_section *sections;
  asection *segment;
}
bfd_mach_o_segment_command;

/* Protection flags: */
#ifndef BFD_MACH_O_PROT_READ
# define BFD_MACH_O_PROT_READ 0x01
#endif /* !BFD_MACH_O_PROT_READ */
#ifndef BFD_MACH_O_PROT_WRITE
# define BFD_MACH_O_PROT_WRITE 0x02
#endif /* !BFD_MACH_O_PROT_WRITE */
#ifndef BFD_MACH_O_PROT_EXECUTE
# define BFD_MACH_O_PROT_EXECUTE 0x04
#endif /* !BFD_MACH_O_PROT_EXECUTE */

/* Expanded internal representation of a relocation entry: */
typedef struct bfd_mach_o_reloc_info
{
  bfd_vma r_address;
  bfd_vma r_value;
  unsigned int r_scattered : 1;
  unsigned int r_type : 4;
  unsigned int r_pcrel : 1;
  unsigned int r_length : 2;
  unsigned int r_extern : 1;
}
bfd_mach_o_reloc_info;

/* The symbol table is sorted like this:
 (1) local.
 (otherwise in order of generation)
 (2) external defined
 (sorted by name)
 (3) external undefined / common
 (sorted by name)
*/
typedef struct bfd_mach_o_symtab_command
{
  unsigned long symoff;
  unsigned long nsyms;
  unsigned long stroff;
  unsigned long strsize;
  asymbol *symbols;
  char *strtab;
  asection *stabs_segment;
  asection *stabstr_segment;
}
bfd_mach_o_symtab_command;

#ifndef BFD_MACH_O_REFERENCE_SIZE
# define BFD_MACH_O_REFERENCE_SIZE 4
#endif /* !BFD_MACH_O_REFERENCE_SIZE */

/* This is the second set of the symbolic information which is used to support
   the data structures for the dynamically link editor.

   The original set of symbolic information in the symtab_command which contains
   the symbol and string tables must also be present when this load command is
   present.  When this load command is present the symbol table is organized
   into three groups of symbols:
       local symbols (static and debugging symbols) - grouped by module
       defined external symbols - grouped by module (sorted by name if not lib)
       undefined external symbols (sorted by name)
   In this load command there are offsets and counts to each of the three groups
   of symbols.

   This load command contains the offsets and sizes of the following new
   symbolic information tables:
       table of contents
       module table
       reference symbol table
       indirect symbol table
   The first three tables above (the table of contents, module table and
   reference symbol table) are only present if the file is a dynamically linked
   shared library.  For executable and object modules, which are files
   containing only one module, the information that would be in these three
   tables is determined as follows:
       table of contents - the defined external symbols are sorted by name
       module table - the file contains only one module so everything in the
                      file is part of the module.
       reference symbol table - is the defined and undefined external symbols

   For dynamically linked shared library files this load command also contains
   offsets and sizes to the pool of relocation entries for all sections
   separated into two groups:
       external relocation entries
       local relocation entries
   For executable and object modules the relocation entries continue to hang
   off the section structures.  */

typedef struct bfd_mach_o_dylib_module
{
  /* Index into the string table indicating the name of the module: */
  unsigned long module_name_idx;
  char *module_name;

  /* Index into the symbol table of the first defined external symbol
   * provided by the module: */
  unsigned long iextdefsym;

  /* Number of external symbols provided by this module: */
  unsigned long nextdefsym;

  /* Index into the external reference table of the first entry
   * provided by this module: */
  unsigned long irefsym;

  /* Number of external reference entries provided by this module: */
  unsigned long nrefsym;

  /* Index into the symbol table of the first local symbol provided by this
   * module: */
  unsigned long ilocalsym;

  /* Number of local symbols provided by this module: */
  unsigned long nlocalsym;

  /* Index into the external relocation table of the first entry provided
   * by this module: */
  unsigned long iextrel;

  /* Number of external relocation entries provided by this module: */
  unsigned long nextrel;

  /* Index in the module initialization section to the pointers for this
   * module: */
  unsigned short iinit;

  /* Index in the module termination section to the pointers for this
   * module: */
  unsigned short iterm;

  /* Number of pointers in the module initialization for this module: */
  unsigned short ninit;

  /* Number of pointers in the module termination for this module: */
  unsigned short nterm;

  /* Number of data byte for this module that are used in the __module_info
   * section of the __OBJC segment: */
  unsigned long objc_module_info_size;

  /* Statically linked address of the start of the data for this module
   * in the __module_info section of the __OBJC_segment: */
  bfd_vma objc_module_info_addr;
}
bfd_mach_o_dylib_module;

typedef struct bfd_mach_o_dylib_table_of_content
{
  /* Index into the symbol table to the defined external symbol: */
  unsigned long symbol_index;

  /* Index into the module table to the module for this entry: */
  unsigned long module_index;
}
bfd_mach_o_dylib_table_of_content;

typedef struct bfd_mach_o_dylib_reference
{
  /* Index into the symbol table for the symbol being referenced: */
  unsigned long isym;

  /* Type of the reference being made (use REFERENCE_FLAGS constants): */
  unsigned long flags;
}
bfd_mach_o_dylib_reference;
#ifndef BFD_MACH_O_REFERENCE_SIZE
# define BFD_MACH_O_REFERENCE_SIZE 4
#endif /* !BFD_MACH_O_REFERENCE_SIZE */

typedef struct bfd_mach_o_dysymtab_command
{
  /* The symbols indicated by symoff and nsyms of the LC_SYMTAB load command
     are grouped into the following three groups:
       local symbols (further grouped by the module they are from)
       defined external symbols (further grouped by the module they are from)
       undefined symbols

     The local symbols are used only for debugging.  The dynamic binding
     process may have to use them to indicate to the debugger the local
     symbols for a module that is being bound.

     The last two groups are used by the dynamic binding process to do the
     binding (indirectly through the module table and the reference symbol
     table when this is a dynamically linked shared library file).  */

  unsigned long ilocalsym;    /* Index to local symbols.  */
  unsigned long nlocalsym;    /* Number of local symbols.  */
  unsigned long iextdefsym;   /* Index to externally defined symbols.  */
  unsigned long nextdefsym;   /* Number of externally defined symbols.  */
  unsigned long iundefsym;    /* Index to undefined symbols.  */
  unsigned long nundefsym;    /* Number of undefined symbols.  */

  /* For the dynamic binding process to find which module a symbol
     is defined in the table of contents is used (analogous to the ranlib
     structure in an archive) which maps defined external symbols to modules
     they are defined in.  This exists only in a dynamically linked shared
     library file.  For executable and object modules the defined external
     symbols are sorted by name and are used as the table of contents.  */

  unsigned long tocoff;       /* File offset to table of contents.  */
  unsigned long ntoc;        /* Number of entries in table of contents.  */

  /* To support dynamic binding of "modules" (whole object files) the symbol
     table must reflect the modules that the file was created from.  This is
     done by having a module table that has indexes and counts into the merged
     tables for each module.  The module structure that these two entries
     refer to is described below.  This exists only in a dynamically linked
     shared library file.  For executable and object modules the file only
     contains one module so everything in the file belongs to the module.  */

  unsigned long modtaboff;    /* File offset to module table.  */
  unsigned long nmodtab;      /* Number of module table entries.  */

  /* To support dynamic module binding the module structure for each module
     indicates the external references (defined and undefined) each module
     makes.  For each module there is an offset and a count into the
     reference symbol table for the symbols that the module references.
     This exists only in a dynamically linked shared library file.  For
     executable and object modules the defined external symbols and the
     undefined external symbols indicate the external references.  */

  unsigned long extrefsymoff; /* Offset to referenced symbol table.  */
  unsigned long nextrefsyms; /* Number of referenced symbol table entries. */

  /* The sections that contain "symbol pointers" and "routine stubs" have
     indexes and (implied counts based on the size of the section and fixed
     size of the entry) into the "indirect symbol" table for each pointer
     and stub.  For every section of these two types the index into the
     indirect symbol table is stored in the section header in the field
     reserved1.  An indirect symbol table entry is simply a 32bit index into
     the symbol table to the symbol that the pointer or stub is referring to.
     The indirect symbol table is ordered to match the entries in the section.  */

  unsigned long indirectsymoff; /* File offset to the indirect symbol table.  */
  unsigned long nindirectsyms; /* Number of indirect symbol table entries.  */

  /* To support relocating an individual module in a library file quickly the
     external relocation entries for each module in the library need to be
     accessed efficiently. Since the relocation entries canot be accessed
     through the section headers for a library file they are separated into
     groups of local and external entries further grouped by module. In this
     case the presents of this load command whose extreloff, nextrel,
     locreloff and nlocrel fields are non-zero indicates that the relocation
     entries of non-merged sections are not referenced through the section
     structures (and the reloff and nreloc fields in the section headers are
     set to zero).

     Since the relocation entries are not accessed through the section headers
     this requires the r_address field to be something other than a section
     offset to identify the item to be relocated.  In this case r_address is
     set to the offset from the vmaddr of the first LC_SEGMENT command.

     The relocation entries are grouped by module and the module table
     entries have indexes and counts into them for the group of external
     relocation entries for that the module.

     For sections that are merged across modules there must not be any
     remaining external relocation entries for them (for merged sections
     remaining relocation entries must be local).  */

  unsigned long extreloff;    /* Offset to external relocation entries.  */
  unsigned long nextrel;      /* Number of external relocation entries.  */

  /* All the local relocation entries are grouped together (they are not
     grouped by their module since they are only used if the object is moved
     from it statically link edited address).  */

  unsigned long locreloff;    /* Offset to local relocation entries.  */
  unsigned long nlocrel;      /* Number of local relocation entries.  */
#if 0
  bfd_mach_o_dylib_module *dylib_module;
  bfd_mach_o_dylib_table_of_content *dylib_toc;
  unsigned int *indirect_syms;
  bfd_mach_o_dylib_reference *ext_refs;
#endif /* 0 */
}
bfd_mach_o_dysymtab_command;

/* An indirect symbol table entry is simply a 32bit index into the symbol table
   to the symbol that the pointer or stub is refering to.  Unless it is for a
   non-lazy symbol pointer section for a defined symbol which strip(1) as
   removed.  In which case it has the value INDIRECT_SYMBOL_LOCAL.  If the
   symbol was also absolute INDIRECT_SYMBOL_ABS is or'ed with that.  */

#define INDIRECT_SYMBOL_LOCAL 0x80000000
#define INDIRECT_SYMBOL_ABS 0x40000000
#ifndef BFD_MACH_O_INDIRECT_SYMBOL_LOCAL
# ifdef INDIRECT_SYMBOL_LOCAL
#  define BFD_MACH_O_INDIRECT_SYMBOL_LOCAL INDIRECT_SYMBOL_LOCAL
# else
#  define BFD_MACH_O_INDIRECT_SYMBOL_LOCAL 0x80000000
# endif /* INDIRECT_SYMBOL_LOCAL */
#endif /* !BFD_MACH_O_INDIRECT_SYMBOL_LOCAL */
#ifndef BFD_MACH_O_INDIRECT_SYMBOL_ABS
# ifdef INDIRECT_SYMBOL_ABS
#  define BFD_MACH_O_INDIRECT_SYMBOL_ABS INDIRECT_SYMBOL_ABS
# else
#  define BFD_MACH_O_INDIRECT_SYMBOL_ABS 0x40000000
# endif /* INDIRECT_SYMBOL_ABS */
#endif /* !BFD_MACH_O_INDIRECT_SYMBOL_ABS */
#ifndef BFD_MACH_O_INDIRECT_SYMBOL_SIZE
# define BFD_MACH_O_INDIRECT_SYMBOL_SIZE 4
#endif /* !BFD_MACH_O_INDIRECT_SYMBOL_SIZE */

/* For LC_TWOLEVEL_HINTS: */
typedef struct bfd_mach_o_twolevel_hints_command
{
  /* Offset to the hint table: */
  unsigned int offset;

  /* Number of entries in the table: */
  unsigned int nhints;
}
bfd_mach_o_twolevel_hints_command;

/* For LC_PREBIND_CKSUM: */
typedef struct bfd_mach_o_prebind_cksum_command
{
  /* Checksum or zero: */
  unsigned int cksum;
}
bfd_mach_o_prebind_cksum_command;

/* For LC_THREAD or LC_UNIXTHREAD: */
typedef struct bfd_mach_o_thread_flavour
{
  unsigned long flavour;
  bfd_vma offset;
  unsigned long size;
}
bfd_mach_o_thread_flavour;

typedef struct bfd_mach_o_thread_command
{
  unsigned long nflavours;
  bfd_mach_o_thread_flavour *flavours;
  asection *section;
}
bfd_mach_o_thread_command;

typedef struct bfd_mach_o_main_command
{
  bfd_vma entryoffset; /* This is the offset of the entry point relative to the TEXT segment.  */
  bfd_vma stacksize; /* If non-zero, this is the original stack size.  */
}
bfd_mach_o_main_command;

/* For LC_LOAD_DYLINKER and LC_ID_DYLINKER: */
typedef struct bfd_mach_o_dylinker_command
{
  unsigned long cmd; /* LC_ID_DYLIB or LC_LOAD_DYLIB or LC_LOAD_UPWARD_DYLIB */
  unsigned long cmdsize;     /* Includes pathname string.  */
  unsigned long name_offset; /* Offset to library's path name.  */
  unsigned long name_len;    /* Offset to library's path name.  */
#if 0
  char *name_str;
#endif /* 0 */
  asection *section;
}
bfd_mach_o_dylinker_command;

/* For LC_LOAD_DYLIB, LC_LOAD_WEAK_DYLIB, LC_ID_DYLIB
 * or LC_REEXPORT_DYLIB: */
typedef struct bfd_mach_o_dylib_command
{
  unsigned long cmd;             /* LC_ID_DYLIB or LC_LOAD_DYLIB.  */
  unsigned long cmdsize;         /* Includes pathname string.  */
  unsigned long name_offset;     /* Offset to library's path name.  */
  unsigned long name_len;        /* Offset to library's path name.  */
  unsigned long timestamp;	 /* Library's build time stamp.  */
  unsigned long current_version; /* Library's current version number.  */
  unsigned long compatibility_version; /* Library's compatibility vers number.  */
#if 0
  char *name_str;
#endif /* 0 */
  asection *section;
}
bfd_mach_o_dylib_command;

/* For LC_PREBOUND_DYLIB: */
typedef struct bfd_mach_o_prebound_dylib_command
{
  unsigned long cmd;                 /* LC_PREBOUND_DYLIB.  */
  unsigned long cmdsize;             /* Includes strings.  */
  unsigned long name;                /* Library's path name.  */
#if 0
  unsigned int name_offset;          /* Library's path name.  */
#endif /* 0 */
  unsigned long nmodules;            /* Number of modules in library.  */
  unsigned long linked_modules;      /* Bit vector of linked modules.  */
#if 0
  unsigned int linked_modules_offset; /* Bit vector of linked modules.  */

  char *name_str;
  unsigned char *linked_modules;
#endif /* 0 */
  asection *section;
}
bfd_mach_o_prebound_dylib_command;

/* For LC_UUID: */
typedef struct bfd_mach_o_uuid_command
{
  unsigned char uuid[16];
}
bfd_mach_o_uuid_command;

/* For LC_CODE_SIGNATURE or LC_SEGMENT_SPLIT_INFO: */
typedef struct bfd_mach_o_linkedit_command
{
  unsigned long dataoff;
  unsigned long datasize;
}
bfd_mach_o_linkedit_command;

typedef struct bfd_mach_o_str_command
{
  unsigned long stroff;
  unsigned long str_len;
  char *str;
}
bfd_mach_o_str_command;

typedef struct bfd_mach_o_fvmlib_command
{
  unsigned int name_offset;
  char *name_str;
  unsigned int minor_version;
  unsigned int header_addr;
}
bfd_mach_o_fvmlib_command;

typedef struct bfd_mach_o_dyld_info_command
{
  /* File offset and size to rebase info: */
  unsigned int rebase_off;
  unsigned int rebase_size;
  unsigned char *rebase_content;

  /* File offset and size of binding info: */
  unsigned int bind_off;
  unsigned int bind_size;
  unsigned char *bind_content;

  /* File offset and size of weak binding info: */
  unsigned int weak_bind_off;
  unsigned int weak_bind_size;
  unsigned char *weak_bind_content;

  /* File offset and size of lazy binding info: */
  unsigned int lazy_bind_off;
  unsigned int lazy_bind_size;
  unsigned char *lazy_bind_content;

  /* File offset and size of export info: */
  unsigned int export_off;
  unsigned int export_size;
  unsigned char *export_content;
}
bfd_mach_o_dyld_info_command;

typedef struct bfd_mach_o_version_min_command
{
  unsigned char rel;
  unsigned char maj;
  unsigned char min;
  unsigned int reserved;
}
bfd_mach_o_version_min_command;

typedef struct bfd_mach_o_encryption_info_command
{
  unsigned int cryptoff;
  unsigned int cryptsize;
  unsigned int cryptid;
}
bfd_mach_o_encryption_info_command;

#if !defined(bfd_mach_o_main_command) && 0
typedef struct bfd_mach_o_main_command
{
  bfd_uint64_t entryoff;
  bfd_uint64_t stacksize;
}
bfd_mach_o_main_command;
#endif /* !bfd_mach_o_main_command && 0 */

typedef struct bfd_mach_o_source_version_command
{
  unsigned int a;
  unsigned short b;
  unsigned short c;
  unsigned short d;
  unsigned short e;
}
bfd_mach_o_source_version_command;

typedef struct bfd_mach_o_load_command
{
#if 0
  /* Next command in the single linked list: */
  struct bfd_mach_o_load_command *next;
#endif /* 0 */

  /* Type and required flag: */
  bfd_mach_o_load_command_type type;
  unsigned int type_required;

  /* Offset and length in the file: */
  bfd_vma offset;
  bfd_vma len;
  union
  {
    bfd_mach_o_segment_command segment;
    bfd_mach_o_symtab_command symtab;
    bfd_mach_o_dysymtab_command dysymtab;
    bfd_mach_o_thread_command thread;
    bfd_mach_o_dylib_command dylib;
    bfd_mach_o_dylinker_command dylinker;
    bfd_mach_o_prebound_dylib_command prebound_dylib;
#if 0
    bfd_mach_o_prebind_cksum_command prebind_cksum;
    bfd_mach_o_twolevel_hints_command twolevel_hints;
    bfd_mach_o_uuid_command uuid;
    bfd_mach_o_linkedit_command linkedit;
    bfd_mach_o_str_command str;
    bfd_mach_o_dyld_info_command dyld_info;
    bfd_mach_o_version_min_command version_min;
    bfd_mach_o_encryption_info_command encryption_info;
    bfd_mach_o_fvmlib_command fvmlib;
#endif /* 0 */
    bfd_mach_o_main_command main;
#if 0
    bfd_mach_o_source_version_command source_version;
#endif /* 0 */
  }
  command;
}
bfd_mach_o_load_command;

typedef struct mach_o_data_struct
{
  /* Mach-O header: */
  bfd_mach_o_header header;
  bfd_mach_o_load_command *commands;
#if 0
  /* Array of load commands (length is given by header.ncmds): */
  bfd_mach_o_load_command *first_command;
  bfd_mach_o_load_command *last_command;
#endif /* 0 */
  unsigned long nsymbols;
  asymbol *symbols;

  /* Flatten array of sections.  The array is 0-based: */
  unsigned long nsects;
  bfd_mach_o_section **sections;
  bfd *ibfd;
  unsigned char uuid[16];
  int scanning_load_cmds;
  int encrypted;
#if 0
  /* Used while writing: current length of the output file.  This is used
   * to allocate space in the file: */
  ufile_ptr filelen;

  /* As symtab is referenced by other load command, it is handy to have
   * a direct access to it.  Although it is not clearly stated, only one
   * symtab is expected: */
  bfd_mach_o_symtab_command *symtab;
  bfd_mach_o_dysymtab_command *dysymtab;

  /* A place to stash dwarf2 info for this bfd: */
  void *dwarf2_find_line_info;

  /* BFD of .dSYM file: */
  bfd *dsym_bfd;

  /* Cache of dynamic relocs: */
  arelent *dyn_reloc_cache;
#endif /* 0 */
}
mach_o_data_struct;

typedef struct mach_o_data_struct bfd_mach_o_data_struct;

typedef struct bfd_mach_o_xlat_name
{
  const char *name;
  unsigned long val;
}
bfd_mach_o_xlat_name;

/* Target specific routines: */
#ifndef bfd_mach_o_get_data
# define bfd_mach_o_get_data(abfd) ((abfd)->tdata.mach_o_data)
#endif /* !bfd_mach_o_get_data */
#ifndef bfd_mach_o_get_backend_data
# define bfd_mach_o_get_backend_data(abfd) \
   ((bfd_mach_o_backend_data*)(abfd)->xvec->backend_data)
#endif /* !bfd_mach_o_get_backend_data */

/* Get the Mach-O header for section SEC: */
#ifndef bfd_mach_o_get_mach_o_section
# define bfd_mach_o_get_mach_o_section(sec) \
   ((bfd_mach_o_section *)(sec)->used_by_bfd)
#endif /* !bfd_mach_o_get_mach_o_section */

/* APPLE LOCAL  Mach-O */
unsigned int bfd_mach_o_version(bfd *);
int bfd_mach_o_stub_library(bfd *);
int bfd_mach_o_uses_dylinker(bfd *);
bfd_boolean bfd_mach_o_encrypted_binary(bfd *);
/* APPLE LOCAL shared cache  */
bfd_boolean bfd_mach_o_in_shared_cached_memory(bfd *);

bfd_boolean bfd_mach_o_kernel_image(bfd *); /* Is this bfd a mach_kernel binary? */

bfd_boolean bfd_mach_o_valid(bfd *);
int bfd_mach_o_scan_read_symtab_symbol(bfd *, bfd_mach_o_symtab_command *,
                                       asymbol *, unsigned long);
int bfd_mach_o_scan_read_symtab_strtab(bfd *, bfd_mach_o_symtab_command *);
int bfd_mach_o_scan_read_symtab_symbols(bfd *,
                                        bfd_mach_o_symtab_command *);
int bfd_mach_o_scan_read_dysymtab_symbol(bfd *,
                                         bfd_mach_o_dysymtab_command *,
                                         bfd_mach_o_symtab_command *,
                                         asymbol *, unsigned long);
int bfd_mach_o_scan_start_address(bfd *);
int bfd_mach_o_scan(bfd *, bfd_mach_o_header *, bfd_mach_o_data_struct *);
bfd_boolean bfd_mach_o_mkobject(bfd *);
const bfd_target *bfd_mach_o_object_p(bfd *);
const bfd_target *bfd_mach_o_core_p(bfd *);
const bfd_target *bfd_mach_o_archive_p(bfd *);
bfd *bfd_mach_o_openr_next_archived_file(bfd *, bfd *);
int bfd_mach_o_lookup_section(bfd *, asection *,
                              bfd_mach_o_load_command **,
                              bfd_mach_o_section **) ATTRIBUTE_W_U_R;
int bfd_mach_o_lookup_command(bfd *, bfd_mach_o_load_command_type,
                              bfd_mach_o_load_command **);
unsigned long bfd_mach_o_stack_addr(enum bfd_mach_o_cpu_type);
bfd_boolean bfd_mach_o_core_fetch_environment(bfd *, unsigned char **,
                                              bfd_size_type *);
char *bfd_mach_o_core_file_failing_command(bfd *);
int bfd_mach_o_core_file_failing_signal(bfd *);
bfd_boolean bfd_mach_o_core_file_matches_executable_p(bfd *, bfd *);
bfd_boolean bfd_mach_o_get_uuid(bfd *, unsigned char* buf,
                                unsigned long buf_len);
unsigned int bfd_mach_o_flavour_from_string(unsigned long cputype,
                                            const char* s);
/* FIXME: prototype for bfd_mach_o_header_p needs typedef for bfd_cleanup */

#ifndef bfd_mach_o_find_line
# define bfd_mach_o_find_line _bfd_nosymbols_find_line
#endif /* !bfd_mach_o_find_line */

/* A placeholder in case we need to suppress emitting the dysymtab for some
 * reason (e.g. compatibility with older system versions).  */
#ifndef bfd_mach_o_should_emit_dysymtab
# define bfd_mach_o_should_emit_dysymtab(x) TRUE
#endif /* !bfd_mach_o_should_emit_dysymtab */

extern const bfd_mach_o_xlat_name bfd_mach_o_section_attribute_name[];
extern const bfd_mach_o_xlat_name bfd_mach_o_section_type_name[];

extern const bfd_target mach_o_be_vec;
extern const bfd_target mach_o_le_vec;
extern const bfd_target mach_o_fat_vec;

/* Interfaces between BFD names and Mach-O names: */
typedef struct mach_o_section_name_xlat
{
  const char *bfd_name;
  const char *mach_o_name;
  flagword bfd_flags;
  unsigned int macho_sectype;
  unsigned int macho_secattr;
  unsigned int sectalign;
} mach_o_section_name_xlat;

typedef struct mach_o_segment_name_xlat
{
  const char *segname;
  const mach_o_section_name_xlat *sections;
} mach_o_segment_name_xlat;

const mach_o_section_name_xlat *
bfd_mach_o_section_data_for_mach_sect(bfd *, const char *, const char *);
const mach_o_section_name_xlat *
bfd_mach_o_section_data_for_bfd_name(bfd *, const char *, const char **);

typedef struct bfd_mach_o_backend_data
{
  enum bfd_architecture arch;
  bfd_vma page_size;
  bfd_boolean (*_bfd_mach_o_swap_reloc_in)(arelent *, bfd_mach_o_reloc_info *);
  bfd_boolean (*_bfd_mach_o_swap_reloc_out)(arelent *, bfd_mach_o_reloc_info *);
  bfd_boolean (*_bfd_mach_o_print_thread)(bfd *, bfd_mach_o_thread_flavour *,
                                          void *, char *);
  const mach_o_segment_name_xlat *segsec_names_xlat;
  bfd_boolean (*bfd_mach_o_section_type_valid_for_target)(unsigned long);
}
bfd_mach_o_backend_data;


/* Values used in symbol.udata.i, to signal that the mach-o-specific data
 * in the symbol are not yet set, or need validation (where this is
 * possible): */
#ifndef SYM_MACHO_FIELDS_UNSET
# define SYM_MACHO_FIELDS_UNSET ((bfd_vma)-1L)
#endif /* !SYM_MACHO_FIELDS_UNSET */
#ifndef SYM_MACHO_FIELDS_NOT_VALIDATED
# define SYM_MACHO_FIELDS_NOT_VALIDATED ((bfd_vma)-2L)
#endif /* !SYM_MACHO_FIELDS_NOT_VALIDATED */

/* FIXME: */
#if defined(bfd_mach_o_section_already_linked) && 0
# undef bfd_mach_o_section_already_linked
#endif /* bfd_mach_o_section_already_linked && 0 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
 #  warning "mach-o.h already included once"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* _BFD_MACH_O_H_ */

/* EOF */
