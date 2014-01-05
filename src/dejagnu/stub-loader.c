/*
 * stub-loader.c
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#else
# warning stub-loader.c would like to include "config.h"
#endif /* HAVE_CONFIG_H */
#include "dejagnu.h"

/* Bleah!! */
int remote_debug = 0;

int main() {
  /* where are these from? */
  set_debug_traps();
  breakpoint();
  return 0;
}

/* EOF */
