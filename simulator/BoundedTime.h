#pragma once

#include <cstdint>
#include <limits>


template<typename T>
inline T BoundedSum(T a, T b) {
    T res;
    if (__builtin_add_overflow(a, b, &res)) [[unlikely]] {
        res = std::numeric_limits<T>::max();
    }
    return res;
}

template<typename T>
inline bool AtBound(T value) {
    return value == std::numeric_limits<T>::max();
}
