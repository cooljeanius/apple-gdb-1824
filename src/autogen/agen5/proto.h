/* -*- buffer-read-only: t -*- vi: set ro:
 *
 * Prototypes for agen5
 * Generated Sun Feb 26 11:09:08 PST 2012
 */
#ifndef AGEN5_PROTO_H_GUARD
#define AGEN5_PROTO_H_GUARD 1

#ifndef LOCAL
#  define LOCAL extern
#  define REDEF_LOCAL 1
#else
#  undef  REDEF_LOCAL
#endif
/*
 *  Extracted from agCgi.c
 */
LOCAL void
loadCgi(void);

/*
 *  Extracted from agDep.c
 */
LOCAL void
add_source_file(char const * pz);

LOCAL void
rm_source_file(char const * pz);

LOCAL void
add_target_file(char const * pz);

LOCAL void
rm_target_file(char const * pz);

LOCAL void
start_dep_file(void);

LOCAL void
wrap_up_depends(void);

/*
 *  Extracted from agInit.c
 */
LOCAL void
initialize(int arg_ct, char** arg_vec);

LOCAL void
config_dep(tOptions * pOptions, tOptDesc * pOptDesc);

/*
 *  Extracted from agShell.c
 */
LOCAL void
close_server_shell(void);

LOCAL char*
shell_cmd(char const * pzCmd);

/*
 *  Extracted from agUtils.c
 */
LOCAL char *
aprf(char const * pzFmt, ...);

LOCAL void
open_trace_file(char ** av, tOptDesc * odsc);

LOCAL void
check_make_dep_env(void);

LOCAL void
doOptions(int arg_ct, char ** arg_vec);

LOCAL char const *
getDefine(char const * pzDefName, ag_bool check_env);

LOCAL char*
spanQuote(char* pzQte);

LOCAL char const *
skipScheme(char const * pzSrc,  char const * pzEnd);

LOCAL int
count_nl(char const * pz);

LOCAL char const *
skipExpression(char const * pzSrc, size_t len);

/*
 *  Extracted from autogen.c
 */
LOCAL void
ag_abend_at(char const * pzMsg
#ifdef DEBUG_ENABLED
            , char const * pzFile, int line
#endif
    );

LOCAL void *
ao_malloc (size_t sz);

LOCAL void *
ao_realloc (void *p, size_t sz);

LOCAL char *
ao_strdup (char const * str);

/*
 *  Extracted from defDirect.c
 */
LOCAL char*
processDirective(char* pzScan);

/*
 *  Extracted from defFind.c
 */
LOCAL int
canonicalizeName(char* pzD, char const* pzS, int srcLen);

LOCAL tDefEntry*
findDefEntry(char * pzName, ag_bool* pIsIndexed);

LOCAL void
print_used_defines(void);

LOCAL tDefEntry**
findEntryList(char* pzName);

/*
 *  Extracted from defLex.c
 */
LOCAL te_dp_event
yylex(void);

LOCAL void
yyerror(char* s);

/*
 *  Extracted from defLoad.c
 */
LOCAL tDefEntry*
getEntry(void);

LOCAL void
print_def(tDefEntry * pDef);

LOCAL tDefEntry*
findPlace(char* name, char const * pzIndex);

LOCAL void
readDefines(void);

LOCAL void
unloadDefs(void);

/*
 *  Extracted from expGuile.c
 */
LOCAL teGuileType
ag_scm_type_e(SCM typ);

LOCAL SCM
ag_scm_c_eval_string_from_file_line(
    char const * pzExpr, char const * pzFile, int line);

/*
 *  Extracted from expOutput.c
 */
LOCAL void
make_readonly(int fd);

LOCAL void
open_output_file(char const * fname, size_t nmsz, char const * mode, int flags);

/*
 *  Extracted from expPrint.c
 */
LOCAL SCM
run_printf(char const * pzFmt, int len, SCM alist);

/*
 *  Extracted from expString.c
 */
LOCAL void
do_multi_subs(char ** ppzStr, ssize_t * pStrLen, SCM match, SCM repl);

/*
 *  Extracted from funcDef.c
 */
LOCAL void
parseMacroArgs(tTemplate* pT, tMacro* pMac);

/*
 *  Extracted from funcEval.c
 */
LOCAL char const *
resolveSCM(SCM s);

LOCAL char const *
evalExpression(ag_bool* pMustFree);

LOCAL SCM
eval(char const* pzExpr);

/*
 *  Extracted from loadPseudo.c
 */
LOCAL char const *
do_suffix(char const * const pzData, char const * pzFileName, int lineNo);

LOCAL char const *
loadPseudoMacro(char const * pzData, char const * pzFileName);

/*
 *  Extracted from scmStrings.c
 */
LOCAL void
ag_scribble_init(void);

LOCAL void
ag_scribble_deinit(void);

LOCAL void
ag_scribble_free(void);

LOCAL char *
ag_scribble(ssize_t size);

LOCAL char *
ag_scm2zchars(SCM s, const char * type);

/*
 *  Extracted from tpLoad.c
 */
LOCAL tTemplate *
findTemplate(char const * pzTemplName);

LOCAL tSuccess
findFile(char const * pzFName,
         char * pzFullName,
         char const * const * papSuffixList,
         char const * pzReferrer);

LOCAL tTemplate *
loadTemplate(char const * pzFileName, char const * referrer);

LOCAL void
unloadTemplate(tTemplate* pT);

LOCAL void
cleanup(tTemplate* pTF);

/*
 *  Extracted from tpParse.c
 */
LOCAL tMacro*
parseTemplate(tMacro* pM, char const ** ppzText);

/*
 *  Extracted from tpProcess.c
 */
LOCAL void
generateBlock(tTemplate * pT, tMacro * pMac, tMacro * pEnd);

LOCAL tOutSpec *
nextOutSpec(tOutSpec * pOS);

LOCAL void
processTemplate(tTemplate* pTF);

LOCAL void
out_close(ag_bool purge);

#ifdef REDEF_LOCAL
#  undef LOCAL
#  define LOCAL
#endif
#endif /* AGEN5_PROTO_H_GUARD */
