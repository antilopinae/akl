#pragma once


namespace akl {


/**
   * \ingroup util
   * Manages a collection of threads.
   *
   * The thread_group object performs limited exception forwarding.
   * exception throws within a thread of type const char* will be caught
   * and forwarded to the join() function.
   * If the call to join() is wrapped by a try-catch block, the exception
   * will be caught safely and thread cleanup will be completed properly.
   *
   * If multiple threads are running in the thread-group, the master should
   * test if running_threads() is > 0, and retry the join().
   */
  class thread_group {
   private:
    size_t m_thread_counter;
    size_t threads_running;
    mutex mut;
    conditional cond;
    std::queue<std::pair<pthread_t, const char*> > joinqueue;
    // not implemented
    thread_group& operator=(const thread_group &thrgrp);
    thread_group(const thread_group&);
    static void invoke(boost::function<void (void)> spawn_function, thread_group *group);
   public:
    /**
     * Initializes a thread group.
     */
    thread_group() : m_thread_counter(0), threads_running(0) { }

    /**
     * Launch a single thread which calls spawn_function No CPU affinity is
     * set so which core it runs on is up to the OS Scheduler
     */
    void launch(const boost::function<void (void)> &spawn_function);

    /**
     * Launch a single thread which calls spawn_function Also sets CPU
     *  Affinity
     */
    void launch(const boost::function<void (void)> &spawn_function, size_t cpu_id);

    /** Waits for all threads to complete execution. const char* exceptions
    thrown by threads are forwarded to the join() function.
    */
    void join();

    /// Returns the number of running threads.
    inline size_t running_threads() {
      return threads_running;
    }
    //! Destructor. Waits for all threads to complete execution
    inline ~thread_group(){ join(); }

  }; // End of thread group


  /// Runs f in a new thread. convenience function for creating a new thread quickly.
  inline thread launch_in_new_thread(const boost::function<void (void)> &f,
                               size_t cpuid = size_t(-1)) {
    thread thr;
    if (cpuid != size_t(-1)) thr.launch(f, cpuid);
    else thr.launch(f);
    return thr;
  }
}