#pragma once

namespace akl {

template <typename T>
T add_and_fetch(volatile T* t, T u) {
#ifdef __KERNEL__
    return *t;
#else
    return ::__sync_add_and_fetch(t, u);
#endif
}

template <typename T>
T fetch_and_add(volatile T* t, T u) {
#ifdef __KERNEL__
    return *t;
#else
    return ::__sync_fetch_and_add(t, u);
#endif
}

template <typename T>
T sub_and_fetch(volatile T* t, T u) {
#ifdef __KERNEL__
    return *t;
#else
    return ::__sync_sub_and_fetch(t, u);
#endif
}

template <typename T>
T fetch_and_sub(volatile T* t, T u) {
#ifdef __KERNEL__
    return *t;
#else
    return ::__sync_fetch_and_sub(t, u);
#endif
}

template <typename T>
T lock_test_and_set(volatile T* t, T u) {
#ifdef __KERNEL__
    return *t;
#else
    return ::__sync_lock_test_and_set(t, u);
#endif
}

template <typename T>
bool bool_compare_and_swap(T* t, T u, T h) {
#ifdef __KERNEL__
    return true;
#else
    return ::__sync_bool_compare_and_swap(t, u, h);
#endif
}

template <typename T>
bool bool_compare_and_swap(volatile T* t, T u, T h) {
#ifdef __KERNEL__
    return true;
#else
    return ::__sync_bool_compare_and_swap(t, u, h);
#endif
}

}  // namespace akl