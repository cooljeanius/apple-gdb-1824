
/**
 * @file autogen.h
 *
 *  Time-stamp:        "2012-01-29 07:43:06 bkorb"
 *
 *  Global header file for AutoGen
 *
 *  This file is part of AutoGen.
 *  AutoGen Copyright (c) 1992-2012 by Bruce Korb - all rights reserved
 *
 * AutoGen is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * AutoGen is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef AUTOGEN_BUILD
#define AUTOGEN_BUILD 1

#include "compat/compat.h"
#include "compat/unlocked-io.h"

#include REGEX_HEADER
#include <libguile/scmconfig.h>
#if GUILE_VERSION < 107000
# include <guile/gh.h>
#else
# include <libguile.h>
#endif

#include "ag-text.h"
#include "opts.h"
#include "expr.h"
#include "autoopts/autoopts.h"
#include "directive.h"
#include "snprintfv/printf.h"

#define  LOG10_2to32  10  /* rounded up */

#if defined(SHELL_ENABLED) || defined(DAEMON_ENABLED)
#  ifndef HAVE_WORKING_FORK
#    error SHELL is enabled and fork() does not work
     choke me
#  endif
#endif

#ifndef DIRCH
# if defined(_WIN32) && !defined(__CYGWIN__)
#  define DIRCH                  '\\'
# else
#  define DIRCH                  '/'
# endif
#endif

#define YYSTYPE t_word

#define ag_offsetof(TYPE, MEMBER) ((unsigned long) &((TYPE *)0)->MEMBER)

/*
 *  Dual pipe opening of a child process
 */
typedef struct {
    int     readFd;
    int     writeFd;
}  tFdPair;

typedef struct {
    FILE*   pfRead;  /* parent read fp  */
    FILE*   pfWrite; /* parent write fp */
}  tpfPair;

#define NOPROCESS   ((pid_t)-1)
#define NULLPROCESS ((pid_t)0)
#define NL          '\n'
#define TAB         '\t'

typedef unsigned char * tpChar;

#include "cgi-fsm.h"
#include "defParse-fsm.h"

typedef union {
    tpChar      pzStr;
    char        ch;
} def_token_u_t;

#define STATE_TABLE           /* set up `atexit' and load Guile   */  \
    _State_( INIT )           /* processing command line options  */  \
    _State_( OPTIONS )        /* Loading guile at option time     */  \
    _State_( GUILE_PRELOAD )  /* Loading value definitions        */  \
    _State_( LOAD_DEFS )      /* Loading library template         */  \
    _State_( LIB_LOAD )       /* Loading primary template         */  \
    _State_( LOAD_TPL )       /* processing templates             */  \
    _State_( EMITTING )       /* loading an included template     */  \
    _State_( INCLUDING )      /* end of processing before exit()  */  \
    _State_( CLEANUP )        /* Clean up code in error response  */  \
    _State_( ABORTING )       /* `exit' has been called           */  \
    _State_( DONE )

#define _State_(n)  PROC_STATE_ ## n,
typedef enum { STATE_TABLE COUNT_PROC_STATE } teProcState;
#undef _State_

#define EXPORT

typedef struct fp_stack      tFpStack;
typedef struct outSpec       tOutSpec;
typedef struct scanContext   tScanCtx;
typedef struct defEntry      tDefEntry;
typedef struct macro_desc    tMacro;
typedef struct template_desc tTemplate;
typedef struct for_info      tForInfo;
typedef struct for_state     tForState;
typedef struct template_lib_marker tTlibMark;

#define MAX_SUFFIX_LEN     8  /* maximum length of a file name suffix */
#define MAX_HEREMARK_LEN  64  /* max length of a here mark */
#define SCRIBBLE_SIZE    256  /* much larger than any short name */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  Template Library Layout
 *
 *  Procedure for loading a template function
 */
typedef tMacro* (tLoadProc)( tTemplate*, tMacro*, char const** ppzScan );
typedef tLoadProc* tpLoadProc;

typedef void (tUnloadProc)( tMacro* );
typedef tUnloadProc* tpUnloadProc;

/*
 *  Procedure for handling a template function
 *  during the text emission phase.
 */
typedef tMacro* (tHdlrProc)( tTemplate*, tMacro* );
typedef tHdlrProc* tpHdlrProc;

/*
 *  This must be included after the function prototypes
 *  (the prototypes are used in the generated tables),
 *  but before the macro descriptor structure (the function
 *  enumeration is generated here).
 */
#include "functions.h"

#define TEMPLATE_REVISION     1
#define TEMPLATE_MAGIC_MARKER {{{'A', 'G', 'L', 'B'}}, \
                               TEMPLATE_REVISION, FUNCTION_CKSUM }

struct template_lib_marker {
    union {
        unsigned char   str[4];  /* {'A', 'G', 'L', 'B'} */
        unsigned int    i[1];
    }           magic;
    unsigned short      revision;    /* TEMPLATE_REVISION    */
    unsigned short      funcSum;     /* FUNCTION_CKSUM       */
};

/*
 *  Defines for conditional expressions.
 *  The first four are an enumeration that appear in the
 *  low four bits and the next-to-lowest four bits.
 *  "PRIMARY_TYPE" and "SECONDARY_TYPE" are masks for
 *  extracting this enumeration.  The rest are flags.
 */
#define EMIT_VALUE          0x0000  /* emit value of variable  */
#define EMIT_EXPRESSION     0x0001  /* Emit Scheme result      */
#define EMIT_SHELL          0x0002  /* emit shell output       */
#define EMIT_STRING         0x0003  /* emit content of expr    */
#define EMIT_PRIMARY_TYPE   0x0007
#define EMIT_SECONDARY_TYPE 0x0070
#define EMIT_SECONDARY_SHIFT     4
#define EMIT_IF_ABSENT      0x0100
#define EMIT_ALWAYS         0x0200  /* emit one of two exprs   */
#define EMIT_FORMATTED      0x0400  /* format, if val present  */
#define EMIT_NO_DEFINE      0x0800  /* don't get defined value */

struct macro_desc {
    teFuncType    funcCode;    /* Macro function           */
    int           lineNo;      /* of macro def             */
    int           endIndex;    /* End of block macro       */
    int           sibIndex;    /* Sibling macro (ELIF or SELECT) */

    uintptr_t     ozName;      /* macro name (sometimes)   */
    uintptr_t     ozText;      /* associated text          */
    uintptr_t     res;         /* some sort of result      */
    void *        funcPrivate;
};

struct template_desc {
    tTlibMark     magic;       /* TEMPLATE_MAGIC_MARKER    */
    size_t        descSize;    /* Structure Size           */
    char *        pNext;       /* Next Pointer             */
    int           macroCt;     /* Count of Macros          */
    char const *  pzTplFile;   /* Name of template file    */
    char *        pzTplName;   /* Defined Macro Name       */
    char *        pzTemplText; /* offset of the text       */
    char          zStartMac[MAX_SUFFIX_LEN];
    char          zEndMac[MAX_SUFFIX_LEN];
    tMacro        aMacros[1];  /* Array of Macros          */
/*  char          text[...];    * strings at end of macros */
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  Name/Value Definitions Layout
 */
typedef enum {
    VALTYP_UNKNOWN = 0,
    VALTYP_TEXT,
    VALTYP_BLOCK
} teValType;


#define NO_INDEX ((short)0x80DEAD)

typedef struct sDefCtx tDefCtx;
struct sDefCtx {
    tDefEntry* pDefs;           /* ptr to current def set     */
    tDefCtx*   pPrev;           /* ptr to previous def set    */
};

typedef union {
    tDefEntry*  pDefEntry;
    char*       pzText;
} uDefValue;

struct defEntry {
    tDefEntry* pNext;           /* next member of same level  */
    tDefEntry* pTwin;           /* next member with same name */
    tDefEntry* pPrevTwin;       /* previous memb. of level    */
    tDefEntry* pEndTwin;        /* head of chain to end ptr   */
    char*      pzDefName;       /* name of this member        */
    long       index;           /* index among twins          */
    uDefValue  val;             /* string or list of children */
    char*      pzSrcFile;       /* definition file name       */
    int        srcLineNum;      /* def file source line       */
    teValType  valType;         /* text/block/not defined yet */
};

struct scanContext {
    tScanCtx *    pCtx;
    char*         pzScan;
    char const *  pzCtxFname;
    char *        pzData;
    int           lineNo;
};

struct outSpec {
    tOutSpec*   pNext;
    char const* pzFileFmt;
    ag_bool     deallocFmt;
    char        zSuffix[ 1 ];
};

#define FPF_FREE        0x0001   /* free the fp structure   */
#define FPF_UNLINK      0x0002   /* unlink file (temp file) */
#define FPF_NOUNLINK    0x0004   /* do not unlink file      */
#define FPF_STATIC_NM   0x0008   /* name statically alloced */
#define FPF_NOCHMOD     0x0010   /* do not chmod(2) file    */
#define FPF_TEMPFILE    0x0020   /* the file is a temp      */

struct fp_stack {
    int                 flags;
    tFpStack *          pPrev;
    FILE *              pFile;
    char const *        pzOutName;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  FOR loop processing state
 */
struct for_info {
    int          fi_depth;
    int          fi_alloc;
    tForState*   fi_data;
};

struct for_state {
    ag_bool      for_loading;
    int          for_from;
    int          for_to;
    int          for_by;
    int          for_index;
    char*        for_pzSep;
    char*        for_pzName;
    ag_bool      for_lastFor;
    ag_bool      for_firstFor;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  Parsing stuff
 */
typedef struct {
    int     entryCt;
    int     allocCt;
    char*   entries[1];
} tList;

#define _MkStr(_s) #_s
#define MK_STR(_s) _MkStr(_s)

typedef struct {
    int          line;
    char const * file;
    char const * text;
} file_line_t;

#define SCM_EVAL_CONST(_s) \
    do { static file_line_t const fl = { __LINE__ - 1, __FILE__, _s }; \
        pzLastScheme = fl.text; \
        ag_scm_c_eval_string_from_file_line(fl.text, fl.file, fl.line); \
    } while (0)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  GLOBAL VARIABLES
 *
 *  General Processing Globals
 */
#define pzProg      autogenOptions.pzProgName
MODE teProcState    procState        VALUE( PROC_STATE_INIT );
MODE tTemplate*     pNamedTplList    VALUE( NULL );
MODE char const *   pzOopsPrefix     VALUE( "" );
/*
 *  "evalExpression" must be able to return a distinct empty string so that
 *  the "CASE" function can distinguish an empty string due to it being a
 *  value from an empty string due to an absent definition.
 */
MODE char const     zNotDefined[1]   VALUE( "" );

/*
 *  Template Processing Globals
 */
MODE char const *   pzCurSfx         VALUE( NULL );
/**
 * The time to set for the modification times of the output files.
 */
MODE time_t         outTime          VALUE( 0 );
/**
 * The original time autogen started
 */
MODE time_t         startTime        VALUE( 0 );
MODE tFpStack *     pCurFp           VALUE( NULL );
MODE tOutSpec *     pOutSpecList     VALUE( NULL );
MODE jmp_buf        fileAbort;
MODE char *         pzCurStart       VALUE( NULL );
MODE uintptr_t      curStartOff      VALUE( 0 );
MODE tForInfo       forInfo          VALUE( { 0 } );
MODE FILE *         pfTrace          VALUE( NULL );
/**
 * temporary file name template
 */
MODE char const *   pz_temp_tpl      VALUE( NULL );
/**
 * Length of the template that is the temp directory
 */
MODE size_t         temp_tpl_dir_len VALUE( 0 );
/**
 * dependency file file pointer.
 */
MODE FILE*          pfDepends        VALUE( NULL );
/**
 * name of target of rule
 */
MODE char const *   pzDepTarget      VALUE( NULL );
/**
 * name of dependency file
 */
MODE char const *   pzDepFile        VALUE( NULL );
/**
 * base name of both source and derived files.
 * Either "_TList" or "_SList" gets put on the end.
 */
MODE char const *   pz_targ_base     VALUE( NULL );
/**
 * The actual list of input (source) files.
 */
MODE char const *   pzSourceList     VALUE( NULL );
MODE size_t         source_size      VALUE( 0 );
MODE size_t         source_used      VALUE( 0 );
MODE ag_bool        dep_phonies      VALUE( AG_FALSE );
MODE char *         cgi_stderr       VALUE( NULL );

MODE char const *   serverArgs[2]    VALUE( { NULL } );
MODE char const *   pzShellProgram   VALUE( MK_STR(CONFIG_SHELL) );

/*
 *  AutoGen definiton and template context
 *
 *  currDefCtx is the current, active list of name/value pairs.
 *  Points to its parent list for full search resolution.
 *
 *  pCurTemplate the template (and DEFINE macro) from which
 *  the current set of macros is being extracted.
 *
 *  These are set in exactly ONE place:
 *  On entry to the dispatch routine (generateBlock).  Two routines, however,
 *  must restore the values: mFunc_Define and mFunc_For.  They are the only
 *  routines that dynamically push name/value pairs on the definition stack.
 */
MODE tDefCtx        currDefCtx       VALUE( { NULL } );
MODE tDefCtx        rootDefCtx       VALUE( { NULL } );
MODE tTemplate *    pCurTemplate     VALUE( NULL );
MODE char const *   pzLastScheme     VALUE( NULL );
#ifdef DAEMON_ENABLED
/*
 *  When operating as a daemon, autogen can be told to reload
 *  its options the next time it wakes up (send it a SIGHUP).
 */
MODE ag_bool        redoOptions      VALUE( AG_TRUE );
#endif

/*
 *  Current Macro
 *
 *  This may be set in exactly three places:
 *  1.  The dispatch routine (generateBlock) that steps through
 *      a list of macros
 *  2.  mFunc_If may transfer to one of its 'ELIF' or 'ELSE'
 *      alternation macros
 *  3.  mFunc_Case may transfer to one of its selection clauses.
 */
MODE tMacro *       pCurMacro        VALUE( NULL );

/*
 *  Template Parsing Globals
 */
MODE int            templLineNo      VALUE( 1 );
MODE tScanCtx *     pBaseCtx         VALUE( NULL );
MODE tScanCtx *     pCurCtx          VALUE( NULL );
MODE tScanCtx *     pDoneCtx         VALUE( NULL );
MODE size_t         endMacLen        VALUE( 0  );
MODE char           zEndMac[   8 ]   VALUE( "" );
MODE size_t         startMacLen      VALUE( 0  );
MODE char           zStartMac[  8 ]  VALUE( "" );
MODE int            guileFailure     VALUE( 0 );
#define name_sep_ch '.'

/*
 *  Definition Parsing Globals
 */
MODE char *         pzDefineData     VALUE( NULL );
MODE char *         pz_token         VALUE( NULL );
MODE te_dp_event    lastToken        VALUE( DP_EV_INVALID );

MODE int            stackDepth       VALUE( 0 );
MODE int            stackSize        VALUE( 16 );
MODE tDefEntry *    parseStack[16]   VALUE( { 0 } );
MODE tDefEntry **   ppParseStack     VALUE( parseStack );
MODE tDefEntry *    pCurrentEntry    VALUE( NULL );

MODE autogen_exit_code_t exit_code   VALUE( AUTOGEN_EXIT_OPTION_ERROR );

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  IF we have fopencookie or funopen, then we also have our own fmemopen
 */
#ifdef ENABLE_FMEMOPEN
extern FILE * ag_fmemopen(void *buf, ssize_t len, char const *mode);
extern int    ag_fmemioctl(FILE * fp, int req, ...);
#endif

typedef union {
    const void*  cp;
    void*        p;
} v2c_t;
MODE v2c_t p2p VALUE( { NULL } );

#ifdef DEBUG_ENABLED
# define AG_ABEND(s)  ag_abend_at(s,__FILE__,__LINE__)
#else
# define AG_ABEND(s)  ag_abend_at(s)
#endif
#define  AG_CANT(_op, _wh) \
    AG_ABEND(aprf(CANNOT_FMT, errno, _op, _wh, strerror(errno)))

#ifdef DEBUG_FSM
# define DEBUG
#else
# undef  DEBUG
#endif

#define AG_ABEND_IN(t,m,s) \
    STMTS( pCurTemplate=(t); pCurMacro=(m); AG_ABEND(s);)

#if defined(DEBUG_ENABLED)
  extern void manageAllocatedData(void* pd);
#else
# define manageAllocatedData(_ptr)
#endif

#if __STDC_VERSION__ < 199901L
# if __GNUC__ >= 2
#  define __func__ __FUNCTION__
# else
#  define __func__ "<unknown>"
# endif
#endif

/*
 *  Code variations based on the version of Guile:
 */
#include "guile-iface.h"

#include "proto.h"

static inline SCM ag_eval(char const * pzStr)
{
    SCM res;
    char const * pzSaveScheme = pzLastScheme; /* Watch for nested calls */
    pzLastScheme = pzStr;

    res = ag_scm_c_eval_string_from_file_line(
        pzStr, pCurTemplate->pzTplFile, pCurMacro->lineNo);

    pzLastScheme = pzSaveScheme;
    return res;
}

/*
 *  Extracted from guile-iface stuff.  Seems to be stable since for at least
 *  1.6.0 through 2.0.0.  1.4.x is thoroughly dead now (May, 2011).
 */
#define AG_SCM_DISPLAY(_s) \
    scm_display(_s, scm_current_output_port())

#define AG_SCM_BOOT_GUILE(_ac, _av, _im) \
    scm_boot_guile((_ac), (_av), (_im), NULL)

#define AG_SCM_APPLY2(_op, _f, _tst) \
    scm_apply(_op, _f, scm_cons(_tst, AG_SCM_LISTOFNULL()))

#define AG_SCM_CHAR_P(_c)            SCM_CHARP(_c)

/*
 * Hide dummy functions from complexity measurement tools
 */
#define HIDE_FN(_t)  _t

#endif /* AUTOGEN_BUILD */
/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * indent-tabs-mode: nil
 * End:
 * end of agen5/autogen.h */
