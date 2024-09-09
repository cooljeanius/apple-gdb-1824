/*
 * efence.h
 */

#ifndef EFENCE_H_INCLUDED
#define EFENCE_H_INCLUDED 1

#include "ansidecl.h" /* for ATTRIBUTE_NORETURN */

#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>

/*
 * ef_number is the largest unsigned integer we will need. On systems that
 * support 64-bit pointers, this may be "unsigned long long".
 */
#if defined(USE_LONG_LONG) && !defined(__STRICT_ANSI__)
typedef unsigned long long ef_number;
#else
typedef unsigned long ef_number;
#endif /* USE_LONG_LONG && !__STRICT_ANSI__ */

/*
 * NBBY is the number of bits per byte. Some systems define it in
 * <sys/param.h> .
 */
#ifndef	NBBY
# define NBBY 8
#endif /* !NBBY */

/*
 * This is used to declare functions with "C" linkage if we are compiling
 * with C++ .
 */
#ifdef __cplusplus
# define C_LINKAGE "C"
#else
# define C_LINKAGE
#endif /* __cplusplus */

extern C_LINKAGE void *efence_realloc(void *oldBuffer, size_t newSize);
extern C_LINKAGE void *efence_malloc(size_t size);
extern C_LINKAGE void *efence_calloc(size_t nelem, size_t elsize);
extern C_LINKAGE void efence_free(void *address);
extern C_LINKAGE void *efence_valloc(size_t size);
extern C_LINKAGE void *efence_memalign(size_t alignment, size_t userSize);

void Page_AllowAccess(void *address, size_t size);
void *Page_Create(size_t size);
void Page_Delete(void *address, size_t size);
void Page_DenyAccess(void *address, size_t size);
size_t Page_Size(void);

void ATTRIBUTE_NORETURN EF_Abort(const char *message, ...);
void EF_Exit(const char *message, ...);
void EF_Print(const char *message, ...);
void EF_Lock(void);
void EF_UnLock(void);

#endif /* EFENCE_H_INCLUDED */

/* EOF */
