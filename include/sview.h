#ifndef MYLIBC_SVIEW_H
#define MYLIBC_SVIEW_H

#include <stddef.h>
#include <stdbool.h>

#define SVIEW_NPOS ((size_t) -1)

typedef struct {
    const char* data;
    size_t len;
} sview;


sview sview_create(const char* str, size_t len);
sview sview_create_null_terminated(const char* str);
#define sview_create_lit(lit) sview_create(lit, sizeof(lit) - 1)

sview sview_subview(sview src, size_t begin, size_t len);
sview sview_chop_left(sview src, size_t begin);
sview sview_chop_right(sview src, size_t count);

int sview_cmp(sview a, sview b);
bool sview_eq(sview a, sview b);

size_t sview_index_of(sview haystack, sview needle);

bool sview_starts_with(sview str, sview start);
bool sview_ends_with(sview str, sview start);

bool sview_split(sview src, sview delim, sview* left, sview* right);

unsigned long long sview_to_ull(sview a);
long long sview_to_ll(sview a);
unsigned int sview_to_u(sview a);
long long sview_to_i(sview a);

#endif 
