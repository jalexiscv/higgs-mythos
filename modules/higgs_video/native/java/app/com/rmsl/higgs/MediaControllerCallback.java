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

import android.media.session.MediaController;
import android.media.session.MediaSession;
import android.media.MediaMetadata;
import android.media.session.PlaybackState;

import java.util.List;

//==============================================================================
public class MediaControllerCallback extends MediaController.Callback
{
    private native void mediaControllerAudioInfoChanged (long host, MediaController.PlaybackInfo info);
    private native void mediaControllerMetadataChanged (long host, MediaMetadata metadata);
    private native void mediaControllerPlaybackStateChanged (long host, PlaybackState state);
    private native void mediaControllerSessionDestroyed (long host);

    MediaControllerCallback (long hostToUse)
    {
        host = hostToUse;
    }

    @Override
    public void onAudioInfoChanged (MediaController.PlaybackInfo info)
    {
        mediaControllerAudioInfoChanged (host, info);
    }

    @Override
    public void onMetadataChanged (MediaMetadata metadata)
    {
        mediaControllerMetadataChanged (host, metadata);
    }

    @Override
    public void onPlaybackStateChanged (PlaybackState state)
    {
        mediaControllerPlaybackStateChanged (host, state);
    }

    @Override
    public void onQueueChanged (List<MediaSession.QueueItem> queue)
    {
    }

    @Override
    public void onSessionDestroyed ()
    {
        mediaControllerSessionDestroyed (host);
    }

    private long host;
}
