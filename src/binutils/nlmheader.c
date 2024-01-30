/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "nlmheader.y"
/* nlmheader.y - parse NLM header specification keywords.
     Copyright 1993, 1994, 1995, 1997, 1998, 2001, 2002, 2003
     Free Software Foundation, Inc.

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
Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.  */

/* Written by Ian Lance Taylor <ian@cygnus.com>.

   This bison file parses the commands recognized by the NetWare NLM
   linker, except for lists of object files.  It stores the
   information in global variables.

   This implementation is based on the description in the NetWare Tool
   Maker Specification manual, edition 1.0.  */

#include "ansidecl.h"
#include <stdio.h>
#include "safe-ctype.h"
#include "bfd.h"
#include "bucomm.h"
#include "nlm/common.h"
#include "nlm/internal.h"
#include "nlmconv.h"

/* Information is stored in the structures pointed to by these
   variables.  */

Nlm_Internal_Fixed_Header *fixed_hdr;
Nlm_Internal_Variable_Header *var_hdr;
Nlm_Internal_Version_Header *version_hdr;
Nlm_Internal_Copyright_Header *copyright_hdr;
Nlm_Internal_Extended_Header *extended_hdr;

/* Procedure named by CHECK.  */
char *check_procedure;
/* File named by CUSTOM.  */
char *custom_file;
/* Whether to generate debugging information (DEBUG).  */
bfd_boolean debug_info;
/* Procedure named by EXIT.  */
const char *exit_procedure;
/* Exported symbols (EXPORT).  */
struct string_list *export_symbols;
/* List of files from INPUT.  */
struct string_list *input_files;
/* Map file name (MAP, FULLMAP).  */
const char *map_file;
/* Whether a full map has been requested (FULLMAP).  */
bfd_boolean full_map;
/* File named by HELP.  */
char *help_file;
/* Imported symbols (IMPORT).  */
struct string_list *import_symbols;
/* File named by MESSAGES.  */
char *message_file;
/* Autoload module list (MODULE).  */
struct string_list *modules;
/* File named by OUTPUT.  */
char *output_file;
/* File named by SHARELIB.  */
char *sharelib_file;
/* Start procedure name (START).  */
const char *start_procedure;
/* VERBOSE.  */
bfd_boolean verbose;
/* RPC description file (XDCDATA).  */
char *rpc_file;

/* The number of serious errors that have occurred.  */
int parse_errors;

/* The current symbol prefix when reading a list of import or export
   symbols.  */
static char *symbol_prefix;

/* Parser error message handler.  */
#define yyerror(msg) nlmheader_error(msg);

/* Local functions.  */
static int yylex(void);
static void nlmlex_file_push(const char *);
static bfd_boolean nlmlex_file_open(const char *);
static int nlmlex_buf_init(void);
static char nlmlex_buf_add(int);
static long nlmlex_get_number(const char *);
static void nlmheader_identify(void);
static void nlmheader_warn(const char *, int);
static void nlmheader_error(const char *);
static struct string_list *string_list_cons(char *, struct string_list *);
static struct string_list *string_list_append(struct string_list *,
                                              struct string_list *);
static struct string_list *string_list_append1(struct string_list *, char *);
static char *xstrdup(const char *);


#line 181 "nlmheader.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_NLMHEADER_H_INCLUDED
# define YY_YY_NLMHEADER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    CHECK = 258,                   /* CHECK  */
    CODESTART = 259,               /* CODESTART  */
    COPYRIGHT = 260,               /* COPYRIGHT  */
    CUSTOM = 261,                  /* CUSTOM  */
    DATE = 262,                    /* DATE  */
    DEBUG = 263,                   /* DEBUG  */
    DESCRIPTION = 264,             /* DESCRIPTION  */
    EXIT = 265,                    /* EXIT  */
    EXPORT = 266,                  /* EXPORT  */
    FLAG_ON = 267,                 /* FLAG_ON  */
    FLAG_OFF = 268,                /* FLAG_OFF  */
    FULLMAP = 269,                 /* FULLMAP  */
    HELP = 270,                    /* HELP  */
    IMPORT = 271,                  /* IMPORT  */
    INPUT = 272,                   /* INPUT  */
    MAP = 273,                     /* MAP  */
    MESSAGES = 274,                /* MESSAGES  */
    MODULE = 275,                  /* MODULE  */
    MULTIPLE = 276,                /* MULTIPLE  */
    OS_DOMAIN = 277,               /* OS_DOMAIN  */
    OUTPUT = 278,                  /* OUTPUT  */
    PSEUDOPREEMPTION = 279,        /* PSEUDOPREEMPTION  */
    REENTRANT = 280,               /* REENTRANT  */
    SCREENNAME = 281,              /* SCREENNAME  */
    SHARELIB = 282,                /* SHARELIB  */
    STACK = 283,                   /* STACK  */
    START = 284,                   /* START  */
    SYNCHRONIZE = 285,             /* SYNCHRONIZE  */
    THREADNAME = 286,              /* THREADNAME  */
    TYPE = 287,                    /* TYPE  */
    VERBOSE = 288,                 /* VERBOSE  */
    VERSIONK = 289,                /* VERSIONK  */
    XDCDATA = 290,                 /* XDCDATA  */
    STRING = 291,                  /* STRING  */
    QUOTED_STRING = 292            /* QUOTED_STRING  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 112 "nlmheader.y"

  char *string;
  struct string_list *list;

#line 273 "nlmheader.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_NLMHEADER_H_INCLUDED  */
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_CHECK = 3,                      /* CHECK  */
  YYSYMBOL_CODESTART = 4,                  /* CODESTART  */
  YYSYMBOL_COPYRIGHT = 5,                  /* COPYRIGHT  */
  YYSYMBOL_CUSTOM = 6,                     /* CUSTOM  */
  YYSYMBOL_DATE = 7,                       /* DATE  */
  YYSYMBOL_DEBUG = 8,                      /* DEBUG  */
  YYSYMBOL_DESCRIPTION = 9,                /* DESCRIPTION  */
  YYSYMBOL_EXIT = 10,                      /* EXIT  */
  YYSYMBOL_EXPORT = 11,                    /* EXPORT  */
  YYSYMBOL_FLAG_ON = 12,                   /* FLAG_ON  */
  YYSYMBOL_FLAG_OFF = 13,                  /* FLAG_OFF  */
  YYSYMBOL_FULLMAP = 14,                   /* FULLMAP  */
  YYSYMBOL_HELP = 15,                      /* HELP  */
  YYSYMBOL_IMPORT = 16,                    /* IMPORT  */
  YYSYMBOL_INPUT = 17,                     /* INPUT  */
  YYSYMBOL_MAP = 18,                       /* MAP  */
  YYSYMBOL_MESSAGES = 19,                  /* MESSAGES  */
  YYSYMBOL_MODULE = 20,                    /* MODULE  */
  YYSYMBOL_MULTIPLE = 21,                  /* MULTIPLE  */
  YYSYMBOL_OS_DOMAIN = 22,                 /* OS_DOMAIN  */
  YYSYMBOL_OUTPUT = 23,                    /* OUTPUT  */
  YYSYMBOL_PSEUDOPREEMPTION = 24,          /* PSEUDOPREEMPTION  */
  YYSYMBOL_REENTRANT = 25,                 /* REENTRANT  */
  YYSYMBOL_SCREENNAME = 26,                /* SCREENNAME  */
  YYSYMBOL_SHARELIB = 27,                  /* SHARELIB  */
  YYSYMBOL_STACK = 28,                     /* STACK  */
  YYSYMBOL_START = 29,                     /* START  */
  YYSYMBOL_SYNCHRONIZE = 30,               /* SYNCHRONIZE  */
  YYSYMBOL_THREADNAME = 31,                /* THREADNAME  */
  YYSYMBOL_TYPE = 32,                      /* TYPE  */
  YYSYMBOL_VERBOSE = 33,                   /* VERBOSE  */
  YYSYMBOL_VERSIONK = 34,                  /* VERSIONK  */
  YYSYMBOL_XDCDATA = 35,                   /* XDCDATA  */
  YYSYMBOL_STRING = 36,                    /* STRING  */
  YYSYMBOL_QUOTED_STRING = 37,             /* QUOTED_STRING  */
  YYSYMBOL_38_ = 38,                       /* '('  */
  YYSYMBOL_39_ = 39,                       /* ')'  */
  YYSYMBOL_YYACCEPT = 40,                  /* $accept  */
  YYSYMBOL_file = 41,                      /* file  */
  YYSYMBOL_commands = 42,                  /* commands  */
  YYSYMBOL_command = 43,                   /* command  */
  YYSYMBOL_44_1 = 44,                      /* $@1  */
  YYSYMBOL_45_2 = 45,                      /* $@2  */
  YYSYMBOL_symbol_list_opt = 46,           /* symbol_list_opt  */
  YYSYMBOL_symbol_list = 47,               /* symbol_list  */
  YYSYMBOL_symbol_prefix = 48,             /* symbol_prefix  */
  YYSYMBOL_symbol = 49,                    /* symbol  */
  YYSYMBOL_string_list = 50                /* string_list  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  64
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   73

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  40
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  11
/* YYNRULES -- Number of rules.  */
#define YYNRULES  52
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  82

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   292


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      38,    39,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   143,   143,   148,   150,   156,   160,   165,   182,   186,
     208,   212,   228,   233,   232,   240,   245,   250,   255,   260,
     265,   264,   272,   276,   280,   284,   288,   292,   296,   300,
     307,   311,   315,   331,   335,   340,   344,   348,   364,   369,
     373,   401,   419,   429,   432,   443,   447,   451,   455,   464,
     475,   493,   496
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "CHECK", "CODESTART",
  "COPYRIGHT", "CUSTOM", "DATE", "DEBUG", "DESCRIPTION", "EXIT", "EXPORT",
  "FLAG_ON", "FLAG_OFF", "FULLMAP", "HELP", "IMPORT", "INPUT", "MAP",
  "MESSAGES", "MODULE", "MULTIPLE", "OS_DOMAIN", "OUTPUT",
  "PSEUDOPREEMPTION", "REENTRANT", "SCREENNAME", "SHARELIB", "STACK",
  "START", "SYNCHRONIZE", "THREADNAME", "TYPE", "VERBOSE", "VERSIONK",
  "XDCDATA", "STRING", "QUOTED_STRING", "'('", "')'", "$accept", "file",
  "commands", "command", "$@1", "$@2", "symbol_list_opt", "symbol_list",
  "symbol_prefix", "symbol", "string_list", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-20)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
      -3,    -1,     1,     2,     4,     5,   -20,     6,     8,   -20,
       9,    10,    11,    12,   -20,    13,    14,    16,    13,   -20,
     -20,    17,   -20,   -20,    18,    20,    21,    22,   -20,    23,
      25,   -20,    26,    27,    38,   -20,    -3,   -20,   -20,   -20,
     -20,    28,   -20,   -20,    -2,   -20,   -20,   -20,   -20,    -2,
      13,   -20,   -20,   -20,   -20,   -20,   -20,   -20,   -20,   -20,
     -20,   -20,    30,   -20,   -20,   -20,    31,   -20,    32,   -20,
      -2,   -20,   -20,   -20,   -20,    33,   -20,     3,   -20,   -20,
     -20,   -20
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       3,     0,     0,     0,     0,     0,    10,     0,     0,    13,
       0,     0,    17,     0,    20,    51,    23,     0,    51,    27,
      28,     0,    30,    31,     0,     0,     0,     0,    36,     0,
       0,    39,     0,     0,     0,     2,     3,     5,     6,     7,
       8,     0,    11,    12,    43,    15,    16,    18,    19,    43,
      51,    22,    24,    25,    26,    29,    32,    33,    34,    35,
      37,    38,     0,    42,     1,     4,     0,    50,     0,    14,
      44,    46,    45,    21,    52,    41,     9,     0,    48,    47,
      40,    49
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -20,   -20,    34,   -20,   -20,   -20,    24,   -20,   -19,   -16,
      15
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,    34,    35,    36,    44,    49,    69,    70,    71,    72,
      51
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
       1,     2,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    54,    67,    37,    68,    38,    64,    39,
      40,    41,    81,    42,    43,    45,    46,    47,    48,    50,
      52,    78,    53,    55,    79,    56,    57,    58,    59,     0,
      60,    61,    62,    63,    66,    74,    75,    76,    77,    80,
      65,     0,     0,    73
};

static const yytype_int8 yycheck[] =
{
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    18,    36,    36,    38,    36,     0,    37,
      36,    36,    39,    37,    36,    36,    36,    36,    36,    36,
      36,    70,    36,    36,    70,    37,    36,    36,    36,    -1,
      37,    36,    36,    36,    36,    50,    36,    36,    36,    36,
      36,    -1,    -1,    49
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    41,    42,    43,    36,    36,    37,
      36,    36,    37,    36,    44,    36,    36,    36,    36,    45,
      36,    50,    36,    36,    50,    36,    37,    36,    36,    36,
      37,    36,    36,    36,     0,    42,    36,    36,    38,    46,
      47,    48,    49,    46,    50,    36,    36,    36,    48,    49,
      36,    39
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    40,    41,    42,    42,    43,    43,    43,    43,    43,
      43,    43,    43,    44,    43,    43,    43,    43,    43,    43,
      45,    43,    43,    43,    43,    43,    43,    43,    43,    43,
      43,    43,    43,    43,    43,    43,    43,    43,    43,    43,
      43,    43,    43,    46,    46,    47,    47,    47,    47,    48,
      49,    50,    50
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     2,     2,     2,     2,     4,
       1,     2,     2,     0,     3,     2,     2,     1,     2,     2,
       0,     3,     2,     1,     2,     2,     2,     1,     1,     2,
       1,     1,     2,     2,     2,     2,     1,     2,     2,     1,
       4,     3,     2,     0,     1,     1,     1,     2,     2,     3,
       1,     0,     2
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 5: /* command: CHECK STRING  */
#line 157 "nlmheader.y"
          {
	    check_procedure = (yyvsp[0].string);
	  }
#line 1356 "nlmheader.c"
    break;

  case 6: /* command: CODESTART STRING  */
#line 161 "nlmheader.y"
          {
	    nlmheader_warn(_("CODESTART is not implemented; sorry"), -1);
	    free((yyvsp[0].string));
	  }
#line 1365 "nlmheader.c"
    break;

  case 7: /* command: COPYRIGHT QUOTED_STRING  */
#line 166 "nlmheader.y"
          {
	    size_t len;

	    strncpy(copyright_hdr->stamp, "CoPyRiGhT=", 11UL);
	    len = strlen((yyvsp[0].string));
	    if (len >= NLM_MAX_COPYRIGHT_MESSAGE_LENGTH)
	      {
		nlmheader_warn(_("copyright string is too long"),
                               (NLM_MAX_COPYRIGHT_MESSAGE_LENGTH - 1));
		len = NLM_MAX_COPYRIGHT_MESSAGE_LENGTH - 1;
	      }
	    copyright_hdr->copyrightMessageLength = len;
	    strncpy(copyright_hdr->copyrightMessage, (yyvsp[0].string), len);
	    copyright_hdr->copyrightMessage[len] = '\0';
	    free((yyvsp[0].string));
	  }
#line 1386 "nlmheader.c"
    break;

  case 8: /* command: CUSTOM STRING  */
#line 183 "nlmheader.y"
          {
	    custom_file = (yyvsp[0].string);
	  }
#line 1394 "nlmheader.c"
    break;

  case 9: /* command: DATE STRING STRING STRING  */
#line 187 "nlmheader.y"
          {
	    /* We don't set the version stamp here, because we use the
	       version stamp to detect whether the required VERSION
	       keyword was given.  */
	    version_hdr->month = nlmlex_get_number((yyvsp[-2].string));
	    version_hdr->day = nlmlex_get_number((yyvsp[-1].string));
	    version_hdr->year = nlmlex_get_number((yyvsp[0].string));
	    free((yyvsp[-2].string));
            if ((yyvsp[-2].string) != (yyvsp[-1].string)) {
	      free((yyvsp[-1].string)); /* FIXME: potential double free? */
            }
            if (((yyvsp[-2].string) != (yyvsp[0].string)) && ((yyvsp[-1].string) != (yyvsp[0].string))) {
	      free((yyvsp[0].string)); /* FIXME: potential double free? */
            }
	    if ((version_hdr->month < 1) || (version_hdr->month > 12))
	      nlmheader_warn(_("illegal month"), -1);
	    if ((version_hdr->day < 1) || (version_hdr->day > 31))
	      nlmheader_warn(_("illegal day"), -1);
	    if ((version_hdr->year < 1900) || (version_hdr->year > 3000))
	      nlmheader_warn(_("illegal year"), -1);
	  }
#line 1420 "nlmheader.c"
    break;

  case 10: /* command: DEBUG  */
#line 209 "nlmheader.y"
          {
	    debug_info = TRUE;
	  }
#line 1428 "nlmheader.c"
    break;

  case 11: /* command: DESCRIPTION QUOTED_STRING  */
#line 213 "nlmheader.y"
          {
	    size_t len;

	    len = strlen((yyvsp[0].string));
	    if (len > NLM_MAX_DESCRIPTION_LENGTH)
	      {
		nlmheader_warn(_("description string is too long"),
                               NLM_MAX_DESCRIPTION_LENGTH);
		len = NLM_MAX_DESCRIPTION_LENGTH;
	      }
	    var_hdr->descriptionLength = len;
	    strncpy(var_hdr->descriptionText, (yyvsp[0].string), len);
	    var_hdr->descriptionText[len] = '\0';
	    free((yyvsp[0].string));
	  }
#line 1448 "nlmheader.c"
    break;

  case 12: /* command: EXIT STRING  */
#line 229 "nlmheader.y"
          {
	    exit_procedure = (yyvsp[0].string);
	  }
#line 1456 "nlmheader.c"
    break;

  case 13: /* $@1: %empty  */
#line 233 "nlmheader.y"
          {
	    symbol_prefix = NULL;
	  }
#line 1464 "nlmheader.c"
    break;

  case 14: /* command: EXPORT $@1 symbol_list_opt  */
#line 237 "nlmheader.y"
          {
	    export_symbols = string_list_append(export_symbols, (yyvsp[0].list));
	  }
#line 1472 "nlmheader.c"
    break;

  case 15: /* command: FLAG_ON STRING  */
#line 241 "nlmheader.y"
          {
	    fixed_hdr->flags |= nlmlex_get_number((yyvsp[0].string));
	    free ((yyvsp[0].string));
	  }
#line 1481 "nlmheader.c"
    break;

  case 16: /* command: FLAG_OFF STRING  */
#line 246 "nlmheader.y"
          {
	    fixed_hdr->flags &=~ nlmlex_get_number((yyvsp[0].string));
	    free((yyvsp[0].string));
	  }
#line 1490 "nlmheader.c"
    break;

  case 17: /* command: FULLMAP  */
#line 251 "nlmheader.y"
          {
	    map_file = "";
	    full_map = TRUE;
	  }
#line 1499 "nlmheader.c"
    break;

  case 18: /* command: FULLMAP STRING  */
#line 256 "nlmheader.y"
          {
	    map_file = (yyvsp[0].string);
	    full_map = TRUE;
	  }
#line 1508 "nlmheader.c"
    break;

  case 19: /* command: HELP STRING  */
#line 261 "nlmheader.y"
          {
	    help_file = (yyvsp[0].string);
	  }
#line 1516 "nlmheader.c"
    break;

  case 20: /* $@2: %empty  */
#line 265 "nlmheader.y"
          {
	    symbol_prefix = NULL;
	  }
#line 1524 "nlmheader.c"
    break;

  case 21: /* command: IMPORT $@2 symbol_list_opt  */
#line 269 "nlmheader.y"
          {
	    import_symbols = string_list_append(import_symbols, (yyvsp[0].list));
	  }
#line 1532 "nlmheader.c"
    break;

  case 22: /* command: INPUT string_list  */
#line 273 "nlmheader.y"
          {
	    input_files = string_list_append(input_files, (yyvsp[0].list));
	  }
#line 1540 "nlmheader.c"
    break;

  case 23: /* command: MAP  */
#line 277 "nlmheader.y"
          {
	    map_file = "";
	  }
#line 1548 "nlmheader.c"
    break;

  case 24: /* command: MAP STRING  */
#line 281 "nlmheader.y"
          {
	    map_file = (yyvsp[0].string);
	  }
#line 1556 "nlmheader.c"
    break;

  case 25: /* command: MESSAGES STRING  */
#line 285 "nlmheader.y"
          {
	    message_file = (yyvsp[0].string);
	  }
#line 1564 "nlmheader.c"
    break;

  case 26: /* command: MODULE string_list  */
#line 289 "nlmheader.y"
          {
	    modules = string_list_append(modules, (yyvsp[0].list));
	  }
#line 1572 "nlmheader.c"
    break;

  case 27: /* command: MULTIPLE  */
#line 293 "nlmheader.y"
          {
	    fixed_hdr->flags |= 0x2;
	  }
#line 1580 "nlmheader.c"
    break;

  case 28: /* command: OS_DOMAIN  */
#line 297 "nlmheader.y"
          {
	    fixed_hdr->flags |= 0x10;
	  }
#line 1588 "nlmheader.c"
    break;

  case 29: /* command: OUTPUT STRING  */
#line 301 "nlmheader.y"
          {
	    if (output_file == NULL)
	      output_file = (yyvsp[0].string);
	    else
	      nlmheader_warn(_("ignoring duplicate OUTPUT statement"), -1);
	  }
#line 1599 "nlmheader.c"
    break;

  case 30: /* command: PSEUDOPREEMPTION  */
#line 308 "nlmheader.y"
          {
	    fixed_hdr->flags |= 0x8;
	  }
#line 1607 "nlmheader.c"
    break;

  case 31: /* command: REENTRANT  */
#line 312 "nlmheader.y"
          {
	    fixed_hdr->flags |= 0x1;
	  }
#line 1615 "nlmheader.c"
    break;

  case 32: /* command: SCREENNAME QUOTED_STRING  */
#line 316 "nlmheader.y"
          {
	    size_t len;

	    len = strlen((yyvsp[0].string));
	    if (len >= NLM_MAX_SCREEN_NAME_LENGTH)
	      {
		nlmheader_warn(_("screen name is too long"),
                               NLM_MAX_SCREEN_NAME_LENGTH);
		len = NLM_MAX_SCREEN_NAME_LENGTH;
	      }
	    var_hdr->screenNameLength = len;
	    strncpy(var_hdr->screenName, (yyvsp[0].string), len);
	    var_hdr->screenName[NLM_MAX_SCREEN_NAME_LENGTH] = '\0';
	    free((yyvsp[0].string));
	  }
#line 1635 "nlmheader.c"
    break;

  case 33: /* command: SHARELIB STRING  */
#line 332 "nlmheader.y"
          {
	    sharelib_file = (yyvsp[0].string);
	  }
#line 1643 "nlmheader.c"
    break;

  case 34: /* command: STACK STRING  */
#line 336 "nlmheader.y"
          {
	    var_hdr->stackSize = nlmlex_get_number((yyvsp[0].string));
	    free((yyvsp[0].string));
	  }
#line 1652 "nlmheader.c"
    break;

  case 35: /* command: START STRING  */
#line 341 "nlmheader.y"
          {
	    start_procedure = (yyvsp[0].string);
	  }
#line 1660 "nlmheader.c"
    break;

  case 36: /* command: SYNCHRONIZE  */
#line 345 "nlmheader.y"
          {
	    fixed_hdr->flags |= 0x4;
	  }
#line 1668 "nlmheader.c"
    break;

  case 37: /* command: THREADNAME QUOTED_STRING  */
#line 349 "nlmheader.y"
          {
	    size_t len;

	    len = strlen((yyvsp[0].string));
	    if (len >= NLM_MAX_THREAD_NAME_LENGTH)
	      {
		nlmheader_warn(_("thread name is too long"),
                               NLM_MAX_THREAD_NAME_LENGTH);
		len = NLM_MAX_THREAD_NAME_LENGTH;
	      }
	    var_hdr->threadNameLength = len;
	    strncpy(var_hdr->threadName, (yyvsp[0].string), len);
	    var_hdr->threadName[len] = '\0';
	    free((yyvsp[0].string));
	  }
#line 1688 "nlmheader.c"
    break;

  case 38: /* command: TYPE STRING  */
#line 365 "nlmheader.y"
          {
	    fixed_hdr->moduleType = nlmlex_get_number((yyvsp[0].string));
	    free((yyvsp[0].string));
	  }
#line 1697 "nlmheader.c"
    break;

  case 39: /* command: VERBOSE  */
#line 370 "nlmheader.y"
          {
	    verbose = TRUE;
	  }
#line 1705 "nlmheader.c"
    break;

  case 40: /* command: VERSIONK STRING STRING STRING  */
#line 374 "nlmheader.y"
          {
	    long val;

	    strncpy(version_hdr->stamp, "VeRsIoN#", 9UL);
	    version_hdr->majorVersion = nlmlex_get_number((yyvsp[-2].string));
	    val = nlmlex_get_number((yyvsp[-1].string));
	    if ((val < 0) || (val > 99))
	      nlmheader_warn(_("illegal minor version number (must be between 0 and 99)"),
			     -1);
	    else
	      version_hdr->minorVersion = val;
	    val = nlmlex_get_number((yyvsp[0].string));
	    if (val < 0)
	      nlmheader_warn(_("illegal revision number (must be between 0 and 26)"),
			     -1);
	    else if (val > 26)
	      version_hdr->revision = 0;
	    else
	      version_hdr->revision = val;
	    free((yyvsp[-2].string));
            if ((yyvsp[-2].string) != (yyvsp[-1].string)) {
	      free((yyvsp[-1].string)); /* FIXME: potential double free? */
            }
            if (((yyvsp[-2].string) != (yyvsp[0].string)) && ((yyvsp[-1].string) != (yyvsp[0].string))) {
	      free((yyvsp[0].string)); /* FIXME: potential double free? */
            }
	  }
#line 1737 "nlmheader.c"
    break;

  case 41: /* command: VERSIONK STRING STRING  */
#line 402 "nlmheader.y"
          {
	    long val;

	    strncpy(version_hdr->stamp, "VeRsIoN#", 9UL);
	    version_hdr->majorVersion = nlmlex_get_number((yyvsp[-1].string));
	    val = nlmlex_get_number((yyvsp[0].string));
	    if ((val < 0) || (val > 99))
	      nlmheader_warn(_("illegal minor version number (must be between 0 and 99)"),
			     -1);
	    else
	      version_hdr->minorVersion = val;
	    version_hdr->revision = 0;
	    free((yyvsp[-1].string));
            if ((yyvsp[-1].string) != (yyvsp[0].string)) {
	      free((yyvsp[0].string)); /* FIXME: potential double free? */
            }
	  }
#line 1759 "nlmheader.c"
    break;

  case 42: /* command: XDCDATA STRING  */
#line 420 "nlmheader.y"
          {
	    rpc_file = (yyvsp[0].string);
	  }
#line 1767 "nlmheader.c"
    break;

  case 43: /* symbol_list_opt: %empty  */
#line 429 "nlmheader.y"
          {
	    (yyval.list) = NULL;
	  }
#line 1775 "nlmheader.c"
    break;

  case 44: /* symbol_list_opt: symbol_list  */
#line 433 "nlmheader.y"
          {
	    (yyval.list) = (yyvsp[0].list);
	  }
#line 1783 "nlmheader.c"
    break;

  case 45: /* symbol_list: symbol  */
#line 444 "nlmheader.y"
          {
	    (yyval.list) = string_list_cons ((yyvsp[0].string), NULL);
	  }
#line 1791 "nlmheader.c"
    break;

  case 46: /* symbol_list: symbol_prefix  */
#line 448 "nlmheader.y"
          {
	    (yyval.list) = NULL;
	  }
#line 1799 "nlmheader.c"
    break;

  case 47: /* symbol_list: symbol_list symbol  */
#line 452 "nlmheader.y"
          {
	    (yyval.list) = string_list_append1 ((yyvsp[-1].list), (yyvsp[0].string));
	  }
#line 1807 "nlmheader.c"
    break;

  case 48: /* symbol_list: symbol_list symbol_prefix  */
#line 456 "nlmheader.y"
          {
	    (yyval.list) = (yyvsp[-1].list);
	  }
#line 1815 "nlmheader.c"
    break;

  case 49: /* symbol_prefix: '(' STRING ')'  */
#line 465 "nlmheader.y"
          {
	    if (symbol_prefix != NULL)
	      free(symbol_prefix);
	    symbol_prefix = (yyvsp[-1].string);
	  }
#line 1825 "nlmheader.c"
    break;

  case 50: /* symbol: STRING  */
#line 476 "nlmheader.y"
          {
	    if (symbol_prefix == NULL)
	      (yyval.string) = (yyvsp[0].string);
	    else
	      {
                size_t symlen = (strlen(symbol_prefix) + strlen((yyvsp[0].string)) + 4UL);
		(yyval.string) = (char *)xmalloc(symlen);
		snprintf((yyval.string), symlen, "%s@%s", symbol_prefix, (yyvsp[0].string));
		free((yyvsp[0].string));
	      }
	  }
#line 1841 "nlmheader.c"
    break;

  case 51: /* string_list: %empty  */
#line 493 "nlmheader.y"
          {
	    (yyval.list) = NULL;
	  }
#line 1849 "nlmheader.c"
    break;

  case 52: /* string_list: STRING string_list  */
#line 497 "nlmheader.y"
          {
	    (yyval.list) = string_list_cons((yyvsp[-1].string), (yyvsp[0].list));
	  }
#line 1857 "nlmheader.c"
    break;


#line 1861 "nlmheader.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 502 "nlmheader.y"


/* Ensure we have declaration for xstrerror(): */
#include "libiberty.h"

/* The lexer is simple, too simple for flex.  Keywords are only
   recognized at the start of lines.  Everything else must be an
   argument.  A comma is treated as whitespace.  */

/* The states the lexer can be in.  */

enum lex_state
{
  /* At the beginning of a line.  */
  BEGINNING_OF_LINE,
  /* In the middle of a line.  */
  IN_LINE
};

/* We need to keep a stack of files to handle file inclusion.  */

struct input
{
  /* The file to read from.  */
  FILE *file;
  /* The name of the file.  */
  char *name;
  /* The current line number.  */
  int lineno;
  /* The current state.  */
  enum lex_state state;
  /* The next file on the stack.  */
  struct input *next;
};

/* The current input file.  */

static struct input current;

/* The character which introduces comments.  */
#define COMMENT_CHAR '#'

/* Start the lexer going on the main input file: */
bfd_boolean
nlmlex_file(const char *name)
{
  current.next = NULL;
  return nlmlex_file_open(name);
}

/* Start the lexer going on a subsidiary input file: */
static void
nlmlex_file_push(const char *name)
{
  struct input *push;

  push = (struct input *)xmalloc(sizeof(struct input));
  *push = current;
  if (nlmlex_file_open(name))
    current.next = push;
  else
    {
      current = *push;
      free(push);
    }
}

/* Start lexing from a file: */
static bfd_boolean
nlmlex_file_open(const char *name)
{
  current.file = fopen(name, "r");
  if (current.file == NULL)
    {
      fprintf(stderr, "%s:%s: %s\n", program_name, name, xstrerror(errno));
      ++parse_errors;
      return FALSE;
    }
  current.name = xstrdup(name);
  current.lineno = 1;
  current.state = BEGINNING_OF_LINE;
  return TRUE;
}

/* Table used to turn keywords into tokens.  */

struct keyword_tokens_struct
{
  const char *keyword;
  int token;
};

static struct keyword_tokens_struct keyword_tokens[] =
{
  { "CHECK", CHECK },
  { "CODESTART", CODESTART },
  { "COPYRIGHT", COPYRIGHT },
  { "CUSTOM", CUSTOM },
  { "DATE", DATE },
  { "DEBUG", DEBUG },
  { "DESCRIPTION", DESCRIPTION },
  { "EXIT", EXIT },
  { "EXPORT", EXPORT },
  { "FLAG_ON", FLAG_ON },
  { "FLAG_OFF", FLAG_OFF },
  { "FULLMAP", FULLMAP },
  { "HELP", HELP },
  { "IMPORT", IMPORT },
  { "INPUT", INPUT },
  { "MAP", MAP },
  { "MESSAGES", MESSAGES },
  { "MODULE", MODULE },
  { "MULTIPLE", MULTIPLE },
  { "OS_DOMAIN", OS_DOMAIN },
  { "OUTPUT", OUTPUT },
  { "PSEUDOPREEMPTION", PSEUDOPREEMPTION },
  { "REENTRANT", REENTRANT },
  { "SCREENNAME", SCREENNAME },
  { "SHARELIB", SHARELIB },
  { "STACK", STACK },
  { "STACKSIZE", STACK },
  { "START", START },
  { "SYNCHRONIZE", SYNCHRONIZE },
  { "THREADNAME", THREADNAME },
  { "TYPE", TYPE },
  { "VERBOSE", VERBOSE },
  { "VERSION", VERSIONK },
  { "XDCDATA", XDCDATA }
};

#define KEYWORD_COUNT (sizeof(keyword_tokens) / sizeof(keyword_tokens[0]))

/* The lexer accumulates strings in these variables.  */
static char *lex_buf;
static int lex_size;
static int lex_pos;

/* Start accumulating strings into the buffer.  */
#define BUF_INIT() \
  ((void)((lex_buf != NULL) ? (lex_pos = 0) : nlmlex_buf_init()))

static int
nlmlex_buf_init(void)
{
  lex_size = 10UL;
  lex_buf = (char *)xmalloc(lex_size + 1UL);
  lex_pos = 0;
  return 0;
}

/* Finish a string in the buffer.  */
#define BUF_FINISH() ((void)(lex_buf[lex_pos] = '\0'))

/* Accumulate a character into the buffer.  */
#define BUF_ADD(c) \
  ((void)((lex_pos < lex_size) \
	  ? lex_buf[lex_pos++] = (c) \
	  : nlmlex_buf_add(c)))

static char
nlmlex_buf_add(int c)
{
  if (lex_pos >= lex_size)
    {
      lex_size *= 2;
      lex_buf = (char *)xrealloc(lex_buf, (lex_size + 1UL));
    }

  return lex_buf[lex_pos++] = c;
}

/* The lexer proper.  This is called by the bison generated parsing code: */
static int
yylex(void)
{
  int c;

tail_recurse:

  c = getc(current.file);

  /* Commas are treated as whitespace characters: */
  while (ISSPACE(c) || (c == ','))
    {
      current.state = IN_LINE;
      if (c == '\n')
	{
	  ++current.lineno;
	  current.state = BEGINNING_OF_LINE;
	}
      c = getc(current.file);
    }

  /* At the end of the file we either pop to the previous file or
     finish up.  */
  if (c == EOF)
    {
      fclose(current.file);
      free(current.name);
      if (current.next == NULL)
	return 0;
      else
	{
	  struct input *next;

	  next = current.next;
	  current = *next;
	  free(next);
	  goto tail_recurse;
	}
    }

  /* A comment character always means to drop everything until the
     next newline.  */
  if (c == COMMENT_CHAR)
    {
      do {
        c = getc(current.file);
      } while (c != '\n');
      ++current.lineno;
      current.state = BEGINNING_OF_LINE;
      goto tail_recurse;
    }

  /* An '@' introduces an include file.  */
  if (c == '@')
    {
      do {
        c = getc(current.file);
        if (c == '\n')
          ++current.lineno;
      } while (ISSPACE(c));
      BUF_INIT();
      while (!ISSPACE(c) && (c != EOF))
	{
	  BUF_ADD(c);
	  c = getc(current.file);
	}
      BUF_FINISH();

      ungetc(c, current.file);

      nlmlex_file_push(lex_buf);
      goto tail_recurse;
    }

  /* A non-space character at the start of a line must be the start of
     a keyword.  */
  if (current.state == BEGINNING_OF_LINE)
    {
      BUF_INIT();
      while (ISALNUM(c) || (c == '_'))
	{
	  BUF_ADD(TOUPPER(c));
	  c = getc(current.file);
	}
      BUF_FINISH();

      if ((c != EOF) && ! ISSPACE(c) && (c != ','))
	{
	  nlmheader_identify();
	  fprintf(stderr, _("%s:%d: illegal character in keyword: %c\n"),
		  current.name, current.lineno, c);
	}
      else
	{
	  unsigned int i;

	  for (i = 0; i < KEYWORD_COUNT; i++)
	    {
	      if ((lex_buf[0] == keyword_tokens[i].keyword[0])
		  && (strcmp(lex_buf, keyword_tokens[i].keyword) == 0))
		{
		  /* Pushing back the final whitespace avoids worrying
		     about \n here.  */
		  ungetc(c, current.file);
		  current.state = IN_LINE;
		  return keyword_tokens[i].token;
		}
	    }

	  nlmheader_identify();
	  fprintf(stderr, _("%s:%d: unrecognized keyword: %s\n"),
		  current.name, current.lineno, lex_buf);
	}

      ++parse_errors;
      /* Treat the rest of this line as a comment: */
      ungetc(COMMENT_CHAR, current.file);
      goto tail_recurse;
    }

  /* Parentheses just represent themselves: */
  if (c == '(' || c == ')')
    return c;

  /* Handle quoted strings: */
  if (c == '"' || c == '\'')
    {
      int quote;
      int start_lineno;

      quote = c;
      start_lineno = current.lineno;

      c = getc(current.file);
      BUF_INIT();
      while ((c != quote) && (c != EOF))
	{
	  BUF_ADD(c);
	  if (c == '\n')
	    ++current.lineno;
	  c = getc(current.file);
	}
      BUF_FINISH();

      if (c == EOF)
	{
	  nlmheader_identify();
	  fprintf(stderr, _("%s:%d: end of file in quoted string\n"),
		  current.name, start_lineno);
	  ++parse_errors;
	}

      /* FIXME: Possible memory leak: */
      yylval.string = xstrdup(lex_buf);
      return QUOTED_STRING;
    }

  /* Gather a generic argument: */
  BUF_INIT();
  while (! ISSPACE(c) && (c != ',') && (c != COMMENT_CHAR) && (c != '(')
	 && (c != ')'))
    {
      BUF_ADD(c);
      c = getc(current.file);
    }
  BUF_FINISH();

  ungetc(c, current.file);

  /* FIXME: Possible memory leak: */
  yylval.string = xstrdup(lex_buf);
  return STRING;
}

/* Get a number from a string: */
static long
nlmlex_get_number(const char *s)
{
  long ret;
  char *send;

  ret = strtol(s, &send, 10);
  if (*send != '\0')
    nlmheader_warn(_("bad number"), -1);
  return ret;
}

/* Prefix the nlmconv warnings with a note as to where they come from.
   We don't use program_name on every warning, because then some
   versions of the emacs next-error function can't recognize the line
   number.  */
static void
nlmheader_identify(void)
{
  static int done;

  if (! done)
    {
      fprintf(stderr, _("%s: problems in NLM command language input:\n"),
	      program_name);
      done = 1;
    }
}

/* Issue a warning: */
static void
nlmheader_warn(const char *s, int imax)
{
  nlmheader_identify();
  fprintf(stderr, "%s:%d: %s", current.name, current.lineno, s);
  if (imax != -1)
    fprintf(stderr, " (max %d)", imax);
  fprintf(stderr, "\n");
}

/* Report an error: */
static void
nlmheader_error(const char *s)
{
  nlmheader_warn(s, -1);
  ++parse_errors;
}

/* Add a string to a string list: */
static struct string_list *
string_list_cons(char *s, struct string_list *l)
{
  struct string_list *ret;

  ret = (struct string_list *)xmalloc(sizeof(struct string_list));
  ret->next = l;
  ret->string = s;
  return ret;
}

/* Append a string list to another string list: */
static struct string_list *
string_list_append(struct string_list *l1, struct string_list *l2)
{
  register struct string_list **pp;

  for (pp = &l1; *pp != NULL; pp = &(*pp)->next)
    ;
  *pp = l2;
  return l1;
}

/* Append a string to a string list: */
static struct string_list *
string_list_append1(struct string_list *l, char *s)
{
  struct string_list *n;
  register struct string_list **pp;

  n = (struct string_list *)xmalloc(sizeof(struct string_list));
  n->next = NULL;
  n->string = s;
  for (pp = &l; *pp != NULL; pp = &(*pp)->next)
    ;
  *pp = n;
  return l;
}

/* Duplicate a string in memory: */
static char *
xstrdup(const char *s)
{
  unsigned long len;
  char *ret;

  len = strlen(s);
  ret = (char *)xmalloc(len + 1UL);
  strcpy(ret, s);
  return ret;
}

/* End of nlmheader.y */
