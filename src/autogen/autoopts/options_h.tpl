[= AutoGen5 Template -*- Mode: C -*-

h=options.h

##  This file is part of AutoOpts, a companion to AutoGen.
##  AutoOpts is free software.
##  AutoOpts is Copyright (C) 1992-2013 by Bruce Korb - all rights reserved
##
##  AutoOpts is available under any one of two licenses.  The license
##  in use must be one of these two and the choice is under the control
##  of the user of the license.
##
##   The GNU Lesser General Public License, version 3 or later
##      See the files "COPYING.lgplv3" and "COPYING.gplv3"
##
##   The Modified Berkeley Software Distribution License
##      See the file "COPYING.mbsd"
##
##  These files have the following sha256 sums:
##
##  8584710e9b04216a394078dc156b781d0b47e1729104d666658aecef8ee32e95  COPYING.gplv3
##  4379e7444a0e2ce2b12dd6f5a52a27a4d02d39d247901d3285c88cf0d37f477b  COPYING.lgplv3
##  13aa749a5b0a454917a944ed8fffc530b784f5ead522b1aacaf4ec8aa55a6239  COPYING.mbsd

=][=

(make-tmp-dir)
(dne " *  " "/*  ")

=]
 *
 *  This file defines all the global structures and special values
 *  used in the automated option processing library.
 *
 *  Automated Options Copyright [=`date +'(C) 1992-%Y'`=] by Bruce Korb
 *
[= (shell "sed -n 's/^##/ */p' " (tpl-file #t)) =]
 */
[=(make-header-guard "autoopts")=]
/** \file [= (out-name) =]
 *
 * @addtogroup autoopts
 * @{
 */
#include <sys/types.h>
#include <stdio.h>

#ifndef COMPAT_H_GUARD
/*
 * This is needed for test compilations where the "compat.h"
 * header is not usually available.
 */
#  if defined(HAVE_STDINT_H)
#    include <stdint.h>
#  elif defined(HAVE_INTTYPES_H)
#    include <inttypes.h>
#  endif /* HAVE_STDINT/INTTYPES_H */

#  if defined(HAVE_LIMITS_H)
#    include <limits.h>
#  elif defined(HAVE_SYS_LIMITS_H)
#    include <sys/limits.h>
#  endif /* HAVE_LIMITS/SYS_LIMITS_H */

#  if defined(HAVE_SYSEXITS_H)
#    include <sysexits.h>
#  endif /* HAVE_SYSEXITS_H */

#  if defined(HAVE_STDBOOL_H)
#    include <stdbool.h>
#  else
     typedef enum { false = 0, true = 1 } _Bool;
#    define bool _Bool

     /* The other macros must be usable in preprocessor directives.  */
#    define false 0
#    define true 1
#  endif /* HAVE_SYSEXITS_H */
#endif /* COMPAT_H_GUARD */
// END-CONFIGURED-HEADERS

/**
 * Defined to abnormal value of EX_USAGE.  Used to indicate that paged usage
 * was requested.  It is used to distinguish a --usage from a --help request.
 * --usage is abbreviated and --help gives as much help as possible.
 */
#define AO_EXIT_REQ_USAGE 10064

/**
 *  PUBLIC DEFINES
 *
 *  The following defines may be used in applications that need to test the
 *  state of an option.  To test against these masks and values, a pointer
 *  to an option descriptor must be obtained.  There are two ways:
 *
 *  1. inside an option processing procedure, it is the second argument,
 *     conventionally "tOptDesc* pOD".
 *
 *  2. Outside of an option procedure (or to reference a different option
 *     descriptor), use either "&DESC( opt_name )" or "&pfx_DESC( opt_name )".
 *
 *  See the relevant generated header file to determine which and what
 *  values for "opt_name" are available.
 * @group version
 * @{
 */
/// autoopts structure version
#define OPTIONS_STRUCT_VERSION      [=  vers-curr    =]
/// autoopts structure version string
#define OPTIONS_VERSION_STRING      "[= vers-sovers  =]"
/// minimum version the autoopts library supports
#define OPTIONS_MINIMUM_VERSION     [=  vers-min     =]
/// minimum version the autoopts library supports as a string
#define OPTIONS_MIN_VER_STRING      "[= vers-min-str =]"
/// the display version of the autoopts library, as a string
#define OPTIONS_DOTTED_VERSION      "[= display-ver  =]"
/// convert a version/release number pair to an integer value
#define OPTIONS_VER_TO_NUM(_v, _r)  (((_v) * 4096) + (_r))
/// @}

/**
 * Option argument types.  This must fit in the OPTST_ARG_TYPE_MASK
 * field of the fOptState field of an option descriptor (tOptDesc).
 * It will be a problem to extend beyond 4 bits.
 */
typedef enum {
    OPARG_TYPE_NONE         =  0, ///< does not take an argument
    OPARG_TYPE_STRING       =  1, ///< default type/ vanilla string
    OPARG_TYPE_ENUMERATION  =  2, ///< opt arg is an enum (keyword list)
    OPARG_TYPE_BOOLEAN      =  3, ///< opt arg is boolean-valued
    OPARG_TYPE_MEMBERSHIP   =  4, ///< opt arg sets set membership bits
    OPARG_TYPE_NUMERIC      =  5, ///< opt arg is a long int
    OPARG_TYPE_HIERARCHY    =  6, ///< option arg is hierarchical value
    OPARG_TYPE_FILE         =  7, ///< option arg names a file
    OPARG_TYPE_TIME         =  8, ///< opt arg is a time duration
    OPARG_TYPE_FLOAT        =  9, ///< opt arg is a floating point num
    OPARG_TYPE_DOUBLE       = 10, ///< opt arg is a double prec. float
    OPARG_TYPE_LONG_DOUBLE  = 11, ///< opt arg is a long double prec.
    OPARG_TYPE_LONG_LONG    = 12  ///< opt arg is a long long int
} teOptArgType;

/**
 * value descriptor for sub options
 */
typedef struct optionValue {
    teOptArgType        valType;        ///< which argument type
    char *              pzName;         ///< name of the sub-option
    union {
        char            strVal[1];      ///< OPARG_TYPE_STRING
        unsigned int    enumVal;        ///< OPARG_TYPE_ENUMERATION
        unsigned int    boolVal;        ///< OPARG_TYPE_BOOLEAN
        unsigned long   setVal;         ///< OPARG_TYPE_MEMBERSHIP
        long            longVal;        ///< OPARG_TYPE_NUMERIC
        void*           nestVal;        ///< OPARG_TYPE_HIERARCHY
    } v;
} tOptionValue;

/**
 * file argument state and handling.
 */
typedef enum {
    FTYPE_MODE_MAY_EXIST        = 0x00, ///< may or may not exist
    FTYPE_MODE_MUST_EXIST       = 0x01, ///< must pre-exist
    FTYPE_MODE_MUST_NOT_EXIST   = 0x02, ///< must *not* pre-exist
    FTYPE_MODE_EXIST_MASK       = 0x03, ///< mask for these bits
    FTYPE_MODE_NO_OPEN          = 0x00, ///< leave file closed
    FTYPE_MODE_OPEN_FD          = 0x10, ///< call open(2)
    FTYPE_MODE_FOPEN_FP         = 0x20, ///< call fopen(3)
    FTYPE_MODE_OPEN_MASK        = 0x30  ///< open/fopen/not open
} teOptFileType;

/**
 * the open flag bits or the mode string, depending on the open type.
 */
typedef union {
    int             file_flags;  ///< open(2) flag bits
    char const *    file_mode;   ///< fopen(3) mode string
} tuFileMode;

/// initial number of option argument holders to allocate
#define MIN_ARG_ALLOC_CT   6
/// amount by which to increment the argument holder allocation.
#define INCR_ARG_ALLOC_CT  8  
/**
 * an argument list.  When an option appears multiple times and
 * the values get "stacked".  \a apzArgs  holds 8 pointers initially
 * and is incremented by \a INCR_ARG_ALLOC_CT as needed.
 */
typedef struct {
    int             useCt;  ///< elements in use

    /// allocated elements, mininum \a MIN_ARG_ALLOC_CT
    /// steps by \a INCR_ARG_ALLOC_CT
    int             allocCt;
    char const *    apzArgs[MIN_ARG_ALLOC_CT]; ///< element array
} tArgList;

/**
 *  Bits in the fOptState option descriptor field.
 * @{
 */
[= `
src_dir=\`pwd\`
mk_mask() {
    hdr=${1%.def}.h
    cd ${tmp_dir}
    ${AGexe} -L ${src_dir}/tpl ${src_dir}/${1} || \
        die "Cannot process ${hdr}"
    sed -e '1,/#define.*GUARD *1/d;/^#include/d;/^#endif/,$d' $hdr
}

mk_mask opt-state.def` =]
/** @} */

#ifdef NO_OPTIONAL_OPT_ARGS
# undef  OPTST_ARG_OPTIONAL
# define OPTST_ARG_OPTIONAL   0
#endif

#define VENDOR_OPTION_VALUE   'W'

#define SELECTED_OPT(_od)     ((_od)->fOptState  & OPTST_SELECTED_MASK)
#define UNUSED_OPT(  _od)     (((_od)->fOptState & OPTST_SET_MASK) == 0)
#define DISABLED_OPT(_od)     ((_od)->fOptState  & OPTST_DISABLED)
#define OPTION_STATE(_od)     ((_od)->fOptState)
#define OPTST_SET_ARGTYPE(_n) ((_n) << OPTST_ARG_TYPE_SHIFT)
#define OPTST_GET_ARGTYPE(_f) \
    (((_f)&OPTST_ARG_TYPE_MASK) >> OPTST_ARG_TYPE_SHIFT)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  PRIVATE INTERFACES
 *
 *  The following values are used in the generated code to communicate
 *  with the option library procedures.  They are not for public use
 *  and may be subject to change.
 */

/**
 *  Define the processing state flags
 * @{
 */
[= `mk_mask proc-state.def` =]
/** @} */

#define STMTS(s)  do { s; } while (false)

/**
 *  Abbreviation for const memory character.
 */
#define tCC         char const

/**
 * Magical values for the program's option pointer
 * @{
 */
typedef enum {
    OP_VAL_EMIT_USAGE       = 1,  ///< request for usage
    OP_VAL_EMIT_SHELL       = 2,  ///< emit value for Bourne shell evaluation
    OP_VAL_RETURN_VALNAME   = 3,  ///< return the value as a string
    OP_VAL_EMIT_LIMIT       = 15  ///< limit for magic values
} opt_proc_vals_t;

/// \a OPT_VAL_EMIT_USAGE cast as a pointer
#define OPTPROC_EMIT_USAGE      ((tOptions *)OP_VAL_EMIT_USAGE)

/// \a OPT_VAL_EMIT_SHELL cast as a pointer
#define OPTPROC_EMIT_SHELL      ((tOptions *)OP_VAL_EMIT_SHELL)

/// \a OPT_VAL_RETURN_VALNAME cast as a pointer
#define OPTPROC_RETURN_VALNAME  ((tOptions *)OP_VAL_RETURN_VALNAME)

/// \a OPT_VAL_EMIT_LIMIT cast as a pointer
#define OPTPROC_EMIT_LIMIT      ((tOptions *)OP_VAL_EMIT_LIMIT)
/** @} */

/** group option processing procedure types
 * @{
 */
/** forward declaration for tOptDesc */
typedef struct opt_desc tOptDesc;
/** forward declaration for tOptiond */
typedef struct options  tOptions;

/**
 *  The option procedures do the special processing for each
 *  option flag that needs it.
 */
typedef void (tOptProc)(tOptions * pOpts, tOptDesc * pOptDesc);

/**
 * a pointer to an option processing procedure
 */
typedef tOptProc * tpOptProc;

/**
 *  The usage procedure will never return.  It calls "exit(2)"
 *  with the "exitCode" argument passed to it.
 */
// coverity[+kill]
typedef void (tUsageProc)(tOptions* pOpts, int exitCode);

/**
 * a pointer to a procedure that prints usage and exits.
 */
typedef tUsageProc * tpUsageProc;
/** @} */

/**
 *  Special definitions.  "NOLIMIT" is the 'max' value to use when
 *  a flag may appear multiple times without limit.  "NO_EQUIVALENT"
 *  is an illegal value for 'optIndex' (option description index).
 * @{
 */
#define NOLIMIT          USHRT_MAX  ///< no occurrance count limit
#define OPTION_LIMIT     SHRT_MAX   ///< maximum number of option types
/// option index to indicate no equivalance or alias
#define NO_EQUIVALENT    (OPTION_LIMIT+1)
/** @} */

/**
 * Option argument value.  Which is valid is determined by:
 * (fOptState & OPTST_ARG_TYPE_MASK) >> OPTST_ARG_TYPE_SHIFT
 * which will yield one of the teOptArgType values.
 */
typedef union {
    char const *    argString;  ///< as a string
    uintptr_t       argEnum;    ///< as an enumeration value
    uintptr_t       argIntptr;  ///< as an integer big enough to hold pointer
    long            argInt;     ///< as a long integer
    unsigned long   argUint;    ///< as an unsigned long ingeger
    unsigned int    argBool;    ///< as a boolean value
    FILE *          argFp;      ///< as a FILE * pointer
    int             argFd;      ///< as a file descriptor (int)
} opt_arg_union_t;

/// Compatibility define: \a pzLastArg is now \a optArg.argString
#define             pzLastArg       optArg.argString
/// The old amorphous argument bucket is now the opt_arg_union_t union.
#define             optArgBucket_t  opt_arg_union_t

/**
 * Enumeration of AutoOpts defined options.  The enumeration is used in
 * marking each option that is defined by AutoOpts so libopts can find
 * the correct descriptor.  This renders \a option_spec_idx_t redundant.
 */
typedef enum {
    AOUSE_USER_DEFINED = 0,     ///< user specified option
    AOUSE_RESET_OPTION,         ///< reset option state option
    AOUSE_VERSION,              ///< request version
    AOUSE_HELP,                 ///< request usage help
    AOUSE_MORE_HELP,            ///< request paged usage
    AOUSE_USAGE,                ///< request short usage
    AOUSE_SAVE_OPTS,            ///< save option state
    AOUSE_LOAD_OPTS,            ///< load options from file
    AOUSE_VENDOR_OPT            ///< specify a vendor option
} opt_usage_t;

/**
 *  Descriptor structure for each option.
 *  Only the fields marked "PUBLIC" are for public use.
 */
struct opt_desc {
    /// Public, the index of this descriptor
    uint16_t const      optIndex;
    /// Public, the flag character (value)
    uint16_t const      optValue;
    /// Public, the index of the option used to activate option
    uint16_t            optActualIndex;
    /// Public, the flag character of the activating option
    uint16_t            optActualValue;

    /// Public, the index of the equivalenced-to option.
    /// This is NO_EQUIVALENT unless activated.
    uint16_t const      optEquivIndex;
    /// Private, the minimum occurrance count
    uint16_t const      optMinCt;
    /// Private, the maximum occurrance count (NOLIMIT, if unlimited)
    uint16_t const      optMaxCt;
    /// Public, the actual occurrance count
    uint16_t            optOccCt;

    /// Public, the option processing state
    opt_state_mask_t    fOptState;
    /// Private, how the option is used (opt_usage_t)
    uint32_t            optUsage;
    /// Public, The current option argument value
    opt_arg_union_t     optArg;
    /// Public, data that is actually private to the code that handles
    /// this particular option.  It is public IFF you have your own
    /// handling function.
    void *              optCookie;

    /// Private, a list of options that must be specified when this option
    /// has been specified
    int const  * const  pOptMust;

    /// Private, a list of options that cannot be specified when this option
    /// has been specified
    int const  * const  pOptCant;

    /// Private, the function to call for handling this option
    tpOptProc    const  pOptProc;

    /// Private, usage information about this option
    char const * const  pzText;

    /// Public, the UPPER CASE, shell variable name syntax name of the option
    char const * const  pz_NAME;

    /// the unmodified name of the option
    char const * const  pz_Name;

    /// the option name to use to disable the option.  Long options names
    /// must be active.
    char const * const  pz_DisableName;

    /// the special prefix that makes the normal option name become the
    /// disablement name.
    char const * const  pz_DisablePfx;
};

/**
 *  Some options need special processing, so we store their
 *  indexes in a known place.
 */
typedef struct {
    uint16_t const  more_help;      ///< passes help text through pager
    uint16_t const  save_opts;      ///< stores option state to a file
    uint16_t const  number_option;  ///< the option "name" is an integer
    /// all arguments are options, this is the default option that must
    /// take an argument.  That argument is the unrecognized option.
    uint16_t const  default_opt;
} option_spec_idx_t;

/**
 *  The procedure generated for translating option text
 */
typedef void (tOptionXlateProc)(void);

/**
 * Everything marked "PUBLIC" is also marked "const".  Public access is not
 * a license to modify.  Other fields are used and modified by the library.
 * They are also subject to change without any notice.
 * Do not even look at these outside of libopts.
 */
struct options {
    int const                   structVersion; ///< The version of this struct
    unsigned int                origArgCt;     ///< program argument count
    char **                     origArgVect;   ///< program argument vector
    proc_state_mask_t           fOptSet;       ///< option proc. state flags
    unsigned int                curOptIdx;     ///< current option index
    char *                      pzCurOpt;      ///< current option text

    /// Public, the full path of the program
    char const * const          pzProgPath;
    /// Public, the name of the executable, without any path
    char const * const          pzProgName;
    /// Public, the upper-cased, shell variable syntax-ed program name
    char const * const          pzPROGNAME;
    /// the name of the "rc file" (configuration file)
    char const * const          pzRcName;
    /// the copyright text
    char const * const          pzCopyright;
    /// the full copyright notice
    char const * const          pzCopyNotice;
    /// a string with the program name, project name and version
    char const * const          pzFullVersion;
    /// a list of pointers to directories to search for the config file
    char const * const *        const papzHomeList;
    /// the title line for usage
    char const * const          pzUsageTitle;
    /// some added explanation for what this program is trying to do
    char const * const          pzExplain;
    /// a detailed explanation of the program's purpose, for use when
    /// full help has been requested
    char const * const          pzDetail;
    /// The public array of option descriptors
    tOptDesc   * const          pOptDesc;
    /// the email address for reporting bugs
    char const * const          pzBugAddr;

    /// Reserved for future use
    void *                      pExtensions;
    /// A copy of the option state when optionSaveState was called.
    void *                      pSavedState;

    /// The procedure to call to print usage text
    // coverity[+kill]
    tpUsageProc                 pUsageProc;
    /// The procedure to call to translate translatable option messages
    tOptionXlateProc *          pTransProc;

    /// Special option indexes.
    option_spec_idx_t           specOptIdx;
    /// the total number of options for the program
    int const                   optCt;
    /// The number of "presettable" options, though some may be marked
    /// "no-preset".  Includes all user specified options, plus a few
    /// that are specified by AutoOpts.
    int const                   presetOptCt;
    /// user specified full usage text
    char const *                pzFullUsage;
    /// user specifed short usage (usage error triggered) message
    char const *                pzShortUsage;
    /// The option argument settings active when optionSaveState was called
    opt_arg_union_t const * const originalOptArgArray;
    /// any saved cookie value
    void * const * const        originalOptArgCookie;
    /// the package data directory (e.g. global configuration files)
    char const * const          pzPkgDataDir;
    /// email address of the project packager
    char const * const          pzPackager;
};

/*
 *  Versions where in various fields first appear:
 *  ($AO_CURRENT * 4096 + $AO_REVISION, but $AO_REVISION must be zero)
 */
/**
 * The version that first stored the original argument vector
 */
#define originalOptArgArray_STRUCT_VERSION  0x20000 /* AO_CURRENT = 32 */
#define HAS_originalOptArgArray(_opt) \
    ((_opt)->structVersion >= originalOptArgArray_STRUCT_VERSION)

/**
 * The version that first stored the package data directory
 */
#define pzPkgDataDir_STRUCT_VERSION  0x22000 /* AO_CURRENT = 34 */
#define HAS_pzPkgDataDir(_opt) \
    ((_opt)->structVersion >= pzPkgDataDir_STRUCT_VERSION)

/**
 * The version that first stored the option usage in each option descriptor
 */
#define opt_usage_t_STRUCT_VERSION  0x26000 /* AO_CURRENT = 38 */
#define HAS_opt_usage_t(_opt) \
    ((_opt)->structVersion >= opt_usage_t_STRUCT_VERSION)

/**
 *  "token list" structure returned by "string_tokenize()"
 */
typedef struct {
    unsigned long   tkn_ct;      ///< number of tokens found
    unsigned char*  tkn_list[1]; ///< array of pointers to tokens
} token_list_t;

/*
 *  Hide the interface - it pollutes a POSIX claim, but leave it for
 *  anyone #include-ing this header
 */
#define strneqvcmp      option_strneqvcmp
#define streqvcmp       option_streqvcmp
#define streqvmap       option_streqvmap
#define strequate       option_strequate
#define strtransform    option_strtransform

/**
 *  Everything needed to be known about an mmap-ed file.
 *
 *  This is an output only structure used by text_mmap and text_munmap.
 *  Clients must not alter the contents and must provide it to both
 *  the text_mmap and text_munmap procedures.  BE ADVISED: if you are
 *  mapping the file with PROT_WRITE the NUL byte at the end MIGHT NOT
 *  BE WRITABLE.  In any event, that byte is not be written back
 *  to the source file.  ALSO: if "txt_data" is valid and "txt_errno"
 *  is not zero, then there *may* not be a terminating NUL.
 */
typedef struct {
    void *      txt_data;      ///< text file data
    size_t      txt_size;      ///< actual file size
    size_t      txt_full_size; ///< mmaped mem size
    int         txt_fd;        ///< file descriptor
    int         txt_zero_fd;   ///< fd for /dev/zero
    int         txt_errno;     ///< warning code
    int         txt_prot;      ///< "prot" flags
    int         txt_flags;     ///< mapping type
} tmap_info_t;

/**
 * mmap result wrapper that yields "true" when mmap has failed.
 */
#define TEXT_MMAP_FAILED_ADDR(a)  ((void*)(a) ==  (void*)MAP_FAILED)

#ifdef  __cplusplus
#define CPLUSPLUS_OPENER extern "C" {
CPLUSPLUS_OPENER
#define CPLUSPLUS_CLOSER }
#else
#define CPLUSPLUS_CLOSER
#endif

/**
 *  The following routines may be coded into AutoOpts client code:
 */[=
 (define if-text   "")
 (define note-text "")
 (define end-text  "")
 (define note-fmt
    "\n *\n * the %s function is available only if %s is%s defined")

 (out-push-new)
 (out-suspend "priv")   =][=

FOR export-func         =][=

  IF

  (if (exist? "ifndef")
      (begin
        (set! if-text    (string-append "\n#ifndef " (get "ifndef")))
        (set! note-text  (sprintf note-fmt (get "name") (get "ifndef") " not"))
        (set! end-text   (sprintf "#endif /* %s */\n" (get "ifndef")))
      )

  (if (exist? "ifdef")
      (begin
        (set! if-text    (string-append "\n#ifdef " (get "ifdef")))
        (set! note-text  (sprintf note-fmt (get "name") (get "ifdef") ""))
        (set! end-text   (sprintf "#endif /* %s */\n" (get "ifdef")))
      )

  (begin
        (set! if-text    "")
        (set! note-text  "")
        (set! end-text   "")
  )  ))

 (not (exist? "private")) =]

/**
 * [=name=] - [=what=]
 *
[=(prefix " *  " (get "doc"))=][=(. note-text)=][=
    IF (exist? "arg") =]
 *[=
       FOR arg     =][=
          (sprintf "\n * @param %-12s " (get "arg-name")) =][=arg-desc=][=
       ENDFOR arg  =][=
    ENDIF          =][=

    IF (exist? "ret-type") =]
 *
 * @return [=ret-type=] - [=ret-desc=][=

    ENDIF  =]
 */[=

  ENDIF =][=

  (if (exist? "private") (out-resume "priv"))

  if-text

=]
extern [= ?% ret-type "%s" "void"  =] [=name=]([=
   IF (not (exist? "arg"))
          =]void[=
   ELSE   =][=(join ", " (stack "arg.arg-type")) =][=
   ENDIF  =]);
[=

  (if (exist? "ifndef")
      (sprintf "\n#endif /* %s */" (get "ifndef"))
  (if (exist? "ifdef")
      (sprintf "\n#endif /* %s */"  (get "ifdef"))  ))

  (if (exist? "private") (out-suspend "priv"))
  end-text

=][=

ENDFOR export-func

=]
/*  AutoOpts PRIVATE FUNCTIONS:  */
tOptProc optionStackArg, optionUnstackArg, optionBooleanVal, optionNumericVal;
[= (out-resume "priv") (out-pop #t) =]
CPLUSPLUS_CLOSER
#endif /* [=(. header-guard)=] */
/** @}
 *
 * Local Variables:
 * c-file-style: "stroustrup"
 * indent-tabs-mode: nil
 * End:
 * [=(out-name)=] ends here */[=
# optexport.tpl ends here  =]
