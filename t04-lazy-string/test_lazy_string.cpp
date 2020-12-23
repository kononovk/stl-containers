#include "lazy_string.hpp"

#include <cassert>
#include <cstring>
#include <string>

static void test_shared_buffer() {
    {
        static_assert(!std::is_convertible_v<size_t, SharedBuffer>);

        SharedBuffer buffer { 10 };
        assert(buffer.useCount() == 1);
        SharedBuffer const ref = buffer;
        assert(buffer.useCount() == 2);
        assert(ref.useCount() == 2);

        assert(buffer.getSize() == 10);
        assert(ref.getSize() == 10);

        for (size_t i = 0; i < buffer.getSize(); ++i) {
            assert(buffer.getData()[i] == 0);
            assert(ref.getData()[i] == 0);

            buffer.getData()[i] = char(i);
            assert(ref.getData()[i] == char(i));
        }

        {
            SharedBuffer copy = buffer;
            assert(buffer.useCount() == 3);
            assert(copy.useCount() == 3);
            assert(buffer.getData() == copy.getData());
        }

        assert(buffer.useCount() == 2);
    }

    {
        static_assert(std::is_copy_constructible_v<SharedBuffer>);
        static_assert(std::is_copy_assignable_v<SharedBuffer>);

        SharedBuffer buf1 {100};
        SharedBuffer buf2 {buf1};
        SharedBuffer buf3 { 10 };

        SharedBuffer const& ref = buf1;

        assert(buf1.getSize() == buf2.getSize() && "Buffers should have the same size");
        assert(buf1.getData() == buf2.getData() && "Buffers should point to the same memory");
        assert(std::strcmp(buf1.getData(), buf2.getData()) == 0 && "Buffers should have equal content");

        buf3 = ref;
        assert(buf1.getData() == buf3.getData() && "After assignment, buf1 and buf3 should point to the same memory too");
    }

    {
        for (size_t i = 0; i < 100; ++i) {
            SharedBuffer b { i };
            SharedBuffer const& ref = b;
            b = ref;
        }
    }
}

static void test_lazy_string() {
    {
        LazyString s1;
        LazyString const& ref = s1;

        assert(s1.getSize() == 0);
        assert(ref.getSize() == 0);
        assert(s1.cStr()[0] == '\0');
        assert(ref.cStr()[0] == '\0');

        assert(s1.useCount() == 1);
    }

    {
        LazyString s1 = "Hello world";
        assert(s1.useCount() == 1);
        LazyString const s2 = s1;
        assert(s1.useCount() == 2);

        assert(s2.getSize() == s1.getSize());
        assert(s1.cStr() == s2.cStr());

        assert(s1.getSize() == sizeof("Hello world") - 1);
        assert(std::strcmp(s1.cStr(), "Hello world") == 0);
    }

    {
        LazyString s1;
        for (size_t i = 0; i < 10; ++i) {
            s1 += "A";
        }

        assert(s1.getSize() == 10);
        assert(std::strcmp(s1.cStr(), "AAAAAAAAAA") == 0);
    }

    {
        LazyString s = "Hello";
        LazyString s_concat = s + " with suffix";
        assert(std::strstr(s_concat.cStr(), s.cStr()) == s_concat.cStr());

        assert(strlen(s.cStr()) + 12 == strlen(s_concat.cStr()));
    }

    {
        std::string str {"Hello, world!"};
        LazyString lazy_str {str.c_str() };

        for (size_t i = 0; i < str.size(); ++i) {
            assert(str[i] == lazy_str.at(i));
        }
    }
}
#include <iostream>
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

    LazyString s4 = s2;
    assert(s2.useCount() == 2);
    ++s4[0];
    assert(s2.useCount() == 1);
    assert(s4.useCount() == 1);
    assert(s2[0] != s4[0]);
    s2[0] = s4[0];
    assert(s2[0] == s4[0]);
    LazyString tmp = "Hello world";
    tmp[0] = 'A';
    assert(tmp[0] == 'A');
}

void testCompare() {
    LazyString s1 = "Some string";
    assert(s1 == s1);

    LazyString s2 = "Some string";
    assert(s1 == s2);

    LazyString s3 = "Some string ";
    assert(s1 != s3);
}

void test_compare() {
    LazyString s1 = "Hello world";
    assert(s1[1] > s1[0]);
    assert(s1[1] >= s1[0]);
    assert(s1[0] < s1[1]);
    assert(s1[0] <= s1[1]);
}

int main() {
    testCompare();
    test_compare();
    test_shared_buffer();
    test_lazy_string();
    test_access_operator();
}






















