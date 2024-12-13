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

#if ! (HIGGS_LINUX || HIGGS_BSD)

#if HIGGS_MAC || HIGGS_IOS
 #include "../native/higgs_Video_mac.h"
#elif HIGGS_WINDOWS
 #include "../native/higgs_Video_windows.h"
#elif HIGGS_ANDROID
 #include "../native/higgs_Video_android.h"
#endif

//==============================================================================
VideoComponent::VideoComponent (bool useNativeControlsIfAvailable)
    : pimpl (new Pimpl (*this, useNativeControlsIfAvailable))
{
    addAndMakeVisible (pimpl.get());
}

VideoComponent::~VideoComponent()
{
    pimpl.reset();
}

Result VideoComponent::load (const File& file)
{
    return loadInternal (file, false);
}

Result VideoComponent::load (const URL& url)
{
    return loadInternal (url, false);
}

void VideoComponent::loadAsync (const URL& url, std::function<void (const URL&, Result)> callback)
{
    if (callback == nullptr)
    {
        jassertfalse;
        return;
    }

   #if HIGGS_ANDROID || HIGGS_IOS || HIGGS_MAC
    pimpl->loadAsync (url, callback);
   #else
    auto result = loadInternal (url, true);
    callback (url, result);
   #endif
}

void VideoComponent::closeVideo()
{
    pimpl->close();
    // Closing on Android is async and resized() will be called internally by pimpl once
    // close operation finished.
   #if ! HIGGS_ANDROID// TODO HIGGS_IOS too?
    resized();
   #endif
}

bool VideoComponent::isVideoOpen() const                    { return pimpl->isOpen(); }

File VideoComponent::getCurrentVideoFile() const            { return pimpl->currentFile; }
URL VideoComponent::getCurrentVideoURL() const              { return pimpl->currentURL; }

double VideoComponent::getVideoDuration() const             { return pimpl->getDuration(); }
Rectangle<int> VideoComponent::getVideoNativeSize() const   { return pimpl->getNativeSize(); }

void VideoComponent::play()                                 { pimpl->play(); }
void VideoComponent::stop()                                 { pimpl->stop(); }

bool VideoComponent::isPlaying() const                      { return pimpl->isPlaying(); }

void VideoComponent::setPlayPosition (double newPos)        { pimpl->setPosition (newPos); }
double VideoComponent::getPlayPosition() const              { return pimpl->getPosition(); }

void VideoComponent::setPlaySpeed (double newSpeed)         { pimpl->setSpeed (newSpeed); }
double VideoComponent::getPlaySpeed() const                 { return pimpl->getSpeed(); }

void VideoComponent::setAudioVolume (float newVolume)       { pimpl->setVolume (newVolume); }
float VideoComponent::getAudioVolume() const                { return pimpl->getVolume(); }

void VideoComponent::resized()
{
    auto r = getLocalBounds();

    if (isVideoOpen() && ! r.isEmpty())
    {
        auto nativeSize = getVideoNativeSize();

        if (nativeSize.isEmpty())
        {
            // if we've just opened the file and are still waiting for it to
            // figure out the size, start our timer
            if (! isTimerRunning())
                startTimer (50);
        }
        else
        {
            r = RectanglePlacement (RectanglePlacement::centred).appliedTo (nativeSize, r);
            stopTimer();
        }
    }
    else
    {
        stopTimer();
    }

    pimpl->setBounds (r);
}

void VideoComponent::timerCallback()
{
    resized();
}

template <class FileOrURL>
Result VideoComponent::loadInternal (const FileOrURL& fileOrUrl, bool loadAsync)
{
   #if HIGGS_ANDROID || HIGGS_IOS
    ignoreUnused (fileOrUrl, loadAsync);
    // You need to use loadAsync on Android & iOS.
    jassertfalse;
    return Result::fail ("load() is not supported on this platform. Use loadAsync() instead.");
   #else
    auto result = pimpl->load (fileOrUrl);

    if (loadAsync)
        startTimer (50);
    else
        resized();

    return result;
   #endif
}

#endif

} // namespace higgs
