/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for an ARA document controller implementation.

  ==============================================================================
*/

#pragma once

#include <higgs_audio_processors/higgs_audio_processors.h>

//==============================================================================
/**
*/
class %%aradocumentcontroller_class_name%%  : public higgs::ARADocumentControllerSpecialisation
{
public:
    //==============================================================================
    using ARADocumentControllerSpecialisation::ARADocumentControllerSpecialisation;

protected:
    //==============================================================================
    // Override document controller customization methods here

    higgs::ARAPlaybackRenderer* doCreatePlaybackRenderer() noexcept override;

    bool doRestoreObjectsFromStream (higgs::ARAInputStream& input, const higgs::ARARestoreObjectsFilter* filter) noexcept override;
    bool doStoreObjectsToStream (higgs::ARAOutputStream& output, const higgs::ARAStoreObjectsFilter* filter) noexcept override;

private:
    //==============================================================================
    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (%%aradocumentcontroller_class_name%%)
};
