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
    A type of AudioSource that takes an input source and changes its sample rate.

    @see AudioSource, LagrangeInterpolator, CatmullRomInterpolator

    @tags{Audio}
*/
class HIGGS_API  ResamplingAudioSource  : public AudioSource
{
public:
    //==============================================================================
    /** Creates a ResamplingAudioSource for a given input source.

        @param inputSource              the input source to read from
        @param deleteInputWhenDeleted   if true, the input source will be deleted when
                                        this object is deleted
        @param numChannels              the number of channels to process
    */
    ResamplingAudioSource (AudioSource* inputSource,
                           bool deleteInputWhenDeleted,
                           int numChannels = 2);

    /** Destructor. */
    ~ResamplingAudioSource() override;

    /** Changes the resampling ratio.

        (This value can be changed at any time, even while the source is running).

        @param samplesInPerOutputSample     if set to 1.0, the input is passed through; higher
                                            values will speed it up; lower values will slow it
                                            down. The ratio must be greater than 0
    */
    void setResamplingRatio (double samplesInPerOutputSample);

    /** Returns the current resampling ratio.

        This is the value that was set by setResamplingRatio().
    */
    double getResamplingRatio() const noexcept                  { return ratio; }

    /** Clears any buffers and filters that the resampler is using. */
    void flushBuffers();

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override;
    void getNextAudioBlock (const AudioSourceChannelInfo&) override;

private:
    //==============================================================================
    OptionalScopedPointer<AudioSource> input;
    double ratio = 1.0, lastRatio = 1.0;
    AudioBuffer<float> buffer;
    int bufferPos = 0, sampsInBuffer = 0;
    double subSampleOffset = 0.0;
    double coefficients[6];
    SpinLock ratioLock;
    CriticalSection callbackLock;
    const int numChannels;
    HeapBlock<float*> destBuffers;
    HeapBlock<const float*> srcBuffers;

    void setFilterCoefficients (double c1, double c2, double c3, double c4, double c5, double c6);
    void createLowPass (double proportionalRate);

    struct FilterState
    {
        double x1, x2, y1, y2;
    };

    HeapBlock<FilterState> filterStates;
    void resetFilters();

    void applyFilter (float* samples, int num, FilterState& fs);

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ResamplingAudioSource)
};

} // namespace higgs
