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

AnimatedAppComponent::AnimatedAppComponent()
{
    setOpaque (true);
}

void AnimatedAppComponent::setFramesPerSecond (int framesPerSecondIn)
{
    jassert (0 < framesPerSecond && framesPerSecond < 1000);
    framesPerSecond = framesPerSecondIn;
    updateSync();
}

void AnimatedAppComponent::updateSync()
{
    if (useVBlank)
    {
        stopTimer();

        if (vBlankAttachment.isEmpty())
            vBlankAttachment = { this, [this] { timerCallback(); } };
    }
    else
    {
        vBlankAttachment = {};

        const auto interval = 1000 / framesPerSecond;

        if (getTimerInterval() != interval)
            startTimer (interval);
    }
}

void AnimatedAppComponent::setSynchroniseToVBlank (bool syncToVBlank)
{
    useVBlank = syncToVBlank;
    updateSync();
}

int AnimatedAppComponent::getMillisecondsSinceLastUpdate() const noexcept
{
    return (int) (Time::getCurrentTime() - lastUpdateTime).inMilliseconds();
}

void AnimatedAppComponent::timerCallback()
{
    ++totalUpdates;
    update();
    repaint();
    lastUpdateTime = Time::getCurrentTime();
}

} // namespace higgs
