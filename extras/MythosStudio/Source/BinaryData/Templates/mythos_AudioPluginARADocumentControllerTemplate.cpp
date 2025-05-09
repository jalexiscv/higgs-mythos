/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for an ARA document controller implementation.

  ==============================================================================
*/

%%aradocumentcontroller_headers%%
%%araplaybackrenderer_headers%%

//==============================================================================
higgs::ARAPlaybackRenderer* %%aradocumentcontroller_class_name%%::doCreatePlaybackRenderer() noexcept
{
    return new %%araplaybackrenderer_class_name%% (getDocumentController());
}

//==============================================================================
bool %%aradocumentcontroller_class_name%%::doRestoreObjectsFromStream (higgs::ARAInputStream& input, const higgs::ARARestoreObjectsFilter* filter) noexcept
{
    // You should use this method to read any persistent data associated with
    // your ARA model graph stored in an archive using the supplied ARAInputStream.
    // Be sure to check the ARARestoreObjectsFilter to determine which objects to restore.
    return true;
}

bool %%aradocumentcontroller_class_name%%::doStoreObjectsToStream (higgs::ARAOutputStream& output, const higgs::ARAStoreObjectsFilter* filter) noexcept
{
    // You should use this method to write any persistent data associated with
    // your ARA model graph into the an archive using the supplied ARAOutputStream.
    // Be sure to check the ARAStoreObjectsFilter to determine which objects to store.
    return true;
}

//==============================================================================
// This creates the static ARAFactory instances for the plugin.
const ARA::ARAFactory* HIGGS_CALLTYPE createARAFactory()
{
    return higgs::ARADocumentControllerSpecialisation::createARAFactory<%%aradocumentcontroller_class_name%%>();
}
