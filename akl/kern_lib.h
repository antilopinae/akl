#ifndef KERN_LIB_H
#define KERN_LIB_H

#ifdef __cplusplus
// #include <cstdarg>

extern "C" {
#else
// #include <stdarg.h>
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

int akl_atomic_xchg(akl_atomic_t* v, int new_val);

int akl_atomic_cmpxchg(akl_atomic_t* v, int old_val, int new_val);

int akl_atomic_add_return(int add_val, akl_atomic_t* v);

int akl_atomic_sub_return(int sub_val, akl_atomic_t* v);

#ifdef __cplusplus
}
#endif

#endif
