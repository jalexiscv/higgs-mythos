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

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

//==============================================================================
public class Receiver extends BroadcastReceiver
{
    @Override
    public void onReceive (Context context, Intent intent)
    {
        onBroadcastResultNative (intent.getIntExtra ("com.rmsl.higgs.HIGGS_REQUEST_CODE", 0));
    }

    private native void onBroadcastResultNative (int requestCode);
}
