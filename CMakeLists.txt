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

cmake_minimum_required(VERSION 3.22)

get_property(_higgs_initial_global_languages GLOBAL PROPERTY ENABLED_LANGUAGES)

project(HIGGS VERSION 9.0.5 LANGUAGES C CXX)

if(NOT (HIGGS_IS_TOP_LEVEL OR "C" IN_LIST _higgs_initial_global_languages))
    message(FATAL_ERROR "A C compiler is required to build targets that depend on HIGGS. "
        "Add 'C' to your project's LANGUAGES.")
endif()

include(CMakeDependentOption)

set_property(GLOBAL PROPERTY USE_FOLDERS YES)

set(HIGGS_MODULES_DIR "${HIGGS_SOURCE_DIR}/modules" CACHE INTERNAL
    "The path to HIGGS modules")

# This option will disable most of the HIGGS helper functions and tools. This option exists to
# facilitate existing CMake builds which handle things like bundle creation, icons, plists, and
# binary data independently of HIGGS. This option is not recommended - use at your own risk!

option(HIGGS_MODULES_ONLY "Only configure the HIGGS modules" OFF)

include(extras/Build/CMake/HIGGSModuleSupport.cmake)

# This option controls whether dummy targets are added to the build, where these targets contain all
# of the source files for each HIGGS module. If you're planning to use an IDE and want to be able to
# browse all of HIGGS's source files, this may be useful. However, it will increase the size of
# generated IDE projects and might slow down configuration a bit. If you enable this, you should
# probably also add `set_property(GLOBAL PROPERTY USE_FOLDERS YES)` to your top level CMakeLists,
# otherwise the module sources will be added directly to the top level of the project, instead of in
# a nice 'Modules' subfolder.

cmake_dependent_option(HIGGS_ENABLE_MODULE_SOURCE_GROUPS
    "Show all module sources in IDE projects" OFF
    "NOT HIGGS_MODULES_ONLY" OFF)

add_subdirectory(modules)

if(HIGGS_MODULES_ONLY)
    return()
endif()

include(extras/Build/CMake/HIGGSUtils.cmake)

set_directory_properties(PROPERTIES
    HIGGS_COMPANY_NAME       "HIGGS"
    HIGGS_COMPANY_WEBSITE    "https://higgsmythos.com"
    HIGGS_COMPANY_EMAIL      "info@higgsmythos.com"
    HIGGS_COMPANY_COPYRIGHT  "Copyright (c) - Raw Material Software Limited")

option(HIGGS_COPY_PLUGIN_AFTER_BUILD
    "Whether or not plugins should be installed to the system after building" OFF)
set_property(GLOBAL PROPERTY HIGGS_COPY_PLUGIN_AFTER_BUILD ${HIGGS_COPY_PLUGIN_AFTER_BUILD})

set(CMAKE_CXX_EXTENSIONS FALSE)

higgs_disable_default_flags()

add_subdirectory(extras/Build)

# If you want to build the HIGGS examples with VST2/ARA support, you'll need to make the
# VST2/ARA headers visible to the higgs_audio_processors module. You can either set the paths on
# the command line, (e.g. -DHIGGS_GLOBAL_ARA_SDK_PATH=/path/to/sdk) if you're just building the HIGGS
# examples, or you can call the `higgs_set_*_sdk_path` functions in your own CMakeLists after
# importing HIGGS.

if(HIGGS_GLOBAL_AAX_SDK_PATH)
    higgs_set_aax_sdk_path("${HIGGS_GLOBAL_AAX_SDK_PATH}")
endif()

if(HIGGS_GLOBAL_VST2_SDK_PATH)
    higgs_set_vst2_sdk_path("${HIGGS_GLOBAL_VST2_SDK_PATH}")
endif()

# The ARA_SDK path should point to the "Umbrella installer" ARA_SDK directory.
# The directory can be obtained by recursively cloning https://github.com/Celemony/ARA_SDK and
# checking out the tag releases/2.1.0.
if(HIGGS_GLOBAL_ARA_SDK_PATH)
    higgs_set_ara_sdk_path("${HIGGS_GLOBAL_ARA_SDK_PATH}")
endif()

# We don't build anything other than the higgsaide by default, because we want to keep configuration
# speedy and the number of targets low. If you want to add targets for the extra projects and
# example PIPs (there's a lot of them!), specify -DHIGGS_BUILD_EXAMPLES=ON and/or
# -DHIGGS_BUILD_EXTRAS=ON when initially generating your build tree.

option(HIGGS_BUILD_EXTRAS "Add build targets for the MythosStudio and other tools" OFF)

if(HIGGS_BUILD_EXTRAS)
    add_subdirectory(extras)
endif()

option(HIGGS_BUILD_EXAMPLES "Add build targets for the DemoRunner and PIPs" OFF)

if(HIGGS_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

string(CONCAT webview2_option_message "Location that overrides the default directory where our "
                                      "FindWebView2 script is looking for the "
                                      "*Microsoft.Web.WebView2* directory")

option(HIGGS_WEBVIEW2_PACKAGE_LOCATION ${webview2_option_message} "")

# ==================================================================================================
# Install configuration

include(CMakePackageConfigHelpers)
write_basic_package_version_file("${HIGGS_BINARY_DIR}/HIGGSConfigVersion.cmake"
    VERSION ${HIGGS_VERSION}
    COMPATIBILITY ExactVersion)

set(HIGGS_INSTALL_DESTINATION "lib/cmake/HIGGS-${HIGGS_VERSION}" CACHE STRING
    "The location, relative to the install prefix, where the HIGGS config file will be installed")

set(HIGGS_MODULE_PATH "include/HIGGS-${HIGGS_VERSION}/modules")
set(UTILS_INSTALL_DIR "${HIGGS_INSTALL_DESTINATION}")
set(HIGGSAIDE_PATH "${HIGGS_TOOL_INSTALL_DIR}/${HIGGS_HIGGSAIDE_NAME}")
configure_package_config_file("${HIGGS_CMAKE_UTILS_DIR}/HIGGSConfig.cmake.in"
    "${HIGGS_BINARY_DIR}/HIGGSConfig.cmake"
    PATH_VARS UTILS_INSTALL_DIR HIGGSAIDE_PATH HIGGS_MODULE_PATH
    INSTALL_DESTINATION "${HIGGS_INSTALL_DESTINATION}")

set(HIGGS_MODULE_PATH "${HIGGS_MODULES_DIR}")
set(UTILS_INSTALL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/extras/Build/CMake")
get_target_property(HIGGSAIDE_PATH higgsaide IMPORTED_LOCATION)
configure_package_config_file("${HIGGS_CMAKE_UTILS_DIR}/HIGGSConfig.cmake.in"
    "${HIGGS_BINARY_DIR}/HIGGSExportConfig.cmake"
    PATH_VARS UTILS_INSTALL_DIR HIGGSAIDE_PATH HIGGS_MODULE_PATH
    INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}"
    INSTALL_DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

install(FILES "${HIGGS_BINARY_DIR}/HIGGSConfigVersion.cmake"
              "${HIGGS_BINARY_DIR}/HIGGSConfig.cmake"
              "${HIGGS_CMAKE_UTILS_DIR}/FindCppwinrt.cmake"
              "${HIGGS_CMAKE_UTILS_DIR}/FindWebView2.cmake"
              "${HIGGS_CMAKE_UTILS_DIR}/FindWindowsMIDIServices.cmake"
              "${HIGGS_CMAKE_UTILS_DIR}/HIGGSCheckAtomic.cmake"
              "${HIGGS_CMAKE_UTILS_DIR}/HIGGSHelperTargets.cmake"
              "${HIGGS_CMAKE_UTILS_DIR}/HIGGSModuleSupport.cmake"
              "${HIGGS_CMAKE_UTILS_DIR}/HIGGSUtils.cmake"
              "${HIGGS_CMAKE_UTILS_DIR}/HiggsLV2Defines.h.in"
              "${HIGGS_CMAKE_UTILS_DIR}/LaunchScreen.storyboard"
              "${HIGGS_CMAKE_UTILS_DIR}/PIPAudioProcessor.cpp.in"
              "${HIGGS_CMAKE_UTILS_DIR}/PIPAudioProcessorWithARA.cpp.in"
              "${HIGGS_CMAKE_UTILS_DIR}/PIPComponent.cpp.in"
              "${HIGGS_CMAKE_UTILS_DIR}/PIPConsole.cpp.in"
              "${HIGGS_CMAKE_UTILS_DIR}/RecentFilesMenuTemplate.nib"
              "${HIGGS_CMAKE_UTILS_DIR}/UnityPluginGUIScript.cs.in"
              "${HIGGS_CMAKE_UTILS_DIR}/bundleplaceholder.mm"
              "${HIGGS_CMAKE_UTILS_DIR}/checkBundleSigning.cmake"
              "${HIGGS_CMAKE_UTILS_DIR}/copyDir.cmake"
              "${HIGGS_CMAKE_UTILS_DIR}/higgs_LinuxSubprocessHelper.cpp"
              "${HIGGS_CMAKE_UTILS_DIR}/higgs_runtime_arch_detection.cpp"
        DESTINATION "${HIGGS_INSTALL_DESTINATION}")

install(DIRECTORY "${HIGGS_CMAKE_UTILS_DIR}/higgs_vst3_helper"
        DESTINATION "${HIGGS_INSTALL_DESTINATION}")

if(("${CMAKE_SOURCE_DIR}" STREQUAL "${HIGGS_SOURCE_DIR}") AND (NOT HIGGS_BUILD_HELPER_TOOLS))
    _higgs_add_lv2_manifest_helper_target()

    if(TARGET higgs_lv2_helper)
        install(TARGETS higgs_lv2_helper EXPORT LV2_HELPER DESTINATION "bin/HIGGS-${HIGGS_VERSION}")
        install(EXPORT LV2_HELPER NAMESPACE higgs:: DESTINATION "${HIGGS_INSTALL_DESTINATION}")
    endif()
endif()
