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
    Contains functions to control the system's master volume.

    @tags{Audio}
*/
class HIGGS_API  SystemAudioVolume
{
public:
    //==============================================================================
    /** Returns the operating system's current volume level in the range 0 to 1.0 */
    static float HIGGS_CALLTYPE getGain();

    /** Attempts to set the operating system's current volume level.
        @param newGain  the level, between 0 and 1.0
        @returns true if the operation succeeds
    */
    static bool HIGGS_CALLTYPE setGain (float newGain);

    /** Returns true if the system's audio output is currently muted. */
    static bool HIGGS_CALLTYPE isMuted();

    /** Attempts to mute the operating system's audio output.
        @param shouldBeMuted    true if you want it to be muted
        @returns true if the operation succeeds
    */
    static bool HIGGS_CALLTYPE setMuted (bool shouldBeMuted);

private:
    SystemAudioVolume(); // Don't instantiate this class, just call its static fns.
    HIGGS_DECLARE_NON_COPYABLE (SystemAudioVolume)
};

} // namespace higgs
