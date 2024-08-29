/* chew.c: chew
 * Copyright 1990, 1991, 1992, 1993, 1994, 1995, 1996, 1998, 2000, 2001,
 * 2002, 2003, 2005
 * Free Software Foundation, Inc.
 * Contributed by steve chamberlain @cygnus */
/*
This file is part of BFD, the Binary File Descriptor library.

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
Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.
 */

/* Yet another way of extracting documentation from source. No, I have NOT
 * finished it yet, but I hope you people like it better than the old way...
 *
 * sac
 *
 * Basically, this is a sort of string forth, maybe we should call it struth?
 *
 * You define new words thus:
 * : <newword> <oldwords> ;
 */

/* Primitives provided by the program:
 *
 * Two stacks are provided, a string stack and an integer stack.
 *
 * Internal state variables:
 *	internal_wanted - indicates whether `-i' was passed
 *	internal_mode - user-settable
 *
 * Commands:
 *	push_text
 *	! - pop top of integer stack for address, pop next for value; store
 *	@ - treat value on integer stack as the address of an integer; push
 *		that integer on the integer stack after popping the "address"
 *	hello - print "hello\n" to stdout
 *	stdout - put stdout marker on TOS
 *	stderr - put stderr marker on TOS
 *	print - print TOS-1 on TOS (eg: "hello\n" stdout print)
 *	skip_past_newline
 *	catstr - fn icatstr
 *	copy_past_newline - append input, up to and including newline into TOS
 *	dup - fn other_dup
 *	drop - discard TOS
 *	idrop - ditto
 *	remchar - delete last character from TOS
 *	get_stuff_in_command
 *	do_fancy_stuff - translate <<foo>> to @code{foo} in TOS
 *	bulletize - if "o" lines found, prepend @itemize @bullet to TOS
 *		and @item to each "o" line; append @end itemize
 *	courierize - put @example around . and | lines, translate {* *} { }
 *	exit - fn chew_exit
 *	swap
 *	outputdots - strip out lines without leading dots
 *	paramstuff - convert full declaration into "PARAMS" form if not already
 *	maybecatstr - do catstr if internal_mode == internal_wanted, discard
 *		value in any case
 *	translatecomments - turn {* and *} into comment delimiters
 *	kill_bogus_lines - get rid of extra newlines
 *	indent
 *	internalmode - pop from integer stack, set `internalmode' to that value
 *	print_stack_level - print current stack depth to stderr
 *	strip_trailing_newlines - go ahead, guess...
 *	[quoted string] - push string onto string stack
 *	[word starting with digit] - push atol(str) onto integer stack
 *
 * A command must be all upper-case, and alone on a line.
 *
 * Foo.
 */

/* includes: */
#include "ansidecl.h"
#include <assert.h>
/* hack: */
#if !defined(__STDC__)
# ifndef _OFF_T
#  define _OFF_T 1
# endif /* !_OFF_T */
#endif /* !__STDC__ */
#include <stdio.h>
#include <ctype.h>
/* hacks to allow compilation with '-traditional-cpp': */
#if !defined(__STDC__)
# ifndef _AVAILABLE_H_
#  define _AVAILABLE_H_ 1
# endif /* !_AVAILABLE_H_ */
# ifndef _ANSI_SOURCE
#  define _ANSI_SOURCE 1
# endif /* !_ANSI_SOURCE */
#endif /* !__STDC__ */
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* defines: */
#ifndef DEF_SIZE
# define DEF_SIZE 5000
#endif /* !DEF_SIZE */
#ifndef STACK
# define STACK 50
#endif /* !STACK */

/* global variables: */
int internal_wanted;
int internal_mode;

int warning;

/* Here is a string type: */
typedef struct buffer
{
  char *ptr;
  unsigned long write_idx;
  unsigned long size;
} string_type;

/* prototypes: */
#if defined(__STDC__) && __STDC__
static void init_string_with_size(string_type *, unsigned int);
static void init_string(string_type *);
static int find(string_type *, const char *);
static void write_buffer(string_type *, FILE *);
static void delete_string(string_type *);
static char *addr(string_type *, unsigned int);
static char at(string_type *, unsigned int);
static void catchar(string_type *, int);
static void overwrite_string(string_type *, string_type *);
static void catbuf(string_type *, const char *, unsigned int);
static void cattext(string_type *, const char *);
static void catstr(string_type *, string_type *);
#endif /* __STDC__ */

/* functions: */
static void init_string_with_size(string_type *buffer, unsigned int size)
{
  buffer->write_idx = 0;
  buffer->size = size;
  buffer->ptr = (char *)malloc((size_t)size);
}

static void init_string(string_type *buffer)
{
  init_string_with_size(buffer, DEF_SIZE);
}

static int find(string_type *str, const char *what)
{
  unsigned int i;
  const char *p;
  p = what;
  for ((i = 0); (i < str->write_idx) && *p; i++) {
      if (*p == str->ptr[i]) {
	  p++;
      } else {
	  p = what;
      }
  }
  return (*p == 0);
}

static void write_buffer(string_type *buffer, FILE *f)
{
  fwrite(buffer->ptr, buffer->write_idx, (size_t)1, f);
}

static void delete_string(string_type *buffer)
{
  free(buffer->ptr);
}

static char *addr(string_type *buffer, unsigned int idx)
{
  return (buffer->ptr + idx);
}

static char at(string_type *buffer, unsigned int pos)
{
  if (pos >= buffer->write_idx) {
      return 0;
  }
  return buffer->ptr[pos];
}

static void catchar(string_type *buffer, int ch)
{
  if (buffer->write_idx == buffer->size) {
      buffer->size *= 2;
      buffer->ptr = (char *)realloc(buffer->ptr, buffer->size);
  }

  buffer->ptr[buffer->write_idx++] = (char)ch;
}

static void overwrite_string(string_type *dst, string_type *src)
{
  free(dst->ptr);
  dst->size = src->size;
  dst->write_idx = src->write_idx;
  dst->ptr = src->ptr;
}

static void catbuf(string_type *buffer, const char *buf, unsigned int len)
{
  if ((buffer->write_idx + len) >= buffer->size) {
      while ((buffer->write_idx + len) >= buffer->size) {
	  buffer->size *= 2;
      }
      buffer->ptr = (char *)realloc(buffer->ptr, buffer->size);
  }
  memcpy((buffer->ptr + buffer->write_idx), buf, (size_t)len);
  buffer->write_idx += len;
}

static void cattext(string_type *buffer, const char *string)
{
  catbuf(buffer, string, (unsigned int)strlen(string));
}

static void catstr(string_type *dst, string_type *src)
{
  catbuf(dst, src->ptr, (unsigned int)src->write_idx);
}

static unsigned int skip_white_and_stars(string_type *src,
                                         unsigned int idx)
{
  char c;
  while ((c = at(src, idx)),
	 isspace((unsigned char)c)
	 || ((c == '*')
	     /* Do NOT skip past end-of-comment/star as 1st char on its line: */
	     && (at(src, idx +1) != '/')
	     && (at(src, idx -1) != '\n'))) {
      idx++;
  }
  return idx;
}

/*************************************************************************/
/* some more global variables: */
string_type stack[STACK];
string_type *tos;

unsigned int idx = 0; /* Position in input buffer... */
string_type *ptr; /* ...and in the (other?) buffer */
typedef void(*stinst_type)(void);
stinst_type *pc;
stinst_type sstack[STACK];
stinst_type *ssp = &sstack[0];
long istack[STACK];
long *isp = &istack[0];

typedef int *word_type;

struct dict_struct
{
  const char *word;
  struct dict_struct *next;
  stinst_type *code;
  int code_length;
  int code_end;
  int var;
#if defined(__LP64__)
  int padding;
#endif /* __LP64__ */
};

typedef struct dict_struct dict_type;

/* back to functions: */
static void die(const char *msg)
{
  fprintf(stderr, "%s\n", msg);
  exit(1);
}

static void check_range(void)
{
  if (tos < stack) {
      die("underflow in string stack");
  }
  if (tos >= (stack + STACK)) {
      die("overflow in string stack");
  }
}

static void icheck_range(void)
{
  if (isp < istack) {
      die("underflow in integer stack");
  }
  if (isp >= (istack + STACK)) {
      die("overflow in integer stack");
  }
}

/* some more prototypes: */
#if defined(__STDC__) && __STDC__
static void exec(dict_type *);
static void call(void);
static void remchar(void), strip_trailing_newlines(void), push_number(void);
static void push_text(void);
static void remove_noncomments(string_type *, string_type *);
static void print_stack_level(void);
static void paramstuff(void), translatecomments(void);
static void outputdots(void), courierize(void), bulletize(void);
static void do_fancy_stuff(void);
static int iscommand(string_type *, unsigned int);
static int copy_past_newline(string_type *, unsigned int, string_type *);
static void icopy_past_newline(void), kill_bogus_lines(void), indent(void);
static void get_stuff_in_command(void), swap(void), other_dup(void);
static void drop(void), idrop(void);
static void icatstr(void), skip_past_newline(void), internalmode(void);
static void maybecatstr(void);
static char *nextword(char *, char **);
static dict_type *lookup_word(char *);
static void perform(void);
static dict_type *newentry(const char *);
static unsigned int add_to_definition(dict_type *, stinst_type);
static void add_intrinsic(const char *, void (*)(void));
static void add_var(char *);
static void compile(char *);
static void bang(void);
static void atsign(void);
static void hello(void);
static void stdout_(void);
static void stderr_(void);
static void print(void);
static void read_in(string_type *, FILE *);
static void usage(void);
static void chew_exit(void);
#endif /* __STDC__ */

/* back to functions again: */
static void exec(dict_type *word)
{
  pc = word->code;
  while (*pc) {
      (*pc)();
  }
}

static void call(void)
{
  stinst_type *oldpc = pc;
  dict_type *e;
  e = (dict_type *)(pc[1]);
  exec(e);
  pc = (oldpc + 2);
}

static void remchar(void)
{
  if (tos->write_idx) {
      tos->write_idx--;
  }
  pc++;
}

static void strip_trailing_newlines(void)
{
  while ((isspace((unsigned char)at(tos, (unsigned int)(tos->write_idx - 1)))
	  || (at(tos, (unsigned int)(tos->write_idx - 1)) == '\n'))
	 && (tos->write_idx > 0)) {
      tos->write_idx--;
  }
  pc++;
}

static void push_number(void)
{
  isp++;
  icheck_range();
  pc++;
  *isp = (long)(intptr_t)(*pc);
  pc++;
}

static void push_text(void)
{
  tos++;
  check_range();
  init_string(tos);
  pc++;
  cattext(tos, *((char **)pc));
  pc++;
}

/* This function removes everything not inside comments starting on the 1st char
 * of the line from the  string, also when copying comments, removes blank space
 * and leading *'s. Blank lines are turned into one blank line. */
static void remove_noncomments(string_type *src, string_type *dst)
{
  unsigned int local_idx = 0;

  while (at(src, local_idx)) {
      /* Now see if we have a comment at the start of the line: */
      if ((at(src, local_idx) == '\n')
	  && (at(src, (local_idx + 1)) == '/')
	  && (at(src, (local_idx + 2)) == '*')) {
	  local_idx += 3;

	  local_idx = skip_white_and_stars(src, local_idx);

	  /* Remove leading dot: */
	  if (at(src, local_idx) == '.') {
	      local_idx++;
	  }

	  /* Copy to the end of the line, or till the end of the comment: */
	  while (at(src, local_idx)) {
	      if (at(src, local_idx) == '\n') {
		  /* end of line, echo and scrape of leading blanks: */
		  if (at(src, (local_idx + 1)) == '\n') {
		      catchar(dst, '\n');
		  }
		  catchar(dst, '\n');
		  local_idx++;
		  local_idx = skip_white_and_stars(src, local_idx);
	      } else if ((at(src, local_idx) == '*')
			 && (at(src, (local_idx + 1)) == '/')) {
		  local_idx += 2;
		  cattext(dst, "\nENDDD\n");
		  break;
	      } else {
		  catchar(dst, at(src, local_idx));
		  local_idx++;
	      }
	  }
      } else {
	  local_idx++;
      }
  }
}

static void print_stack_level(void)
{
  /* two values being subtracted are pointers, so assuming that the value
   * to be printed should actually be of type 'ptrdiff_t'... */
  fprintf(stderr, "current string stack depth = %td, ", (tos - stack));
  fprintf(stderr, "current integer stack depth = %td\n", (isp - istack));
  pc++;
}

/* turn:
 *   foobar name(stuff);
 * into:
 *   foobar
 *   name PARAMS((stuff));
 * and a blank line.
 */
static void paramstuff(void)
{
  unsigned int openp;
  unsigned int fname;
  unsigned int local_idx;
  unsigned int len;
  string_type out;
#define NO_PARAMS 1

#ifdef NO_PARAMS
  int no_params_var = NO_PARAMS;
#else
  int no_params_var = 1;
#endif /* NO_PARAMS */
  init_string(&out);

  /* Make sure that it is not already param'd or proto'd: */
  if (no_params_var
      || find(tos, "PARAMS") || find(tos, "PROTO") || !find(tos, "(")) {
      catstr(&out, tos);
  } else {
      /* Find the open paren: */
      for ((openp = 0); ((at(tos, openp) != '(') && at(tos, openp)); openp++) {
	  ;
      }

      fname = openp;
      /* Step back to the fname: */
      fname--;
      while (fname && isspace((unsigned char)at(tos, fname))) {
	  fname--;
      }
      while (fname
	     && !isspace((unsigned char)at(tos, fname))
	     && (at(tos, fname) != '*')) {
	  fname--;
      }

      fname++;

      /* Output type, omitting trailing whitespace character(s), if any: */
      for ((len = fname); (0 < len); len--) {
	  if (!isspace((unsigned char)at(tos, (len - 1)))) {
	      break;
	  }
      }
      for ((local_idx = 0); (local_idx < len); local_idx++) {
	  catchar(&out, at(tos, local_idx));
      }

      cattext(&out, "\n"); /* Insert a newline between type and fnname */

      /* Output func name, omitting trailing whitespace character(s), if any: */
      for ((len = openp); (0 < len); len--) {
	  if (!isspace((unsigned char) at (tos, len - 1))) {
	      break;
	  }
      }
      for ((local_idx = fname); (local_idx < len); local_idx++) {
	  catchar(&out, at(tos, local_idx));
      }

      cattext(&out, " PARAMS (");

      for ((local_idx = openp); (at(tos, local_idx) && (at(tos, local_idx) != ';')); local_idx++) {
	  catchar(&out, at(tos, local_idx));
      }

      cattext(&out, ");\n\n");
  }
  overwrite_string(tos, &out);
  pc++;
}

/* turn '{*' and '*}' into comments: */
static void translatecomments(void)
{
  unsigned int local_idx = 0;
  string_type out;
  init_string(&out);

  while (at(tos, local_idx)) {
      if ((at(tos, local_idx) == '{') && (at(tos, (local_idx + 1)) == '*')) {
	  cattext(&out, "/*");
	  local_idx += 2;
      } else if ((at(tos, local_idx) == '*') && (at(tos, local_idx + 1) == '}')) {
	  cattext (&out, "*/");
	  local_idx += 2;
      } else {
	  catchar(&out, at(tos, local_idx));
	  local_idx++;
      }
  }

  overwrite_string(tos, &out);

  pc++;
}

/* Mod tos so that only lines with leading dots remain: */
static void outputdots(void)
{
  unsigned int local_idx = 0;
  string_type out;
  init_string (&out);

  while (at(tos, local_idx)) {
      if ((at(tos, local_idx) == '\n') && (at(tos, (local_idx + 1)) == '.')) {
	  char c;
	  local_idx += 2;

	  while ((c = at(tos, local_idx)) && (c != '\n')) {
	      if ((c == '{') && (at(tos, (local_idx + 1)) == '*')) {
		  cattext (&out, "/*");
		  local_idx += 2;
	      } else if ((c == '*') && (at(tos, (local_idx + 1)) == '}')) {
		  cattext (&out, "*/");
		  local_idx += 2;
	      } else {
		  catchar(&out, c);
		  local_idx++;
	      }
	  }
	  catchar(&out, '\n');
      } else {
	  local_idx++;
      }
  }

  overwrite_string(tos, &out);
  pc++;
}

/* Find lines starting with . and | and put example around them on tos: */
static void courierize(void)
{
  string_type out;
  unsigned int local_idx = 0;
  int command = 0;

  init_string (&out);

  while (at(tos, local_idx)) {
      /* parentheses here have gotten kind of confusing, make sure that the
       * logic is still correct here: */
      if ((at(tos, local_idx) == '\n')
	  && ((at(tos, (local_idx + 1)) == '.')
	      || (at(tos, (local_idx + 1)) == '|'))) {
	  cattext(&out, "\n@example\n");
	  do {
	      local_idx += 2;

	      while (at(tos, local_idx) && (at(tos, local_idx) != '\n')) {
		  if (command > 1) {
		      /* We are inside {} parameters of some command;
		       * Just pass through until matching brace: */
		      if (at (tos, local_idx) == '{') {
			  ++command;
		      } else if (at (tos, local_idx) == '}') {
			  --command;
		      }
		  } else if (command != 0) {
		      if (at(tos, local_idx) == '{') {
			  ++command;
		      } else if (!islower((unsigned char)at(tos, local_idx))) {
			  --command;
		      }
		  } else if ((at(tos, local_idx) == '@')
			     && islower((unsigned char)at(tos, (local_idx + 1)))) {
		      ++command;
		  } else if ((at(tos, local_idx) == '{')
			     && (at(tos, (local_idx + 1)) == '*')) {
		      cattext(&out, "/*");
		      local_idx += 2;
		      continue;
		  } else if ((at(tos, local_idx) == '*')
			     && (at(tos, local_idx + 1) == '}')) {
		      cattext(&out, "*/");
		      local_idx += 2;
		      continue;
		  } else if ((at(tos, local_idx) == '{')
			     || (at(tos, local_idx) == '}')) {
		      catchar(&out, '@');
		  }

		  catchar(&out, at(tos, local_idx));
		  local_idx++;
	      }
	      catchar(&out, '\n');
	  } while ((at(tos, local_idx) == '\n')
		   && ((at(tos, (local_idx + 1)) == '.')
		       || (at(tos, (local_idx + 1)) == '|')));
	  /* the above "while" ends a "do... while"-loop; it is NOT the start
	   * of its own "while"-loop */
	  cattext(&out, "@end example");
      } else {
	  catchar(&out, at(tos, local_idx));
	  local_idx++;
      }
  }

  overwrite_string(tos, &out);
  pc++;
}

/* Finds any lines starting with "o ", if there are any, then turns on
 * @itemize @bullet, and @items each of them. Then ends with @end itemize,
 * inplace at TOS: */
static void bulletize(void)
{
  unsigned int local_idx = 0;
  int on = 0;
  string_type out;
  init_string(&out);

  while (at(tos, local_idx)) {
      if ((at(tos, local_idx) == '@')
	  && (at(tos, (local_idx + 1)) == '*')) {
	  cattext(&out, "*");
	  local_idx += 2;
      } else if ((at(tos, local_idx) == '\n')
		 && (at(tos, (local_idx + 1)) == 'o')
		 && isspace((unsigned char)at(tos, (local_idx + 2)))) {
	  if (!on) {
	      cattext(&out, "\n@itemize @bullet\n");
	      on = 1;
	  }
	  cattext(&out, "\n@item\n");
	  local_idx += 3;
      } else {
	  catchar(&out, at(tos, local_idx));
	  if (on && (at(tos, local_idx) == '\n')
	      && (at(tos, (local_idx + 1)) == '\n')
	      && (at(tos, (local_idx + 2)) != 'o')) {
	      cattext(&out, "@end itemize");
	      on = 0;
	  }
	  local_idx++;
      }
  }
  if (on) {
      cattext(&out, "@end itemize\n");
  }

  delete_string(tos);
  *tos = out;
  pc++;
}

/* Turn <<foo>> into @code{foo} in place at TOS: */
static void do_fancy_stuff(void)
{
  unsigned int local_idx = 0;
  string_type out;
  init_string (&out);
  while (at(tos, local_idx)) {
      if ((at(tos, local_idx) == '<')
	  && (at(tos, (local_idx + 1)) == '<')
	  && !isspace((unsigned char)at(tos, (local_idx + 2)))) {
	  /* This qualifies as a << startup: */
	  local_idx += 2;
	  cattext (&out, "@code{");
	  while (at(tos, local_idx)
		 && (at(tos, local_idx) != '>')) {
	      catchar(&out, at(tos, local_idx));
	      local_idx++;
	  }
	  cattext(&out, "}");
	  local_idx += 2;
      } else {
	  catchar(&out, at(tos, local_idx));
	  local_idx++;
      }
  }
  delete_string(tos);
  *tos = out;
  pc++;
}

/* A command is all upper case, and alone on a line: */
static int iscommand(string_type *local_ptr, unsigned int local_idx)
{
  unsigned int len = 0;
  while (at(local_ptr, local_idx)) {
      if (isupper((unsigned char)at(local_ptr, local_idx))
	  || (at(local_ptr, local_idx) == ' ')
	  || (at(local_ptr, local_idx) == '_')) {
	  len++;
	  local_idx++;
      } else if (at(local_ptr, local_idx) == '\n') {
	  if (len > 3) {
	      return 1;
	  }
	  return 0;
      } else {
	  return 0;
      }
  }
  return 0;
}

static int copy_past_newline(string_type *local_ptr,
                             unsigned int local_idx, string_type *dst)
{
  int column = 0;

  while (at(local_ptr, local_idx) && (at(local_ptr, local_idx) != '\n')) {
      if (at(local_ptr, local_idx) == '\t') {
	  /* Expand tabs. Neither makeinfo nor TeX can cope well with them: */
	  do {
	      catchar(dst, ' ');
	  } while (++column & 7);
      } else {
	  catchar(dst, at(local_ptr, local_idx));
	  column++;
      }
      local_idx++;
  }
  catchar(dst, at(local_ptr, local_idx));
  local_idx++;
  return local_idx;
}

static void icopy_past_newline(void)
{
  tos++;
  check_range();
  init_string(tos);
  idx = copy_past_newline(ptr, idx, tos);
  pc++;
}

/* indent
 * Take the string at the top of the stack, do some prettying: */
static void kill_bogus_lines(void)
{
  int sl;

  int local_idx = 0;
  int c;
  int dot = 0;

  string_type out;
  init_string(&out);
  /* Drop leading nl: */
  while (at(tos, (unsigned int)local_idx) == '\n') {
      local_idx++;
  }
  c = local_idx;

  /* If the 1st char is a '.' prepend a newline so that it is recognized
   * properly later: */
  if (at(tos, (unsigned int)local_idx) == '.') {
      catchar(&out, '\n');
  }

  /* Find the last char: */
  while (at(tos, (unsigned int)local_idx)) {
      local_idx++;
  }

  /* Find the last non white before the nl: */
  local_idx--;

  while (local_idx && isspace((unsigned char)at(tos, (unsigned)local_idx))) {
      local_idx--;
  }
  local_idx++;

  /* Copy buffer up to last char, but blank lines before and after dots
   * do NOT count. */
  sl = 1;

  while (c < local_idx) {
      if ((at(tos, (unsigned int)c) == '\n')
	  && (at(tos, (unsigned int)(c + 1)) == '\n')
	  && (at(tos, (unsigned int)(c + 2)) == '.')) {
	  /* Ignore two newlines before a dot. */
	  c++;
      } else if ((at(tos, (unsigned int)c) == '.') && sl) {
	  /* remember that this line started with a dot. */
	  dot = 2;
      } else if ((at(tos, (unsigned int)c) == '\n')
		 && (at(tos, (unsigned int)(c + 1)) == '\n')
		 && dot) {
	  c++;
	  /* Ignore two newlines when last line was dot. */
      }

      catchar(&out, at(tos, (unsigned int)c));
      if (at(tos, (unsigned int)c) == '\n') {
	  sl = 1;

	  if (dot == 2) {
	      dot = 1;
	  } else {
	      dot = 0;
	  }
      } else {
	  sl = 0;
      }

      c++;
  }

  /* Append nl: */
  catchar(&out, '\n');
  pc++;
  delete_string(tos);
  *tos = out;

}

static void indent(void)
{
  string_type out;
  int tab = 0;
  int local_idx = 0;
  int ol = 0;
  init_string (&out);
  while (at(tos, (unsigned int)local_idx)) {
      switch (at(tos, (unsigned int)local_idx)) {
	case '\n':
	  cattext(&out, "\n");
	  local_idx++;
	  if (tab && at(tos, (unsigned int)local_idx)) {
	      cattext(&out, "    ");
	  }
	  ol = 0;
	  break;
	case '(':
	  tab++;
	  if (ol == 0) {
	      cattext(&out, "   ");
	  }
	  local_idx++;
	  cattext(&out, "(");
	  ol = 1;
	  break;
	case ')':
	  tab--;
	  cattext(&out, ")");
	  local_idx++;
	  ol = 1;

	  break;
	default:
	  catchar(&out, at(tos, (unsigned int)local_idx));
	  ol = 1;

	  local_idx++;
	  break;
      }
  }

  pc++;
  delete_string(tos);
  *tos = out;

}

static void get_stuff_in_command(void)
{
  tos++;
  check_range();
  init_string(tos);

  while (at(ptr, idx)) {
      if (iscommand(ptr, idx)) {
	  break;
      }
      idx = copy_past_newline(ptr, idx, tos);
  }
  pc++;
}

static void swap(void)
{
  string_type t;

  t = tos[0];
  tos[0] = tos[-1];
  tos[-1] = t;
  pc++;
}

static void other_dup(void)
{
  tos++;
  check_range();
  init_string(tos);
  catstr(tos, (tos - 1));
  pc++;
}

static void drop(void)
{
  tos--;
  check_range();
  pc++;
}

static void idrop(void)
{
  isp--;
  icheck_range();
  pc++;
}

static void icatstr(void)
{
  tos--;
  check_range();
  catstr(tos, (tos + 1));
  delete_string((tos + 1));
  pc++;
}

static void skip_past_newline(void)
{
  while (at(ptr, idx)
	 && (at(ptr, idx) != '\n')) {
      idx++;
  }
  idx++;
  pc++;
}

static void internalmode(void)
{
  internal_mode = (int)*(isp);
  isp--;
  icheck_range();
  pc++;
}

static void maybecatstr(void)
{
  if (internal_wanted == internal_mode) {
      catstr((tos - 1), tos);
  }
  delete_string(tos);
  tos--;
  check_range();
  pc++;
}

static char *nextword(char *string, char **word)
{
  char *word_start;
  int local_idx;
  char *dst;
  char *src;

  int length = 0;

  while (isspace((unsigned char)*string) || (*string == '-')) {
      if (*string == '-') {
	  while (*string && (*string != '\n')) {
	      string++;
	  }
      } else {
	  string++;
      }
  }
  if (!*string) {
      return 0;
  }

  word_start = string;
  if (*string == '"') {
      do {
	  string++;
	  length++;
	  if (*string == '\\') {
	      string += 2;
	      length += 2;
	  }
      } while (*string != '"');
  } else {
      while (!isspace((unsigned char)*string)) {
	  string++;
	  length++;
      }
  }

  *word = (char *)malloc((size_t)(length + 1));

  dst = *word;
  src = word_start;

  for ((local_idx = 0); (local_idx < length); local_idx++) {
      if (src[local_idx] == '\\') {
	switch (src[(local_idx + 1)]) {
	  case 'n':
	    *dst++ = '\n';
	    local_idx++;
	    break;
	  case '"':
	  case '\\':
	    *dst++ = src[(local_idx + 1)];
	    local_idx++;
	    break;
	  default:
	    *dst++ = '\\';
	    break;
	}
      } else {
	*dst++ = src[local_idx];
      }
  }
  *dst++ = 0;

  /* dummy condition to use value stored to 'dst': */
  if (dst == NULL) {
      ;
  }

  if (*string) {
      return (string + 1);
  } else {
      return 0;
  }
}

dict_type *root;

static dict_type *lookup_word(char *word)
{
  dict_type *local_ptr;
  local_ptr = root;
  while (local_ptr) {
      if (strcmp(local_ptr->word, word) == 0) {
	  return local_ptr;
      }
      local_ptr = local_ptr->next;
  }
  if (warning) {
      fprintf(stderr, "Cannot find %s\n", word);
  }
  return 0;
}

static void perform(void)
{
  tos = stack;

  while (at(ptr, idx)) {
      /* It is worth looking through the command list: */
      if (iscommand(ptr, idx)) {
	  char *next;
	  dict_type *word;

	  (void)nextword(addr(ptr, idx), &next);

	  word = lookup_word(next);

	  if (word) {
	      exec(word);
	  } else {
	      if (warning) {
		  fprintf(stderr, "warning, %s is not recognized\n", next);
	      }
	      skip_past_newline();
	  }
      } else {
	  skip_past_newline();
      }
  }
}

static dict_type *newentry(const char *word)
{
  dict_type *newdict = (dict_type *)malloc(sizeof(dict_type));
  newdict->word = word;
  newdict->next = root;
  root = newdict;
  newdict->code = (stinst_type *)malloc(sizeof(stinst_type));
  newdict->code_length = 1;
  newdict->code_end = 0;
  return newdict;
}

static unsigned int add_to_definition(dict_type *entry, stinst_type word)
{
  if (entry->code_end == entry->code_length) {
      entry->code_length += 2;
      entry->code =
	(stinst_type *)realloc((char *)(entry->code),
			       (entry->code_length * sizeof(word_type)));
  }
  entry->code[entry->code_end] = word;

  return entry->code_end++;
}

static void add_intrinsic(const char *name, void (*func)(void))
{
  dict_type *new = newentry(name);
  add_to_definition(new, func);
  add_to_definition(new, 0);
}

static void add_var(char *name)
{
  dict_type *new = newentry(name);
  add_to_definition(new, push_number);
  add_to_definition(new, (stinst_type)(&(new->var)));
  add_to_definition(new, 0);
}

static void compile(char *string)
{
  /* Add words to the dictionary: */
  char *word;
  string = nextword(string, &word);
  while (string && *string && word[0]) {
      if (strcmp(word, "var") == 0) {
	  string = nextword(string, &word);

	  add_var(word);
	  string = nextword(string, &word);
      } else if (word[0] == ':') {
	  dict_type *local_ptr;
	  /* Compile a word and add to dictionary: */
	  string = nextword(string, &word);

	  local_ptr = newentry(word);
	  string = nextword(string, &word);
	  while (word[0] != ';') {
	      switch (word[0]) {
		case '"':
		  /* got a string, embed magic push string function: */
		  add_to_definition(local_ptr, push_text);
		  add_to_definition(local_ptr, (stinst_type)(word + 1));
		  break;
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
		  /* Got a number, embed the magic push number function: */
		  add_to_definition(local_ptr, push_number);
		  add_to_definition(local_ptr,
                                    (stinst_type)(intptr_t)atol(word));
		  break;
		default:
		  add_to_definition(local_ptr, call);
		  add_to_definition(local_ptr, (stinst_type)lookup_word(word));
	      } /* end "switch (word[0])" */

	      string = nextword(string, &word);
	  } /* end while-loop */
	  add_to_definition(local_ptr, 0);
	  string = nextword(string, &word);
      } else {
	  fprintf(stderr, "syntax error at %s\n", (string - 1));
      }
  }
}

static void bang(void)
{
  *(long *)(intptr_t)((isp[0])) = isp[-1];
  isp -= 2;
  icheck_range();
  pc++;
}

static void atsign(void)
{
  isp[0] = *(long *)(intptr_t)(isp[0]);
  pc++;
}

static void hello(void)
{
  printf("hello\n");
  pc++;
}

static void stdout_(void)
{
  isp++;
  icheck_range();
  *isp = 1;
  pc++;
}

static void stderr_(void)
{
  isp++;
  icheck_range();
  *isp = 2;
  pc++;
}

static void print(void)
{
  if (*isp == 1) {
      write_buffer(tos, stdout);
  } else if (*isp == 2) {
      write_buffer(tos, stderr);
  } else {
      fprintf(stderr, "print: illegal print destination `%ld'\n", *isp);
  }
  isp--;
  tos--;
  icheck_range();
  check_range();
  pc++;
}

static void read_in(string_type *str, FILE *file)
{
  char buff[10000];
  unsigned int r;
  if (file == NULL) {
      fprintf(stderr, "read_in(): file to read is null\n");
  }
  do {
      /* FIXME: hangs on this: */
      r = (unsigned int)fread(buff, (size_t)1, sizeof(buff), file);
      /* (seem to have fixed?) */
#ifdef PARANOID
      if (feof(file)) {
	  fprintf(stderr, "read_in(): end of file reached\n");
	  break;
      }
#endif /* PARANOID */
      if (ferror(file)) {
	  fprintf(stderr, "read_in(): error reading file\n");
	  break;
      }
      catbuf(str, buff, r);
  } while (r && ((int)r != EOF));
  buff[0] = 0;

  catbuf(str, buff, 1);
}

static void usage(void)
{
  fprintf(stderr, "usage: -[d|i|g] <file >file\n");
  exit(33);
}

/* There is no reliable way to declare exit. Sometimes it returns int,
 * and sometimes it returns void. Sometimes it changes between OS releases.
 * Trying to get it declared correctly in the hosts file is a pointless waste
 * of time. */
static void chew_exit(void)
{
  exit(0);
}

int main(int ac, char *av[])
{
  unsigned int i;
  string_type buffer;
  string_type pptr;

  init_string(&buffer);
  init_string(&pptr);
  init_string(stack + 0);
  tos = (stack + 1);
  ptr = &pptr;

  add_intrinsic("push_text", push_text);
  add_intrinsic("!", bang);
  add_intrinsic("@", atsign);
  add_intrinsic("hello", hello);
  add_intrinsic("stdout", stdout_);
  add_intrinsic("stderr", stderr_);
  add_intrinsic("print", print);
  add_intrinsic("skip_past_newline", skip_past_newline);
  add_intrinsic("catstr", icatstr);
  add_intrinsic("copy_past_newline", icopy_past_newline);
  add_intrinsic("dup", other_dup);
  add_intrinsic("drop", drop);
  add_intrinsic("idrop", idrop);
  add_intrinsic("remchar", remchar);
  add_intrinsic("get_stuff_in_command", get_stuff_in_command);
  add_intrinsic("do_fancy_stuff", do_fancy_stuff);
  add_intrinsic("bulletize", bulletize);
  add_intrinsic("courierize", courierize);
  /* If the following line gives an error, exit() is not declared in the
   * "../hosts/foo.h" file for this host. Fix it there, not here!  */
  /* No, do NOT fix it anywhere; see comment on chew_exit--Ian Taylor.  */
  add_intrinsic("exit", chew_exit);
  add_intrinsic("swap", swap);
  add_intrinsic("outputdots", outputdots);
  add_intrinsic("paramstuff", paramstuff);
  add_intrinsic("maybecatstr", maybecatstr);
  add_intrinsic("translatecomments", translatecomments);
  add_intrinsic("kill_bogus_lines", kill_bogus_lines);
  add_intrinsic("indent", indent);
  add_intrinsic("internalmode", internalmode);
  add_intrinsic("print_stack_level", print_stack_level);
  add_intrinsic("strip_trailing_newlines", strip_trailing_newlines);

  /* Put a nl at the start: */
  catchar(&buffer, '\n');

  read_in(&buffer, stdin);
  remove_noncomments(&buffer, ptr);
  for ((i = 1); (i < (unsigned int)ac); i++) {
      if (av[i][0] == '-') {
	  if (av[i][1] == 'f') {
	      string_type b;
	      FILE *f;
	      init_string(&b);

	      f = fopen(av[(i + 1)], "r");
	      if (!f) {
		  fprintf(stderr, "Cannot open the input file %s\n",
			  av[(i + 1)]);
		  return 33;
	      }

	      read_in(&b, f);
	      compile(b.ptr);
	      perform();
	  } else if (av[i][1] == 'i') {
	      internal_wanted = 1;
	  } else if (av[i][1] == 'w') {
	      warning = 1;
	  } else {
	      usage();
	  }
      }
  }
  write_buffer((stack + 0), stdout);
  if (tos != stack) {
      fprintf(stderr, "finishing with current stack level %td\n",
	      (tos - stack));
      return 1;
  }
  return 0;
}

/* silence '-Wunused-macros' warnings: */
#ifdef _AVAILABLE_H_
# undef _AVAILABLE_H_
#endif /* _AVAILABLE_H_ */
#ifdef _ANSI_SOURCE
# undef _ANSI_SOURCE
#endif /* _ANSI_SOURCE */
#ifdef DEF_SIZE
# undef DEF_SIZE
#endif /* DEF_SIZE */
#ifdef STACK
# undef STACK
#endif /* STACK */

/* EOF */
