#ifndef MYLIBC_STRING_BUILDER_H
#define MYLIBC_STRING_BUILDER_H

#include <stddef.h>
#include <sview.h>


typedef struct {
    char* items;
    size_t size;
    size_t capacity;
} string_builder;


void sb_append_c(string_builder* sb, char c);
void sb_append_sv(string_builder* sb, sview sv);
void sb_append_cstr(string_builder* sb, const char* str);
void sb_append_f(string_builder* sb, const char* fmt, ...);
sview sb_to_sview(string_builder* sb);
sview sb_append_null(string_builder* sb);

void sb_free(string_builder* sb);

#endif 
