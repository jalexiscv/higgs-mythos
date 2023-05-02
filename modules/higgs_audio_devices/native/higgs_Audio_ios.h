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

class iOSAudioIODeviceType;

class iOSAudioIODevice : public AudioIODevice
{
public:
    //==============================================================================
    String open (const BigInteger&, const BigInteger&, double, int) override;
    void close() override;

    void start (AudioIODeviceCallback*) override;
    void stop() override;

    Array<double> getAvailableSampleRates() override;
    Array<int> getAvailableBufferSizes() override;

    bool setAudioPreprocessingEnabled (bool) override;

    //==============================================================================
    bool isPlaying() override;
    bool isOpen() override;
    String getLastError() override;

    //==============================================================================
    StringArray getOutputChannelNames() override;
    StringArray getInputChannelNames() override;

    int getDefaultBufferSize() override;
    int getCurrentBufferSizeSamples() override;

    double getCurrentSampleRate() override;

    int getCurrentBitDepth() override;

    BigInteger getActiveOutputChannels() const override;
    BigInteger getActiveInputChannels() const override;

    int getOutputLatencyInSamples() override;
    int getInputLatencyInSamples() override;

    int getXRunCount() const noexcept override;

    AudioWorkgroup getWorkgroup() const override;

    //==============================================================================
    void setMidiMessageCollector (MidiMessageCollector*);
    AudioPlayHead* getAudioPlayHead() const;

    //==============================================================================
    bool isInterAppAudioConnected() const;
   #if HIGGS_MODULE_AVAILABLE_higgs_graphics
    Image getIcon (int size);
   #endif
    void switchApplication();

private:
    //==============================================================================
    iOSAudioIODevice (iOSAudioIODeviceType*, const String&, const String&);

    //==============================================================================
    friend class iOSAudioIODeviceType;
    friend struct AudioSessionHolder;

    struct Pimpl;
    std::unique_ptr<Pimpl> pimpl;

    HIGGS_DECLARE_NON_COPYABLE (iOSAudioIODevice)
};

} // namespace higgs
