/* command.h: Header file for command-reading library command.c.

   Copyright 1986, 1989, 1990, 1991, 1992, 1993, 1994, 1995, 1999,
   2000, 2002, 2004 Free Software Foundation, Inc.

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

#if !defined(COMMAND_H)
#define COMMAND_H 1

#include "ansidecl.h"

/* Command classes are top-level categories into which commands are broken
   down for "help" purposes.
   Notes on classes: class_alias is for alias commands which are not
   abbreviations of the original command.  class-pseudo is for
   commands which are not really commands nor help topics ("stop").  */

enum command_class
{
  /* Special args to help_list: */
  class_deprecated = -3, /* FIXME: not sure if this should be -3 or 0... */
  /* (not that it matters though; it is never used anywhere else...) */
  all_classes = -2, all_commands = -1,
  /* Classes of commands */
  no_class = -1, class_run = 0, class_vars, class_stack,
  class_files, class_support, class_info, class_breakpoint, class_trace,
  class_alias, class_obscure, class_user, class_maintenance,
  class_pseudo, class_tui, class_xdb
};

/* FIXME: cagney/2002-03-17: Once cmd_type() has been removed, ``enum
   cmd_types'' can be moved from "command.h" to "cli-decode.h".  */
/* Not a set/show command.  Note that some commands which begin with
   "set" or "show" might be in this category, if their syntax does
   not fall into one of the following categories.  */
typedef enum cmd_types
  {
    not_set_cmd,
    set_cmd,
    show_cmd
  }
cmd_types;

/* Types of "set" or "show" command.  */
typedef enum var_types
  {
    /* "on" or "off".  *VAR is an integer which is nonzero for on,
       zero for off.  */
    var_boolean,

    /* "on" / "true" / "enable" or "off" / "false" / "disable" or
       "auto.  *VAR is an ``enum auto_boolean''.  NOTE: In general a
       custom show command will need to be implemented - one that for
       "auto" prints both the "auto" and the current auto-selected
       value. */
    var_auto_boolean,

    /* Unsigned Integer.  *VAR is an unsigned int.  The user can type 0
       to mean "unlimited", which is stored in *VAR as UINT_MAX.  */
    var_uinteger,

    /* Like var_uinteger but signed.  *VAR is an int.  The user can type 0
       to mean "unlimited", which is stored in *VAR as INT_MAX.  */
    var_integer,

    /* String which the user enters with escapes (e.g. the user types \n and
       it is a real newline in the stored string).
       *VAR is a malloc'd string, or NULL if the string is empty.  */
    var_string,
    /* String which stores what the user types verbatim.
       *VAR is a malloc'd string, or NULL if the string is empty.  */
    var_string_noescape,
    /* String which stores a filename.  (*VAR) is a malloc'd string,
       or "" if the string was empty.  */
    var_optional_filename,
    /* String which stores a filename.  (*VAR) is a malloc'd
       string.  */
    var_filename,
    /* ZeroableInteger.  *VAR is an int.  Like Unsigned Integer except
       that zero really means zero.  */
    var_zinteger,
    /* Enumerated type.  Can only have one of the specified values.  *VAR is a
       char pointer to the name of the element that we find.  */
    var_enum
  }
var_types;

/* This structure records one command'd definition.  */
struct cmd_list_element;

/* Forward-declarations of the entry-points of cli/cli-decode.c.  */

extern struct cmd_list_element *add_cmd(const char *, enum command_class,
					void (*fun)(const char *, int),
					const char *,
					struct cmd_list_element **);

extern struct cmd_list_element *add_alias_cmd(const char *, const char *,
					      enum command_class, int,
					      struct cmd_list_element **);

extern struct cmd_list_element *add_prefix_cmd(const char *, enum command_class,
					       void (*fun)(const char *, int),
					       const char *,
					       struct cmd_list_element **,
					       const char *, int,
					       struct cmd_list_element **);

extern struct cmd_list_element *add_abbrev_prefix_cmd(const char *,
						      enum command_class,
						      void (*fun)(const char *,
								  int),
						      const char *,
						      struct cmd_list_element
						      **, const char *, int,
						      struct cmd_list_element
						      **);

/* Set the commands corresponding callback.  */

typedef void cmd_cfunc_ftype(const char *args, int from_tty);
extern void set_cmd_cfunc(struct cmd_list_element *cmd,
			  cmd_cfunc_ftype *cfunc);

typedef void cmd_sfunc_ftype(const char *args, int from_tty,
			     struct cmd_list_element *c);
extern void set_cmd_sfunc(struct cmd_list_element *cmd,
			  cmd_sfunc_ftype *sfunc);

extern void set_cmd_completer(struct cmd_list_element *cmd,
			      char **(*completer)(const char *text,
						  char *word));

/* HACK: cagney/2002-02-23: Code, mostly in tracepoints.c, grubs
   around in cmd objects to test the value of the commands sfunc().  */
extern int cmd_cfunc_eq(struct cmd_list_element *cmd,
			void (*cfunc)(const char *args, int from_tty));

/* Each command object has a local context attached to it. .  */
extern void set_cmd_context(struct cmd_list_element *cmd, void *context);
extern void *get_cmd_context(struct cmd_list_element *cmd);


/* Execute CMD's pre/post hook.  Throw an error if the command fails.
   If already executing this pre/post hook, or there is no pre/post
   hook, the call is silently ignored.  */
extern void execute_cmd_pre_hook(struct cmd_list_element *cmd);
extern void execute_cmd_post_hook(struct cmd_list_element *cmd);

/* Return the type of the command: */
extern enum cmd_types cmd_type(struct cmd_list_element *cmd);


extern struct cmd_list_element *lookup_cmd(const char **,
					   struct cmd_list_element *,
					   const char *, int, int);

extern struct cmd_list_element *lookup_cmd_1(const char **,
					     struct cmd_list_element *,
					     struct cmd_list_element **,
					     int);

extern struct cmd_list_element *
  deprecate_cmd(struct cmd_list_element *, const char *);

/* FIXME: should this get ATTRIBUTE_DEPRECATED? Or is the "deprecated" in
 * its name just part of what it does? */
extern void
  deprecated_cmd_warning(const char **);

extern int
  lookup_cmd_composition(const char *text,
                         struct cmd_list_element **alias,
                         struct cmd_list_element **prefix_cmd,
                         struct cmd_list_element **cmd);

extern struct cmd_list_element *add_com(const char *, enum command_class,
					void (*fun)(const char *, int),
					const char *);

extern struct cmd_list_element *add_com_alias(const char *, const char *,
					      enum command_class, int);

extern struct cmd_list_element *add_info(const char *,
					 void (*fun)(const char *, int),
					 const char *);

extern struct cmd_list_element *add_info_alias(const char *, const char *, int);

extern char **complete_on_cmdlist(struct cmd_list_element *, const char *,
				  char *);

extern char **complete_on_enum(const char *enumlist[], const char *, char *);

extern void delete_cmd(const char *, struct cmd_list_element **);

extern void help_cmd(const char *, struct ui_file *);

extern void help_list(struct cmd_list_element *, const char *,
		      enum command_class, struct ui_file *);

extern void help_cmd_list(struct cmd_list_element *, enum command_class,
			  const char *, int, struct ui_file *);

/* NOTE: cagney/2005-02-21: Since every set command should be paired
   with a corresponding show command (i.e., add_setshow_*) this call
   should not be needed.  Unfortunatly some are not (e.g.,
   "maintenance <variable> <value>") and those need to be fixed.  */
extern struct cmd_list_element *deprecated_add_set_cmd(const char *name,
                                                       enum command_class,
                                                       var_types, void *,
                                                       const char *doc,
                                                       struct cmd_list_element **list)
  ATTRIBUTE_DEPRECATED;

/* Method for show a set/show variable's VALUE on FILE.  If this
   method isn't supplied deprecated_show_value_hack() is called (which
   is not good).  */
typedef void (show_value_ftype)(struct ui_file *file,
                                int from_tty,
                                struct cmd_list_element *cmd,
                                const char *value);
/* NOTE: i18n: This function is not i18n friendly.  Callers should
   instead print the value out directly.  */
extern show_value_ftype deprecated_show_value_hack ATTRIBUTE_DEPRECATED;

extern void add_setshow_enum_cmd(const char *name, enum command_class,
				 const char *enumlist[],
				 const char **var,
				 const char *set_doc,
				 const char *show_doc,
				 const char *help_doc,
				 cmd_sfunc_ftype *set_func,
				 show_value_ftype *show_func,
				 struct cmd_list_element **set_list,
				 struct cmd_list_element **show_list);

extern void add_setshow_auto_boolean_cmd(const char *name, enum command_class,
					 enum auto_boolean *var,
					 const char *set_doc,
					 const char *show_doc,
					 const char *help_doc,
					 cmd_sfunc_ftype *set_func,
					 show_value_ftype *show_func,
					 struct cmd_list_element **set_list,
					 struct cmd_list_element **show_list);

extern void add_setshow_boolean_cmd(const char *name, enum command_class,
				    int *var, const char *set_doc,
				    const char *show_doc, const char *help_doc,
				    cmd_sfunc_ftype *set_func,
				    show_value_ftype *show_func,
				    struct cmd_list_element **set_list,
				    struct cmd_list_element **show_list);

extern void add_setshow_filename_cmd(const char *name, enum command_class,
				     char **var, const char *set_doc,
				     const char *show_doc,
				     const char *help_doc,
				     cmd_sfunc_ftype *set_func,
				     show_value_ftype *show_func,
				     struct cmd_list_element **set_list,
				     struct cmd_list_element **show_list);

extern void add_setshow_string_cmd(const char *name, enum command_class,
				   char **var, const char *set_doc,
				   const char *show_doc,
				   const char *help_doc,
				   cmd_sfunc_ftype *set_func,
				   show_value_ftype *show_func,
				   struct cmd_list_element **set_list,
				   struct cmd_list_element **show_list);

extern void add_setshow_string_noescape_cmd(const char *name,
					    enum command_class,
					    char **var,
					    const char *set_doc,
					    const char *show_doc,
					    const char *help_doc,
					    cmd_sfunc_ftype *set_func,
					    show_value_ftype *show_func,
					    struct cmd_list_element **set_list,
					    struct cmd_list_element **show_list);

extern void add_setshow_optional_filename_cmd(const char *name,
					      enum command_class,
					      char **var,
					      const char *set_doc,
					      const char *show_doc,
					      const char *help_doc,
					      cmd_sfunc_ftype *set_func,
					      show_value_ftype *show_func,
					      struct cmd_list_element **set_list,
					      struct cmd_list_element **show_list);

extern void add_setshow_integer_cmd(const char *name, enum command_class,
				    int *var, const char *set_doc,
				    const char *show_doc,
				    const char *help_doc,
				    cmd_sfunc_ftype *set_func,
				    show_value_ftype *show_func,
				    struct cmd_list_element **set_list,
				    struct cmd_list_element **show_list);

extern void add_setshow_uinteger_cmd(const char *name, enum command_class,
				     unsigned int *var, const char *set_doc,
				     const char *show_doc,
				     const char *help_doc,
				     cmd_sfunc_ftype *set_func,
				     show_value_ftype *show_func,
				     struct cmd_list_element **set_list,
				     struct cmd_list_element **show_list);

extern void add_setshow_zinteger_cmd(const char *name, enum command_class,
				     int *var, const char *set_doc,
				     const char *show_doc,
				     const char *help_doc,
				     cmd_sfunc_ftype *set_func,
				     show_value_ftype *show_func,
				     struct cmd_list_element **set_list,
				     struct cmd_list_element **show_list);

/* Do a "show" command for each thing on a command list:  */
extern void cmd_show_list(struct cmd_list_element *, int, const char *);

extern NORETURN void error_no_arg(const char *) ATTR_NORETURN;

extern void dont_repeat(void);

/* Used to mark commands that don't do anything.  If we just leave the
   function field NULL, the command is interpreted as a help topic, or
   as a class of commands.  */

extern void not_just_help_class_command(const char *, int);

/* check function pointer: */
extern int cmd_func_p(struct cmd_list_element *cmd);

/* call the command function: */
extern void cmd_func(struct cmd_list_element *cmd, const char *args,
		     int from_tty);

/* Old deprecated things: */
extern struct cmd_list_element *add_show_from_set(struct cmd_list_element *,
						  struct cmd_list_element **)
  ATTRIBUTE_DEPRECATED;

extern struct cmd_list_element *add_set_cmd(const char *, enum command_class,
					    var_types, char *, const char *,
					    struct cmd_list_element **)
  ATTRIBUTE_DEPRECATED;

#endif /* !defined(COMMAND_H) */

/* EOF */
