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
/**
    A parameter with functions that are useful for plugin hosts.

    @tags{Audio}
*/
struct HIGGS_API  HostedAudioProcessorParameter : public AudioProcessorParameter
{
    using AudioProcessorParameter::AudioProcessorParameter;

    /** Returns an ID that is unique to this parameter.

        Parameter indices are unstable across plugin versions, which means that the
        parameter found at a particular index in one version of a plugin might move
        to a different index in the subsequent version.

        Unlike the parameter index, the ID returned by this function should be
        somewhat stable (depending on the format of the plugin), so it is more
        suitable for storing/recalling automation data.
    */
    virtual String getParameterID() const = 0;
};

} // namespace higgs
