#ifndef TASK_H
#define TASK_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define PRIVATE __thread

typedef struct task {
    struct task *parent;
    struct {
        struct task *prev;
        struct task *next;
    };
    void (*fn)(void *);
    int batch;
    int victim;
    bool is_loop;
    long start, cur, end;
    long chunks, sst;
    // Task body carrying user data
    char data[104];
} Task;

// Pointer to the task that is currently running
extern PRIVATE Task *current_task;

static inline Task *task_zero(Task *task)
{
    task->parent = NULL;
    task->prev = NULL;
    task->next = NULL;
    task->fn = NULL;

    task->batch = 0;
    task->victim = 0;

    task->is_loop = false;
    task->start = 0;
    task->cur = 0;
    task->end = 0;
    task->chunks = 0;
    task->sst = 0;

    return task;
}

static inline Task *task_new(void)
{
    Task *task = (Task *)malloc(sizeof(Task));
    if (!task) {
        fprintf(stderr, "Warning: task_new failed\n");
        return NULL;
    }

    return task_zero(task);
}

static inline void task_delete(Task *task)
{
    free(task);
}

static inline char *task_data(Task *task)
{
    return task->data;
}

static inline void set_current_task(Task *task)
{
    current_task = task;
}

static inline Task *get_current_task(void)
{
    return current_task;
}

static inline bool is_root_task(Task *task)
{
    return task->parent == NULL ? true : false;
}

#endif // TASK_H
