/* coffgrok.h -*- C -*-
 * Copyright 2001, 2002, 2003 Free Software Foundation, Inc.  */
/*
This file is part of GNU Binutils.

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

#ifndef BINUTILS_COFFGROK_H
#define BINUTILS_COFFGROK_H 1

#ifndef PARAMS
# include <ansidecl.h>
#endif /* !PARAMS */

#define T_NULL 0
#define T_VOID 1     /* function argument (only used by compiler) */
#define T_CHAR 2     /* character		*/
#define T_SHORT 3    /* short integer    	*/
#define T_INT 4      /* integer   		*/
#define T_LONG 5     /* long integer		*/
#define T_FLOAT 6    /* floating point   	*/
#define T_DOUBLE 7   /* double word		*/
#define T_STRUCT 8   /* structure 		*/
#define T_UNION 9    /* union    		*/
#define T_ENUM 10    /* enumeration 		*/
#define T_MOE 11     /* member of enumeration   */
#define T_UCHAR	12   /* unsigned character	*/
#define T_USHORT 13  /* unsigned short   	*/
#define T_UINT 14    /* unsigned integer 	*/
#define T_ULONG	15   /* unsigned long    	*/
#define T_LNGDBL 16  /* long double		*/


struct coff_reloc
{
#if defined(HAVE_OFF_T) || defined(off_t) || defined(__STDC__)
  off_t offset;
#else
  int offset;
#endif /* HAVE_OFF_T || off_t || __STDC__ */
  struct coff_symbol *symbol;
#ifdef __BFD_H_SEEN__
  /* keep this the same type as the 'addend' field in 'struct arelent'
   * in "../bfd/bfd.h": */
  bfd_vma addend;
#else
  int addend;
#endif /* __BFD_H_SEEN__ */
};

struct coff_section
{
  const char *name;
  int code;
  int data;
  int address;
  int number;  /* 0..n, .text = 0 */
  int nrelocs;
#ifdef __BFD_H_SEEN__
  bfd_size_type size;
#else
# if defined(HAVE_SIZE_T) || defined(size_t)
  size_t size;
# else
  int size;
# endif /* HAVE_SIZE_T || size_t */
#endif /* __BFD_H_SEEN__ */
  struct coff_reloc *relocs;
  struct bfd_section *bfd_section;
};

struct coff_ofile
{
  int nsources;
  struct coff_sfile *source_head;
  struct coff_sfile *source_tail;
  int nsections;
  struct coff_section *sections;
  struct coff_symbol *symbol_list_head;
  struct coff_symbol *symbol_list_tail;
};

struct coff_isection
{
#if defined(HAVE_OFF_T) || defined(off_t) || defined(__STDC__)
  off_t low;
  off_t high;
#else
  int low;
  int high;
#endif /* HAVE_OFF_T || off_t || __STDC__ */
  int init;
  struct coff_section *parent;
};

struct coff_sfile
{
  char *name;
  struct coff_scope *scope;
  struct coff_sfile *next;

  /* Vector which maps where in each output section the input file has
   * its data: */
  struct coff_isection *section;
};

enum coff_types_e
{
  coff_pointer_type, coff_function_type, coff_array_type,
  coff_structdef_type, coff_basic_type, coff_structref_type,
  coff_enumref_type, coff_enumdef_type, coff_secdef_type
};

struct coff_type
{
  int size;
  enum coff_types_e type;
  union
    {
      struct
	{
          int address;
          int size;
        } asecdef;

      struct
	{
	  int isstruct;
	  struct coff_scope *elements;
	  int idx;
	} astructdef;

      struct
	{
	  struct coff_symbol *ref;
	} astructref;

      struct
	{
	  struct coff_scope *elements;
	  int idx;
	} aenumdef;

      struct
	{
	  struct coff_symbol *ref;
	} aenumref;

      struct
	{
	  struct coff_type *points_to;
	} pointer;

      struct
	{
	  int dim;
	  struct coff_type *array_of;
	} array;

      struct
	{
	  struct coff_type *function_returns;
	  struct coff_scope *parameters;
	  struct coff_scope *code;
	  struct coff_line *lines;
	} function;
      int basic;		/* One of T_VOID.. T_UINT */
    }  u;
};


struct coff_line
{
  int nlines;
  int *lines;
#ifdef __BFD_H_SEEN__
  bfd_vma *addresses;
#else
  int *addresses;
#endif /* __BFD_H_SEEN__ */
};

struct coff_scope
  {
    struct coff_section *sec; /* What section */
#if defined(HAVE_OFF_T) || defined(off_t) || defined(__STDC__)
    off_t offset; /* where (used to be an int) */
#else
    int offset; /* where */
#endif /* HAVE_OFF_T || off_t || __STDC__ */
#if defined(HAVE_SIZE_T) || defined(size_t)
    size_t size; /* How big (used to be an int) */
#else
    int size; /* How big */
#endif /* HAVE_SIZE_T || size_t */
    struct coff_scope *parent;	/* one up */

    struct coff_scope *next;	/*next along */

    int nvars;

    struct coff_symbol *vars_head;	/* symbols */
    struct coff_symbol *vars_tail;

    struct coff_scope *list_head;	/* children */
    struct coff_scope *list_tail;
  };

enum coff_vis_type
{
  coff_vis_ext_def,
  coff_vis_ext_ref,
  coff_vis_int_def,
  coff_vis_common,
  coff_vis_auto,
  coff_vis_register,
  coff_vis_tag,
  coff_vis_member_of_struct,
  coff_vis_member_of_enum,
  coff_vis_autoparam,
  coff_vis_regparam,
};

struct coff_visible
  {
    enum coff_vis_type type;
  };

enum coff_locations
{
  coff_where_stack, coff_where_memory, coff_where_register,
  coff_where_unknown, coff_where_strtag, coff_where_member_of_struct,
  coff_where_member_of_enum, coff_where_entag, coff_where_typedef
};

struct coff_where
  {
    enum coff_locations where;
#if defined(HAVE_OFF_T) || defined(off_t) || defined(__STDC__)
    off_t offset;
#else
    int offset;
#endif /* HAVE_OFF_T || off_t || __STDC__ */
    int bitoffset;
    int bitsize;
    struct coff_section *section;
  };

struct coff_symbol
  {
    char *name;
    int tag;
    struct coff_type *type;
    struct coff_where *where;
    struct coff_visible *visible;
    struct coff_symbol *next;
    struct coff_symbol *next_in_ofile_list; /* For the ofile list */
    int number;
    int er_number;
    struct coff_sfile *sfile;
 };

struct coff_ofile *coff_grok PARAMS ((bfd *));

#if defined(HAVE_OFF_T) || defined(off_t) || defined(__STDC__)
# if defined(__STDC_VERSION__) && !defined(__STRICT_ANSI__) && \
    defined(SIZEOF_OFF_T) && defined(SIZEOF_LONG_LONG) && \
    (SIZEOF_OFF_T == SIZEOF_LONG_LONG)
#  define OFFSET_FMT "ll"
# else
#  define OFFSET_FMT "l"
# endif /* __STDC_VERSION__ && !__STRICT_ANSI__ && (SIZEOF_OFF_T == SIZEOF_LONG_LONG) */
#else
# define OFFSET_FMT /* (nothing) */
#endif /* HAVE_OFF_T || off_t || __STDC__ */

#endif /* !BINUTILS_COFFGROK_H */

/* EOF */
