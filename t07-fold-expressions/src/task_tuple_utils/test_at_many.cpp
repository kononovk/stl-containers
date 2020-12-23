#include "tuple_utils.hpp"
#include <map>
#include <unordered_map>
#include <string>
#include <cassert>

void test_at_many() {
    std::map<std::string, int> m1 = {{"hello", 1},
                                     {"world", 2}};
    std::map <std::string, std::string> m2 = {{"hello", "nothing"}};

    auto expectedResult = std::make_tuple(1, "nothing", 1);
    assert(cls11::at_many("hello", m1, m2, m1) == expectedResult);
}


int main() {
    test_at_many();
}
