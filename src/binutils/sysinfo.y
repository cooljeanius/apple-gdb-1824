/* sysinfo.y: Copyright 2001, 2003, 2005 Free Software Foundation, Inc.
 * Written by Steve Chamberlain of Cygnus Support <steve@cygnus.com>.  */
/*
This file is part of GNU binutils.

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

%{
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

/* in case these got re-defined on us: */
#ifdef malloc
# undef malloc
#endif /* malloc */
#ifdef realloc
# undef realloc
#endif /* realloc */

#include <stdio.h>
#include <stdlib.h>

#ifndef ATTRIBUTE_FALLTHROUGH
# if defined(__GNUC__) && (__GNUC__ >= 7)
#  define ATTRIBUTE_FALLTHROUGH __attribute__((fallthrough))
# else
#  define ATTRIBUTE_FALLTHROUGH /* FALLTHRU */
# endif /* gcc 7+ */
#endif /* !ATTRIBUTE_FALLTHROUGH */

static char writecode;
static const char *it;
static int code;
static const char *repeat;
static const char *oldrepeat;
static const char *name;
static int rdepth;
static const char *names[] = { " ", "[n]", "[n][m]" };
static const char *pnames[] = { "", "*", "**" };

static int yyerror(const char *s);
extern int yylex(void);
%}


%union {
 int i;
 char *s;
}
%token COND
%token REPEAT
%token '(' ')'
%token <s> TYPE
%token <s> NAME
%token <i> NUMBER UNIT
%type <i> attr_size
%type <s> attr_desc attr_id attr_type
%%

top:  {
  switch (writecode)
    {
    case 'i':
      printf("#ifdef SYSROFF_SWAP_IN\n");
      break;
    case 'p':
      printf("#ifdef SYSROFF_p\n");
      break;
    case 'd':
      break;
    case 'g':
      printf("#ifdef SYSROFF_SWAP_OUT\n");
      break;
    case 'c':
      printf("#ifdef SYSROFF_PRINT\n");
      printf("# include <stdio.h>\n");
      printf("# include <stdlib.h>\n");
      printf("# include <ansidecl.h>\n");
      break;
    default:
      break;
    }
 }
it_list {
  switch (writecode) {
  case 'i':
  case 'p':
  case 'g':
  case 'c':
    printf("#endif\n");
    break;
  case 'd':
    break;
  default:;
  }
}

  ;


it_list: it it_list
  |
  ;

it:
	'(' NAME NUMBER
      {
	it = $2; code = $3;
	switch (writecode)
	  {
	  case 'd':
	    printf("\n\n\n#define IT_%s_CODE 0x%x\n", it,code);
	    printf("struct IT_%s;\n", it);
	    printf("extern void sysroff_swap_%s_in PARAMS((struct IT_%s *));\n",
		   $2, it);
	    printf("extern void sysroff_swap_%s_out PARAMS((FILE *, struct IT_%s *));\n",
		   $2, it);
	    printf("extern void sysroff_print_%s_out PARAMS((struct IT_%s *));\n",
		   $2, it);
	    printf("struct IT_%s { \n", it);
	    break;
	  case 'i':
	    printf("void sysroff_swap_%s_in(struct IT_%s *ptr)\n", $2, it);
	    printf("{\n");
	    printf("\tunsigned char raw[255];\n");
	    printf("\tint idx = 0;\n");
	    printf("\tint size;\n");
	    printf("\tmemset(raw, 0, 255);\n");
	    printf("\tmemset(ptr, 0, sizeof(*ptr));\n");
	    printf("\tsize = fillup(raw);\n");
	    break;
	  case 'g':
	    printf("void sysroff_swap_%s_out(FILE *file, struct IT_%s *ptr)\n",
                   $2, it);
	    printf("{\n");
	    printf("\tunsigned char raw[255];\n");
	    printf("\tint idx = 16 ;\n");
	    printf("\tmemset(raw, 0, 255);\n");
	    printf("\tcode = IT_%s_CODE;\n", it);
	    break;
	  case 'o':
	    printf("void sysroff_swap_%s_out(bfd *abfd, struct IT_%s *ptr)\n",
                   $2, it);
	    printf("{\n");
	    printf("\tint idx = 0;\n");
	    break;
	  case 'c':
	    printf("void sysroff_print_%s_out(struct IT_%s *ptr)\n",
                   $2, it);
	    printf("{\n");
	    printf("\titheader(\"%s\", IT_%s_CODE);\n", $2, $2);
	    break;

	  case 't':
	    break;

          default:
            break;
	  }
      }
	it_field_list
')'
{
  switch (writecode) {
  case 'd':
    printf("};\n");
    break;
  case 'g':
    printf("\tchecksum(file, raw, idx, IT_%s_CODE);\n", it);
    ATTRIBUTE_FALLTHROUGH;
  case 'i':

  case 'o':
  case 'c':
    printf("}\n");
    ATTRIBUTE_FALLTHROUGH;
  default:;
  }
}
;



it_field_list:
		it_field it_field_list
	|	cond_it_field it_field_list
	|	repeat_it_field it_field_list
	|
	;

repeat_it_field: '(' REPEAT NAME
	{
	  rdepth++;
	  switch (writecode)
	    {
	    case 'c':
	      if (rdepth == 1)
                printf("\tprintf(\"repeat %%d\\n\", %s);\n", $3);
	      if (rdepth == 2)
                printf("\tprintf(\"repeat %%d\\n\", %s[n]);\n", $3);
	      ATTRIBUTE_FALLTHROUGH;
	    case 'i':
	    case 'g':
	    case 'o':
	      if (rdepth == 1)
		{
                  printf("\t{ int n; for (n = 0; n < %s; n++) {\n",    $3);
                }
	      if (rdepth == 2) {
                printf("\t{ int m; for (m = 0; m < %s[n]; m++) {\n",   $3);
              }
	      break;
            default:;
	    }

	  oldrepeat = repeat;
         repeat = $3;
	}

	 it_field_list ')'

	{
	  repeat = oldrepeat;
	  oldrepeat =0;
	  rdepth--;
	  switch (writecode)
	    {
	    case 'i':
	    case 'g':
	    case 'o':
	    case 'c':
              printf("\t}\n\t}\n");
            default:;
            }
	}
       ;


cond_it_field: '(' COND NAME
	{
	  switch (writecode)
	    {
	    case 'i':
	    case 'g':
	    case 'o':
	    case 'c':
	      printf("\tif (%s) {\n", $3);
	      break;
            default:
              break;
	    }
	}

	 it_field_list ')'
	{
	  switch (writecode)
	    {
	    case 'i':
	    case 'g':
	    case 'o':
	    case 'c':
              printf("\t}\n");
            default:;
            }
	}
       ;

it_field:
	'(' attr_desc '(' attr_type attr_size ')' attr_id
	{name = $7; }
	enums ')'
	{
	  char *desc = $2;
	  char *type = $4;
	  int size = $5;
	  char *the_id = $7;
          const char *p = names[rdepth];
          const char *ptr = pnames[rdepth];
	  switch (writecode)
	    {
	    case 'g':
	      if (size % 8)
		{
		  printf("\twriteBITS(ptr->%s%s, raw, &idx, %d);\n",
			 the_id, names[rdepth], size);
		}
	      else
                {
                  printf("\twrite%s(ptr->%s%s, raw, &idx, %d, file);\n",
                         type, the_id, names[rdepth], (size / 8));
		}
	      break;
	    case 'i':
	      {
		if (rdepth >= 1)
		  {
		    printf("\tif (!ptr->%s) ptr->%s = (%s*)xcalloc(%s, sizeof(ptr->%s[0]));\n",
			   the_id, the_id, type, repeat, the_id);
		  }

		if (rdepth == 2)
		  {
		    printf("\tif (!ptr->%s[n]) ptr->%s[n] = (%s**)xcalloc(%s[n], sizeof(ptr->%s[n][0]));\n",
			   the_id, the_id, type, repeat, the_id);
		  }
	      }

	      if (size % 8)
		{
		  printf("\tptr->%s%s = getBITS(raw, &idx, %d, size);\n",
			 the_id, names[rdepth], size);
		}
	      else
                {
                  printf("\tptr->%s%s = get%s(raw, &idx, %d, size);\n",
                         the_id, names[rdepth], type, (size / 8));
		}
	      break;
	    case 'o':
	      printf("\tput%s(raw, %d, %d, &idx, ptr->%s%s);\n",
                     type, (size / 8), (size % 8), the_id, names[rdepth]);
	      break;
	    case 'd':
	      if (repeat)
		printf("\t/* repeat %s */\n", repeat);

              if (type[0] == 'I') {
                printf("\tint %s%s; \t/* %s */\n", ptr, the_id, desc);
              } else if (type[0] =='C') {
                printf("\tchar %s*%s;\t /* %s */\n", ptr, the_id, desc);
              } else {
                printf("\tbarray %s%s;\t /* %s */\n", ptr, the_id, desc);
              }
              break;
            case 'c':
              printf("\ttabout();\n");
              printf("\tprintf(\"/*%-30s*/ ptr->%s = \");\n", desc, the_id);

              if (type[0] == 'I') {
                printf("\tprintf(\"%%d\\n\", ptr->%s%s);\n", the_id, p);
              } else if (type[0] == 'C') {
                printf("\tprintf(\"%%s\\n\", ptr->%s%s);\n", the_id, p);
              } else if (type[0] == 'B') {
                printf("\tpbarray(&ptr->%s%s);\n", the_id, p);
              } else {
                abort();
              }
              break;
            default:
              break;
            }
	}

	;


attr_type:
	 TYPE { $$ = $1; }
 	|  { $$ = (char *)"INT";}
	;

attr_desc:
	'(' NAME ')'
	{ $$ = $2; }
	;

attr_size:
	 NUMBER UNIT
	{ $$ = $1 * $2; }
	;


attr_id:
		'(' NAME ')'	{ $$ = $2; }
	|	{ $$ = (char *)"dummy";}
	;

enums:
	| '(' enum_list ')' ;

enum_list:
	|
	enum_list '(' NAME NAME ')' {
	  switch (writecode)
	    {
	    case 'd':
	      printf("#define %s %s\n", $3, $4);
	      break;
	    case 'c':
              printf("\tif (ptr->%s%s == %s) { tabout(); printf(\"%s\\n\");}\n",
                     name, names[rdepth], $4, $3);
            default:;
	    }
	}

	;



%%
/* four modes

   -d write structure definitions for sysroff in host format
   -i write functions to swap into sysroff format in
   -o write functions to swap into sysroff format out
   -c write code to print info in human form */

/* FIXME: depends on whether or not we pass -t flag to bison when yaccing: */
int yydebug;

int
main(int ac, char **av)
{
  yydebug = 0;
  if (ac > 1) {
    writecode = av[1][1];
  }
  if (writecode == 'd')
    {
      printf("typedef struct { unsigned char *data; int len; } barray; \n");
      printf("typedef int INT;\n");
      printf("typedef char *CHARS;\n");
      printf("\n/* End text from main() in sysinfo.y */");
    }
  yyparse();
  return 0;
}

/* */
static int
yyerror(const char *s)
{
  fprintf(stderr, "%s\n", s);
  return 0;
}

/* End of sysinfo.y */
