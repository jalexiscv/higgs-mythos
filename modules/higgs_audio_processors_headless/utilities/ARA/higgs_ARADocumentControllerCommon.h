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

#pragma once

namespace higgs
{

class ARADocumentController : public ARA::PlugIn::DocumentController
{
public:
    using ARA::PlugIn::DocumentController::DocumentController;

    template <typename Document_t = ARADocument>
    Document_t* getDocument() const noexcept { return ARA::PlugIn::DocumentController::getDocument<Document_t>(); }

    //==============================================================================
    /** @internal */
    virtual void internalNotifyAudioSourceAnalysisProgressStarted (ARAAudioSource* audioSource) = 0;

    /** @internal */
    virtual void internalNotifyAudioSourceAnalysisProgressUpdated (ARAAudioSource* audioSource, float progress) = 0;

    /** @internal */
    virtual void internalNotifyAudioSourceAnalysisProgressCompleted (ARAAudioSource* audioSource) = 0;

    /** @internal */
    virtual void internalDidUpdateAudioSourceAnalysisProgress (ARAAudioSource* audioSource,
                                                               ARAAudioSource::ARAAnalysisProgressState state,
                                                               float progress) = 0;

    //==============================================================================
    /** @internal */
    virtual void internalNotifyAudioSourceContentChanged (ARAAudioSource* audioSource,
                                                          ARAContentUpdateScopes scopeFlags,
                                                          bool notifyARAHost) = 0;

    /** @internal */
    virtual void internalNotifyAudioModificationContentChanged (ARAAudioModification* audioModification,
                                                                ARAContentUpdateScopes scopeFlags,
                                                                bool notifyARAHost) = 0;

    /** @internal */
    virtual void internalNotifyPlaybackRegionContentChanged (ARAPlaybackRegion* playbackRegion,
                                                             ARAContentUpdateScopes scopeFlags,
                                                             bool notifyARAHost) = 0;

    friend class ARAPlaybackRegionReader;
};

} // namespace higgs
