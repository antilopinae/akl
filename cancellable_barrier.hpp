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
     * This is a simple sense-reversing barrier implementation.
     * In addition to standard barrier functionality, this also
     * provides a "cancel" function which can be used to destroy
     * the barrier, releasing all threads stuck in the barrier.
     *
     * Before you use, see \ref parallel_object_intricacies.
     */
    class cancellable_barrier {
    private:
        graphlab::mutex mutex;
        graphlab::conditional conditional;
        mutable int needed;
        mutable int called;

        mutable bool barrier_sense;
        mutable bool barrier_release;
        bool alive;

        // not copyconstructible
        cancellable_barrier(const cancellable_barrier &) {
        }

    public:
        /// Construct a barrier which will only fall when numthreads enter
        cancellable_barrier(size_t numthreads) {
            needed = numthreads;
            called = 0;
            barrier_sense = false;
            barrier_release = true;
            alive = true;
        }

        // not copyable
        void operator=(const cancellable_barrier &m) {
        }

        void resize_unsafe(size_t numthreads) {
            needed = numthreads;
        }

        /**
         * \warning: This barrier is safely NOT reusable with this cancel
         * definition
         */
        inline void cancel() {
            alive = false;
            conditional.broadcast();
        }

        /// Wait on the barrier until numthreads has called wait
        inline void wait() const {
            if (!alive) return;
            mutex.lock();
            // set waiting;
            called++;
            bool listening_on = barrier_sense;
            if (called == needed) {
                // if I have reached the required limit, wait up. Set waiting
                // to 0 to make sure everyone wakes up
                called = 0;
                barrier_release = barrier_sense;
                barrier_sense = !barrier_sense;
                // clear all waiting
                conditional.broadcast();
            } else {
                // while no one has broadcasted, sleep
                while (barrier_release != listening_on && alive) conditional.wait(mutex);
            }
            mutex.unlock();
        }
    }; // end of conditional
}
