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

namespace higgs
{

//==============================================================================
FileOutputStream::FileOutputStream (const File& f, const size_t bufferSizeToUse)
    : file (f),
      bufferSize (bufferSizeToUse),
      buffer (jmax (bufferSizeToUse, (size_t) 16))
{
    openHandle();
}

FileOutputStream::~FileOutputStream()
{
    flushBuffer();
    closeHandle();
}

int64 FileOutputStream::getPosition()
{
    return currentPosition;
}

bool FileOutputStream::setPosition (int64 newPosition)
{
    if (newPosition != currentPosition)
    {
        flushBuffer();
        currentPosition = higgs_fileSetPosition (fileHandle, newPosition);
    }

    return newPosition == currentPosition;
}

bool FileOutputStream::flushBuffer()
{
    bool ok = true;

    if (bytesInBuffer > 0)
    {
        ok = (writeInternal (buffer, bytesInBuffer) == (ssize_t) bytesInBuffer);
        bytesInBuffer = 0;
    }

    return ok;
}

void FileOutputStream::flush()
{
    flushBuffer();
    flushInternal();
}

bool FileOutputStream::write (const void* const src, const size_t numBytes)
{
    jassert (src != nullptr && ((ssize_t) numBytes) >= 0);

    if (! openedOk())
        return false;

    if (bytesInBuffer + numBytes < bufferSize)
    {
        memcpy (buffer + bytesInBuffer, src, numBytes);
        bytesInBuffer += numBytes;
        currentPosition += (int64) numBytes;
    }
    else
    {
        if (! flushBuffer())
            return false;

        if (numBytes < bufferSize)
        {
            memcpy (buffer + bytesInBuffer, src, numBytes);
            bytesInBuffer += numBytes;
            currentPosition += (int64) numBytes;
        }
        else
        {
            auto bytesWritten = writeInternal (src, numBytes);

            if (bytesWritten < 0)
                return false;

            currentPosition += (int64) bytesWritten;
            return bytesWritten == (ssize_t) numBytes;
        }
    }

    return true;
}

bool FileOutputStream::writeRepeatedByte (uint8 byte, size_t numBytes)
{
    jassert (((ssize_t) numBytes) >= 0);

    if (bytesInBuffer + numBytes < bufferSize)
    {
        memset (buffer + bytesInBuffer, byte, numBytes);
        bytesInBuffer += numBytes;
        currentPosition += (int64) numBytes;
        return true;
    }

    return OutputStream::writeRepeatedByte (byte, numBytes);
}

} // namespace higgs
