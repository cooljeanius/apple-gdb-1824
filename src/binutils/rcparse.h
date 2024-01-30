/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

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

#line 188 "rcparse.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_RCPARSE_H_INCLUDED  */
