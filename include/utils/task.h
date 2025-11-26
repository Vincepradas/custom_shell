#ifndef TASK_H
#define TASK_H

#include <stddef.h>

#define MAX_DESC 256
#define MAX_NAME 64 

typedef enum { DONE, WIP, TODO } Status;

typedef struct {
    int id;
    char name[MAX_NAME];
    char desc[MAX_DESC];
    Status status;  
    long timestamp; 
} Task;

extern int NEXT_ID;

/* Core functions */
Task create_task(const char *name, const char *desc);
void update_task(Task *task, const char *name, const char *desc);
void mark_task(Task *task, Status status);

/* Utility */
const char *status_to_string(Status status);
void print_task(const Task *task);

#endif
