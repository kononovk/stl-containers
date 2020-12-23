#include <cassert>
#include <cstring>
#include <string>

#include "lazy_string.hpp"

static void test_access_operator() {
    LazyString s1 = "Hello world";
    assert(s1.useCount() == 1);
    LazyString s2 = s1;
    assert(s1.useCount() == 2);

    assert(s1[1] == 'e');
    assert(s1.useCount() == 2);
    assert(s1.cStr() == s2.cStr());

    assert('e' == s1[1]);
    assert(s1.useCount() == 2);
    assert(s1.cStr() == s2.cStr());

    s1[1] = 'o';
    assert(s1.useCount() == 1);
    assert(s1.cStr() != s2.cStr());

    LazyString s3 = s2;
    assert(s2.useCount() == 2);
    s2[0]++;
    assert(s2.useCount() == 1);
    assert(s2[0] != s3[0]);
}

int main() {
    test_access_operator();
}
