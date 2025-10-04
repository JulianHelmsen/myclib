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


void queue_tmemcpy(void* restrict  dst, size_t d_off, const void* restrict src, size_t ncount, size_t elem_size) {
    char* cdst = dst;
    cdst += d_off * elem_size;
    memcpy(cdst, src, ncount * elem_size);
}







