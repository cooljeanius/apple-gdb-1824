dnl#                                               -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script.
dnl#

AC_PREREQ([2.61])
dnl# keep version number synced with the one in ../bfd
AC_INIT([opcodes],[2.16.91],[jmolenda@apple.com])
dnl# bug report address is email address of last person to touch this
dnl# directory, according to the Changelog-Apple in this directory
AC_CONFIG_SRCDIR([z8k-dis.c])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])

AC_CANONICAL_TARGET
AC_SEARCH_LIBS([strerror],[cposix])

if test -f ${srcdir}/config.status; then
  AC_MSG_NOTICE([config.status is already present, removing it.])
  rm -f ${srcdir}/config.status
fi
AM_INIT_AUTOMAKE([1.10 gnu dejagnu])

dnl# These must be called before AM_PROG_LIBTOOL, because it may want
dnl# to call AC_CHECK_PROG.
AM_PROG_AR
if test "x${AR}" = "x"; then
	test -z "${AR}"
	AC_CHECK_TOOL([AR],[ar])
else
	test ! -z "${AR}" && export AR
	AC_SUBST([AR])
fi
AC_CHECK_TOOL([RANLIB],[ranlib],[:])
if test "x${RANLIB}" = "x"; then
	test -z "${RANLIB}"
	AC_PROG_RANLIB
else
	test ! -z "${RANLIB}" && export RANLIB
	AC_SUBST([RANLIB])
fi
AM_PROG_AS

dnl# Default to a non shared library. This may be overridden by the
dnl# configure option --enable-shared.
AC_DISABLE_SHARED

# Libtool checks
LT_INIT([disable-fast-install disable-shared dlopen static win32-dll])
LT_LANG([C])

AC_ARG_ENABLE([targets],
[AS_HELP_STRING([--enable-targets],[alternative target configurations])],
[case "${enableval}" in
  yes | "") AC_MSG_ERROR([enable-targets option must specify target names or 'all'])
            ;;
  no)       enable_targets= ;;
  *)        enable_targets=$enableval ;;
esac])dnl
AC_ARG_ENABLE([commonbfdlib],
[AS_HELP_STRING([--enable-commonbfdlib],[build shared BFD/opcodes/libiberty library])],
[case "${enableval}" in
  yes) commonbfdlib=true ;;
  no)  commonbfdlib=false ;;
  *)   AC_MSG_ERROR([bad value ${enableval} for opcodes commonbfdlib option]) ;;
esac])dnl

dnl# APPLE LOCAL change is to omit the following:
AM_BINUTILS_WARNINGS
dnl# (I uncommented it to add it back in anyways)

if test -z "${target}" ; then
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

# gettext
AC_MSG_NOTICE([doing gettext checks])
ALL_LINGUAS="fr sv tr es da de id it pt_BR ro nl fi uk vi"
CY_GNU_GETTEXT
ZW_GNU_GETTEXT_SISTER_DIR
if test "x${GETTEXT_MACRO_VERSION}" = "x"; then
	test -z "${GETTEXT_MACRO_VERSION}"
	AM_PO_SUBDIRS
fi

AC_MSG_NOTICE([sourcing ../bfd/configure.host])
. ${srcdir}/../bfd/configure.host

BFD_CC_FOR_BUILD

AC_SUBST([HDEFINES])
AC_PROG_INSTALL
AM_PROG_INSTALL_STRIP
AC_PROG_LN_S

# Checks for libraries.
if test -d /usr/lib/gcc/darwin/2.95.2; then
	export LDFLAGS="${LDFLAGS} -L/usr/lib/gcc/darwin/2.95.2"
fi
# FIXME: Replace `main' with a function in `-lcc_dynamic':
AC_CHECK_LIB([cc_dynamic],[main],[SYSTEM_FRAMEWORK_LIBS="${SYSTEM_FRAMEWORK_LIBS} -lcc_dynamic"])
# FIXME: Replace `main' with a function in `-lgcc':
AC_CHECK_LIB([gcc],[main],[SYSTEM_FRAMEWORK_LIBS="${SYSTEM_FRAMEWORK_LIBS} -lgcc"])
# FIXME: Replace `main' with a function in `-lSystem':
AC_CHECK_LIB([System],[main],[SYSTEM_FRAMEWORK_LIBS="${SYSTEM_FRAMEWORK_LIBS} -lSystem"])
if test -z "$SYSTEM_FRAMEWORK_LIBS"; then
	AC_MSG_ERROR([Missing some required system libraries.])
fi
AC_SUBST([SYSTEM_FRAMEWORK_LIBS])
AC_CHECK_LIB([c],[printf])
AC_SEARCH_LIBS([dcgettext],[intl])
AC_CHECK_LIB([intl],[gettext],[],[
  AC_CHECK_LIB([intl],[main])
])
AC_CHECK_LIB([iconv],[iconv])

# Checks for header files.
AC_HEADER_STDBOOL dnl# calls AC_CHECK_HEADER_STDBOOL
AC_CHECK_HEADERS([errno.h gettext.h libintl.h malloc.h setjmp.h stddef.h \
                  stdio.h stdio_ext.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_BIGENDIAN
AC_C_CONST
AC_C_PROTOTYPES
AC_TYPE_INT8_T
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T

# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([memmove memset regcomp strdup strerror strncasecmp strstr \
                strtol strtoul strtoull])
AC_CHECK_DECLS([basename])
AC_CHECK_DECLS([stpcpy])

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
AM_CONDITIONAL([CGEN_MAINT],[test x${cgen_maint} = xyes])
AC_SUBST([cgendir])

using_cgen=no

# Check if linker supports --as-needed and --no-as-needed options
AC_CACHE_CHECK([linker --as-needed support],[bfd_cv_ld_as_needed],
	[bfd_cv_ld_as_needed=no
	if ${LD} --help 2>/dev/null | grep as-needed > /dev/null; then
		bfd_cv_ld_as_needed=yes
	fi
	])

LT_LIB_M

# Libs for generator progs
if test "x${cross_compiling}" = "xno"; then
  BUILD_LIBS=../libiberty/libiberty.a
  BUILD_LIB_DEPS=${BUILD_LIBS}
else
  # if cross-compiling, assume that the system provides -liberty
  # and that the version is compatible with new headers.
  BUILD_LIBS=-liberty
  BUILD_LIB_DEPS=
fi
BUILD_LIBS="${BUILD_LIBS} ${LIBINTL}"
BUILD_LIB_DEPS="${BUILD_LIB_DEPS} ${LIBINTL_DEP}"

AC_SUBST([BUILD_LIBS])
AC_SUBST([BUILD_LIB_DEPS])

# Horrible hacks to build DLLs on Windows.
WIN32LDFLAGS=
WIN32LIBADD=
case "${host}" in
*-*-cygwin*)
  if test "${enable_shared}" = "yes"; then
    WIN32LDFLAGS="-no-undefined"
    WIN32LIBADD="-L`pwd`/../bfd -lbfd -L`pwd`/../libiberty -liberty -L`pwd`/../intl -lintl -lcygwin"
  fi
  ;;
esac
AC_SUBST([WIN32LDFLAGS])
AC_SUBST([WIN32LIBADD])

# target-specific stuff:

# Canonicalize the secondary target names.
if test -n "${enable_targets}" ; then
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
selarchs=
for targ in ${target} ${canon_targets}
do
    if test "x${targ}" = "xall" ; then
        all_targets=true
    else
	AC_MSG_NOTICE([sourcing ../bfd/config.bfd])
	. $srcdir/../bfd/config.bfd
	selarchs="${selarchs} ${targ_archs}"
    fi
done	

# Utility var, documents generic cgen support files.

cgen_files="cgen-opc.lo cgen-asm.lo cgen-dis.lo"

# We do NOT do any links based on the target system, just makefile config.

if test x${all_targets} = xfalse ; then

    # Target architecture .o files.
    ta=

    for arch in ${selarchs}
    do
	ad=`echo ${arch} | sed -e s/bfd_//g -e s/_arch//g`
	archdefs="${archdefs} -DARCH_${ad}"
	case "${arch}" in
	bfd_a29k_arch)		ta="${ta} a29k-dis.lo" ;;
	bfd_alliant_arch)	;;
	bfd_alpha_arch)		ta="${ta} alpha-dis.lo alpha-opc.lo" ;;
	bfd_arc_arch)		ta="${ta} arc-dis.lo arc-opc.lo arc-ext.lo" ;;
	bfd_arm_arch)		ta="${ta} arm-dis.lo" ;;
	bfd_avr_arch)		ta="${ta} avr-dis.lo" ;;
	bfd_convex_arch)	;;
	bfd_cris_arch)		ta="${ta} cris-dis.lo cris-opc.lo" ;;
	bfd_crx_arch)    	ta="${ta} crx-dis.lo crx-opc.lo" ;;
	bfd_d10v_arch)		ta="${ta} d10v-dis.lo d10v-opc.lo" ;;
	bfd_d30v_arch)		ta="${ta} d30v-dis.lo d30v-opc.lo" ;;
	bfd_dlx_arch)		ta="${ta} dlx-dis.lo" ;;
	bfd_fr30_arch)          ta="${ta} fr30-asm.lo fr30-desc.lo fr30-dis.lo fr30-ibld.lo fr30-opc.lo" using_cgen=yes ;;
	bfd_frv_arch)           ta="${ta} frv-asm.lo frv-desc.lo frv-dis.lo frv-ibld.lo frv-opc.lo" using_cgen=yes ;;
	bfd_h8300_arch)		ta="${ta} h8300-dis.lo" ;;
	bfd_h8500_arch)		ta="${ta} h8500-dis.lo" ;;
	bfd_hppa_arch)		ta="${ta} hppa-dis.lo" ;;
	bfd_i370_arch)		ta="${ta} i370-dis.lo i370-opc.lo" ;;
	bfd_i386_arch)		ta="${ta} i386-dis.lo i386-opc.lo" ;;
	bfd_i860_arch)		ta="${ta} i860-dis.lo" ;;
	bfd_i960_arch)		ta="${ta} i960-dis.lo" ;;
	bfd_ia64_arch)		ta="${ta} ia64-dis.lo ia64-opc.lo" ;;
	bfd_ip2k_arch)		ta="${ta} ip2k-asm.lo ip2k-desc.lo ip2k-dis.lo ip2k-ibld.lo ip2k-opc.lo" using_cgen=yes ;;
        bfd_iq2000_arch)        ta="${ta} iq2000-asm.lo iq2000-desc.lo iq2000-dis.lo iq2000-ibld.lo iq2000-opc.lo" using_cgen=yes ;;
	bfd_m32c_arch)		ta="${ta} m32c-asm.lo m32c-desc.lo m32c-dis.lo m32c-ibld.lo m32c-opc.lo" using_cgen=yes ;;
	bfd_m32r_arch)		ta="${ta} m32r-asm.lo m32r-desc.lo m32r-dis.lo m32r-ibld.lo m32r-opc.lo m32r-opinst.lo" using_cgen=yes ;;
	bfd_m68hc11_arch)	ta="${ta} m68hc11-dis.lo m68hc11-opc.lo" ;;
	bfd_m68hc12_arch)	ta="${ta} m68hc11-dis.lo m68hc11-opc.lo" ;;
	bfd_m68k_arch)		ta="${ta} m68k-dis.lo m68k-opc.lo" ;;
	bfd_m88k_arch)		ta="${ta} m88k-dis.lo" ;;
	bfd_maxq_arch)		ta="${ta} maxq-dis.lo" ;;
	bfd_mcore_arch)		ta="${ta} mcore-dis.lo" ;;
	bfd_mips_arch)		ta="${ta} mips-dis.lo mips-opc.lo mips16-opc.lo" ;;
	bfd_mmix_arch)		ta="${ta} mmix-dis.lo mmix-opc.lo" ;;
	bfd_mn10200_arch)	ta="${ta} m10200-dis.lo m10200-opc.lo" ;;
	bfd_mn10300_arch)	ta="${ta} m10300-dis.lo m10300-opc.lo" ;;
	bfd_ms1_arch)		ta="${ta} ms1-asm.lo ms1-desc.lo ms1-dis.lo ms1-ibld.lo ms1-opc.lo" using_cgen=yes ;;
	bfd_msp430_arch)	ta="${ta} msp430-dis.lo" ;;
	bfd_ns32k_arch)		ta="${ta} ns32k-dis.lo" ;;
	bfd_openrisc_arch)	ta="${ta} openrisc-asm.lo openrisc-desc.lo openrisc-dis.lo openrisc-ibld.lo openrisc-opc.lo" using_cgen=yes ;;
	bfd_or32_arch)		ta="${ta} or32-dis.lo or32-opc.lo" using_cgen=yes ;;
	bfd_pdp11_arch)		ta="${ta} pdp11-dis.lo pdp11-opc.lo" ;;
	bfd_pj_arch)	        ta="${ta} pj-dis.lo pj-opc.lo" ;;
	bfd_powerpc_arch)	ta="${ta} ppc-dis.lo ppc-opc.lo" ;;
	bfd_powerpc_64_arch)	ta="${ta} ppc-dis.lo ppc-opc.lo" ;;
	bfd_pyramid_arch)	;;
	bfd_romp_arch)		;;
	bfd_rs6000_arch)	ta="${ta} ppc-dis.lo ppc-opc.lo" ;;
	bfd_s390_arch)		ta="${ta} s390-dis.lo s390-opc.lo" ;;
	bfd_sh_arch)
	  # We cannot decide what we want just from the CPU family.
	  # We want SH5 support unless a specific version of sh is
	  # specified, as in sh3-elf, sh3b-linux-gnu, etc.
	  # Include it just for ELF targets, since the SH5 bfd:s are ELF
	  # only.
	  for t in $target $canon_targets; do
	    case $t in
	      all |  sh5*-* | sh64*-* | sh-*-*elf* | shl*-*-*elf* | \
	      sh-*-linux* | shl-*-linux*)
				ta="${ta} sh64-dis.lo sh64-opc.lo"
				archdefs="${archdefs} -DINCLUDE_SHMEDIA"
				break;;
	    esac;
	  done
				ta="${ta} sh-dis.lo" ;;
	bfd_sparc_arch)		ta="${ta} sparc-dis.lo sparc-opc.lo" ;;
	bfd_tahoe_arch)		;;
	bfd_tic30_arch)		ta="${ta} tic30-dis.lo" ;;
        bfd_tic4x_arch)         ta="${ta} tic4x-dis.lo" ;;
	bfd_tic54x_arch)	ta="${ta} tic54x-dis.lo tic54x-opc.lo" ;;
	bfd_tic80_arch)		ta="${ta} tic80-dis.lo tic80-opc.lo" ;;
	bfd_v850_arch)		ta="${ta} v850-opc.lo v850-dis.lo" ;;
	bfd_v850e_arch)		ta="${ta} v850-opc.lo v850-dis.lo" ;;
	bfd_v850ea_arch)	ta="${ta} v850-opc.lo v850-dis.lo" ;;
	bfd_vax_arch)		ta="${ta} vax-dis.lo" ;;
	bfd_w65_arch)		ta="${ta} w65-dis.lo" ;;
	bfd_we32k_arch)		;;
	bfd_x86_64_arch)	ta="${ta} i386-dis.lo i386-opc.lo" ;;
	bfd_xstormy16_arch)	ta="${ta} xstormy16-asm.lo xstormy16-desc.lo xstormy16-dis.lo xstormy16-ibld.lo xstormy16-opc.lo" using_cgen=yes ;;
	bfd_xtensa_arch)	ta="${ta} xtensa-dis.lo" ;;
	bfd_z8k_arch)		ta="${ta} z8k-dis.lo" ;;

	"")			;;
	*)		AC_MSG_ERROR([*** unknown target architecture ${arch}]) ;;
	esac
    done

    if test ${using_cgen} = yes ; then
	ta="${ta} ${cgen_files}"
    fi

    AC_PATH_PROG([UNIQ_BIN],[uniq guniq],[])
    # Weed out duplicate .o files. (what about .lo files?)
    f=""
    for i in ${ta} ; do
	case " ${f} " in
	*" ${i} "*) ;;
	*) f="${f} ${i}" ;;
	esac
    done
    ta="${f}"

    # And duplicate -D flags.
    f=""
    for i in ${archdefs} ; do
	case " ${f} " in
	*" ${i} "*) ;;
	*) f="${f} ${i}" ;;
	esac
    done
    archdefs="${f}"

    BFD_MACHINES="${ta}"

else	# all_targets is true
    archdefs=-DARCH_all
    BFD_MACHINES='$(ALL_MACHINES)'
fi

AC_SUBST([archdefs])
AC_SUBST([BFD_MACHINES])

AC_CONFIG_FILES([Makefile \
                 po/Makefile.in:po/Make-in])

AC_CONFIG_COMMANDS([default],[[sed -e '/POTFILES =/r po/POTFILES' po/Makefile.in > po/Makefile]],[[]])

AC_OUTPUT

if test "${srcdir}" != "."; then
  if test -e ${srcdir}/config.status -a ! -e `pwd`/config.status; then
    cp -v ${srcdir}/config.status `pwd`/config.status || echo ""
  elif test -e `pwd`/config.status -a ! -e ${srcdir}/config.status; then
    cp -v `pwd`/config.status ${srcdir}/config.status || echo ""
  fi
fi
