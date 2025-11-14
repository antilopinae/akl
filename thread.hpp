#pragma once


namespace akl {
    /**
       * \ingroup util
       * A collection of routines for creating and managing threads.
       *
       * The thread object performs limited exception forwarding.
       * exception throws within a thread of type const char* will be caught
       * and forwarded to the join() function.
       * If the call to join() is wrapped by a try-catch block, the exception
       * will be caught safely and thread cleanup will be completed properly.
       */
    class thread {
    public:
        /**
         * This class contains the data unique to each thread. All threads
         * are gauranteed to have an associated graphlab thread_specific
         * data. The thread object is copyable.
         */
        class tls_data {
        public:
            inline tls_data(size_t thread_id) : thread_id_(thread_id) {
            }

            inline size_t thread_id() { return thread_id_; }
            inline void set_thread_id(size_t t) { thread_id_ = t; }
            any &operator[](const size_t &id) { return local_data[id]; }

            bool contains(const size_t &id) const {
                return local_data.find(id) != local_data.end();
            }

            size_t erase(const size_t &id) {
                return local_data.erase(id);
            }

        private:
            size_t thread_id_;
            boost::unordered_map<size_t, any> local_data;
        }; // end of thread specific data


        /// Static helper routines
        // ===============================================================

        /**
         * Get the thread specific data associated with this thread
         */
        static tls_data &get_tls_data();

        /** Get the id of the calling thread.  This will typically be the
            index in the thread group. Between 0 to ncpus. */
        static inline size_t thread_id() { return get_tls_data().thread_id(); }

        /** Set the id of the calling thread.  This will typically be the
            index in the thread group. Between 0 to ncpus. */
        static inline void set_thread_id(size_t t) { get_tls_data().set_thread_id(t); }

        /**
         * Get a reference to an any object
         */
        static inline any &get_local(const size_t &id) {
            return get_tls_data()[id];
        }

        /**
         * Check to see if there is an entry in the local map
         */
        static inline bool contains(const size_t &id) {
            return get_tls_data().contains(id);
        }

        /**
         * Removes the entry from the local map.
         * @return number of elements erased.
         */
        static inline size_t erase(const size_t &id) {
            return get_tls_data().erase(id);
        }

        /**
         * This static method joins the invoking thread with the other
         * thread object.  This thread will not return from the join
         * routine until the other thread complets it run.
         */
        static void join(thread &other);

        // Called just before thread exits. Can be used
        // to do special cleanup... (need for Java JNI)
        static void thread_destroy_callback();

        static void set_thread_destroy_callback(void (*callback)());


        /**
         * Return the number processing units (individual cores) on this
         * system
         */
        static size_t cpu_count();

    private:
        struct invoke_args {
            size_t m_thread_id;
            boost::function<void(void)> spawn_routine;

            invoke_args(size_t m_thread_id, const boost::function<void(void)> &spawn_routine)
                : m_thread_id(m_thread_id), spawn_routine(spawn_routine) {
            };
        };

        //! Little helper function used to launch threads
        static void *invoke(void *_args);

    public:
        /**
         * Creates a thread with a user-defined associated thread ID
         */
        inline thread(size_t thread_id = 0) : m_stack_size(0),
                                              m_p_thread(0),
                                              m_thread_id(thread_id),
                                              thread_started(false) {
            // Calculate the stack size in in bytes;
            const int BYTES_PER_MB = 1048576;
            const int DEFAULT_SIZE_IN_MB = 8;
            m_stack_size = DEFAULT_SIZE_IN_MB * BYTES_PER_MB;
        }

        /**
         * execute this function to spawn a new thread running spawn_function
         * routine
         */
        void launch(const boost::function<void (void)> &spawn_routine);

        /**
         * Same as launch() except that you can specify a CPU on which to
         * run the thread.  This only currently supported in Linux and if
         * invoked on a non Linux based system this will be equivalent to
         * start().
         */
        void launch(const boost::function<void (void)> &spawn_routine, size_t cpu_id);


        /**
         * Join the calling thread with this thread.
         * const char* exceptions
         * thrown by the thread is forwarded to the join() function.
         */
        inline void join() {
            if (this == NULL) {
                std::cout << "Failure on join()" << std::endl;
                exit(EXIT_FAILURE);
            }
            join(*this);
        }

        /// Returns true if the thread is still running
        inline bool active() const {
            return thread_started;
        }

        inline ~thread() {
        }

        /// Returns the pthread thread id
        inline pthread_t pthreadid() {
            return m_p_thread;
        }

    private:
        //! The size of the internal stack for this thread
        size_t m_stack_size;

        //! The internal pthread object
        pthread_t m_p_thread;

        //! the threads id
        size_t m_thread_id;

        bool thread_started;
    }; // End of class thread
}
