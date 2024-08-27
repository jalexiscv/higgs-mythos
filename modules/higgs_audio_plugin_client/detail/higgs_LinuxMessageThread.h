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

#if HIGGS_LINUX || HIGGS_BSD

namespace higgs::detail
{

// Implemented in higgs_Messaging_linux.cpp
bool dispatchNextMessageOnSystemQueue (bool returnIfNoPendingMessages);

class MessageThread : public Thread
{
public:
    MessageThread() : Thread (SystemStats::getHIGGSVersion() + ": Plugin Message Thread")
    {
        start();
    }

    ~MessageThread() override
    {
        MessageManager::getInstance()->stopDispatchLoop();
        stop();
    }

    void start()
    {
        startThread (Priority::high);

        // Wait for setCurrentThreadAsMessageThread() and getInstance to be executed
        // before leaving this method
        threadInitialised.wait (10000);
    }

    void stop()
    {
        signalThreadShouldExit();
        stopThread (-1);
    }

    bool isRunning() const noexcept  { return isThreadRunning(); }

    void run() override
    {
        MessageManager::getInstance()->setCurrentThreadAsMessageThread();
        XWindowSystem::getInstance();

        threadInitialised.signal();

        while (! threadShouldExit())
        {
            if (! dispatchNextMessageOnSystemQueue (true))
                Thread::sleep (1);
        }
    }

private:
    WaitableEvent threadInitialised;
    HIGGS_DECLARE_NON_MOVEABLE (MessageThread)
    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MessageThread)
};

//==============================================================================
class HostDrivenEventLoop
{
public:
    HostDrivenEventLoop()
    {
        messageThread->stop();
        MessageManager::getInstance()->setCurrentThreadAsMessageThread();
    }

    void processPendingEvents()
    {
        MessageManager::getInstance()->setCurrentThreadAsMessageThread();

        for (;;)
            if (! dispatchNextMessageOnSystemQueue (true))
                return;
    }

    ~HostDrivenEventLoop()
    {
        messageThread->start();
    }

private:
    SharedResourcePointer<MessageThread> messageThread;
};

} // namespace higgs::detail

#endif
