AC_DEFUN([GDB_CHECK_PTRACE],[
AC_REQUIRE([AC_TYPE_PID_T])dnl

# Check the return and argument types of ptrace. No canned test for
# this, so roll our own.
gdb_ptrace_headers='
#if defined(HAVE_SYS_TYPES_H) && HAVE_SYS_TYPES_H
# include <sys/types.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "this conftest expects <sys/types.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_SYS_TYPES_H */
#if defined(HAVE_SYS_PTRACE_H) && HAVE_SYS_PTRACE_H
# include <sys/ptrace.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "this conftest expects <sys/ptrace.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_SYS_PTRACE_H */
#if defined(HAVE_UNISTD_H) && HAVE_UNISTD_H
# include <unistd.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "this conftest expects <unistd.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_UNISTD_H */
'
# There is no point in checking if we do not have a prototype.
AC_CHECK_DECLS([ptrace],[],[
  : ${gdb_cv_func_ptrace_ret='int'}
  : ${gdb_cv_func_ptrace_args='int,int,long,long'}
],[${gdb_ptrace_headers}])
# Check return type:
AC_CACHE_CHECK([return type of ptrace],[gdb_cv_func_ptrace_ret],
  [AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
${gdb_ptrace_headers}
   ]],[[
extern int ptrace();
   ]])],[gdb_cv_func_ptrace_ret='int'],[gdb_cv_func_ptrace_ret='long'])
])dnl
AC_DEFINE_UNQUOTED([PTRACE_TYPE_RET],[${gdb_cv_func_ptrace_ret}],
                   [Define as the return type of ptrace.])
# Check argument types:
AC_CACHE_CHECK([types of arguments for ptrace],[gdb_cv_func_ptrace_args],[
for gdb_arg1 in 'int' 'long'; do
 for gdb_arg2 in 'pid_t' 'int' 'long'; do
  for gdb_arg3 in 'int *' 'caddr_t' 'int' 'long'; do
   for gdb_arg4 in 'int' 'long'; do
     AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[${gdb_ptrace_headers}]],[[
extern ${gdb_cv_func_ptrace_ret}
  ptrace(${gdb_arg1}, ${gdb_arg2}, ${gdb_arg3}, ${gdb_arg4});
]])],[gdb_cv_func_ptrace_args="${gdb_arg1},${gdb_arg2},${gdb_arg3},${gdb_arg4}";
    break 4;],[])
    for gdb_arg5 in 'int *' 'int' 'long'; do
     AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[${gdb_ptrace_headers}]],[[
extern ${gdb_cv_func_ptrace_ret}
  ptrace(${gdb_arg1}, ${gdb_arg2}, ${gdb_arg3}, ${gdb_arg4}, ${gdb_arg5});
]])],[
gdb_cv_func_ptrace_args="${gdb_arg1},${gdb_arg2},${gdb_arg3},${gdb_arg4},${gdb_arg5}";
    break 5;],[])
    done
   done
  done
 done
done
# Provide a safe default value:
: ${gdb_cv_func_ptrace_args='int,int,long,long'}
])
ac_save_IFS=${IFS}; IFS=','
set dummy `echo "${gdb_cv_func_ptrace_args}" | sed 's/\*/\*/g'`
IFS=${ac_save_IFS}
shift
if test "x$[3]" = "xcaddr_t"; then
  :
  AH_VERBATIM([01_HEADER_FOR_CADDR_T],[
/* make sure that caddr_t is defined for us: */
#if defined(HAVE_SYS_TYPES_H) && HAVE_SYS_TYPES_H
# include <sys/types.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "config.h expects <sys/types.h> to be included for caddr_t."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* HAVE_SYS_TYPES_H */
])dnl
fi
AC_DEFINE_UNQUOTED([PTRACE_TYPE_ARG3],$[3],
  [Define to the type of arg 3 for ptrace.])
if test -n "$[5]"; then
  AC_DEFINE_UNQUOTED([PTRACE_TYPE_ARG5],$[5],
    [Define to the type of arg 5 for ptrace.])
fi
])dnl
