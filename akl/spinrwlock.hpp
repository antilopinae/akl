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
  * \class spinrwlock
  * rwlock built around "spinning"
  * source adapted from http://locklessinc.com/articles/locks/
  * "Scalable Reader-Writer Synchronization for Shared-Memory Multiprocessors"
  * John Mellor-Crummey and Michael Scott
  */
    class spinrwlock {
        union rwticket {
            unsigned u;
            unsigned short us;
            __extension__ struct {
                unsigned char write;
                unsigned char read;
                unsigned char users;
            } s;
        };

        mutable bool writing;
        mutable volatile rwticket l;

    public:
        spinrwlock() {
            memset(const_cast<rwticket *>(&l), 0, sizeof(rwticket));
        }

        inline void writelock() const {
            unsigned me = atomic_xadd(&l.u, (1<<16));
            unsigned char val = (unsigned char) (me >> 16);

            while (val != l.s.write) asm volatile("pause\n": : :"memory");
            writing = true;
        }

        inline void wrunlock() const {
            rwticket t = *const_cast<rwticket *>(&l);

            t.s.write++;
            t.s.read++;

            *(volatile unsigned short *) (&l) = t.us;
            writing = false;
            __asm("mfence");
        }

        inline void readlock() const {
            unsigned me = atomic_xadd(&l.u, (1<<16));
            unsigned char val = (unsigned char) (me >> 16);

            while (val != l.s.read) asm volatile("pause\n": : :"memory");
            l.s.read++;
        }

        inline void rdunlock() const {
            atomic_inc(&l.s.write);
        }

        inline void unlock() const {
            if (!writing) rdunlock();
            else wrunlock();
        }
    };
}
