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

package com.rmsl.higgs;

import com.google.firebase.messaging.*;

public final class HiggsFirebaseMessagingService extends FirebaseMessagingService
{
    private native void firebaseRemoteMessageReceived (RemoteMessage message);
    private native void firebaseRemoteMessagesDeleted();
    private native void firebaseRemoteMessageSent (String messageId);
    private native void firebaseRemoteMessageSendError (String messageId, String error);

    @Override
    public void onMessageReceived (RemoteMessage message)
    {
        firebaseRemoteMessageReceived (message);
    }

    @Override
    public void onDeletedMessages()
    {
        firebaseRemoteMessagesDeleted();
    }

    @Override
    public void onMessageSent (String messageId)
    {
        firebaseRemoteMessageSent (messageId);
    }

    @Override
    public void onSendError (String messageId, Exception e)
    {
        firebaseRemoteMessageSendError (messageId, e.toString());
    }
}
