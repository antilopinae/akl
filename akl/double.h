#ifndef ANT_DOUBLE_H
#define ANT_DOUBLE_H

#ifdef __KERNEL__
#include <linux/types.h>
#else
#include <stdint.h>
typedef uint32_t u32;
typedef uint64_t u64;
#endif

static inline u64 akl_d64_add_bits(u64 a_bits, u64 b_bits) {
    // I DONT KNOW HOW
    return a_bits + b_bits;
}

static inline u64 akl_d64_sub_bits(u64 a_bits, u64 b_bits) {
    // i DONT KNOW HOW
    return a_bits - b_bits;
}

#endif
