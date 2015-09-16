/* basename.c: Return the basename of a pathname.
 * This file is in the public domain. */

/*

@deftypefn Supplemental char* basename (const char *@var{name})

Returns a pointer to the last component of pathname @var{name}.
Behavior is undefined if the pathname ends in a directory separator.

@end deftypefn

*/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */
#include "ansidecl.h"
#include "libiberty.h"
#include "safe-ctype.h"

#ifndef DIR_SEPARATOR
# define DIR_SEPARATOR '/'
#endif /* !DIR_SEPARATOR */

#if defined(_WIN32) || defined(__MSDOS__) || defined(__DJGPP__) || \
    defined(__OS2__)
# define HAVE_DOS_BASED_FILE_SYSTEM
# ifndef DIR_SEPARATOR_2
#  define DIR_SEPARATOR_2 '\\'
# endif /* !DIR_SEPARATOR_2 */
#endif /* _WIN32 || __MSDOS__ || __DJGPP__ || __OS2__ */

/* Define IS_DIR_SEPARATOR: */
#ifndef DIR_SEPARATOR_2
# define IS_DIR_SEPARATOR(ch) ((ch) == DIR_SEPARATOR)
#else /* DIR_SEPARATOR_2 */
# define IS_DIR_SEPARATOR(ch) \
	(((ch) == DIR_SEPARATOR) || ((ch) == DIR_SEPARATOR_2))
#endif /* DIR_SEPARATOR_2 */

char *
basename(char *name)
{
  char *base;

#if defined(HAVE_DOS_BASED_FILE_SYSTEM)
  /* Skip over the disk name in MSDOS pathnames. */
  if (ISALPHA(name[0]) && (name[1] == ':'))
    name += 2;
#endif /* HAVE_DOS_BASED_FILE_SYSTEM */

  for (base = name; *name; name++)
    {
      if (IS_DIR_SEPARATOR(*name))
	{
	  base = (name + 1);
	}
    }
  return base;
}

/* EOF */

