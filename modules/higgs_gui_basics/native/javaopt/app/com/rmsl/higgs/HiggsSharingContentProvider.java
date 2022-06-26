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

import android.content.ContentProvider;
import android.content.ContentValues;
import android.content.res.AssetFileDescriptor;
import android.content.res.Resources;
import android.database.Cursor;
import android.database.MatrixCursor;
import android.net.Uri;
import android.os.FileObserver;
import android.os.ParcelFileDescriptor;

import java.lang.String;

public final class HiggsSharingContentProvider extends ContentProvider
{
    private Object lock = new Object ();

    private native Cursor contentSharerQuery (Uri uri, String[] projection);
    private native AssetFileDescriptor contentSharerOpenFile (Uri uri, String mode);
    private native String[] contentSharerGetStreamTypes (Uri uri, String mimeTypeFilter);

    @Override
    public boolean onCreate ()
    {
        return true;
    }

    @Override
    public Cursor query (Uri url, String[] projection, String selection,
                         String[] selectionArgs, String sortOrder)
    {
        synchronized (lock)
        {
            return contentSharerQuery (url, projection);
        }
    }

    @Override
    public Uri insert (Uri uri, ContentValues values)
    {
        return null;
    }

    @Override
    public int update (Uri uri, ContentValues values, String selection,
                       String[] selectionArgs)
    {
        return 0;
    }

    @Override
    public int delete (Uri uri, String selection, String[] selectionArgs)
    {
        return 0;
    }

    @Override
    public String getType (Uri uri)
    {
        return null;
    }

    @Override
    public AssetFileDescriptor openAssetFile (Uri uri, String mode)
    {
        synchronized (lock)
        {
            return contentSharerOpenFile (uri, mode);
        }
    }

    @Override
    public ParcelFileDescriptor openFile (Uri uri, String mode)
    {
        synchronized (lock)
        {
            AssetFileDescriptor result = contentSharerOpenFile (uri, mode);

            if (result != null)
                return result.getParcelFileDescriptor ();

            return null;
        }
    }

    public String[] getStreamTypes (Uri uri, String mimeTypeFilter)
    {
        synchronized (lock)
        {
            return contentSharerGetStreamTypes (uri, mimeTypeFilter);
        }
    }
}
