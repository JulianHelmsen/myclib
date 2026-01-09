#include <string_builder.h>
#include <da.h>
#include <stdarg.h>
#include <string.h>

void sb_append_c(string_builder* sb, char c) {
    da_append(sb, c);
}

void sb_append_sv(string_builder* sb, sview sv) {
    da_reserve(sb, sb->size + sv.len);
    memcpy(&sb->items[sb->size], sv.data, sv.len);
    sb->size += sv.len;

}

void sb_append_cstr(string_builder* sb, const char* str) {
    const size_t len = strlen(str);
    da_reserve(sb, sb->size + len);
    memcpy(&sb->items[sb->size], str, len);
    sb->size += len;
}

void sb_append_f(string_builder* sb, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    const size_t n = (size_t) vsnprintf(NULL, 0, fmt, args);
    va_end(args);

    size_t new_cap = n + sb->size;
    da_reserve(sb, new_cap);
    va_start(args, fmt);
    int append = vsprintf(sb->items + sb->size, fmt, args);
    va_end(args);
    sb->size += append;
}

sview sb_to_sview(string_builder* sb) {
    return sview_create(sb->items, sb->size);
}

sview sb_append_null(string_builder* sb) {
    sview view = sb_to_sview(sb);
    sb_append_c(sb, '\0');
    return view;
}

void sb_free(string_builder* sb) {
    da_free(sb);
}

bool sb_replace_nth(string_builder* sb, sview occurr, sview replacement, size_t nth) {
    const size_t idx = sview_index_of_nth(sview_create(sb->items, sb->size), occurr, nth);
    if(idx == SVIEW_NPOS) return false;


    const size_t new_size = sb->size - occurr.len + replacement.len;
    da_reserve(sb, new_size);


    memmove(sb->items + idx + replacement.len, sb->items + idx + occurr.len, (sb->size - (idx + occurr.len)) * sizeof(char));
    memcpy(sb->items + idx, replacement.data, replacement.len * sizeof(char));

    sb->size = new_size;
    return true;
}

bool sb_replace(string_builder* sb, sview occurr, sview replacement) {
    return sb_replace_nth(sb, occurr, replacement, 0);
}

