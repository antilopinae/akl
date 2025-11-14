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
    struct spinrwlock2 {
        mutable unsigned int l;

        spinrwlock2() : l(0) {
        }

        void writelock() const {
            while (1) {
                unsigned state = l;

                /* No readers or writers? */
                if (state < RW_WRITE) {
                    /* Turn off RW_WAIT, and turn on RW_WRITE */
                    if (cmpxchg(&l, state, RW_WRITE) == state) return;

                    /* Someone else got there... time to wait */
                    state = l;
                }

                /* Turn on writer wait bit */
                if (!(state & RW_WAIT))
                    atomic_set_bit(&l, RW_WAIT_BIT);

                /* Wait until can try to take the lock */
                while (l > RW_WAIT)
                    cpu_relax();
            }
        }

        void wrunlock() const {
            atomic_add(&l, -RW_WRITE);
        }

        void readlock() const {
            while (1) {
                /* A writer exists? */
                while (l & (RW_WAIT | RW_WRITE))
                    cpu_relax();

                /* Try to get read lock */
                if (!(atomic_xadd(&l, RW_READ) & (RW_WAIT | RW_WRITE))) return;

                /* Undo */
                atomic_add(&l, -RW_READ);
            }
        }

        void rdunlock() const {
            atomic_add(&l, -RW_READ);
        }
    };
}
