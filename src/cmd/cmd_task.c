#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"
#include "utils/task.h"
#include "utils/colors.h"

int cmd_task(int argc, char *argv[])
{
    load_tasks();
    int running = 1;

    while (running)
    {
        printf(BLUE "================ TASK MENU ================" RESET "\n");
        printf(YELLOW "[1]" RESET " Create task\n");
        printf(YELLOW "[2]" RESET " Get task\n");
        printf(YELLOW "[3]" RESET " Update status\n");
        printf(YELLOW "[4]" RESET " Exit task menu\n");
        printf(BLUE "===========================================" RESET "\n");

        printf("Input: ");
        int choice;
        if (scanf("%d", &choice) != 1)
        {
            int c;
            while ((c = getchar()) != '\n' && c != EOF)
                ;
            printf("\033[31mInvalid input" RESET "\n\n");
            continue;
        }

        int c;
        while ((c = getchar()) != '\n' && c != EOF)
            ;

        switch (choice)
        {
        case 1:
        {
            char n[MAX_NAME];
            char d[MAX_DESC];

            do
            {
                printf("Enter task name: ");
                fgets(n, sizeof(n), stdin);
                n[strcspn(n, "\n")] = 0;
            } while (n[0] == '\0');

            do
            {
                printf("Enter task description: ");
                fgets(d, sizeof(d), stdin);
                d[strcspn(d, "\n")] = 0;
            } while (d[0] == '\0');

            if (task_count < MAX_TASKS)
            {
                tasks[task_count++] = create_task(n, d);
                printf("\033[32mTask created!" RESET "\n\n");
            }
            else
            {
                printf("\033[31mTask list full!" RESET "\n\n");
            }
            break;
        }

        case 2:
        {
            printf("\n");
            list_all_tasks();
            printf("\n");

            char n[MAX_NAME];
            printf("Enter task name: ");
            fgets(n, sizeof(n), stdin);
            n[strcspn(n, "\n")] = 0;

            if (n[0] == '\0')
            {
                printf(RED "Task name cannot be empty.\n" RESET "\n");
                break;
            }

            Task t = get_task_by_name(n);
            if (t.id == 0)
                printf(RED "Task '%s' not found." RESET "\n\n", n);
            else
            {
                print_task(&t);
                printf("\n");
            }

            break;
        }

        case 3:
        { 
            char name[MAX_NAME];
            list_all_tasks();
            printf("Enter task name to update: ");
            fgets(name, sizeof(name), stdin);
            name[strcspn(name, "\n")] = 0;

            Task *found = NULL;
            for (int i = 0; i < task_count; i++)
            {
                if (strcmp(tasks[i].name, name) == 0)
                {
                    found = &tasks[i];
                    break;
                }
            }

            if (!found)
            {
                printf("\033[31mTask not found." RESET "\n\n");
                break;
            }

            printf("Choose new status: [0] DONE, [1] WIP, [2] TODO, [3] CANCELLED: ");
            int st;
            if (scanf("%d", &st) != 1 || st < 0 || st > 3)
            {
                int c;
                while ((c = getchar()) != '\n' && c != EOF)
                    ;
                printf("\033[31mInvalid status" RESET "\n\n");
                break;
            }

            mark_task(found, (Status)st);
            printf("\033[32mStatus updated!" RESET "\n\n");
            break;
        }

        case 4: 
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
