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

import static android.view.View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN;
import static android.view.View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION;
import static android.view.View.SYSTEM_UI_FLAG_LAYOUT_STABLE;

import android.app.Activity;
import android.os.Build;
import android.os.Bundle;
import android.content.Intent;
import android.view.View;

//==============================================================================
public class HiggsActivity   extends Activity
{
    private native void appNewIntent (Intent intent);
    private native void appOnResume();

    @SuppressWarnings ("deprecation")
    private void initEdgeToEdge()
    {
        if (Build.VERSION.SDK_INT < 35)
        {
            View decorView = getWindow().getDecorView();

            final int flags = Build.VERSION.SDK_INT < 30
                    ? (  SYSTEM_UI_FLAG_LAYOUT_STABLE
                       | SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                       | SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN)
                    : 0;

            decorView.setSystemUiVisibility (decorView.getSystemUiVisibility() | flags);
        }

        if (30 <= Build.VERSION.SDK_INT)
            getWindow().setDecorFitsSystemWindows (false);

        if (29 <= Build.VERSION.SDK_INT)
        {
            if (Build.VERSION.SDK_INT < 35)
                getWindow().setStatusBarContrastEnforced (false);

            getWindow().setNavigationBarContrastEnforced (false);
        }
    }

    @Override
    protected void onCreate (Bundle savedInstanceState)
    {
        initEdgeToEdge();

        super.onCreate (savedInstanceState);
    }

    @Override
    protected void onNewIntent (Intent intent)
    {
        super.onNewIntent (intent);
        setIntent (intent);

        appNewIntent (intent);
    }

    @Override
    protected void onResume()
    {
        super.onResume();
        appOnResume();
    }
}
