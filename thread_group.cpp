#include "thread_group.hpp"


namespace akl {
    void thread_group::invoke(boost::function<void (void)> spawn_function,
                              thread_group *group) {
        const char *retval = NULL;
        try {
            spawn_function();
        } catch (const char *c) {
            // signal the thread group to join this thread
            retval = c;
        }
        group->mut.lock();
        group->joinqueue.push(std::make_pair(pthread_self(), retval));
        group->cond.signal();
        group->mut.unlock();
    }


    void thread_group::launch(const boost::function<void (void)> &spawn_function) {
        // Create a thread object and launch it.
        // We do not need to keep a copy of the thread around
        thread local_thread(m_thread_counter++);
        mut.lock();
        threads_running++;
        mut.unlock();
        local_thread.launch(boost::bind(thread_group::invoke, spawn_function, this));
    }


    void thread_group::launch(const boost::function<void (void)> &spawn_function,
                              size_t cpu_id) {
        if (cpu_id == size_t(-1)) {
            launch(spawn_function);
            return;
        }
        // Create a thread object
        thread local_thread(m_thread_counter++);
        mut.lock();
        threads_running++;
        mut.unlock();
        local_thread.launch(boost::bind(thread_group::invoke, spawn_function, this),
                            cpu_id);
    } // end of launch

    void thread_group::join() {
        mut.lock();
        while (threads_running > 0) {
            // if no threads are joining. wait
            while (joinqueue.empty()) cond.wait(mut);
            // a thread is joining
            std::pair<pthread_t, const char *> joining_thread = joinqueue.front();
            joinqueue.pop();
            threads_running--;
            // Reset the thread counter after killing all threads
            if (threads_running == 0) m_thread_counter = 0;
            // unlock here since I might be in join for a little while
            mut.unlock();
            void *unusedstatus = NULL;
            pthread_join(joining_thread.first, &unusedstatus);
            // if there is a return value
            // throw it. It is safe to throw here since I have the mutex unlocked.
            if (joining_thread.second) {
                throw(joining_thread.second);
            }
            mut.lock();
        }
        mut.unlock();
    } // end of join
}
