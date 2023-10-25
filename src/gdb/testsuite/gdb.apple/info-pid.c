#include <stdio.h>
#include <unistd.h>

int
main(void)
{
  volatile int keep_looping = 1;

  while (keep_looping)
    sleep(1);

  return (0);
}

void
print_pid(void)
{
  printf("%d\n", getpid());
}
