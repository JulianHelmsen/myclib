#include <sview.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <limits.h>

sview sview_create(const char* str, size_t len) {
    return (sview) {
        .data = str,
        .len = len
    };
}
sview sview_create_null_terminated(const char* str) {
    return sview_create(str, strlen(str));
}

sview sview_subview(sview src, size_t begin, size_t len) {
    if(len == SVIEW_NPOS) len = src.len - begin;

    assert(begin <= src.len);
    assert(len <= src.len);
    assert(begin + len <= src.len);

    return sview_create(src.data + begin, len);

}

sview sview_chop_left(sview src, size_t begin) {
    assert(begin <= src.len);
    return sview_create(src.data + begin, src.len - begin);
}

sview sview_chop_right(sview src, size_t count) {
    assert(count <= src.len);
    return sview_create(src.data, src.len - count);
}

int sview_cmp(sview a, sview b) {
    for(size_t i = 0; i < a.len; ++i) {
        char ac = a.data[i];
        char bc = i < b.len ? b.data[i] : '\0';
        int diff = ac - bc;
        if(diff != 0)
            return diff;
    }
    return 0;
}

bool sview_eq(sview a, sview b) {
    if(a.len != b.len) return false;
    return sview_cmp(a, b) == 0;
}

size_t sview_index_of(sview haystack, sview needle) {
    if(needle.len > haystack.len) return SVIEW_NPOS;

    for(size_t i = 0; i < 1 + haystack.len - needle.len; ++i) {
        if(memcmp(haystack.data + i, needle.data, needle.len) == 0) {
            return (long long) i;
        }
    }

    return SVIEW_NPOS;
}

bool sview_starts_with(sview str, sview start) {
    if(str.len < start.len) return false;
    return memcmp(str.data, start.data, start.len) == 0;
}

bool sview_ends_with(sview str, sview start) {
    if(str.len < start.len) return false;
    return memcmp(str.data + str.len - start.len, start.data, start.len) == 0;
}

bool sview_split(sview src, sview delim, sview* left, sview* right) {
    const size_t idx = sview_index_of(src, delim);
    if(idx == SVIEW_NPOS) {
        if(left) *left = src;
        if(right) *right = sview_create(NULL, 0);
        return false;
    }


    if(left) *left = sview_create(src.data, idx);
    if(right) *right = sview_subview(src, idx + delim.len, SVIEW_NPOS);

    return true;
}


unsigned long long sview_to_ull(sview a) {
    if(sview_starts_with(a, sview_create_lit("+"))) a = sview_chop_left(a, 1);

    if(a.len == 0) {
        errno = EINVAL;
        return ULLONG_MAX;
    }

    unsigned long long val = 0;
    for(size_t i = 0; i < a.len; ++i) {
        int dig = a.data[i] - '0';
        if(dig < 0 || dig > 9) {
            errno = EINVAL;
            return ULLONG_MAX;
        }

        if(val > (ULLONG_MAX - dig) / 10) {
            errno = ERANGE;
            return ULLONG_MAX;
        }

        val = val * 10 + dig;
    }

    return val;
}

unsigned int sview_to_u(sview a) {
    if(sview_starts_with(a, sview_create_lit("+"))) a = sview_chop_left(a, 1);

    if(a.len == 0) {
        errno = EINVAL;
        return UINT_MAX;
    }

    unsigned int val = 0;
    for(size_t i = 0; i < a.len; ++i) {
        int dig = a.data[i] - '0';
        if(dig < 0 || dig > 9) {
            errno = EINVAL;
            return UINT_MAX;
        }

        if(val > (UINT_MAX - dig) / 10) {
            errno = ERANGE;
            return UINT_MAX;
        }

        val = val * 10 + dig;
    }

    return val;
}

long long sview_to_ll(sview a) {
    bool neg = false;

    if(sview_starts_with(a, sview_create_lit("-"))) {
        neg = true;
        a = sview_chop_left(a, 1);
    }

    errno = 0;
    unsigned long long val = sview_to_ull(a);
    if(val == ULLONG_MAX && errno != 0) return LLONG_MAX;
    if(val > LLONG_MAX) {
        errno = ERANGE;
        return LLONG_MAX;
    }

    return neg ? -(long long) val : (long long) val;
}

long long sview_to_i(sview a) {
    bool neg = false;

    if(sview_starts_with(a, sview_create_lit("-"))) {
        neg = true;
        a = sview_chop_left(a, 1);
    }

    errno = 0;
    unsigned long long val = sview_to_u(a);
    if(val == UINT_MAX && errno != 0) return INT_MAX;
    if(val > INT_MAX) {
        errno = ERANGE;
        return INT_MAX;
    }

    return neg ? -(long long) val : (long long) val;
}
