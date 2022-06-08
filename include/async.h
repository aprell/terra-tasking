#ifndef ASYNC_H
#define ASYNC_H

#include <assert.h>
#include "macro_utils.h"
#include "runtime.h"

// ASYNC tasks without return values

#define ASYNC_DECL(fname, decls, args...) \
struct fname##_task_data { \
    decls; \
}; \
void fname##_task_func(struct fname##_task_data *__d) \
{ \
    Task *this = get_current_task(); \
    assert(!is_root_task(this)); \
    assert((struct fname##_task_data *)this->data == __d); \
    \
    UNPACK(__d, args); \
    fname(args); \
}

#define ASYNC(f, args...) \
do { \
    Task *__task; \
    struct f##_task_data *__d; \
    __task = task_alloc(); \
    __task->parent = get_current_task(); \
    __task->fn = (void (*)(void *))f##_task_func; \
    \
    __d = (struct f##_task_data *)__task->data; \
    PACK(__d, args); \
    push(__task); \
} while (0)

#endif // ASYNC_H
