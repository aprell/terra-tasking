#ifndef FUTURE_H
#define FUTURE_H

#include <assert.h>
#include "macro_utils.h"
#include "runtime.h"

// FUTURE tasks with return values

typedef void *future;

#define new_future(f) new_##f##_future()

#ifndef FUTURE_FREELIST_SIZE
  #define FUTURE_FREELIST_SIZE 1000
#endif

#define FUTURE_DECL_FREELIST(type) \
static PRIVATE unsigned int __future_##type##_freelist_items = 0; \
static PRIVATE future __future_##type##_freelist[FUTURE_FREELIST_SIZE]; \

#define FUTURE_DECL(ret, fname, decls, args...) \
struct fname##_task_data { \
    decls; \
    future __f; \
}; \
static inline future new_##fname##_future(void) \
{ \
    if (__future_##ret##_freelist_items > 0) { \
        return __future_##ret##_freelist[--__future_##ret##_freelist_items]; \
    } \
    return channel_alloc(sizeof(ret), 0, SPSC); \
} \
void fname##_task_func(struct fname##_task_data *__d) \
{ \
    Task *this = get_current_task(); \
    assert(!is_root_task(this)); \
    assert((struct fname##_task_data *)this->data == __d); \
    \
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
    __task->parent = get_current_task(); \
    __task->fn = (void (*)(void *))f##_task_func; \
    \
    __d = (struct f##_task_data *)__task->data; \
    __f = new_future(f); \
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

#endif // FUTURE_H
