/* bfd_compat.c */

/* Stub functions for compatibility between old versions of bfd, and newer
 * versions of bfd in which the functions have been removed.  */

/* DO NOT USE THIS FILE! */

/* (the problem was NOT actually that functions had been removed, but just
 * that I had screwed up the build system for the non-libtool static
 * library in '../../bfd'; since I have fixed that, this file is now
 * unnecessary...) */

#ifdef HAVE_CONFIG_H
# include "config.h"
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning bfd_compat.c expects "config.h" to be included.
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "bfd_compat.c expects <stdlib.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_STDLIB_H */

#include "ansidecl.h"
#include "bfd.h"

/* real version is in ../../bfd/format.c: */
int
bfd_check_format(bfd *pbfd ATTRIBUTE_UNUSED,
                 enum bfd_format bfdfmt ATTRIBUTE_UNUSED)
{
  abort();
  return 0;
}

/* real version is in ../../bfd/section.c: */
bfd_boolean
bfd_get_section_contents(bfd *abfd ATTRIBUTE_UNUSED,
                         sec_ptr section ATTRIBUTE_UNUSED,
                         void *location ATTRIBUTE_UNUSED,
                         file_ptr offset ATTRIBUTE_UNUSED,
                         bfd_size_type count ATTRIBUTE_UNUSED)
{
  abort();
  return (bfd_boolean)0;
}

/* real version is in ../../bfd/opncls.c: */
bfd *
bfd_openr(const char *arg_in ATTRIBUTE_UNUSED,
          const char *unused ATTRIBUTE_UNUSED)
{
  abort();
  return (bfd *)NULL;
}


/* EOF */
