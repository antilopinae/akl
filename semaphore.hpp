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
#ifdef __APPLE__
    /**
     * Custom implementation of a semaphore.
     *
     * Before you use, see \ref parallel_object_intricacies.
     */
    class semaphore {
    private:
        conditional cond;
        mutex mut;
        mutable volatile size_t semvalue;
        mutable volatile size_t waitercount;

    public:
        semaphore() {
            semvalue = 0;
            waitercount = 0;
        }

        /** Copy constructor which does not copy. Do not use!
            Required for compatibility with some STL implementations (LLVM).
            which use the copy constructor for vector resize,
            rather than the standard constructor.    */
        semaphore(const semaphore &) {
            semvalue = 0;
            waitercount = 0;
        }

        // not copyable
        void operator=(const semaphore &m) {
        }

        inline void post() const {
            mut.lock();
            if (waitercount > 0) {
                cond.signal();
            }
            semvalue++;
            mut.unlock();
        }

        inline void wait() const {
            mut.lock();
            waitercount++;
            while (semvalue == 0) {
                cond.wait(mut);
            }
            waitercount--;
            semvalue--;
            mut.unlock();
        }

        ~semaphore() {
            ASSERT_TRUE(waitercount == 0);
            ASSERT_TRUE(semvalue == 0);
        }
    }; // End semaphore
#else
    /**
     * Wrapper around pthread's semaphore
     *
     * Before you use, see \ref parallel_object_intricacies.
     */
    class semaphore {
    private:
        mutable sem_t m_sem;

    public:
        semaphore() {
            int error = sem_init(&m_sem, 0, 0);
            ASSERT_TRUE(!error);
        }

        /** Copy constructor with does not copy. Do not use!
            Required for compatibility with some STL implementations (LLVM).
            which use the copy constructor for vector resize,
            rather than the standard constructor.    */
        semaphore(const semaphore &) {
            int error = sem_init(&m_sem, 0, 0);
            ASSERT_TRUE(!error);
        }

        // not copyable
        void operator=(const semaphore &m) {
        }

        inline void post() const {
            int error = sem_post(&m_sem);
            ASSERT_TRUE(!error);
        }

        inline void wait() const {
            int error = sem_wait(&m_sem);
            ASSERT_TRUE(!error);
        }

        ~semaphore() {
            int error = sem_destroy(&m_sem);
            ASSERT_TRUE(!error);
        }
    }; // End semaphore
#endif
}
