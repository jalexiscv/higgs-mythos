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
    Automatically locks and unlocks a ReadWriteLock object.

    Use one of these as a local variable to control access to a ReadWriteLock.

    e.g. @code

    ReadWriteLock myLock;

    for (;;)
    {
        const ScopedWriteLock myScopedLock (myLock);
        // myLock is now locked

        ...do some stuff...

        // myLock gets unlocked here
    }
    @endcode

    @see ReadWriteLock, ScopedReadLock

    @tags{Core}
*/
class HIGGS_API  ScopedWriteLock
{
public:
    //==============================================================================
    /** Creates a ScopedWriteLock.

        As soon as it is created, this will call ReadWriteLock::enterWrite(), and
        when the ScopedWriteLock object is deleted, the ReadWriteLock will
        be unlocked.

        Make sure this object is created and deleted by the same thread,
        otherwise there are no guarantees what will happen! Best just to use it
        as a local stack object, rather than creating one with the new() operator.
    */
    inline explicit ScopedWriteLock (const ReadWriteLock& lock) noexcept : lock_ (lock) { lock.enterWrite(); }

    /** Destructor.

        The ReadWriteLock's exitWrite() method will be called when the destructor is called.

        Make sure this object is created and deleted by the same thread,
        otherwise there are no guarantees what will happen!
    */
    inline ~ScopedWriteLock() noexcept                                   { lock_.exitWrite(); }


private:
    //==============================================================================
    const ReadWriteLock& lock_;

    HIGGS_DECLARE_NON_COPYABLE (ScopedWriteLock)
};

//==============================================================================
/**
    Automatically locks and unlocks a ReadWriteLock object.

    Use one of these as a local variable to control access to a ReadWriteLock.

    e.g. @code

    ReadWriteLock myLock;

    for (;;)
    {
        const ScopedTryWriteLock myScopedTryLock (myLock);

        // Unlike using a ScopedWriteLock, this may fail to actually get the lock, so you
        // should test this with the isLocked() method before doing your thread-unsafe
        // action.

        if (myScopedTryLock.isLocked())
        {
            ...do some stuff...
        }
        else
        {
            ...our attempt at locking failed because some other thread has already locked the object...
        }

        // myLock gets unlocked here (if it was locked)
    }
    @endcode

    @see ReadWriteLock, ScopedTryWriteLock

    @tags{Core}
*/
class HIGGS_API  ScopedTryWriteLock
{
public:
    //==============================================================================
    /** Creates a ScopedTryWriteLock and calls ReadWriteLock::tryEnterWrite() immediately.
        When the ScopedTryWriteLock object is destructed, the ReadWriteLock will be unlocked
        (if it was successfully acquired).

        Make sure this object is created and destructed by the same thread, otherwise there are no
        guarantees what will happen! Best just to use it as a local stack object, rather than creating
        one with the new() operator.
    */
    ScopedTryWriteLock (ReadWriteLock& lockIn) noexcept
            : ScopedTryWriteLock (lockIn, true) {}

    /** Creates a ScopedTryWriteLock.

        If acquireLockOnInitialisation is true then as soon as it is created, this will call
        ReadWriteLock::tryEnterWrite(), and when the ScopedTryWriteLock object is destructed, the
        ReadWriteLock will be unlocked (if it was successfully acquired).

        Make sure this object is created and destructed by the same thread, otherwise there are no
        guarantees what will happen! Best just to use it as a local stack object, rather than creating
        one with the new() operator.
    */
    ScopedTryWriteLock (ReadWriteLock& lockIn, bool acquireLockOnInitialisation) noexcept
        : lock (lockIn), lockWasSuccessful (acquireLockOnInitialisation && lock.tryEnterWrite()) {}

    /** Destructor.

        The ReadWriteLock's exitWrite() method will be called when the destructor is called.

        Make sure this object is created and destructed by the same thread,
        otherwise there are no guarantees what will happen!
    */
    ~ScopedTryWriteLock() noexcept                  { if (lockWasSuccessful) lock.exitWrite(); }

    /** Returns true if the mutex was successfully locked. */
    bool isLocked() const noexcept                  { return lockWasSuccessful; }

    /** Retry gaining the lock by calling tryEnter on the underlying lock. */
    bool retryLock() noexcept                       { return lockWasSuccessful = lock.tryEnterWrite(); }

private:
    //==============================================================================
    ReadWriteLock& lock;
    bool lockWasSuccessful;

    HIGGS_DECLARE_NON_COPYABLE (ScopedTryWriteLock)
};

}
