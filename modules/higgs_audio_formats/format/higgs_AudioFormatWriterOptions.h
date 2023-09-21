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

/**
    Options that affect the output data format produced by an AudioFormatWriter. Format
    specific writers may ignore some of these options.

    @see AudioFormat::createWriterFor(), AudioFormatWriter

    @tags{Audio}
*/
class HIGGS_API AudioFormatWriterOptions final
{
public:
    /** Used to provide a hint to the AudioFormatWriter for the output sample format.

        Use automatic for the old behaviour. The values integral and floatingPoint can be used with
        the WavAudioFormat when using a bit depth of 32. Other formats are not affected by this
        setting.
    */
    enum class SampleFormat
    {
        automatic,     ///< Lets the writer decide the format based on the other parameter values.
        integral,      ///< Integral format, e.g. PCM in case of the WavAudioFormat
        floatingPoint  ///< IEEE floating point format
    };

    /** Returns a copy of these options with the specified sample format.

        @see SampleFormat
    */
    [[nodiscard]] AudioFormatWriterOptions withSampleFormat (SampleFormat x) const
    {
        return withMember (*this, &AudioFormatWriterOptions::sampleFormat, x);
    }

    /** Returns a copy of these options with the specified sample rate.

        This specifies the sample rate for the file, which must be one of the ones
        returned by AudioFormat::getPossibleSampleRates().
    */
    [[nodiscard]] AudioFormatWriterOptions withSampleRate (double x) const
    {
        return withMember (*this, &AudioFormatWriterOptions::sampleRate, x);
    }

    /** Returns a copy of these options with the specified channel set.

        Setting this option will supersede the value passed into withNumChannels().

        You should prefer to use withChannelLayout(), if specifying an AudioChannelSet is
        applicable, and withNumChannels() otherwise.
    */
    [[nodiscard]] AudioFormatWriterOptions withChannelLayout (const AudioChannelSet& x) const
    {
        return withMember (*this, &AudioFormatWriterOptions::channelLayout, x);
    }

    /** Returns a copy of these options with the specified number of channels.

        This is meant as a fallback for specifying the channel layout. Setting this option will
        have no effect if the channel layout is specified.

        @see withChannelLayout()
    */
    [[nodiscard]] AudioFormatWriterOptions withNumChannels (int x) const
    {
        return withMember (*this, &AudioFormatWriterOptions::numChannels, x);
    }

    /** Returns a copy of these options with the specified bit size per sample.

        This must be one of the values returned by AudioFormat::getPossibleBitDepths().
    */
    [[nodiscard]] AudioFormatWriterOptions withBitsPerSample (int x) const
    {
        return withMember (*this, &AudioFormatWriterOptions::bitsPerSample, x);
    }

    /** Returns a copy of these options with the specified metadata container.

        As an alternative to this function, you can specify the key-value pairs one-by-one using
        the withMetadata function.

        Subsequent calls of this function overwrites all previously added metadata.

        This parameter is a set of metadata values that the writer should try to write to the stream.
        Exactly what these are depends on the format, and the subclass doesn't actually have to do
        anything with them if it doesn't want to. Have a look at the specific format implementation
        classes to see possible values that can be used.
    */
    [[nodiscard]] AudioFormatWriterOptions withMetadataValues (const std::unordered_map<String, String>& x) const
    {
        return withMember (*this, &AudioFormatWriterOptions::metadataValues, x);
    }

    /** Returns a copy of these options with the specified metadata added.

        Subsequent calls of this function adds new metadata values, while also preserving the
        previously added ones.

        Here you can specify metadata values that the writer should try to write to the stream.
        Exactly what these are depends on the format, and the subclass doesn't actually have to do
        anything with them if it doesn't want to. Have a look at the specific format implementation
        classes to see possible values that can be used.
    */
    [[nodiscard]] AudioFormatWriterOptions withMetadata (const String& key, const String& value) const
    {
        auto copy = *this;
        copy.metadataValues[key] = value;
        return copy;
    }

    /** Returns a copy of these options with the specified quality option index.

        The index of one of the items returned by the AudioFormat::getQualityOptions() method.
    */
    [[nodiscard]] AudioFormatWriterOptions withQualityOptionIndex (int x) const
    {
        return withMember (*this, &AudioFormatWriterOptions::qualityOptionIndex, x);
    }

    /** @see withSampleRate() */
    [[nodiscard]] auto getSampleRate()         const { return sampleRate; }
    /** @see withChannelLayout() */
    [[nodiscard]] auto getChannelLayout()      const { return channelLayout; }
    /** @see withNumChannels() */
    [[nodiscard]] auto getNumChannels()        const { return channelLayout.has_value() ? channelLayout->size() : numChannels; }
    /** @see withBitsPerSample() */
    [[nodiscard]] auto getBitsPerSample()      const { return bitsPerSample; }
    /** @see withMetadataValues() */
    [[nodiscard]] auto getMetadataValues()     const { return metadataValues; }
    /** @see withQualityOptionIndex() */
    [[nodiscard]] auto getQualityOptionIndex() const { return qualityOptionIndex; }
    /** @see withSampleFormat() */
    [[nodiscard]] auto getSampleFormat()       const { return sampleFormat; }

private:
    double sampleRate = 48000.0;
    int numChannels = 1;
    std::optional<AudioChannelSet> channelLayout = std::nullopt;
    int bitsPerSample = 16;
    std::unordered_map<String, String> metadataValues;
    int qualityOptionIndex = 0;
    SampleFormat sampleFormat = SampleFormat::automatic;
};

} // namespace higgs
