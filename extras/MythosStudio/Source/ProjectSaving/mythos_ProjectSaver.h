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

#include "../Application/mythos_Headers.h"
#include "mythos_ResourceFile.h"
#include "../Project/Modules/mythos_Modules.h"
#include "mythos_ProjectExporter.h"

//==============================================================================
class ProjectSaver
{
public:
    ProjectSaver (Project& projectToSave);

    void save (Async async, ProjectExporter* exporterToSave, std::function<void (Result)> onCompletion);
    Result saveResourcesOnly();
    void saveBasicProjectItems (const OwnedArray<LibraryModule>& modules, const String& appConfigUserContent);

    Project& getProject()  { return project; }

    Project::Item addFileToGeneratedGroup (const File& file);
    bool copyFolder (const File& source, const File& dest);

    static String getHiggsCodeGroupName()  { return "HIGGS Library Code"; }

private:
    //==============================================================================
    struct SaveThreadWithProgressWindow final : public ThreadWithProgressWindow
    {
    public:
        SaveThreadWithProgressWindow (ProjectSaver& ps,
                                      ProjectExporter* exporterToSave,
                                      std::function<void (Result)> onCompletionIn)
            : ThreadWithProgressWindow ("Saving...", true, false),
              saver (ps),
              specifiedExporterToSave (exporterToSave),
              onCompletion (std::move (onCompletionIn))
        {
            jassert (onCompletion != nullptr);
        }

        void run() override
        {
            setProgress (-1);
            const auto result = saver.saveProject (specifiedExporterToSave);
            const auto callback = onCompletion;

            MessageManager::callAsync ([callback, result] { callback (result); });
        }

    private:
        ProjectSaver& saver;
        ProjectExporter* specifiedExporterToSave;
        std::function<void (Result)> onCompletion;

        HIGGS_DECLARE_NON_COPYABLE (SaveThreadWithProgressWindow)
    };

    //==============================================================================
    Project::Item saveGeneratedFile (const String& filePath, const MemoryOutputStream& newData);
    bool replaceFileIfDifferent (const File& f, const MemoryOutputStream& newData);
    bool deleteUnwantedFilesIn (const File& parent);

    void addError (const String& message);

    File getAppConfigFile() const;
    File getPluginDefinesFile() const;

    String loadUserContentFromAppConfig() const;
    String getAudioPluginDefines() const;
    OwnedArray<LibraryModule> getModules();

    Result saveProject (ProjectExporter* specifiedExporterToSave);
    void saveProjectAsync (ProjectExporter* exporterToSave, std::function<void (Result)> onCompletion);

    template <typename WriterCallback>
    void writeOrRemoveGeneratedFile (const String& name, WriterCallback&& writerCallback);

    void writePluginDefines (MemoryOutputStream& outStream) const;
    void writePluginDefines();
    void writeAppConfigFile (const OwnedArray<LibraryModule>& modules, const String& userContent);
    void writeLV2Defines (MemoryOutputStream&);

    void writeProjectFile();
    void writeAppConfig (MemoryOutputStream& outStream, const OwnedArray<LibraryModule>& modules, const String& userContent);
    void writeAppHeader (MemoryOutputStream& outStream, const OwnedArray<LibraryModule>& modules);
    void writeAppHeader (const OwnedArray<LibraryModule>& modules);
    void writeModuleCppWrappers (const OwnedArray<LibraryModule>& modules);
    void writeBinaryDataFiles();
    void writeReadmeFile();
    void writePluginCharacteristicsFile();
    void writeUnityScriptFile();
    void writeProjects (const OwnedArray<LibraryModule>&, ProjectExporter*);
    void writeLV2DefinesFile();
    void runPostExportScript();
    void saveExporter (ProjectExporter& exporter, const OwnedArray<LibraryModule>& modules);

    //==============================================================================
    Project& project;

    File generatedCodeFolder;
    Project::Item generatedFilesGroup;
    SortedSet<File> filesCreated;
    String projectLineFeed;

    CriticalSection errorLock;
    StringArray errors;

    std::unique_ptr<SaveThreadWithProgressWindow> saveThread;

    bool hasBinaryData = false;

    //==============================================================================
    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ProjectSaver)
    HIGGS_DECLARE_WEAK_REFERENCEABLE (ProjectSaver)
};
