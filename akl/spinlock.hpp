#pragma once

#if 0

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
#if _POSIX_SPIN_LOCKS >= 0
    /**
     * \ingroup util
     *
     * Wrapper around pthread's spinlock.
     *
     * Before you use, see \ref parallel_object_intricacies.
     */
    class spinlock {
    private:
        // mutable not actually needed
        mutable pthread_spinlock_t m_spin;

    public:
        /// constructs a spinlock
        spinlock() {
            int error = pthread_spin_init(&m_spin, PTHREAD_PROCESS_PRIVATE);
            ASSERT_TRUE(!error);
        }

        /** Copy constructor which does not copy. Do not use!
            Required for compatibility with some STL implementations (LLVM).
            which use the copy constructor for vector resize,
            rather than the standard constructor.    */
        spinlock(const spinlock &) {
            int error = pthread_spin_init(&m_spin, PTHREAD_PROCESS_PRIVATE);
            ASSERT_TRUE(!error);
        }

        // not copyable
        void operator=(const spinlock &m) {
        }


        /// Acquires a lock on the spinlock
        inline void lock() const {
            int error = pthread_spin_lock(&m_spin);
            ASSERT_TRUE(!error);
        }

        /// Releases a lock on the spinlock
        inline void unlock() const {
            int error = pthread_spin_unlock(&m_spin);
            ASSERT_TRUE(!error);
        }

        /// Non-blocking attempt to acquire a lock on the spinlock
        inline bool try_lock() const {
            return pthread_spin_trylock(&m_spin) == 0;
        }

        ~spinlock() {
            int error = pthread_spin_destroy(&m_spin);
            ASSERT_TRUE(!error);
        }

        friend class conditional;
    }; // End of spinlock
#define SPINLOCK_SUPPORTED 1
#else
    //! if spinlock not supported, it is typedef it to a mutex.
    typedef mutex spinlock;
#define SPINLOCK_SUPPORTED 0
#endif


    /**
   * \ingroup util
   *If pthread spinlock is not implemented,
   * this provides a simple alternate spin lock implementation.
   *
   * Before you use, see \ref parallel_object_intricacies.
   */
    class simple_spinlock {
    private:
        // mutable not actually needed
        mutable volatile char spinner;

    public:
        /// constructs a spinlock
        simple_spinlock() {
            spinner = 0;
        }

        /** Copy constructor which does not copy. Do not use!
        Required for compatibility with some STL implementations (LLVM).
        which use the copy constructor for vector resize,
        rather than the standard constructor.    */
        simple_spinlock(const simple_spinlock &) {
            spinner = 0;
        }

        // not copyable
        void operator=(const simple_spinlock &m) {
        }


        /// Acquires a lock on the spinlock
        inline void lock() const {
            while (spinner == 1 || __sync_lock_test_and_set(&spinner, 1));
        }

        /// Releases a lock on the spinlock
        inline void unlock() const {
            __sync_synchronize();
            spinner = 0;
        }

        /// Non-blocking attempt to acquire a lock on the spinlock
        inline bool try_lock() const {
            return (__sync_lock_test_and_set(&spinner, 1) == 0);
        }

        ~simple_spinlock() {
            ASSERT_TRUE(spinner == 0);
        }
    };


    /**
   * \ingroup util
   *If pthread spinlock is not implemented,
   * this provides a simple alternate spin lock implementation.
   *
   * Before you use, see \ref parallel_object_intricacies.
   */
    class padded_simple_spinlock {
    private:
        // mutable not actually needed
        mutable volatile char spinner;
        // char padding[63];
    public:
        /// constructs a spinlock
        padded_simple_spinlock() {
            spinner = 0;
        }

        /** Copy constructor which does not copy. Do not use!
        Required for compatibility with some STL implementations (LLVM).
        which use the copy constructor for vector resize,
        rather than the standard constructor.    */
        padded_simple_spinlock(const padded_simple_spinlock &) {
            spinner = 0;
        }

        // not copyable
        void operator=(const padded_simple_spinlock &m) {
        }


        /// Acquires a lock on the spinlock
        inline void lock() const {
            while (spinner == 1 || __sync_lock_test_and_set(&spinner, 1));
        }

        /// Releases a lock on the spinlock
        inline void unlock() const {
            __sync_synchronize();
            spinner = 0;
        }

        /// Non-blocking attempt to acquire a lock on the spinlock
        inline bool try_lock() const {
            return (__sync_lock_test_and_set(&spinner, 1) == 0);
        }

        ~padded_simple_spinlock() {
            ASSERT_TRUE(spinner == 0);
        }
    };
}

#endif