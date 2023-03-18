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
    A simple AudioSource that generates a sine wave.


    @tags{Audio}
*/
class HIGGS_API  ToneGeneratorAudioSource  : public AudioSource
{
public:
    //==============================================================================
    /** Creates a ToneGeneratorAudioSource. */
    ToneGeneratorAudioSource();

    /** Destructor. */
    ~ToneGeneratorAudioSource() override;

    //==============================================================================
    /** Sets the signal's amplitude. */
    void setAmplitude (float newAmplitude);

    /** Sets the signal's frequency. */
    void setFrequency (double newFrequencyHz);


    //==============================================================================
    /** Implementation of the AudioSource method. */
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;

    /** Implementation of the AudioSource method. */
    void releaseResources() override;

    /** Implementation of the AudioSource method. */
    void getNextAudioBlock (const AudioSourceChannelInfo&) override;


private:
    //==============================================================================
    double frequency, sampleRate;
    double currentPhase, phasePerSample;
    float amplitude;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ToneGeneratorAudioSource)
};

} // namespace higgs
