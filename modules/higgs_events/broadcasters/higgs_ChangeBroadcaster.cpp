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

ChangeBroadcaster::ChangeBroadcaster() noexcept
{
    broadcastCallback.owner = this;
}

ChangeBroadcaster::~ChangeBroadcaster()
{
}

void ChangeBroadcaster::addChangeListener (ChangeListener* const listener)
{
    // Listeners can only be safely added when the event thread is locked
    // You can  use a MessageManagerLock if you need to call this from another thread.
    HIGGS_ASSERT_MESSAGE_MANAGER_IS_LOCKED

    changeListeners.add (listener);
    anyListeners = true;
}

void ChangeBroadcaster::removeChangeListener (ChangeListener* const listener)
{
    // Listeners can only be safely removed when the event thread is locked
    // You can  use a MessageManagerLock if you need to call this from another thread.
    HIGGS_ASSERT_MESSAGE_MANAGER_IS_LOCKED

    changeListeners.remove (listener);
    anyListeners = changeListeners.size() > 0;
}

void ChangeBroadcaster::removeAllChangeListeners()
{
    // Listeners can only be safely removed when the event thread is locked
    // You can  use a MessageManagerLock if you need to call this from another thread.
    HIGGS_ASSERT_MESSAGE_MANAGER_IS_LOCKED

    changeListeners.clear();
    anyListeners = false;
}

void ChangeBroadcaster::sendChangeMessage()
{
    if (anyListeners)
        broadcastCallback.triggerAsyncUpdate();
}

void ChangeBroadcaster::sendSynchronousChangeMessage()
{
    // This can only be called by the event thread.
    HIGGS_ASSERT_MESSAGE_MANAGER_IS_LOCKED

    broadcastCallback.cancelPendingUpdate();
    callListeners();
}

void ChangeBroadcaster::dispatchPendingMessages()
{
    broadcastCallback.handleUpdateNowIfNeeded();
}

void ChangeBroadcaster::callListeners()
{
    changeListeners.call ([this] (ChangeListener& l) { l.changeListenerCallback (this); });
}

//==============================================================================
ChangeBroadcaster::ChangeBroadcasterCallback::ChangeBroadcasterCallback()
    : owner (nullptr)
{
}

void ChangeBroadcaster::ChangeBroadcasterCallback::handleAsyncUpdate()
{
    jassert (owner != nullptr);
    owner->callListeners();
}

} // namespace higgs
