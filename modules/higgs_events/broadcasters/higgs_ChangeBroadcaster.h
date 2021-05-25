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
    Holds a list of ChangeListeners, and sends messages to them when instructed.

    @see ChangeListener

    @tags{Events}
*/
class HIGGS_API  ChangeBroadcaster
{
public:
    //==============================================================================
    /** Creates an ChangeBroadcaster. */
    ChangeBroadcaster() noexcept;

    /** Destructor. */
    virtual ~ChangeBroadcaster();

    //==============================================================================
    /** Registers a listener to receive change callbacks from this broadcaster.
        Trying to add a listener that's already on the list will have no effect.
    */
    void addChangeListener (ChangeListener* listener);

    /** Unregisters a listener from the list.
        If the listener isn't on the list, this won't have any effect.
    */
    void removeChangeListener (ChangeListener* listener);

    /** Removes all listeners from the list. */
    void removeAllChangeListeners();

    //==============================================================================
    /** Causes an asynchronous change message to be sent to all the registered listeners.

        The message will be delivered asynchronously by the main message thread, so this
        method will return immediately. To call the listeners synchronously use
        sendSynchronousChangeMessage().
    */
    void sendChangeMessage();

    /** Sends a synchronous change message to all the registered listeners.

        This will immediately call all the listeners that are registered. For thread-safety
        reasons, you must only call this method on the main message thread.

        @see dispatchPendingMessages
    */
    void sendSynchronousChangeMessage();

    /** If a change message has been sent but not yet dispatched, this will call
        sendSynchronousChangeMessage() to make the callback immediately.

        For thread-safety reasons, you must only call this method on the main message thread.
    */
    void dispatchPendingMessages();

private:
    //==============================================================================
    class ChangeBroadcasterCallback  : public AsyncUpdater
    {
    public:
        ChangeBroadcasterCallback();
        ~ChangeBroadcasterCallback() override { cancelPendingUpdate(); }
        void handleAsyncUpdate() override;

        ChangeBroadcaster* owner;
    };

    friend class ChangeBroadcasterCallback;
    ChangeBroadcasterCallback broadcastCallback;
    ListenerList <ChangeListener> changeListeners;

    std::atomic<bool> anyListeners { false };

    void callListeners();

    HIGGS_DECLARE_NON_COPYABLE (ChangeBroadcaster)
};

} // namespace higgs
