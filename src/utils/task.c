#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "utils/task.h"
#include "utils/colors.h"

Task tasks[MAX_TASKS];
int task_count = 0;
int NEXT_ID = 1;

void load_tasks()
{
    FILE *fptr = fopen("data/task_data.json", "r");
    if (!fptr) return;

    char line[512];
    Task t = {0};
    int reading = 0;
    task_count = 0;

    while (fgets(line, sizeof(line), fptr))
    {
        trim(line);
        if (strcmp(line, "[") == 0 || strcmp(line, "]") == 0 || strcmp(line, ",") == 0)
            continue;

        if (strstr(line, "{"))
        {
            reading = 1;
            memset(&t, 0, sizeof(Task));
            continue;
        }
        if (!reading) continue;

        if (strstr(line, "\"id\""))
            sscanf(line, " \"id\" : %d", &t.id);
        if (strstr(line, "\"name\""))
            sscanf(line, " \"name\" : \"%[^\"]\"", t.name);
        if (strstr(line, "\"description\""))
            sscanf(line, " \"description\" : \"%[^\"]\"", t.desc);
        if (strstr(line, "\"status\""))
        {
            char tmp[32];
            sscanf(line, " \"status\" : \"%[^\"]\"", tmp);
            if (strcmp(tmp, "WIP") == 0)
                t.status = WIP;
            else if (strcmp(tmp, "DONE") == 0)
                t.status = DONE;
            else if (strcmp(tmp, "TODO") == 0)
                t.status = TODO;
            else
                t.status = CANCELLED;
        }

        if (strstr(line, "\"timestamp\""))
            sscanf(line, " \"timestamp\" : %ld", &t.timestamp);

        if (strstr(line, "}"))
        {
            reading = 0;
            trim(t.name);
            if (task_count < MAX_TASKS)
                tasks[task_count++] = t;
        }
    }
    fclose(fptr);
}

void trim(char *s)
{
    if (!s)
        return;

    int len = strlen(s);
    while (len > 0 && (s[len - 1] == ' ' || s[len - 1] == '\n' || s[len - 1] == '\r' || s[len - 1] == '\t'))
        s[--len] = '\0';

    int start = 0;
    while (s[start] && (s[start] == ' ' || s[start] == '\n' || s[start] == '\r' || s[start] == '\t'))
        start++;

    if (start > 0)
        memmove(s, s + start, strlen(s + start) + 1);
}

void init_next_id()
{
    FILE *fptr = fopen("data/task_data.json", "r");
    if (!fptr)
    {
        NEXT_ID = 1;
        return;
    }

    char line[512];
    int max_id = 0;
    while (fgets(line, sizeof(line), fptr))
    {
        int id;
        if (sscanf(line, " \"id\" : %d", &id) == 1)
        {
            if (id > max_id)
                max_id = id;
        }
    }
    NEXT_ID = max_id + 1;
    fclose(fptr);
}

int task_name_exists(const char *name)
{
    FILE *fptr = fopen("data/task_data.json", "r");
    if (!fptr)
        return 0;

    char line[512];
    while (fgets(line, sizeof(line), fptr))
    {
        if (strstr(line, "\"name\""))
        {
            char existing[MAX_NAME] = {0};
            sscanf(line, " \"name\" : \"%[^\"]\"", existing);
            trim(existing);
            if (strcmp(existing, name) == 0)
            {
                fclose(fptr);
                return 1;
            }
        }
    }
    fclose(fptr);
    return 0;
}

Task create_task(const char *name, const char *desc)
{
    Task t = {0};

    if (!name || !desc || name[0] == '\0')
        return t;

    char name_copy[MAX_NAME];
    char desc_copy[MAX_DESC];
    strncpy(name_copy, name, MAX_NAME - 1);
    strncpy(desc_copy, desc, MAX_DESC - 1);
    trim(name_copy);
    trim(desc_copy);

    if (task_name_exists(name_copy))
    {
        printf(RED "Task with name '%s' already exists!\n" RESET, name_copy);
        return t;
    }

    t.id = NEXT_ID++;
    strncpy(t.name, name_copy, MAX_NAME - 1);
    strncpy(t.desc, desc_copy, MAX_DESC - 1);
    t.status = WIP;
    t.timestamp = time(NULL);

    FILE *fptr = fopen("data/task_data.json", "r+");
    if (!fptr)
    {
        fptr = fopen("data/task_data.json", "w");
        if (!fptr)
        {
            printf(RED "Failed to open task_data.json for writing.\n" RESET);
            return t;
        }
        fprintf(fptr, "[\n");
    }
    else
    {
        fseek(fptr, 0, SEEK_END);
        long pos = ftell(fptr);
        if (pos > 2)
        {
            fseek(fptr, -2, SEEK_END);
            fprintf(fptr, ",\n");
        }
        else
        {
            fseek(fptr, 0, SEEK_END);
        }
    }
    fprintf(fptr,
            "  {\n"
            "    \"id\": %d,\n"
            "    \"name\": \"%s\",\n"
            "    \"description\": \"%s\",\n"
            "    \"status\": \"%s\",\n"
            "    \"timestamp\": %ld\n"
            "  }\n"
            "]\n",
            t.id,
            t.name,
            t.desc,
            t.status == WIP ? "WIP" : t.status == DONE ? "DONE"
                                  : t.status == TODO   ? "TODO"
                                                       : "CANCELLED",
            t.timestamp);

    fclose(fptr);
    return t;
}

Task get_task_by_name(const char *queryName)
{
    Task t = {0};
    FILE *fptr = fopen("data/task_data.json", "r");
    if (!fptr)
        return t;

    char line[512];
    int reading = 0;

    char tmpName[MAX_NAME];
    strncpy(tmpName, queryName, MAX_NAME - 1);
    trim(tmpName);

    while (fgets(line, sizeof(line), fptr))
    {
        trim(line);
        if (strcmp(line, "[") == 0 || strcmp(line, "]") == 0 || strcmp(line, ",") == 0)
            continue;

        if (strstr(line, "{"))
        {
            reading = 1;
            memset(&t, 0, sizeof(Task));
            continue;
        }
        if (!reading)
            continue;

        if (strstr(line, "\"id\""))
            sscanf(line, " \"id\" : %d", &t.id);
        if (strstr(line, "\"name\""))
            sscanf(line, " \"name\" : \"%[^\"]\"", t.name);
        if (strstr(line, "\"description\""))
            sscanf(line, " \"description\" : \"%[^\"]\"", t.desc);
        if (strstr(line, "\"status\""))
        {
            char tmp[32];
            sscanf(line, " \"status\" : \"%[^\"]\"", tmp);
            if (strcmp(tmp, "WIP") == 0)
                t.status = WIP;
            else if (strcmp(tmp, "DONE") == 0)
                t.status = DONE;
            else if (strcmp(tmp, "TODO") == 0)
                t.status = TODO;
            else
                t.status = CANCELLED;
        }
        if (strstr(line, "\"timestamp\""))
            sscanf(line, " \"timestamp\" : %ld", &t.timestamp);

        if (strstr(line, "}"))
        {
            reading = 0;
            trim(t.name);
            if (strcmp(t.name, tmpName) == 0)
            {
                fclose(fptr);
                return t;
            }
        }
    }

    fclose(fptr);
    return (Task){0};
}

void list_all_tasks()
{
    FILE *fptr = fopen("data/task_data.json", "r");
    if (!fptr)
    {
        printf(RED "No task file found.\n" RESET);
        return;
    }

    char line[512];
    Task t = {0};
    int reading = 0;
    int found = 0;

    printf(CYAN "Available tasks:\n" RESET);

    while (fgets(line, sizeof(line), fptr))
    {
        trim(line);
        if (strcmp(line, "[") == 0 || strcmp(line, "]") == 0 || strcmp(line, ",") == 0)
            continue;

        if (strstr(line, "{"))
        {
            reading = 1;
            memset(&t, 0, sizeof(Task));
            continue;
        }
        if (!reading)
            continue;

        if (strstr(line, "\"name\""))
            sscanf(line, " \"name\" : \"%[^\"]\"", t.name);
        if (strstr(line, "\"status\""))
        {
            char tmp[32];
            sscanf(line, " \"status\" : \"%[^\"]\"", tmp);
            if (strcmp(tmp, "WIP") == 0)
                t.status = WIP;
            else if (strcmp(tmp, "DONE") == 0)
                t.status = DONE;
            else if (strcmp(tmp, "TODO") == 0)
                t.status = TODO;
            else
                t.status = CANCELLED;
        }

        if (strstr(line, "}"))
        {
            reading = 0;
            trim(t.name);
            found = 1;

            switch (t.status)
            {
            case DONE:
                printf(" - " GREEN "%s" RESET "\n", t.name);
                break;
            case WIP:
                printf(" - " YELLOW "%s" RESET "\n", t.name);
                break;
            case TODO:
                printf(" - " RED "%s" RESET "\n", t.name);
                break;
            case CANCELLED:
                printf(" - " MAGENTA "%s" RESET "\n", t.name); // or gray
                break;
            default:
                printf(" - %s\n", t.name);
                break;
            }
        }
    }

    if (!found)
        printf(YELLOW "No tasks saved yet.\n" RESET);

    fclose(fptr);
}

void print_task(const Task *task)
{
    if (!task)
    {
        printf(RED "Error: Task is NULL.\n" RESET);
        return;
    }
    if (task->id <= 0)
    {
        printf(RED "Task not found or invalid.\n" RESET);
        return;
    }

    printf("\n============================\n");
    printf("        TASK INFORMATION     \n");
    printf("============================\n\n");

    printf("TASK ID: %d\n", task->id);
    printf("TASK NAME: %s\n", (task->name[0] != '\0') ? task->name : "N/A");
    printf("TASK DESCRIPTION: %s\n", (task->desc[0] != '\0') ? task->desc : "N/A");

    const char *statusStr = status_to_string(task->status);
    printf("TASK STATUS: %s\n", statusStr);

    printf("TIME STAMP: %ld\n", task->timestamp);

    printf("============================\n\n" RESET);
}

const char *status_to_string(Status s)
{
    switch (s)
    {
    case DONE:
        return "DONE";
    case WIP:
        return "WIP";
    case TODO:
        return "TODO";
    case CANCELLED:
        return "CANCELLED";
    default:
        return "UNKNOWN";
    }
}

void mark_task(Task *t, Status s)
{
    if (!t)
        return;
    t->status = s;

    FILE *fptr = fopen("data/task_data.json", "r");
    if (!fptr)
        return;

    char lines[1024][512];
    int count = 0;
    while (fgets(lines[count], sizeof(lines[count]), fptr))
        count++;
    fclose(fptr);

    int reading = 0;
    char current_name[MAX_NAME] = {0};

    for (int i = 0; i < count; i++)
    {
        char *line = lines[i];
        if (strstr(line, "{"))
        {
            reading = 1;
            current_name[0] = '\0';
        }
        if (reading && strstr(line, "\"name\""))
        {
            sscanf(line, " \"name\" : \"%[^\"]\"", current_name);
        }
        if (reading && strstr(line, "\"status\"") && strcmp(current_name, t->name) == 0)
        {
            snprintf(lines[i], sizeof(lines[i]), "    \"status\" : \"%s\",\n", status_to_string(s));
        }

        if (strstr(line, "}"))
            reading = 0;
    }

    fptr = fopen("data/task_data.json", "w");
    if (!fptr)
        return;

    for (int i = 0; i < count; i++)
        fputs(lines[i], fptr);

    fclose(fptr);
}
