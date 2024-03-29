/* libiberty.h: Function declarations for libiberty.

   Copyright 2001, 2002, 2005 Free Software Foundation, Inc.

   Note - certain prototypes declared in this header file are for
   functions whoes implementation copyright does not belong to the
   FSF.  Those prototypes are present in this file for reference
   purposes only and their presence in this file should not construed
   as an indication of ownership by the FSF of the implementation of
   those functions in any way or form whatsoever.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street - Fifth Floor,
   Boston, MA 02110-1301, USA.

   Written by Cygnus Support, 1994.

   The libiberty library provides a number of functions which are
   missing on some operating systems.  We do not declare those here,
   to avoid conflicts with the system header files on operating
   systems that do support those functions.  In this file we only
   declare those functions which are specific to libiberty.  */

#ifndef LIBIBERTY_H
#define LIBIBERTY_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef _ANSIDECL_H
# include "ansidecl.h"
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__) && defined(DEBUG)
 #  warning "we seem to have already included ansidecl.h once"
# endif /* __GNUC__ && !__STRICT_ANSI__ && DEBUG */
#endif /* !_ANSIDECL_H */

/* Get a definition for size_t: */
#include <stddef.h>
/* Get a definition for va_list: */
#include <stdarg.h>

#include <stdio.h>

/* If the OS supports it, ensure that the supplied stream is setup to
   avoid any multi-threaded locking.  Otherwise leave the FILE pointer
   unchanged.  If the stream is NULL, then do nothing.  */
extern void unlock_stream(FILE *);

/* If the OS supports it, ensure that the standard I/O streams, stdin,
   stdout and stderr are setup to avoid any multi-threaded locking.
   Otherwise do nothing.  */
extern void unlock_std_streams(void);

/* Open and return a FILE pointer.  If the OS supports it, ensure that
   the stream is setup to avoid any multi-threaded locking.  Otherwise
   return the FILE pointer unchanged.  */
extern FILE *fopen_unlocked(const char *, const char *) ATTRIBUTE_W_U_R;
extern FILE *fdopen_unlocked(int, const char *) ATTRIBUTE_W_U_R;
extern FILE *freopen_unlocked(const char *, const char *, FILE *)
  ATTRIBUTE_W_U_R;

/* Build an argument vector from a string.  Allocates memory using
   malloc.  Use freeargv to free the vector.  */
extern char **buildargv(const char *) ATTRIBUTE_MALLOC ATTRIBUTE_W_U_R;

/* Free a vector returned by buildargv: */
extern void freeargv(char **);

/* Duplicate an argument vector. Allocates memory using malloc.  Use
   freeargv to free the vector.  */
extern char **dupargv(char *const *) ATTRIBUTE_MALLOC ATTRIBUTE_W_U_R;

/* Expand "@file" arguments in argv: */
extern void expandargv(int *, char ***);

/* Write argv to an @-file, inserting necessary quoting: */
extern int writeargv(char *const *, FILE *);

/* Return the number of elements in argv: */
extern int countargv(char *const *) ATTRIBUTE_W_U_R;

/* Return the last component of a path name.  Note that we cannot use a
   prototype here because the parameter is declared inconsistently
   across different systems, sometimes as "char *" and sometimes as
   "const char *" */

/* HAVE_DECL_* is a three-state macro: undefined, 0 or 1.  If it is
   undefined, we have NOT run the autoconf check so provide the
   declaration without arguments.  If it is 0, we checked and failed
   to find the declaration so provide a fully prototyped one.  If it
   is 1, then we found it, so do NOT provide any declaration at all.  */
#if !HAVE_DECL_BASENAME
  /* APPLE LOCAL basename */
# if defined(__GNU_LIBRARY__ ) || defined(__linux__) \
     || defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__) \
     || defined(__APPLE__) || defined(__CYGWIN__) || defined(__CYGWIN32__) \
     || defined(__MINGW32__) || defined(__DragonFly__) || defined(HAVE_DECL_BASENAME)
extern char *basename(char *);
# else
/* Do not allow basename to be used if there is no prototype seen.  We
 * either need to use the above prototype or have one from
 * autoconf which would result in HAVE_DECL_BASENAME being set.  */
#  define basename basename_cannot_be_used_without_a_prototype
# endif /* platform-specific macros */
#endif /* !HAVE_DECL_BASENAME */

/* A well-defined basename() that is always compiled in: */
extern const char *lbasename(const char *) ATTRIBUTE_W_U_R;

/* Same, but assumes DOS semantics (drive name, backslash is also a
 * dir separator) regardless of host: */
extern const char *dos_lbasename(const char *) ATTRIBUTE_W_U_R;

/* Same, but assumes Unix semantics (absolute paths always start with
 * a slash, only forward slash is accepted as dir separator)
 * regardless of host: */
extern const char *unix_lbasename(const char *) ATTRIBUTE_W_U_R;

/* A well-defined realpath() that is always compiled in: */
extern char *lrealpath(const char *) ATTRIBUTE_W_U_R;

/* Concatenate an arbitrary number of strings.  You must pass NULL as
   the last argument of this function, to terminate the list of
   strings.  Allocates memory using xmalloc.  */
extern char *concat(const char *, ...)
  ATTRIBUTE_MALLOC ATTRIBUTE_SENTINEL ATTRIBUTE_W_U_R;

/* Concatenate an arbitrary number of strings.  You must pass NULL as
   the last argument of this function, to terminate the list of
   strings.  Allocates memory using xmalloc.  The first argument is
   not one of the strings to be concatenated, but if not NULL is a
   pointer to be freed after the new string is created, similar to the
   way xrealloc works.  */
extern char *reconcat(char *, const char *, ...)
  ATTRIBUTE_MALLOC ATTRIBUTE_SENTINEL ATTRIBUTE_W_U_R;

/* Determine the length of concatenating an arbitrary number of
   strings.  You must pass NULL as the last argument of this function,
   to terminate the list of strings.  */
extern unsigned long concat_length(const char *, ...)
  ATTRIBUTE_SENTINEL ATTRIBUTE_W_U_R;

/* Concatenate an arbitrary number of strings into a SUPPLIED area of
   memory.  You must pass NULL as the last argument of this function,
   to terminate the list of strings.  The supplied memory is assumed
   to be large enough.  */
extern char *concat_copy(char *, const char *, ...)
  ATTRIBUTE_SENTINEL ATTRIBUTE_W_U_R;

/* Concatenate an arbitrary number of strings into a GLOBAL area of
   memory.  You must pass NULL as the last argument of this function,
   to terminate the list of strings.  The supplied memory is assumed
   to be large enough.  */
extern char *concat_copy2(const char *, ...)
  ATTRIBUTE_SENTINEL ATTRIBUTE_W_U_R;

/* This is the global area used by concat_copy2: */
extern char *libiberty_concat_ptr;

/* Concatenate an arbitrary number of strings.  You must pass NULL as
   the last argument of this function, to terminate the list of
   strings.  Allocates memory using alloca.  The arguments are
   evaluated twice!  */
#define ACONCAT(ACONCAT_PARAMS) \
  (libiberty_concat_ptr = (char *)alloca(concat_length ACONCAT_PARAMS + 1), \
   concat_copy2 ACONCAT_PARAMS)

/* Check whether two file descriptors refer to the same file: */
extern int fdmatch(int fd1, int fd2) ATTRIBUTE_W_U_R;

/* Return the position of the first bit set in the argument.  */
/* Prototypes vary from system to system, so we only provide a
   prototype on systems where we know that we need it.  */
#if defined(HAVE_DECL_FFS) && !HAVE_DECL_FFS
extern int ffs(int);
#endif /* !HAVE_DECL_FFS */

/* Get the working directory.  The result is cached, so do NOT call
   chdir() between calls to getpwd().  */
extern char *getpwd(void) ATTRIBUTE_W_U_R;

/* Get the current time.  */
/* Prototypes vary from system to system, so we only provide a
   prototype on systems where we know that we need it.  */
#ifdef __MINGW32__
/* Forward declaration to avoid #include <sys/time.h>.   */
struct timeval;
extern int gettimeofday(struct timeval *, void *);
#endif /* __MINGW32__ */

/* Get the amount of time the process has run, in microseconds: */
extern long get_run_time(void) ATTRIBUTE_W_U_R;

/* Generate a relocated path to some installation directory.  Allocates
   return value using malloc.  */
extern char *make_relative_prefix(const char *, const char *, const char *)
  ATTRIBUTE_MALLOC ATTRIBUTE_W_U_R;

/* Generate a relocated path to some installation directory without
 * attempting to follow any soft links. Allocates
 * return value using malloc.  */
extern char *make_relative_prefix_ignore_links(const char *, const char *,
                                               const char *)
  ATTRIBUTE_MALLOC ATTRIBUTE_W_U_R;

/* Choose a temporary directory to use for scratch files: */
extern char *choose_temp_base(void) ATTRIBUTE_MALLOC ATTRIBUTE_W_U_R;

/* Return a temporary file name or NULL if unable to create one: */
extern char *make_temp_file(const char *) ATTRIBUTE_MALLOC ATTRIBUTE_W_U_R;

/* Remove a link to a file unless it is special: */
extern int unlink_if_ordinary(const char *);

/* Allocate memory filled with spaces.  Allocates using malloc: */
extern const char *spaces(int count) ATTRIBUTE_MALLOC ATTRIBUTE_W_U_R;

/* Return the maximum error number for which strerror will return a
   string.  */
extern int errno_max(void) ATTRIBUTE_W_U_R;

/* Return the name of an errno value (e.g., strerrno(EINVAL) returns
   "EINVAL").  */
extern const char *strerrno(int) ATTRIBUTE_W_U_R;

/* Given the name of an errno value, return the value: */
extern int strtoerrno(const char *) ATTRIBUTE_W_U_R;

/* The strerror() function from ANSI, but more robust in that it will never
 * return a NULL pointer: */
extern char *xstrerror(int) ATTRIBUTE_W_U_R ATTRIBUTE_RETURNS_NONNULL;

/* Return the maximum signal number for which strsignal will return a
   string.  */
extern int signo_max(void) ATTRIBUTE_W_U_R;

/* Return a signal message string for a signal number
   (e.g., strsignal (SIGHUP) returns something like "Hangup").  */
/* This is ifdef-ed out as it can conflict with one in system headers.
   We still document its existence though.  */
#ifdef CONFLICT_WITH_SYSTEM_HEADERS_RESOLVED
extern const char *strsignal(int);
#endif /* CONFLICT_WITH_SYSTEM_HEADERS_RESOLVED */

/* Return the name of a signal number (e.g., strsigno (SIGHUP) returns
   "SIGHUP").  */
extern const char *strsigno(int) ATTRIBUTE_W_U_R;

/* Given the name of a signal, return its number: */
extern int strtosigno(const char *) ATTRIBUTE_W_U_R;

/* Register a function to be run by xexit.  Returns 0 on success: */
extern int xatexit(void (*fn)(void)) ATTRIBUTE_W_U_R;

/* Exit, calling all the functions registered with xatexit: */
extern void xexit(int status) ATTRIBUTE_NORETURN;

/* Set the program name used by xmalloc: */
extern void xmalloc_set_program_name(const char *);

  /* APPLE LOCAL xmalloc */
/* Set the implemtations used by xmalloc, xcalloc, xrealloc, and xfree: */
extern void xmalloc_set_malloc_hooks
	(PTR (*nmalloc)(size_t),
	 PTR (*ncalloc)(size_t, size_t),
	 PTR (*nrealloc)(PTR, size_t),
	 void (*nfree)(PTR));

/* Report an allocation failure.  */
extern void xmalloc_failed(size_t) ATTRIBUTE_NORETURN;

/* Allocate memory without fail.  If malloc fails, this will print a
   message to stderr (using the name set by xmalloc_set_program_name,
   if any) and then call xexit.  */

extern void *xmalloc(size_t) ATTRIBUTE_MALLOC ATTRIBUTE_W_U_R;

/* Reallocate memory without fail.  This works like xmalloc.  Note,
   realloc type functions are not suitable for attribute malloc since
   they may return the same address across multiple calls. */

extern void *xrealloc(void *, size_t) ATTRIBUTE_W_U_R;

/* Allocate memory without fail and set it to zero.  This works like
   xmalloc.  */

extern void *xcalloc(size_t, size_t) ATTRIBUTE_MALLOC ATTRIBUTE_W_U_R;

  /* APPLE LOCAL xmalloc */
/* Free memory allocated with xmalloc, xrealloc, or xcalloc: */
extern void xfree PARAMS((PTR));

/* Copy a string into a memory buffer without fail: */
extern char *xstrdup(const char *) ATTRIBUTE_MALLOC ATTRIBUTE_W_U_R;

/* Copy at most N characters from string into a buffer without fail: */
extern char *xstrndup(const char *, size_t) ATTRIBUTE_MALLOC ATTRIBUTE_W_U_R;

/* Copy an existing memory buffer to a new memory buffer without fail: */
extern void *xmemdup(const void *, size_t, size_t)
  ATTRIBUTE_MALLOC ATTRIBUTE_W_U_R;

/* Physical memory routines.  Return values are in BYTES: */
extern double physmem_total(void) ATTRIBUTE_W_U_R;
extern double physmem_available(void) ATTRIBUTE_W_U_R;

/* Compute the 32-bit CRC of a block of memory: */
extern unsigned int xcrc32(const unsigned char *, int, unsigned int)
  ATTRIBUTE_W_U_R;

/* These macros provide a K&R/C89/C++-friendly way of allocating structures
   with nice encapsulation.  The XDELETE*() macros are technically
   superfluous, but provided here for symmetry.  Using them consistently
   makes it easier to update client code to use different allocators such
   as new/delete and new[]/delete[].  */

/* Scalar allocators: */
#define XALLOCA(T)		((T *)alloca(sizeof(T)))
#define XNEW(T)			((T *)xmalloc(sizeof(T)))
#define XCNEW(T)		((T *)xcalloc(1, sizeof(T)))
#define XDUP(T, P)		((T *)xmemdup((P), sizeof(T), sizeof(T)))
#define XDELETE(P)		free((void*)(P))

/* Array allocators: */
#define XALLOCAVEC(T, N)	((T *)alloca(sizeof(T) * (N)))
#define XNEWVEC(T, N)		((T *)xmalloc(sizeof(T) * (N)))
#define XCNEWVEC(T, N)		((T *)xcalloc((N), sizeof(T)))
#define XDUPVEC(T, P, N) ((T *)xmemdup((P), sizeof(T) * (N), sizeof(T) * (N)))
#define XRESIZEVEC(T, P, N)   ((T *)xrealloc((void *)(P), sizeof(T) * (N)))
#define XDELETEVEC(P)		free((void*)(P))

/* Allocators for variable-sized structures and raw buffers: */
#define XALLOCAVAR(T, S)	((T *)alloca((S)))
#define XNEWVAR(T, S)		((T *)xmalloc((S)))
#define XCNEWVAR(T, S)		((T *)xcalloc(1, (S)))
#define XDUPVAR(T, P, S1, S2)	((T *)xmemdup((P), (S1), (S2)))
#define XRESIZEVAR(T, P, S)	((T *)xrealloc((P), (S)))

/* Type-safe obstack allocator: */
#define XOBNEW(O, T)		((T *)obstack_alloc((O), sizeof(T)))
#define XOBNEWVEC(O, T, N)	((T *)obstack_alloc((O), sizeof(T) * (N)))
#define XOBNEWVAR(O, T, S)	((T *)obstack_alloc((O), (S)))
#define XOBFINISH(O, T)         ((T)obstack_finish((O)))

/* hex character manipulation routines: */
#define _hex_array_size 256
#define _hex_bad	99
extern const unsigned char _hex_value[_hex_array_size];
extern void hex_init(void);
#define hex_p(c)	(hex_value (c) != _hex_bad)
/* If you change this, note well: Some code relies on side effects in
   the argument being performed exactly once.  */
#define hex_value(c)	((unsigned int)_hex_value[(unsigned char)(c)])

/* Flags for pex_init.  These are bits to be or'ed together.  */

/* Record subprocess times, if possible: */
#define PEX_RECORD_TIMES	0x1

/* Use pipes for communication between processes, if possible: */
#define PEX_USE_PIPES		0x2

/* Save files used for communication between processes: */
#define PEX_SAVE_TEMPS		0x4
  
/* Max number of alloca bytes per call before we must switch to malloc.
   
   ?? Swiped from gnulib's regex_internal.h header.  Is this actually
   the case?  This number seems arbitrary, though sane.
   
   The OS usually guarantees only one guard page at the bottom of the stack,
   and a page size can be as small as 4096 bytes.  So we cannot safely
   allocate anything larger than 4096 bytes.  Also care for the possibility
   of a few compiler-allocated temporary stack slots.  */
#ifndef MAX_ALLOCA_SIZE
# define MAX_ALLOCA_SIZE 4032
#endif /* !MAX_ALLOCA_SIZE */

/* Prepare to execute one or more programs, with standard output of
   each program fed to standard input of the next.
   FLAGS	As above.
   PNAME	The name of the program to report in error messages.
   TEMPBASE	A base name to use for temporary files; may be NULL to
   		use a random name.
   Returns NULL on error.  */

extern struct pex_obj *pex_init(int flags, const char *pname,
                                const char *tempbase)
  ATTRIBUTE_W_U_R;

/* Flags for pex_run.  These are bits to be or'ed together.  */

/* Last program in pipeline.  Standard output of program goes to
   OUTNAME, or, if OUTNAME is NULL, to standard output of caller.  Do
   not set this if you want to call pex_read_output.  After this is
   set, pex_run may no longer be called with the same struct
   pex_obj.  */
#define PEX_LAST		0x1

/* Search for program in executable search path: */
#define PEX_SEARCH		0x2

/* OUTNAME is a suffix: */
#define PEX_SUFFIX		0x4

/* Send program's standard error to standard output: */
#define PEX_STDERR_TO_STDOUT	0x8

/* Input file should be opened in binary mode.  This flag is ignored
   on Unix.  */
#define PEX_BINARY_INPUT	0x10

/* Output file should be opened in binary mode.  This flag is ignored
   on Unix.  For proper behaviour PEX_BINARY_INPUT and
   PEX_BINARY_OUTPUT have to match appropriately--i.e., a call using
   PEX_BINARY_OUTPUT should be followed by a call using
   PEX_BINARY_INPUT.  */
#define PEX_BINARY_OUTPUT	0x20

/* Capture stderr to a pipe. The output can be read by
 * calling pex_read_err and reading from the returned
 * FILE object. This flag may be specified only for
 * the last program in a pipeline.
 *
 * This flag is supported only on Unix and Windows: */
#define PEX_STDERR_TO_PIPE	0x40

/* Capture stderr in binary mode. This flag is ignored
 * on Unix: */
#define PEX_BINARY_ERROR	0x80

#ifndef PEX_STDOUT_APPEND
/* Append stdout to existing file instead of truncating it: */
# define PEX_STDOUT_APPEND	0x100
#endif /* !PEX_STDOUT_APPEND */

#ifndef PEX_STDERR_APPEND
/* Thes same as PEX_STDOUT_APPEND, but for STDERR: */
# define PEX_STDERR_APPEND	0x200
#endif /* !PEX_STDERR_APPEND */

/* Execute one program.  Returns NULL on success.  On error returns an
   error string (typically just the name of a system call); the error
   string is statically allocated.

   OBJ		Returned by pex_init.

   FLAGS	As above.

   EXECUTABLE	The program to execute.

   ARGV		NULL terminated array of arguments to pass to the program.

   OUTNAME	Sets the output file name as follows:

		PEX_SUFFIX set (OUTNAME may not be NULL):
		  TEMPBASE parameter to pex_init not NULL:
		    Output file name is the concatenation of TEMPBASE
		    and OUTNAME.
		  TEMPBASE is NULL:
		    Output file name is a random file name ending in
		    OUTNAME.
		PEX_SUFFIX not set:
		  OUTNAME not NULL:
		    Output file name is OUTNAME.
		  OUTNAME NULL, TEMPBASE not NULL:
		    Output file name is randomly chosen using
		    TEMPBASE.
		  OUTNAME NULL, TEMPBASE NULL:
		    Output file name is randomly chosen.

		If PEX_LAST is not set, the output file name is the
   		name to use for a temporary file holding stdout, if
   		any (there will not be a file if PEX_USE_PIPES is set
   		and the system supports pipes).  If a file is used, it
   		will be removed when no longer needed unless
   		PEX_SAVE_TEMPS is set.

		If PEX_LAST is set, and OUTNAME is not NULL, standard
   		output is written to the output file name.  The file
   		will not be removed.  If PEX_LAST and PEX_SUFFIX are
   		both set, TEMPBASE may not be NULL.

   ERRNAME	If not NULL, this is the name of a file to which
		standard error is written.  If NULL, standard error of
		the program is standard error of the caller.

   ERR		On an error return, *ERR is set to an errno value, or
   		to 0 if there is no relevant errno.
*/

extern const char *pex_run(struct pex_obj *obj, int flags,
			   const char *executable, char * const *argv,
			   const char *outname, const char *errname,
			   int *err) ATTRIBUTE_W_U_R;

/* As for pex_run(), but takes an extra parameter to enable the
 * environment for the child process to be specified.
 *
 * ENV		The environment for the child process, specified as
 * 		an array of character pointers. Each element of the
 * 		array should point to a string of the form VAR=VALUE,
 * 		with the exception of the last element which must be
 * 		a null pointer.
 */
extern const char *pex_run_in_environment(struct pex_obj *obj, int flags,
                                          const char *executable,
                                          char * const *argv,
                                          char * const *env,
                                          const char *outname,
                                          const char *errname, int *err)
  ATTRIBUTE_W_U_R;

/* Return a stream for a temporary file to pass to the first program
 * in the pipeline as input. The file name is chosen as for pex_run.
 * pex_run closes the file automatically; do NOT close it yourself. */
extern FILE *pex_input_file(struct pex_obj *obj, int flags,
                            const char *in_name)
  ATTRIBUTE_W_U_R;

/* Return a stream for a pipe connected to the standard input of the
 * first program in the pipeline. You must have passed
 * `PEX_USE_PIPES' to `pex_init'. Close the returned stream
 * yourself.  */
extern FILE *pex_input_pipe(struct pex_obj *obj, int binary)
  ATTRIBUTE_W_U_R;

/* Read the standard output of the last program to be executed.
 * pex_run can not be called after this. BINARY should be non-zero if
 * the file should be opened in binary mode; this is ignored on Unix.
 * Returns NULL on error. Do NOT call fclose on the returned FILE; it
 * will be closed by pex_free.  */
extern FILE *pex_read_output(struct pex_obj *, int binary)
  ATTRIBUTE_W_U_R;

/* Read the standard error of the last program to be executed.
 * pex_run can not be called after this. BINARY should be non-zero if
 * the file should be opened in binary mode; this is ignored on Unix.
 * Returns NULL on error. Do NOT call fclose on the returned FILE; it
 * will be closed by pex_free.  */
extern FILE *pex_read_err(struct pex_obj *, int binary)
  ATTRIBUTE_W_U_R;

/* Return exit status of all programs in VECTOR.  COUNT indicates the
   size of VECTOR.  The status codes in the vector are in the order of
   the calls to pex_run.  Returns 0 on error, 1 on success.  */

extern int pex_get_status(struct pex_obj *, int count, int *vector)
  ATTRIBUTE_W_U_R;

/* Return times of all programs in VECTOR.  COUNT indicates the size
   of VECTOR.  struct pex_time is really just struct timeval, but that
   is not portable to all systems.  Returns 0 on error, 1 on
   success.  */

struct pex_time
{
  unsigned long user_seconds;
  unsigned long user_microseconds;
  unsigned long system_seconds;
  unsigned long system_microseconds;
};

extern int pex_get_times(struct pex_obj *, int count,
                         struct pex_time *vector) ATTRIBUTE_W_U_R;

/* Clean up a pex_obj: */
extern void pex_free(struct pex_obj *);

/* Just execute one program.  Return value is as for pex_run.
   FLAGS	Combination of PEX_SEARCH and PEX_STDERR_TO_STDOUT.
   EXECUTABLE	As for pex_run.
   ARGV		As for pex_run.
   PNAME	As for pex_init.
   OUTNAME	As for pex_run when PEX_LAST is set.
   ERRNAME	As for pex_run.
   STATUS	Set to exit status on success.
   ERR		As for pex_run.
*/

extern const char *pex_one(int flags, const char *executable,
			   char * const *argv, const char *pname,
			   const char *outname, const char *errname,
			   int *status, int *err) ATTRIBUTE_W_U_R;

/* pexecute and pwait are the old pexecute interface, still here for
   backward compatibility.  Do NOT use these for new code.  Instead,
   use pex_init/pex_run/pex_get_status/pex_free, or pex_one.  */

/* Definitions used by the pexecute routine.  */

#define PEXECUTE_FIRST   1
#define PEXECUTE_LAST    2
#define PEXECUTE_ONE     (PEXECUTE_FIRST + PEXECUTE_LAST)
#define PEXECUTE_SEARCH  4
#define PEXECUTE_VERBOSE 8

/* Execute a program: */
extern int pexecute(const char *, char * const *, const char *,
                    const char *, char **, char **, int) ATTRIBUTE_W_U_R;

/* Wait for pexecute to finish: */
extern int pwait(int, int *, int) ATTRIBUTE_W_U_R;

#if (!defined(HAVE_DECL_ASPRINTF) || \
     (defined(HAVE_DECL_ASPRINTF) && !HAVE_DECL_ASPRINTF)) && !defined(asprintf)
/* Like sprintf but provides a pointer to malloc'd storage, which must be freed
 * by the caller: */
extern int asprintf(char **, const char *, ...) ATTRIBUTE_PRINTF_2;
#endif /* !HAVE_DECL_ASPRINTF */

#if !(defined(DEFS_H) && defined(__GDB_DEFS_H__))
/* Like asprintf but allocates memory without fail. This works like
 * xmalloc: */
extern char *xasprintf(const char *, ...) ATTRIBUTE_MALLOC ATTRIBUTE_PRINTF_1;
#endif /* !(DEFS_H && __GDB_DEFS_H__) */

#if !defined(HAVE_DECL_VASPRINTF) || (defined(HAVE_DECL_VASPRINTF) && !HAVE_DECL_VASPRINTF)
/* Like vsprintf but provides a pointer to malloc'd storage, which must be freed
 * by the caller: */
extern int vasprintf(char **, const char *, va_list) ATTRIBUTE_PRINTF(2,0);
#endif /* !HAVE_DECL_VASPRINTF */

/* Like vasprintf but allocates memory without fail. This works like
 * xmalloc. */
extern char *xvasprintf(const char *, va_list) ATTRIBUTE_MALLOC ATTRIBUTE_PRINTF(1,0);

#if defined(HAVE_DECL_SNPRINTF) && !HAVE_DECL_SNPRINTF
/* Like sprintf but prints at most N characters: */
extern int snprintf(char *, size_t, const char *, ...) ATTRIBUTE_PRINTF_3;
#endif /* !HAVE_DECL_SNPRINTF */

#if defined(HAVE_DECL_VSNPRINTF) && !HAVE_DECL_VSNPRINTF
/* Like vsprintf but prints at most N characters: */
extern int vsnprintf(char *, size_t, const char *, va_list) ATTRIBUTE_PRINTF(3,0);
#endif /* !HAVE_DECL_VSNPRINTF */

#if defined(HAVE_DECL_MEMMEM) && !HAVE_DECL_MEMMEM
extern void *memmem(const void *, size_t, const void *, size_t);
#endif /* !HAVE_DECL_MEMMEM */

#if defined(HAVE_DECL_MEMPCPY) && !HAVE_DECL_MEMPCPY
extern PTR mempcpy(PTR, const PTR, size_t);
#endif /* !HAVE_DECL_MEMPCPY */

#if defined(HAVE_DECL_STPNCPY) && !HAVE_DECL_STPNCPY
extern char *stpncpy(char *, const char *, size_t);
#endif /* !HAVE_DECL_STPNCPY */

#if defined(HAVE_DECL_STRNCMP) && !HAVE_DECL_STRNCMP
extern int strncmp(const char *, const char *, size_t);
#endif /* !HAVE_DECL_STRNCMP */

#if defined(HAVE_DECL_STRNDUP) && !HAVE_DECL_STRNDUP
extern char *strndup(const char *, size_t);
#endif /* !HAVE_DECL_STRNDUP */

#if defined(HAVE_DECL_STRNLEN) && !HAVE_DECL_STRNLEN
extern size_t strnlen(const char *, size_t);
#endif /* !HAVE_DECL_STRNLEN */

#if defined(HAVE_DECL_STRVERSCMP) && !HAVE_DECL_STRVERSCMP
/* Compare version strings: */
extern int strverscmp(const char *, const char *);
#endif /* !HAVE_DECL_STRVERSCMP */

/* Set the title of a process: */
extern void setproctitle(const char *name, ...);

/* Increase stack limit if possible: */
extern void stack_limit_increase(unsigned long);

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

/* Drastically simplified alloca configurator.  If we are using GCC,
   we use __builtin_alloca; otherwise we use the C alloca.  The C
   alloca is always available.  You can override GCC by defining
   USE_C_ALLOCA yourself.  The canonical autoconf macro C_ALLOCA is
   also set/unset as it is often used to indicate whether code needs
   to call alloca(0).  */
extern void *C_alloca(size_t) ATTRIBUTE_MALLOC ATTRIBUTE_W_U_R;
#undef alloca
#if (GCC_VERSION >= 2000) && !defined(USE_C_ALLOCA)
# define alloca(x) __builtin_alloca(x)
# undef C_ALLOCA
# define ASTRDUP(X) \
  (__extension__ ({ const char *const libiberty_optr = (X); \
   const unsigned long libiberty_len = (strlen(libiberty_optr) + 1); \
   char *const libiberty_nptr = (char *const)alloca(libiberty_len); \
   (char *)memcpy(libiberty_nptr, libiberty_optr, libiberty_len); }))
#else
# define alloca(x) C_alloca(x)
# undef USE_C_ALLOCA
# define USE_C_ALLOCA 1
# undef C_ALLOCA
# define C_ALLOCA 1
extern const char *libiberty_optr;
extern char *libiberty_nptr;
extern unsigned long libiberty_len;
# define ASTRDUP(X) \
  (libiberty_optr = (X), \
   libiberty_len = (strlen(libiberty_optr) + 1), \
   libiberty_nptr = (char *)alloca(libiberty_len), \
   (char *)memcpy(libiberty_nptr, libiberty_optr, libiberty_len))
#endif /* gcc 2+ && !USE_C_ALLOCA */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ! defined (LIBIBERTY_H) */

/* EOF */
