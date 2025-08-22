#include "test.h"
#include <arena.h>
#include <string.h>


void test_reset(void) {
    arena arena = { 0 };
    arena_marker marker = arena_mark(&arena);
    test(marker.curr == NULL);
    test(marker.offset == 0);

    void* a = arena_alloc(&arena, 100);
    test(a != NULL);
    test(arena.default_area_size == 4096);
    void* b = arena_alloc(&arena, 100);
    test(b != NULL);

    arena_marker m = arena_mark(&arena);
    void* c = arena_alloc(&arena, 4096);
    test(c != NULL);

    arena_reset_to(&arena, m);
    void* c2 = arena_alloc(&arena, 4096);
    test(c == c2);

    arena_reset(&arena);


    void* na = arena_alloc(&arena, 100);
    test(na == a);
    test(arena.default_area_size == 4096);
    void* nb = arena_alloc(&arena, 100);
    test(nb == b);

    arena_marker m2 = arena_mark(&arena);
    void* nc = arena_alloc(&arena, 4096);
    test(nc == c);

    arena_reset_to(&arena, m2);
    void* nc2 = arena_alloc(&arena, 4096);
    test(nc == nc2);
    test(nc == c2);
    arena_destroy(&arena);
}

void test_alloc_count(void) {
    arena a = { 0 };

    void* buf = arena_alloc(&a, 123);
    (void) buf;
    arena_marker marker = arena_mark(&a);
    size_t sizes[] = {4096 * 2, 123, 123, 123, 345, 567, 1, 2, 3, 8789357, 213};
    void* ptrs[sizeof(sizes) / sizeof(sizes[0])];

    for(size_t j = 0; j < sizeof(sizes) / sizeof(sizes[0]); ++j) {
        ptrs[j] = arena_alloc(&a, sizes[j]);
        memset(ptrs[j], 0xFF, sizes[j]);
    }

    arena_reset_to(&a, marker);


    const size_t area_count = a.area_count;
    for(size_t i = 0; i < 10; ++i) {
        test(area_count == a.area_count);

        for(size_t j = 0; j < sizeof(sizes) / sizeof(sizes[0]); ++j) {
            void* p = arena_alloc(&a, sizes[j]);
            memset(ptrs[j], j & 0xFF, sizes[j]);
            test(p == ptrs[j]);
        }

        arena_reset_to(&a, marker);
    }


    test(area_count == a.area_count);
    arena_destroy(&a);


    arena empty = { 0 };
    test(memcmp(&a, &empty, sizeof(a)) == 0 && "Arena is not correctly destroyed");
}


void test_huge_alloc_after_reset(void) {
    arena a = { 0 };

    arena_alloc(&a, 4080);
    arena_marker marker1 = arena_mark(&a);
    arena_alloc(&a, 4080);
    arena_marker marker2 = arena_mark(&a);
    test(marker1.curr != marker2.curr);
    test(a.area_count == 2);

    void* first_a = a.head;
    void* second_a = a.head->next;

    arena_reset_to(&a, marker1);
    arena_alloc(&a, 10000);
    test(a.area_count == 3);
    void* third_a = a.head->next;


    test(first_a == a.head);
    test(third_a == a.head->next);
    test(second_a == a.head->next->next);


    arena_destroy(&a);
}

int main(const int argc, const char** argv) {
    prepare_test(argc, argv);

    test_reset();
    test_alloc_count();
    test_huge_alloc_after_reset();


    return 0;
}
