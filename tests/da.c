#include "test.h"
#include <da.h>

struct ints {
    size_t size;
    size_t capacity;
    size_t* items;
};

int main(const int argc, const char** argv) {
    prepare_test(argc, argv);

    struct ints list = { 0 };

    for(size_t i = 0; i < DA_DEFAULT_CAPACITY; ++i) {
        da_append(&list, i);
        test(list.items[i] == i);
        test(list.size == i + 1);
        test(list.capacity == DA_DEFAULT_CAPACITY);
    }
    da_append(&list, 10);
    test(list.items[DA_DEFAULT_CAPACITY] == 10);
    test(list.size == DA_DEFAULT_CAPACITY + 1);
    test(list.capacity == DA_DEFAULT_CAPACITY * 2);


    da_free(&list);
    test(list.items == NULL);
    test(list.capacity == 0);
    test(list.size == 0);
    return 0;
}

