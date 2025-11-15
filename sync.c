#include "akl/sync.h"
#include "akl/kern_lib.h"

int akl_sync_bool_compare_and_swap(volatile int* t, int expected, int desired) {
#ifdef __KERNEL_MODULE__
    akl_atomic_t* atom = (akl_atomic_t*)t;
    int old = akl_atomic_cmpxchg(atom, expected, desired);
    return old == expected;
#else
    return __sync_bool_compare_and_swap(t, expected, desired);
#endif
}

int akl_sync_add_and_fetch(volatile int* t, int val) {
#ifdef __KERNEL_MODULE__
    akl_atomic_t* atom = (akl_atomic_t*)t;
    return akl_atomic_add_return(val, atom);
#else
    return __sync_add_and_fetch(t, val);
#endif
}

int akl_sync_fetch_and_add(volatile int* t, int val) {
#ifdef __KERNEL_MODULE__
    akl_atomic_t* atom = (akl_atomic_t*)t;
    return akl_atomic_add_return(val, atom) - val;
#else
    return __sync_fetch_and_add(t, val);
#endif
}

int akl_sync_sub_and_fetch(volatile int* t, int val) {
#ifdef __KERNEL_MODULE__
    akl_atomic_t* atom = (akl_atomic_t*)t;
    return akl_atomic_sub_return(val, atom);
#else
    return __sync_sub_and_fetch(t, val);
#endif
}

int akl_sync_fetch_and_sub(volatile int* t, int val) {
#ifdef __KERNEL_MODULE__
    akl_atomic_t* atom = (akl_atomic_t*)t;
    return akl_atomic_sub_return(val, atom) + val;
#else
    return __sync_fetch_and_sub(t, val);
#endif
}

int akl_sync_lock_test_and_set(volatile int* t, int val) {
#ifdef __KERNEL_MODULE__
    akl_atomic_t* atom = (akl_atomic_t*)t;
    return akl_atomic_xchg(atom, val);
#else
    return __sync_lock_test_and_set(t, val);
#endif
}