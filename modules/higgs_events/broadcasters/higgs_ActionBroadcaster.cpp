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

class ActionBroadcaster::ActionMessage final : public MessageManager::MessageBase
{
public:
    ActionMessage (const ActionBroadcaster* ab,
                   const String& messageText, ActionListener* l) noexcept
        : broadcaster (const_cast<ActionBroadcaster*> (ab)),
          message (messageText),
          listener (l)
    {}

    void messageCallback() override
    {
        if (auto b = broadcaster.get())
            if (b->actionListeners.contains (listener))
                listener->actionListenerCallback (message);
    }

private:
    WeakReference<ActionBroadcaster> broadcaster;
    const String message;
    ActionListener* const listener;

    HIGGS_DECLARE_NON_COPYABLE (ActionMessage)
};

//==============================================================================
ActionBroadcaster::ActionBroadcaster()
{
    // are you trying to create this object before or after higgs has been initialised??
    HIGGS_ASSERT_MESSAGE_MANAGER_EXISTS
}

ActionBroadcaster::~ActionBroadcaster()
{
    // all event-based objects must be deleted BEFORE higgs is shut down!
    HIGGS_ASSERT_MESSAGE_MANAGER_EXISTS
}

void ActionBroadcaster::addActionListener (ActionListener* const listener)
{
    const ScopedLock sl (actionListenerLock);

    if (listener != nullptr)
        actionListeners.add (listener);
}

void ActionBroadcaster::removeActionListener (ActionListener* const listener)
{
    const ScopedLock sl (actionListenerLock);
    actionListeners.removeValue (listener);
}

void ActionBroadcaster::removeAllActionListeners()
{
    const ScopedLock sl (actionListenerLock);
    actionListeners.clear();
}

void ActionBroadcaster::sendActionMessage (const String& message) const
{
    const ScopedLock sl (actionListenerLock);

    for (int i = actionListeners.size(); --i >= 0;)
        (new ActionMessage (this, message, actionListeners.getUnchecked (i)))->post();
}

} // namespace higgs
