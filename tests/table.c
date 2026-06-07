#include <table.h>
#include <stdio.h>
#include "test.h"


int main(const int argc, const char** argv) {
    prepare_test(argc, argv);


    int columns[] = { 0, 20, 35, 50};

    table_formatter fmt = table_formatter_create_from_arr(TABLE_FMT_START, columns);

    table_fmt(stdout, &fmt, "First col");
    table_fmt(stdout, &fmt, "Second col");
    table_fmt(stdout, &fmt, "Third col");
    table_fmt_reset(&fmt);

    for(int i = 0; i < 10; ++i) {
        int v = i;
        int v2 = v * v;
        int v3 = v2 * v;
        table_fmt(stdout, &fmt, "%d", v);
        table_fmt(stdout, &fmt, "%d", v2);
        table_fmt(stdout, &fmt, "%d", v3);
        table_fmt_reset(&fmt);

    }


    return 0;
}
