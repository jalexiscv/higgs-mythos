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

import android.app.DialogFragment;
import android.content.Intent;
import android.os.Bundle;

public class FragmentOverlay extends DialogFragment
{
    @Override
    public void onCreate (Bundle state)
    {
        super.onCreate (state);
        cppThis = getArguments ().getLong ("cppThis");

        if (cppThis != 0)
            onCreateNative (cppThis, state);
    }

    @Override
    public void onStart ()
    {
        super.onStart ();

        if (cppThis != 0)
            onStartNative (cppThis);
    }

    public void onRequestPermissionsResult (int requestCode,
                                            String[] permissions,
                                            int[] grantResults)
    {
        if (cppThis != 0)
            onRequestPermissionsResultNative (cppThis, requestCode,
                    permissions, grantResults);
    }

    @Override
    public void onActivityResult (int requestCode, int resultCode, Intent data)
    {
        if (cppThis != 0)
            onActivityResultNative (cppThis, requestCode, resultCode, data);
    }

    public void close ()
    {
        cppThis = 0;
        dismiss ();
    }

    //==============================================================================
    private long cppThis = 0;

    private native void onActivityResultNative (long myself, int requestCode, int resultCode, Intent data);
    private native void onCreateNative (long myself, Bundle state);
    private native void onStartNative (long myself);
    private native void onRequestPermissionsResultNative (long myself, int requestCode,
                                                          String[] permissions, int[] grantResults);
}
