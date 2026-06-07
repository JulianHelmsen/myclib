#include "table.h"
#include <assert.h>
#include <stdarg.h>

table_formatter table_formatter_create(table_fmt_mode mode, const int* cols, size_t n_cols) {
    return (table_formatter) {
        .mode = mode,
        .col_offsets = cols,
        .n_cols = n_cols
    };
}

int compute_start_offset(table_fmt_mode mode, int current, int end, int measured_size) {

    if(end == -1) return current;
    int width = end - current;
    if(width < measured_size) {
        return current; // doesn't fit
    }

    const int center = (end + current) / 2;

    switch(mode) {
    case TABLE_FMT_CENTER: return center - measured_size / 2;
    case TABLE_FMT_START: return current;
    default:
        assert(0 && "Illegal table_fmt_mode");
    }
}


void table_fmt(FILE* f, table_formatter* formatter, const char* fmt, ...) {
    assert(formatter->col < formatter->n_cols);
    va_list args;
    va_start(args, fmt);
    int measured_width;
    {
        va_list measure_args;
        va_copy(measure_args, args);
        measured_width = vsnprintf(NULL, 0, fmt, measure_args);
        va_end(measure_args);
    }

    formatter->current_offset += printf("|");
    int end = -1;
    if(formatter->col < formatter->n_cols)
        end = formatter->col_offsets[formatter->col + 1];

    int start_offset = compute_start_offset(formatter->mode, formatter->current_offset, end, measured_width);

    int leading_spaces = start_offset - formatter->current_offset;
    if(leading_spaces > 0)
        formatter->current_offset += fprintf(f, "%*c", leading_spaces, ' ');

    formatter->current_offset += vfprintf(f, fmt, args);
    int trailing_spaces = 0;
    if(end != -1) 
        trailing_spaces = end - formatter->current_offset;

    if(trailing_spaces > 0)
        formatter->current_offset += fprintf(f, "%*c", trailing_spaces, ' ');

    va_end(args);
    formatter->col += 1;
    if(formatter->col == formatter->n_cols) printf("|\n");

}


void table_fmt_reset(table_formatter* fmt) {
    fmt->col = 0;
    fmt->current_offset = 0;
}
