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

#include "../Application/mythos_Headers.h"
#include "mythos_StoredSettings.h"
#include "../Application/mythos_Application.h"

//==============================================================================
StoredSettings& getAppSettings()
{
    return *MythosStudioApplication::getApp().settings;
}

PropertiesFile& getGlobalProperties()
{
    return getAppSettings().getGlobalProperties();
}

//==============================================================================
StoredSettings::StoredSettings()
    : appearance (true),
      projectDefaults ("PROJECT_DEFAULT_SETTINGS"),
      fallbackPaths ("FALLBACK_PATHS")
{
    updateOldProjectSettingsFiles();

    reload();
    changed (true);
    flush();

    checkHIGGSPaths();

    projectDefaults.addListener (this);
    fallbackPaths.addListener (this);
}

StoredSettings::~StoredSettings()
{
    projectDefaults.removeListener (this);
    fallbackPaths.removeListener (this);
    flush();
}

PropertiesFile& StoredSettings::getGlobalProperties()
{
    return *propertyFiles.getUnchecked (0);
}

static PropertiesFile* createPropsFile (const String& filename, bool isProjectSettings)
{
    return new PropertiesFile (MythosStudioApplication::getApp()
                                .getPropertyFileOptionsFor (filename, isProjectSettings));
}

PropertiesFile& StoredSettings::getProjectProperties (const String& projectUID)
{
    const auto filename = String ("MythosStudio_Project_" + projectUID);

    for (auto i = propertyFiles.size(); --i >= 0;)
    {
        auto* const props = propertyFiles.getUnchecked (i);
        if (props->getFile().getFileNameWithoutExtension() == filename)
            return *props;
    }

    auto* p = createPropsFile (filename, true);
    propertyFiles.add (p);
    return *p;
}

void StoredSettings::updateGlobalPreferences()
{
    // update 'invisible' global settings
    updateRecentFiles();
    updateLastWizardFolder();
    updateKeyMappings();
}

void StoredSettings::updateRecentFiles()
{
    getGlobalProperties().setValue ("recentFiles", recentFiles.toString());
}

void StoredSettings::updateLastWizardFolder()
{
    getGlobalProperties().setValue ("lastWizardFolder", lastWizardFolder.getFullPathName());
}

void StoredSettings::updateKeyMappings()
{
    getGlobalProperties().removeValue ("keyMappings");

    if (auto* commandManager = MythosStudioApplication::getApp().commandManager.get())
    {
        const std::unique_ptr<XmlElement> keys (commandManager->getKeyMappings()->createXml (true));

        if (keys != nullptr)
            getGlobalProperties().setValue ("keyMappings", keys.get());
    }
}

void StoredSettings::flush()
{
    updateGlobalPreferences();
    saveSwatchColours();

    for (auto i = propertyFiles.size(); --i >= 0;)
        propertyFiles.getUnchecked (i)->saveIfNeeded();
}

void StoredSettings::reload()
{
    propertyFiles.clear();
    propertyFiles.add (createPropsFile ("MythosStudio", false));

    if (auto projectDefaultsXml = propertyFiles.getFirst()->getXmlValue ("PROJECT_DEFAULT_SETTINGS"))
        projectDefaults = ValueTree::fromXml (*projectDefaultsXml);

    if (auto fallbackPathsXml = propertyFiles.getFirst()->getXmlValue ("FALLBACK_PATHS"))
        fallbackPaths = ValueTree::fromXml (*fallbackPathsXml);

    // recent files...
    recentFiles.restoreFromString (getGlobalProperties().getValue ("recentFiles"));
    recentFiles.removeNonExistentFiles();

    lastWizardFolder = getGlobalProperties().getValue ("lastWizardFolder");

    loadSwatchColours();
}

Array<File> StoredSettings::getLastProjects()
{
    StringArray s;
    s.addTokens (getGlobalProperties().getValue ("lastProjects"), "|", "");

    Array<File> f;
    for (int i = 0; i < s.size(); ++i)
        f.add (File (s[i]));

    return f;
}

void StoredSettings::setLastProjects (const Array<File>& files)
{
    StringArray s;
    for (int i = 0; i < files.size(); ++i)
        s.add (files.getReference (i).getFullPathName());

    getGlobalProperties().setValue ("lastProjects", s.joinIntoString ("|"));
}

void StoredSettings::updateOldProjectSettingsFiles()
{
    // Global properties file hasn't been created yet so create a dummy file
    auto mythosstudioSettingsDirectory = MythosStudioApplication::getApp().getPropertyFileOptionsFor ("Dummy", false)
                                                                  .getDefaultFile().getParentDirectory();

    auto newProjectSettingsDir = mythosstudioSettingsDirectory.getChildFile ("ProjectSettings");
    newProjectSettingsDir.createDirectory();

    for (const auto& iter : RangedDirectoryIterator (mythosstudioSettingsDirectory, false, "*.settings"))
    {
        auto f = iter.getFile();
        auto oldFileName = f.getFileName();

        if (oldFileName.contains ("Intromythos"))
        {
            auto newFileName = oldFileName.replace ("Intromythos", "MythosStudio");

            if (oldFileName.contains ("_Project"))
            {
                f.moveFileTo (f.getSiblingFile (newProjectSettingsDir.getFileName()).getChildFile (newFileName));
            }
            else
            {
                auto newFile = f.getSiblingFile (newFileName);

                // don't overwrite newer settings file
                if (! newFile.existsAsFile())
                    f.moveFileTo (f.getSiblingFile (newFileName));
            }
        }
    }
}

//==============================================================================
void StoredSettings::loadSwatchColours()
{
    swatchColours.clear();

    #define COL(col)  Colours::col,

    const Colour colours[] =
    {
        #include "../Utility/Helpers/mythos_Colours.h"
        Colours::transparentBlack
    };

    #undef COL

    const auto numSwatchColours = 24;
    auto& props = getGlobalProperties();

    for (auto i = 0; i < numSwatchColours; ++i)
        swatchColours.add (Colour::fromString (props.getValue ("swatchColour" + String (i),
                                                               colours [2 + i].toString())));
}

void StoredSettings::saveSwatchColours()
{
    auto& props = getGlobalProperties();

    for (auto i = 0; i < swatchColours.size(); ++i)
        props.setValue ("swatchColour" + String (i), swatchColours.getReference (i).toString());
}

StoredSettings::ColourSelectorWithSwatches::ColourSelectorWithSwatches() {}
StoredSettings::ColourSelectorWithSwatches::~ColourSelectorWithSwatches() {}

int StoredSettings::ColourSelectorWithSwatches::getNumSwatches() const
{
    return getAppSettings().swatchColours.size();
}

Colour StoredSettings::ColourSelectorWithSwatches::getSwatchColour (int index) const
{
    return getAppSettings().swatchColours [index];
}

void StoredSettings::ColourSelectorWithSwatches::setSwatchColour (int index, const Colour& newColour)
{
    getAppSettings().swatchColours.set (index, newColour);
}

//==============================================================================
void StoredSettings::changed (bool isProjectDefaults)
{
    std::unique_ptr<XmlElement> data (isProjectDefaults ? projectDefaults.createXml()
                                                        : fallbackPaths.createXml());

    propertyFiles.getUnchecked (0)->setValue (isProjectDefaults ? "PROJECT_DEFAULT_SETTINGS" : "FALLBACK_PATHS",
                                              data.get());
}

//==============================================================================
static bool doesSDKPathContainFile (const File& relativeTo, const String& path, const String& fileToCheckFor) noexcept
{
    auto actualPath = path.replace ("${user.home}", File::getSpecialLocation (File::userHomeDirectory).getFullPathName());
    return relativeTo.getChildFile (actualPath + "/" + fileToCheckFor).exists();
}

static bool isGlobalPathValid (const File& relativeTo, const Identifier& key, const String& path)
{
    String fileToCheckFor;

    if (key == Ids::vstLegacyPath)
    {
        fileToCheckFor = "pluginterfaces/vst2.x/aeffect.h";
    }
    else if (key == Ids::aaxPath)
    {
        fileToCheckFor = "Interfaces/AAX_Exports.cpp";
    }
    else if (key == Ids::araPath)
    {
        fileToCheckFor = "ARA_API/ARAInterface.h";
    }
    else if (key == Ids::androidSDKPath)
    {
       #if HIGGS_WINDOWS
        fileToCheckFor = "platform-tools/adb.exe";
       #else
        fileToCheckFor = "platform-tools/adb";
       #endif
    }
    else if (key == Ids::defaultHiggsModulePath)
    {
        fileToCheckFor = "higgs_core";
    }
    else if (key == Ids::defaultUserModulePath)
    {
        fileToCheckFor = {};
    }
    else if (key == Ids::androidStudioExePath)
    {
       #if HIGGS_MAC
        fileToCheckFor = "Android Studio.app";
       #elif HIGGS_WINDOWS
        fileToCheckFor = "studio64.exe";
       #endif
    }
    else if (key == Ids::higgsPath)
    {
        fileToCheckFor = "CHANGE_LIST.md";
    }
    else
    {
        // didn't recognise the key provided!
        jassertfalse;
        return false;
    }

    return doesSDKPathContainFile (relativeTo, path, fileToCheckFor);
}

void StoredSettings::checkHIGGSPaths()
{
    auto moduleFolder = getStoredPath (Ids::defaultHiggsModulePath, TargetOS::getThisOS()).get().toString();
    auto higgsFolder   = getStoredPath (Ids::higgsPath, TargetOS::getThisOS()).get().toString();

    auto validModuleFolder = isGlobalPathValid ({}, Ids::defaultHiggsModulePath, moduleFolder);
    auto validHiggsFolder   = isGlobalPathValid ({}, Ids::higgsPath, higgsFolder);

    if (validModuleFolder && ! validHiggsFolder)
        projectDefaults.getPropertyAsValue (Ids::higgsPath, nullptr) = File (moduleFolder).getParentDirectory().getFullPathName();
    else if (! validModuleFolder && validHiggsFolder)
        projectDefaults.getPropertyAsValue (Ids::defaultHiggsModulePath, nullptr) = File (higgsFolder).getChildFile ("modules").getFullPathName();
}

bool StoredSettings::isHIGGSPathIncorrect()
{
    return ! isGlobalPathValid ({}, Ids::higgsPath, getStoredPath (Ids::higgsPath, TargetOS::getThisOS()).get().toString());
}

static String getFallbackPathForOS (const Identifier& key, DependencyPathOS os)
{
    if (key == Ids::higgsPath)
        return (os == TargetOS::windows ? "C:\\HIGGS" : "~/HIGGS");

    if (key == Ids::defaultHiggsModulePath)
        return (os == TargetOS::windows ? "C:\\HIGGS\\modules" : "~/HIGGS/modules");

    if (key == Ids::defaultUserModulePath)
        return (os == TargetOS::windows ? "C:\\modules" : "~/modules");

    if (key == Ids::vstLegacyPath)
        return {};

    if (key == Ids::aaxPath)
        return {}; // Empty means "use internal SDK"

    if (key == Ids::araPath)
    {
        if (os == TargetOS::windows)  return "C:\\SDKs\\ARA_SDK";
        if (os == TargetOS::osx)      return "~/SDKs/ARA_SDK";
        return {};
    }

    if (key == Ids::androidSDKPath)
    {
        if (os == TargetOS::windows)  return "${user.home}\\AppData\\Local\\Android\\Sdk";
        if (os == TargetOS::osx)      return "${user.home}/Library/Android/sdk";
        if (os == TargetOS::linux)    return "${user.home}/Android/Sdk";

        jassertfalse;
        return {};
    }

    if (key == Ids::androidStudioExePath)
    {
        if (os == TargetOS::windows)
        {
           #if HIGGS_WINDOWS
            auto path = WindowsRegistry::getValue ("HKEY_LOCAL_MACHINE\\SOFTWARE\\Android Studio\\Path", {}, {});

            if (! path.isEmpty())
                return path.unquoted() + "\\bin\\studio64.exe";
           #endif

            return "C:\\Program Files\\Android\\Android Studio\\bin\\studio64.exe";
        }

        if (os == TargetOS::osx)
            return "/Applications/Android Studio.app";

        return {}; // no Android Studio on this OS!
    }

    // unknown key!
    jassertfalse;
    return {};
}

static Identifier identifierForOS (DependencyPathOS os) noexcept
{
    if      (os == TargetOS::osx)     return Ids::osxFallback;
    else if (os == TargetOS::windows) return Ids::windowsFallback;
    else if (os == TargetOS::linux)   return Ids::linuxFallback;

    jassertfalse;
    return {};
}

ValueTreePropertyWithDefault StoredSettings::getStoredPath (const Identifier& key, DependencyPathOS os)
{
    auto tree = (os == TargetOS::getThisOS() ? projectDefaults
                                             : fallbackPaths.getOrCreateChildWithName (identifierForOS (os), nullptr));

    return { tree, key, nullptr, getFallbackPathForOS (key, os) };
}

void StoredSettings::addProjectDefaultsListener (ValueTree::Listener& l)     { projectDefaults.addListener (&l); }
void StoredSettings::removeProjectDefaultsListener (ValueTree::Listener& l)  { projectDefaults.removeListener (&l); }

void StoredSettings::addFallbackPathsListener (ValueTree::Listener& l)       { fallbackPaths.addListener (&l); }
void StoredSettings::removeFallbackPathsListener (ValueTree::Listener& l)    { fallbackPaths.removeListener (&l); }
