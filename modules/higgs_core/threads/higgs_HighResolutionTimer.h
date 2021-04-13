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

/**
    A high-resolution periodic timer.

    This provides accurately-timed regular callbacks. Unlike the normal Timer
    class, this one uses a dedicated thread, not the message thread, so is
    far more stable and precise.

    You should only use this class in situations where you really need accuracy,
    because unlike the normal Timer class, which is very lightweight and cheap
    the HighResolutionTimer will use far more resources and require thread
    safety considerations.

    @see Timer

    @tags{Core}
*/
class HIGGS_API  HighResolutionTimer
{
protected:
    /** Creates a HighResolutionTimer.
        When created, the timer is stopped, so use startTimer() to get it going.
    */
    HighResolutionTimer();

public:
    /** Destructor. */
    virtual ~HighResolutionTimer();

    //==============================================================================
    /** The user-defined callback routine that actually gets called periodically.

        This will be called on a dedicated timer thread, so make sure your
        implementation is thread-safe!

        On some platforms the dedicated timer thread may be shared with
        other HighResolutionTimer's so aim to complete any work in this
        callback as fast as possible.

        It's perfectly ok to call startTimer() or stopTimer() from within this
        callback to change the subsequent intervals. However, if you call
        stopTimer() in the callback it's still best practice to call stopTimer()
        from the destructor in order to avoid data races.
    */
    virtual void hiResTimerCallback() = 0;

    //==============================================================================
    /** Starts the timer and sets the length of interval required.

        If the timer has already started, this will reset the timer, so the time
        between calling this method and the next timer callback will not be less
        than the interval length passed in.

        In exceptional circumstances the dedicated timer thread may not start,
        if this is a potential concern for your use case, you can call
        isTimerRunning() to confirm if the timer actually started.

        On Windows the underlying API only allows 16 high-resolution timers to
        run simultaneously in the same process. A fallback timer will be used
        when this limit is exceeded but the precision may be significantly
        compromised. This is a particular concern for plugins, because other
        plugins in the same host process may already have timers running.
        To avoid issues, try to use the minimum number of HighResolutionTimers
        possible. For example, consider using a SharedResourcePointer so that
        all instances of the same plugin running in the same same process can
        share a single HighResolutionTimer instance.

        @param  intervalInMilliseconds  the interval to use (a value of zero or less will stop the timer)
    */
    void startTimer (int intervalInMilliseconds);

    /** Stops the timer.

        This method may block while it waits for pending callbacks to complete.
        Once it returns, no more callbacks will be made. If it is called from
        the timer's own thread, it will cancel the timer after the current
        callback returns.

        To prevent data races it's normally best practice to call this in the
        derived classes destructor, even if stopTimer() was called in the
        hiResTimerCallback().
    */
    void stopTimer();

    /** Checks if the timer has been started.
        @returns true if the timer is running.
    */
    bool isTimerRunning() const noexcept;

    /** Returns the timer's interval.
        @returns the timer's interval in milliseconds if it's running, or 0 if it's not.
    */
    int getTimerInterval() const noexcept;

private:
    class Impl;
    std::unique_ptr<Impl> impl;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HighResolutionTimer)
};

} // namespace higgs
