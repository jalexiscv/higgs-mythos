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

namespace higgs::dsp
{

/**
    Represents an increasing phase value between 0 and 2*pi.

    This represents a value which can be incremented, and which wraps back to 0 when it
    goes past 2 * pi.

    @tags{DSP}
*/
template <typename Type>
struct Phase
{
    /** Resets the phase to 0. */
    void reset() noexcept               { phase = 0; }

    /** Returns the current value, and increments the phase by the given increment.
        The increment must be a positive value, it can't go backwards!
        The new value of the phase after calling this function will be (phase + increment) % (2 * pi).
    */
    Type advance (Type increment) noexcept
    {
        jassert (increment >= 0); // cannot run this value backwards!

        auto last = phase;
        auto next = last + increment;

        while (next >= MathConstants<Type>::twoPi)
            next -= MathConstants<Type>::twoPi;

        phase = next;
        return last;
    }

    Type phase = 0;
};

} // namespace higgs::dsp
