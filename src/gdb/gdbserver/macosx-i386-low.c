/*
 * macosx-i386-low.c
 */

/* TODO: Put more stuff in this file */

#ifndef _MACOSX_INTEL_LOW_C
# define _MACOSX_INTEL_LOW_C 1
# ifdef HAVE_CONFIG_H
#  include "config.h"
# else
#  define MACOSX_INTEL_LOW_C_NON_AUTOTOOLS_BUILD 1
#  if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#   warning not including "config.h".
#  endif /* __GNUC__ && !__STRICT_ANSI__ */
# endif /* HAVE_CONFIG_H */
# ifdef HAVE_SYS_WAIT_H
#  include <sys/wait.h>
# else
#  if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#   warning "macosx-i386-low.c expects <sys/wait.h> to be included."
#  endif /* __GNUC__ && !__STRICT_ANSI__ */
# endif /* HAVE_SYS_WAIT_H */
# ifdef HAVE_STDIO_H
#  include <stdio.h>
# else
#  if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#   warning "macosx-i386-low.c expects <stdio.h> to be included."
#  endif /* __GNUC__ && !__STRICT_ANSI__ */
# endif /* HAVE_STDIO_H */
# ifdef HAVE_SYS_PARAM_H
#  include <sys/param.h>
# else
#  if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#   warning "macosx-i386-low.c expects <sys/param.h> to be included."
#  endif /* __GNUC__ && !__STRICT_ANSI__ */
# endif /* HAVE_SYS_PARAM_H */
# ifdef HAVE_SYS_PTRACE_H
#  include <sys/ptrace.h>
# else
#  if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#   warning "macosx-i386-low.c wants to include <sys/ptrace.h>"
#  endif /* __GNUC__ && !__STRICT_ANSI__ */
# endif /* HAVE_SYS_PTRACE_H */
# ifdef HAVE_SIGNAL_H
#  include <signal.h>
# else
#  if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#   warning "macosx-i386-low.c expects <signal.h> to be included."
#  endif /* __GNUC__ && !__STRICT_ANSI__ */
# endif /* HAVE_SYS_WAIT_H */
# ifdef HAVE_STRING_H
#  include <string.h>
# else
#  ifdef HAVE_STRINGS_H
#   include <strings.h>
#  else
#   if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#    warning "macosx-i386-low.c expects either <string.h> or <strings.h> to be included."
#   endif /* __GNUC__ && !__STRICT_ANSI__ */
#  endif /* HAVE_STRINGS_H */
# endif /* HAVE_STRING_H */
# ifdef HAVE_STDLIB_H
#  include <stdlib.h>
# else
#  if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#   warning "macosx-i386-low.c expects <stdlib.h> to be included."
#  endif /* __GNUC__ && !__STRICT_ANSI__ */
# endif /* HAVE_STDLIB_H */
# ifdef HAVE_UNISTD_H
#  include <unistd.h>
# else
#  if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#   warning "macosx-i386-low.c expects <unistd.h> to be included."
#  endif /* __GNUC__ && !__STRICT_ANSI__ */
# endif /* HAVE_UNISTD_H */

# ifdef HAVE_MACH_MACH_H
#  include <mach/mach.h>
# else
#  if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#   warning "macosx-i386-low.c expects <mach/mach.h> to be included."
#  endif /* __GNUC__ && !__STRICT_ANSI__ */
# endif /* HAVE_MACH_MACH_H */

# include "macosx-low.h"
# include "server.h"
# include "i387-fp.h"
# if defined(HAVE_FPREGSET_T) && defined(HAVE_GREGSET_T)
#  include "../gdb_proc_service.h"
# endif /* HAVE_FPREGSET_T && HAVE_GREGSET_T */

/* the autoconf configure script checks the length of this file to determine
 * whether to print this warning: */
# ifdef MACOSX_I386_LOW_C_LENGTH_C_DEFINE
#  if (MACOSX_I386_LOW_C_LENGTH_C_DEFINE < 150)
#   if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#    warning "This file (macosx-i386-low.c) does not really do much yet, try adding more stuff to it."
#   endif /* __GNUC__ && !__STRICT_ANSI__ */
#  endif /* < 150 */
# else
#  define UNSURE_OF_LENGTH_OF_THIS_FILE 1
# endif /* MACOSX_I386_LOW_C_LENGTH_C_DEFINE */

# ifndef HAVE_ELF_FPREGSET_T
/* Make sure we have said types. Not all platforms bring in <linux/elf.h>
 * via <sys/procfs.h>.  */
#  ifdef HAVE_LINUX_ELF_H
#   include <linux/elf.h>
#   include "linux-low.h"
#  endif /* HAVE_LINUX_ELF_H */
# endif /* !HAVE_ELF_FPREGSET_T */

# ifdef HAVE_SYS_REG_H
#  include <sys/reg.h>
# else
#  if !defined(__APPLE__) && defined(__GNUC__) && !defined(__STRICT_ANSI__)
#   warning "macosx-i386-low.c wants to include <sys/reg.h> but it is not available, consider removing this include."
#  endif /* !__APPLE__ && __GNUC__ && !__STRICT_ANSI__ */
# endif /* HAVE_SYS_REG_H */
# ifdef HAVE_SYS_PROCFS_H
#  include <sys/procfs.h>
# else
#  if !defined(__APPLE__) && defined(__GNUC__) && !defined(__STRICT_ANSI__)
#   warning "macosx-i386-low.c wants to include <sys/procfs.h> but it is not available, consider removing this include."
#  endif /* !__APPLE__ && __GNUC__ && !__STRICT_ANSI__ */
# endif /* HAVE_SYS_PTRACE_H */

/* Correct for all GNU/Linux targets (for quite some time).  */
# ifndef GDB_GREGSET_T
#  define GDB_GREGSET_T elf_gregset_t
# endif /* !GDB_GREGSET_T */
# ifndef GDB_FPREGSET_T
#  define GDB_FPREGSET_T elf_fpregset_t
# endif /* GDB_FPREGSET_T */

/* This definition comes from prctl.h, but some kernels may not have it: */
# ifndef PTRACE_ARCH_PRCTL
#  define PTRACE_ARCH_PRCTL 30
# endif /* !PTRACE_ARCH_PRCTL */

/* The following definitions come from prctl.h, but may be absent
 * for certain configurations.  */
# ifndef ARCH_GET_FS
#  define ARCH_SET_GS 0x1001
#  define ARCH_SET_FS 0x1002
#  define ARCH_GET_FS 0x1003
#  define ARCH_GET_GS 0x1004
# endif /* !ARCH_GET_FS */

# if defined(HAVE_LINUX_USRREGS) && defined(HAVE_LINUX_REGSETS)
static int x86_64_regmap[] = {
	RAX * 8, RBX * 8, RCX * 8, RDX * 8,
	RSI * 8, RDI * 8, RBP * 8, RSP * 8,
	R8 * 8, R9 * 8, R10 * 8, R11 * 8,
	R12 * 8, R13 * 8, R14 * 8, R15 * 8,
	RIP * 8, EFLAGS * 8, CS * 8, SS * 8,
	DS * 8, ES * 8, FS * 8, GS * 8
};
#  ifndef X86_64_NUM_GREGS
#   define X86_64_NUM_GREGS (sizeof(x86_64_regmap) / sizeof(int))
#  endif /* !X86_64_NUM_GREGS */

/* These next two functions need to stay inside the same ifdef that the
 * declaration of x86_64_regmap[] is inside of. */
static void x86_64_fill_gregset(void *buf)
{
  int i;

  for ((i = 0); (i < X86_64_NUM_GREGS); i++) {
    collect_register(i, (((char *)buf) + x86_64_regmap[i]));
  }
}

static void x86_64_store_gregset(const void *buf)
{
  int i;

  for ((i = 0); (i < X86_64_NUM_GREGS); i++) {
    supply_register(i, (((char *)buf) + x86_64_regmap[i]));
  }
}
# endif /* HAVE_LINUX_USRREGS && HAVE_LINUX_REGSETS */

#if defined(TARGET_X86_64) || defined(HOST_X86_64) || defined(__x86_64__)
static void x86_64_fill_fpregset(void *buf)
{
  i387_cache_to_fxsave(buf);
} /* unused */
#endif /* x86_64 */

#if defined(TARGET_X86_64) || defined(HOST_X86_64) || defined(__x86_64__)
static void x86_64_store_fpregset(const void *buf)
{
  i387_fxsave_to_cache(buf);
} /* unused */
#endif /* x86_64 */

struct macosx_target_ops the_low_target = {
  (void (*)(int))-1,
  NULL,
  NULL,
  NULL,
}; /* copied and pasted from linux-x86-64-low.c; not sure if that will
    * work for OSX... */

#endif /* !_MACOSX_INTEL_LOW_C */

/* EOF */
