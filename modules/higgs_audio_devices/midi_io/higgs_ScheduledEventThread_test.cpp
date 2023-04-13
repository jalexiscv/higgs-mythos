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

class ScheduledEventQueueTests : public UnitTest
{
public:
    ScheduledEventQueueTests()
        : UnitTest ("ScheduledEventQueue", UnitTestCategories::midi)
    {
    }

    void runTest() override
    {
        struct Event
        {
            uint32_t getTimeStamp() const { return time; }
            uint32_t time;
        };

        struct LocalTimeProvider
        {
            uint32_t currentTime;

            uint32_t getMillisecondCounter() const
            {
                return currentTime;
            }
        };

        LocalTimeProvider timeProvider;
        ScheduledEventQueue<Event, LocalTimeProvider> queue { &timeProvider };

        testCase ("An empty queue returns an infinite timeout when popping an event", [&]
        {
            expect (queue.empty());
            const auto popped = queue.popEvent();
            auto* timeout = std::get_if<uint32_t> (&popped);
            expect (timeout != nullptr);

            if (timeout != nullptr)
                expect (*timeout == std::numeric_limits<uint32_t>::max());
        });

        testCase ("Adding an event to an empty queue makes that event the next event", [&]
        {
            constexpr uint32_t time = 100;
            queue.addEvent (Event { time });
            expect (queue.getTimeOfNextEvent() == time);

            queue.clearAllPendingMessages();
            expect (queue.empty());
        });

        testCase ("Adding an event to a single-element queue remains sorted", [&]
        {
            queue.addEvent (Event { 100 });
            queue.addEvent (Event { 200 });

            expect (queue.getTimeOfNextEvent() == 100);

            queue.clearAllPendingMessages();

            queue.addEvent (Event { 100 });
            queue.addEvent (Event { 50 });

            expect (queue.getTimeOfNextEvent() == 50);

            queue.clearAllPendingMessages();
        });

        testCase ("Adding an event when the 'next' even is slightly in the past doesn't drop or postpone the next event", [&]
        {
            timeProvider.currentTime = 0;
            queue.addEvent (Event { 100 });
            timeProvider.currentTime = 110;
            queue.addEvent (Event { 120 });

            expect (queue.getTimeOfNextEvent() == 100);

            queue.clearAllPendingMessages();
        });

        testCase ("Popping an event returns the next event if it is not too far in the past or future", [&]
        {
            constexpr uint32_t eventTime = 100;
            constexpr uint32_t leeway = 20;

            timeProvider.currentTime = 0;
            queue.addEvent (Event { eventTime });
            auto popped = queue.popEvent (leeway);

            // In the future, this should be a continue
            const auto* timeout = std::get_if<uint32_t> (&popped);
            expect (timeout != nullptr);

            if (timeout != nullptr)
                expect (*timeout == eventTime - leeway);

            expect (! queue.empty());

            // Within our leeway
            timeProvider.currentTime = 80;
            popped = queue.popEvent (leeway);

            const auto* got = std::get_if<Event> (&popped);
            expect (got != nullptr);

            if (got != nullptr)
                expect (got->time == eventTime);

            expect (queue.empty());

            timeProvider.currentTime = 0;
            queue.addEvent (Event { eventTime });

            // Exactly on time
            timeProvider.currentTime = 100;
            popped = queue.popEvent (leeway);

            const auto* current = std::get_if<Event> (&popped);
            expect (current != nullptr);

            if (current != nullptr)
                expect (current->time == eventTime);

            expect (queue.empty());

            timeProvider.currentTime = 0;
            queue.addEvent (Event { eventTime });

            // Missed deadline by a little
            timeProvider.currentTime = 200;
            popped = queue.popEvent (leeway);

            const auto* delayed = std::get_if<Event> (&popped);
            expect (delayed != nullptr);

            if (delayed != nullptr)
                expect (delayed->time == eventTime);

            expect (queue.empty());

            timeProvider.currentTime = 0;
            queue.addEvent (Event { eventTime });

            // Missed deadline by a lot
            timeProvider.currentTime = 1'000;
            popped = queue.popEvent (leeway);

            const auto* missed = std::get_if<uint32_t> (&popped);
            expect (missed != nullptr);

            if (missed != nullptr)
                expect (*missed == 0);

            expect (queue.empty());
        });

        testCase ("Added events are sorted relative to the time provider", [&]
        {
            timeProvider.currentTime = 0;
            queue.addEvent (Event { std::numeric_limits<uint32_t>::max() - 10 });
            expect (queue.getTimeOfNextEvent() == std::numeric_limits<uint32_t>::max() - 10);

            queue.addEvent (Event { std::numeric_limits<uint32_t>::max() - 11 });
            expect (queue.getTimeOfNextEvent() == std::numeric_limits<uint32_t>::max() - 11);

            queue.clearAllPendingMessages();

            timeProvider.currentTime = std::numeric_limits<uint32_t>::max() - 20;

            const auto numMsInTwoHours = 1000 * 60 * 60 * 2;
            queue.addEvent (Event { std::numeric_limits<uint32_t>::max() - numMsInTwoHours });
            expect (queue.getTimeOfNextEvent() == std::numeric_limits<uint32_t>::max() - numMsInTwoHours);

            queue.addEvent (Event { std::numeric_limits<uint32_t>::max() - 10 });
            expect (queue.getTimeOfNextEvent() == std::numeric_limits<uint32_t>::max() - 10);
        });
    }
};

static ScheduledEventQueueTests scheduledEventQueueTests;

} // namespace higgs
