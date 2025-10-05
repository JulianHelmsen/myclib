#include "test.h"
#include <hashmap.h>
#include <sview.h>
#include <assert.h>

size_t hash_i(const void* s) {
    return *(int*) s;
}

bool eq_i(const void* s1, const void* s2) {
    return *(int*) s1 == *(int*) s2;
}

hashmap_impl(ii, int, int)


size_t hash_s(const void* s) {
    return sview_hash(*(const sview*) s);
}

bool eq_s(const void* s1, const void* s2) {
    return sview_eq(*(const sview*) s1, *(const sview*) s2);
}


hashmap_impl(si, sview, int)

void test_put_get(void) {
    hashmap map = hashmap_create(sview, int, hash_s, eq_s);

    test(hashmap_put_si(&map, sview_create_lit("abc"), 32));
    test(map.size == 1);
    test(hashmap_put_si(&map, sview_create_lit("abc"), 32));
    test(map.size == 1);

    test(hashmap_put_si(&map, sview_create_lit("xyz"), 987));
    test(map.size == 2);
    test(hashmap_put_si(&map, sview_create_lit("xyz"), 987));
    test(map.size == 2);


    test(hashmap_put_si(&map, sview_create_lit("wow"), 69));
    test(map.size == 3);

    int value;
    test(hashmap_get_si(&map, sview_create_lit("abc"), &value));
    test(value == 32);

    test(hashmap_put_si(&map, sview_create_lit("xyz"), 1));
    test(map.size == 3);

    test(hashmap_get_si(&map, sview_create_lit("xyz"), &value));
    test(value == 1);

    test(hashmap_get_si(&map, sview_create_lit("wow"), &value));
    test(value == 69);

    test(hashmap_remove_si(&map, sview_create_lit("wow")));
    test(map.size == 2);

    value = -1;
    test(!hashmap_get_si(&map, sview_create_lit("wow"), &value));
    test(value == -1);

    test(!hashmap_get_si(&map, sview_create_lit("does not exist"), NULL));
    test(!hashmap_contains_si(&map, sview_create_lit("does not exist")));


    hashmap_destroy(&map);
}

static int rand_int(void) {
    int val = (int) rand() << 16 | rand();
    return val & 0x7FFFFFFF;
}

void test_lots_of_data(void) {
    static int DATA[500000];
    const size_t N = sizeof(DATA) / sizeof(DATA[0]);
    hashmap map = hashmap_create(int, int, hash_i, eq_i);

    for(size_t i = 0; i < N; ++i) {
        const int idx = rand_int() % N;
        assert(idx >= 0);
        DATA[idx] = rand_int() + 1;
        hashmap_put_ii(&map, idx, DATA[idx]);
    }

    for(size_t i = 0; i < N; ++i) {
        if(DATA[i] > 0) {
            test(hashmap_contains_ii(&map, i));
            int v;
            test(hashmap_get_ii(&map, i, &v));
            test(v == DATA[i]);

            test(hashmap_remove_ii(&map, i));
            test(!hashmap_contains_ii(&map, i));
        }
    }


    hashmap_destroy(&map);
}


int main(const int argc, const char** argv) {
    prepare_test(argc, argv);

    test_put_get();

    test_lots_of_data();
    return 0;
}
