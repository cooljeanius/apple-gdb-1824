/* ansidecl.h: ANSI and traditional C compatability macros
 * Copyright 1991, 1992, 1993, 1994, 1995, 1996, 1998, 1999, 2000, 2001
 * Free Software Foundation, Inc.
 * This file is part of the GNU C Library.  */
/*
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

/* ANSI and traditional C compatibility macros

   ANSI C is assumed if __STDC__ is #defined.

   Macro		ANSI C definition	Traditional C definition
   -----		---- - ----------	----------- - ----------
   ANSI_PROTOTYPES	1			not defined
   PTR			`void *'		`char *'
   PTRCONST		`void *const'		`char *'
   LONG_DOUBLE		`long double'		`double'
   const		not defined		`'
   volatile		not defined		`'
   signed		not defined		`'
   VA_START(ap, var)	va_start(ap, var)	va_start(ap)

   Note that it is safe to write "void foo();" indicating a function
   with no return value, in all K+R compilers we have been able to test.

   For declaring functions with prototypes, we also provide these:

   PARAMS ((prototype))
   -- for functions which take a fixed number of arguments.  Use this
   when declaring the function.  When defining the function, write a
   K+R style argument list.  For example:

	char *strcpy PARAMS ((char *dest, char *source));
	...
	char *
	strcpy (dest, source)
	     char *dest;
	     char *source;
	{ ... }


   VPARAMS ((prototype, ...))
   -- for functions which take a variable number of arguments.  Use
   PARAMS to declare the function, VPARAMS to define it.  For example:

	int printf PARAMS ((const char *format, ...));
	...
	int
	printf VPARAMS ((const char *format, ...))
	{
	   ...
	}

   For writing functions which take variable numbers of arguments, we
   also provide the VA_OPEN, VA_CLOSE, and VA_FIXEDARG macros.  These
   hide the differences between K+R <varargs.h> and C89 <stdarg.h> more
   thoroughly than the simple VA_START() macro mentioned above.

   VA_OPEN and VA_CLOSE are used *instead of* va_start and va_end.
   Immediately after VA_OPEN, put a sequence of VA_FIXEDARG calls
   corresponding to the list of fixed arguments.  Then use va_arg
   normally to get the variable arguments, or pass your va_list object
   around.  You do not declare the va_list yourself; VA_OPEN does it
   for you.

   Here is a complete example:

	int
	printf VPARAMS ((const char *format, ...))
	{
	   int result;

	   VA_OPEN (ap, format);
	   VA_FIXEDARG (ap, const char *, format);

	   result = vfprintf (stdout, format, ap);
	   VA_CLOSE (ap);

	   return result;
	}


   You can declare variables either before or after the VA_OPEN,
   VA_FIXEDARG sequence.  Also, VA_OPEN and VA_CLOSE are the beginning
   and end of a block.  They must appear at the same nesting level,
   and any variables declared after VA_OPEN go out of scope at
   VA_CLOSE.  Unfortunately, with a K+R compiler, that includes the
   argument list.  You can have multiple instances of VA_OPEN/VA_CLOSE
   pairs in a single function in case you need to traverse the
   argument list more than once.

   For ease of writing code which uses GCC extensions but needs to be
   portable to other compilers, we provide the GCC_VERSION macro that
   simplifies testing __GNUC__ and __GNUC_MINOR__ together, and various
   wrappers around __attribute__.  Also, __extension__ will be #defined
   to nothing if it does NOT work.  See below.

   This header also defines a lot of obsolete macros:
   CONST, VOLATILE, SIGNED, PROTO, EXFUN, DEFUN, DEFUN_VOID,
   AND, DOTS, NOARGS.  Do NOT use them.  */

#ifndef	_ANSIDECL_H
#define _ANSIDECL_H 1

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if defined(__GNUC__) && defined(__GNUC_MINOR__) && !defined(__cplusplus)
# if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6))
 #  pragma GCC diagnostic push
 #  pragma GCC diagnostic warning "-Wtraditional"
# endif /* gcc 4.6+ */
#endif /* GCC (non-c++) */

/* Every source file includes this file,
   so they will all get the switch for lint.  */
/* LINTLIBRARY */

/* Using MACRO(x,y) in cpp #if conditionals does not work with some
   older preprocessors.  Thus we cannot define something like this:

#define HAVE_GCC_VERSION(MAJOR, MINOR) \
  (__GNUC__ > (MAJOR) || (__GNUC__ == (MAJOR) && __GNUC_MINOR__ >= (MINOR)))

and then test "#if HAVE_GCC_VERSION(2,7)".

So instead we use the macro below and test it against specific values.  */

/* This macro simplifies testing whether we are using gcc, and if it
   is of a particular minimum version. (Both major & minor numbers are
   significant.)  This macro will evaluate to 0 if we are not using
   gcc at all.  */
#ifndef GCC_VERSION
# define GCC_VERSION (__GNUC__ * 1000 + __GNUC_MINOR__)
#endif /* GCC_VERSION */

#if defined(__STDC__) || defined(_AIX) || (defined(__mips) && defined(_SYSTYPE_SVR4)) || defined(_WIN32) || (defined(__alpha) && defined(__cplusplus))
/* All known AIX compilers implement these things (but do NOT always
   define __STDC__).  The RISC/OS MIPS compiler defines these things
   in SVR4 mode, but does not define __STDC__.  */
/* <eraxxon@alumni.rice.edu>: The Compaq C++ compiler, unlike many other
   C++ compilers, does not define __STDC__, though it acts as if this
   was so. (Verified versions: 5.7, 6.2, 6.3, 6.5) */

#define ANSI_PROTOTYPES	1
#define PTR		void *
#define PTRCONST	void *const
#define LONG_DOUBLE	long double

/* PARAMS is often defined elsewhere (e.g. by libintl.h), so wrap it in
   a #ifndef.  */
#ifndef PARAMS
# define PARAMS(ARGS)		ARGS
#endif /* !PARAMS */

#define VPARAMS(ARGS)		ARGS
#define VA_START(VA_LIST, VAR)	va_start(VA_LIST, VAR)

/* variadic function helper macros */
/* "struct Qdmy" swallows the semicolon after VA_OPEN/VA_FIXEDARG's
   use without inhibiting further decls and without declaring an
   actual variable.  */
#define VA_OPEN(AP, VAR)	{ va_list AP; va_start(AP, VAR); { struct Qdmy
#define VA_CLOSE(AP)		} va_end(AP); }
#define VA_FIXEDARG(AP, T, N)	struct Qdmy

#undef const
#undef volatile
#undef signed

/* inline requires special treatment; it is in C99, and GCC >=2.7 supports
 * it too, but it is not in C89.  */
#undef inline
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ > 199901L)
/* it is a keyword */
#else
# if defined(GCC_VERSION) && (GCC_VERSION >= 2007)
#  define inline __inline__   /* __inline__ prevents -pedantic warnings */
# else
#  define inline  /* nothing */
# endif /* gcc 2.7+ */
#endif /* C99 || GCC */

/* These are obsolete.  Do not use them: */
#ifndef IN_GCC
#  define CONST		const
# define VOLATILE	volatile
# define SIGNED		signed

# define PROTO(type, name, arglist)	type name arglist
# define EXFUN(name, proto)		name proto
# define DEFUN(name, arglist, args)	name(args)
# define DEFUN_VOID(name)		name(void)
# define AND		,
# define DOTS		, ...
# define NOARGS		void
#endif /* ! IN_GCC */

#else	/* Not ANSI C: */

#undef  ANSI_PROTOTYPES
#define PTR		char *
#define PTRCONST	PTR
#define LONG_DOUBLE	double

#define PARAMS(args)		()
#define VPARAMS(args)		(va_alist) va_dcl
#define VA_START(va_list, var)	va_start(va_list)

#define VA_OPEN(AP, VAR)		{ va_list AP; va_start(AP); { struct Qdmy
#define VA_CLOSE(AP)			} va_end(AP); }
#define VA_FIXEDARG(AP, TYPE, NAME)	TYPE NAME = va_arg(AP, TYPE)

/* some systems define these in header files for non-ansi mode */
#undef const
#undef volatile
#undef signed
#undef inline
#define const
#define volatile
#define signed
#define inline

#ifndef IN_GCC
# define CONST
# define VOLATILE
# define SIGNED

# define PROTO(type, name, arglist)	type name ()
# define EXFUN(name, proto)		name()
# define DEFUN(name, arglist, args)	name arglist args;
# define DEFUN_VOID(name)		name()
# define AND		;
# define DOTS
# define NOARGS
#endif /* ! IN_GCC */

#endif	/* ANSI C.  */

/* Define macros for some gcc attributes.  This permits us to use the
   macros freely, and know that they will come into play for the
   version of gcc in which they are supported.  */

#if (GCC_VERSION < 2007)
# define __attribute__(x)
#endif /* gcc pre-2.7 */

#if !defined __has_attribute
# define __has_attribute(x) 0
#endif

/* Attribute __malloc__ on functions was valid as of gcc 2.96. */
#ifndef ATTRIBUTE_MALLOC
# if (GCC_VERSION >= 2096) || __has_attribute(__malloc__)
#  define ATTRIBUTE_MALLOC __attribute__((__malloc__))
# else
#  define ATTRIBUTE_MALLOC
# endif /* GNUC >= 2.96 */
#endif /* ATTRIBUTE_MALLOC */

/* Attributes on labels were valid as of gcc 2.93 and g++ 4.5.  For
 * g++ an attribute on a label must be followed by a semicolon: */
#ifndef ATTRIBUTE_UNUSED_LABEL
# ifndef __cplusplus
#  if (GCC_VERSION >= 2093) || __has_attribute(__unused__)
#   define ATTRIBUTE_UNUSED_LABEL ATTRIBUTE_UNUSED
#  else
#   define ATTRIBUTE_UNUSED_LABEL
#  endif /* gcc 2.93+ */
# else /* __cplusplus: */
#  if (GCC_VERSION >= 4005) || __has_attribute(__unused__)
#   define ATTRIBUTE_UNUSED_LABEL ATTRIBUTE_UNUSED ;
#  else
#   define ATTRIBUTE_UNUSED_LABEL
#  endif /* gcc 4.5+ */
# endif /* !__cplusplus */
#endif /* !ATTRIBUTE_UNUSED_LABEL */

/* FIXME: verify the gcc versions in which this is available: */
#ifndef ATTRIBUTE_DEPRECATED
# if (GCC_VERSION >= 3002) || __has_attribute(__deprecated__)
#  define ATTRIBUTE_DEPRECATED __attribute__((__deprecated__))
# else
#  define ATTRIBUTE_DEPRECATED
# endif /* gcc 3.2+ */
#endif /* ATTRIBUTE_DEPRECATED */

/* This part is based on <glib-2.0/glib/gmacros.h>: */
#ifndef ATTRIBUTE_DEPRECATED_FOR
# if (GCC_VERSION >= 4005) || __has_attribute(__deprecated__)
#  define ATTRIBUTE_DEPRECATED_FOR(f) __attribute__((__deprecated__("Use '" #f "' instead")))
# else
#  if defined(_MSC_FULL_VER) && (_MSC_FULL_VER > 140050320)
#   define ATTRIBUTE_DEPRECATED_FOR(f) __declspec(deprecated("is deprecated. Use '" #f "' instead"))
#  else
#   define ATTRIBUTE_DEPRECATED_FOR(f) ATTRIBUTE_DEPRECATED
#  endif /* MSVC */
# endif /* gcc 4.5+ */
#endif /* ATTRIBUTE_DEPRECATED_FOR */


#ifndef ATTRIBUTE_GNU_INLINE
# if (defined(__GNUC__) && defined(__GNUC_VERSION__)) || \
     __has_attribute(__gnu_inline__)
#  define ATTRIBUTE_GNU_INLINE __attribute__((__gnu_inline__))
# else
#  define ATTRIBUTE_GNU_INLINE /* (nothing) */
# endif /* __GNUC__ && __GNUC_VERSION__ */
#endif /* !ATTRIBUTE_GNU_INLINE */

/* Similarly to ARG_UNUSED below.  Prior to GCC 3.4, the C++ frontend
   could NOT parse attributes placed after the identifier name, and now
   the entire compiler is built with C++.  */
#ifndef ATTRIBUTE_UNUSED
# if (GCC_VERSION >= 3004) || __has_attribute(__unused__)
#  define ATTRIBUTE_UNUSED __attribute__((__unused__))
# else
#  define ATTRIBUTE_UNUSED
# endif /* gcc 3.4+ */
#endif /* ATTRIBUTE_UNUSED */

/* Similar to the previous: */
#ifndef ATTRIBUTE_USED
# if (GCC_VERSION >= 3004) || __has_attribute(__used__)
#  define ATTRIBUTE_USED __attribute__((__used__))
# else
#  define ATTRIBUTE_USED
# endif /* gcc 3.4+ */
#endif /* ATTRIBUTE_USED */

/* Before GCC 3.4, the C++ frontend could NOT parse attributes placed after
 * the identifier name: */
#if ! defined(__cplusplus) || (GCC_VERSION >= 3004)
# define ARG_UNUSED(NAME) NAME ATTRIBUTE_UNUSED
#else /* !__cplusplus || GNUC >= 3.4 */
# define ARG_UNUSED(NAME) NAME
#endif /* !__cplusplus || GNUC >= 3.4 */

#ifndef ATTRIBUTE_NOCLONE
# if (GCC_VERSION >= 4005) || __has_attribute(__noclone__)
#  define ATTRIBUTE_NOCLONE __attribute__((__noclone__))
# else
#  define ATTRIBUTE_NOCLONE /* (nothing) */
# endif /* gcc 4.5+ */
#endif /* !ATTRIBUTE_NOCLONE */

#ifndef ATTRIBUTE_NOINLINE
# define ATTRIBUTE_NOINLINE __attribute__((__noinline__))
#endif /* !ATTRIBUTE_NOINLINE */

#ifndef ATTRIBUTE_NOIPA
# if (GCC_VERSION >= 8000) || __has_attribute(__noipa__)
#  define ATTRIBUTE_NOIPA __attribute__((__noipa__))
# else
#  define ATTRIBUTE_NOIPA ATTRIBUTE_NOINLINE ATTRIBUTE_NOCLONE
# endif /* gcc 8+ */
#endif /* !ATTRIBUTE_NOIPA */

#ifndef ATTRIBUTE_NORETURN
# define ATTRIBUTE_NORETURN __attribute__((__noreturn__))
#endif /* ATTRIBUTE_NORETURN */

/* Attribute `nonnull' was valid as of gcc 3.3.  */
#ifndef ATTRIBUTE_NONNULL
# if (GCC_VERSION >= 3003) || __has_attribute(__nonnull__)
#  define ATTRIBUTE_NONNULL(m) __attribute__((__nonnull__(m)))
# else
#  define ATTRIBUTE_NONNULL(m)
# endif /* GNUC >= 3.3 */
#endif /* ATTRIBUTE_NONNULL */

/* Attribute `returns_nonnull' was valid as of gcc 4.9 */
#ifndef ATTRIBUTE_RETURNS_NONNULL
# if (GCC_VERSION >= 4009) || __has_attribute(returns_nonnull)
#  define ATTRIBUTE_RETURNS_NONNULL __attribute__((returns_nonnull))
# else
#  define ATTRIBUTE_RETURNS_NONNULL
# endif /* GNUC >= 4.9 */
#endif /* ATTRIBUTE_RETURNS_NONNULL */

/* Attribute `pure' was valid as of gcc 3.0.  */
#ifndef ATTRIBUTE_PURE
# if (GCC_VERSION >= 3000) || __has_attribute(__pure__)
#  define ATTRIBUTE_PURE __attribute__((__pure__))
# else
#  define ATTRIBUTE_PURE
# endif /* GNUC >= 3.0 */
#endif /* ATTRIBUTE_PURE */

/* Assume likewise about attribute `const': */
#ifndef ATTRIBUTE_CONST
# if (GCC_VERSION >= 3000) || __has_attribute(__const__)
#  define ATTRIBUTE_CONST __attribute__((__const__))
# else
#  define ATTRIBUTE_CONST
# endif /* GNUC >= 3.0 */
#endif /* ATTRIBUTE_CONST */

/* FIXME: add version check: */
#ifndef ATTRIBUTE_FORMAT_ARG
# define ATTRIBUTE_FORMAT_ARG(arg) __attribute__((__format_arg__(arg)))
#endif /* !ATTRIBUTE_FORMAT_ARG */

/* Use ATTRIBUTE_PRINTF when the format specifier must not be NULL.
   This was the case for the `printf' format attribute by itself
   before GCC 3.3, but as of 3.3 we need to add the `nonnull'
   attribute to retain this behavior.  */
#ifndef ATTRIBUTE_PRINTF
#define ATTRIBUTE_PRINTF(m, n) __attribute__((__format__(__printf__, m, n))) ATTRIBUTE_NONNULL(m)
#define ATTRIBUTE_PRINTF_1 ATTRIBUTE_PRINTF(1, 2)
#define ATTRIBUTE_PRINTF_2 ATTRIBUTE_PRINTF(2, 3)
#define ATTRIBUTE_PRINTF_3 ATTRIBUTE_PRINTF(3, 4)
#define ATTRIBUTE_PRINTF_4 ATTRIBUTE_PRINTF(4, 5)
#define ATTRIBUTE_PRINTF_5 ATTRIBUTE_PRINTF(5, 6)
#endif /* ATTRIBUTE_PRINTF */

/* Use ATTRIBUTE_FPTR_PRINTF when the format attribute is to be set on
   a function pointer.  Format attributes were allowed on function
   pointers as of gcc 3.1.  */
#ifndef ATTRIBUTE_FPTR_PRINTF
# if (GCC_VERSION >= 3001) || __has_attribute(__format__)
#  define ATTRIBUTE_FPTR_PRINTF(m, n) ATTRIBUTE_PRINTF(m, n)
# else
#  define ATTRIBUTE_FPTR_PRINTF(m, n)
# endif /* GNUC >= 3.1 */
# define ATTRIBUTE_FPTR_PRINTF_1 ATTRIBUTE_FPTR_PRINTF(1, 2)
# define ATTRIBUTE_FPTR_PRINTF_2 ATTRIBUTE_FPTR_PRINTF(2, 3)
# define ATTRIBUTE_FPTR_PRINTF_3 ATTRIBUTE_FPTR_PRINTF(3, 4)
# define ATTRIBUTE_FPTR_PRINTF_4 ATTRIBUTE_FPTR_PRINTF(4, 5)
# define ATTRIBUTE_FPTR_PRINTF_5 ATTRIBUTE_FPTR_PRINTF(5, 6)
#endif /* ATTRIBUTE_FPTR_PRINTF */

/* Use ATTRIBUTE_NULL_PRINTF when the format specifier may be NULL.  A
   NULL format specifier was allowed as of gcc 3.3.  */
#ifndef ATTRIBUTE_NULL_PRINTF
# if (GCC_VERSION >= 3003) || __has_attribute(__format__)
#  define ATTRIBUTE_NULL_PRINTF(m, n) __attribute__((__format__(__printf__, m, n)))
# else
#  define ATTRIBUTE_NULL_PRINTF(m, n)
# endif /* GNUC >= 3.3 */
# define ATTRIBUTE_NULL_PRINTF_1 ATTRIBUTE_NULL_PRINTF(1, 2)
# define ATTRIBUTE_NULL_PRINTF_2 ATTRIBUTE_NULL_PRINTF(2, 3)
# define ATTRIBUTE_NULL_PRINTF_3 ATTRIBUTE_NULL_PRINTF(3, 4)
# define ATTRIBUTE_NULL_PRINTF_4 ATTRIBUTE_NULL_PRINTF(4, 5)
# define ATTRIBUTE_NULL_PRINTF_5 ATTRIBUTE_NULL_PRINTF(5, 6)
#endif /* ATTRIBUTE_NULL_PRINTF */

/* Attribute `sentinel' was valid as of gcc 3.5.  */
#ifndef ATTRIBUTE_SENTINEL
# if (GCC_VERSION >= 3005) || __has_attribute(__sentinel__)
#  define ATTRIBUTE_SENTINEL __attribute__((__sentinel__))
# else
#  define ATTRIBUTE_SENTINEL
# endif /* GNUC >= 3.5 */
#endif /* ATTRIBUTE_SENTINEL */


#ifndef ATTRIBUTE_ALIGNED_ALIGNOF
# if (GCC_VERSION >= 3000) || __has_attribute(__aligned__)
#  define ATTRIBUTE_ALIGNED_ALIGNOF(m) __attribute__((__aligned__(__alignof__(m))))
# else
#  define ATTRIBUTE_ALIGNED_ALIGNOF(m)
# endif /* GNUC >= 3.0 */
#endif /* ATTRIBUTE_ALIGNED_ALIGNOF */

/* Useful for structures whose layout must much some binary specification
   regardless of the alignment and padding qualities of the compiler.  */
#ifndef ATTRIBUTE_PACKED
# define ATTRIBUTE_PACKED __attribute__((packed))
#endif /* ATTRIBUTE_PACKED */

/* Attribute `hot' and `cold' was valid as of gcc 4.3.  */
#ifndef ATTRIBUTE_COLD
# if (GCC_VERSION >= 4003) || __has_attribute(__cold__)
#  define ATTRIBUTE_COLD __attribute__((__cold__))
# else
#  define ATTRIBUTE_COLD
# endif /* GNUC >= 4.3 */
#endif /* ATTRIBUTE_COLD */
#ifndef ATTRIBUTE_HOT
# if (GCC_VERSION >= 4003) || __has_attribute(__hot__)
#  define ATTRIBUTE_HOT __attribute__((__hot__))
# else
#  define ATTRIBUTE_HOT
# endif /* GNUC >= 4.3 */
#endif /* ATTRIBUTE_HOT */

/* Attribute 'no_sanitize_undefined' was valid as of gcc 4.9.  */
#ifndef ATTRIBUTE_NO_SANITIZE_UNDEFINED
# if (GCC_VERSION >= 4009) || __has_attribute(no_sanitize_undefined)
#  define ATTRIBUTE_NO_SANITIZE_UNDEFINED __attribute__((no_sanitize_undefined))
# else
#  define ATTRIBUTE_NO_SANITIZE_UNDEFINED
# endif /* GNUC >= 4.9 */
#endif /* ATTRIBUTE_NO_SANITIZE_UNDEFINED */

/* Attribute 'warn_unused_result' was valid as of gcc 3.3.  */
#ifndef ATTRIBUTE_W_U_R
# if (GCC_VERSION >= 3003) || __has_attribute(warn_unused_result)
#  define ATTRIBUTE_W_U_R __attribute__((warn_unused_result))
# else
#  define ATTRIBUTE_W_U_R
# endif /* gcc 3.3+ */
#endif /* ATTRIBUTE_W_U_R */

/* Added in gcc 7: */
#ifndef ATTRIBUTE_FALLTHROUGH
# if (GCC_VERSION >= 7000) || __has_attribute(fallthrough)
#  define ATTRIBUTE_FALLTHROUGH __attribute__((fallthrough))
# else
#  define ATTRIBUTE_FALLTHROUGH /* FALLTHRU */
# endif /* gcc 7+ */
#endif /* !ATTRIBUTE_FALLTHROUGH */

/* Attribute 'nonstring' was valid as of gcc 8.  */
#ifndef ATTRIBUTE_NONSTRING
# if (GCC_VERSION >= 8000) || __has_attribute(__nonstring__)
#  define ATTRIBUTE_NONSTRING __attribute__ ((__nonstring__))
# else
#  define ATTRIBUTE_NONSTRING
# endif /* gcc 8+ */
#endif /* !ATTRIBUTE_NONSTRING */

/* Attribute 'alloc_size' was valid as of gcc 4.3.  */
#ifndef ATTRIBUTE_RESULT_SIZE_1
# if (GCC_VERSION >= 4003) || __has_attribute(alloc_size)
#  define ATTRIBUTE_RESULT_SIZE_1 __attribute__((alloc_size(1)))
# else
#  define ATTRIBUTE_RESULT_SIZE_1
# endif /* gcc 4.3+ */
#endif /* !ATTRIBUTE_RESULT_SIZE_1 */

#ifndef ATTRIBUTE_RESULT_SIZE_2
# if (GCC_VERSION >= 4003) || __has_attribute(alloc_size)
#  define ATTRIBUTE_RESULT_SIZE_2 __attribute__((alloc_size(2)))
# else
#  define ATTRIBUTE_RESULT_SIZE_2
# endif /* gcc 4.3+ */
#endif /* !ATTRIBUTE_RESULT_SIZE_2 */

#ifndef ATTRIBUTE_RESULT_SIZE_1_2
# if (GCC_VERSION >= 4003) || __has_attribute(alloc_size)
#  define ATTRIBUTE_RESULT_SIZE_1_2 __attribute__((alloc_size(1, 2)))
# else
#  define ATTRIBUTE_RESULT_SIZE_1_2
# endif /* gcc 4.3+ */
#endif /* !ATTRIBUTE_RESULT_SIZE_1_2 */

/* Added in gcc 13: */
#ifndef ATTRIBUTE_STRICT_FLEX_ARRAY
# if (GCC_VERSION >= 13000) || __has_attribute(strict_flex_array)
#  define ATTRIBUTE_STRICT_FLEX_ARRAY(n) __attribute__((strict_flex_array(n)))
# else
#  define ATTRIBUTE_STRICT_FLEX_ARRAY(n)
# endif /* gcc 13+ */
#endif /* !ATTRIBUTE_STRICT_FLEX_ARRAY */

/* We use __extension__ in some places to suppress -pedantic warnings about
 * GCC extensions. This feature did NOT work properly before gcc 2.8. */
#if GCC_VERSION < 2008
# define __extension__
#endif /* gcc pre-2.8 */

/* This is used to declare a const variable which should be visible
   outside of the current compilation unit.  Use it as
   EXPORTED_CONST int i = 1;
   This is because the semantics of const are different in C and C++.
   "extern const" is permitted in C but it looks strange, and gcc
   warns about it when -Wc++-compat is not used.  */
#ifdef __cplusplus
# define EXPORTED_CONST extern const
#else
# define EXPORTED_CONST const
#endif /* __cplusplus */

/* Be conservative and only use enum bitfields with C++ or GCC.
 * FIXME: provide a complete autoconf test for buggy enum bitfields.  */
#ifdef __cplusplus
# define ENUM_BITFIELD(TYPE) enum TYPE
#else
# if (GCC_VERSION > 2000)
#  define ENUM_BITFIELD(TYPE) __extension__ enum TYPE
# else
#  define ENUM_BITFIELD(TYPE) unsigned int
# endif /* gcc 2+ */
#endif /* __cplusplus */

#ifndef CONSTEXPR
# if defined(__cpp_constexpr) && (__cpp_constexpr >= 200704)
#  define CONSTEXPR constexpr
# else
#  define CONSTEXPR
# endif /* can use constexpr */
#endif /* !CONSTEXPR */

/* C++11 adds the ability to add "override" after an implementation of a
   virtual function in a subclass, to:
     (A) document that this is an override of a virtual function
     (B) allow the compiler to issue a warning if it isn't (e.g. a mismatch
         of the type signature).

   Similarly, it allows us to add a "final" to indicate that no subclass
   may subsequently override the vfunc.

   Provide OVERRIDE and FINAL as macros, allowing us to get these benefits
   when compiling with C++11 support, but without requiring C++11.

   For gcc, use "-std=c++11" to enable C++11 support; gcc 6 onwards enables
   this by default (actually GNU++14).  */

#if defined __cplusplus
# if (__cplusplus >= 201103)
   /* C++11 claims to be available: use it.  Final/override were only
      implemented in 4.7, though.  */
#  if GCC_VERSION < 4007
#   define OVERRIDE
#   define FINAL
#  else
#   define OVERRIDE override
#   define FINAL final
#  endif /* GCC pre-4.7 */
# else
#  if GCC_VERSION >= 4007
   /* G++ 4.7 supports __final in C++98.  */
#   define OVERRIDE
#   define FINAL __final
#  else
   /* No C++11 support; leave the macros empty.  */
#   define OVERRIDE
#   define FINAL
#  endif /* GCC 4.7+ */
# endif /* C++11 */
#else
  /* No C++11 support; leave the macros empty.  */
# define OVERRIDE
# define FINAL
#endif /* C++ */

/* A macro to disable the copy constructor and assignment operator.
   When building with C++11 and above, the methods are explicitly
   deleted, causing a compile-time error if something tries to copy.
   For C++03, this just declares the methods, causing a link-time
   error if the methods end up called (assuming you don't
   define them).  For C++03, for best results, place the macro
   under the private: access specifier, like this,

   class name_lookup
   {
     private:
       DISABLE_COPY_AND_ASSIGN (name_lookup);
   };

   so that most attempts at copy are caught at compile-time.  */

#if defined(__cplusplus)
# if __cplusplus >= 201103
#  define DISABLE_COPY_AND_ASSIGN(TYPE)		\
    TYPE (const TYPE&) = delete;		\
    void operator= (const TYPE &) = delete
# else
#  define DISABLE_COPY_AND_ASSIGN(TYPE)		\
    TYPE (const TYPE&);				\
    void operator= (const TYPE &)
# endif /* __cplusplus >= 201103 */
#endif /* __cplusplus */

/* keep condition the same as where we push: */
#if defined(__GNUC__) && defined(__GNUC_MINOR__) && !defined(__cplusplus)
# if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6))
 #  pragma GCC diagnostic pop
# endif /* gcc 4.6+ */
#endif /* GCC (non-c++) */

/* in case the popping failed: */
#if defined(__GNUC__) && (__GNUC__ >= 4) && defined(__GNUC_MINOR__) && \
    (__GNUC_MINOR__ >= 2) && !defined(__clang__) && !defined(__cplusplus)
 # pragma GCC diagnostic ignored "-Wtraditional"
#endif /* gcc 4.2+ && !__clang__ && !__cplusplus */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	/* ansidecl.h	*/

/* EOF */
