/*
 * page.c
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */
#include "efence.h"
#if !defined(__has_include)
# define __has_include(foo) 0
#endif /* !__has_include */
#if defined(HAVE_FEATURES_H) || __has_include(<features.h>)
# include <features.h>
#endif /* HAVE_FEATURES_H */
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

/*
 * Lots of systems are missing the definition of PROT_NONE.
 */
#ifndef	PROT_NONE
# define PROT_NONE 0
#endif /* !PROT_NONE */

/*
 * 386 BSD has MAP_ANON instead of MAP_ANONYMOUS.
 */
#if (!defined(MAP_ANONYMOUS) && defined(MAP_ANON))
# define MAP_ANONYMOUS MAP_ANON
#endif /* !MAP_ANONYMOUS && MAP_ANON */

/*
 * For some reason, I cannot find mprotect() in any of the headers on
 * IRIX or SunOS 4.1.2
 */
/* extern C_LINKAGE int mprotect(void * addr, size_t len, int prot); */

static caddr_t startAddr = (caddr_t)0;

/* FIXME: improve this preprocessor condition: */
#if (!defined(sgi) && !defined(_AIX) && (!defined(linux) || !defined(_BSD_SOURCE)) \
     && !defined(FIXINC_WRAP_STDIO_H_STDIO_STDARG_H) && !defined(_STDIO_H_) \
     && (!defined(_STDIO_H) || defined(__clang_analyzer__)))
extern int sys_nerr;
# if defined(HAVE_DECL_SYSNERR)
#  undef HAVE_DECL_SYSNERR
# endif /* HAVE_DECL_SYSNERR */
# define HAVE_DECL_SYSNERR 1
extern char *sys_errlist[];
# if defined(HAVE_DECL_SYS_ERRLIST)
#  undef HAVE_DECL_SYS_ERRLIST
# endif /* HAVE_DECL_SYS_ERRLIST */
# define HAVE_DECL_SYS_ERRLIST 1
#endif /* !sgi && !_AIX && (!linux || !_BSD_SOURCE) && !_STDIO_H_, etc. */

static const char *
stringErrorReport(void)
{
#if defined(sgi)
	return strerror(oserror());
#elif defined(_AIX)
	return strerror(errno);
#else
# if HAVE_DECL_SYS_NERR && HAVE_DECL_SYS_ERRLIST
	if ((errno > 0) && (errno < sys_nerr))
		return sys_errlist[errno];
# endif /* HAVE_DECL_SYS_NERR && HAVE_DECL_SYS_ERRLIST */
    return "Unknown error.\n";
#endif /* sgi || _AIX */
}

/*
 * Create memory.
 */
#if defined(MAP_ANONYMOUS)
void *
Page_Create(size_t size)
{
	caddr_t allocation;

	/*
	 * In this version, "startAddr" is a _hint_, not a demand.
	 * When the memory I map here is contiguous with other
	 * mappings, the allocator can coalesce the memory from two
	 * or more mappings into one large contiguous chunk, and thus
	 * might be able to find a fit that would not otherwise have
	 * been possible. I could _force_ it to be contiguous by using
	 * the MMAP_FIXED flag, but I do NOT want to stomp on memory mappings
	 * generated by other software, etc.
	 */
	allocation = (caddr_t)mmap(startAddr, size, (PROT_READ | PROT_WRITE),
                               (MAP_PRIVATE | MAP_ANONYMOUS), -1, 0);

# ifndef __hpux
	/*
	 * Set the "address hint" for the next mmap() so that it will abut
	 * the mapping we just created.
	 *
	 * HP/UX 9.01 has a kernel bug that makes mmap() fail sometimes
	 * when given a non-zero address hint, so we will leave the hint set
	 * to zero on that system. HP recently told me this is now fixed.
	 * Someone please tell me when it is probable to assume that most
	 * of those systems that were running 9.01 have been upgraded.
	 */
	startAddr = (allocation + size);
# endif /* !__hpux */

	if (allocation == (caddr_t)-1)
		EF_Exit("mmap() failed: %s", stringErrorReport());

	return (void *)allocation;
}
#else
void *
Page_Create(size_t size)
{
	static int devZeroFd = -1;
	caddr_t allocation;

	if (devZeroFd == -1) {
		devZeroFd = open("/dev/zero", O_RDWR);
		if (devZeroFd < 0)
			EF_Exit("open() on /dev/zero failed: %s", stringErrorReport());
	}

	/*
	 * In this version, "startAddr" is a _hint_, not a demand.
	 * When the memory I map here is contiguous with other
	 * mappings, the allocator can coalesce the memory from two
	 * or more mappings into one large contiguous chunk, and thus
	 * might be able to find a fit that would not otherwise have
	 * been possible. I could _force_ it to be contiguous by using
	 * the MMAP_FIXED flag, but I do NOT want to stomp on memory mappings
	 * generated by other software, etc.
	 */
	allocation = (caddr_t)mmap(startAddr, size, (PROT_READ | PROT_WRITE),
                               MAP_PRIVATE, devZeroFd, 0);

	startAddr = (allocation + size);

	if (allocation == (caddr_t)-1)
		EF_Exit("mmap() failed: %s", stringErrorReport());

	return (void *)allocation;
}
#endif /* MAP_ANONYMOUS */

static void
mprotectFailed(void)
{
	EF_Exit("mprotect() failed: %s", stringErrorReport());
}

void
Page_AllowAccess(void *address, size_t size)
{
	if (mprotect((caddr_t)address, size, (PROT_READ | PROT_WRITE)) < 0)
		mprotectFailed();
}

void
Page_DenyAccess(void *address, size_t size)
{
	if (mprotect((caddr_t)address, size, PROT_NONE) < 0)
		mprotectFailed();
}

void
Page_Delete(void *address, size_t size)
{
	if (munmap((caddr_t)address, size) < 0)
		Page_DenyAccess(address, size);
}

#if defined(_SC_PAGESIZE)
size_t
Page_Size(void)
{
	return (size_t)sysconf(_SC_PAGESIZE);
}
#elif defined(_SC_PAGE_SIZE)
size_t
Page_Size(void)
{
	return (size_t)sysconf(_SC_PAGE_SIZE);
}
#else
/* extern int getpagesize(); */
size_t
Page_Size(void)
{
	return getpagesize();
}
#endif /* _SC_PAGESIZE || _SC_PAGE_SIZE */

/* EOF */
