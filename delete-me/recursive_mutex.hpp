#pragma once

#include <pthread.h>
#include <cstdlib>

#include "macro.hpp"

namespace akl {
    /**
     * \ingroup util
     *
     * Simple wrapper around pthread's recursive mutex.
     * Before you use, see \ref parallel_object_intricacies.
     */
    class recursive_mutex {
    public:
        // mutable not actually needed
        mutable pthread_mutex_t m_mut;
        /// constructs a mutex
        recursive_mutex() {
            pthread_mutexattr_t attr;
            int error = pthread_mutexattr_init(&attr);
            ASSERT_TRUE(!error);
            error = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
            ASSERT_TRUE(!error);
            error = pthread_mutex_init(&m_mut, &attr);
            ASSERT_TRUE(!error);
            pthread_mutexattr_destroy(&attr);
        }

        /** Copy constructor which does not copy. Do not use!
            Required for compatibility with some STL implementations (LLVM).
            which use the copy constructor for vector resize,
            rather than the standard constructor.    */
        recursive_mutex(const recursive_mutex &) {
            pthread_mutexattr_t attr;
            int error = pthread_mutexattr_init(&attr);
            ASSERT_TRUE(!error);
            error = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
            ASSERT_TRUE(!error);
            error = pthread_mutex_init(&m_mut, &attr);
            ASSERT_TRUE(!error);
            pthread_mutexattr_destroy(&attr);
        }

        ~recursive_mutex() {
            int error = pthread_mutex_destroy(&m_mut);
            ASSERT_TRUE(!error);
        }

        // not copyable
        void operator=(const recursive_mutex &m) {
        }

        /// Acquires a lock on the mutex
        inline void lock() const {
            int error = pthread_mutex_lock(&m_mut);
            ASSERT_TRUE(!error);
        }

        /// Releases a lock on the mutex
        inline void unlock() const {
            int error = pthread_mutex_unlock(&m_mut);
            ASSERT_TRUE(!error);
        }

        /// Non-blocking attempt to acquire a lock on the mutex
        inline bool try_lock() const {
            return pthread_mutex_trylock(&m_mut) == 0;
        }

        friend class conditional;
    }; // End of Mutex
}
