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

bool AudioPlayHead::FrameRate::operator== (const FrameRate& other) const
{
    const auto tie = [] (const FrameRate& x) { return std::tie (x.base, x.drop, x.pulldown); };
    return tie (*this) == tie (other);
}

bool AudioPlayHead::TimeSignature::operator== (const TimeSignature& other) const
{
    const auto tie = [] (auto& x) { return std::tie (x.numerator, x.denominator); };
    return tie (*this) == tie (other);
}

bool AudioPlayHead::LoopPoints::operator== (const LoopPoints& other) const
{
    const auto tie = [] (auto& x) { return std::tie (x.ppqStart, x.ppqEnd); };
    return tie (*this) == tie (other);
}

bool AudioPlayHead::CurrentPositionInfo::operator== (const CurrentPositionInfo& other) const noexcept
{
    const auto tie = [] (const CurrentPositionInfo& i)
    {
        return std::tie (i.timeInSamples,
                         i.ppqPosition,
                         i.editOriginTime,
                         i.ppqPositionOfLastBarStart,
                         i.frameRate,
                         i.isPlaying,
                         i.isRecording,
                         i.bpm,
                         i.timeSigNumerator,
                         i.timeSigDenominator,
                         i.ppqLoopStart,
                         i.ppqLoopEnd,
                         i.isLooping);
    };

    return tie (*this) == tie (other);
}

bool AudioPlayHead::PositionInfo::operator== (const PositionInfo& other) const noexcept
{
    const auto tie = [] (const PositionInfo& i)
    {
        return std::make_tuple (i.getTimeInSamples(),
                                i.getTimeInSeconds(),
                                i.getPpqPosition(),
                                i.getEditOriginTime(),
                                i.getPpqPositionOfLastBarStart(),
                                i.getFrameRate(),
                                i.getBarCount(),
                                i.getTimeSignature(),
                                i.getBpm(),
                                i.getLoopPoints(),
                                i.getHostTimeNs(),
                                i.getIsPlaying(),
                                i.getIsRecording(),
                                i.getIsLooping());
    };

    return tie (*this) == tie (other);
}

bool AudioPlayHead::canControlTransport()                                          { return false; }
void AudioPlayHead::transportPlay ([[maybe_unused]] bool shouldStartPlaying)       {}
void AudioPlayHead::transportRecord ([[maybe_unused]] bool shouldStartRecording)   {}
void AudioPlayHead::transportRewind()                                              {}

} // namespace higgs
