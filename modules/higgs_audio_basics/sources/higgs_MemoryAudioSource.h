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
    An AudioSource which takes some float audio data as an input.

    @tags{Audio}
*/
class HIGGS_API MemoryAudioSource   : public PositionableAudioSource
{
public:
    //==============================================================================
    /**  Creates a MemoryAudioSource by providing an audio buffer.

         If copyMemory is true then the buffer will be copied into an internal
         buffer which will be owned by the MemoryAudioSource. If copyMemory is
         false, then you must ensure that the lifetime of the audio buffer is
         at least as long as the MemoryAudioSource.
    */
    MemoryAudioSource (AudioBuffer<float>& audioBuffer, bool copyMemory, bool shouldLoop = false);

    //==============================================================================
    /** Implementation of the AudioSource method. */
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;

    /** Implementation of the AudioSource method. */
    void releaseResources() override;

    /** Implementation of the AudioSource method. */
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;

    //==============================================================================
    /** Implementation of the PositionableAudioSource method. */
    void setNextReadPosition (int64 newPosition) override;

    /** Implementation of the PositionableAudioSource method. */
    int64 getNextReadPosition() const override;

    /** Implementation of the PositionableAudioSource method. */
    int64 getTotalLength() const override;

    //==============================================================================
    /** Implementation of the PositionableAudioSource method. */
    bool isLooping() const override;

    /** Implementation of the PositionableAudioSource method. */
    void setLooping (bool shouldLoop) override;

private:
    //==============================================================================
    AudioBuffer<float> buffer;
    int position = 0;
    bool isCurrentlyLooping;

    //==============================================================================
    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MemoryAudioSource)
};

} // namespace higgs
