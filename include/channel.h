#ifndef CHANNEL_H
#define CHANNEL_H

#include <stdbool.h>
#include "macro_utils.h"

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

#endif // CHANNEL_H
