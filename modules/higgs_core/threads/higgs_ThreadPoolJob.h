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

class ThreadPool;

//==============================================================================
/**
    A task that is executed by a ThreadPool object.

    A ThreadPool keeps a list of ThreadPoolJob objects which are executed by
    its threads.

    The runJob() method needs to be implemented to do the task, and if the code that
    does the work takes a significant time to run, it must keep checking the shouldExit()
    method to see if something is trying to interrupt the job. If shouldExit() returns
    true, the runJob() method must return immediately.

    @see ThreadPool, Thread

    @tags{Core}
*/
class HIGGS_API  ThreadPoolJob
{
public:
    //==============================================================================
    /** Creates a thread pool job object.
        After creating your job, add it to a thread pool with ThreadPool::addJob().
    */
    explicit ThreadPoolJob (const String& name);

    /** Destructor. */
    virtual ~ThreadPoolJob();

    //==============================================================================
    /** Returns the name of this job.
        @see setJobName
    */
    String getJobName() const;

    /** Changes the job's name.
        @see getJobName
    */
    void setJobName (const String& newName);

    //==============================================================================
    /** These are the values that can be returned by the runJob() method.
    */
    enum JobStatus
    {
        jobHasFinished = 0,     /**< indicates that the job has finished and can be
                                     removed from the pool */

        jobNeedsRunningAgain    /**< indicates that the job would like to be called
                                     again when a thread is free */
    };

    /** Performs the actual work that this job needs to do.

        Your subclass must implement this method, in which is does its work.

        If the code in this method takes a significant time to run, it must repeatedly check
        the shouldExit() method to see if something is trying to interrupt the job.
        If shouldExit() ever returns true, the runJob() method must return immediately.

        If this method returns jobHasFinished, then the job will be removed from the pool
        immediately. If it returns jobNeedsRunningAgain, then the job will be left in the
        pool and will get a chance to run again as soon as a thread is free.

        @see shouldExit()
    */
    virtual JobStatus runJob() = 0;


    //==============================================================================
    /** Returns true if this job is currently running its runJob() method. */
    bool isRunning() const noexcept                     { return isActive; }

    /** Returns true if something is trying to interrupt this job and make it stop.

        Your runJob() method must call this whenever it gets a chance, and if it ever
        returns true, the runJob() method must return immediately.

        @see signalJobShouldExit()
    */
    bool shouldExit() const noexcept                    { return shouldStop; }

    /** Calling this will cause the shouldExit() method to return true, and the job
        should (if it's been implemented correctly) stop as soon as possible.

        @see shouldExit()
    */
    void signalJobShouldExit();

    /** Add a listener to this thread job which will receive a callback when
        signalJobShouldExit was called on this thread job.

        @see signalJobShouldExit, removeListener
    */
    void addListener (Thread::Listener*);

    /** Removes a listener added with addListener. */
    void removeListener (Thread::Listener*);

    //==============================================================================
    /** If the calling thread is being invoked inside a runJob() method, this will
        return the ThreadPoolJob that it belongs to.
    */
    static ThreadPoolJob* getCurrentThreadPoolJob();

    //==============================================================================
private:
    friend class ThreadPool;
    String jobName;
    ThreadPool* pool = nullptr;
    std::atomic<bool> shouldStop { false }, isActive { false }, shouldBeDeleted { false };
    ThreadSafeListenerList<Thread::Listener> listeners;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ThreadPoolJob)
};

} // namespace higgs
