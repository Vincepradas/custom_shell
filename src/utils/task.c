#include <stdio.h>
#include <string.h>
#include <time.h>
#include "utils/task.h"
#include "utils/colors.h"

int NEXT_ID = 0;

Task create_task(const char *name, const char *desc) {
    Task t = {0};
    if (!name || !desc || name[0] == '\0') return t;

    t.id = NEXT_ID++;
    strncpy(t.name, name, MAX_NAME-1);
    t.name[MAX_NAME-1] = '\0';
    strncpy(t.desc, desc, MAX_DESC-1);
    t.desc[MAX_DESC-1] = '\0';
    t.status = WIP;
    t.timestamp = time(NULL);
    return t;
}

void print_task(const Task *task) {
    if (!task) {
        printf("Task not found.\n");
        return;
    }
    printf("TASK ID: %d\n", task->id);
    printf("TASK NAME: %s\n", task->name);
    printf("TASK DESCRIPTION: %s\n", task->desc);
    printf("TASK STATUS: %s\n", status_to_string(task->status));
    printf("TIME STAMP: %ld\n", task->timestamp);
}

const char *status_to_string(Status status) {
    switch(status) {
        case TODO: return "TODO";
        case WIP: return "WIP";
        case DONE: return "DONE";
        default: return "UNKNOWN";
    }
}

void mark_task(Task *t, Status s)
{
    t->status = s;
}