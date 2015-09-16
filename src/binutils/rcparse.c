/* A Bison parser, made by GNU Bison 2.7.12-4996.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.7.12-4996"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
/* Line 371 of yacc.c  */
#line 1 "rcparse.y"
 /* rcparse.y -- parser for Windows rc files
   Copyright 1997, 1998, 1999, 2000, 2001, 2002, 2003, 2005
   Free Software Foundation, Inc.
   Written by Ian Lance Taylor, Cygnus Support.

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
   Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA
   02110-1301, USA.  */

/* This is a parser for Windows rc files.  It is based on the parser
   by Gunther Ebert <gunther.ebert@ixos-leipzig.de>.  */

#include "bfd.h"
#include "bucomm.h"
#include "libiberty.h"
#include "windres.h"
#include "safe-ctype.h"

/* The current language: */
static unsigned short language;

/* The resource information during a sub statement: */
static struct res_res_info sub_res_info;

/* Dialog information.  This is built by the nonterminals styles and
   controls.  */
static struct dialog dialog;

/* This is used when building a style.  It is modified by the
   nonterminal styleexpr.  */
static unsigned long style;

/* These are used when building a control.  They are set before using
   control_params.  */
static unsigned long base_style;
static unsigned long default_style;
static unsigned long class;
static struct res_id res_text_field;
static unichar null_unichar;

/* This is used for COMBOBOX, LISTBOX and EDITTEXT which
   do not allow resource 'text' field in control definition. */
static const struct res_id res_null_text = { 1, {{0, &null_unichar}}};


/* Line 371 of yacc.c  */
#line 127 "rcparse.c"

# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "y.tab.h".  */
#ifndef YY_YY_RCPARSE_H_INCLUDED
# define YY_YY_RCPARSE_H_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     BEG = 258,
     END = 259,
     ACCELERATORS = 260,
     VIRTKEY = 261,
     ASCII = 262,
     NOINVERT = 263,
     SHIFT = 264,
     CONTROL = 265,
     ALT = 266,
     BITMAP = 267,
     CURSOR = 268,
     DIALOG = 269,
     DIALOGEX = 270,
     EXSTYLE = 271,
     CAPTION = 272,
     CLASS = 273,
     STYLE = 274,
     AUTO3STATE = 275,
     AUTOCHECKBOX = 276,
     AUTORADIOBUTTON = 277,
     CHECKBOX = 278,
     COMBOBOX = 279,
     CTEXT = 280,
     DEFPUSHBUTTON = 281,
     EDITTEXT = 282,
     GROUPBOX = 283,
     LISTBOX = 284,
     LTEXT = 285,
     PUSHBOX = 286,
     PUSHBUTTON = 287,
     RADIOBUTTON = 288,
     RTEXT = 289,
     SCROLLBAR = 290,
     STATE3 = 291,
     USERBUTTON = 292,
     BEDIT = 293,
     HEDIT = 294,
     IEDIT = 295,
     FONT = 296,
     ICON = 297,
     LANGUAGE = 298,
     CHARACTERISTICS = 299,
     VERSIONK = 300,
     MENU = 301,
     MENUEX = 302,
     MENUITEM = 303,
     SEPARATOR = 304,
     POPUP = 305,
     CHECKED = 306,
     GRAYED = 307,
     HELP = 308,
     INACTIVE = 309,
     MENUBARBREAK = 310,
     MENUBREAK = 311,
     MESSAGETABLE = 312,
     RCDATA = 313,
     STRINGTABLE = 314,
     VERSIONINFO = 315,
     FILEVERSION = 316,
     PRODUCTVERSION = 317,
     FILEFLAGSMASK = 318,
     FILEFLAGS = 319,
     FILEOS = 320,
     FILETYPE = 321,
     FILESUBTYPE = 322,
     BLOCKSTRINGFILEINFO = 323,
     BLOCKVARFILEINFO = 324,
     VALUE = 325,
     BLOCK = 326,
     MOVEABLE = 327,
     FIXED = 328,
     PURE = 329,
     IMPURE = 330,
     PRELOAD = 331,
     LOADONCALL = 332,
     DISCARDABLE = 333,
     NOT = 334,
     QUOTEDSTRING = 335,
     STRING = 336,
     NUMBER = 337,
     SIZEDSTRING = 338,
     IGNORED_TOKEN = 339,
     NEG = 340
   };
#endif


#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 387 of yacc.c  */
#line 61 "rcparse.y"

  struct accelerator acc;
  struct accelerator *pacc;
  struct dialog_control *dialog_control;
  struct menuitem *menuitem;
  struct
  {
    struct rcdata_item *first;
    struct rcdata_item *last;
  } rcdata;
  struct rcdata_item *rcdata_item;
  struct stringtable_data *stringtable;
  struct fixed_versioninfo *fixver;
  struct ver_info *verinfo;
  struct ver_stringinfo *verstring;
  struct ver_varinfo *vervar;
  struct res_id id;
  struct res_res_info res_info;
  struct
  {
    unsigned short on;
    unsigned short off;
  } memflags;
  struct
  {
    unsigned long val;
    /* Nonzero if this number was explicitly specified as long.  */
    int dword;
  } i;
  unsigned long il;
  unsigned short is;
  const char *s;
  struct
  {
    unsigned long length;
    const char *s;
  } ss;


/* Line 387 of yacc.c  */
#line 295 "rcparse.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_YY_RCPARSE_H_INCLUDED  */

/* Copy the second part of user declarations.  */

/* Line 390 of yacc.c  */
#line 323 "rcparse.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

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

#ifndef __attribute__
/* This feature is available in gcc versions 2.5 and later.  */
# if (! defined __GNUC__ || __GNUC__ < 2 \
      || (__GNUC__ == 2 && __GNUC_MINOR__ < 5))
#  define __attribute__(Spec) /* empty */
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif


/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(N) (N)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   792

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  99
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  91
/* YYNRULES -- Number of rules.  */
#define YYNRULES  247
/* YYNRULES -- Number of states.  */
#define YYNSTATES  500

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   340

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,    92,    87,     2,
      97,    98,    90,    88,    95,    89,     2,    91,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    96,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    86,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    85,     2,    93,     2,     2,     2,
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
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      94
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     7,    10,    13,    16,    19,    22,
      25,    28,    31,    34,    37,    40,    43,    46,    49,    56,
      57,    60,    63,    68,    70,    72,    74,    78,    81,    83,
      85,    87,    89,    91,    93,    98,   103,   104,   118,   119,
     133,   134,   149,   150,   154,   155,   159,   163,   167,   171,
     175,   181,   188,   196,   205,   209,   213,   218,   222,   223,
     226,   227,   232,   233,   238,   239,   244,   245,   250,   251,
     256,   257,   261,   273,   286,   299,   313,   314,   319,   320,
     325,   326,   330,   331,   336,   337,   342,   349,   358,   369,
     381,   382,   387,   388,   392,   393,   398,   399,   404,   405,
     410,   411,   416,   417,   422,   423,   427,   428,   433,   434,
     450,   457,   466,   476,   477,   480,   482,   485,   486,   490,
     491,   495,   496,   500,   501,   505,   510,   515,   519,   526,
     527,   530,   535,   538,   545,   546,   550,   553,   555,   557,
     559,   561,   563,   565,   572,   573,   576,   579,   583,   589,
     592,   598,   605,   613,   623,   628,   635,   640,   641,   644,
     645,   647,   649,   651,   655,   659,   660,   667,   668,   672,
     677,   684,   689,   696,   697,   704,   711,   715,   719,   723,
     727,   731,   732,   741,   749,   750,   756,   757,   761,   763,
     765,   767,   770,   773,   776,   778,   779,   782,   786,   791,
     795,   796,   799,   800,   803,   805,   807,   809,   811,   813,
     815,   817,   819,   821,   823,   826,   830,   835,   837,   841,
     842,   844,   847,   849,   851,   855,   858,   861,   865,   869,
     873,   877,   881,   885,   889,   893,   896,   898,   900,   904,
     907,   911,   915,   919,   923,   927,   931,   935
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     100,     0,    -1,    -1,   100,   101,    -1,   100,   107,    -1,
     100,   108,    -1,   100,   109,    -1,   100,   147,    -1,   100,
     148,    -1,   100,   149,    -1,   100,   150,    -1,   100,   155,
      -1,   100,   158,    -1,   100,   159,    -1,   100,   164,    -1,
     100,   167,    -1,   100,   168,    -1,   100,    84,    -1,   173,
       5,   176,     3,   102,     4,    -1,    -1,   102,   103,    -1,
     104,   187,    -1,   104,   187,    95,   105,    -1,    80,    -1,
     188,    -1,   106,    -1,   105,    95,   106,    -1,   105,   106,
      -1,     6,    -1,     7,    -1,     8,    -1,     9,    -1,    10,
      -1,    11,    -1,   173,    12,   178,   180,    -1,   173,    13,
     177,   180,    -1,    -1,   173,    14,   178,   113,   188,   184,
     184,   184,   110,   114,     3,   115,     4,    -1,    -1,   173,
      15,   178,   113,   188,   184,   184,   184,   111,   114,     3,
     115,     4,    -1,    -1,   173,    15,   178,   113,   188,   184,
     184,   184,   184,   112,   114,     3,   115,     4,    -1,    -1,
      16,    96,   185,    -1,    -1,   114,    17,    80,    -1,   114,
      18,   173,    -1,   114,    19,   181,    -1,   114,    16,   185,
      -1,   114,    18,    80,    -1,   114,    41,   185,    95,    80,
      -1,   114,    41,   185,    95,    80,   184,    -1,   114,    41,
     185,    95,    80,   184,   184,    -1,   114,    41,   185,    95,
      80,   184,   184,   184,    -1,   114,    46,   173,    -1,   114,
      44,   185,    -1,   114,    43,   185,   184,    -1,   114,    45,
     185,    -1,    -1,   115,   116,    -1,    -1,    20,   139,   117,
     138,    -1,    -1,    21,   139,   118,   138,    -1,    -1,    22,
     139,   119,   138,    -1,    -1,    38,   139,   120,   138,    -1,
      -1,    23,   139,   121,   138,    -1,    -1,    24,   122,   138,
      -1,    10,   139,   185,   184,   141,   184,   184,   184,   184,
     183,   140,    -1,    10,   139,   185,   184,   141,   184,   184,
     184,   184,   184,   184,   140,    -1,    10,   139,   185,    95,
      80,   141,   184,   184,   184,   184,   183,   140,    -1,    10,
     139,   185,    95,    80,   141,   184,   184,   184,   184,   184,
     184,   140,    -1,    -1,    25,   139,   123,   138,    -1,    -1,
      26,   139,   124,   138,    -1,    -1,    27,   125,   138,    -1,
      -1,    28,   139,   126,   138,    -1,    -1,    39,   139,   127,
     138,    -1,    42,   175,   185,   184,   184,   140,    -1,    42,
     175,   185,   184,   184,   184,   184,   140,    -1,    42,   175,
     185,   184,   184,   184,   184,   143,   183,   140,    -1,    42,
     175,   185,   184,   184,   184,   184,   143,   184,   184,   140,
      -1,    -1,    40,   139,   128,   138,    -1,    -1,    29,   129,
     138,    -1,    -1,    30,   139,   130,   138,    -1,    -1,    31,
     139,   131,   138,    -1,    -1,    32,   139,   132,   138,    -1,
      -1,    33,   139,   133,   138,    -1,    -1,    34,   139,   134,
     138,    -1,    -1,    35,   135,   138,    -1,    -1,    36,   139,
     136,   138,    -1,    -1,    37,   175,   185,    95,   185,    95,
     185,    95,   185,    95,   185,    95,   137,   181,   183,    -1,
     185,   184,   184,   184,   184,   140,    -1,   185,   184,   184,
     184,   184,   145,   183,   140,    -1,   185,   184,   184,   184,
     184,   145,   184,   184,   140,    -1,    -1,   188,    95,    -1,
      80,    -1,    80,    95,    -1,    -1,     3,   160,     4,    -1,
      -1,    95,   142,   181,    -1,    -1,    95,   144,   181,    -1,
      -1,    95,   146,   181,    -1,   173,    41,   177,   180,    -1,
     173,    42,   177,   180,    -1,    43,   185,   184,    -1,   173,
      46,   176,     3,   151,     4,    -1,    -1,   151,   152,    -1,
      48,    80,   184,   153,    -1,    48,    49,    -1,    50,    80,
     153,     3,   151,     4,    -1,    -1,   153,    95,   154,    -1,
     153,   154,    -1,    51,    -1,    52,    -1,    53,    -1,    54,
      -1,    55,    -1,    56,    -1,   173,    47,   176,     3,   156,
       4,    -1,    -1,   156,   157,    -1,    48,    80,    -1,    48,
      80,   184,    -1,    48,    80,   184,   184,   183,    -1,    48,
      49,    -1,    50,    80,     3,   156,     4,    -1,    50,    80,
     184,     3,   156,     4,    -1,    50,    80,   184,   184,     3,
     156,     4,    -1,    50,    80,   184,   184,   184,   183,     3,
     156,     4,    -1,   173,    57,   178,   180,    -1,   173,    58,
     176,     3,   160,     4,    -1,   173,    58,   176,   180,    -1,
      -1,   161,   162,    -1,    -1,   163,    -1,    83,    -1,   186,
      -1,   163,    95,    83,    -1,   163,    95,   186,    -1,    -1,
      59,   176,     3,   165,   166,     4,    -1,    -1,   166,   185,
      80,    -1,   166,   185,    95,    80,    -1,   173,   173,   176,
       3,   160,     4,    -1,   173,   173,   176,   180,    -1,   173,
      60,   169,     3,   170,     4,    -1,    -1,   169,    61,   185,
     184,   184,   184,    -1,   169,    62,   185,   184,   184,   184,
      -1,   169,    63,   185,    -1,   169,    64,   185,    -1,   169,
      65,   185,    -1,   169,    66,   185,    -1,   169,    67,   185,
      -1,    -1,   170,    68,     3,    71,     3,   171,     4,     4,
      -1,   170,    69,     3,    70,    80,   172,     4,    -1,    -1,
     171,    70,    80,    95,    80,    -1,    -1,   172,   184,   184,
      -1,   188,    -1,    81,    -1,    80,    -1,    80,    95,    -1,
      81,    95,    -1,   188,    95,    -1,   174,    -1,    -1,   176,
     179,    -1,   176,    44,   185,    -1,   176,    43,   185,   184,
      -1,   176,    45,   185,    -1,    -1,   177,   179,    -1,    -1,
     178,   179,    -1,    72,    -1,    73,    -1,    74,    -1,    75,
      -1,    76,    -1,    77,    -1,    78,    -1,    80,    -1,    81,
      -1,   182,    -1,    79,   182,    -1,   181,    85,   182,    -1,
     181,    85,    79,   182,    -1,    82,    -1,    97,   185,    98,
      -1,    -1,   184,    -1,    95,   185,    -1,   186,    -1,    82,
      -1,    97,   186,    98,    -1,    93,   186,    -1,    89,   186,
      -1,   186,    90,   186,    -1,   186,    91,   186,    -1,   186,
      92,   186,    -1,   186,    88,   186,    -1,   186,    89,   186,
      -1,   186,    87,   186,    -1,   186,    86,   186,    -1,   186,
      85,   186,    -1,    95,   188,    -1,   189,    -1,    82,    -1,
      97,   186,    98,    -1,    93,   186,    -1,   189,    90,   186,
      -1,   189,    91,   186,    -1,   189,    92,   186,    -1,   189,
      88,   186,    -1,   189,    89,   186,    -1,   189,    87,   186,
      -1,   189,    86,   186,    -1,   189,    85,   186,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   156,   156,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   178,   189,
     192,   213,   218,   230,   251,   261,   265,   270,   277,   281,
     286,   290,   294,   298,   307,   319,   333,   331,   358,   356,
     385,   383,   415,   418,   424,   426,   432,   436,   441,   445,
     449,   462,   477,   492,   507,   511,   515,   519,   525,   527,
     539,   538,   550,   549,   561,   560,   572,   571,   586,   585,
     597,   596,   609,   620,   629,   642,   654,   653,   665,   664,
     676,   675,   687,   686,   698,   697,   711,   716,   722,   728,
     735,   734,   749,   748,   760,   759,   771,   770,   781,   780,
     792,   791,   803,   802,   814,   813,   825,   824,   837,   835,
     853,   864,   875,   888,   891,   896,   900,   908,   911,   921,
     920,   927,   926,   933,   932,   940,   952,   965,   974,   985,
     988,  1005,  1009,  1013,  1021,  1024,  1028,  1035,  1039,  1043,
    1047,  1051,  1055,  1064,  1075,  1078,  1095,  1099,  1103,  1107,
    1111,  1115,  1119,  1123,  1133,  1145,  1152,  1165,  1165,  1177,
    1181,  1188,  1196,  1204,  1213,  1228,  1227,  1232,  1234,  1241,
    1254,  1261,  1273,  1284,  1289,  1295,  1301,  1306,  1311,  1316,
    1321,  1336,  1339,  1343,  1351,  1354,  1362,  1365,  1374,  1379,
    1395,  1399,  1403,  1411,  1416,  1434,  1440,  1446,  1451,  1456,
    1467,  1472,  1484,  1489,  1501,  1506,  1511,  1516,  1521,  1526,
    1531,  1541,  1545,  1562,  1566,  1570,  1574,  1581,  1585,  1595,
    1598,  1607,  1616,  1625,  1629,  1633,  1638,  1643,  1648,  1653,
    1658,  1663,  1668,  1673,  1678,  1689,  1698,  1709,  1713,  1717,
    1722,  1727,  1732,  1737,  1742,  1747,  1752,  1757
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "BEG", "END", "ACCELERATORS", "VIRTKEY",
  "ASCII", "NOINVERT", "SHIFT", "CONTROL", "ALT", "BITMAP", "CURSOR",
  "DIALOG", "DIALOGEX", "EXSTYLE", "CAPTION", "CLASS", "STYLE",
  "AUTO3STATE", "AUTOCHECKBOX", "AUTORADIOBUTTON", "CHECKBOX", "COMBOBOX",
  "CTEXT", "DEFPUSHBUTTON", "EDITTEXT", "GROUPBOX", "LISTBOX", "LTEXT",
  "PUSHBOX", "PUSHBUTTON", "RADIOBUTTON", "RTEXT", "SCROLLBAR", "STATE3",
  "USERBUTTON", "BEDIT", "HEDIT", "IEDIT", "FONT", "ICON", "LANGUAGE",
  "CHARACTERISTICS", "VERSIONK", "MENU", "MENUEX", "MENUITEM", "SEPARATOR",
  "POPUP", "CHECKED", "GRAYED", "HELP", "INACTIVE", "MENUBARBREAK",
  "MENUBREAK", "MESSAGETABLE", "RCDATA", "STRINGTABLE", "VERSIONINFO",
  "FILEVERSION", "PRODUCTVERSION", "FILEFLAGSMASK", "FILEFLAGS", "FILEOS",
  "FILETYPE", "FILESUBTYPE", "BLOCKSTRINGFILEINFO", "BLOCKVARFILEINFO",
  "VALUE", "BLOCK", "MOVEABLE", "FIXED", "PURE", "IMPURE", "PRELOAD",
  "LOADONCALL", "DISCARDABLE", "NOT", "QUOTEDSTRING", "STRING", "NUMBER",
  "SIZEDSTRING", "IGNORED_TOKEN", "'|'", "'^'", "'&'", "'+'", "'-'", "'*'",
  "'/'", "'%'", "'~'", "NEG", "','", "'='", "'('", "')'", "$accept",
  "input", "accelerator", "acc_entries", "acc_entry", "acc_event",
  "acc_options", "acc_option", "bitmap", "cursor", "dialog", "$@1", "$@2",
  "$@3", "exstyle", "styles", "controls", "control", "$@4", "$@5", "$@6",
  "$@7", "$@8", "$@9", "$@10", "$@11", "$@12", "$@13", "$@14", "$@15",
  "$@16", "$@17", "$@18", "$@19", "$@20", "$@21", "$@22", "$@23", "$@24",
  "control_params", "optresidc", "opt_control_data", "control_styleexpr",
  "$@25", "icon_styleexpr", "$@26", "control_params_styleexpr", "$@27",
  "font", "icon", "language", "menu", "menuitems", "menuitem",
  "menuitem_flags", "menuitem_flag", "menuex", "menuexitems", "menuexitem",
  "messagetable", "rcdata", "optrcdata_data", "$@28", "optrcdata_data_int",
  "rcdata_data", "stringtable", "$@29", "string_data", "user",
  "versioninfo", "fixedverinfo", "verblocks", "vervals", "vertrans", "id",
  "resname", "resref", "suboptions", "memflags_move_discard",
  "memflags_move", "memflag", "file_name", "styleexpr", "parennumber",
  "optcnumexpr", "cnumexpr", "numexpr", "sizednumexpr", "cposnumexpr",
  "posnumexpr", "sizedposnumexpr", YY_NULL
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   124,    94,    38,    43,    45,
      42,    47,    37,   126,   340,    44,    61,    40,    41
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    99,   100,   100,   100,   100,   100,   100,   100,   100,
     100,   100,   100,   100,   100,   100,   100,   100,   101,   102,
     102,   103,   103,   104,   104,   105,   105,   105,   106,   106,
     106,   106,   106,   106,   107,   108,   110,   109,   111,   109,
     112,   109,   113,   113,   114,   114,   114,   114,   114,   114,
     114,   114,   114,   114,   114,   114,   114,   114,   115,   115,
     117,   116,   118,   116,   119,   116,   120,   116,   121,   116,
     122,   116,   116,   116,   116,   116,   123,   116,   124,   116,
     125,   116,   126,   116,   127,   116,   116,   116,   116,   116,
     128,   116,   129,   116,   130,   116,   131,   116,   132,   116,
     133,   116,   134,   116,   135,   116,   136,   116,   137,   116,
     138,   138,   138,   139,   139,   139,   139,   140,   140,   142,
     141,   144,   143,   146,   145,   147,   148,   149,   150,   151,
     151,   152,   152,   152,   153,   153,   153,   154,   154,   154,
     154,   154,   154,   155,   156,   156,   157,   157,   157,   157,
     157,   157,   157,   157,   158,   159,   159,   161,   160,   162,
     162,   163,   163,   163,   163,   165,   164,   166,   166,   166,
     167,   167,   168,   169,   169,   169,   169,   169,   169,   169,
     169,   170,   170,   170,   171,   171,   172,   172,   173,   173,
     174,   174,   174,   175,   175,   176,   176,   176,   176,   176,
     177,   177,   178,   178,   179,   179,   179,   179,   179,   179,
     179,   180,   180,   181,   181,   181,   181,   182,   182,   183,
     183,   184,   185,   186,   186,   186,   186,   186,   186,   186,
     186,   186,   186,   186,   186,   187,   188,   189,   189,   189,
     189,   189,   189,   189,   189,   189,   189,   189
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     6,     0,
       2,     2,     4,     1,     1,     1,     3,     2,     1,     1,
       1,     1,     1,     1,     4,     4,     0,    13,     0,    13,
       0,    14,     0,     3,     0,     3,     3,     3,     3,     3,
       5,     6,     7,     8,     3,     3,     4,     3,     0,     2,
       0,     4,     0,     4,     0,     4,     0,     4,     0,     4,
       0,     3,    11,    12,    12,    13,     0,     4,     0,     4,
       0,     3,     0,     4,     0,     4,     6,     8,    10,    11,
       0,     4,     0,     3,     0,     4,     0,     4,     0,     4,
       0,     4,     0,     4,     0,     3,     0,     4,     0,    15,
       6,     8,     9,     0,     2,     1,     2,     0,     3,     0,
       3,     0,     3,     0,     3,     4,     4,     3,     6,     0,
       2,     4,     2,     6,     0,     3,     2,     1,     1,     1,
       1,     1,     1,     6,     0,     2,     2,     3,     5,     2,
       5,     6,     7,     9,     4,     6,     4,     0,     2,     0,
       1,     1,     1,     3,     3,     0,     6,     0,     3,     4,
       6,     4,     6,     0,     6,     6,     3,     3,     3,     3,
       3,     0,     8,     7,     0,     5,     0,     3,     1,     1,
       1,     2,     2,     2,     1,     0,     2,     3,     4,     3,
       0,     2,     0,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     3,     4,     1,     3,     0,
       1,     2,     1,     1,     3,     2,     2,     3,     3,     3,
       3,     3,     3,     3,     3,     2,     1,     1,     3,     2,
       3,     3,     3,     3,     3,     3,     3,     3
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     1,     0,   195,   189,   237,    17,     0,     0,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,     0,   188,   236,   223,     0,     0,
       0,     0,   222,     0,   239,     0,   195,   202,   200,   202,
     202,   200,   200,   195,   195,   202,   195,   173,   195,     0,
       0,     0,     0,     0,     0,     0,     0,   226,   225,     0,
       0,   127,     0,     0,     0,     0,     0,     0,     0,     0,
     165,     0,     0,     0,   204,   205,   206,   207,   208,   209,
     210,   196,   238,     0,     0,     0,    42,    42,     0,     0,
       0,     0,     0,     0,     0,     0,   247,   246,   245,   243,
     244,   240,   241,   242,   224,   221,   234,   233,   232,   230,
     231,   227,   228,   229,   167,     0,   197,   199,    19,   211,
     212,   203,    34,   201,    35,     0,     0,     0,   125,   126,
     129,   144,   154,   157,   156,   181,     0,     0,     0,     0,
       0,     0,     0,   157,   171,     0,   198,     0,     0,     0,
       0,     0,     0,     0,   159,     0,     0,     0,   176,   177,
     178,   179,   180,     0,   166,     0,    18,    23,    20,     0,
      24,    43,     0,     0,   128,     0,     0,   130,   143,     0,
       0,   145,   155,   161,   158,   160,   162,   172,     0,     0,
       0,     0,   170,   168,     0,     0,    21,     0,     0,   132,
       0,   134,   149,   146,     0,     0,     0,     0,     0,     0,
     169,   235,     0,    36,    38,   134,     0,   147,   144,     0,
     163,   164,     0,     0,   174,   175,    28,    29,    30,    31,
      32,    33,    22,    25,    44,    44,    40,   131,   129,   137,
     138,   139,   140,   141,   142,     0,   136,   219,     0,   144,
       0,   184,   186,     0,    27,     0,     0,    44,     0,   135,
     148,   220,   150,     0,   144,   219,     0,     0,    26,    58,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
       0,   133,   151,     0,     0,     0,     0,   183,     0,     0,
      48,    45,    49,    46,     0,   217,     0,    47,   213,     0,
       0,    55,    57,    54,     0,    58,   152,   144,   182,     0,
     187,    37,   113,   113,   113,   113,   113,    70,   113,   113,
      80,   113,    92,   113,   113,   113,   113,   113,   104,   113,
       0,   113,   113,   113,     0,    59,   214,     0,     0,     0,
      56,    39,     0,     0,     0,   115,     0,     0,    60,    62,
      64,    68,     0,    76,    78,     0,    82,     0,    94,    96,
      98,   100,   102,     0,   106,   190,     0,   194,     0,     0,
      66,    84,    90,     0,   218,     0,   215,    50,    41,   153,
     185,   116,     0,   114,     0,     0,     0,     0,    71,     0,
       0,     0,    81,     0,    93,     0,     0,     0,     0,     0,
     105,     0,   191,   192,     0,   193,     0,     0,     0,     0,
     216,    51,     0,     0,    61,    63,    65,    69,     0,    77,
      79,    83,    95,    97,    99,   101,   103,   107,     0,    67,
      85,    91,     0,    52,     0,   119,     0,     0,     0,   117,
      53,     0,     0,     0,     0,     0,   157,    86,     0,     0,
     120,     0,   117,     0,     0,   117,     0,     0,   123,   110,
     219,     0,   118,   121,    87,   219,     0,   219,     0,   117,
     220,     0,     0,   117,   220,   219,   117,   220,   124,   111,
     117,     0,   122,    88,   117,   117,   220,    72,   117,   112,
       0,    89,    74,   117,    73,   108,    75,     0,   219,   109
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    10,   147,   168,   169,   232,   233,    11,    12,
      13,   234,   235,   257,   126,   255,   289,   335,   384,   385,
     386,   406,   387,   352,   390,   391,   355,   393,   407,   408,
     357,   395,   396,   397,   398,   399,   363,   401,   497,   388,
     346,   447,   436,   442,   465,   472,   460,   468,    14,    15,
      16,    17,   151,   177,   216,   246,    18,   152,   181,    19,
      20,   153,   154,   184,   185,    21,   114,   145,    22,    23,
      94,   155,   266,   267,    24,   367,   368,    33,    85,    84,
      81,   122,   297,   298,   260,   261,   389,    32,   196,   347,
      26
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -426
static const yytype_int16 yypact[] =
{
    -426,    12,  -426,   287,  -426,  -426,  -426,  -426,   287,   287,
    -426,  -426,  -426,  -426,  -426,  -426,  -426,  -426,  -426,  -426,
    -426,  -426,  -426,  -426,   106,  -426,   585,  -426,   287,   287,
     287,   -80,   675,    83,  -426,   637,  -426,  -426,  -426,  -426,
    -426,  -426,  -426,  -426,  -426,  -426,  -426,  -426,  -426,   287,
     287,   287,   287,   287,   287,   287,   287,  -426,  -426,   651,
     287,  -426,   287,   287,   287,   287,   287,   287,   287,   287,
    -426,   287,   287,   287,  -426,  -426,  -426,  -426,  -426,  -426,
    -426,  -426,  -426,   286,   678,   678,   188,   188,   678,   678,
     323,   375,   678,   210,   128,   274,   682,   688,   302,    89,
      89,  -426,  -426,  -426,  -426,  -426,   682,   688,   302,    89,
      89,  -426,  -426,  -426,  -426,   -80,  -426,  -426,  -426,  -426,
    -426,  -426,  -426,  -426,  -426,   -70,   142,   142,  -426,  -426,
    -426,  -426,  -426,  -426,  -426,  -426,   287,   287,   287,   287,
     287,   287,   287,  -426,  -426,     2,  -426,     5,   287,   -80,
     -80,    33,    40,    47,   373,    38,   -80,   -80,  -426,  -426,
    -426,  -426,  -426,    49,  -426,    17,  -426,  -426,  -426,   -41,
    -426,  -426,   -80,   -80,  -426,   -44,   -24,  -426,  -426,   -35,
      36,  -426,  -426,  -426,  -426,   -13,   675,  -426,   129,   138,
     -80,   -80,  -426,  -426,    69,   142,    73,   -80,   -80,  -426,
     -80,  -426,  -426,   -80,     8,   454,   111,    97,   -80,   -80,
    -426,  -426,   775,  -426,   -80,  -426,   119,   -80,  -426,    13,
    -426,   675,   183,   116,  -426,  -426,  -426,  -426,  -426,  -426,
    -426,  -426,    22,  -426,  -426,  -426,  -426,   245,  -426,  -426,
    -426,  -426,  -426,  -426,  -426,   736,  -426,   -80,    96,  -426,
      15,  -426,  -426,   775,  -426,   370,   512,  -426,   158,  -426,
    -426,  -426,  -426,   172,  -426,   -80,    34,     6,  -426,  -426,
     287,   125,   227,    72,   287,   287,   287,   287,   260,  -426,
     523,  -426,  -426,   179,   207,   229,   131,  -426,   -80,   584,
    -426,  -426,  -426,  -426,    43,  -426,   287,   159,  -426,   151,
     -80,  -426,  -426,  -426,   621,  -426,  -426,  -426,  -426,   153,
    -426,  -426,   141,   141,   141,   141,   141,  -426,   141,   141,
    -426,   141,  -426,   141,   141,   141,   141,   141,  -426,   141,
     230,   141,   141,   141,   230,  -426,  -426,   160,   133,   169,
    -426,  -426,   658,   193,   189,   164,   287,   173,  -426,  -426,
    -426,  -426,   287,  -426,  -426,   287,  -426,   287,  -426,  -426,
    -426,  -426,  -426,   287,  -426,   175,   181,  -426,   287,   184,
    -426,  -426,  -426,   287,  -426,    43,  -426,   -80,  -426,  -426,
    -426,  -426,   185,  -426,   287,   287,   287,   287,  -426,   -80,
     287,   287,  -426,   287,  -426,   287,   287,   287,   287,   287,
    -426,   287,  -426,  -426,   197,  -426,   287,   287,   287,   -80,
    -426,   -80,   341,   199,  -426,  -426,  -426,  -426,   -80,  -426,
    -426,  -426,  -426,  -426,  -426,  -426,  -426,  -426,   287,  -426,
    -426,  -426,   -80,   -80,   199,  -426,   -80,   -80,   208,    18,
    -426,   -80,    72,   -80,   -80,   287,  -426,  -426,   -80,   -80,
     159,   -80,    19,   211,   298,    20,   -80,   -80,  -426,  -426,
     -80,   287,  -426,  -426,  -426,   -80,   -80,   -80,    72,   310,
     -80,   219,    72,   310,   -80,   -80,   310,   -80,   159,  -426,
     310,   287,   159,  -426,   310,   310,   -80,  -426,   310,  -426,
     220,  -426,  -426,   310,  -426,  -426,  -426,    72,   -37,  -426
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -426,  -426,  -426,  -426,  -426,  -426,  -426,  -207,  -426,  -426,
    -426,  -426,  -426,  -426,   218,  -222,  -255,  -426,  -426,  -426,
    -426,  -426,  -426,  -426,  -426,  -426,  -426,  -426,  -426,  -426,
    -426,  -426,  -426,  -426,  -426,  -426,  -426,  -426,  -426,   126,
     388,   107,  -113,  -426,  -426,  -426,  -426,  -426,  -426,  -426,
    -426,  -426,    84,  -426,   110,    87,  -426,  -215,  -426,  -426,
    -426,  -142,  -426,  -426,  -426,  -426,  -426,  -426,  -426,  -426,
    -426,  -426,  -426,  -426,   -22,  -426,     0,   397,    82,   212,
     548,   486,  -425,  -286,  -258,    28,    -3,    11,  -426,     3,
    -426
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint16 yytable[] =
{
      31,   163,    48,   248,    25,   199,   164,   284,   336,   166,
     287,   218,     2,   256,   202,    60,   249,   450,   264,    34,
      35,   446,   446,   446,   304,   254,   148,    25,   226,   227,
     228,   229,   230,   231,   263,   280,   200,   174,   285,    57,
      58,    59,   187,   478,   178,   203,   268,   482,   338,   283,
     342,   182,   376,   192,   195,     3,   201,   105,    60,    61,
      96,    97,    98,    99,   100,   101,   102,   103,   115,   116,
     117,     4,   498,   106,   107,   108,   109,   110,   111,   112,
     113,   175,   205,   176,    27,   167,    70,     6,   179,   410,
     180,    28,   343,     5,     6,    29,     7,   193,     8,    30,
     262,    60,     9,    60,   286,     8,   188,   189,    60,     9,
      60,    36,   194,    60,   458,   463,   204,   253,    37,    38,
      39,    40,   238,    88,    89,   295,    71,    72,    73,   149,
     150,   135,   206,   156,   157,   158,   159,   160,   161,   162,
     296,   207,   165,   146,   179,   171,   180,    41,    42,   210,
     170,   294,    43,    44,   295,    74,    75,    76,    77,    78,
      79,    80,   281,    45,    46,   186,    47,   223,   212,   296,
     239,   240,   241,   242,   243,   244,   282,   172,   173,    67,
      68,    69,   222,   306,   190,   191,   251,     5,     6,   136,
     137,   138,   139,   140,   141,   142,   252,   379,   211,     8,
     197,   198,   469,     9,   125,   291,   175,   473,   176,   476,
     307,   309,   375,   133,   245,   295,   221,   485,   208,   209,
     179,   345,   180,     6,     6,   213,   214,   179,   215,   180,
     296,   217,   219,   308,     8,     8,   224,   225,     9,     9,
     499,   179,   236,   180,   338,   247,   339,   250,   344,   377,
     293,    86,    87,    71,    72,    73,   303,    92,   374,   381,
      74,    75,    76,    77,    78,    79,    80,   290,   383,   380,
     402,   299,   300,   301,   302,    25,   403,   143,   265,   405,
     412,    25,    74,    75,    76,    77,    78,    79,    80,   118,
     119,   120,   428,   337,   435,   288,   239,   240,   241,   242,
     243,   244,   462,   445,   454,   127,   461,   292,     5,     6,
     365,   366,     6,   446,   481,   495,   310,    71,    72,    73,
       8,   441,   258,     8,     9,   237,   130,     9,   340,    71,
      72,    73,   259,   369,   373,     0,     0,   369,     0,     0,
     245,     5,     6,   382,     0,     0,    74,    75,    76,    77,
      78,    79,    80,     8,   119,   120,     0,     9,    74,    75,
      76,    77,    78,    79,    80,   404,    71,    72,    73,    27,
     409,     0,     0,   269,     0,     0,    28,     0,   131,     0,
      29,     0,     0,     0,    30,     0,   270,   271,   272,   273,
      65,    66,    67,    68,    69,    74,    75,    76,    77,    78,
      79,    80,     0,     0,     0,   411,     0,     0,     0,   105,
     413,   274,     0,   275,   276,   277,   278,   418,    71,    72,
      73,   434,     0,    27,     0,   438,     0,     0,     0,     0,
      28,     0,     0,    83,    29,     0,     0,   432,    30,   433,
      90,    91,   453,    93,     0,    95,   437,    74,    75,    76,
      77,    78,    79,    80,     0,    27,   183,     0,   471,     0,
     439,   440,    28,     0,   443,   444,    29,   448,     0,   449,
      30,   451,   452,     0,     0,     0,   455,   456,   490,   457,
       0,   392,     0,   394,   466,   467,     0,     0,   470,   400,
       0,     0,     0,   474,   475,   477,     0,     0,   480,     0,
       0,     0,   484,   486,     0,   488,     0,     0,     0,     0,
     414,   415,   416,   417,   493,   279,   419,   420,     0,   421,
       0,   422,   423,   424,   425,   426,   305,   427,   270,   271,
     272,   273,   429,   430,   431,     0,    27,   220,     0,   270,
     271,   272,   273,    28,     0,     0,     0,    29,     0,     0,
       0,    30,     0,   274,     0,   275,   276,   277,   278,   459,
       0,     0,   464,     0,   274,     0,   275,   276,   277,   278,
       0,   124,     0,     0,   128,   129,   479,     0,   132,   134,
     483,   144,     0,   487,     0,     0,     0,   489,   311,     0,
       0,   491,   492,     0,   312,   494,     0,     0,     0,     0,
     496,     0,     0,     0,   313,   314,   315,   316,   317,   318,
     319,   320,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   330,   331,   332,   333,   341,   334,     0,     0,     0,
       0,   312,   121,   123,   121,   121,   123,   123,     0,     0,
     121,   313,   314,   315,   316,   317,   318,   319,   320,   321,
     322,   323,   324,   325,   326,   327,   328,   329,   330,   331,
     332,   333,   378,   334,     0,     0,     0,     0,   312,     0,
      49,    50,    51,    52,    53,    54,    55,    56,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,     0,
     334,   348,   349,   350,   351,     0,   353,   354,     0,   356,
       0,   358,   359,   360,   361,   362,     0,   364,     0,   370,
     371,   372,    62,    63,    64,    65,    66,    67,    68,    69,
       0,     0,     0,     0,     0,    82,    62,    63,    64,    65,
      66,    67,    68,    69,     0,     0,     0,     0,     0,   104,
      74,    75,    76,    77,    78,    79,    80,     0,   119,   120,
      62,    63,    64,    65,    66,    67,    68,    69,    63,    64,
      65,    66,    67,    68,    69,    64,    65,    66,    67,    68,
      69,   226,   227,   228,   229,   230,   231,   239,   240,   241,
     242,   243,   244
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-426)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
       3,   143,    24,   218,     1,    49,     4,   265,   294,     4,
       4,     3,     0,   235,    49,    95,     3,   442,     3,     8,
       9,     3,     3,     3,   279,   232,    96,    24,     6,     7,
       8,     9,    10,    11,   249,   257,    80,     4,     4,    28,
      29,    30,     4,   468,     4,    80,   253,   472,    85,   264,
     305,     4,   338,     4,    95,    43,    80,    60,    95,    31,
      49,    50,    51,    52,    53,    54,    55,    56,    71,    72,
      73,    59,   497,    62,    63,    64,    65,    66,    67,    68,
      69,    48,    95,    50,    82,    80,     3,    82,    48,   375,
      50,    89,   307,    81,    82,    93,    84,    80,    93,    97,
       4,    95,    97,    95,    70,    93,    68,    69,    95,    97,
      95,     5,    95,    95,    95,    95,    80,    95,    12,    13,
      14,    15,     3,    41,    42,    82,    43,    44,    45,   126,
     127,     3,     3,   136,   137,   138,   139,   140,   141,   142,
      97,     3,   145,   115,    48,   148,    50,    41,    42,    80,
     147,    79,    46,    47,    82,    72,    73,    74,    75,    76,
      77,    78,     4,    57,    58,   154,    60,    70,    95,    97,
      51,    52,    53,    54,    55,    56,     4,   149,   150,    90,
      91,    92,    71,     4,   156,   157,     3,    81,    82,    61,
      62,    63,    64,    65,    66,    67,    80,     4,   195,    93,
     172,   173,   460,    97,    16,    80,    48,   465,    50,   467,
       3,    80,    79,     3,    95,    82,   205,   475,   190,   191,
      48,    80,    50,    82,    82,   197,   198,    48,   200,    50,
      97,   203,   204,     4,    93,    93,   208,   209,    97,    97,
     498,    48,   214,    50,    85,   217,    95,   219,    95,    80,
     272,    39,    40,    43,    44,    45,   278,    45,    98,    95,
      72,    73,    74,    75,    76,    77,    78,   270,    95,    80,
      95,   274,   275,   276,   277,   272,    95,     3,   250,    95,
      95,   278,    72,    73,    74,    75,    76,    77,    78,     3,
      80,    81,    95,   296,    95,   267,    51,    52,    53,    54,
      55,    56,     4,    95,   446,    87,    95,    80,    81,    82,
      80,    81,    82,     3,    95,    95,   288,    43,    44,    45,
      93,   434,   238,    93,    97,   215,     3,    97,   300,    43,
      44,    45,   245,   330,   334,    -1,    -1,   334,    -1,    -1,
      95,    81,    82,   346,    -1,    -1,    72,    73,    74,    75,
      76,    77,    78,    93,    80,    81,    -1,    97,    72,    73,
      74,    75,    76,    77,    78,   368,    43,    44,    45,    82,
     373,    -1,    -1,     3,    -1,    -1,    89,    -1,     3,    -1,
      93,    -1,    -1,    -1,    97,    -1,    16,    17,    18,    19,
      88,    89,    90,    91,    92,    72,    73,    74,    75,    76,
      77,    78,    -1,    -1,    -1,   377,    -1,    -1,    -1,   412,
     382,    41,    -1,    43,    44,    45,    46,   389,    43,    44,
      45,    80,    -1,    82,    -1,   428,    -1,    -1,    -1,    -1,
      89,    -1,    -1,    36,    93,    -1,    -1,   409,    97,   411,
      43,    44,   445,    46,    -1,    48,   418,    72,    73,    74,
      75,    76,    77,    78,    -1,    82,    83,    -1,   461,    -1,
     432,   433,    89,    -1,   436,   437,    93,   439,    -1,   441,
      97,   443,   444,    -1,    -1,    -1,   448,   449,   481,   451,
      -1,   355,    -1,   357,   456,   457,    -1,    -1,   460,   363,
      -1,    -1,    -1,   465,   466,   467,    -1,    -1,   470,    -1,
      -1,    -1,   474,   475,    -1,   477,    -1,    -1,    -1,    -1,
     384,   385,   386,   387,   486,     3,   390,   391,    -1,   393,
      -1,   395,   396,   397,   398,   399,     3,   401,    16,    17,
      18,    19,   406,   407,   408,    -1,    82,    83,    -1,    16,
      17,    18,    19,    89,    -1,    -1,    -1,    93,    -1,    -1,
      -1,    97,    -1,    41,    -1,    43,    44,    45,    46,   452,
      -1,    -1,   455,    -1,    41,    -1,    43,    44,    45,    46,
      -1,    85,    -1,    -1,    88,    89,   469,    -1,    92,    93,
     473,    95,    -1,   476,    -1,    -1,    -1,   480,     4,    -1,
      -1,   484,   485,    -1,    10,   488,    -1,    -1,    -1,    -1,
     493,    -1,    -1,    -1,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,     4,    42,    -1,    -1,    -1,
      -1,    10,    84,    85,    86,    87,    88,    89,    -1,    -1,
      92,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,     4,    42,    -1,    -1,    -1,    -1,    10,    -1,
      85,    86,    87,    88,    89,    90,    91,    92,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    -1,
      42,   313,   314,   315,   316,    -1,   318,   319,    -1,   321,
      -1,   323,   324,   325,   326,   327,    -1,   329,    -1,   331,
     332,   333,    85,    86,    87,    88,    89,    90,    91,    92,
      -1,    -1,    -1,    -1,    -1,    98,    85,    86,    87,    88,
      89,    90,    91,    92,    -1,    -1,    -1,    -1,    -1,    98,
      72,    73,    74,    75,    76,    77,    78,    -1,    80,    81,
      85,    86,    87,    88,    89,    90,    91,    92,    86,    87,
      88,    89,    90,    91,    92,    87,    88,    89,    90,    91,
      92,     6,     7,     8,     9,    10,    11,    51,    52,    53,
      54,    55,    56
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   100,     0,    43,    59,    81,    82,    84,    93,    97,
     101,   107,   108,   109,   147,   148,   149,   150,   155,   158,
     159,   164,   167,   168,   173,   188,   189,    82,    89,    93,
      97,   185,   186,   176,   186,   186,     5,    12,    13,    14,
      15,    41,    42,    46,    47,    57,    58,    60,   173,    85,
      86,    87,    88,    89,    90,    91,    92,   186,   186,   186,
      95,   184,    85,    86,    87,    88,    89,    90,    91,    92,
       3,    43,    44,    45,    72,    73,    74,    75,    76,    77,
      78,   179,    98,   176,   178,   177,   178,   178,   177,   177,
     176,   176,   178,   176,   169,   176,   186,   186,   186,   186,
     186,   186,   186,   186,    98,   185,   186,   186,   186,   186,
     186,   186,   186,   186,   165,   185,   185,   185,     3,    80,
      81,   179,   180,   179,   180,    16,   113,   113,   180,   180,
       3,     3,   180,     3,   180,     3,    61,    62,    63,    64,
      65,    66,    67,     3,   180,   166,   184,   102,    96,   188,
     188,   151,   156,   160,   161,   170,   185,   185,   185,   185,
     185,   185,   185,   160,     4,   185,     4,    80,   103,   104,
     188,   185,   184,   184,     4,    48,    50,   152,     4,    48,
      50,   157,     4,    83,   162,   163,   186,     4,    68,    69,
     184,   184,     4,    80,    95,    95,   187,   184,   184,    49,
      80,    80,    49,    80,    80,    95,     3,     3,   184,   184,
      80,   188,    95,   184,   184,   184,   153,   184,     3,   184,
      83,   186,    71,    70,   184,   184,     6,     7,     8,     9,
      10,    11,   105,   106,   110,   111,   184,   153,     3,    51,
      52,    53,    54,    55,    56,    95,   154,   184,   156,     3,
     184,     3,    80,    95,   106,   114,   114,   112,   151,   154,
     183,   184,     4,   156,     3,   184,   171,   172,   106,     3,
      16,    17,    18,    19,    41,    43,    44,    45,    46,     3,
     114,     4,     4,   156,   183,     4,    70,     4,   184,   115,
     185,    80,    80,   173,    79,    82,    97,   181,   182,   185,
     185,   185,   185,   173,   115,     3,     4,     3,     4,    80,
     184,     4,    10,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    42,   116,   182,   185,    85,    95,
     184,     4,   115,   156,    95,    80,   139,   188,   139,   139,
     139,   139,   122,   139,   139,   125,   139,   129,   139,   139,
     139,   139,   139,   135,   139,    80,    81,   174,   175,   188,
     139,   139,   139,   175,    98,    79,   182,    80,     4,     4,
      80,    95,   185,    95,   117,   118,   119,   121,   138,   185,
     123,   124,   138,   126,   138,   130,   131,   132,   133,   134,
     138,   136,    95,    95,   185,    95,   120,   127,   128,   185,
     182,   184,    95,   184,   138,   138,   138,   138,   184,   138,
     138,   138,   138,   138,   138,   138,   138,   138,    95,   138,
     138,   138,   184,   184,    80,    95,   141,   184,   185,   184,
     184,   141,   142,   184,   184,    95,     3,   140,   184,   184,
     181,   184,   184,   185,   160,   184,   184,   184,    95,   140,
     145,    95,     4,    95,   140,   143,   184,   184,   146,   183,
     184,   185,   144,   183,   184,   184,   183,   184,   181,   140,
     184,    95,   181,   140,   184,   183,   184,   140,   184,   140,
     185,   140,   140,   184,   140,    95,   140,   137,   181,   183
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
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
      YYERROR;							\
    }								\
while (YYID (0))

/* Error token number */
#define YYTERROR	1
#define YYERRCODE	256


/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */
#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
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


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULL, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULL;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULL, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YYUSE (yytype);
}




/* The lookahead symbol.  */
int yychar;


#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval YY_INITIAL_VALUE(yyval_default);

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

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

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
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

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

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
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 18:
/* Line 1787 of yacc.c  */
#line 179 "rcparse.y"
    {
	    define_accelerator ((yyvsp[(1) - (6)].id), &(yyvsp[(3) - (6)].res_info), (yyvsp[(5) - (6)].pacc));
	    if (yychar != YYEMPTY)
	      YYERROR;
	    rcparse_discard_strings ();
	  }
    break;

  case 19:
/* Line 1787 of yacc.c  */
#line 189 "rcparse.y"
    {
	    (yyval.pacc) = NULL;
	  }
    break;

  case 20:
/* Line 1787 of yacc.c  */
#line 193 "rcparse.y"
    {
	    struct accelerator *a;

	    a = (struct accelerator *) res_alloc (sizeof *a);
	    *a = (yyvsp[(2) - (2)].acc);
	    if ((yyvsp[(1) - (2)].pacc) == NULL)
	      (yyval.pacc) = a;
	    else
	      {
		struct accelerator **pp;

		for (pp = &(yyvsp[(1) - (2)].pacc)->next; *pp != NULL; pp = &(*pp)->next)
		  ;
		*pp = a;
		(yyval.pacc) = (yyvsp[(1) - (2)].pacc);
	      }
	  }
    break;

  case 21:
/* Line 1787 of yacc.c  */
#line 214 "rcparse.y"
    {
	    (yyval.acc) = (yyvsp[(1) - (2)].acc);
	    (yyval.acc).id = (yyvsp[(2) - (2)].il);
	  }
    break;

  case 22:
/* Line 1787 of yacc.c  */
#line 219 "rcparse.y"
    {
	    (yyval.acc) = (yyvsp[(1) - (4)].acc);
	    (yyval.acc).id = (yyvsp[(2) - (4)].il);
	    (yyval.acc).flags |= (yyvsp[(4) - (4)].is);
	    if (((yyval.acc).flags & ACC_VIRTKEY) == 0
		&& ((yyval.acc).flags & (ACC_SHIFT | ACC_CONTROL)) != 0)
	      rcparse_warning (_("inappropriate modifiers for non-VIRTKEY"));
	  }
    break;

  case 23:
/* Line 1787 of yacc.c  */
#line 231 "rcparse.y"
    {
	    const char *s = (yyvsp[(1) - (1)].s);
	    char ch;

	    (yyval.acc).next = NULL;
	    (yyval.acc).id = 0;
	    ch = *s;
	    if (ch != '^')
	      (yyval.acc).flags = 0;
	    else
	      {
		(yyval.acc).flags = ACC_CONTROL | ACC_VIRTKEY;
		++s;
		ch = *s;
		ch = TOUPPER (ch);
	      }
	    (yyval.acc).key = ch;
	    if (s[1] != '\0')
	      rcparse_warning (_("accelerator should only be one character"));
	  }
    break;

  case 24:
/* Line 1787 of yacc.c  */
#line 252 "rcparse.y"
    {
	    (yyval.acc).next = NULL;
	    (yyval.acc).flags = 0;
	    (yyval.acc).id = 0;
	    (yyval.acc).key = (yyvsp[(1) - (1)].il);
	  }
    break;

  case 25:
/* Line 1787 of yacc.c  */
#line 262 "rcparse.y"
    {
	    (yyval.is) = (yyvsp[(1) - (1)].is);
	  }
    break;

  case 26:
/* Line 1787 of yacc.c  */
#line 266 "rcparse.y"
    {
	    (yyval.is) = (yyvsp[(1) - (3)].is) | (yyvsp[(3) - (3)].is);
	  }
    break;

  case 27:
/* Line 1787 of yacc.c  */
#line 271 "rcparse.y"
    {
	    (yyval.is) = (yyvsp[(1) - (2)].is) | (yyvsp[(2) - (2)].is);
	  }
    break;

  case 28:
/* Line 1787 of yacc.c  */
#line 278 "rcparse.y"
    {
	    (yyval.is) = ACC_VIRTKEY;
	  }
    break;

  case 29:
/* Line 1787 of yacc.c  */
#line 282 "rcparse.y"
    {
	    /* This is just the absence of VIRTKEY.  */
	    (yyval.is) = 0;
	  }
    break;

  case 30:
/* Line 1787 of yacc.c  */
#line 287 "rcparse.y"
    {
	    (yyval.is) = ACC_NOINVERT;
	  }
    break;

  case 31:
/* Line 1787 of yacc.c  */
#line 291 "rcparse.y"
    {
	    (yyval.is) = ACC_SHIFT;
	  }
    break;

  case 32:
/* Line 1787 of yacc.c  */
#line 295 "rcparse.y"
    {
	    (yyval.is) = ACC_CONTROL;
	  }
    break;

  case 33:
/* Line 1787 of yacc.c  */
#line 299 "rcparse.y"
    {
	    (yyval.is) = ACC_ALT;
	  }
    break;

  case 34:
/* Line 1787 of yacc.c  */
#line 308 "rcparse.y"
    {
	    define_bitmap ((yyvsp[(1) - (4)].id), &(yyvsp[(3) - (4)].res_info), (yyvsp[(4) - (4)].s));
	    if (yychar != YYEMPTY)
	      YYERROR;
	    rcparse_discard_strings ();
	  }
    break;

  case 35:
/* Line 1787 of yacc.c  */
#line 320 "rcparse.y"
    {
	    define_cursor ((yyvsp[(1) - (4)].id), &(yyvsp[(3) - (4)].res_info), (yyvsp[(4) - (4)].s));
	    if (yychar != YYEMPTY)
	      YYERROR;
	    rcparse_discard_strings ();
	  }
    break;

  case 36:
/* Line 1787 of yacc.c  */
#line 333 "rcparse.y"
    {
	      memset (&dialog, 0, sizeof dialog);
	      dialog.x = (yyvsp[(5) - (8)].il);
	      dialog.y = (yyvsp[(6) - (8)].il);
	      dialog.width = (yyvsp[(7) - (8)].il);
	      dialog.height = (yyvsp[(8) - (8)].il);
	      dialog.style = WS_POPUP | WS_BORDER | WS_SYSMENU;
	      dialog.exstyle = (yyvsp[(4) - (8)].il);
	      dialog.menu.named = 1;
	      dialog.class.named = 1;
	      dialog.font = NULL;
	      dialog.ex = NULL;
	      dialog.controls = NULL;
	      sub_res_info = (yyvsp[(3) - (8)].res_info);
	      style = 0;
	    }
    break;

  case 37:
/* Line 1787 of yacc.c  */
#line 350 "rcparse.y"
    {
	    define_dialog ((yyvsp[(1) - (13)].id), &sub_res_info, &dialog);
	    if (yychar != YYEMPTY)
	      YYERROR;
	    rcparse_discard_strings ();
	  }
    break;

  case 38:
/* Line 1787 of yacc.c  */
#line 358 "rcparse.y"
    {
	      memset (&dialog, 0, sizeof dialog);
	      dialog.x = (yyvsp[(5) - (8)].il);
	      dialog.y = (yyvsp[(6) - (8)].il);
	      dialog.width = (yyvsp[(7) - (8)].il);
	      dialog.height = (yyvsp[(8) - (8)].il);
	      dialog.style = WS_POPUP | WS_BORDER | WS_SYSMENU;
	      dialog.exstyle = (yyvsp[(4) - (8)].il);
	      dialog.menu.named = 1;
	      dialog.class.named = 1;
	      dialog.font = NULL;
	      dialog.ex = ((struct dialog_ex *)
			   res_alloc (sizeof (struct dialog_ex)));
	      memset (dialog.ex, 0, sizeof (struct dialog_ex));
	      dialog.controls = NULL;
	      sub_res_info = (yyvsp[(3) - (8)].res_info);
	      style = 0;
	    }
    break;

  case 39:
/* Line 1787 of yacc.c  */
#line 377 "rcparse.y"
    {
	    define_dialog ((yyvsp[(1) - (13)].id), &sub_res_info, &dialog);
	    if (yychar != YYEMPTY)
	      YYERROR;
	    rcparse_discard_strings ();
	  }
    break;

  case 40:
/* Line 1787 of yacc.c  */
#line 385 "rcparse.y"
    {
	      memset (&dialog, 0, sizeof dialog);
	      dialog.x = (yyvsp[(5) - (9)].il);
	      dialog.y = (yyvsp[(6) - (9)].il);
	      dialog.width = (yyvsp[(7) - (9)].il);
	      dialog.height = (yyvsp[(8) - (9)].il);
	      dialog.style = (WS_POPUP | WS_BORDER | WS_SYSMENU);
	      dialog.exstyle = (yyvsp[(4) - (9)].il);
	      dialog.menu.named = 1;
	      dialog.class.named = 1;
	      dialog.font = NULL;
	      dialog.ex = ((struct dialog_ex *)
			   res_alloc (sizeof (struct dialog_ex)));
	      memset (dialog.ex, 0, sizeof (struct dialog_ex));
	      dialog.ex->help = (yyvsp[(9) - (9)].il);
	      dialog.controls = NULL;
	      sub_res_info = (yyvsp[(3) - (9)].res_info);
	      style = 0;
	    }
    break;

  case 41:
/* Line 1787 of yacc.c  */
#line 405 "rcparse.y"
    {
	    define_dialog ((yyvsp[(1) - (14)].id), &sub_res_info, &dialog);
	    if (yychar != YYEMPTY)
	      YYERROR;
	    rcparse_discard_strings ();
	  }
    break;

  case 42:
/* Line 1787 of yacc.c  */
#line 415 "rcparse.y"
    {
	    (yyval.il) = 0;
	  }
    break;

  case 43:
/* Line 1787 of yacc.c  */
#line 419 "rcparse.y"
    {
	    (yyval.il) = (yyvsp[(3) - (3)].il);
	  }
    break;

  case 45:
/* Line 1787 of yacc.c  */
#line 427 "rcparse.y"
    {
	    dialog.style |= WS_CAPTION;
	    style |= WS_CAPTION;
	    unicode_from_ascii_old((int *)NULL, &dialog.caption, (yyvsp[(3) - (3)].s));
	  }
    break;

  case 46:
/* Line 1787 of yacc.c  */
#line 433 "rcparse.y"
    {
	    dialog.class = (yyvsp[(3) - (3)].id);
	  }
    break;

  case 47:
/* Line 1787 of yacc.c  */
#line 438 "rcparse.y"
    {
	    dialog.style = style;
	  }
    break;

  case 48:
/* Line 1787 of yacc.c  */
#line 442 "rcparse.y"
    {
	    dialog.exstyle = (yyvsp[(3) - (3)].il);
	  }
    break;

  case 49:
/* Line 1787 of yacc.c  */
#line 446 "rcparse.y"
    {
	    res_string_to_id(& dialog.class, (yyvsp[(3) - (3)].s));
	  }
    break;

  case 50:
/* Line 1787 of yacc.c  */
#line 450 "rcparse.y"
    {
	    dialog.style |= DS_SETFONT;
	    style |= DS_SETFONT;
	    dialog.pointsize = (yyvsp[(3) - (5)].il);
	    unicode_from_ascii_old((int *)NULL, &dialog.font, (yyvsp[(5) - (5)].s));
	    if (dialog.ex != NULL)
	      {
		dialog.ex->weight = 0;
		dialog.ex->italic = 0;
		dialog.ex->charset = 1;
	      }
	  }
    break;

  case 51:
/* Line 1787 of yacc.c  */
#line 463 "rcparse.y"
    {
	    dialog.style |= DS_SETFONT;
	    style |= DS_SETFONT;
	    dialog.pointsize = (yyvsp[(3) - (6)].il);
	    unicode_from_ascii_old((int *)NULL, &dialog.font, (yyvsp[(5) - (6)].s));
	    if (dialog.ex == NULL)
	      rcparse_warning(_("extended FONT requires DIALOGEX"));
	    else
	      {
		dialog.ex->weight = (yyvsp[(6) - (6)].il);
		dialog.ex->italic = 0;
		dialog.ex->charset = 1;
	      }
	  }
    break;

  case 52:
/* Line 1787 of yacc.c  */
#line 478 "rcparse.y"
    {
	    dialog.style |= DS_SETFONT;
	    style |= DS_SETFONT;
	    dialog.pointsize = (yyvsp[(3) - (7)].il);
	    unicode_from_ascii_old((int *)NULL, &dialog.font, (yyvsp[(5) - (7)].s));
	    if (dialog.ex == NULL)
	      rcparse_warning(_("extended FONT requires DIALOGEX"));
	    else
	      {
		dialog.ex->weight = (yyvsp[(6) - (7)].il);
		dialog.ex->italic = (yyvsp[(7) - (7)].il);
		dialog.ex->charset = 1;
	      }
	  }
    break;

  case 53:
/* Line 1787 of yacc.c  */
#line 493 "rcparse.y"
    {
	    dialog.style |= DS_SETFONT;
	    style |= DS_SETFONT;
	    dialog.pointsize = (yyvsp[(3) - (8)].il);
	    unicode_from_ascii_old((int *)NULL, &dialog.font, (yyvsp[(5) - (8)].s));
	    if (dialog.ex == NULL)
	      rcparse_warning(_("extended FONT requires DIALOGEX"));
	    else
	      {
		dialog.ex->weight = (yyvsp[(6) - (8)].il);
		dialog.ex->italic = (yyvsp[(7) - (8)].il);
		dialog.ex->charset = (yyvsp[(8) - (8)].il);
	      }
	  }
    break;

  case 54:
/* Line 1787 of yacc.c  */
#line 508 "rcparse.y"
    {
	    dialog.menu = (yyvsp[(3) - (3)].id);
	  }
    break;

  case 55:
/* Line 1787 of yacc.c  */
#line 512 "rcparse.y"
    {
	    sub_res_info.characteristics = (yyvsp[(3) - (3)].il);
	  }
    break;

  case 56:
/* Line 1787 of yacc.c  */
#line 516 "rcparse.y"
    {
	    sub_res_info.language = (yyvsp[(3) - (4)].il) | ((yyvsp[(4) - (4)].il) << SUBLANG_SHIFT);
	  }
    break;

  case 57:
/* Line 1787 of yacc.c  */
#line 520 "rcparse.y"
    {
	    sub_res_info.version = (yyvsp[(3) - (3)].il);
	  }
    break;

  case 59:
/* Line 1787 of yacc.c  */
#line 528 "rcparse.y"
    {
	    struct dialog_control **pp;

	    for (pp = &dialog.controls; *pp != NULL; pp = &(*pp)->next)
	      ;
	    *pp = (yyvsp[(2) - (2)].dialog_control);
	  }
    break;

  case 60:
/* Line 1787 of yacc.c  */
#line 539 "rcparse.y"
    {
	      default_style = BS_AUTO3STATE | WS_TABSTOP;
	      base_style = BS_AUTO3STATE;
	      class = CTL_BUTTON;
	      res_text_field = (yyvsp[(2) - (2)].id);
	    }
    break;

  case 61:
/* Line 1787 of yacc.c  */
#line 546 "rcparse.y"
    {
	    (yyval.dialog_control) = (yyvsp[(4) - (4)].dialog_control);
	  }
    break;

  case 62:
/* Line 1787 of yacc.c  */
#line 550 "rcparse.y"
    {
	      default_style = BS_AUTOCHECKBOX | WS_TABSTOP;
	      base_style = BS_AUTOCHECKBOX;
	      class = CTL_BUTTON;
	      res_text_field = (yyvsp[(2) - (2)].id);
	    }
    break;

  case 63:
/* Line 1787 of yacc.c  */
#line 557 "rcparse.y"
    {
	    (yyval.dialog_control) = (yyvsp[(4) - (4)].dialog_control);
	  }
    break;

  case 64:
/* Line 1787 of yacc.c  */
#line 561 "rcparse.y"
    {
	      default_style = BS_AUTORADIOBUTTON | WS_TABSTOP;
	      base_style = BS_AUTORADIOBUTTON;
	      class = CTL_BUTTON;
	      res_text_field = (yyvsp[(2) - (2)].id);
	    }
    break;

  case 65:
/* Line 1787 of yacc.c  */
#line 568 "rcparse.y"
    {
	    (yyval.dialog_control) = (yyvsp[(4) - (4)].dialog_control);
	  }
    break;

  case 66:
/* Line 1787 of yacc.c  */
#line 572 "rcparse.y"
    {
	      default_style = ES_LEFT | WS_BORDER | WS_TABSTOP;
	      base_style = ES_LEFT | WS_BORDER | WS_TABSTOP;
	      class = CTL_EDIT;
	      res_text_field = (yyvsp[(2) - (2)].id);
	    }
    break;

  case 67:
/* Line 1787 of yacc.c  */
#line 579 "rcparse.y"
    {
	    (yyval.dialog_control) = (yyvsp[(4) - (4)].dialog_control);
	    if (dialog.ex == NULL)
	      rcparse_warning (_("BEDIT requires DIALOGEX"));
	    res_string_to_id (&(yyval.dialog_control)->class, "BEDIT");
	  }
    break;

  case 68:
/* Line 1787 of yacc.c  */
#line 586 "rcparse.y"
    {
	      default_style = BS_CHECKBOX | WS_TABSTOP;
	      base_style = BS_CHECKBOX | WS_TABSTOP;
	      class = CTL_BUTTON;
	      res_text_field = (yyvsp[(2) - (2)].id);
	    }
    break;

  case 69:
/* Line 1787 of yacc.c  */
#line 593 "rcparse.y"
    {
	    (yyval.dialog_control) = (yyvsp[(4) - (4)].dialog_control);
	  }
    break;

  case 70:
/* Line 1787 of yacc.c  */
#line 597 "rcparse.y"
    {
	      /* This is as per MSDN documentation.  With some (???)
		 versions of MS rc.exe their is no default style.  */
	      default_style = CBS_SIMPLE | WS_TABSTOP;
	      base_style = 0;
	      class = CTL_COMBOBOX;
	      res_text_field = res_null_text;
	    }
    break;

  case 71:
/* Line 1787 of yacc.c  */
#line 606 "rcparse.y"
    {
	    (yyval.dialog_control) = (yyvsp[(3) - (3)].dialog_control);
	  }
    break;

  case 72:
/* Line 1787 of yacc.c  */
#line 611 "rcparse.y"
    {
	    (yyval.dialog_control) = define_control ((yyvsp[(2) - (11)].id), (yyvsp[(3) - (11)].il), (yyvsp[(6) - (11)].il), (yyvsp[(7) - (11)].il), (yyvsp[(8) - (11)].il), (yyvsp[(9) - (11)].il), (yyvsp[(4) - (11)].il), style, (yyvsp[(10) - (11)].il));
	    if ((yyvsp[(11) - (11)].rcdata_item) != NULL)
	      {
		if (dialog.ex == NULL)
		  rcparse_warning (_("control data requires DIALOGEX"));
		(yyval.dialog_control)->data = (yyvsp[(11) - (11)].rcdata_item);
	      }
	  }
    break;

  case 73:
/* Line 1787 of yacc.c  */
#line 622 "rcparse.y"
    {
	    (yyval.dialog_control) = define_control ((yyvsp[(2) - (12)].id), (yyvsp[(3) - (12)].il), (yyvsp[(6) - (12)].il), (yyvsp[(7) - (12)].il), (yyvsp[(8) - (12)].il), (yyvsp[(9) - (12)].il), (yyvsp[(4) - (12)].il), style, (yyvsp[(10) - (12)].il));
	    if (dialog.ex == NULL)
	      rcparse_warning (_("help ID requires DIALOGEX"));
	    (yyval.dialog_control)->help = (yyvsp[(11) - (12)].il);
	    (yyval.dialog_control)->data = (yyvsp[(12) - (12)].rcdata_item);
	  }
    break;

  case 74:
/* Line 1787 of yacc.c  */
#line 631 "rcparse.y"
    {
	    (yyval.dialog_control) = define_control ((yyvsp[(2) - (12)].id), (yyvsp[(3) - (12)].il), (yyvsp[(7) - (12)].il), (yyvsp[(8) - (12)].il), (yyvsp[(9) - (12)].il), (yyvsp[(10) - (12)].il), 0, style, (yyvsp[(11) - (12)].il));
	    if ((yyvsp[(12) - (12)].rcdata_item) != NULL)
	      {
		if (dialog.ex == NULL)
		  rcparse_warning ("control data requires DIALOGEX");
		(yyval.dialog_control)->data = (yyvsp[(12) - (12)].rcdata_item);
	      }
	    (yyval.dialog_control)->class.named = 1;
  	    unicode_from_ascii_old(&(yyval.dialog_control)->class.u.n.length, &(yyval.dialog_control)->class.u.n.name, (yyvsp[(5) - (12)].s));
	  }
    break;

  case 75:
/* Line 1787 of yacc.c  */
#line 644 "rcparse.y"
    {
	    (yyval.dialog_control) = define_control((yyvsp[(2) - (13)].id), (yyvsp[(3) - (13)].il), (yyvsp[(7) - (13)].il), (yyvsp[(8) - (13)].il), (yyvsp[(9) - (13)].il), (yyvsp[(10) - (13)].il), 0, style, (yyvsp[(11) - (13)].il));
	    if (dialog.ex == NULL)
	      rcparse_warning("help ID requires DIALOGEX");
	    (yyval.dialog_control)->help = (yyvsp[(12) - (13)].il);
	    (yyval.dialog_control)->data = (yyvsp[(13) - (13)].rcdata_item);
	    (yyval.dialog_control)->class.named = 1;
  	    unicode_from_ascii_old(&(yyval.dialog_control)->class.u.n.length, &(yyval.dialog_control)->class.u.n.name, (yyvsp[(5) - (13)].s));
	  }
    break;

  case 76:
/* Line 1787 of yacc.c  */
#line 654 "rcparse.y"
    {
	      default_style = SS_CENTER | WS_GROUP;
	      base_style = SS_CENTER;
	      class = CTL_STATIC;
	      res_text_field = (yyvsp[(2) - (2)].id);
	    }
    break;

  case 77:
/* Line 1787 of yacc.c  */
#line 661 "rcparse.y"
    {
	    (yyval.dialog_control) = (yyvsp[(4) - (4)].dialog_control);
	  }
    break;

  case 78:
/* Line 1787 of yacc.c  */
#line 665 "rcparse.y"
    {
	      default_style = BS_DEFPUSHBUTTON | WS_TABSTOP;
	      base_style = BS_DEFPUSHBUTTON | WS_TABSTOP;
	      class = CTL_BUTTON;
	      res_text_field = (yyvsp[(2) - (2)].id);
	    }
    break;

  case 79:
/* Line 1787 of yacc.c  */
#line 672 "rcparse.y"
    {
	    (yyval.dialog_control) = (yyvsp[(4) - (4)].dialog_control);
	  }
    break;

  case 80:
/* Line 1787 of yacc.c  */
#line 676 "rcparse.y"
    {
	      default_style = ES_LEFT | WS_BORDER | WS_TABSTOP;
	      base_style = ES_LEFT | WS_BORDER | WS_TABSTOP;
	      class = CTL_EDIT;
	      res_text_field = res_null_text;
	    }
    break;

  case 81:
/* Line 1787 of yacc.c  */
#line 683 "rcparse.y"
    {
	    (yyval.dialog_control) = (yyvsp[(3) - (3)].dialog_control);
	  }
    break;

  case 82:
/* Line 1787 of yacc.c  */
#line 687 "rcparse.y"
    {
	      default_style = BS_GROUPBOX;
	      base_style = BS_GROUPBOX;
	      class = CTL_BUTTON;
	      res_text_field = (yyvsp[(2) - (2)].id);
	    }
    break;

  case 83:
/* Line 1787 of yacc.c  */
#line 694 "rcparse.y"
    {
	    (yyval.dialog_control) = (yyvsp[(4) - (4)].dialog_control);
	  }
    break;

  case 84:
/* Line 1787 of yacc.c  */
#line 698 "rcparse.y"
    {
	      default_style = ES_LEFT | WS_BORDER | WS_TABSTOP;
	      base_style = ES_LEFT | WS_BORDER | WS_TABSTOP;
	      class = CTL_EDIT;
	      res_text_field = (yyvsp[(2) - (2)].id);
	    }
    break;

  case 85:
/* Line 1787 of yacc.c  */
#line 705 "rcparse.y"
    {
	    (yyval.dialog_control) = (yyvsp[(4) - (4)].dialog_control);
	    if (dialog.ex == NULL)
	      rcparse_warning (_("IEDIT requires DIALOGEX"));
	    res_string_to_id (&(yyval.dialog_control)->class, "HEDIT");
	  }
    break;

  case 86:
/* Line 1787 of yacc.c  */
#line 712 "rcparse.y"
    {
	    (yyval.dialog_control) = define_icon_control ((yyvsp[(2) - (6)].id), (yyvsp[(3) - (6)].il), (yyvsp[(4) - (6)].il), (yyvsp[(5) - (6)].il), 0, 0, 0, (yyvsp[(6) - (6)].rcdata_item),
				      dialog.ex);
          }
    break;

  case 87:
/* Line 1787 of yacc.c  */
#line 718 "rcparse.y"
    {
	    (yyval.dialog_control) = define_icon_control ((yyvsp[(2) - (8)].id), (yyvsp[(3) - (8)].il), (yyvsp[(4) - (8)].il), (yyvsp[(5) - (8)].il), 0, 0, 0, (yyvsp[(8) - (8)].rcdata_item),
				      dialog.ex);
          }
    break;

  case 88:
/* Line 1787 of yacc.c  */
#line 724 "rcparse.y"
    {
	    (yyval.dialog_control) = define_icon_control ((yyvsp[(2) - (10)].id), (yyvsp[(3) - (10)].il), (yyvsp[(4) - (10)].il), (yyvsp[(5) - (10)].il), style, (yyvsp[(9) - (10)].il), 0, (yyvsp[(10) - (10)].rcdata_item),
				      dialog.ex);
          }
    break;

  case 89:
/* Line 1787 of yacc.c  */
#line 730 "rcparse.y"
    {
	    (yyval.dialog_control) = define_icon_control ((yyvsp[(2) - (11)].id), (yyvsp[(3) - (11)].il), (yyvsp[(4) - (11)].il), (yyvsp[(5) - (11)].il), style, (yyvsp[(9) - (11)].il), (yyvsp[(10) - (11)].il), (yyvsp[(11) - (11)].rcdata_item),
				      dialog.ex);
          }
    break;

  case 90:
/* Line 1787 of yacc.c  */
#line 735 "rcparse.y"
    {
	      default_style = ES_LEFT | WS_BORDER | WS_TABSTOP;
	      base_style = ES_LEFT | WS_BORDER | WS_TABSTOP;
	      class = CTL_EDIT;
	      res_text_field = (yyvsp[(2) - (2)].id);
	    }
    break;

  case 91:
/* Line 1787 of yacc.c  */
#line 742 "rcparse.y"
    {
	    (yyval.dialog_control) = (yyvsp[(4) - (4)].dialog_control);
	    if (dialog.ex == NULL)
	      rcparse_warning (_("IEDIT requires DIALOGEX"));
	    res_string_to_id (&(yyval.dialog_control)->class, "IEDIT");
	  }
    break;

  case 92:
/* Line 1787 of yacc.c  */
#line 749 "rcparse.y"
    {
	      default_style = LBS_NOTIFY | WS_BORDER;
	      base_style = LBS_NOTIFY | WS_BORDER;
	      class = CTL_LISTBOX;
	      res_text_field = res_null_text;
	    }
    break;

  case 93:
/* Line 1787 of yacc.c  */
#line 756 "rcparse.y"
    {
	    (yyval.dialog_control) = (yyvsp[(3) - (3)].dialog_control);
	  }
    break;

  case 94:
/* Line 1787 of yacc.c  */
#line 760 "rcparse.y"
    {
	      default_style = SS_LEFT | WS_GROUP;
	      base_style = SS_LEFT;
	      class = CTL_STATIC;
	      res_text_field = (yyvsp[(2) - (2)].id);
	    }
    break;

  case 95:
/* Line 1787 of yacc.c  */
#line 767 "rcparse.y"
    {
	    (yyval.dialog_control) = (yyvsp[(4) - (4)].dialog_control);
	  }
    break;

  case 96:
/* Line 1787 of yacc.c  */
#line 771 "rcparse.y"
    {
	      default_style = BS_PUSHBOX | WS_TABSTOP;
	      base_style = BS_PUSHBOX;
	      class = CTL_BUTTON;
	    }
    break;

  case 97:
/* Line 1787 of yacc.c  */
#line 777 "rcparse.y"
    {
	    (yyval.dialog_control) = (yyvsp[(4) - (4)].dialog_control);
	  }
    break;

  case 98:
/* Line 1787 of yacc.c  */
#line 781 "rcparse.y"
    {
	      default_style = BS_PUSHBUTTON | WS_TABSTOP;
	      base_style = BS_PUSHBUTTON | WS_TABSTOP;
	      class = CTL_BUTTON;
	      res_text_field = (yyvsp[(2) - (2)].id);
	    }
    break;

  case 99:
/* Line 1787 of yacc.c  */
#line 788 "rcparse.y"
    {
	    (yyval.dialog_control) = (yyvsp[(4) - (4)].dialog_control);
	  }
    break;

  case 100:
/* Line 1787 of yacc.c  */
#line 792 "rcparse.y"
    {
	      default_style = BS_RADIOBUTTON | WS_TABSTOP;
	      base_style = BS_RADIOBUTTON;
	      class = CTL_BUTTON;
	      res_text_field = (yyvsp[(2) - (2)].id);
	    }
    break;

  case 101:
/* Line 1787 of yacc.c  */
#line 799 "rcparse.y"
    {
	    (yyval.dialog_control) = (yyvsp[(4) - (4)].dialog_control);
	  }
    break;

  case 102:
/* Line 1787 of yacc.c  */
#line 803 "rcparse.y"
    {
	      default_style = SS_RIGHT | WS_GROUP;
	      base_style = SS_RIGHT;
	      class = CTL_STATIC;
	      res_text_field = (yyvsp[(2) - (2)].id);
	    }
    break;

  case 103:
/* Line 1787 of yacc.c  */
#line 810 "rcparse.y"
    {
	    (yyval.dialog_control) = (yyvsp[(4) - (4)].dialog_control);
	  }
    break;

  case 104:
/* Line 1787 of yacc.c  */
#line 814 "rcparse.y"
    {
	      default_style = SBS_HORZ;
	      base_style = 0;
	      class = CTL_SCROLLBAR;
	      res_text_field = res_null_text;
	    }
    break;

  case 105:
/* Line 1787 of yacc.c  */
#line 821 "rcparse.y"
    {
	    (yyval.dialog_control) = (yyvsp[(3) - (3)].dialog_control);
	  }
    break;

  case 106:
/* Line 1787 of yacc.c  */
#line 825 "rcparse.y"
    {
	      default_style = BS_3STATE | WS_TABSTOP;
	      base_style = BS_3STATE;
	      class = CTL_BUTTON;
	      res_text_field = (yyvsp[(2) - (2)].id);
	    }
    break;

  case 107:
/* Line 1787 of yacc.c  */
#line 832 "rcparse.y"
    {
	    (yyval.dialog_control) = (yyvsp[(4) - (4)].dialog_control);
	  }
    break;

  case 108:
/* Line 1787 of yacc.c  */
#line 837 "rcparse.y"
    { style = WS_CHILD | WS_VISIBLE; }
    break;

  case 109:
/* Line 1787 of yacc.c  */
#line 839 "rcparse.y"
    {
	    (yyval.dialog_control) = define_control ((yyvsp[(2) - (15)].id), (yyvsp[(3) - (15)].il), (yyvsp[(5) - (15)].il), (yyvsp[(7) - (15)].il), (yyvsp[(9) - (15)].il), (yyvsp[(11) - (15)].il), CTL_BUTTON,
				 style, (yyvsp[(15) - (15)].il));
	  }
    break;

  case 110:
/* Line 1787 of yacc.c  */
#line 854 "rcparse.y"
    {
	    (yyval.dialog_control) = define_control (res_text_field, (yyvsp[(1) - (6)].il), (yyvsp[(2) - (6)].il), (yyvsp[(3) - (6)].il), (yyvsp[(4) - (6)].il), (yyvsp[(5) - (6)].il), class,
				 default_style | WS_CHILD | WS_VISIBLE, 0);
	    if ((yyvsp[(6) - (6)].rcdata_item) != NULL)
	      {
		if (dialog.ex == NULL)
		  rcparse_warning (_("control data requires DIALOGEX"));
		(yyval.dialog_control)->data = (yyvsp[(6) - (6)].rcdata_item);
	      }
	  }
    break;

  case 111:
/* Line 1787 of yacc.c  */
#line 866 "rcparse.y"
    {
	    (yyval.dialog_control) = define_control (res_text_field, (yyvsp[(1) - (8)].il), (yyvsp[(2) - (8)].il), (yyvsp[(3) - (8)].il), (yyvsp[(4) - (8)].il), (yyvsp[(5) - (8)].il), class, style, (yyvsp[(7) - (8)].il));
	    if ((yyvsp[(8) - (8)].rcdata_item) != NULL)
	      {
		if (dialog.ex == NULL)
		  rcparse_warning (_("control data requires DIALOGEX"));
		(yyval.dialog_control)->data = (yyvsp[(8) - (8)].rcdata_item);
	      }
	  }
    break;

  case 112:
/* Line 1787 of yacc.c  */
#line 877 "rcparse.y"
    {
	    (yyval.dialog_control) = define_control (res_text_field, (yyvsp[(1) - (9)].il), (yyvsp[(2) - (9)].il), (yyvsp[(3) - (9)].il), (yyvsp[(4) - (9)].il), (yyvsp[(5) - (9)].il), class, style, (yyvsp[(7) - (9)].il));
	    if (dialog.ex == NULL)
	      rcparse_warning (_("help ID requires DIALOGEX"));
	    (yyval.dialog_control)->help = (yyvsp[(8) - (9)].il);
	    (yyval.dialog_control)->data = (yyvsp[(9) - (9)].rcdata_item);
	  }
    break;

  case 113:
/* Line 1787 of yacc.c  */
#line 888 "rcparse.y"
    {
	    res_string_to_id (&(yyval.id), "");
	  }
    break;

  case 114:
/* Line 1787 of yacc.c  */
#line 892 "rcparse.y"
    {
	    (yyval.id).named = 0;
	    (yyval.id).u.id = (yyvsp[(1) - (2)].il);
	  }
    break;

  case 115:
/* Line 1787 of yacc.c  */
#line 897 "rcparse.y"
    {
	    res_string_to_id (&(yyval.id), (yyvsp[(1) - (1)].s));
	  }
    break;

  case 116:
/* Line 1787 of yacc.c  */
#line 901 "rcparse.y"
    {
	    res_string_to_id (&(yyval.id), (yyvsp[(1) - (2)].s));
	  }
    break;

  case 117:
/* Line 1787 of yacc.c  */
#line 908 "rcparse.y"
    {
	    (yyval.rcdata_item) = NULL;
	  }
    break;

  case 118:
/* Line 1787 of yacc.c  */
#line 912 "rcparse.y"
    {
	    (yyval.rcdata_item) = (yyvsp[(2) - (3)].rcdata).first;
	  }
    break;

  case 119:
/* Line 1787 of yacc.c  */
#line 921 "rcparse.y"
    { style = WS_CHILD | WS_VISIBLE; }
    break;

  case 121:
/* Line 1787 of yacc.c  */
#line 927 "rcparse.y"
    { style = SS_ICON | WS_CHILD | WS_VISIBLE; }
    break;

  case 123:
/* Line 1787 of yacc.c  */
#line 933 "rcparse.y"
    { style = base_style | WS_CHILD | WS_VISIBLE; }
    break;

  case 125:
/* Line 1787 of yacc.c  */
#line 941 "rcparse.y"
    {
	    define_font ((yyvsp[(1) - (4)].id), &(yyvsp[(3) - (4)].res_info), (yyvsp[(4) - (4)].s));
	    if (yychar != YYEMPTY)
	      YYERROR;
	    rcparse_discard_strings ();
	  }
    break;

  case 126:
/* Line 1787 of yacc.c  */
#line 953 "rcparse.y"
    {
	    define_icon ((yyvsp[(1) - (4)].id), &(yyvsp[(3) - (4)].res_info), (yyvsp[(4) - (4)].s));
	    if (yychar != YYEMPTY)
	      YYERROR;
	    rcparse_discard_strings ();
	  }
    break;

  case 127:
/* Line 1787 of yacc.c  */
#line 966 "rcparse.y"
    {
	    language = (yyvsp[(2) - (3)].il) | ((yyvsp[(3) - (3)].il) << SUBLANG_SHIFT);
	  }
    break;

  case 128:
/* Line 1787 of yacc.c  */
#line 975 "rcparse.y"
    {
	    define_menu ((yyvsp[(1) - (6)].id), &(yyvsp[(3) - (6)].res_info), (yyvsp[(5) - (6)].menuitem));
	    if (yychar != YYEMPTY)
	      YYERROR;
	    rcparse_discard_strings ();
	  }
    break;

  case 129:
/* Line 1787 of yacc.c  */
#line 985 "rcparse.y"
    {
	    (yyval.menuitem) = NULL;
	  }
    break;

  case 130:
/* Line 1787 of yacc.c  */
#line 989 "rcparse.y"
    {
	    if ((yyvsp[(1) - (2)].menuitem) == NULL)
	      (yyval.menuitem) = (yyvsp[(2) - (2)].menuitem);
	    else
	      {
		struct menuitem **pp;

		for (pp = &(yyvsp[(1) - (2)].menuitem)->next; *pp != NULL; pp = &(*pp)->next)
		  ;
		*pp = (yyvsp[(2) - (2)].menuitem);
		(yyval.menuitem) = (yyvsp[(1) - (2)].menuitem);
	      }
	  }
    break;

  case 131:
/* Line 1787 of yacc.c  */
#line 1006 "rcparse.y"
    {
	    (yyval.menuitem) = define_menuitem ((yyvsp[(2) - (4)].s), (yyvsp[(3) - (4)].il), (yyvsp[(4) - (4)].is), 0, 0, NULL);
	  }
    break;

  case 132:
/* Line 1787 of yacc.c  */
#line 1010 "rcparse.y"
    {
	    (yyval.menuitem) = define_menuitem (NULL, 0, 0, 0, 0, NULL);
	  }
    break;

  case 133:
/* Line 1787 of yacc.c  */
#line 1014 "rcparse.y"
    {
	    (yyval.menuitem) = define_menuitem ((yyvsp[(2) - (6)].s), 0, (yyvsp[(3) - (6)].is), 0, 0, (yyvsp[(5) - (6)].menuitem));
	  }
    break;

  case 134:
/* Line 1787 of yacc.c  */
#line 1021 "rcparse.y"
    {
	    (yyval.is) = 0;
	  }
    break;

  case 135:
/* Line 1787 of yacc.c  */
#line 1025 "rcparse.y"
    {
	    (yyval.is) = (yyvsp[(1) - (3)].is) | (yyvsp[(3) - (3)].is);
	  }
    break;

  case 136:
/* Line 1787 of yacc.c  */
#line 1029 "rcparse.y"
    {
	    (yyval.is) = (yyvsp[(1) - (2)].is) | (yyvsp[(2) - (2)].is);
	  }
    break;

  case 137:
/* Line 1787 of yacc.c  */
#line 1036 "rcparse.y"
    {
	    (yyval.is) = MENUITEM_CHECKED;
	  }
    break;

  case 138:
/* Line 1787 of yacc.c  */
#line 1040 "rcparse.y"
    {
	    (yyval.is) = MENUITEM_GRAYED;
	  }
    break;

  case 139:
/* Line 1787 of yacc.c  */
#line 1044 "rcparse.y"
    {
	    (yyval.is) = MENUITEM_HELP;
	  }
    break;

  case 140:
/* Line 1787 of yacc.c  */
#line 1048 "rcparse.y"
    {
	    (yyval.is) = MENUITEM_INACTIVE;
	  }
    break;

  case 141:
/* Line 1787 of yacc.c  */
#line 1052 "rcparse.y"
    {
	    (yyval.is) = MENUITEM_MENUBARBREAK;
	  }
    break;

  case 142:
/* Line 1787 of yacc.c  */
#line 1056 "rcparse.y"
    {
	    (yyval.is) = MENUITEM_MENUBREAK;
	  }
    break;

  case 143:
/* Line 1787 of yacc.c  */
#line 1065 "rcparse.y"
    {
	    define_menu ((yyvsp[(1) - (6)].id), &(yyvsp[(3) - (6)].res_info), (yyvsp[(5) - (6)].menuitem));
	    if (yychar != YYEMPTY)
	      YYERROR;
	    rcparse_discard_strings ();
	  }
    break;

  case 144:
/* Line 1787 of yacc.c  */
#line 1075 "rcparse.y"
    {
	    (yyval.menuitem) = NULL;
	  }
    break;

  case 145:
/* Line 1787 of yacc.c  */
#line 1079 "rcparse.y"
    {
	    if ((yyvsp[(1) - (2)].menuitem) == NULL)
	      (yyval.menuitem) = (yyvsp[(2) - (2)].menuitem);
	    else
	      {
		struct menuitem **pp;

		for (pp = &(yyvsp[(1) - (2)].menuitem)->next; *pp != NULL; pp = &(*pp)->next)
		  ;
		*pp = (yyvsp[(2) - (2)].menuitem);
		(yyval.menuitem) = (yyvsp[(1) - (2)].menuitem);
	      }
	  }
    break;

  case 146:
/* Line 1787 of yacc.c  */
#line 1096 "rcparse.y"
    {
	    (yyval.menuitem) = define_menuitem ((yyvsp[(2) - (2)].s), 0, 0, 0, 0, NULL);
	  }
    break;

  case 147:
/* Line 1787 of yacc.c  */
#line 1100 "rcparse.y"
    {
	    (yyval.menuitem) = define_menuitem ((yyvsp[(2) - (3)].s), (yyvsp[(3) - (3)].il), 0, 0, 0, NULL);
	  }
    break;

  case 148:
/* Line 1787 of yacc.c  */
#line 1104 "rcparse.y"
    {
	    (yyval.menuitem) = define_menuitem ((yyvsp[(2) - (5)].s), (yyvsp[(3) - (5)].il), (yyvsp[(4) - (5)].il), (yyvsp[(5) - (5)].il), 0, NULL);
	  }
    break;

  case 149:
/* Line 1787 of yacc.c  */
#line 1108 "rcparse.y"
    {
 	    (yyval.menuitem) = define_menuitem (NULL, 0, 0, 0, 0, NULL);
 	  }
    break;

  case 150:
/* Line 1787 of yacc.c  */
#line 1112 "rcparse.y"
    {
	    (yyval.menuitem) = define_menuitem ((yyvsp[(2) - (5)].s), 0, 0, 0, 0, (yyvsp[(4) - (5)].menuitem));
	  }
    break;

  case 151:
/* Line 1787 of yacc.c  */
#line 1116 "rcparse.y"
    {
	    (yyval.menuitem) = define_menuitem ((yyvsp[(2) - (6)].s), (yyvsp[(3) - (6)].il), 0, 0, 0, (yyvsp[(5) - (6)].menuitem));
	  }
    break;

  case 152:
/* Line 1787 of yacc.c  */
#line 1120 "rcparse.y"
    {
	    (yyval.menuitem) = define_menuitem ((yyvsp[(2) - (7)].s), (yyvsp[(3) - (7)].il), (yyvsp[(4) - (7)].il), 0, 0, (yyvsp[(6) - (7)].menuitem));
	  }
    break;

  case 153:
/* Line 1787 of yacc.c  */
#line 1125 "rcparse.y"
    {
	    (yyval.menuitem) = define_menuitem ((yyvsp[(2) - (9)].s), (yyvsp[(3) - (9)].il), (yyvsp[(4) - (9)].il), (yyvsp[(5) - (9)].il), (yyvsp[(6) - (9)].il), (yyvsp[(8) - (9)].menuitem));
	  }
    break;

  case 154:
/* Line 1787 of yacc.c  */
#line 1134 "rcparse.y"
    {
	    define_messagetable ((yyvsp[(1) - (4)].id), &(yyvsp[(3) - (4)].res_info), (yyvsp[(4) - (4)].s));
	    if (yychar != YYEMPTY)
	      YYERROR;
	    rcparse_discard_strings ();
	  }
    break;

  case 155:
/* Line 1787 of yacc.c  */
#line 1146 "rcparse.y"
    {
	    define_rcdata ((yyvsp[(1) - (6)].id), &(yyvsp[(3) - (6)].res_info), (yyvsp[(5) - (6)].rcdata).first);
	    if (yychar != YYEMPTY)
	      YYERROR;
	    rcparse_discard_strings ();
	  }
    break;

  case 156:
/* Line 1787 of yacc.c  */
#line 1153 "rcparse.y"
    {
	    define_rcdata_file ((yyvsp[(1) - (4)].id), &(yyvsp[(3) - (4)].res_info), (yyvsp[(4) - (4)].s));
	    if (yychar != YYEMPTY)
	      YYERROR;
	    rcparse_discard_strings ();
	  }
    break;

  case 157:
/* Line 1787 of yacc.c  */
#line 1165 "rcparse.y"
    {
	    rcparse_rcdata ();
	  }
    break;

  case 158:
/* Line 1787 of yacc.c  */
#line 1169 "rcparse.y"
    {
	    rcparse_normal ();
	    (yyval.rcdata) = (yyvsp[(2) - (2)].rcdata);
	  }
    break;

  case 159:
/* Line 1787 of yacc.c  */
#line 1177 "rcparse.y"
    {
	    (yyval.rcdata).first = NULL;
	    (yyval.rcdata).last = NULL;
	  }
    break;

  case 160:
/* Line 1787 of yacc.c  */
#line 1182 "rcparse.y"
    {
	    (yyval.rcdata) = (yyvsp[(1) - (1)].rcdata);
	  }
    break;

  case 161:
/* Line 1787 of yacc.c  */
#line 1189 "rcparse.y"
    {
	    struct rcdata_item *ri;

	    ri = define_rcdata_string ((yyvsp[(1) - (1)].ss).s, (yyvsp[(1) - (1)].ss).length);
	    (yyval.rcdata).first = ri;
	    (yyval.rcdata).last = ri;
	  }
    break;

  case 162:
/* Line 1787 of yacc.c  */
#line 1197 "rcparse.y"
    {
	    struct rcdata_item *ri;

	    ri = define_rcdata_number ((yyvsp[(1) - (1)].i).val, (yyvsp[(1) - (1)].i).dword);
	    (yyval.rcdata).first = ri;
	    (yyval.rcdata).last = ri;
	  }
    break;

  case 163:
/* Line 1787 of yacc.c  */
#line 1205 "rcparse.y"
    {
	    struct rcdata_item *ri;

	    ri = define_rcdata_string ((yyvsp[(3) - (3)].ss).s, (yyvsp[(3) - (3)].ss).length);
	    (yyval.rcdata).first = (yyvsp[(1) - (3)].rcdata).first;
	    (yyvsp[(1) - (3)].rcdata).last->next = ri;
	    (yyval.rcdata).last = ri;
	  }
    break;

  case 164:
/* Line 1787 of yacc.c  */
#line 1214 "rcparse.y"
    {
	    struct rcdata_item *ri;

	    ri = define_rcdata_number ((yyvsp[(3) - (3)].i).val, (yyvsp[(3) - (3)].i).dword);
	    (yyval.rcdata).first = (yyvsp[(1) - (3)].rcdata).first;
	    (yyvsp[(1) - (3)].rcdata).last->next = ri;
	    (yyval.rcdata).last = ri;
	  }
    break;

  case 165:
/* Line 1787 of yacc.c  */
#line 1228 "rcparse.y"
    { sub_res_info = (yyvsp[(2) - (3)].res_info); }
    break;

  case 168:
/* Line 1787 of yacc.c  */
#line 1235 "rcparse.y"
    {
	    define_stringtable (&sub_res_info, (yyvsp[(2) - (3)].il), (yyvsp[(3) - (3)].s));
	    if (yychar != YYEMPTY)
	      YYERROR;
	    rcparse_discard_strings ();
	  }
    break;

  case 169:
/* Line 1787 of yacc.c  */
#line 1242 "rcparse.y"
    {
	    define_stringtable (&sub_res_info, (yyvsp[(2) - (4)].il), (yyvsp[(4) - (4)].s));
	    if (yychar != YYEMPTY)
	      YYERROR;
	    rcparse_discard_strings ();
	  }
    break;

  case 170:
/* Line 1787 of yacc.c  */
#line 1255 "rcparse.y"
    {
	    define_user_data ((yyvsp[(1) - (6)].id), (yyvsp[(2) - (6)].id), &(yyvsp[(3) - (6)].res_info), (yyvsp[(5) - (6)].rcdata).first);
	    if (yychar != YYEMPTY)
	      YYERROR;
	    rcparse_discard_strings ();
	  }
    break;

  case 171:
/* Line 1787 of yacc.c  */
#line 1262 "rcparse.y"
    {
	    define_user_file ((yyvsp[(1) - (4)].id), (yyvsp[(2) - (4)].id), &(yyvsp[(3) - (4)].res_info), (yyvsp[(4) - (4)].s));
	    if (yychar != YYEMPTY)
	      YYERROR;
	    rcparse_discard_strings ();
	  }
    break;

  case 172:
/* Line 1787 of yacc.c  */
#line 1274 "rcparse.y"
    {
	    define_versioninfo ((yyvsp[(1) - (6)].id), language, (yyvsp[(3) - (6)].fixver), (yyvsp[(5) - (6)].verinfo));
	    if (yychar != YYEMPTY)
	      YYERROR;
	    rcparse_discard_strings ();
	  }
    break;

  case 173:
/* Line 1787 of yacc.c  */
#line 1284 "rcparse.y"
    {
	    (yyval.fixver) = ((struct fixed_versioninfo *)
		  res_alloc (sizeof (struct fixed_versioninfo)));
	    memset ((yyval.fixver), 0, sizeof (struct fixed_versioninfo));
	  }
    break;

  case 174:
/* Line 1787 of yacc.c  */
#line 1290 "rcparse.y"
    {
	    (yyvsp[(1) - (6)].fixver)->file_version_ms = ((yyvsp[(3) - (6)].il) << 16) | (yyvsp[(4) - (6)].il);
	    (yyvsp[(1) - (6)].fixver)->file_version_ls = ((yyvsp[(5) - (6)].il) << 16) | (yyvsp[(6) - (6)].il);
	    (yyval.fixver) = (yyvsp[(1) - (6)].fixver);
	  }
    break;

  case 175:
/* Line 1787 of yacc.c  */
#line 1296 "rcparse.y"
    {
	    (yyvsp[(1) - (6)].fixver)->product_version_ms = ((yyvsp[(3) - (6)].il) << 16) | (yyvsp[(4) - (6)].il);
	    (yyvsp[(1) - (6)].fixver)->product_version_ls = ((yyvsp[(5) - (6)].il) << 16) | (yyvsp[(6) - (6)].il);
	    (yyval.fixver) = (yyvsp[(1) - (6)].fixver);
	  }
    break;

  case 176:
/* Line 1787 of yacc.c  */
#line 1302 "rcparse.y"
    {
	    (yyvsp[(1) - (3)].fixver)->file_flags_mask = (yyvsp[(3) - (3)].il);
	    (yyval.fixver) = (yyvsp[(1) - (3)].fixver);
	  }
    break;

  case 177:
/* Line 1787 of yacc.c  */
#line 1307 "rcparse.y"
    {
	    (yyvsp[(1) - (3)].fixver)->file_flags = (yyvsp[(3) - (3)].il);
	    (yyval.fixver) = (yyvsp[(1) - (3)].fixver);
	  }
    break;

  case 178:
/* Line 1787 of yacc.c  */
#line 1312 "rcparse.y"
    {
	    (yyvsp[(1) - (3)].fixver)->file_os = (yyvsp[(3) - (3)].il);
	    (yyval.fixver) = (yyvsp[(1) - (3)].fixver);
	  }
    break;

  case 179:
/* Line 1787 of yacc.c  */
#line 1317 "rcparse.y"
    {
	    (yyvsp[(1) - (3)].fixver)->file_type = (yyvsp[(3) - (3)].il);
	    (yyval.fixver) = (yyvsp[(1) - (3)].fixver);
	  }
    break;

  case 180:
/* Line 1787 of yacc.c  */
#line 1322 "rcparse.y"
    {
	    (yyvsp[(1) - (3)].fixver)->file_subtype = (yyvsp[(3) - (3)].il);
	    (yyval.fixver) = (yyvsp[(1) - (3)].fixver);
	  }
    break;

  case 181:
/* Line 1787 of yacc.c  */
#line 1336 "rcparse.y"
    {
	    (yyval.verinfo) = NULL;
	  }
    break;

  case 182:
/* Line 1787 of yacc.c  */
#line 1340 "rcparse.y"
    {
	    (yyval.verinfo) = append_ver_stringfileinfo ((yyvsp[(1) - (8)].verinfo), (yyvsp[(4) - (8)].s), (yyvsp[(6) - (8)].verstring));
	  }
    break;

  case 183:
/* Line 1787 of yacc.c  */
#line 1344 "rcparse.y"
    {
	    (yyval.verinfo) = append_ver_varfileinfo ((yyvsp[(1) - (7)].verinfo), (yyvsp[(5) - (7)].s), (yyvsp[(6) - (7)].vervar));
	  }
    break;

  case 184:
/* Line 1787 of yacc.c  */
#line 1351 "rcparse.y"
    {
	    (yyval.verstring) = NULL;
	  }
    break;

  case 185:
/* Line 1787 of yacc.c  */
#line 1355 "rcparse.y"
    {
	    (yyval.verstring) = append_verval ((yyvsp[(1) - (5)].verstring), (yyvsp[(3) - (5)].s), (yyvsp[(5) - (5)].s));
	  }
    break;

  case 186:
/* Line 1787 of yacc.c  */
#line 1362 "rcparse.y"
    {
	    (yyval.vervar) = NULL;
	  }
    break;

  case 187:
/* Line 1787 of yacc.c  */
#line 1366 "rcparse.y"
    {
	    (yyval.vervar) = append_vertrans ((yyvsp[(1) - (3)].vervar), (yyvsp[(2) - (3)].il), (yyvsp[(3) - (3)].il));
	  }
    break;

  case 188:
/* Line 1787 of yacc.c  */
#line 1375 "rcparse.y"
    {
	    (yyval.id).named = 0;
	    (yyval.id).u.id = (yyvsp[(1) - (1)].il);
	  }
    break;

  case 189:
/* Line 1787 of yacc.c  */
#line 1380 "rcparse.y"
    {
	    char *copy, *s;

	    /* It seems that resource ID's are forced to upper case.  */
	    copy = xstrdup ((yyvsp[(1) - (1)].s));
	    for (s = copy; *s != '\0'; s++)
	      *s = TOUPPER (*s);
	    res_string_to_id (&(yyval.id), copy);
	    free (copy);
	  }
    break;

  case 190:
/* Line 1787 of yacc.c  */
#line 1396 "rcparse.y"
    {
	    (yyval.s) = (yyvsp[(1) - (1)].s);
	  }
    break;

  case 191:
/* Line 1787 of yacc.c  */
#line 1400 "rcparse.y"
    {
	    (yyval.s) = (yyvsp[(1) - (2)].s);
	  }
    break;

  case 192:
/* Line 1787 of yacc.c  */
#line 1404 "rcparse.y"
    {
	    (yyval.s) = (yyvsp[(1) - (2)].s);
	  }
    break;

  case 193:
/* Line 1787 of yacc.c  */
#line 1412 "rcparse.y"
    {
	    (yyval.id).named = 0;
	    (yyval.id).u.id = (yyvsp[(1) - (2)].il);
	  }
    break;

  case 194:
/* Line 1787 of yacc.c  */
#line 1417 "rcparse.y"
    {
	    char *copy, *s;

	    /* It seems that resource ID's are forced to upper case.  */
	    copy = xstrdup ((yyvsp[(1) - (1)].s));
	    for (s = copy; *s != '\0'; s++)
	      *s = TOUPPER (*s);
	    res_string_to_id (&(yyval.id), copy);
	    free (copy);
	  }
    break;

  case 195:
/* Line 1787 of yacc.c  */
#line 1434 "rcparse.y"
    {
	    memset (&(yyval.res_info), 0, sizeof (struct res_res_info));
	    (yyval.res_info).language = language;
	    /* FIXME: Is this the right default?  */
	    (yyval.res_info).memflags = MEMFLAG_MOVEABLE | MEMFLAG_PURE | MEMFLAG_DISCARDABLE;
	  }
    break;

  case 196:
/* Line 1787 of yacc.c  */
#line 1441 "rcparse.y"
    {
	    (yyval.res_info) = (yyvsp[(1) - (2)].res_info);
	    (yyval.res_info).memflags |= (yyvsp[(2) - (2)].memflags).on;
	    (yyval.res_info).memflags &=~ (yyvsp[(2) - (2)].memflags).off;
	  }
    break;

  case 197:
/* Line 1787 of yacc.c  */
#line 1447 "rcparse.y"
    {
	    (yyval.res_info) = (yyvsp[(1) - (3)].res_info);
	    (yyval.res_info).characteristics = (yyvsp[(3) - (3)].il);
	  }
    break;

  case 198:
/* Line 1787 of yacc.c  */
#line 1452 "rcparse.y"
    {
	    (yyval.res_info) = (yyvsp[(1) - (4)].res_info);
	    (yyval.res_info).language = (yyvsp[(3) - (4)].il) | ((yyvsp[(4) - (4)].il) << SUBLANG_SHIFT);
	  }
    break;

  case 199:
/* Line 1787 of yacc.c  */
#line 1457 "rcparse.y"
    {
	    (yyval.res_info) = (yyvsp[(1) - (3)].res_info);
	    (yyval.res_info).version = (yyvsp[(3) - (3)].il);
	  }
    break;

  case 200:
/* Line 1787 of yacc.c  */
#line 1467 "rcparse.y"
    {
	    memset (&(yyval.res_info), 0, sizeof (struct res_res_info));
	    (yyval.res_info).language = language;
	    (yyval.res_info).memflags = MEMFLAG_MOVEABLE | MEMFLAG_DISCARDABLE;
	  }
    break;

  case 201:
/* Line 1787 of yacc.c  */
#line 1473 "rcparse.y"
    {
	    (yyval.res_info) = (yyvsp[(1) - (2)].res_info);
	    (yyval.res_info).memflags |= (yyvsp[(2) - (2)].memflags).on;
	    (yyval.res_info).memflags &=~ (yyvsp[(2) - (2)].memflags).off;
	  }
    break;

  case 202:
/* Line 1787 of yacc.c  */
#line 1484 "rcparse.y"
    {
	    memset (&(yyval.res_info), 0, sizeof (struct res_res_info));
	    (yyval.res_info).language = language;
	    (yyval.res_info).memflags = MEMFLAG_MOVEABLE | MEMFLAG_PURE | MEMFLAG_DISCARDABLE;
	  }
    break;

  case 203:
/* Line 1787 of yacc.c  */
#line 1490 "rcparse.y"
    {
	    (yyval.res_info) = (yyvsp[(1) - (2)].res_info);
	    (yyval.res_info).memflags |= (yyvsp[(2) - (2)].memflags).on;
	    (yyval.res_info).memflags &=~ (yyvsp[(2) - (2)].memflags).off;
	  }
    break;

  case 204:
/* Line 1787 of yacc.c  */
#line 1502 "rcparse.y"
    {
	    (yyval.memflags).on = MEMFLAG_MOVEABLE;
	    (yyval.memflags).off = 0;
	  }
    break;

  case 205:
/* Line 1787 of yacc.c  */
#line 1507 "rcparse.y"
    {
	    (yyval.memflags).on = 0;
	    (yyval.memflags).off = MEMFLAG_MOVEABLE;
	  }
    break;

  case 206:
/* Line 1787 of yacc.c  */
#line 1512 "rcparse.y"
    {
	    (yyval.memflags).on = MEMFLAG_PURE;
	    (yyval.memflags).off = 0;
	  }
    break;

  case 207:
/* Line 1787 of yacc.c  */
#line 1517 "rcparse.y"
    {
	    (yyval.memflags).on = 0;
	    (yyval.memflags).off = MEMFLAG_PURE;
	  }
    break;

  case 208:
/* Line 1787 of yacc.c  */
#line 1522 "rcparse.y"
    {
	    (yyval.memflags).on = MEMFLAG_PRELOAD;
	    (yyval.memflags).off = 0;
	  }
    break;

  case 209:
/* Line 1787 of yacc.c  */
#line 1527 "rcparse.y"
    {
	    (yyval.memflags).on = 0;
	    (yyval.memflags).off = MEMFLAG_PRELOAD;
	  }
    break;

  case 210:
/* Line 1787 of yacc.c  */
#line 1532 "rcparse.y"
    {
	    (yyval.memflags).on = MEMFLAG_DISCARDABLE;
	    (yyval.memflags).off = 0;
	  }
    break;

  case 211:
/* Line 1787 of yacc.c  */
#line 1542 "rcparse.y"
    {
	    (yyval.s) = (yyvsp[(1) - (1)].s);
	  }
    break;

  case 212:
/* Line 1787 of yacc.c  */
#line 1546 "rcparse.y"
    {
	    (yyval.s) = (yyvsp[(1) - (1)].s);
	  }
    break;

  case 213:
/* Line 1787 of yacc.c  */
#line 1563 "rcparse.y"
    {
	    style |= (yyvsp[(1) - (1)].il);
	  }
    break;

  case 214:
/* Line 1787 of yacc.c  */
#line 1567 "rcparse.y"
    {
	    style &=~ (yyvsp[(2) - (2)].il);
	  }
    break;

  case 215:
/* Line 1787 of yacc.c  */
#line 1571 "rcparse.y"
    {
	    style |= (yyvsp[(3) - (3)].il);
	  }
    break;

  case 216:
/* Line 1787 of yacc.c  */
#line 1575 "rcparse.y"
    {
	    style &=~ (yyvsp[(4) - (4)].il);
	  }
    break;

  case 217:
/* Line 1787 of yacc.c  */
#line 1582 "rcparse.y"
    {
	    (yyval.il) = (yyvsp[(1) - (1)].i).val;
	  }
    break;

  case 218:
/* Line 1787 of yacc.c  */
#line 1586 "rcparse.y"
    {
	    (yyval.il) = (yyvsp[(2) - (3)].il);
	  }
    break;

  case 219:
/* Line 1787 of yacc.c  */
#line 1595 "rcparse.y"
    {
	    (yyval.il) = 0;
	  }
    break;

  case 220:
/* Line 1787 of yacc.c  */
#line 1599 "rcparse.y"
    {
	    (yyval.il) = (yyvsp[(1) - (1)].il);
	  }
    break;

  case 221:
/* Line 1787 of yacc.c  */
#line 1608 "rcparse.y"
    {
	    (yyval.il) = (yyvsp[(2) - (2)].il);
	  }
    break;

  case 222:
/* Line 1787 of yacc.c  */
#line 1617 "rcparse.y"
    {
	    (yyval.il) = (yyvsp[(1) - (1)].i).val;
	  }
    break;

  case 223:
/* Line 1787 of yacc.c  */
#line 1626 "rcparse.y"
    {
	    (yyval.i) = (yyvsp[(1) - (1)].i);
	  }
    break;

  case 224:
/* Line 1787 of yacc.c  */
#line 1630 "rcparse.y"
    {
	    (yyval.i) = (yyvsp[(2) - (3)].i);
	  }
    break;

  case 225:
/* Line 1787 of yacc.c  */
#line 1634 "rcparse.y"
    {
	    (yyval.i).val = ~ (yyvsp[(2) - (2)].i).val;
	    (yyval.i).dword = (yyvsp[(2) - (2)].i).dword;
	  }
    break;

  case 226:
/* Line 1787 of yacc.c  */
#line 1639 "rcparse.y"
    {
	    (yyval.i).val = - (yyvsp[(2) - (2)].i).val;
	    (yyval.i).dword = (yyvsp[(2) - (2)].i).dword;
	  }
    break;

  case 227:
/* Line 1787 of yacc.c  */
#line 1644 "rcparse.y"
    {
	    (yyval.i).val = (yyvsp[(1) - (3)].i).val * (yyvsp[(3) - (3)].i).val;
	    (yyval.i).dword = (yyvsp[(1) - (3)].i).dword || (yyvsp[(3) - (3)].i).dword;
	  }
    break;

  case 228:
/* Line 1787 of yacc.c  */
#line 1649 "rcparse.y"
    {
	    (yyval.i).val = (yyvsp[(1) - (3)].i).val / (yyvsp[(3) - (3)].i).val;
	    (yyval.i).dword = (yyvsp[(1) - (3)].i).dword || (yyvsp[(3) - (3)].i).dword;
	  }
    break;

  case 229:
/* Line 1787 of yacc.c  */
#line 1654 "rcparse.y"
    {
	    (yyval.i).val = (yyvsp[(1) - (3)].i).val % (yyvsp[(3) - (3)].i).val;
	    (yyval.i).dword = (yyvsp[(1) - (3)].i).dword || (yyvsp[(3) - (3)].i).dword;
	  }
    break;

  case 230:
/* Line 1787 of yacc.c  */
#line 1659 "rcparse.y"
    {
	    (yyval.i).val = (yyvsp[(1) - (3)].i).val + (yyvsp[(3) - (3)].i).val;
	    (yyval.i).dword = (yyvsp[(1) - (3)].i).dword || (yyvsp[(3) - (3)].i).dword;
	  }
    break;

  case 231:
/* Line 1787 of yacc.c  */
#line 1664 "rcparse.y"
    {
	    (yyval.i).val = (yyvsp[(1) - (3)].i).val - (yyvsp[(3) - (3)].i).val;
	    (yyval.i).dword = (yyvsp[(1) - (3)].i).dword || (yyvsp[(3) - (3)].i).dword;
	  }
    break;

  case 232:
/* Line 1787 of yacc.c  */
#line 1669 "rcparse.y"
    {
	    (yyval.i).val = (yyvsp[(1) - (3)].i).val & (yyvsp[(3) - (3)].i).val;
	    (yyval.i).dword = (yyvsp[(1) - (3)].i).dword || (yyvsp[(3) - (3)].i).dword;
	  }
    break;

  case 233:
/* Line 1787 of yacc.c  */
#line 1674 "rcparse.y"
    {
	    (yyval.i).val = (yyvsp[(1) - (3)].i).val ^ (yyvsp[(3) - (3)].i).val;
	    (yyval.i).dword = (yyvsp[(1) - (3)].i).dword || (yyvsp[(3) - (3)].i).dword;
	  }
    break;

  case 234:
/* Line 1787 of yacc.c  */
#line 1679 "rcparse.y"
    {
	    (yyval.i).val = (yyvsp[(1) - (3)].i).val | (yyvsp[(3) - (3)].i).val;
	    (yyval.i).dword = (yyvsp[(1) - (3)].i).dword || (yyvsp[(3) - (3)].i).dword;
	  }
    break;

  case 235:
/* Line 1787 of yacc.c  */
#line 1690 "rcparse.y"
    {
	    (yyval.il) = (yyvsp[(2) - (2)].il);
	  }
    break;

  case 236:
/* Line 1787 of yacc.c  */
#line 1699 "rcparse.y"
    {
	    (yyval.il) = (yyvsp[(1) - (1)].i).val;
	  }
    break;

  case 237:
/* Line 1787 of yacc.c  */
#line 1710 "rcparse.y"
    {
	    (yyval.i) = (yyvsp[(1) - (1)].i);
	  }
    break;

  case 238:
/* Line 1787 of yacc.c  */
#line 1714 "rcparse.y"
    {
	    (yyval.i) = (yyvsp[(2) - (3)].i);
	  }
    break;

  case 239:
/* Line 1787 of yacc.c  */
#line 1718 "rcparse.y"
    {
	    (yyval.i).val = ~ (yyvsp[(2) - (2)].i).val;
	    (yyval.i).dword = (yyvsp[(2) - (2)].i).dword;
	  }
    break;

  case 240:
/* Line 1787 of yacc.c  */
#line 1723 "rcparse.y"
    {
	    (yyval.i).val = (yyvsp[(1) - (3)].i).val * (yyvsp[(3) - (3)].i).val;
	    (yyval.i).dword = (yyvsp[(1) - (3)].i).dword || (yyvsp[(3) - (3)].i).dword;
	  }
    break;

  case 241:
/* Line 1787 of yacc.c  */
#line 1728 "rcparse.y"
    {
	    (yyval.i).val = (yyvsp[(1) - (3)].i).val / (yyvsp[(3) - (3)].i).val;
	    (yyval.i).dword = (yyvsp[(1) - (3)].i).dword || (yyvsp[(3) - (3)].i).dword;
	  }
    break;

  case 242:
/* Line 1787 of yacc.c  */
#line 1733 "rcparse.y"
    {
	    (yyval.i).val = (yyvsp[(1) - (3)].i).val % (yyvsp[(3) - (3)].i).val;
	    (yyval.i).dword = (yyvsp[(1) - (3)].i).dword || (yyvsp[(3) - (3)].i).dword;
	  }
    break;

  case 243:
/* Line 1787 of yacc.c  */
#line 1738 "rcparse.y"
    {
	    (yyval.i).val = (yyvsp[(1) - (3)].i).val + (yyvsp[(3) - (3)].i).val;
	    (yyval.i).dword = (yyvsp[(1) - (3)].i).dword || (yyvsp[(3) - (3)].i).dword;
	  }
    break;

  case 244:
/* Line 1787 of yacc.c  */
#line 1743 "rcparse.y"
    {
	    (yyval.i).val = (yyvsp[(1) - (3)].i).val - (yyvsp[(3) - (3)].i).val;
	    (yyval.i).dword = (yyvsp[(1) - (3)].i).dword || (yyvsp[(3) - (3)].i).dword;
	  }
    break;

  case 245:
/* Line 1787 of yacc.c  */
#line 1748 "rcparse.y"
    {
	    (yyval.i).val = (yyvsp[(1) - (3)].i).val & (yyvsp[(3) - (3)].i).val;
	    (yyval.i).dword = (yyvsp[(1) - (3)].i).dword || (yyvsp[(3) - (3)].i).dword;
	  }
    break;

  case 246:
/* Line 1787 of yacc.c  */
#line 1753 "rcparse.y"
    {
	    (yyval.i).val = (yyvsp[(1) - (3)].i).val ^ (yyvsp[(3) - (3)].i).val;
	    (yyval.i).dword = (yyvsp[(1) - (3)].i).dword || (yyvsp[(3) - (3)].i).dword;
	  }
    break;

  case 247:
/* Line 1787 of yacc.c  */
#line 1758 "rcparse.y"
    {
	    (yyval.i).val = (yyvsp[(1) - (3)].i).val | (yyvsp[(3) - (3)].i).val;
	    (yyval.i).dword = (yyvsp[(1) - (3)].i).dword || (yyvsp[(3) - (3)].i).dword;
	  }
    break;


/* Line 1787 of yacc.c  */
#line 4245 "rcparse.c"
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
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
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

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
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
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


/* Line 2050 of yacc.c  */
#line 1764 "rcparse.y"


/* Set the language from the command line.  */

void
rcparse_set_language (int lang)
{
  language = lang;
}
