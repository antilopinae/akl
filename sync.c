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

akl_atomic_float_t akl_sync_lock_test_and_set_float(
    volatile akl_atomic_float_t* t, akl_atomic_float_t val
) {
#ifdef __KERNEL_MODULE__
    return akl_atomic_xchg_float(t, val);
#else
    akl_atomic_float_t oldv, newv;

    oldv.f = t->f;
    newv.f = val.f;

    akl_u32 prev;
    do {
        prev = oldv.u;
        oldv.u = __sync_val_compare_and_swap((volatile akl_u32*)t, prev, newv.u);
    } while (oldv.u != prev);

    return *(akl_atomic_float_t*)&prev;
#endif
}

// double akl_sync_lock_test_and_set_double(volatile double* t, double val) {
// #ifdef __KERNEL_MODULE__
//     akl_atomic_double_t* atom = (akl_atomic_double_t*)t;
//
//     akl_atomic_double_t new_val;
//     new_val.d = val;
//
//     return akl_atomic_xchg_double(atom, new_val.u);
// #else
//     akl_atomic_double_t oldv, newv;
//
//     oldv.d = *t;
//     newv.d = val;
//
//     akl_u64 prev;
//     do {
//         prev = oldv.u;
//         oldv.u = __sync_val_compare_and_swap((volatile akl_u64*)t, prev, newv.u);
//     } while (oldv.u != prev);
//
//     return *(double*)&prev;
// #endif
// }

int akl_atomic_compare_and_swap(volatile int* t, int expected, int desired) {
    return akl_sync_bool_compare_and_swap(t, expected, desired);
}

int akl_atomic_compare_and_swap_float(
    volatile akl_atomic_float_t* t,
    akl_atomic_float_t expected,
    akl_atomic_float_t desired
) {
#ifdef __KERNEL_MODULE__
    return akl_atomic_cmpxchg_float(t, expected, desired);
#else
    volatile akl_u32* a_ptr = (volatile akl_u32*)t;
    const akl_u32* oldval_ptr = (const akl_u32*)&expected;
    const akl_u32* newval_ptr = (const akl_u32*)&desired;
    return __sync_bool_compare_and_swap(a_ptr, *oldval_ptr, *newval_ptr);
#endif
}

// int akl_atomic_compare_and_swap_double(
//     volatile double* t, double expected, double desired
// ) {
// #ifdef __KERNEL_MODULE__
//     akl_atomic_double_t* atom = (akl_atomic_double_t*)t;
//
//     const akl_u64* oldval_ptr = (const akl_u64*)&expected;
//     const akl_u64* newval_ptr = (const akl_u64*)&desired;
//
//     return akl_atomic_cmpxchg_double(atom, *oldval_ptr, *newval_ptr);
// #else
//     volatile akl_u64* a_ptr = (volatile akl_u64*)t;
//     const akl_u64* oldval_ptr = (const akl_u64*)&expected;
//     const akl_u64* newval_ptr = (const akl_u64*)&desired;
//     return __sync_bool_compare_and_swap(a_ptr, *oldval_ptr, *newval_ptr);
// #endif
// }

void akl_atomic_exchange(volatile int* a, int* b) {
    *b = akl_sync_lock_test_and_set(a, *b);
}

int akl_fetch_and_store(volatile int* a, const int* newval) {
    return akl_sync_lock_test_and_set(a, *newval);
}
