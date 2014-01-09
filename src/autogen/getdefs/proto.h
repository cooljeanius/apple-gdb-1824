/* -*- buffer-read-only: t -*- vi: set ro:
 *
 * Prototypes for getdefs
 * Generated Sun Feb 26 11:09:15 PST 2012
 */
#ifndef GETDEFS_PROTO_H_GUARD
#define GETDEFS_PROTO_H_GUARD 1

#ifndef LOCAL
#  define LOCAL extern
#  define REDEF_LOCAL 1
#else
#  undef  REDEF_LOCAL
#endif
/*
 *  Extracted from gdemit.c
 */
LOCAL char*
emitDefinition(char* pzDef, char* pzOut);

/*
 *  Extracted from gdinit.c
 */
LOCAL void
die(char const * fmt, ...);

LOCAL void
fserr_die(char const * fmt, ...);

LOCAL void
processEmbeddedOptions(char* pzText);

LOCAL void
validateOptions(void);

/*
 *  Extracted from getdefs.c
 */
LOCAL char*
loadFile(char const * pzFname);

#ifdef REDEF_LOCAL
#  undef LOCAL
#  define LOCAL
#endif
#endif /* GETDEFS_PROTO_H_GUARD */
