#ifndef SHELL_H
#define SHELL_H

#include "parser.h" 

// Built-in cmds
int cmd_exit(int argc, char *argv[]);
int cmd_hello(int argc, char *argv[]);

// Dispatcher for built-in cmds
int execute_builtin(Command *cmd);

#endif
