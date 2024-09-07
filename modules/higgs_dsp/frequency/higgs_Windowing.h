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
    A class which provides multiple windowing functions useful for filter design
    and spectrum analyzers.

    The different functions provided here can be used by creating either a
    WindowingFunction object, or a static function to fill an array with the
    windowing method samples.

    @tags{DSP}
*/
template <typename FloatType>
class HIGGS_API  WindowingFunction
{
public:
    //==============================================================================
    /** The windowing methods available. */
    enum WindowingMethod
    {
        rectangular = 0,
        triangular,
        hann,
        hamming,
        blackman,
        blackmanHarris,
        flatTop,
        kaiser,
        numWindowingMethods
    };

    //==============================================================================
    /** This constructor automatically fills a buffer of the specified size using
        the fillWindowingTables function and the specified arguments.

        @see fillWindowingTables
    */
    WindowingFunction (size_t size, WindowingMethod,
                       bool normalise = true, FloatType beta = 0);

    //==============================================================================
    /** Fills the content of the object array with a given windowing method table.

        @param size         the size of the destination buffer allocated in the object
        @param type         the type of windowing method being used
        @param normalise    if the result must be normalised, creating a DC amplitude
                            response of one
        @param beta         an optional argument useful only for Kaiser's method
                            which must be positive and sets the properties of the
                            method (bandwidth and attenuation increases with beta)
    */
    void fillWindowingTables (size_t size, WindowingMethod type,
                              bool normalise = true, FloatType beta = 0) noexcept;

    /** Fills the content of an array with a given windowing method table.

        @param samples      the destination buffer pointer
        @param size         the size of the destination buffer allocated in the object
        @param normalise    if the result must be normalised, creating a DC amplitude
                            response of one
        @param beta         an optional argument useful only for Kaiser's method,
                            which must be positive and sets the properties of the
                            method (bandwidth and attenuation increases with beta)
    */
    static void fillWindowingTables (FloatType* samples, size_t size, WindowingMethod,
                                     bool normalise = true, FloatType beta = 0) noexcept;

    /** Multiplies the content of a buffer with the given window. */
    void multiplyWithWindowingTable (FloatType* samples, size_t size) const noexcept;

    /** Returns the name of a given windowing method. */
    static const char* getWindowingMethodName (WindowingMethod) noexcept;


private:
    //==============================================================================
    Array<FloatType> windowTable;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WindowingFunction)
};

} // namespace higgs::dsp
