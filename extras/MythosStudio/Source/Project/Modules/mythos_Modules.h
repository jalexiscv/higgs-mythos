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

#include "../mythos_Project.h"

class ProjectExporter;
class ProjectSaver;

//==============================================================================
class LibraryModule
{
public:
    LibraryModule (const ModuleDescription&);

    bool isValid() const                    { return moduleDescription.isValid(); }
    String getID() const                    { return moduleDescription.getID(); }
    String getVendor() const                { return moduleDescription.getVendor(); }
    String getVersion() const               { return moduleDescription.getVersion(); }
    String getName() const                  { return moduleDescription.getName(); }
    String getDescription() const           { return moduleDescription.getDescription(); }
    String getLicense() const               { return moduleDescription.getLicense(); }
    String getMinimumCppStandard() const    { return moduleDescription.getMinimumCppStandard(); }

    File getFolder() const                  { return moduleDescription.getFolder(); }

    String getWebviewInteropLibraryVersion() const;

    void writeIncludes (ProjectSaver&, OutputStream&);
    void addSettingsForModuleToExporter (ProjectExporter&, ProjectSaver&) const;
    void getConfigFlags (Project&, OwnedArray<Project::ConfigFlag>& flags) const;
    void findBrowseableFiles (const File& localModuleFolder, Array<File>& files) const;

    struct CompileUnit
    {
        File file;
        bool isCompiledForObjC = false, isCompiledForNonObjC = false;

        bool isNeededForExporter (ProjectExporter&) const;
        String getFilenameForProxyFile() const;
    };

    Array<CompileUnit> getAllCompileUnits (build_tools::ProjectType::Target::Type forTarget =
                                               build_tools::ProjectType::Target::unspecified) const;
    void findAndAddCompiledUnits (ProjectExporter&, ProjectSaver*, Array<File>& result,
                                  build_tools::ProjectType::Target::Type forTarget =
                                      build_tools::ProjectType::Target::unspecified) const;

    ModuleDescription moduleDescription;

private:
    void addSearchPathsToExporter (ProjectExporter&) const;
    void addDefinesToExporter (ProjectExporter&) const;
    void addCompileUnitsToExporter (ProjectExporter&, ProjectSaver&) const;
    void addLibsToExporter (ProjectExporter&) const;

    void addBrowseableCode (ProjectExporter&, const Array<File>& compiled, const File& localModuleFolder) const;

    mutable Array<File> sourceFiles;
    OwnedArray<Project::ConfigFlag> configFlags;
};

//==============================================================================
class EnabledModulesList
{
public:
    EnabledModulesList (Project&, const ValueTree&);

    //==============================================================================
    ValueTree getState() const              { return state; }

    StringArray getAllModules() const;
    void createRequiredModules (OwnedArray<LibraryModule>& modules);
    void sortAlphabetically();

    File getDefaultModulesFolder() const;

    int getNumModules() const               { return state.getNumChildren(); }
    String getModuleID (int index) const    { return state.getChild (index) [Ids::ID].toString(); }

    ModuleDescription getModuleInfo (const String& moduleID) const;

    bool isModuleEnabled (const String& moduleID) const;

    StringArray getExtraDependenciesNeeded (const String& moduleID) const;
    bool tryToFixMissingDependencies (const String& moduleID);

    bool doesModuleHaveHigherCppStandardThanProject (const String& moduleID) const;

    bool shouldUseGlobalPath (const String& moduleID) const;
    Value shouldUseGlobalPathValue (const String& moduleID) const;

    bool shouldShowAllModuleFilesInProject (const String& moduleID) const;
    Value shouldShowAllModuleFilesInProjectValue (const String& moduleID) const;

    bool shouldCopyModuleFilesLocally (const String& moduleID) const;
    Value shouldCopyModuleFilesLocallyValue (const String& moduleID) const;

    bool areMostModulesUsingGlobalPath() const;
    bool areMostModulesCopiedLocally() const;

    StringArray getModulesWithHigherCppStandardThanProject() const;
    StringArray getModulesWithMissingDependencies() const;

    String getHighestModuleCppStandard() const;

    //==============================================================================
    void addModule (const File& moduleManifestFile, bool copyLocally, bool useGlobalPath);
    void addModuleInteractive (const String& moduleID);
    void addModuleFromUserSelectedFile();
    void addModuleOfferingToCopy (const File&, bool isFromUserSpecifiedFolder);

    void removeModule (String moduleID);

private:
    UndoManager* getUndoManager() const     { return project.getUndoManagerFor (state); }

    Project& project;

    CriticalSection stateLock;
    ValueTree state;

    std::unique_ptr<FileChooser> chooser;
    ScopedMessageBox messageBox;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EnabledModulesList)
};
