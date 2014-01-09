[= AutoGen5 template  -*- Mode: html -*-

h

##  This file is part of AutoGen.
##  AutoGen Copyright (c) 1992-2012 by Bruce Korb - all rights reserved
##
##  AutoGen is free software: you can redistribute it and/or modify it
##  under the terms of the GNU General Public License as published by the
##  Free Software Foundation, either version 3 of the License, or
##  (at your option) any later version.
##
##  AutoGen is distributed in the hope that it will be useful, but
##  WITHOUT ANY WARRANTY; without even the implied warranty of
##  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
##  See the GNU General Public License for more details.
##
##  You should have received a copy of the GNU General Public License along
##  with this program.  If not, see <http://www.gnu.org/licenses/>.

(setenv "SHELL" "/bin/sh")

=]
[=(dne " *  " "/*  ")=]
 *
 *  Generated for a [=`uname -r`=] [=`uname`=] platform
 *
 *  strsignal copyright (c) 1992-2012 by Bruce Korb - all rights reserved
 *
[=(gpl "AutoGen" " *  ")=]
 */
#ifndef MAX_SIGNAL_NUMBER
#define MAX_SIGNAL_NUMBER   [= (high-lim "signal") =]
#define SIGNAL_IN_RANGE(s)  (((unsigned)s) <= MAX_SIGNAL_NUMBER)
#define SIGNAL_NAME(s)      (zSigNames + sigNameOffset[s])
#define SIGNAL_INFO(s)      (zSigInfo  + sigInfoOffset[s])[=

(define sig-names "\"INVALID\\0\"\n")
(define name-refs "")
(define name-ix   (- (string-length sig-names) 4))
(define tmp-text  "")

(define sig-info  "")
(define info-refs "")
(define info-ix   0)

(define add-string (lambda()
  (if (exist? "signame")
      (begin
        (set! tmp-text  (string-append "\"SIG" (get "signame") "\\0\"\n"))
        (set! sig-names (string-append sig-names tmp-text))
        (set! name-refs (string-append name-refs
                        (sprintf "%d\n" name-ix)  ))
        (set! name-ix   (+ name-ix (string-length tmp-text) -4))

        (set! tmp-text  (sprintf "/* %3d */ \"%s\\0\"\n"
                                 (for-index) (get "sigtext")))
      )

      (begin
        (set! name-refs (string-append name-refs "0\n"))
        (set! tmp-text  (sprintf "/* %1$3d */ \"Signal %1$d invalid\\0\"\n"
                                 (for-index)))
  )   )

  (set! sig-info  (string-append sig-info tmp-text))
  (set! info-refs (string-append info-refs
                  (sprintf "%d\n" info-ix)  ))
  (set! info-ix   (+ info-ix (string-length tmp-text) -14))
))                      =][=

FOR signal (for-from 0) (for-by 1) =][=

  (add-string)          =][=

ENDFOR signal           =]

static char const zSigNames[] =
[=(shell (string-append
  "columns -I4 --spread=1 <<'_EOF_'\n"
  sig-names
  "_EOF_"  ))           =];

static const unsigned int sigNameOffset[] = {
[=(shell (string-append
  "columns -I4 -S, --spread=1 <<'_EOF_'\n"
  name-refs
  "_EOF_"  ))           =] };

#ifndef HAVE_SYS_SIGLIST
static char const zSigInfo[] =
[=(shell (string-append
  "columns -I4 --spread=1 <<'_EOF_'\n"
  sig-info
  "_EOF_"  ))           =];

static const unsigned int sigInfoOffset[] = {
[=(shell (string-append
  "columns -I4 -S, --spread=1 <<'_EOF_'\n"
  info-refs
  "_EOF_"  ))           =] };

#endif /* MAX_SIGNAL_NUMBER */

#ifndef HAVE_STRSIGNAL
extern char * strsignal( int signo );
#endif

#ifdef DEBUG_STRSIGNAL
#include <stdio.h>

int
main( int argc, char** argv )
{
    int sig = 0;
    fputs( "Sig  Sig-Name    Description\n"
           "===  ========    ===========\n", stdout );
    do {
        printf( "%3d  %-10s  %s\n", sig, SIGNAL_NAME(sig),
                SIGNAL_INFO(sig) );
        ++sig;
    } while (SIGNAL_IN_RANGE(sig));
    return 0;
}
#endif /* DEBUG */
#endif /* MAX_SIGNAL_NUMBER */
