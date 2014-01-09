/**
 * \file gdemit.c
 *
 *  getdefs Copyright (c) 1999-2012 by Bruce Korb - all rights reserved
 *
 *  Author:            Bruce Korb <bkorb@gnu.org>
 *  Time-stamp:        "2012-02-12 09:59:30 bkorb"
 *
 *  This file is part of AutoGen.
 *  AutoGen copyright (c) 1992-2012 by Bruce Korb - all rights reserved
 *
 *  AutoGen is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  AutoGen is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* = = = START-STATIC-FORWARD = = = */
static void
compress_def(char * pz);

static char *
list_attrib(char * pzText, char * pzOut);

static char *
emit_quote(char ** ppzText, char * pzOut);

static void
next_def_entry(char ** txt_pp, char const ** def_pp);

static void
emit_attribute(char const ** def_pp, char ** out_pp);

static char*
emit_subblock(char const * pzDefList, char * pzText, char * pzOut);

static char*
subblock_str(char ** ppzText, uint_t sepChar, char * pzOut);
/* = = = END-STATIC-FORWARD = = = */

/*
 *  compress_def
 *
 *  Compress the definition text.  Each input line has some prefix
 *  stuff to ensure it is a comment as seen by the normal processor
 *  of the file.  In "C", the entire block is surrounded by the
 *  '/'-'*' and '*'-'/' pairs.  In shell, every line would start
 *  with a hash character ('#').  Etc.  To make life easy, we require
 *  that every line be prefixed with something that matches the
 *  pattern:
 *
 *        "^[^*]*\*+"
 *
 *  and any line that does not is ignored.  So, here we strip off
 *  that prefix before we go ahead and try to parse it.
 */
static void
compress_def(char * pz)
{
    char * pzStrt = pz;
    char * pzDest = pz;
    char * pzSrc  = pz;
    int    nlCt   = 0;

    /*
     *  Search until we find a line that contains an asterisk
     *  and is followed by something other than whitespace.
     */
    nlCt =  0;

skip_leading_space:
    while (isspace( *pzSrc )) {
        if (*(pzSrc++) == '\n') {
            nlCt++;
            for (;;) {
                switch (*(pzSrc++)) {
                case '*':
                    while (*pzSrc == '*')     pzSrc++;
                    goto skip_leading_space;

                case NUL:
                    *pzStrt = NUL;
                    return;

                case '\n':
                    nlCt++;
                }
            }
        }
    }

    if (*pzSrc == NUL) {
        *pzStrt = NUL;
        return;
    }

    /*
     *  FOR as long as we still have more text, ...
     */
    for (;;) {
        /*
         *  IF we passed over one or more newlines while looking for
         *  an asterisk, then insert one extra newline into the output
         */
        if (nlCt > 0) {
            *pzDest++ = '\n';
            nlCt =  0;
        }

        /*
         *  FOR all the data on the current input line, ...
         */
        for (;;) {
            /*
             *  Move the source to destination until we find
             *  either a new-line or a NUL.
             */
            switch (*pzDest++ = *pzSrc++) {
            case '\n':
                if (*pzSrc != NUL)
                    goto lineDone;

            case NUL:
                pzDest--;
                goto compressDone;

            default:
                ;
            }
        } lineDone:;

        /*
         *  Trim trailing white space off the end of the line.
         */
        if ((pzDest[-2] == ' ') || (pzDest[-2] == '\t')) {
            do  {
                pzDest--;
            } while ((pzDest[-2] == ' ') || (pzDest[-2] == '\t'));
            pzDest[-1] = '\n';
        }

        /*
         *  We found a new-line.  Skip forward to an asterisk.
         */
    foundNewline:
        while (*pzSrc != '*') {
            if (*pzSrc == NUL)
                goto compressDone;
            if (*pzSrc == '\n')
                nlCt++;
            pzSrc++;
        }

        /*
         *  Skip over the asterisk we found and all the ones that follow
         */
        while (*pzSrc == '*')     pzSrc++;
        while (isspace( *pzSrc )) {
            /*
             *  IF we stumble into another newline,
             *  THEN we go back to look for an asterisk.
             */
            if (*pzSrc == '\n')
                goto foundNewline;
            pzSrc++;
        }
    } compressDone:;

    /*
     *  Trim off all the trailing white space, including newlines
     */
    while ((pzDest > pzStrt) && isspace( pzDest[-1] )) pzDest--;
    *pzDest = NUL;
}


/*
 *  emitDefinition
 */
LOCAL char*
emitDefinition(char* pzDef, char* pzOut)
{
    char sep_char;
    char zEntryName[ MAXNAMELEN ];

    /*
     *  Indent attribute definitions four spaces
     */
    {
        char*  p = zEntryName;
        *pzOut++ = ' '; *pzOut++ = ' '; *pzOut++ = ' '; *pzOut++ = ' ';

        while (AG_NAME_CHAR(*pzDef))
            *p++ = *pzOut++ = *pzDef++;

        if (p >= zEntryName + sizeof(zEntryName))
            die("names are constrained to %d bytes\n", MAXNAMELEN);

        *p = NUL;
    }

    /*
     *  Strip the prefixes from all the definition lines
     *  (viz., the "^.*\*" text, except that it is a shortest match
     *  instead of longest match).  Skip the ':' before starting.
     */
    compress_def(++pzDef);

    if (HAVE_OPT( SUBBLOCK )) {
        int    ct  = STACKCT_OPT(  SUBBLOCK );
        char const **  ppz = STACKLST_OPT( SUBBLOCK );

        do  {
            char const * pz = *ppz++;
            if (strcmp(pz, zEntryName) == 0)
                return emit_subblock(pz, pzDef, pzOut);
        } while (--ct > 0);
    }

    if (HAVE_OPT( LISTATTR )) {
        int    ct  = STACKCT_OPT(  LISTATTR );
        char const ** ppz = STACKLST_OPT( LISTATTR );

        do  {
            if (strcmp(*ppz++, zEntryName) == 0)
                return list_attrib(pzDef, pzOut);
        } while (--ct > 0);
    }

    if (isspace(*pzDef))
         sep_char = *pzDef++;
    else sep_char = ' ';

    switch (*pzDef) {
    case NUL:
        *pzOut++ = ';'; *pzOut++ = '\n';
        break;

    case '"':
    case '\'':
    case '{':
        /*
         *  Quoted entries or subblocks do their own stringification
         *  sprintf is safe because we are copying strings around
         *  and *always* making the result smaller than the original
         */
        pzOut += sprintf(pzOut, " =%c%s;\n", sep_char, pzDef);
        break;

    default:
        *pzOut++ = ' '; *pzOut++ = '='; *pzOut++ = sep_char;
        *pzOut++ = '\'';

        for (;;) {
            switch (*pzOut++ = *pzDef++) {
            case '\\':
                *pzOut++  = '\\';
                break;

            case '\'':
                pzOut[-1] = '\\';
                *pzOut++  = '\'';
                break;

            case NUL:
                goto unquotedDone;
            }
        } unquotedDone:;
        pzOut[-1] = '\''; *pzOut++ = ';'; *pzOut++ = '\n';
        break;
    }
    return pzOut;
}


/*
 *  list_attrib
 */
static char *
list_attrib(char * pzText, char * pzOut)
{
    static char const  zStart[]  = " = ";

    uint_t sepChar   = ',';
    int    FirstAttr = 1;

    strcpy(pzOut, zStart);
    pzOut += sizeof(zStart) - 1;

    /*
     *  See if there is an alternate separator character.
     *  It must be a punctuation character that is not also
     *  a quote character.
     */
    if (ispunct(*pzText) && (*pzText != '"') && (*pzText != '\''))
        sepChar = (uint_t)*(pzText++);
    while (isspace(*pzText)) pzText++;

    /*
     *  Loop for as long as we have text entries
     */
    while (*pzText != NUL) {

        if (FirstAttr)
            FirstAttr = 0;
        else
            *(pzOut++) = ',';

        /*
         *  If the first thing we find is the separator char,
         *  then emit the empty string.
         */
        if ((uint_t)*pzText == sepChar) {
            *(pzOut++) = '\''; *(pzOut++) = '\'';
            pzText++;
            continue;
        }

        /*
         *  Emit whatever we have.  The call will consume any trailing
         *  separator character.
         */
        pzOut = subblock_str(&pzText, sepChar, pzOut);
    }

    /*
     *  IF there were no definitions, THEN emit an empty one
     */
    if (FirstAttr)
        pzOut -= sizeof(zStart) - 1;

    *(pzOut++) = ';';
    *(pzOut++) = '\n';

    return pzOut;
}


/*
 *  The text is quoted, so copy it as is, ensuring that escaped
 *  characters are not used to end the quoted text.
 */
static char *
emit_quote(char ** ppzText, char * pzOut)
{
    char*  pzText = *ppzText;
    char   svch   = (*pzOut++ = *pzText++);

    for (;;) {
        switch (*pzOut++ = *pzText++) {

        case '\\':
            if ((*pzOut++ = *pzText++) != NUL)
                break;

        case NUL:
            pzText--;
            pzOut[-1] = svch;
            svch = NUL;
            /* FALLTHROUGH */

        case '"':
        case '\'':
            if (pzOut[-1] == svch)
                goto quoteDone;

            break;
        }
    }

quoteDone:
    *ppzText = pzText;
    return pzOut;
}

static void
next_def_entry(char ** txt_pp, char const ** def_pp)
{
    char const * p = *def_pp;
    (*txt_pp)++;
    for (;;) {
        switch (*++p) {
        case ' ': p++; /* FALLTHROUGH */
        case NUL:
            *def_pp = p;
            return;
        }
    }
}

static void
emit_attribute(char const ** def_pp, char ** out_pp)
{
    static char const pfx[]   = "\n        ";

    char * out = *out_pp;
    char const * def = *def_pp;

    memcpy(out, pfx, sizeof(pfx) - 1);
    out += sizeof(pfx) - 1;

    for (;;) {
        *out++ = *def++;
        switch (*def) {
        case ' ': def++; /* FALLTHROUGH */
        case NUL:
            goto leave_emit_attribute;
        }
    }

leave_emit_attribute:

    *out++  = ';';
    *out_pp = out;
    *def_pp = def;
}

/*
 *  emit_subblock
 */
static char*
emit_subblock(char const * pzDefList, char * pzText, char * pzOut)
{
    static char const zStart[]  = " = {";
    static char const zEnd[]    = "\n    };\n";

    uint_t sepChar   = ',';
    int    FirstAttr = 1;

    /*
     *  Advance past subblock name to the entry name list
     */
    pzDefList += strlen(pzDefList) + 1;
    strcpy(pzOut, zStart);
    pzOut += sizeof(zStart) - 1;

    /*
     *  See if there is an alternate separator character.
     *  It must be a punctuation character that is not also
     *  a quote character.
     */
    if (ispunct(*pzText) && (*pzText != '"') && (*pzText != '\''))
        sepChar = (uint_t)*(pzText++);

    /*
     *  Loop for as long as we have text entries and subblock
     *  attribute names, ...
     */
    do  {
        /*
         *  IF the first character is the separator,
         *  THEN this entry is skipped.
         */
        if ((uint_t)*pzText == sepChar) {
            next_def_entry(&pzText, &pzDefList);
            continue;
        }

        /*
         *  Skip leading white space in the attribute and check for done.
         */
        while (isspace(*pzText)) pzText++;
        if (*pzText == NUL) {
            /*
             *  IF there were no definitions, THEN emit one anyway
             */
            if (FirstAttr)
                emit_attribute(&pzDefList, &pzOut);

            break;
        }

        /*
         *  Copy out the attribute name
         */
        emit_attribute(&pzDefList, &pzOut);
        FirstAttr = 0;

        /*
         *  IF there are no data for this attribute,
         *  THEN we leave the definition empty.
         */
        if ((uint_t)*pzText == sepChar) {
            pzText++;
            continue;
        }

        /*
         *  Copy out the assignment operator and emit the string
         */
        pzOut[-1] = ' '; *pzOut++ = '='; *pzOut++ = ' ';
        pzOut = subblock_str(&pzText, sepChar, pzOut);
        *pzOut++ = ';';

    } while (isalpha(*pzDefList));

    memcpy(pzOut, zEnd, sizeof(zEnd));
    return pzOut + sizeof(zEnd) - 1;
}


/*
 *  Emit a string in a fashion that autogen will be able to
 *  correctly reconstruct it.
 */
static char*
subblock_str(char ** ppzText, uint_t sepChar, char * pzOut)
{
    char * pzText  = *ppzText;
    char * pcComma;
    char * pcEnd;

    /*
     *  Skip leading space
     */
    while (isspace(*pzText)) pzText++;

    /*
     *  IF the text is already quoted,
     *  THEN call the quoted text emitting routine
     */
    if ((*pzText == '"') || (*pzText == '\'')) {
        pzOut = emit_quote(&pzText, pzOut);

        /*
         *  Make sure we strip off trailing white space and any
         *  separation character.
         */
        while (isspace(*pzText)) pzText++;
        if (*pzText == sepChar) {
            pzText++;
            while (isspace(*pzText)) pzText++;
        }
        *ppzText = pzText;
        return pzOut;
    }

    /*
     *  Look for the character that separates this entry text
     *  from the entry text for the next attribute.  Leave 'pcComma'
     *  pointing to the character _before_ the character where we
     *  are to resume our text scan.  (i.e. at the comma, or the
     *  last character in the string)
     */
    pcComma = strchr(pzText, (int)sepChar);
    if (pcComma == (char*)NULL) {
        pcEnd = pzText + strlen(pzText);
        pcComma = pcEnd-1;
    } else {
        pcEnd = pcComma;
    }

    /*
     *  Clean off trailing white space.
     */
    while ((pcEnd > pzText) && isspace(pcEnd[-1])) pcEnd--;

    /*
     *  Copy the text, surrounded by single quotes
     */
    *pzOut++ = '\'';
    {
        char svch = *pcEnd;
        *pcEnd = NUL;
        for (;;) {
            char ch = *pzText++;
            switch (ch) {
            case '\'':
                *pzOut++ = '\\';
            default:
                *pzOut++ = ch;
                break;
            case NUL:
                goto copyDone;
            }
        } copyDone: ;

        pzText = pcComma+1;
        *pcEnd = svch;
    }

    *pzOut++ = '\'';
    while (isspace(*pzText)) pzText++;
    *ppzText = pzText;
    return pzOut;
}

/* emacs
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * indent-tabs-mode: nil
 * End:
 * end of getdefs/gdemit.c */
