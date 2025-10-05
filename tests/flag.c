#include "test.h"
#include <flag.h>
#include <macros.h>
#include <string.h>


static void test_empty(void) {
    flag_reset();
    int* n = flag_opt_int("n", 32, "an integer");
    bool* b = flag_opt_bool("b", false, "a bool");
    sview* s = flag_opt_string("s", "a string", "a string description");


    const char* argv[] = {
        "prog",
    };
    bool succ = flag_parse(ARRAY_LEN(argv), argv);
    test(succ);
    test(*n == 32);
    test(*b == false);
    test(sview_eq(*s, sview_create_lit("a string")));
}

static void test_options(void) {
    flag_reset();
    int* n = flag_opt_int("n", 32, "an integer");
    bool* b = flag_opt_bool("b", false, "a bool");
    sview* s = flag_opt_string("s", "a string", "a string description");


    const char* argv[] = {
        "prog",
        "--n=55",
        "--b=true",
    };
    bool succ = flag_parse(ARRAY_LEN(argv), argv);
    test(*n == 55);
    test(*b == true);
    test(sview_eq(*s, sview_create_lit("a string")));
    test(succ);
}

static void test_unknown_option(void) {
    flag_reset();
    int* n = flag_opt_int("n", 32, "an integer");
    bool* b = flag_opt_bool("b", false, "a bool");
    sview* s = flag_opt_string("s", "a string", "a string description");


    const char* argv[] = {
        "prog",
        "--n=55",
        "--b=true",
        "--unknown=true",
    };
    bool succ = flag_parse(ARRAY_LEN(argv), argv);
    test(!succ);
    test(*n == 55);
    test(*b == true);
    test(sview_eq(*s, sview_create_lit("a string")));
}

static void test_unknown_flag(void) {
    flag_reset();
    int* n = flag_opt_int("n", 32, "an integer");
    bool* b = flag_opt_bool("b", false, "a bool");
    sview* s = flag_opt_string("s", "a string", "a string description");


    const char* argv[] = {
        "prog",
        "--n=55",
        "--b=true",
        "-x",
    };
    bool succ = flag_parse(ARRAY_LEN(argv), argv);
    test(!succ);
    test(*n == 55);
    test(*b == true);
    test(sview_eq(*s, sview_create_lit("a string")));
}

static void test_flags(void) {
    flag_reset();
    bool* b = flag_opt_bool("b", false, "a bool");
    bool* recursive = flag_exists('r', "recursive");
    bool* numeric = flag_exists('n', "numeric");
    bool* strict = flag_exists('s', "strict mode");


    const char* argv[] = {
        "prog",
        "-rn",
        "--b=true",
    };
    bool succ = flag_parse(ARRAY_LEN(argv), argv);
    test(succ);
    test(*b);
    test(*recursive);
    test(*numeric);
    test(!*strict);
}


static void test_args(void) {
    flag_reset();
    printf("----- TEST_ARGS ---- \n");
    bool* recursive = flag_exists('r', "recursive");
    bool* print_line_numbers = flag_exists('n', "print_line_numbers");
    sview* regex = flag_opt_string("include", "*", "pattern to for files to match");


    const char* argv[] = {
        "grep",
        "-rn",
        "--include=*.h",
        "./src/",
        "./test/",
        "./examples/"
    };
    bool succ = flag_parse(ARRAY_LEN(argv), argv);
    test(succ);
    test(*recursive);
    test(*print_line_numbers);
    test(sview_eq(*regex, sview_create_lit("*.h")));
    test(flag_argument_count() == 3);

    test(sview_eq(flag_argument_at(0), sview_create_lit("./src/")));
    test(sview_eq(flag_argument_at(1), sview_create_lit("./test/")));
    test(sview_eq(flag_argument_at(2), sview_create_lit("./examples/")));
}

int main(const int argc, const char** argv) {
    prepare_test(argc, argv);

    test_empty();
    test_options();
    test_unknown_option();
    test_unknown_flag();
    test_flags();
    test_args();


    return 0;
}
