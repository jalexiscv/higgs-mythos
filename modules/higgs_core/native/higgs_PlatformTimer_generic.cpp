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

class GenericPlatformTimer final : private Thread
{
public:
    explicit GenericPlatformTimer (PlatformTimerListener& ptl)
        : Thread { "HighResolutionTimerThread" },
          listener { ptl }
    {
        if (startThread (Priority::highest))
            return;

        // This likely suggests there are too many threads running!
        jassertfalse;
    }

    ~GenericPlatformTimer() override
    {
        stopThread (-1);
    }

    void startTimer (int newIntervalMs)
    {
        jassert (newIntervalMs > 0);
        jassert (timer == nullptr);

        {
            std::scoped_lock lock { runCopyMutex };
            timer = std::make_shared<Timer> (listener, newIntervalMs);
        }

        notify();
    }

    void cancelTimer()
    {
        jassert (timer != nullptr);

        timer->cancel();

        // Note the only race condition we need to protect against
        // here is the copy in run().
        //
        // Calls to startTimer(), cancelTimer(), and getIntervalMs()
        // are already guaranteed to be both thread safe and well
        // synchronised.

        std::scoped_lock lock { runCopyMutex };
        timer = nullptr;
    }

    int getIntervalMs() const
    {
        return isThreadRunning() && timer != nullptr ? timer->getIntervalMs() : 0;
    }

private:
    void run() final
    {
        const auto copyTimer = [&]
        {
            std::scoped_lock lock { runCopyMutex };
            return timer;
        };

        while (! threadShouldExit())
        {
            if (auto t = copyTimer())
                t->run();

            wait (-1);
        }
    }

    class Timer
    {
    public:
        Timer (PlatformTimerListener& l, int i)
            : listener { l }, intervalMs { i } {}

        int getIntervalMs() const
        {
            return intervalMs;
        }

        void cancel()
        {
            stop.signal();
        }

        void run()
        {
           #if HIGGS_MAC || HIGGS_IOS
            tryToUpgradeCurrentThreadToRealtime (Thread::RealtimeOptions{}.withPeriodMs (intervalMs));
           #endif

            const auto millisecondsUntil = [] (auto time)
            {
                return jmax (0.0, time - Time::getMillisecondCounterHiRes());
            };

            while (! stop.wait (millisecondsUntil (nextEventTime)))
            {
                if (Time::getMillisecondCounterHiRes() >= nextEventTime)
                {
                    listener.onTimerExpired();
                    nextEventTime += intervalMs;
                }
            }
        }

    private:
        PlatformTimerListener& listener;
        const int intervalMs;
        double nextEventTime = Time::getMillisecondCounterHiRes() + intervalMs;
        WaitableEvent stop { true };

        HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Timer)
        HIGGS_DECLARE_NON_MOVEABLE (Timer)
    };

    PlatformTimerListener& listener;
    mutable std::mutex runCopyMutex;
    std::shared_ptr<Timer> timer;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GenericPlatformTimer)
    HIGGS_DECLARE_NON_MOVEABLE (GenericPlatformTimer)
};

#if ! HIGGS_WINDOWS
using PlatformTimer = GenericPlatformTimer;
#endif

} // namespace higgs
