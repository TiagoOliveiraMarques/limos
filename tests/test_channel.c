#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <assert.h>
#include "channel.h"

#define N 100000

static volatile int sum;

void* consume(void* state) {
    channel_t* inbox = (channel_t*)state;
    int msg;

    while (true) {
        int status = channel_recv(inbox, &msg);
        if (status == CHANNEL_ERR || status == CHANNEL_CLOSED) break;

        fprintf(stderr, "[consumer] read %d from channel\n", msg);

        sum += msg;
    }

    fprintf(stderr, "[consumer] exiting\n");

    return NULL;
}

void* produce(void* state) {
    channel_t* chan = (channel_t*)state;
    
    for (int i = 1; i <= N; i++) {
        int status = channel_send(chan, &i);
        if (status == CHANNEL_CLOSED || status == CHANNEL_ERR) break;

        fprintf(stderr, "[producer] wrote %d to channel\n", i);
    }

    channel_close(chan);

    fprintf(stderr, "[producer] exiting\n");

    return NULL;
}

int main(void) {
    channel_t* channel = channel_create(50, sizeof(int));
    pthread_t consumer_thread;
    pthread_t producer_thread;
    sum = 0;

    if (pthread_create(&consumer_thread, NULL, consume, channel) != 0) exit(-1);
    if (pthread_create(&producer_thread, NULL, produce, channel) != 0) exit(-1);

    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    // Compute the single thread sum
    int expected = 0;
    for (int i = 0; i <= N; i++) {
        expected += i;
    }

    if (sum != expected) {
        fprintf(stderr, "Expected sum to be %d, got %d\n", expected, sum);
        return -1;
    }
    return 0;
}