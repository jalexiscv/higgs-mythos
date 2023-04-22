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

/**
    Represents a virtual MIDI 1.0 input port.

    @tags{Audio}
*/
class LegacyVirtualInput
{
public:
    /** Creates an invalid virtual port that doesn't correspond to any virtual device. */
    LegacyVirtualInput();
    ~LegacyVirtualInput();

    LegacyVirtualInput (LegacyVirtualInput&&) noexcept;
    LegacyVirtualInput& operator= (LegacyVirtualInput&&) noexcept;

    LegacyVirtualInput (const LegacyVirtualInput&) = delete;
    LegacyVirtualInput& operator= (const LegacyVirtualInput&) = delete;

    /** Retrieves the unique id of this input.

        You can pass this ID to Session::connectInput() in order to receive messages sent to this
        input.

        Note that this ID is *not* guaranteed to be stable - creating the 'same' virtual device
        across several program invocations may produce a different ID each time.

        To fetch the current details of this device, you can pass this ID to Endpoints::getEndpoint().
    */
    EndpointId getId() const;

    bool isAlive() const;

    explicit operator bool() const { return isAlive(); }

    /** @internal */
    class Impl;

private:
    explicit LegacyVirtualInput (std::unique_ptr<Impl>);

    std::unique_ptr<Impl> impl;
};

}
