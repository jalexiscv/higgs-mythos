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

AudioPluginFormat::AudioPluginFormat() {}
AudioPluginFormat::~AudioPluginFormat() {}

std::unique_ptr<AudioPluginInstance> AudioPluginFormat::createInstanceFromDescription (const PluginDescription& desc,
                                                                                       double initialSampleRate,
                                                                                       int initialBufferSize)
{
    String errorMessage;
    return createInstanceFromDescription (desc, initialSampleRate, initialBufferSize, errorMessage);
}

std::unique_ptr<AudioPluginInstance> AudioPluginFormat::createInstanceFromDescription (const PluginDescription& desc,
                                                                                       double initialSampleRate,
                                                                                       int initialBufferSize,
                                                                                       String& errorMessage)
{
    if (MessageManager::getInstance()->isThisTheMessageThread()
          && requiresUnblockedMessageThreadDuringCreation (desc))
    {
        errorMessage = NEEDS_TRANS ("This plug-in cannot be instantiated synchronously");
        return {};
    }

    WaitableEvent finishedSignal;
    std::unique_ptr<AudioPluginInstance> instance;

    auto callback = [&] (std::unique_ptr<AudioPluginInstance> p, const String& error)
    {
       errorMessage = error;
       instance = std::move (p);
       finishedSignal.signal();
    };

    if (! MessageManager::getInstance()->isThisTheMessageThread())
        createPluginInstanceAsync (desc, initialSampleRate, initialBufferSize, std::move (callback));
    else
        createPluginInstance (desc, initialSampleRate, initialBufferSize, std::move (callback));

    finishedSignal.wait();
    return instance;
}

struct AudioPluginFormat::AsyncCreateMessage final : public Message
{
    AsyncCreateMessage (const PluginDescription& d, double sr, int size, PluginCreationCallback call)
        : desc (d), sampleRate (sr), bufferSize (size), callbackToUse (std::move (call))
    {
    }

    PluginDescription desc;
    double sampleRate;
    int bufferSize;
    PluginCreationCallback callbackToUse;
};

void AudioPluginFormat::createPluginInstanceAsync (const PluginDescription& description,
                                                   double initialSampleRate, int initialBufferSize,
                                                   PluginCreationCallback callback)
{
    jassert (callback != nullptr);
    postMessage (new AsyncCreateMessage (description, initialSampleRate, initialBufferSize, std::move (callback)));
}

void AudioPluginFormat::handleMessage (const Message& message)
{
    if (auto m = dynamic_cast<const AsyncCreateMessage*> (&message))
        createPluginInstance (m->desc, m->sampleRate, m->bufferSize, std::move (m->callbackToUse));
}

} // namespace higgs
