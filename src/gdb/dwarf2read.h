/* dwarf2read.h -*- C -*- */

#ifndef DWARF2READ_H
#define DWARF2READ_H 1

#include "bfd.h"
#include "buildsym.h"
#include "elf/dwarf2.h"
#include "hashtab.h"
/* APPLE LOCAL - dwarf repository  */
#include "db-access-functions.h" /* gets <sqlite3.h> for us */

extern asection *dwarf_frame_section;
extern asection *dwarf_eh_frame_section;

/* APPLE LOCAL debug map take a bfd parameter: */
char *dwarf2_read_section(struct objfile *, bfd *, asection *);

/* When expanding a psymtab to a symtab we get the
   addresses of all the symbols in the executable (the "final"
   addresses) and the minimal symbols (linker symbols, etc) from
   the .o file and create a table of these address tuples (plus the
   symbol name) to allow for fixing up all the addresses in the .o
   file's DWARF.  NB: I don't think I actually use the symbol name
   once this array is created, just the address tuples.  But for
   now I'll keep it around to aid in debugging.  */

struct oso_final_addr_tuple {
  /* Linker symbol name aka minsym aka physname */
  char *name;
  /* Start address in the .o file */

  CORE_ADDR oso_low_addr;
  /* End address in the .o file (the same as the start address of the
     next highest oso_final_addr_tuple).  */
  CORE_ADDR oso_high_addr;

  /* Low address in the final, linked image */
  CORE_ADDR final_addr;
  /* Whether this function is present in the final executable or not.  */
  int present_in_final;
};


/* This array is sorted by OSO_ADDR so that we can do quick lookups of
   addresses we find in the .o file DWARF entries.  */

struct oso_to_final_addr_map {
  struct partial_symtab *pst;
  int entries;
  struct oso_final_addr_tuple *tuples;

  /* PowerPC has a "-mlong-branch" option that generates trampoline code for
     long branches. This trampoline code follows the code for a function. The
     address range for the function in the .o DWARF currently spans this
     trampoline code. The address range for the function in the linked
     executable can be smaller than the address range in the .o DWARF when
     our newest linker determines that the long branch trampoline code is not
     needed. In such cases, the trampoline code gets stripped, and the branch
     to the trampoline code gets fixed to branch directly to its destination.
     This optimization can cause the size of the function to be reduced in
     the final executable and in the debug map. There didn't seem to be any
     easy way to propagate the function size (N_FUN end stab with no name)
     found in the debug map up to the debug map address translation functions
     whilst only using only minimal and partial symbols. Minimal symbols are
     made from the normal nlist entries (non-STAB) and these nlist entries
     have no size (even though the min symbols in gdb have a size member that
     we could use). Partial symbols for functions get made from debug map
     N_FUN entries and the ending N_FUN entry that contains the new,
     and possibly smaller, function size get used only to set the max
     partial symtab address. Partial symbols in gdb also don't have a size
     member. The oso_to_final_addr_map.tuples array is sorted by OSO_LOW_ADDR.
     The FINAL_ADDR_INDEX member was added so we can quickly search the
     oso_to_final_addr_map.tuples array by FINAL_ADDR. The FINAL_ADDR_INDEX
     contains zero based indexes into the oso_to_final_addr_map.tuples array
     and gets created in CONVERT_OSO_MAP_TO_FINAL_MAP. When translating a high
     pc address in TRANSLATE_DEBUG_MAP_ADDRESS, we can shorten a function's
     address range by making sure the next FINAL_ADDR is not less than our
     current value for our translated high pc.  */

  /* In short: An array of element index values sorted by final address.  */
  int *final_addr_index;

  /* Reuse the above for the common symbols where the .o file has no
     address (just 0x0) -- so for COMMON_PAIRS we're storing the
     symbol name and the final address.  This array is sorted by
     the symbol name. */

  int common_entries;
  struct oso_final_addr_tuple *common_pairs;
};

int translate_debug_map_address(struct oso_to_final_addr_map *,
                                CORE_ADDR, CORE_ADDR *, int);

extern int dwarf2_inform_debugging_optimized_code;
extern struct pending *inlined_subroutine_symbols;

/* We hold several abbreviation tables in memory at the same time. */
#ifndef ABBREV_HASH_SIZE
# define ABBREV_HASH_SIZE 121
#endif /* !ABBREV_HASH_SIZE */

/* The data in a compilation unit header, after target2host translation, looks
 * like this: */
struct comp_unit_head
{
  unsigned long length;
  short version;
  unsigned int abbrev_offset;
  unsigned char addr_size;
  unsigned char signed_addr_p;

  /* Size of file offsets; either 4 or 8: */
  unsigned int offset_size;

  /* Size of the length field; either 4 or 12: */
  unsigned int initial_length_size;

  /* Offset to the first byte of this compilation unit header in the
   * .debug_info section, for resolving relative reference dies.  */
  unsigned int offset;

  /* Pointer to this compilation unit header in the .debug_info section: */
  char *cu_head_ptr;

  /* Pointer to the first die of this compilation unit.  This will be the first
   * byte following the compilation unit header.  */
  char *first_die_ptr;

  /* Pointer to the next compilation unit header in the program: */
  struct comp_unit_head *next;

  /* Base address of this compilation unit.  */
  /* APPLE LOCAL NB: I have changed this from BASE_ADDRESS to
   * BASE_ADDRESS_UNTRANSLATED to make it clear what this value is.
   * In the case of debug-info-in-.o-files or a kext dSYM, where the
   * DWARF info does not have the final relocated addresses, this base
   * address is the address of the CU in the .o file or kext dSYM file: */
  CORE_ADDR base_address_untranslated;

  /* Non-zero if base_address has been set: */
  int base_known;
};

/* Fixed size for the DIE hash table: */
#ifndef REF_HASH_SIZE
# define REF_HASH_SIZE 1021
#endif /* !REF_HASH_SIZE */

/* Internal state when decoding a particular compilation unit: */
struct dwarf2_cu
{
  /* The objfile containing this compilation unit: */
  struct objfile *objfile;

  /* The header of the compilation unit.
   *
   * FIXME drow/2003-11-10: Some of the things from the comp_unit_head
   * should logically be moved to the dwarf2_cu structure.  */
  struct comp_unit_head header;

  struct function_range *first_fn, *last_fn, *cached_fn;

  /* The language we are debugging: */
  enum language language;
  const struct language_defn *language_defn;

  const char *producer;

  /* APPLE LOCAL: Retain the compilation directory pathname for header file
   * relative pathnames (via gcc parameters like "-I../../../include").  */
  char *comp_dir;

  /* The generic symbol table building routines have separate lists for file
   * scope symbols and all all other scopes (local scopes).  So we need to
   * select the right one to pass to add_symbol_to_list().  We do it by keeping
   * a pointer to the correct list in list_in_scope.
   *
   * FIXME: The original dwarf code just treated the file scope as the 1st local
   * scope, and all other local scopes as nested local scopes, and worked fine.
   * Check to see if we really need to distinguish these in buildsym.c: */
  struct pending **list_in_scope;

  /* Maintain an array of referenced fundamental types for the current
   * compilation unit being read.  For DWARF version 1, we have to construct
   * the fundamental types on the fly, since no information about the
   * fundamental types is supplied.  Each such fundamental type is created by
   * calling a language dependent routine to create the type, and then a
   * pointer to that type is then placed in the array at the index specified
   * by its FT_<TYPENAME> value.  The array has a fixed size set by the
   * FT_NUM_MEMBERS compile time constant, which is the number of predefined
   * fundamental types gdb knows how to construct.  */
  struct type *ftypes[FT_NUM_MEMBERS];	/* Fundamental types */

  /* DWARF abbreviation table associated with this compilation unit: */
  struct abbrev_info **dwarf2_abbrevs;

  /* Storage for the abbrev table: */
  struct obstack abbrev_obstack;

  /* Hash table holding all the loaded partial DIEs: */
  htab_t partial_dies;

  /* Storage for things with the same lifetime as this read-in compilation
   unit, including partial DIEs.  */
  struct obstack comp_unit_obstack;

  /* When multiple dwarf2_cu structures are living in memory, this field
   * chains them all together, so that they can be released efficiently.
   * We will probably also want a generation counter so that most-recently-used
   * compilation units are cached...  */
  struct dwarf2_per_cu_data *read_in_chain;

  /* Backchain to our per_cu entry if the tree has been built: */
  struct dwarf2_per_cu_data *per_cu;

  /* How many compilation units ago was this CU last referenced?  */
  int last_used;

  /* A hash table of die offsets for following references: */
  struct die_info *die_ref_table[REF_HASH_SIZE];

  /* Full DIEs if read in: */
  struct die_info *dies;

  /* A set of pointers to dwarf2_per_cu_data objects for compilation units
   * referenced by this one.  Only set during full symbol processing; partial
   * symbol tables do not have dependencies.  */
  htab_t dependencies;

  /* Mark used when releasing cached dies: */
  unsigned int mark : 1;

  /* This flag will be set if this compilation unit might include
   * inter-compilation-unit references.  */
  unsigned int has_form_ref_addr : 1;

  /* This flag will be set if this compilation unit includes any
   * DW_TAG_namespace DIEs.  If we know that there are explicit DIEs for
   * namespaces, then we do NOT need to try to infer them from mangled names: */
  unsigned int has_namespace_info : 1;

  /* APPLE LOCAL begin Inform users about debugging optimized code  */
  /* This flag will be set if the compilation unit die has the
   * DW_AT_APPLE_optimized attribute.  It means the entire compilation
   * unit was compiled with optimizations turned on.  */
  unsigned int cu_is_optimized : 1;
  /* APPLE LOCAL end Inform users about debugging optimized code  */

  /* APPLE LOCAL begin dwarf repository  */
  sqlite3 *repository;

  char *repository_name;
  /* APPLE LOCAL end dwarf repository  */

  /* APPLE LOCAL debug map */
  struct oso_to_final_addr_map *addr_map;
};

enum db_status { DB_UNKNOWN, DB_OPEN, DB_ABBREVS_LOADED, DB_CLOSED };

struct objfile_list_node {
  struct objfile *ofile;
  struct objfile_list_node *next;
};

struct database_info {
  char *fullname;
  struct abbrev_info *abbrev_table;
  enum db_status current_status;
  struct rb_tree_node *db_types;
  struct objfile_list_node *num_uses;
  struct dwarf2_cu *dummy_cu;
  struct objfile *dummy_objfile;
  sqlite3 *db;
};

extern struct database_info *repositories;
extern int num_open_dbs;

extern sqlite3_stmt *db_stmt1;
extern sqlite3_stmt *db_stmt2;

/* Everything from here on are static things that had to be made extern by the
 * split into dwarf2read-repository.c: */

extern void increment_use_count(struct database_info *, struct objfile *);
extern void decrement_use_count(struct database_info *, struct objfile *);

/* APPLE LOCAL begin dwarf repository: */
extern int open_dwarf_repository(char *, char *, struct objfile *,
				 struct dwarf2_cu *);
extern void dwarf2_read_repository_abbrevs(struct dwarf2_cu *);
extern struct dwarf2_cu *build_dummy_cu(struct objfile *, struct dwarf2_cu *);
extern struct objfile *build_dummy_objfile(struct objfile *);
extern void read_in_db_abbrev_table(struct abbrev_info **, sqlite3 *);
extern void db_error(const char *, const char *, sqlite3 *);
extern struct die_info *db_lookup_type(int, sqlite3 *, struct abbrev_info *);
extern void fill_in_die_info(struct die_info *, int, uint8_t *, uint8_t *,
			     struct abbrev_info *, sqlite3 *);
extern uint32_t get_uleb128(uint8_t **);
extern void set_repository_cu_language(unsigned int, struct dwarf2_cu *);
extern int finalize_stmts(sqlite3 *);
extern struct database_info *find_open_repository(sqlite3 *);

extern int byte_swap_p;
/* APPLE LOCAL end dwarf repository */

/* This data structure holds the information of an abbrev. */
struct abbrev_info
{
  unsigned int number;	/* number identifying abbrev */
  enum dwarf_tag tag;		/* dwarf tag */
  unsigned short has_children;		/* boolean */
  unsigned short num_attrs;	/* number of attributes */
  struct attr_abbrev *attrs;	/* an array of attribute descriptions */
  struct abbrev_info *next;	/* next in chain */
};

struct attr_abbrev
{
  enum dwarf_attribute name;
  enum dwarf_form form;
};

/* This data structure holds a complete die structure: */
struct die_info
{
  enum dwarf_tag tag;		/* Tag indicating type of die */
  unsigned int abbrev;	/* Abbrev number */
  unsigned int offset;	/* Offset in .debug_info section */
  /* APPLE LOCAL - dwarf repository  */
  unsigned int repository_id; /* Id number in debug repository */
  unsigned int num_attrs;	/* Number of attributes */
  struct attribute *attrs;	/* An array of attributes */
  struct die_info *next_ref;	/* Next die in ref hash table */

  /* The dies in a compilation unit form an n-ary tree.  PARENT
   points to this die's parent; CHILD points to the first child of
   this node; and all the children of a given node are chained
   together via their SIBLING fields, terminated by a die whose
   tag is zero.  */
  struct die_info *child;	/* Its first child, if any.  */
  struct die_info *sibling;	/* Its next sibling, if any.  */
  struct die_info *parent;	/* Its parent, if any.  */

  struct type *type;		/* Cached type information */
};

/* Blocks are a bunch of untyped bytes: */
struct dwarf_block
{
  unsigned int size;
  char *data;
};

/* Attributes have a name and a value: */
struct attribute
{
  enum dwarf_attribute name;
  enum dwarf_form form;
  union
  {
    char *str;
    struct dwarf_block *blk;
    unsigned long unsnd;
    long int snd;
    CORE_ADDR addr;
  }
  u;
};

/* Get at parts of an attribute structure with these macros: */
#define DW_STRING(attr)    ((attr)->u.str)
#define DW_UNSND(attr)     ((attr)->u.unsnd)
#define DW_BLOCK(attr)     ((attr)->u.blk)
#define DW_SND(attr)       ((attr)->u.snd)
#define DW_ADDR(attr)	   ((attr)->u.addr)

extern uint8_t *db_read_attribute_value(struct attribute *, unsigned int,
                                        uint8_t **);
extern struct die_info *follow_db_ref(struct die_info *, struct attribute *,
				      struct dwarf2_cu *);
extern struct attribute *get_repository_name(struct attribute *,
					     struct dwarf2_cu *);

/* APPLE LOCAL begin  red-black trees, part 1. */
/* The following data structures and function declarations are for the
 implementaion of red-black tree data structures.  Red-black trees are
 an efficient form of pseudo-balanced binary trees.  For more information
 see the comments labelled "red-black trees, part 2.  */

/* The following data structure is used by the Dwarf types repository
 code.  When the types repository uses a red-black tree, the important
 types repository data is stored in this structure, which the red-black
 tree  nodes point to via their "void * data" field.  */

struct rb_repository_data {
  struct type *type_data;
  struct die_info *die_data;
};

/* The definitions for rb_tree_colors and rb_tree_struct can be found in
 inlining.h.  */

/* Function declarations for red-black tree manipulation functions.  See
 function definitions for more informatino about the functions.  */

extern struct rb_tree_node *
rb_tree_find_and_remove_node(struct rb_tree_node **, struct rb_tree_node *,
                             long long, int);
extern struct rb_tree_node *rb_tree_remove_node(struct rb_tree_node **,
                                                struct rb_tree_node *);
extern struct rb_tree_node *rb_tree_minimum(struct rb_tree_node *);
extern struct rb_tree_node *rb_tree_successor(struct rb_tree_node *);
extern void left_rotate(struct rb_tree_node **, struct rb_tree_node *);
extern void right_rotate(struct rb_tree_node **, struct rb_tree_node *);
extern void rb_delete_fixup(struct rb_tree_node **, struct rb_tree_node *);
extern void plain_tree_insert(struct rb_tree_node **, struct rb_tree_node *);
extern int verify_rb_tree(struct rb_tree_node *);
extern int verify_tree_heights(struct rb_tree_node *);
extern int verify_tree_colors(struct rb_tree_node *);
extern int tree_height(struct rb_tree_node *);
extern int num_nodes_in_tree(struct rb_tree_node *);
extern void rb_print_tree(struct rb_tree_node *, int);

/* APPLE LOCAL end  red-black trees, part 1. */

extern const char *dwarf_form_name(unsigned int);
extern void set_cu_language(unsigned int, struct dwarf2_cu *);
extern struct type *tag_type_to_type(struct die_info *, struct dwarf2_cu *);

#endif /* DWARF2READ_H */

/* EOF */
