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

#include "../../Utility/Helpers/mythos_MiscUtilities.h"

//==============================================================================
namespace NewProjectTemplates
{
    enum class ProjectCategory
    {
        application,
        plugin,
        library
    };

    inline String getProjectCategoryString (ProjectCategory category)
    {
        if (category == ProjectCategory::application)  return "Application";
        if (category == ProjectCategory::plugin)       return "Plug-In";
        if (category == ProjectCategory::library)      return "Library";

        jassertfalse;
        return "Unknown";
    }

    enum class FileCreationOptions
    {
        noFiles,
        main,
        header,
        headerAndCpp,
        processorAndEditor,
        araPluginFiles
    };

    using FilenameAndContent = std::pair<String, String>;
    using OptionAndFilenameAndContent = std::pair<FileCreationOptions, std::vector<FilenameAndContent>>;
    using OptionsAndFiles = std::vector<OptionAndFilenameAndContent>;

    struct ProjectTemplate
    {
        ProjectCategory category;
        String displayName, description, projectTypeString;

        const char* icon;
        StringArray requiredModules;
        OptionsAndFiles fileOptionsAndFiles;
        FileCreationOptions defaultFileOption;

        std::vector<FilenameAndContent> getFilesForOption (FileCreationOptions option) const
        {
            auto iter = std::find_if (fileOptionsAndFiles.begin(), fileOptionsAndFiles.end(),
                                      [option] (const OptionAndFilenameAndContent& opt) { return opt.first == option; });

            if (iter != fileOptionsAndFiles.end())
                return iter->second;

            return {};
        }
    };

    inline bool isApplication (const ProjectTemplate& t) noexcept  { return t.category == ProjectCategory::application; }
    inline bool isPlugin (const ProjectTemplate& t) noexcept       { return t.category == ProjectCategory::plugin; }
    inline bool isLibrary (const ProjectTemplate& t) noexcept      { return t.category == ProjectCategory::library; }

    //==============================================================================
    inline var getVarForFileOption (FileCreationOptions opt)
    {
        if (opt == FileCreationOptions::noFiles)             return "none";
        if (opt == FileCreationOptions::main)                return "main";
        if (opt == FileCreationOptions::header)              return "header";
        if (opt == FileCreationOptions::headerAndCpp)        return "headercpp";
        if (opt == FileCreationOptions::processorAndEditor)  return "processoreditor";
        if (opt == FileCreationOptions::araPluginFiles)      return "arapluginfiles";

        jassertfalse;
        return {};
    }

    inline FileCreationOptions getFileOptionForVar (var opt)
    {
        if (opt == "none")             return FileCreationOptions::noFiles;
        if (opt == "main")             return FileCreationOptions::main;
        if (opt == "header")           return FileCreationOptions::header;
        if (opt == "headercpp")        return FileCreationOptions::headerAndCpp;
        if (opt == "processoreditor")  return FileCreationOptions::processorAndEditor;
        if (opt == "arapluginfiles")   return FileCreationOptions::araPluginFiles;

        jassertfalse;
        return {};
    }

    inline String getStringForFileOption (FileCreationOptions opt)
    {
        if (opt == FileCreationOptions::noFiles)            return "No Files";
        if (opt == FileCreationOptions::main)               return "Main.cpp";
        if (opt == FileCreationOptions::header)             return "Main.cpp + .h";
        if (opt == FileCreationOptions::headerAndCpp)       return "Main.cpp + .h/.cpp ";
        if (opt == FileCreationOptions::processorAndEditor) return "Processor and Editor";
        if (opt == FileCreationOptions::araPluginFiles)     return "ARA Plugin Files";

        jassertfalse;
        return {};
    }

    //==============================================================================
    template <typename... Strings>
    inline StringArray addAndReturn (StringArray arr, Strings... strings)
    {
        arr.addArray ({ strings... });
        return arr;
    }

    inline std::vector<ProjectTemplate> getAllTemplates()
    {
        return
        {
            { ProjectCategory::application,
              "Blank", "Creates a blank HIGGS GUI application.",
              build_tools::ProjectType_GUIApp::getTypeName(),
              BinaryData::wizard_GUI_svg,
              getModulesRequiredForComponent(),
              {},
              FileCreationOptions::noFiles
            },

            { ProjectCategory::application,
              "GUI", "Creates a blank HIGGS GUI application with a single window component.",
              build_tools::ProjectType_GUIApp::getTypeName(),
              BinaryData::wizard_GUI_svg,
              getModulesRequiredForComponent(),
              {
                { FileCreationOptions::noFiles, {} },
                { FileCreationOptions::main, { { "Main.cpp", "mythos_MainTemplate_NoWindow_cpp" } } },
                { FileCreationOptions::header, { { "Main.cpp", "mythos_MainTemplate_Window_cpp" },
                                                 { "MainComponent.h", "mythos_ContentCompSimpleTemplate_h" } } },
                { FileCreationOptions::headerAndCpp, { { "Main.cpp", "mythos_MainTemplate_Window_cpp" },
                                                       { "MainComponent.h", "mythos_ContentCompTemplate_h" },
                                                       { "MainComponent.cpp", "mythos_ContentCompTemplate_cpp" } } }
              },
              FileCreationOptions::headerAndCpp },

            { ProjectCategory::application,
              "Audio", "Creates a blank HIGGS GUI application with a single window component and audio and MIDI in/out functions.",
              build_tools::ProjectType_GUIApp::getTypeName(),
              BinaryData::wizard_AudioApp_svg,
              addAndReturn (getModulesRequiredForComponent(), "higgs_audio_basics", "higgs_audio_devices", "higgs_audio_formats",
                                                              "higgs_audio_processors", "higgs_audio_utils", "higgs_gui_extra"),
              {
                { FileCreationOptions::header, { { "Main.cpp", "mythos_MainTemplate_Window_cpp" },
                                                 { "MainComponent.h", "mythos_AudioComponentSimpleTemplate_h" } } },
                { FileCreationOptions::headerAndCpp, { { "Main.cpp", "mythos_MainTemplate_Window_cpp" },
                                                       { "MainComponent.h", "mythos_AudioComponentTemplate_h" },
                                                       { "MainComponent.cpp", "mythos_AudioComponentTemplate_cpp" } } }
              },
              FileCreationOptions::headerAndCpp },

            { ProjectCategory::application,
              "Console", "Creates a command-line application without GUI support.",
              build_tools::ProjectType_ConsoleApp::getTypeName(),
              BinaryData::wizard_ConsoleApp_svg,
              getModulesRequiredForConsole(),
              {
                  { FileCreationOptions::noFiles, {} },
                  { FileCreationOptions::main, { { "Main.cpp", "mythos_MainConsoleAppTemplate_cpp" } } }
              },
             FileCreationOptions::main },

            { ProjectCategory::application,
              "Animated", "Creates a HIGGS GUI application which draws an animated graphical display.",
              build_tools::ProjectType_GUIApp::getTypeName(),
              BinaryData::wizard_AnimatedApp_svg,
              addAndReturn (getModulesRequiredForComponent(), "higgs_gui_extra"),
              {
                { FileCreationOptions::header, { { "Main.cpp", "mythos_MainTemplate_Window_cpp" },
                                                 { "MainComponent.h", "mythos_AudioComponentSimpleTemplate_h" } } },
                { FileCreationOptions::headerAndCpp, { { "Main.cpp", "mythos_MainTemplate_Window_cpp" },
                                                       { "MainComponent.h", "mythos_AnimatedComponentTemplate_h" },
                                                       { "MainComponent.cpp", "mythos_AnimatedComponentTemplate_cpp" } } }
              },
              FileCreationOptions::headerAndCpp },

            { ProjectCategory::application,
              "OpenGL", "Creates a blank HIGGS application with a single window component. "
                        "This component supports openGL drawing features including 3D model import and GLSL shaders.",
              build_tools::ProjectType_GUIApp::getTypeName(),
              BinaryData::wizard_OpenGL_svg,
              addAndReturn (getModulesRequiredForComponent(), "higgs_gui_extra", "higgs_opengl"),
              {
                { FileCreationOptions::header, { { "Main.cpp", "mythos_MainTemplate_Window_cpp" },
                                                 { "MainComponent.h", "mythos_AudioComponentSimpleTemplate_h" } } },
                { FileCreationOptions::headerAndCpp, { { "Main.cpp", "mythos_MainTemplate_Window_cpp" },
                                                       { "MainComponent.h", "mythos_OpenGLComponentTemplate_h" },
                                                       { "MainComponent.cpp", "mythos_OpenGLComponentTemplate_cpp" } } }
              },
              FileCreationOptions::headerAndCpp },

            { ProjectCategory::plugin,
              "Basic", "Creates an audio plug-in with a single window GUI and audio/MIDI IO functions.",
              build_tools::ProjectType_AudioPlugin::getTypeName(),
              BinaryData::wizard_AudioPlugin_svg,
              getModulesRequiredForAudioProcessor(),
              {
                { FileCreationOptions::processorAndEditor, { { "PluginProcessor.cpp", "mythos_AudioPluginFilterTemplate_cpp" },
                                                             { "PluginProcessor.h",   "mythos_AudioPluginFilterTemplate_h" },
                                                             { "PluginEditor.cpp",    "mythos_AudioPluginEditorTemplate_cpp" },
                                                             { "PluginEditor.h",      "mythos_AudioPluginEditorTemplate_h" } } }
              },
              FileCreationOptions::processorAndEditor
            },

            { ProjectCategory::plugin,
              "ARA", "Creates an ARA audio plug-in, augmenting the basic audio plug-in with ARA functionality.",
              build_tools::ProjectType_ARAAudioPlugin::getTypeName(),
              BinaryData::wizard_AudioPlugin_svg,
              getModulesRequiredForAudioProcessor(),
              {
                  { FileCreationOptions::araPluginFiles, { { "PluginProcessor.cpp",               "mythos_AudioPluginFilterTemplate_cpp" },
                                                           { "PluginProcessor.h",                 "mythos_AudioPluginARAFilterTemplate_h" },
                                                           { "PluginEditor.cpp",                  "mythos_AudioPluginARAEditorTemplate_cpp" },
                                                           { "PluginEditor.h",                    "mythos_AudioPluginARAEditorTemplate_h" },
                                                           { "PluginARADocumentController.cpp",   "mythos_AudioPluginARADocumentControllerTemplate_cpp" },
                                                           { "PluginARADocumentController.h",     "mythos_AudioPluginARADocumentControllerTemplate_h" },
                                                           { "PluginARAPlaybackRenderer.cpp",     "mythos_AudioPluginARAPlaybackRendererTemplate_cpp" },
                                                           { "PluginARAPlaybackRenderer.h",       "mythos_AudioPluginARAPlaybackRendererTemplate_h" }} }
              },
              FileCreationOptions::araPluginFiles
            },

            { ProjectCategory::library,
              "Static Library", "Creates a static library.",
              build_tools::ProjectType_StaticLibrary::getTypeName(),
              BinaryData::wizard_StaticLibrary_svg,
              getModulesRequiredForConsole(),
              {},
              FileCreationOptions::noFiles
            },

            { ProjectCategory::library,
              "Dynamic Library", "Creates a dynamic library.",
              build_tools::ProjectType_DLL::getTypeName(),
              BinaryData::wizard_DLL_svg,
              getModulesRequiredForConsole(),
              {},
              FileCreationOptions::noFiles
            }
        };
    }
}
