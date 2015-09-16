[= AutoGen5 Template -*- Mode: C -*-

#/*  config-edit Copyright (C) 2008-2013 by Bruce Korb - all rights reserved
# *
# *  config-edit is free software: you can redistribute it and/or modify it
# *  under the terms of the GNU General Public License as published by the
# *  Free Software Foundation, either version 3 of the License, or
# *  (at your option) any later version.
# *
# *  config-edit is distributed in the hope that it will be useful, but
# *  WITHOUT ANY WARRANTY; without even the implied warranty of
# *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# *  See the GNU General Public License for more details.
# *
# *  You should have received a copy of the GNU General Public License along
# *  with this program.  If not, see <http://www.gnu.org/licenses/>.
# */

=][= # /*

DESCRIPTION:
Config file editor
Create a program to edit a daemon config file by processing the options,
rewriting the config file and sending SIGHUP to the daemon.

PURPOSE:
   This template will produce a program that will process its configuration
   file and command line options, save the option state to its config file,
   and then signal its associated daemon file with a SIGHUP.  Included with
   this template are some auxialiary options and attributes in a file named
   "editor.def".

YOU SUPPLY:
   an AutoGen definitions file.  It must contain the following:

   The definitions line:
      autogen definitions options;

   The name of the config file editor program.  e.g.:
      prog-name = edit-daemon-conf;

   Include directives for "editor.def" and your daemon's options.  e.g.:
      #include editor.def
      #include daemon-opts.def

THE TEMPLATE PRODUCES:

   After processing this definitions file with:
      autogen edit-your-daemon-conf.def

   you will have a header and .c file for a  program named
   edit-<your-daemon-name>-conf.  Compile this thus:

      cflags=$( autoopts-config cflags)
      ldflags=$(autoopts-config ldflags)
      cc -o edit-<your-daemon-name>-conf $cflags \
         edit-<your-daemon-name>-conf.c $ldflags

   the result is a program with this usage text similar to this:

   edit-<your-daemon-name>-conf - example dynamic config daemon - Ver. 1.2
   USAGE:  edit-daemon-conf [ -<flag> [<val>] | --<name>[{=| }<val>] ]...
     Flg Arg Option-Name    Description
         Num pid            pid of daemon process
                                   - an alternate for pid-file
         Fil pid-file       file containing daemon pid
                                   - file must pre-exist
                                   - may not be preset
         no  no-sighup      do not send SIGHUP to daemon
                                   - an alternate for pid-file
   [[options from the daemon]]
      -R Str reset-option   Reset an option's state
      -? no  help           Display usage information and exit
      -! no  more-help      Extended usage information passed thru pager
      -> opt save-opts      Save the option state to a config file
      -< Str load-opts      Load options from a config file
                                   - disabled as --no-load-opts
                                   - may appear multiple times

   Options are specified by doubled hyphens and their name
   or by a single hyphen and the flag character.

   The following option preset mechanisms are supported:
    - reading file [[config file name]]

   Running this program will re-compute the option state, save it away
   in the configuration file and, optionally, signal the daemon with
   SIGHUP to reload its configuration.  (Using any of four methods.
   If none of the three option methods is specified, then the "pkill"
   program will be used.)

 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

=][=

   (if (not (exist? "homerc"))
       (error "editor.tpl requires 'homerc' attribute"))
   (define PNAME (string-upcase (string->c-name! (get "prog-name"))))
\=]
#include <sys/types.h>
#include <sys/wait.h>

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "edit-[=(. pname)=]-conf.h"

int
main(int argc, char** argv)
{
    int res = [=(. PNAME)=]_EXIT_SUCCESS;
    {
        int ct = optionProcess(&[=(. pname)=]Options, argc, argv);
        argc -= ct;
        argv += ct;
    }

    switch (argc) {
    case 1:
        SET_OPT_SAVE_OPTS(*argv);
        /* FALLTHROUGH */

    case 0:
        optionSaveFile(&[=(. pname)=]Options);
        break;

    default:
        fprintf(stderr, "%s error: too many operands\n",
                [=(. pname)=]Options.pzProgName);
        USAGE([=(. PNAME)=]_EXIT_OPERANDS);
        /* NOTREACHED */
    }

    /* still here?  The options are fine and we've stored the new version */
    switch (WHICH_IDX_PID_FILE) {
    case INDEX_OPT_PID:
        if (kill(OPT_VALUE_PID, SIGHUP) != 0) {
        bad_kill:
            res = [=(. PNAME)=]_EXIT_CANNOT_HUP;
            fprintf(stderr, "fs error %d (%s) sending SIGHUP to pid %d\n",
                    errno, strerror(errno), OPT_VALUE_PID);
        }
        break;

    case INDEX_OPT_PID_FILE:
    {
        char buf[256];
        pid_t kill_pid;
        fread(buf, 1, sizeof(buf), OPT_VALUE_PID_FILE);
        kill_pid = (pid_t)strtoul(buf, NULL, 0);
        if (kill(kill_pid, SIGHUP) != 0)
            goto bad_kill;
        fclose(OPT_VALUE_PID_FILE);
        break;
    }

    case INDEX_OPT_NO_SIGHUP:
        return ([=(. PNAME)=]_EXIT_SUCCESS);

    case NO_EQUIVALENT:
    {
        /*
         *  No pid selected, so use "pkill" program.
         */
        static char const cmd_fmt[] =
            "pkill -hup -u %d [= prog-name[1] =]";
        char z[sizeof(cmd_fmt) + 10];
        uid_t myid = geteuid();
        sprintf(z, cmd_fmt, myid);
        res = system(z);
        if (res < 0)
            goto bad_kill;
        if (res != 0) {
            res = WEXITSTATUS(res);
            fprintf(stderr, "pkill exited with status %d\n\t%s\n", res, z);
        }
    }
    }

    return res;
}
