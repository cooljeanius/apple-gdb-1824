[= AutoGen5 Template c=fork.c -*- Mode: C -*- =]
[= # Time-stamp:        "2011-04-03 13:51:36 bkorb"

 *  This file is part of AutoGen.
 *  AutoGen Copyright (c) 1992-2012 by Bruce Korb - all rights reserved
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
 =][=

(define up-c-name  (lambda (ag-name)
  (string-upcase! (string->c-name! (get ag-name)))  ))

(dne " *  " "/*  ")=]
 *
 *  This module will fire up autogen and have it read definitions
 *  from its standard-in.
 */

static char const zFsError[] = "%s fs ERROR %d (%s) on %s\n";

static void
add_arg(char const* pzArg, int ix)
{
    char** pArgv = xml2agOptions.origArgVect;
    if (ix >= xml2agOptions.origArgCt) {
        xml2agOptions.origArgCt += 5;
        pArgv = realloc(pArgv, sizeof( void* ) * (xml2agOptions.origArgCt+1));
        if (pArgv == NULL) {
            fprintf(stderr, "No memory for %d args\n",
                    xml2agOptions.origArgCt);
            exit(EXIT_FAILURE);
        }
        xml2agOptions.origArgVect = pArgv;
    }
    pArgv[ ix ] = (void*)pzArg;
}

static int
become_child(int * fd, char const* pzInput)
{
    if (pipe(fd) != 0) {
        fprintf(stderr, zFsError, xml2agOptions.pzProgName,
                errno, strerror( errno ), "pipe(2)");
        exit(EXIT_FAILURE);
    }

    fflush(stdout);
    fflush(stdin);

    switch (fork()) {
    case -1:
        fprintf(stderr, zFsError, xml2agOptions.pzProgName,
                errno, strerror( errno ), "fork(2)");
        exit(EXIT_FAILURE);

    case 0:
        fclose(stdin);
        if (dup2(fd[0], STDIN_FILENO) != STDIN_FILENO) {
            fprintf(stderr, zFsError, xml2agOptions.pzProgName,
                    errno, strerror( errno ), "dup2(2) w/ STDIN_FILENO");
            exit(EXIT_FAILURE);
        }
        close(fd[1]);
        break;

    default:
        errno = 0;
        outFp = fdopen(fd[1], "w");
        if (outFp == NULL) {
            fprintf(stderr, zFsError, xml2agOptions.pzProgName,
                    errno, strerror( errno ), "fdopen(2) w/ pipe[1]");
            exit(EXIT_FAILURE);
        }
        close(fd[0]);
        return 0;
    }

    if (! HAVE_OPT( BASE_NAME )) {
        if (pzInput == NULL)
            pzInput = "stdin";
        else {
            char* pz = strrchr(pzInput, '.');
            if (pz != NULL) {
                pzInput = pz = strdup(pzInput);
                pz = strrchr(pz, '.');
                *pz = '\0';
            }
        }
        SET_OPT_BASE_NAME(pzInput);
    }

    return 1;
}

void
forkAutogen(char const* pzInput)
{
    int fd[2];

    if (! become_child(fd, pzInput))
        return;

    {
        static char const zAg[] = "autogen";
        char*  pzArg;
        int    ix    = 1;

        {
            char* pz = malloc(strlen( xml2agOptions.pzProgPath ) + 7);
            char* p  = strrchr(xml2agOptions.pzProgPath, '/');

            if (p == NULL) {
                strcpy(pz, zAg);
            } else {
                size_t len = (p - xml2agOptions.pzProgPath) + 1;
                memcpy(pz, xml2agOptions.pzProgPath, len);
                strcpy(pz + len, zAg);
            }

            add_arg(pz, 0);
        }[=

        FOR flag                   =][=
          IF (define opt-name (up-c-name "name"))
             (not (~~ opt-name "OVERRIDE_TPL|OUTPUT") ) =]

        if (HAVE_OPT([=(. opt-name)=])) {[=

          CASE arg-type            =][=

          ==*  key                 =]
            static char const * kwlist[] = {
[=(shellf "${CLexe:-columns} -I16 -f'\"%%s\"' -S, --spread=2 <<_EOF_\n%s\n_EOF_"
   (join "\n" (stack "keyword"))  )=] };
            pzArg = malloc([= (+ 4 (string-length (get "name")))
                    =] + strlen( kwlist[ OPT_VALUE_[=(. opt-name)=] ] ));
            sprintf(pzArg, "--[=name=]=%s", kwlist[ OPT_VALUE_[=
                    (. opt-name)=] ]);
            add_arg(pzArg, ix++);[=

          ==*  num                 =]
            pzArg = malloc((size_t)[= (+ 16 (string-length (get "name"))) =]);
            sprintf(pzArg, "--[=name=]=%d", (int)OPT_VALUE_[=(. opt-name)=]);
            add_arg(pzArg, ix++);[=

          ==*  bool                =]
            static char z[] = "--[=name=]=false";
            if (OPT_VALUE_[=(. opt-name)=])
                strcpy(z + [= (+ 3 (string-length (get "name"))) =], "true");
            add_arg(z, ix++);[=

          ==*  str                 =][=
               IF (exist? "max")   =]
            int  optCt = STACKCT_OPT([=(. opt-name)=]);
            char const ** ppOA  = STACKLST_OPT([=(. opt-name)=]);
            do  {
                char const * pA = *(ppOA++);
                pzArg = malloc([= (+ 4 (string-length (get "name")))
                                =] + strlen(pA));
                sprintf(pzArg, "--[=name=]=%s", pA);
                add_arg(pzArg, ix++);
            } while (--optCt > 0);[=
               ELSE !exists-max    =]
            pzArg = malloc([= (+ 4 (string-length (get "name")))
                            =] + strlen( OPT_ARG( [=(. opt-name)=] )));
            sprintf(pzArg, "--[=name=]=%s", OPT_ARG( [=(. opt-name)=] ));
            add_arg(pzArg, ix++);[=
               ENDIF exists-max    =][=

          ==   ""                  =]
            add_arg("--[=name=]", ix++);[=

          ESAC arg-type            =]
        }[=
          ENDIF (not override)  =][=
        ENDFOR                     =]

        xml2agOptions.origArgVect[ix] = NULL;
        execvp(xml2agOptions.origArgVect[0], xml2agOptions.origArgVect);

        /*
         *  IF the first try fails, it may be because xml2ag and autogen have
         *  different paths.  Try again with just plain "autogen" and let
         *  the OS search "PATH" for the program.
         */
        execvp(zAg, xml2agOptions.origArgVect);
        fprintf(stderr, zFsError, xml2agOptions.pzProgName,
                errno, strerror(errno), "execvp(2)");
        exit(EXIT_FAILURE);
    }
}

/*
 * Local Variables:
 * c-file-style: "stroustrup"
 * indent-tabs-mode: nil
 * End:
 * end of autogen.c */
