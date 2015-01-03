/* -*- C -*-
 * alloca-conf.h
 */

#ifdef HAVE_CONFIG_H
# if !defined(__CONFIG_H__) || !defined(__BFD_CONFIG_H__)
#  include "config.h"
# else
#  if defined(_INCLUDING_CONFIG_H) && defined(IN_LIBINTL)
#   if defined(__GNUC__) && !defined(__STRICT_ANSI__)
 #    warning "the config.h from ../intl might have already been included."
#   endif /* __GNUC__ && !__STRICT_ANSI__ */
#  else
#   ifdef _APPLE_GDB_TOP_SRC_CONFIG_H
#    if defined(__GNUC__) && !defined(__STRICT_ANSI__)
 #     warning "the config.h from our parent directory might have already been included."
#    endif /* __GNUC__ && !__STRICT_ANSI__ */
#   else
#    if defined(__CONFIG_H__) && defined(__BFD_CONFIG_H__) && defined(BFD_SYSDEP_H)
#     if defined(__GNUC__) && !defined(__STRICT_ANSI__) && defined(lint)
 #      warning "the config.h from ../bfd might have already been included"
#     endif /* __GNUC__ && !__STRICT_ANSI__ && lint */
#    else
#     if defined(__GNUC__) && !defined(__STRICT_ANSI__)
 #      warning "config.h might have already been included and it might have been a different one."
#     endif /* __GNUC__ && !__STRICT_ANSI__ */
#    endif /* __CONFIG_H__ && __BFD_CONFIG_H__ && BFD_SYSDEP_H */
#   endif /* _APPLE_GDB_TOP_SRC_CONFIG_H */
#  endif /* _INCLUDING_CONFIG_H && IN_LIBINTL */
# endif /* !__CONFIG_H__ || !__BFD_CONFIG_H__ */
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
 #  warning "alloca-conf.h expects config.h to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_CONFIG_H */

#if defined(__GNUC__) && !defined(C_ALLOCA)
# ifndef alloca
#  define alloca __builtin_alloca
# endif
#else /* ! defined (__GNUC__) */
#  ifdef _AIX
 #pragma alloca
#  else
#  if defined(HAVE_ALLOCA_H) && !defined(C_ALLOCA)
#   include <alloca.h>
#  else /* ! defined (HAVE_ALLOCA_H) */
#   ifdef __STDC__
extern PTR alloca(size_t);
#   else /* ! defined (__STDC__) */
extern PTR alloca();
#   endif /* ! defined (__STDC__) */
#  endif /* ! defined (HAVE_ALLOCA_H) */
#  ifdef _WIN32
#   include <malloc.h>
#  endif /* _WIN32 */
# endif /* ! defined (_AIX) */
#endif /* ! defined (__GNUC__) */

/* EOF */
