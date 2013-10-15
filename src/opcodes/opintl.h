/* opintl.h - opcodes specific header for gettext code.
   Copyright 1998, 1999, 2000 Free Software Foundation, Inc.

   Written by Tom Tromey <tromey@cygnus.com>

   This file is part of the opcodes library used by GAS and the GNU binutils.

   You should have received a copy of the GNU General Public License
   along with GAS; see the file COPYING.  If not, write to the Free
   Software Foundation, 51 Franklin Street - Fifth Floor, Boston, MA
   02110-1301, USA. */

#ifdef HAVE_CONFIG_H
# include "config.h"
#else
# warning Not including "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef ENABLE_NLS
# ifdef HAVE_LIBINTL_H
#  include <libintl.h>
# else
#  ifdef HAVE_GETTEXT_H
#   include <gettext.h>
#  else
#   warning opintl.h expects either <libintl.h> or <gettext.h> to be included.
#  endif /* HAVE_GETTEXT_H */
# endif /* HAVE_LIBINTL_H */
/* Note the use of dgetext() and PACKAGE here, rather than gettext().

   This is because the code in this directory is used to build a library which
   will be linked with code in other directories to form programs.  We want to
   maintain a seperate translation file for this directory however, rather
   than being forced to merge it with that of any program linked to
   libopcodes. This is a library, so it cannot depend on the catalog
   currently loaded.

   In order to do this, we have to make sure that when we extract messages we
   use the OPCODES domain rather than the domain of the program that included
   the opcodes library, (eg OBJDUMP).  Hence we use dgettext (PACKAGE, String)
   and define PACKAGE to be 'opcodes'.  (See the code in configure).  */
# define _(String) dgettext (PACKAGE, String)
# ifdef gettext_noop
#  define N_(String) gettext_noop (String)
# else
#  define N_(String) (String)
# endif /* gettext_noop */
# ifndef dcgettext
#  define dcgettext(Domainname, Msgid, Category) (Msgid)
# endif /* !dcgettext */
#else
# define gettext(Msgid) (Msgid)
# define dgettext(Domainname, Msgid) (Msgid)
# define dcgettext(Domainname, Msgid, Category) (Msgid)
# define textdomain(Domainname) while (0) /* nothing */
# define bindtextdomain(Domainname, Dirname) while (0) /* nothing */
# define _(String) (String)
# define N_(String) (String)
#endif /* ENABLE_NLS */
