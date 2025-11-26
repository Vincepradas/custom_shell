#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"
#include "utils/task.h"
#include "utils/colors.h"
#define MAX_TASKS 100
static Task tasks[MAX_TASKS];
static int task_count = 0;

int cmd_task(int argc, char *argv[])
{
    int running = 1;

    while (running) {
        // Colored menu
        printf(BLUE "================ TASK MENU ================" RESET "\n");
        printf(YELLOW "[1]" RESET " Create task\n");
        printf(YELLOW "[2]" RESET " Get task\n");
        printf(YELLOW "[3]" RESET " Update status\n");
        printf(YELLOW "[4]" RESET " Exit task menu\n");
        printf(BLUE "===========================================" RESET "\n");

        printf("Input: ");
        int choice;
        if (scanf("%d", &choice) != 1) {
            int c; while ((c = getchar()) != '\n' && c != EOF);
            printf("\033[31mInvalid input" RESET "\n\n");
            continue;
        }

        int c; while ((c = getchar()) != '\n' && c != EOF); // flush newline

        switch (choice) {
            case 1: { // Create task
                char n[MAX_NAME];
                char d[MAX_DESC];

                do {
                    printf("Enter task name: ");
                    fgets(n, sizeof(n), stdin);
                    n[strcspn(n, "\n")] = 0;
                } while (n[0] == '\0');

                do {
                    printf("Enter task description: ");
                    fgets(d, sizeof(d), stdin);
                    d[strcspn(d, "\n")] = 0;
                } while (d[0] == '\0');

                if (task_count < MAX_TASKS) {
                    tasks[task_count++] = create_task(n, d);
                    printf("\033[32mTask created!" RESET "\n\n");
                } else {
                    printf("\033[31mTask list full!" RESET "\n\n");
                }
                break;
            }

            case 2: { // Get task
                if (task_count == 0) {
                    printf("\033[33mNo tasks available." RESET "\n\n");
                    break;
                }

                // List all tasks
                printf("\033[1;36mAvailable tasks:" RESET "\n");
                for (int i = 0; i < task_count; i++) {
                    printf(" - \033[32m%s" RESET "\n", tasks[i].name);
                }

                char n[MAX_NAME];
                printf("\nEnter task name: ");
                fgets(n, sizeof(n), stdin);
                n[strcspn(n, "\n")] = 0;

                Task *found = NULL;
                for (int i = 0; i < task_count; i++) {
                    if (strcmp(tasks[i].name, n) == 0) {
                        found = &tasks[i];
                        break;
                    }
                }

                print_task(found);
                printf("\n");
                break;
            }

            case 3: { // Update status
                char name[MAX_NAME];
                printf("Enter task name to update: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;

                Task *found = NULL;
                for (int i = 0; i < task_count; i++) {
                    if (strcmp(tasks[i].name, name) == 0) {
                        found = &tasks[i];
                        break;
                    }
                }

                if (!found) {
                    printf("\033[31mTask not found." RESET "\n\n");
                    break;
                }

                printf("Choose new status: [0] DONE, [1] WIP, [2] TODO: ");
                int st;
                if (scanf("%d", &st) != 1 || st < 0 || st > 2) {
                    while ((c = getchar()) != '\n' && c != EOF);
                    printf("\033[31mInvalid status" RESET "\n\n");
                    break;
                }

                mark_task(found, (Status)st);
                printf("\033[32mStatus updated!" RESET "\n\n");
                break;
            }

            case 4: // Exit
                running = 0;
                printf("\033[33mExiting task menu..." RESET "\n\n");
                break;

            default:
                printf("\033[31mInvalid choice" RESET "\n\n");
                break;
        }
    }

    return 0;
}
