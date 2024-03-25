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
    A component containing controls to let the user change the audio settings of
    an AudioDeviceManager object.

    Very easy to use - just create one of these and show it to the user.

    @see AudioDeviceManager

    @tags{Audio}
*/
class HIGGS_API  AudioDeviceSelectorComponent  : public Component,
                                                private ChangeListener
{
public:
    //==============================================================================
    /** Creates the component.

        If your app needs only output channels, you might ask for a maximum of 0 input
        channels, and the component won't display any options for choosing the input
        channels. And likewise if you're doing an input-only app.

        @param deviceManager            the device manager that this component should control
        @param minAudioInputChannels    the minimum number of audio input channels that the application needs
        @param maxAudioInputChannels    the maximum number of audio input channels that the application needs
        @param minAudioOutputChannels   the minimum number of audio output channels that the application needs
        @param maxAudioOutputChannels   the maximum number of audio output channels that the application needs
        @param showMidiInputOptions     if true, the component will allow the user to select which midi inputs are enabled
        @param showMidiOutputSelector   if true, the component will let the user choose a default midi output device
        @param showChannelsAsStereoPairs    if true, channels will be treated as pairs; if false, channels will be
                                        treated as a set of separate mono channels.
        @param hideAdvancedOptionsWithButton    if true, only the minimum amount of UI components
                                        are shown, with an "advanced" button that shows the rest of them
    */
    AudioDeviceSelectorComponent (AudioDeviceManager& deviceManager,
                                  int minAudioInputChannels,
                                  int maxAudioInputChannels,
                                  int minAudioOutputChannels,
                                  int maxAudioOutputChannels,
                                  bool showMidiInputOptions,
                                  bool showMidiOutputSelector,
                                  bool showChannelsAsStereoPairs,
                                  bool hideAdvancedOptionsWithButton);

    /** Destructor */
    ~AudioDeviceSelectorComponent() override;

    /** The device manager that this component is controlling */
    AudioDeviceManager& deviceManager;

    /** Sets the standard height used for items in the panel. */
    void setItemHeight (int itemHeight);

    /** Returns the standard height used for items in the panel. */
    int getItemHeight() const noexcept      { return itemHeight; }

    //==============================================================================
    /** @internal */
    void resized() override;

    /** @internal */
    void childBoundsChanged (Component* child) override;

private:
    //==============================================================================
    void handleBluetoothButton();
    void updateDeviceType();
    void changeListenerCallback (ChangeBroadcaster*) override;
    void updateAllControls();

    std::unique_ptr<ComboBox> deviceTypeDropDown;
    std::unique_ptr<Label> deviceTypeDropDownLabel;
    std::unique_ptr<Component> audioDeviceSettingsComp;
    String audioDeviceSettingsCompType;
    int itemHeight = 0;
    const int minOutputChannels, maxOutputChannels, minInputChannels, maxInputChannels;
    const bool showChannelsAsStereoPairs;
    const bool hideAdvancedOptionsWithButton;

    class MidiInputSelectorComponent;
    class MidiOutputSelector;

    Array<MidiDeviceInfo> currentMidiOutputs;
    std::unique_ptr<MidiInputSelectorComponent> midiInputsList;
    std::unique_ptr<MidiOutputSelector> midiOutputSelector;
    std::unique_ptr<Label> midiInputsLabel, midiOutputLabel;
    std::unique_ptr<TextButton> bluetoothButton;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioDeviceSelectorComponent)
};

} // namespace higgs
