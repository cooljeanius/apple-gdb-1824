/* ar.c - Archive modify and extract.
   Copyright 1991, 1992, 1993, 1994, 1995, 1996, 1997, 1998, 1999, 2000,
   2001, 2002, 2003, 2004, 2005
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
   Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.  */

/* Bugs:
 * * Should use getopt the way tar does (complete w/optional -) and
 *   should have long options, too.
 * * GNU ar used to check file against filesystem in quick_update and
 *   replace operations (would check mtime).
 * * Does NOT warn when name truncated.
 * * No way to specify pos_end.
 * * Error messages should be more consistent.
 * * Check compatibility with various other versions of `ar`?
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "sysdep.h"
#include "bfd.h"
#include "libiberty.h"
#include "progress.h"
#include "getopt.h"
#include "aout/ar.h"
#include "libbfd.h"
#include "bucomm.h"
#include "arsup.h"
#include "filenames.h"
#include "binemul.h"
#include "plugin.h"
#include <sys/stat.h>

#ifdef __GO32__
# define EXT_NAME_LEN 3	 /* bufflen of addition to name if it is MS-DOS */
#else
# define EXT_NAME_LEN 6	 /* ditto for *NIX */
#endif /* __GO32__ */

/* set this to debug the option parsing: */
#ifndef FLAGS_DEBUG
# define FLAGS_DEBUG 1
#endif /* !FLAGS_DEBUG */

/* We need to open files in binary modes on system where that makes a
 * difference: */
#ifndef O_BINARY
# define O_BINARY 0
#endif /* !O_BINARY */

/* Kludge declaration from BFD!  This is ugly!  FIXME!  XXX */
struct ar_hdr *
  bfd_special_undocumented_glue(bfd * abfd, const char *filename);

/* Static declarations: */
static void mri_emul(void);
static const char *normalize(const char *, bfd *);
static void remove_output(void);
static void map_over_members(bfd *, void (*)(bfd *), char **, int);
static void print_contents(bfd * member);
static void delete_members(bfd *, char **files_to_delete);

static void move_members(bfd *, char **files_to_move);
static void replace_members(bfd *, char **files_to_replace,
                            bfd_boolean quick);
static void print_descr(bfd * abfd);
static void write_archive(bfd *);
static void ranlib_only(const char *archname);
static void ranlib_touch(const char *archname);
static void usage(int);

/** Globals and flags: */
static int mri_mode;

/* This flag distinguishes between ar and ranlib:
 * 1 means this is 'ranlib'; 0 means this is 'ar'.
 * -1 means if we should use argv[0] to decide.  */
extern int is_ranlib;

/* Non-0 means do NOT warn about creating the archive file if necessary: */
extern int silent_create;
int silent_create = 0;

/* Nonzero means describe each action performed: */
extern int verbose;
int verbose = 0;

/* Nonzero means preserve dates of members when extracting them: */
extern int preserve_dates;
int preserve_dates = 0;

/* Nonzero means do NOT replace existing members whose dates are more
 * recent than the corresponding files: */
extern int newer_only;
int newer_only = 0;

/* Controls the writing of an archive symbol table (in BSD: a __.SYMDEF
 * member).  -1 means we have been explicitly asked to not write a symbol
 * table; +1 means we ha ve been explicitly asked to write it; 0 is the
 * default.  Traditionally, the default in BSD has been to not write the
 * table.  However, for POSIX.2 compliance the default is now to write
 * a symbol table if any of the members are object files: */
extern int write_armap;
int write_armap = 0;

/* Operate in deterministic mode: write zero for timestamps, uids,
 * and gids for archive members and the archive symbol table, and write
 * consistent file modes.  */
extern int deterministic;
int deterministic = -1;			/* Determinism indeterminate.  */

/* Nonzero means it is the name of an existing member; position new/moved
 * files with respect to this one: */
extern char *posname;
char *posname = NULL;

/* Sez how to use `posname': pos_before means position before that member.
 * pos_after means position after that member. pos_end means always at end.
 * pos_default means default appropriately. For the latter two, `posname'
 * should also be zero: */
enum pos
  {
    pos_default, pos_before, pos_after, pos_end
  } postype = pos_default;

static bfd **
get_pos_bfd(bfd **, enum pos, const char *);

/* For extract/delete only.  If COUNTED_NAME_MODE is TRUE, we only
 * extract the COUNTED_NAME_COUNTER instance of that name.  */
static bfd_boolean counted_name_mode = 0;
static int counted_name_counter = 0;

/* Whether to truncate names of files stored in the archive: */
static bfd_boolean ar_truncate = FALSE;

/* Whether to use a full file name match when searching an archive.
 * This is convenient for archives created by the Microsoft lib program: */
static bfd_boolean full_pathname = FALSE;

/* Whether to create a "thin" archive (symbol index only -- no files): */
static bfd_boolean make_thin_archive = FALSE;

/* ('show_version' is a local variable in main() in this version; not sure
 * if we can make it global yet...) */

static int show_help = 0;

#if defined(BFD_SUPPORTS_PLUGINS) && BFD_SUPPORTS_PLUGINS
static const char *plugin_target = "plugin";
#else
static const char *plugin_target = NULL;
#endif /* BFD_SUPPORTS_PLUGINS */

/* ('target' is a local variable in open_inarch() in this version; not sure
 * if we can make it global yet...) */

#ifndef OPTION_PLUGIN
# define OPTION_PLUGIN 201
#endif /* !OPTION_PLUGIN */
#ifndef OPTION_TARGET
# define OPTION_TARGET 202
#endif /* !OPTION_TARGET */

/* still needs a few things to fall into place before this can work: */
#if 0
static struct option long_options[] =
{
  { "help", no_argument, &show_help, 1 },
  { "plugin", required_argument, NULL, OPTION_PLUGIN },
  { "target", required_argument, NULL, OPTION_TARGET },
  { "version", no_argument, &show_version, 1 },
  { NULL, no_argument, NULL, 0 }
};
#endif /* 0 */

int interactive = 0;

static void
mri_emul(void)
{
  interactive = isatty(fileno(stdin));
  yyparse();
}

/* If COUNT is 0, then FUNCTION is called once on each entry.  If nonzero,
 * COUNT is the length of the FILES chain; FUNCTION is called on each entry
 * whose name matches one in FILES.  */
static void
map_over_members(bfd *arch, void (*function)(bfd *), char **files,
                 int count)
{
  bfd *head;
  int match_count;

  if (count == 0)
    {
      for (head = arch->next; head; head = head->next)
	{
	  PROGRESS(1);
	  function(head);
	}
      return;
    }

  /* bfd requires some changes before we can do this: */
#if 0
  for (head = arch->archive_next; head; head = head->archive_next) {
    head->archive_pass = 0;
  }
#endif /* 0 */

  /* This may appear to be a baroque way of accomplishing what we want.
   * However we have to iterate over the filenames in order to notice where
   * a filename is requested but does not exist in the archive.  Ditto
   * mapping over each file each time -- we want to hack multiple
   * references.  */
  for (; count > 0; files++, count--)
    {
      bfd_boolean found = FALSE;

      match_count = 0;
      for (head = arch->next; head; head = head->next)
	{
          const char *filename;

	  PROGRESS(1);
          /* PR binutils/15796: Once an archive element has been matched,
           * do not match it again.  If the user provides multiple
           * same-named parameters on the command line, their intent is
           * to match multiple same-named entries in the archive, not the
           * same entry multiple times: */
	  if (head->archive_pass) {
	    continue;
          }

          filename = head->filename;
	  if (filename == NULL)
	    {
              /* Some archive formats do NOT get the filenames filled in
               * until the elements are opened: */
	      struct stat buf;
	      bfd_stat_arch_elt(head, &buf);
	    }
#if defined(bfd_is_thin_archive)
          else if (bfd_is_thin_archive(arch))
            {
              /* Thin archives store full pathnames.  Need to normalize: */
              filename = normalize(filename, arch);
            }
#endif /* bfd_is_thin_archive */

	  if ((filename != NULL) &&
	      (!FILENAME_CMP(normalize(*files, arch), filename)))
	    {
	      ++match_count;
	      if (counted_name_mode
		  && (match_count != counted_name_counter))
		{
		  /* Counting, and did NOT match on count; go on to the
                   * next one: */
		  continue;
		}

	      found = TRUE;
	      function(head);
              head->archive_pass = 1;
	      /* PR binutils/15796: Once a file has been matched, do not
               * match any more same-named files in the archive.  If the
               * user does want to match multiple same-name files in an
               * archive, then they should provide multiple same-name
               * parameters to the ar command: */
	      break;
	    }
	}
      if (!found) {
	/* xgettext:c-format */
	fprintf(stderr, _("no entry %s in archive\n"), *files);
      }
    }
}

extern bfd_boolean operation_alters_arch;
bfd_boolean operation_alters_arch = FALSE;

/* see comment in version.c about NORETURN in binutils; relevant here: */
static void
usage(int help)
{
  FILE *s;

#ifdef ALLOW_UNUSED_VARIABLES
# if defined(BFD_SUPPORTS_PLUGINS) && BFD_SUPPORTS_PLUGINS
  /* xgettext:c-format */
  const char *command_line
    = _("Usage: %s [emulation options] [-]{dmpqrstx}[abcDfilMNoPsSTuvV]"
        " [--plugin <name>] [member-name] [count] archive-file file...\n");
# else
  /* xgettext:c-format */
  const char *command_line
    = _("Usage: %s [emulation options] [-]{dmpqrstx}[abcDfilMNoPsSTuvV]"
        " [member-name] [count] archive-file file...\n");
# endif /* BFD_SUPPORTS_PLUGINS */
#endif /* ALLOW_UNUSED_VARIABLES */

  s = (help ? stdout : stderr);

  if (! is_ranlib)
    {
      /* xgettext:c-format */
      fprintf(s, _("Usage: %s [emulation options] [-]{dmpqrstx}[abcfilNoPsSuvV] [member-name] [count] archive-file file...\n"),
              program_name);
      /* xgettext:c-format */
      fprintf(s, _("       %s -M [<mri-script]\n"), program_name);
      fprintf(s, _(" commands:\n"));
      fprintf(s, _("  d            - delete file(s) from the archive\n"));
      fprintf(s, _("  m[ab]        - move file(s) in the archive\n"));
      fprintf(s, _("  p            - print file(s) found in the archive\n"));
      fprintf(s, _("  q[f]         - quick append file(s) to the archive\n"));
      fprintf(s, _("  r[ab][f][u]  - replace existing or insert new file(s) into the archive\n"));
      fprintf(s, _("  t            - display contents of archive\n"));
      fprintf(s, _("  x[o]         - extract file(s) from the archive\n"));
      fprintf(s, _(" command specific modifiers:\n"));
      fprintf(s, _("  [a]          - put file(s) after [member-name]\n"));
      fprintf(s, _("  [b]          - put file(s) before [member-name] (same as [i])\n"));
      fprintf(s, _("  [N]          - use instance [count] of name\n"));
      fprintf(s, _("  [f]          - truncate inserted file names\n"));
      fprintf(s, _("  [P]          - use full path names when matching\n"));
      fprintf(s, _("  [o]          - preserve original dates\n"));
      fprintf(s, _("  [u]          - only replace files that are newer than current archive contents\n"));
      fprintf(s, _(" generic modifiers:\n"));
      fprintf(s, _("  [c]          - do not warn if the library had to be created\n"));
      fprintf(s, _("  [s]          - create an archive index (cf. ranlib)\n"));
      fprintf(s, _("  [S]          - do not build a symbol table\n"));
      fprintf(s, _("  [v]          - be verbose\n"));
      fprintf(s, _("  [V]          - display the version number\n"));

      ar_emul_usage(s);
    }
  else
    {
      /* xgettext:c-format */
      fprintf(s, _("Usage: %s [options] archive\n"), program_name);
      fprintf(s, _(" Generate an index to speed access to archives\n"));
      fprintf(s, _(" The options are:\n\
  -h --help                    Print this help message\n\
  -V --version                 Print version information\n"));
    }

  list_supported_targets(program_name, stderr);

  if (REPORT_BUGS_TO[0] && help) {
    fprintf(s, _("Report bugs to %s\n"), REPORT_BUGS_TO);
  }

  xexit(help ? 0 : 1);
}

/* Normalize a file name specified on the command line into a file
 * name which we will use in an archive: */
static const char *
normalize(const char *file, bfd *abfd)
{
  /* (avoid setting breakpoints here with gdb) */
  const char *filename;

  if (full_pathname) {
    return file;
  }

  /* TODO: move this to lbasename() from libiberty: */
  filename = strrchr(file, '/');
#ifdef HAVE_DOS_BASED_FILE_SYSTEM
  {
    /* We could have foo/bar\\baz, or foo\\bar, or d:bar.  */
    char *bslash = strrchr(file, '\\');
    if ((filename == NULL) || ((bslash != NULL) && (bslash > filename))) {
      filename = bslash;
    }
    if ((filename == NULL) && (file[0] != '\0') && (file[1] == ':')) {
      filename = (file + 1);
    }
  }
#endif /* HAVE_DOS_BASED_FILE_SYSTEM */
  if (filename != (char *)NULL) {
    filename++;
  } else {
    filename = file;
  }

  if (ar_truncate
      && (abfd != NULL)
      && (strlen(filename) > abfd->xvec->ar_max_namelen))
    {
      char *s;

      /* Space leak: */
      s = (char *)xmalloc(abfd->xvec->ar_max_namelen + 1UL);
      memcpy(s, filename, abfd->xvec->ar_max_namelen);
      s[abfd->xvec->ar_max_namelen] = '\0';
      filename = s;
    }

  return filename;
}

/* Remove any output file.  This is only called via xatexit: */
static const char *output_filename = NULL;
static FILE *output_file = NULL;
static bfd *output_bfd = NULL;

static void
remove_output(void)
{
  if (output_filename != NULL)
    {
      if (output_bfd != NULL) {
	bfd_cache_close(output_bfd);
      }
      if (output_file != NULL) {
	fclose(output_file);
      }
      unlink_if_ordinary(output_filename);
    }
}

/* If neither -D nor -U was specified explicitly,
 * then use the configured default: */
static void
default_deterministic(void)
{
  /* (avoid setting breakpoints here with gdb) */
  if (deterministic < 0) {
    deterministic = DEFAULT_AR_DETERMINISTIC;
  }
}

/* The option parsing should be in its own function.
 * It will be when I have getopt working.  */
int main(int, char **);

int
main(int argc, char **argv)
{
  char *arg_ptr;
  char c;   /* used for options (maybe change to 'int'?) */
  char cbak = ' ';
  enum {
    none = 0,
    delete_it,
    replace,
    print_table,
    print_files,
    extract,
    move,
    quick_append
  } operation = none;
  int arg_index;  /* stays in main() in newer versions */
  char **files;  /* stays in main() in newer versions */
  int file_count;  /* stays in main() in newer versions */
  char *inarch_filename;  /* stays in main() in newer versions */
  int show_version;  /* becomes global in newer versions */
  int i;  /* stays in main() in newer versions */
  int do_posix = 0;

#if defined (HAVE_SETLOCALE) && defined (HAVE_LC_MESSAGES)
  setlocale(LC_MESSAGES, "");
#endif /* HAVE_SETLOCALE && HAVE_LC_MESSAGES */
#if defined (HAVE_SETLOCALE)
  setlocale(LC_CTYPE, "");
#endif /* HAVE_SETLOCALE */
  bindtextdomain(PACKAGE, LOCALEDIR);
  textdomain(PACKAGE);

  program_name = argv[0];
  xmalloc_set_program_name(program_name);

#if defined(BFD_SUPPORTS_PLUGINS) && BFD_SUPPORTS_PLUGINS
  bfd_plugin_set_program_name(program_name);
#endif /* BFD_SUPPORTS_PLUGINS */

#if defined(LIBIBERTY_H)
  expandargv(&argc, &argv);
#endif /* LIBIBERTY_H */

  if (is_ranlib < 0)
    {
      char *temp;

      /* TODO: move this to lbasename() from libiberty: */
      temp = strrchr(program_name, '/');
#ifdef HAVE_DOS_BASED_FILE_SYSTEM
      {
	/* We could have foo/bar\\baz, or foo\\bar, or d:bar.  */
	char *bslash = strrchr(program_name, '\\');
	if ((temp == NULL) || ((bslash != NULL) && (bslash > temp))) {
	  temp = bslash;
        }
	if ((temp == NULL)
            && (program_name[0] != '\0') && (program_name[1] == ':')) {
	  temp = (program_name + 1);
        }
      }
#endif /* HAVE_DOS_BASED_FILE_SYSTEM */
      if (temp == NULL) {
	temp = program_name;
      } else {
	++temp;
      }
      if ((strlen(temp) >= 6)
	  && (FILENAME_CMP((temp + strlen(temp) - 6), "ranlib") == 0)) {
	is_ranlib = 1;
      } else {
	is_ranlib = 0;
      }
    }

  if ((argc > 1) && (argv[1][0] == '-'))
    {
      if (strcmp(argv[1], "--help") == 0)
        {
          usage(1);
        }
      else if (strcmp(argv[1], "--version") == 0)
	{
	  if (is_ranlib) {
	    print_version("ranlib");
	  } else {
	    print_version("ar");
          }
	}
    }

  START_PROGRESS(program_name, 0);

  bfd_init();
  set_default_bfd_target();

  show_version = 0;

  xatexit(remove_output);

  for (i = 1; i < argc; i++) {
    if (! ar_emul_parse_arg(argv[i])) {
      break;
    }
  }
  argv += (i - 1);
  argc -= (i - 1);

  if (is_ranlib)
    {
      /* this becomes a separate function ('ranlib_main()') in newer
       * versions: */
      bfd_boolean touch = FALSE;

      if ((argc < 2)
	  || (strcmp(argv[1], "--help") == 0)
	  || (strcmp(argv[1], "-h") == 0)
	  || (strcmp(argv[1], "-H") == 0)) {
	usage(0);
      }

      if ((strcmp(argv[1], "-V") == 0)
	  || (strcmp(argv[1], "-v") == 0)
	  || (strncmp(argv[1], "--v", 3) == 0)) {
	print_version("ranlib");
      }

      default_deterministic();

      arg_index = 1;  /* change the '1' to 'optind' once we have that */

      if (strcmp(argv[1], "-t") == 0)
	{
	  ++arg_index;
	  touch = TRUE;
	}

      while (arg_index < argc)
	{
	  if (! touch) {
	    ranlib_only(argv[arg_index]);
	  } else {
	    ranlib_touch(argv[arg_index]);
          }
	  ++arg_index;
	}
      xexit(0);
    }

  if ((argc == 2) && (strcmp(argv[1], "-M") == 0))
    {
      mri_emul();
      xexit(0);
    }

  if (argc < 2) {
    usage(0);
  }

  arg_index = 1;  /* should that '1' be a '0' instead? */
  arg_ptr = argv[arg_index];

  if (*arg_ptr == '-')
    {
      /* When the first option starts with '-', we support POSIX-compatible
       * option parsing: */
      do_posix = 1;
      ++arg_ptr;		  /* compatibility */
    }

  do {
      while ((c = *arg_ptr++) != '\0')
	{
#ifdef FLAGS_DEBUG
          if (verbose) {
            printf("'%c' being interpreted as a flag\n", c);
          }
#endif /* FLAGS_DEBUG */
	  switch (c)
	    {
	    case 'd':
	    case 'm':
	    case 'p':
	    case 'q':
	    case 'r':
	    case 't':
	    case 'x':
            {
              if (operation != none) {
		fatal(_("error: two different operation options specified: '%c' and '%c'."),
                      c, cbak);
              }
	      switch (c)
		{
                case 'h':
                  show_help = 1;
                  cbak = 'h';
                  break;
		case 'd':
		  operation = delete_it;
		  operation_alters_arch = TRUE;
                  cbak = 'd';
		  break;
		case 'm':
		  operation = move;
		  operation_alters_arch = TRUE;
                  cbak = 'm';
		  break;
		case 'p':
		  operation = print_files;
                  cbak = 'p';
		  break;
		case 'q':
		  operation = quick_append;
		  operation_alters_arch = TRUE;
                  cbak = 'q';
		  break;
		case 'r':
		  operation = replace;
		  operation_alters_arch = TRUE;
                  cbak = 'r';
		  break;
		case 't':
		  operation = print_table;
                  cbak = 't';
		  break;
		case 'x':
		  operation = extract;
                  cbak = 'x';
		  break;
		}
            }
	    case 'l':
	      break;
	    case 'c':
	      silent_create = 1;
	      break;
	    case 'o':
	      preserve_dates = 1;
	      break;
	    case 'V':
	      show_version = TRUE;
	      break;
	    case 's':
	      write_armap = 1;
	      break;
	    case 'S':
	      write_armap = -1;
	      break;
	    case 'u':
	      newer_only = 1;
	      break;
	    case 'v':
	      verbose = 1;
	      break;
	    case 'a':
	      postype = pos_after;
	      break;
	    case 'b':
	      postype = pos_before;
	      break;
	    case 'i':
	      postype = pos_before;
	      break;
	    case 'M':
	      mri_mode = 1;
	      break;
	    case 'N':
	      counted_name_mode = TRUE;
	      break;
	    case 'f':
	      ar_truncate = TRUE;
	      break;
	    case 'P':
	      full_pathname = TRUE;
	      break;
            case 'T':
              make_thin_archive = TRUE;
              break;
            case 'D':
              deterministic = TRUE;
              break;
            case 'U':
              deterministic = FALSE;
              break;
#if defined(OPTION_PLUGIN) && 0
            case OPTION_PLUGIN:
# if defined(BFD_SUPPORTS_PLUGINS) && BFD_SUPPORTS_PLUGINS
              bfd_plugin_set_plugin(optarg);
# else
              fprintf(stderr,
                      _("sorry - this program has been built without plugin support\n"));
              xexit(1);
# endif /* BFD_SUPPORTS_PLUGINS */
              break;
#endif /* OPTION_PLUGIN && 0 */
#if defined(OPTION_TARGET) && 0
            case OPTION_TARGET:
              target = optarg;
              break;
#endif /* OPTION_TARGET && 0 */
            case 0:	    /* A long option that just sets a flag.  */
              break;
	    default:
	      /* xgettext:c-format */
	      non_fatal(_("illegal option -- %c"), c);
	      usage(0);
	    }
	}

      /* With POSIX-compatible option parsing continue with the next
	 argument if it starts with '-'.  */
      if (do_posix && ((arg_index + 1) < argc)
          && (argv[arg_index + 1][0] == '-')) {
	arg_ptr = (argv[++arg_index] + 1);
      } else {
	do_posix = 0;
      }
  } while (do_posix);

  if (show_version) {
    print_version("ar");
  }

  ++arg_index;
  if (arg_index >= argc) {
    usage(0);
  }

  if (mri_mode)
    {
      default_deterministic();
      mri_emul();
    }
  else
    {
      bfd *arch;

      /* We do NOT use do_quick_append any more.  Too many systems expect
       * ar to always rebuild the symbol table even when q is used.  */

      /* We cannot write an armap when using `ar q`, so just do `ar r`
       * instead: */
      if ((operation == quick_append) && write_armap) {
	operation = replace;
      }

      if (((operation == none) || (operation == print_table))
	  && (write_armap == 1))
	{
	  ranlib_only(argv[arg_index]);
	  xexit(0);
          /* once ranlib_only() has a return value, we can exit with that,
           * instead... */
	}

      if (operation == none) {
	fatal(_("no operation specified"));
      }

      if (newer_only && (operation != replace)) {
	fatal(_("`u' is only meaningful with the `r' option."));
      }

      if (newer_only && (deterministic > 0)) {
        fatal(_("`u' is not meaningful with the `D' option."));
      }

      if (newer_only && (deterministic < 0) && DEFAULT_AR_DETERMINISTIC) {
        non_fatal(_("\
`u' modifier ignored since `D' is the default (see `U')"));
      }

      default_deterministic();

      if (postype != pos_default) {
	posname = argv[arg_index++];
      }

      if (counted_name_mode)
	{
	  if ((operation != extract) && (operation != delete_it)) {
            fatal(_("`N' is only meaningful with the `x' and `d' options."));
          }
	  counted_name_counter = atoi(argv[arg_index++]);
	  if (counted_name_counter <= 0) {
	    fatal(_("Value for `N' must be positive."));
          }
	}

      inarch_filename = argv[arg_index++];

      files = ((arg_index < argc) ? (argv + arg_index) : NULL);
      file_count = (argc - arg_index);

      arch = open_inarch(inarch_filename,
                         ((files == NULL) ? (char *)NULL : files[0]));

#if defined(bfd_is_thin_archive)
      if (operation == extract && bfd_is_thin_archive(arch)) {
	fatal(_("`x' cannot be used on thin archives."));
      }
#endif /* bfd_is_thin_archive */

      switch (operation)
	{
	case print_table:
	  map_over_members(arch, print_descr, files, file_count);
	  break;
	case print_files:
	  map_over_members(arch, print_contents, files, file_count);
	  break;
	case extract:
	  map_over_members(arch, extract_file, files, file_count);
	  break;
	case delete_it:
          if (files != NULL) {
	    delete_members(arch, files);
	  } else {
	    output_filename = NULL;
          }
	  break;
	case move:
          /* PR 12558: Creating and moving at the same time does not
           * make sense.  Just create the archive instead: */
          if (! silent_create)
            {
              if (files != NULL) {
                move_members(arch, files);
              } else {
                output_filename = NULL;
              }
              break;
            }
          /* Fall through to: */
	case replace:
	case quick_append:
          if ((files != NULL) || (write_armap > 0)) {
	    replace_members(arch, files, (operation == quick_append));
	  } else {
	    output_filename = NULL;
          }
	  break;
        case none:
          non_fatal(_("no operation specified"));
          break;
	  /* Should never happen! */
	default:
	  /* xgettext:c-format */
	  fatal(_("internal error -- this option not implemented"));
	}
    }

  END_PROGRESS(program_name);

  xexit(0);
#if !defined(__clang__)
  return 0; /* clang has '-Wunreachable-code-return' */
#endif /* !__clang__ */
}

bfd *
open_inarch(const char *archive_filename, const char *file)
{
  const char *target;  /* becomes a global in newer versions */
  bfd **last_one;
  bfd *next_one;
  struct stat sbuf;
  bfd *arch;
  char **matching;

  bfd_set_error(bfd_error_no_error);

  target = NULL;

  /* dummy to use it for now: */
  if ((target == NULL) && (plugin_target == NULL)) {
    target = plugin_target;
  }

  if (stat(archive_filename, &sbuf) != 0)
    {
#if !defined(__GO32__) || defined(__DJGPP__)
      /* FIXME: I do NOT understand why this fragment was ifndef'ed
       * away for __GO32__; perhaps it was in the days of DJGPP v1.x.
       * stat() works just fine in v2.x, so I think this should be
       * removed.  For now, I enable it for DJGPP v2. -- EZ.  */

/* KLUDGE ALERT! Temporary fix until I figger why
 * stat() is wrong ... think it is buried in GO32's IDT - Jax */
      if (errno != ENOENT) {
	bfd_fatal(archive_filename);
      }
#endif /* !__GO32__ || __DJGPP__ */

      if (!operation_alters_arch)
	{
	  fprintf(stderr, "%s: ", program_name);
	  perror(archive_filename);
	  maybequit();
	  return NULL;
	}

      /* Try to figure out the target to use for the archive from the
       * first object on the list.  */
      if (file != NULL)
	{
	  bfd *obj;

	  obj = bfd_openr(file, NULL);
	  if (obj != NULL)
	    {
	      if (bfd_check_format(obj, bfd_object)) {
		target = bfd_get_target(obj);
              }
	      (void)bfd_close(obj);
	    }
	}

      /* Create an empty archive: */
      arch = bfd_openw(archive_filename, target);
      if (arch == NULL) {
        if (verbose) {
          non_fatal("arch is NULL");
        }
        bfd_fatal(archive_filename);
      } else if (! bfd_set_format(arch, bfd_archive)) {
        if (verbose) {
          non_fatal("bfd_set_format() failed");
        }
        bfd_fatal(archive_filename);
      } else if (! bfd_close(arch)) {
        if (verbose) {
          non_fatal("bfd_close() failed");
        }
	bfd_fatal(archive_filename);
      } else if (!silent_create) {
        non_fatal(_("creating %s"), archive_filename);
      }

      /* If we die creating a new archive, do NOT leave it around: */
      output_filename = archive_filename;
    }

  arch = bfd_openr(archive_filename, target);
  if (arch == NULL)
    {
    bloser:
      bfd_fatal(archive_filename);
    }

  if (! bfd_check_format_matches(arch, bfd_archive, &matching))
    {
      bfd_nonfatal(archive_filename);
      if (bfd_get_error() == bfd_error_file_ambiguously_recognized)
	{
	  list_matching_formats(matching);
	  free(matching);
	}
      xexit(1);
    }

#if defined(bfd_is_thin_archive) && 0
  if (((operation == replace) || (operation == quick_append))
      && (bfd_openr_next_archived_file(arch, NULL) != NULL))
    {
      /* PR 15140: Catch attempts to convert a normal archive into a thin
       * archive or vice versa: */
      if (make_thin_archive && ! bfd_is_thin_archive(arch))
        {
          fatal(_("Cannot convert existing library %s to thin format"),
                bfd_get_filename(arch));
          goto bloser;
        }
      else if (! make_thin_archive && bfd_is_thin_archive(arch))
        {
          fatal(_("Cannot convert existing thin library %s to normal format"),
                bfd_get_filename(arch));
          goto bloser;
        }
    }
#endif /* bfd_is_thin_archive && 0 */

  last_one = &(arch->next);
  /* Read all the contents right away, regardless: */
  for (next_one = bfd_openr_next_archived_file(arch, NULL);
       next_one;
       next_one = bfd_openr_next_archived_file(arch, next_one))
    {
      PROGRESS(1);
      *last_one = next_one;
      last_one = &next_one->next;
    }
  *last_one = (bfd *)NULL;
  if (bfd_get_error() != bfd_error_no_more_archived_files) {
    goto bloser;
  }
  return arch;
}

static void
print_contents(bfd *abfd)
{
#ifdef __BFD_H_SEEN__
  bfd_size_type ncopied = 0UL;
#else
  int ncopied = 0;
#endif /* __BFD_H_SEEN__ */
  char *cbuf = (char *)xmalloc(BUFSIZE);
  struct stat buf;
#ifdef __BFD_H_SEEN__
  bfd_size_type size;
#else
# if defined(HAVE_OFF_T) || defined(off_t) || defined(__STDC__)
  off_t size;
# else
  long size;
# endif /* HAVE_OFF_T || off_t || __STDC__ */
#endif /* __BFD_H_SEEN__ */
  if (bfd_stat_arch_elt(abfd, &buf) != 0) {
    /* xgettext:c-format */
    fatal(_("internal stat error on %s"), bfd_get_filename(abfd));
  }

  if (verbose) {
    /* xgettext:c-format */
    printf(_("\n<%s>\n\n"), bfd_get_filename(abfd));
  }

  bfd_seek(abfd, (file_ptr)0, SEEK_SET);

#ifdef __BFD_H_SEEN__
  size = (bfd_size_type)buf.st_size;
#else
  size = buf.st_size;
#endif /* __BFD_H_SEEN__ */
  while (ncopied < size)
    {
#ifdef __BFD_H_SEEN__
      bfd_size_type nread;
      bfd_size_type tocopy = (size - ncopied);
#else
# if defined(HAVE_SIZE_T) || defined(size_t)
      size_t nread;
      size_t tocopy = (size_t)(size - ncopied);
# else
      int nread;
      int tocopy = (size - ncopied);
# endif /* HAVE_SIZE_T || size_t */
#endif /* __BFD_H_SEEN__ */
      if (tocopy > BUFSIZE) {
	tocopy = BUFSIZE;
      }

#if defined(HAVE_SIZE_T) || defined(size_t)
      nread = (size_t)bfd_bread(cbuf, (bfd_size_type)tocopy, abfd);
#else
      nread = bfd_bread(cbuf, (bfd_size_type)tocopy, abfd);
#endif /* HAVE_SIZE_T || size_t */
      if (nread != tocopy) {
	/* xgettext:c-format */
	fatal(_("%s is not a valid archive"),
              bfd_get_filename(bfd_my_archive(abfd)));
      }

      /* fwrite() in mingw32 may return int instead of bfd_size_type.
       * Cast the return value to bfd_size_type to avoid comparison
       * between signed and unsigned values: */
      if ((bfd_size_type)fwrite(cbuf, 1, (size_t)nread, stdout) != nread) {
	fatal("stdout: %s", strerror(errno));
      }
      ncopied += tocopy;
    }
  free(cbuf);
}

/* Extract a member of the archive into its own file.
 *
 * We defer opening the new file until after we have read a BUFSIZ chunk of
 * the old one, since we know we have just read the archive header for the
 * old one.  Since most members are shorter than BUFSIZ, this means we will
 * read the old header, read the old data, write a new inode for the new
 * file, and write the new data, and be done. This 'optimization' is what
 * comes from sitting next to a bare disk and hearing it every time it
 * seeks.  -- Gnu Gilmore  */
void
extract_file(bfd *abfd)
{
  FILE *ostream;
  char *cbuf = (char *)xmalloc(BUFSIZE);
#ifdef __BFD_H_SEEN__
  bfd_size_type nread, tocopy;
  bfd_size_type ncopied = 0UL;
  bfd_size_type size;
#else
# if defined(HAVE_SIZE_T) || defined(size_t)
  size_t nread, tocopy;
# else
  int nread, tocopy;
# endif /* HAVE_SIZE_T || size_t */
  long ncopied = 0L;
# if defined(HAVE_OFF_T) || defined(off_t) || defined(__STDC__)
  off_t size;
# else
  long size;
# endif /* HAVE_OFF_T || off_t || __STDC__ */
#endif /* __BFD_H_SEEN__ */
  struct stat buf;

  if (bfd_stat_arch_elt(abfd, &buf) != 0) {
    /* xgettext:c-format */
    fatal(_("internal stat error on %s"), bfd_get_filename(abfd));
  }
#ifdef __BFD_H_SEEN__
  size = (bfd_size_type)buf.st_size;
#else
  size = buf.st_size;
#endif /* __BFD_H_SEEN__ */

  /* 'size' could be signed depending on the preprocessor macros, so ignore
   * the warnings that assume that it is unsigned: */
#if defined(__GNUC__) && defined(__GNUC_MINOR__)
# if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6))
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wtype-limits"
# endif /* gcc 4.6+ */
#endif /* GCC */
  if (size < 0L) {
    /* xgettext:c-format */
    fatal(_("stat returns negative size for %s"), bfd_get_filename(abfd));
  }
  /* keep condition the same as where we push: */
#if defined(__GNUC__) && defined(__GNUC_MINOR__)
# if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6))
#  pragma GCC diagnostic pop
# endif /* gcc 4.6+ */
#endif /* GCC */

  if (verbose) {
    printf("x - %s\n", bfd_get_filename(abfd));
  }

  bfd_seek(abfd, (file_ptr)0, SEEK_SET);

  ostream = NULL;
  if (size == 0)
    {
      /* Seems like an abstraction violation, eh?  Well it is OK! */
      output_filename = bfd_get_filename(abfd);

      ostream = fopen(bfd_get_filename(abfd), FOPEN_WB);
      if (ostream == NULL)
	{
	  perror(bfd_get_filename(abfd));
	  xexit(1);
	}

      output_file = ostream;
    }
  else
    {
      while (ncopied < size)
        {
#if defined(HAVE_SIZE_T) || defined(size_t)
          tocopy = (size_t)(size - ncopied);
#else
          tocopy = (size - ncopied);
#endif /* HAVE_SIZE_T || size_t */
          if (tocopy > BUFSIZE) {
            tocopy = BUFSIZE;
          }

#if defined(HAVE_SIZE_T) || defined(size_t)
          nread = (size_t)bfd_bread(cbuf, (bfd_size_type)tocopy, abfd);
#else
          nread = bfd_bread(cbuf, (bfd_size_type)tocopy, abfd);
#endif /* HAVE_SIZE_T || size_t */
          if (nread != tocopy) {
            /* xgettext:c-format */
            fatal(_("%s is not a valid archive"),
                  bfd_get_filename(bfd_my_archive(abfd)));
          }

          /* See comment above; this saves disk arm motion: */
          if (ostream == NULL)
            {
              /* Seems like an abstraction violation?  Well it is OK! */
              output_filename = bfd_get_filename(abfd);

              ostream = fopen(bfd_get_filename(abfd), FOPEN_WB);
              if (ostream == NULL)
                {
                  perror(bfd_get_filename(abfd));
                  xexit(1);
                }

              output_file = ostream;
            }

          /* fwrite in mingw32 may return int instead of bfd_size_type.
           * Cast the return value to bfd_size_type to avoid comparison
           * between signed and unsigned values: */
          if ((bfd_size_type)fwrite(cbuf, 1, (size_t)nread, ostream) != nread) {
            fatal("%s: %s", output_filename, strerror(errno));
          }
          ncopied += tocopy;
        }
    }

  if (ostream != NULL) {
    fclose(ostream);
  }

  output_file = NULL;
  output_filename = NULL;

  chmod(bfd_get_filename(abfd), buf.st_mode);

  if (preserve_dates)
    {
      /* Set access time to modification time.  Only st_mtime is
       * initialized by bfd_stat_arch_elt: */
      buf.st_atime = buf.st_mtime;
      set_times(bfd_get_filename(abfd), &buf);
    }

  free(cbuf);
}

static void
write_archive(bfd *iarch)
{
  bfd *obfd;
  char *old_name, *new_name;
  bfd *contents_head = iarch->next;

  old_name = (char *)xmalloc(strlen(bfd_get_filename(iarch)) + 1);
  strcpy(old_name, bfd_get_filename(iarch));
  new_name = make_tempname(old_name);

  if (new_name == NULL) {
    bfd_fatal(_("failed to create temporary file whilst writing archive"));
  }

  output_filename = new_name;

  obfd = bfd_openw(new_name, bfd_get_target(iarch));

  if (obfd == NULL) {
    bfd_fatal(old_name);
  }

  output_bfd = obfd;

  bfd_set_format(obfd, bfd_archive);

  /* Request writing the archive symbol table unless we have been
   * explicitly requested not to do so: */
  obfd->has_armap = (write_armap >= 0);

  if (ar_truncate)
    {
      /* This should really use bfd_set_file_flags, but that rejects
       * archives: */
      obfd->flags |= BFD_TRADITIONAL_FORMAT;
    }

#ifdef BFD_DETERMINISTIC_OUTPUT
  if (deterministic) {
    obfd->flags |= BFD_DETERMINISTIC_OUTPUT;
  }
#endif /* BFD_DETERMINISTIC_OUTPUT */

#if defined(bfd_is_thin_archive)
  if (make_thin_archive || bfd_is_thin_archive(iarch)) {
    bfd_is_thin_archive(obfd) = 1;
  }
#endif /* bfd_is_thin_archive */

  if (!bfd_set_archive_head(obfd, contents_head)) {
    bfd_fatal(old_name);
  }

  if (!bfd_close(obfd)) {
    bfd_fatal(old_name);
  }

  output_bfd = NULL;
  output_filename = NULL;

  /* We do NOT care if this fails; we might be creating the archive: */
  bfd_close(iarch);

  if (smart_rename(new_name, old_name, 0) != 0) {
    xexit(1);
  }
  free(old_name);
}

/* Return a pointer to the pointer to the entry which should be rplacd'd
 * into when altering.  DEFAULT_POS should be how to interpret pos_default,
 * and should be a pos value: */
static bfd **
get_pos_bfd(bfd **contents, enum pos default_pos,
            const char *default_posname)
{
  bfd **after_bfd = contents;
  enum pos realpos;
  const char *realposname;

  if (postype == pos_default)
    {
      realpos = default_pos;
      realposname = default_posname;
    }
  else
    {
      realpos = postype;
      realposname = posname;
    }

  if (realpos == pos_end)
    {
      while (*after_bfd) {
	after_bfd = &((*after_bfd)->next);
      }
    }
  else
    {
      for (; *after_bfd; after_bfd = &(*after_bfd)->next) {
	if (FILENAME_CMP((*after_bfd)->filename, realposname) == 0)
	  {
	    if (realpos == pos_after) {
	      after_bfd = &(*after_bfd)->next;
            }
	    break;
	  }
      }
    }
  return after_bfd;
}

static void
delete_members(bfd *arch, char **files_to_delete)
{
  bfd **current_ptr_ptr;
  bfd_boolean found;
  bfd_boolean something_changed = FALSE;
  int match_count;

  for (; *files_to_delete != NULL; ++files_to_delete)
    {
      /* In a.out systems, the armap is optional.  It is also called
       * __.SYMDEF.  So if the user asked to delete it, we should remember
       * that fact. This isn't quite right for COFF systems (where
       * __.SYMDEF might be regular member), but it is very unlikely
       * to be a problem.  FIXME */

      if (!strcmp(*files_to_delete, "__.SYMDEF"))
	{
	  arch->has_armap = FALSE;
	  write_armap = -1;
	  continue;
	}

      found = FALSE;
      match_count = 0;
      current_ptr_ptr = &(arch->next);
      while (*current_ptr_ptr)
	{
	  if (FILENAME_CMP(normalize(*files_to_delete, arch),
                           (*current_ptr_ptr)->filename) == 0)
	    {
	      ++match_count;
	      if (counted_name_mode
		  && (match_count != counted_name_counter))
		{
                  ;
		  /* Counting, and did NOT match on count; go on to the
                   * next one.  */
		}
	      else
		{
		  found = TRUE;
		  something_changed = TRUE;
		  if (verbose) {
		    printf("d - %s\n", *files_to_delete);
                  }
		  *current_ptr_ptr = ((*current_ptr_ptr)->next);
		  goto next_file;
		}
	    }

	  current_ptr_ptr = &((*current_ptr_ptr)->next);
	}

      if (verbose && !found)
	{
	  /* xgettext:c-format */
	  printf(_("No member named `%s'\n"), *files_to_delete);
	}
    next_file:
      ;
    }

  if (something_changed) {
    write_archive(arch);
  } else {
    output_filename = NULL;
  }
}


/* Reposition existing members within an archive: */
static void
move_members(bfd *arch, char **files_to_move)
{
  bfd **after_bfd;		/* New entries go after this one */
  bfd **current_ptr_ptr;	/* cdr pointer into contents */

  for (; *files_to_move; ++files_to_move)
    {
      current_ptr_ptr = &(arch->next);
      while (*current_ptr_ptr)
	{
	  bfd *current_ptr = *current_ptr_ptr;
	  if (FILENAME_CMP(normalize(*files_to_move, arch),
                           current_ptr->filename) == 0)
	    {
	      /* Move this file to the end of the list - first cut from
		 where it is.  */
	      bfd *link;
	      *current_ptr_ptr = current_ptr->next;

	      /* Now glue to end: */
	      after_bfd = get_pos_bfd(&arch->next, pos_end, NULL);
	      link = *after_bfd;
	      *after_bfd = current_ptr;
	      current_ptr->next = link;

	      if (verbose) {
		printf("m - %s\n", *files_to_move);
              }

	      goto next_file;
	    }

	  current_ptr_ptr = &((*current_ptr_ptr)->next);
	}
      /* xgettext:c-format */
      fatal(_("no entry %s in archive %s!"), *files_to_move,
            arch->filename);

    next_file:;
    }

  write_archive(arch);
}

/* We ought to default to replacing in place, but this is the existing
 * practice! */
static void
replace_members(bfd *arch, char **files_to_move, bfd_boolean quick)
{
  bfd_boolean changed = FALSE;
  bfd **after_bfd;		/* New entries go after this one.  */
  bfd *current;
  bfd **current_ptr;

  while (files_to_move && *files_to_move)
    {
      if (! quick)
	{
	  current_ptr = &arch->next;
	  while (*current_ptr)
	    {
	      current = *current_ptr;

	      /* For compatibility with existing ar programs, we permit
               * the same file to be added multiple times: */
	      if ((FILENAME_CMP(normalize(*files_to_move, arch),
                                normalize(current->filename, arch)) == 0)
		  && (current->arelt_data != NULL))
		{
		  if (newer_only)
		    {
		      struct stat fsbuf, asbuf;

		      if (stat(*files_to_move, &fsbuf) != 0)
			{
			  if (errno != ENOENT) {
			    bfd_fatal(*files_to_move);
                          }
			  goto next_file;
			}
		      if (bfd_stat_arch_elt(current, &asbuf) != 0) {
			/* xgettext:c-format */
			fatal(_("internal stat error on %s"),
                              current->filename);
                      }

		      if (fsbuf.st_mtime <= asbuf.st_mtime) {
			goto next_file;
                      }
		    }

		  after_bfd = get_pos_bfd(&arch->next, pos_after,
                                          current->filename);
		  if (ar_emul_replace(after_bfd, *files_to_move, verbose))
		    {
		      /* Snip out this entry from the chain: */
		      *current_ptr = (*current_ptr)->next;
		      changed = TRUE;
		    }

		  goto next_file;
		}
	      current_ptr = &(current->next);
	    }
	}

      /* Add to the end of the archive: */
      after_bfd = get_pos_bfd(&arch->next, pos_end, NULL);

      if (ar_emul_append(after_bfd, *files_to_move, verbose)) {
	changed = TRUE;
      }

    next_file:;

      files_to_move++;
    }

  if (changed) {
    write_archive(arch);
  } else {
    output_filename = NULL;
  }
}

static void
ranlib_only(const char *archname)
{
  bfd *arch;

  if (get_file_size(archname) < 1) {
    return;
  }
  write_armap = 1;
  arch = open_inarch(archname, (char *)NULL);
  if (arch == NULL) {
    xexit(1);
  }
  write_archive(arch);
  return; /* '0' once changing return type to 'int' */
}

/* Update the timestamp of the symbol map of an archive: */
static void
ranlib_touch(const char *archname)
{
#ifdef __GO32__
  /* I do NOT think that updating works on go32: */
  ranlib_only(archname);
#else /* everything else: */
  int f;
  bfd *arch;
  char **matching;

  if (get_file_size(archname) < 1) {
    return;
  }
  f = open(archname, (O_RDWR | O_BINARY), 0);
  if (f < 0)
    {
      bfd_set_error(bfd_error_system_call);
      bfd_fatal(archname);
    }

  arch = bfd_fdopenr(archname, (const char *)NULL, f);
  if (arch == NULL) {
    bfd_fatal(archname);
  }
  if (! bfd_check_format_matches(arch, bfd_archive, &matching))
    {
      bfd_nonfatal(archname);
      if (bfd_get_error() == bfd_error_file_ambiguously_recognized)
	{
	  list_matching_formats(matching);
	  free(matching);
	}
      xexit(1);
    }

  if (! bfd_has_map(arch)) {
    /* xgettext:c-format */
    fatal(_("%s: no archive map to update"), archname);
  }

#ifdef BFD_DETERMINISTIC_OUTPUT
  if (deterministic) {
    arch->flags |= BFD_DETERMINISTIC_OUTPUT;
  }
#endif /* BFD_DETERMINISTIC_OUTPUT */

  bfd_update_armap_timestamp(arch);

  if (! bfd_close(arch)) {
    bfd_fatal(archname);
  }
#endif /* __GO32__ || not */
  return; /* '0' once changing return type to 'int' */
}

/* Things which are interesting to map over all or some of the files: */
static void
print_descr(bfd *abfd)
{
  print_arelt_descr(stdout, abfd, verbose);
}

#ifdef EXT_NAME_LEN
# undef EXT_NAME_LEN
#endif /* EXT_NAME_LEN */

/* EOF */
