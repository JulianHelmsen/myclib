#include "test.h"
#include <queue.h>

bool queue_full(int_queue q);


static void queue_push_back_test(void) {
    int_queue q = { 0 };

    test(queue_size(&q) == 0);

    queue_push_back(&q, 1);
    test(queue_size(&q) == 1);
    queue_push_back(&q, 2);
    test(queue_size(&q) == 2);
    queue_push_back(&q, 3);
    test(queue_size(&q) == 3);
    queue_push_back(&q, 4);
    test(queue_size(&q) == 4);

    test(queue_pop_back(&q) == 4);
    test(queue_size(&q) == 3);
    test(queue_pop_back(&q) == 3);
    test(queue_size(&q) == 2);
    test(queue_pop_back(&q) == 2);
    test(queue_size(&q) == 1);
    test(queue_pop_back(&q) == 1);
    test(queue_size(&q) == 0);

    queue_free(&q);
}

static void queue_push_front_test(void) {
    int_queue q = { 0 };

    test(queue_size(&q) == 0);

    queue_push_front(&q, 1);
    test(queue_size(&q) == 1);
    queue_push_front(&q, 2);
    test(queue_size(&q) == 2);
    queue_push_front(&q, 3);
    test(queue_size(&q) == 3);
    queue_push_front(&q, 4);
    test(queue_size(&q) == 4);

    test(queue_front(&q) == 4);
    test(queue_pop_front(&q) == 4);
    test(queue_size(&q) == 3);
    test(queue_front(&q) == 3);
    test(queue_pop_front(&q) == 3);
    test(queue_size(&q) == 2);
    test(queue_front(&q) == 2);
    test(queue_pop_front(&q) == 2);
    test(queue_size(&q) == 1);
    test(queue_front(&q) == 1);
    test(queue_pop_front(&q) == 1);
    test(queue_size(&q) == 0);

    queue_free(&q);
}


static void queue_push_both_test(void) {
    int_queue q = { 0 };

    queue_push_front(&q, -1);
    queue_push_back(&q, 1);
    test(queue_size(&q) == 2);

    test(q.items[q.capacity - 1] == -1);
    test(q.items[0] == 1);

    queue_push_front(&q, -2);
    queue_push_back(&q, 2);

    test(q.items[q.capacity - 2] == -2);
    test(q.items[1] == 2);


    test(queue_pop_back(&q) == 2);
    test(queue_pop_back(&q) == 1);
    test(queue_pop_back(&q) == -1);
    test(queue_pop_back(&q) == -2);
    test(queue_size(&q) == 0);

    queue_free(&q);
}

static void queue_push_both_resize_test(void) {
    int_queue q = { 0 };
    const int N = 1000;

    for(int i = 0; i < N; ++i) {
        queue_push_front(&q, -i);
        queue_push_back(&q, i);
        test(queue_size(&q) == (size_t) 2 * (i + 1));
    }

    for(int i = N - 1; i >= 0; --i) {
        test(queue_pop_back(&q) == i);
    }

    for(int i = 0; i < N; ++i) {
        test(queue_pop_back(&q) == -i);
    }


    queue_free(&q);
}

void queue_push_front_overflow_test(void) {
    int_queue q = { 0 };
    queue_push_back(&q, 2);
    test(queue_pop_front(&q) == 2);

    test(q.front == 1);
    test(q.back == 1);

    queue_push_front(&q, 34);
    test(q.front == 0);
    queue_push_front(&q, 35);
    test(q.front == MIN_QUEUE_CAPACITY - 1);
    test(q.back == 1);

    test(queue_pop_front(&q) == 35);
    test(q.front == 0);
    test(queue_pop_front(&q) == 34);
    test(q.front == 1);
    test(queue_size(&q) == 0);

    queue_free(&q);
}

void queue_push_back_overflow_test(void) {
    int_queue q = { 0 };

    // _ _ _ _ _ _ _ _ _ _
    // a (all)
    queue_push_front(&q, 1);
    queue_push_front(&q, 2);
    queue_push_front(&q, 3);
    // _ _ _ _ _ _ _ 3 2 1
    // b             f

    test(q.front == MIN_QUEUE_CAPACITY - 3);

    test(queue_back(&q) == 1);
    test(queue_pop_back(&q) == 1);
    test(queue_back(&q) == 2);
    test(queue_pop_back(&q) == 2);
    test(queue_back(&q) == 3);
    test(queue_pop_back(&q) == 3);

    // _ _ _ _ _ _ _ 3 2 1
    //               a

    test(q.front == MIN_QUEUE_CAPACITY - 3);
    test(q.back == MIN_QUEUE_CAPACITY - 3);
    test(queue_size(&q) == 0);

    queue_push_back(&q, 6);
    queue_push_back(&q, 9);
    queue_push_back(&q, 8);
    test(q.back == 0);
    queue_push_back(&q, 7);

    // 7 _ _ _ _ _ _ 6 9 8
    //   b           f

    test(q.back == 1);

    test(queue_pop_back(&q) == 7);
    test(queue_pop_back(&q) == 8);
    test(queue_pop_back(&q) == 9);
    test(queue_pop_back(&q) == 6);

    queue_free(&q);
}

static void queue_push_back_resize_test(void) {
    int_queue q = { 0 };
    const int N = 1000;
    for(int i = 0; i < N; ++i)
        queue_push_back(&q, i);
    test(queue_size(&q) == 1000);

    for(int i = N  - 1; i >= 0; --i)
        test(queue_pop_back(&q) == i);

    queue_free(&q);
}

static void queue_push_front_resize_test(void) {
    int_queue q = { 0 };
    const int N = 1000;
    for(int i = 0; i < N; ++i)
        queue_push_front(&q, i);
    test(queue_size(&q) == 1000);

    for(int i = N  - 1; i >= 0; --i) {
        test(queue_front(&q) == i);
        test(queue_pop_front(&q) == i);
    }

    queue_free(&q);
}

int main(const int argc, const char** argv) {
    prepare_test(argc, argv);

    queue_push_back_test();
    queue_push_front_test();
    queue_push_both_test();
    queue_push_both_resize_test();
    queue_push_front_overflow_test();
    queue_push_back_overflow_test();
    queue_push_back_resize_test();
    queue_push_front_resize_test();


    return 0;
}
