# ==============================================================================
#
#  This file is part of the Higgs Mythos framework.
#  Copyright (c) Raw Material Software Limited
#
#  Higgs Mythos is free software: you can redistribute it and/or modify
#  it under the terms of the GNU Affero General Public License as published
#  by the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  Higgs Mythos is distributed in the hope that it will be useful, but
#  WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#  GNU Affero General Public License for more details.
#
#  You should have received a copy of the GNU Affero General Public License
#  along with Higgs Mythos. If not, see <https://www.gnu.org/licenses/>.
#
# ==============================================================================

higgs_add_gui_app(MythosStudio
    BUNDLE_ID                           com.higgs.themythosstudio
    ICON_BIG                            ${CMAKE_CURRENT_SOURCE_DIR}/Source/BinaryData/Icons/higgs_icon.png
    ICON_COMPOSER_BUNDLE                "${CMAKE_CURRENT_SOURCE_DIR}/Source/Assets/AppIcon.icon"
    DOCUMENT_EXTENSIONS                 mythos
    NEEDS_CURL                          TRUE
    NEEDS_WEB_BROWSER                   FALSE)

higgs_generate_higgs_header(MythosStudio)

target_sources(MythosStudio PRIVATE
    Source/Application/mythos_AutoUpdater.cpp
    Source/Application/mythos_CommandLine.cpp
    Source/Application/mythos_Main.cpp
    Source/Application/mythos_MainWindow.cpp
    Source/Application/StartPage/mythos_StartPageComponent.cpp
    Source/Application/StartPage/mythos_NewProjectWizard.cpp
    Source/CodeEditor/mythos_DocumentEditorComponent.cpp
    Source/CodeEditor/mythos_OpenDocumentManager.cpp
    Source/CodeEditor/mythos_SourceCodeEditor.cpp
    Source/Project/Modules/mythos_Modules.cpp
    Source/Project/UI/mythos_HeaderComponent.cpp
    Source/Project/mythos_Project.cpp
    Source/ProjectSaving/mythos_ProjectExporter.cpp
    Source/ProjectSaving/mythos_ProjectSaver.cpp
    Source/ProjectSaving/mythos_ResourceFile.cpp
    Source/Settings/mythos_AppearanceSettings.cpp
    Source/Settings/mythos_StoredSettings.cpp
    Source/Utility/Helpers/mythos_CodeHelpers.cpp
    Source/Utility/Helpers/mythos_FileHelpers.cpp
    Source/Utility/Helpers/mythos_MiscUtilities.cpp
    Source/Utility/Helpers/mythos_NewFileWizard.cpp
    Source/Utility/Helpers/mythos_VersionInfo.cpp
    Source/Utility/PIPs/mythos_PIPGenerator.cpp
    Source/Utility/UI/mythos_Icons.cpp
    Source/Utility/UI/mythos_MythosTreeViewBase.cpp
    Source/Utility/UI/mythos_MythosStudioLookAndFeel.cpp
    Source/Utility/UI/mythos_SlidingPanelComponent.cpp)

target_compile_definitions(MythosStudio PRIVATE
    HIGGS_ALLOW_STATIC_NULL_VARIABLES=0
    HIGGS_LOAD_CURL_SYMBOLS_LAZILY=1
    HIGGS_LOG_ASSERTIONS=1
    HIGGS_STRICT_REFCOUNTEDPOINTER=1
    HIGGS_USE_CURL=1
    HIGGS_WEB_BROWSER=0
    # This is a temporary workaround to allow builds to complete on Xcode 15.
    # Add -Wl,-ld_classic to the OTHER_LDFLAGS build setting if you need to
    # deploy to older versions of macOS.
    HIGGS_SILENCE_XCODE_15_LINKER_WARNING=1)

higgs_add_binary_data(MythosStudioData SOURCES
    Source/BinaryData/Icons/background_logo.svg
    Source/BinaryData/Icons/export_android.svg
    Source/BinaryData/Icons/export_linux.svg
    Source/BinaryData/Icons/export_visualStudio.svg
    Source/BinaryData/Icons/export_xcode.svg
    Source/BinaryData/Icons/higgs_icon.png
    Source/BinaryData/Icons/wizard_AnimatedApp.svg
    Source/BinaryData/Icons/wizard_AudioApp.svg
    Source/BinaryData/Icons/wizard_AudioPlugin.svg
    Source/BinaryData/Icons/wizard_ConsoleApp.svg
    Source/BinaryData/Icons/wizard_DLL.svg
    Source/BinaryData/Icons/wizard_GUI.svg
    Source/BinaryData/Icons/wizard_Highlight.svg
    Source/BinaryData/Icons/wizard_OpenGL.svg
    Source/BinaryData/Icons/wizard_Openfile.svg
    Source/BinaryData/Icons/wizard_StaticLibrary.svg
    Source/BinaryData/Templates/mythos_AnimatedComponentSimpleTemplate.h
    Source/BinaryData/Templates/mythos_AnimatedComponentTemplate.cpp
    Source/BinaryData/Templates/mythos_AnimatedComponentTemplate.h
    Source/BinaryData/Templates/mythos_AudioComponentSimpleTemplate.h
    Source/BinaryData/Templates/mythos_AudioComponentTemplate.cpp
    Source/BinaryData/Templates/mythos_AudioComponentTemplate.h
    Source/BinaryData/Templates/mythos_AudioPluginARADocumentControllerTemplate.cpp
    Source/BinaryData/Templates/mythos_AudioPluginARADocumentControllerTemplate.h
    Source/BinaryData/Templates/mythos_AudioPluginARAPlaybackRendererTemplate.cpp
    Source/BinaryData/Templates/mythos_AudioPluginARAPlaybackRendererTemplate.h
    Source/BinaryData/Templates/mythos_AudioPluginEditorTemplate.cpp
    Source/BinaryData/Templates/mythos_AudioPluginEditorTemplate.h
    Source/BinaryData/Templates/mythos_AudioPluginARAEditorTemplate.cpp
    Source/BinaryData/Templates/mythos_AudioPluginARAEditorTemplate.h
    Source/BinaryData/Templates/mythos_AudioPluginFilterTemplate.cpp
    Source/BinaryData/Templates/mythos_AudioPluginFilterTemplate.h
    Source/BinaryData/Templates/mythos_AudioPluginARAFilterTemplate.h
    Source/BinaryData/Templates/mythos_ComponentTemplate.cpp
    Source/BinaryData/Templates/mythos_ComponentTemplate.h
    Source/BinaryData/Templates/mythos_ContentCompSimpleTemplate.h
    Source/BinaryData/Templates/mythos_ContentCompTemplate.cpp
    Source/BinaryData/Templates/mythos_ContentCompTemplate.h
    Source/BinaryData/Templates/mythos_InlineComponentTemplate.h
    Source/BinaryData/Templates/mythos_MainConsoleAppTemplate.cpp
    Source/BinaryData/Templates/mythos_MainTemplate_NoWindow.cpp
    Source/BinaryData/Templates/mythos_MainTemplate_Window.cpp
    Source/BinaryData/Templates/mythos_NewComponentTemplate.cpp
    Source/BinaryData/Templates/mythos_NewComponentTemplate.h
    Source/BinaryData/Templates/mythos_NewCppFileTemplate.cpp
    Source/BinaryData/Templates/mythos_NewCppFileTemplate.h
    Source/BinaryData/Templates/mythos_NewInlineComponentTemplate.h
    Source/BinaryData/Templates/mythos_OpenGLComponentSimpleTemplate.h
    Source/BinaryData/Templates/mythos_OpenGLComponentTemplate.cpp
    Source/BinaryData/Templates/mythos_OpenGLComponentTemplate.h
    Source/BinaryData/Templates/mythos_PIPAudioProcessorTemplate.h
    Source/BinaryData/Templates/mythos_PIPTemplate.h
    Source/BinaryData/colourscheme_dark.xml
    Source/BinaryData/colourscheme_light.xml
    Source/BinaryData/gradle/LICENSE
    Source/BinaryData/gradle/gradle-wrapper.jar
    Source/BinaryData/gradle/gradlew
    Source/BinaryData/gradle/gradlew.bat
    Source/BinaryData/higgs_SimpleBinaryBuilder.cpp

    ../Build/CMake/HiggsLV2Defines.h.in
    ../Build/CMake/LaunchScreen.storyboard
    ../Build/CMake/PIPAudioProcessor.cpp.in
    ../Build/CMake/PIPAudioProcessorWithARA.cpp.in
    ../Build/CMake/PIPComponent.cpp.in
    ../Build/CMake/PIPConsole.cpp.in
    ../Build/CMake/RecentFilesMenuTemplate.nib
    ../Build/CMake/UnityPluginGUIScript.cs.in
    ../Build/CMake/higgs_runtime_arch_detection.cpp
    ../Build/CMake/higgs_LinuxSubprocessHelper.cpp)

target_link_libraries(MythosStudio PRIVATE
    MythosStudioData
    higgs::higgs_build_tools
    higgs::higgs_cryptography
    higgs::higgs_gui_extra
    higgs::higgs_recommended_config_flags
    higgs::higgs_recommended_lto_flags
    higgs::higgs_recommended_warning_flags)
