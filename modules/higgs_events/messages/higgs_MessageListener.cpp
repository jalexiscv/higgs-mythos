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

Message::Message() noexcept {}
Message::~Message() {}

void Message::messageCallback()
{
    if (auto* r = recipient.get())
        r->handleMessage (*this);
}

MessageListener::MessageListener() noexcept
{
    // Are you trying to create a messagelistener before or after higgs has been initialised??
    HIGGS_ASSERT_MESSAGE_MANAGER_EXISTS
}

MessageListener::~MessageListener()
{
    masterReference.clear();
}

void MessageListener::postMessage (Message* const message) const
{
    message->recipient = const_cast<MessageListener*> (this);
    message->post();
}

} // namespace higgs
