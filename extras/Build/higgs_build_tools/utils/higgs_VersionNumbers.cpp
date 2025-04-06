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

namespace higgs::build_tools
{

    StringArray getVersionSegments (StringRef p)
    {
        auto segments = StringArray::fromTokens (p, ",.", "");
        segments.trim();
        segments.removeEmptyStrings();
        return segments;
    }

    int getVersionAsHexIntegerFromParts (const StringArray& segments)
    {
        auto value = (segments[0].getIntValue() << 16)
                   + (segments[1].getIntValue() << 8)
                   +  segments[2].getIntValue();

        if (segments.size() > 3)
            value = (value << 8) + segments[3].getIntValue();

        return value;
    }

    int getVersionAsHexInteger (StringRef versionString)
    {
        return getVersionAsHexIntegerFromParts (getVersionSegments (versionString));
    }

    String getVersionAsHex (StringRef versionString)
    {
        return "0x" + String::toHexString (getVersionAsHexInteger (versionString));
    }

} // namespace higgs::build_tools
