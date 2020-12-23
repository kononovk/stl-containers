#include "tuple_utils.hpp"
#include <cassert>
#include <tuple>


struct Summator {
    template<typename... Args>
    decltype(auto) operator()(Args... args) const {
        return (args + ...);
    }
};


void test_apply() {
    auto result = cls11::apply(Summator{}, std::make_tuple(1, 2, 3L, 'a'));
    assert(result == 103);
}


int main () {
    test_apply();
}
