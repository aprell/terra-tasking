#ifndef TASKING_H
#define TASKING_H

#include <stdbool.h>
#include "macro_utils.h"



// ASYNC tasks without return values

#define ASYNC_DECL(fname, decls, args...) \
struct fname##_task_data { \
    decls; \
}; \
void fname##_task_func(struct fname##_task_data *__d) \
{ \
    UNPACK(__d, args); \
    fname(args); \
}

#define ASYNC(f, args...) \
do { \
    Task *__task; \
    struct f##_task_data *__d; \
    __task = task_alloc(); \
    __task->parent = current_task; \
    __task->fn = (void (*)(void *))f##_task_func; \
    \
    __d = (struct f##_task_data *)__task->data; \
    PACK(__d, args); \
    push(__task); \
} while (0)



// FUTURE tasks with return values

typedef void *future;

#ifndef FUTURE_FREELIST_SIZE
  #define FUTURE_FREELIST_SIZE 1000
#endif

#define FUTURE_DECL_FREELIST(type) \
static __thread unsigned int __future_##type##_freelist_items = 0; \
static __thread future __future_##type##_freelist[FUTURE_FREELIST_SIZE]; \

#define FUTURE_DECL(ret, fname, decls, args...) \
struct fname##_task_data { \
    decls; \
    future __f; \
}; \
static inline future fname##_future_create(void) \
{ \
    if (__future_##ret##_freelist_items > 0) { \
        return __future_##ret##_freelist[--__future_##ret##_freelist_items]; \
    } \
    return channel_alloc(sizeof(ret), 0, SPSC); \
} \
void fname##_task_func(struct fname##_task_data *__d) \
{ \
    UNPACK(__d, args, __f); \
    ret __tmp = fname(args); \
    channel_send(__f, &__tmp, sizeof(__tmp)); \
}

#define FUTURE(f, args...) \
({  \
    Task *__task; \
    struct f##_task_data *__d; \
    future __f; \
    __task = task_alloc(); \
    __task->parent = current_task; \
    __task->fn = (void (*)(void *))f##_task_func; \
    \
    __d = (struct f##_task_data *)__task->data; \
    __f = f##_future_create(); \
    PACK(__d, args, __f); \
    push(__task); \
    __f; \
})

// Returns the result of evaluating future `f'
// `type' is used to declare a temporary variable
#define AWAIT(f, type) \
({ \
    type __tmp; \
    RT_force_future(f, &__tmp, sizeof(__tmp)); \
    if (__future_##type##_freelist_items < FUTURE_FREELIST_SIZE) { \
        __future_##type##_freelist[__future_##type##_freelist_items++] = (f); \
    } else { \
        channel_free(f); \
    } \
    __tmp; \
})



typedef struct task {
    struct task *parent;
    void *t0[2];
    void (*fn)(void *);
    long t1[7];
    char data[104];
} Task;

extern __thread Task *current_task;

Task *task_alloc(void);
void push(Task *task);



typedef struct Channel Channel;

// Different channel implementations, in order of decreasing generality
enum {
    MPMC, // multiple producers, multiple consumers
    MPSC, // multiple producers, single consumer
    SPSC  // single producer, single consumer
};

#define __channel_alloc_impl2(n, ...) __channel_alloc_impl__ ## n(__VA_ARGS__)
#define __channel_alloc_impl(n, ...) __channel_alloc_impl2(n, __VA_ARGS__)
#define channel_alloc(...) __channel_alloc_impl(VA_NARGS(__VA_ARGS__), __VA_ARGS__)

Channel *channel_alloc(unsigned int size, unsigned int n, int impl);
Channel *channel_alloc(unsigned int size, unsigned int n); // MPMC
Channel *channel_alloc(int impl);

void channel_free(Channel *chan);

// Send size bytes of data over chan
bool channel_send(Channel *chan, void *data, unsigned int size);

// Receive size bytes of data from chan
bool channel_receive(Channel *chan, void *data, unsigned int size);



int tasking_init(int *argc, char **argv[]);
int tasking_exit_signal(void);
int tasking_exit(void);

int RT_barrier(void);
void RT_force_future(Channel *, void *, unsigned int);

#endif // TASKING_H
