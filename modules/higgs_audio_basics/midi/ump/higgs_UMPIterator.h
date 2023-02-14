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
    Enables iteration over a collection of Universal MIDI Packets stored as
    a contiguous range of 32-bit words.

    This iterator is used by Packets to allow access to the messages
    that it contains.

    @tags{Audio}
*/
class Iterator
{
public:
    /** Creates an invalid (singular) iterator. */
    Iterator() noexcept = default;

    /** Creates an iterator pointing at `ptr`. */
    explicit Iterator (const uint32_t* ptr, size_t words) noexcept;

    using difference_type    = std::iterator_traits<const uint32_t*>::difference_type;
    using value_type         = View;
    using reference          = const View&;
    using pointer            = const View*;
    using iterator_category  = std::forward_iterator_tag;

    /** Moves this iterator to the next packet in the range. */
    Iterator& operator++() noexcept
    {
        const auto increment = view.size();

       #if HIGGS_DEBUG
        // If you hit this, the memory region contained a truncated or otherwise
        // malformed Universal MIDI Packet.
        // The Iterator can only be used on regions containing complete packets!
        jassert (increment <= wordsRemaining);
        wordsRemaining -= increment;
       #endif

        view = View (view.data() + increment);
        return *this;
    }

    /** Moves this iterator to the next packet in the range,
        returning the value of the iterator before it was
        incremented.
    */
    Iterator operator++ (int) noexcept
    {
        auto copy = *this;
        ++(*this);
        return copy;
    }

    /** Returns true if this iterator points to the same address
        as another iterator.
    */
    bool operator== (const Iterator& other) const noexcept
    {
        return view == other.view;
    }

    /** Returns false if this iterator points to the same address
        as another iterator.
    */
    bool operator!= (const Iterator& other) const noexcept
    {
        return ! operator== (other);
    }

    /** Returns a reference to a View of the packet currently
        pointed-to by this iterator.

        The View can be queried for its size and content.
    */
    reference operator*() const noexcept { return view; }

    /** Returns a pointer to a View of the packet currently
        pointed-to by this iterator.

        The View can be queried for its size and content.
    */
    pointer operator->() const noexcept { return &view; }

private:
    View view;

   #if HIGGS_DEBUG
    size_t wordsRemaining = 0;
   #endif
};

} // namespace higgs::universal_midi_packets
/** @endcond */
