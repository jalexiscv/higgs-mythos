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

/*
    Note that a lot of methods that you'd expect to find in this file actually
    live in higgs_posix_SharedCode.h!
*/

bool Thread::createNativeThread (Priority)
{
    PosixThreadAttribute attr { threadStackSize };
    PosixSchedulerPriority::getNativeSchedulerAndPriority (realtimeOptions, {}).apply (attr);

    threadId = threadHandle = makeThreadHandle (attr, this, [] (void* userData) -> void*
    {
        auto* myself = static_cast<Thread*> (userData);

        higgs_threadEntryPoint (myself);

        return nullptr;
    });

    return threadId != nullptr;
}

void Thread::killThread()
{
    if (threadHandle != nullptr)
        pthread_cancel ((pthread_t) threadHandle.load());
}

// Until we implement Nice awareness, these don't do anything on Linux.
Thread::Priority Thread::getPriority() const
{
    jassert (Thread::getCurrentThreadId() == getThreadId());

    return priority;
}

bool Thread::setPriority (Priority newPriority)
{
    jassert (Thread::getCurrentThreadId() == getThreadId());

    // Return true to make it compatible with other platforms.
    priority = newPriority;
    return true;
}

//==============================================================================
HIGGS_API void HIGGS_CALLTYPE Process::setPriority (ProcessPriority) {}

static bool swapUserAndEffectiveUser()
{
    auto result1 = setreuid (geteuid(), getuid());
    auto result2 = setregid (getegid(), getgid());
    return result1 == 0 && result2 == 0;
}

HIGGS_API void HIGGS_CALLTYPE Process::raisePrivilege()  { if (geteuid() != 0 && getuid() == 0) swapUserAndEffectiveUser(); }
HIGGS_API void HIGGS_CALLTYPE Process::lowerPrivilege()  { if (geteuid() == 0 && getuid() != 0) swapUserAndEffectiveUser(); }

} // namespace higgs
