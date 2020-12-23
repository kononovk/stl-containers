#include "shared_buffer.hpp"

#include <cassert>
#include <cstddef>
#include <cstring>
#include <type_traits>

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

int main() {
    test_shared_buffer();
}