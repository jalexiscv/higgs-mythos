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
    A type of AudioSource which can be repositioned.

    The basic AudioSource just streams continuously with no idea of a current
    time or length, so the PositionableAudioSource is used for a finite stream
    that has a current read position.

    @see AudioSource, AudioTransportSource

    @tags{Audio}
*/
class HIGGS_API  PositionableAudioSource  : public AudioSource
{
protected:
    //==============================================================================
    /** Creates the PositionableAudioSource. */
    PositionableAudioSource() = default;

public:
    /** Destructor */
    ~PositionableAudioSource() override = default;

    //==============================================================================
    /** Tells the stream to move to a new position.

        Calling this indicates that the next call to AudioSource::getNextAudioBlock()
        should return samples from this position.

        Note that this may be called on a different thread to getNextAudioBlock(),
        so the subclass should make sure it's synchronised.
    */
    virtual void setNextReadPosition (int64 newPosition) = 0;

    /** Returns the position from which the next block will be returned.

        @see setNextReadPosition
    */
    virtual int64 getNextReadPosition() const = 0;

    /** Returns the total length of the stream (in samples). */
    virtual int64 getTotalLength() const = 0;

    /** Returns true if this source is actually playing in a loop. */
    virtual bool isLooping() const = 0;

    /** Tells the source whether you'd like it to play in a loop. */
    virtual void setLooping (bool shouldLoop);
};

} // namespace higgs
