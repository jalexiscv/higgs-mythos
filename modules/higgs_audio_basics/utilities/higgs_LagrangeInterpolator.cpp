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

template <int k>
struct LagrangeResampleHelper
{
    static forcedinline void calc (float& a, float b) noexcept   { a *= b * (1.0f / k); }
};

template <>
struct LagrangeResampleHelper<0>
{
    static forcedinline void calc (float&, float) noexcept {}
};

template <int k>
static float calcCoefficient (float input, float offset) noexcept
{
    LagrangeResampleHelper<0 - k>::calc (input, -2.0f - offset);
    LagrangeResampleHelper<1 - k>::calc (input, -1.0f - offset);
    LagrangeResampleHelper<2 - k>::calc (input,  0.0f - offset);
    LagrangeResampleHelper<3 - k>::calc (input,  1.0f - offset);
    LagrangeResampleHelper<4 - k>::calc (input,  2.0f - offset);
    return input;
}

float Interpolators::LagrangeTraits::valueAtOffset (const float* inputs, float offset, int index) noexcept
{
    float result = 0.0f;

    result += calcCoefficient<0> (inputs[index], offset); if (++index == 5) index = 0;
    result += calcCoefficient<1> (inputs[index], offset); if (++index == 5) index = 0;
    result += calcCoefficient<2> (inputs[index], offset); if (++index == 5) index = 0;
    result += calcCoefficient<3> (inputs[index], offset); if (++index == 5) index = 0;
    result += calcCoefficient<4> (inputs[index], offset);

    return result;
}

} // namespace higgs
