#ifndef AKL_TYPES_H
#define AKL_TYPES_H

#ifndef __KERNEL_MODULE__
#include <assert.h>
#endif

#ifdef __KERNEL_MODULE__
#define ASSERT_TRUE(expr)                                                              \
if (!(expr)) {                                                                     \
}
#else
#define ASSERT_TRUE(expr) assert(expr)
#endif

#ifdef __cplusplus
#include <cstdint>

#else
#ifdef __KERNEL_MODULE__
#include <linux/types.h>

#else
#include <stdint.h>

#endif
#endif

typedef uint32_t akl_u32;
typedef uint64_t akl_u64;

#endif
