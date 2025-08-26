#include <mview.h>
#include "test.h"
#include <stdint.h>

unsigned char BUFFER[] = {
    0xaa, 0xbb, 0xcc, 0xdd,
    1, 2, 3, 4, 5, 6, 7, 8,
    'x', 'y', 'z',
    1, 2, 3, 4, 1, 2, 3, 4,
};

int main(const int argc, const char** argv) {
    prepare_test(argc, argv);

    mview view = mview_create(BUFFER, sizeof(BUFFER));


    uint32_t first;
    view = mview_store_advance(view, &first);
    test(first == 0xddccbbaa); // TODO: endianess
    test(view.data == BUFFER + sizeof(uint32_t)); 
    test(view.len + sizeof(uint32_t) == sizeof(BUFFER));


    view = mview_chop_left(view, 8);
    test(view.len + sizeof(uint32_t) + 8 == sizeof(BUFFER));
    test(BUFFER + sizeof(uint32_t) + 8 == view.data);

    char read[3];
    view = mview_store_advance(view, &read[0]);
    view = mview_store_advance(view, &read[1]);
    view = mview_store_advance(view, &read[2]);
    test(read[0] == 'x');
    test(read[1] == 'y');
    test(read[2] == 'z');
    test(view.len == 8);

    mview view_left = mview_subview(view, 0, 4);
    mview view_right = mview_subview(view, 4, MVIEW_NPOS);

    test(view_left.len == 4);
    test(view_right.len == 4);

    test(mview_eq(view_left, view_right));

    return 0;
}
