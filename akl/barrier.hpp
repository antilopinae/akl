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
#include "cancellable_barrier.hpp"

namespace akl {
    /**
      * \class barrier
      * Wrapper around pthread's barrier
      *
      * Before you use, see \ref parallel_object_intricacies.
      */
#ifdef __linux__
    /**
     * \ingroup util
     * Wrapper around pthread's barrier
     */
    class barrier {
    private:
        mutable pthread_barrier_t m_barrier;
        // not copyconstructable
        barrier(const barrier &) {
        }

    public:
        /// Construct a barrier which will only fall when numthreads enter
        barrier(size_t numthreads) {
            pthread_barrier_init(&m_barrier, NULL, (unsigned) numthreads);
        }

        // not copyable
        void operator=(const barrier &m) {
        }

        void resize_unsafe(size_t numthreads) {
            pthread_barrier_destroy(&m_barrier);
            pthread_barrier_init(&m_barrier, NULL, (unsigned) numthreads);
        }

        ~barrier() { pthread_barrier_destroy(&m_barrier); }
        /// Wait on the barrier until numthreads has called wait
        inline void wait() const { pthread_barrier_wait(&m_barrier); }
    };

#else
    /* In some systems, pthread_barrier is not available.
    */
    typedef cancellable_barrier barrier;
#endif
}
