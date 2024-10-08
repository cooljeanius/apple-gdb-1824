/* Handle lists of commands, their decoding and documentation, for GDB.
   Copyright 1986, 1989, 1990, 1991, 1998 Free Software Foundation, Inc.

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
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

#include "defs.h"
#include "gdbcmd.h"
#include "symtab.h"
#include "value.h"
#include <ctype.h>
#include "gdb_string.h"
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif /* HAVE_UNISTD_H */

#ifdef HAVE_WAIT_H
# include <wait.h>
#else
# ifdef HAVE_SYS_WAIT_H
#  include <sys/wait.h>
# endif /* HAVE_SYS_WAIT_H */
#endif /* HAVE_WAIT_H */

#include "wait.h"
#include "top.h"

#include "cli/cli-decode.h"
#include "cli/cli-setshow.h"
#include "readline/tilde.h"
#include "tui/tui.h"

#ifndef GDB_FILE
# define GDB_FILE struct ui_file
#endif /* !GDB_FILE */

/* Prototypes for local functions */

static void undef_cmd_error PARAMS((const char *, const char *));

static void show_user PARAMS((const char *, int));

static void show_user_1 PARAMS((struct cmd_list_element *, GDB_FILE *));

static void make_command PARAMS((const char *, int));

static void shell_escape PARAMS((const char *, int));

static int parse_binary_operation PARAMS((const char *));

#if !defined(CLI_DECODE_H)
static void print_doc_line PARAMS((GDB_FILE *, const char *));
#endif /* !CLI_DECODE_H */

static struct cmd_list_element *find_cmd PARAMS((char *command, int len,
                                                 struct cmd_list_element *clist,
                                                 int ignore_help_classes,
                                                 int *nfound));

void _initialize_command PARAMS((void));

#ifndef STRCMP
# define STRCMP(foo, bar) strcmp(foo, bar)
#endif /* !STRCMP */

/* Add element named NAME.
   CLASS is the top level category into which commands are broken down
   for "help" purposes.
   FUN should be the function to execute the command;
   it will get a character string as argument, with leading
   and trailing blanks already eliminated.

   DOC is a documentation string for the command.
   Its first line should be a complete sentence.
   It should start with ? for a command that is an abbreviation
   or with * for a command that most users don't need to know about.

   Add this command to command list *LIST.  

   Returns a pointer to the added command (not necessarily the head 
   of *LIST). */
struct cmd_list_element *
add_cmd(const char *name, enum command_class cmdclass,
        void (*fun) PARAMS((const char *, int)), const char *doc,
        struct cmd_list_element **list)
{
  register struct cmd_list_element *c =
    (struct cmd_list_element *)xmalloc(sizeof(struct cmd_list_element));
  struct cmd_list_element *p;

  delete_cmd(name, list);

  if (*list == NULL || (STRCMP((*list)->name, name) >= 0))
    {
      c->next = *list;
      *list = c;
    }
  else
    {
      p = *list;
      while (p->next && (STRCMP(p->next->name, name) <= 0))
        {
          p = p->next;
        }
      c->next = p->next;
      p->next = c;
    }

  c->name = name;
  c->cmdclass = cmdclass;
  c->function.cfunc = fun;
  c->doc = doc;
#if 0
  c->hook = NULL;
#endif /* 0 */
  c->prefixlist = NULL;
  c->prefixname = NULL;
  c->allow_unknown = 0;
  c->abbrev_flag = 0;
  c->completer = make_symbol_completion_list;
#if 0
  c->completer_quote_characters = gdb_completer_quote_characters;
  c->completer_word_break_characters = gdb_completer_word_break_characters;
#endif /* 0 */
  c->type = not_set_cmd;
  c->var = NULL;
  c->var_type = var_boolean;
  c->enums = NULL;
  c->user_commands = NULL;
  c->hookee = NULL;
  c->cmd_pointer = NULL;

  return c;
}

/* Same as above, except that the abbrev_flag is set. */
#ifdef ALLOW_UNUSED_FUNCTIONS
struct cmd_list_element *
add_abbrev_cmd(char *name, enum command_class cmdclass,
               void (*fun) PARAMS((const char *, int)), char *doc,
               struct cmd_list_element **list)
{
  register struct cmd_list_element *c = add_cmd(name, cmdclass, fun, doc, list);

  c->abbrev_flag = 1;
  return c;
}
#endif /* ALLOW_UNUSED_FUNCTIONS */

struct cmd_list_element *
add_alias_cmd(const char *name, const char *oldname,
              enum command_class cmdclass, int abbrev_flag,
              struct cmd_list_element **list)
{
  /* Must do this since lookup_cmd tries to side-effect its first arg */
  const char *copied_name;
  register struct cmd_list_element *old;
  register struct cmd_list_element *c;
  copied_name = (const char *)alloca(strlen(oldname) + 1UL);
  strcpy((char *)copied_name, oldname);
  old = lookup_cmd(&copied_name, *list, "", 1, 1);

  if (old == 0)
    {
      delete_cmd(name, list);
      return 0;
    }

  c = add_cmd(name, cmdclass, old->function.cfunc, old->doc, list);
  c->prefixlist = old->prefixlist;
  c->prefixname = old->prefixname;
  c->allow_unknown = old->allow_unknown;
  c->abbrev_flag = abbrev_flag;
  c->cmd_pointer = old;
  return c;
}

/* Like add_cmd but adds an element for a command prefix:
   a name that should be followed by a subcommand to be looked up
   in another command list.  PREFIXLIST should be the address
   of the variable containing that list.  */
struct cmd_list_element *
add_prefix_cmd(const char *name, enum command_class cmdclass,
               void (*fun) PARAMS((const char *, int)), const char *doc,
               struct cmd_list_element **prefixlist, const char *prefixname,
               int allow_unknown, struct cmd_list_element **list)
{
  register struct cmd_list_element *c = add_cmd(name, cmdclass, fun, doc, list);
  c->prefixlist = prefixlist;
  c->prefixname = prefixname;
  c->allow_unknown = allow_unknown;
  return c;
}

/* Like add_prefix_cmd but sets the abbrev_flag on the new command. */
   
struct cmd_list_element *
add_abbrev_prefix_cmd(const char *name, enum command_class cmdclass,
                      void (*fun) PARAMS((const char *, int)), const char *doc,
                      struct cmd_list_element **prefixlist,
                      const char *prefixname, int allow_unknown,
                      struct cmd_list_element **list)
{
  register struct cmd_list_element *c = add_cmd(name, cmdclass, fun, doc, list);
  c->prefixlist = prefixlist;
  c->prefixname = prefixname;
  c->allow_unknown = allow_unknown;
  c->abbrev_flag = 1;
  return c;
}

/* This is an empty "cfunc".  */
void
not_just_help_class_command(const char *args, int from_tty)
{
  (void)args;
  (void)from_tty;
}

/* This is an empty "sfunc".  */
static void empty_sfunc PARAMS((const char *, int, struct cmd_list_element *));

static void
empty_sfunc(const char *args, int from_tty, struct cmd_list_element *c)
{
  (void)args;
  (void)from_tty;
  (void)c;
}

/* Add element named NAME to command list LIST (the list for set
   or some sublist thereof).
   CLASS is as in add_cmd.
   VAR_TYPE is the kind of thing we are setting.
   VAR is address of the variable being controlled by this command.
   DOC is the documentation string.  */
struct cmd_list_element *
add_set_cmd(const char *name, enum command_class cmdclass, var_types var_type,
	    char *var, const char *doc, struct cmd_list_element **list)
{
  struct cmd_list_element *c = add_cmd(name, cmdclass, NULL, doc, list);

  c->type = set_cmd;
  c->var_type = var_type;
  c->var = var;
  /* This needs to be something besides NO_FUNCTION so that this isn't
     treated as a help class.  */
  c->function.sfunc = empty_sfunc;
  return c;
}

/* Add element named NAME to command list LIST (the list for set
   or some sublist thereof).
   CLASS is as in add_cmd.
   ENUMLIST is a list of strings which may follow NAME.
   VAR is address of the variable which will contain the matching string
     (from ENUMLIST).
   DOC is the documentation string.  */
struct cmd_list_element *
add_set_enum_cmd(char *name, enum command_class cmdclass,
		 const char *enumlist[], char *var, char *doc,
   	  	 struct cmd_list_element **list)
{
  struct cmd_list_element *c
    = add_set_cmd(name, cmdclass, var_enum, var, doc, list);
  c->enums = enumlist;

  return c;
}

/* Where SETCMD has already been added, add the corresponding show
   command to LIST and return a pointer to the added command (not 
   necessarily the head of LIST).  */
struct cmd_list_element *
add_show_from_set(struct cmd_list_element *setcmd,
                  struct cmd_list_element **list)
{
  struct cmd_list_element *showcmd =
    (struct cmd_list_element *) xmalloc (sizeof (struct cmd_list_element));
  struct cmd_list_element *p;

  memcpy (showcmd, setcmd, sizeof (struct cmd_list_element));
  delete_cmd (showcmd->name, list);
  showcmd->type = show_cmd;
  
  /* Replace "set " at start of docstring with "show ".  */
  if (setcmd->doc[0] == 'S' && setcmd->doc[1] == 'e'
      && setcmd->doc[2] == 't' && setcmd->doc[3] == ' ')
    showcmd->doc = concat ("Show ", setcmd->doc + 4, NULL);
  else
    fprintf_unfiltered (gdb_stderr, "GDB internal error: Bad docstring for set command\n");
  
    if (*list == NULL || STRCMP ((*list)->name, showcmd->name) >= 0)
    {
      showcmd->next = *list;
      *list = showcmd;
    }
  else
    {
      p = *list;
      while (p->next && STRCMP (p->next->name, showcmd->name) <= 0)
        {
          p = p->next;
        }
      showcmd->next = p->next;
      p->next = showcmd;
    }

  return showcmd;
}

#ifndef STREQ
# ifdef DEPRECATED_STREQ
#  define STREQ(str1, str2) DEPRECATED_STREQ(str1, str2)
# endif /* DEPRECATED_STREQ */
#endif /* !STREQ */

/* Remove the command named NAME from the command list.  */
void
delete_cmd(const char *name, struct cmd_list_element **list)
{
  register struct cmd_list_element *c;
  struct cmd_list_element *p;

  while (*list && STREQ ((*list)->name, name))
    {
      if ((*list)->hookee)
	(*list)->hookee->hook = 0;	/* Hook slips out of its mouth */
      p = (*list)->next;
      free ((PTR)*list);
      *list = p;
    }

  if (*list)
    for (c = *list; c->next;)
      {
	if (STREQ (c->next->name, name))
	  {
	    if (c->next->hookee)
	      c->next->hookee->hook = 0;  /* hooked cmd gets away.  */
	    p = c->next->next;
	    free ((PTR)c->next);
	    c->next = p;
	  }
	else
	  c = c->next;
      }
}

extern struct cmd_list_element *cmdlist;

/* This command really has to deal with two things:
 *     1) I want documentation on *this string* (usually called by
 * "help commandname").
 *     2) I want documentation on *this list* (usually called by
 * giving a command that requires subcommands.  Also called by saying
 * just "help".)
 *
 *   I am going to split this into two seperate comamnds, help_cmd and
 * help_list. 
 */
void
help_cmd(const char *command, GDB_FILE *stream)
{
  struct cmd_list_element *c;

  if (!command)
    {
      help_list (cmdlist, "", all_classes, stream);
      return;
    }

  c = lookup_cmd (&command, cmdlist, "", 0, 0);

  if (c == 0)
    return;

  /* There are three cases here.
     If c->prefixlist is nonzero, we have a prefix command.
     Print its documentation, then list its subcommands.
     
     If c->function is nonzero, we really have a command.
     Print its documentation and return.
     
     If c->function is zero, we have a class name.
     Print its documentation (as if it were a command)
     and then set class to the number of this class
     so that the commands in the class will be listed.  */

  fputs_filtered (c->doc, stream);
  fputs_filtered ("\n", stream);

  if (c->prefixlist == 0 && c->function.cfunc != NULL)
    return;
  fprintf_filtered (stream, "\n");

  /* If this is a prefix command, print it's subcommands */
  if (c->prefixlist)
    help_list (*c->prefixlist, c->prefixname, all_commands, stream);

  /* If this is a class name, print all of the commands in the class */
  if (c->function.cfunc == NULL)
    help_list (cmdlist, "", c->cmdclass, stream);

  if (c->hook)
    fprintf_filtered (stream, "\nThis command has a hook defined: %s\n",
		      c->hook->name);
}

/*
 * Get a specific kind of help on a command list.
 *
 * LIST is the list.
 * CMDTYPE is the prefix to use in the title string.
 * CLASS is the class with which to list the nodes of this list (see
 * documentation for help_cmd_list below),  As usual, ALL_COMMANDS for
 * everything, ALL_CLASSES for just classes, and non-negative for only things
 * in a specific class.
 * and STREAM is the output stream on which to print things.
 * If you call this routine with a class >= 0, it recurses.
 */
void
help_list(struct cmd_list_element *list, const char *cmdtype,
	  enum command_class cmdclass, GDB_FILE *stream)
{
  int len;
  char *cmdtype1, *cmdtype2;
  
  /* If CMDTYPE is "foo ", CMDTYPE1 gets " foo" and CMDTYPE2 gets "foo sub"  */
  len = strlen (cmdtype);
  cmdtype1 = (char *) alloca (len + 1);
  cmdtype1[0] = 0;
  cmdtype2 = (char *) alloca (len + 4);
  cmdtype2[0] = 0;
  if (len)
    {
      cmdtype1[0] = ' ';
      strncpy (cmdtype1 + 1, cmdtype, len - 1);
      cmdtype1[len] = 0;
      strncpy (cmdtype2, cmdtype, len - 1);
      strcpy (cmdtype2 + len - 1, " sub");
    }

  if (cmdclass == all_classes)
    fprintf_filtered (stream, "List of classes of %scommands:\n\n", cmdtype2);
  else
    fprintf_filtered (stream, "List of %scommands:\n\n", cmdtype2);

  help_cmd_list(list, cmdclass, cmdtype, (int)cmdclass >= 0, stream);

  if (cmdclass == all_classes)
    fprintf_filtered (stream, "\n\
Type \"help%s\" followed by a class name for a list of commands in that class.",
	     cmdtype1);

  fprintf_filtered (stream, "\n\
Type \"help%s\" followed by %scommand name for full documentation.\n\
Command name abbreviations are allowed if unambiguous.\n",
	   cmdtype1, cmdtype2);
}
     
/* Print only the first line of STR on STREAM.  */
#ifndef CLI_DECODE_H
static
#endif /* !CLI_DECODE_H */
void
print_doc_line(GDB_FILE *stream, const char *str)
{
  static char *line_buffer = 0;
  static int line_size;
  register const char *p;

  if (!line_buffer)
    {
      line_size = 80;
      line_buffer = (char *) xmalloc (line_size);
    }

  p = str;
  while (*p && *p != '\n' && *p != '.' && *p != ',')
    p++;
  if (p - str > line_size - 1)
    {
      line_size = p - str + 1;
      free ((PTR)line_buffer);
      line_buffer = (char *) xmalloc (line_size);
    }
  strncpy (line_buffer, str, p - str);
  line_buffer[p - str] = '\0';
  if (islower (line_buffer[0]))
    line_buffer[0] = toupper (line_buffer[0]);
  fputs_filtered (line_buffer, stream);
}

/*
 * Implement a help command on command list LIST.
 * RECURSE should be non-zero if this should be done recursively on
 * all sublists of LIST.
 * PREFIX is the prefix to print before each command name.
 * STREAM is the stream upon which the output should be written.
 * CLASS should be:
 *	A non-negative class number to list only commands in that
 * class.
 *	ALL_COMMANDS to list all commands in list.
 *	ALL_CLASSES  to list all classes in list.
 *
 *   Note that RECURSE will be active on *all* sublists, not just the
 * ones selected by the criteria above (ie. the selection mechanism
 * is at the low level, not the high-level).
 */
void
help_cmd_list(struct cmd_list_element *list, enum command_class cmdclass,
              const char *prefix, int recurse, GDB_FILE *stream)
{
  register struct cmd_list_element *c;

  for (c = list; c; c = c->next)
    {
      if (c->abbrev_flag == 0 &&
	  (cmdclass == all_commands
	  || (cmdclass == all_classes && c->function.cfunc == NULL)
	  || (cmdclass == c->cmdclass && c->function.cfunc != NULL)))
	{
	  fprintf_filtered (stream, "%s%s -- ", prefix, c->name);
	  print_doc_line (stream, c->doc);
	  fputs_filtered ("\n", stream);
	}
      if (recurse
	  && c->prefixlist != 0
	  && c->abbrev_flag == 0)
	help_cmd_list(*c->prefixlist, cmdclass, c->prefixname, 1, stream);
    }
}


/* Search the input clist for 'command'.  Return the command if
   found (or NULL if not), and return the number of commands
   found in nfound */

static struct cmd_list_element *
find_cmd(char *command, int len, struct cmd_list_element *clist,
	 int ignore_help_classes, int *nfound)
{
  struct cmd_list_element *found, *c;

  found = (struct cmd_list_element *)NULL;
  *nfound = 0;
  for (c = clist; c; c = c->next)
    if (!strncmp (command, c->name, len)
        && (!ignore_help_classes || c->function.cfunc))
      {
        found = c;
        (*nfound)++;
        if (c->name[len] == '\0')
          {
            *nfound = 1;
            break;
          }
      }
  return found;
}

#ifndef TUI_DATA_H
extern int tui_version;
#endif /* !TUI_DATA_H */

/* This routine takes a line of TEXT and a CLIST in which to start the
   lookup.  When it returns it will have incremented the text pointer past
   the section of text it matched, set *RESULT_LIST to point to the list in
   which the last word was matched, and will return a pointer to the cmd
   list element which the text matches.  It will return NULL if no match at
   all was possible.  It will return -1 (cast appropriately, ick) if ambigous
   matches are possible; in this case *RESULT_LIST will be set to point to
   the list in which there are ambiguous choices (and *TEXT will be set to
   the ambiguous text string).

   If the located command was an abbreviation, this routine returns the base
   command of the abbreviation.

   It does no error reporting whatsoever; control will always return
   to the superior routine.

   In the case of an ambiguous return (-1), *RESULT_LIST will be set to point
   at the prefix_command (ie. the best match) *or* (special case) will be NULL
   if no prefix command was ever found.  For example, in the case of "info a",
   "info" matches without ambiguity, but "a" could be "args" or "address", so
   *RESULT_LIST is set to the cmd_list_element for "info".  So in this case
   RESULT_LIST should not be interpeted as a pointer to the beginning of a
   list; it simply points to a specific command.  In the case of an ambiguous
   return *TEXT is advanced past the last non-ambiguous prefix (e.g.
   "info t" can be "info types" or "info target"; upon return *TEXT has been
   advanced past "info ").

   If RESULT_LIST is NULL, don't set *RESULT_LIST (but don't otherwise
   affect the operation).

   This routine does *not* modify the text pointed to by TEXT.
   
   If IGNORE_HELP_CLASSES is nonzero, ignore any command list elements which
   are actually help classes rather than commands (i.e. the function field of
   the struct cmd_list_element is NULL).  */
struct cmd_list_element *
lookup_cmd_1(const char **text, struct cmd_list_element *clist,
             struct cmd_list_element **result_list, int ignore_help_classes)
{
  const char *p;
  char *command;
  int len, tmp, nfound;
  struct cmd_list_element *found, *c;

  while (**text == ' ' || **text == '\t')
    (*text)++;

  /* Treating underscores as part of command words is important
     so that "set args_foo()" doesn't get interpreted as
     "set args _foo()".  */
  for (p = *text;
       *p && (isalnum(*p) || *p == '-' || *p == '_' ||
	      (tui_version &&
	       (*p == '+' || *p == '<' || *p == '>' || *p == '$')) ||
	      (xdb_commands && (*p == '!' || *p == '/' || *p == '?')));
       p++)
    ;

  /* If nothing but whitespace, return 0.  */
  if (p == *text)
    return 0;
  
  len = p - *text;

  /* *text and p now bracket the first command word to lookup (and
     it's length is len).  We copy this into a local temporary */


  command = (char *) alloca (len + 1);
  for (tmp = 0; tmp < len; tmp++)
    {
      char x = (*text)[tmp];
      command[tmp] = x;
    }
  command[len] = '\0';

  /* Look it up.  */
  found = 0;
  nfound = 0;
  found = find_cmd(command, len, clist, ignore_help_classes, &nfound);

  /* 
  ** We didn't find the command in the entered case, so lower case it
  ** and search again.
  */
  if (!found || nfound == 0)
    {
      for (tmp = 0; tmp < len; tmp++)
        {
          char x = command[tmp];
          command[tmp] = isupper(x) ? tolower(x) : x;
        }
      found = find_cmd(command, len, clist, ignore_help_classes, &nfound);
    }

  /* If nothing matches, we have a simple failure.  */
  if (nfound == 0)
    return 0;

  if (nfound > 1)
    {
      if (result_list != NULL)
	/* Will be modified in calling routine
	   if we know what the prefix command is.  */
	*result_list = 0;		
      return (struct cmd_list_element *) -1; /* Ambiguous.  */
    }

  /* We've matched something on this list.  Move text pointer forward. */

  *text = p;

  /* If this was an abbreviation, use the base command instead.  */

  if (found->cmd_pointer)
    found = found->cmd_pointer;

  /* If we found a prefix command, keep looking.  */

  if (found->prefixlist)
    {
      c = lookup_cmd_1 (text, *found->prefixlist, result_list,
			ignore_help_classes);
      if (!c)
	{
	  /* Didn't find anything; this is as far as we got.  */
	  if (result_list != NULL)
	    *result_list = clist;
	  return found;
	}
      else if (c == (struct cmd_list_element *) -1)
	{
	  /* We've gotten this far properly, but the next step
	     is ambiguous.  We need to set the result list to the best
	     we've found (if an inferior hasn't already set it).  */
	  if (result_list != NULL)
	    if (!*result_list)
	      /* This used to say *result_list = *found->prefixlist
		 If that was correct, need to modify the documentation
		 at the top of this function to clarify what is supposed
		 to be going on.  */
	      *result_list = found;
	  return c;
	}
      else
	{
	  /* We matched!  */
	  return c;
	}
    }
  else
    {
      if (result_list != NULL)
	*result_list = clist;
      return found;
    }
  return NULL;
}

/* All this hair to move the space to the front of cmdtype */
static void
undef_cmd_error(const char *cmdtype, const char *q)
{
  error("Undefined %scommand: \"%s\".  Try \"help%s%.*s\".", cmdtype, q,
  	(*cmdtype? " ": ""), ((int)strlen(cmdtype) - 1), cmdtype);
}

/* Look up the contents of *LINE as a command in the command list LIST.
   LIST is a chain of struct cmd_list_element's.
   If it is found, return the struct cmd_list_element for that command
   and update *LINE to point after the command name, at the first argument.
   If not found, call error if ALLOW_UNKNOWN is zero
   otherwise (or if error returns) return zero.
   Call error if specified command is ambiguous,
   unless ALLOW_UNKNOWN is negative.
   CMDTYPE precedes the word "command" in the error message.

   If INGNORE_HELP_CLASSES is nonzero, ignore any command list
   elements which are actually help classes rather than commands (i.e.
   the function field of the struct cmd_list_element is 0).  */

struct cmd_list_element *
lookup_cmd(const char **line, struct cmd_list_element *list,
	   const char *cmdtype, int allow_unknown, int ignore_help_classes)
{
  struct cmd_list_element *last_list = 0;
  struct cmd_list_element *c =
    lookup_cmd_1 (line, list, &last_list, ignore_help_classes);
#if 0
  /* This is wrong for complete_command.  */
  char *ptr = (*line) + strlen (*line) - 1;

  /* Clear off trailing whitespace.  */
  while (ptr >= *line && (*ptr == ' ' || *ptr == '\t'))
    ptr--;
  *(ptr + 1) = '\0';
#endif /* 0 */
  
  if (!c)
    {
      if (!allow_unknown)
	{
	  if (!*line)
	    error ("Lack of needed %scommand", cmdtype);
	  else
	    {
	      const char *p = *line;
              char *q;

	      while (isalnum(*p) || *p == '-')
		p++;

	      q = (char *) alloca (p - *line + 1);
	      strncpy (q, *line, p - *line);
	      q[p - *line] = '\0';
	      undef_cmd_error (cmdtype, q);
	    }
	}
      else
	return 0;
    }
  else if (c == (struct cmd_list_element *) -1)
    {
      /* Ambigous.  Local values should be off prefixlist or called
	 values.  */
      int local_allow_unknown = (last_list ? last_list->allow_unknown :
				 allow_unknown);
      const char *local_cmdtype = (last_list ? last_list->prefixname : cmdtype);
      struct cmd_list_element *local_list =
	(last_list ? *(last_list->prefixlist) : list);
      
      if (local_allow_unknown < 0)
	{
	  if (last_list)
	    return last_list;	/* Found something.  */
	  else
	    return 0;		/* Found nothing.  */
	}
      else
	{
	  /* Report as error.  */
	  int amb_len;
	  char ambbuf[100];

	  for (amb_len = 0;
	       ((*line)[amb_len] && (*line)[amb_len] != ' '
		&& (*line)[amb_len] != '\t');
	       amb_len++)
	    ;
	  
	  ambbuf[0] = 0;
	  for (c = local_list; c; c = c->next)
	    if (!strncmp (*line, c->name, amb_len))
	      {
		if (strlen (ambbuf) + strlen (c->name) + 6 < (int)sizeof ambbuf)
		  {
		    if (strlen (ambbuf))
		      strcat (ambbuf, ", ");
		    strcat (ambbuf, c->name);
		  }
		else
		  {
		    strcat (ambbuf, "..");
		    break;
		  }
	      }
	  error ("Ambiguous %scommand \"%s\": %s.", local_cmdtype,
		 *line, ambbuf);
	  return 0;		/* lint */
	}
    }
  else
    {
      /* We've got something.  It may still not be what the caller
         wants (if this command *needs* a subcommand).  */
      while (**line == ' ' || **line == '\t')
	(*line)++;

      if (c->prefixlist && **line && !c->allow_unknown)
	undef_cmd_error (c->prefixname, *line);

      /* Seems to be what he wants.  Return it.  */
      return c;
    }
  return 0;
}
	
#if 0
/* Look up the contents of *LINE as a command in the command list LIST.
   LIST is a chain of struct cmd_list_element's.
   If it is found, return the struct cmd_list_element for that command
   and update *LINE to point after the command name, at the first argument.
   If not found, call error if ALLOW_UNKNOWN is zero
   otherwise (or if error returns) return zero.
   Call error if specified command is ambiguous,
   unless ALLOW_UNKNOWN is negative.
   CMDTYPE precedes the word "command" in the error message.  */

struct cmd_list_element *
lookup_cmd (line, list, cmdtype, allow_unknown)
     char **line;
     struct cmd_list_element *list;
     char *cmdtype;
     int allow_unknown;
{
  register char *p;
  register struct cmd_list_element *c, *found;
  int nfound;
  char ambbuf[100];
  char *processed_cmd;
  int i, cmd_len;

  /* Skip leading whitespace.  */

  while (**line == ' ' || **line == '\t')
    (*line)++;

  /* Clear out trailing whitespace.  */

  p = *line + strlen (*line);
  while (p != *line && (p[-1] == ' ' || p[-1] == '\t'))
    p--;
  *p = 0;

  /* Find end of command name.  */

  p = *line;
  while (*p == '-' || isalnum(*p))
    p++;

  /* Look up the command name.
     If exact match, keep that.
     Otherwise, take command abbreviated, if unique.  Note that (in my
     opinion) a null string does *not* indicate ambiguity; simply the
     end of the argument.  */

  if (p == *line)
    {
      if (!allow_unknown)
	error ("Lack of needed %scommand", cmdtype);
      return 0;
    }
  
  /* Copy over to a local buffer, converting to lowercase on the way.
     This is in case the command being parsed is a subcommand which
     doesn't match anything, and that's ok.  We want the original
     untouched for the routine of the original command.  */
  
  processed_cmd = (char *) alloca (p - *line + 1);
  for (cmd_len = 0; cmd_len < p - *line; cmd_len++)
    {
      char x = (*line)[cmd_len];
      if (isupper(x))
	processed_cmd[cmd_len] = tolower(x);
      else
	processed_cmd[cmd_len] = x;
    }
  processed_cmd[cmd_len] = '\0';

  /* Check all possibilities in the current command list.  */
  found = 0;
  nfound = 0;
  for (c = list; c; c = c->next)
    {
      if (!strncmp (processed_cmd, c->name, cmd_len))
	{
	  found = c;
	  nfound++;
	  if (c->name[cmd_len] == 0)
	    {
	      nfound = 1;
	      break;
	    }
	}
    }

  /* Report error for undefined command name.  */

  if (nfound != 1)
    {
      if (nfound > 1 && allow_unknown >= 0)
	{
	  ambbuf[0] = 0;
	  for (c = list; c; c = c->next)
	    if (!strncmp (processed_cmd, c->name, cmd_len))
	      {
		if (strlen (ambbuf) + strlen (c->name) + 6 < sizeof ambbuf)
		  {
		    if (strlen (ambbuf))
		      strcat (ambbuf, ", ");
		    strcat (ambbuf, c->name);
		  }
		else
		  {
		    strcat (ambbuf, "..");
		    break;
		  }
	      }
	  error ("Ambiguous %scommand \"%s\": %s.", cmdtype,
		 processed_cmd, ambbuf);
	}
      else if (!allow_unknown)
	error ("Undefined %scommand: \"%s\".", cmdtype, processed_cmd);
      return 0;
    }

  /* Skip whitespace before the argument.  */

  while (*p == ' ' || *p == '\t') p++;
  *line = p;

  if (found->prefixlist && *p)
    {
      c = lookup_cmd (line, *found->prefixlist, found->prefixname,
		      found->allow_unknown);
      if (c)
	return c;
    }

  return found;
}
#endif /* 0 */

/* Helper function for SYMBOL_COMPLETION_FUNCTION.  */

/* Return a vector of char pointers which point to the different
   possible completions in LIST of TEXT.  

   WORD points in the same buffer as TEXT, and completions should be
   returned relative to this position.  For example, suppose TEXT is "foo"
   and we want to complete to "foobar".  If WORD is "oo", return
   "oobar"; if WORD is "baz/foo", return "baz/foobar".  */

char **
complete_on_cmdlist(struct cmd_list_element *list, const char *text, char *word)
{
  struct cmd_list_element *ptr;
  char **matchlist;
  int sizeof_matchlist;
  int matches;
  int textlen = strlen (text);

  sizeof_matchlist = 10;
  matchlist = (char **) xmalloc (sizeof_matchlist * sizeof (char *));
  matches = 0;

  for (ptr = list; ptr; ptr = ptr->next)
    if (!strncmp (ptr->name, text, textlen)
	&& !ptr->abbrev_flag
	&& (ptr->function.cfunc
	    || ptr->prefixlist))
      {
	if (matches == sizeof_matchlist)
	  {
	    sizeof_matchlist *= 2;
	    matchlist = (char **) xrealloc ((char *)matchlist,
					    (sizeof_matchlist
					     * sizeof (char *)));
	  }

	matchlist[matches] = (char *) 
	  xmalloc (strlen (word) + strlen (ptr->name) + 1);
	if (word == text)
	  strcpy (matchlist[matches], ptr->name);
	else if (word > text)
	  {
	    /* Return some portion of ptr->name.  */
	    strcpy (matchlist[matches], ptr->name + (word - text));
	  }
	else
	  {
	    /* Return some of text plus ptr->name.  */
	    strncpy (matchlist[matches], word, text - word);
	    matchlist[matches][text - word] = '\0';
	    strcat (matchlist[matches], ptr->name);
	  }
	++matches;
      }

  if (matches == 0)
    {
      free ((PTR)matchlist);
      matchlist = 0;
    }
  else
    {
      matchlist = (char **) xrealloc ((char *)matchlist, ((matches + 1)
						* sizeof (char *)));
      matchlist[matches] = (char *) 0;
    }

  return matchlist;
}

/* Helper function for SYMBOL_COMPLETION_FUNCTION.  */

/* Return a vector of char pointers which point to the different
   possible completions in CMD of TEXT.  

   WORD points in the same buffer as TEXT, and completions should be
   returned relative to this position.  For example, suppose TEXT is "foo"
   and we want to complete to "foobar".  If WORD is "oo", return
   "oobar"; if WORD is "baz/foo", return "baz/foobar".  */
char **
complete_on_enum(const char *enumlist[], const char *text, char *word)
{
  char **matchlist;
  int sizeof_matchlist;
  int matches;
  int textlen = strlen (text);
  int i;
  const char *name;

  sizeof_matchlist = 10;
  matchlist = (char **) xmalloc (sizeof_matchlist * sizeof (char *));
  matches = 0;

  for (i = 0; (name = enumlist[i]) != NULL; i++)
    if (strncmp (name, text, textlen) == 0)
      {
	if (matches == sizeof_matchlist)
	  {
	    sizeof_matchlist *= 2;
	    matchlist = (char **) xrealloc ((char *)matchlist,
					    (sizeof_matchlist
					     * sizeof (char *)));
	  }

	matchlist[matches] = (char *) 
	  xmalloc (strlen (word) + strlen (name) + 1);
	if (word == text)
	  strcpy (matchlist[matches], name);
	else if (word > text)
	  {
	    /* Return some portion of name.  */
	    strcpy (matchlist[matches], name + (word - text));
	  }
	else
	  {
	    /* Return some of text plus name.  */
	    strncpy (matchlist[matches], word, text - word);
	    matchlist[matches][text - word] = '\0';
	    strcat (matchlist[matches], name);
	  }
	++matches;
      }

  if (matches == 0)
    {
      free ((PTR)matchlist);
      matchlist = 0;
    }
  else
    {
      matchlist = (char **) xrealloc ((char *)matchlist, ((matches + 1)
						* sizeof (char *)));
      matchlist[matches] = (char *) 0;
    }

  return matchlist;
}

/* */
static int
parse_binary_operation(const char *arg)
{
  int length;

  if (!arg || !*arg)
    return 1;

  length = strlen (arg);

  while (arg[length - 1] == ' ' || arg[length - 1] == '\t')
    length--;

  if (!strncmp (arg, "on", length)
      || !strncmp (arg, "1", length)
      || !strncmp (arg, "yes", length))
    return 1;
  else
    if (!strncmp (arg, "off", length)
	|| !strncmp (arg, "0", length)
	|| !strncmp (arg, "no", length))
      return 0;
    else 
      {
	error ("\"on\" or \"off\" expected.");
	return 0;
      }
}

/* Do a "set" or "show" command.  ARG is NULL if no argument, or the text
   of the argument, and FROM_TTY is nonzero if this command is being entered
   directly by the user (i.e. these are just like any other
   command).  C is the command list element for the command.  */
void
do_setshow_command(const char *arg, int from_tty, struct cmd_list_element *c)
{
  if (c->type == set_cmd)
    {
      switch (c->var_type)
	{
	case var_string:
	  {
	    char *newstr;
	    const char *p;
	    char *q;
	    int ch;
	    
	    if (arg == NULL)
	      arg = "";
	    newstr = (char *)xmalloc(strlen(arg) + 2UL);
	    p = arg;
            q = newstr;
	    while ((ch = *p++) != '\000')
	      {
		if (ch == '\\')
		  {
		    /* \ at end of argument is used after spaces
		       so they won't be lost.  */
		    /* This is obsolete now that we no longer strip
		       trailing whitespace and actually, the backslash
		       didn't get here in my test, readline or
		       something did something funky with a backslash
		       right before a newline.  */
		    if (*p == 0)
		      break;
		    ch = parse_escape (&p);
		    if (ch == 0)
		      break; /* C loses */
		    else if (ch > 0)
		      *q++ = ch;
		  }
		else
		  *q++ = ch;
	      }
#if 0
	    if (*(p - 1) != '\\')
	      *q++ = ' ';
#endif /* 0 */
	    *q++ = '\0';
	    newstr = (char *)xrealloc(newstr, (q - newstr));
	    if (*(char **)c->var != NULL)
	      free(*(char **)c->var);
	    *(char **)c->var = newstr;
	  }
	  break;
	case var_string_noescape:
	  if (arg == NULL)
	    arg = "";
	  if (*(char **)c->var != NULL)
	    free (*(char **)c->var);
	  *(char **) c->var = savestring (arg, strlen (arg));
	  break;
	case var_filename:
	  if (arg == NULL)
	    error_no_arg ("filename to set it to.");
	  if (*(char **)c->var != NULL)
	    free (*(char **)c->var);
	  *(char **)c->var = tilde_expand (arg);
	  break;
	case var_boolean:
	  *(int *) c->var = parse_binary_operation (arg);
	  break;
	case var_uinteger:
	  if (arg == NULL)
	    error_no_arg ("integer to set it to.");
	  *(unsigned int *) c->var = parse_and_eval_address (arg);
	  if (*(unsigned int *) c->var == 0)
	    *(unsigned int *) c->var = UINT_MAX;
	  break;
	case var_integer:
	  {
	    unsigned int val;
	    if (arg == NULL)
	      error_no_arg ("integer to set it to.");
	    val = parse_and_eval_address (arg);
	    if (val == 0)
	      *(int *) c->var = INT_MAX;
	    else if (val >= INT_MAX)
	      error ("integer %u out of range", val);
	    else
	      *(int *) c->var = val;
	    break;
	  }
	case var_zinteger:
	  if (arg == NULL)
	    error_no_arg ("integer to set it to.");
	  *(int *) c->var = parse_and_eval_address (arg);
	  break;
	case var_enum:
	  {
	    int i;
	    int len;
	    int nmatches;
	    const char *match = NULL;
	    char *p;

	    /* if no argument was supplied, print an informative error message */
	    if (arg == NULL)
	      {
		char msg[1024];
		strcpy(msg, "Requires an argument. Valid arguments are ");
		for (i = 0; c->enums[i]; i++)
		  {
		    if (i != 0)
		      strcat(msg, ", ");
		    strcat(msg, c->enums[i]);
		  }
		strcat(msg, ".");
		error("%s", msg);
	      }

	    p = strchr (arg, ' ');
	    
	    if (p)
	      len = p - arg;
	    else
	      len = strlen (arg);

	    nmatches = 0;
	    for (i = 0; c->enums[i]; i++)
	      if (strncmp (arg, c->enums[i], len) == 0)
		{
		  match = c->enums[i];
		  nmatches++;
		}

	    if (nmatches <= 0)
	      error ("Undefined item: \"%s\".", arg);

	    if (nmatches > 1)
	      error ("Ambiguous item \"%s\".", arg);

	    *(char **)c->var = (char *)match;
	  }
	  break;
	default:
	  error ("gdb internal error: bad var_type in do_setshow_command");
	}
    }
  else if (c->type == show_cmd)
    {
      /* Print doc minus "show" at start.  */
      print_doc_line (gdb_stdout, c->doc + 5);
      
      fputs_filtered (" is ", gdb_stdout);
      wrap_here ("    ");
      switch (c->var_type)
	{
      case var_string:
	{
	  unsigned char *p;

	  fputs_filtered ("\"", gdb_stdout);
	  if (*(unsigned char **)c->var)
	    for (p = *(unsigned char **)c->var; *p != '\0'; p++)
	      gdb_printchar(*p, gdb_stdout, '"');
	  fputs_filtered("\"", gdb_stdout);
	}
	break;
      case var_string_noescape:
      case var_filename:
      case var_enum:
	fputs_filtered ("\"", gdb_stdout);
	if (*(char **)c->var)
	  fputs_filtered (*(char **) c->var, gdb_stdout);
	fputs_filtered ("\"", gdb_stdout);
	break;
      case var_boolean:
	fputs_filtered (*(int *) c->var ? "on" : "off", gdb_stdout);
	break;
      case var_uinteger:
	if (*(unsigned int *) c->var == UINT_MAX) {
	  fputs_filtered ("unlimited", gdb_stdout);
	  break;
	}
	/* else fall through */
      case var_zinteger:
	fprintf_filtered (gdb_stdout, "%u", *(unsigned int *) c->var);
	break;
      case var_integer:
	if (*(int *) c->var == INT_MAX)
	  {
	    fputs_filtered ("unlimited", gdb_stdout);
	  }
	else
	  fprintf_filtered (gdb_stdout, "%d", *(int *) c->var);
	break;
	    
      default:
	error ("gdb internal error: bad var_type in do_setshow_command");
      }
      fputs_filtered (".\n", gdb_stdout);
    }
  else
    error ("gdb internal error: bad cmd_type in do_setshow_command");
  (*c->function.sfunc) (NULL, from_tty, c);
}

/* Show all the settings in a list of show commands.  */
void
cmd_show_list(struct cmd_list_element *list, int from_tty, const char *prefix)
{
  for (; list != NULL; list = list->next) {
    /* If we find a prefix, run its list, prefixing our output by its
       prefix (with "show " skipped).  */
    if (list->prefixlist && !list->abbrev_flag)
      cmd_show_list (*list->prefixlist, from_tty, list->prefixname + 5);
    if (list->type == show_cmd)
      {
	fputs_filtered (prefix, gdb_stdout);
	fputs_filtered (list->name, gdb_stdout);
	fputs_filtered (":  ", gdb_stdout);
	do_setshow_command((const char *)NULL, from_tty, list);
      }
  }
}

/* ARGSUSED */
static void
shell_escape(const char *arg, int from_tty)
{
#ifdef CANT_FORK
  /* FIXME: what about errors (I dunno how GO32 system() handles them)?  */
  system (arg);
#else /* Can fork.  */
  int rc, status, pid;
  const char *p, *user_shell;

  if ((user_shell = (const char *)getenv("SHELL")) == NULL)
    user_shell = "/bin/sh";

  /* Get the name of the shell for arg0 */
  if ((p = strrchr (user_shell, '/')) == NULL)
    p = user_shell;
  else
    p++;			/* Get past '/' */

  if ((pid = fork()) == 0)
    {
      if (!arg)
	execl(user_shell, p, (char *)0);
      else
	execl(user_shell, p, "-c", arg, (char *)0);

      fprintf_unfiltered (gdb_stderr, "Cannot execute %s: %s\n", user_shell,
			  safe_strerror (errno));
      gdb_flush (gdb_stderr);
      _exit (0177);
    }

  if (pid != -1)
    while ((rc = waitpid (pid, &status, 0)) != pid && rc != -1)
      ;
  else
    error ("Fork failed");
#endif /* Can fork.  */
}

static void
make_command(const char *arg, int from_tty)
{
  const char *p;

  if (arg == 0)
    p = "make";
  else
    {
      p = (const char *)xmalloc(sizeof("make ") + strlen(arg));
      strcpy((char *)p, "make ");
      strcpy((char *)p + (sizeof("make ") - 1UL), arg);
    }
  
  shell_escape (p, from_tty);
}

/* */
static void
show_user_1(struct cmd_list_element *c, GDB_FILE *stream)
{
  register struct command_line *cmdlines;

  cmdlines = c->user_commands;
  if (!cmdlines)
    return;
  fputs_filtered ("User command ", stream);
  fputs_filtered (c->name, stream);
  fputs_filtered (":\n", stream);

  while (cmdlines)
    {
      print_command_line (cmdlines, 4, stream);
      cmdlines = cmdlines->next;
    }
  fputs_filtered ("\n", stream);
}

/* ARGSUSED */
static void
show_user(const char *args, int from_tty)
{
  struct cmd_list_element *c;

  if (args)
    {
      c = lookup_cmd(&args, cmdlist, "", 0, 1);
      if (c->cmdclass != class_user)
	error("Not a user command.");
      show_user_1(c, gdb_stdout);
    }
  else
    {
      for (c = cmdlist; c; c = c->next)
	{
	  if (c->cmdclass == class_user)
	    show_user_1(c, gdb_stdout);
	}
    }
}

/* */
void
_initialize_command(void)
{
  add_com ("shell", class_support, shell_escape,
	   "Execute the rest of the line as a shell command.  \n\
With no arguments, run an inferior shell.");

  if (xdb_commands)
    add_com_alias("!", "shell", class_support, 0);

  add_com ("make", class_support, make_command,
	   "Run the ``make'' program using the rest of the line as arguments.");
  add_cmd ("user", no_class, show_user, 
	   "Show definitions of user defined commands.\n\
Argument is the name of the user defined command.\n\
With no argument, show definitions of all user defined commands.", &showlist);
}
