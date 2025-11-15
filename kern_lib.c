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