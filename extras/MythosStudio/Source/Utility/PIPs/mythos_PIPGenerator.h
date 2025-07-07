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

#include "../Helpers/mythos_MiscUtilities.h"
#include "../../Project/Modules/mythos_AvailableModulesList.h"

//==============================================================================
class PIPGenerator
{
public:
    PIPGenerator (const File& pipFile, const File& outputDirectory = {},
                  const File& pathToHIGGSModules = {}, const File& pathToUserModules = {});

    //==============================================================================
    bool hasValidPIP() const noexcept                   { return ! metadata[Ids::name].toString().isEmpty(); }
    File getMythosFile() const noexcept                  { return outputDirectory.getChildFile (metadata[Ids::name].toString() + ".mythos"); }
    File getPIPFile() const noexcept                    { return useLocalCopy ? outputDirectory.getChildFile ("Source").getChildFile (pipFile.getFileName()) : pipFile; }

    String getMainClassName() const noexcept            { return metadata[Ids::mainClass]; }

    File getOutputDirectory() const noexcept            { return outputDirectory; }

    //==============================================================================
    Result createMythosFile();
    Result createMainCpp();

private:
    //==============================================================================
    void addFileToTree (ValueTree& groupTree, const String& name, bool compile, const String& path);
    void createFiles (ValueTree& mythosTree);
    String getDocumentControllerClass() const;

    ValueTree createModulePathChild (const String& moduleID);
    ValueTree createBuildConfigChild (bool isDebug);
    ValueTree createExporterChild (const Identifier& exporterIdentifier);
    ValueTree createModuleChild (const String& moduleID);

    void addExporters (ValueTree& mythosTree);
    void addModules (ValueTree& mythosTree);

    Result setProjectSettings (ValueTree& mythosTree);

    void setModuleFlags (ValueTree& mythosTree);

    String getMainFileTextForType();

    //==============================================================================
    Array<File> replaceRelativeIncludesAndGetFilesToMove();
    bool copyRelativeFileToLocalSourceDirectory (const File&) const noexcept;

    StringArray getExtraPluginFormatsToBuild() const;

    String getPathForModule (const String&) const;
    File getExamplesDirectory() const;

    //==============================================================================
    File pipFile, outputDirectory, higgsModulesPath, userModulesPath;
    std::unique_ptr<AvailableModulesList> availableUserModules;
    var metadata;
    bool isTemp = false, useLocalCopy = false;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PIPGenerator)
};
