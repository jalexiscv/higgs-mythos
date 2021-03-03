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

import java.lang.reflect.*;

public class HiggsInvocationHandler implements InvocationHandler
{
    public HiggsInvocationHandler (long nativeContextRef)
    {
        nativeContext = nativeContextRef;
    }

    public void clear()
    {
        nativeContext = 0;
    }

    @Override
    public void finalize()
    {
        if (nativeContext != 0)
            dispatchFinalize (nativeContext);
    }

    @Override
    public Object invoke (Object proxy, Method method, Object[] args) throws Throwable
    {
        if (nativeContext != 0)
            return dispatchInvoke (nativeContext, proxy, method, args);

        return null;
    }

    //==============================================================================
    private long nativeContext = 0;

    private native void dispatchFinalize (long nativeContextRef);
    private native Object dispatchInvoke (long nativeContextRef, Object proxy, Method method, Object[] args);
}
