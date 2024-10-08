/* ada-exp.y: YACC parser for Ada expressions, for GDB.
 * Copyright (C) 1986, 1989-1991, 1993-1994, 1997, 2000, 2003-2004
 * Free Software Foundation, Inc.  */
/*
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
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* Parse an Ada expression from text in a string,
   and return the result as a  struct expression  pointer.
   That structure contains arithmetic operations in reverse polish,
   with constants represented by operations that are followed by special data.
   See expression.h for the details of the format.
   What is important here is that it can be built up sequentially
   during the process of parsing; the lower levels of the tree always
   come first in the result.

   malloc's and realloc's in this file are transformed to
   xmalloc and xrealloc respectively by the same sed command in the
   makefile that remaps any other malloc/realloc inserted by the parser
   generator.  Doing this with #defines and trying to control the interaction
   with include files (<malloc.h> and <stdlib.h> for example) just became
   too messy, particularly when such includes can be inserted at random
   times by the parser generator.  */

%{

/* define this because of flex: */
#if !defined(NO_POISON) && defined(POISON_FREE_TOO)
# define NO_POISON 1
#endif /* !NO_POISON && POISON_FREE_TOO */

#include "defs.h"
#if defined(S_SPLINT_S)
# if !defined(_DARWIN_C_SOURCE)
#  define _DARWIN_C_SOURCE 1
# endif /* !_DARWIN_C_SOURCE */
#endif /* S_SPLINT_S */
#include "gdb_string.h"
#ifndef _STRING_H_
# include <string.h>
#endif /* !_STRING_H_ */
#ifdef HAVE_STRINGS_H
# include <strings.h>
#endif /* HAVE_STRINGS_H */
#include <ctype.h>
#include "expression.h"
#include "value.h"
#include "parser-defs.h"
#include "language.h"
#include "ada-lang.h"
#include "bfd.h" /* Required by objfiles.h.  */
#include "symfile.h" /* Required by objfiles.h.  */
#include "objfiles.h" /* For have_full_symbols and have_partial_symbols */
#include "frame.h"
#include "block.h"
#include "../include/obstack.h"

#ifndef ADA_EXP_Y
# define ADA_EXP_Y 1
#endif /* !ADA_EXP_Y */

/* Remap normal yacc parser interface names (yyparse, yylex, yyerror, etc),
   as well as gratuitiously global symbol names, so we can have multiple
   yacc generated parsers in gdb.  These are only the variables
   produced by yacc.  If other parser generators (bison, byacc, etc) produce
   additional global names that conflict at link time, then those parser
   generators need to be fixed instead of adding those names to this list.  */

/* NOTE: This is clumsy, especially since BISON and FLEX provide --prefix
   options.  I presume we are maintaining it to accommodate systems
   without BISON?  (PNH) */

#define	yymaxdepth ada_maxdepth
#define	yyparse	_ada_parse	/* ada_parse calls this after  initialization */
#define	yylex	ada_lex
#define	yyerror	ada_error
#define	yylval	ada_lval
#define	yychar	ada_char
#define	yydebug	ada_debug
#define	yypact	ada_pact
#define	yyr1	ada_r1
#define	yyr2	ada_r2
#define	yydef	ada_def
#define	yychk	ada_chk
#define	yypgo	ada_pgo
#define	yyact	ada_act
#define	yyexca	ada_exca
#define yyerrflag ada_errflag
#define yynerrs	ada_nerrs
#define	yyps	ada_ps
#define	yypv	ada_pv
#define	yys	ada_s
#define	yy_yys	ada_yys
#define	yystate	ada_state
#define	yytmp	ada_tmp
#define	yyv	ada_v
#define	yy_yyv	ada_yyv
#define	yyval	ada_val
#define	yylloc	ada_lloc
#define yyreds	ada_reds		/* With YYDEBUG defined */
#define yytoks	ada_toks		/* With YYDEBUG defined */
#define yyname	ada_name		/* With YYDEBUG defined */
#define yyrule	ada_rule		/* With YYDEBUG defined */

#ifndef yyinput
# define yyinput input
#endif /* !yyinput */

#ifndef YYDEBUG
# define YYDEBUG 1		/* Default to yydebug support */
#endif /* !YYDEBUG */

#define YYFPRINTF parser_fprintf

struct name_info {
  struct symbol *sym;
  struct minimal_symbol *msym;
  struct block *block;
  struct stoken stoken;
};

/* If expression is in the context of TYPE'(...), then TYPE, else
 * NULL.  */
static struct type *type_qualifier;

int yyparse(void);

static int yylex(void);

void yyerror(const char *);

static struct stoken string_to_operator(struct stoken);

static void write_int(LONGEST, struct type *);

static void write_object_renaming(struct block *, struct symbol *, int);

static void write_var_from_name(struct block *, struct name_info);

static LONGEST convert_char_literal(struct type *, LONGEST);

static struct type *type_int(void);

static struct type *type_long(void);

static struct type *type_long_long(void);

static struct type *type_float(void);

static struct type *type_double(void);

static struct type *type_long_double(void);

static struct type *type_char(void);

static struct type *type_system_address(void);

extern void _initialize_ada_exp(void);
%}

%union
  {
    LONGEST lval;
    struct {
      LONGEST val;
      struct type *type;
    } typed_val;
    struct {
      DOUBLEST dval;
      struct type *type;
    } typed_val_float;
    struct type *tval;
    struct stoken sval;
    struct name_info ssym;
    int voidval;
    struct block *bval;
    struct internalvar *ivar;
  }

%type <voidval> exp exp1 simple_exp start variable
%type <tval> type

%token <typed_val> INT NULL_PTR CHARLIT
%token <typed_val_float> FLOAT
%token <tval> TYPENAME
%token <bval> BLOCKNAME

/* Both NAME and TYPENAME tokens represent symbols in the input,
   and both convey their data as strings.
   But a TYPENAME is a string that happens to be defined as a typedef
   or builtin type name (such as int or char)
   and a NAME is any other symbol.
   Contexts where this distinction is not important can use the
   nonterminal "name", which matches either NAME or TYPENAME.  */

%token <sval> STRING
%token <ssym> NAME DOT_ID OBJECT_RENAMING
%type <bval> block
%type <lval> arglist tick_arglist

%type <tval> save_qualifier

%token DOT_ALL

/* Special type cases, put in to allow the parser to distinguish different
   legal basetypes.  */
%token <sval> SPECIAL_VARIABLE

%nonassoc ASSIGN
%left _AND_ OR XOR THEN ELSE
%left '=' NOTEQUAL '<' '>' LEQ GEQ IN DOTDOT
%left '@'
%left '+' '-' '&'
%left UNARY
%left '*' '/' MOD REM
%right STARSTAR ABS NOT
 /* The following are right-associative only so that reductions at this
    precedence have lower precedence than '.' and '('.  The syntax still
    forces a.b.c, e.g., to be LEFT-associated.  */
%right TICK_ACCESS TICK_ADDRESS TICK_FIRST TICK_LAST TICK_LENGTH
%right TICK_MAX TICK_MIN TICK_MODULUS
%right TICK_POS TICK_RANGE TICK_SIZE TICK_TAG TICK_VAL
%right '.' '(' '[' DOT_ID DOT_ALL

%token ARROW NEW


%%

start   :	exp1
	|	type	{ write_exp_elt_opcode(OP_TYPE);
			  write_exp_elt_type($1);
 			  write_exp_elt_opcode(OP_TYPE); }
	;

/* Expressions, including the sequencing operator.  */
exp1	:	exp
	|	exp1 ';' exp
			{ write_exp_elt_opcode(BINOP_COMMA); }
	;

/* Expressions, not including the sequencing operator: */
simple_exp :	simple_exp DOT_ALL
			{ write_exp_elt_opcode(UNOP_IND); }
	;

simple_exp :	simple_exp DOT_ID
			{ write_exp_elt_opcode(STRUCTOP_STRUCT);
			  write_exp_string($2.stoken);
			  write_exp_elt_opcode(STRUCTOP_STRUCT);
			  }
	;

simple_exp :	simple_exp '(' arglist ')'
			{
			  write_exp_elt_opcode(OP_FUNCALL);
			  write_exp_elt_longcst($3);
			  write_exp_elt_opcode(OP_FUNCALL);
		        }
	;

simple_exp :	type '(' exp ')'
			{
			  write_exp_elt_opcode(UNOP_CAST);
			  write_exp_elt_type($1);
			  write_exp_elt_opcode(UNOP_CAST);
			}
	;

simple_exp :	type '\'' save_qualifier { type_qualifier = $1; } '(' exp ')'
			{
			  write_exp_elt_opcode((enum exp_opcode)UNOP_QUAL);
			  write_exp_elt_type($1);
			  write_exp_elt_opcode((enum exp_opcode)UNOP_QUAL);
			  type_qualifier = $3;
			}
	;

save_qualifier : 	{ $$ = type_qualifier; }
	;

simple_exp :
		simple_exp '(' exp DOTDOT exp ')'
			{ write_exp_elt_opcode(TERNOP_SLICE); }
	;

simple_exp :	'(' exp1 ')'	{ }
	;

simple_exp :	variable
	;

simple_exp:	SPECIAL_VARIABLE /* Various GDB extensions */
			{ write_dollar_variable($1); }
	;

exp	: 	simple_exp
	;

exp	: 	exp ASSIGN exp   /* Extension for convenience */
			{ write_exp_elt_opcode(BINOP_ASSIGN); }
	;

exp	:	'-' exp    %prec UNARY
			{ write_exp_elt_opcode(UNOP_NEG); }
	;

exp	:	'+' exp    %prec UNARY
			{ write_exp_elt_opcode(UNOP_PLUS); }
	;

exp     :	NOT exp    %prec UNARY
			{ write_exp_elt_opcode(UNOP_LOGICAL_NOT); }
	;

exp	:       ABS exp	   %prec UNARY
			{ write_exp_elt_opcode(UNOP_ABS); }
	;

arglist	:		{ $$ = 0; }
	;

arglist	:	exp
			{ $$ = 1; }
	|	any_name ARROW exp
			{ $$ = 1; }
	|	arglist ',' exp
			{ $$ = $1 + 1; }
	|	arglist ',' any_name ARROW exp
			{ $$ = $1 + 1; }
	;

exp	:	'{' type '}' exp  %prec '.'
		/* GDB extension */
			{ write_exp_elt_opcode(UNOP_MEMVAL);
			  write_exp_elt_type($2);
			  write_exp_elt_opcode(UNOP_MEMVAL);
			}
	;

/* Binary operators in order of decreasing precedence.  */

exp 	: 	exp STARSTAR exp
			{ write_exp_elt_opcode(BINOP_EXP); }
	;

exp	:	exp '*' exp
			{ write_exp_elt_opcode(BINOP_MUL); }
	;

exp	:	exp '/' exp
			{ write_exp_elt_opcode(BINOP_DIV); }
	;

exp	:	exp REM exp /* May need to be fixed to give correct Ada REM */
			{ write_exp_elt_opcode(BINOP_REM); }
	;

exp	:	exp MOD exp
			{ write_exp_elt_opcode(BINOP_MOD); }
	;

exp	:	exp '@' exp	/* GDB extension */
			{ write_exp_elt_opcode(BINOP_REPEAT); }
	;

exp	:	exp '+' exp
			{ write_exp_elt_opcode(BINOP_ADD); }
	;

exp	:	exp '&' exp
			{ write_exp_elt_opcode(BINOP_CONCAT); }
	;

exp	:	exp '-' exp
			{ write_exp_elt_opcode(BINOP_SUB); }
	;

exp	:	exp '=' exp
			{ write_exp_elt_opcode(BINOP_EQUAL); }
	;

exp	:	exp NOTEQUAL exp
			{ write_exp_elt_opcode(BINOP_NOTEQUAL); }
	;

exp	:	exp LEQ exp
			{ write_exp_elt_opcode(BINOP_LEQ); }
	;

exp	:	exp IN exp DOTDOT exp
			{ write_exp_elt_opcode((enum exp_opcode)TERNOP_IN_RANGE); }
        |       exp IN exp TICK_RANGE tick_arglist
			{ write_exp_elt_opcode((enum exp_opcode)BINOP_IN_BOUNDS);
			  write_exp_elt_longcst((LONGEST)$5);
			  write_exp_elt_opcode((enum exp_opcode)BINOP_IN_BOUNDS);
			}
 	|	exp IN TYPENAME		%prec TICK_ACCESS
			{ write_exp_elt_opcode((enum exp_opcode)UNOP_IN_RANGE);
		          write_exp_elt_type($3);
		          write_exp_elt_opcode((enum exp_opcode)UNOP_IN_RANGE);
			}
	|	exp NOT IN exp DOTDOT exp
			{ write_exp_elt_opcode((enum exp_opcode)TERNOP_IN_RANGE);
		          write_exp_elt_opcode(UNOP_LOGICAL_NOT);
			}
        |       exp NOT IN exp TICK_RANGE tick_arglist
			{ write_exp_elt_opcode((enum exp_opcode)BINOP_IN_BOUNDS);
			  write_exp_elt_longcst((LONGEST)$6);
			  write_exp_elt_opcode((enum exp_opcode)BINOP_IN_BOUNDS);
		          write_exp_elt_opcode(UNOP_LOGICAL_NOT);
			}
 	|	exp NOT IN TYPENAME	%prec TICK_ACCESS
			{ write_exp_elt_opcode((enum exp_opcode)UNOP_IN_RANGE);
		          write_exp_elt_type($4);
		          write_exp_elt_opcode((enum exp_opcode)UNOP_IN_RANGE);
		          write_exp_elt_opcode(UNOP_LOGICAL_NOT);
			}
	;

exp	:	exp GEQ exp
			{ write_exp_elt_opcode(BINOP_GEQ); }
	;

exp	:	exp '<' exp
			{ write_exp_elt_opcode(BINOP_LESS); }
	;

exp	:	exp '>' exp
			{ write_exp_elt_opcode(BINOP_GTR); }
	;

exp     :	exp _AND_ exp  /* Fix for Ada elementwise AND.  */
			{ write_exp_elt_opcode(BINOP_BITWISE_AND); }
        ;

exp     :       exp _AND_ THEN exp	%prec _AND_
			{ write_exp_elt_opcode(BINOP_LOGICAL_AND); }
        ;

exp     :	exp OR exp     /* Fix for Ada elementwise OR */
			{ write_exp_elt_opcode(BINOP_BITWISE_IOR); }
        ;

exp     :       exp OR ELSE exp
			{ write_exp_elt_opcode(BINOP_LOGICAL_OR); }
        ;

exp     :       exp XOR exp    /* Fix for Ada elementwise XOR */
			{ write_exp_elt_opcode(BINOP_BITWISE_XOR); }
        ;

simple_exp :	simple_exp TICK_ACCESS
			{ write_exp_elt_opcode(UNOP_ADDR); }
	|	simple_exp TICK_ADDRESS
			{ write_exp_elt_opcode(UNOP_ADDR);
			  write_exp_elt_opcode(UNOP_CAST);
			  write_exp_elt_type(type_system_address());
			  write_exp_elt_opcode(UNOP_CAST);
			}
	|	simple_exp TICK_FIRST tick_arglist
			{ write_int($3, type_int());
			  write_exp_elt_opcode((enum exp_opcode)OP_ATR_FIRST); }
	|	simple_exp TICK_LAST tick_arglist
			{ write_int($3, type_int());
			  write_exp_elt_opcode((enum exp_opcode)OP_ATR_LAST); }
	| 	simple_exp TICK_LENGTH tick_arglist
			{ write_int($3, type_int());
			  write_exp_elt_opcode((enum exp_opcode)OP_ATR_LENGTH); }
        |       simple_exp TICK_SIZE
			{ write_exp_elt_opcode((enum exp_opcode)OP_ATR_SIZE); }
	|	simple_exp TICK_TAG
			{ write_exp_elt_opcode((enum exp_opcode)OP_ATR_TAG); }
        |       opt_type_prefix TICK_MIN '(' exp ',' exp ')'
			{ write_exp_elt_opcode((enum exp_opcode)OP_ATR_MIN); }
        |       opt_type_prefix TICK_MAX '(' exp ',' exp ')'
			{ write_exp_elt_opcode((enum exp_opcode)OP_ATR_MAX); }
	| 	opt_type_prefix TICK_POS '(' exp ')'
			{ write_exp_elt_opcode((enum exp_opcode)OP_ATR_POS); }
	|	type_prefix TICK_FIRST tick_arglist
			{ write_int($3, type_int());
			  write_exp_elt_opcode((enum exp_opcode)OP_ATR_FIRST); }
	|	type_prefix TICK_LAST tick_arglist
			{ write_int($3, type_int());
			  write_exp_elt_opcode((enum exp_opcode)OP_ATR_LAST); }
	| 	type_prefix TICK_LENGTH tick_arglist
			{ write_int($3, type_int());
			  write_exp_elt_opcode((enum exp_opcode)OP_ATR_LENGTH); }
	|	type_prefix TICK_VAL '(' exp ')'
			{ write_exp_elt_opcode((enum exp_opcode)OP_ATR_VAL); }
	|	type_prefix TICK_MODULUS
			{ write_exp_elt_opcode((enum exp_opcode)OP_ATR_MODULUS); }
	;

tick_arglist :			%prec '('
			{ $$ = 1; }
	| 	'(' INT ')'
			{ $$ = $2.val; }
	;

type_prefix :
		TYPENAME
			{ write_exp_elt_opcode(OP_TYPE);
			  write_exp_elt_type($1);
			  write_exp_elt_opcode(OP_TYPE); }
	;

opt_type_prefix :
		type_prefix
	| 	/* EMPTY */
			{ write_exp_elt_opcode(OP_TYPE);
			  write_exp_elt_type(builtin_type_void);
			  write_exp_elt_opcode(OP_TYPE); }
	;


exp	:	INT
			{ write_int((LONGEST)$1.val, $1.type); }
	;

exp	:	CHARLIT
                  { write_int(convert_char_literal(type_qualifier, $1.val),
			      ((type_qualifier == NULL)
                               ? $1.type : type_qualifier));
		  }
	;

exp	:	FLOAT
			{ write_exp_elt_opcode(OP_DOUBLE);
			  write_exp_elt_type($1.type);
			  write_exp_elt_dblcst($1.dval);
			  write_exp_elt_opcode(OP_DOUBLE);
			}
	;

exp	:	NULL_PTR
			{ write_int(0, type_int()); }
	;

exp	:	STRING
			{
			  write_exp_elt_opcode(OP_STRING);
			  write_exp_string($1);
			  write_exp_elt_opcode(OP_STRING);
			}
	;

exp	: 	NEW TYPENAME
			{ error("NEW not implemented."); }
	;

variable:	NAME   		{ write_var_from_name(NULL, $1); }
	|	block NAME  	/* GDB extension */
                                { write_var_from_name($1, $2); }
	|	OBJECT_RENAMING
		    { write_object_renaming(NULL, $1.sym,
				            MAX_RENAMING_CHAIN_LENGTH); }
	|	block OBJECT_RENAMING
		    { write_object_renaming($1, $2.sym,
					    MAX_RENAMING_CHAIN_LENGTH); }
	;

any_name :	NAME 		{ }
        |       TYPENAME	{ }
        |       OBJECT_RENAMING	{ }
        ;

block	:	BLOCKNAME  /* GDB extension */
			{ $$ = $1; }
	|	block BLOCKNAME /* GDB extension */
			{ $$ = $2; }
	;


type	:	TYPENAME	{ $$ = $1; }
	|	block TYPENAME  { $$ = $2; }
	| 	TYPENAME TICK_ACCESS
				{ $$ = lookup_pointer_type($1); }
	|	block TYPENAME TICK_ACCESS
				{ $$ = lookup_pointer_type($2); }
        ;

/* Some extensions borrowed from C, for the benefit of those who find they
   cannot get used to Ada notation in GDB.  */

exp	:	'*' exp		%prec '.'
			{ write_exp_elt_opcode(UNOP_IND); }
	|	'&' exp		%prec '.'
			{ write_exp_elt_opcode(UNOP_ADDR); }
	|	exp '[' exp ']'
			{ write_exp_elt_opcode(BINOP_SUBSCRIPT); }
	;

%%

/* yylex defined in ada-lex.c: Reads one token, getting characters */
/* through lexptr.  */

/* Remap normal flex interface names (yylex) as well as gratuitiously */
/* global symbol names, so we can have multiple flex-generated parsers */
/* in gdb.  */

/* (See note above on previous definitions for YACC.) */

#define yy_create_buffer ada_yy_create_buffer
#define yy_delete_buffer ada_yy_delete_buffer
#define yy_init_buffer ada_yy_init_buffer
#define yy_load_buffer_state ada_yy_load_buffer_state
#define yy_switch_to_buffer ada_yy_switch_to_buffer
#define yyrestart ada_yyrestart
#define yytext ada_yytext
#define yywrap ada_yywrap

static struct obstack temp_parse_space;

/* The following kludge was found necessary to prevent conflicts between */
/* defs.h and non-standard stdlib.h files.  */
#define qsort __qsort__dummy
#include "ada-lex.c"

#if defined(obstack_free) && defined(WITH_GNULIB_BUILDDIR)
# undef obstack_free
#endif /* obstack_free && WITH_GNULIB_BUILDDIR */

int
ada_parse(void)
{
  lexer_init(yyin);		/* (Re-)initialize lexer.  */
  left_block_context = NULL;
  type_qualifier = NULL;
  obstack_free(&temp_parse_space, NULL);
  obstack_init(&temp_parse_space);

  return _ada_parse();
}

void ATTR_NORETURN
yyerror(const char *msg)
{
  error("A %s in expression, near '%s'.", (msg ? msg : "error"),
  	(lexptr ? lexptr : "lexptr"));
}

/* The operator name corresponding to operator symbol STRING (adds
   quotes and maps to lower-case).  Destroys the previous contents of
   the array pointed to by STRING.ptr.  Error if STRING does not match
   a valid Ada operator.  Assumes that STRING.ptr points to a
   null-terminated string and that, if STRING is a valid operator
   symbol, the array pointed to by STRING.ptr contains at least
   STRING.length+3 characters.  */

static struct stoken
string_to_operator(struct stoken string)
{
  int i;

  for (i = 0; ada_opname_table[i].encoded != NULL; i += 1)
    {
      if ((string.length == (strlen(ada_opname_table[i].decoded) - 2UL))
	  && (strncasecmp(string.ptr, (ada_opname_table[i].decoded + 1),
			  string.length) == 0))
	{
	  strncpy(string.ptr, ada_opname_table[i].decoded,
		  (string.length + 2UL));
	  string.length += 2;
	  return string;
	}
    }
  error(_("Invalid operator symbol `%s'"), string.ptr);
  /*NOTREACHED*/
}

/* Emit expression to access an instance of SYM, in block BLOCK (if
 * non-NULL), and with :: qualification ORIG_LEFT_CONTEXT.  */
static void
write_var_from_sym(struct block *orig_left_context, const struct block *block,
		   struct symbol *sym)
{
  if ((orig_left_context == NULL) && symbol_read_needs_frame(sym))
    {
      if ((innermost_block == 0)
	  || contained_in(block, innermost_block))
	innermost_block = (struct block *)block;
    }

  write_exp_elt_opcode(OP_VAR_VALUE);
  write_exp_elt_block(block);
  write_exp_elt_sym(sym);
  write_exp_elt_opcode(OP_VAR_VALUE);
}

/* Emit expression to access an instance of NAME in :: context
 * ORIG_LEFT_CONTEXT.  If no unique symbol for NAME has been found,
 * output a dummy symbol (good to the next call of ada_parse) for NAME
 * in the UNDEF_DOMAIN, for later resolution by ada_resolve.  */
static void
write_var_from_name(struct block *orig_left_context,
		    struct name_info name)
{
  if (name.msym != NULL)
    {
      write_exp_msymbol(name.msym,
                        lookup_function_type(type_int()),
                        type_int());
    }
  else if (name.sym == NULL)
    {
      /* Multiple matches: record name and starting block for later
         resolution by ada_resolve.  */
      char *encoded_name = ada_encode(name.stoken.ptr);
      struct symbol *sym =
	(struct symbol *)obstack_alloc(&temp_parse_space,
                                       sizeof(struct symbol));
      memset(sym, 0, sizeof(struct symbol));
      SYMBOL_DOMAIN(sym) = UNDEF_DOMAIN;
      SYMBOL_LINKAGE_NAME(sym) =
	obsavestring(encoded_name, (int)strlen(encoded_name),
		     &temp_parse_space);
      SYMBOL_LANGUAGE(sym) = language_ada;

      write_exp_elt_opcode(OP_VAR_VALUE);
      write_exp_elt_block(name.block);
      write_exp_elt_sym(sym);
      write_exp_elt_opcode(OP_VAR_VALUE);
    }
  else
    write_var_from_sym(orig_left_context, name.block, name.sym);
}

/* Write integer constant ARG of type TYPE: */
static void
write_int(LONGEST arg, struct type *type)
{
  write_exp_elt_opcode(OP_LONG);
  write_exp_elt_type(type);
  write_exp_elt_longcst(arg);
  write_exp_elt_opcode(OP_LONG);
}

/* Emit expression corresponding to the renamed object designated by
 * the type RENAMING, which must be the referent of an object renaming
 * type, in the context of ORIG_LEFT_CONTEXT.  MAX_DEPTH is the maximum
 * number of cascaded renamings to allow.  */
static void
write_object_renaming (struct block *orig_left_context,
		       struct symbol *renaming, int max_depth)
{
  const char *qualification = SYMBOL_LINKAGE_NAME (renaming);
  const char *simple_tail;
  const char *expr = TYPE_FIELD_NAME (SYMBOL_TYPE (renaming), 0);
  const char *suffix;
  char *name;
  struct symbol *sym;
  enum { SIMPLE_INDEX, LOWER_BOUND, UPPER_BOUND } slice_state;

  if (max_depth <= 0)
    error ("Could not find renamed symbol");

  /* if orig_left_context is null, then use the currently selected
     block; otherwise we might fail our symbol lookup below.  */
  if (orig_left_context == NULL)
    orig_left_context = get_selected_block (NULL);

  for (simple_tail = qualification + strlen (qualification);
       simple_tail != qualification; simple_tail -= 1)
    {
      if (*simple_tail == '.')
	{
	  simple_tail += 1;
	  break;
	}
      else if (strncmp (simple_tail, "__", 2) == 0)
	{
	  simple_tail += 2;
	  break;
	}
    }

  suffix = strstr (expr, "___XE");
  if (suffix == NULL)
    goto BadEncoding;

  name = (char *)obstack_alloc(&temp_parse_space, (int)(suffix - expr + 1));
  strncpy(name, expr, (size_t)(suffix - expr));
  name[suffix - expr] = '\000';
  sym = lookup_symbol(name, orig_left_context, VAR_DOMAIN, 0, NULL);
  if (sym == NULL)
    error(_("Could not find renamed variable: %s"), ada_decode(name));
  if (ada_is_object_renaming(sym))
    write_object_renaming(orig_left_context, sym, max_depth-1);
  else
    write_var_from_sym(orig_left_context, block_found, sym);

  suffix += 5;
  slice_state = SIMPLE_INDEX;
  while (*suffix == 'X')
    {
      suffix += 1;

      switch (*suffix) {
      case 'A':
        suffix += 1;
        write_exp_elt_opcode(UNOP_IND);
        break;
      case 'L':
	slice_state = LOWER_BOUND;
	break; /* for -Wimplicit-fallthrough */
      case 'S':
	suffix += 1;
	if (isdigit (*suffix))
	  {
	    char *next;
	    long val = strtol (suffix, &next, 10);
	    if (next == suffix)
	      goto BadEncoding;
	    suffix = next;
	    write_exp_elt_opcode (OP_LONG);
	    write_exp_elt_type (type_int ());
	    write_exp_elt_longcst ((LONGEST) val);
	    write_exp_elt_opcode (OP_LONG);
	  }
	else
	  {
	    const char *end;
	    char *index_name;
	    size_t index_len;
	    struct symbol *index_sym;

	    end = strchr (suffix, 'X');
	    if (end == NULL)
	      end = suffix + strlen(suffix);

	    index_len = ((size_t)(simple_tail - qualification) + 2UL
			 + (size_t)(suffix - end) + 1UL);
	    index_name = (char *)obstack_alloc(&temp_parse_space,
					       (int)index_len);
	    memset(index_name, '\000', index_len);
	    strncpy(index_name, qualification,
		    (size_t)(simple_tail - qualification));
	    index_name[simple_tail - qualification] = '\000';
	    strncat(index_name, suffix, (size_t)(suffix - end));
	    suffix = end;

	    index_sym =
	      lookup_symbol(index_name, NULL, VAR_DOMAIN, 0, NULL);
	    if (index_sym == NULL)
	      error(_("Failed to find %s"), index_name);
	    write_var_from_sym(NULL, block_found, sym);
	  }
	if (slice_state == SIMPLE_INDEX)
	  {
	    write_exp_elt_opcode(OP_FUNCALL);
	    write_exp_elt_longcst((LONGEST)1L);
	    write_exp_elt_opcode(OP_FUNCALL);
	  }
	else if (slice_state == LOWER_BOUND)
	  slice_state = UPPER_BOUND;
	else if (slice_state == UPPER_BOUND)
	  {
	    write_exp_elt_opcode(TERNOP_SLICE);
	    slice_state = SIMPLE_INDEX;
	  }
	break;

      case 'R':
	{
	  struct stoken field_name;
	  const char *end;
	  suffix += 1;

	  if (slice_state != SIMPLE_INDEX)
	    goto BadEncoding;
	  end = strchr(suffix, 'X');
	  if (end == NULL)
	    end = (suffix + strlen(suffix));
	  field_name.length = (size_t)(end - suffix);
	  field_name.ptr = (char *)xmalloc((size_t)(end - suffix) + 1UL);
	  strncpy(field_name.ptr, suffix, (size_t)(end - suffix));
	  field_name.ptr[end - suffix] = '\000';
	  suffix = end;
	  write_exp_elt_opcode(STRUCTOP_STRUCT);
	  write_exp_string(field_name);
	  write_exp_elt_opcode(STRUCTOP_STRUCT);
	  break;
	}

      default:
	goto BadEncoding;
      }
    }
  if (slice_state == SIMPLE_INDEX)
    return;

 BadEncoding:
  error("Internal error in encoding of renaming declaration: %s",
        SYMBOL_LINKAGE_NAME(renaming));
}

/* Convert the character literal whose ASCII value would be VAL to the
   appropriate value of type TYPE, if there is a translation.
   Otherwise return VAL.  Hence, in an enumeration type ('A', 'B'),
   the literal 'A' (VAL == 65), returns 0.  */
static LONGEST
convert_char_literal(struct type *type, LONGEST val)
{
  char name[11]; /* Big enough for -Wformat-length */
  int f;

  if ((type == NULL) || (TYPE_CODE(type) != TYPE_CODE_ENUM))
    return val;
  snprintf(name, sizeof(name), "QU%02x", (unsigned int)val);
  for (f = 0; f < TYPE_NFIELDS(type); f += 1)
    {
      if (strcmp(name, TYPE_FIELD_NAME(type, f)) == 0)
	return TYPE_FIELD_BITPOS(type, f);
    }
  return val;
}

static struct type *
type_int(void)
{
  return get_builtin_type(current_gdbarch)->builtin_int;
}

static struct type *
type_long(void)
{
  return get_builtin_type(current_gdbarch)->builtin_long;
}

static struct type *
type_long_long(void)
{
  return get_builtin_type(current_gdbarch)->builtin_long_long;
}

static struct type *
type_float(void)
{
  return get_builtin_type(current_gdbarch)->builtin_float;
}

static struct type *
type_double(void)
{
  return get_builtin_type(current_gdbarch)->builtin_double;
}

static struct type *
type_long_double(void)
{
  return get_builtin_type(current_gdbarch)->builtin_long_double;
}

static struct type *
type_char(void)
{
  return language_string_char_type(current_language, current_gdbarch);
}

static struct type *
type_system_address(void)
{
  struct type *type
    = language_lookup_primitive_type_by_name(current_language,
                                             current_gdbarch,
                                             "system__address");
  return ((type != NULL) ? type : lookup_pointer_type(builtin_type_void));
}

void
_initialize_ada_exp(void)
{
  obstack_init(&temp_parse_space);
}

/* FIXME: hilfingr/2004-10-05: Hack to remove warning.  The function
   string_to_operator is supposed to be used for cases where one
   calls an operator function with prefix notation, as in
   "+" (a, b), but at some point, this code seems to have gone
   missing. */

struct stoken(*dummy_string_to_ada_operator)(struct stoken)
     = string_to_operator;

#ifdef qsort
# undef qsort
#endif /* qsort */

/* End of ada-exp.y */
