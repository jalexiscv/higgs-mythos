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
    Opens a Bluetooth MIDI pairing dialogue that allows the user to view and
    connect to Bluetooth MIDI devices that are currently found nearby.

    The dialogue will ignore non-MIDI Bluetooth devices.

    Only after a Bluetooth MIDI device has been paired will its MIDI ports
    be available through HIGGS's MidiInput and MidiOutput classes.

    This dialogue is currently only available on macOS targeting versions 10.11+,
    iOS and Android. When targeting older versions of macOS you should instead
    pair Bluetooth MIDI devices using the "Audio MIDI Setup" app (located in
    /Applications/Utilities). On Windows, you should use the system settings. On
    Linux, Bluetooth MIDI devices are currently not supported.

    @tags{Audio}
*/
class HIGGS_API BluetoothMidiDevicePairingDialogue
{
public:

    /** Opens the Bluetooth MIDI pairing dialogue, if it is available.

        @param  exitCallback A callback which will be called when the modal
                bluetooth dialog is closed.
        @param  btWindowBounds The bounds of the bluetooth window that will
                be opened. The dialog itself is opened by the OS so cannot
                be customised by HIGGS.
        @return true if the dialogue was opened, false on error.

        @see ModalComponentManager::Callback
    */
    static bool open (ModalComponentManager::Callback* exitCallback = nullptr,
                      Rectangle<int>* btWindowBounds = nullptr);

    /** Checks if a Bluetooth MIDI pairing dialogue is available on this
        platform.

        On iOS, this will be true for iOS versions 8.0 and higher.

        On Android, this will be true only for Android SDK versions 23 and
        higher, and additionally only if the device itself supports MIDI
        over Bluetooth.

        On desktop platforms, this will typically be false as the bluetooth
        pairing is not done inside the app but by other means.

        @return true if the Bluetooth MIDI pairing dialogue is available,
                false otherwise.
    */
    static bool isAvailable();
};

} // namespace higgs
