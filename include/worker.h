#ifndef LIMOS_WORKER_H_INCLUDED
#define LIMOS_WORKER_H_INCLUDED

#include <stdlib.h>

typedef struct worker_t worker_t;

worker_t* worker_create(size_t capacity);
int worker_send(worker_t* wkr, void (*fn)(void*), void* state);
int worker_close(worker_t* wkr);
void worker_destroy(worker_t* wkr);

#endif