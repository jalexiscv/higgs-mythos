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

#if ! HIGGS_WASM

NamedPipe::NamedPipe() {}

NamedPipe::~NamedPipe()
{
    close();
}

bool NamedPipe::openExisting (const String& pipeName)
{
    close();

    ScopedWriteLock sl (lock);
    currentPipeName = pipeName;
    return openInternal (pipeName, false, false);
}

bool NamedPipe::isOpen() const
{
    ScopedReadLock sl (lock);
    return pimpl != nullptr;
}

bool NamedPipe::createNewPipe (const String& pipeName, bool mustNotExist)
{
    close();

    ScopedWriteLock sl (lock);
    currentPipeName = pipeName;
    return openInternal (pipeName, true, mustNotExist);
}

String NamedPipe::getName() const
{
    ScopedReadLock sl (lock);
    return currentPipeName;
}

// other methods for this class are implemented in the platform-specific files


//==============================================================================
//==============================================================================
#if HIGGS_UNIT_TESTS

class NamedPipeTests final : public UnitTest
{
public:
    //==============================================================================
    NamedPipeTests()
        : UnitTest ("NamedPipe", UnitTestCategories::networking)
    {}

    void runTest() override
    {
        const auto pid =
       #if HIGGS_WINDOWS
            _getpid();
       #else
            getpid();
       #endif

        const auto pipeName = "TestPipe-"
                            + String::toHexString (pid)
                            + "-"
                            + String::toHexString (Time::currentTimeMillis());
        beginTest ("Pre test cleanup");
        {
            NamedPipe pipe;
            expect (pipe.createNewPipe (pipeName, false));
        }

        beginTest ("Create pipe");
        {
            NamedPipe pipe;
            expect (! pipe.isOpen());

            expect (pipe.createNewPipe (pipeName, true));
            expect (pipe.isOpen());

            expect (pipe.createNewPipe (pipeName, false));
            expect (pipe.isOpen());

            NamedPipe otherPipe;
            expect (! otherPipe.createNewPipe (pipeName, true));
            expect (! otherPipe.isOpen());
        }

        beginTest ("Existing pipe");
        {
            NamedPipe pipe;

            expect (! pipe.openExisting (pipeName));
            expect (! pipe.isOpen());

            expect (pipe.createNewPipe (pipeName, true));

            NamedPipe otherPipe;
            expect (otherPipe.openExisting (pipeName));
            expect (otherPipe.isOpen());
        }

        int sendData = 4684682;

        beginTest ("Receive message created pipe");
        {
            NamedPipe pipe;
            expect (pipe.createNewPipe (pipeName, true));

            WaitableEvent senderFinished;
            SenderThread sender (pipeName, false, senderFinished, sendData);

            sender.startThread();

            int recvData = -1;
            auto bytesRead = pipe.read (&recvData, sizeof (recvData), 2000);

            expect (senderFinished.wait (4000));

            expectEquals (bytesRead, (int) sizeof (recvData));
            expectEquals (sender.result, (int) sizeof (sendData));
            expectEquals (recvData, sendData);
        }

        beginTest ("Receive message existing pipe");
        {
            WaitableEvent senderFinished;
            SenderThread sender (pipeName, true, senderFinished, sendData);

            NamedPipe pipe;
            expect (pipe.openExisting (pipeName));

            sender.startThread();

            int recvData = -1;
            auto bytesRead = pipe.read (&recvData, sizeof (recvData), 2000);

            expect (senderFinished.wait (4000));

            expectEquals (bytesRead, (int) sizeof (recvData));
            expectEquals (sender.result, (int) sizeof (sendData));
            expectEquals (recvData, sendData);
        }

        beginTest ("Send message created pipe");
        {
            NamedPipe pipe;
            expect (pipe.createNewPipe (pipeName, true));

            WaitableEvent receiverFinished;
            ReceiverThread receiver (pipeName, false, receiverFinished);

            receiver.startThread();

            auto bytesWritten = pipe.write (&sendData, sizeof (sendData), 2000);

            expect (receiverFinished.wait (4000));

            expectEquals (bytesWritten, (int) sizeof (sendData));
            expectEquals (receiver.result, (int) sizeof (receiver.recvData));
            expectEquals (receiver.recvData, sendData);
        }

        beginTest ("Send message existing pipe");
        {
            WaitableEvent receiverFinished;
            ReceiverThread receiver (pipeName, true, receiverFinished);

            NamedPipe pipe;
            expect (pipe.openExisting (pipeName));

            receiver.startThread();

            auto bytesWritten = pipe.write (&sendData, sizeof (sendData), 2000);

            expect (receiverFinished.wait (4000));

            expectEquals (bytesWritten, (int) sizeof (sendData));
            expectEquals (receiver.result, (int) sizeof (receiver.recvData));
            expectEquals (receiver.recvData, sendData);
        }
    }

private:
    //==============================================================================
    struct NamedPipeThread : public Thread
    {
        NamedPipeThread (const String& tName, const String& pName,
                         bool shouldCreatePipe, WaitableEvent& completed)
            : Thread (tName), pipeName (pName), workCompleted (completed)
        {
            if (shouldCreatePipe)
                pipe.createNewPipe (pipeName);
            else
                pipe.openExisting (pipeName);
        }

        NamedPipe pipe;
        const String& pipeName;
        WaitableEvent& workCompleted;

        int result = -2;
    };

    //==============================================================================
    struct SenderThread final : public NamedPipeThread
    {
        SenderThread (const String& pName, bool shouldCreatePipe,
                      WaitableEvent& completed, int sData)
            : NamedPipeThread ("NamePipeSender", pName, shouldCreatePipe, completed),
              sendData (sData)
        {}

        ~SenderThread() override
        {
            stopThread (100);
        }

        void run() override
        {
            result = pipe.write (&sendData, sizeof (sendData), 2000);
            workCompleted.signal();
        }

        const int sendData;
    };

    //==============================================================================
    struct ReceiverThread final : public NamedPipeThread
    {
        ReceiverThread (const String& pName, bool shouldCreatePipe,
                        WaitableEvent& completed)
            : NamedPipeThread ("NamePipeReceiver", pName, shouldCreatePipe, completed)
        {}

        ~ReceiverThread() override
        {
            stopThread (100);
        }

        void run() override
        {
            result = pipe.read (&recvData, sizeof (recvData), 2000);
            workCompleted.signal();
        }

        int recvData = -2;
    };
};

static NamedPipeTests namedPipeTests;

#endif
#endif

} // namespace higgs
