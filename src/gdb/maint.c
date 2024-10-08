/* maint.c: Support for GDB maintenance commands.

   Copyright 1992, 1993, 1994, 1995, 1996, 1997, 1999, 2000, 2001,
   2002, 2003, 2004 Free Software Foundation, Inc.

   Written by Fred Fish at Cygnus Support.

   This file is part of GDB.

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


#include "defs.h"
#include <ctype.h>
#include <signal.h>
#include "command.h"
#include "gdbcmd.h"
#include "symtab.h"
#include "gdbtypes.h"
#include "demangle.h"
#include "gdbcore.h"
#include "expression.h"		/* For language.h */
#include "language.h"
#include "maint.h"
#include "symfile.h"
#include "objfiles.h"
#include "value.h"

#include "cli/cli-decode.h"

#include "objc-lang.h"

extern void _initialize_maint_cmds(void);

static void maintenance_command(const char *, int);

static void maintenance_dump_me(const char *, int);

static void maintenance_internal_error(const char *args, int from_tty);

static void maintenance_demangle(const char *, int);

static void maintenance_time_display(const char *, int);

static void maintenance_space_display(const char *, int);

static void maintenance_info_command(const char *, int);

static void maintenance_info_sections(const char *, int);

static void maintenance_print_command(const char *, int);

static void maintenance_do_deprecate(const char *, int);

/* Set this to the maximum number of seconds to wait instead of waiting forever
   in target_wait().  If this timer times out, then it generates an error and
   the command is aborted.  This replaces most of the need for timeouts in the
   GDB test suite, and makes it possible to distinguish between a hung target
   and one with slow communications.  */

int watchdog = 0;
static void
show_watchdog(struct ui_file *file, int from_tty,
	      struct cmd_list_element *c, const char *value)
{
  fprintf_filtered(file, _("Watchdog timer is %s.\n"), value);
}

/*
   LOCAL FUNCTION

   maintenance_command -- access the maintenance subcommands

   SYNOPSIS

   void maintenance_command(const char *args, int from_tty)

   DESCRIPTION
 */

static void
maintenance_command(const char *args, int from_tty)
{
  printf_unfiltered(_("\"maintenance\" must be followed by the name of a maintenance command.\n"));
  help_list(maintenancelist, "maintenance ",
            (enum command_class)-1, gdb_stdout);
}

#ifndef _WIN32
static void
maintenance_dump_me(const char *args, int from_tty)
{
  if (query("Should GDB dump core? "))
    {
# ifdef __DJGPP__
      /* SIGQUIT by default is ignored, so use SIGABRT instead: */
      signal(SIGABRT, SIG_DFL);
      kill(getpid(), SIGABRT);
# else
      signal(SIGQUIT, SIG_DFL);
      kill(getpid(), SIGQUIT);
# endif /* __DJGPP__ */
    }
}
#endif /* !_WIN32 */

/* Stimulate the internal error mechanism that GDB uses when an
   internal problem is detected.  Allows testing of the mechanism.
   Also useful when the user wants to drop a core file but not exit
   GDB. */

static void ATTR_NORETURN
maintenance_internal_error(const char *args, int from_tty)
{
  internal_error(__FILE__, __LINE__, "%s", ((args == NULL) ? "" : args));
}

/* Stimulate the internal error mechanism that GDB uses when an
   internal problem is detected.  Allows testing of the mechanism.
   Also useful when the user wants to drop a core file but not exit
   GDB. */

static void
maintenance_internal_warning(const char *args, int from_tty)
{
  internal_warning(__FILE__, __LINE__, "%s", (args == NULL ? "" : args));
}

/* Someday we should allow demangling for things other than just
   explicit strings.  For example, we might want to be able to specify
   the address of a string in either GDB's process space or the
   debuggee's process space, and have gdb fetch and demangle that
   string.  If we have a char* pointer "ptr" that points to a string,
   we might want to be able to given just the name and have GDB
   demangle and print what it points to, etc.  (FIXME) */

static void
maintenance_demangle(const char *args, int from_tty)
{
  char *demangled;

  if (args == NULL || *args == '\0')
    {
      printf_unfiltered(_("\"maintenance demangle\" takes an argument to demangle.\n"));
    }
  else
    {
      /* APPLE LOCAL: Using language_demangle is wrong here, because this is a
	 simple utility function, and should work in most cases even when the
	 language is not correct... */
#if !(defined(__APPLE__) && defined(__APPLE_CC__))
      demangled = language_demangle(current_language, args,
				    (DMGL_ANSI | DMGL_PARAMS));
#endif /* !(__APPLE__ && __APPLE_CC__) */
      switch (current_language->la_language)
        {
        case language_objc:
          demangled = objc_demangle(args, 0);
          break;
        case language_objcplus:
          demangled = objcplus_demangle(args, (DMGL_ANSI | DMGL_PARAMS));
          break;
        case language_cplus:
        default:
          demangled = cplus_demangle(args, (DMGL_ANSI | DMGL_PARAMS));
          break;
        }

      if (demangled != NULL)
	{
	  printf_unfiltered("%s\n", demangled);
	  xfree(demangled);
	}
      else
	{
	  printf_unfiltered(_("Cannot demangle \"%s\"\n"), args);
	}
    }
}

extern int display_time; /* only declare this once in this file, up here */

static void
maintenance_time_display(const char *args, int from_tty)
{
  if (args == NULL || *args == '\0')
    printf_unfiltered(_("\"maintenance time\" takes a numeric argument.\n"));
  else
    display_time = (int)strtol(args, NULL, 10);
}

extern int display_space;

static void
maintenance_space_display(const char *args, int from_tty)
{
  if (args == NULL || *args == '\0')
    printf_unfiltered("\"maintenance space\" takes a numeric argument.\n");
  else
    display_space = (int)strtol(args, NULL, 10);
}

/* The "maintenance info" command is defined as a prefix, with
   allow_unknown 0.  Therefore, its own definition is called only for
   "maintenance info" with no args.  */

static void
maintenance_info_command(const char *arg, int from_tty)
{
  printf_unfiltered(_("\"maintenance info\" must be followed by the name of an info command.\n"));
  help_list(maintenanceinfolist, "maintenance info ",
            (enum command_class)-1, gdb_stdout);
}

/* Mini tokenizing lexer for 'maint info sections' command.  */

static int
match_substring(const char *string, const char *substr)
{
  size_t substr_len = strlen(substr);
  const char *tok;

  while ((tok = strstr(string, substr)) != NULL)
    {
      /* Got a partial match.  Is it a whole word? */
      if (tok == string
	  || tok[-1] == ' '
	  || tok[-1] == '\t')
      {
	/* Token is delimited at the front... */
	if (tok[substr_len] == ' '
	    || tok[substr_len] == '\t'
	    || tok[substr_len] == '\0')
	{
	  /* Token is delimited at the rear.  Got a whole-word match.  */
	  return 1;
	}
      }
      /* Token didn't match as a whole word.  Advance and try again.  */
      string = tok + 1;
    }
  return 0;
}

/* */
static int
match_bfd_flags(const char *string, flagword flags)
{
  if (flags & SEC_ALLOC)
    if (match_substring (string, "ALLOC"))
      return 1;
  if (flags & SEC_LOAD)
    if (match_substring (string, "LOAD"))
      return 1;
  if (flags & SEC_RELOC)
    if (match_substring (string, "RELOC"))
      return 1;
  if (flags & SEC_READONLY)
    if (match_substring (string, "READONLY"))
      return 1;
  if (flags & SEC_CODE)
    if (match_substring (string, "CODE"))
      return 1;
  if (flags & SEC_DATA)
    if (match_substring (string, "DATA"))
      return 1;
  if (flags & SEC_ROM)
    if (match_substring (string, "ROM"))
      return 1;
  if (flags & SEC_CONSTRUCTOR)
    if (match_substring (string, "CONSTRUCTOR"))
      return 1;
  if (flags & SEC_HAS_CONTENTS)
    if (match_substring (string, "HAS_CONTENTS"))
      return 1;
  if (flags & SEC_NEVER_LOAD)
    if (match_substring (string, "NEVER_LOAD"))
      return 1;
  if (flags & SEC_COFF_SHARED_LIBRARY)
    if (match_substring (string, "COFF_SHARED_LIBRARY"))
      return 1;
  if (flags & SEC_IS_COMMON)
    if (match_substring (string, "IS_COMMON"))
      return 1;

  return 0;
}

static void
print_bfd_flags(flagword flags)
{
  if (flags & SEC_ALLOC)
    printf_filtered(" ALLOC");
  if (flags & SEC_LOAD)
    printf_filtered(" LOAD");
  if (flags & SEC_RELOC)
    printf_filtered(" RELOC");
  if (flags & SEC_READONLY)
    printf_filtered(" READONLY");
  if (flags & SEC_CODE)
    printf_filtered(" CODE");
  if (flags & SEC_DATA)
    printf_filtered(" DATA");
  if (flags & SEC_ROM)
    printf_filtered(" ROM");
  if (flags & SEC_CONSTRUCTOR)
    printf_filtered(" CONSTRUCTOR");
  if (flags & SEC_HAS_CONTENTS)
    printf_filtered(" HAS_CONTENTS");
  if (flags & SEC_NEVER_LOAD)
    printf_filtered(" NEVER_LOAD");
  if (flags & SEC_COFF_SHARED_LIBRARY)
    printf_filtered(" COFF_SHARED_LIBRARY");
  if (flags & SEC_IS_COMMON)
    printf_filtered(" IS_COMMON");
}

static void
maint_print_section_info(const char *name, flagword flags,
			 CORE_ADDR addr, CORE_ADDR endaddr,
			 unsigned long filepos)
{
  /* FIXME-32x64: Need deprecated_print_address_numeric with field
     width.  */
  printf_filtered("    0x%s", paddr(addr));
  printf_filtered("->0x%s", paddr(endaddr));
  printf_filtered(" at %s",
		  hex_string_custom((unsigned long)filepos, 8));
  printf_filtered(": %s", name);
  print_bfd_flags(flags);
  printf_filtered("\n");
}

static void
print_bfd_section_info(bfd *abfd, asection *asect, void *arg)
{
  flagword flags = bfd_get_section_flags(abfd, asect);
  const char *name = bfd_section_name(abfd, asect);

  if ((arg == NULL) || (*((char *)arg) == '\0')
      || match_substring((char *)arg, name)
      || match_bfd_flags((char *)arg, flags))
    {
      CORE_ADDR addr, endaddr;

      addr = bfd_section_vma(abfd, asect);
      endaddr = (addr + bfd_section_size(abfd, asect));
      maint_print_section_info(name, flags, addr, endaddr,
                               (unsigned long)asect->filepos);
    }
}

/* */
static void
print_objfile_section_info(bfd *abfd, struct obj_section *asect,
			   const char *string)
{
  flagword flags = bfd_get_section_flags(abfd, asect->the_bfd_section);
  const char *name = bfd_section_name(abfd, asect->the_bfd_section);

  if ((string == NULL) || (*string == '\0')
      || match_substring(string, name)
      || match_bfd_flags(string, flags))
    {
      maint_print_section_info(name, flags, asect->addr, asect->endaddr,
			  (unsigned long)asect->the_bfd_section->filepos);
    }
}

/* */
static void
maintenance_info_sections(const char *arg, int from_tty)
{
  if (exec_bfd)
    {
      printf_filtered (_("Exec file:\n"));
      printf_filtered ("    `%s', ", bfd_get_filename (exec_bfd));
      wrap_here ("        ");
      printf_filtered (_("file type %s.\n"), bfd_get_target (exec_bfd));
      if (arg && *arg && match_substring (arg, "ALLOBJ"))
	{
	  struct objfile *ofile;
	  struct obj_section *osect;

	  /* Only this function cares about the 'ALLOBJ' argument;
	     if 'ALLOBJ' is the only argument, discard it rather than
	     passing it down to print_objfile_section_info (which
	     wouldn't know how to handle it).  */
	  if (strcmp (arg, "ALLOBJ") == 0)
	    arg = NULL;

	  ALL_OBJFILES (ofile)
	    {
	      printf_filtered (_("  Object file: %s\n"),
			       bfd_get_filename (ofile->obfd));
	      ALL_OBJFILE_OSECTIONS (ofile, osect)
		{
		  print_objfile_section_info (ofile->obfd, osect, arg);
		}
	    }
	}
      else
	bfd_map_over_sections(exec_bfd, print_bfd_section_info, (void *)arg);
    }

  if (core_bfd)
    {
      printf_filtered(_("Core file:\n"));
      printf_filtered("    `%s', ", bfd_get_filename(core_bfd));
      wrap_here("        ");
      printf_filtered(_("file type %s.\n"), bfd_get_target(core_bfd));
      bfd_map_over_sections(core_bfd, print_bfd_section_info, (void *)arg);
    }
}

/* */
void
maintenance_print_statistics(const char *args, int from_tty)
{
  print_objfile_statistics();
  print_symbol_bcache_statistics();
}

/* */
static void
maintenance_print_architecture(const char *args, int from_tty)
{
  if (args == NULL)
    gdbarch_dump (current_gdbarch, gdb_stdout);
  else
    {
      struct ui_file *file = gdb_fopen (args, "w");
      if (file == NULL)
	perror_with_name (_("maintenance print architecture"));
      gdbarch_dump (current_gdbarch, file);
      ui_file_delete (file);
    }
}

/* The "maintenance print" command is defined as a prefix, with
   allow_unknown 0.  Therefore, its own definition is called only for
   "maintenance print" with no args.  */

static void
maintenance_print_command(const char *arg, int from_tty)
{
  printf_unfiltered(_("\"maintenance print\" must be followed by the name of a print command.\n"));
  help_list(maintenanceprintlist, "maintenance print ",
            (enum command_class)-1, gdb_stdout);
}

/* The "maintenance translate-address" command converts a section and address
   to a symbol.  This can be called in two ways:
   maintenance translate-address <secname> <addr>
   or   maintenance translate-address <addr>
 */

static void
maintenance_translate_address(const char *arg, int from_tty)
{
  CORE_ADDR address;
  asection *sect;
  char *p;
  struct minimal_symbol *sym;
  struct objfile *objfile;

  if (arg == NULL || *arg == 0)
    error(_("requires argument (address or section + address)"));

  sect = NULL;
  p = (char *)arg;

  if (!isdigit(*p))
    {				/* See if we have a valid section name */
      while (*p && !isspace(*p))	/* Find end of section name */
	p++;
      if (*p == '\000')		/* End of command? */
	error(_("Need to specify <section-name> and <address>"));
      *p++ = '\000';
      while (isspace(*p))
	p++;			/* Skip whitespace */

      ALL_OBJFILES(objfile)
      {
	sect = bfd_get_section_by_name(objfile->obfd, arg);
	if (sect != NULL)
	  break;
      }

      if (!sect)
	error(_("Unknown section %s."), arg);
    }

  address = parse_and_eval_address(p);

  if (sect)
    sym = lookup_minimal_symbol_by_pc_section(address, sect);
  else
    sym = lookup_minimal_symbol_by_pc(address);

  if (sym)
    printf_filtered("%s+%s\n",
		    SYMBOL_PRINT_NAME(sym),
		    paddr_u(address - SYMBOL_VALUE_ADDRESS(sym)));
  else if (sect)
    printf_filtered(_("no symbol at %s:0x%s\n"), sect->name, paddr(address));
  else
    printf_filtered(_("no symbol at 0x%s\n"), paddr(address));

  return;
}


/* When a command is deprecated the user will be warned the first time
   the command is used.  If possible, a replacement will be
   offered. */

static void
maintenance_deprecate(const char *args, int from_tty)
{
  if (args == NULL || *args == '\0')
    {
      printf_unfiltered (_("\"maintenance deprecate\" takes an argument, \n\
the command you want to deprecate, and optionally the replacement command \n\
enclosed in quotes.\n"));
    }

  maintenance_do_deprecate (args, 1);
}

/* */
static void
maintenance_undeprecate(const char *args, int from_tty)
{
  if (args == NULL || *args == '\0')
    {
      printf_unfiltered (_("\"maintenance undeprecate\" takes an argument, \n\
the command you want to undeprecate.\n"));
    }

  maintenance_do_deprecate (args, 0);
}

/* You really shouldn't be using this. It is just for the testsuite.
   Rather, you should use deprecate_cmd() when the command is created
   in _initialize_blah().

   This function deprecates a command and optionally assigns it a
   replacement.  */

static void
maintenance_do_deprecate(const char *text, int deprecate)
{
  struct cmd_list_element *alias = NULL;
  struct cmd_list_element *prefix_cmd = NULL;
  struct cmd_list_element *cmd = NULL;

  char *start_ptr = NULL;
  char *end_ptr = NULL;
  ptrdiff_t len;
  char *replacement = NULL;

  if (text == NULL)
    return;

  if (!lookup_cmd_composition (text, &alias, &prefix_cmd, &cmd))
    {
      printf_filtered(_("Cannot find command '%s' to deprecate.\n"), text);
      return;
    }

  if (deprecate)
    {
      /* look for a replacement command */
      start_ptr = strchr(text, '\"');
      if (start_ptr != NULL)
	{
	  start_ptr++;
	  end_ptr = strrchr(start_ptr, '\"');
	  if (end_ptr != NULL)
	    {
	      len = (end_ptr - start_ptr);
	      start_ptr[len] = '\0';
	      replacement = xstrdup(start_ptr);
	    }
	}
    }

  if (!start_ptr || !end_ptr)
    replacement = NULL;


  /* If they used an alias, we only want to deprecate the alias.

     Note the MALLOCED_REPLACEMENT test.  If the command's replacement
     string was allocated at compile time we don't want to free the
     memory. */
  if (alias)
    {
      if (alias->flags & MALLOCED_REPLACEMENT)
	xfree (alias->replacement);

      if (deprecate)
	alias->flags |= (DEPRECATED_WARN_USER | CMD_DEPRECATED);
      else
	alias->flags &= ~(DEPRECATED_WARN_USER | CMD_DEPRECATED);
      alias->replacement = replacement;
      alias->flags |= MALLOCED_REPLACEMENT;
      return;
    }
  else if (cmd)
    {
      if (cmd->flags & MALLOCED_REPLACEMENT)
	xfree (cmd->replacement);

      if (deprecate)
	cmd->flags |= (DEPRECATED_WARN_USER | CMD_DEPRECATED);
      else
	cmd->flags &= ~(DEPRECATED_WARN_USER | CMD_DEPRECATED);
      cmd->replacement = replacement;
      cmd->flags |= MALLOCED_REPLACEMENT;
      return;
    }
}

/* Maintenance set/show framework.  */

struct cmd_list_element *maintenance_set_cmdlist;
struct cmd_list_element *maintenance_show_cmdlist;

static void
maintenance_set_cmd(const char *args, int from_tty)
{
  printf_unfiltered(_("\"maintenance set\" must be followed by the name of a set command.\n"));
  help_list(maintenance_set_cmdlist, "maintenance set ",
            (enum command_class)-1, gdb_stdout);
}

static void
maintenance_show_cmd(const char *args, int from_tty)
{
  cmd_show_list(maintenance_show_cmdlist, from_tty, "");
}

/* Profiling support.  */

static int maintenance_profile_p;
static void
show_maintenance_profile_p (struct ui_file *file, int from_tty,
			    struct cmd_list_element *c, const char *value)
{
  fprintf_filtered (file, _("Internal profiling is %s.\n"), value);
}

#if (defined(HAVE_MONSTARTUP) && defined(HAVE__MCLEANUP))

# ifdef HAVE__ETEXT
extern char _etext;
#  define TEXTEND &_etext
# else
extern char etext;
#  define TEXTEND &etext
# endif /* HAVE__ETEXT */

static int profiling_state;

#if !defined(HAVE_DECL__MCLEANUP) || !HAVE_DECL__MCLEANUP
extern void _mcleanup(void);
#endif /* !HAVE_DECL__MCLEANUP */

/* */
static void
mcleanup_wrapper(void)
{
  if (profiling_state)
    _mcleanup();
}

#if !defined(HAVE_DECL_MONSTARTUP) || !HAVE_DECL_MONSTARTUP
extern void monstartup(unsigned long, unsigned long);
#endif /* !HAVE_DECL_MONSTARTUP */
extern int main();

/* */
static void
maintenance_set_profile_cmd(const char *args, int from_tty,
                            struct cmd_list_element *c)
{
  if (maintenance_profile_p == profiling_state)
    return;

  profiling_state = maintenance_profile_p;

  if (maintenance_profile_p)
    {
      static int profiling_initialized;

      if (!profiling_initialized)
	{
	  atexit(mcleanup_wrapper);
	  profiling_initialized = 1;
	}

      /* "main" is now always the first function in the text segment, so use
	 its address for monstartup.  */
      monstartup((unsigned long)&main, (unsigned long)TEXTEND);
    }
  else
    {
      _mcleanup();
    }
}
#else
static void ATTR_NORETURN
maintenance_set_profile_cmd(const char *args, int from_tty,
			    struct cmd_list_element *c)
{
  error(_("Profiling support is not available on this system."));
}
#endif


/* APPLE LOCAL: Provide "functional area" timers...  */
struct gdb_timer
{
  char *name;          /* The name of this timer */
  int running;         /* Are we running this timer? */
  char *last_mssg;     /* When you start the timer, you register a mssg to */
		       /* be printed when it is reported.  This is the mssg.  */
  long last_start;     /* This is the clock time when the timer was last started.  */
  long total_time;     /* This is the cumulative time on this timer.  */
  long last_interval;  /* This is the last interval added to the timer.  */

  long child_times;    /* This is the sum total of times that accrued */
		       /* in other timers while this one was running.  */
		       /* We subtract that from this timer's total time.  */
};

struct gdb_timer_stack
{
  struct gdb_timer *timer;
  struct gdb_timer_stack *next;
};

static struct gdb_timer *timer_list;
static struct gdb_timer_stack *timer_stack;
static char *active_timer_list;
static int max_timers = 0;
static int n_timers = 0;
int maint_use_timers = 0;

/* Turns on and off printing interval timers.  Right now this
   is an all or nothing thing.  But we could ass a timer regexp
   or timer list or something and check the timers against that.  */
static void
maintenance_interval_display(const char *args, int from_tty)
{
  if (args == NULL || *args == '\0')
    printf_unfiltered(_("\"maintenance interval\" takes \"all\", \"off\" or a list of timers.\n"));
  else if (strcmp(args, "off") == 0)
    {
      maint_use_timers = 0;
      if (active_timer_list != NULL)
	xfree(active_timer_list);
      active_timer_list = NULL;
    }
  else
    {
      maint_use_timers = 1;
      if (active_timer_list != NULL)
	xfree (active_timer_list);
      if (strcmp (args, "all") == 0)
	active_timer_list = NULL;
      else
	active_timer_list = xstrdup (args);
    }
}

/* init_timer makes a new timer with name NAME, and
   returns the token for the timer.  */

/* Allocate space for a new timer with name "NAME" on the timer_list.
   This doesn't check whether the timer exists or not, but will always
   add a new one.  */

static int
init_timer(const char *name)
{
  int this_timer;

  if (n_timers == max_timers)
    {
      max_timers += 64;
      timer_list = ((struct gdb_timer *)
		    xrealloc(timer_list,
			     (max_timers * sizeof(struct gdb_timer *))));
    }
  this_timer = n_timers;
  n_timers++;

  memset(&timer_list[this_timer], 0, sizeof(struct gdb_timer));
  timer_list[this_timer].name = xstrdup(name);

  return this_timer;
}

/* Push TIMER onto the timer stack.  */

static void
push_timer (struct gdb_timer *timer)
{
  struct gdb_timer_stack *next =
    (struct gdb_timer_stack *) xmalloc (sizeof (struct gdb_timer_stack));

  next->timer = timer;
  next->next = timer_stack;
  timer_stack = next;
}

/* This pops the top timer from the timer stack, and
   also charges it's time to the next timer's child_time.  */

void
pop_timer(void)
{
  struct gdb_timer_stack *this_timer = timer_stack;
  struct gdb_timer_stack *next_timer = timer_stack->next;

  if (next_timer != NULL)
    next_timer->timer->child_times += (this_timer->timer->last_interval
                                       + this_timer->timer->child_times);

  timer_stack = next_timer;
  xfree(this_timer);
}

/* This gets the current interval for TIMER, and charges it
   to the timer.  */

static void
stop_timer (struct gdb_timer *timer)
{
  timer->last_interval = get_run_time () - timer->last_start  - timer->child_times;
  timer->total_time += timer->last_interval;
}

/* Print out the data for TIMER.  If LAST_INTERVAL_P is 1, we print out
   the interval information, otherwise we just print out the total.  */

static void
report_timer_internal(struct gdb_timer *timer, int last_interval_p)
{
  struct cleanup *notify_cleanup;
  notify_cleanup = make_cleanup_ui_out_notify_begin_end(uiout, "timer-data");

  ui_out_text(uiout, "\n*+*+* Timer ");
  ui_out_field_string(uiout, "name", timer->name);
  if (last_interval_p)
    {
      ui_out_text(uiout, " - ");
      ui_out_field_string(uiout, "mssg", timer->last_mssg);
    }
  ui_out_text(uiout, " total: ");
  ui_out_field_fmt(uiout, "total", "%0.5f",
                   (double)(timer->total_time / 1000000.0f));
  if (last_interval_p)
    {
      ui_out_text(uiout, " interval: ");
      ui_out_field_fmt(uiout, "interval", "%0.5f",
                       (double)(timer->last_interval / 1000000.0f));
      ui_out_text(uiout, " child times: ");
      ui_out_field_fmt(uiout, "child", "%0.5f",
                       (double)(timer->child_times / 1000000.0f));
    }
  ui_out_text(uiout, " *-*-*\n");

  if (last_interval_p)
    xfree(timer->last_mssg);
  do_cleanups(notify_cleanup);
}

/* Report summary times for all timers: */
static void
maintenance_report_interval_command(const char *args, int is_tty)
{
  int i;
  for (i = 0; i < n_timers; i++)
    report_timer_internal(&timer_list[i], 0);
}

/* Stops timer pointed to by PTR, reports it and pops it.
   For now we require all timers to strictly nest, and throw
   an error if they don't.  */

static void
stop_report_timer (void *ptr)
{
  struct gdb_timer *timer = (struct gdb_timer *) ptr;
  if (timer_stack->timer != timer)
    internal_error (__FILE__, __LINE__, "stoping timer not on top of stack: "
		    "timer %s top of stack: %s", timer->name, timer_stack->timer->name);

  stop_timer (timer);
  report_timer_internal (timer, 1);
  pop_timer ();
}

/* Starts timer TIMER_ID, STRING is the name associated with this
   interval.  Returns a cleanup to stop & report it.  N.B. the timers
   are required to strictly nest.  So be sure you stop your timer in
   the same scope where you start it.  */

static struct cleanup *
make_cleanup_start_report_timer(int timer_id, const char *string)
{
  struct gdb_timer *timer;
  if (timer_id == -1)
    return make_cleanup(null_cleanup, NULL);

  if (timer_id > n_timers)
    error(_("Invalid timer in start_timers"));

  timer = &timer_list[timer_id];

  push_timer(timer);
  timer->last_start = get_run_time();
  if (string == NULL)
    string = (char *)"<Unknown>";

  timer->last_mssg = xstrdup(string);
  timer->last_interval = 0;
  timer->child_times = 0;
  return make_cleanup(stop_report_timer, timer);
}

/* Finds the timer NAME or creates a new one if
   one doesn't already exist.  Returns the timer token.  */

int
find_timer(const char *name)
{
  int i;
  if (maint_use_timers == 0)
    return -1;

  if (active_timer_list != NULL)
    {
      if (strstr(name, active_timer_list) == NULL)
	return -1;
    }

  for (i = 0; i < n_timers; i++)
    {
      if (strcmp(timer_list[i].name, name) == 0)
	return i;
    }
  return init_timer(name);
}

/* This is the easiest way to start "interval timers".  Pass in
   a pointer to timer id TIMER_VAR.  First time you call this pass
   in TIMER_VAR set to -1 and it will find the timer TIMER_NAME and
   return that timer's TIMER_VAR.  After that, you can pass back
   the value of TIMER_VAR you got, and we'll start up that timer without
   having to do the name search.  THIS_MSSG is the message that this
   interval will be labeled with.  It will return a cleanup you can
   use to stop the timer.  */

struct cleanup *
start_timer(int *timer_var, const char *timer_name, const char *this_mssg)
{
  if (*timer_var == -1)
    *timer_var = find_timer(timer_name);
  if (*timer_var == -1)
    return make_cleanup(null_cleanup, 0);

  return make_cleanup_start_report_timer(*timer_var, this_mssg);
}


/* END APPLE LOCAL */

void
_initialize_maint_cmds(void)
{
  add_prefix_cmd ("maintenance", class_maintenance, maintenance_command, _("\
Commands for use by GDB maintainers.\n\
Includes commands to dump specific internal GDB structures in\n\
a human readable form, to cause GDB to deliberately dump core,\n\
to test internal functions such as the C++/ObjC demangler, etc."),
		  &maintenancelist, "maintenance ", 0,
		  &cmdlist);

  add_com_alias ("mt", "maintenance", class_maintenance, 1);

  add_prefix_cmd ("info", class_maintenance, maintenance_info_command, _("\
Commands for showing internal info about the program being debugged."),
		  &maintenanceinfolist, "maintenance info ", 0,
		  &maintenancelist);
  add_alias_cmd ("i", "info", class_maintenance, 1, &maintenancelist);

  add_cmd ("sections", class_maintenance, maintenance_info_sections, _("\
List the BFD sections of the exec and core files. \n\
Arguments may be any combination of:\n\
	[one or more section names]\n\
	ALLOC LOAD RELOC READONLY CODE DATA ROM CONSTRUCTOR\n\
	HAS_CONTENTS NEVER_LOAD COFF_SHARED_LIBRARY IS_COMMON\n\
Sections matching any argument will be listed (no argument\n\
implies all sections).  In addition, the special argument\n\
	ALLOBJ\n\
lists all sections from all object files, including shared libraries."),
	   &maintenanceinfolist);

  add_prefix_cmd ("print", class_maintenance, maintenance_print_command,
		  _("Maintenance command for printing GDB internal state."),
		  &maintenanceprintlist, "maintenance print ", 0,
		  &maintenancelist);

  add_prefix_cmd ("set", class_maintenance, maintenance_set_cmd, _("\
Set GDB internal variables used by the GDB maintainer.\n\
Configure variables internal to GDB that aid in GDB's maintenance"),
		  &maintenance_set_cmdlist, "maintenance set ",
		  0/*allow-unknown*/,
		  &maintenancelist);

  add_prefix_cmd ("show", class_maintenance, maintenance_show_cmd, _("\
Show GDB internal variables used by the GDB maintainer.\n\
Configure variables internal to GDB that aid in GDB's maintenance"),
		  &maintenance_show_cmdlist, "maintenance show ",
		  0/*allow-unknown*/,
		  &maintenancelist);

#ifndef _WIN32
  add_cmd("dump-me", class_maintenance, maintenance_dump_me, _("\
Get fatal error; make debugger dump its core.\n\
GDB sets its handling of SIGQUIT back to SIG_DFL and then sends\n\
itself a SIGQUIT signal."),
          &maintenancelist);
#endif /* !_WIN32 */

  add_cmd ("internal-error", class_maintenance,
	   maintenance_internal_error, _("\
Give GDB an internal error.\n\
Cause GDB to behave as if an internal error was detected."),
	   &maintenancelist);

  add_cmd ("internal-warning", class_maintenance,
	   maintenance_internal_warning, _("\
Give GDB an internal warning.\n\
Cause GDB to behave as if an internal warning was reported."),
	   &maintenancelist);

  add_cmd ("demangle", class_maintenance, maintenance_demangle, _("\
Demangle a C++/ObjC mangled name.\n\
Call internal GDB demangler routine to demangle a C++ link name\n\
and prints the result."),
	   &maintenancelist);

  add_cmd ("time", class_maintenance, maintenance_time_display, _("\
Set the display of time usage.\n\
If nonzero, will cause the execution time for each command to be\n\
displayed, following the command's output."),
	   &maintenancelist);

  add_cmd ("space", class_maintenance, maintenance_space_display, _("\
Set the display of space usage.\n\
If nonzero, will cause the execution space for each command to be\n\
displayed, following the command's output."),
	   &maintenancelist);

  /* APPLE LOCAL: interval timers */
  add_cmd ("interval", class_maintenance, maintenance_interval_display, _("\
Set the report of low-level interval timers.\n\
If nonzero, will cause the interval timer data to be gathered and printed."),
	   &maintenancelist);

  add_cmd ("report-interval", class_maintenance, maintenance_report_interval_command, _("\
Report the summary values for all the low-level interval timers."),
	   &maintenancelist);

  add_cmd ("type", class_maintenance, maintenance_print_type, _("\
Print a type chain for a given symbol.\n\
For each node in a type chain, print the raw data for each member of\n\
the type structure, and the interpretation of the data."),
	   &maintenanceprintlist);

  add_cmd ("symbols", class_maintenance, maintenance_print_symbols, _("\
Print dump of current symbol definitions.\n\
Entries in the full symbol table are dumped to file OUTFILE.\n\
If a SOURCE file is specified, dump only that file's symbols."),
	   &maintenanceprintlist);

  add_cmd ("msymbols", class_maintenance, maintenance_print_msymbols, _("\
Print dump of current minimal symbol definitions.\n\
Entries in the minimal symbol table are dumped to file OUTFILE.\n\
If a SOURCE file is specified, dump only that file's minimal symbols."),
	   &maintenanceprintlist);

  add_cmd ("psymbols", class_maintenance, maintenance_print_psymbols, _("\
Print dump of current partial symbol definitions.\n\
Entries in the partial symbol table are dumped to file OUTFILE.\n\
If a SOURCE file is specified, dump only that file's partial symbols."),
	   &maintenanceprintlist);

  add_cmd ("objfiles", class_maintenance, maintenance_print_objfiles,
	   _("Print dump of current object file definitions."),
	   &maintenanceprintlist);

  add_cmd ("symtabs", class_maintenance, maintenance_info_symtabs, _("\
List the full symbol tables for all object files.\n\
This does not include information about individual symbols, blocks, or\n\
linetables --- just the symbol table structures themselves.\n\
With an argument REGEXP, list the symbol tables whose names that match that."),
	   &maintenanceinfolist);

  add_cmd ("psymtabs", class_maintenance, maintenance_info_psymtabs, _("\
List the partial symbol tables for all object files.\n\
This does not include information about individual partial symbols,\n\
just the symbol table structures themselves."),
	   &maintenanceinfolist);

  add_cmd ("statistics", class_maintenance, maintenance_print_statistics,
	   _("Print statistics about internal gdb state."),
	   &maintenanceprintlist);

  add_cmd ("architecture", class_maintenance,
	   maintenance_print_architecture, _("\
Print the internal architecture configuration.\n\
Takes an optional file parameter."),
	   &maintenanceprintlist);

  add_cmd ("check-symtabs", class_maintenance, maintenance_check_symtabs,
	   _("Check consistency of psymtabs and symtabs."),
	   &maintenancelist);

  add_cmd ("translate-address", class_maintenance, maintenance_translate_address,
	   _("Translate a section name and address to a symbol."),
	   &maintenancelist);

  add_cmd ("deprecate", class_maintenance, maintenance_deprecate, _("\
Deprecate a command.  Note that this is just in here so the \n\
testsuite can check the comamnd deprecator. You probably shouldn't use this,\n\
rather you should use the C function deprecate_cmd().  If you decide you \n\
want to use it: maintenance deprecate 'commandname' \"replacement\". The \n\
replacement is optional."), &maintenancelist);

  add_cmd ("undeprecate", class_maintenance, maintenance_undeprecate, _("\
Undeprecate a command.  Note that this is just in here so the \n\
testsuite can check the comamnd deprecator. You probably shouldn't use this,\n\
If you decide you want to use it: maintenance undeprecate 'commandname'"),
	   &maintenancelist);

  add_setshow_zinteger_cmd ("watchdog", class_maintenance, &watchdog, _("\
Set watchdog timer."), _("\
Show watchdog timer."), _("\
When non-zero, this timeout is used instead of waiting forever for a target\n\
to finish a low-level step or continue operation.  If the specified amount\n\
of time passes without a response from the target, an error occurs."),
			    NULL,
			    show_watchdog,
			    &setlist, &showlist);

  add_setshow_boolean_cmd("profile", class_maintenance,
			  &maintenance_profile_p, _("\
Set internal profiling."), _("\
Show internal profiling."), _("\
When enabled GDB is profiled."),
			  maintenance_set_profile_cmd,
			  show_maintenance_profile_p,
			  &maintenance_set_cmdlist,
			  &maintenance_show_cmdlist);
}

/* EOF */
