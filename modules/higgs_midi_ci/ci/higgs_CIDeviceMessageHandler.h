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

//==============================================================================
/**
    An interface that will receive a callback every time a Device wishes to send a new MIDI-CI
    message.

    @tags{Audio}
*/
struct DeviceMessageHandler
{
    DeviceMessageHandler() = default;
    virtual ~DeviceMessageHandler() = default;
    DeviceMessageHandler (const DeviceMessageHandler&) = default;
    DeviceMessageHandler (DeviceMessageHandler&&) = default;
    DeviceMessageHandler& operator= (const DeviceMessageHandler&) = default;
    DeviceMessageHandler& operator= (DeviceMessageHandler&&) = default;

    /** Called with the bytes of a MIDI-CI message, along with the message's group.

        To send the message on, format the message appropriately (either into bytestream sysex
        or into multiple UMP sysex packets).
    */
    virtual void processMessage (ump::BytesOnGroup) = 0;
};

} // namespace higgs::midi_ci
