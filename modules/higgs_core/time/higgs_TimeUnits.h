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

/** A type used to store years with double precision. */
using Years = std::chrono::duration<double, std::ratio<31556952>>;

/** A type used to store months with double precision. */
using Months = std::chrono::duration<double, std::ratio<2629746>>;

/** A type used to store weeks with double precision. */
using Weeks = std::chrono::duration<double, std::ratio<604800>>;

/** A type used to store days with double precision. */
using Days = std::chrono::duration<double, std::ratio<86400>>;

/** A type used to store hours with double precision. */
using Hours = std::chrono::duration<double, std::ratio<3600>>;

/** A type used to store minutes with double precision. */
using Minutes = std::chrono::duration<double, std::ratio<60>>;

/** A type used to store seconds with double precision. */
using Seconds = std::chrono::duration<double, std::ratio<1>>;

/** A type used to store milliseconds with double precision. */
using Milliseconds = std::chrono::duration<double, std::milli>;

/** A type used to store microseconds with double precision. */
using Microseconds = std::chrono::duration<double, std::micro>;

/** A type used to store nanoseconds with double precision. */
using Nanoseconds = std::chrono::duration<double, std::nano>;

} // namespace higgs
