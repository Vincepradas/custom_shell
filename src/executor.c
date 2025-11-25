#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include "executor.h"

void execute_command(Command *cmd) {
    if (cmd->command == NULL) return;

    // command: exit
    if (strcmp(cmd->command, "exit") == 0) {
        printf("Bye!\n");
        _exit(0);
    }

    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        if (execvp(cmd->command, cmd->args) == -1)
            perror("exec failed");
        _exit(1);
    } else if (pid > 0) {
        wait(NULL); // Parent waits
    } else {
        perror("fork failed");
    }
}
