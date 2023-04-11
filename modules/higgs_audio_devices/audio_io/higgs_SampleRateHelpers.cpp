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

namespace higgs::SampleRateHelpers
{

static inline const std::vector<double>& getCommonSampleRates()
{
    static auto sampleRates = std::invoke ([]
    {
        std::vector<double> result;
        constexpr double baseRates[] = { 8000.0, 11025.0, 12000.0 };
        constexpr double maxRate = 768000.0;

        for (auto rate : baseRates)
            for (; rate <= maxRate; rate *= 2)
                result.insert (std::upper_bound (result.begin(), result.end(), rate),
                               rate);

        return result;
    });

    return sampleRates;
}

} // namespace higgs::SampleRateHelpers
