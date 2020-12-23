#include "any_all.hpp"
#include <optional>
#include <cassert>
#include <memory>

void test_any() {
    assert(cls11::any(42, '\0', std::optional<int>{}, std::unique_ptr<int>()));
    assert(cls11::any(0, 'a', std::optional<int>{}, std::unique_ptr<int>()));
    assert(cls11::any(0, '\0', std::optional<int>{42}, std::unique_ptr<int>{}));
    assert(cls11::any(0, '\0', std::optional<int>{}, std::unique_ptr<int>{new int {42}}));
}

void test_all() {
    assert(!cls11::all(42, '\0', std::optional<int>{}, std::unique_ptr<int>()));
    assert(!cls11::all(0, 'a', std::optional<int>{}, std::unique_ptr<int>()));
    assert(!cls11::all(0, '\0', std::optional<int>{42}, std::unique_ptr<int>{}));
    assert(!cls11::all(0, '\0', std::optional<int>{}, std::unique_ptr<int>{new int {42}}));

    assert(cls11::all(42, 'a', std::optional<int>{42}, std::unique_ptr<int>{new int {42}}));
}

int main() {
    test_any();
    test_all();
}

