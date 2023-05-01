#pragma once

#include <cstdint>
#include <limits>

#define MICROS_IN_S 1'000'000LLU
#define MILLI_IN_S 1'000LLU

static_assert(sizeof(uint64_t) == sizeof(unsigned long long));

constexpr auto operator""_S2MICROS(unsigned long long value) {
    return value * MICROS_IN_S;
}

constexpr auto operator""_S2MILLIS(unsigned long long value) {
    return value * MILLI_IN_S;
}

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
