/* objc-exp.y: YACC parser for C expressions, for GDB.

   Copyright 1986, 1989, 1990, 1991, 1993, 1994, 2002 Free Software
   Foundation, Inc.

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

/* Parse a C expression from text in a string, and return the result
   as a struct expression pointer.  That structure contains arithmetic
   operations in reverse polish, with constants represented by
   operations that are followed by special data.  See expression.h for
   the details of the format.  What is important here is that it can
   be built up sequentially during the process of parsing; the lower
   levels of the tree always come first in the result.

   Note that malloc's, realloc's, and free's in this file are transformed to
   xmalloc, xrealloc, and xfree respectively by the same sed command in the
   makefile that remaps any other malloc/realloc/free inserted by the
   parser generator.  Doing this with #defines and trying to control
   the interaction with include files (<malloc.h> and <stdlib.h> for
   example) just became too messy, particularly when such includes can
   be inserted at random times by the parser generator.  */

%{

#include "defs.h"
#include "gdb_string.h"
#include <ctype.h>
#include "expression.h"

#include "objc-lang.h"	/* For objc language constructs.  */

#include "value.h"
#include "parser-defs.h"
#include "language.h"
#include "c-lang.h"
#include "bfd.h" /* Required by objfiles.h.  */
#include "symfile.h" /* Required by objfiles.h.  */
#include "objfiles.h" /* For have_full_symbols and have_partial_symbols. */
#include "top.h"
#include "completer.h" /* For skip_quoted().  */
#include "block.h"

/* Remap normal yacc parser interface names (yyparse, yylex, yyerror,
   etc), as well as gratuitiously global symbol names, so we can have
   multiple yacc generated parsers in gdb.  Note that these are only
   the variables produced by yacc.  If other parser generators (bison,
   byacc, etc) produce additional global names that conflict at link
   time, then those parser generators need to be fixed instead of
   adding those names to this list.  */

#define	yymaxdepth	objc_maxdepth
#define	yyparse		objc_parse
#define	yylex		objc_lex
#define	yyerror		objc_error
#define	yylval		objc_lval
#define	yychar		objc_char
#define	yydebug		objc_debug
#define	yypact		objc_pact
#define	yyr1		objc_r1
#define	yyr2		objc_r2
#define	yydef		objc_def
#define	yychk		objc_chk
#define	yypgo		objc_pgo
#define	yyact		objc_act
#define	yyexca		objc_exca
#define yyerrflag	objc_errflag
#define yynerrs		objc_nerrs
#define	yyps		objc_ps
#define	yypv		objc_pv
#define	yys		objc_s
#define	yy_yys		objc_yys
#define	yystate		objc_state
#define	yytmp		objc_tmp
#define	yyv		objc_v
#define	yy_yyv		objc_yyv
#define	yyval		objc_val
#define	yylloc		objc_lloc
#define yyreds		objc_reds		/* With YYDEBUG defined */
#define yytoks		objc_toks		/* With YYDEBUG defined */
#define yyname  	objc_name          	/* With YYDEBUG defined */
#define yyrule  	objc_rule          	/* With YYDEBUG defined */
#define yylhs		objc_yylhs
#define yylen		objc_yylen
#define yydefred	objc_yydefred
#define yydgoto		objc_yydgoto
#define yysindex	objc_yysindex
#define yyrindex	objc_yyrindex
#define yygindex	objc_yygindex
#define yytable		objc_yytable
#define yycheck		objc_yycheck

#ifndef YYDEBUG
# define YYDEBUG 0		/* Default to no yydebug support.  */
#endif /* !YYDEBUG */

/* APPLE LOCAL - Avoid calling lookup_objc_class unnecessarily.  */
static int square_bracket_seen = 0;

int
yyparse PARAMS((void));

static int
yylex PARAMS((void));

void
yyerror PARAMS((const char *));

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
    struct objc_class_str oclass; /* I hope renaming this was okay? */

    struct type **tvec;
    int *ivec;
  }

%{
/* YYSTYPE gets defined by %union.  */
static int
parse_number PARAMS((char *, int, int, YYSTYPE *));
%}

%type <voidval> exp exp1 type_exp start variable qualified_name lcurly
%type <lval> rcurly
%type <tval> type typebase
%type <tvec> nonempty_typelist
/* %type <bval> block */

/* Fancy type parsing.  */
%type <voidval> func_mod direct_abs_decl abs_decl
%type <tval> ptype
%type <lval> array_mod

%token <typed_val_int> INT
%token <typed_val_float> FLOAT

/* Both NAME and TYPENAME tokens represent symbols in the input, and
   both convey their data as strings.  But a TYPENAME is a string that
   happens to be defined as a typedef or builtin type name (such as
   int or char) and a NAME is any other symbol.  Contexts where this
   distinction is not important can use the nonterminal "name", which
   matches either NAME or TYPENAME.  */

%token <sval> STRING
%token <sval> NSSTRING		/* ObjC Foundation "NSString" literal */
%token <sval> SELECTOR		/* ObjC "@selector" pseudo-operator   */
%token <ssym> NAME /* BLOCKNAME defined below to give it higher precedence. */
%token <tsym> TYPENAME
%token <oclass> CLASSNAME	/* ObjC Class name */
%type <sval> name
%type <ssym> name_not_typename
%type <tsym> typename

/* A NAME_OR_INT is a symbol which is not known in the symbol table,
   but which would parse as a valid number in the current input radix.
   E.g. "c" when input_radix==16.  Depending on the parse, it will be
   turned into a name or into a number.  */

%token <ssym> NAME_OR_INT

%token STRUCT CLASS UNION ENUM SIZEOF UNSIGNED COLONCOLON
%token TEMPLATE
%token ERROR

/* Special type cases, put in to allow the parser to distinguish
   different legal basetypes.  */
%token SIGNED_KEYWORD LONG SHORT INT_KEYWORD CONST_KEYWORD VOLATILE_KEYWORD DOUBLE_KEYWORD

%token <voidval> VARIABLE

%token <opcode> ASSIGN_MODIFY

%left ','
%left ABOVE_COMMA
%right '=' ASSIGN_MODIFY
%right '?'
%left OROR
%left ANDAND
%left '|'
%left '^'
%left '&'
%left EQUAL NOTEQUAL
%left '<' '>' LEQ GEQ
%left LSH RSH
%left '@'
%left '+' '-'
%left '*' '/' '%'
%right UNARY INCREMENT DECREMENT
%right ARROW '.' '[' '('
%token <ssym> BLOCKNAME
%type <bval> block
%left COLONCOLON


%%

start   :	exp1
	|	type_exp
	;

type_exp:	type
			{ write_exp_elt_opcode(OP_TYPE);
			  write_exp_elt_type($1);
			  write_exp_elt_opcode(OP_TYPE);}
	;

/* Expressions, including the comma operator: */
exp1	:	exp
	|	exp1 ',' exp
			{ write_exp_elt_opcode(BINOP_COMMA); }
	;

/* Expressions, not including the comma operator: */
exp	:	'*' exp    %prec UNARY
			{ write_exp_elt_opcode(UNOP_IND); }
	;

exp	:	'&' exp    %prec UNARY
			{ write_exp_elt_opcode(UNOP_ADDR); }
	;

exp	:	'-' exp    %prec UNARY
			{ write_exp_elt_opcode(UNOP_NEG); }
	;

exp	:	'!' exp    %prec UNARY
			{ write_exp_elt_opcode(UNOP_LOGICAL_NOT); }
	;

exp	:	'~' exp    %prec UNARY
			{ write_exp_elt_opcode(UNOP_COMPLEMENT); }
	;

exp	:	INCREMENT exp    %prec UNARY
			{ write_exp_elt_opcode(UNOP_PREINCREMENT); }
	;

exp	:	DECREMENT exp    %prec UNARY
			{ write_exp_elt_opcode(UNOP_PREDECREMENT); }
	;

exp	:	exp INCREMENT    %prec UNARY
			{ write_exp_elt_opcode(UNOP_POSTINCREMENT); }
	;

exp	:	exp DECREMENT    %prec UNARY
			{ write_exp_elt_opcode(UNOP_POSTDECREMENT); }
	;

exp	:	SIZEOF exp       %prec UNARY
			{ write_exp_elt_opcode(UNOP_SIZEOF); }
	;

exp	:	exp ARROW name
			{ write_exp_elt_opcode (STRUCTOP_PTR);
			  write_exp_string ($3);
			  write_exp_elt_opcode (STRUCTOP_PTR); }
	;

exp	:	exp ARROW qualified_name
			{ /* exp->type::name becomes exp->*(&type::name) */
			  /* Note: this doesn't work if name is a
			     static member!  FIXME */
			  write_exp_elt_opcode (UNOP_ADDR);
			  write_exp_elt_opcode (STRUCTOP_MPTR); }
	;
exp	:	exp ARROW '*' exp
			{ write_exp_elt_opcode (STRUCTOP_MPTR); }
	;

exp	:	exp '.' name
			{ write_exp_elt_opcode (STRUCTOP_STRUCT);
			  write_exp_string ($3);
			  write_exp_elt_opcode (STRUCTOP_STRUCT); }
	;


exp	:	exp '.' qualified_name
			{ /* exp.type::name becomes exp.*(&type::name) */
			  /* Note: this doesn't work if name is a
			     static member!  FIXME */
			  write_exp_elt_opcode (UNOP_ADDR);
			  write_exp_elt_opcode (STRUCTOP_MEMBER); }
	;

exp	:	exp '.' '*' exp
			{ write_exp_elt_opcode (STRUCTOP_MEMBER); }
	;

exp	:	exp '[' exp1 ']'
			{ write_exp_elt_opcode (BINOP_SUBSCRIPT); }
	;
/*
 * The rules below parse ObjC message calls of the form:
 *	'[' target selector {':' argument}* ']'
 */

exp	: 	'[' TYPENAME
			{
			  CORE_ADDR objc_class;

			  objc_class = lookup_objc_class(copy_name($2.stoken));
			  if (objc_class == 0)
			    error("%s is not an ObjC Class",
				  copy_name($2.stoken));
			  write_exp_elt_opcode(OP_LONG);
			  write_exp_elt_type(builtin_type_void_data_ptr);
			  write_exp_elt_longcst((LONGEST)objc_class);
			  write_exp_elt_opcode(OP_LONG);
			  start_msglist();
			}
		msglist ']'
			{ write_exp_elt_opcode(OP_OBJC_MSGCALL);
			  end_msglist();
			  write_exp_elt_opcode(OP_OBJC_MSGCALL);
			}
	;

exp	:	'[' CLASSNAME
			{
			  write_exp_elt_opcode(OP_LONG);
			  write_exp_elt_type(builtin_type_void_data_ptr);
			  write_exp_elt_longcst((LONGEST)$2.objc_class);
			  write_exp_elt_opcode(OP_LONG);
			  start_msglist();
			}
		msglist ']'
			{ write_exp_elt_opcode(OP_OBJC_MSGCALL);
			  end_msglist();
			  write_exp_elt_opcode(OP_OBJC_MSGCALL);
			}
	;

exp	:	'[' exp
			{ start_msglist(); }
		msglist ']'
			{ write_exp_elt_opcode(OP_OBJC_MSGCALL);
			  end_msglist();
			  write_exp_elt_opcode(OP_OBJC_MSGCALL);
			}
	;

msglist :	name
			{ add_msglist(&$1, 0); }
	|	msgarglist
	;

msgarglist :	msgarg
	|	msgarglist msgarg
	;

msgarg	:	name ':' exp
			{ add_msglist(&$1, 1); }
	|	':' exp	/* Unnamed arg.  */
			{ add_msglist(0, 1);   }
	|	',' exp	/* Variable number of args.  */
			{ add_msglist(0, 0);   }
	;

exp	:	exp '('
			/* This is to save the value of arglist_len
			   being accumulated by an outer function call.  */
			{ start_arglist (); }
		arglist ')'	%prec ARROW
			{ write_exp_elt_opcode (OP_FUNCALL);
			  write_exp_elt_longcst ((LONGEST) end_arglist ());
			  write_exp_elt_opcode (OP_FUNCALL); }
	;

lcurly	:	'{'
			{ start_arglist (); }
	;

arglist	:
	;

arglist	:	exp
			{ arglist_len = 1; }
	;

arglist	:	arglist ',' exp   %prec ABOVE_COMMA
			{ arglist_len++; }
	;

rcurly	:	'}'
			{ $$ = end_arglist () - 1; }
	;
exp	:	lcurly arglist rcurly	%prec ARROW
			{ write_exp_elt_opcode (OP_ARRAY);
			  write_exp_elt_longcst ((LONGEST) 0);
			  write_exp_elt_longcst ((LONGEST) $3);
			  write_exp_elt_opcode (OP_ARRAY); }
	;

exp	:	lcurly type rcurly exp  %prec UNARY
			{ write_exp_elt_opcode (UNOP_MEMVAL);
			  write_exp_elt_type ($2);
			  write_exp_elt_opcode (UNOP_MEMVAL); }
	;

exp	:	'(' type ')' exp  %prec UNARY
			{ write_exp_elt_opcode (UNOP_CAST);
			  write_exp_elt_type ($2);
			  write_exp_elt_opcode (UNOP_CAST); }
	;

exp	:	'(' exp1 ')'
			{ }
	;

/* Binary operators in order of decreasing precedence.  */

exp	:	exp '@' exp
			{ write_exp_elt_opcode(BINOP_REPEAT); }
	;

exp	:	exp '*' exp
			{ write_exp_elt_opcode(BINOP_MUL); }
	;

exp	:	exp '/' exp
			{ write_exp_elt_opcode(BINOP_DIV); }
	;

exp	:	exp '%' exp
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

exp	:	exp EQUAL exp
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

exp	:	exp '&' exp
			{ write_exp_elt_opcode(BINOP_BITWISE_AND); }
	;

exp	:	exp '^' exp
			{ write_exp_elt_opcode(BINOP_BITWISE_XOR); }
	;

exp	:	exp '|' exp
			{ write_exp_elt_opcode(BINOP_BITWISE_IOR); }
	;

exp	:	exp ANDAND exp
			{ write_exp_elt_opcode(BINOP_LOGICAL_AND); }
	;

exp	:	exp OROR exp
			{ write_exp_elt_opcode(BINOP_LOGICAL_OR); }
	;

exp	:	exp '?' exp ':' exp	%prec '?'
			{ write_exp_elt_opcode(TERNOP_COND); }
	;

exp	:	exp '=' exp
			{ write_exp_elt_opcode(BINOP_ASSIGN); }
	;

exp	:	exp ASSIGN_MODIFY exp
			{ write_exp_elt_opcode(BINOP_ASSIGN_MODIFY);
			  write_exp_elt_opcode($2);
			  write_exp_elt_opcode(BINOP_ASSIGN_MODIFY); }
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
			{ write_exp_elt_opcode (OP_DOUBLE);
			  write_exp_elt_type ($1.type);
			  write_exp_elt_dblcst ($1.dval);
			  write_exp_elt_opcode (OP_DOUBLE); }
	;

exp	:	variable
	;

exp	:	VARIABLE
			/* Already written by write_dollar_variable.  */
	;

exp	:	SELECTOR
			{
			  write_exp_elt_opcode (OP_OBJC_SELECTOR);
			  write_exp_string ($1);
			  write_exp_elt_opcode (OP_OBJC_SELECTOR); }
	;

exp	:	SIZEOF '(' type ')'	%prec UNARY
			{ write_exp_elt_opcode (OP_LONG);
			  write_exp_elt_type (builtin_type_int);
			  CHECK_TYPEDEF ($3);
			  write_exp_elt_longcst ((LONGEST) TYPE_LENGTH ($3));
			  write_exp_elt_opcode (OP_LONG); }
	;

exp	:	STRING
			{ /* C strings are converted into array
			     constants with an explicit null byte
			     added at the end.  Thus the array upper
			     bound is the string length.  There is no
			     such thing in C as a completely empty
			     string.  */
			  char *sp = $1.ptr; size_t count = $1.length;
			  while (count-- > 0UL)
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
			  write_exp_elt_longcst((LONGEST)0L);
			  write_exp_elt_longcst((LONGEST)($1.length));
			  write_exp_elt_opcode(OP_ARRAY); }
	;

exp     :	NSSTRING	/* ObjC NextStep NSString constant
				 * of the form '@' '"' string '"'.
				 */
			{ write_exp_elt_opcode (OP_OBJC_NSSTRING);
			  write_exp_string ($1);
			  write_exp_elt_opcode (OP_OBJC_NSSTRING); }
	;

block	:	BLOCKNAME
			{
			  if ($1.sym != 0)
			      $$ = SYMBOL_BLOCK_VALUE ($1.sym);
			  else
			    {
			      struct symtab *tem =
				  lookup_symtab (copy_name ($1.stoken));
			      if (tem)
				$$ = BLOCKVECTOR_BLOCK (BLOCKVECTOR (tem), STATIC_BLOCK);
			      else
				error ("No file or function \"%s\".",
				       copy_name ($1.stoken));
			    }
			}
	;

block	:	block COLONCOLON name
			{ struct symbol *tem
			    = lookup_symbol (copy_name ($3), $1,
					     VAR_DOMAIN, (int *) NULL,
					     (struct symtab **) NULL);
			  if (!tem || SYMBOL_CLASS (tem) != LOC_BLOCK)
			    error ("No function \"%s\" in specified context.",
				   copy_name ($3));
			  $$ = SYMBOL_BLOCK_VALUE (tem); }
	;

variable:	block COLONCOLON name
			{ struct symbol *sym;
			  sym = lookup_symbol (copy_name ($3), $1,
					       VAR_DOMAIN, (int *) NULL,
					       (struct symtab **) NULL);
			  if (sym == 0)
			    error ("No symbol \"%s\" in specified context.",
				   copy_name ($3));

			  write_exp_elt_opcode (OP_VAR_VALUE);
			  /* block_found is set by lookup_symbol.  */
			  write_exp_elt_block (block_found);
			  write_exp_elt_sym (sym);
			  write_exp_elt_opcode (OP_VAR_VALUE); }
	;

qualified_name:	typebase COLONCOLON name
			{
			  struct type *type = $1;
			  if (TYPE_CODE (type) != TYPE_CODE_STRUCT
			      && TYPE_CODE (type) != TYPE_CODE_UNION)
			    error ("`%s' is not defined as an aggregate type.",
				   TYPE_NAME (type));

			  write_exp_elt_opcode (OP_SCOPE);
			  write_exp_elt_type (type);
			  write_exp_string ($3);
			  write_exp_elt_opcode (OP_SCOPE);
			}
	|	typebase COLONCOLON '~' name
			{
			  struct type *type = $1;
			  struct stoken tmp_token;
			  if (TYPE_CODE (type) != TYPE_CODE_STRUCT
			      && TYPE_CODE (type) != TYPE_CODE_UNION)
			    error ("`%s' is not defined as an aggregate type.",
				   TYPE_NAME (type));

			  if (!DEPRECATED_STREQ (type_name_no_tag (type), $4.ptr))
			    error ("invalid destructor `%s::~%s'",
				   type_name_no_tag (type), $4.ptr);

			  tmp_token.ptr = (char*) alloca ($4.length + 2);
			  tmp_token.length = $4.length + 1;
			  tmp_token.ptr[0] = '~';
			  memcpy (tmp_token.ptr+1, $4.ptr, $4.length);
			  tmp_token.ptr[tmp_token.length] = 0;
			  write_exp_elt_opcode (OP_SCOPE);
			  write_exp_elt_type (type);
			  write_exp_string (tmp_token);
			  write_exp_elt_opcode (OP_SCOPE);
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
				  if ((innermost_block == 0)
				      || contained_in(block_found,
						      innermost_block))
				    innermost_block = (struct block *)block_found;
				}

			      write_exp_elt_opcode (OP_VAR_VALUE);
			      /* We want to use the selected frame, not
				 another more inner frame which happens to
				 be in the same block.  */
			      write_exp_elt_block (NULL);
			      write_exp_elt_sym (sym);
			      write_exp_elt_opcode (OP_VAR_VALUE);
			    }
			  else if ($1.is_a_field_of_this)
			    {
                              struct symbol *func;
                              enum runtime_type runtime = OBJC_RUNTIME;

			      /* C++/ObjC: it hangs off of `this'/'self'.
				 Must not inadvertently convert from a
				 method call to data ref.  */

			      if ((innermost_block == 0)
				  || contained_in(block_found, innermost_block))
				innermost_block = (struct block *)block_found;
                              if (innermost_block)
                                {
                                  struct block *bl = innermost_block;
                                  while ((BLOCK_FUNCTION(bl) == NULL)
                                         && BLOCK_SUPERBLOCK(bl))
                                    bl = BLOCK_SUPERBLOCK(bl);
                                  func = BLOCK_FUNCTION(bl);
                                }
                              else
                                func = NULL;

                              /* If this is an ObjC++ file we could have either
                                 a C++ function or an ObjC function - which
                                 determines whether OP_OBJC_SELF or OP_THIS is
                                 the right opcode to add here.  */

                              if (func
                                  && TYPE_RUNTIME (SYMBOL_TYPE (func)) != OBJC_RUNTIME)
                                {
                                  if (SYMBOL_LANGUAGE(func) == language_cplus)
                                    {
                                      runtime = CPLUS_RUNTIME;
                                    }
				  else if (SYMBOL_LANGUAGE(func) == language_objcplus)
				    {
				      /* If we didn't actually set the runtime for the
					 function type from the debug info, let's try
					 to determine it heuristically here.  The easiest
					 course is to see if this looks like an ObjC method
					 name.  That will start with "-[" or "+[", and since
					 those aren't legal C++ names that's a pretty good
					 test.  */
				      const char *name;
				      name = SYMBOL_NATURAL_NAME(func);
				      if ((name != NULL)
					  && (strlen(name) > 2UL)
					  && ((name[0] == '-' || name[0] == '+')
					      && name[1] == '['))
					{
					  runtime = OBJC_RUNTIME;
					}
				      else
					{
					  runtime = CPLUS_RUNTIME;
					}
				    }
                                }

                              if (runtime == OBJC_RUNTIME)
                                {
			          write_exp_elt_opcode (OP_OBJC_SELF);
			          write_exp_elt_opcode (OP_OBJC_SELF);
                                }
                              else
                                {
			          write_exp_elt_opcode (OP_THIS);
			          write_exp_elt_opcode (OP_THIS);
                                }
			      write_exp_elt_opcode (STRUCTOP_PTR);
			      write_exp_string ($1.stoken);
			      write_exp_elt_opcode (STRUCTOP_PTR);
			    }
			  else
			    {
			      struct minimal_symbol *msymbol;
			      char *arg = copy_name ($1.stoken);

			      msymbol =
				lookup_minimal_symbol (arg, NULL, NULL);
			      if (msymbol != NULL)
				{
				  write_exp_msymbol (msymbol,
						     lookup_function_type (builtin_type_int),
						     builtin_type_int);
				}
			      else if (!have_full_symbols () &&
				       !have_partial_symbols ())
				error ("No symbol table is loaded.  Use the \"file\" command.");
			      else
				error ("No symbol \"%s\" in current context.",
				       copy_name ($1.stoken));
			    }
			}
	;


ptype	:	typebase
	/* "const" and "volatile" are curently ignored.  A type
	   qualifier before the type is currently handled in the
	   typebase rule.  The reason for recognizing these here
	   (shift/reduce conflicts) might be obsolete now that some
	   pointer to member rules have been deleted.  */
	|	typebase CONST_KEYWORD
	|	typebase VOLATILE_KEYWORD
	|	typebase abs_decl
		{ $$ = follow_types ($1); }
	|	typebase CONST_KEYWORD abs_decl
		{ $$ = follow_types ($1); }
	|	typebase VOLATILE_KEYWORD abs_decl
		{ $$ = follow_types ($1); }
	;

abs_decl:	'*'
			{ push_type (tp_pointer); $$ = 0; }
	|	'*' abs_decl
			{ push_type (tp_pointer); $$ = $2; }
	|	'&'
			{ push_type (tp_reference); $$ = 0; }
	|	'&' abs_decl
			{ push_type (tp_reference); $$ = $2; }
	|	direct_abs_decl
	;

direct_abs_decl: '(' abs_decl ')'
			{ $$ = $2; }
	|	direct_abs_decl array_mod
			{
			  push_type_int((int)$2);
			  push_type(tp_array);
			}
	|	array_mod
			{
			  push_type_int((int)$1);
			  push_type(tp_array);
			  $$ = 0;
			}

	| 	direct_abs_decl func_mod
			{ push_type (tp_function); }
	|	func_mod
			{ push_type (tp_function); }
	;

array_mod:	'[' ']'
			{ $$ = -1; }
	|	'[' INT ']'
			{ $$ = $2.val; }
	;

func_mod:	'(' ')'
			{ $$ = 0; }
	|	'(' nonempty_typelist ')'
			{ free ($2); $$ = 0; }
	;

/* We used to try to recognize more pointer to member types here, but
   that didn't work (shift/reduce conflicts meant that these rules
   never got executed).  The problem is that
     int (foo::bar::baz::bizzle)
   is a function type but
     int (foo::bar::baz::bizzle::*)
   is a pointer to member type.  Stroustrup loses again!  */

type	:	ptype
	|	typebase COLONCOLON '*'
			{ $$ = lookup_member_type(builtin_type_int, $1); }
	;

typebase  /* Implements (approximately): (type-qualifier)* type-specifier.  */
	:	TYPENAME
			{ $$ = $1.type; }
	|	CLASSNAME
			{
			  if ($1.type == NULL)
			    error("No symbol \"%s\" in current context.",
				  copy_name($1.stoken));
			  else
			    $$ = $1.type;
			}
	|	INT_KEYWORD
			{ $$ = builtin_type_int; }
	|	LONG
			{ $$ = builtin_type_long; }
	|	SHORT
			{ $$ = builtin_type_short; }
	|	LONG INT_KEYWORD
			{ $$ = builtin_type_long; }
	|	UNSIGNED LONG INT_KEYWORD
			{ $$ = builtin_type_unsigned_long; }
	|	LONG LONG
			{ $$ = builtin_type_long_long; }
	|	LONG LONG INT_KEYWORD
			{ $$ = builtin_type_long_long; }
	|	UNSIGNED LONG LONG
			{ $$ = builtin_type_unsigned_long_long; }
	|	UNSIGNED LONG LONG INT_KEYWORD
			{ $$ = builtin_type_unsigned_long_long; }
	|	SHORT INT_KEYWORD
			{ $$ = builtin_type_short; }
	|	UNSIGNED SHORT INT_KEYWORD
			{ $$ = builtin_type_unsigned_short; }
	|	DOUBLE_KEYWORD
			{ $$ = builtin_type_double; }
	|	LONG DOUBLE_KEYWORD
			{ $$ = builtin_type_long_double; }
	|	STRUCT name
			{ $$ = lookup_struct (copy_name ($2),
					      expression_context_block); }
	|	CLASS name
			{ $$ = lookup_struct (copy_name ($2),
					      expression_context_block); }
	|	UNION name
			{ $$ = lookup_union (copy_name ($2),
					     expression_context_block); }
	|	ENUM name
			{ $$ = lookup_enum (copy_name ($2),
					    expression_context_block); }
	|	UNSIGNED typename
			{ $$ = lookup_unsigned_typename(TYPE_NAME($2.type)); }
	|	UNSIGNED
			{ $$ = builtin_type_unsigned_int; }
	|	SIGNED_KEYWORD typename
			{ $$ = lookup_signed_typename(TYPE_NAME($2.type)); }
	|	SIGNED_KEYWORD
			{ $$ = builtin_type_int; }
	|	TEMPLATE name '<' type '>'
			{ $$ = lookup_template_type(copy_name($2), $4,
						    expression_context_block);
			}
	/* "const" and "volatile" are curently ignored.  A type
	   qualifier after the type is handled in the ptype rule.  I
	   think these could be too.  */
	|	CONST_KEYWORD typebase { $$ = $2; }
	|	VOLATILE_KEYWORD typebase { $$ = $2; }
	;

typename:	TYPENAME
	|	INT_KEYWORD
		{
		  $$.stoken.ptr = (char *)"int";
		  $$.stoken.length = 3;
		  $$.type = builtin_type_int;
		}
	|	LONG
		{
		  $$.stoken.ptr = (char *)"long";
		  $$.stoken.length = 4;
		  $$.type = builtin_type_long;
		}
	|	SHORT
		{
		  $$.stoken.ptr = (char *)"short";
		  $$.stoken.length = 5;
		  $$.type = builtin_type_short;
		}
	;

nonempty_typelist
	:	type
		{ $$ = (struct type **)malloc(sizeof(struct type *) * 2UL);
		  $<ivec>$[0] = 1;	/* Number of types in vector.  */
		  $$[1] = $1;
		}
	|	nonempty_typelist ',' type
		{ size_t len = sizeof(struct type *) * (++($<ivec>1[0]) + 1UL);
		  $$ = (struct type **)realloc((char *)$1, len);
		  $$[$<ivec>$[0]] = $3;
		}
	;

name	:	NAME        { $$ = $1.stoken; }
	|	BLOCKNAME   { $$ = $1.stoken; }
	|	TYPENAME    { $$ = $1.stoken; }
	|	CLASSNAME   { $$ = $1.stoken; }
	|	NAME_OR_INT { $$ = $1.stoken; }
	;

name_not_typename :	NAME
	|	BLOCKNAME
/* These would be useful if name_not_typename was useful, but it is
   just a fake for "variable", so these cause reduce/reduce conflicts
   because the parser can't tell whether NAME_OR_INT is a
   name_not_typename (=variable, =exp) or just an exp.  If
   name_not_typename was ever used in an lvalue context where only a
   name could occur, this might be useful.  */
/*  	| NAME_OR_INT */
	;

%%

/* Take care of parsing a number (anything that starts with a digit).
   Set yylval and return the token type; update lexptr.  LEN is the
   number of characters in it.  */

/*** Needs some error checking for the float case.  ***/

/* APPLE LOCAL: I replaced the objc-exp.y version with the c-exp.y
   version.  There were inessential differences which had no good
   purpose, and only made maintaining them harder.  I really should
   extract this into a separate .c file & #include it here.  */

static int
parse_number(char *p, int len, int parsed_float, YYSTYPE *putithere)
{
  /* FIXME: Shouldn't these be unsigned?  We don't deal with negative values
     here, and we do kind of silly things like cast to unsigned.  */
  LONGEST n = 0L;
  LONGEST prevn = 0L;
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
      /* It's a float since it contains a point or an exponent.  */
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
      if (num == 1) 		/* check scanf found ONLY a float ... */
        putithere->typed_val_float.type =
          get_builtin_type(current_gdbarch)->builtin_double;
      else if (num == 2)
	{
	  /* See if it has `f' or `l' suffix (float or long double).  */

	  c_c = (char)tolower(p[len - 1]);
	  /* There's no way to tell, using sscanf, whether we actually
	     did ingest all the input.  But this check will catch things
	     like: 123fghi.jklmn, though of course it will be fooled by
	     123fghi.jklmf.  I'm not really all that worried about this,
	     however.  */

	  if (c_c != p[len - 1])
	    return ERROR;

	  if (c_c == 'f')
	    putithere->typed_val_float.type =
	      get_builtin_type(current_gdbarch)->builtin_float;
	  else if (c_c == 'l')
	    putithere->typed_val_float.type =
	      get_builtin_type(current_gdbarch)->builtin_long_double;
	  else
	    return ERROR;
	}
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
	 a second check for zero).  FIXME: Can't we just make n and prevn
	 unsigned and avoid this?  */
      if ((i_c != 'l') && (i_c != 'u') && (prevn >= n) && (n != 0))
	unsigned_p = 1;		/* Try something unsigned */

      /* Portably test for unsigned overflow.
	 FIXME: This check is wrong; for example it doesn't find overflow
	 on 0x123456789 when LONGEST is 32 bits.  */
      if ((i_c != 'l') && (i_c != 'u') && (n != 0))
	{
	  if (unsigned_p && ((ULONGEST)prevn >= (ULONGEST)n))
	    error(_("Numeric constant too large."));
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

  un = ((ULONGEST)n >> 2);
  if ((long_p == 0)
      && (un >> (TARGET_INT_BIT - 2)) == 0)
    {
      high_bit = ((ULONGEST)1UL) << (TARGET_INT_BIT - 1);

      /* A large decimal (not hex or octal) constant (between INT_MAX
	 and UINT_MAX) is a long or unsigned long, according to ANSI,
	 never an unsigned int, but this code treats it as unsigned
	 int.  This probably should be fixed.  GCC gives a warning on
	 such constants.  */

      unsigned_type = get_builtin_type(current_gdbarch)->builtin_unsigned_int;
      signed_type = get_builtin_type(current_gdbarch)->builtin_int;
    }
  else if ((long_p <= 1)
	   && ((un >> (TARGET_LONG_BIT - 2)) == 0))
    {
      high_bit = (((ULONGEST)1UL) << (TARGET_LONG_BIT - 1));
      unsigned_type = get_builtin_type(current_gdbarch)->builtin_unsigned_long;
      signed_type = get_builtin_type(current_gdbarch)->builtin_long;
    }
  else
    {
      int shift;
      if ((sizeof(ULONGEST) * HOST_CHAR_BIT) < (size_t)TARGET_LONG_LONG_BIT)
	/* A long long does not fit in a LONGEST.  */
	shift = (sizeof(ULONGEST) * HOST_CHAR_BIT - 1);
      else
	shift = (TARGET_LONG_LONG_BIT - 1);
      high_bit = ((ULONGEST)1UL << shift);
      unsigned_type = get_builtin_type(current_gdbarch)->builtin_unsigned_long_long;
      signed_type = get_builtin_type(current_gdbarch)->builtin_long_long;
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

struct token
{
  const char *ooperator;
  int token;
  enum exp_opcode opcode;
};

static const struct token tokentab3[] =
  {
    {">>=", ASSIGN_MODIFY, BINOP_RSH},
    {"<<=", ASSIGN_MODIFY, BINOP_LSH}
  };

static const struct token tokentab2[] =
  {
    {"+=", ASSIGN_MODIFY, BINOP_ADD},
    {"-=", ASSIGN_MODIFY, BINOP_SUB},
    {"*=", ASSIGN_MODIFY, BINOP_MUL},
    {"/=", ASSIGN_MODIFY, BINOP_DIV},
    {"%=", ASSIGN_MODIFY, BINOP_REM},
    {"|=", ASSIGN_MODIFY, BINOP_BITWISE_IOR},
    {"&=", ASSIGN_MODIFY, BINOP_BITWISE_AND},
    {"^=", ASSIGN_MODIFY, BINOP_BITWISE_XOR},
    {"++", INCREMENT, BINOP_END},
    {"--", DECREMENT, BINOP_END},
    {"->", ARROW, BINOP_END},
    {"&&", ANDAND, BINOP_END},
    {"||", OROR, BINOP_END},
    {"::", COLONCOLON, BINOP_END},
    {"<<", LSH, BINOP_END},
    {">>", RSH, BINOP_END},
    {"==", EQUAL, BINOP_END},
    {"!=", NOTEQUAL, BINOP_END},
    {"<=", LEQ, BINOP_END},
    {">=", GEQ, BINOP_END}
  };

/* APPLE LOCAL: from objc-lang.c: */
extern struct symbol *lookup_struct_typedef(const char *name,
                                            struct block *block,
                                            int noerr);
/* end APPLE LOCAL prototype */

/* Read one token, getting characters through lexptr: */
static int
yylex(void)
{
  int c, tokchr;
  int namelen;
  unsigned int i;
  char *tokstart;
  const char *tokptr;
  int tempbufindex;
  static char *tempbuf;
  static int tempbufsize;

 retry:

  tokstart = (char *)lexptr;
  /* See if it is a special token of length 3.  */
  for (i = 0; i < (sizeof(tokentab3) / sizeof(tokentab3[0])); i++)
    if (DEPRECATED_STREQN(tokstart, tokentab3[i].ooperator, 3))
      {
	lexptr += 3;
	yylval.opcode = tokentab3[i].opcode;
	return tokentab3[i].token;
      }

  /* See if it is a special token of length 2.  */
  for (i = 0; i < (sizeof(tokentab2) / sizeof(tokentab2[0])); i++)
    if (DEPRECATED_STREQN(tokstart, tokentab2[i].ooperator, 2))
      {
	lexptr += 2;
	yylval.opcode = tokentab2[i].opcode;
	return tokentab2[i].token;
      }

  /* APPLE LOCAL: initialize c to the first character here.
     It is used that way below where we go to parse a number.  */
  switch (tokchr = c = *tokstart)
    {
    case 0:
      return 0;

    case ' ':
    case '\t':
    case '\n':
      lexptr++;
      goto retry;

    case '\'':
      /* We either have a character constant ('0' or '\177' for
	 example) or we have a quoted symbol reference ('foo(int,int)'
	 in C++ for example).  */
      lexptr++;
      c = *lexptr++;
      if (c == '\\')
	c = parse_escape((const char **)&lexptr);
      else if (c == '\'')
	error(_("Empty character constant."));

      yylval.typed_val_int.val = c;
      yylval.typed_val_int.type = builtin_type_char;

      c = *lexptr++;
      if (c != '\'')
	{
	  namelen = (int)(skip_quoted(tokstart) - tokstart);
	  if (namelen > 2)
	    {
	      lexptr = tokstart + namelen;
	      if (lexptr[-1] != '\'')
		error(_("Unmatched single quote."));
	      namelen -= 2;
	      tokstart++;
	      goto tryname;
	    }
	  error(_("Invalid character constant."));
	}
      return INT;

    case '(':
      paren_depth++;
      lexptr++;
      return '(';

    case ')':
      if (paren_depth == 0)
	return 0;
      paren_depth--;
      lexptr++;
      return ')';

    case ',':
      if (comma_terminates && paren_depth == 0)
	return 0;
      lexptr++;
      return ',';

    case '.':
      /* Might be a floating point number.  */
      if (input_radix != 16 && !isdigit (lexptr[1]))
	goto symbol;		/* Nope, must be a symbol.  */
      /* Might be a floating point num in P format, e.g. 0x1.e84810f5c28fp+19 */
      if (input_radix == 16 && !ishexnumber (lexptr[1]))
	goto symbol;		/* Nope, must be a symbol. */
      /* FALL THRU into number case.  */
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
	/* APPLE LOCAL: I replaced the code here (which was probably
	   originally from NeXT) with the c-exp.y version.  The
	   original objc-exp.y version claimed to do better error
	   checking, but I am unconvinced of that, and in the process
	   it rejects all type modifiers, which seems too high a
	   price to pay for some unclear benefit.  */

	/* It is a number.  */
	int got_dot = 0, got_e = 0, got_p = 0, toktype;
	char *p = tokstart;
	int hex = input_radix > 10;

	if (c == '0' && (p[1] == 'x' || p[1] == 'X'))
	  {
	    p += 2;
	    hex = 1;
	  }
	else if (c == '0' && (p[1]=='t' || p[1]=='T' || p[1]=='d' || p[1]=='D'))
	  {
	    p += 2;
	    hex = 0;
	  }

	for (;; ++p)
	  {
	    /* This test includes !hex because 'e' is a valid hex digit
	       and thus does not indicate a floating point number when
	       the radix is hex.  */
	    if (!hex && !got_e && (*p == 'e' || *p == 'E'))
	      got_dot = got_e = 1;
	    /* This test does not include !hex, because a '.' always indicates
	       a decimal floating point number regardless of the radix.  */
	    else if (!got_dot && *p == '.')
	      got_dot = 1;
            /* APPLE LOCAL: Recognize the P formatting of floating point
               numbers with two hex components, e.g.
                  1000000.53 == 0x1.e84810f5c28f60000p+19 */
            else if (got_dot && hex && (*p == 'p' || *p == 'P'))
              got_p = 1;
            else if (((got_e && (p[-1] == 'e' || p[-1] == 'E')) ||
                      (got_p && (p[-1] == 'p' || p[-1] == 'P')))
                     && (*p == '-' || *p == '+'))
	      /* This is the sign of the exponent, not the end of the
		 number.  */
	      continue;
	    /* We will take any letters or digits.  parse_number will
	       complain if past the radix, or if L or U are not final.  */
	    else if ((*p < '0' || *p > '9')
		     && ((*p < 'a' || *p > 'z')
				  && (*p < 'A' || *p > 'Z')))
	      break;
	  }
	toktype = parse_number(tokstart, (int)(p - tokstart),
			       (got_dot | got_e | got_p), &yylval);
        if (toktype == ERROR)
	  {
	    char *err_copy = (char *)alloca(p - tokstart + 1UL);

	    memcpy(err_copy, tokstart, p - tokstart);
	    err_copy[p - tokstart] = 0;
	    error("Invalid number \"%s\".", err_copy);
	  }
	lexptr = p;
	return toktype;
	/* END APPLE LOCAL */
      }

    case '+':
    case '-':
    case '*':
    case '/':
    case '%':
    case '|':
    case '&':
    case '^':
    case '~':
    case '!':
#if 0
    case '@':		/* Moved out below.  */
#endif /* 0 */
    case '<':
    case '>':
      /* APPLE LOCAL begin avoid calling lookup_objc_class unnecessarily  */
      /* case '[':  Moved out below.  */
      /* case ']':  Moved out below.  */
      /* APPLE LOCAL end avoid calling lookup_objc_class unnecessarily  */
    case '?':
    case ':':
    case '=':
    case '{':
    case '}':
    symbol:
      lexptr++;
      return tokchr;

    /* APPLE LOCAL begin avoid calling lookup_objc_class unnecessarily  */
    case '[':
      square_bracket_seen = 1;
      lexptr++;
      return tokchr;

    case ']':
      square_bracket_seen = 0;
      lexptr++;
      return tokchr;

    /* APPLE LOCAL end avoid calling lookup_objc_class unnecessarily  */
    case '@':
      if (strncmp(tokstart, "@selector", 9) == 0)
	{
	  tokptr = strchr(tokstart, '(');
	  if (tokptr == NULL)
	    {
	      error("Missing '(' in @selector(...)");
	    }
	  tempbufindex = 0;
	  tokptr++;	/* Skip the '('.  */
	  do {
	    /* Grow the static temp buffer if necessary, including
	       allocating the first one on demand.  */
	    if (tempbufindex + 1 >= tempbufsize)
	      {
		tempbuf = (char *) realloc (tempbuf, tempbufsize += 64);
	      }
	    tempbuf[tempbufindex++] = *tokptr++;
	  } while ((*tokptr != ')') && (*tokptr != '\0'));
	  if (*tokptr++ != ')')
	    {
	      error ("Missing ')' in @selector(...)");
	    }
	  tempbuf[tempbufindex] = '\0';
	  yylval.sval.ptr = tempbuf;
	  yylval.sval.length = tempbufindex;
	  lexptr = (char *)tokptr;
	  return SELECTOR;
	}
      if (tokstart[1] != '"')
        {
          lexptr++;
          return tokchr;
        }
      /* ObjC NextStep NSString constant: fall thru and parse like
         STRING.  */
      tokstart++;
      ATTRIBUTE_FALLTHROUGH;
    case '"':

      /* Build the gdb internal form of the input string in tempbuf,
	 translating any standard C escape forms seen.  Note that the
	 buffer is null byte terminated *only* for the convenience of
	 debugging gdb itself and printing the buffer contents when
	 the buffer contains no embedded nulls.  Gdb does not depend
	 upon the buffer being null byte terminated, it uses the
	 length string instead.  This allows gdb to handle C strings
	 (as well as strings in other languages) with embedded null
	 bytes.  */

      tokptr = ++tokstart;
      tempbufindex = 0;

      do {
	/* Grow the static temp buffer if necessary, including
	   allocating the first one on demand.  */
	if (tempbufindex + 1 >= tempbufsize)
	  {
	    tempbuf = (char *)realloc(tempbuf, tempbufsize += 64);
	  }
	switch (*tokptr)
	  {
	  case '\0':
	  case '"':
	    /* Do nothing, loop will terminate.  */
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
	  error ("Unterminated string in expression.");
	}
      tempbuf[tempbufindex] = '\0';	/* See note above.  */
      yylval.sval.ptr = tempbuf;
      yylval.sval.length = tempbufindex;
      lexptr = (char *)tokptr;
      return (tokchr == '@' ? NSSTRING : STRING);

    default:;
    } /* end switch */

  if (!(tokchr == '_' || tokchr == '$' ||
       (tokchr >= 'a' && tokchr <= 'z') || (tokchr >= 'A' && tokchr <= 'Z')))
    /* We must have come across a bad character (e.g. ';').  */
    error ("Invalid character '%c' in expression.", c);

  /* It's a name.  See how long it is.  */
  namelen = 0;
  for (c = tokstart[namelen];
       (c == '_' || c == '$' || (c >= '0' && c <= '9')
	|| (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '<');)
    {
       if (c == '<')
	 {
	   /* APPLE LOCAL: Make the template search code match the
	      version in c-exp.y.  */
	   /* Template parameter lists are part of the name.
	      FIXME: This mishandles `print $a<4&&$a>3'.  */

	   /* Scan ahead to get rest of the template specification.  Note
	      that we look ahead only when the '<' adjoins non-whitespace
	      characters; for comparison expressions, e.g. "a < b > c",
	      there must be spaces before the '<', etc. */

	   const char *p = find_template_name_end(tokstart + namelen);
	   if (p)
	     namelen = (int)(p - tokstart);
	   break;
	   /* END APPLE LOCAL */
	 }
       c = tokstart[++namelen];
     }

  /* The token "if" terminates the expression and is NOT
     removed from the input stream.  */
  if (namelen == 2 && tokstart[0] == 'i' && tokstart[1] == 'f')
    {
      return 0;
    }

  lexptr += namelen;

  tryname:

  /* Catch specific keywords.  Should be done with a data structure.  */
  switch (namelen)
    {
    case 8:
      if (DEPRECATED_STREQN (tokstart, "unsigned", 8))
	return UNSIGNED;
      if ((current_language->la_language == language_cplus
	   || current_language->la_language == language_objcplus)
	  && strncmp (tokstart, "template", 8) == 0)
	return TEMPLATE;
      if (DEPRECATED_STREQN (tokstart, "volatile", 8))
	return VOLATILE_KEYWORD;
      break;
    case 6:
      if (DEPRECATED_STREQN (tokstart, "struct", 6))
	return STRUCT;
      if (DEPRECATED_STREQN (tokstart, "signed", 6))
	return SIGNED_KEYWORD;
      if (DEPRECATED_STREQN (tokstart, "sizeof", 6))
	return SIZEOF;
      if (DEPRECATED_STREQN (tokstart, "double", 6))
	return DOUBLE_KEYWORD;
      break;
    case 5:
      if ((current_language->la_language == language_cplus
	   || current_language->la_language == language_objcplus)
	  && strncmp(tokstart, "class", 5) == 0)
	return CLASS;
      if (DEPRECATED_STREQN(tokstart, "union", 5))
	return UNION;
      if (DEPRECATED_STREQN(tokstart, "short", 5))
	return SHORT;
      if (DEPRECATED_STREQN(tokstart, "const", 5))
	return CONST_KEYWORD;
      break;
    case 4:
      if (DEPRECATED_STREQN(tokstart, "enum", 4))
	return ENUM;
      if (DEPRECATED_STREQN(tokstart, "long", 4))
	return LONG;
      break;
    case 3:
      if (DEPRECATED_STREQN(tokstart, "int", 3))
	return INT_KEYWORD;
      break;
    default:
      break;
    }

  yylval.sval.ptr = tokstart;
  yylval.sval.length = namelen;

  if (*tokstart == '$')
    {
      write_dollar_variable (yylval.sval);
      return VARIABLE;
    }

  /* Use token-type BLOCKNAME for symbols that happen to be defined as
     functions or symtabs.  If this is not so, then ...
     Use token-type TYPENAME for symbols that happen to be defined
     currently as names of types; NAME for other symbols.
     The caller is not constrained to care about the distinction.  */
  {
    char *tmp = copy_name (yylval.sval);
    struct symbol *sym;
    int is_a_field_of_this = 0, *need_this;
    int hextype;

    if (current_language->la_language == language_cplus
	|| current_language->la_language == language_objc
	|| current_language->la_language == language_objcplus)
      need_this = &is_a_field_of_this;
    else
      need_this = (int *) NULL;

    sym = lookup_symbol (tmp, expression_context_block,
			 VAR_DOMAIN,
			 need_this,
			 (struct symtab **) NULL);
    /* Call lookup_symtab, not lookup_partial_symtab, in case there
       are no psymtabs (coff, xcoff, or some future change to blow
       away the psymtabs once symbols are read).  */
    if ((sym && SYMBOL_CLASS (sym) == LOC_BLOCK) ||
        lookup_symtab (tmp))
      {
	yylval.ssym.sym = sym;
	yylval.ssym.is_a_field_of_this = is_a_field_of_this;
	return BLOCKNAME;
      }
    if (sym && SYMBOL_CLASS (sym) == LOC_TYPEDEF)
        {
#if 1
	  /* Despite the following flaw, we need to keep this code
	     enabled.  Because we can get called from
	     check_stub_method, if we don't handle nested types then
	     it screws many operations in any program which uses
	     nested types.  */
	  /* In "A::x", if x is a member function of A and there
	     happens to be a type (nested or not, since the stabs
	     don't make that distinction) named x, then this code
	     incorrectly thinks we are dealing with nested types
	     rather than a member function.  */

	  const char *p;
	  const char *namestart;
	  struct symbol *best_sym;

	  /* Look ahead to detect nested types.  This probably should
	     be done in the grammar, but trying seemed to introduce a
	     lot of shift/reduce and reduce/reduce conflicts.  It's
	     possible that it could be done, though.  Or perhaps a
	     non-grammar, but less ad hoc, approach would work well.  */

	  /* Since we do not currently have any way of distinguishing
	     a nested type from a non-nested one (the stabs don't tell
	     us whether a type is nested), we just ignore the
	     containing type.  */

	  p = lexptr;
	  best_sym = sym;
	  while (1)
	    {
	      /* Skip whitespace.  */
	      while (*p == ' ' || *p == '\t' || *p == '\n')
		++p;
	      if (*p == ':' && p[1] == ':')
		{
		  /* Skip the `::'.  */
		  p += 2;
		  /* Skip whitespace.  */
		  while (*p == ' ' || *p == '\t' || *p == '\n')
		    ++p;
		  namestart = p;
		  while (*p == '_' || *p == '$' || (*p >= '0' && *p <= '9')
			 || (*p >= 'a' && *p <= 'z')
			 || (*p >= 'A' && *p <= 'Z'))
		    ++p;
		  if (p != namestart)
		    {
		      struct symbol *cur_sym;
		      /* As big as the whole rest of the expression,
			 which is at least big enough.  */
		      char *ncopy = (char *)alloca(strlen(tmp)
						   + strlen(namestart) + 3UL);
		      char *tmp1;

		      tmp1 = ncopy;
		      memcpy(tmp1, tmp, strlen(tmp));
		      tmp1 += strlen(tmp);
		      memcpy(tmp1, "::", 2);
		      tmp1 += 2;
		      memcpy(tmp1, namestart, (p - namestart));
		      tmp1[p - namestart] = '\0';
		      cur_sym = lookup_symbol(ncopy,
					      expression_context_block,
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

	  yylval.tsym.type = SYMBOL_TYPE (best_sym);
#else /* not 0 */
	  yylval.tsym.type = SYMBOL_TYPE (sym);
#endif /* not 0 */
	  return TYPENAME;
        }
    yylval.tsym.type
      = language_lookup_primitive_type_by_name(current_language,
                                               current_gdbarch, tmp);
    if (yylval.tsym.type != NULL)
      return TYPENAME;

    /* See if it is an ObjC classname: */
    if (!sym)
      {
        sym = lookup_struct_typedef(tmp, expression_context_block, 1);
	/* APPLE LOCAL begin avoid calling lookup_objc_class unnecessarily  */
        if (sym && square_bracket_seen)
          {
	    CORE_ADDR Class = lookup_objc_class(tmp);
	    square_bracket_seen = 0;
	    /* APPLE LOCAL end avoid calling lookup_objc_class unnecessarily  */
	    if (Class)
	      {
	        yylval.oclass.objc_class = Class;
	        if ((sym = lookup_struct_typedef(tmp,
					         expression_context_block,
					         1)))
	          yylval.oclass.type = SYMBOL_TYPE(sym);
	        return CLASSNAME;
	      }
          }
      }

    /* Input names that aren't symbols but ARE valid hex numbers,
       when the input radix permits them, can be names or numbers
       depending on the parse.  Note we support radixes > 16 here.  */
    if (!sym &&
        (((tokstart[0] >= 'a') && (tokstart[0] < (char)('a' + input_radix - 10)))
         || ((tokstart[0] >= 'A') && (tokstart[0] < (char)('A' + input_radix - 10)))))
      {
 	YYSTYPE newlval;	/* Its value is ignored.  */
	hextype = parse_number (tokstart, namelen, 0, &newlval);
	if (hextype == INT)
	  {
	    yylval.ssym.sym = sym;
	    yylval.ssym.is_a_field_of_this = is_a_field_of_this;
	    return NAME_OR_INT;
	  }
      }

    /* Any other kind of symbol: */
    yylval.ssym.sym = sym;
    yylval.ssym.is_a_field_of_this = is_a_field_of_this;
    return NAME;
  }
}

void ATTR_NORETURN
yyerror(const char *msg)
{
  if (*lexptr == '\0')
    error("A %s near end of expression.", (msg ? msg : "error"));
  else
    error("A %s in expression, near '%s'.", (msg ? msg : "error"),
          (lexptr ? lexptr : "lexptr"));
}

/* End of obj-exp.y */
