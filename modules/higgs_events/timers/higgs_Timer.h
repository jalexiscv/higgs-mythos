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
    Makes repeated callbacks to a virtual method at a specified time interval.

    A Timer's timerCallback() method will be repeatedly called at a given
    interval. When you create a Timer object, it will do nothing until the
    startTimer() method is called, which will cause the message thread to
    start making callbacks at the specified interval, until stopTimer() is called
    or the object is deleted.

    The time interval isn't guaranteed to be precise to any more than maybe
    10-20ms, and the intervals may end up being much longer than requested if the
    system is busy. Because the callbacks are made by the main message thread,
    anything that blocks the message queue for a period of time will also prevent
    any timers from running until it can carry on.

    If you need to have a single callback that is shared by multiple timers with
    different frequencies, then the MultiTimer class allows you to do that - its
    structure is very similar to the Timer class, but contains multiple timers
    internally, each one identified by an ID number.

    @see HighResolutionTimer, MultiTimer

    @tags{Events}
*/
class HIGGS_API  Timer
{
protected:
    //==============================================================================
    /** Creates a Timer.
        When created, the timer is stopped, so use startTimer() to get it going.
    */
    Timer() noexcept;

    /** Creates a copy of another timer.

        Note that this timer won't be started, even if the one you're copying
        is running.
    */
    Timer (const Timer&) noexcept;

public:
    //==============================================================================
    /** Destructor. */
    virtual ~Timer();

    //==============================================================================
    /** The user-defined callback routine that actually gets called periodically.

        It's perfectly ok to call startTimer() or stopTimer() from within this
        callback to change the subsequent intervals.
    */
    virtual void timerCallback() = 0;

    //==============================================================================
    /** Starts the timer and sets the length of interval required.

        If the timer is already started, this will reset it, so the time between
        calling this method and the next timer callback will not be less than
        the interval length passed in.

        This method may be called from any thread. It only affects when future
        callbacks are scheduled. Callbacks themselves always run on the message
        thread.

        @param  intervalInMilliseconds  the interval to use (any value less
                                        than 1 will be rounded up to 1)
    */
    void startTimer (int intervalInMilliseconds) noexcept;

    /** Starts the timer with an interval specified in Hertz.
        This is effectively the same as calling startTimer (1000 / timerFrequencyHz).
    */
    void startTimerHz (int timerFrequencyHz) noexcept;

    /** Stops the timer.

        This method may be called from any thread. It cancels any future
        callbacks for this timer. If a callback is already running on the
        message thread, this method does not wait for it to finish and may
        return while that callback is still executing.

        Do not destroy the Timer (or data it uses) from another thread while a
        callback may still be running. Before destroying a timer it's best
        practice to stop the timer from the message thread. If the Timer will be
        destroyed from a background thread consider using MessageManager::callSync()
        to call stopTimer() from the message thread.
    */
    void stopTimer() noexcept;

    //==============================================================================
    /** Returns true if the timer is currently running. */
    bool isTimerRunning() const noexcept                    { return timerPeriodMs.load() > 0; }

    /** Returns the timer's interval.
        @returns the timer's interval in milliseconds if it's running, or 0 if it's not.
    */
    int getTimerInterval() const noexcept                   { return timerPeriodMs.load(); }

    //==============================================================================
    /** Invokes a lambda after a given number of milliseconds. */
    static void HIGGS_CALLTYPE callAfterDelay (int milliseconds, std::function<void()> functionToCall);

    //==============================================================================
    /** For internal use only: invokes any timers that need callbacks.
        Don't call this unless you really know what you're doing!
    */
    static void HIGGS_CALLTYPE callPendingTimersSynchronously();

private:
    class TimerThread;
    size_t positionInQueue = (size_t) -1;
    std::atomic<int> timerPeriodMs = 0;
    SharedResourcePointer<TimerThread> timerThread;

    Timer& operator= (const Timer&) = delete;

    HIGGS_LEAK_DETECTOR (Timer)
};

} // namespace higgs
