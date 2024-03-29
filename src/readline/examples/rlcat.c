/*
 * rlcat - cat(1) using readline
 *
 * usage: rlcat
 */

/* Copyright (C) 1987-2002 Free Software Foundation, Inc.

   This file is part of the GNU Readline Library, a library for
   reading lines of text with interactive input and history editing.

   The GNU Readline Library is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.

   The GNU Readline Library is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   The GNU General Public License is often shipped with GNU software, and
   is generally kept in a file called COPYING or LICENSE.  If you do not
   have a copy of the license, write to the Free Software Foundation,
   59 Temple Place, Suite 330, Boston, MA 02111 USA. */

#if defined(HAVE_CONFIG_H)
# include "config.h"
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning rlcat.c expects "config.h" to be included.
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_UNISTD_H
# include <unistd.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "rlcat.c expects unistd.h to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_UNISTD_H */

#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "rlcat.c expects <sys/types.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_SYS_TYPES_H */
#include "posixstat.h"

#ifdef HAVE_STDIO_H
# include <stdio.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "rlcat.c expects <stdio.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_STDIO_H */
#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "rlcat.c expects <stdlib.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_STDLIB_H */
#include <ctype.h>
#if defined(HAVE_STRING_H)
# include <string.h>
#else /* !HAVE_STRING_H */
# if defined(HAVE_STRINGS_H)
#  include <strings.h>
# else
#  if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#   warning "rlcat.c expects either string.h or strings.h to be included."
#  endif /* __GNUC__ && !__STRICT_ANSI__ */
# endif /* HAVE_STRINGS_H */
#endif /* !HAVE_STRING_H */
#include <errno.h>

#ifndef errno
extern int errno;
#endif /* !errno */

#if defined(READLINE_LIBRARY)
# include "readline.h"
# include "history.h"
#else
# include <readline/readline.h>
# include <readline/history.h>
#endif /* READLINE_LIBRARY */

extern int optind;
extern char *optarg;

static int stdcat(int, char **);

static char *progname;
static int vflag;

/* */
static void
usage(void)
{
  fprintf(stderr, "%s: usage: %s [-vEVN] [filename]\n",
          progname, progname);
}

/* */
int
main(int argc, char **argv)
{
  char *temp;
  int opt, Vflag, Nflag;

  progname = strrchr(argv[0], '/');
  if (progname == 0)
    progname = argv[0];
  else
    progname++;

  vflag = Vflag = Nflag = 0;
  while ((opt = getopt(argc, argv, "vEVN")) != EOF)
    {
      switch (opt)
	{
	case 'v':
	  vflag = 1;
	  break;
	case 'V':
	  Vflag = 1;
	  break;
	case 'E':
	  Vflag = 0;
	  break;
	case 'N':
	  Nflag = 1;
	  break;
	default:
	  usage();
	  exit(2);
	}
    }

  argc -= optind;
  argv += optind;

  if ((isatty(0) == 0) || argc || Nflag)
    return stdcat(argc, argv);

  rl_variable_bind("editing-mode", Vflag ? "vi" : "emacs");
  while ((temp = readline("")))
    {
      if (*temp)
        add_history(temp);
      printf("%s\n", temp);
    }

  return (ferror(stdout));
}

/* */
static int
fcopy(FILE *fp)
{
  int c;
  char *x;

  while ((c = getc(fp)) != EOF)
    {
      if (vflag && isascii((unsigned char)c) && isprint((unsigned char)c) == 0)
	{
	  x = rl_untranslate_keyseq(c);
	  if (fputs(x, stdout) != 0)
	    return 1;
	}
      else if (putchar(c) == EOF)
        return 1;
    }
  return (ferror(stdout));
}

/* */
int
stdcat(int argc, char **argv)
{
  int  i, r;
  FILE *fp;

  if (argc == 0)
    return (fcopy(stdin));

  for (i = 0, r = 1; i < argc; i++)
    {
      if ((*argv[i] == '-') && (argv[i][1] == 0))
	fp = stdin;
      else
	{
	  fp = fopen(argv[i], "r");
	  if (fp == 0)
	    {
	      fprintf(stderr, "%s: %s: cannot open: %s\n",
                      progname, argv[i], strerror(errno));
	      continue;
	    }
        }
      r = fcopy(fp);
      if (fp != stdin)
	fclose(fp);
    }
  return r;
}

/* EOF */
