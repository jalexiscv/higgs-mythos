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

ReadWriteLock::ReadWriteLock() noexcept
{
    readerThreads.ensureStorageAllocated (16);
}

ReadWriteLock::~ReadWriteLock() noexcept
{
    jassert (readerThreads.size() == 0);
    jassert (numWriters == 0);
}

//==============================================================================
void ReadWriteLock::enterRead() const noexcept
{
    while (! tryEnterRead())
        readWaitEvent.wait (100);
}

bool ReadWriteLock::tryEnterRead() const noexcept
{
    auto threadId = Thread::getCurrentThreadId();

    const SpinLock::ScopedLockType sl (accessLock);

    for (auto& readerThread : readerThreads)
    {
        if (readerThread.threadID == threadId)
        {
            readerThread.count++;
            return true;
        }
    }

    if (numWriters + numWaitingWriters == 0
         || (threadId == writerThreadId && numWriters > 0))
    {
        readerThreads.add ({ threadId, 1 });
        return true;
    }

    return false;
}

void ReadWriteLock::exitRead() const noexcept
{
    auto threadId = Thread::getCurrentThreadId();
    const SpinLock::ScopedLockType sl (accessLock);

    for (int i = 0; i < readerThreads.size(); ++i)
    {
        auto& readerThread = readerThreads.getReference (i);

        if (readerThread.threadID == threadId)
        {
            if (--(readerThread.count) == 0)
            {
                readerThreads.remove (i);

                readWaitEvent.signal();
                writeWaitEvent.signal();
            }

            return;
        }
    }

    jassertfalse; // unlocking a lock that wasn't locked
}

//==============================================================================
void ReadWriteLock::enterWrite() const noexcept
{
    auto threadId = Thread::getCurrentThreadId();
    const SpinLock::ScopedLockType sl (accessLock);

    while (! tryEnterWriteInternal (threadId))
    {
        ++numWaitingWriters;
        accessLock.exit();
        writeWaitEvent.wait (100);
        accessLock.enter();
        --numWaitingWriters;
    }
}

bool ReadWriteLock::tryEnterWrite() const noexcept
{
    const SpinLock::ScopedLockType sl (accessLock);
    return tryEnterWriteInternal (Thread::getCurrentThreadId());
}

bool ReadWriteLock::tryEnterWriteInternal (Thread::ThreadID threadId) const noexcept
{
    if (readerThreads.size() + numWriters == 0
         || threadId == writerThreadId
         || (readerThreads.size() == 1 && readerThreads.getReference (0).threadID == threadId))
    {
        writerThreadId = threadId;
        ++numWriters;
        return true;
    }

    return false;
}

void ReadWriteLock::exitWrite() const noexcept
{
    const SpinLock::ScopedLockType sl (accessLock);

    // check this thread actually had the lock
    jassert (numWriters > 0 && writerThreadId == Thread::getCurrentThreadId());

    if (--numWriters == 0)
    {
        writerThreadId = {};

        readWaitEvent.signal();
        writeWaitEvent.signal();
    }
}

} // namespace higgs
