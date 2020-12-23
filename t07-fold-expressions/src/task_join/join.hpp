#pragma once

#include <functional>

namespace cls11 {
template<class ... Functor>
struct Joined : private Functor ... {
    Joined(Functor &&... funcs) :
            Functor(std::forward<Functor>(funcs)) ... {}
    using Functor::operator()...;
};

template<class ... Functor>
decltype(auto) join(Functor&& ... funcs) {
    return Joined<Functor...>{std::forward<Functor>(funcs)...};
}
}