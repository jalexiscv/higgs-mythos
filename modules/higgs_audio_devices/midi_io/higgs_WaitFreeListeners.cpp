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

#if HIGGS_UNIT_TESTS

class WaitFreeListenersTest : public UnitTest
{
public:
    struct Listener
    {
        virtual ~Listener() = default;
        virtual void notify() = 0;
    };

    WaitFreeListenersTest() : UnitTest ("WaitFreeListeners", UnitTestCategories::midi) {}

    void runTest() override
    {
        using Receivers = WaitFreeListeners<Listener>;

        struct CountingReceiver : public Listener
        {
            void notify() override { ++numCalls; }
            int numCalls = 0;
        };

        testCase ("Adding and immediately removing a receiver works", [&]
        {
            Receivers receivers;
            CountingReceiver receiver;
            receivers.add (receiver);

            expect (receiver.numCalls == 0);

            receivers.remove (receiver);

            expect (receiver.numCalls == 0);
        });

        testCase ("Notifying receivers works", [&]
        {
            Receivers receivers;
            std::array<CountingReceiver, 63> receiverArray;

            for (size_t i = 0; i < receiverArray.size(); ++i)
            {
                receivers.add (receiverArray[i]);

                expect (receiverArray[i].numCalls == 0);

                receivers.call ([] (auto& l) { l.notify(); });

                expect (receiverArray[i].numCalls == 1);
            }

            expect ((size_t) receiverArray.front().numCalls == receiverArray.size());
        });

        testCase ("Adding and removing receivers while notifying them works", [&]
        {
            std::atomic<bool> exit { false };
            Receivers receivers;

            std::thread notifier
            {
                [&]
                {
                    while (! exit)
                        receivers.call ([] (auto& l) { l.notify(); });
                }
            };

            std::vector<std::thread> responders;

            for (size_t i = 0; i < 10; ++i)
            {
                responders.emplace_back ([&]
                {
                    for (auto attempt = 0; attempt < 100; ++attempt)
                    {
                        CountingReceiver counter;

                        receivers.add (counter);
                        receivers.remove (counter);
                    }
                });
            }

            for (auto& t : responders)
                t.join();

            exit = true;
            notifier.join();
        });

        testCase ("Concurrent calls work", [&]
        {
            Receivers receivers;

            class StageGate
            {
            public:
                void waitForStageThenProceed (int s)
                {
                    std::unique_lock lock { mutex };
                    condvar.wait (lock, [&] { return stage == s; });
                    stage = s + 1;
                    condvar.notify_all();
                }

            private:
                std::condition_variable condvar;
                std::mutex mutex;
                int stage = 0;
            };

            struct TestReceiver : public Listener { void notify() override {} };

            TestReceiver testReceiver;
            receivers.add (testReceiver);

            std::vector<std::thread> notifiers;

            StageGate gate;

            notifiers.emplace_back ([&]
            {
                receivers.call ([&] (auto&)
                {
                    gate.waitForStageThenProceed (1);
                    gate.waitForStageThenProceed (3);
                });

                gate.waitForStageThenProceed (4);
            });

            notifiers.emplace_back ([&]
            {
                gate.waitForStageThenProceed (2);

                receivers.call ([&] (auto&)
                {
                    gate.waitForStageThenProceed (5);
                });
            });

            gate.waitForStageThenProceed (0);
            gate.waitForStageThenProceed (6);

            for (auto& t : notifiers)
                t.join();

            receivers.remove (testReceiver);
        });
    }
};

static WaitFreeListenersTest receiversTest;

#endif

} // namespace higgs
