#pragma once

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <iomanip>

#define FIXED_PRC std::fixed << std::setprecision(15)

#ifndef PANIC_OFF
#define PANIC(S) printf("PANIC: " S); exit(25)
#else
#define PANIC(S)
#endif

#ifndef UNREACHABLE_OFF
#define UNREACHABLE(S) printf("UNREACHABLE: " S); exit(24)
#else
#define UNREACHABLE(S)
#endif

#ifndef ASSERT_OFF
#define ASSERT(expr)                                                               \
if (!static_cast<bool>(expr)) [[unlikely]] {                                       \
    printf("\nASSERT " #expr " In File: " __FILE__ " In Line: %i\n", __LINE__);    \
    exit(23);                                                                      \
};
#else
#define ASSERT(S)
#endif

#ifndef DEEBUG_PRINT_OFF
#define DEBUG_PRINT(S) printf(S)
#else
#define DEBUG_PRINT(S)
#endif
