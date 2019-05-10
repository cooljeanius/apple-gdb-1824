/* cli/cli-logging.h */

#ifndef CLI_LOGGING_H
#define CLI_LOGGING_H 1

extern void log_command(const char *);

extern void show_logging_command(const char *, int);

extern int logging_commands;
extern int logging_overwrite;
extern int logging_redirect;

#endif /* !CLI_LOGGING_H */

/* EOF */
