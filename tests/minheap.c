#include "test.h"
#include <minheap.h>

struct minheap {
    size_t* items;
    size_t capacity;
    size_t size;
};


int cmp_e(size_t a, size_t b, void* unused) {
    (void) unused;
    return (int) a - (int) b;
}


void test_manually(void) {
    struct minheap heap = { 0 };
    heap_push(&heap, 10, cmp_e, NULL);

    test(heap.items[0] == 10);
    test(heap.size == 1);

    heap_push(&heap, 5, cmp_e, NULL);
    test(heap.items[0] == 5);
    test(heap.size == 2);

    heap_push(&heap, 15, cmp_e, NULL);
    test(heap.items[0] == 5);
    test(heap.size == 3);

    test(heap.items[0] == 5);
    test(heap.items[1] == 10);
    test(heap.items[2] == 15);

    heap_push(&heap, 13, cmp_e, NULL);
    heap_push(&heap, 9, cmp_e, NULL);
    test(heap.size == 5);
    test(heap.items[0] == 5);
    test(heap.items[1] == 9);
    test(heap.items[2] == 15);
    test(heap.items[3] == 13);
    test(heap.items[4] == 10);


    heap_push(&heap, 2, cmp_e, NULL);
    test(heap.items[0] == 2);
    test(heap.items[1] == 9);
    test(heap.items[2] == 5);
    test(heap.items[3] == 13);
    test(heap.items[4] == 10);
    test(heap.items[5] == 15);
}



void test_sorting(void) {
    struct minheap heap = { 0 };
    for(size_t i = 0; i < 1000; ++i) {
        heap_push(&heap, rand() & 0xFF, cmp_e, NULL);
    }

    size_t prev;
    heap_pop(&heap, &prev, cmp_e, NULL);
    while(heap.size > 0) {
        size_t now;
        heap_pop(&heap, &now, cmp_e, NULL);
        test(prev <= now);
        prev = now;
    }
}

int main(const int argc, const char** argv) {
    prepare_test(argc, argv);

    test_manually();
    test_sorting();


    return 0;
}
