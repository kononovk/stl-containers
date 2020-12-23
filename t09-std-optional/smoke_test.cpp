#include <cassert>
#include <string>
#include <functional>

#include <optional>

#include "maybe.hpp"

#define TASK1
#define TASK2
#define TASK3

using std::string;

namespace
{
    std::size_t glb_alloc{ 0 };
    std::size_t glb_dealloc{ 0 };
}

void* operator new(std::size_t sz)
{
    glb_alloc += 1;
    return std::malloc(sz);
}

void operator delete(void* ptr) noexcept
{
    glb_dealloc += 1;
    std::free(ptr);
}

void check_mem()
{
    size_t was_alloc = glb_alloc;
    size_t was_dealloc = glb_dealloc;

    {
        control_03::maybe<int> mb;
        assert(was_alloc == glb_alloc);

        mb.reset(123);
        assert(was_alloc == glb_alloc);

        mb = 456;
        assert(was_alloc == glb_alloc);

        control_03::maybe<int> mb2(mb);
        assert(was_alloc == glb_alloc);

        control_03::maybe<int> mb3(789);
        assert(was_alloc == glb_alloc);

    }

    assert(was_dealloc == glb_dealloc);
}

void check_copy_n_create()
{
    struct no_def_ctor
    {
        no_def_ctor() = delete;

        no_def_ctor(int v) : param(v) {}
       /* ~no_def_ctor() {
            std::cout << "Here" << std::endl;
        }*/
        int param;
    };

    const int value = 42;
    no_def_ctor ndc(value);

    control_03::maybe<no_def_ctor> mb0;
    control_03::maybe<no_def_ctor> mb1(ndc);
    control_03::maybe<no_def_ctor> mb2(mb1);
    control_03::maybe<no_def_ctor> mb4(mb0);

    assert(!mb0.has_value());
    assert(mb1.has_value());
    assert(mb2.has_value());

    assert(mb1.value().param == value);
    assert(mb2.value().param == value);

    mb0 = no_def_ctor(value + 1);
    assert(mb0.has_value());
    assert(mb0.value().param == value + 1);
}

namespace test_destroy
{
    struct test
    {
        static int count;

        test()  { ++count; }
        test(test const&) { ++count; }

        ~test() { --count; }

        static bool no_leaks()
        {
            return count == 0;
        }
    };

    int test::count = 0;
} // test_destroy

void check_correct_destroy()
{
    using test_destroy::test;

    {
        control_03::maybe<test> mb0;
        control_03::maybe<test> mb1(test{});
        control_03::maybe<test> mb2(mb1);
        mb1 = mb2;
    }

    assert(test::no_leaks());
}

void check_operators()
{
    struct test
    {
        string field = "some_string";
    };

    control_03::maybe<test> mb1(test{});
    const control_03::maybe<test> mb2(mb1);
    control_03::maybe<test> mb3;

    assert(mb1);
    assert(!mb3);

    mb1.value().field = "other_string";

    assert(mb2.value().field == "some_string");
    assert(mb1.value().field == "other_string");

    mb1.value().field = "super_string";

    assert(mb2.value().field == "some_string");
    assert(mb1.value().field == "super_string");

    control_03::maybe<test> mb4{mb3};
    mb4 = mb3;
    mb3 = mb4;
    mb4 = mb1;
    mb1 = mb4;
    mb3 = mb1;
    assert(mb3.value().field == "super_string");

    mb3 = test{ "yet_another_string" };
    assert(mb3.value().field == "yet_another_string");

    // NOTE!
    // compilation should fail

    //const control_03::maybe<int> w(5);
    //w.value() = 6;
}

void check_reset()
{
    struct test
    {
        string field = "some_string";
    };

    control_03::maybe<test> mb1(test{});
    const control_03::maybe<test> mb2(mb1);

    mb1.reset();
    assert(!mb1.has_value());

    mb1.reset(test{ "new_value" });
    assert(mb1.value().field == "new_value");
}

void check_size()
{
    control_03::maybe<string> mbs;
    assert(sizeof(mbs) >= sizeof(string));
}

#ifdef TASK2
void check_ptr_maybe()
{
    string some = "hello";
    control_03::maybe<string*> mbs;

    mbs = nullptr;
    assert(!mbs);
    mbs.reset(&some);
    assert(mbs.value() == &some);
    assert(mbs);

    mbs.reset();
    assert(!mbs);

    assert(sizeof(mbs) == sizeof(string*));
}
#endif

template <typename>
struct MethodCounters {
    static inline size_t DefaultConstructed = 0;
    static inline size_t CopyConstructed = 0;
    static inline size_t CopyAssigned = 0;
    static inline size_t MoveConstructed = 0;
    static inline size_t MoveAssigned = 0;
    static inline size_t Destroyed = 0;
};

#define DefineCountedClass(ClassName) \
    struct ClassName {                                                                         \
        using Counters = MethodCounters<ClassName>;                                            \
        ClassName() { ++Counters::DefaultConstructed; }                                        \
        ClassName(const ClassName&) { ++Counters::CopyConstructed; }                           \
        ClassName& operator=(const ClassName &) { ++Counters::CopyAssigned; return *this; }    \
        ClassName(ClassName&&) { ++Counters::MoveConstructed; }                                \
        ClassName& operator=(ClassName &&) { ++Counters::MoveAssigned; return *this; }         \
        ~ClassName() { ++Counters::Destroyed; }                                                \
    };


void check_move_maybe()
{
    DefineCountedClass(MaybeMoveTestType);

    {
        MaybeMoveTestType o;
        control_03::maybe<MaybeMoveTestType> mb{o};
        control_03::maybe<MaybeMoveTestType> mb1;
        mb1 = std::move(mb);
    }

    assert(MethodCounters<MaybeMoveTestType>::DefaultConstructed == 1);
    assert(MethodCounters<MaybeMoveTestType>::CopyConstructed == 1);
    assert(MethodCounters<MaybeMoveTestType>::CopyAssigned == 0);
    assert(MethodCounters<MaybeMoveTestType>::MoveConstructed == 1);
    assert(MethodCounters<MaybeMoveTestType>::MoveAssigned == 0);
    assert(MethodCounters<MaybeMoveTestType>::Destroyed >= 3);
}

void check_move_value_to_maybe()
{
    DefineCountedClass(MaybeMoveValueTestType);

    {
        MaybeMoveValueTestType o;
        control_03::maybe<MaybeMoveValueTestType> mb{MaybeMoveValueTestType{}};
        control_03::maybe<MaybeMoveValueTestType> mb1{std::move(o)};
        mb1 = std::move(mb);
    }

    assert(MethodCounters<MaybeMoveValueTestType>::DefaultConstructed == 2);
    assert(MethodCounters<MaybeMoveValueTestType>::CopyConstructed == 0);
    assert(MethodCounters<MaybeMoveValueTestType>::CopyAssigned == 0);
    assert(MethodCounters<MaybeMoveValueTestType>::MoveConstructed == 2);
    assert(MethodCounters<MaybeMoveValueTestType>::MoveAssigned == 1);
    assert(MethodCounters<MaybeMoveValueTestType>::Destroyed >= 3);
}


#ifdef TASK3
void check_emplace()
{
    struct no_copy
    {
        enum
        {
            one_arg,
            two_args,
            ref,
            str,

            total
        };

        no_copy()                   : type(total) {}
        no_copy(int)                : type(one_arg) {}
        no_copy(int, double const&) : type(two_args) {}
        no_copy(double&)            : type(ref) {}
        no_copy(string)             : type(str) {}

        no_copy(no_copy const&) = delete;
        no_copy& operator=(no_copy const&) = delete;
        no_copy(no_copy&&) = delete;
        no_copy& operator=(no_copy&&) = delete;

        int type = total;
    };

    control_03::maybe<no_copy> y;

    y.emplace(42);
    assert(y.value().type == no_copy::one_arg);

    y.emplace(42, 42.);
    assert(y.value().type == no_copy::two_args);

    double tmp;
    y.emplace(std::ref(tmp));
    assert(y.value().type == no_copy::ref);

    y.emplace();
    assert(y.value().type == no_copy::total);
}

namespace frw_ns {
    DefineCountedClass(EmplaceForwardTestType);

    struct EmplaceForwardMaybeType {
        static inline size_t LvalueCount = 0;
        static inline size_t RvalueCount = 0;

        EmplaceForwardMaybeType(int, const EmplaceForwardTestType &) { ++LvalueCount; }

        EmplaceForwardMaybeType(int, EmplaceForwardTestType &&) { ++RvalueCount; }
    };
}

void check_emplace_perfect_forward()
{
    using namespace frw_ns;
    {
        control_03::maybe<EmplaceForwardMaybeType> mb;
        mb.emplace(42, EmplaceForwardTestType{});

        EmplaceForwardTestType local;
        mb.emplace(42, local);
        mb.emplace(42, std::move(local));
    }

    assert(MethodCounters<EmplaceForwardTestType>::DefaultConstructed == 2);
    assert(MethodCounters<EmplaceForwardTestType>::CopyConstructed == 0);
    assert(MethodCounters<EmplaceForwardTestType>::CopyAssigned == 0);
    assert(MethodCounters<EmplaceForwardTestType>::MoveConstructed == 0);
    assert(MethodCounters<EmplaceForwardTestType>::MoveAssigned == 0);
    assert(MethodCounters<EmplaceForwardTestType>::Destroyed == 2);

    assert(EmplaceForwardMaybeType::LvalueCount == 1);
    assert(EmplaceForwardMaybeType::RvalueCount == 2);
}
#endif

int main()
{
#ifdef TASK1
    check_mem();
    check_copy_n_create();
    check_correct_destroy();

    check_operators();
    check_reset();

    check_size();
    check_move_maybe();
    check_move_value_to_maybe();
#endif

#ifdef TASK2
    check_ptr_maybe();
#endif

#ifdef TASK3
    check_emplace();
    check_emplace_perfect_forward();
#endif
}
