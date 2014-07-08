dnl# gdb/gdbserver/configure.in uses BFD_HAVE_SYS_PROCFS_TYPE.
sinclude(../../bfd/bfd.m4)

AC_DEFUN([SRV_CHECK_THREAD_DB],
[AC_CACHE_CHECK([for libthread_db],[srv_cv_thread_db],
 [old_LIBS="${LIBS}"
  LIBS="${LIBS} -lthread_db"
  AC_LINK_IFELSE([AC_LANG_SOURCE([[
void ps_pglobal_lookup() {}
void ps_pdread() {}
void ps_pdwrite() {}
void ps_lgetregs() {}
void ps_lsetregs() {}
void ps_lgetfpregs() {}
void ps_lsetfpregs() {}
void ps_get_thread_area() {}
void ps_getpid() {}
  ]],[[
td_ta_new();
  ]])],[srv_cv_thread_db="-lthread_db"],
       [srv_cv_thread_db=no

        if test "x${prefix}" = "/usr" || test "x${prefix}" = "xNONE"; then
          thread_db="/lib/libthread_db.so.1"
        else
          thread_db='${prefix}/lib/libthread_db.so.1'
        fi
        LIBS="${old_LIBS} `eval echo "${thread_db}"`"
        AC_LINK_IFELSE([AC_LANG_SOURCE([[
void ps_pglobal_lookup() {}
void ps_pdread() {}
void ps_pdwrite() {}
void ps_lgetregs() {}
void ps_lsetregs() {}
void ps_lgetfpregs() {}
void ps_lsetfpregs() {}
void ps_get_thread_area() {}
void ps_getpid() {}
        ]],[[
td_ta_new();
        ]])],[srv_cv_thread_db="${thread_db}"],
             [srv_cv_thread_db=no])
        ])
  LIBS="${old_LIBS}"
  ])
])

# GDB_PATH_NM
# ----------
# find the pathname to a BSD- or MS-compatible name lister
# copied from libtool, but with libtool-specific parts removed
AC_DEFUN([GDB_PATH_NM],[
AC_REQUIRE([AC_PROG_CC])dnl
AC_REQUIRE([AC_PROG_SED])dnl

AC_CACHE_CHECK([for BSD- or MS-compatible name lister (nm)],
               [gdb_cv_path_NM],
[if test -n "${NM}"; then
  # Let the user override the test.
  AC_ARG_VAR([NM],[path to a BSD- or MS-compatible name lister (nm)])
  gdb_cv_path_NM="${NM}"
else
  gdb_nm_to_check="${ac_tool_prefix}nm"
  if test -n "${ac_tool_prefix}" && test "x${build}" = "x${host}"; then
    gdb_nm_to_check="${gdb_nm_to_check} nm"
  fi
  for gdb_tmp_nm in ${gdb_nm_to_check}; do
    gdb_save_ifs="${IFS}"; IFS=${PATH_SEPARATOR}
    for ac_dir in ${PATH} /usr/ccs/bin/elf /usr/ccs/bin /usr/ucb /bin; do
      IFS="${gdb_save_ifs}"
      test -z "${ac_dir}" && ac_dir=.
      tmp_nm="${ac_dir}/${gdb_tmp_nm}"
      if test -f "${tmp_nm}" || test -f "${tmp_nm}${ac_exeext}"; then
	# Check to see if the nm accepts a BSD-compat flag.
	# Adding `sed 1q' prevents false positives on HP-UX, which says:
	#   nm: unknown option "B" ignored
	# Tru64's nm complains that /dev/null is an invalid object file
	case `"${tmp_nm}" -B /dev/null 2>&1 | sed '1q'` in
	*/dev/null* | *'Invalid file or object type'*)
	  gdb_cv_path_NM="${tmp_nm} -B"
	  break
	  ;;
	*)
	  case `"${tmp_nm}" -p /dev/null 2>&1 | sed '1q'` in
	  */dev/null*)
	    gdb_cv_path_NM="${tmp_nm} -p"
	    break
	    ;;
	  *)
	    gdb_cv_path_NM=${gdb_cv_path_NM="${tmp_nm}"} # keep the 1st match, but:
	    continue # so we can try to find one that supports BSD flags
	    ;;
	  esac
	  ;;
	esac
      fi
    done
    IFS="${gdb_save_ifs}"
  done
  : ${gdb_cv_path_NM=no}
fi])
if test "x${gdb_cv_path_NM}" != "xno"; then
  NM="${gdb_cv_path_NM}"
else
  # Failed to find any BSD compatible name lister, look for dumpbin.
  if test -n "${DUMPBIN}"; then
    # Let the user override the test.
    AC_ARG_VAR([DUMPBIN],[tool that can be used instead of nm])
    :
  else
    AC_CHECK_TOOLS([DUMPBIN],[dumpbin "link -dump"],[:])
    case `${DUMPBIN} -symbols /dev/null 2>&1 | sed '1q'` in
    *COFF*)
      DUMPBIN="${DUMPBIN} -symbols"
      ;;
    *)
      DUMPBIN=:
      ;;
    esac
  fi
  AC_SUBST([DUMPBIN])
  if test "x${DUMPBIN}" != "x:"; then
    NM="${DUMPBIN}"
  fi
fi
test -z "${NM}" && export NM=nm
AC_SUBST([NM])

AC_REQUIRE([AC_EXEEXT])dnl
AC_REQUIRE([AC_OBJEXT])dnl
AC_REQUIRE([AC_PROG_GREP])dnl

AC_CACHE_CHECK([the name lister (${NM}) interface],[gdb_cv_nm_interface],
  [gdb_cv_nm_interface="BSD nm"
  echo "int some_variable = 0;" > conftest.${ac_ext}
  (eval echo "\"\${as_me}:${LINENO}: ${ac_compile}\"" >&AS_MESSAGE_LOG_FD)
  (eval "${ac_compile}" 2>conftest.err)
  cat conftest.err >&AS_MESSAGE_LOG_FD
  (eval echo "\"\${as_me}:${LINENO}: ${NM} \\\"conftest.${ac_objext}\\\"\"" >&AS_MESSAGE_LOG_FD)
  (eval "${NM} \"conftest.${ac_objext}\"" 2>conftest.err > conftest.out)
  cat conftest.err >&AS_MESSAGE_LOG_FD
  (eval echo "\"\${as_me}:${LINENO}: output\"" >&AS_MESSAGE_LOG_FD)
  cat conftest.out >&AS_MESSAGE_LOG_FD
  if ${GREP} 'External.*some_variable' conftest.out > /dev/null; then
    gdb_cv_nm_interface="MS dumpbin"
  fi
  rm -rf conftest*])
test -n "${gdb_cv_nm_interface}" && test -z "${NM_INTERFACE}" && export NM_INTERFACE="${gdb_cv_nm_interface}"
AC_SUBST([NM_INTERFACE])

])# GDB_PATH_NM

# GDB_CMD_GLOBAL_SYMBOLS
# ----------------------
AC_DEFUN([GDB_CMD_GLOBAL_SYMBOLS],[
AC_REQUIRE([AC_CANONICAL_HOST])dnl
AC_REQUIRE([AC_PROG_CC])dnl
AC_REQUIRE([AC_PROG_AWK])dnl
AC_REQUIRE([GDB_PATH_NM])dnl
AC_REQUIRE([AC_PROG_SED])dnl
AC_REQUIRE([AC_PROG_EGREP])dnl
AC_REQUIRE([AC_PROG_GREP])dnl
AC_REQUIRE([AC_OBJEXT])dnl
AC_REQUIRE([AC_EXEEXT])dnl

# Check for command to grab raw symbol name followed by C symbol from nm:
AC_MSG_CHECKING([command to parse ${NM} output from ${CC} object])
AC_CACHE_VAL([gdb_cv_sys_global_symbol_pipe],[
# These are sane defaults that work on at least a few old systems.
# [They come from Ultrix.  What could be older than Ultrix?!!]

# Character class describing NM global symbol codes.
symcode='[[BCDEGRST]]'

# Regexp to match symbols that can be accessed directly from C.
sympat='\([[_A-Za-z]][[_A-Za-z0-9]]*\)'

# Define system-specific variables:
case ${host_os} in
aix*)
  symcode='[[BCDT]]'
  ;;
cygwin* | mingw* | pw32* | cegcc*)
  symcode='[[ABCDGISTW]]'
  ;;
hpux*)
  if test "x${host_cpu}" = "xia64"; then
    symcode='[[ABCDEGRST]]'
  fi
  ;;
irix* | nonstopux*)
  symcode='[[BCDEGRST]]'
  ;;
osf*)
  symcode='[[BCDEGQRST]]'
  ;;
solaris*)
  symcode='[[BDRT]]'
  ;;
sco3.2v5*)
  symcode='[[DT]]'
  ;;
sysv4.2uw2*)
  symcode='[[DT]]'
  ;;
sysv5* | sco5v6* | unixware* | OpenUNIX*)
  symcode='[[ABDT]]'
  ;;
sysv4)
  symcode='[[DFNSTU]]'
  ;;
esac

# If we are using GNU nm, then use its standard symbol codes.
case `${NM} -V 2>&1` in
*GNU* | *'with BFD'*)
  symcode='[[ABCDGIRSTW]]' ;;
esac

# Transform an extracted symbol line into a proper C declaration.
# Some systems (esp. on ia64) link data and code symbols differently,
# so use this general approach.
gdb_cv_sys_global_symbol_to_cdecl="sed -n -e 's/^T .* \(.*\)$/extern int \1();/p' -e 's/^$symcode* .* \(.*\)$/extern char \1;/p'"

# Transform an extracted symbol line into symbol name and symbol address:
gdb_cv_sys_global_symbol_to_c_name_address="sed -n -e 's/^: \([[^ ]]*\)[[ ]]*$/  {\\\"\1\\\", (void *) 0},/p' -e 's/^$symcode* \([[^ ]]*\) \([[^ ]]*\)$/  {\"\2\", (void *) \&\2},/p'"
gdb_cv_sys_global_symbol_to_c_name_address_lib_prefix="sed -n -e 's/^: \([[^ ]]*\)[[ ]]*$/  {\\\"\1\\\", (void *) 0},/p' -e 's/^$symcode* \([[^ ]]*\) \(lib[[^ ]]*\)$/  {\"\2\", (void *) \&\2},/p' -e 's/^$symcode* \([[^ ]]*\) \([[^ ]]*\)$/  {\"lib\2\", (void *) \&\2},/p'"

# Handle CRLF in mingw tool chain
opt_cr=""
case ${build_os} in
mingw*)
  opt_cr=`$ECHO 'x\{0,1\}' | tr x '\015'` # option cr in regexp
  ;;
esac

# Try without a prefix underscore, then with it.
for ac_symprfx in "" "_"; do

  # Transform symcode, sympat, and symprfx into a raw symbol and a C symbol
  symxfrm="\\1 $ac_symprfx\\2 \\2"

  # Write the raw and C identifiers.
  if test "x${gdb_cv_nm_interface}" = "xMS dumpbin"; then
    # Fake it for dumpbin and say T for any non-static function
    # and D for any global variable.
    # Also find C++ and __fastcall symbols from MSVC++,
    # which start with @ or ?.
    gdb_cv_sys_global_symbol_pipe="${AWK} ['"\
"     {last_section=section; section=\$ 3};"\
"     /^COFF SYMBOL TABLE/{for(i in hide) delete hide[i]};"\
"     /Section length .*#relocs.*(pick any)/{hide[last_section]=1};"\
"     \$ 0!~/External *\|/{next};"\
"     / 0+ UNDEF /{next}; / UNDEF \([^|]\)*()/{next};"\
"     {if(hide[section]) next};"\
"     {f=0}; \$ 0~/\(\).*\|/{f=1}; {printf f ? \"T \" : \"D \"};"\
"     {split(\$ 0, a, /\||\r/); split(a[2], s)};"\
"     s[1]~/^[@?]/{print s[1], s[1]; next};"\
"     s[1]~prfx {split(s[1],t,\"@\"); print t[1], substr(t[1],length(prfx))}"\
"     ' prfx=^$ac_symprfx]"
  else
    gdb_cv_sys_global_symbol_pipe="sed -n -e 's/^.*[[\t ]]\($symcode$symcode*\)[[\t ]][[\t ]]*$ac_symprfx$sympat$opt_cr$/$symxfrm/p'"
  fi
  gdb_cv_sys_global_symbol_pipe="${gdb_cv_sys_global_symbol_pipe} | sed '/ __gnu_lto/d'"

  # Check to see that the pipe works correctly.
  pipe_works=no

  rm -f conftest*
  cat > conftest.${ac_ext} <<_GDB_EOF
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
char nm_test_var;
void nm_test_func(void);
void nm_test_func(void){}
#ifdef __cplusplus
}
#endif /* __cplusplus */
int main(){nm_test_var='a';nm_test_func();return(0);}
_GDB_EOF

  if AC_TRY_EVAL(ac_compile); then
    # Now try to grab the symbols.
    nlist=conftest.nm
    if AC_TRY_EVAL(NM conftest.${ac_objext} \| "${gdb_cv_sys_global_symbol_pipe}" \> ${nlist}) && test -s "${nlist}"; then
      # Try sorting and uniquifying the output.
      if sort "${nlist}" | uniq > "${nlist}"T; then
	mv -f "${nlist}"T "${nlist}"
      else
	rm -f "${nlist}"T
      fi

      # Make sure that we snagged all the symbols we need.
      if ${GREP} ' nm_test_var$' "${nlist}" >/dev/null; then
	if ${GREP} ' nm_test_func$' "${nlist}" >/dev/null; then
	  cat <<_GDB_EOF > conftest.${ac_ext}
#if defined(_WIN32) || defined(__CYGWIN__) || defined(_WIN32_WCE)
/* DATA imports from DLLs on WIN32 cannot be const, because runtime
 * relocations are performed -- see ld's documentation on pseudo-relocs. */
# define GDB@&t@_DLSYM_CONST
#elif defined(__osf__)
/* This system does not cope well with relocations in const data.  */
# define GDB@&t@_DLSYM_CONST
#else
# define GDB@&t@_DLSYM_CONST const
#endif /* (_WIN32 || __CYGWIN__ || _WIN32_WCE) || __osf__ */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

_GDB_EOF
	  # Now generate the symbol file.
	  eval "${gdb_cv_sys_global_symbol_to_cdecl}"' < "${nlist}" | ${GREP} -v main >> conftest.${ac_ext}'

	  cat <<_GDB_EOF >> conftest.${ac_ext}

/* The mapping between symbol names and symbols. */
GDB@&t@_DLSYM_CONST struct {
  const char *name;
  void       *address;
}
gdb__PROGRAM__GDBX_preloaded_symbols[[]] =
{
  { "@PROGRAM@", (void *) 0 },
_GDB_EOF
	  ${SED} "s/^$symcode$symcode* \(.*\) \(.*\)$/  {\"\2\", (void *) \&\2},/" < "${nlist}" | ${GREP} -v main >> conftest.${ac_ext}
	  cat <<\_GDB_EOF >> conftest.${ac_ext}
  {0, (void *) 0}
};

/* This works around a problem in FreeBSD linker: */
#ifdef FREEBSD_WORKAROUND
static const void *gdb_preloaded_setup(void) {
  return gdb__PROGRAM__GDBX_preloaded_symbols;
}
#endif /* FREEBSD_WORKAROUND */

#ifdef __cplusplus
}
#endif /* __cplusplus */
_GDB_EOF
	  # Now try linking the two files.
	  mv conftest.${ac_objext} conftstm.${ac_objext}
	  gdb_globsym_save_LIBS=${LIBS}
	  gdb_globsym_save_CFLAGS=${CFLAGS}
	  LIBS="conftstm.${ac_objext}"
	  CFLAGS="${CFLAGS} -fno-builtin"
	  if AC_TRY_EVAL(ac_link) && test -s conftest${ac_exeext}; then
	    pipe_works=yes
	  fi
	  LIBS=${gdb_globsym_save_LIBS}
	  CFLAGS=${gdb_globsym_save_CFLAGS}
	else
	  echo "cannot find nm_test_func in ${nlist}" >&AS_MESSAGE_LOG_FD
	fi
      else
	echo "cannot find nm_test_var in ${nlist}" >&AS_MESSAGE_LOG_FD
      fi
    else
      echo "cannot run ${gdb_cv_sys_global_symbol_pipe}" >&AS_MESSAGE_LOG_FD
    fi
  else
    echo "${progname}: failed program was:" >&AS_MESSAGE_LOG_FD
    cat conftest.${ac_ext} >&5
  fi
  rm -rf conftest* conftst*

  # Do not use the global_symbol_pipe unless it works.
  if test "x${pipe_works}" = "xyes"; then
    break
  else
    gdb_cv_sys_global_symbol_pipe=""
  fi
done
])
if test -z "${gdb_cv_sys_global_symbol_pipe}"; then
  gdb_cv_sys_global_symbol_to_cdecl=""
fi
if test -z "${gdb_cv_sys_global_symbol_pipe}${gdb_cv_sys_global_symbol_to_cdecl}"; then
  AC_MSG_RESULT([failed])
  AC_MSG_WARN([generating source files based on nm output might fail])
else
  AC_MSG_RESULT([ok])
  AC_SUBST([gdb_cv_sys_global_symbol_pipe])
  AC_SUBST([gdb_cv_sys_global_symbol_to_cdecl])
  AC_SUBST([gdb_cv_sys_global_symbol_to_c_name_address])
  AC_SUBST([gdb_cv_sys_global_symbol_to_c_name_address_lib_prefix])
fi

# Response file support.
if test "x${gdb_cv_nm_interface}" = "xMS dumpbin"; then
  nm_file_list_spec='@'
elif ${NM} --help 2>/dev/null | grep '[[@]]FILE' >/dev/null; then
  nm_file_list_spec='@'
fi

]) # GDB_CMD_GLOBAL_SYMBOLS
