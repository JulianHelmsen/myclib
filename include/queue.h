#ifndef MYLIBC_QUEUE_H
#define MYLIBC_QUEUE_H

#include <stddef.h>

#ifndef MIN_QUEUE_CAPACITY
#define MIN_QUEUE_CAPACITY (10)
#endif 

#ifndef QUEUE_SCALING
#define QUEUE_SCALING (2)
#endif

#define QUEUE_MEMBERS(type) \
    size_t capacity; \
    size_t front; \
    size_t back; \
    type* items

typedef struct {
    QUEUE_MEMBERS(int);
} int_queue;


size_t queue_size(int_queue* q);
void queue_push_front(int_queue* q, int val);
void queue_push_back(int_queue* q, int val);
int queue_pop_front(int_queue* q);
int queue_pop_back(int_queue* q);
void queue_free(int_queue* q);

int queue_front(int_queue* q);
int queue_back(int_queue* q);



// internal
void queue_tmemcpy(void* restrict  dst, size_t d_off, const void* restrict src, size_t ncount, size_t elem_size);
#endif // MYLIBC_QUEUE_H
