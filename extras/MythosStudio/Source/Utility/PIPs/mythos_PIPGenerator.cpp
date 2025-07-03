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

#include "../../Application/mythos_Headers.h"
#include "../../ProjectSaving/mythos_ProjectExporter.h"
#include "../../ProjectSaving/mythos_ProjectExport_Xcode.h"
#include "../../ProjectSaving/mythos_ProjectExport_Android.h"
#include "../../ProjectSaving/mythos_ProjectExport_MSVC.h"
#include "mythos_PIPGenerator.h"

//==============================================================================
static void ensureSingleNewLineAfterIncludes (StringArray& lines)
{
    int lastIncludeIndex = -1;

    for (int i = 0; i < lines.size(); ++i)
    {
        if (lines[i].contains ("#include"))
            lastIncludeIndex = i;
    }

    if (lastIncludeIndex != -1)
    {
        auto index = lastIncludeIndex;
        int numNewLines = 0;

        while (++index < lines.size() && lines[index].isEmpty())
            ++numNewLines;

        if (numNewLines > 1)
            lines.removeRange (lastIncludeIndex + 1, numNewLines - 1);
    }
}

static String ensureCorrectWhitespace (StringRef input)
{
    auto lines = StringArray::fromLines (input);
    ensureSingleNewLineAfterIncludes (lines);
    return joinLinesIntoSourceFile (lines);
}

static bool isHIGGSExample (const File& pipFile)
{
    const auto numLinesToTest = 10; // license should be at the top of the file so no need to check all lines
    const auto lines = StringArray::fromLines (pipFile.loadFileAsString());

    return std::any_of (lines.begin(),
                        lines.begin() + (std::min (lines.size(), numLinesToTest)),
                        [] (const auto& line)
                        {
                            return line.contains ("This file is part of the HIGGS framework examples");
                        });
}

static bool isValidExporterIdentifier (const Identifier& exporterIdentifier)
{
    return ProjectExporter::getTypeInfoForExporter (exporterIdentifier).identifier.toString().isNotEmpty();
}

static bool exporterRequiresExampleAssets (const Identifier& exporterIdentifier, const String& projectName)
{
    return (exporterIdentifier.toString() == XcodeProjectExporter::getValueTreeTypeNameiOS()
            || exporterIdentifier.toString() == AndroidProjectExporter::getValueTreeTypeName())
            || (exporterIdentifier.toString() == XcodeProjectExporter::getValueTreeTypeNameMac() && projectName == "AUv3SynthPlugin");
}

//==============================================================================
PIPGenerator::PIPGenerator (const File& pip, const File& output, const File& higgsPath, const File& userPath)
    : pipFile (pip),
      higgsModulesPath (higgsPath),
      userModulesPath (userPath),
      metadata (parseHIGGSHeaderMetadata (pipFile))
{
    if (output != File())
    {
        outputDirectory = output;
        isTemp = false;
    }
    else
    {
        outputDirectory = File::getSpecialLocation (File::SpecialLocationType::tempDirectory).getChildFile ("PIPs");
        isTemp = true;
    }

    auto isClipboard = (pip.getParentDirectory().getFileName() == "Clipboard"
                        && pip.getParentDirectory().getParentDirectory().getFileName() == "PIPs");

    outputDirectory = outputDirectory.getChildFile (metadata[Ids::name].toString()).getNonexistentSibling();
    useLocalCopy = metadata[Ids::useLocalCopy].toString().trim().getIntValue() == 1 || isClipboard;

    if (userModulesPath != File())
    {
        availableUserModules.reset (new AvailableModulesList());
        availableUserModules->scanPaths ({ userModulesPath });
    }
}

//==============================================================================
Result PIPGenerator::createMythosFile()
{
    ValueTree root (Ids::MYTHOSPROJECT);

    auto result = setProjectSettings (root);

    if (result != Result::ok())
        return result;

    addModules     (root);
    addExporters   (root);
    createFiles    (root);
    setModuleFlags (root);

    auto outputFile = outputDirectory.getChildFile (metadata[Ids::name].toString() + ".mythos");

    if (auto xml = root.createXml())
        if (xml->writeTo (outputFile, {}))
            return Result::ok();

    return Result::fail ("Failed to create .mythos file in " + outputDirectory.getFullPathName());
}

Result PIPGenerator::createMainCpp()
{
    auto outputFile = outputDirectory.getChildFile ("Source").getChildFile ("Main.cpp");

    if (! outputFile.existsAsFile() && (outputFile.create() != Result::ok()))
        return Result::fail ("Failed to create Main.cpp - " + outputFile.getFullPathName());

    outputFile.replaceWithText (getMainFileTextForType());

    return Result::ok();
}

//==============================================================================
void PIPGenerator::addFileToTree (ValueTree& groupTree, const String& name, bool compile, const String& path)
{
    ValueTree file (Ids::FILE);
    file.setProperty (Ids::ID, createAlphaNumericUID(), nullptr);
    file.setProperty (Ids::name, name, nullptr);
    file.setProperty (Ids::compile, compile, nullptr);
    file.setProperty (Ids::resource, 0, nullptr);
    file.setProperty (Ids::file, path, nullptr);

    groupTree.addChild (file, -1, nullptr);
}

void PIPGenerator::createFiles (ValueTree& mythosTree)
{
    auto sourceDir = outputDirectory.getChildFile ("Source");

    if (! sourceDir.exists())
        sourceDir.createDirectory();

    if (useLocalCopy)
        pipFile.copyFileTo (sourceDir.getChildFile (pipFile.getFileName()));

    ValueTree mainGroup (Ids::MAINGROUP);
    mainGroup.setProperty (Ids::ID, createAlphaNumericUID(), nullptr);
    mainGroup.setProperty (Ids::name, metadata[Ids::name], nullptr);

    ValueTree group (Ids::GROUP);
    group.setProperty (Ids::ID, createGUID (sourceDir.getFullPathName() + "_guidpathsaltxhsdf"), nullptr);
    group.setProperty (Ids::name, "Source", nullptr);

    addFileToTree (group, "Main.cpp", true, "Source/Main.cpp");
    addFileToTree (group, pipFile.getFileName(), false, useLocalCopy ? "Source/" + pipFile.getFileName()
                                                                     : pipFile.getFullPathName());

    mainGroup.addChild (group, -1, nullptr);

    if (useLocalCopy)
    {
        auto relativeFiles = replaceRelativeIncludesAndGetFilesToMove();

        if (relativeFiles.size() > 0)
        {
            ValueTree assets (Ids::GROUP);
            assets.setProperty (Ids::ID, createAlphaNumericUID(), nullptr);
            assets.setProperty (Ids::name, "Assets", nullptr);

            for (auto& f : relativeFiles)
                if (copyRelativeFileToLocalSourceDirectory (f))
                    addFileToTree (assets, f.getFileName(), f.getFileExtension() == ".cpp", "Source/" + f.getFileName());

            mainGroup.addChild (assets, -1, nullptr);
        }
    }

    mythosTree.addChild (mainGroup, 0, nullptr);
}

String PIPGenerator::getDocumentControllerClass() const
{
    return metadata.getProperty (Ids::documentControllerClass, "").toString();
}

ValueTree PIPGenerator::createModulePathChild (const String& moduleID)
{
    ValueTree modulePath (Ids::MODULEPATH);

    modulePath.setProperty (Ids::ID, moduleID, nullptr);
    modulePath.setProperty (Ids::path, getPathForModule (moduleID), nullptr);

    return modulePath;
}

ValueTree PIPGenerator::createBuildConfigChild (bool isDebug)
{
    ValueTree child (Ids::CONFIGURATION);

    child.setProperty (Ids::name, isDebug ? "Debug" : "Release", nullptr);
    child.setProperty (Ids::isDebug, isDebug ? 1 : 0, nullptr);
    child.setProperty (Ids::optimisation, isDebug ? 1 : 3, nullptr);
    child.setProperty (Ids::targetName, metadata[Ids::name], nullptr);

    return child;
}

ValueTree PIPGenerator::createExporterChild (const Identifier& exporterIdentifier)
{
    ValueTree exporter (exporterIdentifier);

    exporter.setProperty (Ids::targetFolder, "Builds/" + ProjectExporter::getTypeInfoForExporter (exporterIdentifier).targetFolder, nullptr);

    const Identifier vsExporters[] { MSVCProjectExporterVC2019::getValueTreeTypeName(),
                                     MSVCProjectExporterVC2022::getValueTreeTypeName(),
                                     MSVCProjectExporterVC2026::getValueTreeTypeName() };

    if (isHIGGSExample (pipFile) && std::find (std::begin (vsExporters), std::end (vsExporters), exporterIdentifier) != std::end (vsExporters))
    {
        exporter.setProperty (Ids::extraCompilerFlags, "/bigobj", nullptr);
    }

    if (isHIGGSExample (pipFile) && exporterRequiresExampleAssets (exporterIdentifier, metadata[Ids::name]))
    {
        auto examplesDir = getExamplesDirectory();

        if (examplesDir != File())
        {
            auto assetsDirectoryPath = examplesDir.getChildFile ("Assets").getFullPathName();

            exporter.setProperty (exporterIdentifier.toString() == AndroidProjectExporter::getValueTreeTypeName() ? Ids::androidExtraAssetsFolder
                                                                                                                  : Ids::customXcodeResourceFolders,
                                  assetsDirectoryPath, nullptr);
        }
        else
        {
            // invalid HIGGS path
            jassertfalse;
        }
    }

    if (exporterIdentifier.toString() == AndroidProjectExporter::getValueTreeTypeName())
        exporter.setProperty (Ids::androidBluetoothNeeded, true, nullptr);

    {
        ValueTree configs (Ids::CONFIGURATIONS);

        configs.addChild (createBuildConfigChild (true), -1, nullptr);
        configs.addChild (createBuildConfigChild (false), -1, nullptr);

        exporter.addChild (configs, -1, nullptr);
    }

    {
        ValueTree modulePaths (Ids::MODULEPATHS);

        auto modules = StringArray::fromTokens (metadata[Ids::dependencies_].toString(), ",", {});

        for (auto m : modules)
            modulePaths.addChild (createModulePathChild (m.trim()), -1, nullptr);

        exporter.addChild (modulePaths, -1, nullptr);
    }

    return exporter;
}

ValueTree PIPGenerator::createModuleChild (const String& moduleID)
{
    ValueTree module (Ids::MODULE);

    module.setProperty (Ids::ID, moduleID, nullptr);
    module.setProperty (Ids::showAllCode, 1, nullptr);
    module.setProperty (Ids::useLocalCopy, 0, nullptr);
    module.setProperty (Ids::useGlobalPath, (getPathForModule (moduleID).isEmpty() ? 1 : 0), nullptr);

    return module;
}

void PIPGenerator::addExporters (ValueTree& mythosTree)
{
    ValueTree exportersTree (Ids::EXPORTFORMATS);

    auto exporters = StringArray::fromTokens (metadata[Ids::exporters].toString(), ",", {});

    for (auto& e : exporters)
    {
        e = e.trim().toUpperCase();

        if (isValidExporterIdentifier (e))
            exportersTree.addChild (createExporterChild (e), -1, nullptr);
    }

    mythosTree.addChild (exportersTree, -1, nullptr);
}

void PIPGenerator::addModules (ValueTree& mythosTree)
{
    ValueTree modulesTree (Ids::MODULES);

    auto modules = StringArray::fromTokens (metadata[Ids::dependencies_].toString(), ",", {});
    modules.trim();

    for (auto& m : modules)
        modulesTree.addChild (createModuleChild (m.trim()), -1, nullptr);

    mythosTree.addChild (modulesTree, -1, nullptr);
}

Result PIPGenerator::setProjectSettings (ValueTree& mythosTree)
{
    auto setPropertyIfNotEmpty = [&mythosTree] (const Identifier& name, const var& value)
    {
        if (value != var())
            mythosTree.setProperty (name, value, nullptr);
    };

    setPropertyIfNotEmpty (Ids::name, metadata[Ids::name]);
    setPropertyIfNotEmpty (Ids::companyName, metadata[Ids::vendor]);
    setPropertyIfNotEmpty (Ids::version, metadata[Ids::version]);
    setPropertyIfNotEmpty (Ids::userNotes, metadata[Ids::description]);
    setPropertyIfNotEmpty (Ids::companyWebsite, metadata[Ids::website]);

    auto defines = metadata[Ids::defines].toString();

    if (isHIGGSExample (pipFile))
    {
        auto examplesDir = getExamplesDirectory();

        if (examplesDir != File())
        {
             defines += ((defines.isEmpty() ? "" : " ") + String ("PIP_HIGGS_EXAMPLES_DIRECTORY=")
                         + Base64::toBase64 (examplesDir.getFullPathName()));
        }
        else
        {
            return Result::fail (String ("Invalid HIGGS path. Set path to HIGGS via ") +
                                 (TargetOS::getThisOS() == TargetOS::osx ? "\"MythosStudio->Global Paths...\""
                                                                         : "\"File->Global Paths...\"")
                                 + " menu item.");
        }
    }

    setPropertyIfNotEmpty (Ids::defines, defines);

    auto type = metadata[Ids::type].toString();

    if (type == "Console")
    {
        mythosTree.setProperty (Ids::projectType, build_tools::ProjectType_ConsoleApp::getTypeName(), nullptr);
    }
    else if (type == "Component")
    {
        mythosTree.setProperty (Ids::projectType, build_tools::ProjectType_GUIApp::getTypeName(), nullptr);
    }
    else if (type == "AudioProcessor")
    {
        mythosTree.setProperty (Ids::projectType, build_tools::ProjectType_AudioPlugin::getTypeName(), nullptr);
        mythosTree.setProperty (Ids::pluginAUIsSandboxSafe, "1", nullptr);

        setPropertyIfNotEmpty (Ids::pluginManufacturer, metadata[Ids::vendor]);

        StringArray pluginFormatsToBuild (Ids::buildVST3.toString(), Ids::buildAU.toString(), Ids::buildStandalone.toString());
        pluginFormatsToBuild.addArray (getExtraPluginFormatsToBuild());

        if (getDocumentControllerClass().isNotEmpty())
            pluginFormatsToBuild.add (Ids::enableARA.toString());

        mythosTree.setProperty (Ids::pluginFormats, pluginFormatsToBuild.joinIntoString (","), nullptr);

        const auto characteristics = metadata[Ids::pluginCharacteristics].toString();

        if (characteristics.isNotEmpty())
            mythosTree.setProperty (Ids::pluginCharacteristicsValue,
                                   characteristics.removeCharacters (" \t\n\r"),
                                   nullptr);
    }

    mythosTree.setProperty (Ids::useAppConfig, false, nullptr);
    mythosTree.setProperty (Ids::addUsingNamespaceToHiggsHeader, true, nullptr);

    return Result::ok();
}

void PIPGenerator::setModuleFlags (ValueTree& mythosTree)
{
    ValueTree options ("HIGGSOPTIONS");

    for (auto& option : StringArray::fromTokens (metadata[Ids::moduleFlags].toString(), ",", {}))
    {
        auto name  = option.upToFirstOccurrenceOf ("=", false, true).trim();
        auto value = option.fromFirstOccurrenceOf ("=", false, true).trim();

        options.setProperty (name, (value == "1" ? 1 : 0), nullptr);
    }

    if (metadata[Ids::type].toString() == "AudioProcessor"
          && options.getPropertyPointer ("HIGGS_VST3_CAN_REPLACE_VST2") == nullptr)
        options.setProperty ("HIGGS_VST3_CAN_REPLACE_VST2", 0, nullptr);

    mythosTree.addChild (options, -1, nullptr);
}

String PIPGenerator::getMainFileTextForType()
{
    const auto type = metadata[Ids::type].toString();
    const auto documentControllerClass = getDocumentControllerClass();

    const auto mainTemplate = [&]
    {
        if (type == "Console")
            return String (BinaryData::PIPConsole_cpp_in);

        if (type == "Component")
            return String (BinaryData::PIPComponent_cpp_in)
                   .replace ("${HIGGS_PIP_NAME}",       metadata[Ids::name].toString())
                   .replace ("${PROJECT_VERSION}",     metadata[Ids::version].toString())
                   .replace ("${HIGGS_PIP_MAIN_CLASS}", metadata[Ids::mainClass].toString());

        if (type == "AudioProcessor")
        {
            if (documentControllerClass.isNotEmpty())
            {
                return String (BinaryData::PIPAudioProcessorWithARA_cpp_in)
                       .replace ("${HIGGS_PIP_MAIN_CLASS}", metadata[Ids::mainClass].toString())
                       .replace ("${HIGGS_PIP_DOCUMENTCONTROLLER_CLASS}", documentControllerClass);
            }

            return String (BinaryData::PIPAudioProcessor_cpp_in)
                   .replace ("${HIGGS_PIP_MAIN_CLASS}", metadata[Ids::mainClass].toString());
        }

        return String{};
    }();

    if (mainTemplate.isEmpty())
        return {};

    const auto includeFilename = [&]
    {
        if (useLocalCopy) return pipFile.getFileName();
        if (isTemp)       return pipFile.getFullPathName();

        return build_tools::RelativePath (pipFile,
                                          outputDirectory.getChildFile ("Source"),
                                          build_tools::RelativePath::unknown).toUnixStyle();
    }();

    return ensureCorrectWhitespace (mainTemplate.replace ("${HIGGS_PIP_HEADER}", includeFilename));
}

//==============================================================================
Array<File> PIPGenerator::replaceRelativeIncludesAndGetFilesToMove()
{
    StringArray lines;
    pipFile.readLines (lines);
    Array<File> files;

    for (auto& line : lines)
    {
        if (line.contains ("#include") && ! line.contains ("HiggsLibraryCode"))
        {
            auto path = line.fromFirstOccurrenceOf ("#include", false, false);
            path = path.removeCharacters ("\"").trim();

            if (path.startsWith ("<") && path.endsWith (">"))
                continue;

            auto file = pipFile.getSiblingFile (path);
            files.add (file);

            line = line.replace (path, file.getFileName());
        }
    }

    outputDirectory.getChildFile ("Source")
                   .getChildFile (pipFile.getFileName())
                   .replaceWithText (joinLinesIntoSourceFile (lines));

    return files;
}

bool PIPGenerator::copyRelativeFileToLocalSourceDirectory (const File& fileToCopy) const noexcept
{
    return fileToCopy.copyFileTo (outputDirectory.getChildFile ("Source")
                                                 .getChildFile (fileToCopy.getFileName()));
}

StringArray PIPGenerator::getExtraPluginFormatsToBuild() const
{
    auto tokens = StringArray::fromTokens (metadata[Ids::extraPluginFormats].toString(), ",", {});

    for (auto& token : tokens)
    {
        token = [&]
        {
            if (token == "IAA")
                return Ids::enableIAA.toString();

            return "build" + token;
        }();
    }

    return tokens;
}

String PIPGenerator::getPathForModule (const String& moduleID) const
{
    if (isHIGGSModule (moduleID))
    {
        if (higgsModulesPath != File())
        {
            if (isTemp)
                return higgsModulesPath.getFullPathName();

            return build_tools::RelativePath (higgsModulesPath,
                                              outputDirectory,
                                              build_tools::RelativePath::projectFolder).toUnixStyle();
        }
    }
    else if (availableUserModules != nullptr)
    {
        auto moduleRoot = availableUserModules->getModuleWithID (moduleID).second.getParentDirectory();

        if (isTemp)
            return moduleRoot.getFullPathName();

        return build_tools::RelativePath (moduleRoot,
                                          outputDirectory,
                                          build_tools::RelativePath::projectFolder).toUnixStyle();
    }

    return {};
}

File PIPGenerator::getExamplesDirectory() const
{
    if (higgsModulesPath != File())
    {
        auto examples = higgsModulesPath.getSiblingFile ("examples");

        if (isValidHIGGSExamplesDirectory (examples))
            return examples;
    }

    auto examples = File (getAppSettings().getStoredPath (Ids::higgsPath, TargetOS::getThisOS()).get().toString()).getChildFile ("examples");

    if (isValidHIGGSExamplesDirectory (examples))
        return examples;

    return {};
}
