/*   -*- buffer-read-only: t -*- vi: set ro:
 *  
 *  DO NOT EDIT THIS FILE   (directive.h)
 *  
 *  It has been AutoGen-ed  February 26, 2012 at 11:09:00 AM by AutoGen 5.15pre14
 *  From the definitions    directive.def
 *  and the template file   directive
 *
 *  copyright (c) 1992-2012 by Bruce Korb - all rights reserved
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
#ifndef AUTOGEN_DIRECTIVE_H_GUARD
#define AUTOGEN_DIRECTIVE_H_GUARD 1
#ifdef DEFINING

typedef char* (tDirProc)( char* pzArg, char* pzScan );

typedef struct dir_table tDirTable;
struct dir_table {
    size_t        nameSize;
    char const *  pzDirName;
    tDirProc *    pDirProc;
    int           unused;
};

/*
 *  Declare the procedures that will handle the directives.
 */
static tDirProc doDir_IGNORE;
static tDirProc doDir_assert;
static tDirProc doDir_define;
static tDirProc doDir_elif;
static tDirProc doDir_else;
static tDirProc doDir_endif;
static tDirProc doDir_endmac;
static tDirProc doDir_endshell;
static tDirProc doDir_error;
#define         doDir_ident doDir_IGNORE
static tDirProc doDir_if;
static tDirProc doDir_ifdef;
static tDirProc doDir_ifndef;
static tDirProc doDir_include;
#define         doDir_let doDir_IGNORE
static tDirProc doDir_line;
static tDirProc doDir_macdef;
static tDirProc doDir_option;
#define         doDir_pragma doDir_IGNORE
static tDirProc doDir_shell;
static tDirProc doDir_undef;

/*
 *  Define the constant string names for each directive.
 *  We supply all the needed terminating NULs, so tell the compiler
 *  the size to allocate.
 */
static char const zDirectives[124] =
    "assert\0"    "define\0"    "elif\0"      "else\0"      "endif\0"
    "endmac\0"    "endshell\0"  "error\0"     "ident\0"     "if\0"
    "ifdef\0"     "ifndef\0"    "include\0"   "let\0"       "line\0"
    "macdef\0"    "option\0"    "pragma\0"    "shell\0"     "undef\0";

/*
 *  Enumerate the directives
 */
typedef enum {
    DIR_ASSERT,   DIR_DEFINE,   DIR_ELIF,     DIR_ELSE,     DIR_ENDIF,
    DIR_ENDMAC,   DIR_ENDSHELL, DIR_ERROR,    DIR_IDENT,    DIR_IF,
    DIR_IFDEF,    DIR_IFNDEF,   DIR_INCLUDE,  DIR_LET,      DIR_LINE,
    DIR_MACDEF,   DIR_OPTION,   DIR_PRAGMA,   DIR_SHELL,    DIR_UNDEF
} teDirectives;

/*
 *  Set up the table for handling each directive.
 */
#define DIRECTIVE_CT  20
static tDirTable dirTable[ DIRECTIVE_CT ] = {
    {  6, zDirectives +  0, doDir_assert,    0 },
    {  6, zDirectives +  7, doDir_define,    0 },
    {  4, zDirectives + 14, doDir_elif,      0 },
    {  4, zDirectives + 19, doDir_else,      0 },
    {  5, zDirectives + 24, doDir_endif,     0 },
    {  6, zDirectives + 30, doDir_endmac,    0 },
    {  8, zDirectives + 37, doDir_endshell,  0 },
    {  5, zDirectives + 46, doDir_error,     0 },
    {  5, zDirectives + 52, doDir_ident,     0 },
    {  2, zDirectives + 58, doDir_if,        0 },
    {  5, zDirectives + 61, doDir_ifdef,     0 },
    {  6, zDirectives + 67, doDir_ifndef,    0 },
    {  7, zDirectives + 74, doDir_include,   0 },
    {  3, zDirectives + 82, doDir_let,       0 },
    {  4, zDirectives + 86, doDir_line,      0 },
    {  6, zDirectives + 91, doDir_macdef,    0 },
    {  6, zDirectives + 98, doDir_option,    0 },
    {  6, zDirectives +105, doDir_pragma,    0 },
    {  5, zDirectives +112, doDir_shell,     0 },
    {  5, zDirectives +118, doDir_undef,     0 } };

#ifdef DAEMON_ENABLED
typedef struct inet_family_map_s {
    char const*     pz_name;
    unsigned short  nm_len;
    unsigned short  family;
} inet_family_map_t;

#define INET_FAMILY_TYPE_CT 0
inet_family_map_t inet_family_map[ 1 ] = {
    { NULL, 0, 0 } };

#endif /* DAEMON_ENABLED */
#endif /* DEFINING */
#endif /* AUTOGEN_DIRECTIVE_H_GUARD */
/*
 *  End of directive.h */