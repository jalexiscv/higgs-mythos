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

import android.hardware.camera2.CameraCaptureSession;
import android.hardware.camera2.CaptureRequest;
import android.hardware.camera2.TotalCaptureResult;
import android.hardware.camera2.CaptureFailure;
import android.hardware.camera2.CaptureResult;

public class CameraCaptureSessionCaptureCallback extends CameraCaptureSession.CaptureCallback
{
    private native void cameraCaptureSessionCaptureCompleted (long host, boolean isPreview, CameraCaptureSession session, CaptureRequest request, TotalCaptureResult result);
    private native void cameraCaptureSessionCaptureFailed (long host, boolean isPreview, CameraCaptureSession session, CaptureRequest request, CaptureFailure failure);
    private native void cameraCaptureSessionCaptureProgressed (long host, boolean isPreview, CameraCaptureSession session, CaptureRequest request, CaptureResult partialResult);
    private native void cameraCaptureSessionCaptureStarted (long host, boolean isPreview, CameraCaptureSession session, CaptureRequest request, long timestamp, long frameNumber);
    private native void cameraCaptureSessionCaptureSequenceAborted (long host, boolean isPreview, CameraCaptureSession session, int sequenceId);
    private native void cameraCaptureSessionCaptureSequenceCompleted (long host, boolean isPreview, CameraCaptureSession session, int sequenceId, long frameNumber);

    CameraCaptureSessionCaptureCallback (long hostToUse, boolean shouldBePreview)
    {
        host = hostToUse;
        preview = shouldBePreview;
    }

    @Override
    public void onCaptureCompleted (CameraCaptureSession session, CaptureRequest request,
                                    TotalCaptureResult result)
    {
        cameraCaptureSessionCaptureCompleted (host, preview, session, request, result);
    }

    @Override
    public void onCaptureFailed (CameraCaptureSession session, CaptureRequest request, CaptureFailure failure)
    {
        cameraCaptureSessionCaptureFailed (host, preview, session, request, failure);
    }

    @Override
    public void onCaptureProgressed (CameraCaptureSession session, CaptureRequest request,
                                     CaptureResult partialResult)
    {
        cameraCaptureSessionCaptureProgressed (host, preview, session, request, partialResult);
    }

    @Override
    public void onCaptureSequenceAborted (CameraCaptureSession session, int sequenceId)
    {
        cameraCaptureSessionCaptureSequenceAborted (host, preview, session, sequenceId);
    }

    @Override
    public void onCaptureSequenceCompleted (CameraCaptureSession session, int sequenceId, long frameNumber)
    {
        cameraCaptureSessionCaptureSequenceCompleted (host, preview, session, sequenceId, frameNumber);
    }

    @Override
    public void onCaptureStarted (CameraCaptureSession session, CaptureRequest request, long timestamp,
                                  long frameNumber)
    {
        cameraCaptureSessionCaptureStarted (host, preview, session, request, timestamp, frameNumber);
    }

    private long host;
    private boolean preview;
}
