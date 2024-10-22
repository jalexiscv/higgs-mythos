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

namespace higgs::midi_ci
{

/**
    Represents a destination into which MIDI-CI messages can be written.

    Each message should be written into the output buffer. Then, send() will
    send the current contents of the buffer to the specified group.

    @tags{Audio}
*/
class BufferOutput
{
public:
    BufferOutput() = default;
    virtual ~BufferOutput() = default;

    /** Returns the MUID of the responder. */
    virtual MUID getMuid() const = 0;

    /** Returns the buffer into which replies should be written. */
    virtual std::vector<std::byte>& getOutputBuffer() = 0;

    /** Sends the current contents of the buffer to the provided group. */
    virtual void send (uint8_t group) = 0;

    HIGGS_DECLARE_NON_COPYABLE (BufferOutput)
    HIGGS_DECLARE_NON_MOVEABLE (BufferOutput)
};

//==============================================================================
/**
    A buffer output that additionally provides information about an incoming message, so that
    an appropriate reply can be constructed for that message.

    @tags{Audio}
*/
class ResponderOutput : public BufferOutput
{
public:
    /** Returns the header of the message that was received. */
    virtual Message::Header getIncomingHeader() const = 0;

    /** Returns the group of the message that was received. */
    virtual uint8_t getIncomingGroup() const = 0;

    /** Returns the channel to which the incoming message was addressed. */
    ChannelAddress getChannelAddress() const;

    /** Returns a default header that can be used for outgoing replies.

        This always sets the destination MUID equal to the source MUID of the incoming header,
        so it's not suitable for broadcast messages.
    */
    Message::Header getReplyHeader (std::byte replySubID) const;
};

} // namespace higgs::midi_ci
