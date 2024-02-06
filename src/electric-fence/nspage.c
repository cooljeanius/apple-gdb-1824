/* -*- C -*-
 * nspage.c
 */

#include "efence.h"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#if !defined(__has_include)
# define __has_include(foo) 0
#endif /* !__has_include */

#if defined(HAVE_MACH_MACH_H) || __has_include(<mach/mach.h>) || \
    defined(__MACH__) || defined(__APPLE__)
# include <mach/mach.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "nspage.c expects <mach/mach.h> to be included."
# endif /* __GNUC__ && !defined(__STRICT_ANSI__) */
# ifndef KERN_SUCCESS
#  define KERN_SUCCESS 0
# endif /* !KERN_SUCCESS */
# ifndef _MACH_I386_KERN_RETURN_H_
#  define _MACH_I386_KERN_RETURN_H_ 1
#  if defined(__i386__) || defined(__x86_64__)
#   ifndef ASSEMBLER
typedef int kern_return_t;
#   endif /* !ASSEMBLER */
#  endif /* __i386__ || __x86_64__ */
# endif /* !_MACH_I386_KERN_RETURN_H_ */
# ifndef _MACH_VM_PROT_H_
#  define _MACH_VM_PROT_H_ 1
typedef int vm_prot_t;
#  ifndef VM_PROT_NONE
#   define VM_PROT_NONE ((vm_prot_t)0x00)
#  endif /* !VM_PROT_NONE */
#  ifndef VM_PROT_READ
#   define VM_PROT_READ ((vm_prot_t)0x01)
#  endif /* !VM_PROT_READ */
#  ifndef VM_PROT_WRITE
#   define VM_PROT_WRITE ((vm_prot_t)0x02)
#  endif /* !VM_PROT_WRITE */
# endif /* !_MACH_VM_PROT_H_ */
# ifndef _MACH_VM_TYPES_H_
#  define _MACH_VM_TYPES_H_ 1
#  ifndef _MACH_I386_VM_TYPES_H_
#   define _MACH_I386_VM_TYPES_H_ 1
#   if defined(__i386__) || defined(__x86_64__)
#    ifndef ASSEMBLER
typedef unsigned int natural_t;
#     if defined(__LP64__) && (defined(HAVE_UINTPTR_T) || defined(uintptr_t))
typedef uintptr_t vm_offset_t;
typedef uintptr_t vm_size_t;
#     else
typedef natural_t vm_offset_t;
typedef natural_t vm_size_t;
#     endif  /* __LP64__ && (HAVE_UINTPTR_T || uintptr_t) */
typedef vm_offset_t vm_address_t;
#    endif /* !ASSEMBLER */
#   endif /* __i386__ || __x86_64__ */
#  endif /* !_MACH_I386_VM_TYPES_H_ */
# endif /* !_MACH_VM_TYPES_H_ */
#endif /* HAVE_MACH_MACH_H || __MACH__ || __APPLE__ */

#if (!defined(__GNUC__) || (__GNUC__ < 2) || __GNUC_MINOR__ < (defined __cplusplus ? 6 : 4))
# define __MACH_CHECK_FUNCTION ((__const char *)0)
#else
# if defined(__PRETTY_FUNCTION__) && !defined(__STRICT_ANSI__)
#  define __MACH_CHECK_FUNCTION __PRETTY_FUNCTION__
# else
#  if defined(NULL) || defined(__STDC__)
#   define __MACH_CHECK_FUNCTION NULL
#  else
#   define __MACH_CHECK_FUNCTION ((void *)0)
#  endif /* NULL || __STDC__ */
# endif /* __PRETTY_FUNCTION__ && !__STRICT_ANSI__ */
#endif /* ? */

#define MACH_CHECK_ERROR(ret) \
  mach_check_error(ret, __FILE__, __LINE__, __MACH_CHECK_FUNCTION);

#ifndef MACH_CHECK_NOERROR
# define MACH_CHECK_NOERROR(ret) \
   mach_check_noerror(ret, __FILE__, __LINE__, __MACH_CHECK_FUNCTION);
#endif /* !MACH_CHECK_NOERROR */

/* */
static void mach_check_error(kern_return_t ret, const char *file,
                             unsigned int line, const char *func)
{
  if (ret == KERN_SUCCESS) {
    return;
  }
  if (func == NULL) {
    func = "[UNKNOWN]";
  }

  EF_Exit("fatal Mach error on line %u of \"%s\" in function \"%s\": %s\n",
          line, file, func, mach_error_string(ret));
}

/* */
void *Page_Create(size_t size)
{
  kern_return_t kret;
  vm_address_t address = 0;

  kret = vm_allocate(mach_task_self(), &address, size, 1);
  MACH_CHECK_ERROR(kret);

  return ((void *)address);
}

/* */
void Page_AllowAccess(void *address, size_t size)
{
  kern_return_t kret;

  kret = vm_protect(mach_task_self(), (vm_address_t)address, size, 0,
                    (VM_PROT_READ | VM_PROT_WRITE));
  MACH_CHECK_ERROR(kret);
}

/* */
void Page_DenyAccess(void *address, size_t size)
{
  kern_return_t kret;

  kret = vm_protect(mach_task_self(), (vm_address_t)address, size, 0,
                    VM_PROT_NONE);
  MACH_CHECK_ERROR(kret);
}

/* */
void Page_Delete(void *address, size_t size)
{
  kern_return_t kret;

  kret = vm_deallocate(mach_task_self(), (vm_address_t)address, size);
  MACH_CHECK_ERROR(kret);
}

/* */
size_t Page_Size(void)
{
#if defined(__MACH30__)
  kern_return_t result;
  vm_size_t page_size = 0;

  result = host_page_size(mach_host_self(), &page_size);
  MACH_CHECK_ERROR(result);

  return (size_t)page_size;
#else /* ! __MACH30__ */
  static struct vm_statistics stats_data;
  static struct vm_statistics *stats = NULL;
  kern_return_t kret;

  if (stats == NULL) {
    kret = vm_statistics(mach_task_self(), &stats_data);
    MACH_CHECK_ERROR(kret);

    stats = &stats_data;
  }

  return stats->pagesize;
#endif /* ! __MACH30__ */
}

#ifdef MACH_CHECK_NOERROR
# undef MACH_CHECK_NOERROR
#endif /* MACH_CHECK_NOERROR */

/* EOF */
