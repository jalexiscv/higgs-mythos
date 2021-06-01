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
/**
    A message that invokes a callback method when it gets delivered.

    You can use this class to fire off actions that you want to be performed later
    on the message thread.

    To use it, create a subclass of CallbackMessage which implements the messageCallback()
    method, then call post() to dispatch it. The event thread will then invoke your
    messageCallback() method later on, and will automatically delete the message object
    afterwards.

    Always create a new instance of a CallbackMessage on the heap, as it will be
    deleted automatically after the message has been delivered.

    Note that this class was essential back in the days before C++11, but in modern
    times you may prefer to use MessageManager::callAsync() with a lambda.

    @see MessageManager::callAsync, MessageListener, ActionListener, ChangeListener

    @tags{Events}
*/
class HIGGS_API  CallbackMessage   : public MessageManager::MessageBase
{
public:
    //==============================================================================
    CallbackMessage() = default;

    /** Destructor. */
    ~CallbackMessage() override = default;

    //==============================================================================
    /** Called when the message is delivered.

        You should implement this method and make it do whatever action you want
        to perform.

        Note that like all other messages, this object will be deleted immediately
        after this method has been invoked.
    */
    void messageCallback() override = 0;

private:
    // Avoid the leak-detector because for plugins, the host can unload our DLL with undelivered
    // messages still in the system event queue. These aren't harmful, but can cause annoying assertions.
    HIGGS_DECLARE_NON_COPYABLE (CallbackMessage)
};

} // namespace higgs
