#ifndef ASYNC_H
#define ASYNC_H

#include <assert.h>
#include "channel.h"
#include "task.h"

/* Count variadic macro arguments (1-10 arguments, extend as needed)
 */
#define VA_NARGS_IMPL(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) N
#define VA_NARGS(...) VA_NARGS_IMPL(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)

/* Pack args into data structure x
 */
#define PACK(x, args...) \
do { \
    *(x) = (typeof(*(x))){ args }; \
} while (0)

/* Unpack data structure x (up to ten members, extend as needed)
 */
#define UNPACK1(x, _1) \
    typeof((x)->_1) _1 = (x)->_1

#define UNPACK2(x, _1, _2) \
    typeof((x)->_1) _1 = (x)->_1; \
    typeof((x)->_2) _2 = (x)->_2

#define UNPACK3(x, _1, _2, _3) \
    typeof((x)->_1) _1 = (x)->_1; \
    typeof((x)->_2) _2 = (x)->_2; \
    typeof((x)->_3) _3 = (x)->_3

#define UNPACK4(x, _1, _2, _3, _4) \
    typeof((x)->_1) _1 = (x)->_1; \
    typeof((x)->_2) _2 = (x)->_2; \
    typeof((x)->_3) _3 = (x)->_3; \
    typeof((x)->_4) _4 = (x)->_4

#define UNPACK5(x, _1, _2, _3, _4, _5) \
    typeof((x)->_1) _1 = (x)->_1; \
    typeof((x)->_2) _2 = (x)->_2; \
    typeof((x)->_3) _3 = (x)->_3; \
    typeof((x)->_4) _4 = (x)->_4; \
    typeof((x)->_5) _5 = (x)->_5

#define UNPACK6(x, _1, _2, _3, _4, _5, _6) \
    typeof((x)->_1) _1 = (x)->_1; \
    typeof((x)->_2) _2 = (x)->_2; \
    typeof((x)->_3) _3 = (x)->_3; \
    typeof((x)->_4) _4 = (x)->_4; \
    typeof((x)->_5) _5 = (x)->_5; \
    typeof((x)->_6) _6 = (x)->_6

#define UNPACK7(x, _1, _2, _3, _4, _5, _6, _7) \
    typeof((x)->_1) _1 = (x)->_1; \
    typeof((x)->_2) _2 = (x)->_2; \
    typeof((x)->_3) _3 = (x)->_3; \
    typeof((x)->_4) _4 = (x)->_4; \
    typeof((x)->_5) _5 = (x)->_5; \
    typeof((x)->_6) _6 = (x)->_6; \
    typeof((x)->_7) _7 = (x)->_7

#define UNPACK8(x, _1, _2, _3, _4, _5, _6, _7, _8) \
    typeof((x)->_1) _1 = (x)->_1; \
    typeof((x)->_2) _2 = (x)->_2; \
    typeof((x)->_3) _3 = (x)->_3; \
    typeof((x)->_4) _4 = (x)->_4; \
    typeof((x)->_5) _5 = (x)->_5; \
    typeof((x)->_6) _6 = (x)->_6; \
    typeof((x)->_7) _7 = (x)->_7; \
    typeof((x)->_8) _8 = (x)->_8

#define UNPACK9(x, _1, _2, _3, _4, _5, _6, _7, _8, _9) \
    typeof((x)->_1) _1 = (x)->_1; \
    typeof((x)->_2) _2 = (x)->_2; \
    typeof((x)->_3) _3 = (x)->_3; \
    typeof((x)->_4) _4 = (x)->_4; \
    typeof((x)->_5) _5 = (x)->_5; \
    typeof((x)->_6) _6 = (x)->_6; \
    typeof((x)->_7) _7 = (x)->_7; \
    typeof((x)->_8) _8 = (x)->_8; \
    typeof((x)->_9) _9 = (x)->_9

#define UNPACK10(x, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10) \
    typeof((x)->_1)   _1 = (x)->_1; \
    typeof((x)->_2)   _2 = (x)->_2; \
    typeof((x)->_3)   _3 = (x)->_3; \
    typeof((x)->_4)   _4 = (x)->_4; \
    typeof((x)->_5)   _5 = (x)->_5; \
    typeof((x)->_6)   _6 = (x)->_6; \
    typeof((x)->_7)   _7 = (x)->_7; \
    typeof((x)->_8)   _8 = (x)->_8; \
    typeof((x)->_9)   _9 = (x)->_9; \
    typeof((x)->_10) _10 = (x)->_10

#define UNPACK_IMPL2(x, n, ...) UNPACK ## n(x, __VA_ARGS__)
#define UNPACK_IMPL(x, n, ...) UNPACK_IMPL2(x, n, __VA_ARGS__)
#define UNPACK(x, ...) UNPACK_IMPL(x, VA_NARGS(__VA_ARGS__), __VA_ARGS__)

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

// FUTURE tasks with return values

typedef Channel *future;

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

// Returns the result of evaluating future f
// type is used to declare a temporary variable
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

Task *task_alloc(void);
void push(Task *task);
void RT_force_future(Channel *, void *, unsigned int);

#endif // ASYNC_H
