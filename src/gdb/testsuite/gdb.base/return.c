#include <stdio.h>
/*  Test "return" command.  */

void
func1(void)
{
  printf("in func1\n");
}

int
func2(void)
{
  return -5;
}

double
func3(void)
{
  return -5.0;
}

int tmp2;
double tmp3;

int
main(void)
{
#ifdef usestubs
  set_debug_traps();
  breakpoint();
#endif
  func1 ();
  printf("in main after func1\n");
  tmp2 = func2 ();
  tmp3 = func3 ();
  printf("exiting\n");
  return 0;
}
