%{
/* arlex.l - Strange script language lexer */

/* Copyright 1992, 1997, 2000-2004
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


/* Contributed by Steve Chamberlain <sac@cygnus.com>.  */

#ifndef DONTDECLARE_MALLOC
# define DONTDECLARE_MALLOC 1
#endif /* !DONTDECLARE_MALLOC */
#ifndef YY_STACK_USED
# define YY_STACK_USED 0
#endif /* !YY_STACK_USED */
#ifndef YY_ALWAYS_INTERACTIVE
# define YY_ALWAYS_INTERACTIVE 0
#endif /* !YY_ALWAYS_INTERACTIVE */
#ifndef YY_NEVER_INTERACTIVE
# define YY_NEVER_INTERACTIVE 0
#endif /* !YY_NEVER_INTERACTIVE */
#ifndef YY_MAIN
# define YY_MAIN 0
#endif /* !YY_MAIN */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "ansidecl.h"
#include "libiberty.h"
#include "arparse.h"
  
#ifdef HAVE_UNISTD_H
# include <unistd.h> /* for isatty() */
#endif /* HAVE_UNISTD_H */

#ifndef YY_NO_UNPUT
# define YY_NO_UNPUT 1
#endif /* !YY_NO_UNPUT */

extern int yylex(void);
extern int yyinunput(void);
  
#if defined(__GNUC__) && defined(__GNUC_MINOR__)
# if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 2))
#  pragma GCC diagnostic ignored "-Wswitch-default"
#  pragma GCC diagnostic ignored "-Wold-style-definition"
# endif /* gcc 4.2+ */
#endif /* any gcc */

/* making this explicitly "extern" to pacify clang just causes linkage
 * issues, so instead just ignore clang: */
int linenumber;
%}

%a 10000
%o 25000

%%

"ADDLIB"   	{ return ADDLIB; }
"ADDMOD"   	{ return ADDMOD; }
"CLEAR"   	{ return CLEAR; }
"CREATE"   	{ return CREATE; }
"DELETE"   	{ return DELETE; }
"DIRECTORY"   	{ return DIRECTORY; }
"END"   	{ return END; }
"EXTRACT"   	{ return EXTRACT; }
"FULLDIR"   	{ return FULLDIR; }
"HELP"   	{ return HELP; }
"LIST"		{ return LIST; }
"OPEN"   	{ return OPEN; }
"REPLACE"   	{ return REPLACE; }
"VERBOSE"   	{ return VERBOSE; }
"SAVE"   	{ return SAVE; }
"addlib"   	{ return ADDLIB; }
"addmod"   	{ return ADDMOD; }
"clear"   	{ return CLEAR; }
"create"   	{ return CREATE; }
"delete"   	{ return DELETE; }
"directory"   	{ return DIRECTORY; }
"end"   	{ return END; }
"extract"   	{ return EXTRACT; }
"fulldir"   	{ return FULLDIR; }
"help"   	{ return HELP; }
"list"		{ return LIST; }
"open"   	{ return OPEN; }
"replace"   	{ return REPLACE; }
"verbose"   	{ return VERBOSE; }
"save"   	{ return SAVE; }
"+\n"           { linenumber ++; }
"("             { return '('; }
")"             { return ')'; }
","             { return ','; }
[A-Za-z0-9/\\$:.\-\_]+  {
		yylval.name =  xstrdup(yytext);
		return FILENAME;
		}
"*".* 		{ }
";".* 		{ }
" "		{ }
"\n"	 	 { linenumber ++; return NEWLINE; }

%%
#ifndef yywrap
/* Needed for lex, though not flex: */
int yywrap(void) { return 1; }
#endif /* !yywrap */

/* use some unused functions: */
#if defined(yyunput) || defined(input) || !defined(YY_NO_UNPUT) || \
    defined(HAVE_CONFIG_H)
extern int yyinunput(void)
{
# if defined(yyunput) || !defined(YY_NO_UNPUT)
  yyunput(0, "");
# endif /* yyunput || !YY_NO_UNPUT */

# if defined(input) || defined(__STRICT_ANSI__) || defined(HAVE_CONFIG_H)
  return input();
# endif /* input || __STRICT_ANSI__ || HAVE_CONFIG_H */
}
#endif /* yyunput || input || !YY_NO_UNPUT || HAVE_CONFIG_H */

#ifdef DONTDECLARE_MALLOC
# undef DONTDECLARE_MALLOC
#endif /* DONTDECLARE_MALLOC */
#ifdef YY_NO_UNPUT
# undef YY_NO_UNPUT
#endif /* YY_NO_UNPUT */

/* End of arlex.l */
