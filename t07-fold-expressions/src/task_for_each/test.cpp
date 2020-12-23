#include "for_each.hpp"
#include <tuple>
#include <cassert>

struct Incrementer {
    void operator()(int& i) const { i += 10; }
    void operator()(char& c) const { c += 10; }
};

struct StatefulIncrementer {
    int state = 1;
    void operator()(int& i) { i += 10 * state++; }
    void operator()(char& c) { c += 10 * state++; }
};

void test_for_each() {
    std::tuple<int, char> tp{42, 'a'};

    cls11::for_each(tp, Incrementer{});

    assert(std::get<0>(tp) == 52);
    assert(std::get<1>(tp) == 'k');
}

void test_for_each_order() {
    std::tuple<char, int> tp{'a', 42};

    cls11::for_each(tp, StatefulIncrementer{});

    assert(std::get<0>(tp) == 'k');
    assert(std::get<1>(tp) == 62);
}


int main() {
    test_for_each();
    test_for_each_order();
}
