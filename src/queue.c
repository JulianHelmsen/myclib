#include <queue.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/*
 * Notes:
 * queue->front always points to the first element
 * queue->back always points to next free element
 */

size_t queue_size(int_queue* q) {
    if(q->front == q->back) return 0;
    if(q->front < q->back)
        return q->back - q->front;
    return q->capacity - q->front + q->back;
}

bool queue_full(int_queue* q) {
    return queue_size(q) + 1 >= q->capacity;
}

void queue_tmemcpy(void* restrict  dst, size_t d_off, const void* restrict src, size_t ncount, size_t elem_size) {
    char* cdst = dst;
    cdst += d_off * elem_size;
    memcpy(cdst, src, ncount * elem_size);
}

static void queue_grow(int_queue* q) {
    const size_t new_cap = q->capacity == 0 ? MIN_QUEUE_CAPACITY : q->capacity * QUEUE_SCALING;
    char* new_mem = malloc(new_cap * sizeof(*q->items));
    if(new_mem == NULL) {
        perror("new_mem");
        exit(1);
    }

    if(q->back < q->front) {
        // copy q->items[0..q->back] to new_mem[q->capacity..q->capacity + q->back]
        queue_tmemcpy(new_mem, q->capacity, q->items, q->back, sizeof(*q->items));

        // copy q->items[q->front..q->capacity] to new_mem[q->front..q->capacity]
        queue_tmemcpy(new_mem, q->front, &q->items[q->front], q->capacity - q->front, sizeof(*q->items));

        // adjust the back index
        q->back = q->capacity + q->back;
    }else{
        queue_tmemcpy(new_mem, q->front, &q->items[q->front], q->back - q->front, sizeof(*q->items));
    }

    free(q->items);

    q->capacity = new_cap;
    q->items = (void*) new_mem;
}

void queue_push_front(int_queue* q, int val) {
    if(queue_full(q)) queue_grow(q);
    assert(!queue_full(q));

    if(--q->front >= q->capacity)
        q->front = q->capacity - 1;

    q->items[q->front] = val;

    assert(q->back != q->front);
}

void queue_push_back(int_queue* q, int val) {
    if(queue_full(q)) queue_grow(q);
    assert(!queue_full(q));

    q->items[q->back++] = val;
    if(q->back == q->capacity)
        q->back = 0;

    assert(q->back != q->front);
}

int queue_pop_front(int_queue* q) {
    assert(q->front != q->back);

    const int val = q->items[q->front];
    if(++q->front == q->capacity)
        q->front = 0;

    return val;
}

int queue_pop_back(int_queue* q) {
    if(--q->back >= q->capacity)
        q->back = q->capacity - 1;
    return q->items[q->back];
}

void queue_free(int_queue* q) {
    q->front = 0;
    q->back = 0;
    free(q->items);
    q->capacity = 0;
    q->items = NULL;
}


int queue_front(int_queue* q) {
    assert(q->back != q->front);
    return q->items[q->front];
}

int queue_back(int_queue* q) {
    assert(q->back != q->front);
    if(q->back == 0)
        return q->items[q->capacity - 1];
    return q->items[q->back - 1];
}
