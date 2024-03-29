%{
/* arparse.y - Stange script language parser */

/*   Copyright 1992, 1993, 1995, 1997, 1999, 2002, 2003
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
Foundation, Inc., 51 Franklin St., 5th Floor, Boston, MA 02110-1301, USA */


/* Contributed by Steve Chamberlain
   		  <sac@cygnus.com>
*/
#ifndef DONTDECLARE_MALLOC
# define DONTDECLARE_MALLOC 1
#endif /* !DONTDECLARE_MALLOC */
#include "config.h"
#include "bfd.h"
#include "bucomm.h"
#include "arsup.h"
  
#if defined(__GNUC__) && defined(__GNUC_MINOR__)
# if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6))
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wunsafe-loop-optimizations"
# endif /* gcc 4.6+ */
#endif /* GCC */

extern int verbose;
extern int yylex(void);
static int yyerror(const char *);
#ifndef yyparse
# define yyparse yy_arparse
#endif /* !yyparse */
#ifndef yydebug
# define yydebug yy_ardebug
#endif /* !yydebug */
%}

%union {
  char *name;
struct list *list ;

};

%token NEWLINE
%token VERBOSE
%token <name> FILENAME
%token ADDLIB
%token LIST
%token ADDMOD
%token CLEAR
%token CREATE
%token DELETE
%token DIRECTORY
%token END
%token EXTRACT
%token FULLDIR
%token HELP
%token QUIT
%token REPLACE
%token SAVE
%token OPEN

%type <list> modulelist
%type <list> modulename
%type <name> optional_filename
%%

start:
	{ prompt(); } session
	;

session:
	    session command_line
	|
	;

command_line:
		command NEWLINE { prompt(); }
	;

command:
		open_command
	|	create_command
	| 	verbose_command
	|	directory_command
	|	addlib_command
	|	clear_command
	|	addmod_command
	| 	save_command
        |       extract_command
	|	replace_command
	|	delete_command
	|	list_command
	| 	END	 { ar_end(); return 0; }
	| 	error
	|       FILENAME { yyerror("foo"); }
	|
	;


extract_command:
                EXTRACT modulename
		{ ar_extract($2); }
	;

replace_command:
		REPLACE modulename
		{ ar_replace($2); }
	;

clear_command:
		CLEAR
		{ ar_clear(); }
	;

delete_command:
		DELETE modulename
		{ ar_delete($2); }
	;
addmod_command:
	ADDMOD modulename
		{ ar_addmod($2); }
	;

list_command:
		LIST
		{ ar_list(); }
	;

save_command:
		SAVE
		{ ar_save(); }
	;



open_command:
		OPEN FILENAME
		{ ar_open($2,0); }
	;

create_command:
		CREATE FILENAME
		{ ar_open($2,1); }
	;


addlib_command:
		ADDLIB FILENAME modulelist
		{ ar_addlib($2,$3); }
	;
directory_command:
		DIRECTORY FILENAME modulelist optional_filename
		{ ar_directory($2, $3, $4); }
	;



optional_filename:
		FILENAME
		{ $$ = $1; }
	|	{ $$ = 0; }
	;

modulelist:
	'(' modulename ')'
		{ $$ = $2; }
	|
		{ $$ = 0; }
	;

modulename:
		modulename optcomma FILENAME
		{ 	struct list *n = (struct list *)malloc(sizeof(struct list));
			n->next = $1;
			n->name = $3;
			$$ = n;
		 }
	|	{ $$ = 0; }
	;


optcomma:
		','
	|
	;


verbose_command:
	VERBOSE
		{ verbose = !verbose; }
	;


%%

extern int linenumber;

/* */
static int
yyerror(const char *x ATTRIBUTE_UNUSED)
{
  printf(_("Syntax error in archive script, line %d\n"), (linenumber + 1));
  return 0;
}

/* keep condition the same as where we push: */
#if defined(__GNUC__) && defined(__GNUC_MINOR__)
# if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6))
#  pragma GCC diagnostic pop
# endif /* gcc 4.6+ */
#endif /* GCC */

#ifdef DONTDECLARE_MALLOC
# undef DONTDECLARE_MALLOC
#endif /* DONTDECLARE_MALLOC */

/* End of arparse.y */
