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
    Allows threads to wait for events triggered by other threads.

    A thread can call WaitableEvent::wait() to suspend the calling thread until
    another thread wakes it up by calling the WaitableEvent::signal() method.

    @tags{Core}
*/
class HIGGS_API  WaitableEvent
{
public:
    //==============================================================================
    /** Creates a WaitableEvent object.

        The object is initially in an un-signalled state.

        @param manualReset  If this is false, the event will be reset
                            automatically when the wait() method is called.
                            If manualReset is true, then once the event is
                            signalled, the only way to reset it will be by
                            calling the reset() method.
    */
    explicit WaitableEvent (bool manualReset = false) noexcept;

    //==============================================================================
    /** Suspends the calling thread until the event has been signalled.

        This will wait until the object's signal() method is called by another
        thread, or until the timeout expires.

        After the event has been signalled, this method will return true and if
        manualReset was set to false in the WaitableEvent's constructor, then
        the event will be reset.

        Unlike wait (double), a negative timeout is not supported and will
        trigger an assertion. To wait indefinitely, call wait() with no
        arguments.

        @param timeOut  the maximum time to wait. Must not be negative.

        @returns    true if the object has been signalled, false if the timeout
                    expires first.
        @see signal, reset
    */
    bool wait (Seconds timeOut) const;

    /** Suspends the calling thread until the event has been signalled.

        This will wait until the object's signal() method is called by another
        thread, or until the timeout expires.

        After the event has been signalled, this method will return true and if
        manualReset was set to false in the WaitableEvent's constructor, then
        the event will be reset.

        @param timeOutMilliseconds  the maximum time to wait, in milliseconds.
                                    A negative value will cause it to wait
                                    forever.

        @returns    true if the object has been signalled, false if the timeout
                    expires first.
        @see signal, reset
    */
    bool wait (double timeOutMilliseconds) const;

    /** Suspends the calling thread until the event has been signalled.

        This will wait until the object's signal() method is called by another
        thread.

        After the event has been signalled, this method will return and if
        manualReset was set to false in the WaitableEvent's constructor, then
        the event will be reset.

        @see signal, reset
    */
    void wait() const;

    /** Wakes up any threads that are currently waiting on this object.

        If signal() is called when nothing is waiting, the next thread to call
        wait() will return immediately and reset the signal.

        If the WaitableEvent is manual reset, all current and future threads
        that wait upon this object will be woken, until reset() is explicitly
        called.

        If the WaitableEvent is automatic reset, and one or more threads is
        waiting upon the object, then one of them will be woken up. If no
        threads are currently waiting, then the next thread to call wait() will
        be woken up. As soon as a thread is woken, the signal is automatically
        reset.

        @see wait, reset
    */
    void signal() const;

    /** Resets the event to an unsignalled state.
        If it's not already signalled, this does nothing.
    */
    void reset() const;

private:
    //==============================================================================
    bool useManualReset;

    mutable std::mutex mutex;
    mutable std::condition_variable condition;
    mutable std::atomic<bool> triggered { false };

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaitableEvent)
};

} // namespace higgs
