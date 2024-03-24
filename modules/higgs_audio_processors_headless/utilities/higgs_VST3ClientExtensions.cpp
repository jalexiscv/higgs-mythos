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

std::map<uint32_t, String> VST3ClientExtensions::getCompatibleParameterIds (const VST3Interface::Id&) const
{
    return {};
}

uint32_t VST3ClientExtensions::convertHiggsParameterId (const String& parameterId, bool studioOneCompatible)
{
    auto hash = (uint32_t) (parameterId.hashCode());

    if (studioOneCompatible)
        hash &= 0x7fffffff;

    return hash;
}

} // namespace higgs
