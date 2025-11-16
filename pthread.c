#include "akl/pthread.h"

#ifdef __KERNEL_MODULE__
#include <linux/mutex.h>
#else
#include <assert.h>
#include <pthread.h>
#endif

int akl_pthread_mutex_init(akl_pthread_mutex_t* mutex, void*) {
#ifdef __KERNEL_MODULE__
    mutex_init((struct mutex*)&mutex);
    return 0;
#else
    return pthread_mutex_init((pthread_mutex_t*)mutex, NULL);
#endif
}

int akl_pthread_mutex_destroy(akl_pthread_mutex_t* mutex) {
#ifdef __KERNEL_MODULE__
    mutex_destroy((struct mutex*)&mutex);
    return 0;
#else
    return pthread_mutex_destroy((pthread_mutex_t*)&mutex);
#endif
}

int akl_pthread_mutex_lock(akl_pthread_mutex_t* mutex) {
#ifdef __KERNEL_MODULE__
    mutex_lock((struct mutex*)&mutex);
    return 0;
#else
    return pthread_mutex_lock((pthread_mutex_t*)&mutex);
#endif
}

int akl_pthread_mutex_unlock(akl_pthread_mutex_t* mutex) {
#ifdef __KERNEL_MODULE__
    mutex_unlock((struct mutex*)&mutex);
    return 0;
#else
    return pthread_mutex_unlock((pthread_mutex_t*)&mutex);
#endif
}

int akl_pthread_mutex_trylock(akl_pthread_mutex_t* mutex) {
#ifdef __KERNEL_MODULE__
    return mutex_trylock((struct mutex*)&mutex) ? 0 : 1;
#else
    return pthread_mutex_trylock((pthread_mutex_t*)&mutex);
#endif
}
