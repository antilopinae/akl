#pragma once

#include "pthread.h"

namespace akl {
/**
 * \ingroup util
 *
 * Simple wrapper around pthread's mutex.
 */
class mutex {
public:
    // mutable not actually needed
    mutable akl_pthread_mutex_t m_mut;

    /// constructs a mutex
    mutex() {
        int error = akl_pthread_mutex_init(&m_mut, NULL);
        ASSERT_TRUE(!error);
    }

    /** Copy constructor which does not copy. Do not use!
        Required for compatibility with some STL implementations (LLVM).
        which use the copy constructor for vector resize,
        rather than the standard constructor.    */
    mutex(const mutex&) {
        int error = akl_pthread_mutex_init(&m_mut, NULL);
        ASSERT_TRUE(!error);
    }

    ~mutex() {
        int error = akl_pthread_mutex_destroy(&m_mut);
        ASSERT_TRUE(!error);
    }

    // not copyable
    void operator=(const mutex& m) {}

    /// Acquires a lock on the mutex
    inline void lock() const {
        int error = akl_pthread_mutex_lock(&m_mut);
        ASSERT_TRUE(!error);
    }

    /// Releases a lock on the mutex
    inline void unlock() const {
        int error = akl_pthread_mutex_unlock(&m_mut);
        ASSERT_TRUE(!error);
    }

    /// Non-blocking attempt to acquire a lock on the mutex
    inline bool try_lock() const {
        return akl_pthread_mutex_trylock(&m_mut) == 0;
    }

    friend class conditional;
};
}  // namespace akl
