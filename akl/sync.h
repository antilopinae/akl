#pragma once

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

#ifdef __cplusplus
}

#endif