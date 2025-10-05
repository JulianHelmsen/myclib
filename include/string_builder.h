#ifndef MYLIBC_STRING_BUILDER_H
#define MYLIBC_STRING_BUILDER_H

#include <stddef.h>
#include <sview.h>


typedef struct {
    char* items;
    size_t size;
    size_t capacity;
} string_builder;

#if defined(__GNUC__) || defined(__clang__)
#define ATTR_PRINTF(fmt_idx, args_idx) __attribute__((__format__(__printf__, fmt_idx, args_idx)))
#else
#define ATTR_PRINTF(fmt_idx, args_idx) 
#endif

void sb_append_c(string_builder* sb, char c);
void sb_append_sv(string_builder* sb, sview sv);
void sb_append_cstr(string_builder* sb, const char* str);
ATTR_PRINTF(2, 3) void sb_append_f(string_builder* sb, const char* fmt, ...);
sview sb_to_sview(string_builder* sb);
sview sb_append_null(string_builder* sb);

void sb_free(string_builder* sb);

#endif 
