/*
 * macosx-mutils.h
 */

#ifndef __GDBSERVER_MACOSX_MUTILS_H__
#define __GDBSERVER_MACOSX_MUTILS_H__

#ifdef HAVE_CONFIG_H
# include "config.h"
#else
# warning not including "config.h".
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_MACH_PORT_H
# include <mach/port.h>
#else
# warning macosx-mutils.h expects <mach/port.h> to be included.
#endif /* HAVE_MACH_PORT_H */

#if MACH64
# ifdef HAVE_MACH_MACH_VM_H
#  include <mach/mach_vm.h>
# else
#  warning macosx-mutils.h expects <mach/mach_vm.h> to be included.
# endif /* HAVE_MACH_MACH_VM_H */
#else
# define NOT_MACH64_IN_MACOSX_MUTILS_H 1
#endif /* MACH64 */

#ifdef HAVE_UNSIGNED_INT
# ifndef HAVE_VM_SIZE_T
#  warning We do not have the vm_size_t type, which this file needs.
/* Try defining it as an "unsigned int". */
# endif /* !HAVE_VM_SIZE_T */
#else
# warning macosx-mutils.h expects the "unsigned int" type to be available.
#endif /* HAVE_UNSIGNED_INT */

/* This function prototype used to be of type "unsigned int", but
 * macosx-mutils.c has child_get_pagesize as being of type vm_size_t, so trying
 * that... */
vm_size_t child_get_pagesize (void);

int mach_xfer_memory (CORE_ADDR memaddr, char *myaddr, int len, int write,
		      task_t task);

int macosx_thread_valid (task_t task, thread_t thread);

#endif /* __GDBSERVER_MACOSX_MUTILS_H__ */
