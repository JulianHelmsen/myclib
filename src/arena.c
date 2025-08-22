#include <arena.h>
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

static inline size_t align_up(size_t nbytes) {
    size_t div_up = (nbytes+7) / 8;
    return div_up * 8;
}

static inline void* arena_area_data(arena_area* area) {
    return (void*) (area + 1);
}

arena_area* arena_area_create(size_t area_size) {
    arena_area* area = malloc(sizeof(arena_area) + area_size);
    if(area == NULL) return NULL;
    area->size = area_size;
    area->offset = 0;
    area->next = NULL;
    return area;
}

static void arena_init(arena* a) {
    assert(a->curr == NULL);
    assert(a->head == NULL);
    if(a->default_area_size == 0)
        a->default_area_size = 4096;

}

void* arena_alloc(arena* a, size_t nbytes) {
    nbytes = align_up(nbytes);

    if(a->default_area_size == 0) arena_init(a);

    arena_area* current = a->curr;
    if(current == NULL || current->offset + nbytes > current->size) {
        // does not fit in current area
        if(current == NULL || current->next == NULL) {
            // allocate new area
            size_t area_size = nbytes < a->default_area_size ? a->default_area_size : nbytes;
            arena_area* area = arena_area_create(area_size);
            if(area == NULL) return NULL;

            if(a->curr == NULL) {
                assert(a->head == NULL);
                a->head = area;
                a->curr = area;
            }else{
                a->curr->next = area;
                a->curr = area;
            }
            a->area_count += 1;
        }else{
            // select next area
            a->curr = current->next;
            a->curr->offset = 0;
        }
        current = a->curr;
    }

    if(current->offset + nbytes > current->size) {
        return NULL;
    }

    void* p = (char*) arena_area_data(current) + current->offset;
    current->offset += nbytes;
    return p;
}

arena_marker arena_mark(arena* a) {
    if(a->curr == NULL)
        return (arena_marker) {.curr = NULL, .offset = 0};
    return (arena_marker) { .curr = a->curr, .offset = a->curr->offset };
}

void arena_reset(arena* a) {
    arena_reset_to(a, (arena_marker) {.curr = NULL, .offset = 0});
}

void arena_reset_to(arena* a, arena_marker marker) {
    if(marker.curr == NULL) {
        marker.curr = a->head;
        assert(marker.offset == 0);
    }
    a->curr = marker.curr;
    a->curr->offset = marker.offset;
}

void arena_destroy(arena* a) {
    arena_area* area = a->head;
    a->head = NULL;
    a->curr = NULL;
    a->default_area_size = 0;
    a->area_count = 0;

    while(area != NULL) {
        arena_area* next = area->next;
        free(area);
        area = next;
    }
}


