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

//==============================================================================
/* An internal message pump class used in OSX and iOS. */
class MessageQueue
{
public:
    MessageQueue()
    {
       #if HIGGS_IOS
        runLoop = CFRunLoopGetCurrent();
       #else
        runLoop = CFRunLoopGetMain();
       #endif

        CFRunLoopSourceContext sourceContext;
        zerostruct (sourceContext); // (can't use "= { 0 }" on this object because it's typedef'ed as a C struct)
        sourceContext.info = this;
        sourceContext.perform = runLoopSourceCallback;
        runLoopSource.reset (CFRunLoopSourceCreate (kCFAllocatorDefault, 1, &sourceContext));
        CFRunLoopAddSource (runLoop, runLoopSource.get(), kCFRunLoopCommonModes);
    }

    ~MessageQueue() noexcept
    {
        CFRunLoopRemoveSource (runLoop, runLoopSource.get(), kCFRunLoopCommonModes);
        CFRunLoopSourceInvalidate (runLoopSource.get());
    }

    void post (MessageManager::MessageBase* const message)
    {
        messages.add (message);
        wakeUp();
    }

private:
    ReferenceCountedArray<MessageManager::MessageBase, CriticalSection> messages;
    CFRunLoopRef runLoop;
    CFUniquePtr<CFRunLoopSourceRef> runLoopSource;

    void wakeUp() noexcept
    {
        CFRunLoopSourceSignal (runLoopSource.get());
        CFRunLoopWakeUp (runLoop);
    }

    bool deliverNextMessage()
    {
        const MessageManager::MessageBase::Ptr nextMessage (messages.removeAndReturn (0));

        if (nextMessage == nullptr)
            return false;

        HIGGS_AUTORELEASEPOOL
        {
            HIGGS_TRY
            {
                nextMessage->messageCallback();
            }
            HIGGS_CATCH_EXCEPTION
        }

        return true;
    }

    void runLoopCallback() noexcept
    {
        for (int i = 4; --i >= 0;)
            if (! deliverNextMessage())
                return;

        wakeUp();
    }

    static void runLoopSourceCallback (void* info) noexcept
    {
        static_cast<MessageQueue*> (info)->runLoopCallback();
    }
};

} // namespace higgs
