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
    An AudioSource that mixes together the output of a set of other AudioSources.

    Input sources can be added and removed while the mixer is running as long as their
    prepareToPlay() and releaseResources() methods are called before and after adding
    them to the mixer.

    @tags{Audio}
*/
class HIGGS_API  MixerAudioSource  : public AudioSource
{
public:
    //==============================================================================
    /** Creates a MixerAudioSource. */
    MixerAudioSource();

    /** Destructor. */
    ~MixerAudioSource() override;

    //==============================================================================
    /** Adds an input source to the mixer.

        If the mixer is running you'll need to make sure that the input source
        is ready to play by calling its prepareToPlay() method before adding it.
        If the mixer is stopped, then its input sources will be automatically
        prepared when the mixer's prepareToPlay() method is called.

        @param newInput             the source to add to the mixer
        @param deleteWhenRemoved    if true, then this source will be deleted when
                                    no longer needed by the mixer.
    */
    void addInputSource (AudioSource* newInput, bool deleteWhenRemoved);

    /** Removes an input source.
        If the source was added by calling addInputSource() with the deleteWhenRemoved
        flag set, it will be deleted by this method.
    */
    void removeInputSource (AudioSource* input);

    /** Removes all the input sources.
        Any sources which were added by calling addInputSource() with the deleteWhenRemoved
        flag set will be deleted by this method.
    */
    void removeAllInputs();

    //==============================================================================
    /** Implementation of the AudioSource method.
        This will call prepareToPlay() on all its input sources.
    */
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;

    /** Implementation of the AudioSource method.
        This will call releaseResources() on all its input sources.
    */
    void releaseResources() override;

    /** Implementation of the AudioSource method. */
    void getNextAudioBlock (const AudioSourceChannelInfo&) override;


private:
    //==============================================================================
    Array<AudioSource*> inputs;
    BigInteger inputsToDelete;
    CriticalSection lock;
    AudioBuffer<float> tempBuffer;
    double currentSampleRate;
    int bufferSizeExpected;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MixerAudioSource)
};

} // namespace higgs
