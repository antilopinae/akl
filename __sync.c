#include "akl/__sync.h"
#include "akl/kern_lib.h"

int bool_compare_and_swap(volatile int* t, int expected, int desired) {
#ifdef __KERNEL_MODULE__
    katomic_t* atom = (katomic_t*)t;
    int old = katomic_cmpxchg(atom, expected, desired);
    return old == expected;
#else
    return __sync_bool_compare_and_swap(t, expected, desired);
#endif
}