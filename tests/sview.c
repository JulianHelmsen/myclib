#include <sview.h>
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include "test.h"

#define lit(s) sview_create_lit(s)


int main(const int argc, const char** argv) {
    prepare_test(argc, argv);

    test(lit("").len == 0);
    test(lit("abc").len == 3);

    test(sview_create_null_terminated("").len == 0);
    test(sview_create_null_terminated("abc").len == 3);

    test(sview_index_of(lit("xyzabcde"), lit("abc")) == 3);
    test(sview_index_of(lit("xyzabc"), lit("abc")) == 3);
    test(sview_index_of(lit("abcxyz"), lit("abc")) == 0);
    test(sview_index_of(lit("abc"), lit("abc")) == 0);
    test(sview_index_of(lit("a bc"), lit("abc")) == SVIEW_NPOS);
    test(sview_index_of(lit(""), lit("abc")) == SVIEW_NPOS);

    test(sview_cmp(lit("a"), lit("b")) == -1);
    test(sview_cmp(lit("b"), lit("a")) == 1);
    test(sview_cmp(lit("xyz"), lit("xyz")) == 0);
    test(sview_cmp(lit("xyzaaa"), lit("xyzaba")) == -1);
    test(sview_cmp(lit("A"), lit("B")) == -1);
    test(sview_cmp(lit("A"), lit("b")) == -33);
    test(sview_cmp(lit("B"), lit("A")) == 1);
    test(sview_cmp(lit("B"), lit("a")) == -31);
    test(sview_cmp(lit("XYZ"), lit("XYZ")) == 0);
    test(sview_cmp(lit("XYZAAA"), lit("XYZABA")) == -1);



    test(!sview_eq(lit("a"), lit("b")));
    test(!sview_eq(lit("b"), lit("a")));
    test(sview_eq(lit("xyz"), lit("xyz")));

    test(sview_eq(sview_subview(lit("xyzabc"), 1, SVIEW_NPOS), lit("yzabc")));
    test(sview_eq(sview_subview(lit("xyzabc"), 1, 2), lit("yz")));

    test(sview_eq(sview_chop_left(lit("xyzabc"), 1), lit("yzabc")));
    test(sview_eq(sview_chop_right(lit("xyzabc"), 1), lit("xyzab")));

    test(sview_starts_with(lit("xyzabc"), lit("xyz")));
    test(!sview_starts_with(lit("axyzabc"), lit("xyz")));
    test(!sview_starts_with(lit(""), lit("xyz")));
    test(!sview_starts_with(lit("xy"), lit("xyz")));

    test(sview_ends_with(lit("xyzabc"), lit("abc")));
    test(!sview_ends_with(lit("axyzabic"), lit("abc")));
    test(!sview_ends_with(lit(""), lit("xyz")));
    test(!sview_ends_with(lit("xy"), lit("xyz")));

    
    sview left, right;
    test(sview_split(lit("abc"), lit("b"), &left, &right));
    test(sview_eq(left, lit("a")));
    test(sview_eq(right, lit("c")));

    test(!sview_split(lit("afc"), lit("b"), &left, &right));
    test(sview_eq(left, lit("afc")));
    test(right.len == 0);

    test(!sview_split(lit("afc"), lit("b"), &left, NULL));
    test(sview_eq(left, lit("afc")));

 
    test(!sview_split(lit("afc"), lit("b"), NULL, &right));
    test(right.len == 0);

    test(sview_to_ull(lit("0")) == 0ull);
    test(sview_to_ull(lit("12387")) == 12387ull);
    test(sview_to_ull(lit("+12387")) == 12387ull);
    test(sview_to_ull(lit("-12387")) == ULLONG_MAX);
    test(errno == EINVAL); errno = 0;
    test(sview_to_ull(lit("+")) == ULLONG_MAX);
    test(errno == EINVAL); errno = 0;
    test(sview_to_ull(lit("")) == ULLONG_MAX);
    test(errno == EINVAL); errno = 0;
    test(sview_to_ull(lit("abcdef")) == ULLONG_MAX);
    test(errno == EINVAL); errno = 0;
    test(sview_to_ull(lit("123asd")) == ULLONG_MAX);
    test(errno == EINVAL); errno = 0;
    test(sview_to_ull(lit("18446744073709552004")) == ULLONG_MAX);
    test(errno == ERANGE); errno = 0;

    test(sview_to_u(lit("0")) == 0u);
    test(sview_to_u(lit("12387")) == 12387u);
    test(sview_to_u(lit("+12387")) == 12387u);
    test(sview_to_u(lit("-12387")) == UINT_MAX);
    test(errno == EINVAL); errno = 0;
    test(sview_to_u(lit("+")) == UINT_MAX);
    test(errno == EINVAL); errno = 0;
    test(sview_to_u(lit("")) == UINT_MAX);
    test(errno == EINVAL); errno = 0;
    test(sview_to_u(lit("abcdef")) == UINT_MAX);
    test(errno == EINVAL); errno = 0;
    test(sview_to_u(lit("123asd")) == UINT_MAX);
    test(errno == EINVAL); errno = 0;
    test(sview_to_u(lit("4294967296")) == UINT_MAX);
    test(errno == ERANGE); errno = 0;
    test(sview_to_u(lit("4294967295")) == 4294967295);

    test(sview_to_ll(lit("0")) == 0ll);
    test(sview_to_ll(lit("12387")) == 12387ll);
    test(sview_to_ll(lit("+12387")) == 12387ll);
    test(sview_to_ll(lit("-12387")) == -12387ll);
    test(sview_to_ll(lit("+")) == LLONG_MAX);
    test(errno == EINVAL); errno = 0;
    test(sview_to_ll(lit("")) == LLONG_MAX);
    test(errno == EINVAL); errno = 0;
    test(sview_to_ll(lit("abcdef")) == LLONG_MAX);
    test(errno == EINVAL); errno = 0;
    test(sview_to_ll(lit("123asd")) == LLONG_MAX);
    test(errno == EINVAL); errno = 0;
    test(sview_to_ll(lit("9223372036854776000")) == LLONG_MAX);
    test(errno == ERANGE); errno = 0;
    test(sview_to_ll(lit("-9223372036854776000")) == LLONG_MAX);
    test(errno == ERANGE); errno = 0;

    test(sview_to_i(lit("0")) == 0);
    test(sview_to_i(lit("12387")) == 12387);
    test(sview_to_i(lit("+12387")) == 12387);
    test(sview_to_i(lit("-12387")) == -12387);
    test(sview_to_i(lit("+")) == INT_MAX);
    test(errno == EINVAL); errno = 0;
    test(sview_to_i(lit("")) == INT_MAX);
    test(errno == EINVAL); errno = 0;
    test(sview_to_i(lit("abcdef")) == INT_MAX);
    test(errno == EINVAL); errno = 0;
    test(sview_to_i(lit("123asd")) == INT_MAX);
    test(errno == EINVAL); errno = 0;
    test(sview_to_i(lit("2147483648")) == INT_MAX);
    test(errno == ERANGE); errno = 0;
    test(sview_to_i(lit("-2147483648")) == INT_MAX);
    test(errno == ERANGE); errno = 0;

    sview trim_test_left_empty = sview_create_lit("");
    test(sview_trim_left(trim_test_left_empty).len == 0);
    sview trim_test_right_empty = sview_create_lit("");
    test(sview_trim_right(trim_test_right_empty).len == 0);


    sview trim_test_left = sview_create_lit("asd");
    test(sview_eq(sview_trim_left(trim_test_left), trim_test_left));

    trim_test_left = sview_create_lit(" \r\n\t asd");
    test(sview_eq(sview_trim_left(trim_test_left), sview_create_lit("asd")));

    trim_test_left = sview_create_lit(" \r\n\t asd      \r\n\t");
    test(sview_eq(sview_trim_left(trim_test_left), sview_create_lit("asd      \r\n\t")));

    sview trim_test_right = sview_create_lit(" \r\n\t asd      \r\n\t");
    test(sview_eq(sview_trim_right(trim_test_right), sview_create_lit(" \r\n\t asd")));
    trim_test_right = sview_create_lit("asd      \r\n\t");
    test(sview_eq(sview_trim_right(trim_test_right), sview_create_lit("asd")));

    sview trim_test = sview_create_lit(" \r\n\t asd      \r\n\t");
    test(sview_eq(sview_trim(trim_test), sview_create_lit("asd")));
 

    sview a = lit("vn 1 2 3");
    sview b;
    test(!sview_strip_prefix(a, lit("asd"), &b));
    test(sview_strip_prefix(a, lit("v"), &b));
    test(sview_eq(b, lit("n 1 2 3")));
    test(sview_strip_prefix(a, lit("vn"), &b));
    test(sview_eq(b, lit(" 1 2 3")));
    test(!sview_strip_prefix(a, lit("vna"), &b));
    test(sview_strip_prefix(a, lit(""), &b));
    test(sview_eq(b, a));


    test(sview_hash(lit("")) == 0);
    test(sview_hash(lit("a")) == 97);
    test(sview_hash(lit("ab")) == 'b' * 31 + 'a');
    test(sview_hash(lit("ba")) == 'a' * 31 + 'b');
    test(sview_hash(lit("b\0")) == 'b');
    test(sview_hash(lit("\0b")) == 'b' * 31);

    test(sview_cmp_ign_case(lit("A"), lit("B")) == -1);
    test(sview_cmp_ign_case(lit("A"), lit("b")) == -1);
    test(sview_cmp_ign_case(lit("B"), lit("A")) == 1);
    test(sview_cmp_ign_case(lit("B"), lit("a")) == 1);
    test(sview_cmp_ign_case(lit("XYZ"), lit("XYZ")) == 0);
    test(sview_cmp_ign_case(lit("uYZ"), lit("uYZ")) == 0);
    test(sview_cmp_ign_case(lit("uyz"), lit("uyz")) == 0);
    test(sview_cmp_ign_case(lit("XYZAAA"), lit("XYZABA")) == -1);
    test(sview_cmp_ign_case(lit(""), lit("")) == 0);

    
    test(!sview_eq_ign_case(lit("a"), lit("b")));
    test(!sview_eq_ign_case(lit("b"), lit("a")));
    test(sview_eq_ign_case(lit("xyz"), lit("xyz")));

    test(sview_eq_ign_case(sview_subview(lit("xyzabc"), 1, SVIEW_NPOS), lit("yzabc")));
    test(sview_eq_ign_case(sview_subview(lit("xyzabc"), 1, 2), lit("yz")));

    test(sview_eq_ign_case(sview_chop_left(lit("xyzabc"), 1), lit("yzabc")));
    test(sview_eq_ign_case(sview_chop_right(lit("xyzabc"), 1), lit("xyzab")));

    test(!sview_eq_ign_case(lit("a"), lit("B")));
    test(!sview_eq_ign_case(lit("b"), lit("A")));
    test(sview_eq_ign_case(lit("xyz"), lit("Xyz")));

    test(sview_eq_ign_case(sview_subview(lit("xyzabc"), 1, SVIEW_NPOS), lit("yzabc")));
    test(sview_eq_ign_case(sview_subview(lit("xYZABC"), 1, 2), lit("yz")));

    test(sview_eq_ign_case(sview_chop_left(lit("xyzabc"), 1), lit("yzabc")));
    test(sview_eq_ign_case(sview_chop_right(lit("xyzabc"), 1), lit("xyzab")));


    return 0;
}
