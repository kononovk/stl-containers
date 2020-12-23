#include "join.hpp"
#include <string>
#include <utility>
#include <cassert>


void test_join() {
    auto both = cls11::join(
            [](std::string const& s) -> std::string { return "it's a string"; },
            [](std::pair<int, int> const& p) -> std::string { return "it's a pair"; }
    );

    assert(both("string") == "it's a string");
    assert(both(std::pair(1, 2)) == "it's a pair");
}

int main() {
    test_join();
}

