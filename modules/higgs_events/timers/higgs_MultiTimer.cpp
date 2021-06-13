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

struct MultiTimerCallback final : public Timer
{
    MultiTimerCallback (const int tid, MultiTimer& mt) noexcept
        : owner (mt), timerID (tid)
    {
    }

    void timerCallback() override
    {
        owner.timerCallback (timerID);
    }

    MultiTimer& owner;
    const int timerID;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MultiTimerCallback)
};

//==============================================================================
MultiTimer::MultiTimer() noexcept {}
MultiTimer::MultiTimer (const MultiTimer&) noexcept {}

MultiTimer::~MultiTimer()
{
    const SpinLock::ScopedLockType sl (timerListLock);
    timers.clear();
}

//==============================================================================
Timer* MultiTimer::getCallback (int timerID) const noexcept
{
    for (int i = timers.size(); --i >= 0;)
    {
        MultiTimerCallback* const t = static_cast<MultiTimerCallback*> (timers.getUnchecked (i));

        if (t->timerID == timerID)
            return t;
    }

    return nullptr;
}

void MultiTimer::startTimer (const int timerID, const int intervalInMilliseconds) noexcept
{
    const SpinLock::ScopedLockType sl (timerListLock);

    Timer* timer = getCallback (timerID);

    if (timer == nullptr)
        timers.add (timer = new MultiTimerCallback (timerID, *this));

    timer->startTimer (intervalInMilliseconds);
}

void MultiTimer::stopTimer (const int timerID) noexcept
{
    const SpinLock::ScopedLockType sl (timerListLock);

    if (Timer* const t = getCallback (timerID))
        t->stopTimer();
}

bool MultiTimer::isTimerRunning (const int timerID) const noexcept
{
    const SpinLock::ScopedLockType sl (timerListLock);

    if (Timer* const t = getCallback (timerID))
        return t->isTimerRunning();

    return false;
}

int MultiTimer::getTimerInterval (const int timerID) const noexcept
{
    const SpinLock::ScopedLockType sl (timerListLock);

    if (Timer* const t = getCallback (timerID))
        return t->getTimerInterval();

    return 0;
}

} // namespace higgs
