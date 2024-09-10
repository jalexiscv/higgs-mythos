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

template <typename FloatType>
LookupTable<FloatType>::LookupTable()
{
    data.resize (1);
}

template <typename FloatType>
LookupTable<FloatType>::LookupTable (const std::function<FloatType (size_t)>& functionToApproximate,
                                     size_t numPointsToUse)
{
    initialise (functionToApproximate, numPointsToUse);
}

//==============================================================================
template <typename FloatType>
void LookupTable<FloatType>::initialise (const std::function<FloatType (size_t)>& functionToApproximate,
                                         size_t numPointsToUse)
{
    data.resize (static_cast<int> (getRequiredBufferSize (numPointsToUse)));

    for (size_t i = 0; i < numPointsToUse; ++i)
    {
        auto value = functionToApproximate (i);

        jassert (! std::isnan (value));
        jassert (! std::isinf (value));
        // Make sure functionToApproximate returns a sensible value for the entire specified range.
        // E.g., this won't work for zero:  [] (size_t i) { return 1.0f / i; }

        data.getReference (static_cast<int> (i)) = value;
    }

    prepare();
}

template <typename FloatType>
void LookupTable<FloatType>::prepare() noexcept
{
    auto guardIndex = static_cast<int> (getGuardIndex());
    data.getReference (guardIndex) = data.getUnchecked (guardIndex - 1);
}

template <typename FloatType>
void LookupTableTransform<FloatType>::initialise (const std::function<FloatType (FloatType)>& functionToApproximate,
                                                  FloatType minInputValueToUse,
                                                  FloatType maxInputValueToUse,
                                                  size_t numPoints)
{
    jassert (maxInputValueToUse > minInputValueToUse);

    minInputValue = minInputValueToUse;
    maxInputValue = maxInputValueToUse;
    scaler = FloatType (numPoints - 1) / (maxInputValueToUse - minInputValueToUse);
    offset = -minInputValueToUse * scaler;

    const auto initFn = [functionToApproximate, minInputValueToUse, maxInputValueToUse, numPoints] (size_t i)
    {
        return functionToApproximate (
            jlimit (
                minInputValueToUse, maxInputValueToUse,
                jmap (FloatType (i), FloatType (0), FloatType (numPoints - 1), minInputValueToUse, maxInputValueToUse))
            );
    };

    lookupTable.initialise (initFn, numPoints);
}

//==============================================================================
template <typename FloatType>
double LookupTableTransform<FloatType>::calculateMaxRelativeError (const std::function<FloatType (FloatType)>& functionToApproximate,
                                                                   FloatType minInputValue,
                                                                   FloatType maxInputValue,
                                                                   size_t numPoints,
                                                                   size_t numTestPoints)
{
    jassert (maxInputValue > minInputValue);

    if (numTestPoints == 0)
        numTestPoints = 100 * numPoints;    // use default

    LookupTableTransform transform (functionToApproximate, minInputValue, maxInputValue, numPoints);

    double maxError = 0;

    for (size_t i = 0; i < numTestPoints; ++i)
    {
        auto inputValue = jmap (FloatType (i), FloatType (0), FloatType (numTestPoints - 1), minInputValue, maxInputValue);
        auto approximatedOutputValue = transform.processSample (inputValue);
        auto referenceOutputValue = functionToApproximate (inputValue);

        maxError = jmax (maxError, calculateRelativeDifference ((double) referenceOutputValue, (double) approximatedOutputValue));
    }

    return maxError;
}

//==============================================================================
template <typename FloatType>
double LookupTableTransform<FloatType>::calculateRelativeDifference (double x, double y) noexcept
{
    static const auto eps = std::numeric_limits<double>::min();

    auto absX = std::abs (x);
    auto absY = std::abs (y);
    auto absDiff = std::abs (x - y);

    if (absX < eps)
    {
        if (absY >= eps)
            return absDiff / absY;

        return absDiff;    // return the absolute error if both numbers are too close to zero
    }

    return absDiff / std::min (absX, absY);
}

//==============================================================================
template class LookupTable<float>;
template class LookupTable<double>;

template class LookupTableTransform<float>;
template class LookupTableTransform<double>;

} // namespace higgs::dsp
