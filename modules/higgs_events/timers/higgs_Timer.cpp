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

class Timer::TimerThread final : private Thread,
                                 private Thread::Listener,
                                 private MessageManager::LifetimeListener
{
public:
    using LockType = CriticalSection;

    TimerThread()
        : Thread (SystemStats::getHIGGSVersion() + ": Timer")
    {
        timers.reserve (32);
        addListener (this);
        MessageManager::addLifetimeListener (*this);
    }

    ~TimerThread() override
    {
        MessageManager::removeLifetimeListener (*this);
        stopThread();
        removeListener (this);
    }

    void startTimer (Timer* t, int interval)
    {
        const LockType::ScopedLockType sl (lock);

        if (t->timerPeriodMs.exchange (interval) > 0)
            resetTimerCounter (t);
        else
            addTimer (t);
    }

    void stopTimer (Timer* t)
    {
        if (! t->isTimerRunning())
            return;

        const LockType::ScopedLockType sl (lock);

        if (t->timerPeriodMs.exchange (0) > 0)
            removeTimer (t);
    }

    void callTimersSynchronously()
    {
        callTimers();
    }

private:
    void run() override
    {
        auto lastTime = Time::getMillisecondCounter();
        ReferenceCountedObjectPtr<CallTimersMessage> messageToSend (new CallTimersMessage());

        while (! threadShouldExit())
        {
            auto now = Time::getMillisecondCounter();
            auto elapsed = (int) (now >= lastTime ? (now - lastTime)
                                                  : (std::numeric_limits<uint32>::max() - (lastTime - now)));
            lastTime = now;

            auto timeUntilFirstTimer = getTimeUntilFirstTimer (elapsed);

            if (timeUntilFirstTimer <= 0)
            {
                if (callbackArrived.wait (0))
                {
                    // already a message in flight - do nothing
                }
                else
                {
                    messageToSend->post();

                    // Sometimes our message can get discarded by the OS (e.g.
                    // when running as an RTAS when the app has a modal loop),
                    // so this is how long to wait before assuming the message
                    // has been lost and trying again.
                    if (callbackArrived.wait (300) || threadShouldExit())
                        continue;

                    messageToSend->post();
                }
            }

            // don't wait for too long because running this loop also helps keep the
            // Time::getApproximateMillisecondTimer value stay up-to-date
            wait (jlimit (1, 100, timeUntilFirstTimer));
        }
    }

    void callTimers()
    {
        auto timeout = Time::getMillisecondCounter() + 100;

        const LockType::ScopedLockType sl (lock);

        while (! isShuttingDown && ! timers.empty())
        {
            auto& first = timers.front();

            if (first.countdownMs > 0)
                break;

            auto* timer = first.timer;
            first.countdownMs = timer->getTimerInterval();
            shuffleTimerBackInQueue (0);

            const LockType::ScopedUnlockType ul (lock);

            HIGGS_TRY
            {
                timer->timerCallback();
            }
            HIGGS_CATCH_EXCEPTION

            // avoid getting stuck in a loop if a timer callback repeatedly takes too long
            if (Time::getMillisecondCounter() > timeout)
                break;
        }

        callbackArrived.signal();
        notify();
    }

    void addTimer (Timer* t)
    {
        // Trying to add a timer that's already here - shouldn't get to this point,
        // so if you get this assertion, let me know!
        jassert (std::none_of (timers.begin(), timers.end(),
                               [t] (TimerCountdown i) { return i.timer == t; }));

        auto pos = timers.size();

        timers.push_back ({ t, t->getTimerInterval() });
        t->positionInQueue = pos;
        shuffleTimerForwardInQueue (pos);

        tryStartThread();
        notify();
    }

    void removeTimer (Timer* t)
    {
        const auto pos = t->positionInQueue;
        const auto lastIndex = timers.size() - 1;

        jassert (pos <= lastIndex);
        jassert (timers[pos].timer == t);

        for (auto i = pos; i < lastIndex; ++i)
        {
            timers[i] = timers[i + 1];
            timers[i].timer->positionInQueue = i;
        }

        timers.pop_back();
    }

    void resetTimerCounter (Timer* t) noexcept
    {
        auto pos = t->positionInQueue;

        jassert (pos < timers.size());
        jassert (timers[pos].timer == t);

        const auto lastCountdown = timers[pos].countdownMs;
        const auto newCountdown = t->getTimerInterval();

        if (newCountdown != lastCountdown)
        {
            timers[pos].countdownMs = newCountdown;

            if (newCountdown > lastCountdown)
                shuffleTimerBackInQueue (pos);
            else
                shuffleTimerForwardInQueue (pos);

            notify();
        }
    }

    LockType lock;

    struct TimerCountdown
    {
        Timer* timer;
        int countdownMs;
    };

    std::vector<TimerCountdown> timers;
    bool isShuttingDown = false;

    WaitableEvent callbackArrived;

    struct CallTimersMessage final : public MessageManager::MessageBase
    {
        CallTimersMessage() = default;

        void messageCallback() override
        {
            Timer::callPendingTimersSynchronously();
        }
    };

    //==============================================================================
    void shuffleTimerBackInQueue (size_t pos)
    {
        auto numTimers = timers.size();

        if (pos < numTimers - 1)
        {
            auto t = timers[pos];

            for (;;)
            {
                auto next = pos + 1;

                if (next == numTimers || timers[next].countdownMs >= t.countdownMs)
                    break;

                timers[pos] = timers[next];
                timers[pos].timer->positionInQueue = pos;

                ++pos;
            }

            timers[pos] = t;
            t.timer->positionInQueue = pos;
        }
    }

    void shuffleTimerForwardInQueue (size_t pos)
    {
        if (pos > 0)
        {
            auto t = timers[pos];

            while (pos > 0)
            {
                auto& prev = timers[(size_t) pos - 1];

                if (prev.countdownMs <= t.countdownMs)
                    break;

                timers[pos] = prev;
                timers[pos].timer->positionInQueue = pos;

                --pos;
            }

            timers[pos] = t;
            t.timer->positionInQueue = pos;
        }
    }

    int getTimeUntilFirstTimer (int numMillisecsElapsed)
    {
        const LockType::ScopedLockType sl (lock);

        if (timers.empty())
            return 1000;

        for (auto& t : timers)
            t.countdownMs -= numMillisecsElapsed;

        return timers.front().countdownMs;
    }

    //==============================================================================
    void tryStartThread()
    {
        if (isThreadRunning()
            || timers.empty()
            || isShuttingDown
            || MessageManager::getInstanceWithoutCreating() == nullptr)
            return;

        startThread (Priority::high);
    }

    void messageManagerStarting() final
    {
        const LockType::ScopedLockType sl (lock);
        isShuttingDown = false;
        tryStartThread();
    }

    void messageManagerStopping() final
    {
        {
            const LockType::ScopedLockType sl (lock);
            isShuttingDown = true;
        }

        stopThread();
    }

    void exitSignalSent() final
    {
        callbackArrived.signal();
    }

    //==============================================================================
    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TimerThread)
};

//==============================================================================
Timer::Timer() noexcept {}
Timer::Timer (const Timer&) noexcept {}

Timer::~Timer()
{
    // If you're destroying a timer on a background thread, make sure the timer has
    // been stopped before execution reaches this point. A simple way to achieve this
    // is to add a call to `stopTimer()` to the destructor of your class which inherits
    // from Timer.
    jassert (! isTimerRunning()
             || MessageManager::getInstanceWithoutCreating() == nullptr
             || MessageManager::getInstanceWithoutCreating()->currentThreadHasLockedMessageManager());

    stopTimer();
}

void Timer::startTimer (int interval) noexcept
{
    timerThread->startTimer (this, jmax (1, interval));
}

void Timer::startTimerHz (int timerFrequencyHz) noexcept
{
    if (timerFrequencyHz > 0)
        startTimer (1000 / timerFrequencyHz);
    else
        stopTimer();
}

void Timer::stopTimer() noexcept
{
    timerThread->stopTimer (this);
}

void HIGGS_CALLTYPE Timer::callPendingTimersSynchronously()
{
    if (auto instance = SharedResourcePointer<TimerThread>::getSharedObjectWithoutCreating())
        (*instance)->callTimersSynchronously();
}

struct LambdaInvoker final : private Timer,
                             private DeletedAtShutdown
{
    LambdaInvoker (int milliseconds, std::function<void()> f)
        : function (std::move (f))
    {
        startTimer (milliseconds);
    }

    ~LambdaInvoker() final
    {
        stopTimer();
    }

    void timerCallback() final
    {
        NullCheckedInvocation::invoke (function);
        delete this;
    }

    std::function<void()> function;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LambdaInvoker)
};

void HIGGS_CALLTYPE Timer::callAfterDelay (int milliseconds, std::function<void()> f)
{
    new LambdaInvoker (milliseconds, std::move (f));
}

//==============================================================================
#if HIGGS_UNIT_TESTS

class TimerTests final : public UnitTest
{
public:
    TimerTests()
        : UnitTest ("Timer", UnitTestCategories::threads)
    {}

    void runTest() final
    {
        ScopedHiggsInitialiser_GUI libraryInitialiser;

        beginTest ("Start and stop a timer");
        {
            TestTimer timer;
            expect (! timer.isTimerRunning());
            expectEquals (timer.getTimerInterval(), 0);

            timer.startTimer (1000);
            expect (timer.isTimerRunning());
            expectEquals (timer.getTimerInterval(), 1000);

            timer.stopTimer();
            expect (! timer.isTimerRunning());
            expectEquals (timer.getTimerInterval(), 0);
        }

        beginTest ("Changing the interval of a running timer");
        {
            TestTimer timer;
            timer.startTimer (1000);
            expectEquals (timer.getTimerInterval(), 1000);

            timer.startTimer (50);
            expect (timer.isTimerRunning());
            expectEquals (timer.getTimerInterval(), 50);

            timer.stopTimer();
        }

        beginTest ("startTimerHz");
        {
            TestTimer timer;
            timer.startTimerHz (10);
            expect (timer.isTimerRunning());
            expectEquals (timer.getTimerInterval(), 100);

            timer.startTimerHz (0);
            expect (! timer.isTimerRunning());
        }

        beginTest ("startTimer and stopTimer can be called from a background thread");
        {
            TestTimer timer;
            std::atomic<bool> runningAfterStart { false };
            std::atomic<bool> runningAfterStop { true };

            WorkerThread worker {[&]
            {
                timer.startTimer (1000);
                runningAfterStart = timer.isTimerRunning();
                timer.stopTimer();
                runningAfterStop = timer.isTimerRunning();
            }};

            expect (worker.waitForThreadToExit (maximumTimeout));
            expect (runningAfterStart);
            expect (! runningAfterStop);
        }
    }

private:
    static constexpr Seconds maximumTimeout { 30 };

    class WorkerThread final : public Thread
    {
    public:
        explicit WorkerThread (std::function<void()> fn)
            : Thread ("TimerTests worker"), callback (std::move (fn))
        {
            startThread();
        }

        ~WorkerThread() final { stopThread(); }

        void run() final { callback(); }

    private:
        std::function<void()> callback;
    };

    class TestTimer final : public Timer
    {
    public:
        TestTimer() = default;
        ~TestTimer() override { stopTimer(); }

        void timerCallback() final {}
    };
};

static TimerTests timerTests;

#endif

} // namespace higgs
