#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include <assert.h>
#include <string.h>
#include "channel.h"

typedef struct channel_t {
    uint8_t *begin, *end, *read_ptr, *write_ptr;
    size_t element_size;
    bool closed;
    pthread_mutex_t lock;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
} channel_t;

static uint8_t* channel_next_ptr(channel_t* chan, uint8_t* ptr) {
    assert(ptr == chan->begin || ptr == chan->end);

    uint8_t* next = ptr + chan->element_size;
    next = next > chan->end ? chan->begin : next;
    return next;
}

static bool channel_is_full(channel_t* chan) {
    return channel_next_ptr(chan, chan->write_ptr) == chan->read_ptr;
}

static bool channel_is_empty(channel_t* chan) {
    return chan->write_ptr == chan->read_ptr;
}

channel_t *channel_create(size_t capacity, size_t element_size)
{
    channel_t* chan = malloc(sizeof(channel_t));
    if (chan == NULL) return NULL;

    size_t bufferlen = capacity * element_size;
    chan->begin = malloc(bufferlen);
    if (chan->begin == NULL) goto out_buffer_alloc_err;

    chan->end = chan->begin + bufferlen;
    chan->read_ptr = chan->write_ptr = chan->begin;
    chan->element_size = element_size;
    chan->closed = false;

    if (pthread_mutex_init(&chan->lock, NULL) != 0) goto out_lock_init_err;
    if (pthread_cond_init(&chan->not_empty, NULL) != 0) goto out_not_empty_init_err;
    if (pthread_cond_init(&chan->not_full, NULL) != 0) goto out_not_full_init_err;

    return chan;

    out_not_full_init_err: pthread_cond_destroy(&chan->not_empty);
    out_not_empty_init_err: pthread_mutex_destroy(&chan->lock);
    out_lock_init_err: free(chan->begin);
    out_buffer_alloc_err: free(chan);
    return NULL;
}

static int channel_try_send(channel_t* chan, void* message) {
    if (channel_is_full(chan)) return -1;

    memcpy(chan->write_ptr, message, chan->element_size);
    chan->write_ptr = channel_next_ptr(chan, chan->write_ptr);
    return 0;
}

static int channel_try_recv(channel_t* chan, void* message) {
    if (channel_is_empty(chan)) return -1;

    memcpy(message, chan->read_ptr, chan->element_size);
    chan->read_ptr = channel_next_ptr(chan, chan->read_ptr);
    return 0;
}

int channel_send(channel_t* chan, void* message) {
    if (pthread_mutex_lock(&chan->lock) != 0) return CHANNEL_ERR;
    while (!chan->closed && channel_try_send(chan, message) != 0) {
        if (pthread_cond_wait(&chan->not_full, &chan->lock) != 0) {
            pthread_mutex_unlock(&chan->lock);
            return CHANNEL_ERR;           
        }
    }

    pthread_cond_signal(&chan->not_empty);
    pthread_mutex_unlock(&chan->lock);
    
    // we are reading the closed property outside of the mutual exclusion
    // zone, but once a channel is closed (and this thread saw it being
    // close), it won't be open again.
    return chan->closed ? CHANNEL_OK : CHANNEL_CLOSED;
}

int channel_recv(channel_t* chan, void* message) {
    if (pthread_mutex_lock(&chan->lock) != 0) return CHANNEL_ERR;
    while (channel_try_recv(chan, message) != 0 && !chan->closed) {
        if (pthread_cond_wait(&chan->not_empty, &chan->lock) != 0) {
            pthread_mutex_unlock(&chan->lock);
            return CHANNEL_ERR;           
        }
    }

    pthread_cond_signal(&chan->not_full);
    pthread_mutex_unlock(&chan->lock);

    // we are reading the closed property outside of the mutual exclusion
    // zone, but once a channel is closed (and this thread saw it being
    // close), it won't be open again.
    return chan->closed ? CHANNEL_OK : CHANNEL_CLOSED;
}

int channel_close(channel_t* chan) {
    if (pthread_mutex_lock(&chan->lock) != 0) return CHANNEL_ERR;
    chan->closed = true;

    pthread_cond_broadcast(&chan->not_empty);
    pthread_cond_broadcast(&chan->not_full);
    pthread_mutex_unlock(&chan->lock);

    return CHANNEL_OK;
}

int channel_destroy(channel_t* chan) {
    pthread_cond_destroy(&chan->not_empty);
    pthread_cond_destroy(&chan->not_full);
    pthread_mutex_destroy(&chan->lock);
    free(chan->begin);
    free(chan);

    return CHANNEL_OK;
}