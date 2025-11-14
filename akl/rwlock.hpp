#pragma once

#include <cstdlib>
#include <pthread.h>
#include <semaphore.h>
#include <sched.h>
#include <signal.h>
#include <sys/time.h>
#include <vector>
#include <list>
#include <queue>
#include <iostream>

#include "macro.hpp"
#include "mutex.hpp"

namespace akl {
    /**
       * \class rwlock
       * Wrapper around pthread's rwlock
       *
       * Before you use, see \ref parallel_object_intricacies.
       */
    class rwlock {
    private:
        mutable pthread_rwlock_t m_rwlock;

    public:
        rwlock() {
            int error = pthread_rwlock_init(&m_rwlock, NULL);
            ASSERT_TRUE(!error);
        }

        ~rwlock() {
            int error = pthread_rwlock_destroy(&m_rwlock);
            ASSERT_TRUE(!error);
        }

        // not copyable
        void operator=(const rwlock &m) {
        }

        /**
         * \todo: Remove!
         *
         * Copy constructor which does not copy. Do not use!  Required for
         * compatibility with some STL implementations (LLVM).  which use
         * the copy constructor for vector resize, rather than the
         * standard constructor.  */
        rwlock(const rwlock &) {
            int error = pthread_rwlock_init(&m_rwlock, NULL);
            ASSERT_TRUE(!error);
        }

        inline void readlock() const {
            pthread_rwlock_rdlock(&m_rwlock);
            //ASSERT_TRUE(!error);
        }

        inline void writelock() const {
            pthread_rwlock_wrlock(&m_rwlock);
            //ASSERT_TRUE(!error);
        }

        inline bool try_readlock() const {
            return pthread_rwlock_tryrdlock(&m_rwlock) == 0;
        }

        inline bool try_writelock() const {
            return pthread_rwlock_trywrlock(&m_rwlock) == 0;
        }

        inline void unlock() const {
            pthread_rwlock_unlock(&m_rwlock);
            //ASSERT_TRUE(!error);
        }

        inline void rdunlock() const {
            unlock();
        }

        inline void wrunlock() const {
            unlock();
        }
    }; // End rwlock
}
