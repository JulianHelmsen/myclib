#ifndef MYLIBC_MINHEAP_H
#define MYLIBC_MINHEAP_H

#include <da.h>
#include <assert.h>


#define heap_parent(i) ((i) / 2)
#define heap_left(i) ((i) * 2)
#define heap_right(i) ((i) * 2 + 1)


#define heap_push(h, val, cmp, ctx) do{ \
        da_append((h), (val)); \
        da_reserve(h, (h)->size + 1);\
        (h)->items[(h)->size] = (h)->items[(h)->size - 1];\
        const size_t TEMP_LOC = (h)->size; \
        size_t heap_pos = (h)->size; \
        while(heap_pos != 1) {\
            const size_t heap_parent_pos = heap_parent(heap_pos);\
            const int cmp_res = cmp((h)->items[TEMP_LOC], (h)->items[heap_parent_pos - 1], ctx);\
            if(cmp_res >= 0)\
                break;\
            (h)->items[heap_pos - 1] = (h)->items[heap_parent_pos - 1];\
            (h)->items[heap_parent_pos - 1] = (h)->items[TEMP_LOC];\
            heap_pos = heap_parent(heap_pos);\
        }\
    }while(0)

#define heap_pop(h, dst, cmp, ctx) do{\
        assert((h)->size > 0);\
        assert(dst != NULL);\
        *(dst) = (h)->items[0];\
        const size_t TEMP_LOC = (h)->size - 1;\
        (h)->items[0] = (h)->items[--(h)->size];\
        size_t heap_pos = 1;\
        while(heap_right(heap_pos) < (h)->size) {\
            const int cmp_res = cmp((h)->items[heap_left(heap_pos) - 1], (h)->items[heap_right(heap_pos) - 1], ctx);\
            size_t smaller_pos;\
            if(cmp_res <= 0) { \
                smaller_pos = heap_left(heap_pos);\
            }else{\
                smaller_pos = heap_right(heap_pos);\
            }\
            const int fcmp = cmp((h)->items[heap_pos - 1], (h)->items[smaller_pos - 1], ctx);\
            if(fcmp <= 0) { \
                break;\
            }\
            (h)->items[TEMP_LOC] = (h)->items[heap_pos - 1];\
            (h)->items[heap_pos - 1] = (h)->items[smaller_pos - 1];\
            (h)->items[smaller_pos - 1] = (h)->items[TEMP_LOC];\
            heap_pos = smaller_pos;\
        }\
        if(heap_left(heap_pos) < (h)->size) {\
            const int fcmp = cmp((h)->items[heap_pos - 1], (h)->items[heap_left(heap_pos) - 1], ctx);\
            if(fcmp <= 0) { \
                break;\
            }\
            (h)->items[TEMP_LOC] = (h)->items[heap_pos - 1];\
            (h)->items[heap_pos - 1] = (h)->items[heap_left(heap_pos) - 1];\
            (h)->items[heap_left(heap_pos) - 1] = (h)->items[TEMP_LOC];\
        }\
    }while(0)

#endif 
