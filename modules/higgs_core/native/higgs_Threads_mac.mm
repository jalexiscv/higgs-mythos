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

#if HIGGS_IOS
 bool isIOSAppActive = true;
#endif

static auto getNativeQOS (Thread::Priority priority)
{
    switch (priority)
    {
        case Thread::Priority::highest:    return QOS_CLASS_USER_INTERACTIVE;
        case Thread::Priority::high:       return QOS_CLASS_USER_INITIATED;
        case Thread::Priority::low:        return QOS_CLASS_UTILITY;
        case Thread::Priority::background: return QOS_CLASS_BACKGROUND;
        case Thread::Priority::normal:     break;
    }

    return QOS_CLASS_DEFAULT;
}

static auto getHiggsPriority (qos_class_t qos)
{
    switch (qos)
    {
        case QOS_CLASS_USER_INTERACTIVE:    return Thread::Priority::highest;
        case QOS_CLASS_USER_INITIATED:      return Thread::Priority::high;
        case QOS_CLASS_UTILITY:             return Thread::Priority::low;
        case QOS_CLASS_BACKGROUND:          return Thread::Priority::background;

        case QOS_CLASS_UNSPECIFIED:
        case QOS_CLASS_DEFAULT:             break;
    }

    return Thread::Priority::normal;
}

template <typename Type>
static std::optional<Type> firstOptionalWithValue (const std::initializer_list<std::optional<Type>>& optionals)
{
    for (const auto& optional : optionals)
        if (optional.has_value())
            return optional;

    return {};
}

static bool tryToUpgradeCurrentThreadToRealtime (const Thread::RealtimeOptions& options)
{
    const auto periodMs = options.getPeriodMs().value_or (0.0);

    const auto processingTimeMs = firstOptionalWithValue (
    {
        options.getProcessingTimeMs(),
        options.getMaximumProcessingTimeMs(),
        options.getPeriodMs()
    }).value_or (10.0);

    const auto maxProcessingTimeMs = options.getMaximumProcessingTimeMs()
                                            .value_or (processingTimeMs);

    // The processing time can not exceed the maximum processing time!
    jassert (maxProcessingTimeMs >= processingTimeMs);

    thread_time_constraint_policy_data_t policy;
    policy.period = (uint32_t) Time::secondsToHighResolutionTicks (periodMs / 1'000.0);
    policy.computation = (uint32_t) Time::secondsToHighResolutionTicks (processingTimeMs / 1'000.0);
    policy.constraint = (uint32_t) Time::secondsToHighResolutionTicks (maxProcessingTimeMs / 1'000.0);
    policy.preemptible = true;

    const auto result = thread_policy_set (pthread_mach_thread_np (pthread_self()),
                                           THREAD_TIME_CONSTRAINT_POLICY,
                                           (thread_policy_t) &policy,
                                           THREAD_TIME_CONSTRAINT_POLICY_COUNT);

    if (result == KERN_SUCCESS)
        return true;

    // testing has shown that passing a computation value > 50ms can
    // lead to thread_policy_set returning an error indicating that an
    // invalid argument was passed. If that happens this code tries to
    // limit that value in the hope of resolving the issue.

    if (result == KERN_INVALID_ARGUMENT && options.getProcessingTimeMs() > 50.0)
        return tryToUpgradeCurrentThreadToRealtime (options.withProcessingTimeMs (50.0));

    return false;
}

bool Thread::createNativeThread (Priority priority)
{
    PosixThreadAttribute attribute { threadStackSize };

    pthread_attr_set_qos_class_np (attribute.get(), getNativeQOS (priority), 0);

    struct ThreadData
    {
        Thread& thread;
        std::promise<bool> started{};
    };

    ThreadData threadData { *this, {} };

    auto* handle = makeThreadHandle (attribute, &threadData, [] (void* userData) -> void*
    {
        auto& data { *static_cast<ThreadData*> (userData) };
        auto& thread = data.thread;

        if (thread.isRealtime()
            && ! tryToUpgradeCurrentThreadToRealtime (*thread.realtimeOptions))
        {
            data.started.set_value (false);
            return nullptr;
        }

        data.started.set_value (true);

        higgs_threadEntryPoint (&thread);

        return nullptr;
    });

    if (handle == nullptr || ! threadData.started.get_future().get())
        return false;

    threadId = threadHandle = handle;
    return true;
}

void Thread::killThread()
{
    if (threadHandle != nullptr)
        pthread_cancel ((pthread_t) threadHandle.load());
}

Thread::Priority Thread::getPriority() const
{
    jassert (Thread::getCurrentThreadId() == getThreadId());

    if (! isRealtime())
        return getHiggsPriority (qos_class_self());

    return {};
}

bool Thread::setPriority (Priority priority)
{
    jassert (Thread::getCurrentThreadId() == getThreadId());

    return pthread_set_qos_class_self_np (getNativeQOS (priority), 0) == 0;
}

//==============================================================================
HIGGS_API bool HIGGS_CALLTYPE Process::isForegroundProcess()
{
   if (SystemStats::isRunningInAppExtensionSandbox())
       return true;

   #if HIGGS_MAC
    return [NSApp isActive];
   #else
    return isIOSAppActive;
   #endif
}

HIGGS_API void HIGGS_CALLTYPE Process::makeForegroundProcess()
{
   #if HIGGS_MAC
    if (! SystemStats::isRunningInAppExtensionSandbox())
        [NSApp activateIgnoringOtherApps: YES];
   #endif
}

HIGGS_API void HIGGS_CALLTYPE Process::hide()
{
    if (! SystemStats::isRunningInAppExtensionSandbox())
    {
       #if HIGGS_MAC
        [NSApp hide: nil];
       #elif HIGGS_IOS
        [[UIApplication sharedApplication] performSelector: @selector (suspend)];
       #endif
    }
}

HIGGS_API void HIGGS_CALLTYPE Process::raisePrivilege() {}
HIGGS_API void HIGGS_CALLTYPE Process::lowerPrivilege() {}

HIGGS_API void HIGGS_CALLTYPE Process::setPriority (ProcessPriority) {}

//==============================================================================
HIGGS_API bool HIGGS_CALLTYPE higgs_isRunningUnderDebugger() noexcept
{
    struct kinfo_proc info;
    int m[] = { CTL_KERN, KERN_PROC, KERN_PROC_PID, getpid() };
    size_t sz = sizeof (info);
    sysctl (m, 4, &info, &sz, nullptr, 0);
    return (info.kp_proc.p_flag & P_TRACED) != 0;
}

} // namespace higgs
