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

constexpr auto FontFeatureSetting::tie() const
{
    return std::tuple (tag, value);
}

constexpr bool FontFeatureSetting::operator< (const FontFeatureSetting& other) const
{
    return tie() <  other.tie();
}

constexpr bool FontFeatureSetting::operator<= (const FontFeatureSetting& other) const
{
    return tie() <= other.tie();
}

constexpr bool FontFeatureSetting::operator> (const FontFeatureSetting& other) const
{
    return tie() >  other.tie();
}

constexpr bool FontFeatureSetting::operator>= (const FontFeatureSetting& other) const
{
    return tie() >= other.tie();
}

constexpr bool FontFeatureSetting::operator== (const FontFeatureSetting& other) const
{
    return tie() == other.tie();
}

constexpr bool FontFeatureSetting::operator!= (const FontFeatureSetting& other) const
{
    return tie() != other.tie();
}

//==============================================================================
FontFeatureTag FontFeatureTag::fromString (const String& string)
{
    if (string.getNumBytesAsUTF8() != 4)
    {
        jassertfalse;
        return FontFeatureTag (0);
    }

    char characters[5]{};
    memcpy (characters, string.toRawUTF8(), 4);
    return FontFeatureTag { characters };
}

String FontFeatureTag::toString() const
{
    const char characters[5]
    {
        static_cast<char> (tag >> 24),
        static_cast<char> (tag >> 16),
        static_cast<char> (tag >> 8),
        static_cast<char> (tag),
        '\0'
    };

    return String { characters };
}

constexpr auto FontVariableSetting::tie() const { return std::tuple (tag, value); }

constexpr bool FontVariableSetting::operator< (const FontVariableSetting& other) const
{
    return tie() < other.tie();
}

constexpr bool FontVariableSetting::operator<= (const FontVariableSetting& other) const
{
    return tie() <= other.tie();
}

constexpr bool FontVariableSetting::operator> (const FontVariableSetting& other) const
{
    return tie() > other.tie();
}

constexpr bool FontVariableSetting::operator>= (const FontVariableSetting& other) const
{
    return tie() >= other.tie();
}

constexpr bool FontVariableSetting::operator!= (const FontVariableSetting& other) const
{
    return tie() != other.tie();
}

constexpr bool FontVariableSetting::operator== (const FontVariableSetting& other) const
{
    return tag == other.tag && approximatelyEqual (value, other.value);
}


} // namespace higgs
