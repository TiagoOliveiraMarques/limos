#include <stdlib.h>
#include <stdio.h>
#include "worker.h"

static int accumulated = 0;

typedef struct message_t {
    int value;
} message_t;

void handle_msg(void* state) {
    message_t* msg = (message_t*) state;
    accumulated += msg->value;
    free(msg);
}

int main(void) {
    worker_t* wkr = worker_create(10);

    int expected = 0;
    for (int i = 0; i < 100000; i++) {
        expected += i;
        message_t* msg = malloc(sizeof(message_t));
        if (msg == NULL) return -1;
        msg->value = i;
        if (worker_send(wkr, handle_msg, msg) != 0) return -1;
    }

    worker_close(wkr);
    worker_destroy(wkr);

    if (expected != accumulated) {
        fprintf(stderr, "Expected sum to be %d, got %d\n", expected, accumulated);
        return -1;
    }

    return 0;
}