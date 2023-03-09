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


// This file will be included directly by macOS/iOS-specific .cpps
#pragma once

/** @cond */

#include <mach/mach_time.h>

namespace higgs
{

struct CoreAudioTimeConversions
{
public:
    CoreAudioTimeConversions()
    {
        mach_timebase_info_data_t info{};
        mach_timebase_info (&info);
        numerator   = info.numer;
        denominator = info.denom;
    }

    /** A reimplementation of AudioConvertHostTimeToNanos() that does not depend on Core Audio. */
    uint64_t hostTimeToNanos (uint64_t hostTime) const
    {
        return multiplyByRatio (hostTime, numerator, denominator);
    }

    /** A reimplementation of AudioConvertNanosToHostTime() that does not depend on Core Audio. */
    uint64_t nanosToHostTime (uint64_t nanos) const
    {
        return multiplyByRatio (nanos, denominator, numerator);
    }

private:
    // Adapted from CAHostTimeBase.h in the Core Audio Utility Classes
    static uint64_t multiplyByRatio (uint64_t toMultiply, uint64_t numerator, uint64_t denominator)
    {
       #if defined (__SIZEOF_INT128__)
        unsigned __int128
       #else
        long double
       #endif
            result = toMultiply;

        if (numerator != denominator)
        {
            result *= numerator;
            result /= denominator;
        }

        return (uint64_t) result;
    }

    uint64_t numerator = 0, denominator = 0;
};

} // namespace higgs

/** @endcond */
