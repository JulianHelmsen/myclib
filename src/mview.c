#include <mview.h>
#include <assert.h>
#include <string.h>



mview mview_create(void* ptr, size_t len) {
    return (mview) {
        .data = ptr,
        .len = len,
    };
}

mview mview_subview(mview src, size_t begin, size_t len) {
    if(len == MVIEW_NPOS) len = src.len - begin;
    assert(begin <= src.len);
    assert(len <= src.len);
    assert(begin + len <= src.len);

    return mview_create((char*) src.data + begin, len);
}

mview mview_chop_left(mview src, size_t begin) {
    assert(begin <= src.len);
    return mview_create((char*) src.data + begin, src.len - begin);
}

mview mview_store_advance_impl(mview view, void* pdst, size_t len) {
    assert(len <= view.len);
    memcpy(pdst, view.data, len);
    return mview_chop_left(view, len);
}

bool mview_eq(mview a, mview b) {
    if(a.len != b.len) return false;
    return memcmp(a.data, b.data, a.len) == 0;
}
