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

template <typename T>
static constexpr bool contains (std::initializer_list<T> span, const T& b)
{
    for (const auto& i : span)
        if (i == b)
            return true;

    return false;
}

struct UnicodeAnalysisPoint
{
    char32_t character = 0;
    UnicodeEntry data{};

    UnicodeAnalysisPoint (char32_t characterIn, UnicodeEntry entry)
        : character { characterIn },
          data { std::move (entry) }
    {}

    LineBreakType getBreakType() const
    {
        return data.bt;
    }

    auto getGeneralCategory() const
    {
        return SBCodepointGetGeneralCategory (character);
    }

    auto getScriptType() const
    {
        return SBCodepointGetScript (character);
    }
};

//==============================================================================
/*  Types of breaks between characters. */
enum class TextBreakType
{
    none, // The sequence of characters should not be broken.

    soft, // The sequence of characters can be broken, if required.

    hard  // The sequence of characters must be broken here.
};

} // namespace higgs
