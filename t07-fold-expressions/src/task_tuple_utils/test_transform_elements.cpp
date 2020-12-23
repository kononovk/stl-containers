/*
#include "tuple_utils.hpp"
#include <cassert>
#include <utility>

struct Transformator {
    template <typename T>
    decltype(auto) operator()(T& t) const {
        return std::make_pair(t, t);
    }

    auto operator()(char& c) const { return c + 10; }
};


void test_transform_all_elements() {
    std::tuple<int, char> tp{42, 'a'};

    auto transformedTuple = cls11::transformElements(tp, Transformator{});

    assert(std::get<0>(transformedTuple) == std::make_pair(42, 42));
    assert(std::get<1>(transformedTuple) == 'k');
}

void test_transform_some_elements() {
    std::tuple<std::pair<int, int>, char> tp{std::make_pair(42, 24), 'a'};

    auto transformedTuple = cls11::transformElements(tp, [](const char &c) { return c + 10; });

    assert(std::get<0>(transformedTuple) == std::make_pair(42, 24));
    assert(std::get<1>(transformedTuple) == 'k');
}

int main() {
    test_transform_all_elements();
    test_transform_some_elements();
}*/

#include "tuple_utils.hpp"
#include <cassert>
#include <utility>

struct TransformatorWithRvalueOverloads {
    template <typename T>
    decltype(auto) operator()(T& t) const & {
        return std::make_pair(t, t);
    }

    char operator()(char& c) const & { return static_cast<char>(c + 10); }

    template <typename T>
    decltype(auto) operator()(T& t) const && {
        return std::make_tuple(t, t, t, t);
    }

    int operator()(char&) const && { return 42; }
};

static void test_forwarding() {
    std::tuple<int, char> tp{42, 'a'};

    TransformatorWithRvalueOverloads tr;
    auto transformedTuple = cls11::transformElements(tp, tr);

    assert(std::get<0>(transformedTuple) == std::make_pair(42, 42));
    assert(std::get<1>(transformedTuple) == 'k');

    auto transformedTupleWithRvalueFunctor = cls11::transformElements(tp, std::move(tr));

    assert(std::get<0>(transformedTupleWithRvalueFunctor) == std::make_tuple(42, 42, 42, 42));
    assert(std::get<1>(transformedTupleWithRvalueFunctor) == 42);
}

int main() {
    test_forwarding();
    return 0;
}
