
/*  testing daemon
 *
 *  config-edit Copyright (C) 2008-2013 by Bruce Korb - all rights reserved
 *
 *  config-edit is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  config-edit is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <unistd.h>
#include <signal.h>
#include <stdio.h>

#include "daemon-opts.h"

int reset = 0;

static void
handle_sighup(int sig)
{
    static char const * fake_av[2] = { "daemon", NULL };
    optionRestore(&daemonOptions);

    /*
     * We use a fake arg vector because we've already saved the initial
     * options into our config file.  Someone has changed that config file,
     * so we do not want to reprocess command line options.
     */
    (void)optionProcess(&daemonOptions, 1, (char **)fake_av);
    reset = 1;
}

int
main(int argc, char ** argv)
{
    optionSaveState(&daemonOptions);
    (void)optionProcess(&daemonOptions, argc, argv);

    /*
     *  Save the resulting state.  The command line options are augment
     *  the current config and are saved for future reference.
     */
    if (argc > 1)
        optionSaveFile(&daemonOptions);

    (void)signal(SIGHUP, handle_sighup);

    {
        FILE * fp = fopen("daemon.pid", "w");
        if (fp != NULL) {
            fprintf(fp, "%d\n", getpid());
            fclose(fp);
        }
    }

    for (;;) {
        printf("text: %-70s\r", OPT_ARG(TEXT));
        fflush(stdout);
        sleep(10);
        if (reset) {
            fputc('\n', stdout);
            reset = 0;
        }
    }
}
