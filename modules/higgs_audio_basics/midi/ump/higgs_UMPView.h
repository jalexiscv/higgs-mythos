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

/** @cond */
namespace higgs::universal_midi_packets
{

/**
    Points to a single Universal MIDI Packet.

    The packet must be well-formed for member functions to work correctly.

    Specifically, the constructor argument must be the beginning of a region of
    uint32_t that contains at least `getNumWordsForMessageType (*data)` items,
    where `data` is the constructor argument.

    NOTE: Instances of this class do not own the memory that they point to!
    If you need to store a packet pointed-to by a View for later use, copy
    the view contents to a Packets collection, or use the Utils::PacketX types.

    @tags{Audio}
*/
class View
{
public:
    /** Create an invalid view. */
    View() noexcept = default;

    /** Create a view of the packet starting at address `d`. */
    explicit View (const uint32_t* data) noexcept : ptr (data) {}

    /** Get a pointer to the first word in the Universal MIDI Packet currently
        pointed-to by this view.
    */
    const uint32_t* data() const noexcept { return ptr; }

    /** Get the number of 32 bit words (between 1 and 4 inclusive) in the Universal
        MIDI Packet currently pointed-to by this view.
    */
    uint32_t size() const noexcept;

    /** Get a specific word from this packet.

        Passing an `index` that is greater than or equal to the result of `size`
        will cause undefined behaviour.
    */
    const uint32_t& operator[] (size_t index) const noexcept { return ptr[index]; }

    /** Get an iterator pointing to the first word in the packet. */
    const uint32_t* begin() const noexcept { return ptr; }
    const uint32_t* cbegin() const noexcept { return ptr; }

    /** Get an iterator pointing one-past the last word in the packet. */
    const uint32_t* end() const noexcept { return ptr + size(); }
    const uint32_t* cend() const noexcept { return ptr + size(); }

    /** Return true if this view is pointing to the same address as another view. */
    bool operator== (const View& other) const noexcept { return ptr == other.ptr; }

    /** Return false if this view is pointing to the same address as another view. */
    bool operator!= (const View& other) const noexcept { return ! operator== (other); }

private:
    const uint32_t* ptr = nullptr;
};

} // namespace higgs::universal_midi_packets
/** @endcond */
