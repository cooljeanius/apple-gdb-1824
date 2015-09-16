[= AutoGen5 template -*- Mode: C -*-

# Time-stamp:        "2012-01-15 08:39:12 bkorb"

##
## This file is part of AutoGen.
## AutoGen Copyright (c) 1992-2012 by Bruce Korb - all rights reserved
##
## AutoGen is free software: you can redistribute it and/or modify it
## under the terms of the GNU General Public License as published by the
## Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## AutoGen is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
## See the GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License along
## with this program.  If not, see <http://www.gnu.org/licenses/>.
##

h =]
[=

  (define ix 0)
  (define tmp-txt "")
  (define dir-tbl "")
  (define dir-enm "_EOF_\n")
  (define dir-nms "_EOF_\n")

  (string-append
     (dne " *  " "/*  ")
     "\n *"
     "\n *  copyright (c) 1992-2012 by Bruce Korb - all rights reserved"
     "\n *\n"
     (gpl "AutoGen" " *  ")
     "\n */\n"
     (make-header-guard "autogen")
  )

=]
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
static tDirProc doDir_IGNORE;[=
FOR directive    =][=

  (set! tmp-txt (get "name"))
  (set! dir-tbl (string-append dir-tbl
        (sprintf "    { %2d, zDirectives +%3d, doDir_%-10s 0 }"
                 (string-length tmp-txt) ix
                 (string-downcase! (string-append (get "name") ","))  )
        (if (last-for?) " };" ",\n")
  )     )

  (set! dir-enm (string-append dir-enm
                "DIR_" (string-upcase! (get "name")) "\n" ))

  (set! dir-nms (string-append dir-nms
                 " \"" tmp-txt "\\0\"\n" ))

  (set! ix (+ ix (string-length tmp-txt) 1))

=][=

  IF (not (exist? "dummy")) =]
static tDirProc doDir_[=name=];[=
  ELSE           =]
#define         doDir_[=name=] doDir_IGNORE[=
  ENDIF          =][=
ENDFOR directive =]

/*
 *  Define the constant string names for each directive.
 *  We supply all the needed terminating NULs, so tell the compiler
 *  the size to allocate.
 */
static char const zDirectives[[=(. ix)=]] =
[= (shellf "columns --spread=1 -I3 <<%s_EOF_" dir-nms) =];

/*
 *  Enumerate the directives
 */
typedef enum {
[= (shellf "columns -I4 -S, --spread=1 <<%s_EOF_" dir-enm) =]
} teDirectives;

/*
 *  Set up the table for handling each directive.
 */
#define DIRECTIVE_CT  [= (+ (high-lim "directive") 1) =]
static tDirTable dirTable[ DIRECTIVE_CT ] = {
[= (. dir-tbl) =]

#ifdef DAEMON_ENABLED
typedef struct inet_family_map_s {
    char const*     pz_name;
    unsigned short  nm_len;
    unsigned short  family;
} inet_family_map_t;

[= `

list=\`find /usr/include -follow -name socket.h | \
 xargs egrep '^#define[ \t]+AF_[A-Z0-9]+[ \t]+[0-9]' | \
 sed 's,^.*#define[ \t]*AF_,,;s,[ \t].*,,;/^MAX$/d'\`

set -- $list
echo "#define INET_FAMILY_TYPE_CT $#"
echo "inet_family_map_t inet_family_map[ \`expr $# + 1\` ] = {"

for f
do
   g=\`echo $f | tr '[A-Z]' '[a-z]'\`':'
   ct=\`echo $g | wc -c\`
   printf "    { %-14s %3d, AF_${f} },\\n" "\\\"${g}\\\"," ${ct}
done | sort -u

`=]
    { NULL, 0, 0 } };

#endif /* DAEMON_ENABLED */
#endif /* DEFINING */
#endif /* [=(. header-guard)=] */
/*
 *  End of [= (out-name) =] */[= #

end of directive.tpl  =]
