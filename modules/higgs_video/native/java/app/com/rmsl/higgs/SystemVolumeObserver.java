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

import android.database.ContentObserver;
import android.app.Activity;
import android.net.Uri;

//==============================================================================
public class SystemVolumeObserver extends ContentObserver
{
    private native void mediaSessionSystemVolumeChanged (long host);

    SystemVolumeObserver (Activity activityToUse, long hostToUse)
    {
        super (null);

        activity = activityToUse;
        host = hostToUse;
    }

    void setEnabled (boolean shouldBeEnabled)
    {
        if (shouldBeEnabled)
            activity.getApplicationContext ().getContentResolver ().registerContentObserver (android.provider.Settings.System.CONTENT_URI, true, this);
        else
            activity.getApplicationContext ().getContentResolver ().unregisterContentObserver (this);
    }

    @Override
    public void onChange (boolean selfChange, Uri uri)
    {
        if (uri.toString ().startsWith ("content://settings/system/volume_music"))
            mediaSessionSystemVolumeChanged (host);
    }

    private Activity activity;
    private long host;
}
