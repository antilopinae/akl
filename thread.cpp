#include "thread.hpp"

#include <stdlib.h>

namespace akl {
    void destroy_tls_data(void *ptr);

    struct thread_keys {
        pthread_key_t GRAPHLAB_TSD_ID;

        thread_keys() : GRAPHLAB_TSD_ID(0) {
            pthread_key_create(&GRAPHLAB_TSD_ID,
                               destroy_tls_data);
        }
    };

    // This function is to be called prior to any thread starting
    // execution to ensure that the static member keys is constructed
    // prior to any threads launching
    static pthread_key_t get_tsd_id() {
        static thread_keys keys;
        return keys.GRAPHLAB_TSD_ID;
    }

    // This forces get_tsd_id to be called prior to main.
    static pthread_key_t __unused_init_keys__(get_tsd_id());

    // the combination of the two mechanisms above will force the
    // thread local store to be initialized
    // 1: before main
    // 2: before any other global variables which spawn threads

    // END MAGIC =============================================================>

    // -----------------------------------------------------------------
    //                 Thread Object Static Members
    // -----------------------------------------------------------------


    /**
     * Create thread specific data
     */
    thread::tls_data *create_tls_data(size_t thread_id = 0) {
        // Require that the data not yet exist
        assert(pthread_getspecific(get_tsd_id()) == NULL);
        // Create the data
        thread::tls_data *data =
                new thread::tls_data(thread_id);
        assert(data != NULL);
        // Set the data
        pthread_setspecific(get_tsd_id(), data);
        // Return the associated tsd
        return data;
    } // end create the thread specific data

    /**
     * This function tries to get the thread specific data.  If no
     * thread specific data has been associated with the thread than it
     * is created.
     */
    thread::tls_data &thread::get_tls_data() {
        // get the tsd
        tls_data *tsd =
                reinterpret_cast<tls_data *>
                (pthread_getspecific(get_tsd_id()));
        // If no tsd be has been associated, create one
        if (tsd == NULL) tsd = create_tls_data();
        assert(tsd != NULL);
        return *tsd;
    } // end of get thread specific data


    /**
     * Create thread specific data
     */
    void destroy_tls_data(void *ptr) {
        thread::tls_data *tsd =
                reinterpret_cast<thread::tls_data *>(ptr);
        if (tsd != NULL) {
            delete tsd;
        }
    } // end destroy the thread specific data


    //! Little helper function used to launch threads
    void *thread::invoke(void *_args) {
        void *retval = NULL;
        thread::invoke_args *args = static_cast<thread::invoke_args *>(_args);
        // Create the graphlab thread specific data
        create_tls_data(args->m_thread_id);
        //! Run the users thread code
        try {
            args->spawn_routine();
        } catch (const char *msg) {
            retval = (void *) msg;
        }
        //! Delete the arguments
        delete args;

        //! Properly kill the thread
        thread_destroy_callback();
        return retval;
    } // end of invoke


    /**
     * This static method joins the invoking thread with the other
     * thread object.  This thread will not return from the join
     * routine until the other thread complets it run.
     */
    void thread::join(thread &other) {
        void *status = NULL;
        // joint the first element
        int error = 0;
        if (other.active()) {
            error = pthread_join(other.m_p_thread, &status);
            if (status != NULL) {
                const char *strstatus = (const char *) status;
                throw strstatus;
            }
        }
        if (error) {
            std::cout << "Major error in join" << std::endl;
            std::cout << "pthread_join() returned error " << error << std::endl;
            exit(EXIT_FAILURE);
        }
    } // end of join


    /**
     * Return the number processing units (individual cores) on this
     * system
     */
    size_t thread::cpu_count() {
#if defined __linux__
        char *jobsStr = getenv("GRAPHLAB_THREADS_PER_WORKER");
        if (jobsStr) {
            int nThreads = atoi(jobsStr);
            if (nThreads < 2) return 2;
            else return nThreads;
        } else {
            return sysconf(_SC_NPROCESSORS_CONF);
        }
#elif defined(__MACH__) && defined(_SC_NPROCESSORS_ONLN)
        return sysconf(_SC_NPROCESSORS_ONLN);
#elif defined(__MACH__) && defined(HW_NCPU)
        int ncpus = 1;
        size_t len = sizeof(ncpus);
        sysctl((int[2]){CTL_HW, HW_NCPU}, 2, &ncpus, &len, NULL, 0);
        return ncpus;
#else
        return 0;
#endif
    } // end of cpu count

    /**
      * Allow defining a callback when thread is destroyed.
      * This is needed at least from Java JNI, where we have to detach
      * thread from JVM before it dies.
      */
    void (*__thr_callback)() = NULL;

    void thread::thread_destroy_callback() {
        if (__thr_callback != NULL) __thr_callback();
    }

    void thread::set_thread_destroy_callback(void (*callback)()) {
        __thr_callback = callback;
    }

    void thread::launch(const boost::function<void (void)> &spawn_routine) {
        get_tsd_id();
        ASSERT_FALSE(thread_started);
        // fill in the thread attributes
        pthread_attr_t attr;
        int error = 0;
        error = pthread_attr_init(&attr);
        ASSERT_TRUE(!error);
        error = pthread_attr_setstacksize(&attr, m_stack_size);
        ASSERT_TRUE(!error);
        error = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
        ASSERT_TRUE(!error);
        error =
                pthread_create(&m_p_thread,
                               &attr,
                               invoke,
                               static_cast<void *>(new invoke_args(m_thread_id,
                                                                   spawn_routine)));
        thread_started = true;
        if (error) {
            std::cout << "Major error in thread_group.launch (pthread_create). Error: "
                    << error << std::endl;
            exit(EXIT_FAILURE);
        }
        // destroy the attribute object
        error = pthread_attr_destroy(&attr);
        ASSERT_TRUE(!error);
    }

    void thread::launch(const boost::function<void (void)> &spawn_routine,
                        size_t cpu_id) {
        get_tsd_id();
        // if this is not a linux based system simply invoke start and
        // return;
#ifndef __linux__
        launch(spawn_routine);
        return;
#else
        ASSERT_FALSE(thread_started);
        if (cpu_id == size_t(-1)) {
            launch(spawn_routine);
            return;
        }
        if (cpu_count() > 0) {
            cpu_id = cpu_id % cpu_count();
        } else {
            // unknown CPU count
            launch(spawn_routine);
            return;
        }

        // fill in the thread attributes
        pthread_attr_t attr;
        int error = 0;
        error = pthread_attr_init(&attr);
        ASSERT_TRUE(!error);
        error = pthread_attr_setstacksize(&attr, m_stack_size);
        ASSERT_TRUE(!error);
        error = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
        ASSERT_TRUE(!error);

#ifdef HAS_SET_AFFINITY
        // Set Processor Affinity masks (linux only)
        cpu_set_t cpu_set;
        CPU_ZERO(&cpu_set);
        CPU_SET(cpu_id % CPU_SETSIZE, &cpu_set);

        pthread_attr_setaffinity_np(&attr, sizeof(cpu_set), &cpu_set);
#endif

        // Launch the thread
        error = pthread_create(&m_p_thread,
                               &attr,
                               invoke,
                               static_cast<void *>(new invoke_args(m_thread_id,
                                                                   spawn_routine)));
        thread_started = true;
        if (error) {
            std::cout << "Major error in thread_group.launch" << std::endl;
            std::cout << "pthread_create() returned error " << error << std::endl;
            exit(EXIT_FAILURE);
        }


        // destroy the attribute object
        error = pthread_attr_destroy(&attr);
        ASSERT_TRUE(!error);
#endif
    }
}
