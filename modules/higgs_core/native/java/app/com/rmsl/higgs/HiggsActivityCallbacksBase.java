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

import android.app.Activity;
import android.app.Application;
import android.os.Bundle;

public class HiggsActivityCallbacksBase implements Application.ActivityLifecycleCallbacks
{
    @Override
    public void onActivityCreated (Activity activity, Bundle savedInstanceState) {}

    @Override
    public void onActivityStarted (Activity activity) {}

    @Override
    public void onActivityResumed (Activity activity) {}

    @Override
    public void onActivityPaused (Activity activity) {}

    @Override
    public void onActivityStopped (Activity activity) {}

    @Override
    public void onActivitySaveInstanceState (Activity activity, Bundle outState) {}

    @Override
    public void onActivityDestroyed (Activity activity) {}
}
