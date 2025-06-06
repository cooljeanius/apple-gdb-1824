/* targets.c: Generic target-file-type support for the BFD library.
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

#include "sysdep.h"
#include "bfd.h"
#include "libbfd.h"
#include "fnmatch.h"

/*
   It is okay to see some:
#if 0
   directives in this source file, as targets.c uses them to exclude
   certain BFD vectors.  This comment is specially formatted to catch
   users who grep for ^#if 0, so please keep it this way!
*/

/*
SECTION
	Targets

DESCRIPTION
	Each port of BFD to a different machine requires the creation
	of a target back end. All the back end provides to the root
	part of BFD is a structure containing pointers to functions
	which perform certain low level operations on files. BFD
	translates the applications's requests through a pointer into
	calls to the back end routines.

	When a file is opened with <<bfd_openr>>, its format and
	target are unknown. BFD uses various mechanisms to determine
	how to interpret the file. The operations performed are:

	o Create a BFD by calling the internal routine
	<<_bfd_new_bfd>>, then call <<bfd_find_target>> with the
	target string supplied to <<bfd_openr>> and the new BFD pointer.

	o If a null target string was provided to <<bfd_find_target>>,
	look up the environment variable <<GNUTARGET>> and use
	that as the target string.

	o If the target string is still <<NULL>>, or the target string is
	<<default>>, then use the first item in the target vector
	as the target type, and set <<target_defaulted>> in the BFD to
	cause <<bfd_check_format>> to loop through all the targets.
	@xref{bfd_target}.  @xref{Formats}.

	o Otherwise, inspect the elements in the target vector
	one by one, until a match on target name is found. When found,
	use it.

	o Otherwise return the error <<bfd_error_invalid_target>> to
	<<bfd_openr>>.

	o <<bfd_openr>> attempts to open the file using
	<<bfd_open_file>>, and returns the BFD.

	Once the BFD has been opened and the target selected, the file
	format may be determined. This is done by calling
	<<bfd_check_format>> on the BFD with a suggested format.
	If <<target_defaulted>> has been set, each possible target
	type is tried to see if it recognizes the specified format.
	<<bfd_check_format>> returns <<TRUE>> when the caller guesses right.
@menu
@* bfd_target::
@end menu
*/

/*

INODE
	bfd_target,  , Targets, Targets
DOCDD
SUBSECTION
	bfd_target

DESCRIPTION
	This structure contains everything that BFD knows about a
	target. It includes things like its byte order, name, and which
	routines to call to do various operations.

	Every BFD points to a target structure with its <<xvec>>
	member.

	The macros below are used to dispatch to functions through the
	<<bfd_target>> vector. They are used in a number of macros further
	down in @file{bfd.h}, and are also used when calling various
	routines by hand inside the BFD implementation.  The @var{arglist}
	argument must be parenthesized; it contains all the arguments
	to the called function.

	They make the documentation (more) unpleasant to read, so if
	someone wants to fix this and not break the above, please do.

.#define BFD_SEND(bfd, message, arglist) \
.  ((*((bfd)->xvec->message)) arglist)
.
.#ifdef DEBUG_BFD_SEND
.#undef BFD_SEND
.#define BFD_SEND(bfd, message, arglist) \
.  (((bfd) && (bfd)->xvec && (bfd)->xvec->message) ? \
.    ((*((bfd)->xvec->message)) arglist) : \
.    (bfd_assert (__FILE__,__LINE__), NULL))
.#endif

	For operations which index on the BFD format:

.#define BFD_SEND_FMT(bfd, message, arglist) \
.  (((bfd)->xvec->message[(int) ((bfd)->format)]) arglist)
.
.#ifdef DEBUG_BFD_SEND
.#undef BFD_SEND_FMT
.#define BFD_SEND_FMT(bfd, message, arglist) \
.  (((bfd) && (bfd)->xvec && (bfd)->xvec->message) ? \
.   (((bfd)->xvec->message[(int) ((bfd)->format)]) arglist) : \
.   (bfd_assert (__FILE__,__LINE__), NULL))
.#endif
.
	This is the structure which defines the type of BFD this is.  The
	<<xvec>> member of the struct <<bfd>> itself points here.  Each
	module that implements access to a different target under BFD,
	defines one of these.

	FIXME, these names should be rationalised with the names of
	the entry points which call them. Too bad we can't have one
	macro to define them both!

.enum bfd_flavour
.{
.  bfd_target_unknown_flavour,
.  bfd_target_aout_flavour,
.  bfd_target_coff_flavour,
.  bfd_target_ecoff_flavour,
.  bfd_target_xcoff_flavour,
.  bfd_target_elf_flavour,
.  bfd_target_ieee_flavour,
.  bfd_target_nlm_flavour,
.  bfd_target_oasys_flavour,
.  bfd_target_tekhex_flavour,
.  bfd_target_srec_flavour,
.  bfd_target_verilog_flavour,
.  bfd_target_ihex_flavour,
.  bfd_target_som_flavour,
.  bfd_target_os9k_flavour,
.  bfd_target_versados_flavour,
.  bfd_target_msdos_flavour,
.  bfd_target_ovax_flavour,
.  bfd_target_evax_flavour,
.  bfd_target_mmo_flavour,
.  bfd_target_mach_o_flavour,
.  bfd_target_pef_flavour,
.  bfd_target_pef_xlib_flavour,
.  bfd_target_sym_flavour
.};
.
.enum bfd_endian { BFD_ENDIAN_BIG, BFD_ENDIAN_LITTLE, BFD_ENDIAN_UNKNOWN };
.
.{* Forward declaration.  *}
.typedef struct bfd_link_info _bfd_link_info; {* from ../include/bfdlink.h *}
.
.{* Forward declaration.  *}
.typedef struct flag_info flag_info;
.
.typedef struct bfd_target
.{
.  {* Identifies the kind of target, e.g., SunOS4, Ultrix, etc.  *}
.  const char *name;
.
. {* The "flavour" of a back end is a general indication about
.    the contents of a file.  *}
.  enum bfd_flavour flavour;
.
.  {* The order of bytes within the data area of a file.  *}
.  enum bfd_endian byteorder;
.
. {* The order of bytes within the header parts of a file.  *}
.  enum bfd_endian header_byteorder;
.
.  {* A mask of all the flags which an executable may have set -
.     from the set <<BFD_NO_FLAGS>>, <<HAS_RELOC>>, ...<<D_PAGED>>.  *}
.  flagword object_flags;
.
. {* A mask of all the flags which a section may have set - from
.    the set <<SEC_NO_FLAGS>>, <<SEC_ALLOC>>, ...<<SET_NEVER_LOAD>>.  *}
.  flagword section_flags;
.
. {* The character normally found at the front of a symbol.
.    (if any), perhaps `_'.  *}
.  char symbol_leading_char;
.
. {* The pad character for file names within an archive header.  *}
.  char ar_pad_char;
.
.  {* The maximum number of characters in an archive header.  *}
.  unsigned short ar_max_namelen;
.
.#if 0
.  {* How well this target matches, used to select between various
.     possible targets when more than one target matches.  *}
.  unsigned char match_priority;
.#endif
.
.  {* Entries for byte swapping for data. These are different from the
.     other entry points, since they don't take a BFD as the first argument.
.     Certain other handlers could do the same.  *}
.  bfd_uint64_t   (*bfd_getx64) (const void *);
.  bfd_int64_t    (*bfd_getx_signed_64) (const void *);
.  void           (*bfd_putx64) (bfd_uint64_t, void *);
.  bfd_vma        (*bfd_getx32) (const void *);
.  bfd_signed_vma (*bfd_getx_signed_32) (const void *);
.  void           (*bfd_putx32) (bfd_vma, void *);
.  bfd_vma        (*bfd_getx16) (const void *);
.  bfd_signed_vma (*bfd_getx_signed_16) (const void *);
.  void           (*bfd_putx16) (bfd_vma, void *);
.
.  {* Byte swapping for the headers.  *}
.  bfd_uint64_t   (*bfd_h_getx64) (const void *);
.  bfd_int64_t    (*bfd_h_getx_signed_64) (const void *);
.  void           (*bfd_h_putx64) (bfd_uint64_t, void *);
.  bfd_vma        (*bfd_h_getx32) (const void *);
.  bfd_signed_vma (*bfd_h_getx_signed_32) (const void *);
.  void           (*bfd_h_putx32) (bfd_vma, void *);
.  bfd_vma        (*bfd_h_getx16) (const void *);
.  bfd_signed_vma (*bfd_h_getx_signed_16) (const void *);
.  void           (*bfd_h_putx16) (bfd_vma, void *);
.
.  {* Format dependent routines: these are vectors of entry points
.     within the target vector structure, one for each format to check.  *}
.
.  {* Check the format of a file being read.  Return a <<bfd_target *>> or zero.  *}
.  const struct bfd_target *(*_bfd_check_format[bfd_type_end]) (bfd *);
.
.  {* Set the format of a file being written.  *}
.  bfd_boolean (*_bfd_set_format[bfd_type_end]) (bfd *);
.
.  {* Write cached information into a file being written, at <<bfd_close>>.  *}
.  bfd_boolean (*_bfd_write_contents[bfd_type_end]) (bfd *);
.
The general target vector.  These vectors are initialized using the
BFD_JUMP_TABLE macros.
.
.  {* Generic entry points.  *}
.#define BFD_JUMP_TABLE_GENERIC(NAME) \
.  NAME##_close_and_cleanup, \
.  NAME##_bfd_free_cached_info, \
.  NAME##_new_section_hook, \
.  NAME##_get_section_contents, \
.  NAME##_get_section_contents_in_window, \
.  NAME##_get_section_contents_in_window_with_mode
.
.  {* Called when the BFD is being closed to do any necessary cleanup.  *}
.  bfd_boolean (*_close_and_cleanup) (bfd *);
.  {* Ask the BFD to free all cached information.  *}
.  bfd_boolean (*_bfd_free_cached_info) (bfd *);
.  {* Called when a new section is created.  *}
.  bfd_boolean (*_new_section_hook) (bfd *, sec_ptr);
.  {* Read the contents of a section.  *}
.  bfd_boolean (*_bfd_get_section_contents)
.    (bfd *, sec_ptr, void *, file_ptr, bfd_size_type);
.  bfd_boolean (*_bfd_get_section_contents_in_window)
.    (bfd *, sec_ptr, bfd_window *, file_ptr, bfd_size_type);
.  bfd_boolean (*_bfd_get_section_contents_in_window_with_mode)
.    (bfd *, sec_ptr, bfd_window *, file_ptr, bfd_size_type, bfd_boolean);
.
.  {* Entry points to copy private data.  *}
.#define BFD_JUMP_TABLE_COPY(NAME) \
.  NAME##_bfd_copy_private_bfd_data, \
.  NAME##_bfd_merge_private_bfd_data, \
.  NAME##_bfd_copy_private_section_data, \
.  NAME##_bfd_copy_private_symbol_data, \
.  NAME##_bfd_copy_private_header_data, \
.  NAME##_bfd_set_private_flags, \
.  NAME##_bfd_print_private_bfd_data
.
.  {* Called to copy BFD general private data from one object file
.     to another.  *}
.  bfd_boolean (*_bfd_copy_private_bfd_data) (bfd *, bfd *);
.  {* Called to merge BFD general private data from one object file
.     to a common output file when linking.  *}
.  bfd_boolean (*_bfd_merge_private_bfd_data) (bfd *, bfd *);
.  {* Called to copy BFD private section data from one object file
.     to another.  *}
.  bfd_boolean (*_bfd_copy_private_section_data)
.    (bfd *, sec_ptr, bfd *, sec_ptr);
.  {* Called to copy BFD private symbol data from one symbol
.     to another.  *}
.  bfd_boolean (*_bfd_copy_private_symbol_data)
.    (bfd *, asymbol *, bfd *, asymbol *);
.  {* Called to copy BFD private header data from one object file
.     to another.  *}
.  bfd_boolean (*_bfd_copy_private_header_data)
.    (bfd *, bfd *);
.  {* Called to set private backend flags.  *}
.  bfd_boolean (*_bfd_set_private_flags) (bfd *, flagword);
.
.  {* Called to print private BFD data.  *}
.  bfd_boolean (*_bfd_print_private_bfd_data) (bfd *, void *);
.
.  {* Core file entry points.  *}
.#define BFD_JUMP_TABLE_CORE(NAME) \
.  NAME##_core_file_failing_command, \
.  NAME##_core_file_failing_signal, \
.  NAME##_core_file_matches_executable_p
.
.  char *      (*_core_file_failing_command) (bfd *);
.  int         (*_core_file_failing_signal) (bfd *);
.  bfd_boolean (*_core_file_matches_executable_p) (bfd *, bfd *);
.
.  {* Archive entry points.  *}
.#define BFD_JUMP_TABLE_ARCHIVE(NAME) \
.  NAME##_slurp_armap, \
.  NAME##_slurp_extended_name_table, \
.  NAME##_construct_extended_name_table, \
.  NAME##_truncate_arname, \
.  NAME##_write_armap, \
.  NAME##_read_ar_hdr, \
.  NAME##_openr_next_archived_file, \
.  NAME##_get_elt_at_index, \
.  NAME##_generic_stat_arch_elt, \
.  NAME##_update_armap_timestamp
.
.  bfd_boolean (*_bfd_slurp_armap) (bfd *);
.  bfd_boolean (*_bfd_slurp_extended_name_table) (bfd *);
.  bfd_boolean (*_bfd_construct_extended_name_table)
.    (bfd *, char **, bfd_size_type *, const char **);
.  void        (*_bfd_truncate_arname) (bfd *, const char *, char *);
.  bfd_boolean (*write_armap)
.    (bfd *, unsigned int, struct orl *, unsigned int, int);
.  void *      (*_bfd_read_ar_hdr_fn) (bfd *);
.  bfd *       (*openr_next_archived_file) (bfd *, bfd *);
.#define bfd_get_elt_at_index(b,i) BFD_SEND (b, _bfd_get_elt_at_index, (b,i))
.  bfd *       (*_bfd_get_elt_at_index) (bfd *, symindex);
.  int         (*_bfd_stat_arch_elt) (bfd *, struct stat *);
.  bfd_boolean (*_bfd_update_armap_timestamp) (bfd *);
.
.  {* Entry points used for symbols.  *}
.#define BFD_JUMP_TABLE_SYMBOLS(NAME) \
.  NAME##_get_symtab_upper_bound, \
.  NAME##_canonicalize_symtab, \
.  NAME##_make_empty_symbol, \
.  NAME##_print_symbol, \
.  NAME##_get_symbol_info, \
.  NAME##_bfd_is_local_label_name, \
.  NAME##_bfd_is_target_special_symbol, \
.  NAME##_get_lineno, \
.  NAME##_find_nearest_line, \
.  _bfd_generic_find_line, \
.  NAME##_find_inliner_info, \
.  NAME##_bfd_make_debug_symbol, \
.  NAME##_read_minisymbols, \
.  NAME##_minisymbol_to_symbol
.
.  long        (*_bfd_get_symtab_upper_bound) (bfd *);
.  long        (*_bfd_canonicalize_symtab)
.    (bfd *, struct bfd_symbol **);
.  struct bfd_symbol *
.              (*_bfd_make_empty_symbol) (bfd *);
.  void        (*_bfd_print_symbol)
.    (bfd *, void *, struct bfd_symbol *, bfd_print_symbol_type);
.#define bfd_print_symbol(b,p,s,e) BFD_SEND (b, _bfd_print_symbol, (b,p,s,e))
.  void        (*_bfd_get_symbol_info)
.    (bfd *, struct bfd_symbol *, symbol_info *);
.#define bfd_get_symbol_info(b,p,e) BFD_SEND (b, _bfd_get_symbol_info, (b,p,e))
.  bfd_boolean (*_bfd_is_local_label_name) (bfd *, const char *);
.  bfd_boolean (*_bfd_is_target_special_symbol) (bfd *, asymbol *);
.  alent *     (*_get_lineno) (bfd *, struct bfd_symbol *);
.  bfd_boolean (*_bfd_find_nearest_line)
.    (bfd *, struct bfd_section *, struct bfd_symbol **, bfd_vma,
.     const char **, const char **, unsigned int *);
.  bfd_boolean (*_bfd_find_line)
.    (bfd *, struct bfd_symbol **, struct bfd_symbol *,
.     const char **, unsigned int *);
.  bfd_boolean (*_bfd_find_inliner_info)
.    (bfd *, const char **, const char **, unsigned int *);
. {* Back-door to allow format-aware applications to create debug symbols
.    while using BFD for everything else.  Currently used by the assembler
.    when creating COFF files.  *}
.  asymbol *   (*_bfd_make_debug_symbol)
.    (bfd *, void *, unsigned long size);
.#define bfd_read_minisymbols(b, d, m, s) \
.  BFD_SEND (b, _read_minisymbols, (b, d, m, s))
.  long        (*_read_minisymbols)
.    (bfd *, bfd_boolean, void **, unsigned int *);
.#define bfd_minisymbol_to_symbol(b, d, m, f) \
.  BFD_SEND (b, _minisymbol_to_symbol, (b, d, m, f))
.  asymbol *   (*_minisymbol_to_symbol)
.    (bfd *, bfd_boolean, const void *, asymbol *);
.
.  {* Routines for relocs.  *}
.#define BFD_JUMP_TABLE_RELOCS(NAME) \
.  NAME##_get_reloc_upper_bound, \
.  NAME##_canonicalize_reloc, \
.  NAME##_bfd_reloc_type_lookup
.
.  long        (*_get_reloc_upper_bound) (bfd *, sec_ptr);
.  long        (*_bfd_canonicalize_reloc)
.    (bfd *, sec_ptr, arelent **, struct bfd_symbol **);
.  {* See documentation on reloc types.  *}
.  reloc_howto_type *
.              (*reloc_type_lookup) (bfd *, bfd_reloc_code_real_type);
.
.  {* Routines used when writing an object file.  *}
.#define BFD_JUMP_TABLE_WRITE(NAME) \
.  NAME##_set_arch_mach, \
.  NAME##_set_section_contents
.
.  bfd_boolean (*_bfd_set_arch_mach)
.    (bfd *, enum bfd_architecture, unsigned long);
.  bfd_boolean (*_bfd_set_section_contents)
.    (bfd *, sec_ptr, const void *, file_ptr, bfd_size_type);
.
.  {* Routines used by the linker.  *}
.#define BFD_JUMP_TABLE_LINK(NAME) \
.  NAME##_sizeof_headers, \
.  NAME##_bfd_get_relocated_section_contents, \
.  NAME##_bfd_relax_section, \
.  NAME##_bfd_link_hash_table_create, \
.  NAME##_bfd_link_hash_table_free, \
.  NAME##_bfd_link_add_symbols, \
.  NAME##_bfd_link_just_syms, \
.  NAME##_bfd_final_link, \
.  NAME##_bfd_link_split_section, \
.  NAME##_bfd_gc_sections, \
.  NAME##_bfd_merge_sections, \
.  NAME##_bfd_is_group_section, \
.  NAME##_bfd_discard_group, \
.  NAME##_section_already_linked \
.
.  int         (*_bfd_sizeof_headers) (bfd *, bfd_boolean);
.  bfd_byte *  (*_bfd_get_relocated_section_contents)
.    (bfd *, struct bfd_link_info *, struct bfd_link_order *,
.     bfd_byte *, bfd_boolean, struct bfd_symbol **);
.
.  bfd_boolean (*_bfd_relax_section)
.    (bfd *, struct bfd_section *, struct bfd_link_info *, bfd_boolean *);
.
.  {* Create a hash table for the linker.  Different backends store
.     different information in this table.  *}
.  struct bfd_link_hash_table *
.              (*_bfd_link_hash_table_create) (bfd *);
.
.  {* Release the memory associated with the linker hash table.  *}
.  void        (*_bfd_link_hash_table_free) (struct bfd_link_hash_table *);
.
.  {* Add symbols from this object file into the hash table.  *}
.  bfd_boolean (*_bfd_link_add_symbols) (bfd *, struct bfd_link_info *);
.
.  {* Indicate that we are only retrieving symbol values from this section.  *}
.  void        (*_bfd_link_just_syms) (asection *, struct bfd_link_info *);
.
.  {* Do a link based on the link_order structures attached to each
.     section of the BFD.  *}
.  bfd_boolean (*_bfd_final_link) (bfd *, struct bfd_link_info *);
.
.  {* Should this section be split up into smaller pieces during linking.  *}
.  bfd_boolean (*_bfd_link_split_section) (bfd *, struct bfd_section *);
.
.  {* Remove sections that are not referenced from the output.  *}
.  bfd_boolean (*_bfd_gc_sections) (bfd *, struct bfd_link_info *);
.
.  {* Attempt to merge SEC_MERGE sections.  *}
.  bfd_boolean (*_bfd_merge_sections) (bfd *, struct bfd_link_info *);
.
.  {* Is this section a member of a group?  *}
.  bfd_boolean (*_bfd_is_group_section) (bfd *, const struct bfd_section *);
.
.  {* Discard members of a group.  *}
.  bfd_boolean (*_bfd_discard_group) (bfd *, struct bfd_section *);
.
.  {* Check if SEC has been already linked during a reloceatable or
.     final link.  *}
.  void (*_section_already_linked) (bfd *, struct bfd_section *);
.
.  {* Routines to handle dynamic symbols and relocs.  *}
.#define BFD_JUMP_TABLE_DYNAMIC(NAME) \
.  NAME##_get_dynamic_symtab_upper_bound, \
.  NAME##_canonicalize_dynamic_symtab, \
.  NAME##_get_synthetic_symtab, \
.  NAME##_get_dynamic_reloc_upper_bound, \
.  NAME##_canonicalize_dynamic_reloc
.
.  {* Get the amount of memory required to hold the dynamic symbols.  *}
.  long        (*_bfd_get_dynamic_symtab_upper_bound) (bfd *);
.  {* Read in the dynamic symbols.  *}
.  long        (*_bfd_canonicalize_dynamic_symtab)
.    (bfd *, struct bfd_symbol **);
.  {* Create synthetized symbols.  *}
.  long        (*_bfd_get_synthetic_symtab)
.    (bfd *, long, struct bfd_symbol **, long, struct bfd_symbol **,
.     struct bfd_symbol **);
.  {* Get the amount of memory required to hold the dynamic relocs.  *}
.  long        (*_bfd_get_dynamic_reloc_upper_bound) (bfd *);
.  {* Read in the dynamic relocs.  *}
.  long        (*_bfd_canonicalize_dynamic_reloc)
.    (bfd *, arelent **, struct bfd_symbol **);
.

A pointer to an alternative bfd_target in case the current one is not
satisfactory.  This can happen when the target cpu supports both big
and little endian code, and target chosen by the linker has the wrong
endianness.  The function open_output() in ld/ldlang.c uses this field
to find an alternative output format that is suitable.

.  {* Opposite endian version of this target.  *}
.  const struct bfd_target * alternative_target;
.

.  {* Data for use by back-end routines, which isn't
.     generic enough to belong in this structure.  *}
.  const void *backend_data;
.
.} bfd_target;
.
*/

/* All known xvecs (even those that do NOT compile on all systems).
   Alphabetized for easy reference.
   They are listed a second time below, since
   we cannot intermix extern's and initializers.  */
extern const bfd_target a29kcoff_big_vec;
extern const bfd_target aarch64_elf32_be_vec;
extern const bfd_target aarch64_elf32_le_vec;
extern const bfd_target aarch64_elf64_be_vec;
extern const bfd_target aarch64_elf64_le_vec;
extern const bfd_target aarch64_mach_o_vec;
extern const bfd_target alpha_ecoff_le_vec;
extern const bfd_target alpha_elf64_vec;
extern const bfd_target alpha_elf64_fbsd_vec;
extern const bfd_target alpha_nlm32_vec;
extern const bfd_target alpha_vms_vec;
extern const bfd_target alpha_vms_lib_txt_vec;
extern const bfd_target am33_elf32_linux_vec;
extern const bfd_target a_out_adobe_vec;
extern const bfd_target aix5coff64_vec;
extern const bfd_target aout0_be_vec;
extern const bfd_target aout0_big_vec;
extern const bfd_target aout64_vec;
extern const bfd_target aout_vec;
extern const bfd_target aout_adobe_vec;
extern const bfd_target aout_arm_big_vec;
extern const bfd_target aout_arm_little_vec;
extern const bfd_target aout_mips_big_vec;
extern const bfd_target aout_mips_little_vec;
extern const bfd_target apollocoff_vec;
extern const bfd_target arc_elf32_be_vec;
extern const bfd_target arc_elf32_le_vec;
extern const bfd_target arm_aout_be_vec;
extern const bfd_target arm_aout_le_vec;
extern const bfd_target arm_aout_nbsd_vec;
extern const bfd_target arm_aout_riscix_vec;
extern const bfd_target arm_coff_be_vec;
extern const bfd_target arm_coff_le_vec;
extern const bfd_target arm_elf32_be_vec;
extern const bfd_target arm_elf32_le_vec;
extern const bfd_target arm_elf32_nacl_be_vec;
extern const bfd_target arm_elf32_nacl_le_vec;
extern const bfd_target arm_elf32_symbian_be_vec;
extern const bfd_target arm_elf32_symbian_le_vec;
extern const bfd_target arm_elf32_vxworks_be_vec;
extern const bfd_target arm_elf32_vxworks_le_vec;
extern const bfd_target arm_epoc_pe_big_vec;
extern const bfd_target arm_epoc_pe_little_vec;
extern const bfd_target arm_epoc_pei_big_vec;
extern const bfd_target arm_epoc_pei_little_vec;
extern const bfd_target arm_mach_o_vec;
extern const bfd_target arm_pe_be_vec;
extern const bfd_target arm_pe_le_vec;
extern const bfd_target arm_pe_epoc_be_vec;
extern const bfd_target arm_pe_epoc_le_vec;
extern const bfd_target arm_pe_wince_be_vec;
extern const bfd_target arm_pe_wince_le_vec;
extern const bfd_target arm_pei_be_vec;
extern const bfd_target arm_pei_le_vec;
extern const bfd_target arm_pei_epoc_be_vec;
extern const bfd_target arm_pei_epoc_le_vec;
extern const bfd_target arm_pei_wince_be_vec;
extern const bfd_target arm_pei_wince_le_vec;
extern const bfd_target armcoff_big_vec;
extern const bfd_target armcoff_little_vec;
extern const bfd_target armnetbsd_vec;
extern const bfd_target armpe_big_vec;
extern const bfd_target armpe_little_vec;
extern const bfd_target armpei_big_vec;
extern const bfd_target armpei_little_vec;
extern const bfd_target avr_elf32_vec;
extern const bfd_target b_out_vec_big_host;
extern const bfd_target b_out_vec_little_host;
extern const bfd_target bfd_efi_app_ia32_vec;
extern const bfd_target bfd_efi_app_ia64_vec;
extern const bfd_target bfd_elf32_avr_vec;
extern const bfd_target bfd_elf32_bfin_vec;
extern const bfd_target bfd_elf32_big_generic_vec;
extern const bfd_target bfd_elf32_bigarc_vec;
extern const bfd_target bfd_elf32_bigarm_vec;
extern const bfd_target bfd_elf32_bigarm_symbian_vec;
extern const bfd_target bfd_elf32_bigarm_vxworks_vec;
extern const bfd_target bfd_elf32_bigmips_vec;
extern const bfd_target bfd_elf32_cr16c_vec;
extern const bfd_target bfd_elf32_cris_vec;
extern const bfd_target bfd_elf32_crx_vec;
extern const bfd_target bfd_elf32_d10v_vec;
extern const bfd_target bfd_elf32_d30v_vec;
extern const bfd_target bfd_elf32_dlx_big_vec;
extern const bfd_target bfd_elf32_fr30_vec;
extern const bfd_target bfd_elf32_frv_vec;
extern const bfd_target bfd_elf32_frvfdpic_vec;
extern const bfd_target bfd_elf32_h8300_vec;
extern const bfd_target bfd_elf32_hppa_linux_vec;
extern const bfd_target bfd_elf32_hppa_nbsd_vec;
extern const bfd_target bfd_elf32_hppa_vec;
extern const bfd_target bfd_elf32_i370_vec;
extern const bfd_target bfd_elf32_i386_freebsd_vec;
extern const bfd_target bfd_elf32_i386_vxworks_vec;
extern const bfd_target bfd_elf32_i386_vec;
extern const bfd_target bfd_elf32_i860_little_vec;
extern const bfd_target bfd_elf32_i860_vec;
extern const bfd_target bfd_elf32_i960_vec;
extern const bfd_target bfd_elf32_ia64_big_vec;
extern const bfd_target bfd_elf32_ia64_hpux_big_vec;
extern const bfd_target bfd_elf32_ip2k_vec;
extern const bfd_target bfd_elf32_iq2000_vec;
extern const bfd_target bfd_elf32_little_generic_vec;
extern const bfd_target bfd_elf32_littlearc_vec;
extern const bfd_target bfd_elf32_littlearm_vec;
extern const bfd_target bfd_elf32_littlearm_symbian_vec;
extern const bfd_target bfd_elf32_littlearm_vxworks_vec;
extern const bfd_target bfd_elf32_littlemips_vec;
extern const bfd_target bfd_elf32_m32c_vec;
extern const bfd_target bfd_elf32_m32r_vec;
extern const bfd_target bfd_elf32_m32rle_vec;
extern const bfd_target bfd_elf32_m32rlin_vec;
extern const bfd_target bfd_elf32_m32rlelin_vec;
extern const bfd_target bfd_elf32_m68hc11_vec;
extern const bfd_target bfd_elf32_m68hc12_vec;
extern const bfd_target bfd_elf32_m68k_vec;
extern const bfd_target bfd_elf32_m88k_vec;
extern const bfd_target bfd_elf32_mcore_big_vec;
extern const bfd_target bfd_elf32_mcore_little_vec;
extern const bfd_target bfd_elf32_mn10200_vec;
extern const bfd_target bfd_elf32_mn10300_vec;
extern const bfd_target bfd_elf32_ms1_vec;
extern const bfd_target bfd_elf32_msp430_vec;
extern const bfd_target bfd_elf32_nbigmips_vec;
extern const bfd_target bfd_elf32_nlittlemips_vec;
extern const bfd_target bfd_elf32_ntradbigmips_vec;
extern const bfd_target bfd_elf32_ntradlittlemips_vec;
extern const bfd_target bfd_elf32_openrisc_vec;
extern const bfd_target bfd_elf32_or32_big_vec;
extern const bfd_target bfd_elf32_pj_vec;
extern const bfd_target bfd_elf32_pjl_vec;
extern const bfd_target bfd_elf32_powerpc_vec;
extern const bfd_target bfd_elf32_powerpcle_vec;
extern const bfd_target bfd_elf32_powerpc_vxworks_vec;
extern const bfd_target bfd_elf32_s390_vec;
extern const bfd_target bfd_elf32_sh64_vec;
extern const bfd_target bfd_elf32_sh64l_vec;
extern const bfd_target bfd_elf32_sh64lin_vec;
extern const bfd_target bfd_elf32_sh64blin_vec;
extern const bfd_target bfd_elf32_sh64lnbsd_vec;
extern const bfd_target bfd_elf32_sh64nbsd_vec;
extern const bfd_target bfd_elf32_sh_vec;
extern const bfd_target bfd_elf32_shblin_vec;
extern const bfd_target bfd_elf32_shl_vec;
extern const bfd_target bfd_elf32_shl_symbian_vec;
extern const bfd_target bfd_elf32_shlin_vec;
extern const bfd_target bfd_elf32_shlnbsd_vec;
extern const bfd_target bfd_elf32_shnbsd_vec;
extern const bfd_target bfd_elf32_sparc_vec;
extern const bfd_target bfd_elf32_tradbigmips_vec;
extern const bfd_target bfd_elf32_tradlittlemips_vec;
extern const bfd_target bfd_elf32_us_cris_vec;
extern const bfd_target bfd_elf32_v850_vec;
extern const bfd_target bfd_elf32_vax_vec;
extern const bfd_target bfd_elf32_xstormy16_vec;
extern const bfd_target bfd_elf32_xtensa_be_vec;
extern const bfd_target bfd_elf32_xtensa_le_vec;
extern const bfd_target bfd_elf64_alpha_freebsd_vec;
extern const bfd_target bfd_elf64_alpha_vec;
extern const bfd_target bfd_elf64_big_generic_vec;
extern const bfd_target bfd_elf64_bigmips_vec;
extern const bfd_target bfd_elf64_hppa_linux_vec;
extern const bfd_target bfd_elf64_hppa_vec;
extern const bfd_target bfd_elf64_ia64_big_vec;
extern const bfd_target bfd_elf64_ia64_hpux_big_vec;
extern const bfd_target bfd_elf64_ia64_little_vec;
extern const bfd_target bfd_elf64_little_generic_vec;
extern const bfd_target bfd_elf64_littlemips_vec;
extern const bfd_target bfd_elf64_mmix_vec;
extern const bfd_target bfd_elf64_powerpc_vec;
extern const bfd_target bfd_elf64_powerpcle_vec;
extern const bfd_target bfd_elf64_s390_vec;
extern const bfd_target bfd_elf64_sh64_vec;
extern const bfd_target bfd_elf64_sh64l_vec;
extern const bfd_target bfd_elf64_sh64lin_vec;
extern const bfd_target bfd_elf64_sh64blin_vec;
extern const bfd_target bfd_elf64_sh64lnbsd_vec;
extern const bfd_target bfd_elf64_sh64nbsd_vec;
extern const bfd_target bfd_elf64_sparc_vec;
extern const bfd_target bfd_elf64_tradbigmips_vec;
extern const bfd_target bfd_elf64_tradlittlemips_vec;
extern const bfd_target bfd_elf64_x86_64_vec;
extern const bfd_target bfd_mmo_vec;
extern const bfd_target bfd_powerpc_pe_vec;
extern const bfd_target bfd_powerpc_pei_vec;
extern const bfd_target bfd_powerpcle_pe_vec;
extern const bfd_target bfd_powerpcle_pei_vec;
extern const bfd_target bfin_elf32_vec;
extern const bfd_target bfin_elf32_fdpic_vec;
extern const bfd_target bout_be_vec;
extern const bfd_target bout_le_vec;
extern const bfd_target cr16_elf32_vec;
extern const bfd_target cr16c_elf32_vec;
extern const bfd_target cris_aout_vec;
extern const bfd_target cris_elf32_vec;
extern const bfd_target cris_elf32_us_vec;
extern const bfd_target crx_elf32_vec;
extern const bfd_target d10v_elf32_vec;
extern const bfd_target d30v_elf32_vec;
extern const bfd_target demo_64_vec;
extern const bfd_target dlx_elf32_be_vec;
extern const bfd_target ecoff_big_vec;
extern const bfd_target ecoff_biglittle_vec;
extern const bfd_target ecoff_little_vec;
extern const bfd_target ecoffalpha_little_vec;
extern const bfd_target elf32_be_vec;
extern const bfd_target elf32_le_vec;
extern const bfd_target elf64_be_vec;
extern const bfd_target elf64_le_vec;
extern const bfd_target epiphany_elf32_vec;
extern const bfd_target fr30_elf32_vec;
extern const bfd_target frv_elf32_vec;
extern const bfd_target frv_elf32_fdpic_vec;
extern const bfd_target go32coff_vec;
extern const bfd_target go32stubbedcoff_vec;
extern const bfd_target h8300_coff_vec;
extern const bfd_target h8300_elf32_vec;
extern const bfd_target h8300coff_vec;
extern const bfd_target h8500_coff_vec;
extern const bfd_target h8500coff_vec;
extern const bfd_target host_aout_vec;
extern const bfd_target hp300bsd_vec;
extern const bfd_target hp300hpux_vec;
extern const bfd_target hppa_elf32_vec;
extern const bfd_target hppa_elf32_linux_vec;
extern const bfd_target hppa_elf32_nbsd_vec;
extern const bfd_target hppa_elf64_vec;
extern const bfd_target hppa_elf64_linux_vec;
extern const bfd_target hppa_som_vec;
extern const bfd_target i370_elf32_vec;
extern const bfd_target i386_aout_vec;
extern const bfd_target i386_aout_bsd_vec;
extern const bfd_target i386_aout_dynix_vec;
extern const bfd_target i386_aout_fbsd_vec;
extern const bfd_target i386_aout_linux_vec;
extern const bfd_target i386_aout_lynx_vec;
extern const bfd_target i386_aout_mach3_vec;
extern const bfd_target i386_aout_nbsd_vec;
extern const bfd_target i386_aout_os9k_vec;
extern const bfd_target i386_coff_vec;
extern const bfd_target i386_coff_go32_vec;
extern const bfd_target i386_coff_go32stubbed_vec;
extern const bfd_target i386_coff_lynx_vec;
extern const bfd_target i386_elf32_vec;
extern const bfd_target i386_elf32_fbsd_vec;
extern const bfd_target i386_elf32_nacl_vec;
extern const bfd_target i386_elf32_sol2_vec;
extern const bfd_target i386_elf32_vxworks_vec;
extern const bfd_target i386_mach_o_vec;
extern const bfd_target i386_msdos_vec;
extern const bfd_target i386_nlm32_vec;
extern const bfd_target i386_pe_vec;
extern const bfd_target i386_pei_vec;
extern const bfd_target i386aout_vec;
extern const bfd_target i386bsd_vec;
extern const bfd_target i386coff_vec;
extern const bfd_target i386dynix_vec;
extern const bfd_target i386freebsd_vec;
extern const bfd_target i386linux_vec;
extern const bfd_target i386lynx_aout_vec;
extern const bfd_target i386lynx_coff_vec;
extern const bfd_target i386mach3_vec;
extern const bfd_target i386msdos_vec;
extern const bfd_target i386netbsd_vec;
extern const bfd_target i386os9k_vec;
extern const bfd_target i386pe_vec;
extern const bfd_target i386pei_vec;
extern const bfd_target i860_coff_vec;
extern const bfd_target i860_elf32_vec;
extern const bfd_target i860_elf32_le_vec;
extern const bfd_target i860coff_vec;
extern const bfd_target i960_elf32_vec;
extern const bfd_target ia64_elf32_be_vec;
extern const bfd_target ia64_elf32_hpux_be_vec;
extern const bfd_target ia64_elf64_be_vec;
extern const bfd_target ia64_elf64_le_vec;
extern const bfd_target ia64_elf64_hpux_be_vec;
extern const bfd_target ia64_elf64_vms_vec;
extern const bfd_target ia64_pei_vec;
extern const bfd_target icoff_be_vec;
extern const bfd_target icoff_big_vec;
extern const bfd_target icoff_le_vec;
extern const bfd_target icoff_little_vec;
extern const bfd_target ieee_vec;
extern const bfd_target ip2k_elf32_vec;
extern const bfd_target iq2000_elf32_vec;
extern const bfd_target k1om_elf64_vec;
extern const bfd_target k1om_elf64_fbsd_vec;
extern const bfd_target l1om_elf64_vec;
extern const bfd_target l1om_elf64_fbsd_vec;
extern const bfd_target lm32_elf32_vec;
extern const bfd_target lm32_elf32_fdpic_vec;
extern const bfd_target loongarch_elf64_vec;
extern const bfd_target loongarch_elf32_vec;
extern const bfd_target loongarch64_pei_vec;
extern const bfd_target m32c_elf32_vec;
extern const bfd_target m32r_elf32_vec;
extern const bfd_target m32r_elf32_le_vec;
extern const bfd_target m32r_elf32_linux_vec;
extern const bfd_target m32r_elf32_linux_le_vec;
extern const bfd_target m68hc11_elf32_vec;
extern const bfd_target m68hc12_elf32_vec;
extern const bfd_target m68k_aout_4knbsd_vec;
extern const bfd_target m68k_aout_hp300bsd_vec;
extern const bfd_target m68k_aout_hp300hpux_vec;
extern const bfd_target m68k_aout_linux_vec;
extern const bfd_target m68k_aout_nbsd_vec;
extern const bfd_target m68k_aout_newsos3_vec;
extern const bfd_target m68k_coff_vec;
extern const bfd_target m68k_coff_apollo_vec;
extern const bfd_target m68k_coff_aux_vec;
extern const bfd_target m68k_coff_sysv_vec;
extern const bfd_target m68k_coff_un_vec;
extern const bfd_target m68k_elf32_vec;
extern const bfd_target m68k_versados_vec;
extern const bfd_target m68k4knetbsd_vec;
extern const bfd_target m68kaux_coff_vec;
extern const bfd_target m68kcoff_vec;
extern const bfd_target m68kcoffun_vec;
extern const bfd_target m68klinux_vec;
extern const bfd_target m68knetbsd_vec;
extern const bfd_target m68ksysvcoff_vec;
extern const bfd_target m88k_aout_mach3_vec;
extern const bfd_target m88k_aout_obsd_vec;
extern const bfd_target m88k_coff_bcs_vec;
extern const bfd_target m88k_elf32_vec;
extern const bfd_target m88kbcs_vec;
extern const bfd_target m88kmach3_vec;
extern const bfd_target m88kopenbsd_vec;
extern const bfd_target mach_o_be_vec;
extern const bfd_target mach_o_le_vec;
extern const bfd_target mach_o_fat_vec;
extern const bfd_target maxqcoff_vec;
extern const bfd_target mcore_elf32_be_vec;
extern const bfd_target mcore_elf32_le_vec;
extern const bfd_target mcore_pe_be_vec;
extern const bfd_target mcore_pe_big_vec;
extern const bfd_target mcore_pe_le_vec;
extern const bfd_target mcore_pe_little_vec;
extern const bfd_target mcore_pei_be_vec;
extern const bfd_target mcore_pei_big_vec;
extern const bfd_target mcore_pei_le_vec;
extern const bfd_target mcore_pei_little_vec;
extern const bfd_target mep_elf32_vec;
extern const bfd_target mep_elf32_le_vec;
extern const bfd_target metag_elf32_vec;
extern const bfd_target microblaze_elf32_vec;
extern const bfd_target microblaze_elf32_le_vec;
extern const bfd_target mips_aout_be_vec;
extern const bfd_target mips_aout_le_vec;
extern const bfd_target mips_ecoff_be_vec;
extern const bfd_target mips_ecoff_le_vec;
extern const bfd_target mips_ecoff_bele_vec;
extern const bfd_target mips_elf32_be_vec;
extern const bfd_target mips_elf32_le_vec;
extern const bfd_target mips_elf32_n_be_vec;
extern const bfd_target mips_elf32_n_le_vec;
extern const bfd_target mips_elf32_ntrad_be_vec;
extern const bfd_target mips_elf32_ntrad_le_vec;
extern const bfd_target mips_elf32_ntradfbsd_be_vec;
extern const bfd_target mips_elf32_ntradfbsd_le_vec;
extern const bfd_target mips_elf32_trad_be_vec;
extern const bfd_target mips_elf32_trad_le_vec;
extern const bfd_target mips_elf32_tradfbsd_be_vec;
extern const bfd_target mips_elf32_tradfbsd_le_vec;
extern const bfd_target mips_elf32_vxworks_be_vec;
extern const bfd_target mips_elf32_vxworks_le_vec;
extern const bfd_target mips_elf64_be_vec;
extern const bfd_target mips_elf64_le_vec;
extern const bfd_target mips_elf64_trad_be_vec;
extern const bfd_target mips_elf64_trad_le_vec;
extern const bfd_target mips_elf64_tradfbsd_be_vec;
extern const bfd_target mips_elf64_tradfbsd_le_vec;
extern const bfd_target mips_pe_le_vec;
extern const bfd_target mips_pei_le_vec;
extern const bfd_target mipslpe_vec;
extern const bfd_target mipslpei_vec;
extern const bfd_target mmix_elf64_vec;
extern const bfd_target mmix_mmo_vec;
extern const bfd_target mn10200_elf32_vec;
extern const bfd_target mn10300_elf32_vec;
extern const bfd_target moxie_elf32_be_vec;
extern const bfd_target moxie_elf32_le_vec;
extern const bfd_target msp430_elf32_vec;
extern const bfd_target msp430_elf32_ti_vec;
extern const bfd_target mt_elf32_vec;
extern const bfd_target nds32_elf32_be_vec;
extern const bfd_target nds32_elf32_le_vec;
extern const bfd_target nds32_elf32_linux_be_vec;
extern const bfd_target nds32_elf32_linux_le_vec;
extern const bfd_target newsos3_vec;
extern const bfd_target nios2_elf32_be_vec;
extern const bfd_target nios2_elf32_le_vec;
extern const bfd_target nlm32_alpha_vec;
extern const bfd_target nlm32_i386_vec;
extern const bfd_target nlm32_powerpc_vec;
extern const bfd_target nlm32_sparc_vec;
extern const bfd_target ns32k_aout_pc532mach_vec;
extern const bfd_target ns32k_aout_pc532nbsd_vec;
extern const bfd_target oasys_vec;
extern const bfd_target or1k_elf32_vec;
extern const bfd_target or32coff_big_vec;
extern const bfd_target pc532machaout_vec;
extern const bfd_target pc532netbsd_vec;
extern const bfd_target pdp11_aout_vec;
extern const bfd_target pef_vec;
extern const bfd_target pef_xlib_vec;
extern const bfd_target pj_elf32_vec;
extern const bfd_target pj_elf32_le_vec;
extern const bfd_target plugin_vec;
extern const bfd_target pmac_xcoff_vec;
extern const bfd_target pmac_xcoff64_vec;
extern const bfd_target powerpc_boot_vec;
extern const bfd_target powerpc_elf32_vec;
extern const bfd_target powerpc_elf32_le_vec;
extern const bfd_target powerpc_elf32_fbsd_vec;
extern const bfd_target powerpc_elf32_vxworks_vec;
extern const bfd_target powerpc_elf64_vec;
extern const bfd_target powerpc_elf64_le_vec;
extern const bfd_target powerpc_elf64_fbsd_vec;
extern const bfd_target powerpc_nlm32_vec;
extern const bfd_target powerpc_pe_vec;
extern const bfd_target powerpc_pe_le_vec;
extern const bfd_target powerpc_pei_vec;
extern const bfd_target powerpc_pei_le_vec;
extern const bfd_target powerpc_xcoff_vec;
extern const bfd_target ppcboot_vec;
extern const bfd_target riscix_vec;
extern const bfd_target rl78_elf32_vec;
extern const bfd_target rs6000_xcoff64_vec;
extern const bfd_target rs6000_xcoff64_aix_vec;
extern const bfd_target rs6000_xcoff_vec;
extern const bfd_target rs6000coff64_vec;
extern const bfd_target rs6000coff_vec;
extern const bfd_target rx_elf32_be_vec;
extern const bfd_target rx_elf32_be_ns_vec;
extern const bfd_target rx_elf32_le_vec;
extern const bfd_target s390_elf32_vec;
extern const bfd_target s390_elf64_vec;
extern const bfd_target score_elf32_be_vec;
extern const bfd_target score_elf32_le_vec;
extern const bfd_target sh64_elf32_vec;
extern const bfd_target sh64_elf32_le_vec;
extern const bfd_target sh64_elf32_linux_vec;
extern const bfd_target sh64_elf32_linux_be_vec;
extern const bfd_target sh64_elf32_nbsd_vec;
extern const bfd_target sh64_elf32_nbsd_le_vec;
extern const bfd_target sh64_elf64_vec;
extern const bfd_target sh64_elf64_le_vec;
extern const bfd_target sh64_elf64_linux_vec;
extern const bfd_target sh64_elf64_linux_be_vec;
extern const bfd_target sh64_elf64_nbsd_vec;
extern const bfd_target sh64_elf64_nbsd_le_vec;
extern const bfd_target sh_coff_vec;
extern const bfd_target sh_coff_le_vec;
extern const bfd_target sh_coff_small_vec;
extern const bfd_target sh_coff_small_le_vec;
extern const bfd_target sh_elf32_vec;
extern const bfd_target sh_elf32_le_vec;
extern const bfd_target sh_elf32_fdpic_be_vec;
extern const bfd_target sh_elf32_fdpic_le_vec;
extern const bfd_target sh_elf32_linux_vec;
extern const bfd_target sh_elf32_linux_be_vec;
extern const bfd_target sh_elf32_nbsd_vec;
extern const bfd_target sh_elf32_nbsd_le_vec;
extern const bfd_target sh_elf32_symbian_le_vec;
extern const bfd_target sh_elf32_vxworks_vec;
extern const bfd_target sh_elf32_vxworks_le_vec;
extern const bfd_target sh_pe_le_vec;
extern const bfd_target sh_pei_le_vec;
extern const bfd_target shcoff_small_vec;
extern const bfd_target shcoff_vec;
extern const bfd_target shlcoff_small_vec;
extern const bfd_target shlcoff_vec;
extern const bfd_target shlpe_vec;
extern const bfd_target shlpei_vec;
extern const bfd_target som_vec;
extern const bfd_target sparc_aout_le_vec;
extern const bfd_target sparc_aout_linux_vec;
extern const bfd_target sparc_aout_lynx_vec;
extern const bfd_target sparc_aout_nbsd_vec;
extern const bfd_target sparc_aout_sunos_be_vec;
extern const bfd_target sparc_coff_vec;
extern const bfd_target sparc_coff_lynx_vec;
extern const bfd_target sparc_elf32_vec;
extern const bfd_target sparc_elf32_sol2_vec;
extern const bfd_target sparc_elf32_vxworks_vec;
extern const bfd_target sparc_elf64_vec;
extern const bfd_target sparc_elf64_fbsd_vec;
extern const bfd_target sparc_elf64_sol2_vec;
extern const bfd_target sparc_nlm32_vec;
extern const bfd_target sparccoff_vec;
extern const bfd_target sparcle_aout_vec;
extern const bfd_target sparclinux_vec;
extern const bfd_target sparclynx_aout_vec;
extern const bfd_target sparclynx_coff_vec;
extern const bfd_target sparcnetbsd_vec;
extern const bfd_target spu_elf32_vec;
extern const bfd_target sunos_big_vec;
extern const bfd_target sym_vec;
extern const bfd_target tic30_aout_vec;
extern const bfd_target tic30_coff_vec;
extern const bfd_target tic4x_coff0_beh_vec;
extern const bfd_target tic4x_coff0_vec;
extern const bfd_target tic4x_coff1_beh_vec;
extern const bfd_target tic4x_coff1_vec;
extern const bfd_target tic4x_coff2_beh_vec;
extern const bfd_target tic4x_coff2_vec;
extern const bfd_target tic54x_coff0_beh_vec;
extern const bfd_target tic54x_coff0_vec;
extern const bfd_target tic54x_coff1_beh_vec;
extern const bfd_target tic54x_coff1_vec;
extern const bfd_target tic54x_coff2_beh_vec;
extern const bfd_target tic54x_coff2_vec;
extern const bfd_target tic6x_elf32_be_vec;
extern const bfd_target tic6x_elf32_le_vec;
extern const bfd_target tic6x_elf32_c6000_be_vec;
extern const bfd_target tic6x_elf32_c6000_le_vec;
extern const bfd_target tic6x_elf32_linux_be_vec;
extern const bfd_target tic6x_elf32_linux_le_vec;
extern const bfd_target tic80_coff_vec;
extern const bfd_target tic80coff_vec;
extern const bfd_target tilegx_elf32_be_vec;
extern const bfd_target tilegx_elf32_le_vec;
extern const bfd_target tilegx_elf64_be_vec;
extern const bfd_target tilegx_elf64_le_vec;
extern const bfd_target tilepro_elf32_vec;
extern const bfd_target v800_elf32_vec;
extern const bfd_target v850_elf32_vec;
extern const bfd_target vax_aout_1knbsd_vec;
extern const bfd_target vax_aout_bsd_vec;
extern const bfd_target vax_aout_nbsd_vec;
extern const bfd_target vax_elf32_vec;
extern const bfd_target vaxbsd_vec;
extern const bfd_target vaxnetbsd_vec;
extern const bfd_target vax1knetbsd_vec;
extern const bfd_target versados_vec;
extern const bfd_target vms_alpha_vec;
extern const bfd_target vms_vax_vec;
extern const bfd_target w65_coff_vec;
extern const bfd_target w65_vec;
extern const bfd_target we32k_coff_vec;
extern const bfd_target we32kcoff_vec;
extern const bfd_target x86_64_coff_vec;
extern const bfd_target x86_64_elf32_vec;
extern const bfd_target x86_64_elf32_nacl_vec;
extern const bfd_target x86_64_elf64_vec;
extern const bfd_target x86_64_elf64_fbsd_vec;
extern const bfd_target x86_64_elf64_nacl_vec;
extern const bfd_target x86_64_elf64_sol2_vec;
extern const bfd_target x86_64_mach_o_vec;
extern const bfd_target x86_64_pe_vec;
extern const bfd_target x86_64_pe_be_vec;
extern const bfd_target x86_64_pei_vec;
extern const bfd_target xc16x_elf32_vec;
extern const bfd_target xgate_elf32_vec;
extern const bfd_target xstormy16_elf32_vec;
extern const bfd_target xtensa_elf32_be_vec;
extern const bfd_target xtensa_elf32_le_vec;
extern const bfd_target z80_coff_vec;
extern const bfd_target z8k_coff_vec;
extern const bfd_target z8kcoff_vec;

/* These are always included.  */
extern const bfd_target srec_vec;
extern const bfd_target symbolsrec_vec;
extern const bfd_target verilog_vec;
extern const bfd_target tekhex_vec;
extern const bfd_target binary_vec;
extern const bfd_target ihex_vec;

/* All of the xvecs for core files: */
extern const bfd_target aix386_core_vec;
extern const bfd_target cisco_core_big_vec;
extern const bfd_target cisco_core_little_vec;
extern const bfd_target hppabsd_core_vec;
extern const bfd_target hpux_core_vec;
extern const bfd_target irix_core_vec;
extern const bfd_target netbsd_core_vec;
extern const bfd_target osf_core_vec;
extern const bfd_target ptrace_core_vec;
extern const bfd_target sco5_core_vec;
extern const bfd_target trad_core_vec;

/* All of the new names for the xvecs for core files: */
extern const bfd_target core_aix386_vec;
extern const bfd_target core_cisco_be_vec;
extern const bfd_target core_cisco_le_vec;
extern const bfd_target core_hppabsd_vec;
extern const bfd_target core_hpux_vec;
extern const bfd_target core_irix_vec;
extern const bfd_target core_netbsd_vec;
extern const bfd_target core_osf_vec;
extern const bfd_target core_ptrace_vec;
extern const bfd_target core_sco5_vec;
extern const bfd_target core_trad_vec;

extern const bfd_target bfd_elf32_am33lin_vec;

static const bfd_target * const _bfd_target_vector[] = {
#ifdef SELECT_VECS
	SELECT_VECS,
#else /* not SELECT_VECS */
# ifdef DEFAULT_VECTOR
	&DEFAULT_VECTOR,
# endif /* DEFAULT_VECTOR */
	/* This list is alphabetized to make it easy to compare
	   with other vector lists -- the decls above and
	   the case statement in configure.ac.
    	   Try to keep it in order when adding new targets, and
	   use a name of the form <cpu>_<format>_<other>_<endian>_vec.
	   Note that sorting is done as if _<endian>_vec were left out.
	   Vectors that do NOT compile on all systems, or are unfinished,
	   should have an entry here with #if 0 around it, to show that
	   it was NOT omitted by mistake.  */
	&a29kcoff_big_vec,
# ifdef BFD64
	&aarch64_elf32_be_vec,
	&aarch64_elf32_le_vec,
	&aarch64_elf64_be_vec,
	&aarch64_elf64_le_vec,
	&aarch64_mach_o_vec,
# endif /* BFD64 */
	&a_out_adobe_vec,
# ifdef BFD64
	&aix5coff64_vec,
# endif /* BFD64 */
	&aout0_big_vec,
# if 0
	/* We have no way of distinguishing these from other a.out variants: */
	&aout_arm_big_vec,
	&aout_arm_little_vec,
	/* No one seems to use this: */
	&aout_mips_big_vec,
# endif /* 0 */
	&aout_mips_little_vec,
# if 0
	&apollocoff_vec,
# endif /* 0 */
	&arm_epoc_pe_big_vec,
	&arm_epoc_pe_little_vec,
	&arm_epoc_pei_big_vec,
	&arm_epoc_pei_little_vec,
# ifdef __arm__
        &arm_mach_o_vec,
# endif /* __arm__ */
# if 0
	&arm_pe_wince_be_vec,
	&arm_pe_wince_le_vec,
	&arm_pei_wince_be_vec,
	&arm_pei_wince_le_vec,
# endif /* 0 */
	&armcoff_big_vec,
	&armcoff_little_vec,
	&armnetbsd_vec,
	&armpe_big_vec,
	&armpe_little_vec,
	&armpei_big_vec,
	&armpei_little_vec,
	&b_out_vec_big_host,
	&b_out_vec_little_host,
	&bfd_efi_app_ia32_vec,
# ifdef BFD64
	&bfd_efi_app_ia64_vec,
# endif /* BFD64 */
	&bfd_elf32_avr_vec,
	&bfd_elf32_bfin_vec,

	/* This, and other vectors, may not be used in any *.mt configuration.
	   But that does not mean they are unnecessary.  If configured with
	   --enable-targets=all, objdump or gdb should be able to examine
	   the file even if we don't recognize the machine type.  */
	&bfd_elf32_big_generic_vec,
	&bfd_elf32_bigarc_vec,
	&bfd_elf32_bigarm_vec,
	&bfd_elf32_bigarm_symbian_vec,
	&bfd_elf32_bigarm_vxworks_vec,
	&bfd_elf32_bigmips_vec,
	&bfd_elf32_cr16c_vec,
	&bfd_elf32_cris_vec,
	&bfd_elf32_crx_vec,
	&bfd_elf32_d10v_vec,
	&bfd_elf32_d30v_vec,
	&bfd_elf32_dlx_big_vec,
	&bfd_elf32_fr30_vec,
	&bfd_elf32_frv_vec,
	&bfd_elf32_frvfdpic_vec,
	&bfd_elf32_h8300_vec,
	&bfd_elf32_hppa_linux_vec,
	&bfd_elf32_hppa_nbsd_vec,
	&bfd_elf32_hppa_vec,
	&bfd_elf32_i370_vec,
	&bfd_elf32_i386_freebsd_vec,
	&bfd_elf32_i386_vxworks_vec,
	&bfd_elf32_i386_vec,
	&bfd_elf32_i860_little_vec,
	&bfd_elf32_i860_vec,
	&bfd_elf32_i960_vec,
# if 0
	&bfd_elf32_ia64_big_vec,
# endif /* 0 */
# ifdef BFD64
	&bfd_elf32_ia64_hpux_big_vec,
# endif /* BFD64 */
	&bfd_elf32_ip2k_vec,
	&bfd_elf32_iq2000_vec,
	&bfd_elf32_little_generic_vec,
	&bfd_elf32_littlearc_vec,
	&bfd_elf32_littlearm_vec,
	&bfd_elf32_littlearm_symbian_vec,
	&bfd_elf32_littlearm_vxworks_vec,
	&bfd_elf32_littlemips_vec,
	&bfd_elf32_m32c_vec,
	&bfd_elf32_m32r_vec,
        &bfd_elf32_m32rle_vec,
        &bfd_elf32_m32rlin_vec,
        &bfd_elf32_m32rlelin_vec,
	&bfd_elf32_m68hc11_vec,
	&bfd_elf32_m68hc12_vec,
	&bfd_elf32_m68k_vec,
	&bfd_elf32_m88k_vec,
	&bfd_elf32_mcore_big_vec,
	&bfd_elf32_mcore_little_vec,
	&bfd_elf32_mn10200_vec,
	&bfd_elf32_mn10300_vec,
	&bfd_elf32_ms1_vec,
	&bfd_elf32_msp430_vec,
# ifdef BFD64
	&bfd_elf32_nbigmips_vec,
	&bfd_elf32_nlittlemips_vec,
	&bfd_elf32_ntradbigmips_vec,
	&bfd_elf32_ntradlittlemips_vec,
# endif /* BFD64 */
	&bfd_elf32_openrisc_vec,
	&bfd_elf32_or32_big_vec,
	&bfd_elf32_pj_vec,
	&bfd_elf32_pjl_vec,
	&bfd_elf32_powerpc_vec,
	&bfd_elf32_powerpc_vxworks_vec,
	&bfd_elf32_powerpcle_vec,
	&bfd_elf32_s390_vec,
        &bfd_elf32_sh_vec,
        &bfd_elf32_shblin_vec,
        &bfd_elf32_shl_vec,
        &bfd_elf32_shl_symbian_vec,
        &bfd_elf32_shlin_vec,
	&bfd_elf32_shlnbsd_vec,
	&bfd_elf32_shnbsd_vec,
# ifdef BFD64
	&bfd_elf32_sh64_vec,
	&bfd_elf32_sh64l_vec,
	&bfd_elf32_sh64lnbsd_vec,
	&bfd_elf32_sh64nbsd_vec,
	&bfd_elf32_sh64lin_vec,
	&bfd_elf32_sh64blin_vec,
# endif /* BFD64 */
	&bfd_elf32_sparc_vec,
	&bfd_elf32_tradbigmips_vec,
	&bfd_elf32_tradlittlemips_vec,
	&bfd_elf32_us_cris_vec,
	&bfd_elf32_v850_vec,
	&bfd_elf32_vax_vec,
	&bfd_elf32_xstormy16_vec,
	&bfd_elf32_xtensa_be_vec,
	&bfd_elf32_xtensa_le_vec,
# ifdef BFD64
	&bfd_elf64_alpha_freebsd_vec,
	&bfd_elf64_alpha_vec,
	&bfd_elf64_big_generic_vec,
	&bfd_elf64_bigmips_vec,
	&bfd_elf64_hppa_linux_vec,
	&bfd_elf64_hppa_vec,
	&bfd_elf64_ia64_big_vec,
	&bfd_elf64_ia64_hpux_big_vec,
	&bfd_elf64_ia64_little_vec,
	&bfd_elf64_little_generic_vec,
	&bfd_elf64_littlemips_vec,
	&bfd_elf64_mmix_vec,
	&bfd_elf64_powerpc_vec,
	&bfd_elf64_powerpcle_vec,
	&bfd_elf64_s390_vec,
	&bfd_elf64_sh64_vec,
	&bfd_elf64_sh64l_vec,
	&bfd_elf64_sh64lnbsd_vec,
	&bfd_elf64_sh64nbsd_vec,
	&bfd_elf64_sh64lin_vec,
	&bfd_elf64_sh64blin_vec,
	&bfd_elf64_sparc_vec,
	&bfd_elf64_tradbigmips_vec,
	&bfd_elf64_tradlittlemips_vec,
	&bfd_elf64_x86_64_vec,
	&bfd_mmo_vec,
# endif /* BFD64 */
	&bfd_powerpc_pe_vec,
	&bfd_powerpc_pei_vec,
	&bfd_powerpcle_pe_vec,
	&bfd_powerpcle_pei_vec,
# ifdef HAVE_powerpc_xcoff_vec
	/* This has (or at least should have) the same magic number as RS/6000: */
	&powerpc_xcoff_vec;
# endif /* HAVE_powerpc_xcoff_vec */
	&cris_aout_vec,
# ifdef BFD64
	&demo_64_vec,	/* Only compiled if host has long-long support.  */
# endif /* BFD64 */
	&ecoff_big_vec,
	&ecoff_biglittle_vec,
	&ecoff_little_vec,
# ifdef BFD64
	&ecoffalpha_little_vec,
# endif /* BFD64 */
	&go32coff_vec,
	&go32stubbedcoff_vec,
	&h8300coff_vec,
	&h8500coff_vec,
# if 0
	/* Since a.out files lack decent magic numbers, no way to recognize
	   which kind of a.out file it is.  */
	&host_aout_vec,
	/* Clashes with sunos_big_vec magic no.  */
	&hp300bsd_vec,
# endif /* 0 */
	&hp300hpux_vec,
	&i386aout_vec,
	&i386bsd_vec,
	&i386coff_vec,
# if 0
	&i386dynix_vec,
# endif /* 0 */
	&i386freebsd_vec,
# if 0
	/* Since a.out files lack decent magic numbers, no way to recognize
	   which kind of a.out file it is.  */
	&i386linux_vec,
# endif /* 0 */
	&i386lynx_aout_vec,
	&i386lynx_coff_vec,
# if 0
	/* No distinguishing features for Mach 3 executables: */
	&i386mach3_vec,
# endif /* 0 */
	&i386msdos_vec,
	&i386netbsd_vec,
	&i386os9k_vec,
	&i386pe_vec,
	&i386pei_vec,
	&i860coff_vec,
	&icoff_big_vec,
	&icoff_little_vec,
	&ieee_vec,
# if 0
	&m68k4knetbsd_vec,
	&m68kaux_coff_vec,
# endif /* 0 */
	&m68kcoff_vec,
	&m68kcoffun_vec,
# if 0
	/* Since a.out files lack decent magic numbers, no way to recognize
	   which kind of a.out file it is.  */
	&m68klinux_vec,
# endif /* 0 */
	&m68knetbsd_vec,
	&m68ksysvcoff_vec,
	&m88kbcs_vec,
	&m88kmach3_vec,
	&m88kopenbsd_vec,
	&mach_o_be_vec,
	&mach_o_le_vec,
	&mach_o_fat_vec,
	&maxqcoff_vec,
	&mcore_pe_big_vec,
	&mcore_pe_little_vec,
	&mcore_pei_big_vec,
	&mcore_pei_little_vec,
	&mipslpe_vec,
	&mipslpei_vec,
	&newsos3_vec,
# ifdef BFD64
	&nlm32_alpha_vec,
# endif /* BFD64 */
	&nlm32_i386_vec,
	&nlm32_powerpc_vec,
	&nlm32_sparc_vec,
# if 0
	/* We have no oasys tools anymore, so we cannot test any of this
	 * anymore. If you want to test the stuff yourself, go ahead...
	 * <steve@cygnus.com>
	 * Worse, since there is no magic number for archives, there
	 * can be annoying target mis-matches.  */
	&oasys_vec,
# endif /* 0 */
	/* Entry for the OpenRISC family: */
	&or32coff_big_vec,

	&pc532machaout_vec,
	&pc532netbsd_vec,
	&pdp11_aout_vec,
	&pef_vec,
	&pef_xlib_vec,
# if 0
	/* This has the same magic number as RS/6000: */
	&pmac_xcoff_vec,
# endif /* 0 */
	&ppcboot_vec,
# if 0
	/* We have no way of distinguishing these from other a.out variants: */
	&riscix_vec,
# endif /* 0 */
# ifdef BFD64
	&rs6000coff64_vec,
# endif /* BFD64 */
	&rs6000coff_vec,
	&shcoff_small_vec,
	&shcoff_vec,
	&shlcoff_small_vec,
	&shlcoff_vec,
	&shlpe_vec,
	&shlpei_vec,
# if defined(HOST_HPPAHPUX) || defined(HOST_HPPABSD) || defined(HOST_HPPAOSF)
	&som_vec,
# endif /* HOST_HPPAHPUX || HOST_HPPABSD || HOST_HPPAOSF */
	&sparccoff_vec,
	&sparcle_aout_vec,
	&sparclinux_vec,
	&sparclynx_aout_vec,
	&sparclynx_coff_vec,
	&sparcnetbsd_vec,
	&sunos_big_vec,
	&sym_vec,
	&tic30_aout_vec,
	&tic30_coff_vec,
	&tic54x_coff0_beh_vec,
	&tic54x_coff0_vec,
	&tic54x_coff1_beh_vec,
	&tic54x_coff1_vec,
	&tic54x_coff2_beh_vec,
	&tic54x_coff2_vec,
	&tic80coff_vec,
	&vaxbsd_vec,
	&vaxnetbsd_vec,
	&vax1knetbsd_vec,
	&versados_vec,
# ifdef BFD64
	&vms_alpha_vec,
# endif /* BFD64 */
	&vms_vax_vec,
	&w65_vec,
	&we32kcoff_vec,
	&z8kcoff_vec,
	&bfd_elf32_am33lin_vec,
# ifdef BFD64
	&loongarch_elf32_vec,
	&loongarch_elf64_vec,
	&loongarch64_pei_vec,
# endif /* BFD64 */
#endif /* not SELECT_VECS */

/* Always support S-records, for convenience: */
	&srec_vec,
	&symbolsrec_vec,
#if 0
/* And verilog: */
        &verilog_vec,
#endif /* 0 */
/* And tekhex: */
	&tekhex_vec,
/* Likewise for binary output: */
	&binary_vec,
/* Likewise for ihex: */
	&ihex_vec,

/* Add any required traditional-core-file-handler: */
#ifdef AIX386_CORE
	&aix386_core_vec,
#endif /* AIX386_CORE */
#if 0
	/* We do NOT include cisco_core_*_vec.  Although it has a magic number,
	   the magic number is NOT at the beginning of the file, and thus
	   might spuriously match other kinds of files.  */
	&cisco_core_big_vec,
	&cisco_core_little_vec,
#endif /* 0 */
#ifdef HPPABSD_CORE
	&hppabsd_core_vec,
#endif /* HPPABSD_CORE */
#ifdef HPUX_CORE
	&hpux_core_vec,
#endif /* HPUX_CORE */
#ifdef IRIX_CORE
	&irix_core_vec,
#endif /* IRIX_CORE */
#ifdef NETBSD_CORE
	&netbsd_core_vec,
#endif /* NETBSD_CORE */
#ifdef OSF_CORE
	&osf_core_vec,
#endif /* OSF_CORE */
#ifdef PTRACE_CORE
	&ptrace_core_vec,
#endif /* PTRACE_CORE */
#ifdef SCO5_CORE
	&sco5_core_vec,
#endif /* SCO5_CORE */
#ifdef TRAD_CORE
	&trad_core_vec,
#endif /* TRAD_CORE */

	NULL /* end of list marker */
};
const bfd_target * const *bfd_target_vector = _bfd_target_vector;

/* bfd_default_vector[0] contains either the address of the default vector,
   if there is one, or zero if there is NOT.  */
const bfd_target *bfd_default_vector[] = {
#ifdef DEFAULT_VECTOR
	&DEFAULT_VECTOR,
#endif /* DEFAULT_VECTOR */
	NULL
};

/* bfd_associated_vector[] contains the associated target vectors used
   to reduce the ambiguity in bfd_check_format_matches.  */
static const bfd_target *_bfd_associated_vector[] = {
#ifdef ASSOCIATED_VECS
	ASSOCIATED_VECS,
#endif /* ASSOCIATED_VECS */
	NULL
};
const bfd_target * const *bfd_associated_vector = _bfd_associated_vector;

/* When there is an ambiguous match, bfd_check_format_matches puts the
 * names of the matching targets in an array.  This variable is the maximum
 * number of entries that the array could possibly need.  */
#ifdef __clang__
extern const size_t _bfd_target_vector_entries;
#endif /* __clang__ */
const size_t _bfd_target_vector_entries = (sizeof(_bfd_target_vector) / sizeof(*_bfd_target_vector));

/* This array maps configuration triplets onto BFD vectors: */
struct targmatch
{
  /* The configuration triplet: */
  const char *triplet;
  /* The BFD vector.  If this is NULL, then the vector is found by
     searching forward for the next structure with a non NULL vector
     field.  */
  const bfd_target *vector;
};

/* targmatch.h is built by Makefile out of config.bfd.  */
static const struct targmatch bfd_target_match[] = {
#include "targmatch.h"
  { NULL, NULL }
};

/* Find a target vector, given a name or configuration triplet: */
static const bfd_target *
find_target (const char *name)
{
  const bfd_target * const *target;
  const struct targmatch *match;

  for (target = &bfd_target_vector[0]; *target != NULL; target++)
    if (strcmp(name, (*target)->name) == 0)
      return *target;

  /* If we could NOT match on the exact name, then try matching on the
   * configuration triplet.  FIXME: We should run the triplet through
   * config.sub first, but that is hard.  */
  for (match = &bfd_target_match[0]; match->triplet != NULL; match++)
    {
      if (fnmatch(match->triplet, name, 0) == 0)
	{
	  while (match->vector == NULL)
	    ++match;
	  return match->vector;
#if !defined(__clang__)
	  break;
#endif /* !__clang__ */
	}
    }

  bfd_set_error(bfd_error_invalid_target);
  return NULL;
}

/*
FUNCTION
	bfd_set_default_target

SYNOPSIS
	bfd_boolean bfd_set_default_target (const char *name);

DESCRIPTION
	Set the default target vector to use when recognizing a BFD.
	This takes the name of the target, which may be a BFD target
	name or a configuration triplet.
*/

bfd_boolean
bfd_set_default_target (const char *name)
{
  const bfd_target *target;

  if (bfd_default_vector[0] != NULL
      && strcmp (name, bfd_default_vector[0]->name) == 0)
    return TRUE;

  target = find_target (name);
  if (target == NULL)
    return FALSE;

  bfd_default_vector[0] = target;
  return TRUE;
}

/*
FUNCTION
	bfd_find_target

SYNOPSIS
	const bfd_target *bfd_find_target (const char *target_name, bfd *abfd);

DESCRIPTION
	Return a pointer to the transfer vector for the object target
	named @var{target_name}.  If @var{target_name} is <<NULL>>, choose the
	one in the environment variable <<GNUTARGET>>; if that is null or not
	defined, then choose the first entry in the target list.
	Passing in the string "default" or setting the environment
	variable to "default" will cause the first entry in the target
	list to be returned, and "target_defaulted" will be set in the
	BFD.  This causes <<bfd_check_format>> to loop over all the
	targets to find the one that matches the file being read.
*/

const bfd_target *
bfd_find_target (const char *target_name, bfd *abfd)
{
  const char *targname;
  const bfd_target *target;

  if (target_name != NULL)
    targname = target_name;
  else
    targname = getenv ("GNUTARGET");

  /* This is safe; the vector cannot be null.  */
  if (targname == NULL || strcmp (targname, "default") == 0)
    {
      abfd->target_defaulted = TRUE;
      if (bfd_default_vector[0] != NULL)
	abfd->xvec = bfd_default_vector[0];
      else
	abfd->xvec = bfd_target_vector[0];
      return abfd->xvec;
    }

  abfd->target_defaulted = FALSE;

  target = find_target (targname);
  if (target == NULL)
    return NULL;

  abfd->xvec = target;
  return target;
}

/* Helper function for bfd_get_target_info to determine the target's
   architecture.  This method handles bfd internal target names as
   tuples and triplets.  */
static bfd_boolean
_bfd_find_arch_match(const char *tname, const char **arch,
		     const char **def_target_arch)
{
  if (!arch)
    return FALSE;

  while (*arch != NULL)
    {
      const char *in_a = strstr(*arch, tname);
      char end_ch = (in_a ? in_a[strlen(tname)] : '\0');

      if (in_a && (in_a == *arch || in_a[-1] == ':')
          && end_ch == 0)
        {
          *def_target_arch = *arch;
          return TRUE;
        }
      arch++;
    }
  return FALSE;
}

/*
FUNCTION
	bfd_get_target_info

SYNOPSIS
	const bfd_target *bfd_get_target_info(const char *target_name,
                                              bfd *abfd,
                                              bfd_boolean *is_bigendian,
                                              int *underscoring,
                                              const char **def_target_arch);
DESCRIPTION
	Return a pointer to the transfer vector for the object target
	named @var{target_name}.  If @var{target_name} is <<NULL>>,
	choose the one in the environment variable <<GNUTARGET>>; if
	that is null or not defined, then choose the first entry in the
	target list.  Passing in the string "default" or setting the
	environment variable to "default" will cause the first entry in
	the target list to be returned, and "target_defaulted" will be
	set in the BFD if @var{abfd} is not <<NULL>>.  This causes
	<<bfd_check_format>> to loop over all the targets to find the
	one that matches the file being read.
	If @var{is_bigendian} is not <<NULL>>, then set this value to target's
	endian mode. True for big-endian, FALSE for little-endian or for
	invalid target.
	If @var{underscoring} is not <<NULL>>, then set this value to target's
	underscoring mode. Zero for none-underscoring, -1 for invalid target,
	else the value of target vector's symbol underscoring.
	If @var{def_target_arch} is not <<NULL>>, then set it to the architecture
	string specified by the target_name.
*/

const bfd_target *
bfd_get_target_info(const char *target_name, bfd *abfd,
                    bfd_boolean *is_bigendian,
                    int *underscoring, const char **def_target_arch)
{
  const bfd_target *target_vec;

  if (is_bigendian)
    *is_bigendian = FALSE;
  if (underscoring)
    *underscoring = -1;
  if (def_target_arch)
    *def_target_arch = NULL;
  target_vec = bfd_find_target(target_name, abfd);
  if (! target_vec)
    return NULL;
  if (is_bigendian)
    *is_bigendian = ((target_vec->byteorder == BFD_ENDIAN_BIG) ? TRUE
                     : FALSE);
  if (underscoring)
    *underscoring = (((int)target_vec->symbol_leading_char) & 0xff);

  if (def_target_arch)
    {
      const char *tname = target_vec->name;
      const char **arches = bfd_arch_list();

      if (arches && tname)
        {
          char *hyp = strchr(tname, '-');

          if (hyp != NULL)
            {
              tname = ++hyp;

              /* Make sure we detect architecture names
                 for triplets like "pe-arm-wince-little".  */
              if (!_bfd_find_arch_match(tname, arches, def_target_arch))
                {
                  char new_tname[50];

                  strncpy(new_tname, hyp, (sizeof(new_tname) - 1UL));
                  while ((hyp = strrchr(new_tname, '-')) != NULL)
                    {
                      *hyp = 0;
                      if (_bfd_find_arch_match(new_tname, arches,
                                               def_target_arch))
                        break;
                    }
                }
            }
          else
            _bfd_find_arch_match(tname, arches, def_target_arch);
        }

      if (arches)
        free(arches);
    }
  return target_vec;
}

/*
FUNCTION
	bfd_target_list

SYNOPSIS
	const char ** bfd_target_list (void);

DESCRIPTION
	Return a freshly malloced NULL-terminated
	vector of the names of all the valid BFD targets. Do not
	modify the names.

*/

const char **
bfd_target_list(void)
{
  int vec_length = 0;
  bfd_size_type amt;
#if defined(HOST_HPPAHPUX) && !defined(__STDC__)
  /* The native compiler on the HP9000/700 has a bug which causes it
   * to loop endlessly when compiling this file.  This avoids it.  */
  volatile
#endif /* HOST_HPPAHPUX && !__STDC__ */
  const bfd_target * const *target;
  const  char **name_list, **name_ptr;

  for (target = &bfd_target_vector[0]; *target != NULL; target++)
    vec_length++;

  amt = (bfd_size_type)((size_t)(vec_length + 1) * sizeof(char **));
  name_ptr = name_list = (const char **)bfd_malloc(amt);

  if (name_list == NULL)
    return NULL;

  for (target = &bfd_target_vector[0]; *target != NULL; target++)
    if (target == &bfd_target_vector[0]
	|| *target != bfd_target_vector[0])
      *name_ptr++ = (*target)->name;

  *name_ptr = NULL;
  return name_list;
}

/*
FUNCTION
	bfd_seach_for_target

SYNOPSIS
	const bfd_target *bfd_search_for_target
	  (int (*search_func) (const bfd_target *, void *),
	   void *);

DESCRIPTION
	Return a pointer to the first transfer vector in the list of
	transfer vectors maintained by BFD that produces a non-zero
	result when passed to the function @var{search_func}.  The
	parameter @var{data} is passed, unexamined, to the search
	function.
*/

const bfd_target *
bfd_search_for_target(int (*search_func)(const bfd_target *, void *),
                      void *data)
{
  const bfd_target * const *target;

  for (target = bfd_target_vector; *target != NULL; target ++) {
    if (search_func(*target, data)) {
      return *target;
    }
  }

  return NULL;
}

/* End of targets.c */
