#ifndef AKL_PTHREAD_H
#define AKL_PTHREAD_H

#include "types.h"

typedef struct {
    char data_[5];
} akl_pthread_mutex_t;

int akl_pthread_mutex_init(akl_pthread_mutex_t* mutex, void*);

int akl_pthread_mutex_destroy(akl_pthread_mutex_t* mutex);

int akl_pthread_mutex_lock(akl_pthread_mutex_t* mutex);

int akl_pthread_mutex_unlock(akl_pthread_mutex_t* mutex);

int akl_pthread_mutex_trylock(akl_pthread_mutex_t* mutex);

#endif
