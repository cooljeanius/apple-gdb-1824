/*
 * macosx/macosx-nat-utils.h
 */

#ifndef __GDB_MACOSX_NAT_UTILS_H__
#define __GDB_MACOSX_NAT_UTILS_H__

#ifndef NO_POISON
/* include chain from <mach/mach.h> can drag in "free": */
# define NO_POISON 1
#endif /* !NO_POISON */

#ifndef __has_include
# define __has_include(foo) 0
#endif /* !__has_include */

#if defined(HAVE_MACH_MACH_H) || __has_include(<mach/mach.h>)
# include <mach/mach.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "macosx/macosx-nat-utils.h expects <mach/mach.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_MACH_MACH_H */

#if (defined __GNUC__)
# define __MACH_CHECK_FUNCTION __extension__ __PRETTY_FUNCTION__
#else
# define __MACH_CHECK_FUNCTION ((__const char *)0)
#endif /* __GNUC__ */

#define MACH_PROPAGATE_ERROR(ret) \
{ MACH_WARN_ERROR(ret); if ((ret) != KERN_SUCCESS) { return ret; } }

#define MACH_CHECK_ERROR(ret) \
  mach_check_error(ret, __FILE__, __LINE__, __MACH_CHECK_FUNCTION);

#define MACH_WARN_ERROR(ret) \
  mach_warn_error(ret, __FILE__, __LINE__, __MACH_CHECK_FUNCTION);

#define MACH_ERROR_STRING(ret) \
  (mach_error_string(ret) ? mach_error_string(ret) : "[UNKNOWN]")

const void *macosx_parse_plist(const char *plist_path);

void macosx_free_plist(const void **plist);

void macosx_print_extra_stop_info(int code, CORE_ADDR address);

struct value *macosx_load_dylib(char *name, char *flags);

int macosx_check_safe_call(int which,
                           enum check_which_threads thread_mode);

const char *macosx_get_plist_posix_value(const void *plist,
                                         const char* key);

const char *macosx_get_plist_string_value(const void *plist,
                                          const char* key);

char *macosx_filename_in_bundle(const char *filename, int mainline);

void mach_check_error(kern_return_t ret, const char *file,
                      unsigned int line, const char *func);

void mach_warn_error(kern_return_t ret, const char *file,
                     unsigned int line, const char *func);

#endif /* __GDB_MACOSX_NAT_UTILS_H__ */

/* EOF */
