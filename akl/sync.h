#pragma once

#include "kern_lib.h"

#ifdef __cplusplus
extern "C" {

#else
#endif

int akl_sync_bool_compare_and_swap(volatile int* t, int expected, int desired);

int akl_sync_add_and_fetch(volatile int* t, int val);

int akl_sync_fetch_and_add(volatile int* t, int val);

int akl_sync_sub_and_fetch(volatile int* t, int val);

int akl_sync_fetch_and_sub(volatile int* t, int val);

int akl_sync_lock_test_and_set(volatile int* t, int val);

akl_atomic_float_t akl_sync_lock_test_and_set_float(
    volatile akl_atomic_float_t* t, akl_atomic_float_t val
);

// akl_atomic_double_t akl_sync_lock_test_and_set_double(
//     volatile akl_atomic_double_t* t, akl_atomic_double_t val
// );

int akl_atomic_compare_and_swap(volatile int* t, int expected, int desired);

int akl_atomic_compare_and_swap_float(
    volatile akl_atomic_float_t* t,
    akl_atomic_float_t expected,
    akl_atomic_float_t desired
);

// int akl_atomic_compare_and_swap_double(
//     volatile akl_atomic_double_t* t,
//     akl_atomic_double_t expected,
//     akl_atomic_double_t desired
// );

void akl_atomic_exchange(volatile int* a, int* b);

int akl_fetch_and_store(volatile int* a, const int* newval);

#ifdef __cplusplus
}

#endif