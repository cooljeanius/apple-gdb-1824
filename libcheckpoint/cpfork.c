/*
 * cpfork.c
 */

#include <stdio.h> /* for printf() */
#include <sys/types.h> /* legacy requirement for fork() */
#include <sys/wait.h> /* for wait() */
#include <unistd.h> /* for fork() and sleep() */

/* just keep prototype here for now, because we have no headers here: */
extern pid_t _gdbcp_fork(void);

pid_t
_gdbcp_fork(void)
{
  int status;
#ifdef DEBUG
  int wait_length = 0;
#endif /* DEBUG */
  pid_t pid = fork();

  /* Return the child's pid, so debugger can manage it: */
  if (pid != 0)
    return pid;
  /* Make the child spin forever: */
  while (1)
    {
      wait(&status);
      sleep(1);
#ifdef DEBUG
      wait_length++;
      if (wait_length > 0)
        {
          printf("This is loop iteration number '%d'.\n", wait_length);
        }
#endif /* DEBUG */
    }
}

/* EOF */
