/* p-exp.y: YACC parser for Pascal expressions, for GDB.
 * Copyright 2000
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
Foundation, Inc., 59 Temple Pl., Suite 330, Boston, MA 02111-1307, USA */

/* This file is derived from c-exp.y */

/* Parse a Pascal expression from text in a string,
   and return the result as a  struct expression  pointer.
   That structure contains arithmetic operations in reverse polish,
   with constants represented by operations that are followed by special data.
   See expression.h for the details of the format.
   What is important here is that it can be built up sequentially
   during the process of parsing; the lower levels of the tree always
   come first in the result.

   Note that malloc's, realloc's, and free's in this file are transformed to
   xmalloc, xrealloc, and xfree respectively by the same sed command in the
   makefile that remaps any other malloc/realloc/free inserted by the parser
   generator.  Doing this with #defines and trying to control the interaction
   with include files (<malloc.h> and <stdlib.h> for example) just became
   too messy, particularly when such includes can be inserted at random
   times by the parser generator.  */

/* Known bugs or limitations:
    - pascal string operations are not supported at all.
    - there are some problems with boolean types.
    - Pascal type hexadecimal constants are not supported
      because they conflict with the internal variables format.
   Probably also lots of other problems, less well defined PM */
%{

#include "defs.h"
#include "gdb_string.h"
#include <ctype.h>
#ifdef HAVE_LIMITS_H
# include <limits.h>
#endif /* HAVE_LIMITS_H */
#include "expression.h"
#include "value.h"
#include "parser-defs.h"
#include "language.h"
#include "p-lang.h"
#include "bfd.h" /* Required by objfiles.h.  */
#include "symfile.h" /* Required by objfiles.h.  */
#include "objfiles.h" /* For have_full_symbols and have_partial_symbols */
#include "block.h"
#include "top.h"
#include "completer.h"

/* Remap normal yacc parser interface names (yyparse, yylex, yyerror, etc),
 * as well as gratuitiously global symbol names, so we can have multiple
 * yacc generated parsers in gdb.  Note that these are only the variables
 * produced by yacc.  If other parser generators (bison, byacc, etc.)
 * produce additional global names that conflict at link time, then those
 * parser generators need to be fixed instead of adding those names to this
 * list. */

#define	yymaxdepth pascal_maxdepth
#define	yyparse	pascal_parse
#define	yylex	pascal_lex
#define	yyerror	pascal_error
#define	yylval	pascal_lval
#define	yychar	pascal_char
#define	yydebug	pascal_debug
#define	yypact	pascal_pact
#define	yyr1	pascal_r1
#define	yyr2	pascal_r2
#define	yydef	pascal_def
#define	yychk	pascal_chk
#define	yypgo	pascal_pgo
#define	yyact	pascal_act
#define	yyexca	pascal_exca
#define yyerrflag pascal_errflag
#define yynerrs	pascal_nerrs
#define	yyps	pascal_ps
#define	yypv	pascal_pv
#define	yys	pascal_s
#define	yy_yys	pascal_yys
#define	yystate	pascal_state
#define	yytmp	pascal_tmp
#define	yyv	pascal_v
#define	yy_yyv	pascal_yyv
#define	yyval	pascal_val
#define	yylloc	pascal_lloc
#define yyreds	pascal_reds		/* With YYDEBUG defined */
#define yytoks	pascal_toks		/* With YYDEBUG defined */
#define yyname	pascal_name		/* With YYDEBUG defined */
#define yyrule	pascal_rule		/* With YYDEBUG defined */
#define yylhs	pascal_yylhs
#define yylen	pascal_yylen
#define yydefred pascal_yydefred
#define yydgoto	pascal_yydgoto
#define yysindex pascal_yysindex
#define yyrindex pascal_yyrindex
#define yygindex pascal_yygindex
#define yytable	 pascal_yytable
#define yycheck	 pascal_yycheck

#ifndef YYDEBUG
# define YYDEBUG 1		/* Default to yydebug support */
#endif /* !YYDEBUG */

#define YYFPRINTF parser_fprintf

int yyparse(void);

static int yylex(void);

void
yyerror(const char *);

static char *uptok(char *, int);
%}

/* Although the yacc "value" of an expression is not used,
   since the result is stored in the structure being created,
   other node types do have values.  */

%union
  {
    LONGEST lval;
    struct {
      LONGEST val;
      struct type *type;
    } typed_val_int;
    struct {
      DOUBLEST dval;
      struct type *type;
    } typed_val_float;
    struct symbol *sym;
    struct type *tval;
    struct stoken sval;
    struct ttype tsym;
    struct symtoken ssym;
    int voidval;
    struct block *bval;
    enum exp_opcode opcode;
    struct internalvar *ivar;

    struct type **tvec;
    int *ivec;
  }

%{
/* YYSTYPE gets defined by %union */
static int
parse_number(char *, int, int, YYSTYPE *);

static struct type *current_type;

static void push_current_type(void);
static void pop_current_type(void);
static int search_field;
%}

%type <voidval> exp exp1 type_exp start normal_start variable qualified_name
%type <tval> type typebase
/* %type <bval> block */

/* Fancy type parsing: */
%type <tval> ptype

%token <typed_val_int> INT
%token <typed_val_float> FLOAT

/* Both NAME and TYPENAME tokens represent symbols in the input,
   and both convey their data as strings.
   But a TYPENAME is a string that happens to be defined as a typedef
   or builtin type name (such as int or char)
   and a NAME is any other symbol.
   Contexts where this distinction is not important can use the
   nonterminal "name", which matches either NAME or TYPENAME.  */

%token <sval> STRING
%token <sval> FIELDNAME
%token <ssym> NAME /* BLOCKNAME defined below to give it higher precedence. */
%token <tsym> TYPENAME
%type <sval> name
%type <ssym> name_not_typename

/* A NAME_OR_INT is a symbol which is not known in the symbol table,
   but which would parse as a valid number in the current input radix.
   E.g. "c" when input_radix==16.  Depending on the parse, it will be
   turned into a name or into a number.  */

%token <ssym> NAME_OR_INT

%token STRUCT CLASS SIZEOF COLONCOLON
%token ERROR

/* Special type cases, put in to allow the parser to distinguish different
   legal basetypes.  */

%token <voidval> VARIABLE


/* Object pascal */
%token THIS
%token <lval> TRUEKEYWORD FALSEKEYWORD

%left ','
%left ABOVE_COMMA
%right ASSIGN
%left NOT
%left OR
%left XOR
%left ANDAND
%left '=' NOTEQUAL
%left '<' '>' LEQ GEQ
%left LSH RSH DIV MOD
%left '@'
%left '+' '-'
%left '*' '/'
%right UNARY INCREMENT DECREMENT
%right ARROW '.' '[' '('
%left '^'
%token <ssym> BLOCKNAME
%type <bval> block
%left COLONCOLON


%%

start   :	{ current_type = NULL;
		  search_field = 0;
		}
		normal_start {}
	;

normal_start	:
		exp1
	|	type_exp
	;

type_exp:	type
			{ write_exp_elt_opcode(OP_TYPE);
			  write_exp_elt_type($1);
			  write_exp_elt_opcode(OP_TYPE);
			  current_type = $1; } ;

/* Expressions, including the comma operator.  */
exp1	:	exp
	|	exp1 ',' exp
			{ write_exp_elt_opcode (BINOP_COMMA); }
	;

/* Expressions, not including the comma operator: */
exp	:	exp '^'   %prec UNARY
			{ write_exp_elt_opcode(UNOP_IND);
			  if (current_type)
			    current_type = TYPE_TARGET_TYPE(current_type); }
	;

exp	:	'@' exp    %prec UNARY
			{ write_exp_elt_opcode(UNOP_ADDR);
			  if (current_type)
			    current_type = TYPE_POINTER_TYPE(current_type); }
	;

exp	:	'-' exp    %prec UNARY
			{ write_exp_elt_opcode(UNOP_NEG); }
	;

exp	:	NOT exp    %prec UNARY
			{ write_exp_elt_opcode(UNOP_LOGICAL_NOT); }
	;

exp	:	INCREMENT '(' exp ')'   %prec UNARY
			{ write_exp_elt_opcode(UNOP_PREINCREMENT); }
	;

exp	:	DECREMENT  '(' exp ')'   %prec UNARY
			{ write_exp_elt_opcode(UNOP_PREDECREMENT); }
	;

exp	:	exp '.' { search_field = 1; }
		FIELDNAME
		/* name */
			{ write_exp_elt_opcode(STRUCTOP_STRUCT);
			  write_exp_string($4);
			  write_exp_elt_opcode(STRUCTOP_STRUCT);
			  search_field = 0;
			  if (current_type)
			    { while (TYPE_CODE(current_type) == TYPE_CODE_PTR)
				current_type = TYPE_TARGET_TYPE(current_type);
			      current_type = lookup_struct_elt_type (
				current_type, $4.ptr, 0); };
			 } ;
exp	:	exp '['
			/* We need to save the current_type value: */
			{ char *arrayname;
			  int arrayfieldindex;
			  arrayfieldindex =
                            is_pascal_string_type(current_type, NULL, NULL,
                                                  NULL, NULL, &arrayname);
			  if (arrayfieldindex)
			    {
			      struct stoken stringsval;
			      stringsval.ptr = (char *)alloca(strlen(arrayname) + 1UL);
			      stringsval.length = strlen(arrayname);
			      strcpy(stringsval.ptr, arrayname);
			      current_type =
                                TYPE_FIELD_TYPE(current_type,
                                                (arrayfieldindex - 1));
			      write_exp_elt_opcode(STRUCTOP_STRUCT);
			      write_exp_string(stringsval);
			      write_exp_elt_opcode(STRUCTOP_STRUCT);
			    }
			  push_current_type();  }
		exp1 ']'
			{ pop_current_type();
			  write_exp_elt_opcode(BINOP_SUBSCRIPT);
			  if (current_type) {
			    current_type = TYPE_TARGET_TYPE(current_type);
                          }; }
	;

exp	:	exp '('
			/* This is to save the value of arglist_len
			 * being accumulated by an outer function call: */
			{ push_current_type();
			  start_arglist(); }
		arglist ')'	%prec ARROW
			{ write_exp_elt_opcode(OP_FUNCALL);
			  write_exp_elt_longcst((LONGEST) end_arglist());
			  write_exp_elt_opcode(OP_FUNCALL);
			  pop_current_type(); }
	;

arglist	:
         | exp
			{ arglist_len = 1; }
	 | arglist ',' exp   %prec ABOVE_COMMA
			{ arglist_len++; }
	;

exp	:	type '(' exp ')' %prec UNARY
			{ if (current_type)
			    {
			      /* Allow automatic dereference of classes: */
			      if ((TYPE_CODE(current_type) == TYPE_CODE_PTR)
				  && (TYPE_CODE(TYPE_TARGET_TYPE(current_type)) == TYPE_CODE_CLASS)
				  && (TYPE_CODE($1) == TYPE_CODE_CLASS))
				write_exp_elt_opcode(UNOP_IND);
			    }
			  write_exp_elt_opcode(UNOP_CAST);
			  write_exp_elt_type($1);
			  write_exp_elt_opcode(UNOP_CAST);
			  current_type = $1; }
	;

exp	:	'(' exp1 ')'
			{ }
	;

/* Binary operators in order of decreasing precedence: */

exp	:	exp '*' exp
			{ write_exp_elt_opcode(BINOP_MUL); }
	;

exp	:	exp '/' exp
			{ write_exp_elt_opcode(BINOP_DIV); }
	;

exp	:	exp DIV exp
			{ write_exp_elt_opcode(BINOP_INTDIV); }
	;

exp	:	exp MOD exp
			{ write_exp_elt_opcode(BINOP_REM); }
	;

exp	:	exp '+' exp
			{ write_exp_elt_opcode(BINOP_ADD); }
	;

exp	:	exp '-' exp
			{ write_exp_elt_opcode(BINOP_SUB); }
	;

exp	:	exp LSH exp
			{ write_exp_elt_opcode(BINOP_LSH); }
	;

exp	:	exp RSH exp
			{ write_exp_elt_opcode(BINOP_RSH); }
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

exp	:	exp GEQ exp
			{ write_exp_elt_opcode(BINOP_GEQ); }
	;

exp	:	exp '<' exp
			{ write_exp_elt_opcode(BINOP_LESS); }
	;

exp	:	exp '>' exp
			{ write_exp_elt_opcode(BINOP_GTR); }
	;

exp	:	exp ANDAND exp
			{ write_exp_elt_opcode(BINOP_BITWISE_AND); }
	;

exp	:	exp XOR exp
			{ write_exp_elt_opcode(BINOP_BITWISE_XOR); }
	;

exp	:	exp OR exp
			{ write_exp_elt_opcode(BINOP_BITWISE_IOR); }
	;

exp	:	exp ASSIGN exp
			{ write_exp_elt_opcode(BINOP_ASSIGN); }
	;

exp	:	TRUEKEYWORD
			{ write_exp_elt_opcode(OP_BOOL);
			  write_exp_elt_longcst((LONGEST) $1);
			  write_exp_elt_opcode(OP_BOOL); }
	;

exp	:	FALSEKEYWORD
			{ write_exp_elt_opcode(OP_BOOL);
			  write_exp_elt_longcst((LONGEST) $1);
			  write_exp_elt_opcode(OP_BOOL); }
	;

exp	:	INT
			{ write_exp_elt_opcode(OP_LONG);
			  write_exp_elt_type($1.type);
			  write_exp_elt_longcst((LONGEST)($1.val));
			  write_exp_elt_opcode(OP_LONG); }
	;

exp	:	NAME_OR_INT
			{ YYSTYPE val;
			  parse_number($1.stoken.ptr, (int)$1.stoken.length, 0,
				       &val);
			  write_exp_elt_opcode(OP_LONG);
			  write_exp_elt_type(val.typed_val_int.type);
			  write_exp_elt_longcst((LONGEST)val.typed_val_int.val);
			  write_exp_elt_opcode(OP_LONG);
			}
	;


exp	:	FLOAT
			{ write_exp_elt_opcode(OP_DOUBLE);
			  write_exp_elt_type($1.type);
			  write_exp_elt_dblcst($1.dval);
			  write_exp_elt_opcode(OP_DOUBLE); }
	;

exp	:	variable
	;

exp	:	VARIABLE
			/* Already written by write_dollar_variable. */
	;

exp	:	SIZEOF '(' type ')'	%prec UNARY
			{ write_exp_elt_opcode(OP_LONG);
			  write_exp_elt_type(builtin_type_int);
			  CHECK_TYPEDEF($3);
			  write_exp_elt_longcst((LONGEST)TYPE_LENGTH($3));
			  write_exp_elt_opcode(OP_LONG); }
	;

exp	:	STRING
			{ /* C strings are converted into array constants with
			     an explicit null byte added at the end.  Thus
			     the array upper bound is the string length.
			     There is no such thing in C as a completely empty
			     string. */
			  char *sp = $1.ptr; size_t count = $1.length;
			  while (count-- > 0)
			    {
			      write_exp_elt_opcode(OP_LONG);
			      write_exp_elt_type(builtin_type_char);
			      write_exp_elt_longcst((LONGEST)(*sp++));
			      write_exp_elt_opcode(OP_LONG);
			    }
			  write_exp_elt_opcode(OP_LONG);
			  write_exp_elt_type(builtin_type_char);
			  write_exp_elt_longcst((LONGEST)'\0');
			  write_exp_elt_opcode(OP_LONG);
			  write_exp_elt_opcode(OP_ARRAY);
			  write_exp_elt_longcst((LONGEST)0);
			  write_exp_elt_longcst((LONGEST)($1.length));
			  write_exp_elt_opcode(OP_ARRAY); }
	;

/* Object pascal  */
exp	:	THIS
			{
			  struct value *this_val;
			  struct type *this_type;
			  write_exp_elt_opcode(OP_THIS);
			  write_exp_elt_opcode(OP_THIS);
			  /* we need type of this: */
			  this_val = value_of_this(0);
			  if (this_val)
			    this_type = value_type(this_val);
			  else
			    this_type = NULL;
			  if (this_type)
			    {
			      if (TYPE_CODE(this_type) == TYPE_CODE_PTR)
				{
				  this_type = TYPE_TARGET_TYPE(this_type);
				  write_exp_elt_opcode(UNOP_IND);
				}
			    }

			  current_type = this_type;
			}
	;

/* end of object pascal.  */

block	:	BLOCKNAME
			{
			  if ($1.sym != 0)
			      $$ = SYMBOL_BLOCK_VALUE($1.sym);
			  else
			    {
			      struct symtab *tem =
				  lookup_symtab(copy_name($1.stoken));
			      if (tem)
				$$ = BLOCKVECTOR_BLOCK(BLOCKVECTOR(tem), STATIC_BLOCK);
			      else
				error("No file or function \"%s\".",
                                      copy_name($1.stoken));
			    }
			}
	;

block	:	block COLONCOLON name
			{ struct symbol *tem
			    = lookup_symbol(copy_name ($3), $1,
					    VAR_DOMAIN, (int *)NULL,
					    (struct symtab **)NULL);
			  if (!tem || (SYMBOL_CLASS(tem) != LOC_BLOCK))
			    error("No function \"%s\" in specified context.",
                                  copy_name($3));
			  $$ = SYMBOL_BLOCK_VALUE(tem); }
	;

variable:	block COLONCOLON name
			{ struct symbol *sym;
			  sym = lookup_symbol(copy_name($3), $1,
					      VAR_DOMAIN, (int *)NULL,
					      (struct symtab **)NULL);
			  if (sym == 0)
			    error("No symbol \"%s\" in specified context.",
                                  copy_name($3));

			  write_exp_elt_opcode(OP_VAR_VALUE);
			  /* block_found is set by lookup_symbol.  */
			  write_exp_elt_block(block_found);
			  write_exp_elt_sym(sym);
			  write_exp_elt_opcode(OP_VAR_VALUE); }
	;

qualified_name:	typebase COLONCOLON name
			{
			  struct type *type = $1;
			  if ((TYPE_CODE(type) != TYPE_CODE_STRUCT)
			      && (TYPE_CODE(type) != TYPE_CODE_UNION))
			    error("`%s' is not defined as an aggregate type.",
                                  TYPE_NAME(type));

			  write_exp_elt_opcode(OP_SCOPE);
			  write_exp_elt_type(type);
			  write_exp_string($3);
			  write_exp_elt_opcode(OP_SCOPE);
			}
	;

variable:	qualified_name
	|	COLONCOLON name
			{
			  char *name = copy_name($2);
			  struct symbol *sym;
			  struct minimal_symbol *msymbol;

			  sym =
			    lookup_symbol(name, (const struct block *)NULL,
					  VAR_DOMAIN, (int *)NULL,
					  (struct symtab **)NULL);
			  if (sym)
			    {
			      write_exp_elt_opcode(OP_VAR_VALUE);
			      write_exp_elt_block(NULL);
			      write_exp_elt_sym(sym);
			      write_exp_elt_opcode(OP_VAR_VALUE);
			      break;
			    }

			  msymbol = lookup_minimal_symbol(name, NULL, NULL);
			  if (msymbol != NULL)
			    {
			      write_exp_msymbol(msymbol,
                                                lookup_function_type(builtin_type_int),
                                                builtin_type_int);
			    }
			  else
			    if (!have_full_symbols() && !have_partial_symbols())
			      error("No symbol table is loaded.  Use the \"file\" command.");
			    else
			      error("No symbol \"%s\" in current context.", name);
			}
	;

variable:	name_not_typename
			{ struct symbol *sym = $1.sym;

			  if (sym)
			    {
			      if (symbol_read_needs_frame(sym))
				{
				  if ((innermost_block == 0) ||
				      contained_in(block_found,
                                                   innermost_block))
				    innermost_block = (struct block *)block_found;
				}

			      write_exp_elt_opcode(OP_VAR_VALUE);
			      /* We want to use the selected frame, not
				 another more inner frame which happens to
				 be in the same block.  */
			      write_exp_elt_block(NULL);
			      write_exp_elt_sym(sym);
			      write_exp_elt_opcode(OP_VAR_VALUE);
			      current_type = sym->type; }
			  else if ($1.is_a_field_of_this)
			    {
			      struct value * this_val;
			      struct type * this_type;
			      /* Object pascal: it hangs off of `this'.  Must
			         not inadvertently convert from a method call
				 to data ref.  */
			      if ((innermost_block == 0) ||
				  contained_in(block_found, innermost_block))
				innermost_block = (struct block *)block_found;
			      write_exp_elt_opcode(OP_THIS);
			      write_exp_elt_opcode(OP_THIS);
			      write_exp_elt_opcode(STRUCTOP_PTR);
			      write_exp_string($1.stoken);
			      write_exp_elt_opcode(STRUCTOP_PTR);
			      /* we need type of this: */
			      this_val = value_of_this(0);
			      if (this_val)
				this_type = value_type(this_val);
			      else
				this_type = NULL;
			      if (this_type)
				current_type = lookup_struct_elt_type (
				  this_type,
				  copy_name($1.stoken), 0);
			      else
				current_type = NULL;
			    }
			  else
			    {
			      struct minimal_symbol *msymbol;
			      char *arg = copy_name($1.stoken);

			      msymbol =
				lookup_minimal_symbol(arg, NULL, NULL);
			      if (msymbol != NULL)
				{
				  write_exp_msymbol(msymbol,
						    lookup_function_type(builtin_type_int),
						    builtin_type_int);
				}
			      else if (!have_full_symbols() && !have_partial_symbols())
				error("No symbol table is loaded.  Use the \"file\" command.");
			      else
				error("No symbol \"%s\" in current context.",
				      copy_name($1.stoken));
			    }
			}
	;


ptype	:	typebase
	;

/* We used to try to recognize more pointer to member types here, but
   that didn't work (shift/reduce conflicts meant that these rules never
   got executed).  The problem is that
     int (foo::bar::baz::bizzle)
   is a function type but
     int (foo::bar::baz::bizzle::*)
   is a pointer to member type.  Stroustrup loses again!  */

type	:	ptype
	|	typebase COLONCOLON '*'
			{ $$ = lookup_member_type(builtin_type_int, $1); }
	;

typebase  /* Implements (approximately): (type-qualifier)* type-specifier */
	:	'^' typebase
			{ $$ = lookup_pointer_type($2); }
	|	TYPENAME
			{ $$ = $1.type; }
	|	STRUCT name
			{ $$ = lookup_struct(copy_name($2),
					     expression_context_block); }
	|	CLASS name
			{ $$ = lookup_struct(copy_name($2),
					     expression_context_block); }
	/* "const" and "volatile" are curently ignored.  A type qualifier
	   after the type is handled in the ptype rule.  I think these could
	   be too.  */
	;

name	:	NAME { $$ = $1.stoken; }
	|	BLOCKNAME { $$ = $1.stoken; }
	|	TYPENAME { $$ = $1.stoken; }
	|	NAME_OR_INT  { $$ = $1.stoken; }
	;

name_not_typename :	NAME
	|	BLOCKNAME
/* These would be useful if name_not_typename was useful, but it is just
   a fake for "variable", so these cause reduce/reduce conflicts because
   the parser can't tell whether NAME_OR_INT is a name_not_typename (=variable,
   =exp) or just an exp.  If name_not_typename was ever used in an lvalue
   context where only a name could occur, this might be useful.
  	|	NAME_OR_INT
 */
	;

%%

/* Take care of parsing a number (anything that starts with a digit).
   Set yylval and return the token type; update lexptr.
   LEN is the number of characters in it.  */

/*** Needs some error checking for the float case ***/

static int
parse_number(char *p, int len, int parsed_float, YYSTYPE *putithere)
{
  /* FIXME: Should these not be unsigned?  We do NOT deal with negative
   * values here, and we do kind of silly things like cast to unsigned: */
  LONGEST n = 0;
  LONGEST prevn = 0;
  ULONGEST un;

  int i = 0;
  int i_c;
  int base = input_radix;
  int unsigned_p = 0;

  /* Number of "L" suffixes encountered.  */
  int long_p = 0;

  /* We have found a "L" or "U" suffix.  */
  int found_suffix = 0;

  ULONGEST high_bit;
  struct type *signed_type;
  struct type *unsigned_type;

  if (parsed_float)
    {
      /* It is a float since it contains a point or an exponent.  */
      char c_c;
      int num = 0;	/* number of tokens scanned by scanf */
      char saved_char = p[len];

      p[len] = 0;	/* null-terminate the token */
      if (sizeof(putithere->typed_val_float.dval) <= sizeof(float))
	num = sscanf(p, "%g%c", (float *)&putithere->typed_val_float.dval,
		     &c_c);
      else if (sizeof(putithere->typed_val_float.dval) <= sizeof(double))
	num = sscanf(p, "%lg%c", (double *)&putithere->typed_val_float.dval,
		     &c_c);
      else
	{
#ifdef SCANF_HAS_LONG_DOUBLE
	  num = sscanf(p, "%Lg%c", &putithere->typed_val_float.dval, &c_c);
#else
	  /* Scan it into a double, then assign it to the long double.
	     This at least wins with values representable in the range
	     of doubles. */
	  double temp;
	  num = sscanf(p, "%lg%c", &temp, &c_c);
	  putithere->typed_val_float.dval = temp;
#endif /* SCANF_HAS_LONG_DOUBLE */
	}
      p[len] = saved_char;	/* restore the input stream */
      if (num != 1) 		/* check scanf found ONLY a float ... */
	return ERROR;
      /* See if it has `f' or `l' suffix (float or long double).  */

      c_c = (char)tolower(p[len - 1]);

      if (c_c == 'f')
	putithere->typed_val_float.type = builtin_type_float;
      else if (c_c == 'l')
	putithere->typed_val_float.type = builtin_type_long_double;
      else if (isdigit(c_c) || (c_c == '.'))
	putithere->typed_val_float.type = builtin_type_double;
      else
	return ERROR;

      return FLOAT;
    }

  /* Handle base-switching prefixes 0x, 0t, 0d, 0 */
  if (p[0] == '0')
    switch (p[1])
      {
      case 'x':
      case 'X':
	if (len >= 3)
	  {
	    p += 2;
	    base = 16;
	    len -= 2;
	  }
	break;

      case 't':
      case 'T':
      case 'd':
      case 'D':
	if (len >= 3)
	  {
	    p += 2;
	    base = 10;
	    len -= 2;
	  }
	break;

      default:
	base = 8;
	break;
      }

  while (len-- > 0)
    {
      i_c = *p++;
      if ((i_c >= 'A') && (i_c <= 'Z'))
	i_c += ('a' - 'A');
      if ((i_c != 'l') && (i_c != 'u'))
	n *= base;
      if ((i_c >= '0') && (i_c <= '9'))
	{
	  if (found_suffix)
	    return ERROR;
	  n += i = (i_c - '0');
	}
      else
	{
	  if ((base > 10) && (i_c >= 'a') && (i_c <= 'f'))
	    {
	      if (found_suffix)
		return ERROR;
	      n += i = (i_c - 'a' + 10);
	    }
	  else if (i_c == 'l')
	    {
	      ++long_p;
	      found_suffix = 1;
	    }
	  else if (i_c == 'u')
	    {
	      unsigned_p = 1;
	      found_suffix = 1;
	    }
	  else
	    return ERROR;	/* Char not a digit */
	}
      if (i >= base)
	return ERROR;		/* Invalid digit in this base */

      /* Portably test for overflow (only works for nonzero values, so make
	 a second check for zero).  FIXME: Can we not just make n and prevn
	 unsigned and avoid this?  */
      if ((i_c != 'l') && (i_c != 'u') && (prevn >= n) && (n != 0))
	unsigned_p = 1;		/* Try something unsigned */

      /* Portably test for unsigned overflow.
	 FIXME: This check is wrong; for example it does NOT find overflow
	 on 0x123456789 when LONGEST is 32 bits.  */
      if ((i_c != 'l') && (i_c != 'u') && (n != 0))
	{
	  if (unsigned_p && ((ULONGEST)prevn >= (ULONGEST)n))
	    error("Numeric constant too large.");
	}
      prevn = n;
    }

  /* An integer constant is an int, a long, or a long long.  An L
     suffix forces it to be long; an LL suffix forces it to be long
     long.  If not forced to a larger size, it gets the first type of
     the above that it fits in.  To figure out whether it fits, we
     shift it right and see whether anything remains.  Note that we
     can't shift sizeof (LONGEST) * HOST_CHAR_BIT bits or more in one
     operation, because many compilers will warn about such a shift
     (which always produces a zero result).  Sometimes TARGET_INT_BIT
     or TARGET_LONG_BIT will be that big, sometimes not.  To deal with
     the case where it is we just always shift the value more than
     once, with fewer bits each time.  */

  un = (ULONGEST)n >> 2;
  if ((long_p == 0)
      && ((un >> (TARGET_INT_BIT - 2)) == 0))
    {
      high_bit = ((ULONGEST)1UL) << (TARGET_INT_BIT - 1);

      /* A large decimal (not hex or octal) constant (between INT_MAX
	 and UINT_MAX) is a long or unsigned long, according to ANSI,
	 never an unsigned int, but this code treats it as unsigned
	 int.  This probably should be fixed.  GCC gives a warning on
	 such constants.  */

      unsigned_type = builtin_type_unsigned_int;
      signed_type = builtin_type_int;
    }
  else if ((long_p <= 1)
	   && ((un >> (TARGET_LONG_BIT - 2)) == 0))
    {
      high_bit = ((ULONGEST)1UL) << (TARGET_LONG_BIT - 1);
      unsigned_type = builtin_type_unsigned_long;
      signed_type = builtin_type_long;
    }
  else
    {
      int shift;
      if ((sizeof(ULONGEST) * HOST_CHAR_BIT) < (size_t)TARGET_LONG_LONG_BIT)
	/* A long long does not fit in a LONGEST: */
	shift = (sizeof(ULONGEST) * HOST_CHAR_BIT - 1);
      else
	shift = (TARGET_LONG_LONG_BIT - 1);
      high_bit = ((ULONGEST)1UL << shift);
      unsigned_type = builtin_type_unsigned_long_long;
      signed_type = builtin_type_long_long;
    }

   putithere->typed_val_int.val = n;

   /* If the high bit of the worked out type is set then this number
      has to be unsigned. */

   if (unsigned_p || (n & high_bit))
     {
       putithere->typed_val_int.type = unsigned_type;
     }
   else
     {
       putithere->typed_val_int.type = signed_type;
     }

   return INT;
}


struct type_push
{
  struct type *stored;
  struct type_push *next;
};

static struct type_push *tp_top = NULL;

static void
push_current_type (void)
{
  struct type_push *tpnew;
  tpnew = (struct type_push *)malloc(sizeof(struct type_push));
  tpnew->next = tp_top;
  tpnew->stored = current_type;
  current_type = NULL;
  tp_top = tpnew;
}

static void
pop_current_type(void)
{
  struct type_push *tp = tp_top;
  if (tp)
    {
      current_type = tp->stored;
      tp_top = tp->next;
      xfree (tp);
    }
}

struct token
{
  const char *poperator;
  int token;
  enum exp_opcode opcode;
};

static const struct token tokentab3[] =
  {
    { "shr", RSH, BINOP_END },
    { "shl", LSH, BINOP_END },
    { "and", ANDAND, BINOP_END },
    { "div", DIV, BINOP_END },
    { "not", NOT, BINOP_END },
    { "mod", MOD, BINOP_END },
    { "inc", INCREMENT, BINOP_END },
    { "dec", DECREMENT, BINOP_END },
    { "xor", XOR, BINOP_END }
  };

static const struct token tokentab2[] =
  {
    { "or", OR, BINOP_END },
    { "<>", NOTEQUAL, BINOP_END },
    { "<=", LEQ, BINOP_END },
    { ">=", GEQ, BINOP_END },
    { ":=", ASSIGN, BINOP_END },
    { "::", COLONCOLON, BINOP_END }
  };

/* Allocate uppercased var */
/* make an uppercased copy of tokstart */
static char *uptok(char *tokstart, int namelen)
{
  int i;
  char *uptokstart = (char *)malloc(namelen + 1UL);
  for (i = 0; (i <= namelen) && (i < INT_MAX); i++)
    {
      if ((tokstart[i] >= 'a') && (tokstart[i] <= 'z'))
        uptokstart[i] = (tokstart[i] - ('a' - 'A'));
      else
        uptokstart[i] = tokstart[i];
    }
  uptokstart[namelen] = '\0';
  return uptokstart;
}

/* Read one token, getting characters through lexptr: */
static int
yylex(void)
{
  int c;
  size_t namelen = 0UL;
  unsigned int u_i;
  char *tokstart;
  char *uptokstart;
  const char *tokptr;
  size_t explen;
  int tempbufindex;
  static char *tempbuf;
  static int tempbufsize;

 retry:

  prev_lexptr = lexptr;

  tokstart = (char *)lexptr;
  explen = strlen(lexptr);
  /* See if it is a special token of length 3.  */
  if (explen > 2)
    for (u_i = 0UL; u_i < (sizeof(tokentab3) / sizeof(tokentab3[0])); u_i++)
      if ((strncasecmp(tokstart, tokentab3[u_i].poperator, 3) == 0)
          && (!isalpha(tokentab3[u_i].poperator[0]) || (explen == 3)
              || (!isalpha(tokstart[3]) && !isdigit(tokstart[3]) && (tokstart[3] != '_'))))
        {
          lexptr += 3;
          yylval.opcode = tokentab3[u_i].opcode;
          return tokentab3[u_i].token;
        }

  /* See if it is a special token of length 2: */
  if (explen > 1)
    for (u_i = 0UL; u_i < (sizeof(tokentab2) / sizeof(tokentab2[0])); u_i++)
      if ((strncasecmp(tokstart, tokentab2[u_i].poperator, 2) == 0)
          && (!isalpha(tokentab2[u_i].poperator[0]) || (explen == 2)
              || (!isalpha(tokstart[2]) && !isdigit(tokstart[2]) && (tokstart[2] != '_'))))
        {
          lexptr += 2;
          yylval.opcode = tokentab2[u_i].opcode;
          return tokentab2[u_i].token;
        }

  switch (c = *tokstart)
    {
    case 0:
      return 0;

    case ' ':
    case '\t':
    case '\n':
      lexptr++;
      goto retry;

    case '\'':
      /* We either have a character constant ('0' or '\177' for example)
	 or we have a quoted symbol reference ('foo(int,int)' in object pascal
	 for example). */
      lexptr++;
      c = *lexptr++;
      if (c == '\\')
	c = parse_escape((const char **)&lexptr);
      else if (c == '\'')
	error("Empty character constant.");

      yylval.typed_val_int.val = c;
      yylval.typed_val_int.type = builtin_type_char;

      c = *lexptr++;
      if (c != '\'')
	{
	  namelen = (skip_quoted(tokstart) - tokstart);
	  if (namelen > 2)
	    {
	      lexptr = (tokstart + namelen);
	      if (lexptr[-1] != '\'')
		error(_("Unmatched single quote."));
	      namelen -= 2;
              tokstart++;
              uptokstart = uptok(tokstart, (int)namelen);
	      goto tryname;
	    }
	  error(_("Invalid character constant."));
	}
      return INT;

    case '(':
      paren_depth++;
      lexptr++;
      return c;

    case ')':
      if (paren_depth == 0)
	return 0;
      paren_depth--;
      lexptr++;
      return c;

    case ',':
      if (comma_terminates && paren_depth == 0)
	return 0;
      lexptr++;
      return c;

    case '.':
      /* Might be a floating point number: */
      if ((lexptr[1] < '0') || (lexptr[1] > '9'))
	goto symbol;		/* Nope, must be a symbol. */
      /* FALL THRU into number case: */
      ATTRIBUTE_FALLTHROUGH;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      {
	/* It is a number: */
	int got_dot = 0, got_e = 0, toktype;
	char *p = tokstart;
	int hex = input_radix > 10;

	if ((c == '0') && ((p[1] == 'x') || (p[1] == 'X')))
	  {
	    p += 2;
	    hex = 1;
	  }
	else if ((c == '0')
                 && ((p[1] == 't') || (p[1] == 'T') || (p[1] == 'd')
                     || (p[1] == 'D')))
	  {
	    p += 2;
	    hex = 0;
	  }

	for (;; ++p)
	  {
	    /* This test includes !hex because 'e' is a valid hex digit
	       and thus does not indicate a floating point number when
	       the radix is hex.  */
	    if (!hex && !got_e && ((*p == 'e') || (*p == 'E')))
	      got_dot = got_e = 1;
	    /* This test does not include !hex, because a '.' always indicates
	       a decimal floating point number regardless of the radix.  */
	    else if (!got_dot && (*p == '.'))
	      got_dot = 1;
	    else if (got_e && ((p[-1] == 'e') || (p[-1] == 'E'))
		     && ((*p == '-') || (*p == '+')))
	      /* This is the sign of the exponent, not the end of the
		 number.  */
	      continue;
	    /* We will take any letters or digits.  parse_number will
	       complain if past the radix, or if L or U are not final.  */
	    else if (((*p < '0') || (*p > '9'))
		     && (((*p < 'a') || (*p > 'z'))
                         && ((*p < 'A') || (*p > 'Z'))))
	      break;
	  }
	toktype = parse_number(tokstart, (int)(p - tokstart), (got_dot | got_e),
                               &yylval);
        if (toktype == ERROR)
	  {
	    char *err_copy = (char *)alloca(p - tokstart + 1);

	    memcpy(err_copy, tokstart, (p - tokstart));
	    err_copy[p - tokstart] = 0;
	    error("Invalid number \"%s\".", err_copy);
	  }
	lexptr = p;
	return toktype;
      }

    case '+':
    case '-':
    case '*':
    case '/':
    case '|':
    case '&':
    case '^':
    case '~':
    case '!':
    case '@':
    case '<':
    case '>':
    case '[':
    case ']':
    case '?':
    case ':':
    case '=':
    case '{':
    case '}':
    symbol:
      lexptr++;
      return c;

    case '"':

      /* Build the gdb internal form of the input string in tempbuf,
	 translating any standard C escape forms seen.  Note that the
	 buffer is null byte terminated *only* for the convenience of
	 debugging gdb itself and printing the buffer contents when
	 the buffer contains no embedded nulls.  Gdb does not depend
	 upon the buffer being null byte terminated, it uses the length
	 string instead.  This allows gdb to handle C strings (as well
	 as strings in other languages) with embedded null bytes */

      tokptr = ++tokstart;
      tempbufindex = 0;

      do {
	/* Grow the static temp buffer if necessary, including allocating
	   the first one on demand. */
	if ((tempbufindex + 1) >= tempbufsize)
	  {
	    tempbuf = (char *)realloc(tempbuf, tempbufsize += 64);
	  }

	switch (*tokptr)
	  {
	  case '\0':
	  case '"':
	    /* Do nothing, loop will terminate. */
	    break;
	  case '\\':
	    tokptr++;
	    c = parse_escape(&tokptr);
	    if (c == -1)
	      {
		continue;
	      }
	    tempbuf[tempbufindex++] = (char)c;
	    break;
	  default:
	    tempbuf[tempbufindex++] = *tokptr++;
	    break;
	  }
      } while ((*tokptr != '"') && (*tokptr != '\0'));
      if (*tokptr++ != '"')
	{
	  error("Unterminated string in expression.");
	}
      tempbuf[tempbufindex] = '\0';	/* See note above */
      yylval.sval.ptr = tempbuf;
      yylval.sval.length = tempbufindex;
      lexptr = (char *)tokptr;
      return (STRING);

    default:;
    } /* end switch */

  if (!((c == '_') || (c == '$')
	|| ((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z'))))
    /* We must have come across a bad character (e.g. ';').  */
    error("Invalid character '%c' in expression.", c);

  /* It is a name.  See how long it is: */
  namelen = 0;
  for (c = tokstart[namelen];
       ((c == '_') || (c == '$') || ((c >= '0') && (c <= '9'))
	|| ((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z')) || (c == '<'));)
    {
      /* Template parameter lists are part of the name.
	 FIXME: This mishandles `print $a<4&&$a>3'.  */
      if (c == '<')
	{
	  size_t sz_i = namelen;
	  int nesting_level = 1;
	  while (tokstart[++sz_i])
	    {
	      if (tokstart[sz_i] == '<')
		nesting_level++;
	      else if (tokstart[sz_i] == '>')
		{
		  if (--nesting_level == 0)
		    break;
		}
	    }
	  if (tokstart[sz_i] == '>')
	    namelen = sz_i;
	  else
	    break;
	}

      /* do NOT uppercase internals because of registers !!! */
      c = tokstart[++namelen];
    }

  uptokstart = uptok(tokstart, (int)namelen);

  /* The token "if" terminates the expression and is NOT
     removed from the input stream.  */
  if ((namelen == 2) && (uptokstart[0] == 'I') && (uptokstart[1] == 'F'))
    {
      return 0;
    }

  lexptr += namelen;

  tryname:

  /* Catch specific keywords.  Should be done with a data structure: */
  switch (namelen)
    {
    case 6:
      if (DEPRECATED_STREQ(uptokstart, "OBJECT"))
	return CLASS;
      if (DEPRECATED_STREQ(uptokstart, "RECORD"))
	return STRUCT;
      if (DEPRECATED_STREQ(uptokstart, "SIZEOF"))
	return SIZEOF;
      break;
    case 5:
      if (DEPRECATED_STREQ(uptokstart, "CLASS"))
	return CLASS;
      if (DEPRECATED_STREQ(uptokstart, "FALSE"))
	{
          yylval.lval = 0;
          return FALSEKEYWORD;
        }
      break;
    case 4:
      if (DEPRECATED_STREQ(uptokstart, "TRUE"))
	{
          yylval.lval = 1;
  	  return TRUEKEYWORD;
        }
      if (DEPRECATED_STREQ(uptokstart, "SELF"))
        {
          /* here we search for 'this' like
             inserted in FPC stabs debug info */
	  static const char this_name[] = "this";

	  if (lookup_symbol(this_name, expression_context_block,
			    VAR_DOMAIN, (int *)NULL,
			    (struct symtab **)NULL))
	    return THIS;
	}
      break;
    default:
      break;
    }

  yylval.sval.ptr = tokstart;
  yylval.sval.length = namelen;

  if (*tokstart == '$')
    {
      /* $ is the normal prefix for pascal hexadecimal values
        but this conflicts with the GDB use for debugger variables
        so in expression to enter hexadecimal values
        we still need to use C syntax with 0xff  */
      write_dollar_variable(yylval.sval);
      return VARIABLE;
    }

  /* Use token-type BLOCKNAME for symbols that happen to be defined as
     functions or symtabs.  If this is not so, then ...
     Use token-type TYPENAME for symbols that happen to be defined
     currently as names of types; NAME for other symbols.
     The caller is not constrained to care about the distinction.  */
  {
    char *tmp = copy_name(yylval.sval);
    struct symbol *sym;
    int is_a_field_of_this = 0;
    int is_a_field = 0;
    int hextype;


    if (search_field && current_type)
      is_a_field = (lookup_struct_elt_type(current_type, tmp, 1) != NULL);
    if (is_a_field)
      sym = NULL;
    else
      sym = lookup_symbol(tmp, expression_context_block,
			  VAR_DOMAIN,
			  &is_a_field_of_this,
			  (struct symtab **)NULL);
    /* second chance uppercased (as Free Pascal does): */
    if (!sym && !is_a_field_of_this && !is_a_field)
      {
       for (u_i = 0U; (u_i <= namelen) && (u_i < UINT_MAX); u_i++)
         {
           if ((tmp[u_i] >= 'a') && (tmp[u_i] <= 'z'))
             tmp[u_i] -= ('a' - 'A');
         }
       if (search_field && current_type)
	 is_a_field = (lookup_struct_elt_type(current_type, tmp, 1) != NULL);
       if (is_a_field)
	 sym = NULL;
       else
	 sym = lookup_symbol(tmp, expression_context_block,
                             VAR_DOMAIN, &is_a_field_of_this,
                             (struct symtab **)NULL);
       if (sym || is_a_field_of_this || is_a_field)
         for (u_i = 0U; (u_i <= namelen) && (u_i < UINT_MAX); u_i++)
           {
             if ((tokstart[u_i] >= 'a') && (tokstart[u_i] <= 'z'))
               tokstart[u_i] -= ('a' - 'A');
           }
      }
    /* Third chance Capitalized (as GPC does): */
    if (!sym && !is_a_field_of_this && !is_a_field)
      {
       for (u_i = 0U; (u_i <= namelen) && (u_i < UINT_MAX); u_i++)
         {
           if (u_i == 0U)
             {
              if ((tmp[u_i] >= 'a') && (tmp[u_i] <= 'z'))
                tmp[u_i] -= ('a' - 'A');
             }
           else
             {
              if ((tmp[u_i] >= 'A') && (tmp[u_i] <= 'Z'))
                tmp[u_i] -= ('A' - 'a');
             }
          }
       if (search_field && current_type)
	 is_a_field = (lookup_struct_elt_type(current_type, tmp, 1) != NULL);
       if (is_a_field)
	 sym = NULL;
       else
	 sym = lookup_symbol(tmp, expression_context_block,
                             VAR_DOMAIN, &is_a_field_of_this,
                             (struct symtab **)NULL);
       if (sym || is_a_field_of_this || is_a_field)
          for (u_i = 0U; (u_i <= namelen) && (u_i < UINT_MAX); u_i++)
            {
              if (u_i == 0U)
                {
                  if ((tokstart[u_i] >= 'a') && (tokstart[u_i] <= 'z'))
                    tokstart[u_i] -= ('a' - 'A');
                }
              else
                {
                  if ((tokstart[u_i] >= 'A') && (tokstart[u_i] <= 'Z'))
                    tokstart[u_i] -= ('A' - 'a');
                }
            }
      }

    if (is_a_field)
      {
	tempbuf = (char *)realloc(tempbuf, (namelen + 1UL));
	strncpy(tempbuf, tokstart, namelen); tempbuf[namelen] = 0;
	yylval.sval.ptr = tempbuf;
	yylval.sval.length = namelen;
	return FIELDNAME;
      }
    /* Call lookup_symtab, not lookup_partial_symtab, in case there are
       no psymtabs (coff, xcoff, or some future change to blow away the
       psymtabs once once symbols are read).  */
    if ((sym && (SYMBOL_CLASS(sym) == LOC_BLOCK)) ||
        lookup_symtab(tmp))
      {
	yylval.ssym.sym = sym;
	yylval.ssym.is_a_field_of_this = is_a_field_of_this;
	return BLOCKNAME;
      }
    if (sym && (SYMBOL_CLASS(sym) == LOC_TYPEDEF))
        {
#if 1
	  /* Despite the following flaw, we need to keep this code enabled.
	     Because we can get called from check_stub_method, if we do NOT
	     handle nested types then it screws many operations in any
	     program which uses nested types.  */
	  /* In "A::x", if x is a member function of A and there happens
	     to be a type (nested or not, since the stabs do NOT make that
	     distinction) named x, then this code incorrectly thinks we
	     are dealing w/nested types rather than a member function.  */

	  const char *p;
	  const char *namestart;
	  struct symbol *best_sym;

	  /* Look ahead to detect nested types.  This probably should be
	     done in the grammar, but trying seemed to introduce a lot
	     of shift/reduce and reduce/reduce conflicts.  It is possible
	     that it could be done, though.  Or perhaps a non-grammar, but
	     less ad hoc, approach would work well.  */

	  /* Since we do not currently have any way of distinguishing
	     a nested type from a non-nested one (the stabs do NOT tell
	     us whether a type is nested), we just ignore the
	     containing type.  */

	  p = lexptr;
	  best_sym = sym;
	  while (1)
	    {
	      /* Skip whitespace: */
	      while ((*p == ' ') || (*p == '\t') || (*p == '\n'))
		++p;
	      if ((*p == ':') && (p[1] == ':'))
		{
		  /* Skip the `::'.  */
		  p += 2;
		  /* Skip whitespace: */
		  while ((*p == ' ') || (*p == '\t') || (*p == '\n'))
		    ++p;
		  namestart = p;
		  while ((*p == '_') || (*p == '$') || ((*p >= '0') && (*p <= '9'))
			 || ((*p >= 'a') && (*p <= 'z'))
			 || ((*p >= 'A') && (*p <= 'Z')))
		    ++p;
		  if (p != namestart)
		    {
		      struct symbol *cur_sym;
		      /* As big as the whole rest of the expression, which
                       * is at least big enough: */
		      char *ncopy = (char *)alloca(strlen(tmp) + strlen(namestart) + 3UL);
		      char *tmp1;

		      tmp1 = ncopy;
		      memcpy(tmp1, tmp, strlen(tmp));
		      tmp1 += strlen(tmp);
		      memcpy(tmp1, "::", 2);
		      tmp1 += 2;
		      memcpy(tmp1, namestart, p - namestart);
		      tmp1[p - namestart] = '\0';
		      cur_sym = lookup_symbol(ncopy, expression_context_block,
                                              VAR_DOMAIN, (int *)NULL,
                                              (struct symtab **)NULL);
		      if (cur_sym)
			{
			  if (SYMBOL_CLASS(cur_sym) == LOC_TYPEDEF)
			    {
			      best_sym = cur_sym;
			      lexptr = p;
			    }
			  else
			    break;
			}
		      else
			break;
		    }
		  else
		    break;
		}
	      else
		break;
	    }

	  yylval.tsym.type = SYMBOL_TYPE(best_sym);
#else /* not 0 */
	  yylval.tsym.type = SYMBOL_TYPE(sym);
#endif /* not 0 */
	  return TYPENAME;
        }
    yylval.tsym.type
      = language_lookup_primitive_type_by_name(current_language,
                                               current_gdbarch, tmp);
    if (yylval.tsym.type != NULL)
      return TYPENAME;

    /* Input names that are NOT symbols but ARE valid hex numbers,
       when the input radix permits them, can be names or numbers
       depending on the parse.  Note we support radixes > 16 here.  */
    if (!sym &&
        (((tokstart[0] >= 'a') && (tokstart[0] < (char)('a' + input_radix - 10)))
         || ((tokstart[0] >= 'A') && (tokstart[0] < (char)('A' + input_radix - 10)))))
      {
 	YYSTYPE newlval;	/* Its value is ignored.  */
	hextype = parse_number(tokstart, (int)namelen, 0, &newlval);
	if (hextype == INT)
	  {
	    yylval.ssym.sym = sym;
	    yylval.ssym.is_a_field_of_this = is_a_field_of_this;
	    return NAME_OR_INT;
	  }
      }

    free(uptokstart);
    /* Any other kind of symbol: */
    yylval.ssym.sym = sym;
    yylval.ssym.is_a_field_of_this = is_a_field_of_this;
    return NAME;
  }
}

void ATTR_NORETURN
yyerror(const char *msg)
{
  if (prev_lexptr)
    lexptr = prev_lexptr;

  error("A %s in expression, near `%s'.", (msg ? msg : "error"),
  	(lexptr ? lexptr : "lexptr"));
}

/* End of p-exp.y */
