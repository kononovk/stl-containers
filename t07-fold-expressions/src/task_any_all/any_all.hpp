#pragma once

namespace cls11 {
template<class ... Args>
bool all(Args... args) {
    return (args && ...);
}

template<class ... Args>
bool any(Args... args) {
    return (args || ...);
}
}
