#ifndef SHELL_H
#define SHELL_H

#include "parser.h" 

// custom cmds
typedef struct
{
    const char *name;
    const char *desc;
    int (*func)(int argc, char *argv[]);
} Builtin;

extern Builtin builtin_cmds[];
extern int num_builtins;

// custom cmds
int cmd_exit(int argc, char *argv[]);
int cmd_greet(int argc, char *argv[]);
int cmd_listCmd(int argc, char *argv[]);
int cmd_cd(int argc, char *argv[]);
int cmd_mail(int argc, char *argv[]);
int cmd_task(int argc, char *argv[]);

// Dispatcher for custom cmds
int execute_builtin(Command *cmd);

#endif
