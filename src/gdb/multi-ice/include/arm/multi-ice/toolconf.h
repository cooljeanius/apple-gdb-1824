/* toolconf - module to hold a tool configuration database
 Copyright (C) 2001 Free Software Foundation, Inc.


 This file is part of GDB.


 GDB is free software; you can redistribute it and/or modify it under
 the terms of the GNU General Public License as published by the Free
 Software Foundation; either version 2, or (at your option) any later
 version.


 GDB is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 for more details.


 You should have received a copy of the GNU General Public License
 along with GDB; see the file COPYING.  If not, write to the Free
 Software Foundation, 59 Temple Place - Suite 330, Boston, MA
 02111-1307, USA.  */

/*
 * RCS $Revision: 1.25 $
 * Checkin $Date: 2000/08/19 17:22:56 $
 * Revising $Author: dsinclai $
 */

#ifndef toolconf_h
#define toolconf_h


#include "host.h"


#ifdef __cplusplus
extern "C" {
#endif


    /*
     * A toolconf is opaque but can be accessed by the functions in this
     * header file. It can be read from and written to a file in text format,
     * but there are at present no functions to turn it into a binary format
     * for inclusion in another file (e.g. a project).
     *
     * A toolconf file is intended to be user editable if required. There are
     * #if and #endif commands which the user can use to force a different path
     * through the file.
     *
     * A toolconf consists of (tag, value, toolconf) entries.
     * That is, it associates a "tag" with a "value" (both plain strings) and
     * another "toolconf" (the 'child'). Here is an example file:
     *
     * { ARM710
     * Core=ARM7
     * CacheSize=8Kb
     * Architecture=v3
     * }
     * { ARM810
     * Core=ARM8
     * CacheSize=8Kb
     * Architecture=v4
     * }
     *
     * this is translated to the toolconf:
     * ((ARM710 "" ((CORE ARM7 NULL)
     *              (CACHESIZE 8Kb NULL)
     *              (ARCHITECTURE v3 NULL)))
     *  (ARM810 "" ((CORE ARM8 NULL)
     *              (CACHESIZE 8Kb NULL)
     *              (ARCHITECTURE v4 NULL))))
     *
     * (the case information is discarded from tags, but not values)
     *
     * If a tag is encountered a second time, then:
     *  - if the tag already has a value, any new value is ignored
     *  - if the tag has no value, the new value is assigned
     *  - if a child is specified, that is merged with the existing child,
     *    if one exists, or becomes the child if there is none.
     *
     */

    /*
     * the opaque toolconf type
     */
    /* Changed to use "struct hashblk;" instead of
     * typedef void *toolconf;
     * by MRN 28-May-1998 - for C++ compatibility.
     */

    struct hashblk;
    typedef struct hashblk *toolconf;

    /*
     * type for a tag.
     * programs are expected to define all the tags they use in a header
     * and then use the macros. this will remove some of the chance of error,
     * and may allow us to extend the system in the future to use numeric
     * tags.
     */
    typedef const unsigned char *tag_t;

    /*
     * functions for adding to the config database. returns the database
     * handle.
     */
    extern toolconf ToolConf_Add(toolconf hashv, const char *optstring);
    extern toolconf ToolConf_AddTagged(
                                       toolconf hashv, tag_t tag, const char *value);

    /* the "Update" versions change the value if it already exists */
    extern toolconf ToolConf_Update(toolconf hash, const char *optstring);
    extern toolconf ToolConf_UpdateTagged(
                                          toolconf hashv, tag_t tag, const char *value);

    /*
     * Delete an entry from the toolconf. The space *isn't* reusable. However,
     * when the toolconf is resized, the deleted entries aren't moved across.
     * Any child, and hence any values in a child, are *DELETED*, so take care
     * against having pointers into children.
     */
    extern toolconf ToolConf_Delete(toolconf hash, tag_t tag);

    /*
     * A general add/update function
     */

    /* First, a list of possible types */
#define tcnf_String 0x00
#define tcnf_UInt   0x01
#define tcnf_Int    0x02
#define tcnf_Bool   0x03
#define tcnf_Ptr    0x04
#define tcnf_Tag    0x05
#define tcnf_bPwr   0x06        /* k/M/G - base 1024 */
#define tcnf_dPwr   0x07        /* k/M/G - base 1000 */

#define tcnf_Update 0x100

    extern toolconf ToolConf_AddTyped(toolconf, tag_t, unsigned /*type*/, ...);
    extern toolconf ToolConf_UpdateTyped(toolconf, tag_t, unsigned /*type*/, ...);
    /* e.g. ToolConf_AddTyped(config, "Flag", tcnf_Bool, TRUE); */

    /*
     * any tag in the config can have a child.
     * a child is a separate database for that option. To add a child call
     * AddChild() instead of Add() - it returns the child config database
     * rather than the parent.
     */
    extern toolconf ToolConf_AddChild(toolconf, const char *optstring);

    /*
     * functions for extracting from the config database. The lookup and child
     * functions will search the parent for the tag if it's not found in the
     * child specified. Thus a child acts like a delta on the parent.
     */
    extern const char *ToolConf_Lookup(toolconf, tag_t tag);
    extern toolconf ToolConf_Child(toolconf, tag_t tag);
    extern toolconf ToolConf_Parent(toolconf);
    extern toolconf ToolConf_Root(toolconf);

    /*
     * These versions do *not* look into the parent
     */
    extern const char *ToolConf_FlatLookup(toolconf, tag_t tag);
    extern toolconf ToolConf_FlatChild(toolconf, tag_t tag);

    /*
     * enumerate all the tags in a config. This does not list things in the
     * parent, only the child. Similarly NumberOfTags does not go to the parent.
     * Enumeration is stopped if the "proc" returns non-0.
     */
    typedef int ToolConf_TagEnumProc(
                                     toolconf, tag_t tag, const char *value, toolconf child, void *arg);
    extern bool ToolConf_EnumerateTags(
                                       toolconf, ToolConf_TagEnumProc *, void *arg);
    /* This is like the above, but returns the first non-zero integer
     * returned by the callback, rather than the logical not thereof. */
    extern int ToolConf_IterateTags(
                                    toolconf, ToolConf_TagEnumProc *, void *arg);
    extern unsigned int ToolConf_NumberOfTags(toolconf);

    /*
     * function to read a config file. toolname is, e.g., "armsd". path is
     * the argv[0] passed to the tool. returns the database handle.
     */
    extern toolconf ToolConf_Read(
                                  toolconf, const char *path, const char *toolname);

    extern bool ToolConf_Write(toolconf hb, const char *pathname);

    /*
     * Read a toolconf in from an absolute filename
     */
    extern toolconf ToolConf_ReadFromFile(
                                          toolconf, const char *filename);

    /* Versions that take FILE * handles */
    extern toolconf ToolConf_ReadFrom(toolconf, FILE *);
    extern toolconf ToolConf_WriteTo(toolconf, FILE *);

    /* dsinclai 2000-07-17
     * toolconfs are passed - there is no static one. */
#ifdef OldCode
    extern toolconf ToolConf_Base;     /* the root hashv */
#endif

    /*
     * ToolConf_New: creates a new heap of the given size. If 'size' is -1,
     * a default size is used
     */
    extern toolconf ToolConf_New(int size);

    /*
     * A function to make a complete copy of a config database.
     */
    extern toolconf ToolConf_Clone(toolconf);

    /*
     * A function to completely destroy the config database. This wipes out
     * the entire tree (including all data), so use with care!
     * It is safe to pass NULL (does nothing).
     */
    extern void ToolConf_Reset(toolconf);

    /*
     * ToolConf_DLookupXXXX: lookup a value of a given type, with a
     * given default value.
     */
    extern bool ToolConf_DLookupBool(
                                     toolconf hash, tag_t tag, bool default_value);
    extern long ToolConf_DLookupInt(toolconf hash, tag_t tag, long default_value);
    extern unsigned long ToolConf_DLookupUInt(
                                              toolconf hash, tag_t tag, unsigned long default_value);
    extern void *ToolConf_DLookupPtr(
                                     toolconf hash, tag_t tag, void *default_value);


    /*
     * Functions that perform directly on a child
     */
    extern int ToolConf_UpdateChild(
                                    toolconf hash, tag_t parent, tag_t tag, const char *value);
    extern const char *ToolConf_LookupChild(
                                            toolconf hash, tag_t parent, tag_t tag);

    /*
     * Similarly, for integers
     */
    extern int ToolConf_DLookupChildInt(
                                        toolconf hash, tag_t parent, tag_t tag, int default_value);
    extern int ToolConf_UpdateChildInt(
                                       toolconf hash, tag_t parent, tag_t tag, int value);


    /* ==== useful functions ==== */

    /*
     * case independent compare
     */
    extern int ToolConf_Cmp(const char *s1, const char *s2);

    /*
     * take a string of form xMb/ykHz, etc., and return x*1024*1024/y*1000 etc.
     */
    extern unsigned long ToolConf_Power(const char *s, int power_of_two);

    /*
     * take option (as returned from lookup), a flags word, a bitflag,
     * and add that bitflag to the flags word as a boolean.
     * set_or_clear specifies whether to set of clear the bit.
     * values for "set" (or clear if set_or_clear is FALSE) are:
     * TRUE/YES/ON/HIGH/HI/1, and for "clear" (...) are:
     * FALSE/NO/OFF/LOW/LO/0
     */
    extern unsigned long ToolConf_AddFlag(
                                          const char *option, unsigned long flword, unsigned long flag,
                                          int set_or_clear);

    /*
     * This is used for telling the user where they went wrong.
     * It writes, as text, ':'-separated path to the child from the root.
     */
#define TOOLPATH_SEPARATOR ':'
    char *ToolConf_Path2str(toolconf child, char *txt, unsigned txt_size);


#ifdef __cplusplus
}
#endif


#endif

