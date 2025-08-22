#ifndef MYLIBC_ARENA_H
#define MYLIBC_ARENA_H

#include <stddef.h>

typedef struct arena_area {
    size_t size;
    size_t offset;
    struct arena_area* next;
} arena_area ;

typedef struct {
    size_t default_area_size;
    arena_area* head;
    arena_area* curr;
    size_t area_count;
}arena;

typedef struct arena_marker {
    arena_area* curr;
    size_t offset;
}arena_marker;


void* arena_alloc(arena* a, size_t nbytes);
arena_marker arena_mark(arena* a);
void arena_reset_to(arena* a, arena_marker marker);
void arena_reset(arena* a);
void arena_destroy(arena* a);

#endif 
