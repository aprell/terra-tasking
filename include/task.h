#ifndef TASK_H
#define TASK_H

typedef struct task {
    struct task *parent;
    void *t0[2];
    void (*fn)(void *);
    long t1[7];
    char data[104];
} Task;

extern __thread Task *current_task;

#endif // TASK_H
