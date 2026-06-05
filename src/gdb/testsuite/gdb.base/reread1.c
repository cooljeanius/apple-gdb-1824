/* pr 13484 */

#include <stdio.h>

int x;

void
bar(void)
{
  x--;
}

void
foo(void)
{
  x++;
}

int
main(void)
{
#ifdef usestubs
  set_debug_traps ();
  breakpoint ();
#endif
  foo();
  bar();
  return 0;
}
