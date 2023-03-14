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
    An AudioSource that performs an IIR filter on another source.

    @tags{Audio}
*/
class HIGGS_API  IIRFilterAudioSource  : public AudioSource
{
public:
    //==============================================================================
    /** Creates a IIRFilterAudioSource for a given input source.

        @param inputSource              the input source to read from - this must not be null
        @param deleteInputWhenDeleted   if true, the input source will be deleted when
                                        this object is deleted
    */
    IIRFilterAudioSource (AudioSource* inputSource,
                          bool deleteInputWhenDeleted);

    /** Destructor. */
    ~IIRFilterAudioSource() override;

    //==============================================================================
    /** Changes the filter to use the same parameters as the one being passed in. */
    void setCoefficients (const IIRCoefficients& newCoefficients);

    /** Calls IIRFilter::makeInactive() on all the filters being used internally. */
    void makeInactive();

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override;
    void getNextAudioBlock (const AudioSourceChannelInfo&) override;

private:
    //==============================================================================
    OptionalScopedPointer<AudioSource> input;
    OwnedArray<IIRFilter> iirFilters;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IIRFilterAudioSource)
};

} // namespace higgs
