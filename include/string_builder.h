#ifndef MYLIBC_STRING_BUILDER_H
#define MYLIBC_STRING_BUILDER_H

#include <stddef.h>
#include "sview.h"
#include "macros.h"


typedef struct {
    size_t size;
    size_t capacity;
    char* items __counted_by(size);
} string_builder;


void sb_append_c(string_builder* sb, char c);
void sb_append_sv(string_builder* sb, sview sv);
void sb_append_cstr(string_builder* sb, const char* str);
ATTR_PRINTF(2, 3) void sb_append_f(string_builder* sb, const char* fmt, ...);
sview sb_to_sview(string_builder* sb);
sview sb_append_null(string_builder* sb);

void sb_free(string_builder* sb);

bool sb_replace(string_builder* sb, sview occurr, sview replacement);
bool sb_replace_nth(string_builder* sb, sview occurr, sview replacement, size_t nth);

#endif 
