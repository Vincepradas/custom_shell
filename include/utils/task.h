#ifndef TASK_H
#define TASK_H

#include <stddef.h>

#define MAX_DESC 256
#define MAX_NAME 64
#define MAX_TASKS 100

typedef enum { DONE = 0, WIP = 1, TODO = 2, CANCELLED = 3 } Status;


typedef struct
{
    int id;
    char name[MAX_NAME];
    char desc[MAX_DESC];
    Status status;
    long timestamp;
} Task;

extern Task tasks[MAX_TASKS];
extern int NEXT_ID;
extern int task_count; 

/* Core functions */
Task create_task(const char *name, const char *desc);
void mark_task(Task *task, Status status);
Task get_task_by_name(const char *queryName);
void list_all_tasks();
void init_next_id();
int task_name_exists(const char *name);

/* Utility */
const char *status_to_string(Status status);
void print_task(const Task *task);
void trim(char *s);
void load_tasks();

#endif
