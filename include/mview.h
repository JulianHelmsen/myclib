#ifndef mview_H
#define mview_H

#include <stddef.h>
#include <stdbool.h>

#define MVIEW_NPOS ((size_t) -1)

typedef struct {
    size_t len;
    void* data;
} mview;


mview mview_create(void* ptr, size_t len);
mview mview_subview(mview src, size_t begin, size_t len);

#define mview_store_advance(view, pdst) mview_store_advance_impl(view, (pdst), sizeof(*(pdst)))

mview mview_store_advance_impl(mview view, void* pdst, size_t len);
mview mview_chop_left(mview src, size_t begin);

bool mview_eq(mview a, mview b);
#endif 
