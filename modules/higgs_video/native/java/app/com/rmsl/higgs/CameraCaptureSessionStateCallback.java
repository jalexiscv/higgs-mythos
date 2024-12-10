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

public class CameraCaptureSessionStateCallback extends CameraCaptureSession.StateCallback
{
    private native void cameraCaptureSessionActive (long host, CameraCaptureSession session);

    private native void cameraCaptureSessionClosed (long host, CameraCaptureSession session);

    private native void cameraCaptureSessionConfigureFailed (long host, CameraCaptureSession session);

    private native void cameraCaptureSessionConfigured (long host, CameraCaptureSession session);

    private native void cameraCaptureSessionReady (long host, CameraCaptureSession session);

    CameraCaptureSessionStateCallback (long hostToUse)
    {
        host = hostToUse;
    }

    @Override
    public void onActive (CameraCaptureSession session)
    {
        cameraCaptureSessionActive (host, session);
    }

    @Override
    public void onClosed (CameraCaptureSession session)
    {
        cameraCaptureSessionClosed (host, session);
    }

    @Override
    public void onConfigureFailed (CameraCaptureSession session)
    {
        cameraCaptureSessionConfigureFailed (host, session);
    }

    @Override
    public void onConfigured (CameraCaptureSession session)
    {
        cameraCaptureSessionConfigured (host, session);
    }

    @Override
    public void onReady (CameraCaptureSession session)
    {
        cameraCaptureSessionReady (host, session);
    }

    private long host;
}
