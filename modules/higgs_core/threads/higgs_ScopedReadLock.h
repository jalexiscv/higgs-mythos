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
        const ScopedReadLock myScopedLock (myLock);
        // myLock is now locked

        ...do some stuff...

        // myLock gets unlocked here
    }
    @endcode

    @see ReadWriteLock, ScopedWriteLock

    @tags{Core}
*/
class HIGGS_API  ScopedReadLock
{
public:
    //==============================================================================
    /** Creates a ScopedReadLock.

        As soon as it is created, this will call ReadWriteLock::enterRead(), and
        when the ScopedReadLock object is deleted, the ReadWriteLock will
        be unlocked.

        Make sure this object is created and deleted by the same thread,
        otherwise there are no guarantees what will happen! Best just to use it
        as a local stack object, rather than creating one with the new() operator.
    */
    inline explicit ScopedReadLock (const ReadWriteLock& lock) noexcept   : lock_ (lock) { lock.enterRead(); }

    /** Destructor.

        The ReadWriteLock's exitRead() method will be called when the destructor is called.

        Make sure this object is created and deleted by the same thread,
        otherwise there are no guarantees what will happen!
    */
    inline ~ScopedReadLock() noexcept                                     { lock_.exitRead(); }


private:
    //==============================================================================
    const ReadWriteLock& lock_;

    HIGGS_DECLARE_NON_COPYABLE (ScopedReadLock)
};

//==============================================================================
/**
    Automatically locks and unlocks a ReadWriteLock object.

    Use one of these as a local variable to control access to a ReadWriteLock.

    e.g. @code

    ReadWriteLock myLock;

    for (;;)
    {
        const ScopedTryReadLock myScopedTryLock (myLock);

        // Unlike using a ScopedReadLock, this may fail to actually get the lock, so you
        // should test this with the isLocked() method before doing your thread-unsafe
        // action.

        if (myScopedTryLock.isLocked())
        {
            ...do some stuff...
        }
        else
        {
            ...our attempt at locking failed because a write lock has already been issued...
        }

        // myLock gets unlocked here (if it was locked)
    }
    @endcode

    @see ReadWriteLock, ScopedTryWriteLock

    @tags{Core}
*/
class HIGGS_API  ScopedTryReadLock
{
public:
    //==============================================================================
    /** Creates a ScopedTryReadLock and calls ReadWriteLock::tryEnterRead() as soon as it is
        created. When the ScopedTryReadLock object is destructed, the ReadWriteLock will be unlocked
        (if it was successfully acquired).

        Make sure this object is created and destructed by the same thread, otherwise there are no
        guarantees what will happen! Best just to use it as a local stack object, rather than creating
        one with the new() operator.
    */
    explicit ScopedTryReadLock (ReadWriteLock& lockIn)
        : ScopedTryReadLock (lockIn, true) {}

    /** Creates a ScopedTryReadLock.

        If acquireLockOnInitialisation is true then as soon as it is created, this will call
        ReadWriteLock::tryEnterRead(), and when the ScopedTryReadLock object is destructed, the
        ReadWriteLock will be unlocked (if it was successfully acquired).

        Make sure this object is created and destructed by the same thread, otherwise there are no
        guarantees what will happen! Best just to use it as a local stack object, rather than creating
        one with the new() operator.
    */
    ScopedTryReadLock (ReadWriteLock& lockIn, bool acquireLockOnInitialisation) noexcept
        : lock (lockIn), lockWasSuccessful (acquireLockOnInitialisation && lock.tryEnterRead()) {}

    /** Destructor.

        The ReadWriteLock's exitRead() method will be called when the destructor is called.

        Make sure this object is created and destructed by the same thread, otherwise there are no
        guarantees what will happen!
    */
    ~ScopedTryReadLock() noexcept                   { if (lockWasSuccessful) lock.exitRead(); }

    /** Returns true if the mutex was successfully locked. */
    bool isLocked() const noexcept                  { return lockWasSuccessful; }

    /** Retry gaining the lock by calling tryEnter on the underlying lock. */
    bool retryLock() noexcept                       { return lockWasSuccessful = lock.tryEnterRead(); }

private:
    //==============================================================================
    ReadWriteLock& lock;
    bool lockWasSuccessful;

    HIGGS_DECLARE_NON_COPYABLE (ScopedTryReadLock)
};

} // namespace higgs
