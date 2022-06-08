#ifndef MACRO_UTILS_H
#define MACRO_UTILS_H

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

#endif // MACRO_UTILS_H
