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

#include "Modules/mythos_AvailableModulesList.h"

class ProjectExporter;
class LibraryModule;
class EnabledModulesList;
class ProjectSaver;

namespace ProjectMessages
{
    namespace Ids
    {
       #define DECLARE_ID(name)  static const Identifier name (#name)

        DECLARE_ID (projectMessages);

        DECLARE_ID (cppStandard);
        DECLARE_ID (moduleNotFound);
        DECLARE_ID (higgsPath);
        DECLARE_ID (mythosFileModified);
        DECLARE_ID (missingModuleDependencies);
        DECLARE_ID (oldMythosStudio);
        DECLARE_ID (newVersionAvailable);
        DECLARE_ID (pluginCodeInvalid);
        DECLARE_ID (manufacturerCodeInvalid);
        DECLARE_ID (deprecatedExporter);
        DECLARE_ID (unsupportedArm32Config);
        DECLARE_ID (arm64Warning);
        DECLARE_ID (lv2PaceProtectionWarning);

        DECLARE_ID (notification);
        DECLARE_ID (warning);

        DECLARE_ID (isVisible);

       #undef DECLARE_ID
    }

    inline Identifier getTypeForMessage (const Identifier& message)
    {
        static Identifier warnings[] = { Ids::cppStandard, Ids::moduleNotFound, Ids::higgsPath,
                                         Ids::mythosFileModified, Ids::missingModuleDependencies,
                                         Ids::oldMythosStudio, Ids::pluginCodeInvalid, Ids::manufacturerCodeInvalid,
                                         Ids::deprecatedExporter, Ids::unsupportedArm32Config, Ids::arm64Warning,
                                         Ids::lv2PaceProtectionWarning };

        if (std::find (std::begin (warnings), std::end (warnings), message) != std::end (warnings))
            return Ids::warning;

        static Identifier notifications[] = { Ids::newVersionAvailable };

        if (std::find (std::begin (notifications), std::end (notifications), message) != std::end (notifications))
            return Ids::notification;

        jassertfalse;
        return {};
    }

    inline String getTitleForMessage (const Identifier& message)
    {
        if (message == Ids::cppStandard)                return "C++ Standard";
        if (message == Ids::moduleNotFound)             return "Module Not Found";
        if (message == Ids::higgsPath)                   return "HIGGS Path";
        if (message == Ids::mythosFileModified)          return "Project File Modified";
        if (message == Ids::missingModuleDependencies)  return "Missing Module Dependencies";
        if (message == Ids::oldMythosStudio)                return "MythosStudio Out of Date";
        if (message == Ids::newVersionAvailable)        return "New Version Available";
        if (message == Ids::pluginCodeInvalid)          return "Invalid Plugin Code";
        if (message == Ids::manufacturerCodeInvalid)    return "Invalid Manufacturer Code";
        if (message == Ids::deprecatedExporter)         return "Deprecated Exporter";
        if (message == Ids::unsupportedArm32Config)     return "Unsupported Architecture";
        if (message == Ids::arm64Warning)               return "Prefer arm64ec over arm64";
        if (message == Ids::lv2PaceProtectionWarning)   return "LV2 Plugin Format Not Supported by PACE";

        jassertfalse;
        return {};
    }

    inline String getDescriptionForMessage (const Identifier& message)
    {
        if (message == Ids::cppStandard)                return "Module(s) have a higher C++ standard requirement than the project.";
        if (message == Ids::moduleNotFound)             return "Module(s) could not be found at the specified paths.";
        if (message == Ids::higgsPath)                   return "The path to your HIGGS folder is incorrect.";
        if (message == Ids::mythosFileModified)          return "The .mythos file has been modified since the last save.";
        if (message == Ids::missingModuleDependencies)  return "Module(s) have missing dependencies.";
        if (message == Ids::oldMythosStudio)                return "The version of the MythosStudio you are using is out of date.";
        if (message == Ids::newVersionAvailable)        return "A new version of HIGGS is available to download.";
        if (message == Ids::pluginCodeInvalid)          return "The plugin code should be exactly four characters in length.";
        if (message == Ids::manufacturerCodeInvalid)    return "The manufacturer code should be exactly four characters in length.";
        if (message == Ids::deprecatedExporter)         return "The project includes a deprecated exporter.";
        if (message == Ids::unsupportedArm32Config)     return "The project includes a Visual Studio configuration that uses the 32-bit Arm architecture, which is no longer supported. This configuration has been hidden, and will be removed on save.";
        if (message == Ids::arm64Warning)               return "For software where interoperability is a concern (such as plugins and hosts), arm64ec will provide the best compatibility with existing x64 software";
        if (message == Ids::lv2PaceProtectionWarning)   return "Either disable the LV2 plugin format or disable PACE Fusion protection on all exporters.";

        jassertfalse;
        return {};
    }

    using MessageAction = std::pair<String, std::function<void()>>;
}

// Can be shared between multiple classes wanting to create a MessageBox. Ensures that there is one
// MessageBox active at a given time.
class MessageBoxQueue : private AsyncUpdater
{
public:
    struct Listener
    {
        using CreatorFunction = std::function<ScopedMessageBox (MessageBoxOptions, std::function<void (int)>)>;

        virtual ~Listener() = default;

        virtual void canCreateMessageBox (CreatorFunction) = 0;
    };

    void handleAsyncUpdate() override
    {
        schedule();
    }

    auto addListener (Listener& l)
    {
        triggerAsyncUpdate();
        return listeners.addScoped (l);
    }

private:
    ScopedMessageBox create (MessageBoxOptions options, std::function<void (int)> callback)
    {
        hasActiveMessageBox = true;

        return AlertWindow::showScopedAsync (options, [this, cb = std::move (callback)] (int result)
                                             {
                                                 cb (result);
                                                 hasActiveMessageBox = false;
                                                 triggerAsyncUpdate();
                                             });
    }

    void schedule()
    {
        if (hasActiveMessageBox)
            return;

        auto& currentListeners = listeners.getListeners();

        if (! currentListeners.isEmpty())
        {
            currentListeners[0]->canCreateMessageBox ([this] (auto o, auto c)
                                                              {
                                                                  return create (o, c);
                                                              });
        }
    }

    ListenerList<Listener> listeners;
    bool hasActiveMessageBox = false;
};

enum class Async { no, yes };

//==============================================================================
class Project final : public FileBasedDocument,
                      private ValueTree::Listener,
                      private ChangeListener,
                      private AvailableModulesList::Listener,
                      private MessageBoxQueue::Listener
{
public:
    //==============================================================================
    Project (const File&);
    ~Project() override;

    //==============================================================================
    String getDocumentTitle() override;
    Result loadDocument (const File& file) override;
    Result saveDocument (const File& file) override;
    void saveDocumentAsync (const File& file, std::function<void (Result)> callback) override;

    void saveProject (Async, ProjectExporter* exporterToSave, std::function<void (Result)> onCompletion);
    void saveAndMoveTemporaryProject (bool openInIDE);
    Result saveResourcesOnly();
    void openProjectInIDE (ProjectExporter& exporterToOpen);

    File getLastDocumentOpened() override;
    void setLastDocumentOpened (const File& file) override;

    void setTitle (const String& newTitle);

    //==============================================================================
    File getProjectFolder() const                               { return getFile().getParentDirectory(); }
    File getGeneratedCodeFolder() const                         { return getFile().getSiblingFile ("HiggsLibraryCode"); }
    File getSourceFilesFolder() const                           { return getProjectFolder().getChildFile ("Source"); }
    File getLocalModulesFolder() const                          { return getGeneratedCodeFolder().getChildFile ("modules"); }
    File getLocalModuleFolder (const String& moduleID) const    { return getLocalModulesFolder().getChildFile (moduleID); }
    File getAppIncludeFile() const                              { return getGeneratedCodeFolder().getChildFile (getHiggsSourceHFilename()); }

    File getBinaryDataCppFile (int index) const;
    File getBinaryDataHeaderFile() const                        { return getBinaryDataCppFile (0).withFileExtension (".h"); }

    static String getAppConfigFilename()                        { return "AppConfig.h"; }
    static String getPluginDefinesFilename()                    { return "HiggsPluginDefines.h"; }
    static String getHiggsSourceHFilename()                      { return "HiggsHeader.h"; }
    static String getHiggsLV2DefinesFilename()                   { return "HiggsLV2Defines.h"; }
    static String getLV2FileWriterName()                        { return "higgs_lv2_helper"; }
    static String getVST3FileWriterName()                       { return "higgs_vst3_helper"; }

    //==============================================================================
    template <class FileType>
    bool shouldBeAddedToBinaryResourcesByDefault (const FileType& file)
    {
        return ! file.hasFileExtension (sourceOrHeaderFileExtensions);
    }

    File resolveFilename (String filename) const;
    String getRelativePathForFile (const File& file) const;

    //==============================================================================
    // Creates editors for the project settings
    void createPropertyEditors (PropertyListBuilder&);

    //==============================================================================
    ValueTree getProjectRoot() const                     { return projectRoot; }
    Value getProjectValue (const Identifier& name)       { return projectRoot.getPropertyAsValue (name, getUndoManagerFor (projectRoot)); }
    var   getProjectVar   (const Identifier& name) const { return projectRoot.getProperty        (name); }

    const build_tools::ProjectType& getProjectType() const;
    String getProjectTypeString() const                  { return projectTypeValue.get(); }
    void setProjectType (const String& newProjectType)   { projectTypeValue = newProjectType; }

    String getProjectNameString() const                  { return projectNameValue.get(); }
    String getProjectFilenameRootString()                { return File::createLegalFileName (getDocumentTitle()); }
    String getProjectUIDString() const                   { return projectUIDValue.get(); }

    String getProjectLineFeed() const                    { return projectLineFeedValue.get(); }

    String getVersionString() const                      { return versionValue.get(); }
    String getVersionAsHex() const                       { return build_tools::getVersionAsHex (getVersionString()); }
    int getVersionAsHexInteger() const                   { return build_tools::getVersionAsHexInteger (getVersionString()); }
    void setProjectVersion (const String& newVersion)    { versionValue = newVersion; }

    String getBundleIdentifierString() const             { return bundleIdentifierValue.get(); }
    String getDefaultBundleIdentifierString() const;
    String getDefaultCompanyWebsiteString() const;
    String getDefaultAAXIdentifierString() const         { return getDefaultBundleIdentifierString(); }
    String getDefaultPluginManufacturerString() const;
    String getDefaultLV2URI() const                      { return getCompanyWebsiteString() + "/plugins/" + build_tools::makeValidIdentifier (getProjectNameString(), false, true, false); }
    String getDefaultARAFactoryIDString() const;
    String getDefaultARADocumentArchiveID() const;
    String getDefaultARACompatibleArchiveIDs() const;

    String getCompanyNameString() const                  { return companyNameValue.get(); }
    String getCompanyCopyrightString() const             { return companyCopyrightValue.get(); }
    String getCompanyWebsiteString() const               { return companyWebsiteValue.get(); }
    String getCompanyEmailString() const                 { return companyEmailValue.get(); }

    String getHeaderSearchPathsString() const            { return headerSearchPathsValue.get(); }

    StringPairArray getPreprocessorDefs() const          { return parsedPreprocessorDefs; }

    int getMaxBinaryFileSize() const                     { return maxBinaryFileSizeValue.get(); }
    bool shouldIncludeBinaryInHiggsHeader() const         { return includeBinaryDataInHiggsHeaderValue.get(); }
    String getBinaryDataNamespaceString() const          { return binaryDataNamespaceValue.get(); }

    static StringArray getCppStandardStrings()           { return { "C++17", "C++20", "Use Latest" }; }
    static Array<var> getCppStandardVars()               { return { "17",    "20",    "latest" }; }

    static String getLatestNumberedCppStandardString()
    {
        auto cppStandardVars = getCppStandardVars();
        return cppStandardVars[cppStandardVars.size() - 2];
    }

    String getCppStandardString() const                  { return cppStandardValue.get(); }

    StringArray getCompilerFlagSchemes() const;
    void addCompilerFlagScheme (const String&);
    void removeCompilerFlagScheme (const String&);

    String getPostExportShellCommandPosixString() const  { return postExportShellCommandPosixValue.get(); }
    String getPostExportShellCommandWinString() const    { return postExportShellCommandWinValue.get(); }

    bool shouldUseAppConfig() const                      { return useAppConfigValue.get(); }
    bool shouldAddUsingNamespaceToHiggsHeader() const     { return addUsingNamespaceToHiggsHeader.get(); }

    //==============================================================================
    String getPluginNameString() const                { return pluginNameValue.get(); }
    String getPluginDescriptionString() const         { return pluginDescriptionValue.get();}
    String getPluginManufacturerString() const        { return pluginManufacturerValue.get(); }
    String getPluginManufacturerCodeString() const    { return pluginManufacturerCodeValue.get(); }
    String getPluginCodeString() const                { return pluginCodeValue.get(); }
    String getPluginChannelConfigsString() const      { return pluginChannelConfigsValue.get(); }
    String getAAXIdentifierString() const             { return pluginAAXIdentifierValue.get(); }
    String getARAFactoryIDString() const              { return pluginARAFactoryIDValue.get(); }
    String getARADocumentArchiveIDString() const      { return pluginARAArchiveIDValue.get(); }
    String getARACompatibleArchiveIDStrings() const   { return pluginARACompatibleArchiveIDsValue.get(); }
    String getPluginAUExportPrefixString() const      { return pluginAUExportPrefixValue.get(); }
    String getPluginAUMainTypeString() const          { return pluginAUMainTypeValue.get(); }
    String getVSTNumMIDIInputsString() const          { return pluginVSTNumMidiInputsValue.get(); }
    String getVSTNumMIDIOutputsString() const         { return pluginVSTNumMidiOutputsValue.get(); }

    static bool checkMultiChoiceVar (const ValueTreePropertyWithDefault& valueToCheck, Identifier idToCheck) noexcept
    {
        if (! valueToCheck.get().isArray())
            return false;

        auto v = valueToCheck.get();

        if (auto* varArray = v.getArray())
            return varArray->contains (idToCheck.toString());

        return false;
    }

    bool isAudioPluginProject() const                 { return getProjectType().isAudioPlugin(); }

    bool shouldBuildVST() const                       { return isAudioPluginProject() && checkMultiChoiceVar (pluginFormatsValue, Ids::buildVST); }
    bool shouldBuildVST3() const                      { return isAudioPluginProject() && checkMultiChoiceVar (pluginFormatsValue, Ids::buildVST3); }
    bool shouldBuildAU() const                        { return isAudioPluginProject() && checkMultiChoiceVar (pluginFormatsValue, Ids::buildAU); }
    bool shouldBuildAUv3() const                      { return isAudioPluginProject() && checkMultiChoiceVar (pluginFormatsValue, Ids::buildAUv3); }
    bool shouldBuildAAX() const                       { return isAudioPluginProject() && checkMultiChoiceVar (pluginFormatsValue, Ids::buildAAX); }
    bool shouldBuildStandalonePlugin() const          { return isAudioPluginProject() && checkMultiChoiceVar (pluginFormatsValue, Ids::buildStandalone); }
    bool shouldBuildUnityPlugin() const               { return isAudioPluginProject() && checkMultiChoiceVar (pluginFormatsValue, Ids::buildUnity); }
    bool shouldBuildLV2() const                       { return isAudioPluginProject() && checkMultiChoiceVar (pluginFormatsValue, Ids::buildLV2); }
    bool shouldEnableIAA() const                      { return isAudioPluginProject() && checkMultiChoiceVar (pluginFormatsValue, Ids::enableIAA); }
    bool shouldEnableARA() const                      { return (isAudioPluginProject() && checkMultiChoiceVar (pluginFormatsValue, Ids::enableARA)) || getProjectType().isARAAudioPlugin(); }

    bool isPluginSynth() const                        { return checkMultiChoiceVar (pluginCharacteristicsValue, Ids::pluginIsSynth); }
    bool pluginWantsMidiInput() const                 { return checkMultiChoiceVar (pluginCharacteristicsValue, Ids::pluginWantsMidiIn); }
    bool pluginProducesMidiOutput() const             { return checkMultiChoiceVar (pluginCharacteristicsValue, Ids::pluginProducesMidiOut); }
    bool isPluginMidiEffect() const                   { return checkMultiChoiceVar (pluginCharacteristicsValue, Ids::pluginIsMidiEffectPlugin); }
    bool pluginEditorNeedsKeyFocus() const            { return checkMultiChoiceVar (pluginCharacteristicsValue, Ids::pluginEditorRequiresKeys); }
    bool isPluginAAXBypassDisabled() const            { return checkMultiChoiceVar (pluginCharacteristicsValue, Ids::pluginAAXDisableBypass); }
    bool isPluginAAXMultiMonoDisabled() const         { return checkMultiChoiceVar (pluginCharacteristicsValue, Ids::pluginAAXDisableMultiMono); }

    void disableStandaloneForARAPlugIn();

    static StringArray getAllAUMainTypeStrings() noexcept;
    static Array<var> getAllAUMainTypeVars() noexcept;
    Array<var> getDefaultAUMainTypes() const noexcept;

    String getDefaultLV2PluginClass() const noexcept;

    static StringArray getAllVSTCategoryStrings() noexcept;
    Array<var> getDefaultVSTCategories() const noexcept;

    String getDefaultVST3Categories() const noexcept;

    static StringArray getAllAAXCategoryStrings() noexcept;
    static Array<var> getAllAAXCategoryVars() noexcept;
    Array<var> getDefaultAAXCategories() const noexcept;

    bool getDefaultEnableARA() const noexcept;
    static StringArray getAllARAContentTypeStrings() noexcept;
    static Array<var> getAllARAContentTypeVars() noexcept;
    Array<var> getDefaultARAContentTypes() const noexcept;

    static StringArray getAllARATransformationFlagStrings() noexcept;
    static Array<var> getAllARATransformationFlagVars() noexcept;
    Array<var> getDefaultARATransformationFlags() const noexcept;

    String getAUMainTypeString() const noexcept;
    bool isAUSandBoxSafe() const noexcept;
    String getLV2PluginClassString() const noexcept;
    String getVSTCategoryString() const noexcept;
    String getVST3CategoryString() const noexcept;
    int getAAXCategory() const noexcept;
    int getARAContentTypes() const noexcept;
    int getARATransformationFlags() const noexcept;

    String getIAATypeCode() const;
    String getIAAPluginName() const;

    String getUnityScriptName() const    { return addUnityPluginPrefixIfNecessary (getProjectNameString()) + "_UnityScript.cs"; }
    static String addUnityPluginPrefixIfNecessary (const String& name)
    {
        if (! name.startsWithIgnoreCase ("audioplugin"))
            return "audioplugin_" + name;

        return name;
    }

    String getLV2URI() const        { return pluginLV2URIValue.get(); }

    //==============================================================================
    bool isAUPluginHost()   const;
    bool isVSTPluginHost()  const;
    bool isVST3PluginHost() const;
    bool isLV2PluginHost()  const;
    bool isARAPluginHost()  const;

    //==============================================================================
    bool shouldBuildTargetType (build_tools::ProjectType::Target::Type targetType) const noexcept;
    static build_tools::ProjectType::Target::Type getTargetTypeFromFilePath (const File& file, bool returnSharedTargetIfNoValidSuffix);

    static String getTargetSuffixWithoutSpaces (const String& target)
    {
        if (target.isEmpty())
            return {};

        return String ("_") + target.removeCharacters (" ");
    }

    String getTargetBaseName (const String& target)
    {
        return getProjectFilenameRootString() + getTargetSuffixWithoutSpaces (target);
    }

    //==============================================================================
    StringPairArray getAppConfigDefs();
    StringPairArray getAudioPluginFlags() const;

    //==============================================================================
    class Item
    {
    public:
        //==============================================================================
        Item (Project& project, const ValueTree& itemNode, bool isModuleCode);
        Item (const Item& other);

        static Item createGroup (Project& project, const String& name, const String& uid, bool isModuleCode);
        void initialiseMissingProperties();

        //==============================================================================
        bool isValid() const                            { return state.isValid(); }
        bool operator== (const Item& other) const       { return state == other.state && &project == &other.project; }
        bool operator!= (const Item& other) const       { return ! operator== (other); }

        //==============================================================================
        bool isFile() const;
        bool isGroup() const;
        bool isMainGroup() const;
        bool isImageFile() const;
        bool isSourceFile() const;

        String getID() const;
        void setID (const String& newID);
        Item findItemWithID (const String& targetId) const; // (recursive search)

        //==============================================================================
        Value getNameValue();
        String getName() const;
        File getFile() const;
        String getFilePath() const;
        void setFile (const File& file);
        void setFile (const build_tools::RelativePath& file);
        File determineGroupFolder() const;
        bool renameFile (const File& newFile);

        bool shouldBeAddedToTargetProject() const;
        bool shouldBeAddedToTargetExporter (const ProjectExporter&) const;
        bool shouldBeCompiled() const;
        Value getShouldCompileValue();

        bool shouldBeAddedToBinaryResources() const;
        Value getShouldAddToBinaryResourcesValue();

        bool shouldBeAddedToXcodeResources() const;
        Value getShouldAddToXcodeResourcesValue();

        Value getShouldInhibitWarningsValue();
        bool shouldInhibitWarnings() const;

        bool isModuleCode() const;

        Value getShouldSkipPCHValue();
        bool shouldSkipPCH() const;

        Value getCompilerFlagSchemeValue();
        String getCompilerFlagSchemeString() const;

        void setCompilerFlagScheme (const String&);
        void clearCurrentCompilerFlagScheme();

        //==============================================================================
        bool canContain (const Item& child) const;
        int getNumChildren() const                      { return state.getNumChildren(); }
        Item getChild (int index) const                 { return Item (project, state.getChild (index), belongsToModule); }

        Item addNewSubGroup (const String& name, int insertIndex);
        Item getOrCreateSubGroup (const String& name);
        void addChild (const Item& newChild, int insertIndex);
        bool addFileAtIndex (const File& file, int insertIndex, bool shouldCompile);
        bool addFileRetainingSortOrder (const File& file, bool shouldCompile);
        void addFileUnchecked (const File& file, int insertIndex, bool shouldCompile);
        bool addRelativeFile (const build_tools::RelativePath& file, int insertIndex, bool shouldCompile);
        void removeItemFromProject();
        void sortAlphabetically (bool keepGroupsAtStart, bool recursive);
        Item findItemForFile (const File& file) const;
        bool containsChildForFile (const build_tools::RelativePath& file) const;

        Item getParent() const;
        Item createCopy();

        UndoManager* getUndoManager() const              { return project.getUndoManagerFor (state); }

        Icon getIcon (bool isOpen = false) const;
        bool isIconCrossedOut() const;

        bool needsSaving() const noexcept;

        Project& project;
        ValueTree state;

    private:
        Item& operator= (const Item&);
        bool belongsToModule;
    };

    Item getMainGroup();

    void findAllImageItems (OwnedArray<Item>& items);

    struct IconComposerNameAndItem
    {
        String name;
        Item item;
    };

    std::vector<IconComposerNameAndItem> findAllIconComposerItems();

    //==============================================================================
    ValueTree getExporters();
    int getNumExporters();
    std::unique_ptr<ProjectExporter> createExporter (int index);
    void addNewExporter (const Identifier& exporterIdentifier);
    void createExporterForCurrentPlatform();

    struct ExporterIterator
    {
        ExporterIterator (Project& project);

        bool next();

        ProjectExporter& operator*() const       { return *exporter; }
        ProjectExporter* operator->() const      { return exporter.get(); }

        std::unique_ptr<ProjectExporter> exporter;
        int index;

    private:
        Project& project;
        HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ExporterIterator)
    };

    //==============================================================================
    struct ConfigFlag
    {
        String symbol, description, sourceModuleID;
        ValueTreePropertyWithDefault value;
    };

    ValueTreePropertyWithDefault getConfigFlag (const String& name);
    bool isConfigFlagEnabled (const String& name, bool defaultIsEnabled = false) const;

    //==============================================================================
    void createEnabledModulesList();

          EnabledModulesList& getEnabledModules();
    const EnabledModulesList& getEnabledModules() const;

    AvailableModulesList& getExporterPathsModulesList()  { return exporterPathsModulesList; }
    void rescanExporterPathModules (bool async = false);

    std::pair<String, File> getModuleWithID (const String&);

    //==============================================================================
    PropertiesFile& getStoredProperties() const;

    //==============================================================================
    UndoManager* getUndoManagerFor (const ValueTree&) const             { return nullptr; }
    UndoManager* getUndoManager() const                                 { return nullptr; }

    //==============================================================================
    static const char* projectFileExtension;

    //==============================================================================
    bool updateCachedFileState();
    String getCachedFileStateContent() const noexcept  { return cachedFileState.second; }

    String serialiseProjectXml (std::unique_ptr<XmlElement>) const;

    //==============================================================================
    String getUniqueTargetFolderSuffixForExporter (const Identifier& exporterIdentifier, const String& baseTargetFolder);

    //==============================================================================
    bool isCurrentlySaving() const noexcept              { return saver != nullptr; }

    bool isTemporaryProject() const noexcept             { return tempDirectory != File(); }
    File getTemporaryDirectory() const noexcept          { return tempDirectory; }
    void setTemporaryDirectory (const File&) noexcept;

    //==============================================================================
    ValueTree getProjectMessages() const  { return projectMessages; }

    void addProjectMessage (const Identifier& messageToAdd, std::vector<ProjectMessages::MessageAction>&& messageActions);
    void removeProjectMessage (const Identifier& messageToRemove);

    std::vector<ProjectMessages::MessageAction> getMessageActions (const Identifier& message);

    //==============================================================================
    bool isFileModificationCheckPending() const;
    bool isSaveAndExportDisabled() const;

    MessageBoxQueue messageBoxQueue;

private:
    //==============================================================================
    void valueTreePropertyChanged (ValueTree&, const Identifier&) override;
    void valueTreeChildAdded (ValueTree&, ValueTree&) override;
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override;
    void valueTreeChildOrderChanged (ValueTree&, int, int) override;

    void valueTreeChildAddedOrRemoved (ValueTree&, ValueTree&);

    //==============================================================================
    void canCreateMessageBox (CreatorFunction) override;

    //==============================================================================
    template <typename This>
    static auto& getEnabledModulesImpl (This&);

    //==============================================================================
    struct ProjectFileModificationPoller final : private Timer
    {
        ProjectFileModificationPoller (Project& p);
        bool isCheckPending() const noexcept  { return pending; }

    private:
        void timerCallback() override;
        void reset();

        void resaveProject();
        void reloadProjectFromDisk();

        Project& project;
        bool pending = false;
    };

    //==============================================================================
    ValueTree projectRoot  { Ids::MYTHOSPROJECT };

    ValueTreePropertyWithDefault projectNameValue, projectUIDValue, projectLineFeedValue, projectTypeValue, versionValue, bundleIdentifierValue, companyNameValue,
                                 companyCopyrightValue, companyWebsiteValue, companyEmailValue, cppStandardValue, headerSearchPathsValue, preprocessorDefsValue,
                                 userNotesValue, maxBinaryFileSizeValue, includeBinaryDataInHiggsHeaderValue, binaryDataNamespaceValue, compilerFlagSchemesValue,
                                 postExportShellCommandPosixValue, postExportShellCommandWinValue, useAppConfigValue, addUsingNamespaceToHiggsHeader;

    ValueTreePropertyWithDefault pluginFormatsValue, pluginNameValue, pluginDescriptionValue, pluginManufacturerValue, pluginManufacturerCodeValue,
                                 pluginCodeValue, pluginChannelConfigsValue, pluginCharacteristicsValue, pluginAUExportPrefixValue, pluginAAXIdentifierValue,
                                 pluginAUMainTypeValue, pluginAUSandboxSafeValue, pluginLV2PluginClassValue, pluginVSTCategoryValue, pluginVST3CategoryValue, pluginAAXCategoryValue,
                                 pluginEnableARA, pluginARAAnalyzableContentValue, pluginARAFactoryIDValue, pluginARAArchiveIDValue, pluginARACompatibleArchiveIDsValue, pluginARATransformFlagsValue,
                                 pluginVSTNumMidiInputsValue, pluginVSTNumMidiOutputsValue, pluginLV2URIValue;

    //==============================================================================
    std::unique_ptr<EnabledModulesList> enabledModulesList;

    AvailableModulesList exporterPathsModulesList;

    //==============================================================================
    void updateDeprecatedProjectSettings();

    //==============================================================================
    bool shouldWriteLegacyPluginFormatSettings = false;
    bool shouldWriteLegacyPluginCharacteristicsSettings = false;

    static Array<Identifier> getLegacyPluginFormatIdentifiers() noexcept;
    static Array<Identifier> getLegacyPluginCharacteristicsIdentifiers() noexcept;

    void writeLegacyPluginFormatSettings();
    void writeLegacyPluginCharacteristicsSettings();

    void coalescePluginFormatValues();
    void coalescePluginCharacteristicsValues();
    void updatePluginCategories();

    //==============================================================================
    File tempDirectory;
    std::pair<Time, String> cachedFileState;

    //==============================================================================
    StringPairArray parsedPreprocessorDefs;

    //==============================================================================
    void initialiseProjectValues();
    void initialiseMainGroup();
    void initialiseAudioPluginValues();

    bool setCppVersionFromOldExporterSettings();

    void createAudioPluginPropertyEditors (PropertyListBuilder& props);

    //==============================================================================
    void updateTitleDependencies();
    void updateVersionDependencies();
    void updateCompanyNameDependencies();
    void updateProjectSettings();
    void updateWebsiteDependencies();
    ValueTree getConfigurations() const;
    ValueTree getConfigNode();

    void updateOldStyleConfigList();
    void moveOldPropertyFromProjectToAllExporters (Identifier name);
    void removeDefunctExporters();
    void updateOldModulePaths();

    //==============================================================================
    void changeListenerCallback (ChangeBroadcaster*) override;
    void availableModulesChanged (AvailableModulesList*) override;

    void updateHIGGSPathWarning();

    void updateModuleWarnings();
    void updateExporterWarnings();
    void updateCppStandardWarning (bool showWarning);
    void updateMissingModuleDependenciesWarning (bool showWarning);
    void updateOldMythosStudioWarning (bool showWarning);
    void updateModuleNotFoundWarning (bool showWarning);
    void updateCodeWarning (Identifier identifier, String value);
    void updatePaceFusionWarnings();

    ValueTree projectMessages { ProjectMessages::Ids::projectMessages, {},
                                { { ProjectMessages::Ids::notification, {} }, { ProjectMessages::Ids::warning, {} } } };
    std::map<Identifier, std::vector<ProjectMessages::MessageAction>> messageActions;

    ProjectFileModificationPoller fileModificationPoller { *this };

    std::unique_ptr<FileChooser> chooser;
    std::unique_ptr<ProjectSaver> saver;

    std::optional<MessageBoxOptions> exporterRemovalMessageBoxOptions;
    ErasedScopeGuard messageBoxQueueListenerScope;
    ScopedMessageBox messageBox;

    //==============================================================================
    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Project)
    HIGGS_DECLARE_WEAK_REFERENCEABLE (Project)
};
