#ifndef LIMOS_CHANNEL_H_INCLUDED
#define LIMOS_CHANNEL_H_INCLUDED

#include <stdlib.h>

typedef struct channel_t channel_t;

enum {
    CHANNEL_OK,
    CHANNEL_ERR,
    CHANNEL_CLOSED
};

channel_t* channel_create(size_t capacity, size_t element_size);
int channel_send(channel_t* chan, void* message);
int channel_recv(channel_t* chan, void* message);
int channel_close(channel_t* chan);
int channel_destroy(channel_t* chan);

#endif