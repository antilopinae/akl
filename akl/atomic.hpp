#pragma once

#include "double.h"
#include "float.h"
#include "kern_lib.h"
#include "sync.h"

namespace akl {

namespace details {

/* int types @only@ */
template <typename T>
class atomic_impl {
public:
    //! The current value of the atomic number
    volatile T value;

#ifndef __KERNEL__
    static_assert(sizeof(T) == sizeof(int));
#endif

    //! Creates an atomic number with value "value"
    atomic_impl(const T* value)
        : value(*value) {}

    atomic_impl(T value)
        : value(value) {}

    //! Performs an atomic increment by 1, returning the new value
    T inc() {
        int res = akl_sync_add_and_fetch((volatile int*)&value, 1);
        return *(T*)&res;
    }

    //! Performs an atomic decrement by 1, returning the new value
    T dec() {
        int res = akl_sync_sub_and_fetch((volatile int*)&value, 1);
        return *(T*)&res;
    }

    //! Lvalue implicit cast
    operator T() const {
        return value;
    }

    //! Performs an atomic increment by 1, returning the new value
    T operator++() {
        return inc();
    }

    //! Performs an atomic decrement by 1, returning the new value
    T operator--() {
        return dec();
    }

    //! Performs an atomic increment by 'val', returning the new value
    T inc(const T val) {
        int res = akl_sync_add_and_fetch((volatile int*)&value, *(int*)&val);
        return *(T*)&res;
    }

    //! Performs an atomic decrement by 'val', returning the new value
    T dec(const T val) {
        int res = akl_sync_sub_and_fetch((volatile int*)&value, *(int*)&val);
        return *(T*)&res;
    }

    //! Performs an atomic increment by 'val', returning the new value
    T operator+=(const T val) {
        return inc(val);
    }

    //! Performs an atomic decrement by 'val', returning the new value
    T operator-=(const T val) {
        return dec(val);
    }

    //! Performs an atomic increment by 1, returning the old value
    T inc_ret_last() {
        int res = akl_sync_fetch_and_add((volatile int*)&value, 1);
        return *(T*)&res;
    }

    //! Performs an atomic decrement by 1, returning the old value
    T dec_ret_last() {
        int res = akl_sync_fetch_and_sub((volatile int*)&value, 1);
        return *(T*)&res;
    }

    //! Performs an atomic increment by 1, returning the old value
    T operator++(int) {
        return inc_ret_last();
    }

    //! Performs an atomic decrement by 1, returning the old value
    T operator--(int) {
        return dec_ret_last();
    }

    //! Performs an atomic increment by 'val', returning the old value
    T inc_ret_last(const T val) {
        int res = akl_sync_fetch_and_add((volatile int*)&value, *(int*)&val);
        return *(T*)&res;
    }

    //! Performs an atomic decrement by 'val', returning the new value
    T dec_ret_last(const T val) {
        int res = akl_sync_fetch_and_sub((volatile int*)&value, *(int*)&val);
        return *(T*)&res;
    }

    //! Performs an atomic exchange with 'val', returning the previous value
    T exchange(const T val) {
        int res = akl_sync_lock_test_and_set((volatile int*)&value, *(int*)&val);
        return *(T*)&res;
    }
};

/* atomic for int */
template <>
class atomic_impl<int> {
public:
    using T = int;

    //! The current value of the atomic number
    volatile T value;

    //! Creates an atomic number with value "value"
    atomic_impl(const T* value)
        : value(*value) {}

    atomic_impl(T value)
        : value(value) {}

    //! Performs an atomic increment by 1, returning the new value
    T inc() {
        return akl_sync_add_and_fetch(&value, 1);
    }

    //! Performs an atomic decrement by 1, returning the new value
    T dec() {
        return akl_sync_sub_and_fetch(&value, 1);
    }

    //! Lvalue implicit cast
    operator T() const {
        return value;
    }

    //! Performs an atomic increment by 1, returning the new value
    T operator++() {
        return inc();
    }

    //! Performs an atomic decrement by 1, returning the new value
    T operator--() {
        return dec();
    }

    //! Performs an atomic increment by 'val', returning the new value
    T inc(const T val) {
        return akl_sync_add_and_fetch(&value, val);
    }

    //! Performs an atomic decrement by 'val', returning the new value
    T dec(const T val) {
        return akl_sync_sub_and_fetch(&value, val);
    }

    //! Performs an atomic increment by 'val', returning the new value
    T operator+=(const T val) {
        return inc(val);
    }

    //! Performs an atomic decrement by 'val', returning the new value
    T operator-=(const T val) {
        return dec(val);
    }

    //! Performs an atomic increment by 1, returning the old value
    T inc_ret_last() {
        return akl_sync_fetch_and_add(&value, 1);
    }

    //! Performs an atomic decrement by 1, returning the old value
    T dec_ret_last() {
        return akl_sync_fetch_and_sub(&value, 1);
    }

    //! Performs an atomic increment by 1, returning the old value
    T operator++(int) {
        return inc_ret_last();
    }

    //! Performs an atomic decrement by 1, returning the old value
    T operator--(int) {
        return dec_ret_last();
    }

    //! Performs an atomic increment by 'val', returning the old value
    T inc_ret_last(const T val) {
        return akl_sync_fetch_and_add(&value, val);
    }

    //! Performs an atomic decrement by 'val', returning the new value
    T dec_ret_last(const T val) {
        return akl_sync_fetch_and_sub(&value, val);
    }

    //! Performs an atomic exchange with 'val', returning the previous value
    T exchange(const T val) {
        return akl_sync_lock_test_and_set(&value, val);
    }
};

/* atomic for floats */
template <>
class atomic_impl<akl_atomic_float_t> {
public:
    using T = akl_atomic_float_t;

    //! The current value of the atomic number
    volatile T value;

    //! Creates an atomic number with value "value"
    atomic_impl(const T* value)
        : value(*value) {}

    atomic_impl(const T value)
        : value(value) {}

    //! Performs an atomic increment by 1, returning the new value
    T inc() {
        return inc({.f = 1});
    }

    //! Performs an atomic decrement by 1, returning the new value
    T dec() {
        return dec({.f = 1});
    }

    //! Lvalue implicit cast
    operator T() const {
        return *const_cast<const T*>(&value);
    }

    //! Performs an atomic increment by 1, returning the new value
    T operator++() {
        return inc();
    }

    //! Performs an atomic decrement by 1, returning the new value
    T operator--() {
        return dec();
    }

    //! Performs an atomic increment by 'val', returning the new value
    T inc(const T val) {
        T prev_value = {};
        T new_value = {};
        do {
            prev_value.f = value.f;
            new_value.u = akl_f32_add_bits(prev_value.u, val.u);
        } while (!akl_atomic_compare_and_swap_float(&value, prev_value, new_value));
        return new_value;
    }

    //! Performs an atomic decrement by 'val', returning the new value
    T dec(const T val) {
        T prev_value = {};
        T new_value = {};
        do {
            prev_value.f = value.f;
            new_value.u = akl_f32_sub_bits(prev_value.u, val.u);
        } while (!akl_atomic_compare_and_swap_float(&value, prev_value, new_value));
        return new_value;
    }

    //! Performs an atomic increment by 'val', returning the new value
    T operator+=(const T val) {
        return inc(val);
    }

    //! Performs an atomic decrement by 'val', returning the new value
    T operator-=(const T val) {
        return dec(val);
    }

    //! Performs an atomic increment by 1, returning the old value
    T inc_ret_last() {
        return inc_ret_last({.f = 1});
    }

    //! Performs an atomic decrement by 1, returning the old value
    T dec_ret_last() {
        return dec_ret_last({.f = 1});
    }

    //! Performs an atomic increment by 1, returning the old value
    T operator++(int) {
        return inc_ret_last();
    }

    //! Performs an atomic decrement by 1, returning the old value
    T operator--(int) {
        return dec_ret_last();
    }

    //! Performs an atomic increment by 'val', returning the old value
    T inc_ret_last(const T val) {
        T prev_value = {};
        T new_value = {};
        do {
            prev_value.f = value.f;
            new_value.u = akl_f32_add_bits(prev_value.u, val.u);
        } while (!akl_atomic_compare_and_swap_float(&value, prev_value, new_value));
        return prev_value;
    }

    //! Performs an atomic decrement by 'val', returning the new value
    T dec_ret_last(const T val) {
        T prev_value = {};
        T new_value = {};
        do {
            prev_value.f = value.f;
            new_value.u = akl_f32_sub_bits(prev_value.u, val.u);
        } while (!akl_atomic_compare_and_swap_float(&value, prev_value, new_value));
        return prev_value;
    }

    //! Performs an atomic exchange with 'val', returning the previous value
    T exchange(const T val) {
        return akl_sync_lock_test_and_set_float(&value, val);
    }
};

/* atomic for doubles */
template <>
class atomic_impl<akl_atomic_double_t> {
public:
    using T = akl_atomic_double_t;

    //! The current value of the atomic number
    volatile T value;

    //! Creates an atomic number with value "value"
    atomic_impl(const T* value)
        : value(*value) {}

    atomic_impl(const T value)
        : value(value) {}

    //! Performs an atomic increment by 1, returning the new value
    T inc() {
        return inc({.d = 1});
    }

    //! Performs an atomic decrement by 1, returning the new value
    T dec() {
        return dec({.d = 1});
    }

    //! Lvalue implicit cast
    operator T() const {
        return *const_cast<T*>(&value);
    }

    //! Performs an atomic increment by 1, returning the new value
    T operator++() {
        return inc();
    }

    //! Performs an atomic decrement by 1, returning the new value
    T operator--() {
        return dec();
    }

    //! Performs an atomic increment by 'val', returning the new value
    T inc(const T val) {
        T prev_value = {};
        T new_value = {};
        do {
            prev_value.d = value.d;
            new_value.u = akl_d64_sub_bits(prev_value.u, val.u);
        } while (!akl_atomic_compare_and_swap_double(&value, prev_value, new_value));
        return new_value;
    }

    //! Performs an atomic decrement by 'val', returning the new value
    T dec(const T val) {
        T prev_value = {};
        T new_value = {};
        do {
            prev_value.d = value.d;
            new_value.u = akl_d64_sub_bits(prev_value.u, val.u);
        } while (!akl_atomic_compare_and_swap_double(&value, prev_value, new_value));
        return new_value;
    }

    //! Performs an atomic increment by 'val', returning the new value
    T operator+=(const T val) {
        return inc(val);
    }

    //! Performs an atomic decrement by 'val', returning the new value
    T operator-=(const T val) {
        return dec(val);
    }

    //! Performs an atomic increment by 1, returning the old value
    T inc_ret_last() {
        return inc_ret_last({.d = 1});
    }

    //! Performs an atomic decrement by 1, returning the old value
    T dec_ret_last() {
        return dec_ret_last({.d = 1});
    }

    //! Performs an atomic increment by 1, returning the old value
    T operator++(int) {
        return inc_ret_last();
    }

    //! Performs an atomic decrement by 1, returning the old value
    T operator--(int) {
        return dec_ret_last();
    }

    //! Performs an atomic increment by 'val', returning the old value
    T inc_ret_last(const T val) {
        T prev_value = {};
        T new_value = {};
        do {
            prev_value.d = value.d;
            new_value.u = akl_d64_add_bits(prev_value.u, val.u);
        } while (!akl_atomic_compare_and_swap_double(&value, prev_value, new_value));
        return prev_value;
    }

    //! Performs an atomic decrement by 'val', returning the new value
    T dec_ret_last(const T val) {
        T prev_value = {};
        T new_value = {};
        do {
            prev_value.d = value.d;
            new_value.u = akl_d64_sub_bits(prev_value.u, val.u);
        } while (!akl_atomic_compare_and_swap_double(&value, prev_value, new_value));
        return prev_value;
    }

    //! Performs an atomic exchange with 'val', returning the previous value
    T exchange(const T val) {
        return akl_sync_lock_test_and_set_double(&value, val);
    }
};

}  // namespace details

template <typename T>
class atomic : public details::atomic_impl<T> {
public:
    atomic(const T* value)
        : details::atomic_impl<T>(value) {}

    atomic(T value)
        : details::atomic_impl<T>(value) {}
};

using atomic_int_ = atomic<int>;

using atomic_float_ = atomic<akl_atomic_float_t>;

using atomic_double_ = atomic<akl_atomic_double_t>;

}  // namespace akl