/*
 * stub-loader.c
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning stub-loader.c would like to include "config.h"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_CONFIG_H */
#include "dejagnu.h"

/* FIXME: check to make sure it is there first: */
#include "../gdb/stubs/gdbstubs.h"

/* Bleah!! */
int remote_debug = 0;

int main()
{
  set_debug_traps();
  breakpoint();
  return 0;
}

/* EOF */
