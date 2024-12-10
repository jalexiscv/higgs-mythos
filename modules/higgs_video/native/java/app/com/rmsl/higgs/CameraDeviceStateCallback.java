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

import android.hardware.camera2.CameraDevice;

public class CameraDeviceStateCallback extends CameraDevice.StateCallback
{
    private native void cameraDeviceStateClosed (long host, CameraDevice camera);
    private native void cameraDeviceStateDisconnected (long host, CameraDevice camera);
    private native void cameraDeviceStateError (long host, CameraDevice camera, int error);
    private native void cameraDeviceStateOpened (long host, CameraDevice camera);

    CameraDeviceStateCallback (long hostToUse)
    {
        host = hostToUse;
    }

    @Override
    public void onClosed (CameraDevice camera)
    {
        cameraDeviceStateClosed (host, camera);
    }

    @Override
    public void onDisconnected (CameraDevice camera)
    {
        cameraDeviceStateDisconnected (host, camera);
    }

    @Override
    public void onError (CameraDevice camera, int error)
    {
        cameraDeviceStateError (host, camera, error);
    }

    @Override
    public void onOpened (CameraDevice camera)
    {
        cameraDeviceStateOpened (host, camera);
    }

    private long host;
}
