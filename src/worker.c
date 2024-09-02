#include <stdlib.h>
#include <pthread.h>
#include "channel.h"

typedef struct worker_job_t {
    void (*fn)(void*);
    void *state;
} worker_job_t;

typedef struct worker_t {
    channel_t* inbox;
    pthread_t thread;
} worker_t;

static void* worker_thread(void* state) {
    worker_t* wkr = (worker_t*) state;
    worker_job_t job;

    while (channel_recv(wkr->inbox, &job) == CHANNEL_OK) {
        job.fn(job.state);
    }

    return NULL;
}

worker_t* worker_create(size_t capacity) {
    worker_t* wkr = malloc(sizeof(worker_t));
    if (wkr == NULL) return NULL;

    wkr->inbox = channel_create(capacity, sizeof(worker_job_t));
    if (wkr->inbox == NULL) 
        goto out_channel_err;

    if (pthread_create(&wkr->thread, NULL, worker_thread, wkr) != 0)
        goto out_pthread_create_err;

    return wkr;

    out_pthread_create_err:
    channel_destroy(wkr->inbox);
    out_channel_err:
    free(wkr);
    return NULL;
}

int worker_send(worker_t* wkr, void (*fn)(void*), void* state) {
    worker_job_t job = {
        .fn = fn,
        .state = state,
    };

    return channel_send(wkr->inbox, &job);
}

int worker_close(worker_t* wkr) {
    if (channel_close(wkr->inbox)) return -1;
    pthread_join(wkr->thread, NULL);
    return 0;
}

void worker_destroy(worker_t* wkr) {
    channel_destroy(wkr->inbox);
    free(wkr);
}
