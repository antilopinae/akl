#ifndef ANT_FLOAT_H
#define ANT_FLOAT_H

#ifdef __KERNEL__
#include <linux/types.h>
#else
#include <stdint.h>
typedef uint32_t u32;
typedef uint64_t u64;
#endif

static inline u32 f32_add_bits(u32 a_bits, u32 b_bits) {
    // I DONT KNOW HOW
    return a_bits + b_bits;
}

static inline u32 f32_sub_bits(u32 a_bits, u32 b_bits) {
    // i DONT KNOW HOW
    return a_bits - b_bits;
}

#endif