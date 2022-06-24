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

import android.database.Cursor;
import android.database.MatrixCursor;

import java.lang.String;

public final class HiggsContentProviderCursor extends MatrixCursor
{
    public HiggsContentProviderCursor (long hostToUse, String[] columnNames)
    {
        super (columnNames);

        host = hostToUse;
    }

    @Override
    public void close ()
    {
        super.close ();

        contentSharerCursorClosed (host);
    }

    private native void contentSharerCursorClosed (long host);

    private long host;
}
