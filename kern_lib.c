#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/atomic.h>

#include "akl/kern_lib.h"

/* memory */

void kcmemset(void *dst, int c, unsigned int len)
{
    memset(dst, c, len);
}

void *kcmemcpy(void *dst, void *src, unsigned int len)
{
    memcpy(dst, src, len);
    return dst;
}

void *kcmemmove(void *dst, void *src, unsigned int len)
{
    memmove(dst, src, len);
    return dst;
}

int kcmemcmp(void *p1, void *p2, unsigned int len)
{
    return memcmp(p1, p2, len);
}

void *kcmalloc(unsigned int size)
{
    return kmalloc(size, GFP_ATOMIC);
}

void *kcrealloc(void *mem, unsigned int size)
{
    return krealloc(mem, size, GFP_ATOMIC);
}

void kcfree(void *mem)
{
    kfree(mem);
}

/* atomic */

int katomic_cmpxchg(katomic_t *v, int old_val, int new_val)
{
    return atomic_cmpxchg((atomic_t *)v, old_val, new_val);
}