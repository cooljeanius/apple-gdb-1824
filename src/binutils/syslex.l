%{ /* syslex.l */
/* Copyright 2001, 2003, 2005 Free Software Foundation, Inc.

This file is part of GNU Binutils.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GLD; see the file COPYING.  If not, write to the Free
Software Foundation, 51 Franklin St., 5th Floor, Boston, MA
02110-1301, USA.  */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */
#if defined(HAVE_STRING_H) || (defined(STDC_HEADERS) && defined(__STDC__))
# include <string.h>
#else
# ifdef HAVE_STRINGS_H
#  include <strings.h>
# else
#  ifdef HAVE_MEMORY_H
#   include <memory.h>
#  else
#   if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#    warning "syslex.l needs a string-related header"
#   endif /* __GNUC__ && !__STRICT_ANSI__ */
#  endif /* HAVE_MEMORY_H */
# endif /* HAVE_STRINGS_H */
#endif /* HAVE_STRING_H || (STDC_HEADERS && __STDC__) */
#ifdef HAVE_UNISTD_H
# include <unistd.h> /* for isatty() */
#endif /* HAVE_UNISTD_H */
#include "sysinfo.h"

/* in case these got re-defined on us: */
#ifdef malloc
# undef malloc
#endif /* malloc */
#ifdef realloc
# undef realloc
#endif /* realloc */

#define YY_NO_UNPUT /* (nothing) */

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

#if defined(__GNUC__) && defined(__GNUC_MINOR__)
# if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 2))
#  pragma GCC diagnostic ignored "-Wswitch-default"
#  pragma GCC diagnostic ignored "-Wsign-compare"
#  pragma GCC diagnostic ignored "-Wold-style-definition"
# endif /* gcc 4.2+ */
#endif /* any gcc */

#ifndef yywrap
static int yywrap(void) { return 1; }
#endif /* !yywrap */

extern int yylex(void);
extern int yyinunput(void);
%}
%%
"(" { return '(';}
")" { return ')';}
"[" { return '[';}
"]" { return ']';}
" " { ; }
";".* { ; }
"\t" { ; }
"\n" { ; }
"\""[^\"]*"\"" {
	yylval.s = (char *)malloc(yyleng - 1);
	memcpy(yylval.s, (yytext + 1), (yyleng - 2));
	yylval.s[yyleng - 2] = '\0';
        return NAME;
	}

0x[0-9a-f]+ {
        yylval.i = (int)strtol(yytext, 0, 16);
	return  NUMBER;
	}

[0-9]+ {
        yylval.i = atoi(yytext);
	return  NUMBER;
	}


"bits" { yylval.i =1 ;return UNIT;}
"bit" { yylval.i = 1; return UNIT;}
"bytes" { yylval.i= 8; return UNIT;}
"byte" { yylval.i = 8; return UNIT;}

"int" { yylval.s = "INT"; return TYPE;}
"barray" { yylval.s = "BARRAY"; return TYPE;}
"chars" { yylval.s = "CHARS"; return TYPE;}
"variable" { yylval.i = 0; return NUMBER;}
"counted" { yylval.i = -4; return NUMBER;}
"addrsize" { yylval.i = -2; return NUMBER; }
"segsize" { yylval.i = -1; return NUMBER; }
"cond" { return COND;}
"repeat" { return REPEAT;}

%{
/* End of syslex.l */
%}
