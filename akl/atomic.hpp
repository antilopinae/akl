#pragma once

#include "__sync_definitions.hpp"
#include "atomic_ops.hpp"

namespace akl {
#ifdef __KERNEL__

#define volatile

#endif

namespace details {
template <typename T, bool IsIntegral>
class atomic_impl {};

template <typename T>
class atomic_impl<T, true> {
public:
    //! The current value of the atomic number
    volatile T value;

    //! Creates an atomic number with value "value"
    atomic_impl(const T& value = T())
        : value(value) {}

    //! Performs an atomic increment by 1, returning the new value
    T inc() {
        return add_and_fetch(&value, 1);
    }

    //! Performs an atomic decrement by 1, returning the new value
    T dec() {
        return sub_and_fetch(&value, 1);
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
        return add_and_fetch(&value, val);
    }

    //! Performs an atomic decrement by 'val', returning the new value
    T dec(const T val) {
        return sub_and_fetch(&value, val);
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
        return fetch_and_add(&value, 1);
    }

    //! Performs an atomic decrement by 1, returning the old value
    T dec_ret_last() {
        return fetch_and_sub(&value, 1);
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
        return fetch_and_add(&value, val);
    }

    //! Performs an atomic decrement by 'val', returning the new value
    T dec_ret_last(const T val) {
        return fetch_and_sub(&value, val);
    }

    //! Performs an atomic exchange with 'val', returning the previous value
    T exchange(const T val) {
        return lock_test_and_set(&value, val);
    }
};

// specialization for floats and doubles
template <typename T>
class atomic_impl<T, false> {
public:
    //! The current value of the atomic number
    volatile T value;

    //! Creates an atomic number with value "value"
    atomic_impl(const T& value = T())
        : value(value) {}

    //! Performs an atomic increment by 1, returning the new value
    T inc() {
        return inc(1);
    }

    //! Performs an atomic decrement by 1, returning the new value
    T dec() {
        return dec(1);
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
        T prev_value;
        T new_value;
        do {
            prev_value = value;
            new_value = prev_value + val;
        } while (!atomic_compare_and_swap(value, prev_value, new_value));
        return new_value;
    }

    //! Performs an atomic decrement by 'val', returning the new value
    T dec(const T val) {
        T prev_value;
        T new_value;
        do {
            prev_value = value;
            new_value = prev_value - val;
        } while (!atomic_compare_and_swap(value, prev_value, new_value));
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
        return inc_ret_last(1);
    }

    //! Performs an atomic decrement by 1, returning the old value
    T dec_ret_last() {
        return dec_ret_last(1);
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
        T prev_value;
        T new_value;
        do {
            prev_value = value;
            new_value = prev_value + val;
        } while (!atomic_compare_and_swap(value, prev_value, new_value));
        return prev_value;
    }

    //! Performs an atomic decrement by 'val', returning the new value
    T dec_ret_last(const T val) {
        T prev_value;
        T new_value;
        do {
            prev_value = value;
            new_value = prev_value - val;
        } while (!atomic_compare_and_swap(value, prev_value, new_value));
        return prev_value;
    }

    //! Performs an atomic exchange with 'val', returning the previous value
    T exchange(const T val) {
        return lock_test_and_set(&value, val);
    }
};
}  // namespace details

template <typename T>
class atomic : public details::atomic_impl<T, /*boost::is_integral<T>::value*/ true> {
public:
    //! Creates an atomic number with value "value"
    atomic(const T& value = T())
        : details::atomic_impl<T, /*boost::is_integral<T>::value*/ true>(value) {}
};

#ifdef __KERNEL__

#undef volatile

#endif
}  // namespace akl