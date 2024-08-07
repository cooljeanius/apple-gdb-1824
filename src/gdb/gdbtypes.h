/* gdbtypes.h: Internal type definitions for GDB.

   Copyright 1992, 1993, 1994, 1995, 1996, 1997, 1998, 1999, 2000,
   2001, 2002, 2003, 2004 Free Software Foundation, Inc.

   Contributed by Cygnus Support, using pieces from other GDB modules.

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

#if !defined (GDBTYPES_H)
#define GDBTYPES_H 1

/* Forward declarations for prototypes.  */
struct field;
struct block;

/* Codes for `fundamental types'.  This is a monstrosity based on the
   bogus notion that there are certain compiler-independent
   `fundamental types'.  None of these is well-defined (how big is
   FT_SHORT?  Does it depend on the language?  How does the
   language-specific code know which type to correlate to FT_SHORT?)  */

#define FT_VOID			0
#define FT_BOOLEAN		1
#define FT_CHAR			2	/* we use this for not-unsigned C/C++ chars */
#define FT_SIGNED_CHAR		3	/* we use this for C++ signed chars */
#define FT_UNSIGNED_CHAR	4	/* we use this for C/C++ unsigned chars */
#define FT_SHORT		5
#define FT_SIGNED_SHORT		6
#define FT_UNSIGNED_SHORT	7
#define FT_INTEGER		8
#define FT_SIGNED_INTEGER	9
#define FT_UNSIGNED_INTEGER	10
#define FT_LONG			11
#define FT_SIGNED_LONG		12
#define FT_UNSIGNED_LONG	13
#define FT_LONG_LONG		14
#define FT_SIGNED_LONG_LONG	15
#define FT_UNSIGNED_LONG_LONG	16
#define FT_FLOAT		17
#define FT_DBL_PREC_FLOAT	18
#define FT_EXT_PREC_FLOAT	19
#define FT_COMPLEX		20
#define FT_DBL_PREC_COMPLEX	21
#define FT_EXT_PREC_COMPLEX	22
#define FT_STRING		23
#define FT_FIXED_DECIMAL	24
#define FT_FLOAT_DECIMAL	25
#define FT_BYTE			26
#define FT_UNSIGNED_BYTE	27
#define FT_TEMPLATE_ARG		28

#define FT_NUM_MEMBERS		29	/* Highest FT_* above, plus one. */

/* Some macros for char-based bitfields.  */

#define B_SET(a,x)	((a)[(x)>>3] |= (1 << ((x)&7)))
#define B_CLR(a,x)	((a)[(x)>>3] &= ~(1 << ((x)&7)))
#define B_TST(a,x)	((a)[(x)>>3] & (1 << ((x)&7)))
#define B_TYPE		unsigned char
#define	B_BYTES(x)	( 1 + ((x)>>3) )
#define	B_CLRALL(a,x)	memset ((a), 0, B_BYTES(x))

/* Different kinds of data types are distinguished by the `code' field.  */

enum type_code
  {
    TYPE_CODE_UNDEF,		/* Not used; catches errors */
    TYPE_CODE_PTR,		/* Pointer type */

    /* Array type with lower & upper bounds.

       Regardless of the language, GDB represents multidimensional
       array types the way C does: as arrays of arrays.  So an
       instance of a GDB array type T can always be seen as a series
       of instances of TYPE_TARGET_TYPE (T) laid out sequentially in
       memory.

       Row-major languages like C lay out multi-dimensional arrays so
       that incrementing the rightmost index in a subscripting
       expression results in the smallest change in the address of the
       element referred to.  Column-major languages like Fortran lay
       them out so that incrementing the leftmost index results in the
       smallest change.

       This means that, in column-major languages, working our way
       from type to target type corresponds to working through indices
       from right to left, not left to right.  */
    TYPE_CODE_ARRAY,

    TYPE_CODE_STRUCT,		/* C struct or Pascal record */
    TYPE_CODE_UNION,		/* C union or Pascal variant part */
    TYPE_CODE_ENUM,		/* Enumeration type */
    TYPE_CODE_FLAGS,            /* Bit flags type */
    TYPE_CODE_FUNC,		/* Function type */
    TYPE_CODE_INT,		/* Integer type */

    /* Floating type.  This is *NOT* a complex type.  Beware, there are parts
       of GDB which bogusly assume that TYPE_CODE_FLT can mean complex.  */
    TYPE_CODE_FLT,

    /* Void type.  The length field specifies the length (probably always
       one) which is used in pointer arithmetic involving pointers to
       this type, but actually dereferencing such a pointer is invalid;
       a void type has no length and no actual representation in memory
       or registers.  A pointer to a void type is a generic pointer.  */
    TYPE_CODE_VOID,

    TYPE_CODE_SET,		/* Pascal sets */
    TYPE_CODE_RANGE,		/* Range (integers within spec'd bounds) */

    /* A string type which is like an array of character but prints
       differently (at least for (the deleted) CHILL).  It does not
       contain a length field as Pascal strings (for many Pascals,
       anyway) do; if we want to deal with such strings, we should use
       a new type code.  */
    TYPE_CODE_STRING,

    /* String of bits; like TYPE_CODE_SET but prints differently (at
       least for (the deleted) CHILL).  */
    TYPE_CODE_BITSTRING,

    /* Unknown type.  The length field is valid if we were able to
       deduce that much about the type, or 0 if we don't even know that.  */
    TYPE_CODE_ERROR,

    /* C++ */
    TYPE_CODE_MEMBER,		/* Member type */
    TYPE_CODE_METHOD,		/* Method type */

    /* Pointer-to-member-function type.  This describes how to access a
       particular member function of a class (possibly a virtual
       member function).  The representation may vary between different
       C++ ABIs.  */
    TYPE_CODE_METHODPTR,

    /* Pointer-to-member type.  This is the offset within a class to some
       particular data member.  The only currently supported representation
       uses an unbiased offset, with -1 representing NULL; this is used
       by the Itanium C++ ABI (used by GCC on all platforms).  */
    TYPE_CODE_MEMBERPTR,

    TYPE_CODE_REF,		/* C++ Reference types */

    TYPE_CODE_CHAR,		/* *real* character type */

    /* Boolean type.  0 is false, 1 is true, and other values are non-boolean
       (e.g. FORTRAN "logical" used as unsigned int).  */
    TYPE_CODE_BOOL,

    /* Fortran */
    TYPE_CODE_COMPLEX,		/* Complex float */

    TYPE_CODE_TYPEDEF,
    TYPE_CODE_TEMPLATE,		/* C++ template */
    TYPE_CODE_TEMPLATE_ARG,	/* C++ template arg */

    TYPE_CODE_NAMESPACE		/* C++ namespace.  */
  };

/* For now allow source to use TYPE_CODE_CLASS for C++ classes, as an
   alias for TYPE_CODE_STRUCT.  This is for DWARF, which has a distinct
   "class" attribute.  Perhaps we should actually have a separate TYPE_CODE
   so that we can print "class" or "struct" depending on what the debug
   info said.  It was not clear if we should bother...  */
#define TYPE_CODE_CLASS TYPE_CODE_STRUCT
/* FIXME: some locations in the code check against both, as if they had
 * different values, which provokes -Wlogical-op warnings with recent
 * versions of gcc... */

/* Some bits for the type's flags word, and macros to test them. */

/* Unsigned integer type.  If this is not set for a TYPE_CODE_INT, the
   type is signed (unless TYPE_FLAG_NOSIGN (below) is set). */

#define TYPE_FLAG_UNSIGNED	(1 << 0)
#define TYPE_UNSIGNED(t)	(TYPE_FLAGS (t) & TYPE_FLAG_UNSIGNED)

/* No sign for this type.  In C++, "char", "signed char", and "unsigned
   char" are distinct types; so we need an extra flag to indicate the
   absence of a sign! */

#define TYPE_FLAG_NOSIGN	(1 << 1)
#define TYPE_NOSIGN(t)		(TYPE_FLAGS (t) & TYPE_FLAG_NOSIGN)

/* This appears in a type's flags word if it is a stub type (e.g., if
   someone referenced a type that wasn't defined in a source file
   via (struct sir_not_appearing_in_this_film *)).  */

#define TYPE_FLAG_STUB		(1 << 2)
#define TYPE_STUB(t)		(TYPE_FLAGS (t) & TYPE_FLAG_STUB)

/* The target type of this type is a stub type, and this type needs to
   be updated if it gets un-stubbed in check_typedef.
   Used for arrays and ranges, in which TYPE_LENGTH of the array/range
   gets set based on the TYPE_LENGTH of the target type.
   Also, set for TYPE_CODE_TYPEDEF. */

#define TYPE_FLAG_TARGET_STUB	(1 << 3)
#define TYPE_TARGET_STUB(t)	(TYPE_FLAGS (t) & TYPE_FLAG_TARGET_STUB)

/* Static type.  If this is set, the corresponding type had
 * a static modifier.
 * Note: This may be unnecessary, since static data members
 * are indicated by other means (bitpos == -1)
 */

#define TYPE_FLAG_STATIC	(1 << 4)
#define TYPE_STATIC(t)		(TYPE_FLAGS (t) & TYPE_FLAG_STATIC)

/* Constant type.  If this is set, the corresponding type has a
 * const modifier.
 */

#define TYPE_FLAG_CONST		(1 << 5)
#define TYPE_CONST(t)		(TYPE_INSTANCE_FLAGS (t) & TYPE_FLAG_CONST)

/* Volatile type.  If this is set, the corresponding type has a
 * volatile modifier.
 */

#define TYPE_FLAG_VOLATILE	(1 << 6)
#define TYPE_VOLATILE(t)	(TYPE_INSTANCE_FLAGS (t) & TYPE_FLAG_VOLATILE)


/* This is a function type which appears to have a prototype.  We need this
   for function calls in order to tell us if it's necessary to coerce the args,
   or to just do the standard conversions.  This is used with a short field. */

#define TYPE_FLAG_PROTOTYPED	(1 << 7)
#define TYPE_PROTOTYPED(t)	(TYPE_FLAGS (t) & TYPE_FLAG_PROTOTYPED)

/* This flag is used to indicate that processing for this type
   is incomplete.

   (Mostly intended for HP platforms, where class methods, for
   instance, can be encountered before their classes in the debug
   info; the incomplete type has to be marked so that the class and
   the method can be assigned correct types.) */

#define TYPE_FLAG_INCOMPLETE	(1 << 8)
#define TYPE_INCOMPLETE(t)	(TYPE_FLAGS (t) & TYPE_FLAG_INCOMPLETE)

/* Instruction-space delimited type.  This is for Harvard architectures
   which have separate instruction and data address spaces (and perhaps
   others).

   GDB usually defines a flat address space that is a superset of the
   architecture's two (or more) address spaces, but this is an extension
   of the architecture's model.

   If TYPE_FLAG_INST is set, an object of the corresponding type
   resides in instruction memory, even if its address (in the extended
   flat address space) does not reflect this.

   Similarly, if TYPE_FLAG_DATA is set, then an object of the
   corresponding type resides in the data memory space, even if
   this is not indicated by its (flat address space) address.

   If neither flag is set, the default space for functions / methods
   is instruction space, and for data objects is data memory.  */

#define TYPE_FLAG_CODE_SPACE	(1 << 9)
#define TYPE_CODE_SPACE(t)	(TYPE_INSTANCE_FLAGS (t) & TYPE_FLAG_CODE_SPACE)

#define TYPE_FLAG_DATA_SPACE	(1 << 10)
#define TYPE_DATA_SPACE(t)	(TYPE_INSTANCE_FLAGS (t) & TYPE_FLAG_DATA_SPACE)

/* FIXME drow/2002-06-03:  Only used for methods, but applies as well
   to functions.  */

#define TYPE_FLAG_VARARGS	(1 << 11)
#define TYPE_VARARGS(t)		(TYPE_FLAGS (t) & TYPE_FLAG_VARARGS)

/* Identify a vector type.  Gcc is handling this by adding an extra
   attribute to the array type.  We slurp that in as a new flag of a
   type.  This is used only in dwarf2read.c.  */
#define TYPE_FLAG_VECTOR	(1 << 12)
#define TYPE_VECTOR(t)		(TYPE_FLAGS (t) & TYPE_FLAG_VECTOR)

/* Address class flags.  Some environments provide for pointers whose
   size is different from that of a normal pointer or address types
   where the bits are interpreted differently than normal addresses.  The
   TYPE_FLAG_ADDRESS_CLASS_n flags may be used in target specific
   ways to represent these different types of address classes.  */
#define TYPE_FLAG_ADDRESS_CLASS_1 (1 << 13)
#define TYPE_ADDRESS_CLASS_1(t) (TYPE_INSTANCE_FLAGS(t) \
                                 & TYPE_FLAG_ADDRESS_CLASS_1)
#define TYPE_FLAG_ADDRESS_CLASS_2 (1 << 14)
#define TYPE_ADDRESS_CLASS_2(t) (TYPE_INSTANCE_FLAGS(t) \
				 & TYPE_FLAG_ADDRESS_CLASS_2)
#define TYPE_FLAG_ADDRESS_CLASS_ALL (TYPE_FLAG_ADDRESS_CLASS_1 \
				     | TYPE_FLAG_ADDRESS_CLASS_2)
#define TYPE_ADDRESS_CLASS_ALL(t) (TYPE_INSTANCE_FLAGS(t) \
				   & TYPE_FLAG_ADDRESS_CLASS_ALL)

/* The debugging formats (especially STABS) do not contain enough information
   to represent all Ada types---especially those whose size depends on
   dynamic quantities.  Therefore, the GNAT Ada compiler includes
   extra information in the form of additional type definitions
   connected by naming conventions.  This flag indicates that the
   type is an ordinary (unencoded) GDB type that has been created from
   the necessary run-time information, and does not need further
   interpretation. Optionally marks ordinary, fixed-size GDB type. */

#define TYPE_FLAG_FIXED_INSTANCE (1 << 15)

/* APPLE LOCAL: This type is a "Closure struct".  It's both a structure,
   AND it is treated by the compiler as a "function pointer".  */
#define TYPE_FLAG_APPLE_CLOSURE (1 << 16)

/* APPLE LOCAL: This is a restrict qualifier on the type, similar to
   const or volatile.  */
#define TYPE_FLAG_RESTRICT (1 << 17)
#define TYPE_RESTRICT(t)	(TYPE_INSTANCE_FLAGS (t) & TYPE_FLAG_RESTRICT)

/* APPLE LOCAL: This flag marks function types that were optimized by
   the compiler.  */
#define TYPE_FLAG_OPTIMIZED (1 << 18)
#define TYPE_OPTIMIZED(t)       (TYPE_FLAGS (t) & TYPE_FLAG_OPTIMIZED)

/*  Array bound type.  */
enum array_bound_type
{
  BOUND_SIMPLE = 0,
  BOUND_BY_VALUE_IN_REG,
  BOUND_BY_REF_IN_REG,
  BOUND_BY_VALUE_ON_STACK,
  BOUND_BY_REF_ON_STACK,
  BOUND_CANNOT_BE_DETERMINED
};

/* Removed pushing of -Wpadded on here */

/* un-nested for C++ usage: */
struct field
{
  union field_location
  {
    /* Position of this field, counting in bits from start of
     * containing structure.
     * For BITS_BIG_ENDIAN=1 targets, it is the bit offset to the MSB.
     * For BITS_BIG_ENDIAN=0 targets, it is the bit offset to the LSB.
     * For a range bound or enum value, this is the value itself. */

    int bitpos;

    /* For a static field, if TYPE_FIELD_STATIC_HAS_ADDR then physaddr
     * is the location (in the target) of the static field.
     * Otherwise, physname is the mangled label of the static field. */

    CORE_ADDR physaddr;
    char *physname;
  }
  loc;

  /* For a function or member type, this is 1 if the argument is marked
   * artificial.  Artificial arguments should not be shown to the user: */
  unsigned int artificial : 1;

  /* This flag is zero for non-static fields, 1 for fields whose location
   * is specified by the label loc.physname, and 2 for fields whose
   * location is specified by loc.physaddr: */
  unsigned int static_kind : 2;

  /* Size of this field, in bits, or zero if not packed.
   * For an unpacked field, the field's type's length
   * says how many bytes the field occupies: */
  unsigned int bitsize : 29;

  /* In a struct or union type, type of this field.
   * In a function or member type, type of this argument.
   * In an array type, the domain-type of the array: */
  struct type *type;

  /* Name of field, value or argument.
   * NULL for range bounds, array domains, and member function
   * arguments: */
  const char *name;
};

/* This structure is space-critical.
   Its layout has been tweaked to reduce the space used.  */
typedef struct main_type
{
  /* Code for kind of type: */
  ENUM_BITFIELD(type_code) code : 8;

  /* Array bounds.  These fields appear at this location because
     they pack nicely here.  */

  ENUM_BITFIELD(array_bound_type) upper_bound_type : 4;
  ENUM_BITFIELD(array_bound_type) lower_bound_type : 4;

  /* Name of this type, or NULL if none.

     This is used for printing only, except by poorly designed C++ code.
     For looking up a name, look for a symbol in the VAR_DOMAIN.  */

  const char *name;

  /* Tag name for this type, or NULL if none.  This means that the
     name of the type consists of a keyword followed by the tag name.
     Which keyword is determined by the type code ("struct" for
     TYPE_CODE_STRUCT, etc.).  As far as I know C/C++ are the only languages
     with this feature.

     This is used for printing only, except by poorly designed C++ code.
     For looking up a name, look for a symbol in the STRUCT_DOMAIN.
     One more legitimate use is that if TYPE_FLAG_STUB is set, this is
     the name to use to look for definitions in other files.  */

  const char *tag_name;

  /* Every type is now associated with a particular objfile, and the
     type is allocated on the objfile_obstack for that objfile.  One problem
     however, is that there are times when gdb allocates new types while
     it is not in the process of reading symbols from a particular objfile.
     Fortunately, these happen when the type being created is a derived
     type of an existing type, such as in lookup_pointer_type().  So
     we can just allocate the new type using the same objfile as the
     existing type, but to do this we need a backpointer to the objfile
     from the existing type.  Yes this is somewhat ugly, but without
     major overhaul of the internal type system, it can't be avoided
     for now. */

  struct objfile *objfile;

  /* For a pointer type, describes the type of object pointed to.
     For an array type, describes the type of the elements.
     For a function or method type, describes the type of the return value.
     For a range type, describes the type of the full range.
     For a complex type, describes the type of each coordinate.
     Unused otherwise.  */

  struct type *target_type;

  /* Flags about this type: */
  int flags : 30;

  /* The byte-order of the type.  Uses the same values as the byte
     order stored in the gdbarch structure. */
  unsigned int byte_order : 2;

  /* Number of fields described for this type: */
  short nfields;

  /* Field number of the virtual function table pointer in
     VPTR_BASETYPE.  If -1, we were unable to find the virtual
     function table pointer in initial symbol reading, and
     fill_in_vptr_fieldno should be called to find it if possible.

     Unused if this type does not have virtual functions.  */
  short vptr_fieldno;

  /* For structure and union types, a description of each field.
     For set and pascal array types, there is one "field",
     whose type is the domain type of the set or array.
     For range types, there are two "fields",
     the minimum and maximum values (both inclusive).
     For enum types, each possible value is described by one "field".
     For a function or method type, a "field" for each parameter.
     For C++ classes, there is one field for each base class (if it is
     a derived class) plus one field for each class data member.  Member
     functions are recorded elsewhere.

     Using a pointer to a separate array of fields
     allows all types to have the same size, which is useful
     because we can allocate the space for a type before
     we know what to put in it.  */
  struct field *fields;

  /* For types with virtual functions (TYPE_CODE_STRUCT), VPTR_BASETYPE
     is the base class which defined the virtual function table pointer.

     For types that are pointer to member types (TYPE_CODE_MEMBER),
     VPTR_BASETYPE is the type that this pointer is a member of.

     For method types (TYPE_CODE_METHOD), VPTR_BASETYPE is the aggregate
     type that contains the method.

     Unused otherwise.  */
  struct type *vptr_basetype;

  /* Slot to point to additional language-specific fields of this type: */
  union type_specific
  {
    /* CPLUS_STUFF is for TYPE_CODE_STRUCT.  It is initialized to point to
       cplus_struct_default, a default static instance of a struct
       cplus_struct_type. */

    struct cplus_struct_type *cplus_stuff;

    /* FLOATFORMAT is for TYPE_CODE_FLT.  It is a pointer to the
       floatformat object that describes the floating-point value
       that resides within the type.  */

    const struct floatformat *floatformat;
  } type_specific;
} ATTRIBUTE_PACKED gdbtypes_h_main_type_t;

/* Removed popping of -Wpadded off here */

/* A ``struct type'' describes a particular instance of a type, with
   some particular qualification.  */
struct type
{
  /* Type that is a pointer to this type.
     NULL if no such pointer-to type is known yet.
     The debugger may add the address of such a type
     if it has to construct one later.  */

  struct type *pointer_type;

  /* C++: also need a reference type.  */

  struct type *reference_type;

  /* Variant chain.  This points to a type that differs from this one only
     in qualifiers and length.  Currently, the possible qualifiers are
     const, volatile, code-space, data-space, and address class.  The
     length may differ only when one of the address class flags are set.
     The variants are linked in a circular ring and share MAIN_TYPE.  */
  struct type *chain;

  /* Flags specific to this instance of the type, indicating where
     on the ring we are.  */
  int instance_flags;

  /* Length of storage for a value of this type.  This is what
     sizeof(type) would return; use it for address arithmetic,
     memory reads and writes, etc.  This size includes padding.  For
     example, an i386 extended-precision floating point value really
     only occupies ten bytes, but most ABI's declare its size to be
     12 bytes, to preserve alignment.  A `struct type' representing
     such a floating-point type would have a `length' value of 12,
     even though the last two bytes are unused.

     There's a bit of a host/target mess here, if you're concerned
     about machines whose bytes aren't eight bits long, or who don't
     have byte-addressed memory.  Various places pass this to memcpy
     and such, meaning it must be in units of host bytes.  Various
     other places expect they can calculate addresses by adding it
     and such, meaning it must be in units of target bytes.  For
     some DSP targets, in which HOST_CHAR_BIT will (presumably) be 8
     and TARGET_CHAR_BIT will be (say) 32, this is a problem.

     One fix would be to make this field in bits (requiring that it
     always be a multiple of HOST_CHAR_BIT and TARGET_CHAR_BIT) ---
     the other choice would be to make it consistently in units of
     HOST_CHAR_BIT.  However, this would still fail to address
     machines based on a ternary or decimal representation.  */

  /* APPLE LOCAL: I changed length from an unsigned int to an int.
   * I need to be able to mark the length as "uncertain", which I do
   * by reversing the sign.  See the comments in front of
   * objc_invalidate_class in objc-lang.c for more details.
   * I doubt we will ever get a struct whose length overflows
   * a signed integer so for all practical purposes this should
   * be fine.  */
  int length;

  /* Core type, shared by a group of qualified types.  */
  struct main_type *main_type;
};

#define	NULL_TYPE ((struct type *) 0)

/* C++ language-specific information for TYPE_CODE_STRUCT and TYPE_CODE_UNION
   nodes.  */

enum runtime_type
  {
    CPLUS_RUNTIME,
    OBJC_RUNTIME/*,*/
  };

/* for use later: */
struct fn_field
{
  /* If is_stub is clear, this is the mangled name which we can
   * look up to find the address of the method (FIXME: it would
   * be cleaner to have a pointer to the struct symbol here
   * instead).  */

  /* If is_stub is set, this is the portion of the mangled
   * name which specifies the arguments.  For example, "ii",
   * if there are two int arguments, or "" if there are no
   * arguments.  See gdb_mangle_name for the conversion from this
   * format to the one used if is_stub is clear.  */

  const char *physname;

  /* The function type for the method.
   * (This comment used to say "The return value of the method",
   * but that is wrong. The function type
   * is expected here, i.e. something with TYPE_CODE_FUNC,
   * and *not* the return-value type). */

  struct type *type;

  /* For virtual functions.
   * First baseclass that defines this virtual function.   */

  struct type *fcontext;

  /* Attributes:*/
  unsigned int is_const:1;
  unsigned int is_volatile:1;
  unsigned int is_private:1;
  unsigned int is_protected:1;
  unsigned int is_public:1;
  unsigned int is_abstract:1;
  unsigned int is_static:1;
  unsigned int is_final:1;
  unsigned int is_synchronized:1;
  unsigned int is_native:1;
  unsigned int is_artificial:1;

  /* A stub method only has some fields valid (but they are enough
   * to reconstruct the rest of the fields).  */
  unsigned int is_stub:1;

  /* C++ method that is inlined: */
  unsigned int is_inlined:1;

  /* Unused: */
  unsigned int dummy:3;

  /* Index into that baseclass's virtual function table,
   * minus 2; else if static: VOFFSET_STATIC; else: 0.  */
  unsigned int voffset:16;

#define VOFFSET_STATIC 1
};

/* likewise, un-nested for C++ usage: */
struct fn_fieldlist
{
  /* The overloaded name: */
  const char *name;

  /* The number of methods with this name: */
  int length;

  /* The list of methods: */
  struct fn_field *fn_fields;
};

/* likewise: */
struct template_arg
{
  char *name;
  struct type *type;
};

/* likewise: */
struct runtime_info
{
  short has_vtable;
  struct type *primary_base;
  struct type **virtual_base_list;
};

/* likewise: */
struct local_type_info
{
  char *file;
  int line;
};

struct cplus_struct_type
  {
    /* Number of base classes this type derives from.  The baseclasses are
       stored in the first N_BASECLASSES fields (i.e. the `fields' field of
       the struct type).  I think only the `type' field of such a field has
       any meaning.  */

    short n_baseclasses;

    /* Number of methods with unique names.  All overloaded methods with
       the same name count only once. */

    short nfn_fields;

    /* Number of methods described for this type, not including the
       methods that it derives from.  */

    short nfn_fields_total;

    /* The "declared_type" field contains a code saying how the
       user really declared this type, e.g., "class s", "union s",
       "struct s".
       The 3 above things come out from the C++ compiler looking like classes,
       but we keep track of the real declaration so we can give
       the correct information on "ptype". (Note: TEMPLATE may not
       belong in this list...)  */

#define DECLARED_TYPE_CLASS 0
#define DECLARED_TYPE_UNION 1
#define DECLARED_TYPE_STRUCT 2
#define DECLARED_TYPE_TEMPLATE 3
    short declared_type;	/* One of the above codes */

    /* APPLE LOCAL: The struct cplus_struct_type actually gets used for
       ObjC AND C++ Objects.  But for some things you need to treat ObjC
       objects differently.  So we record here in the type which runtime
       this object belongs to.  */
    enum runtime_type runtime_type;

    /* For derived classes, the number of base classes is given by n_baseclasses
       and virtual_field_bits is a bit vector containing one bit per base class.
       If the base class is virtual, the corresponding bit will be set.
       I.E, given:

       class A{};
       class B{};
       class C : public B, public virtual A {};

       B is a baseclass of C; A is a virtual baseclass for C.
       This is a C++ 2.0 language feature. */

    B_TYPE *virtual_field_bits;

    /* For classes with private fields, the number of fields is given by
       nfields and private_field_bits is a bit vector containing one bit
       per field.
       If the field is private, the corresponding bit will be set. */

    B_TYPE *private_field_bits;

    /* For classes with protected fields, the number of fields is given by
       nfields and protected_field_bits is a bit vector containing one bit
       per field.
       If the field is private, the corresponding bit will be set. */

    B_TYPE *protected_field_bits;

    /* for classes with fields to be ignored, either this is optimized out
       or this field has length 0 */

    B_TYPE *ignore_field_bits;

    /* For classes, structures, and unions, a description of each field,
       which consists of an overloaded name, followed by the types of
       arguments that the method expects, and then the name after it
       has been renamed to make it distinct.

       fn_fieldlists points to an array of nfn_fields of these. */

    struct fn_fieldlist *fn_fieldlists;

    /* If this "struct type" describes a template, then it
     * has arguments. "template_args" points to an array of
     * template arg descriptors, of length "ntemplate_args".
     * The only real information in each of these template arg descriptors
     * is a name. "type" will typically just point to a "struct type" with
     * the placeholder TYPE_CODE_TEMPLATE_ARG type.  */
    short ntemplate_args;
    struct template_arg *template_args;

    /* If this "struct type" describes a template, it has a list
     * of instantiations. "instantiations" is a pointer to an array
     * of type's, one representing each instantiation. There
     * are "ninstantiations" elements in this array.  */
    short ninstantiations;
    struct type **instantiations;

    /* The following points to information relevant to the runtime model
     * of the compiler.
     * Currently being used only for HP's ANSI C++ compiler.
     * (This type may have to be changed/enhanced for other compilers.)
     *
     * RUNTIME_PTR is NULL if there is no runtime information (currently
     * this means the type was not compiled by HP aCC).
     *
     * Fields in structure pointed to:
     * ->HAS_VTABLE : 0 => no virtual table, 1 => vtable present
     *
     * ->PRIMARY_BASE points to the first non-virtual base class that has
     * a virtual table.
     *
     * ->VIRTUAL_BASE_LIST points to a list of struct type * pointers that
     * point to the type information for all virtual bases among the
     * ancestors of this type.  */
    struct runtime_info *runtime_ptr;

    /* Pointer to information about enclosing scope, if this is a
     * local type.  If it is not a local type, then this is NULL: */
    struct local_type_info *localtype_ptr;
  };

/* Struct used in computing virtual base list */
struct vbase
  {
    struct type *vbasetype;	/* pointer to virtual base */
    struct vbase *next;		/* next in chain */
  };

/* Struct used for ranking a function for overload resolution */
struct badness_vector
  {
    int length;
    int *rank;
  };

/* APPLE LOCAL BEGIN: Helper function type defintions for easily
   building bitfield built in types.  */
struct gdbtypes_enum_info
  {
    const char *name;
    int32_t value;
  };

/* Struct used by  for quickly creating enumerations to be used with */
struct gdbtypes_bitfield_info
  {
    const char *name;
    struct type *type;
    uint8_t msbit;
    uint8_t lsbit;
  };
/* APPLE LOCAL END.  */


/* APPLE LOCAL: I need these two objc fixup functions for TYPE_LENGTH
   and TYPE_FIELD_BITPOS.  */
int objc_fixup_ivar_offset(const struct type *type, int ivar);
int objc_fixup_class_length(const struct type *type);
/* END APPLE LOCAL */


/* The default value of TYPE_CPLUS_SPECIFIC(T) points to the
   this shared static structure. */

extern struct cplus_struct_type cplus_struct_default;

extern void allocate_cplus_struct_type (struct type *);

#define INIT_CPLUS_SPECIFIC(type) \
  (TYPE_CPLUS_SPECIFIC(type) = NULL)
#define ALLOCATE_CPLUS_STRUCT_TYPE(type) allocate_cplus_struct_type (type)
#define HAVE_CPLUS_STRUCT(type) \
  (TYPE_CPLUS_SPECIFIC(type) != NULL)

#define TYPE_INSTANCE_FLAGS(thistype) (thistype)->instance_flags
#define TYPE_MAIN_TYPE(thistype) (thistype)->main_type
#define TYPE_NAME(thistype) TYPE_MAIN_TYPE(thistype)->name
#define TYPE_TAG_NAME(type) TYPE_MAIN_TYPE(type)->tag_name
#define TYPE_TARGET_TYPE(thistype) TYPE_MAIN_TYPE(thistype)->target_type
#define TYPE_POINTER_TYPE(thistype) (thistype)->pointer_type
#define TYPE_REFERENCE_TYPE(thistype) (thistype)->reference_type
#define TYPE_CHAIN(thistype) (thistype)->chain
/* Note that if thistype is a TYPEDEF type, you have to call check_typedef.
   But check_typedef does set the TYPE_LENGTH of the TYPEDEF type,
   so you only have to call check_typedef once.  Since allocate_value
   calls check_typedef, TYPE_LENGTH (VALUE_TYPE (X)) is safe.  */
/* APPLE LOCAL: Like with TYPE_FIELD_BITPOS for an ObjC class you cannot
   actually trust the debug info for ivar offsets OR type lengths for
   objc classes.  I set the length to -1, and then test it here,
   and return the actual value if not -1, or I fix it up and
   return the fixed up value.  I also need an ASSIGN version.  */
#define TYPE_LENGTH_ASSIGN(thistype) (thistype)->length
#define TYPE_LENGTH(thistype) ((TYPE_LENGTH_ASSIGN(thistype) < 0) \
			       ? objc_fixup_class_length(thistype)  \
			       : TYPE_LENGTH_ASSIGN(thistype))

#define TYPE_OBJFILE(thistype) TYPE_MAIN_TYPE(thistype)->objfile
#define TYPE_FLAGS(thistype) TYPE_MAIN_TYPE(thistype)->flags
/* Note that TYPE_CODE can be TYPE_CODE_TYPEDEF, so if you want the real
   type, you need to do TYPE_CODE (check_type (this_type)). */
#define TYPE_CODE(thistype) TYPE_MAIN_TYPE(thistype)->code
#define TYPE_BYTE_ORDER(thistype) TYPE_MAIN_TYPE(thistype)->byte_order
#define TYPE_NFIELDS(thistype) TYPE_MAIN_TYPE(thistype)->nfields
#define TYPE_FIELDS(thistype) TYPE_MAIN_TYPE(thistype)->fields
#define TYPE_TEMPLATE_ARGS(thistype) TYPE_CPLUS_SPECIFIC_NONULL(thistype)->template_args
#define TYPE_INSTANTIATIONS(thistype) TYPE_CPLUS_SPECIFIC_NONULL(thistype)->instantiations
/* APPLE LOCAL: Which runtime this type belongs to:  */
#define TYPE_RUNTIME(thistype) (TYPE_CPLUS_SPECIFIC_NONULL(thistype)->runtime_type)

#define TYPE_INDEX_TYPE(type) TYPE_FIELD_TYPE (type, 0)
#define TYPE_LOW_BOUND(range_type) TYPE_FIELD_BITPOS_ASSIGN (range_type, 0)
#define TYPE_HIGH_BOUND(range_type) TYPE_FIELD_BITPOS_ASSIGN (range_type, 1)
/* APPLE LOCAL: Used to store the stride of vector types, to prevent reverse-order indexing. */
#define TYPE_STRIDE(range_type) TYPE_FIELD_BITPOS_ASSIGN (range_type, 2)

/* Moto-specific stuff for FORTRAN arrays */

#define TYPE_ARRAY_UPPER_BOUND_TYPE(thistype) \
	TYPE_MAIN_TYPE(thistype)->upper_bound_type
#define TYPE_ARRAY_LOWER_BOUND_TYPE(thistype) \
	TYPE_MAIN_TYPE(thistype)->lower_bound_type

#define TYPE_ARRAY_UPPER_BOUND_VALUE(arraytype) \
   (TYPE_FIELD_BITPOS_ASSIGN((TYPE_FIELD_TYPE((arraytype),0)),1))

#define TYPE_ARRAY_LOWER_BOUND_VALUE(arraytype) \
   (TYPE_FIELD_BITPOS_ASSIGN((TYPE_FIELD_TYPE((arraytype),0)),0))

/* C++ */

#if defined(__GNUC__) && defined(__GNUC_MINOR__)
# if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6))
 #  pragma GCC diagnostic push
 #  pragma GCC diagnostic ignored "-Waddress"
# endif /* gcc 4.6+ */
#endif /* any gcc */

#define TYPE_VPTR_BASETYPE(thistype) TYPE_MAIN_TYPE(thistype)->vptr_basetype
#define TYPE_DOMAIN_TYPE(thistype) TYPE_MAIN_TYPE(thistype)->vptr_basetype
#define TYPE_VPTR_FIELDNO(thistype) TYPE_MAIN_TYPE(thistype)->vptr_fieldno
#define TYPE_FN_FIELDS(thistype) TYPE_CPLUS_SPECIFIC_NONULL(thistype)->fn_fields
#define TYPE_NFN_FIELDS(thistype) TYPE_CPLUS_SPECIFIC_NONULL(thistype)->nfn_fields
#define TYPE_NFN_FIELDS_TOTAL(thistype) TYPE_CPLUS_SPECIFIC_NONULL(thistype)->nfn_fields_total
#define TYPE_NTEMPLATE_ARGS(thistype) TYPE_CPLUS_SPECIFIC_NONULL(thistype)->ntemplate_args
#define TYPE_NINSTANTIATIONS(thistype) TYPE_CPLUS_SPECIFIC_NONULL(thistype)->ninstantiations
#define TYPE_DECLARED_TYPE(thistype) TYPE_CPLUS_SPECIFIC_NONULL(thistype)->declared_type
#define	TYPE_TYPE_SPECIFIC(thistype) TYPE_MAIN_TYPE(thistype)->type_specific
#define TYPE_CPLUS_SPECIFIC(thistype) TYPE_MAIN_TYPE(thistype)->type_specific.cplus_stuff
#define TYPE_CPLUS_SPECIFIC_NONULL(thistype) ((TYPE_CPLUS_SPECIFIC(thistype) != NULL) ? \
  TYPE_CPLUS_SPECIFIC(thistype) : &cplus_struct_default)
#define TYPE_FLOATFORMAT(thistype) TYPE_MAIN_TYPE(thistype)->type_specific.floatformat
#define TYPE_BASECLASS(thistype,index) TYPE_MAIN_TYPE(thistype)->fields[index].type
#define TYPE_N_BASECLASSES(thistype) TYPE_CPLUS_SPECIFIC_NONULL(thistype)->n_baseclasses
#define TYPE_BASECLASS_NAME(thistype,index) TYPE_MAIN_TYPE(thistype)->fields[index].name
#define TYPE_BASECLASS_BITPOS(thistype,index) TYPE_FIELD_BITPOS(thistype,index)
#define BASETYPE_VIA_PUBLIC(thistype, index) \
  ((!TYPE_FIELD_PRIVATE(thistype, index)) && (!TYPE_FIELD_PROTECTED(thistype, index)))

#define BASETYPE_VIA_VIRTUAL(thistype, index) \
  (TYPE_CPLUS_SPECIFIC_NONULL(thistype)->virtual_field_bits == NULL ? 0 \
    : B_TST(TYPE_CPLUS_SPECIFIC_NONULL(thistype)->virtual_field_bits, (index)))

#define FIELD_TYPE(thisfld) ((thisfld).type)
#define FIELD_NAME(thisfld) ((thisfld).name)
#define FIELD_BITPOS(thisfld) ((thisfld).loc.bitpos)
#define FIELD_ARTIFICIAL(thisfld) ((thisfld).artificial)
#define FIELD_BITSIZE(thisfld) ((thisfld).bitsize)
#define FIELD_STATIC_KIND(thisfld) ((thisfld).static_kind)
#define FIELD_PHYSNAME(thisfld) ((thisfld).loc.physname)
#define FIELD_PHYSADDR(thisfld) ((thisfld).loc.physaddr)
#define SET_FIELD_PHYSNAME(thisfld, name) \
  ((thisfld).static_kind = 1, FIELD_PHYSNAME(thisfld) = (name))
#define SET_FIELD_PHYSADDR(thisfld, name) \
  ((thisfld).static_kind = 2, FIELD_PHYSADDR(thisfld) = (name))
#define TYPE_FIELD(thistype, n) TYPE_MAIN_TYPE(thistype)->fields[n]
#define TYPE_FIELD_TYPE(thistype, n) FIELD_TYPE(TYPE_FIELD(thistype, n))
#define TYPE_FIELD_NAME(thistype, n) FIELD_NAME(TYPE_FIELD(thistype, n))

/* APPLE LOCAL: The debug info for ObjC classes is not trustworthy.  I could
   go fix it on readin, but that would be slow.  Instead I want to intercept
   reading the bitpos & just fix it up on demand.  To do that I need to separate
   assigning the BITPOS from reading it.  */
#define TYPE_FIELD_BITPOS_ASSIGN(thistype, n) FIELD_BITPOS(TYPE_FIELD(thistype,n))
#define TYPE_FIELD_BITPOS(thistype, n) \
  (FIELD_BITPOS(TYPE_FIELD(thistype,n)) < 0 \
   ? objc_fixup_ivar_offset(thistype,n) \
   : FIELD_BITPOS(TYPE_FIELD(thistype,n)))
/* END APPLE LOCAL */

#define TYPE_FIELD_ARTIFICIAL(thistype, n) FIELD_ARTIFICIAL(TYPE_FIELD(thistype,n))
#define TYPE_FIELD_BITSIZE(thistype, n) FIELD_BITSIZE(TYPE_FIELD(thistype,n))
#define TYPE_FIELD_PACKED(thistype, n) (FIELD_BITSIZE(TYPE_FIELD(thistype,n))!=0)
#define TYPE_TEMPLATE_ARG(thistype, n) TYPE_CPLUS_SPECIFIC_NONULL(thistype)->template_args[n]
#define TYPE_INSTANTIATION(thistype, n) TYPE_CPLUS_SPECIFIC_NONULL(thistype)->instantiations[n]

#define TYPE_FIELD_PRIVATE_BITS(thistype) \
  TYPE_CPLUS_SPECIFIC_NONULL(thistype)->private_field_bits
#define TYPE_FIELD_PROTECTED_BITS(thistype) \
  TYPE_CPLUS_SPECIFIC_NONULL(thistype)->protected_field_bits
#define TYPE_FIELD_IGNORE_BITS(thistype) \
  TYPE_CPLUS_SPECIFIC_NONULL(thistype)->ignore_field_bits
#define TYPE_FIELD_VIRTUAL_BITS(thistype) \
  TYPE_CPLUS_SPECIFIC_NONULL(thistype)->virtual_field_bits
#define SET_TYPE_FIELD_PRIVATE(thistype, n) \
  B_SET (TYPE_CPLUS_SPECIFIC_NONULL(thistype)->private_field_bits, (n))
#define SET_TYPE_FIELD_PROTECTED(thistype, n) \
  B_SET (TYPE_CPLUS_SPECIFIC_NONULL(thistype)->protected_field_bits, (n))
#define SET_TYPE_FIELD_IGNORE(thistype, n) \
  B_SET (TYPE_CPLUS_SPECIFIC_NONULL(thistype)->ignore_field_bits, (n))
#define SET_TYPE_FIELD_VIRTUAL(thistype, n) \
  B_SET (TYPE_CPLUS_SPECIFIC_NONULL(thistype)->virtual_field_bits, (n))
#define TYPE_FIELD_PRIVATE(thistype, n) \
  (TYPE_CPLUS_SPECIFIC_NONULL(thistype)->private_field_bits == NULL ? 0 \
    : B_TST(TYPE_CPLUS_SPECIFIC_NONULL(thistype)->private_field_bits, (n)))
#define TYPE_FIELD_PROTECTED(thistype, n) \
  (TYPE_CPLUS_SPECIFIC_NONULL(thistype)->protected_field_bits == NULL ? 0 \
    : B_TST(TYPE_CPLUS_SPECIFIC_NONULL(thistype)->protected_field_bits, (n)))
#define TYPE_FIELD_IGNORE(thistype, n) \
  (TYPE_CPLUS_SPECIFIC_NONULL(thistype)->ignore_field_bits == NULL ? 0 \
    : B_TST(TYPE_CPLUS_SPECIFIC_NONULL(thistype)->ignore_field_bits, (n)))
#define TYPE_FIELD_VIRTUAL(thistype, n) \
  (TYPE_CPLUS_SPECIFIC_NONULL(thistype)->virtual_field_bits == NULL ? 0 \
    : B_TST(TYPE_CPLUS_SPECIFIC_NONULL(thistype)->virtual_field_bits, (n)))

#define TYPE_FIELD_STATIC(thistype, n) (TYPE_MAIN_TYPE (thistype)->fields[n].static_kind != 0)
#define TYPE_FIELD_STATIC_KIND(thistype, n) TYPE_MAIN_TYPE (thistype)->fields[n].static_kind
#define TYPE_FIELD_STATIC_HAS_ADDR(thistype, n) (TYPE_MAIN_TYPE (thistype)->fields[n].static_kind == 2)
#define TYPE_FIELD_STATIC_PHYSNAME(thistype, n) FIELD_PHYSNAME(TYPE_FIELD(thistype, n))
#define TYPE_FIELD_STATIC_PHYSADDR(thistype, n) FIELD_PHYSADDR(TYPE_FIELD(thistype, n))

#define TYPE_FN_FIELDLISTS(thistype) TYPE_CPLUS_SPECIFIC_NONULL(thistype)->fn_fieldlists
#define TYPE_FN_FIELDLIST(thistype, n) TYPE_CPLUS_SPECIFIC_NONULL(thistype)->fn_fieldlists[n]
#define TYPE_FN_FIELDLIST1(thistype, n) TYPE_CPLUS_SPECIFIC_NONULL(thistype)->fn_fieldlists[n].fn_fields
#define TYPE_FN_FIELDLIST_NAME(thistype, n) TYPE_CPLUS_SPECIFIC_NONULL(thistype)->fn_fieldlists[n].name
#define TYPE_FN_FIELDLIST_LENGTH(thistype, n) TYPE_CPLUS_SPECIFIC_NONULL(thistype)->fn_fieldlists[n].length

#define TYPE_FN_FIELD(thisfn, n) (thisfn)[n]
#define TYPE_FN_FIELD_PHYSNAME(thisfn, n) (thisfn)[n].physname
#define TYPE_FN_FIELD_TYPE(thisfn, n) (thisfn)[n].type
#define TYPE_FN_FIELD_ARGS(thisfn, n) TYPE_FIELDS ((thisfn)[n].type)
#define TYPE_FN_FIELD_CONST(thisfn, n) ((thisfn)[n].is_const)
#define TYPE_FN_FIELD_VOLATILE(thisfn, n) ((thisfn)[n].is_volatile)
#define TYPE_FN_FIELD_PRIVATE(thisfn, n) ((thisfn)[n].is_private)
#define TYPE_FN_FIELD_PROTECTED(thisfn, n) ((thisfn)[n].is_protected)
#define TYPE_FN_FIELD_PUBLIC(thisfn, n) ((thisfn)[n].is_public)
#define TYPE_FN_FIELD_STATIC(thisfn, n) ((thisfn)[n].is_static)
#define TYPE_FN_FIELD_FINAL(thisfn, n) ((thisfn)[n].is_final)
#define TYPE_FN_FIELD_SYNCHRONIZED(thisfn, n) ((thisfn)[n].is_synchronized)
#define TYPE_FN_FIELD_NATIVE(thisfn, n) ((thisfn)[n].is_native)
#define TYPE_FN_FIELD_ARTIFICIAL(thisfn, n) ((thisfn)[n].is_artificial)
#define TYPE_FN_FIELD_ABSTRACT(thisfn, n) ((thisfn)[n].is_abstract)
#define TYPE_FN_FIELD_STUB(thisfn, n) ((thisfn)[n].is_stub)
#define TYPE_FN_FIELD_INLINED(thisfn, n) ((thisfn)[n].is_inlined)
#define TYPE_FN_FIELD_FCONTEXT(thisfn, n) ((thisfn)[n].fcontext)
#define TYPE_FN_FIELD_VOFFSET(thisfn, n) ((thisfn)[n].voffset-2)
#define TYPE_FN_FIELD_VIRTUAL_P(thisfn, n) ((thisfn)[n].voffset > 1)
#define TYPE_FN_FIELD_STATIC_P(thisfn, n) ((thisfn)[n].voffset == VOFFSET_STATIC)

#define TYPE_RUNTIME_PTR(thistype) (TYPE_CPLUS_SPECIFIC_NONULL(thistype)->runtime_ptr)
#define TYPE_VTABLE(thistype) (TYPE_RUNTIME_PTR(thistype)->has_vtable)
#define TYPE_HAS_VTABLE(thistype) (TYPE_RUNTIME_PTR(thistype) && TYPE_VTABLE(thistype))
#define TYPE_PRIMARY_BASE(thistype) (TYPE_RUNTIME_PTR(thistype)->primary_base)
#define TYPE_VIRTUAL_BASE_LIST(thistype) (TYPE_RUNTIME_PTR(thistype)->virtual_base_list)

#define TYPE_LOCALTYPE_PTR(thistype) (TYPE_CPLUS_SPECIFIC_NONULL(thistype)->localtype_ptr)
#define TYPE_LOCALTYPE_FILE(thistype) (TYPE_CPLUS_SPECIFIC_NONULL(thistype)->localtype_ptr->file)
#define TYPE_LOCALTYPE_LINE(thistype) (TYPE_CPLUS_SPECIFIC_NONULL(thistype)->localtype_ptr->line)

/* APPLE LOCAL: A struct type is opaque (a declaration only, no definition
   available) if it has no subelements (NFIELDS==0) *or* if it has a non-zero
   length.  This latter part comes in to play if you define a struct with no
   elements, e.g. 'struct POSITION { };' - that type will have a length of 1
   (at least by gcc's current behavior) and is a definition,
   not just a declaration.  */

#define TYPE_IS_OPAQUE(thistype) (((TYPE_CODE(thistype) == TYPE_CODE_STRUCT) ||        \
                                   (TYPE_CODE(thistype) == TYPE_CODE_UNION))        && \
                                  ((TYPE_NFIELDS(thistype) == 0) && (TYPE_LENGTH(thistype) == 0)) && \
                                  (TYPE_CPLUS_SPECIFIC_NONULL(thistype) && (TYPE_NFN_FIELDS(thistype) == 0)))

/* keep the condition the same as where we push: */
#if defined(__GNUC__) && defined(__GNUC_MINOR__)
# if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6))
 #  pragma GCC diagnostic pop
# endif /* gcc 4.6+ */
#endif /* any gcc */

struct builtin_type
{
  /* Address/pointer types.  */

  /* `pointer to data' type.  Some target platforms use an implicitly
     {sign,zero} -extended 32-bit ABI pointer on a 64-bit ISA.  */
  struct type *builtin_data_ptr;

  /* `pointer to function (returning void)' type.  Harvard
     architectures mean that ABI function and code pointers are not
     interconvertible.  Similarly, since ANSI, C standards have
     explicitly said that pointers to functions and pointers to data
     are not interconvertible --- that is, you can't cast a function
     pointer to void * and back, and expect to get the same value.
     However, all function pointer types are interconvertible, so void
     (*) () can server as a generic function pointer.  */
  struct type *builtin_func_ptr;

  /* The target CPU's address type.  This is the ISA address size.  */
  struct type *builtin_core_addr;

  /* Integral types.  */

  /* We use this for the '/c' print format, because c_char is just a
     one-byte integral type, which languages less laid back than C
     will print as ... well, a one-byte integral type.  */
  struct type *builtin_true_char;

  /* Implicit size/sign (based on the the architecture's ABI).  */
  struct type *builtin_void;
  struct type *builtin_char;
  struct type *builtin_short;
  struct type *builtin_int;
  struct type *builtin_long;
  struct type *builtin_signed_char;
  struct type *builtin_unsigned_char;
  struct type *builtin_unsigned_short;
  struct type *builtin_unsigned_int;
  struct type *builtin_unsigned_long;
  struct type *builtin_float;
  struct type *builtin_double;
  struct type *builtin_long_double;
  struct type *builtin_complex;
  struct type *builtin_double_complex;
  struct type *builtin_string;
  struct type *builtin_bool;
  struct type *builtin_long_long;
  struct type *builtin_unsigned_long_long;
};

/* Return the type table for the specified architecture.  */
extern const struct builtin_type *get_builtin_type(struct gdbarch *gdbarch);

/* Implicit sizes */
extern struct type *builtin_type_void;
extern struct type *builtin_type_char;
extern struct type *builtin_type_short;
extern struct type *builtin_type_int;
extern struct type *builtin_type_long;
extern struct type *builtin_type_signed_char;
extern struct type *builtin_type_unsigned_char;
extern struct type *builtin_type_unsigned_short;
extern struct type *builtin_type_unsigned_int;
extern struct type *builtin_type_unsigned_long;
extern struct type *builtin_type_float;
extern struct type *builtin_type_double;
extern struct type *builtin_type_long_double;
extern struct type *builtin_type_complex;
extern struct type *builtin_type_double_complex;
extern struct type *builtin_type_string;
extern struct type *builtin_type_bool;

/* Address/pointer types: */
/* (C) Language `pointer to data' type.  Some target platforms use an
   implicitly {sign,zero} -extended 32 bit C language pointer on a 64
   bit ISA.  */
extern struct type *builtin_type_void_data_ptr;

/* (C) Language `pointer to function returning void' type.  Since
   ANSI, C standards have explicitly said that pointers to functions
   and pointers to data are not interconvertible --- that is, you
   can't cast a function pointer to void * and back, and expect to get
   the same value.  However, all function pointer types are
   interconvertible, so void (*)() can server as a generic function
   pointer.  */
extern struct type *builtin_type_void_func_ptr;

/* The target CPU's address type.  This is the ISA address size. */
extern struct type *builtin_type_CORE_ADDR;
/* The symbol table address type.  Some object file formats have a 32
   bit address type even though the TARGET has a 64 bit pointer type
   (cf MIPS). */
extern struct type *builtin_type_bfd_vma;
extern struct type *builtin_type_voidptrfuncptr;

/* Explicit sizes - see C9X <intypes.h> for naming scheme.  The "int0"
   is for when an architecture needs to describe a register that has
   no size.  */
extern struct type *builtin_type_int0;
extern struct type *builtin_type_int8;
extern struct type *builtin_type_uint8;
extern struct type *builtin_type_int16;
extern struct type *builtin_type_uint16;
extern struct type *builtin_type_int32;
extern struct type *builtin_type_uint32;
extern struct type *builtin_type_int64;
extern struct type *builtin_type_uint64;
extern struct type *builtin_type_int128;
extern struct type *builtin_type_uint128;

/* SIMD types.  We inherit these names from GCC.  */
extern struct type *builtin_type_v4sf;
extern struct type *builtin_type_v4si;
extern struct type *builtin_type_v16qi;
extern struct type *builtin_type_v8qi;
extern struct type *builtin_type_v8hi;
extern struct type *builtin_type_v4hi;
extern struct type *builtin_type_v2si;

/* Types for 64 bit vectors. */
extern struct type *builtin_type_v2_float;
extern struct type *builtin_type_v2_int32;
extern struct type *builtin_type_v4_int16;
extern struct type *builtin_type_v8_int8;
extern struct type *builtin_type_vec64;

/* Types for 128 bit vectors. */
extern struct type *builtin_type_v2_double;
extern struct type *builtin_type_v4_float;
extern struct type *builtin_type_v2_int64;
extern struct type *builtin_type_v4_int32;
extern struct type *builtin_type_v8_int16;
extern struct type *builtin_type_v16_int8;
extern struct type *builtin_type_vec128;

/* Explicit floating-point formats.  See "floatformat.h".  */
extern struct type *builtin_type_ieee_single[BFD_ENDIAN_UNKNOWN];
extern struct type *builtin_type_ieee_single_big;
extern struct type *builtin_type_ieee_single_little;
extern struct type *builtin_type_ieee_double[BFD_ENDIAN_UNKNOWN];
extern struct type *builtin_type_ieee_double_big;
extern struct type *builtin_type_ieee_double_little;
extern struct type *builtin_type_ieee_double_littlebyte_bigword;
extern struct type *builtin_type_i387_ext;
extern struct type *builtin_type_m68881_ext;
extern struct type *builtin_type_i960_ext;
extern struct type *builtin_type_m88110_ext;
extern struct type *builtin_type_m88110_harris_ext;
extern struct type *builtin_type_arm_ext[BFD_ENDIAN_UNKNOWN];
extern struct type *builtin_type_arm_ext_big;
extern struct type *builtin_type_arm_ext_littlebyte_bigword;
extern struct type *builtin_type_ia64_spill[BFD_ENDIAN_UNKNOWN];
extern struct type *builtin_type_ia64_spill_big;
extern struct type *builtin_type_ia64_spill_little;
extern struct type *builtin_type_ia64_quad[BFD_ENDIAN_UNKNOWN];
extern struct type *builtin_type_ia64_quad_big;
extern struct type *builtin_type_ia64_quad_little;

/* We use this for the '/c' print format, because builtin_type_char is
   just a one-byte integral type, which languages less laid back than
   C will print as ... well, a one-byte integral type.  */
extern struct type *builtin_type_true_char;

/* This type represents a type that was unrecognized in symbol
   read-in.  */

extern struct type *builtin_type_error;

extern struct type *builtin_type_long_long;
extern struct type *builtin_type_unsigned_long_long;

/* Modula-2 types */

extern struct type *builtin_type_m2_char;
extern struct type *builtin_type_m2_int;
extern struct type *builtin_type_m2_card;
extern struct type *builtin_type_m2_real;
extern struct type *builtin_type_m2_bool;

/* OBSOLETE Chill types */

extern struct type *builtin_type_chill_bool;
extern struct type *builtin_type_chill_char;
extern struct type *builtin_type_chill_long;
extern struct type *builtin_type_chill_ulong;
extern struct type *builtin_type_chill_real;

/* Fortran (F77) types */

extern struct type *builtin_type_f_character;
extern struct type *builtin_type_f_integer;
extern struct type *builtin_type_f_integer_s2;
extern struct type *builtin_type_f_logical;
extern struct type *builtin_type_f_logical_s1;
extern struct type *builtin_type_f_logical_s2;
extern struct type *builtin_type_f_real;
extern struct type *builtin_type_f_real_s8;
extern struct type *builtin_type_f_real_s16;
extern struct type *builtin_type_f_complex_s8;
extern struct type *builtin_type_f_complex_s16;
extern struct type *builtin_type_f_complex_s32;
extern struct type *builtin_type_f_void;

/* RTTI for C++ */
#if defined(_CPPRTTI) || defined(__GXX_RTTI)
extern struct type *builtin_type_cxx_typeinfo;
#endif /* _CPPRTTI || __GXX_RTTI */

/* Maximum and minimum values of built-in types */

#define	MAX_OF_TYPE(t)	\
   (TYPE_UNSIGNED(t) ? UMAX_OF_SIZE(TYPE_LENGTH(t)) \
    : MAX_OF_SIZE(TYPE_LENGTH(t)))

#define MIN_OF_TYPE(t)	\
   (TYPE_UNSIGNED(t) ? UMIN_OF_SIZE(TYPE_LENGTH(t)) \
    : MIN_OF_SIZE(TYPE_LENGTH(t)))

/* Allocate space for storing data associated with a particular type.
   We ensure that the space is allocated using the same mechanism that
   was used to allocate the space for the type structure itself.  I.E.
   if the type is on an objfile's objfile_obstack, then the space for data
   associated with that type will also be allocated on the objfile_obstack.
   If the type is not associated with any particular objfile (such as
   builtin types), then the data space will be allocated with xmalloc,
   the same as for the type structure. */

#define TYPE_ALLOC(t,size)  \
   ((TYPE_OBJFILE(t) != NULL)  \
    ? obstack_alloc(&TYPE_OBJFILE(t)->objfile_obstack, size) \
    : xmalloc(size))

#define TYPE_ZALLOC(t,size)  \
   (TYPE_OBJFILE_OWNED(t) \
    ? memset(obstack_alloc(&TYPE_OBJFILE(t)->objfile_obstack, size),  \
	      0, size)  \
    : xzalloc(size))

extern struct type *alloc_type(struct objfile *);
extern struct type *alloc_type_arch(struct gdbarch *);
extern struct type *alloc_type_copy(const struct type *);

/* * Return the type's architecture.  For types owned by an
 architecture, that architecture is returned.  For types owned by an
 objfile, that objfile's architecture is returned.  */

extern struct gdbarch *get_type_arch(const struct type *);

extern struct type *init_type(enum type_code, int, int, const char *,
			      struct objfile *);

/* Helper functions to construct a struct or record type.  An
   initially empty type is created using init_composite_type().
   Fields are then added using append_struct_type_field().  A union
   type has its size set to the largest field.  A struct type has each
   field packed against the previous.  */

extern struct type *init_composite_type(const char *name, enum type_code code);
extern void append_composite_type_field(struct type *t, const char *name,
					struct type *field);

extern struct type *lookup_reference_type(struct type *);

extern struct type *make_reference_type(struct type *, struct type **);

extern struct type *make_cvr_type(int, int, int, struct type *, struct type **);

extern void replace_type(struct type *, struct type *);

extern int address_space_name_to_int(char *);

extern const char *address_space_int_to_name(int);

extern struct type *make_type_with_address_space (struct type *type,
						  int space_identifier);

extern struct type *lookup_member_type (struct type *, struct type *);

extern void
smash_to_method_type (struct type *type, struct type *domain,
		      struct type *to_type, struct field *args,
		      int nargs, int varargs);

extern void smash_to_member_type (struct type *, struct type *, struct type *);

extern struct type *allocate_stub_method (struct type *);

extern const char *type_name_no_tag(const struct type *);

extern struct type *lookup_struct_elt_type(struct type *, const char *, int);

extern struct type *make_pointer_type (struct type *, struct type **);

extern struct type *lookup_pointer_type (struct type *);

/* APPLE LOCAL - Inform users about debugging optimized code  */
extern struct type *make_function_type (struct type *, struct type **, int);

extern struct type *lookup_function_type (struct type *);

extern struct type *create_range_type(struct type *, struct type *, int,
				      int);

extern struct type *create_array_type(struct type *, struct type *,
				      struct type *);

extern struct type *create_string_type(struct type *, struct type *);

extern struct type *create_set_type(struct type *, struct type *);

extern int chill_varying_type(struct type *);

extern struct type *lookup_unsigned_typename(const char *);

extern struct type *lookup_signed_typename(const char *);

extern struct type *check_typedef(struct type *);

#define CHECK_TYPEDEF(TYPE) (TYPE) = check_typedef(TYPE)

extern void check_stub_method_group(struct type *, int);

extern char *gdb_mangle_name (struct type *, int, int);

extern struct type *lookup_typename(const char *, struct block *, int);

extern struct type *lookup_template_type (char *, struct type *,
					  struct block *);

extern struct type *lookup_fundamental_type (struct objfile *, int);

extern void fill_in_vptr_fieldno (struct type *);

extern int get_destructor_fn_field (struct type *, int *, int *);

extern int get_discrete_bounds (struct type *, LONGEST *, LONGEST *);

/* APPLE LOCAL: A special case of get_discrete_bounds, to support alternate array indexing. */
extern int get_array_bounds (struct type *, LONGEST *, LONGEST *, LONGEST *);

extern int is_ancestor (struct type *, struct type *);
/* APPLE LOCAL: Version where we only know the ancestor by name: */
int is_ancestor_by_name (const char *base, struct type *dclass);

extern int has_vtable (struct type *);

extern struct type *primary_base_class (struct type *);

extern struct type **virtual_base_list (struct type *);

extern int virtual_base_list_length (struct type *);
extern int virtual_base_list_length_skip_primaries (struct type *);

extern int virtual_base_index (struct type *, struct type *);
extern int virtual_base_index_skip_primaries (struct type *, struct type *);


extern int class_index_in_primary_list (struct type *);

extern int count_virtual_fns (struct type *);

/* Constants for HP/Taligent ANSI C++ runtime model */

/* Where virtual function entries begin in the
 * virtual table, in the non-RRBC vtable format.
 * First 4 are the metavtable pointer, top offset,
 * typeinfo pointer, and dup base info pointer */
#define HP_ACC_VFUNC_START        4

/* (Negative) Offset where virtual base offset entries begin
 * in the virtual table. Skips over metavtable pointer and
 * the self-offset entry.
 * NOTE: NEGATE THIS BEFORE USING! The virtual base offsets
 * appear before the address point of the vtable (the slot
 * pointed to by the object's vtable pointer), i.e. at lower
 * addresses than the vtable pointer. */
#define HP_ACC_VBASE_START        2

/* (Positive) Offset where the pointer to the typeinfo
 * object is present in the virtual table */
#define HP_ACC_TYPEINFO_OFFSET    2

/* (Positive) Offset where the ``top offset'' entry of
 * the virtual table is */
#define HP_ACC_TOP_OFFSET_OFFSET  1

/* Overload resolution */

#define LENGTH_MATCH(bv) ((bv)->rank[0])

/* Badness if parameter list length doesn't match arg list length */
#define LENGTH_MISMATCH_BADNESS      100
/* Dummy badness value for nonexistent parameter positions */
#define TOO_FEW_PARAMS_BADNESS       100
/* Badness if no conversion among types */
#define INCOMPATIBLE_TYPE_BADNESS    100

/* Badness of integral promotion */
#define INTEGER_PROMOTION_BADNESS      1
/* Badness of floating promotion */
#define FLOAT_PROMOTION_BADNESS        1
/* Badness of integral conversion */
#define INTEGER_CONVERSION_BADNESS     2
/* Badness of floating conversion */
#define FLOAT_CONVERSION_BADNESS       2
/* Badness of integer<->floating conversions */
#define INT_FLOAT_CONVERSION_BADNESS   2
/* Badness of converting to a boolean */
#define BOOLEAN_CONVERSION_BADNESS     2
/* Badness of pointer conversion */
#define POINTER_CONVERSION_BADNESS     2
/* Badness of conversion of pointer to void pointer */
#define VOID_PTR_CONVERSION_BADNESS    2
/* Badness of converting derived to base class */
#define BASE_CONVERSION_BADNESS        2
/* Badness of converting from non-reference to reference */
#define REFERENCE_CONVERSION_BADNESS   2

/* Non-standard conversions allowed by the debugger */
/* Converting a pointer to an int is usually OK */
#define NS_POINTER_CONVERSION_BADNESS 10


extern int compare_badness(struct badness_vector *, struct badness_vector *);

extern struct badness_vector *rank_function(struct type **, int,
					    struct type **, int);

extern int rank_one_type(struct type *, struct type *);

extern const char *type_code_name(int code);

extern void recursive_dump_type(struct type *, int);

/* printcmd.c */

extern void print_scalar_formatted(const void *, struct type *, int, int,
				   struct ui_file *);

extern int can_dereference(struct type *);

extern int is_integral_type(struct type *);

extern void maintenance_print_type(const char *, int);

/* APPLE LOCAL: This is to handle arrays whose element types were undefined
   when the array type was constructed.  */
extern void cleanup_undefined_arrays (void);

/* APPLE LOCAL BEGIN: Helper functions for easily building bitfield
   built in types.  */
extern struct type *build_builtin_enum (const char *name, uint32_t size,
					int flags, struct gdbtypes_enum_info *,
					uint32_t n);

extern struct type *build_builtin_bitfield (const char *name, uint32_t size,
					    struct gdbtypes_bitfield_info *,
					    uint32_t n);
extern struct type *get_closure_dynamic_type (struct value *in_value);
extern struct value *get_closure_implementation_fn (struct value *);
/* APPLE LOCAL END.  */

/* APPLE LOCAL: from parse.c - does this function type have debug info.  */
int ftype_has_debug_info_p (struct type *type);

/* APPLE LOCAL: needed for the MI resolved types.  */
struct type *remove_all_typedefs (struct type *type);

/* Needed for linux-tdep.c: */
extern struct type *init_vector_type(struct type *elt_type, int n);

/* FIXME: the rest of these all need work in gdbtypes.c to get to compile: */

/* Helper functions to construct architecture-owned types.  */
extern struct type *arch_type (struct gdbarch *, enum type_code, int,
			       const char *);
extern struct type *arch_integer_type (struct gdbarch *, int, int,
				       const char *);
extern struct type *arch_character_type (struct gdbarch *, int, int,
					 const char *);
extern struct type *arch_boolean_type (struct gdbarch *, int, int,
				       const char *);
extern struct type *arch_float_type (struct gdbarch *, int, const char *,
				     const struct floatformat **);
extern struct type *arch_decfloat_type (struct gdbarch *, int, const char *);
extern struct type *arch_complex_type (struct gdbarch *, const char *,
				       struct type *);
extern struct type *arch_pointer_type (struct gdbarch *, int, const char *,
				       struct type *);

/* Helper functions to construct a struct or record type.  An
 initially empty type is created using arch_composite_type().
 Fields are then added using append_composite_type_field*().  A union
 type has its size set to the largest field.  A struct type has each
 field packed against the previous.  */

extern struct type *arch_composite_type (struct gdbarch *gdbarch,
					 const char *name, enum type_code code);
extern void append_composite_type_field (struct type *t, const char *name,
					 struct type *field);
extern void append_composite_type_field_aligned (struct type *t,
						 const char *name,
						 struct type *field,
						 int alignment);
struct field *append_composite_type_field_raw (struct type *t, const char *name,
					       struct type *field);

/* Helper functions to construct a bit flags type.  An initially empty
 type is created using arch_flag_type().  Flags are then added using
 append_flag_type_field() and append_flag_type_flag().  */
extern struct type *arch_flags_type(struct gdbarch *gdbarch,
				    const char *name, int length);
extern void append_flags_type_field(struct type *type,
				    int start_bitpos, int nr_bits,
				    struct type *field_type, const char *name);
extern void append_flags_type_flag(struct type *type, int bitpos,
				  const char *name);

/* */
extern int opaque_type_resolution;
extern int use_stride;
extern int overload_debug;

#endif /* GDBTYPES_H */

/* EOF */
