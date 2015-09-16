/*
 * macosx-nat-cfmthread.h
 */

#ifndef __GDB_MACOSX_NAT_CFMTHREAD_H__
#define __GDB_MACOSX_NAT_CFMTHREAD_H__ 1

#if WITH_CFM

#include "defs.h"
#include "macosx-nat-mutils.h"
#include "macosx-nat-threads.h"
#include "macosx-nat-cfm.h"

#include <sys/wait.h>

struct macosx_cfm_thread_status
{
  CORE_ADDR notify_debugger;
  CORE_ADDR info_api_cookie;
  struct cfm_parser parser;
  struct breakpoint *cfm_breakpoint;
};
typedef struct macosx_cfm_thread_status macosx_cfm_thread_status;

void macosx_cfm_thread_init(macosx_cfm_thread_status *s);
void macosx_cfm_thread_create(macosx_cfm_thread_status *s);
void macosx_cfm_thread_destroy(macosx_cfm_thread_status *s);

#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "macosx-nat-cfmthread.h is useless unless WITH_CFM is defined."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* WITH_CFM */

#endif /* __GDB_MACOSX_NAT_CFMTHREAD_H__ */

/* EOF */
