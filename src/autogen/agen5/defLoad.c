
/**
 * @file defLoad.c
 *
 *  Time-stamp:        "2011-12-30 15:33:37 bkorb"
 *
 *  This module loads the definitions, calls yyparse to decipher them,
 *  and then makes a fixup pass to point all children definitions to
 *  their parent definition.
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

typedef enum {
    INPUT_DONE,
    INPUT_STDIN,
    INPUT_FILE
} def_input_mode_t;

static tDefEntry* pFreeEntryList = NULL;
static void*      pAllocList     = NULL;

#define ENTRY_SPACE        (4096 - sizeof(void*))
#define ENTRY_ALLOC_CT     (ENTRY_SPACE / sizeof(tDefEntry))
#define ENTRY_ALLOC_SIZE   \
    ((ENTRY_ALLOC_CT * sizeof(tDefEntry)) + sizeof(void*))

/* = = = START-STATIC-FORWARD = = = */
static tDefEntry*
insertDef(tDefEntry* pDef);

static def_input_mode_t
ready_input(char const ** ppzfile, size_t * psz);
/* = = = END-STATIC-FORWARD = = = */

#if defined(DEBUG_ENABLED)
void
manageAllocatedData(void* pd)
{
    static int    allocPtrCt   = 0;
    static int    usedPtrCt    = 0;
    static void** papAllocData = NULL;

    if (pd == NULL) {
        void** pp = papAllocData;
        if (pp == NULL)
            return;

        while (--usedPtrCt >= 0)
            AGFREE(*(pp++));

        AGFREE(papAllocData);
        papAllocData = NULL;

    } else {
        if (++usedPtrCt > allocPtrCt) {
            allocPtrCt += 16;
            papAllocData = (usedPtrCt > 1)
                ? AGREALOC(papAllocData, allocPtrCt * sizeof(void*), "atbl")
                : AGALOC(allocPtrCt * sizeof(void*), "atbl");
        }
        papAllocData[usedPtrCt-1] = pd;
    }
}
#endif

LOCAL tDefEntry*
getEntry(void)
{
    tDefEntry*  pRes = pFreeEntryList;

    if (pRes != NULL) {
        pFreeEntryList = pRes->pNext;

    } else {
        int   ct = ENTRY_ALLOC_CT-1;
        void* p  = AGALOC(ENTRY_ALLOC_SIZE, "def headers");

        manageAllocatedData(p);

        *((void**)p) = pAllocList;
        pAllocList   = p;
        pRes = pFreeEntryList = (tDefEntry*)((void**)p + 1);

        /*
         *  This is a post-loop test loop.  It will cycle one fewer times
         *  than there are 'tDefEntry' structs in the memory we just alloced.
         */
        do  {
            tDefEntry* pNxt = pRes+1;
            pRes->pNext = pNxt;

            /*
             *  When the loop ends, "pRes" will point to the last allocated
             *  structure instance.  That is the one we will return.
             */
            pRes = pNxt;
        } while (--ct > 0);

        /*
         *  Unlink the last entry from the chain.  The next time this
         *  routine is called, the *FIRST* structure in this list will
         *  be returned.
         */
        pRes[-1].pNext = NULL;
    }

    memset((void*)pRes, 0, sizeof(*pRes));
    return pRes;
}

/*
 *  Append a new entry at the end of a sibling (or twin) list.
 */
LOCAL void
print_def(tDefEntry * pDef)
{
    int ix = 32 - (2 * stackDepth);
    char const * space = PRINT_DEF_SPACES + ((ix < 0) ? 0 : ix);

    char const * vtyp;

    switch (pDef->valType) {
    case VALTYP_UNKNOWN: vtyp = DEF_TYPE_UNKNOWN; break;
    case VALTYP_TEXT:    vtyp = DEF_TYPE_TEXT;    break;
    case VALTYP_BLOCK:   vtyp = DEF_TYPE_BLOCK;   break;
    default:             vtyp = DEF_TYPE_INVALID; break;
    }

    fprintf(pfTrace, PRINT_DEF_SHOW_FMT,
            space,
            pDef->pzDefName, (unsigned int)pDef->index,
            vtyp,
            pDef->pzSrcFile, pDef->srcLineNum);
}

/*
 *  Append a new entry at the end of a sibling (or twin) list.
 */
static tDefEntry*
insertDef(tDefEntry* pDef)
{
    tDefEntry* pList = ppParseStack[ stackDepth ];

    /*
     *  If the current level is empty, then just insert this one and quit.
     */
    if (pList->val.pDefEntry == NULL) {
        if (pDef->index == NO_INDEX)
            pDef->index = 0;
        pList->val.pDefEntry = pDef;

        return pDef;
    }
    pList = pList->val.pDefEntry;

    /*
     *  Scan the list looking for a "twin" (same-named entry).
     */
    while (strcmp(pDef->pzDefName, pList->pzDefName) != 0) {
        /*
         *  IF we are at the end of the list,
         *  THEN put the new entry at the end of the list.
         *       This is a new name in the current context.
         *       The value type is forced to be the same type.
         */
        if (pList->pNext == NULL) {
            pList->pNext = pDef;

            if (pDef->index == NO_INDEX)
                pDef->index = 0;

            return pDef;
        }

        /*
         *  Check the next sibling for a twin value.
         */
        pList = pList->pNext;
    }

    /*  * * * * *  WE HAVE FOUND A TWIN
     *
     *  Link in the new twin chain entry into the list.
     */
    if (pDef->index == NO_INDEX) {
        tDefEntry* pT = pList->pEndTwin;
        if (pT == NULL)
            pT = pList;

        pDef->index = pT->index + 1;
        pT->pTwin = pDef;
        pDef->pPrevTwin = pT;
        pList->pEndTwin = pDef;

    } else if (pList->index > pDef->index) {

        /*
         *  Insert the new entry before any other in the list.
         *  We actually do this by leaving the pList pointer alone and swapping
         *  the contents of the definition entry.
         */
        tDefEntry def = *pDef;

        memcpy(&(pDef->pzDefName), &(pList->pzDefName),
               sizeof(def) - ag_offsetof(tDefEntry, pzDefName));

        memcpy(&(pList->pzDefName), &(def.pzDefName),
               sizeof(def) - ag_offsetof(tDefEntry, pzDefName));

        /*
         * Contents are swapped.  Link "pDef" after "pList" and return "pList".
         */
        pDef->pTwin = pList->pTwin;
        if (pDef->pTwin != NULL)
            pDef->pTwin->pPrevTwin = pDef;

        pDef->pPrevTwin = pList;
        pList->pTwin  = pDef;

        /*
         *  IF this is the first twin, then the original list head is now
         *  the "end twin".
         */
        if (pList->pEndTwin == NULL)
            pList->pEndTwin = pDef;

        pDef = pList;  /* Return the replacement structure address */

    } else {
        tDefEntry* pL = pList;
        tDefEntry* pT = pL->pTwin;

        /*
         *  Insert someplace after the first entry.  Scan the list until
         *  we either find a larger index or we get to the end.
         */
        while ((pT != NULL) && (pT->index < pDef->index)) {
            pL = pT;
            pT = pT->pTwin;
        }

        pDef->pTwin = pT;

        pL->pTwin = pDef;
        pDef->pPrevTwin = pL;
        if (pT == NULL)
            pList->pEndTwin = pDef;
        else
            pT->pPrevTwin = pDef;
    }

    return pDef; /* sometimes will change */
}

/**
 * Figure out where to insert an entry in a list of twins.
 */
LOCAL tDefEntry*
findPlace(char* name, char const * pzIndex)
{
    tDefEntry* pE = getEntry();

    pE->pzDefName = name;

    if (pzIndex == NULL)
        pE->index = NO_INDEX;

    else if (IS_DEC_DIGIT_CHAR(*pzIndex) || (*pzIndex == '-'))
        pE->index = strtol(pzIndex, NULL, 0);

    else {
        pzIndex = getDefine(pzIndex, AG_TRUE);
        if (pzIndex != NULL)
             pE->index = strtol(pzIndex, NULL, 0);
        else pE->index = NO_INDEX;
    }

    strtransform(pE->pzDefName, pE->pzDefName);
    pE->valType     = VALTYP_UNKNOWN;
    pE->pzSrcFile   = (char*)pCurCtx->pzCtxFname;
    pE->srcLineNum  = pCurCtx->lineNo;
    return (pCurrentEntry = insertDef(pE));
}

/**
 * figure out which file descriptor to use for reading definitions.
 */
static def_input_mode_t
ready_input(char const ** ppzfile, size_t * psz)
{
    struct stat stbf;

    if (! ENABLED_OPT(DEFINITIONS)) {
        pBaseCtx = (tScanCtx*)AGALOC(sizeof(tScanCtx), "scan context");
        memset((void*)pBaseCtx, 0, sizeof(tScanCtx));
        pBaseCtx->lineNo     = 1;
        pBaseCtx->pzCtxFname = READY_INPUT_NODEF;
        manageAllocatedData(pBaseCtx);

        if (! ENABLED_OPT(SOURCE_TIME))
            outTime = time(NULL);
        return INPUT_DONE;
    }

    *ppzfile = OPT_ARG(DEFINITIONS);

    if (OPT_VALUE_TRACE >= TRACE_EXPRESSIONS)
        fprintf(pfTrace, TRACE_DEF_LOAD);

    /*
     *  Check for stdin as the input file.  We use the current time
     *  as the modification time for stdin.  We also note it so we
     *  do not try to open it and we try to allocate more memory if
     *  the stdin input exceeds our initial allocation of 16K.
     */
    if (strcmp(*ppzfile, "-") == 0) {
        *ppzfile = OPT_ARG(DEFINITIONS) = "stdin";
        if (getenv(REQUEST_METHOD) != NULL) {
            loadCgi();
            pCurCtx = pBaseCtx;
            dp_run_fsm();
            return INPUT_DONE;
        }

    accept_fifo:
        outTime  = time(NULL);
        *psz = 0x4000 - (4+sizeof(*pBaseCtx));
        return INPUT_STDIN;
    }

    /*
     *  This, then, must be a regular file.  Make sure of that and
     *  find out how big it was and when it was last modified.
     */
    if (stat(*ppzfile, &stbf) != 0)
        AG_CANT(READY_INPUT_STAT, *ppzfile);

    if (! S_ISREG(stbf.st_mode)) {
        if (S_ISFIFO(stbf.st_mode))
            goto accept_fifo;

        errno = EINVAL;
        AG_CANT(READY_INPUT_NOT_REG, *ppzfile);
    }

    /*
     *  IF the source-time option has been enabled, then
     *  our output file mod time will start as one second after
     *  the mod time on this file.  If any of the template files
     *  are more recent, then it will be adjusted.
     */
    *psz = stbf.st_size;

    if (ENABLED_OPT(SOURCE_TIME))
         outTime = stbf.st_mtime + 1;
    else outTime = time(NULL);

    return INPUT_FILE;
}

/**
 *  Suck in the entire definitions file and parse it.
 */
LOCAL void
readDefines(void)
{
    char const *  pzDefFile;
    char *        pzData;
    size_t        dataSize;
    size_t        sizeLeft;
    FILE *        fp;
    def_input_mode_t in_mode = ready_input(&pzDefFile, &dataSize);

    if (in_mode == INPUT_DONE)
        return;

    /*
     *  Allocate the space we need for our definitions.
     */
    sizeLeft = dataSize+4+sizeof(*pBaseCtx);
    pBaseCtx = (tScanCtx*)AGALOC(sizeLeft, "file buf");
    memset((void*)pBaseCtx, 0, sizeLeft);
    pBaseCtx->lineNo = 1;
    sizeLeft = dataSize;

    /*
     *  Our base context will have its currency pointer set to this
     *  input.  It is also a scanning pointer, but since this buffer
     *  is never deallocated, we do not have to remember the initial
     *  value.  (It may get reallocated here in this routine, tho...)
     */
    pzData =
        pBaseCtx->pzScan =
        pBaseCtx->pzData = (char*)(pBaseCtx+1);
    pBaseCtx->pCtx = NULL;

    /*
     *  Set the input file pointer, as needed
     */
    if (in_mode == INPUT_STDIN)
        fp = stdin;

    else {
        fp = fopen(pzDefFile, "r" FOPEN_TEXT_FLAG);
        if (fp == NULL)
            AG_CANT(READ_DEF_OPEN, pzDefFile);

        if (pfDepends != NULL)
            add_source_file(pzDefFile);
    }

    /*
     *  Read until done...
     */
    for (;;) {
        size_t rdct = fread((void*)pzData, (size_t)1, sizeLeft, fp);

        /*
         *  IF we are done,
         */
        if (rdct == 0) {
            /*
             *  IF it is because we are at EOF, then break out
             *  ELSE abend.
             */
            if (feof(fp) || (in_mode == INPUT_STDIN))
                break;

            AG_CANT(READ_DEF_READ, pzDefFile);
        }

        /*
         *  Advance input pointer, decrease remaining count
         */
        pzData   += rdct;
        sizeLeft -= rdct;

        /*
         *  See if there is any space left
         */
        if (sizeLeft == 0) {
            tScanCtx* p;
            off_t dataOff;

            /*
             *  IF it is a regular file, then we are done
             */
            if (in_mode != INPUT_STDIN)
                break;

            /*
             *  We have more data and we are out of space.
             *  Try to reallocate our input buffer.
             */
            dataSize += (sizeLeft = 0x1000);
            dataOff = pzData - pBaseCtx->pzData;
            p = AGREALOC((void*)pBaseCtx, dataSize+4+sizeof(*pBaseCtx),
                         "expand f buf");

            /*
             *  The buffer may have moved.  Set the data pointer at an
             *  offset within the new buffer and make sure our base pointer
             *  has been corrected as well.
             */
            if (p != pBaseCtx) {
                p->pzScan = \
                    p->pzData = (char*)(p+1);
                pzData = p->pzData + dataOff;
                pBaseCtx = p;
            }
        }
    }

    if (pzData == pBaseCtx->pzData)
        AG_ABEND(READ_DEF_NO_DEFS);

    *pzData = NUL;
    AGDUPSTR(pBaseCtx->pzCtxFname, pzDefFile, "def file name");
    manageAllocatedData(pBaseCtx);
    manageAllocatedData((void*)pBaseCtx->pzCtxFname);

    /*
     *  Close the input file, parse the data
     *  and alphabetically sort the definition tree contents.
     */
    if (in_mode != INPUT_STDIN)
        fclose(fp);

    pCurCtx = pBaseCtx;
    dp_run_fsm();
}


LOCAL void
unloadDefs(void)
{
    return;
}
/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * indent-tabs-mode: nil
 * End:
 * end of agen5/defLoad.c */
