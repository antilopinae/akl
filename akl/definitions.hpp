#pragma once

#ifdef __KERNEL__
#warning("Building in kernel mode")

#include <linux/types.h>
#define uint64_t u64
#define size_t u64

#else
#warning("Building in userspace mode")

#define size_t unsigned long
#include <stdint.h>

#endif
