#include <stdio.h>
#include <unistd.h>
int
main(void)
{
  puts ("ABOUT TO SLEEP");
  sleep (20);
  puts ("DONE");
  return 0;
}
