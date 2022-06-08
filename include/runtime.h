#ifndef RUNTIME_H
#define RUNTIME_H

#include "channel.h"
#include "task.h"

int tasking_init(int *argc, char **argv[]);

int tasking_exit_signal(void);

int tasking_exit(void);

Task *task_alloc(void);

void push(Task *task);

int RT_barrier(void);

void RT_force_future(Channel *, void *, unsigned int);

#endif // RUNTIME_H
