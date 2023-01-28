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

AudioProcessLoadMeasurer::AudioProcessLoadMeasurer()  = default;
AudioProcessLoadMeasurer::~AudioProcessLoadMeasurer() = default;

void AudioProcessLoadMeasurer::reset()
{
    reset (0, 0);
}

void AudioProcessLoadMeasurer::reset (double sampleRate, int blockSize)
{
    const SpinLock::ScopedLockType lock (mutex);

    cpuUsageProportion = 0;
    xruns = 0;

    samplesPerBlock = blockSize;
    msPerSample = (sampleRate > 0.0 && blockSize > 0) ? 1000.0 / sampleRate : 0;
}

void AudioProcessLoadMeasurer::registerBlockRenderTime (double milliseconds)
{
    const SpinLock::ScopedTryLockType lock (mutex);

    if (lock.isLocked())
        registerRenderTimeLocked (milliseconds, samplesPerBlock);
}

void AudioProcessLoadMeasurer::registerRenderTime (double milliseconds, int numSamples)
{
    const SpinLock::ScopedTryLockType lock (mutex);

    if (lock.isLocked())
        registerRenderTimeLocked (milliseconds, numSamples);
}

void AudioProcessLoadMeasurer::registerRenderTimeLocked (double milliseconds, int numSamples)
{
    if (approximatelyEqual (msPerSample, 0.0))
        return;

    const auto maxMilliseconds = numSamples * msPerSample;
    const auto usedProportion = milliseconds / maxMilliseconds;
    const auto filterAmount = 0.2;
    const auto proportion = cpuUsageProportion.load();
    cpuUsageProportion = proportion + filterAmount * (usedProportion - proportion);

    if (milliseconds > maxMilliseconds)
        ++xruns;
}

double AudioProcessLoadMeasurer::getLoadAsProportion() const   { return jlimit (0.0, 1.0, cpuUsageProportion.load()); }
double AudioProcessLoadMeasurer::getLoadAsPercentage() const   { return 100.0 * getLoadAsProportion(); }

int AudioProcessLoadMeasurer::getXRunCount() const             { return xruns; }

AudioProcessLoadMeasurer::ScopedTimer::ScopedTimer (AudioProcessLoadMeasurer& p)
    : ScopedTimer (p, p.samplesPerBlock)
{
}

AudioProcessLoadMeasurer::ScopedTimer::ScopedTimer (AudioProcessLoadMeasurer& p, int numSamplesInBlock)
    : owner (p), startTime (Time::getMillisecondCounterHiRes()), samplesInBlock (numSamplesInBlock)
{
    // numSamplesInBlock should never be zero. Did you remember to call AudioProcessLoadMeasurer::reset(),
    // passing the expected samples per block?
    jassert (numSamplesInBlock);
}

AudioProcessLoadMeasurer::ScopedTimer::~ScopedTimer()
{
    owner.registerRenderTime (Time::getMillisecondCounterHiRes() - startTime, samplesInBlock);
}

} // namespace higgs
