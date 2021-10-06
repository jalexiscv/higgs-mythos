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

/** @cond */
/** @internal
    Helper methods for sorting FontFeatureSettings and FontVariableSettings by tag.
*/
struct FontComparators
{
    FontComparators() = delete;

    struct FeatureSettingComparator
    {
        constexpr bool operator() (FontFeatureSetting a, FontFeatureSetting b)   const { return a.tag < b.tag; }
        constexpr bool operator() (FontFeatureTag a, FontFeatureSetting b)       const { return a < b.tag; }
        constexpr bool operator() (FontFeatureSetting a, FontFeatureTag b)       const { return a.tag < b; }
    };

    struct VariableSettingComparator
    {
        constexpr bool operator() (FontVariableSetting a, FontVariableSetting b) const { return a.tag < b.tag; }
        constexpr bool operator() (FontFeatureTag a, FontVariableSetting b)      const { return a < b.tag; }
        constexpr bool operator() (FontVariableSetting a, FontFeatureTag b)      const { return a.tag < b; }
    };
};
/** @endcond */

} // namespace higgs
