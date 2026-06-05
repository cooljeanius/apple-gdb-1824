#include <stdio.h>

void
foo (int i)
{
  i++;
  printf ("In foo %d\n", i);
}

int
main(void)
{
  foo (1);
}
