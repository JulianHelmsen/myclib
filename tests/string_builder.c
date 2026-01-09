#include "test.h"
#include <string_builder.h>


void append_test(void) {
    string_builder sb = { 0 };

    sb_append_c(&sb, 'h');
    sb_append_c(&sb, 'e');
    sb_append_c(&sb, 'l');
    sb_append_c(&sb, 'l');
    sb_append_c(&sb, 'o');
    sb_append_c(&sb, ' ');

    test(sview_eq(sb_to_sview(&sb), sview_create_lit("hello ")));
    sb_append_sv(&sb, sview_create_lit(", World"));
    test(sview_eq(sb_to_sview(&sb), sview_create_lit("hello , World")));
    sb_append_cstr(&sb, "!\n");
    test(sview_eq(sb_to_sview(&sb), sview_create_lit("hello , World!\n")));
    sb_append_f(&sb, "Some crazy format: %llu, %s\n", 123llu, "string-format");
    test(sview_eq(sb_to_sview(&sb), sview_create_lit("hello , World!\nSome crazy format: 123, string-format\n")));

    const size_t len = sb.size;
    sview terminated = sb_append_null(&sb);
    test(len == terminated.len);
    sb_free(&sb);
}

void replace_test(void) {
    string_builder sb = { 0 };
    sb_append_cstr(&sb, "Hello. World");

    test(!sb_replace(&sb, sview_create_lit(","), sview_create_lit(",")));
    test(sview_eq(sb_to_sview(&sb), sview_create_lit("Hello. World")));
    test(sb_replace(&sb, sview_create_lit("."), sview_create_lit(",")));
    test(sview_eq(sb_to_sview(&sb), sview_create_lit("Hello, World")));

    test(sb_replace(&sb, sview_create_lit("World"), sview_create_lit("beautiful World")));
    test(sview_eq(sb_to_sview(&sb), sview_create_lit("Hello, beautiful World")));
    test(sb_replace(&sb, sview_create_lit("beautiful "), sview_create_lit("")));
    test(sview_eq(sb_to_sview(&sb), sview_create_lit("Hello, World")));

    sb_free(&sb);
}

int main(const int argc, const char** argv) {
    prepare_test(argc, argv);
  
    append_test();
    replace_test();

    return 0;
}
