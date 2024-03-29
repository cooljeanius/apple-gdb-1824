/* **************************************************************** */
/*								    */
/*			Testing Readline			    */
/*								    */
/* **************************************************************** */

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
#  warning rltest.c expects "config.h" to be included.
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_STDIO_H
# include <stdio.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "rltest.c expects <stdio.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_STDIO_H */
#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "rltest.c expects <stdlib.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_STDLIB_H */
#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "rltest.c expects <sys/types.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_SYS_TYPES_H */

#ifdef READLINE_LIBRARY
# include "readline.h"
# include "history.h"
#else
# include <readline/readline.h>
# include <readline/history.h>
#endif /* READLINE_LIBRARY */

extern HIST_ENTRY **history_list(void);

int main(void)
{
  char *temp;
  const char *prompt;
  int done;

  temp = (char *)NULL;
  prompt = "readline$ ";
  done = 0;

  while (!done)
    {
      temp = readline(prompt);

      /* Test for EOF. */
      if (!temp)
	exit(1);

      /* If there is anything on the line, print it and remember it. */
      if (*temp)
	{
	  fprintf(stderr, "%s\r\n", temp);
	  add_history(temp);
	}

      /* Check for `command' that we handle. */
      if (strcmp(temp, "quit") == 0)
	done = 1;

      if (strcmp(temp, "list") == 0)
	{
	  HIST_ENTRY **list;
	  register int i;

	  list = history_list();
	  if (list)
	    {
	      for (i = 0; list[i]; i++)
		fprintf(stderr, "%d: %s\r\n", i, list[i]->line);
	    }
	}
      free(temp);
    }
  exit(0);
}

/* EOF */
