#pragma once

#include <tuple>
#include <functional>

namespace cls11 {

namespace details {
template<class Tuple, class Functor, std::size_t... Is>
decltype(auto) applyImpl(Tuple &&t, Functor &&f, std::index_sequence<Is...> &&) {
    return f(std::get<Is>(std::forward<Tuple>(t))...);
}

template<class Functor, class Arg>
decltype(auto) try_invoke(Functor &&f, Arg &&arg) {
    if constexpr (std::is_invocable_v<Functor, Arg>) {
        return std::forward<Functor>(f)(std::forward<Arg>(arg));
    } else {
        return std::forward<Arg>(arg);
    }
}

template<class Tuple, class Functor, std::size_t... Is>
decltype(auto) transformElementsImpl(Tuple &&t, Functor &&f, std::index_sequence<Is...> &&) {
    return std::make_tuple(try_invoke(std::forward<Functor>(f), (std::get<Is>(std::forward<Tuple>(t)))) ...);
}
}

template<class Tuple, class Functor>
decltype(auto) apply(Functor &&f, Tuple &&t) {
    constexpr std::size_t S = std::tuple_size_v<std::decay_t<Tuple>>;
    return details::applyImpl(std::forward<Tuple>(t),
                              std::forward<Functor>(f),
                              std::make_index_sequence<S> {});
}

template<class Key, class ... Containers>
decltype(auto) at_many(Key &&key, Containers &&... containers) {
    return std::make_tuple(std::forward<Containers>(containers).at(std::forward<Key>(key))...);
}

template<class Tuple, class Functor>
decltype(auto) transformElements(Tuple &&t, Functor &&f) {
    constexpr std::size_t tuple_size = std::tuple_size_v<std::decay_t<Tuple>>;
    return details::transformElementsImpl(std::forward<Tuple>(t),
                                          std::forward<Functor>(f),
                                          std::make_index_sequence<tuple_size> {});
}
}