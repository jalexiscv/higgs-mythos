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

AudioIODeviceType::AudioIODeviceType (const String& name)
    : typeName (name)
{
}

AudioIODeviceType::~AudioIODeviceType()
{
}

//==============================================================================
void AudioIODeviceType::addListener (Listener* l)      { listeners.add (l); }
void AudioIODeviceType::removeListener (Listener* l)   { listeners.remove (l); }

void AudioIODeviceType::callDeviceChangeListeners()
{
    listeners.call ([] (Listener& l) { l.audioDeviceListChanged(); });
}

//==============================================================================
#if HIGGS_MAC
 AudioIODeviceType* AudioIODeviceType::createAudioIODeviceType_CoreAudio()  { return new CoreAudioClasses::CoreAudioIODeviceType(); }
#else
 AudioIODeviceType* AudioIODeviceType::createAudioIODeviceType_CoreAudio()  { return nullptr; }
#endif

#if HIGGS_IOS
 AudioIODeviceType* AudioIODeviceType::createAudioIODeviceType_iOSAudio()   { return new iOSAudioIODeviceType(); }
#else
 AudioIODeviceType* AudioIODeviceType::createAudioIODeviceType_iOSAudio()   { return nullptr; }
#endif

#if HIGGS_WINDOWS && HIGGS_WASAPI
 AudioIODeviceType* AudioIODeviceType::createAudioIODeviceType_WASAPI (WASAPIDeviceMode deviceMode)
 {
     return new WasapiClasses::WASAPIAudioIODeviceType (deviceMode);
 }

 AudioIODeviceType* AudioIODeviceType::createAudioIODeviceType_WASAPI (bool exclusiveMode)
 {
     return createAudioIODeviceType_WASAPI (exclusiveMode ? WASAPIDeviceMode::exclusive
                                                          : WASAPIDeviceMode::shared);
 }
#else
 AudioIODeviceType* AudioIODeviceType::createAudioIODeviceType_WASAPI (WASAPIDeviceMode)  { return nullptr; }
 AudioIODeviceType* AudioIODeviceType::createAudioIODeviceType_WASAPI (bool)              { return nullptr; }
#endif

#if HIGGS_WINDOWS && HIGGS_DIRECTSOUND
 AudioIODeviceType* AudioIODeviceType::createAudioIODeviceType_DirectSound()  { return new DSoundAudioIODeviceType(); }
#else
 AudioIODeviceType* AudioIODeviceType::createAudioIODeviceType_DirectSound()  { return nullptr; }
#endif

#if HIGGS_WINDOWS && HIGGS_ASIO
 AudioIODeviceType* AudioIODeviceType::createAudioIODeviceType_ASIO()         { return new ASIOAudioIODeviceType(); }
#else
 AudioIODeviceType* AudioIODeviceType::createAudioIODeviceType_ASIO()         { return nullptr; }
#endif

#if (HIGGS_LINUX || HIGGS_BSD) && HIGGS_ALSA
 AudioIODeviceType* AudioIODeviceType::createAudioIODeviceType_ALSA()         { return createAudioIODeviceType_ALSA_PCMDevices(); }
#else
 AudioIODeviceType* AudioIODeviceType::createAudioIODeviceType_ALSA()         { return nullptr; }
#endif

#if (HIGGS_LINUX || HIGGS_BSD || HIGGS_MAC || HIGGS_WINDOWS) && HIGGS_JACK
 AudioIODeviceType* AudioIODeviceType::createAudioIODeviceType_JACK()         { return new JackAudioIODeviceType(); }
#else
 AudioIODeviceType* AudioIODeviceType::createAudioIODeviceType_JACK()         { return nullptr; }
#endif

#if HIGGS_ANDROID
 AudioIODeviceType* AudioIODeviceType::createAudioIODeviceType_Android()
 {
    #if HIGGS_USE_ANDROID_OBOE
     if (isOboeAvailable())
         return nullptr;
    #endif

    #if HIGGS_USE_ANDROID_OPENSLES
     if (isOpenSLAvailable())
         return nullptr;
    #endif

     return new AndroidAudioIODeviceType();
 }
#else
 AudioIODeviceType* AudioIODeviceType::createAudioIODeviceType_Android()   { return nullptr; }
#endif

#if HIGGS_ANDROID && HIGGS_USE_ANDROID_OPENSLES
 AudioIODeviceType* AudioIODeviceType::createAudioIODeviceType_OpenSLES()
 {
     return isOpenSLAvailable() ? new OpenSLAudioDeviceType() : nullptr;
 }
#else
 AudioIODeviceType* AudioIODeviceType::createAudioIODeviceType_OpenSLES()  { return nullptr; }
#endif

#if HIGGS_ANDROID && HIGGS_USE_ANDROID_OBOE
 AudioIODeviceType* AudioIODeviceType::createAudioIODeviceType_Oboe()
 {
     return isOboeAvailable() ? new OboeAudioIODeviceType() : nullptr;
 }
#else
 AudioIODeviceType* AudioIODeviceType::createAudioIODeviceType_Oboe()      { return nullptr; }
#endif

} // namespace higgs
