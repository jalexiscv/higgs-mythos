/*
  ==============================================================================

   This file is part of the Higgs Mythos framework.
   Copyright (c) Raw Material Software Limited

   Higgs Mythos is free software: you can redistribute it and/or modify
   it under the terms of the GNU Affero General Public License as published
   by the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   Higgs Mythos is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU Affero General Public License for more details.

   You should have received a copy of the GNU Affero General Public License
   along with Higgs Mythos. If not, see <https://www.gnu.org/licenses/>.

  ==============================================================================
*/

namespace higgs
{

//==============================================================================
/**
    A set of threads that will run a list of jobs.

    When a ThreadPoolJob object is added to the ThreadPool's list, its runJob() method
    will be called by the next pooled thread that becomes free.

    @see ThreadPoolJob, Thread

    @tags{Core}
*/
struct ThreadPoolOptions
{
    /** The name to give each thread in the pool. */
    [[nodiscard]] ThreadPoolOptions withThreadName (String newThreadName) const
    {
        return withMember (*this, &ThreadPoolOptions::threadName, newThreadName);
    }

    /** The number of threads to run.
        These will be started when a pool is created, and run until the pool is destroyed.
    */
    [[nodiscard]] ThreadPoolOptions withNumberOfThreads (int newNumberOfThreads) const
    {
        return withMember (*this, &ThreadPoolOptions::numberOfThreads, newNumberOfThreads);
    }

    /** The size of the stack of each thread in the pool. */
    [[nodiscard]] ThreadPoolOptions withThreadStackSizeBytes (size_t newThreadStackSizeBytes) const
    {
        return withMember (*this, &ThreadPoolOptions::threadStackSizeBytes, newThreadStackSizeBytes);
    }

    /** The desired priority of each thread in the pool. */
    [[nodiscard]] ThreadPoolOptions withDesiredThreadPriority (Thread::Priority newDesiredThreadPriority) const
    {
        return withMember (*this, &ThreadPoolOptions::desiredThreadPriority, newDesiredThreadPriority);
    }

    String threadName { "Pool" };
    int numberOfThreads { SystemStats::getNumCpus() };
    size_t threadStackSizeBytes { Thread::osDefaultStackSize };
    Thread::Priority desiredThreadPriority { Thread::Priority::normal };
};


//==============================================================================
/**
    A set of threads that will run a list of jobs.

    When a ThreadPoolJob object is added to the ThreadPool's list, its runJob() method
    will be called by the next pooled thread that becomes free.

    @see ThreadPoolJob, Thread

    @tags{Core}
*/
class HIGGS_API  ThreadPool
{
public:
    using Options = ThreadPoolOptions;

    //==============================================================================
    /** Creates a thread pool based on the provided options.
        Once you've created a pool, you can give it some jobs by calling addJob().

        @see ThreadPool::ThreadPoolOptions
    */
    explicit ThreadPool (const Options& options);

    /** Creates a thread pool based using the default arguments provided by
        ThreadPoolOptions.

        Once you've created a pool, you can give it some jobs by calling addJob().

        @see ThreadPoolOptions
    */
    ThreadPool() : ThreadPool { Options{} } {}

    /** Creates a thread pool.
        Once you've created a pool, you can give it some jobs by calling addJob().

        @param numberOfThreads       the number of threads to run. These will be started
                                     immediately, and will run until the pool is deleted.
        @param threadStackSizeBytes  the size of the stack of each thread. If this value
                                     is zero then the default stack size of the OS will
                                     be used.
        @param desiredThreadPriority the desired priority of each thread in the pool.
    */
    ThreadPool (int numberOfThreads,
                size_t threadStackSizeBytes = Thread::osDefaultStackSize,
                Thread::Priority desiredThreadPriority = Thread::Priority::normal);

    /** Destructor.

        This will attempt to remove all the jobs before deleting, but if you want to
        specify a timeout, you should call removeAllJobs() explicitly before deleting
        the pool.
    */
    ~ThreadPool();

    //==============================================================================
    /** A callback class used when you need to select which ThreadPoolJob objects are suitable
        for some kind of operation.
        @see ThreadPool::removeAllJobs
    */
    class HIGGS_API  JobSelector
    {
    public:
        virtual ~JobSelector() = default;

        /** Should return true if the specified thread matches your criteria for whatever
            operation that this object is being used for.

            Any implementation of this method must be extremely fast and thread-safe!
        */
        virtual bool isJobSuitable (ThreadPoolJob* job) = 0;
    };

    //==============================================================================
    /** Adds a job to the queue.

        Once a job has been added, then the next time a thread is free, it will run
        the job's ThreadPoolJob::runJob() method. Depending on the return value of the
        runJob() method, the pool will either remove the job from the pool or add it to
        the back of the queue to be run again.

        If deleteJobWhenFinished is true, then the job object will be owned and deleted by
        the pool when not needed - if you do this, make sure that your object's destructor
        is thread-safe.

        If deleteJobWhenFinished is false, the pointer will be used but not deleted, and
        the caller is responsible for making sure the object is not deleted before it has
        been removed from the pool.
    */
    void addJob (ThreadPoolJob* job,
                 bool deleteJobWhenFinished);

    /** Adds an invokable to be called as a job.

        This will create an internal ThreadPoolJob object to encapsulate and
        call the invokable object.

        @param  jobToRun    An invokable object that takes no arguments and
                            returns either void or a ThreadPoolJob::JobStatus.
                            If it returns void it will only be run once.
    */
    template <typename Invokable,
              std::enable_if_t<detail::canRunThreadPoolLambdaJob<Invokable>, int> = 0>
    void addJob (Invokable&& jobToRun)
    {
        addJob (new detail::ThreadPoolLambdaJob<Invokable> (std::forward<Invokable> (jobToRun)), true);
    }

    /** Tries to remove a job from the pool.

        If the job isn't yet running, this will simply remove it. If it is running, it
        will wait for it to finish.

        If the timeout period expires before the job finishes running, then the job will be
        left in the pool and this will return false. It returns true if the job is successfully
        stopped and removed.

        @param job                  the job to remove
        @param interruptIfRunning   if true, then if the job is currently busy, its
                                    ThreadPoolJob::signalJobShouldExit() method will be called to try
                                    to interrupt it. If false, then if the job will be allowed to run
                                    until it stops normally (or the timeout expires)
        @param timeOutMilliseconds  the length of time this method should wait for the job to finish
                                    before giving up and returning false
    */
    bool removeJob (ThreadPoolJob* job,
                    bool interruptIfRunning,
                    int timeOutMilliseconds);

    /** Tries to remove all jobs from the pool.

        @param interruptRunningJobs if true, then all running jobs will have their ThreadPoolJob::signalJobShouldExit()
                                    methods called to try to interrupt them
        @param timeOutMilliseconds  the length of time this method should wait for all the jobs to finish
                                    before giving up and returning false
        @param selectedJobsToRemove if this is not a nullptr, the JobSelector object is asked to decide
                                    which jobs should be removed. If it is a nullptr, all jobs are removed
        @returns    true if all jobs are successfully stopped and removed; false if the timeout period
                    expires while waiting for one or more jobs to stop
    */
    bool removeAllJobs (bool interruptRunningJobs,
                        int timeOutMilliseconds,
                        JobSelector* selectedJobsToRemove = nullptr);

    /** Returns the number of jobs currently running or queued. */
    int getNumJobs() const noexcept;

    /** Returns the number of threads assigned to this thread pool. */
    int getNumThreads() const noexcept;

    /** Returns one of the jobs in the queue.

        Note that this can be a very volatile list as jobs might be continuously getting shifted
        around in the list, and this method may return nullptr if the index is currently out-of-range.
    */
    ThreadPoolJob* getJob (int index) const noexcept;

    /** Returns true if the given job is currently queued or running.

        @see isJobRunning()
    */
    bool contains (const ThreadPoolJob* job) const noexcept;

    /** Returns true if the given job is currently being run by a thread. */
    bool isJobRunning (const ThreadPoolJob* job) const noexcept;

    /** Waits until a job has finished running and has been removed from the pool.

        This will wait until the job is no longer in the pool - i.e. until its
        runJob() method returns ThreadPoolJob::jobHasFinished.

        If the timeout period expires before the job finishes, this will return false;
        it returns true if the job has finished successfully.
    */
    bool waitForJobToFinish (const ThreadPoolJob* job,
                             int timeOutMilliseconds) const;

    /** If the given job is in the queue, this will move it to the front so that it
        is the next one to be executed.
    */
    void moveJobToFront (const ThreadPoolJob* jobToMove) noexcept;

    /** Returns a list of the names of all the jobs currently running or queued.
        If onlyReturnActiveJobs is true, only the ones currently running are returned.
    */
    StringArray getNamesOfAllJobs (bool onlyReturnActiveJobs) const;

private:
    //==============================================================================
    Array<ThreadPoolJob*> jobs;

    struct ThreadPoolThread;
    friend class ThreadPoolJob;
    OwnedArray<ThreadPoolThread> threads;

    CriticalSection lock;
    WaitableEvent jobFinishedSignal;

    bool runNextJob (ThreadPoolThread&);
    ThreadPoolJob* pickNextJobToRun();
    void addToDeleteList (OwnedArray<ThreadPoolJob>&, ThreadPoolJob*) const;
    void stopThreads();

    // Note that this method has changed, and no longer has a parameter to indicate
    // whether the jobs should be deleted - see the new method for details.
    void removeAllJobs (bool, int, bool);

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ThreadPool)
};

} // namespace higgs
