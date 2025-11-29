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
    if (!fptr) {
        NEXT_ID = 1;
        return;
    }

    char line[512];
    Task t = {0};
    int reading = 0;
    task_count = 0;
    int max_id = 0;

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
            // Track max ID for NEXT_ID calculation
            if (t.id > max_id)
                max_id = t.id;
        }
    }
    fclose(fptr);
    
    // Set NEXT_ID based on loaded tasks
    NEXT_ID = max_id + 1;
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
    // NEXT_ID is now set by load_tasks(), so this function simply
    // uses the already-loaded tasks array to find max ID if needed.
    // This avoids redundant file I/O.
    if (task_count == 0) {
        NEXT_ID = 1;
        return;
    }
    
    int max_id = 0;
    for (int i = 0; i < task_count; i++) {
        if (tasks[i].id > max_id)
            max_id = tasks[i].id;
    }
    NEXT_ID = max_id + 1;
}

int task_name_exists(const char *name)
{
    // Use the already-loaded tasks array instead of re-reading the file.
    // This avoids redundant file I/O.
    for (int i = 0; i < task_count; i++) {
        if (strcmp(tasks[i].name, name) == 0) {
            return 1;
        }
    }
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
    // Use the already-loaded tasks array instead of re-reading the file.
    // This avoids redundant file I/O.
    char tmpName[MAX_NAME];
    strncpy(tmpName, queryName, MAX_NAME - 1);
    tmpName[MAX_NAME - 1] = '\0';
    trim(tmpName);

    for (int i = 0; i < task_count; i++) {
        if (strcmp(tasks[i].name, tmpName) == 0) {
            return tasks[i];
        }
    }

    return (Task){0};
}

void list_all_tasks()
{
    // Use the already-loaded tasks array instead of re-reading the file.
    // This avoids redundant file I/O.
    if (task_count == 0)
    {
        printf(YELLOW "No tasks saved yet.\n" RESET);
        return;
    }

    printf(CYAN "Available tasks:\n" RESET);

    for (int i = 0; i < task_count; i++)
    {
        switch (tasks[i].status)
        {
        case DONE:
            printf(" - " GREEN "%s" RESET "\n", tasks[i].name);
            break;
        case WIP:
            printf(" - " YELLOW "%s" RESET "\n", tasks[i].name);
            break;
        case TODO:
            printf(" - " RED "%s" RESET "\n", tasks[i].name);
            break;
        case CANCELLED:
            printf(" - " MAGENTA "%s" RESET "\n", tasks[i].name);
            break;
        default:
            printf(" - %s\n", tasks[i].name);
            break;
        }
    }
}

int delete_task(const char *name)
{
    if (!name || task_count == 0) {
        printf(RED "No task found to delete.\n" RESET);
        return 0;
    }

    int index = -1;
    for (int i = 0; i < task_count; i++) {
        if (strcmp(tasks[i].name, name) == 0) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf(RED "Task '%s' not found.\n" RESET, name);
        return 0;
    }

    // Confirm deletion
    char confirm[8];
    printf(YELLOW "Are you sure you want to delete task '%s'? (yes/no): " RESET, name);
    if (!fgets(confirm, sizeof(confirm), stdin)) return 0;
    trim(confirm);
    if (strcmp(confirm, "yes") != 0) {
        printf(CYAN "Deletion cancelled.\n" RESET);
        return 0;
    }

    // Shift tasks to overwrite deleted task
    for (int i = index; i < task_count - 1; i++) {
        tasks[i] = tasks[i + 1];
    }
    task_count--;

    // Rewrite the JSON file
    FILE *fptr = fopen("data/task_data.json", "w");
    if (!fptr) {
        printf(RED "Failed to update task_data.json.\n" RESET);
        return 0;
    }

    fprintf(fptr, "[\n");
    for (int i = 0; i < task_count; i++) {
        fprintf(fptr,
                "  {\n"
                "    \"id\": %d,\n"
                "    \"name\": \"%s\",\n"
                "    \"description\": \"%s\",\n"
                "    \"status\": \"%s\",\n"
                "    \"timestamp\": %ld\n"
                "  }%s\n",
                tasks[i].id, tasks[i].name, tasks[i].desc,
                status_to_string(tasks[i].status),
                tasks[i].timestamp,
                i < task_count - 1 ? "," : "");
    }
    fprintf(fptr, "]\n");
    fclose(fptr);

    printf(GREEN "Task '%s' deleted successfully!\n" RESET, name);
    return 1;
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

    // Get file size to allocate appropriate buffer
    fseek(fptr, 0, SEEK_END);
    long file_size = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);

    if (file_size <= 0) {
        fclose(fptr);
        return;
    }

    // Allocate buffer for entire file content
    char *file_content = (char *)malloc(file_size + 1);
    if (!file_content) {
        fclose(fptr);
        return;
    }

    size_t bytes_read = fread(file_content, 1, file_size, fptr);
    fclose(fptr);
    file_content[bytes_read] = '\0';

    // Create output buffer (same size is sufficient)
    char *output = (char *)malloc(file_size + 256); // Extra space for potential longer status
    if (!output) {
        free(file_content);
        return;
    }
    output[0] = '\0';
    size_t output_len = 0;

    int reading = 0;
    char current_name[MAX_NAME] = {0};
    char *line_start = file_content;
    char *line_end;

    while ((line_end = strchr(line_start, '\n')) != NULL || *line_start != '\0') {
        size_t line_len;
        if (line_end) {
            line_len = line_end - line_start;
        } else {
            line_len = strlen(line_start);
            line_end = line_start + line_len - 1; // Point to last char
        }

        // Create temporary null-terminated line for parsing
        char line[512];
        size_t copy_len = line_len < sizeof(line) - 1 ? line_len : sizeof(line) - 1;
        memcpy(line, line_start, copy_len);
        line[copy_len] = '\0';

        if (strstr(line, "{")) {
            reading = 1;
            current_name[0] = '\0';
        }
        if (reading && strstr(line, "\"name\"")) {
            sscanf(line, " \"name\" : \"%[^\"]\"", current_name);
        }

        if (reading && strstr(line, "\"status\"") && strcmp(current_name, t->name) == 0) {
            // Write modified status line
            int written = snprintf(output + output_len, file_size + 256 - output_len, 
                                   "    \"status\": \"%s\",\n", status_to_string(s));
            output_len += written;
        } else {
            // Copy original line
            memcpy(output + output_len, line_start, line_len);
            output_len += line_len;
            if (line_end && *line_end == '\n') {
                output[output_len++] = '\n';
            }
        }

        if (strstr(line, "}"))
            reading = 0;

        if (!line_end || *(line_end + 1) == '\0')
            break;
        line_start = line_end + 1;
    }
    output[output_len] = '\0';

    free(file_content);

    fptr = fopen("data/task_data.json", "w");
    if (!fptr) {
        free(output);
        return;
    }

    fputs(output, fptr);
    fclose(fptr);
    free(output);
}
