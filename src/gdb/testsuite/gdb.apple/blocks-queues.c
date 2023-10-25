#include <stdio.h>
#include <limits.h>
#include <dispatch/dispatch.h>
#include <pthread.h>

int 
main(void)
{
  __block hicount = 1;
  dispatch_queue_t queue1, queue2, queue3;
  dispatch_group_t group = dispatch_group_create ();
  queue1 = dispatch_queue_create ("com.apple.gdb.queue1", NULL);
  queue2 = dispatch_queue_create ("com.apple.gdb.queue2", NULL);
  queue3 = dispatch_queue_create ("com.apple.gdb.queue3", NULL);

  pthread_setname_np ("main execution thread");

  dispatch_group_async (group, queue1, ^{sleep (20); puts("queue1 sleep done");});
  dispatch_group_async (group, queue1, ^{sleep (20); puts("queue1 sleep done");});
  dispatch_group_async (group, queue2, ^{sleep (20); puts("queue2 sleep done");});
  dispatch_group_async (group, queue2, ^{sleep (20); puts("queue2 sleep done");});

#ifdef DISPLAY_EXTRA_OUTPUT
  /* This is pretty cool, but the extra output makes writing a testsuite
   * case more difficult, so just leave it ifdef-ed out for now.  */
  dispatch_source_timer_create (DISPATCH_TIMER_INTERVAL, 1 * NSEC_PER_SEC, 0,
                                NULL, queue3, 
                                ^(dispatch_event_t event) { 
                          printf (" hello %d\n", hicount++); 
                          if (hicount == 34)
                            {
                              dispatch_release (dispatch_event_get_source (event));
                              puts ("done saying hello");
                            }
                         });
#endif /* DISPLAY_EXTRA_OUTPUT */

  sleep (1);
  puts ("waiting for work group's queues to complete."); // breakpoint here
  dispatch_group_wait (group, UINT64_MAX);
  /* (currently takes about 40 seconds to complete on my current machine) */

  puts ("all queues completed");

  dispatch_release (queue1);
  dispatch_release (queue2);
  dispatch_release (queue3);
  dispatch_release (group);
  return (0);
}
