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

/** Utility class wrapping a single non-null callback called by a Timer.

    You can use the usual Timer functions to start and stop the TimedCallback. Deleting the
    TimedCallback will automatically stop the underlying Timer.

    With this class you can use the Timer facility without inheritance.

    @see Timer
    @tags{Events}
*/
class TimedCallback final : private Timer
{
public:
    /** Constructor. The passed in callback must be non-null. */
    explicit TimedCallback (std::function<void()> callbackIn)
        : callback (std::move (callbackIn))
    {
        jassert (callback);
    }

    /** Destructor. */
    ~TimedCallback() noexcept override { stopTimer(); }

    using Timer::startTimer;
    using Timer::startTimerHz;
    using Timer::stopTimer;
    using Timer::isTimerRunning;
    using Timer::getTimerInterval;

private:
    void timerCallback() override { callback(); }

    std::function<void()> callback;
};

} // namespace higgs
