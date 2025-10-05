#include "test.h"
#include <string_builder.h>

int main(const int argc, const char** argv) {
    prepare_test(argc, argv);

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
    sb_append_f(&sb, "Some crazy format: %llu, %s\n", 123, "string-format");
    test(sview_eq(sb_to_sview(&sb), sview_create_lit("hello , World!\nSome crazy format: 123, string-format\n")));

    const size_t len = sb.size;
    sview terminated = sb_append_null(&sb);
    test(len == terminated.len);
    sb_free(&sb);
    return 0;
}
