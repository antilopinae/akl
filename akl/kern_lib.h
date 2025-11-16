#ifndef ANT_KERN_LIB_H
#define ANT_KERN_LIB_H

#include "types.h"

#ifdef __cplusplus
extern "C" {

#else
#endif

/* memory */

void akl_memset(void* dst, int c, unsigned int len);

void* akl_cmemcpy(void* dst, void* src, unsigned int len);

void* akl_cmemmove(void* dst, void* src, unsigned int len);

int akl_cmemcmp(void* p1, void* p2, unsigned int len);

void* akl_cmalloc(unsigned int size);

void* akl_crealloc(void* mem, unsigned int size);

void akl_cfree(void* mem);

/* atomic */

typedef struct {
    int counter;
} akl_atomic_t;

typedef struct {
    union {
        float f;
        akl_u32 u;
    };
} akl_atomic_float_t;

typedef struct {
    union {
        double d;
        akl_u64 u;
    };
} akl_atomic_double_t;

#ifndef __KERNEL_MODULE__
static_assert(sizeof(float) == sizeof(akl_u32));
static_assert(sizeof(double) == sizeof(akl_u64));
#endif

int akl_atomic_xchg(akl_atomic_t* v, int new_val);

akl_atomic_float_t akl_atomic_xchg_float(
    akl_atomic_float_t* v, akl_atomic_float_t new_val
);

akl_atomic_double_t akl_atomic_xchg_double(
    akl_atomic_double_t* v, akl_atomic_double_t new_val
);

int akl_atomic_cmpxchg(akl_atomic_t* v, int old_val, int new_val);

int akl_atomic_add_return(int add_val, akl_atomic_t* v);

int akl_atomic_sub_return(int sub_val, akl_atomic_t* v);

int akl_atomic_cmpxchg_float(
    akl_atomic_float_t* ptr, akl_atomic_float_t oldv, akl_atomic_float_t newv
);

int akl_atomic_cmpxchg_double(
    akl_atomic_double_t* ptr, akl_atomic_double_t oldv, akl_atomic_double_t newv
);

#ifdef __cplusplus
}
#endif

#endif
