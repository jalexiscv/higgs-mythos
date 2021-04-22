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

WaitableEvent::WaitableEvent (bool manualReset) noexcept
    : useManualReset (manualReset)
{
}

void WaitableEvent::wait() const
{
    std::unique_lock<std::mutex> lock (mutex);

    if (! triggered)
        condition.wait (lock, [this] { return triggered == true; });

    if (! useManualReset)
        reset();
}

bool WaitableEvent::wait (Seconds timeOut) const
{
    // Unlike wait (double), a negative timeout is not supported. To wait
    // indefinitely, call wait() with no arguments.
    jassert (timeOut >= Seconds { 0.0 });

    std::unique_lock<std::mutex> lock (mutex);

    if (! triggered && ! condition.wait_for (lock, timeOut, [this] { return triggered == true; }))
        return false;

    if (! useManualReset)
        reset();

    return true;
}

bool WaitableEvent::wait (double timeOutMilliseconds) const
{
    if (timeOutMilliseconds >= 0.0)
        return wait (Milliseconds { timeOutMilliseconds });

    wait();
    return true;
}

void WaitableEvent::signal() const
{
    std::lock_guard<std::mutex> lock (mutex);

    triggered = true;
    condition.notify_all();
}

void WaitableEvent::reset() const
{
    triggered = false;
}

} // namespace higgs
