#ifndef CLUSTERSIMULATOR_ASSERTS_H
#define CLUSTERSIMULATOR_ASSERTS_H

#include <cassert>
#include <cstdio>
#include <cstdlib>


#ifndef PANIC_OFF
#define PANIC(S) printf("PANIC: " S); abort()
#else
#define PANIC(S)
#endif

#ifndef UNREACHABLE_OFF
#define UNREACHABLE(S) printf("UNREACHABLE: " S); abort()
#else
#define UNREACHABLE(S)
#endif

#ifndef ASSERT_OFF
#define ASSERT(expr)                                                               \
if (!static_cast<bool>(expr)) [[unlikely]] {                                       \
    printf("\nASSERT " #expr " In File: " __FILE__ " In Line: %i\n", __LINE__);    \
    abort();                                                                       \
};
#else
#define ASSERT(S)
#endif

#ifndef DEEBUG_PRINT_OFF
#define DEBUG_PRINT(S) printf(S)
#else
#define DEBUG_PRINT(S)
#endif


#endif //CLUSTERSIMULATOR_ASSERTS_H
