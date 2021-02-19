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

void Logger::outputDebugString (const String& text)
{
    char* data = text.toUTF8().getAddress();
    const size_t length = CharPointer_UTF8::getBytesRequiredFor (text.getCharPointer());
    const size_t chunkSize = 1023;

    size_t position = 0;
    size_t numToRead = jmin (chunkSize, length);

    while (numToRead > 0)
    {
        __android_log_print (ANDROID_LOG_INFO, "HIGGS", "%s", data + position);

        position += numToRead;
        numToRead = jmin (chunkSize, length - position);
    }
}

} // namespace higgs
