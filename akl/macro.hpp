#pragma once

#if 0

#include "atomic_ops.hpp"

#define ASSERT_TRUE(expr)                                                              \
    do {                                                                               \
        if (!(expr)) {                                                                 \
            std::abort();                                                              \
        }                                                                              \
    } while (0)


#define cpu_relax() asm volatile("pause\n" : : : "memory")

#define RW_WAIT_BIT 0
#define RW_WRITE_BIT 1
#define RW_READ_BIT 2

#define RW_WAIT 1
#define RW_WRITE 2
#define RW_READ 4

inline void prefetch_range(void* addr, size_t len) {
    char* cp;
    char* end = (char*)(addr) + len;

    for (cp = (char*)(addr); cp < end; cp += 64) {
        __builtin_prefetch(cp, 0);
    }
}

inline void prefetch_range_write(void* addr, size_t len) {
    char* cp;
    char* end = (char*)(addr) + len;

    for (cp = (char*)(addr); cp < end; cp += 64) {
        __builtin_prefetch(cp, 1);
    }
}

#define QUEUED_RW_LOCK_REQUEST_READ 0
#define QUEUED_RW_LOCK_REQUEST_WRITE 1
#define QUEUED_RW_LOCK_REQUEST_NONE 2

#define atomic_xadd(P, V) __sync_fetch_and_add((P), (V))
#define cmpxchg(P, O, N) __sync_val_compare_and_swap((P), (O), (N))
#define atomic_inc(P) __sync_add_and_fetch((P), 1)
#define atomic_add(P, V) __sync_add_and_fetch((P), (V))
#define atomic_set_bit(P, V) __sync_or_and_fetch((P), 1 << (V))

namespace akl {

template<typename T>
inline void force_store(T &x, const T &value) {
    *(volatile T *) &x = value;
}

}

#endif