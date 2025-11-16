#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/atomic.h>

#include "akl/kern_lib.h"

/* memory */

void akl_cmemset(void *dst, int c, unsigned int len)
{
    memset(dst, c, len);
}

void *akl_cmemcpy(void *dst, void *src, unsigned int len)
{
    memcpy(dst, src, len);
    return dst;
}

void *akl_cmemmove(void *dst, void *src, unsigned int len)
{
    memmove(dst, src, len);
    return dst;
}

int akl_cmemcmp(void *p1, void *p2, unsigned int len)
{
    return memcmp(p1, p2, len);
}

void *akl_cmalloc(unsigned int size)
{
    return kmalloc(size, GFP_ATOMIC);
}

void *akl_crealloc(void *mem, unsigned int size)
{
    return krealloc(mem, size, GFP_ATOMIC);
}

void akl_cfree(void *mem)
{
    kfree(mem);
}

/* atomic */

int akl_atomic_xchg(akl_atomic_t* v, int new_val)
{
    return atomic_xchg((atomic_t *)v, new_val);
}

akl_atomic_float_t akl_atomic_xchg_float(akl_atomic_float_t* v, akl_atomic_float_t new_val)
{
    akl_u32* p = (akl_u32*)v;
	akl_u32 res = atomic_xchg((atomic_t *)p, new_val.u);
    return *(akl_atomic_float_t*)&res;
}

akl_atomic_double_t akl_atomic_xchg_double(akl_atomic_double_t* v, akl_atomic_double_t new_val)
{
    akl_u64* p = (akl_u64*)v;
	akl_u64 res = atomic64_xchg((atomic64_t *)p, new_val.u);
    return *(akl_atomic_double_t*)&res;
}

int akl_atomic_cmpxchg(akl_atomic_t *v, int old_val, int new_val)
{
    return atomic_cmpxchg((atomic_t *)v, old_val, new_val);
}

int akl_atomic_add_return(int add_val, akl_atomic_t* v)
{
    return atomic_add_return(add_val, (atomic_t *)v);
}

int akl_atomic_sub_return(int sub_val, akl_atomic_t* v)
{
    return atomic_sub_return(sub_val, (atomic_t *)v);
}

int akl_atomic_cmpxchg_float(akl_atomic_float_t* ptr, akl_atomic_float_t oldv, akl_atomic_float_t newv) {
    akl_u32* p = (akl_u32*)ptr;
    akl_u32 prev = atomic_cmpxchg((atomic_t*)p, oldv.u, newv.u);
    return prev == oldv.u;
}

int akl_atomic_cmpxchg_double(akl_atomic_double_t* ptr, akl_atomic_double_t oldv, akl_atomic_double_t newv) {
    akl_u64* p = (akl_u64*)ptr;
    akl_u64 prev = atomic64_cmpxchg((atomic64_t*)p, oldv.u, newv.u);
    return prev == oldv.u;
}
