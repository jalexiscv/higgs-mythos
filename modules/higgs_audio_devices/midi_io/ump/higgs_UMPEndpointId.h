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

namespace higgs::universal_midi_packets
{

/** Directional properties of a MIDI endpoint. */
enum class IOKind : uint8_t
{
    src,    ///< A source of MIDI events
    dst,    ///< A destination for MIDI events
};

/** All possible MIDI directions. */
constexpr IOKind ioKinds[] { IOKind::src, IOKind::dst };

//==============================================================================
/**
    Identifies a MIDI endpoint. This is intended to be an opaque type that can only be compared with
    other instances.

    For backwards compatibility, we need to ensure that port identifier strings that used to work
    with MidiInput and MidiOutput continue to function in the same way. However, the old identifiers
    weren't necessarily unique between inputs and outputs (a MIDI 1.0 input and output could have
    the same ID), which means that a single id string isn't enough to uniquely identify an input
    or output port.

    @tags{Audio}
*/
class EndpointId
{
    auto tie() const;

public:
    /** @internal */
    class Impl;

    EndpointId() = default;

    bool operator== (const EndpointId& x) const;
    bool operator!= (const EndpointId& x) const;
    bool operator<  (const EndpointId& x) const;
    bool operator<= (const EndpointId& x) const;
    bool operator>  (const EndpointId& x) const;
    bool operator>= (const EndpointId& x) const;

    String get (IOKind k) const { return k == IOKind::src ? src : dst; }

    static EndpointId make (IOKind dir, const String& id)
    {
        return dir == IOKind::src ? makeSrcDst (id, {}) : makeSrcDst ({}, id);
    }

    static EndpointId makeSrcDst (const String& s, const String& d)
    {
        return { s, d };
    }

    String src, dst;

private:
    EndpointId (const String& s, const String& d) : src (s), dst (d) {}
};

}
