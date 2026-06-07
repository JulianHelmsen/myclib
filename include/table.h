#ifndef MYLIBC_TABLE_H
#define MYLIBC_TABLE_H

#include <stdio.h>

#ifndef ATTR_PRINTF
#define ATTR_PRINTF(fmt_idx, args_idx) 
#endif


typedef enum {
    TABLE_FMT_CENTER,
    TABLE_FMT_START
} table_fmt_mode;

typedef struct {
    table_fmt_mode mode;
    const int* col_offsets;
    size_t n_cols;

    int current_offset;
    size_t col;
} table_formatter;

#define table_formatter_create_from_arr(mode, columns) table_formatter_create(mode, columns, sizeof(columns) / sizeof(columns[0]) - 1)

table_formatter table_formatter_create(table_fmt_mode mode, const int* cols, size_t n_cols);

void table_horizontal_line(table_formatter* fmt);


 ATTR_PRINTF(3, 4) void table_fmt(FILE* f, table_formatter* formatter, const char* fmt, ...);

void table_fmt_reset(table_formatter* fmt);
#endif // MYLIBC_TABLE_H
