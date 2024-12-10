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

import android.media.session.MediaSession;

import java.lang.String;

import android.os.Bundle;
import android.content.Intent;

import java.util.List;

//==============================================================================
public class MediaSessionCallback extends MediaSession.Callback
{
    private native void mediaSessionPause (long host);
    private native void mediaSessionPlay (long host);
    private native void mediaSessionPlayFromMediaId (long host, String mediaId, Bundle extras);
    private native void mediaSessionSeekTo (long host, long pos);
    private native void mediaSessionStop (long host);

    MediaSessionCallback (long hostToUse)
    {
        host = hostToUse;
    }

    @Override
    public void onPause ()
    {
        mediaSessionPause (host);
    }

    @Override
    public void onPlay ()
    {
        mediaSessionPlay (host);
    }

    @Override
    public void onPlayFromMediaId (String mediaId, Bundle extras)
    {
        mediaSessionPlayFromMediaId (host, mediaId, extras);
    }

    @Override
    public void onSeekTo (long pos)
    {
        mediaSessionSeekTo (host, pos);
    }

    @Override
    public void onStop ()
    {
        mediaSessionStop (host);
    }

    @Override
    public void onFastForward () {}

    @Override
    public boolean onMediaButtonEvent (Intent mediaButtonIntent)
    {
        return true;
    }

    @Override
    public void onRewind () {}

    @Override
    public void onSkipToNext () {}

    @Override
    public void onSkipToPrevious () {}

    @Override
    public void onSkipToQueueItem (long id) {}

    private long host;
}
