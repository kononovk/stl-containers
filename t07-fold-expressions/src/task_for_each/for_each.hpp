#pragma once

#include <tuple>
#include <iostream>

namespace cls11 {

namespace detail {
template<class Tuple, class Functor, std::size_t... Is>
void for_each_impl(Tuple &&t, Functor &&f, std::index_sequence<Is...> &&) {
    /// (void)f(arg1), (void)f(arg2), ...
    ((void) f(std::get<Is>(std::forward<Tuple>(t))), ...);
    /// or this way:
    /// ((f(std::get<Is>(t)), (void)0), ...); // (f(arg1), void(0)), (f(arg2), void(0)), ...
}
}

template<class Tuple, class Functor>
void for_each(Tuple &&t, Functor &&f) {
    using tuple_t = std::decay_t<Tuple>;
    constexpr size_t S = std::tuple_size_v<tuple_t>;
    detail::for_each_impl(std::forward<Tuple>(t),
             std::forward<Functor>(f),
             std::make_index_sequence<S> {});
}
}