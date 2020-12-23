#include "unique_ptr.hpp"

#include <string>
#include <type_traits>
#include <cassert>


void test_class_props() {
    using int_unique_ptr = cls09::unique_ptr<int>;

    static_assert(!std::is_copy_constructible<int_unique_ptr>::value, "unique_ptr should not be copiable!");

    static_assert(!std::is_copy_assignable<int_unique_ptr>::value, "unique_ptr should not be assignable with '='!");

    static_assert(std::is_nothrow_move_assignable<int_unique_ptr>::value, "unique_ptr should be movable!");

    static_assert(std::is_nothrow_move_constructible<int_unique_ptr>::value,
                  "unique_ptr should be move constructible!");

    static_assert(std::is_nothrow_default_constructible<int_unique_ptr>::value,
                  "unique_ptr should be nothrow default constructible");

}

void test_empty_unique_ptr_is_ok() {
    cls09::unique_ptr<int> u1;
    cls09::unique_ptr<int> u2{};

    assert(!u1);
    assert(!u2);
}

void test_ptr_has_pointer_constructor() {
    cls09::unique_ptr<int> u1(new int(200));
    cls09::unique_ptr<int> u2{new int{100}};
}

void test_ptr_can_be_moved_from() {
    cls09::unique_ptr<int> first{new int{100}};

    assert(first); // not empty

    auto second = std::move(first);

    assert(!first); // empty
    assert(second); // not empty

    cls09::unique_ptr<int> third;
    third = std::move(second);

    assert(!first);
    assert(!second);
    assert(third);
}

void test_basic_methods() {
    struct Int {
        int value;
    };

    auto pointer = new Int{100};
    cls09::unique_ptr<Int> from{pointer};

    assert(from->value == 100);
    assert((*from).value == 100);

    assert(from.get() == pointer);
}

typedef unsigned int uint;
void test_deletion_happens() {
    uint destructions = 0;
    struct DestructionAware {
        uint* count_;

        explicit DestructionAware(uint* count) : count_(count) {}

        ~DestructionAware() {
            *count_ += 1;
        }
    };

    {
        cls09::unique_ptr<DestructionAware> ptr;
    }
    assert(destructions == 0);

    {
        cls09::unique_ptr<DestructionAware> ptr{new DestructionAware(&destructions)};
    }
    assert(destructions == 1);
    destructions = 0;

    {
        cls09::unique_ptr<DestructionAware> ptr1{new DestructionAware(&destructions)};

        ptr1 = cls09::unique_ptr<DestructionAware>{new DestructionAware(&destructions)};
        assert(destructions == 1);

        ptr1 = cls09::unique_ptr<DestructionAware>();
        assert(destructions == 2);
    }
    assert(destructions == 2);
    destructions = 0;

    {
        cls09::unique_ptr<DestructionAware> ptr1{new DestructionAware(&destructions)};
        cls09::unique_ptr<DestructionAware> ptr2{new DestructionAware(&destructions)};
        cls09::unique_ptr<DestructionAware> ptr3{new DestructionAware(&destructions)};

        ptr1 = std::move(ptr2); // destruction
        ptr2 = std::move(ptr3);
        ptr3 = std::move(ptr1);

        assert(destructions == 1);
    }

    assert(destructions == 3);
    destructions = 0;
}

static void test_reset_and_release() {
    {
        cls09::unique_ptr<int> ptr1{new int{10}};
        cls09::unique_ptr<int> ptr2{ptr1.release()};

        assert(!ptr1 && ptr1.get() == nullptr);
        assert(ptr2 && *ptr2 == 10);
    }

    {
        cls09::unique_ptr<int> ptr1;

        ptr1.reset(new int{10});
        assert(ptr1 && *ptr1 == 10);

        ptr1.reset(new int{20});
        assert(ptr1 && *ptr1 == 20);

        ptr1.reset();
        assert(!ptr1 && ptr1.get() == nullptr);
    }
}

void test_make_unique() {
    auto ptr = cls09::make_unique<std::string>(10, 'a');
    assert(*ptr == "aaaaaaaaaa");

    auto ptr2 = cls09::make_unique<std::string>(*ptr);
    assert(*ptr == *ptr2);

    auto ptr3 = cls09::make_unique<std::string>();
    assert(ptr3->empty());
}

int main() {
    test_class_props();
    test_empty_unique_ptr_is_ok();
    test_ptr_has_pointer_constructor();
    test_ptr_can_be_moved_from();
    test_basic_methods();
    test_deletion_happens();
    test_reset_and_release();
    test_make_unique();
}
