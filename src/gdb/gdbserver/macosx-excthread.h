/*
 * gdbserver/macosx-excthread.h
 */

#ifndef __GDB_MACOSX_NAT_EXCTHREAD_H__
#define __GDB_MACOSX_NAT_EXCTHREAD_H__

#include <sys/wait.h>

#ifdef HAVE_CONFIG_H
# include "config.h"
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "not including autoheader-generated config header"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
# define MACOSX_EXCTHREAD_H_NON_AUTOTOOLS_BUILD 1
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_MACH_EXCEPTION_TYPES_H
# include <mach/exception_types.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  if defined(__APPLE__) && !defined(MACOSX_EXCTHREAD_H_NON_AUTOTOOLS_BUILD)
#   ifndef exception_mask_t
#    warning "macosx-excthread.h needs to include <mach/exception_types.h> for exception_mask_t"
#   else
#    warning "macosx-excthread.h wants to include <mach/exception_types.h>"
#   endif /* !exception_mask_t */
#  endif /* __APPLE__ && !MACOSX_EXCTHREAD_H_NON_AUTOTOOLS_BUILD */
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_MACH_EXCEPTION_TYPES_H */

struct macosx_exception_info
{
  exception_mask_t masks[EXC_TYPES_COUNT];
  mach_port_t ports[EXC_TYPES_COUNT];
  exception_behavior_t behaviors[EXC_TYPES_COUNT];
  thread_state_flavor_t flavors[EXC_TYPES_COUNT];
  mach_msg_type_number_t count;
};
typedef struct macosx_exception_info macosx_exception_info;

struct macosx_exception_thread_status
{
  gdb_thread_t exception_thread;

  int transmit_from_fd;  /* This pipe is used to transmit data from the */
  int receive_from_fd;   /* exception thread to the main thread.  */
  int transmit_to_fd;    /* This pipe is used to wake up the exception */
  int receive_to_fd;     /* thread. 0 means continue, 1 exit.  */
  int error_transmit_fd; /* The exception thread uses the to signal an error */
  int error_receive_fd;  /* to the main thread.  */

  mach_port_t inferior_exception_port;
  task_t task;
  int stopped_in_softexc;
  macosx_exception_info saved_exceptions;
  macosx_exception_info saved_exceptions_step;
  int saved_exceptions_stepping;
};
typedef struct macosx_exception_thread_status macosx_exception_thread_status;

struct macosx_exception_thread_message
{
  task_t task_port;
  thread_t thread_port;
  exception_type_t exception_type;
  exception_data_t exception_data;
  mach_msg_type_number_t data_count;
};
typedef struct macosx_exception_thread_message
  macosx_exception_thread_message;

void macosx_exception_thread_init(macosx_exception_thread_status *s);

void macosx_exception_thread_create(macosx_exception_thread_status *s);
void macosx_exception_thread_destroy(macosx_exception_thread_status *s);
void macosx_exception_get_write_lock(macosx_exception_thread_status *s);
void macosx_exception_release_write_lock(macosx_exception_thread_status *s);

#endif /* __GDB_MACOSX_NAT_EXCTHREAD_H__ */

/* EOF */
