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
   * \ingroup util
   * Wrapper around pthread's condition variable
   *
   * Before you use, see \ref parallel_object_intricacies.
   */
    class conditional {
    private:
        mutable pthread_cond_t m_cond;

    public:
        conditional() {
            int error = pthread_cond_init(&m_cond, NULL);
            ASSERT_TRUE(!error);
        }

        /** Copy constructor which does not copy. Do not use!
            Required for compatibility with some STL implementations (LLVM).
            which use the copy constructor for vector resize,
            rather than the standard constructor.    */
        conditional(const conditional &) {
            int error = pthread_cond_init(&m_cond, NULL);
            ASSERT_TRUE(!error);
        }

        // not copyable
        void operator=(const conditional &m) {
        }


        /// Waits on condition. The mutex must already be acquired. Caller
        /// must be careful about spurious wakes.
        inline void wait(const mutex &mut) const {
            int error = pthread_cond_wait(&m_cond, &mut.m_mut);
            ASSERT_TRUE(!error);
        }

        /// Like wait() but with a time limit of "sec" seconds
        inline int timedwait(const mutex &mut, size_t sec) const {
            struct timespec timeout;
            struct timeval tv;
            struct timezone tz;
            gettimeofday(&tv, &tz);
            timeout.tv_nsec = tv.tv_usec * 1000;
            timeout.tv_sec = tv.tv_sec + (time_t) sec;
            return pthread_cond_timedwait(&m_cond, &mut.m_mut, &timeout);
        }

        /// Like wait() but with a time limit of "ms" milliseconds
        inline int timedwait_ms(const mutex &mut, size_t ms) const {
            struct timespec timeout;
            struct timeval tv;
            gettimeofday(&tv, NULL);
            // convert ms to s and ns
            size_t s = ms / 1000;
            ms = ms % 1000;
            size_t ns = ms * 1000000;
            // convert timeval to timespec
            timeout.tv_nsec = tv.tv_usec * 1000;
            timeout.tv_sec = tv.tv_sec;

            // add the time
            timeout.tv_nsec += (suseconds_t) ns;
            timeout.tv_sec += (time_t) s;
            // shift the nsec to sec if overflow
            if (timeout.tv_nsec > 1000000000) {
                timeout.tv_sec++;
                timeout.tv_nsec -= 1000000000;
            }
            return pthread_cond_timedwait(&m_cond, &mut.m_mut, &timeout);
        }

        /// Like wait() but with a time limit of "ns" nanoseconds
        inline int timedwait_ns(const mutex &mut, size_t ns) const {
            struct timespec timeout;
            struct timeval tv;
            gettimeofday(&tv, NULL);
            assert(ns > 0);
            // convert ns to s and ns
            size_t s = ns / 1000000;
            ns = ns % 1000000;

            // convert timeval to timespec
            timeout.tv_nsec = tv.tv_usec * 1000;
            timeout.tv_sec = tv.tv_sec;

            // add the time
            timeout.tv_nsec += (suseconds_t) ns;
            timeout.tv_sec += (time_t) s;
            // shift the nsec to sec if overflow
            if (timeout.tv_nsec > 1000000000) {
                timeout.tv_sec++;
                timeout.tv_nsec -= 1000000000;
            }
            return pthread_cond_timedwait(&m_cond, &mut.m_mut, &timeout);
        }

        /// Signals one waiting thread to wake up
        inline void signal() const {
            int error = pthread_cond_signal(&m_cond);
            ASSERT_TRUE(!error);
        }

        /// Wakes up all waiting threads
        inline void broadcast() const {
            int error = pthread_cond_broadcast(&m_cond);
            ASSERT_TRUE(!error);
        }

        ~conditional() {
            int error = pthread_cond_destroy(&m_cond);
            ASSERT_TRUE(!error);
        }
    }; // End conditional
}
