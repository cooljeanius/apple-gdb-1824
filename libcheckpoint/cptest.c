/*
 *  cptest.c
 *  gdb
 *
 *  Created by Eric Gallager on 12/4/14.
 *  Test file for functions in libcheckpoint.
 */

#include <errno.h> /* for errno */
#include <signal.h> /* for kill() */
#include <stdio.h> /* for fprintf() */

/* prototypes: */
extern int _gdbcp_cg_save(pid_t);
extern int _gdbcp_cg_rollback(pid_t);
extern int _gdbcp_cg_delete(pid_t);
extern pid_t _gdbcp_fork(void);

int main(void)
{
    int errCode = 0; /* used for return value checking */
    pid_t cpid = 0; /* dummy value */

    cpid = _gdbcp_fork();
    errCode = (int)cpid;

    if (errCode != 0) {
        fprintf(stderr, "After forking, errCode became '%d'.\n", errCode);
    }

    errCode = _gdbcp_cg_save(cpid);

    if (errCode != 0) {
        fprintf(stderr, "After saving checkpoint, errCode became '%d'.\n",
                errCode);
        return errCode;
    }

    errCode = _gdbcp_cg_rollback(cpid);

    if (errCode != 0) {
        fprintf(stderr,
                "After rolling back checkpoint, errCode became '%d'.\n",
                errCode);
        return errCode;
    }

    errCode = _gdbcp_cg_delete(cpid);

    if (errCode != 0) {
        fprintf(stderr,
                "After deleting checkpoint, errCode became '%d'.\n",
                errCode);
        return errCode;
    }

    if (cpid != 0) {
        printf("Trying to kill process with pid '%d'...\n", cpid);
        errCode = kill(cpid, SIGKILL);
        if (errCode != 0) {
            fprintf(stderr,
                    "kill() failed; checking errno to see why...\n");
            if (errno == EINVAL) {
                fprintf(stderr, "WTF, SIGKILL should be valid!\n");
            } else if (errno == EPERM) {
                fprintf(stderr,
                        "WTF, somehow the child escalated its privileges...\n");
            } else if (errno == ESRCH) {
                fprintf(stderr,
                        "Uh oh, something must have happened to the child...\n");
            }
        } else {
            printf("Killed.\n");
        }
    }

    return 0;
}

/* EOF */

