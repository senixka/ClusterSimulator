#ifndef CLUSTERSIMULATOR_BOUNDEDTIME_H
#define CLUSTERSIMULATOR_BOUNDEDTIME_H

#include <cstdint>
#include <limits>

static_assert(sizeof(uint64_t) == sizeof(unsigned long long));

constexpr auto operator""_S2MICROS(unsigned long long value) {
    return value * 1'000'000LLU;
}

template<typename T>
T BoundedSum(T a, T b) {
    T res;
    if (__builtin_add_overflow(a, b, &res)) [[unlikely]] {
        res = std::numeric_limits<T>::max();
    }
    return res;
}

template<typename T>
bool AtBound(T value) {
    return value == std::numeric_limits<T>::max();
}


#endif //CLUSTERSIMULATOR_BOUNDEDTIME_H
