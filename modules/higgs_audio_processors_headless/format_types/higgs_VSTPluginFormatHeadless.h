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

#if HIGGS_INTERNAL_HAS_VST || DOXYGEN

namespace higgs
{

//==============================================================================
/**
    Implements a plugin format manager for VSTs.

    @tags{Audio}
*/
class HIGGS_API  VSTPluginFormatHeadless   : public AudioPluginFormat
{
public:
    //==============================================================================
    VSTPluginFormatHeadless() = default;

    //==============================================================================
    /** Given a suitable function pointer to a VSTPluginMain function, this will attempt to
        instantiate and return a plugin for it.
    */
    static AudioPluginInstance* createCustomVSTFromMainCall (void* entryPointFunction,
                                                             double initialSampleRate,
                                                             int initialBufferSize);

    using ExtraFunctions = AudioPluginExtensions::VSTClient::ExtraFunctions;

    //==============================================================================
    /** Given a VstEffectInterface* (aka vst::AEffect*), this will return the higgs AudioPluginInstance
        that is being used to wrap it
    */
    static AudioPluginInstance* getPluginInstanceFromVstEffectInterface (void* aEffect);

    //==============================================================================
    static String getFormatName()                   { return "VST"; }
    String getName() const override                 { return getFormatName(); }
    bool canScanForPlugins() const override         { return true; }
    bool isTrivialToScan() const override           { return false; }

    void findAllTypesForFile (OwnedArray<PluginDescription>&, const String& fileOrIdentifier) override;
    bool fileMightContainThisPluginType (const String& fileOrIdentifier) override;
    String getNameOfPluginFromIdentifier (const String& fileOrIdentifier) override;
    bool pluginNeedsRescanning (const PluginDescription&) override;
    StringArray searchPathsForPlugins (const FileSearchPath&, bool recursive, bool) override;
    bool doesPluginStillExist (const PluginDescription&) override;
    FileSearchPath getDefaultLocationsToSearch() override;

    /** Can be overridden to receive a callback when each member of a shell plugin is about to be
        tested during a call to findAllTypesForFile().
        Only the name and uid members of the PluginDescription are guaranteed to be valid when
        this is called.
    */
    virtual void aboutToScanVSTShellPlugin (const PluginDescription&);

private:
    //==============================================================================
    void createPluginInstance (const PluginDescription&, double initialSampleRate,
                               int initialBufferSize, PluginCreationCallback) override;
    bool requiresUnblockedMessageThreadDuringCreation (const PluginDescription&) const override;
    void recursiveFileSearch (StringArray&, const File&, bool recursive);

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VSTPluginFormatHeadless)
};

} // namespace higgs

#endif
