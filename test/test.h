#ifndef TEST_H
#define TEST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define RED "\033[0;31m"
#define BLACK "\033[0;30m"
#define GREEN "\033[0;32m"
#define FAIL RED"[FAILED]: "
#define SUCC GREEN"[SUCCESS]: "

static int fail_count = 0;
static int succ_count = 0;
static const char* s_prog = NULL;


static void print_test_status(void) {
    const char* prefix = fail_count > 0 ? RED : GREEN;
    printf("%sCompleted test %s. Status: %d/%d\n", prefix, s_prog, succ_count, succ_count + fail_count);
    printf(BLACK);
}

static void test_impl(bool cond, const char* cond_str, int line, const char* file) {
    if(!cond) {
        printf(FAIL "%s:%d: %s\n", file, line, cond_str);
        fail_count += 1;
    }else{
        succ_count += 1;
    }
}



static void prepare_test(const int argc, const char** argv) {
    (void) argc;
    s_prog = argv[0];
    printf(BLACK"Running test: %s\n", s_prog);
    atexit(print_test_status);
    (void) test_impl;
}

#define test(cond) test_impl(cond, #cond, __LINE__, __FILE__)

#endif 
