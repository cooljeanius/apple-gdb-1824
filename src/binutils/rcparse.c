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
static unsigned long controlclass;
static struct res_id res_text_field;
static unichar null_unichar;

/* This is used for COMBOBOX, LISTBOX and EDITTEXT which
   do not allow resource 'text' field in control definition. */
static const struct res_id res_null_text = { 1, {{0, &null_unichar}}};


#line 130 "rcparse.c"

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
#ifndef YY_YY_RCPARSE_H_INCLUDED
# define YY_YY_RCPARSE_H_INCLUDED
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
    BEG = 258,                     /* BEG  */
    END = 259,                     /* END  */
    ACCELERATORS = 260,            /* ACCELERATORS  */
    VIRTKEY = 261,                 /* VIRTKEY  */
    ASCII = 262,                   /* ASCII  */
    NOINVERT = 263,                /* NOINVERT  */
    SHIFT = 264,                   /* SHIFT  */
    CONTROL = 265,                 /* CONTROL  */
    ALT = 266,                     /* ALT  */
    BITMAP = 267,                  /* BITMAP  */
    CURSOR = 268,                  /* CURSOR  */
    DIALOG = 269,                  /* DIALOG  */
    DIALOGEX = 270,                /* DIALOGEX  */
    EXSTYLE = 271,                 /* EXSTYLE  */
    CAPTION = 272,                 /* CAPTION  */
    CLASS = 273,                   /* CLASS  */
    STYLE = 274,                   /* STYLE  */
    AUTO3STATE = 275,              /* AUTO3STATE  */
    AUTOCHECKBOX = 276,            /* AUTOCHECKBOX  */
    AUTORADIOBUTTON = 277,         /* AUTORADIOBUTTON  */
    CHECKBOX = 278,                /* CHECKBOX  */
    COMBOBOX = 279,                /* COMBOBOX  */
    CTEXT = 280,                   /* CTEXT  */
    DEFPUSHBUTTON = 281,           /* DEFPUSHBUTTON  */
    EDITTEXT = 282,                /* EDITTEXT  */
    GROUPBOX = 283,                /* GROUPBOX  */
    LISTBOX = 284,                 /* LISTBOX  */
    LTEXT = 285,                   /* LTEXT  */
    PUSHBOX = 286,                 /* PUSHBOX  */
    PUSHBUTTON = 287,              /* PUSHBUTTON  */
    RADIOBUTTON = 288,             /* RADIOBUTTON  */
    RTEXT = 289,                   /* RTEXT  */
    SCROLLBAR = 290,               /* SCROLLBAR  */
    STATE3 = 291,                  /* STATE3  */
    USERBUTTON = 292,              /* USERBUTTON  */
    BEDIT = 293,                   /* BEDIT  */
    HEDIT = 294,                   /* HEDIT  */
    IEDIT = 295,                   /* IEDIT  */
    FONT = 296,                    /* FONT  */
    ICON = 297,                    /* ICON  */
    LANGUAGE = 298,                /* LANGUAGE  */
    CHARACTERISTICS = 299,         /* CHARACTERISTICS  */
    VERSIONK = 300,                /* VERSIONK  */
    MENU = 301,                    /* MENU  */
    MENUEX = 302,                  /* MENUEX  */
    MENUITEM = 303,                /* MENUITEM  */
    SEPARATOR = 304,               /* SEPARATOR  */
    POPUP = 305,                   /* POPUP  */
    CHECKED = 306,                 /* CHECKED  */
    GRAYED = 307,                  /* GRAYED  */
    HELP = 308,                    /* HELP  */
    INACTIVE = 309,                /* INACTIVE  */
    MENUBARBREAK = 310,            /* MENUBARBREAK  */
    MENUBREAK = 311,               /* MENUBREAK  */
    MESSAGETABLE = 312,            /* MESSAGETABLE  */
    RCDATA = 313,                  /* RCDATA  */
    STRINGTABLE = 314,             /* STRINGTABLE  */
    VERSIONINFO = 315,             /* VERSIONINFO  */
    FILEVERSION = 316,             /* FILEVERSION  */
    PRODUCTVERSION = 317,          /* PRODUCTVERSION  */
    FILEFLAGSMASK = 318,           /* FILEFLAGSMASK  */
    FILEFLAGS = 319,               /* FILEFLAGS  */
    FILEOS = 320,                  /* FILEOS  */
    FILETYPE = 321,                /* FILETYPE  */
    FILESUBTYPE = 322,             /* FILESUBTYPE  */
    BLOCKSTRINGFILEINFO = 323,     /* BLOCKSTRINGFILEINFO  */
    BLOCKVARFILEINFO = 324,        /* BLOCKVARFILEINFO  */
    VALUE = 325,                   /* VALUE  */
    BLOCK = 326,                   /* BLOCK  */
    MOVEABLE = 327,                /* MOVEABLE  */
    FIXED = 328,                   /* FIXED  */
    PURE = 329,                    /* PURE  */
    IMPURE = 330,                  /* IMPURE  */
    PRELOAD = 331,                 /* PRELOAD  */
    LOADONCALL = 332,              /* LOADONCALL  */
    DISCARDABLE = 333,             /* DISCARDABLE  */
    NOT = 334,                     /* NOT  */
    QUOTEDSTRING = 335,            /* QUOTEDSTRING  */
    STRING = 336,                  /* STRING  */
    NUMBER = 337,                  /* NUMBER  */
    SIZEDSTRING = 338,             /* SIZEDSTRING  */
    IGNORED_TOKEN = 339,           /* IGNORED_TOKEN  */
    NEG = 340                      /* NEG  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
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

#line 304 "rcparse.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_RCPARSE_H_INCLUDED  */
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_BEG = 3,                        /* BEG  */
  YYSYMBOL_END = 4,                        /* END  */
  YYSYMBOL_ACCELERATORS = 5,               /* ACCELERATORS  */
  YYSYMBOL_VIRTKEY = 6,                    /* VIRTKEY  */
  YYSYMBOL_ASCII = 7,                      /* ASCII  */
  YYSYMBOL_NOINVERT = 8,                   /* NOINVERT  */
  YYSYMBOL_SHIFT = 9,                      /* SHIFT  */
  YYSYMBOL_CONTROL = 10,                   /* CONTROL  */
  YYSYMBOL_ALT = 11,                       /* ALT  */
  YYSYMBOL_BITMAP = 12,                    /* BITMAP  */
  YYSYMBOL_CURSOR = 13,                    /* CURSOR  */
  YYSYMBOL_DIALOG = 14,                    /* DIALOG  */
  YYSYMBOL_DIALOGEX = 15,                  /* DIALOGEX  */
  YYSYMBOL_EXSTYLE = 16,                   /* EXSTYLE  */
  YYSYMBOL_CAPTION = 17,                   /* CAPTION  */
  YYSYMBOL_CLASS = 18,                     /* CLASS  */
  YYSYMBOL_STYLE = 19,                     /* STYLE  */
  YYSYMBOL_AUTO3STATE = 20,                /* AUTO3STATE  */
  YYSYMBOL_AUTOCHECKBOX = 21,              /* AUTOCHECKBOX  */
  YYSYMBOL_AUTORADIOBUTTON = 22,           /* AUTORADIOBUTTON  */
  YYSYMBOL_CHECKBOX = 23,                  /* CHECKBOX  */
  YYSYMBOL_COMBOBOX = 24,                  /* COMBOBOX  */
  YYSYMBOL_CTEXT = 25,                     /* CTEXT  */
  YYSYMBOL_DEFPUSHBUTTON = 26,             /* DEFPUSHBUTTON  */
  YYSYMBOL_EDITTEXT = 27,                  /* EDITTEXT  */
  YYSYMBOL_GROUPBOX = 28,                  /* GROUPBOX  */
  YYSYMBOL_LISTBOX = 29,                   /* LISTBOX  */
  YYSYMBOL_LTEXT = 30,                     /* LTEXT  */
  YYSYMBOL_PUSHBOX = 31,                   /* PUSHBOX  */
  YYSYMBOL_PUSHBUTTON = 32,                /* PUSHBUTTON  */
  YYSYMBOL_RADIOBUTTON = 33,               /* RADIOBUTTON  */
  YYSYMBOL_RTEXT = 34,                     /* RTEXT  */
  YYSYMBOL_SCROLLBAR = 35,                 /* SCROLLBAR  */
  YYSYMBOL_STATE3 = 36,                    /* STATE3  */
  YYSYMBOL_USERBUTTON = 37,                /* USERBUTTON  */
  YYSYMBOL_BEDIT = 38,                     /* BEDIT  */
  YYSYMBOL_HEDIT = 39,                     /* HEDIT  */
  YYSYMBOL_IEDIT = 40,                     /* IEDIT  */
  YYSYMBOL_FONT = 41,                      /* FONT  */
  YYSYMBOL_ICON = 42,                      /* ICON  */
  YYSYMBOL_LANGUAGE = 43,                  /* LANGUAGE  */
  YYSYMBOL_CHARACTERISTICS = 44,           /* CHARACTERISTICS  */
  YYSYMBOL_VERSIONK = 45,                  /* VERSIONK  */
  YYSYMBOL_MENU = 46,                      /* MENU  */
  YYSYMBOL_MENUEX = 47,                    /* MENUEX  */
  YYSYMBOL_MENUITEM = 48,                  /* MENUITEM  */
  YYSYMBOL_SEPARATOR = 49,                 /* SEPARATOR  */
  YYSYMBOL_POPUP = 50,                     /* POPUP  */
  YYSYMBOL_CHECKED = 51,                   /* CHECKED  */
  YYSYMBOL_GRAYED = 52,                    /* GRAYED  */
  YYSYMBOL_HELP = 53,                      /* HELP  */
  YYSYMBOL_INACTIVE = 54,                  /* INACTIVE  */
  YYSYMBOL_MENUBARBREAK = 55,              /* MENUBARBREAK  */
  YYSYMBOL_MENUBREAK = 56,                 /* MENUBREAK  */
  YYSYMBOL_MESSAGETABLE = 57,              /* MESSAGETABLE  */
  YYSYMBOL_RCDATA = 58,                    /* RCDATA  */
  YYSYMBOL_STRINGTABLE = 59,               /* STRINGTABLE  */
  YYSYMBOL_VERSIONINFO = 60,               /* VERSIONINFO  */
  YYSYMBOL_FILEVERSION = 61,               /* FILEVERSION  */
  YYSYMBOL_PRODUCTVERSION = 62,            /* PRODUCTVERSION  */
  YYSYMBOL_FILEFLAGSMASK = 63,             /* FILEFLAGSMASK  */
  YYSYMBOL_FILEFLAGS = 64,                 /* FILEFLAGS  */
  YYSYMBOL_FILEOS = 65,                    /* FILEOS  */
  YYSYMBOL_FILETYPE = 66,                  /* FILETYPE  */
  YYSYMBOL_FILESUBTYPE = 67,               /* FILESUBTYPE  */
  YYSYMBOL_BLOCKSTRINGFILEINFO = 68,       /* BLOCKSTRINGFILEINFO  */
  YYSYMBOL_BLOCKVARFILEINFO = 69,          /* BLOCKVARFILEINFO  */
  YYSYMBOL_VALUE = 70,                     /* VALUE  */
  YYSYMBOL_BLOCK = 71,                     /* BLOCK  */
  YYSYMBOL_MOVEABLE = 72,                  /* MOVEABLE  */
  YYSYMBOL_FIXED = 73,                     /* FIXED  */
  YYSYMBOL_PURE = 74,                      /* PURE  */
  YYSYMBOL_IMPURE = 75,                    /* IMPURE  */
  YYSYMBOL_PRELOAD = 76,                   /* PRELOAD  */
  YYSYMBOL_LOADONCALL = 77,                /* LOADONCALL  */
  YYSYMBOL_DISCARDABLE = 78,               /* DISCARDABLE  */
  YYSYMBOL_NOT = 79,                       /* NOT  */
  YYSYMBOL_QUOTEDSTRING = 80,              /* QUOTEDSTRING  */
  YYSYMBOL_STRING = 81,                    /* STRING  */
  YYSYMBOL_NUMBER = 82,                    /* NUMBER  */
  YYSYMBOL_SIZEDSTRING = 83,               /* SIZEDSTRING  */
  YYSYMBOL_IGNORED_TOKEN = 84,             /* IGNORED_TOKEN  */
  YYSYMBOL_85_ = 85,                       /* '|'  */
  YYSYMBOL_86_ = 86,                       /* '^'  */
  YYSYMBOL_87_ = 87,                       /* '&'  */
  YYSYMBOL_88_ = 88,                       /* '+'  */
  YYSYMBOL_89_ = 89,                       /* '-'  */
  YYSYMBOL_90_ = 90,                       /* '*'  */
  YYSYMBOL_91_ = 91,                       /* '/'  */
  YYSYMBOL_92_ = 92,                       /* '%'  */
  YYSYMBOL_93_ = 93,                       /* '~'  */
  YYSYMBOL_NEG = 94,                       /* NEG  */
  YYSYMBOL_95_ = 95,                       /* ','  */
  YYSYMBOL_96_ = 96,                       /* '='  */
  YYSYMBOL_97_ = 97,                       /* '('  */
  YYSYMBOL_98_ = 98,                       /* ')'  */
  YYSYMBOL_YYACCEPT = 99,                  /* $accept  */
  YYSYMBOL_input = 100,                    /* input  */
  YYSYMBOL_accelerator = 101,              /* accelerator  */
  YYSYMBOL_acc_entries = 102,              /* acc_entries  */
  YYSYMBOL_acc_entry = 103,                /* acc_entry  */
  YYSYMBOL_acc_event = 104,                /* acc_event  */
  YYSYMBOL_acc_options = 105,              /* acc_options  */
  YYSYMBOL_acc_option = 106,               /* acc_option  */
  YYSYMBOL_bitmap = 107,                   /* bitmap  */
  YYSYMBOL_cursor = 108,                   /* cursor  */
  YYSYMBOL_dialog = 109,                   /* dialog  */
  YYSYMBOL_110_1 = 110,                    /* $@1  */
  YYSYMBOL_111_2 = 111,                    /* $@2  */
  YYSYMBOL_112_3 = 112,                    /* $@3  */
  YYSYMBOL_exstyle = 113,                  /* exstyle  */
  YYSYMBOL_styles = 114,                   /* styles  */
  YYSYMBOL_controls = 115,                 /* controls  */
  YYSYMBOL_control = 116,                  /* control  */
  YYSYMBOL_117_4 = 117,                    /* $@4  */
  YYSYMBOL_118_5 = 118,                    /* $@5  */
  YYSYMBOL_119_6 = 119,                    /* $@6  */
  YYSYMBOL_120_7 = 120,                    /* $@7  */
  YYSYMBOL_121_8 = 121,                    /* $@8  */
  YYSYMBOL_122_9 = 122,                    /* $@9  */
  YYSYMBOL_123_10 = 123,                   /* $@10  */
  YYSYMBOL_124_11 = 124,                   /* $@11  */
  YYSYMBOL_125_12 = 125,                   /* $@12  */
  YYSYMBOL_126_13 = 126,                   /* $@13  */
  YYSYMBOL_127_14 = 127,                   /* $@14  */
  YYSYMBOL_128_15 = 128,                   /* $@15  */
  YYSYMBOL_129_16 = 129,                   /* $@16  */
  YYSYMBOL_130_17 = 130,                   /* $@17  */
  YYSYMBOL_131_18 = 131,                   /* $@18  */
  YYSYMBOL_132_19 = 132,                   /* $@19  */
  YYSYMBOL_133_20 = 133,                   /* $@20  */
  YYSYMBOL_134_21 = 134,                   /* $@21  */
  YYSYMBOL_135_22 = 135,                   /* $@22  */
  YYSYMBOL_136_23 = 136,                   /* $@23  */
  YYSYMBOL_137_24 = 137,                   /* $@24  */
  YYSYMBOL_control_params = 138,           /* control_params  */
  YYSYMBOL_optresidc = 139,                /* optresidc  */
  YYSYMBOL_opt_control_data = 140,         /* opt_control_data  */
  YYSYMBOL_control_styleexpr = 141,        /* control_styleexpr  */
  YYSYMBOL_142_25 = 142,                   /* $@25  */
  YYSYMBOL_icon_styleexpr = 143,           /* icon_styleexpr  */
  YYSYMBOL_144_26 = 144,                   /* $@26  */
  YYSYMBOL_control_params_styleexpr = 145, /* control_params_styleexpr  */
  YYSYMBOL_146_27 = 146,                   /* $@27  */
  YYSYMBOL_font = 147,                     /* font  */
  YYSYMBOL_icon = 148,                     /* icon  */
  YYSYMBOL_language = 149,                 /* language  */
  YYSYMBOL_menu = 150,                     /* menu  */
  YYSYMBOL_menuitems = 151,                /* menuitems  */
  YYSYMBOL_menuitem = 152,                 /* menuitem  */
  YYSYMBOL_menuitem_flags = 153,           /* menuitem_flags  */
  YYSYMBOL_menuitem_flag = 154,            /* menuitem_flag  */
  YYSYMBOL_menuex = 155,                   /* menuex  */
  YYSYMBOL_menuexitems = 156,              /* menuexitems  */
  YYSYMBOL_menuexitem = 157,               /* menuexitem  */
  YYSYMBOL_messagetable = 158,             /* messagetable  */
  YYSYMBOL_rcdata = 159,                   /* rcdata  */
  YYSYMBOL_optrcdata_data = 160,           /* optrcdata_data  */
  YYSYMBOL_161_28 = 161,                   /* $@28  */
  YYSYMBOL_optrcdata_data_int = 162,       /* optrcdata_data_int  */
  YYSYMBOL_rcdata_data = 163,              /* rcdata_data  */
  YYSYMBOL_stringtable = 164,              /* stringtable  */
  YYSYMBOL_165_29 = 165,                   /* $@29  */
  YYSYMBOL_string_data = 166,              /* string_data  */
  YYSYMBOL_user = 167,                     /* user  */
  YYSYMBOL_versioninfo = 168,              /* versioninfo  */
  YYSYMBOL_fixedverinfo = 169,             /* fixedverinfo  */
  YYSYMBOL_verblocks = 170,                /* verblocks  */
  YYSYMBOL_vervals = 171,                  /* vervals  */
  YYSYMBOL_vertrans = 172,                 /* vertrans  */
  YYSYMBOL_id = 173,                       /* id  */
  YYSYMBOL_resname = 174,                  /* resname  */
  YYSYMBOL_resref = 175,                   /* resref  */
  YYSYMBOL_suboptions = 176,               /* suboptions  */
  YYSYMBOL_memflags_move_discard = 177,    /* memflags_move_discard  */
  YYSYMBOL_memflags_move = 178,            /* memflags_move  */
  YYSYMBOL_memflag = 179,                  /* memflag  */
  YYSYMBOL_file_name = 180,                /* file_name  */
  YYSYMBOL_styleexpr = 181,                /* styleexpr  */
  YYSYMBOL_parennumber = 182,              /* parennumber  */
  YYSYMBOL_optcnumexpr = 183,              /* optcnumexpr  */
  YYSYMBOL_cnumexpr = 184,                 /* cnumexpr  */
  YYSYMBOL_numexpr = 185,                  /* numexpr  */
  YYSYMBOL_sizednumexpr = 186,             /* sizednumexpr  */
  YYSYMBOL_cposnumexpr = 187,              /* cposnumexpr  */
  YYSYMBOL_posnumexpr = 188,               /* posnumexpr  */
  YYSYMBOL_sizedposnumexpr = 189           /* sizedposnumexpr  */
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
typedef yytype_int16 yy_state_t;

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
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   792

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  99
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  91
/* YYNRULES -- Number of rules.  */
#define YYNRULES  247
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  500

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   340


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
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
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
     853,   865,   877,   891,   894,   899,   903,   911,   914,   924,
     923,   930,   929,   936,   935,   943,   955,   968,   977,   988,
     991,  1008,  1012,  1016,  1024,  1027,  1031,  1038,  1042,  1046,
    1050,  1054,  1058,  1067,  1078,  1081,  1098,  1102,  1106,  1110,
    1114,  1118,  1122,  1126,  1136,  1148,  1155,  1168,  1168,  1180,
    1184,  1191,  1199,  1207,  1216,  1231,  1230,  1235,  1237,  1244,
    1257,  1264,  1276,  1287,  1292,  1298,  1304,  1309,  1314,  1319,
    1324,  1339,  1342,  1346,  1354,  1357,  1365,  1368,  1377,  1382,
    1398,  1402,  1406,  1414,  1419,  1437,  1443,  1449,  1454,  1459,
    1470,  1475,  1487,  1492,  1504,  1509,  1514,  1519,  1524,  1529,
    1534,  1544,  1548,  1565,  1569,  1573,  1577,  1584,  1588,  1598,
    1601,  1610,  1619,  1628,  1632,  1636,  1641,  1646,  1651,  1656,
    1661,  1666,  1671,  1676,  1681,  1692,  1701,  1712,  1716,  1720,
    1725,  1730,  1735,  1740,  1745,  1750,  1755,  1760
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
  "\"end of file\"", "error", "\"invalid token\"", "BEG", "END",
  "ACCELERATORS", "VIRTKEY", "ASCII", "NOINVERT", "SHIFT", "CONTROL",
  "ALT", "BITMAP", "CURSOR", "DIALOG", "DIALOGEX", "EXSTYLE", "CAPTION",
  "CLASS", "STYLE", "AUTO3STATE", "AUTOCHECKBOX", "AUTORADIOBUTTON",
  "CHECKBOX", "COMBOBOX", "CTEXT", "DEFPUSHBUTTON", "EDITTEXT", "GROUPBOX",
  "LISTBOX", "LTEXT", "PUSHBOX", "PUSHBUTTON", "RADIOBUTTON", "RTEXT",
  "SCROLLBAR", "STATE3", "USERBUTTON", "BEDIT", "HEDIT", "IEDIT", "FONT",
  "ICON", "LANGUAGE", "CHARACTERISTICS", "VERSIONK", "MENU", "MENUEX",
  "MENUITEM", "SEPARATOR", "POPUP", "CHECKED", "GRAYED", "HELP",
  "INACTIVE", "MENUBARBREAK", "MENUBREAK", "MESSAGETABLE", "RCDATA",
  "STRINGTABLE", "VERSIONINFO", "FILEVERSION", "PRODUCTVERSION",
  "FILEFLAGSMASK", "FILEFLAGS", "FILEOS", "FILETYPE", "FILESUBTYPE",
  "BLOCKSTRINGFILEINFO", "BLOCKVARFILEINFO", "VALUE", "BLOCK", "MOVEABLE",
  "FIXED", "PURE", "IMPURE", "PRELOAD", "LOADONCALL", "DISCARDABLE", "NOT",
  "QUOTEDSTRING", "STRING", "NUMBER", "SIZEDSTRING", "IGNORED_TOKEN",
  "'|'", "'^'", "'&'", "'+'", "'-'", "'*'", "'/'", "'%'", "'~'", "NEG",
  "','", "'='", "'('", "')'", "$accept", "input", "accelerator",
  "acc_entries", "acc_entry", "acc_event", "acc_options", "acc_option",
  "bitmap", "cursor", "dialog", "$@1", "$@2", "$@3", "exstyle", "styles",
  "controls", "control", "$@4", "$@5", "$@6", "$@7", "$@8", "$@9", "$@10",
  "$@11", "$@12", "$@13", "$@14", "$@15", "$@16", "$@17", "$@18", "$@19",
  "$@20", "$@21", "$@22", "$@23", "$@24", "control_params", "optresidc",
  "opt_control_data", "control_styleexpr", "$@25", "icon_styleexpr",
  "$@26", "control_params_styleexpr", "$@27", "font", "icon", "language",
  "menu", "menuitems", "menuitem", "menuitem_flags", "menuitem_flag",
  "menuex", "menuexitems", "menuexitem", "messagetable", "rcdata",
  "optrcdata_data", "$@28", "optrcdata_data_int", "rcdata_data",
  "stringtable", "$@29", "string_data", "user", "versioninfo",
  "fixedverinfo", "verblocks", "vervals", "vertrans", "id", "resname",
  "resref", "suboptions", "memflags_move_discard", "memflags_move",
  "memflag", "file_name", "styleexpr", "parennumber", "optcnumexpr",
  "cnumexpr", "numexpr", "sizednumexpr", "cposnumexpr", "posnumexpr",
  "sizedposnumexpr", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-426)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
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

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
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

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,    10,   147,   168,   169,   232,   233,    11,    12,
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

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
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

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
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

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
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

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
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
  case 18: /* accelerator: id ACCELERATORS suboptions BEG acc_entries END  */
#line 179 "rcparse.y"
          {
	    define_accelerator ((yyvsp[-5].id), &(yyvsp[-3].res_info), (yyvsp[-1].pacc));
	    if (yychar != YYEMPTY)
	      YYERROR;
	    rcparse_discard_strings ();
	  }
#line 1898 "rcparse.c"
    break;

  case 19: /* acc_entries: %empty  */
#line 189 "rcparse.y"
          {
	    (yyval.pacc) = NULL;
	  }
#line 1906 "rcparse.c"
    break;

  case 20: /* acc_entries: acc_entries acc_entry  */
#line 193 "rcparse.y"
          {
	    struct accelerator *a;

	    a = (struct accelerator *) res_alloc (sizeof *a);
	    *a = (yyvsp[0].acc);
	    if ((yyvsp[-1].pacc) == NULL)
	      (yyval.pacc) = a;
	    else
	      {
		struct accelerator **pp;

		for (pp = &(yyvsp[-1].pacc)->next; *pp != NULL; pp = &(*pp)->next)
		  ;
		*pp = a;
		(yyval.pacc) = (yyvsp[-1].pacc);
	      }
	  }
#line 1928 "rcparse.c"
    break;

  case 21: /* acc_entry: acc_event cposnumexpr  */
#line 214 "rcparse.y"
          {
	    (yyval.acc) = (yyvsp[-1].acc);
	    (yyval.acc).id = (yyvsp[0].il);
	  }
#line 1937 "rcparse.c"
    break;

  case 22: /* acc_entry: acc_event cposnumexpr ',' acc_options  */
#line 219 "rcparse.y"
          {
	    (yyval.acc) = (yyvsp[-3].acc);
	    (yyval.acc).id = (yyvsp[-2].il);
	    (yyval.acc).flags |= (yyvsp[0].is);
	    if (((yyval.acc).flags & ACC_VIRTKEY) == 0
		&& ((yyval.acc).flags & (ACC_SHIFT | ACC_CONTROL)) != 0)
	      rcparse_warning (_("inappropriate modifiers for non-VIRTKEY"));
	  }
#line 1950 "rcparse.c"
    break;

  case 23: /* acc_event: QUOTEDSTRING  */
#line 231 "rcparse.y"
          {
	    const char *s = (yyvsp[0].s);
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
#line 1975 "rcparse.c"
    break;

  case 24: /* acc_event: posnumexpr  */
#line 252 "rcparse.y"
          {
	    (yyval.acc).next = NULL;
	    (yyval.acc).flags = 0;
	    (yyval.acc).id = 0;
	    (yyval.acc).key = (yyvsp[0].il);
	  }
#line 1986 "rcparse.c"
    break;

  case 25: /* acc_options: acc_option  */
#line 262 "rcparse.y"
          {
	    (yyval.is) = (yyvsp[0].is);
	  }
#line 1994 "rcparse.c"
    break;

  case 26: /* acc_options: acc_options ',' acc_option  */
#line 266 "rcparse.y"
          {
	    (yyval.is) = (yyvsp[-2].is) | (yyvsp[0].is);
	  }
#line 2002 "rcparse.c"
    break;

  case 27: /* acc_options: acc_options acc_option  */
#line 271 "rcparse.y"
          {
	    (yyval.is) = (yyvsp[-1].is) | (yyvsp[0].is);
	  }
#line 2010 "rcparse.c"
    break;

  case 28: /* acc_option: VIRTKEY  */
#line 278 "rcparse.y"
          {
	    (yyval.is) = ACC_VIRTKEY;
	  }
#line 2018 "rcparse.c"
    break;

  case 29: /* acc_option: ASCII  */
#line 282 "rcparse.y"
          {
	    /* This is just the absence of VIRTKEY.  */
	    (yyval.is) = 0;
	  }
#line 2027 "rcparse.c"
    break;

  case 30: /* acc_option: NOINVERT  */
#line 287 "rcparse.y"
          {
	    (yyval.is) = ACC_NOINVERT;
	  }
#line 2035 "rcparse.c"
    break;

  case 31: /* acc_option: SHIFT  */
#line 291 "rcparse.y"
          {
	    (yyval.is) = ACC_SHIFT;
	  }
#line 2043 "rcparse.c"
    break;

  case 32: /* acc_option: CONTROL  */
#line 295 "rcparse.y"
          {
	    (yyval.is) = ACC_CONTROL;
	  }
#line 2051 "rcparse.c"
    break;

  case 33: /* acc_option: ALT  */
#line 299 "rcparse.y"
          {
	    (yyval.is) = ACC_ALT;
	  }
#line 2059 "rcparse.c"
    break;

  case 34: /* bitmap: id BITMAP memflags_move file_name  */
#line 308 "rcparse.y"
          {
	    define_bitmap ((yyvsp[-3].id), &(yyvsp[-1].res_info), (yyvsp[0].s));
	    if (yychar != YYEMPTY)
	      YYERROR;
	    rcparse_discard_strings ();
	  }
#line 2070 "rcparse.c"
    break;

  case 35: /* cursor: id CURSOR memflags_move_discard file_name  */
#line 320 "rcparse.y"
          {
	    define_cursor ((yyvsp[-3].id), &(yyvsp[-1].res_info), (yyvsp[0].s));
	    if (yychar != YYEMPTY)
	      YYERROR;
	    rcparse_discard_strings ();
	  }
#line 2081 "rcparse.c"
    break;

  case 36: /* $@1: %empty  */
#line 333 "rcparse.y"
            {
	      memset (&dialog, 0, sizeof dialog);
	      dialog.x = (yyvsp[-3].il);
	      dialog.y = (yyvsp[-2].il);
	      dialog.width = (yyvsp[-1].il);
	      dialog.height = (yyvsp[0].il);
	      dialog.style = WS_POPUP | WS_BORDER | WS_SYSMENU;
	      dialog.exstyle = (yyvsp[-4].il);
	      dialog.menu.named = 1;
	      dialog.classname.named = 1;
	      dialog.font = NULL;
	      dialog.ex = NULL;
	      dialog.controls = NULL;
	      sub_res_info = (yyvsp[-5].res_info);
	      style = 0;
	    }
#line 2102 "rcparse.c"
    break;

  case 37: /* dialog: id DIALOG memflags_move exstyle posnumexpr cnumexpr cnumexpr cnumexpr $@1 styles BEG controls END  */
#line 350 "rcparse.y"
          {
	    define_dialog ((yyvsp[-12].id), &sub_res_info, &dialog);
	    if (yychar != YYEMPTY)
	      YYERROR;
	    rcparse_discard_strings ();
	  }
#line 2113 "rcparse.c"
    break;

  case 38: /* $@2: %empty  */
#line 358 "rcparse.y"
            {
	      memset (&dialog, 0, sizeof dialog);
	      dialog.x = (yyvsp[-3].il);
	      dialog.y = (yyvsp[-2].il);
	      dialog.width = (yyvsp[-1].il);
	      dialog.height = (yyvsp[0].il);
	      dialog.style = WS_POPUP | WS_BORDER | WS_SYSMENU;
	      dialog.exstyle = (yyvsp[-4].il);
	      dialog.menu.named = 1;
	      dialog.classname.named = 1;
	      dialog.font = NULL;
	      dialog.ex = ((struct dialog_ex *)
			   res_alloc (sizeof (struct dialog_ex)));
	      memset (dialog.ex, 0, sizeof (struct dialog_ex));
	      dialog.controls = NULL;
	      sub_res_info = (yyvsp[-5].res_info);
	      style = 0;
	    }
#line 2136 "rcparse.c"
    break;

  case 39: /* dialog: id DIALOGEX memflags_move exstyle posnumexpr cnumexpr cnumexpr cnumexpr $@2 styles BEG controls END  */
#line 377 "rcparse.y"
          {
	    define_dialog ((yyvsp[-12].id), &sub_res_info, &dialog);
	    if (yychar != YYEMPTY)
	      YYERROR;
	    rcparse_discard_strings ();
	  }
#line 2147 "rcparse.c"
    break;

  case 40: /* $@3: %empty  */
#line 385 "rcparse.y"
            {
	      memset (&dialog, 0, sizeof dialog);
	      dialog.x = (yyvsp[-4].il);
	      dialog.y = (yyvsp[-3].il);
	      dialog.width = (yyvsp[-2].il);
	      dialog.height = (yyvsp[-1].il);
	      dialog.style = (WS_POPUP | WS_BORDER | WS_SYSMENU);
	      dialog.exstyle = (yyvsp[-5].il);
	      dialog.menu.named = 1;
	      dialog.classname.named = 1;
	      dialog.font = NULL;
	      dialog.ex = ((struct dialog_ex *)
			   res_alloc (sizeof (struct dialog_ex)));
	      memset (dialog.ex, 0, sizeof (struct dialog_ex));
	      dialog.ex->help = (yyvsp[0].il);
	      dialog.controls = NULL;
	      sub_res_info = (yyvsp[-6].res_info);
	      style = 0;
	    }
#line 2171 "rcparse.c"
    break;

  case 41: /* dialog: id DIALOGEX memflags_move exstyle posnumexpr cnumexpr cnumexpr cnumexpr cnumexpr $@3 styles BEG controls END  */
#line 405 "rcparse.y"
          {
	    define_dialog ((yyvsp[-13].id), &sub_res_info, &dialog);
	    if (yychar != YYEMPTY)
	      YYERROR;
	    rcparse_discard_strings ();
	  }
#line 2182 "rcparse.c"
    break;

  case 42: /* exstyle: %empty  */
#line 415 "rcparse.y"
          {
	    (yyval.il) = 0;
	  }
#line 2190 "rcparse.c"
    break;

  case 43: /* exstyle: EXSTYLE '=' numexpr  */
#line 419 "rcparse.y"
          {
	    (yyval.il) = (yyvsp[0].il);
	  }
#line 2198 "rcparse.c"
    break;

  case 45: /* styles: styles CAPTION QUOTEDSTRING  */
#line 427 "rcparse.y"
          {
	    dialog.style |= WS_CAPTION;
	    style |= WS_CAPTION;
	    unicode_from_ascii_old((int *)NULL, &dialog.caption, (yyvsp[0].s));
	  }
#line 2208 "rcparse.c"
    break;

  case 46: /* styles: styles CLASS id  */
#line 433 "rcparse.y"
          {
	    dialog.classname = (yyvsp[0].id);
	  }
#line 2216 "rcparse.c"
    break;

  case 47: /* styles: styles STYLE styleexpr  */
#line 438 "rcparse.y"
          {
	    dialog.style = style;
	  }
#line 2224 "rcparse.c"
    break;

  case 48: /* styles: styles EXSTYLE numexpr  */
#line 442 "rcparse.y"
          {
	    dialog.exstyle = (yyvsp[0].il);
	  }
#line 2232 "rcparse.c"
    break;

  case 49: /* styles: styles CLASS QUOTEDSTRING  */
#line 446 "rcparse.y"
          {
	    res_string_to_id(& dialog.classname, (yyvsp[0].s));
	  }
#line 2240 "rcparse.c"
    break;

  case 50: /* styles: styles FONT numexpr ',' QUOTEDSTRING  */
#line 450 "rcparse.y"
          {
	    dialog.style |= DS_SETFONT;
	    style |= DS_SETFONT;
	    dialog.pointsize = (yyvsp[-2].il);
	    unicode_from_ascii_old((int *)NULL, &dialog.font, (yyvsp[0].s));
	    if (dialog.ex != NULL)
	      {
		dialog.ex->weight = 0;
		dialog.ex->italic = 0;
		dialog.ex->charset = 1;
	      }
	  }
#line 2257 "rcparse.c"
    break;

  case 51: /* styles: styles FONT numexpr ',' QUOTEDSTRING cnumexpr  */
#line 463 "rcparse.y"
          {
	    dialog.style |= DS_SETFONT;
	    style |= DS_SETFONT;
	    dialog.pointsize = (yyvsp[-3].il);
	    unicode_from_ascii_old((int *)NULL, &dialog.font, (yyvsp[-1].s));
	    if (dialog.ex == NULL)
	      rcparse_warning(_("extended FONT requires DIALOGEX"));
	    else
	      {
		dialog.ex->weight = (yyvsp[0].il);
		dialog.ex->italic = 0;
		dialog.ex->charset = 1;
	      }
	  }
#line 2276 "rcparse.c"
    break;

  case 52: /* styles: styles FONT numexpr ',' QUOTEDSTRING cnumexpr cnumexpr  */
#line 478 "rcparse.y"
          {
	    dialog.style |= DS_SETFONT;
	    style |= DS_SETFONT;
	    dialog.pointsize = (yyvsp[-4].il);
	    unicode_from_ascii_old((int *)NULL, &dialog.font, (yyvsp[-2].s));
	    if (dialog.ex == NULL)
	      rcparse_warning(_("extended FONT requires DIALOGEX"));
	    else
	      {
		dialog.ex->weight = (yyvsp[-1].il);
		dialog.ex->italic = (yyvsp[0].il);
		dialog.ex->charset = 1;
	      }
	  }
#line 2295 "rcparse.c"
    break;

  case 53: /* styles: styles FONT numexpr ',' QUOTEDSTRING cnumexpr cnumexpr cnumexpr  */
#line 493 "rcparse.y"
          {
	    dialog.style |= DS_SETFONT;
	    style |= DS_SETFONT;
	    dialog.pointsize = (yyvsp[-5].il);
	    unicode_from_ascii_old((int *)NULL, &dialog.font, (yyvsp[-3].s));
	    if (dialog.ex == NULL)
	      rcparse_warning(_("extended FONT requires DIALOGEX"));
	    else
	      {
		dialog.ex->weight = (yyvsp[-2].il);
		dialog.ex->italic = (yyvsp[-1].il);
		dialog.ex->charset = (yyvsp[0].il);
	      }
	  }
#line 2314 "rcparse.c"
    break;

  case 54: /* styles: styles MENU id  */
#line 508 "rcparse.y"
          {
	    dialog.menu = (yyvsp[0].id);
	  }
#line 2322 "rcparse.c"
    break;

  case 55: /* styles: styles CHARACTERISTICS numexpr  */
#line 512 "rcparse.y"
          {
	    sub_res_info.characteristics = (yyvsp[0].il);
	  }
#line 2330 "rcparse.c"
    break;

  case 56: /* styles: styles LANGUAGE numexpr cnumexpr  */
#line 516 "rcparse.y"
          {
	    sub_res_info.language = (yyvsp[-1].il) | ((yyvsp[0].il) << SUBLANG_SHIFT);
	  }
#line 2338 "rcparse.c"
    break;

  case 57: /* styles: styles VERSIONK numexpr  */
#line 520 "rcparse.y"
          {
	    sub_res_info.version = (yyvsp[0].il);
	  }
#line 2346 "rcparse.c"
    break;

  case 59: /* controls: controls control  */
#line 528 "rcparse.y"
          {
	    struct dialog_control **pp;

	    for (pp = &dialog.controls; *pp != NULL; pp = &(*pp)->next)
	      ;
	    *pp = (yyvsp[0].dialog_control);
	  }
#line 2358 "rcparse.c"
    break;

  case 60: /* $@4: %empty  */
#line 539 "rcparse.y"
            {
	      default_style = BS_AUTO3STATE | WS_TABSTOP;
	      base_style = BS_AUTO3STATE;
	      controlclass = CTL_BUTTON;
	      res_text_field = (yyvsp[0].id);
	    }
#line 2369 "rcparse.c"
    break;

  case 61: /* control: AUTO3STATE optresidc $@4 control_params  */
#line 546 "rcparse.y"
          {
	    (yyval.dialog_control) = (yyvsp[0].dialog_control);
	  }
#line 2377 "rcparse.c"
    break;

  case 62: /* $@5: %empty  */
#line 550 "rcparse.y"
            {
	      default_style = BS_AUTOCHECKBOX | WS_TABSTOP;
	      base_style = BS_AUTOCHECKBOX;
	      controlclass = CTL_BUTTON;
	      res_text_field = (yyvsp[0].id);
	    }
#line 2388 "rcparse.c"
    break;

  case 63: /* control: AUTOCHECKBOX optresidc $@5 control_params  */
#line 557 "rcparse.y"
          {
	    (yyval.dialog_control) = (yyvsp[0].dialog_control);
	  }
#line 2396 "rcparse.c"
    break;

  case 64: /* $@6: %empty  */
#line 561 "rcparse.y"
            {
	      default_style = BS_AUTORADIOBUTTON | WS_TABSTOP;
	      base_style = BS_AUTORADIOBUTTON;
	      controlclass = CTL_BUTTON;
	      res_text_field = (yyvsp[0].id);
	    }
#line 2407 "rcparse.c"
    break;

  case 65: /* control: AUTORADIOBUTTON optresidc $@6 control_params  */
#line 568 "rcparse.y"
          {
	    (yyval.dialog_control) = (yyvsp[0].dialog_control);
	  }
#line 2415 "rcparse.c"
    break;

  case 66: /* $@7: %empty  */
#line 572 "rcparse.y"
            {
	      default_style = ES_LEFT | WS_BORDER | WS_TABSTOP;
	      base_style = ES_LEFT | WS_BORDER | WS_TABSTOP;
	      controlclass = CTL_EDIT;
	      res_text_field = (yyvsp[0].id);
	    }
#line 2426 "rcparse.c"
    break;

  case 67: /* control: BEDIT optresidc $@7 control_params  */
#line 579 "rcparse.y"
          {
	    (yyval.dialog_control) = (yyvsp[0].dialog_control);
	    if (dialog.ex == NULL)
	      rcparse_warning (_("BEDIT requires DIALOGEX"));
	    res_string_to_id (&(yyval.dialog_control)->classname, "BEDIT");
	  }
#line 2437 "rcparse.c"
    break;

  case 68: /* $@8: %empty  */
#line 586 "rcparse.y"
            {
	      default_style = BS_CHECKBOX | WS_TABSTOP;
	      base_style = BS_CHECKBOX | WS_TABSTOP;
	      controlclass = CTL_BUTTON;
	      res_text_field = (yyvsp[0].id);
	    }
#line 2448 "rcparse.c"
    break;

  case 69: /* control: CHECKBOX optresidc $@8 control_params  */
#line 593 "rcparse.y"
          {
	    (yyval.dialog_control) = (yyvsp[0].dialog_control);
	  }
#line 2456 "rcparse.c"
    break;

  case 70: /* $@9: %empty  */
#line 597 "rcparse.y"
            {
	      /* This is as per MSDN documentation.  With some (???)
		 versions of MS rc.exe their is no default style.  */
	      default_style = CBS_SIMPLE | WS_TABSTOP;
	      base_style = 0;
	      controlclass = CTL_COMBOBOX;
	      res_text_field = res_null_text;
	    }
#line 2469 "rcparse.c"
    break;

  case 71: /* control: COMBOBOX $@9 control_params  */
#line 606 "rcparse.y"
          {
	    (yyval.dialog_control) = (yyvsp[0].dialog_control);
	  }
#line 2477 "rcparse.c"
    break;

  case 72: /* control: CONTROL optresidc numexpr cnumexpr control_styleexpr cnumexpr cnumexpr cnumexpr cnumexpr optcnumexpr opt_control_data  */
#line 611 "rcparse.y"
          {
	    (yyval.dialog_control) = define_control ((yyvsp[-9].id), (yyvsp[-8].il), (yyvsp[-5].il), (yyvsp[-4].il), (yyvsp[-3].il), (yyvsp[-2].il), (yyvsp[-7].il), style, (yyvsp[-1].il));
	    if ((yyvsp[0].rcdata_item) != NULL)
	      {
		if (dialog.ex == NULL)
		  rcparse_warning (_("control data requires DIALOGEX"));
		(yyval.dialog_control)->data = (yyvsp[0].rcdata_item);
	      }
	  }
#line 2491 "rcparse.c"
    break;

  case 73: /* control: CONTROL optresidc numexpr cnumexpr control_styleexpr cnumexpr cnumexpr cnumexpr cnumexpr cnumexpr cnumexpr opt_control_data  */
#line 622 "rcparse.y"
          {
	    (yyval.dialog_control) = define_control ((yyvsp[-10].id), (yyvsp[-9].il), (yyvsp[-6].il), (yyvsp[-5].il), (yyvsp[-4].il), (yyvsp[-3].il), (yyvsp[-8].il), style, (yyvsp[-2].il));
	    if (dialog.ex == NULL)
	      rcparse_warning (_("help ID requires DIALOGEX"));
	    (yyval.dialog_control)->help = (yyvsp[-1].il);
	    (yyval.dialog_control)->data = (yyvsp[0].rcdata_item);
	  }
#line 2503 "rcparse.c"
    break;

  case 74: /* control: CONTROL optresidc numexpr ',' QUOTEDSTRING control_styleexpr cnumexpr cnumexpr cnumexpr cnumexpr optcnumexpr opt_control_data  */
#line 631 "rcparse.y"
          {
	    (yyval.dialog_control) = define_control ((yyvsp[-10].id), (yyvsp[-9].il), (yyvsp[-5].il), (yyvsp[-4].il), (yyvsp[-3].il), (yyvsp[-2].il), 0, style, (yyvsp[-1].il));
	    if ((yyvsp[0].rcdata_item) != NULL)
	      {
		if (dialog.ex == NULL)
		  rcparse_warning ("control data requires DIALOGEX");
		(yyval.dialog_control)->data = (yyvsp[0].rcdata_item);
	      }
	    (yyval.dialog_control)->classname.named = 1;
  	    unicode_from_ascii_old(&(yyval.dialog_control)->classname.u.n.length, &(yyval.dialog_control)->classname.u.n.name, (yyvsp[-7].s));
	  }
#line 2519 "rcparse.c"
    break;

  case 75: /* control: CONTROL optresidc numexpr ',' QUOTEDSTRING control_styleexpr cnumexpr cnumexpr cnumexpr cnumexpr cnumexpr cnumexpr opt_control_data  */
#line 644 "rcparse.y"
          {
	    (yyval.dialog_control) = define_control((yyvsp[-11].id), (yyvsp[-10].il), (yyvsp[-6].il), (yyvsp[-5].il), (yyvsp[-4].il), (yyvsp[-3].il), 0, style, (yyvsp[-2].il));
	    if (dialog.ex == NULL)
	      rcparse_warning("help ID requires DIALOGEX");
	    (yyval.dialog_control)->help = (yyvsp[-1].il);
	    (yyval.dialog_control)->data = (yyvsp[0].rcdata_item);
	    (yyval.dialog_control)->classname.named = 1;
  	    unicode_from_ascii_old(&(yyval.dialog_control)->classname.u.n.length, &(yyval.dialog_control)->classname.u.n.name, (yyvsp[-8].s));
	  }
#line 2533 "rcparse.c"
    break;

  case 76: /* $@10: %empty  */
#line 654 "rcparse.y"
            {
	      default_style = SS_CENTER | WS_GROUP;
	      base_style = SS_CENTER;
	      controlclass = CTL_STATIC;
	      res_text_field = (yyvsp[0].id);
	    }
#line 2544 "rcparse.c"
    break;

  case 77: /* control: CTEXT optresidc $@10 control_params  */
#line 661 "rcparse.y"
          {
	    (yyval.dialog_control) = (yyvsp[0].dialog_control);
	  }
#line 2552 "rcparse.c"
    break;

  case 78: /* $@11: %empty  */
#line 665 "rcparse.y"
            {
	      default_style = BS_DEFPUSHBUTTON | WS_TABSTOP;
	      base_style = BS_DEFPUSHBUTTON | WS_TABSTOP;
	      controlclass = CTL_BUTTON;
	      res_text_field = (yyvsp[0].id);
	    }
#line 2563 "rcparse.c"
    break;

  case 79: /* control: DEFPUSHBUTTON optresidc $@11 control_params  */
#line 672 "rcparse.y"
          {
	    (yyval.dialog_control) = (yyvsp[0].dialog_control);
	  }
#line 2571 "rcparse.c"
    break;

  case 80: /* $@12: %empty  */
#line 676 "rcparse.y"
            {
	      default_style = ES_LEFT | WS_BORDER | WS_TABSTOP;
	      base_style = ES_LEFT | WS_BORDER | WS_TABSTOP;
	      controlclass = CTL_EDIT;
	      res_text_field = res_null_text;
	    }
#line 2582 "rcparse.c"
    break;

  case 81: /* control: EDITTEXT $@12 control_params  */
#line 683 "rcparse.y"
          {
	    (yyval.dialog_control) = (yyvsp[0].dialog_control);
	  }
#line 2590 "rcparse.c"
    break;

  case 82: /* $@13: %empty  */
#line 687 "rcparse.y"
            {
	      default_style = BS_GROUPBOX;
	      base_style = BS_GROUPBOX;
	      controlclass = CTL_BUTTON;
	      res_text_field = (yyvsp[0].id);
	    }
#line 2601 "rcparse.c"
    break;

  case 83: /* control: GROUPBOX optresidc $@13 control_params  */
#line 694 "rcparse.y"
          {
	    (yyval.dialog_control) = (yyvsp[0].dialog_control);
	  }
#line 2609 "rcparse.c"
    break;

  case 84: /* $@14: %empty  */
#line 698 "rcparse.y"
            {
	      default_style = ES_LEFT | WS_BORDER | WS_TABSTOP;
	      base_style = ES_LEFT | WS_BORDER | WS_TABSTOP;
	      controlclass = CTL_EDIT;
	      res_text_field = (yyvsp[0].id);
	    }
#line 2620 "rcparse.c"
    break;

  case 85: /* control: HEDIT optresidc $@14 control_params  */
#line 705 "rcparse.y"
          {
	    (yyval.dialog_control) = (yyvsp[0].dialog_control);
	    if (dialog.ex == NULL)
	      rcparse_warning (_("IEDIT requires DIALOGEX"));
	    res_string_to_id (&(yyval.dialog_control)->classname, "HEDIT");
	  }
#line 2631 "rcparse.c"
    break;

  case 86: /* control: ICON resref numexpr cnumexpr cnumexpr opt_control_data  */
#line 712 "rcparse.y"
          {
	    (yyval.dialog_control) = define_icon_control ((yyvsp[-4].id), (yyvsp[-3].il), (yyvsp[-2].il), (yyvsp[-1].il), 0, 0, 0, (yyvsp[0].rcdata_item),
				      dialog.ex);
          }
#line 2640 "rcparse.c"
    break;

  case 87: /* control: ICON resref numexpr cnumexpr cnumexpr cnumexpr cnumexpr opt_control_data  */
#line 718 "rcparse.y"
          {
	    (yyval.dialog_control) = define_icon_control ((yyvsp[-6].id), (yyvsp[-5].il), (yyvsp[-4].il), (yyvsp[-3].il), 0, 0, 0, (yyvsp[0].rcdata_item),
				      dialog.ex);
          }
#line 2649 "rcparse.c"
    break;

  case 88: /* control: ICON resref numexpr cnumexpr cnumexpr cnumexpr cnumexpr icon_styleexpr optcnumexpr opt_control_data  */
#line 724 "rcparse.y"
          {
	    (yyval.dialog_control) = define_icon_control ((yyvsp[-8].id), (yyvsp[-7].il), (yyvsp[-6].il), (yyvsp[-5].il), style, (yyvsp[-1].il), 0, (yyvsp[0].rcdata_item),
				      dialog.ex);
          }
#line 2658 "rcparse.c"
    break;

  case 89: /* control: ICON resref numexpr cnumexpr cnumexpr cnumexpr cnumexpr icon_styleexpr cnumexpr cnumexpr opt_control_data  */
#line 730 "rcparse.y"
          {
	    (yyval.dialog_control) = define_icon_control ((yyvsp[-9].id), (yyvsp[-8].il), (yyvsp[-7].il), (yyvsp[-6].il), style, (yyvsp[-2].il), (yyvsp[-1].il), (yyvsp[0].rcdata_item),
				      dialog.ex);
          }
#line 2667 "rcparse.c"
    break;

  case 90: /* $@15: %empty  */
#line 735 "rcparse.y"
            {
	      default_style = ES_LEFT | WS_BORDER | WS_TABSTOP;
	      base_style = ES_LEFT | WS_BORDER | WS_TABSTOP;
	      controlclass = CTL_EDIT;
	      res_text_field = (yyvsp[0].id);
	    }
#line 2678 "rcparse.c"
    break;

  case 91: /* control: IEDIT optresidc $@15 control_params  */
#line 742 "rcparse.y"
          {
	    (yyval.dialog_control) = (yyvsp[0].dialog_control);
	    if (dialog.ex == NULL)
	      rcparse_warning (_("IEDIT requires DIALOGEX"));
	    res_string_to_id (&(yyval.dialog_control)->classname, "IEDIT");
	  }
#line 2689 "rcparse.c"
    break;

  case 92: /* $@16: %empty  */
#line 749 "rcparse.y"
            {
	      default_style = LBS_NOTIFY | WS_BORDER;
	      base_style = LBS_NOTIFY | WS_BORDER;
	      controlclass = CTL_LISTBOX;
	      res_text_field = res_null_text;
	    }
#line 2700 "rcparse.c"
    break;

  case 93: /* control: LISTBOX $@16 control_params  */
#line 756 "rcparse.y"
          {
	    (yyval.dialog_control) = (yyvsp[0].dialog_control);
	  }
#line 2708 "rcparse.c"
    break;

  case 94: /* $@17: %empty  */
#line 760 "rcparse.y"
            {
	      default_style = SS_LEFT | WS_GROUP;
	      base_style = SS_LEFT;
	      controlclass = CTL_STATIC;
	      res_text_field = (yyvsp[0].id);
	    }
#line 2719 "rcparse.c"
    break;

  case 95: /* control: LTEXT optresidc $@17 control_params  */
#line 767 "rcparse.y"
          {
	    (yyval.dialog_control) = (yyvsp[0].dialog_control);
	  }
#line 2727 "rcparse.c"
    break;

  case 96: /* $@18: %empty  */
#line 771 "rcparse.y"
            {
	      default_style = BS_PUSHBOX | WS_TABSTOP;
	      base_style = BS_PUSHBOX;
	      controlclass = CTL_BUTTON;
	    }
#line 2737 "rcparse.c"
    break;

  case 97: /* control: PUSHBOX optresidc $@18 control_params  */
#line 777 "rcparse.y"
          {
	    (yyval.dialog_control) = (yyvsp[0].dialog_control);
	  }
#line 2745 "rcparse.c"
    break;

  case 98: /* $@19: %empty  */
#line 781 "rcparse.y"
            {
	      default_style = BS_PUSHBUTTON | WS_TABSTOP;
	      base_style = BS_PUSHBUTTON | WS_TABSTOP;
	      controlclass = CTL_BUTTON;
	      res_text_field = (yyvsp[0].id);
	    }
#line 2756 "rcparse.c"
    break;

  case 99: /* control: PUSHBUTTON optresidc $@19 control_params  */
#line 788 "rcparse.y"
          {
	    (yyval.dialog_control) = (yyvsp[0].dialog_control);
	  }
#line 2764 "rcparse.c"
    break;

  case 100: /* $@20: %empty  */
#line 792 "rcparse.y"
            {
	      default_style = BS_RADIOBUTTON | WS_TABSTOP;
	      base_style = BS_RADIOBUTTON;
	      controlclass = CTL_BUTTON;
	      res_text_field = (yyvsp[0].id);
	    }
#line 2775 "rcparse.c"
    break;

  case 101: /* control: RADIOBUTTON optresidc $@20 control_params  */
#line 799 "rcparse.y"
          {
	    (yyval.dialog_control) = (yyvsp[0].dialog_control);
	  }
#line 2783 "rcparse.c"
    break;

  case 102: /* $@21: %empty  */
#line 803 "rcparse.y"
            {
	      default_style = SS_RIGHT | WS_GROUP;
	      base_style = SS_RIGHT;
	      controlclass = CTL_STATIC;
	      res_text_field = (yyvsp[0].id);
	    }
#line 2794 "rcparse.c"
    break;

  case 103: /* control: RTEXT optresidc $@21 control_params  */
#line 810 "rcparse.y"
          {
	    (yyval.dialog_control) = (yyvsp[0].dialog_control);
	  }
#line 2802 "rcparse.c"
    break;

  case 104: /* $@22: %empty  */
#line 814 "rcparse.y"
            {
	      default_style = SBS_HORZ;
	      base_style = 0;
	      controlclass = CTL_SCROLLBAR;
	      res_text_field = res_null_text;
	    }
#line 2813 "rcparse.c"
    break;

  case 105: /* control: SCROLLBAR $@22 control_params  */
#line 821 "rcparse.y"
          {
	    (yyval.dialog_control) = (yyvsp[0].dialog_control);
	  }
#line 2821 "rcparse.c"
    break;

  case 106: /* $@23: %empty  */
#line 825 "rcparse.y"
            {
	      default_style = BS_3STATE | WS_TABSTOP;
	      base_style = BS_3STATE;
	      controlclass = CTL_BUTTON;
	      res_text_field = (yyvsp[0].id);
	    }
#line 2832 "rcparse.c"
    break;

  case 107: /* control: STATE3 optresidc $@23 control_params  */
#line 832 "rcparse.y"
          {
	    (yyval.dialog_control) = (yyvsp[0].dialog_control);
	  }
#line 2840 "rcparse.c"
    break;

  case 108: /* $@24: %empty  */
#line 837 "rcparse.y"
            { style = WS_CHILD | WS_VISIBLE; }
#line 2846 "rcparse.c"
    break;

  case 109: /* control: USERBUTTON resref numexpr ',' numexpr ',' numexpr ',' numexpr ',' numexpr ',' $@24 styleexpr optcnumexpr  */
#line 839 "rcparse.y"
          {
	    (yyval.dialog_control) = define_control ((yyvsp[-13].id), (yyvsp[-12].il), (yyvsp[-10].il), (yyvsp[-8].il), (yyvsp[-6].il), (yyvsp[-4].il), CTL_BUTTON,
				 style, (yyvsp[0].il));
	  }
#line 2855 "rcparse.c"
    break;

  case 110: /* control_params: numexpr cnumexpr cnumexpr cnumexpr cnumexpr opt_control_data  */
#line 854 "rcparse.y"
          {
	    (yyval.dialog_control) = define_control(res_text_field, (yyvsp[-5].il), (yyvsp[-4].il), (yyvsp[-3].il), (yyvsp[-2].il), (yyvsp[-1].il),
     	 			controlclass,
                                (default_style | WS_CHILD | WS_VISIBLE), 0);
	    if ((yyvsp[0].rcdata_item) != NULL)
	      {
		if (dialog.ex == NULL)
		  rcparse_warning(_("control data requires DIALOGEX"));
		(yyval.dialog_control)->data = (yyvsp[0].rcdata_item);
	      }
	  }
#line 2871 "rcparse.c"
    break;

  case 111: /* control_params: numexpr cnumexpr cnumexpr cnumexpr cnumexpr control_params_styleexpr optcnumexpr opt_control_data  */
#line 867 "rcparse.y"
          {
	    (yyval.dialog_control) = define_control(res_text_field, (yyvsp[-7].il), (yyvsp[-6].il), (yyvsp[-5].il), (yyvsp[-4].il), (yyvsp[-3].il),
                                controlclass, style, (yyvsp[-1].il));
	    if ((yyvsp[0].rcdata_item) != NULL)
	      {
		if (dialog.ex == NULL)
		  rcparse_warning(_("control data requires DIALOGEX"));
		(yyval.dialog_control)->data = (yyvsp[0].rcdata_item);
	      }
	  }
#line 2886 "rcparse.c"
    break;

  case 112: /* control_params: numexpr cnumexpr cnumexpr cnumexpr cnumexpr control_params_styleexpr cnumexpr cnumexpr opt_control_data  */
#line 879 "rcparse.y"
          {
	    (yyval.dialog_control) = define_control(res_text_field, (yyvsp[-8].il), (yyvsp[-7].il), (yyvsp[-6].il), (yyvsp[-5].il), (yyvsp[-4].il),
     	 			controlclass, style, (yyvsp[-2].il));
	    if (dialog.ex == NULL)
	      rcparse_warning(_("help ID requires DIALOGEX"));
	    (yyval.dialog_control)->help = (yyvsp[-1].il);
	    (yyval.dialog_control)->data = (yyvsp[0].rcdata_item);
	  }
#line 2899 "rcparse.c"
    break;

  case 113: /* optresidc: %empty  */
#line 891 "rcparse.y"
          {
	    res_string_to_id(&(yyval.id), "");
	  }
#line 2907 "rcparse.c"
    break;

  case 114: /* optresidc: posnumexpr ','  */
#line 895 "rcparse.y"
          {
	    (yyval.id).named = 0;
	    (yyval.id).u.id = (yyvsp[-1].il);
	  }
#line 2916 "rcparse.c"
    break;

  case 115: /* optresidc: QUOTEDSTRING  */
#line 900 "rcparse.y"
          {
	    res_string_to_id (&(yyval.id), (yyvsp[0].s));
	  }
#line 2924 "rcparse.c"
    break;

  case 116: /* optresidc: QUOTEDSTRING ','  */
#line 904 "rcparse.y"
          {
	    res_string_to_id (&(yyval.id), (yyvsp[-1].s));
	  }
#line 2932 "rcparse.c"
    break;

  case 117: /* opt_control_data: %empty  */
#line 911 "rcparse.y"
          {
	    (yyval.rcdata_item) = NULL;
	  }
#line 2940 "rcparse.c"
    break;

  case 118: /* opt_control_data: BEG optrcdata_data END  */
#line 915 "rcparse.y"
          {
	    (yyval.rcdata_item) = (yyvsp[-1].rcdata).first;
	  }
#line 2948 "rcparse.c"
    break;

  case 119: /* $@25: %empty  */
#line 924 "rcparse.y"
          { style = WS_CHILD | WS_VISIBLE; }
#line 2954 "rcparse.c"
    break;

  case 121: /* $@26: %empty  */
#line 930 "rcparse.y"
          { style = SS_ICON | WS_CHILD | WS_VISIBLE; }
#line 2960 "rcparse.c"
    break;

  case 123: /* $@27: %empty  */
#line 936 "rcparse.y"
          { style = base_style | WS_CHILD | WS_VISIBLE; }
#line 2966 "rcparse.c"
    break;

  case 125: /* font: id FONT memflags_move_discard file_name  */
#line 944 "rcparse.y"
          {
	    define_font ((yyvsp[-3].id), &(yyvsp[-1].res_info), (yyvsp[0].s));
	    if (yychar != YYEMPTY)
	      YYERROR;
	    rcparse_discard_strings ();
	  }
#line 2977 "rcparse.c"
    break;

  case 126: /* icon: id ICON memflags_move_discard file_name  */
#line 956 "rcparse.y"
          {
	    define_icon ((yyvsp[-3].id), &(yyvsp[-1].res_info), (yyvsp[0].s));
	    if (yychar != YYEMPTY)
	      YYERROR;
	    rcparse_discard_strings ();
	  }
#line 2988 "rcparse.c"
    break;

  case 127: /* language: LANGUAGE numexpr cnumexpr  */
#line 969 "rcparse.y"
          {
	    language = (yyvsp[-1].il) | ((yyvsp[0].il) << SUBLANG_SHIFT);
	  }
#line 2996 "rcparse.c"
    break;

  case 128: /* menu: id MENU suboptions BEG menuitems END  */
#line 978 "rcparse.y"
          {
	    define_menu ((yyvsp[-5].id), &(yyvsp[-3].res_info), (yyvsp[-1].menuitem));
	    if (yychar != YYEMPTY)
	      YYERROR;
	    rcparse_discard_strings ();
	  }
#line 3007 "rcparse.c"
    break;

  case 129: /* menuitems: %empty  */
#line 988 "rcparse.y"
          {
	    (yyval.menuitem) = NULL;
	  }
#line 3015 "rcparse.c"
    break;

  case 130: /* menuitems: menuitems menuitem  */
#line 992 "rcparse.y"
          {
	    if ((yyvsp[-1].menuitem) == NULL)
	      (yyval.menuitem) = (yyvsp[0].menuitem);
	    else
	      {
		struct menuitem **pp;

		for (pp = &(yyvsp[-1].menuitem)->next; *pp != NULL; pp = &(*pp)->next)
		  ;
		*pp = (yyvsp[0].menuitem);
		(yyval.menuitem) = (yyvsp[-1].menuitem);
	      }
	  }
#line 3033 "rcparse.c"
    break;

  case 131: /* menuitem: MENUITEM QUOTEDSTRING cnumexpr menuitem_flags  */
#line 1009 "rcparse.y"
          {
	    (yyval.menuitem) = define_menuitem ((yyvsp[-2].s), (yyvsp[-1].il), (yyvsp[0].is), 0, 0, NULL);
	  }
#line 3041 "rcparse.c"
    break;

  case 132: /* menuitem: MENUITEM SEPARATOR  */
#line 1013 "rcparse.y"
          {
	    (yyval.menuitem) = define_menuitem (NULL, 0, 0, 0, 0, NULL);
	  }
#line 3049 "rcparse.c"
    break;

  case 133: /* menuitem: POPUP QUOTEDSTRING menuitem_flags BEG menuitems END  */
#line 1017 "rcparse.y"
          {
	    (yyval.menuitem) = define_menuitem ((yyvsp[-4].s), 0, (yyvsp[-3].is), 0, 0, (yyvsp[-1].menuitem));
	  }
#line 3057 "rcparse.c"
    break;

  case 134: /* menuitem_flags: %empty  */
#line 1024 "rcparse.y"
          {
	    (yyval.is) = 0;
	  }
#line 3065 "rcparse.c"
    break;

  case 135: /* menuitem_flags: menuitem_flags ',' menuitem_flag  */
#line 1028 "rcparse.y"
          {
	    (yyval.is) = (yyvsp[-2].is) | (yyvsp[0].is);
	  }
#line 3073 "rcparse.c"
    break;

  case 136: /* menuitem_flags: menuitem_flags menuitem_flag  */
#line 1032 "rcparse.y"
          {
	    (yyval.is) = (yyvsp[-1].is) | (yyvsp[0].is);
	  }
#line 3081 "rcparse.c"
    break;

  case 137: /* menuitem_flag: CHECKED  */
#line 1039 "rcparse.y"
          {
	    (yyval.is) = MENUITEM_CHECKED;
	  }
#line 3089 "rcparse.c"
    break;

  case 138: /* menuitem_flag: GRAYED  */
#line 1043 "rcparse.y"
          {
	    (yyval.is) = MENUITEM_GRAYED;
	  }
#line 3097 "rcparse.c"
    break;

  case 139: /* menuitem_flag: HELP  */
#line 1047 "rcparse.y"
          {
	    (yyval.is) = MENUITEM_HELP;
	  }
#line 3105 "rcparse.c"
    break;

  case 140: /* menuitem_flag: INACTIVE  */
#line 1051 "rcparse.y"
          {
	    (yyval.is) = MENUITEM_INACTIVE;
	  }
#line 3113 "rcparse.c"
    break;

  case 141: /* menuitem_flag: MENUBARBREAK  */
#line 1055 "rcparse.y"
          {
	    (yyval.is) = MENUITEM_MENUBARBREAK;
	  }
#line 3121 "rcparse.c"
    break;

  case 142: /* menuitem_flag: MENUBREAK  */
#line 1059 "rcparse.y"
          {
	    (yyval.is) = MENUITEM_MENUBREAK;
	  }
#line 3129 "rcparse.c"
    break;

  case 143: /* menuex: id MENUEX suboptions BEG menuexitems END  */
#line 1068 "rcparse.y"
          {
	    define_menu ((yyvsp[-5].id), &(yyvsp[-3].res_info), (yyvsp[-1].menuitem));
	    if (yychar != YYEMPTY)
	      YYERROR;
	    rcparse_discard_strings ();
	  }
#line 3140 "rcparse.c"
    break;

  case 144: /* menuexitems: %empty  */
#line 1078 "rcparse.y"
          {
	    (yyval.menuitem) = NULL;
	  }
#line 3148 "rcparse.c"
    break;

  case 145: /* menuexitems: menuexitems menuexitem  */
#line 1082 "rcparse.y"
          {
	    if ((yyvsp[-1].menuitem) == NULL)
	      (yyval.menuitem) = (yyvsp[0].menuitem);
	    else
	      {
		struct menuitem **pp;

		for (pp = &(yyvsp[-1].menuitem)->next; *pp != NULL; pp = &(*pp)->next)
		  ;
		*pp = (yyvsp[0].menuitem);
		(yyval.menuitem) = (yyvsp[-1].menuitem);
	      }
	  }
#line 3166 "rcparse.c"
    break;

  case 146: /* menuexitem: MENUITEM QUOTEDSTRING  */
#line 1099 "rcparse.y"
          {
	    (yyval.menuitem) = define_menuitem ((yyvsp[0].s), 0, 0, 0, 0, NULL);
	  }
#line 3174 "rcparse.c"
    break;

  case 147: /* menuexitem: MENUITEM QUOTEDSTRING cnumexpr  */
#line 1103 "rcparse.y"
          {
	    (yyval.menuitem) = define_menuitem ((yyvsp[-1].s), (yyvsp[0].il), 0, 0, 0, NULL);
	  }
#line 3182 "rcparse.c"
    break;

  case 148: /* menuexitem: MENUITEM QUOTEDSTRING cnumexpr cnumexpr optcnumexpr  */
#line 1107 "rcparse.y"
          {
	    (yyval.menuitem) = define_menuitem ((yyvsp[-3].s), (yyvsp[-2].il), (yyvsp[-1].il), (yyvsp[0].il), 0, NULL);
	  }
#line 3190 "rcparse.c"
    break;

  case 149: /* menuexitem: MENUITEM SEPARATOR  */
#line 1111 "rcparse.y"
          {
 	    (yyval.menuitem) = define_menuitem (NULL, 0, 0, 0, 0, NULL);
 	  }
#line 3198 "rcparse.c"
    break;

  case 150: /* menuexitem: POPUP QUOTEDSTRING BEG menuexitems END  */
#line 1115 "rcparse.y"
          {
	    (yyval.menuitem) = define_menuitem ((yyvsp[-3].s), 0, 0, 0, 0, (yyvsp[-1].menuitem));
	  }
#line 3206 "rcparse.c"
    break;

  case 151: /* menuexitem: POPUP QUOTEDSTRING cnumexpr BEG menuexitems END  */
#line 1119 "rcparse.y"
          {
	    (yyval.menuitem) = define_menuitem ((yyvsp[-4].s), (yyvsp[-3].il), 0, 0, 0, (yyvsp[-1].menuitem));
	  }
#line 3214 "rcparse.c"
    break;

  case 152: /* menuexitem: POPUP QUOTEDSTRING cnumexpr cnumexpr BEG menuexitems END  */
#line 1123 "rcparse.y"
          {
	    (yyval.menuitem) = define_menuitem ((yyvsp[-5].s), (yyvsp[-4].il), (yyvsp[-3].il), 0, 0, (yyvsp[-1].menuitem));
	  }
#line 3222 "rcparse.c"
    break;

  case 153: /* menuexitem: POPUP QUOTEDSTRING cnumexpr cnumexpr cnumexpr optcnumexpr BEG menuexitems END  */
#line 1128 "rcparse.y"
          {
	    (yyval.menuitem) = define_menuitem ((yyvsp[-7].s), (yyvsp[-6].il), (yyvsp[-5].il), (yyvsp[-4].il), (yyvsp[-3].il), (yyvsp[-1].menuitem));
	  }
#line 3230 "rcparse.c"
    break;

  case 154: /* messagetable: id MESSAGETABLE memflags_move file_name  */
#line 1137 "rcparse.y"
          {
	    define_messagetable ((yyvsp[-3].id), &(yyvsp[-1].res_info), (yyvsp[0].s));
	    if (yychar != YYEMPTY)
	      YYERROR;
	    rcparse_discard_strings ();
	  }
#line 3241 "rcparse.c"
    break;

  case 155: /* rcdata: id RCDATA suboptions BEG optrcdata_data END  */
#line 1149 "rcparse.y"
          {
	    define_rcdata ((yyvsp[-5].id), &(yyvsp[-3].res_info), (yyvsp[-1].rcdata).first);
	    if (yychar != YYEMPTY)
	      YYERROR;
	    rcparse_discard_strings ();
	  }
#line 3252 "rcparse.c"
    break;

  case 156: /* rcdata: id RCDATA suboptions file_name  */
#line 1156 "rcparse.y"
          {
	    define_rcdata_file ((yyvsp[-3].id), &(yyvsp[-1].res_info), (yyvsp[0].s));
	    if (yychar != YYEMPTY)
	      YYERROR;
	    rcparse_discard_strings ();
	  }
#line 3263 "rcparse.c"
    break;

  case 157: /* $@28: %empty  */
#line 1168 "rcparse.y"
          {
	    rcparse_rcdata ();
	  }
#line 3271 "rcparse.c"
    break;

  case 158: /* optrcdata_data: $@28 optrcdata_data_int  */
#line 1172 "rcparse.y"
          {
	    rcparse_normal ();
	    (yyval.rcdata) = (yyvsp[0].rcdata);
	  }
#line 3280 "rcparse.c"
    break;

  case 159: /* optrcdata_data_int: %empty  */
#line 1180 "rcparse.y"
          {
	    (yyval.rcdata).first = NULL;
	    (yyval.rcdata).last = NULL;
	  }
#line 3289 "rcparse.c"
    break;

  case 160: /* optrcdata_data_int: rcdata_data  */
#line 1185 "rcparse.y"
          {
	    (yyval.rcdata) = (yyvsp[0].rcdata);
	  }
#line 3297 "rcparse.c"
    break;

  case 161: /* rcdata_data: SIZEDSTRING  */
#line 1192 "rcparse.y"
          {
	    struct rcdata_item *ri;

	    ri = define_rcdata_string ((yyvsp[0].ss).s, (yyvsp[0].ss).length);
	    (yyval.rcdata).first = ri;
	    (yyval.rcdata).last = ri;
	  }
#line 3309 "rcparse.c"
    break;

  case 162: /* rcdata_data: sizednumexpr  */
#line 1200 "rcparse.y"
          {
	    struct rcdata_item *ri;

	    ri = define_rcdata_number ((yyvsp[0].i).val, (yyvsp[0].i).dword);
	    (yyval.rcdata).first = ri;
	    (yyval.rcdata).last = ri;
	  }
#line 3321 "rcparse.c"
    break;

  case 163: /* rcdata_data: rcdata_data ',' SIZEDSTRING  */
#line 1208 "rcparse.y"
          {
	    struct rcdata_item *ri;

	    ri = define_rcdata_string ((yyvsp[0].ss).s, (yyvsp[0].ss).length);
	    (yyval.rcdata).first = (yyvsp[-2].rcdata).first;
	    (yyvsp[-2].rcdata).last->next = ri;
	    (yyval.rcdata).last = ri;
	  }
#line 3334 "rcparse.c"
    break;

  case 164: /* rcdata_data: rcdata_data ',' sizednumexpr  */
#line 1217 "rcparse.y"
          {
	    struct rcdata_item *ri;

	    ri = define_rcdata_number ((yyvsp[0].i).val, (yyvsp[0].i).dword);
	    (yyval.rcdata).first = (yyvsp[-2].rcdata).first;
	    (yyvsp[-2].rcdata).last->next = ri;
	    (yyval.rcdata).last = ri;
	  }
#line 3347 "rcparse.c"
    break;

  case 165: /* $@29: %empty  */
#line 1231 "rcparse.y"
            { sub_res_info = (yyvsp[-1].res_info); }
#line 3353 "rcparse.c"
    break;

  case 168: /* string_data: string_data numexpr QUOTEDSTRING  */
#line 1238 "rcparse.y"
          {
	    define_stringtable (&sub_res_info, (yyvsp[-1].il), (yyvsp[0].s));
	    if (yychar != YYEMPTY)
	      YYERROR;
	    rcparse_discard_strings ();
	  }
#line 3364 "rcparse.c"
    break;

  case 169: /* string_data: string_data numexpr ',' QUOTEDSTRING  */
#line 1245 "rcparse.y"
          {
	    define_stringtable (&sub_res_info, (yyvsp[-2].il), (yyvsp[0].s));
	    if (yychar != YYEMPTY)
	      YYERROR;
	    rcparse_discard_strings ();
	  }
#line 3375 "rcparse.c"
    break;

  case 170: /* user: id id suboptions BEG optrcdata_data END  */
#line 1258 "rcparse.y"
          {
	    define_user_data ((yyvsp[-5].id), (yyvsp[-4].id), &(yyvsp[-3].res_info), (yyvsp[-1].rcdata).first);
	    if (yychar != YYEMPTY)
	      YYERROR;
	    rcparse_discard_strings ();
	  }
#line 3386 "rcparse.c"
    break;

  case 171: /* user: id id suboptions file_name  */
#line 1265 "rcparse.y"
          {
	    define_user_file ((yyvsp[-3].id), (yyvsp[-2].id), &(yyvsp[-1].res_info), (yyvsp[0].s));
	    if (yychar != YYEMPTY)
	      YYERROR;
	    rcparse_discard_strings ();
	  }
#line 3397 "rcparse.c"
    break;

  case 172: /* versioninfo: id VERSIONINFO fixedverinfo BEG verblocks END  */
#line 1277 "rcparse.y"
          {
	    define_versioninfo ((yyvsp[-5].id), language, (yyvsp[-3].fixver), (yyvsp[-1].verinfo));
	    if (yychar != YYEMPTY)
	      YYERROR;
	    rcparse_discard_strings ();
	  }
#line 3408 "rcparse.c"
    break;

  case 173: /* fixedverinfo: %empty  */
#line 1287 "rcparse.y"
          {
	    (yyval.fixver) = ((struct fixed_versioninfo *)
		  res_alloc (sizeof (struct fixed_versioninfo)));
	    memset ((yyval.fixver), 0, sizeof (struct fixed_versioninfo));
	  }
#line 3418 "rcparse.c"
    break;

  case 174: /* fixedverinfo: fixedverinfo FILEVERSION numexpr cnumexpr cnumexpr cnumexpr  */
#line 1293 "rcparse.y"
          {
	    (yyvsp[-5].fixver)->file_version_ms = ((yyvsp[-3].il) << 16) | (yyvsp[-2].il);
	    (yyvsp[-5].fixver)->file_version_ls = ((yyvsp[-1].il) << 16) | (yyvsp[0].il);
	    (yyval.fixver) = (yyvsp[-5].fixver);
	  }
#line 3428 "rcparse.c"
    break;

  case 175: /* fixedverinfo: fixedverinfo PRODUCTVERSION numexpr cnumexpr cnumexpr cnumexpr  */
#line 1299 "rcparse.y"
          {
	    (yyvsp[-5].fixver)->product_version_ms = ((yyvsp[-3].il) << 16) | (yyvsp[-2].il);
	    (yyvsp[-5].fixver)->product_version_ls = ((yyvsp[-1].il) << 16) | (yyvsp[0].il);
	    (yyval.fixver) = (yyvsp[-5].fixver);
	  }
#line 3438 "rcparse.c"
    break;

  case 176: /* fixedverinfo: fixedverinfo FILEFLAGSMASK numexpr  */
#line 1305 "rcparse.y"
          {
	    (yyvsp[-2].fixver)->file_flags_mask = (yyvsp[0].il);
	    (yyval.fixver) = (yyvsp[-2].fixver);
	  }
#line 3447 "rcparse.c"
    break;

  case 177: /* fixedverinfo: fixedverinfo FILEFLAGS numexpr  */
#line 1310 "rcparse.y"
          {
	    (yyvsp[-2].fixver)->file_flags = (yyvsp[0].il);
	    (yyval.fixver) = (yyvsp[-2].fixver);
	  }
#line 3456 "rcparse.c"
    break;

  case 178: /* fixedverinfo: fixedverinfo FILEOS numexpr  */
#line 1315 "rcparse.y"
          {
	    (yyvsp[-2].fixver)->file_os = (yyvsp[0].il);
	    (yyval.fixver) = (yyvsp[-2].fixver);
	  }
#line 3465 "rcparse.c"
    break;

  case 179: /* fixedverinfo: fixedverinfo FILETYPE numexpr  */
#line 1320 "rcparse.y"
          {
	    (yyvsp[-2].fixver)->file_type = (yyvsp[0].il);
	    (yyval.fixver) = (yyvsp[-2].fixver);
	  }
#line 3474 "rcparse.c"
    break;

  case 180: /* fixedverinfo: fixedverinfo FILESUBTYPE numexpr  */
#line 1325 "rcparse.y"
          {
	    (yyvsp[-2].fixver)->file_subtype = (yyvsp[0].il);
	    (yyval.fixver) = (yyvsp[-2].fixver);
	  }
#line 3483 "rcparse.c"
    break;

  case 181: /* verblocks: %empty  */
#line 1339 "rcparse.y"
          {
	    (yyval.verinfo) = NULL;
	  }
#line 3491 "rcparse.c"
    break;

  case 182: /* verblocks: verblocks BLOCKSTRINGFILEINFO BEG BLOCK BEG vervals END END  */
#line 1343 "rcparse.y"
          {
	    (yyval.verinfo) = append_ver_stringfileinfo ((yyvsp[-7].verinfo), (yyvsp[-4].s), (yyvsp[-2].verstring));
	  }
#line 3499 "rcparse.c"
    break;

  case 183: /* verblocks: verblocks BLOCKVARFILEINFO BEG VALUE QUOTEDSTRING vertrans END  */
#line 1347 "rcparse.y"
          {
	    (yyval.verinfo) = append_ver_varfileinfo ((yyvsp[-6].verinfo), (yyvsp[-2].s), (yyvsp[-1].vervar));
	  }
#line 3507 "rcparse.c"
    break;

  case 184: /* vervals: %empty  */
#line 1354 "rcparse.y"
          {
	    (yyval.verstring) = NULL;
	  }
#line 3515 "rcparse.c"
    break;

  case 185: /* vervals: vervals VALUE QUOTEDSTRING ',' QUOTEDSTRING  */
#line 1358 "rcparse.y"
          {
	    (yyval.verstring) = append_verval ((yyvsp[-4].verstring), (yyvsp[-2].s), (yyvsp[0].s));
	  }
#line 3523 "rcparse.c"
    break;

  case 186: /* vertrans: %empty  */
#line 1365 "rcparse.y"
          {
	    (yyval.vervar) = NULL;
	  }
#line 3531 "rcparse.c"
    break;

  case 187: /* vertrans: vertrans cnumexpr cnumexpr  */
#line 1369 "rcparse.y"
          {
	    (yyval.vervar) = append_vertrans ((yyvsp[-2].vervar), (yyvsp[-1].il), (yyvsp[0].il));
	  }
#line 3539 "rcparse.c"
    break;

  case 188: /* id: posnumexpr  */
#line 1378 "rcparse.y"
          {
	    (yyval.id).named = 0;
	    (yyval.id).u.id = (yyvsp[0].il);
	  }
#line 3548 "rcparse.c"
    break;

  case 189: /* id: STRING  */
#line 1383 "rcparse.y"
          {
	    char *copy, *s;

	    /* It seems that resource ID's are forced to upper case.  */
	    copy = xstrdup ((yyvsp[0].s));
	    for (s = copy; *s != '\0'; s++)
	      *s = TOUPPER (*s);
	    res_string_to_id (&(yyval.id), copy);
	    free (copy);
	  }
#line 3563 "rcparse.c"
    break;

  case 190: /* resname: QUOTEDSTRING  */
#line 1399 "rcparse.y"
          {
	    (yyval.s) = (yyvsp[0].s);
	  }
#line 3571 "rcparse.c"
    break;

  case 191: /* resname: QUOTEDSTRING ','  */
#line 1403 "rcparse.y"
          {
	    (yyval.s) = (yyvsp[-1].s);
	  }
#line 3579 "rcparse.c"
    break;

  case 192: /* resname: STRING ','  */
#line 1407 "rcparse.y"
          {
	    (yyval.s) = (yyvsp[-1].s);
	  }
#line 3587 "rcparse.c"
    break;

  case 193: /* resref: posnumexpr ','  */
#line 1415 "rcparse.y"
          {
	    (yyval.id).named = 0;
	    (yyval.id).u.id = (yyvsp[-1].il);
	  }
#line 3596 "rcparse.c"
    break;

  case 194: /* resref: resname  */
#line 1420 "rcparse.y"
          {
	    char *copy, *s;

	    /* It seems that resource ID's are forced to upper case.  */
	    copy = xstrdup ((yyvsp[0].s));
	    for (s = copy; *s != '\0'; s++)
	      *s = TOUPPER (*s);
	    res_string_to_id (&(yyval.id), copy);
	    free (copy);
	  }
#line 3611 "rcparse.c"
    break;

  case 195: /* suboptions: %empty  */
#line 1437 "rcparse.y"
          {
	    memset (&(yyval.res_info), 0, sizeof (struct res_res_info));
	    (yyval.res_info).language = language;
	    /* FIXME: Is this the right default?  */
	    (yyval.res_info).memflags = MEMFLAG_MOVEABLE | MEMFLAG_PURE | MEMFLAG_DISCARDABLE;
	  }
#line 3622 "rcparse.c"
    break;

  case 196: /* suboptions: suboptions memflag  */
#line 1444 "rcparse.y"
          {
	    (yyval.res_info) = (yyvsp[-1].res_info);
	    (yyval.res_info).memflags |= (yyvsp[0].memflags).on;
	    (yyval.res_info).memflags &=~ (yyvsp[0].memflags).off;
	  }
#line 3632 "rcparse.c"
    break;

  case 197: /* suboptions: suboptions CHARACTERISTICS numexpr  */
#line 1450 "rcparse.y"
          {
	    (yyval.res_info) = (yyvsp[-2].res_info);
	    (yyval.res_info).characteristics = (yyvsp[0].il);
	  }
#line 3641 "rcparse.c"
    break;

  case 198: /* suboptions: suboptions LANGUAGE numexpr cnumexpr  */
#line 1455 "rcparse.y"
          {
	    (yyval.res_info) = (yyvsp[-3].res_info);
	    (yyval.res_info).language = (yyvsp[-1].il) | ((yyvsp[0].il) << SUBLANG_SHIFT);
	  }
#line 3650 "rcparse.c"
    break;

  case 199: /* suboptions: suboptions VERSIONK numexpr  */
#line 1460 "rcparse.y"
          {
	    (yyval.res_info) = (yyvsp[-2].res_info);
	    (yyval.res_info).version = (yyvsp[0].il);
	  }
#line 3659 "rcparse.c"
    break;

  case 200: /* memflags_move_discard: %empty  */
#line 1470 "rcparse.y"
          {
	    memset (&(yyval.res_info), 0, sizeof (struct res_res_info));
	    (yyval.res_info).language = language;
	    (yyval.res_info).memflags = MEMFLAG_MOVEABLE | MEMFLAG_DISCARDABLE;
	  }
#line 3669 "rcparse.c"
    break;

  case 201: /* memflags_move_discard: memflags_move_discard memflag  */
#line 1476 "rcparse.y"
          {
	    (yyval.res_info) = (yyvsp[-1].res_info);
	    (yyval.res_info).memflags |= (yyvsp[0].memflags).on;
	    (yyval.res_info).memflags &=~ (yyvsp[0].memflags).off;
	  }
#line 3679 "rcparse.c"
    break;

  case 202: /* memflags_move: %empty  */
#line 1487 "rcparse.y"
          {
	    memset (&(yyval.res_info), 0, sizeof (struct res_res_info));
	    (yyval.res_info).language = language;
	    (yyval.res_info).memflags = MEMFLAG_MOVEABLE | MEMFLAG_PURE | MEMFLAG_DISCARDABLE;
	  }
#line 3689 "rcparse.c"
    break;

  case 203: /* memflags_move: memflags_move memflag  */
#line 1493 "rcparse.y"
          {
	    (yyval.res_info) = (yyvsp[-1].res_info);
	    (yyval.res_info).memflags |= (yyvsp[0].memflags).on;
	    (yyval.res_info).memflags &=~ (yyvsp[0].memflags).off;
	  }
#line 3699 "rcparse.c"
    break;

  case 204: /* memflag: MOVEABLE  */
#line 1505 "rcparse.y"
          {
	    (yyval.memflags).on = MEMFLAG_MOVEABLE;
	    (yyval.memflags).off = 0;
	  }
#line 3708 "rcparse.c"
    break;

  case 205: /* memflag: FIXED  */
#line 1510 "rcparse.y"
          {
	    (yyval.memflags).on = 0;
	    (yyval.memflags).off = MEMFLAG_MOVEABLE;
	  }
#line 3717 "rcparse.c"
    break;

  case 206: /* memflag: PURE  */
#line 1515 "rcparse.y"
          {
	    (yyval.memflags).on = MEMFLAG_PURE;
	    (yyval.memflags).off = 0;
	  }
#line 3726 "rcparse.c"
    break;

  case 207: /* memflag: IMPURE  */
#line 1520 "rcparse.y"
          {
	    (yyval.memflags).on = 0;
	    (yyval.memflags).off = MEMFLAG_PURE;
	  }
#line 3735 "rcparse.c"
    break;

  case 208: /* memflag: PRELOAD  */
#line 1525 "rcparse.y"
          {
	    (yyval.memflags).on = MEMFLAG_PRELOAD;
	    (yyval.memflags).off = 0;
	  }
#line 3744 "rcparse.c"
    break;

  case 209: /* memflag: LOADONCALL  */
#line 1530 "rcparse.y"
          {
	    (yyval.memflags).on = 0;
	    (yyval.memflags).off = MEMFLAG_PRELOAD;
	  }
#line 3753 "rcparse.c"
    break;

  case 210: /* memflag: DISCARDABLE  */
#line 1535 "rcparse.y"
          {
	    (yyval.memflags).on = MEMFLAG_DISCARDABLE;
	    (yyval.memflags).off = 0;
	  }
#line 3762 "rcparse.c"
    break;

  case 211: /* file_name: QUOTEDSTRING  */
#line 1545 "rcparse.y"
          {
	    (yyval.s) = (yyvsp[0].s);
	  }
#line 3770 "rcparse.c"
    break;

  case 212: /* file_name: STRING  */
#line 1549 "rcparse.y"
          {
	    (yyval.s) = (yyvsp[0].s);
	  }
#line 3778 "rcparse.c"
    break;

  case 213: /* styleexpr: parennumber  */
#line 1566 "rcparse.y"
          {
	    style |= (yyvsp[0].il);
	  }
#line 3786 "rcparse.c"
    break;

  case 214: /* styleexpr: NOT parennumber  */
#line 1570 "rcparse.y"
          {
	    style &=~ (yyvsp[0].il);
	  }
#line 3794 "rcparse.c"
    break;

  case 215: /* styleexpr: styleexpr '|' parennumber  */
#line 1574 "rcparse.y"
          {
	    style |= (yyvsp[0].il);
	  }
#line 3802 "rcparse.c"
    break;

  case 216: /* styleexpr: styleexpr '|' NOT parennumber  */
#line 1578 "rcparse.y"
          {
	    style &=~ (yyvsp[0].il);
	  }
#line 3810 "rcparse.c"
    break;

  case 217: /* parennumber: NUMBER  */
#line 1585 "rcparse.y"
          {
	    (yyval.il) = (yyvsp[0].i).val;
	  }
#line 3818 "rcparse.c"
    break;

  case 218: /* parennumber: '(' numexpr ')'  */
#line 1589 "rcparse.y"
          {
	    (yyval.il) = (yyvsp[-1].il);
	  }
#line 3826 "rcparse.c"
    break;

  case 219: /* optcnumexpr: %empty  */
#line 1598 "rcparse.y"
          {
	    (yyval.il) = 0;
	  }
#line 3834 "rcparse.c"
    break;

  case 220: /* optcnumexpr: cnumexpr  */
#line 1602 "rcparse.y"
          {
	    (yyval.il) = (yyvsp[0].il);
	  }
#line 3842 "rcparse.c"
    break;

  case 221: /* cnumexpr: ',' numexpr  */
#line 1611 "rcparse.y"
          {
	    (yyval.il) = (yyvsp[0].il);
	  }
#line 3850 "rcparse.c"
    break;

  case 222: /* numexpr: sizednumexpr  */
#line 1620 "rcparse.y"
          {
	    (yyval.il) = (yyvsp[0].i).val;
	  }
#line 3858 "rcparse.c"
    break;

  case 223: /* sizednumexpr: NUMBER  */
#line 1629 "rcparse.y"
          {
	    (yyval.i) = (yyvsp[0].i);
	  }
#line 3866 "rcparse.c"
    break;

  case 224: /* sizednumexpr: '(' sizednumexpr ')'  */
#line 1633 "rcparse.y"
          {
	    (yyval.i) = (yyvsp[-1].i);
	  }
#line 3874 "rcparse.c"
    break;

  case 225: /* sizednumexpr: '~' sizednumexpr  */
#line 1637 "rcparse.y"
          {
	    (yyval.i).val = ~ (yyvsp[0].i).val;
	    (yyval.i).dword = (yyvsp[0].i).dword;
	  }
#line 3883 "rcparse.c"
    break;

  case 226: /* sizednumexpr: '-' sizednumexpr  */
#line 1642 "rcparse.y"
          {
	    (yyval.i).val = - (yyvsp[0].i).val;
	    (yyval.i).dword = (yyvsp[0].i).dword;
	  }
#line 3892 "rcparse.c"
    break;

  case 227: /* sizednumexpr: sizednumexpr '*' sizednumexpr  */
#line 1647 "rcparse.y"
          {
	    (yyval.i).val = (yyvsp[-2].i).val * (yyvsp[0].i).val;
	    (yyval.i).dword = (yyvsp[-2].i).dword || (yyvsp[0].i).dword;
	  }
#line 3901 "rcparse.c"
    break;

  case 228: /* sizednumexpr: sizednumexpr '/' sizednumexpr  */
#line 1652 "rcparse.y"
          {
	    (yyval.i).val = (yyvsp[-2].i).val / (yyvsp[0].i).val;
	    (yyval.i).dword = (yyvsp[-2].i).dword || (yyvsp[0].i).dword;
	  }
#line 3910 "rcparse.c"
    break;

  case 229: /* sizednumexpr: sizednumexpr '%' sizednumexpr  */
#line 1657 "rcparse.y"
          {
	    (yyval.i).val = (yyvsp[-2].i).val % (yyvsp[0].i).val;
	    (yyval.i).dword = (yyvsp[-2].i).dword || (yyvsp[0].i).dword;
	  }
#line 3919 "rcparse.c"
    break;

  case 230: /* sizednumexpr: sizednumexpr '+' sizednumexpr  */
#line 1662 "rcparse.y"
          {
	    (yyval.i).val = (yyvsp[-2].i).val + (yyvsp[0].i).val;
	    (yyval.i).dword = (yyvsp[-2].i).dword || (yyvsp[0].i).dword;
	  }
#line 3928 "rcparse.c"
    break;

  case 231: /* sizednumexpr: sizednumexpr '-' sizednumexpr  */
#line 1667 "rcparse.y"
          {
	    (yyval.i).val = (yyvsp[-2].i).val - (yyvsp[0].i).val;
	    (yyval.i).dword = (yyvsp[-2].i).dword || (yyvsp[0].i).dword;
	  }
#line 3937 "rcparse.c"
    break;

  case 232: /* sizednumexpr: sizednumexpr '&' sizednumexpr  */
#line 1672 "rcparse.y"
          {
	    (yyval.i).val = (yyvsp[-2].i).val & (yyvsp[0].i).val;
	    (yyval.i).dword = (yyvsp[-2].i).dword || (yyvsp[0].i).dword;
	  }
#line 3946 "rcparse.c"
    break;

  case 233: /* sizednumexpr: sizednumexpr '^' sizednumexpr  */
#line 1677 "rcparse.y"
          {
	    (yyval.i).val = (yyvsp[-2].i).val ^ (yyvsp[0].i).val;
	    (yyval.i).dword = (yyvsp[-2].i).dword || (yyvsp[0].i).dword;
	  }
#line 3955 "rcparse.c"
    break;

  case 234: /* sizednumexpr: sizednumexpr '|' sizednumexpr  */
#line 1682 "rcparse.y"
          {
	    (yyval.i).val = (yyvsp[-2].i).val | (yyvsp[0].i).val;
	    (yyval.i).dword = (yyvsp[-2].i).dword || (yyvsp[0].i).dword;
	  }
#line 3964 "rcparse.c"
    break;

  case 235: /* cposnumexpr: ',' posnumexpr  */
#line 1693 "rcparse.y"
          {
	    (yyval.il) = (yyvsp[0].il);
	  }
#line 3972 "rcparse.c"
    break;

  case 236: /* posnumexpr: sizedposnumexpr  */
#line 1702 "rcparse.y"
          {
	    (yyval.il) = (yyvsp[0].i).val;
	  }
#line 3980 "rcparse.c"
    break;

  case 237: /* sizedposnumexpr: NUMBER  */
#line 1713 "rcparse.y"
          {
	    (yyval.i) = (yyvsp[0].i);
	  }
#line 3988 "rcparse.c"
    break;

  case 238: /* sizedposnumexpr: '(' sizednumexpr ')'  */
#line 1717 "rcparse.y"
          {
	    (yyval.i) = (yyvsp[-1].i);
	  }
#line 3996 "rcparse.c"
    break;

  case 239: /* sizedposnumexpr: '~' sizednumexpr  */
#line 1721 "rcparse.y"
          {
	    (yyval.i).val = ~ (yyvsp[0].i).val;
	    (yyval.i).dword = (yyvsp[0].i).dword;
	  }
#line 4005 "rcparse.c"
    break;

  case 240: /* sizedposnumexpr: sizedposnumexpr '*' sizednumexpr  */
#line 1726 "rcparse.y"
          {
	    (yyval.i).val = (yyvsp[-2].i).val * (yyvsp[0].i).val;
	    (yyval.i).dword = (yyvsp[-2].i).dword || (yyvsp[0].i).dword;
	  }
#line 4014 "rcparse.c"
    break;

  case 241: /* sizedposnumexpr: sizedposnumexpr '/' sizednumexpr  */
#line 1731 "rcparse.y"
          {
	    (yyval.i).val = (yyvsp[-2].i).val / (yyvsp[0].i).val;
	    (yyval.i).dword = (yyvsp[-2].i).dword || (yyvsp[0].i).dword;
	  }
#line 4023 "rcparse.c"
    break;

  case 242: /* sizedposnumexpr: sizedposnumexpr '%' sizednumexpr  */
#line 1736 "rcparse.y"
          {
	    (yyval.i).val = (yyvsp[-2].i).val % (yyvsp[0].i).val;
	    (yyval.i).dword = (yyvsp[-2].i).dword || (yyvsp[0].i).dword;
	  }
#line 4032 "rcparse.c"
    break;

  case 243: /* sizedposnumexpr: sizedposnumexpr '+' sizednumexpr  */
#line 1741 "rcparse.y"
          {
	    (yyval.i).val = (yyvsp[-2].i).val + (yyvsp[0].i).val;
	    (yyval.i).dword = (yyvsp[-2].i).dword || (yyvsp[0].i).dword;
	  }
#line 4041 "rcparse.c"
    break;

  case 244: /* sizedposnumexpr: sizedposnumexpr '-' sizednumexpr  */
#line 1746 "rcparse.y"
          {
	    (yyval.i).val = (yyvsp[-2].i).val - (yyvsp[0].i).val;
	    (yyval.i).dword = (yyvsp[-2].i).dword || (yyvsp[0].i).dword;
	  }
#line 4050 "rcparse.c"
    break;

  case 245: /* sizedposnumexpr: sizedposnumexpr '&' sizednumexpr  */
#line 1751 "rcparse.y"
          {
	    (yyval.i).val = (yyvsp[-2].i).val & (yyvsp[0].i).val;
	    (yyval.i).dword = (yyvsp[-2].i).dword || (yyvsp[0].i).dword;
	  }
#line 4059 "rcparse.c"
    break;

  case 246: /* sizedposnumexpr: sizedposnumexpr '^' sizednumexpr  */
#line 1756 "rcparse.y"
          {
	    (yyval.i).val = (yyvsp[-2].i).val ^ (yyvsp[0].i).val;
	    (yyval.i).dword = (yyvsp[-2].i).dword || (yyvsp[0].i).dword;
	  }
#line 4068 "rcparse.c"
    break;

  case 247: /* sizedposnumexpr: sizedposnumexpr '|' sizednumexpr  */
#line 1761 "rcparse.y"
          {
	    (yyval.i).val = (yyvsp[-2].i).val | (yyvsp[0].i).val;
	    (yyval.i).dword = (yyvsp[-2].i).dword || (yyvsp[0].i).dword;
	  }
#line 4077 "rcparse.c"
    break;


#line 4081 "rcparse.c"

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

#line 1767 "rcparse.y"


/* Set the language from the command line: */
void
rcparse_set_language(int lang)
{
  language = lang;
}

/* End of rcparse.y */
