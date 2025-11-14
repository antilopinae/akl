#pragma once

namespace akl {

template <typename T, typename U>
T add_and_fetch(T& t, U& u) {
#ifdef __KERNEL__
    return t;
#else
    return ::__sync_add_and_fetch(t, u);
#endif
}

template <typename T, typename U>
T fetch_and_add(T& t, U& u) {
#ifdef __KERNEL__
    return t;
#else
    return ::__sync_fetch_and_add(t, u);
#endif
}

template <typename T, typename U>
T sub_and_fetch(T& t, U& u) {
#ifdef __KERNEL__
    return t;
#else
    return ::__sync_sub_and_fetch(t, u);
#endif
}

template <typename T, typename U>
T fetch_and_sub(T& t, U& u) {
#ifdef __KERNEL__
    return t;
#else
    return ::__sync_fetch_and_sub(t, u);
#endif
}

template <typename T, typename U>
T lock_test_and_set(T t, U u) {
#ifdef __KERNEL__
    return t;
#else
    return ::__sync_lock_test_and_set(t, u);
#endif
}

template <typename T, typename U, typename H>
bool bool_compare_and_swap(T& t, U& u, H& h) {
#ifdef __KERNEL__
    return true;
#else
    return ::__sync_bool_compare_and_swap(t, u, h);
#endif
}

}  // namespace akl