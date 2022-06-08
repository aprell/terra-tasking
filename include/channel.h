#ifndef CHANNEL_H
#define CHANNEL_H

#include <stdbool.h>
#include "macro_utils.h"

typedef struct Channel Channel;

enum {
    channel_UNBUF = 0, channel_B = 0,   // Unbuffered (blocking) channel
    channel_BUF = 1, channel_NB = 1     // Buffered (nonblocking) channel
};

// Different channel implementations, in order of decreasing generality
enum {
    MPMC, // multiple producers, multiple consumers
    MPSC, // multiple producers, single consumer
    SPSC  // single producer, single consumer
};

/*****************************************************************************
 * Channel API
 *****************************************************************************/

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

// Is the channel buffered or unbuffered?
bool channel_buffered(Channel *chan);
#define channel_unbuffered(chan) (!channel_buffered(chan))

// Returns the number of buffered items
// A value of 1 for unbuffered channels means that the sender is blocked
unsigned int channel_peek(Channel *chan);

// Returns the channel's capacity
// Always 0 for unbuffered channels
unsigned int channel_capacity(Channel *chan);

// Returns channel implementation
int channel_impl(Channel *chan);

// Closes channel chan, meaning no further values will be received
bool channel_close(Channel *chan);

// (Re)opens channel chan, meaning more values will be received
bool channel_open(Channel *chan);

// Returns true if channel is closed, false otherwise
bool channel_closed(Channel *chan);

#endif // CHANNEL_H
