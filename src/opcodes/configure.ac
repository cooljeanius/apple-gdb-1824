dnl#                                               -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script.
dnl# (this one is for libopcodes)

AC_PREREQ([2.61])
dnl# keep the version number synced with the one in '../bfd':
AC_INIT([opcodes],[2.16.91],[jmolenda@apple.com])
dnl# bug report address is email address of last person to touch this
dnl# directory, according to the Changelog-Apple in this directory
AC_CONFIG_SRCDIR([z8k-dis.c])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])dnl

dnl# PR 14072:
AH_VERBATIM([00_CONFIG_H_CHECK],[
/* Check that config.h is #included before system headers
 * (this works only for glibc, but that should be enough).  */
#if defined(__GLIBC__) && !defined(__FreeBSD_kernel__) && !defined(__CONFIG_H__)
#  error "config.h must be #included before system headers"
#endif /* __GLIBC__ && !__FreeBSD_kernel__ && !__CONFIG_H__ */
#define __CONFIG_H__ 1])dnl

# build/host/target stuff:
AC_CANONICAL_TARGET
if test -z "${BUILD_ARCHITECTURE}" -a -n "${build_cpu}"; then
  export BUILD_ARCHITECTURE="${build_cpu}"
  AC_SUBST([BUILD_ARCHITECTURE])
fi
if test -z "${HOST_ARCHITECTURE}" -a -n "${host_cpu}"; then
  export HOST_ARCHITECTURE="${host_cpu}"
  AC_SUBST([HOST_ARCHITECTURE])
fi
if test -z "${TARGET_ARCHITECTURE}" -a -n "${target_cpu}"; then
  export TARGET_ARCHITECTURE="${target_cpu}"
  AC_SUBST([TARGET_ARCHITECTURE])
fi
AC_SEARCH_LIBS([strerror],[cposix])dnl# what used to be AC\_ISC\_POSIX

# Automake hack:
AC_MSG_CHECKING([automake things])
AC_MSG_RESULT([ ])
if test -f ${srcdir}/config.status; then
  AC_MSG_NOTICE([config.status is already present, removing it.])
  rm -f ${srcdir}/config.status
fi
AM_INIT_AUTOMAKE([1.11.6 gnits dejagnu])dnl
dnl# (maintainer mode macro is called farther below)
  ## whitespace...

dnl# These must be called before AM_PROG_LIBTOOL, because it may want
dnl# to call AC_CHECK_PROG or similar:
AC_MSG_CHECKING([programs])
AC_MSG_RESULT([ ])
AM_PROG_AR
if test "x${AR}" = "x"; then
  test -z "${AR}"
  AC_CHECK_TOOLS([AR],[ar gar])dnl
  ## ok (might want to switch it around though)
else
  test ! -z "${AR}" && export AR
  AC_SUBST([AR])dnl
  ## dummy
fi
AC_CHECK_TOOLS([RANLIB],[ranlib granlib],[:])dnl
  ## keep autoscan happy:
if test "x${RANLIB}" = "x"; then
  test -z "${RANLIB}"
  AC_PROG_RANLIB
else
  test ! -z "${RANLIB}" && export RANLIB
  AC_SUBST([RANLIB])dnl
  ## dummy
fi
AM_PROG_AS

AC_DEFUN([AC_REQUIRE_SED],[
  AC_REQUIRE([AC_PROG_SED])dnl
])dnl
AC_REQUIRE_SED
AC_DEFUN([AC_REQUIRE_GREPS],[
  AC_REQUIRE([AC_PROG_GREP])dnl
  AC_REQUIRE([AC_PROG_EGREP])dnl
  AC_REQUIRE([AC_PROG_FGREP])dnl
])dnl
AC_REQUIRE_GREPS

dnl# Default to a non shared library. This may be overridden by the
dnl# configure option --enable-shared.
AC_DISABLE_SHARED

# Libtool checks:
AC_PROG_CXX
AC_MSG_CHECKING([libtool things])
AC_MSG_RESULT([ ])
LT_INIT([disable-fast-install disable-shared dlopen static win32-dll])dnl
LT_LANG([C])dnl
dnl# (temporarily) set this while I update everything libtool-related:
LT_PREREQ([2.4.6])dnl

if test -z "${lt_cv_objdir}"; then
  AC_MSG_WARN([lt_cv_objdir somehow was left unset, setting to default])
  export lt_cv_objdir=.libs
  AC_SUBST([lt_cv_objdir])dnl
  ## other possible names for it:
  if test -z "${objdir}"; then
    export objdir=.libs
    AC_SUBST([objdir])
  fi
  if test -z "${libtooldir}"; then
    export libtooldir=.libs
    AC_SUBST([libtooldir])
  fi
elif test "x${lt_cv_objdir}" = "x.."; then
  AC_MSG_WARN([lt_cv_objdir is one level too high, reassigning])
  export lt_cv_objdir=.
  if test -z "${objdir}" -o "x${objdir}" = "x.."; then
    export objdir=.
  fi
  if test -z "${libtooldir}" -o "x${libtooldir}" = "x.."; then
    export libtooldir=.
  fi
elif test "x${lt_cv_objdir}" = "x../.libs"; then
  AC_MSG_WARN([lt_cv_objdir is in one level too high, reassigning])
  export lt_cv_objdir=./.libs
  export objdir=.
  if test -z "${objdir}" -o "x${objdir}" = "x../.libs"; then
    export objdir=./.libs
  fi
  if test -z "${libtooldir}" -o "x${libtooldir}" = "x../.libs"; then
    export libtooldir=./.libs
  fi
elif test -n "${lt_cv_objdir}"; then
  if test -z "${objdir}"; then
    export objdir="${lt_cv_objdir}"
  fi
  if test -z "${libtooldir}"; then
    export libtooldir="${lt_cv_objdir}"
  fi
# This last one should not be reached; the previous cases should have
# covered all the possibilities, but just in case they failed to do so:
elif test -n "${objdir}"; then
  if test -z "${libtooldir}"; then
    AC_MSG_WARN([setting libtooldir to "${objdir}" should be unnecessary])
    export libtooldir="${objdir}"
  fi
fi
if test "x${lt_sysroot}" = "x" -a "x${prefix}" != "x"; then
  test -z "${lt_sysroot}" && test -n "${prefix}"
  AC_MSG_NOTICE([trying to set lt_sysroot to prefix])
  lt_sysroot="${prefix}"
fi
if test -x ${srcdir}/libtool; then
  test -r libtool
  if test "x${USE_MAINTAINER_MODE}" = "xyes"; then
    test ! -z "${USE_MAINTAINER_MODE}"
    ## (bfd puts fixes here, but leave them out in this version for now)
  fi
fi

AC_ARG_ENABLE([targets],
[AS_HELP_STRING([--enable-targets],
                [A comma-delimited list of alternate target configurations.
                 Alternatively, whitespace-delimiting it could work, too,
                 if you remember to quote it properly.])],
[case "${enableval}" in
  yes | "") 
      AC_MSG_ERROR([enable-targets flag must specify targ names or 'all'])
      ;;
  no) enable_targets="" ;;
  *)  enable_targets=${enableval} ;;
esac
test -n "${enable_targets}"])dnl
  ## this flag could be dangerous:
AC_ARG_ENABLE([obsolete],
[AS_HELP_STRING([--enable-obsolete],
                [allow building of obsolete target configurations])],
[case "${enableval}" in
  yes) export enable_obsolete="yes" ;;
  no)  export enable_obsolete="no" ;;
  *)   AC_MSG_ERROR([bad value '${enableval}' for obsolete flag]) ;;
esac
test -n "${enable_obsolete}"])dnl
  ## other subdirs share this flag, too:
AC_ARG_ENABLE([commonbfdlib],
[AS_HELP_STRING([--enable-commonbfdlib],
                [build shared BFD/opcodes/libiberty library])],
[case "${enableval}" in
  yes) commonbfdlib=true ;;
  no)  commonbfdlib=false ;;
  *) AC_MSG_ERROR([bad value ${enableval} for opcodes commonbfdlib flag])
     ;;
esac
test -n "${commonbfdlib}"])dnl

dnl# (warnings are moved farther below)

if test -z "${target}"; then
  AC_MSG_ERROR([Unrecognized target system type; please check config.sub.])
fi

AM_MAINTAINER_MODE
AM_INSTALL_LIBBFD
AC_OBJEXT
AC_EXEEXT

# host-specific stuff:
if test "x${CC}" = "x"; then
  test -z "${CC}"
  AC_PROG_CC
else
  test ! -z "${CC}"
  AC_PROG_GCC_TRADITIONAL
fi

dnl# APPLE LOCAL:  mmap() always works right on Darwin. Take our word for
dnl# it. CY_GNU_GETTEXT will call AC_FUNC_MMAP for us.
case "${host}" in
  *-apple-darwin* | *-apple-macos*)
    ac_cv_func_mmap_fixed_mapped=yes
  ;;
esac

# gettext:
AC_MSG_NOTICE([doing gettext checks])
if test -d ../intl; then
  if test -z "${LDFLAGS}"; then
    export LDFLAGS="-L. -L../intl"
  else
    export LDFLAGS="${LDFLAGS} -L../intl"
  fi
elif test -d ../libintl; then
  if test -z "${LDFLAGS}"; then
    export LDFLAGS="-L. -L../libintl"
  else
    export LDFLAGS="${LDFLAGS} -L../libintl"
  fi
fi
ALL_LINGUAS="fr sv tr es da de id it pt_BR ro nl fi uk vi ga zh_CN"
CY_GNU_GETTEXT
AC_DEFUN([AC_GETTEXT_EXTRA_THINGS],
[m4_ifdef([ZW_GNU_GETTEXT_SISTER_DIR],[
   AC_REQUIRE([ZW_GNU_GETTEXT_SISTER_DIR])dnl
 ])dnl
 dnl#FIXME: require does NOT work with conditionals like this:
 if test "x${GETTEXT_MACRO_VERSION}" = "x"; then
   test -z "${GETTEXT_MACRO_VERSION}"
   AC_REQUIRE([AM_PO_SUBDIRS])dnl
   ## maybe handle differently?
 fi
])dnl
AC_GETTEXT_EXTRA_THINGS

AC_MSG_NOTICE([sourcing ../bfd/configure.host])
. ${srcdir}/../bfd/configure.host

BFD_CC_FOR_BUILD

AC_SUBST([HDEFINES])dnl
AC_PROG_INSTALL
AM_PROG_INSTALL_STRIP
AC_PROG_LN_S

# Checks for libraries.
AC_MSG_CHECKING([libraries])
AC_MSG_RESULT([ ])
AC_MSG_CHECKING([for linker flag to verify linked libraries have the correct architecture])
# this is also done in warning.m4, but make sure it is done up here first:
if test "x${acl_cv_wl}" = "x"; then
  test -z "${acl_cv_wl}" && export acl_cv_wl='-Wl,'
fi
pre_saved_LDFLAGS="${LDFLAGS}"
LDFLAGS="${LDFLAGS} ${acl_cv_wl}-arch_errors_fatal"
AC_LINK_IFELSE([AC_LANG_PROGRAM([[]],[[]])],
               [LD_ARCHFLAGS="${acl_cv_wl}-arch_errors_fatal"],[])
LDFLAGS="${pre_saved_LDFLAGS}"
if test "x${LD_ARCHFLAGS}" != "x"; then
  test -n "${LD_ARCHFLAGS}"
  AC_MSG_RESULT([${LD_ARCHFLAGS}])
  if test "x${ERROR_ON_WARNING}" = "xyes"; then
    if test "x${WARN_LDFLAGS}" != "x"; then
      test -n "${WARN_LDFLAGS}" && WARN_LDFLAGS="${WARN_LDFLAGS} ${LD_ARCHFLAGS}"
    else
      test -z "${WARN_LDFLAGS}" && WARN_LDFLAGS="${LD_ARCHFLAGS}"
    fi
  fi
  AC_SUBST([LD_ARCHFLAGS])dnl
  ## that should be all we need to do with them for now...
else
  test -z "${LD_ARCHFLAGS}"
  AC_MSG_RESULT([none])
fi
# might need to check here:
if test -d /usr/lib/gcc/darwin/2.95.2; then
  export LDFLAGS="${LDFLAGS} -L/usr/lib/gcc/darwin/2.95.2"
fi
# check arch when "main()" is the only function for which we check:
ARCHLESS_LDFLAGS="${LDFLAGS}"
LDFLAGS="${LDFLAGS} ${LD_ARCHFLAGS}"
# FIXME: Replace `main' with a function in `-lcc_dynamic':
AC_CHECK_LIB([cc_dynamic],[main],
           [SYSTEM_FRAMEWORK_LIBS="${SYSTEM_FRAMEWORK_LIBS} -lcc_dynamic"])
# FIXME: Replace `main' with a function in `-lgcc':
AC_CHECK_LIB([gcc],[main],
             [SYSTEM_FRAMEWORK_LIBS="${SYSTEM_FRAMEWORK_LIBS} -lgcc"])dnl
# FIXME: Replace `main' with a function in `-lSystem':
AC_CHECK_LIB([System],[main],
            [SYSTEM_FRAMEWORK_LIBS="${SYSTEM_FRAMEWORK_LIBS} -lSystem"])dnl
  ## reset these:
LDFLAGS="${ARCHLESS_LDFLAGS}"
  ## TODO: might only want to make this an error depending on platform:
if test -z "${SYSTEM_FRAMEWORK_LIBS}"; then
  AC_MSG_ERROR([Missing some required system libraries.])
fi
AC_SUBST([SYSTEM_FRAMEWORK_LIBS])dnl
AC_CHECK_LIB([c],[printf])dnl
# ../intl is already added to LDFLAGS above, so this should be fine:
AC_SEARCH_LIBS([dcgettext],[intl])dnl
AC_CHECK_LIB([intl],[gettext],[],[
  PRE_INTL_LDFLAGS="${LDFLAGS}"
  LDFLAGS="${LDFLAGS} ${LD_ARCHFLAGS}"
  AC_CHECK_LIB([intl],[main])dnl
  ## reset:
  LDFLAGS="${PRE_INTL_LDFLAGS}"
])dnl
AC_CHECK_LIB([iconv],[iconv])dnl

# Checks for header files.
AC_MSG_CHECKING([headers])
AC_MSG_RESULT([ ])
AC_HEADER_STDBOOL dnl# also "_CHECK"s it
AC_DEFUN([AC_REQUIRE_HEADER_STDC],[
  AC_REQUIRE([AC_HEADER_STDC])dnl
])dnl
AC_REQUIRE_HEADER_STDC
if test -d ../intl; then
  if test -z "${CPPFLAGS}"; then
    export CPPFLAGS="-I. -I../intl"
  else
    export CPPFLAGS="${CPPFLAGS} -I../intl"
  fi
elif test -d ../libintl; then
  if test -z "${CPPFLAGS}"; then
    export CPPFLAGS="-I. -I../libintl"
  else
    export CPPFLAGS="${CPPFLAGS} -I../libintl"
  fi
fi
unset ac_cv_header_libintl_h
unset ac_cv_header_malloc_h
unset ac_cv_header_stdlib_h
AC_CHECK_HEADERS([err.h errno.h gettext.h libgen.h libintl.h \
                  malloc.h malloc/malloc.h setjmp.h stddef.h \
                  stdio.h stdio_ext.h])dnl

# Checks for typedefs, structures, and compiler characteristics.
AC_MSG_CHECKING([C language characteristics that your C compiler supports])
AC_MSG_RESULT([ ])
AC_C_BIGENDIAN
if test "x${ac_cv_c_const}" = "x"; then
  test -z "${ac_cv_c_const}"
  AC_C_CONST
fi
AC_C_PROTOTYPES
AC_C_STRINGIZE
AC_MSG_CHECKING([types])
AC_MSG_RESULT([ ])
AC_TYPE_INTPTR_T
AC_TYPE_INT8_T
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_STRUCT_TIMEZONE dnl# also does the "_TM" one

# Checks for library functions.
AC_MSG_CHECKING([functions])
AC_MSG_RESULT([ ])
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_FUNC_MEMCMP
AC_FUNC_REALLOC
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([chdir err memmove memset regcomp strdup strerror \
                strncasecmp strstr strtol strtoul strtoull])dnl

# Checks for declarations.
AC_MSG_CHECKING([declarations])
AC_MSG_RESULT([ ])
AC_CHECK_DECLS([basename],[],[],[
/* declared in this header on darwin at least: */
#ifdef HAVE_LIBGEN_H
# include <libgen.h>
#endif /* HAVE_LIBGEN_H */
])dnl
AC_CHECK_DECLS([chdir])dnl
AC_CHECK_DECLS_ONCE([getopt])dnl
AC_CHECK_DECLS([stpcpy])dnl

cgen_maint=no
cgendir='$(srcdir)/../cgen'

AC_ARG_ENABLE([cgen-maint],
[AS_HELP_STRING([--enable-cgen-maint[=dir]],[build cgen generated files])],
[case "${enableval}" in
  yes)	cgen_maint=yes ;;
  no)	cgen_maint=no ;;
  *)
	# argument is cgen install directory (not implemented yet).
	# Having a `share' directory might be more appropriate for
	# the .scm, .cpu, etc. files.
	cgen_maint=yes
	cgendir=${cgen_maint}/lib/cgen
	;;
esac])dnl
AM_CONDITIONAL([CGEN_MAINT],[test "x${cgen_maint}" = "xyes"])
AC_SUBST([cgendir])dnl

using_cgen=no

# Check if linker supports --as-needed and --no-as-needed options
AC_CACHE_CHECK([linker --as-needed support],[bfd_cv_ld_as_needed],
	[bfd_cv_ld_as_needed=no
	if ${LD} --help 2>/dev/null | grep as-needed > /dev/null; then
		bfd_cv_ld_as_needed=yes
	fi
	])dnl

dnl# check for libm:
LT_LIB_M

# Libs for generator progs:
if test "x${cross_compiling}" = "xno"; then
  BUILD_LIBS=../libiberty/libiberty.a
  BUILD_LIB_DEPS=${BUILD_LIBS}
else
  # if cross-compiling, assume that the system provides -liberty
  # and that the version is compatible with new headers.
  BUILD_LIBS=-liberty
  BUILD_LIB_DEPS=""
fi
BUILD_LIBS="${BUILD_LIBS} ${LIBINTL}"
BUILD_LIB_DEPS="${BUILD_LIB_DEPS} ${LIBINTL_DEP}"

AC_SUBST([BUILD_LIBS])dnl
AC_SUBST([BUILD_LIB_DEPS])dnl

# Horrible hacks to build DLLs on Windows.
WIN32LDFLAGS=""
WIN32LIBADD=""
case "${host}" in
*-*-cygwin*)
  if test "x${enable_shared}" = "xyes"; then
    WIN32LDFLAGS="-no-undefined"
    WIN32LIBADD="-L`pwd`/../bfd -lbfd -L`pwd`/../libiberty -liberty -L`pwd`/../intl -lintl -lcygwin"
  fi
  ;;
esac
AC_SUBST([WIN32LDFLAGS])dnl
AC_SUBST([WIN32LIBADD])dnl

# target-specific stuff:

# Canonicalize the secondary target names.
if test -n "${enable_targets}"; then
    for targ in `echo ${enable_targets} | sed 's/,/ /g'`
    do
	result=`${ac_config_sub} ${targ} 2>/dev/null`
	if test -n "${result}" ; then
	    canon_targets="${canon_targets} ${result}"
	else
	    # Allow targets that config.sub does NOT recognize, like "all".
	    canon_targets="${canon_targets} ${targ}"
	fi
    done
fi

all_targets=false
selarchs=""

AC_MSG_CHECKING([which targets to look up])
AC_MSG_RESULT([plain ones are '${target}' and canonical ones are '${canon_targets}'])
for targ in ${target} ${canon_targets}
do
    if test "x${targ}" = "xall"; then
        AC_MSG_NOTICE([no need to source config.bfd when targ is 'all'])
        all_targets=true
    else
	AC_MSG_NOTICE([sourcing ../bfd/config.bfd for target '${targ}'])
	. ${srcdir}/../bfd/config.bfd
	selarchs="${selarchs} ${targ_archs}"
	AC_MSG_NOTICE([selarchs are now '${selarchs}'])
    fi
done	

# Utility var, documents generic cgen support files:
cgen_files="cgen-opc.lo cgen-asm.lo cgen-dis.lo"

# We do NOT do any links based on the target system, just makefile config:
if test "x${all_targets}" = "xfalse"; then
    # Target architecture .o files:
    ta=""

    for arch in ${selarchs}
    do
	ad=`echo ${arch} | sed -e s/bfd_//g -e s/_arch//g`
	archdefs="${archdefs} -DARCH_${ad}"
	case "${arch}" in
	bfd_a29k_arch)	     ta="${ta} a29k-dis.lo" ;;
	bfd_alliant_arch)    ;;
	bfd_alpha_arch)	     ta="${ta} alpha-dis.lo alpha-opc.lo" ;;
	bfd_arc_arch)	     ta="${ta} arc-dis.lo arc-opc.lo arc-ext.lo" ;;
	bfd_arm_arch)	     ta="${ta} arm-dis.lo" ;;
	bfd_avr_arch)	     ta="${ta} avr-dis.lo" ;;
	bfd_convex_arch)     ;;
	bfd_cris_arch)	     ta="${ta} cris-dis.lo cris-opc.lo" ;;
	bfd_crx_arch)        ta="${ta} crx-dis.lo crx-opc.lo" ;;
	bfd_d10v_arch)	     ta="${ta} d10v-dis.lo d10v-opc.lo" ;;
	bfd_d30v_arch)	     ta="${ta} d30v-dis.lo d30v-opc.lo" ;;
	bfd_dlx_arch)	     ta="${ta} dlx-dis.lo" ;;
	bfd_fr30_arch)       ta="${ta} fr30-asm.lo fr30-desc.lo fr30-dis.lo fr30-ibld.lo fr30-opc.lo"
                             using_cgen=yes ;;
	bfd_frv_arch)        ta="${ta} frv-asm.lo frv-desc.lo frv-dis.lo frv-ibld.lo frv-opc.lo"
                             using_cgen=yes ;;
	bfd_h8300_arch)	     ta="${ta} h8300-dis.lo" ;;
	bfd_h8500_arch)	     ta="${ta} h8500-dis.lo" ;;
	bfd_hppa_arch)	     ta="${ta} hppa-dis.lo" ;;
	bfd_i370_arch)	     ta="${ta} i370-dis.lo i370-opc.lo" ;;
	bfd_i386_arch)	     ta="${ta} i386-dis.lo i386-opc.lo" ;;
	bfd_i860_arch)	     ta="${ta} i860-dis.lo" ;;
	bfd_i960_arch)	     ta="${ta} i960-dis.lo" ;;
	bfd_ia64_arch)	     ta="${ta} ia64-dis.lo ia64-opc.lo" ;;
	bfd_ip2k_arch)	     ta="${ta} ip2k-asm.lo ip2k-desc.lo ip2k-dis.lo ip2k-ibld.lo ip2k-opc.lo"
                             using_cgen=yes ;;
        bfd_iq2000_arch)     ta="${ta} iq2000-asm.lo iq2000-desc.lo iq2000-dis.lo iq2000-ibld.lo iq2000-opc.lo"
                             using_cgen=yes ;;
	bfd_m32c_arch)	     ta="${ta} m32c-asm.lo m32c-desc.lo m32c-dis.lo m32c-ibld.lo m32c-opc.lo"
                             using_cgen=yes ;;
	bfd_m32r_arch)	     ta="${ta} m32r-asm.lo m32r-desc.lo m32r-dis.lo m32r-ibld.lo m32r-opc.lo m32r-opinst.lo"
                             using_cgen=yes ;;
	bfd_m68hc11_arch)    ta="${ta} m68hc11-dis.lo m68hc11-opc.lo" ;;
	bfd_m68hc12_arch)    ta="${ta} m68hc11-dis.lo m68hc11-opc.lo" ;;
	bfd_m68k_arch)	     ta="${ta} m68k-dis.lo m68k-opc.lo" ;;
	bfd_m88k_arch)	     ta="${ta} m88k-dis.lo" ;;
	bfd_maxq_arch)	     ta="${ta} maxq-dis.lo" ;;
	bfd_mcore_arch)	     ta="${ta} mcore-dis.lo" ;;
	bfd_mips_arch)	     ta="${ta} mips-dis.lo mips-opc.lo mips16-opc.lo" ;;
	bfd_mmix_arch)	     ta="${ta} mmix-dis.lo mmix-opc.lo" ;;
	bfd_mn10200_arch)    ta="${ta} m10200-dis.lo m10200-opc.lo" ;;
	bfd_mn10300_arch)    ta="${ta} m10300-dis.lo m10300-opc.lo" ;;
	bfd_ms1_arch)	     ta="${ta} ms1-asm.lo ms1-desc.lo ms1-dis.lo ms1-ibld.lo ms1-opc.lo"
                             using_cgen=yes ;;
	bfd_msp430_arch)     ta="${ta} msp430-dis.lo" ;;
	bfd_ns32k_arch)	     ta="${ta} ns32k-dis.lo" ;;
	bfd_openrisc_arch)   ta="${ta} openrisc-asm.lo openrisc-desc.lo openrisc-dis.lo openrisc-ibld.lo openrisc-opc.lo"
                             using_cgen=yes ;;
	bfd_or32_arch)	     ta="${ta} or32-dis.lo or32-opc.lo"
                             using_cgen=yes ;;
	bfd_pdp11_arch)	     ta="${ta} pdp11-dis.lo pdp11-opc.lo" ;;
	bfd_pj_arch)	     ta="${ta} pj-dis.lo pj-opc.lo" ;;
	bfd_powerpc_arch)    ta="${ta} ppc-dis.lo ppc-opc.lo" ;;
	bfd_powerpc_64_arch) ta="${ta} ppc-dis.lo ppc-opc.lo" ;;
	bfd_pyramid_arch)    ;;
	bfd_romp_arch)	     ;;
	bfd_rs6000_arch)     ta="${ta} ppc-dis.lo ppc-opc.lo" ;;
	bfd_s390_arch)	     ta="${ta} s390-dis.lo s390-opc.lo" ;;
	bfd_sh_arch)
	  # We cannot decide what we want just from the CPU family.
	  # We want SH5 support unless a specific version of sh is
	  # specified, as in sh3-elf, sh3b-linux-gnu, etc.
	  # Include it just for ELF targets, since the SH5 bfd:s are ELF
	  # only.
	  for t in ${target} ${canon_targets}; do
	    case ${t} in
	      all |  sh5*-* | sh64*-* | sh-*-*elf* | shl*-*-*elf* | \
	      sh-*-linux* | shl-*-linux*)
	        ta="${ta} sh64-dis.lo sh64-opc.lo"
	        archdefs="${archdefs} -DINCLUDE_SHMEDIA"
	        break;;
	    esac;
	  done
	  ta="${ta} sh-dis.lo" ;;
	bfd_sparc_arch)	    ta="${ta} sparc-dis.lo sparc-opc.lo" ;;
	bfd_tahoe_arch)	    ;;
	bfd_tic30_arch)	    ta="${ta} tic30-dis.lo" ;;
        bfd_tic4x_arch)     ta="${ta} tic4x-dis.lo" ;;
	bfd_tic54x_arch)    ta="${ta} tic54x-dis.lo tic54x-opc.lo" ;;
	bfd_tic80_arch)	    ta="${ta} tic80-dis.lo tic80-opc.lo" ;;
	bfd_v850_arch)	    ta="${ta} v850-opc.lo v850-dis.lo" ;;
	bfd_v850e_arch)	    ta="${ta} v850-opc.lo v850-dis.lo" ;;
	bfd_v850ea_arch)    ta="${ta} v850-opc.lo v850-dis.lo" ;;
	bfd_vax_arch)	    ta="${ta} vax-dis.lo" ;;
	bfd_w65_arch)	    ta="${ta} w65-dis.lo" ;;
	bfd_we32k_arch)	    ;;
	bfd_x86_64_arch)    ta="${ta} i386-dis.lo i386-opc.lo" ;;
	bfd_xstormy16_arch) ta="${ta} xstormy16-asm.lo xstormy16-desc.lo xstormy16-dis.lo xstormy16-ibld.lo xstormy16-opc.lo" using_cgen=yes ;;
	bfd_xtensa_arch)    ta="${ta} xtensa-dis.lo" ;;
	bfd_z8k_arch)	    ta="${ta} z8k-dis.lo" ;;

	"")		    ;;
	*)
          AC_MSG_ERROR([*** unknown target architecture ${arch}])
          ;;
	esac
    done

    if test "x${using_cgen}" = "xyes"; then
	ta="${ta} ${cgen_files}"
    fi

    AC_PATH_PROG([UNIQ_BIN],[uniq guniq],[])dnl
    # Weed out duplicate .o files. (what about .lo files?)
    f=""
    for i in ${ta}; do
	case " ${f} " in
	*" ${i} "*) ;;
	*) f="${f} ${i}" ;;
	esac
    done
    ta="${f}"

    # And duplicate -D flags.
    f=""
    for i in ${archdefs}; do
	case " ${f} " in
	*" ${i} "*) ;;
	*) f="${f} ${i}" ;;
	esac
    done
    archdefs="${f}"

    BFD_MACHINES="${ta}"
else
    # all_targets is true:
    archdefs=-DARCH_all
    BFD_MACHINES='$(ALL_MACHINES)'
fi

AC_SUBST([archdefs])dnl
AC_SUBST([BFD_MACHINES])dnl

dnl# APPLE LOCAL change is to omit the following:
if test "x${cgen_maint}" = "xyes" && test "x${using_cgen}" = "xno"; then
  dnl# only do warnings if we can regenerate cgen-erated sources, but are
  dnl# not actually going to do so:
  AM_BINUTILS_WARNINGS
fi
dnl# (I uncommented it to add it back in anyways)

## Output.
AC_CONFIG_FILES([Makefile \
                 po/Makefile.in:po/Make-in])dnl

AC_CONFIG_COMMANDS([default],[[sed -e '/POTFILES =/r po/POTFILES' po/Makefile.in > po/Makefile]],[[]])dnl

AC_CONFIG_COMMANDS([config_status_vpath_build_fix],[
if test "${srcdir}" != "."; then
  if test -e ${srcdir}/config.status -a ! -e `pwd`/config.status; then
    cp -v ${srcdir}/config.status `pwd`/config.status || echo ""
  elif test -e `pwd`/config.status -a ! -e ${srcdir}/config.status; then
    cp -v `pwd`/config.status ${srcdir}/config.status || echo ""
  fi
fi
],[])dnl

dnl# order of these first 2 libtool hacks is reversed from '../bfd':
AC_CONFIG_COMMANDS([libtool_lengthening_hack_fix1],[
# Hack in case libtool is too short:
changequote(,)
# The "1000" is just an arbitrarily chosen number that is usually right
if test "$(wc -l < libtool | tr -d [:blank:])" -lt "1000"; then
changequote([,])
  echo "libtool is too short, trying to create a new one"
  if test -x ../ltconfig; then
    # I have no clue why this is a thing:
    if test -e /mount/endor/wildenhu/local-x86_64/bin/sed; then
      if test -r ../ltmain.sh; then
        echo "../ltconfig ../ltmain.sh"
        ../ltconfig ../ltmain.sh
      elif test -r ../ltmain-old.sh; then
        echo "../ltconfig ../ltmain-old.sh"
        ../ltconfig ../ltmain-old.sh
      elif test -r ../ltcf-c.sh; then
        echo "../ltconfig ../ltcf-c.sh"
        ../ltconfig ../ltcf-c.sh
      else
        echo "not sure which ltmain.sh to use to do so"
      fi
    else
      echo "actually never mind (for now)"
    fi
  else
    echo "unable to do so: ../ltconfig is missing and/or unusable"
  fi
fi
],[])dnl

AC_CONFIG_COMMANDS([libtool_lengthening_hack_fix2],[
# Also try the first hack from ../bfd here as well:
if test -e libtool -a -e libtool.old; then
  stat libtool > /dev/null
  stat libtool.old > /dev/null
changequote(,)
  if test "$(wc -l < libtool.old | tr -d [:blank:])" -gt "$(wc -l < libtool | tr -d [:blank:])"; then
changequote([,])
    echo "libtool.old is longer than libtool"
    echo "that means that it is probably the better one to use"
    cp -vf libtool.old libtool
  else
    echo "libtool.old is NOT longer than libtool"
    echo "leaving it alone..."
  fi
elif test -e libtool.old -a ! -e libtool; then
  stat libtool.old > /dev/null
  echo "libtool is not present, but libtool.old is"
  echo "so use libtool.old instead"
  cp -v libtool.old libtool
elif test -e libtool -a ! -e libtool.old; then
  stat libtool > /dev/null
  echo "using default libtool"
elif test ! -e libtool -a ! -e libtool.old; then
  echo "no libtool found"
else
  echo "this should not happen"
fi
],[])dnl

dnl# ...and the 3rd one:
AC_CONFIG_COMMANDS([broken_libtool_hack_fix3],[
if test -x ./config.status; then
  if test -z "${our_libtool_script}"; then
    if test -x ./libtool; then
      our_libtool_script="./libtool"
    elif test -n "${srcdir}" -a -x ${srcdir}/libtool; then
      our_libtool_script="${srcdir}/libtool"
    else
      our_libtool_script=libtool
    fi
  fi
  if test -z "${LIBTOOL}"; then
    if test -x /bin/sh; then
      LIBTOOL="/bin/sh ${our_libtool_script}"
    else
      LIBTOOL="${our_libtool_script}"
    fi
  fi  
  if (${LIBTOOL} --config | ${GREP} "objdir="); then
    my_libtooldir=`${LIBTOOL} --config | ${SED} -n -e 's/^objdir=//p'`
    if test -n "${my_libtooldir}"; then
      echo "${our_libtool_script} already defines objdir: '${my_libtooldir}'"
    else
      echo "error: my_libtooldir should be defined if ${our_libtool_script} already defines objdir" >&2
      exit 1
    fi
  elif test -e ${our_libtool_script}; then
    echo "" >> ${our_libtool_script}
    if test -n "${lt_cv_objdir}"; then
      echo "using lt_cv_objdir ('${lt_cv_objdir}') to define objdir in ${our_libtool_script}"
      echo "objdir=${lt_cv_objdir}" >> ${our_libtool_script}
    elif test -n "${objdir}"; then
      echo "using objdir ('${objdir}') to define objdir in ${our_libtool_script}"
      echo "objdir=${objdir}" >> ${our_libtool_script}
    elif test -n "${libtooldir}"; then
      echo "using libtooldir ('${libtooldir}') to define objdir in ${our_libtool_script}"
      echo "objdir=${libtooldir}" >> ${our_libtool_script}
    elif test -d .libs; then
      echo "using .libs to define objdir in ${our_libtool_script}"
      echo "objdir=.libs" >> ${our_libtool_script}
    elif test -n "${srcdir}" -a -d ${srcdir}/.libs; then
      echo "using ${srcdir}/.libs to define objdir in ${our_libtool_script}"
      echo "objdir=${srcdir}/.libs" >> ${our_libtool_script}
    else
      echo "warning: need to define objdir in ${our_libtool_script}, but cannot figure out how to do so"
    fi
  else
    echo "I hate libtool..."
  fi
fi
],[])dnl

# Output happens for real now.
AC_OUTPUT

echo "Done configuring opcodes."
