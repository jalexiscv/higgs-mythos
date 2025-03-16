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

# ==================================================================================================
# HIGGS Target Support Helper Functions
#
# In this file, functions intended for use by end-users have the prefix `higgs_`.
# Functions beginning with an underscore should be considered private and susceptible to
# change, so don't call them directly.
#
# See the readme at `docs/CMake API.md` for more information about CMake usage,
# including documentation of the public functions in this file.
# ==================================================================================================

include_guard(GLOBAL)
cmake_minimum_required(VERSION 3.22)

define_property(TARGET PROPERTY HIGGS_COMPANY_NAME INHERITED
    BRIEF_DOCS "The company name for a particular target"
    FULL_DOCS "This can be found in ProjectInfo::companyName in a generated HiggsHeader.h")
set_property(GLOBAL PROPERTY HIGGS_COMPANY_NAME "yourcompany")

define_property(TARGET PROPERTY HIGGS_COMPANY_WEBSITE INHERITED
    BRIEF_DOCS "The company website for a particular target"
    FULL_DOCS "This will be placed in the Info.plist for the target")
set_property(GLOBAL PROPERTY HIGGS_COMPANY_WEBSITE "")

define_property(TARGET PROPERTY HIGGS_COMPANY_EMAIL INHERITED
    BRIEF_DOCS "The company email address for a particular target"
    FULL_DOCS "This will be placed in the Info.plist for the target")
set_property(GLOBAL PROPERTY HIGGS_COMPANY_EMAIL "")

define_property(TARGET PROPERTY HIGGS_COMPANY_COPYRIGHT INHERITED
    BRIEF_DOCS "The company copyright for a particular target"
    FULL_DOCS "This will be placed in the Info.plist for the target")
set_property(GLOBAL PROPERTY HIGGS_COMPANY_COPYRIGHT "")

define_property(TARGET PROPERTY HIGGS_VST_COPY_DIR INHERITED
    BRIEF_DOCS "Install location for VST2 plugins"
    FULL_DOCS "This is where the plugin will be copied if plugin copying is enabled")

define_property(TARGET PROPERTY HIGGS_VST3_COPY_DIR INHERITED
    BRIEF_DOCS "Install location for VST3 plugins"
    FULL_DOCS "This is where the plugin will be copied if plugin copying is enabled")

define_property(TARGET PROPERTY HIGGS_AU_COPY_DIR INHERITED
    BRIEF_DOCS "Install location for AU plugins"
    FULL_DOCS "This is where the plugin will be copied if plugin copying is enabled")

define_property(TARGET PROPERTY HIGGS_AAX_COPY_DIR INHERITED
    BRIEF_DOCS "Install location for AAX plugins"
    FULL_DOCS "This is where the plugin will be copied if plugin copying is enabled")

define_property(TARGET PROPERTY HIGGS_UNITY_COPY_DIR INHERITED
    BRIEF_DOCS "Install location for Unity plugins"
    FULL_DOCS "This is where the plugin will be copied if plugin copying is enabled")

define_property(TARGET PROPERTY HIGGS_LV2_COPY_DIR INHERITED
    BRIEF_DOCS "Install location for LV2 plugins"
    FULL_DOCS "This is where the plugin will be copied if plugin copying is enabled")

define_property(TARGET PROPERTY HIGGS_COPY_PLUGIN_AFTER_BUILD INHERITED
    BRIEF_DOCS "Whether or not plugins should be copied after building"
    FULL_DOCS "Whether or not plugins should be copied after building")
set_property(GLOBAL PROPERTY HIGGS_COPY_PLUGIN_AFTER_BUILD FALSE)

function(_higgs_available_pkgconfig_module_or_else out package alternative_package)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(package_to_be_found ${package} QUIET)

    if(package_to_be_found_FOUND)
        set(${out} ${package} PARENT_SCOPE)
    else()
        set(${out} ${alternative_package} PARENT_SCOPE)
    endif()
endfunction()

if((CMAKE_SYSTEM_NAME STREQUAL "Linux") OR (CMAKE_SYSTEM_NAME MATCHES ".*BSD"))
    _higgs_create_pkgconfig_target(HIGGS_CURL_LINUX_DEPS libcurl)
    _higgs_create_pkgconfig_target(HIGGS_GLES_LINUX_DEPS egl glesv2)
    _higgs_available_pkgconfig_module_or_else(webkit_package_name webkit2gtk-4.1 webkit2gtk-4.0)

    # All browser related libs are loaded dynamically only if they are available during runtime
    _higgs_create_pkgconfig_target(HIGGS_BROWSER_LINUX_DEPS NOLINK ${webkit_package_name} gtk+-x11-3.0)
endif()

# We set up default/fallback copy dirs here. If you need different copy dirs, use
# set_directory_properties or set_target_properties to adjust the values of `HIGGS_*_COPY_DIR` at
# the appropriate scope.

function(_higgs_set_default_properties)
    if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
        set_property(GLOBAL PROPERTY HIGGS_LV2_COPY_DIR    "$ENV{HOME}/Library/Audio/Plug-Ins/LV2")
        set_property(GLOBAL PROPERTY HIGGS_VST_COPY_DIR    "$ENV{HOME}/Library/Audio/Plug-Ins/VST")
        set_property(GLOBAL PROPERTY HIGGS_VST3_COPY_DIR   "$ENV{HOME}/Library/Audio/Plug-Ins/VST3")
        set_property(GLOBAL PROPERTY HIGGS_AU_COPY_DIR     "$ENV{HOME}/Library/Audio/Plug-Ins/Components")
        set_property(GLOBAL PROPERTY HIGGS_UNITY_COPY_DIR  "$ENV{HOME}/Library/Audio/Plug-Ins/Unity")
        set_property(GLOBAL PROPERTY HIGGS_AAX_COPY_DIR    "/Library/Application Support/Avid/Audio/Plug-Ins")
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        if(CMAKE_SIZEOF_VOID_P EQUAL 8)
            set_property(GLOBAL PROPERTY HIGGS_VST_COPY_DIR "$ENV{ProgramW6432}/Steinberg/Vstplugins")
            set(prefix "$ENV{CommonProgramW6432}")
        else()
            set_property(GLOBAL PROPERTY HIGGS_VST_COPY_DIR "$ENV{programfiles\(x86\)}/Steinberg/Vstplugins")
            set(prefix "$ENV{CommonProgramFiles\(x86\)}")
        endif()

        set_property(GLOBAL PROPERTY HIGGS_VST3_COPY_DIR   "${prefix}/VST3")
        set_property(GLOBAL PROPERTY HIGGS_AAX_COPY_DIR    "${prefix}/Avid/Audio/Plug-Ins")
        set_property(GLOBAL PROPERTY HIGGS_LV2_COPY_DIR    "$ENV{APPDATA}/LV2")
        set_property(GLOBAL PROPERTY HIGGS_UNITY_COPY_DIR  "$ENV{APPDATA}/Unity")
    elseif((CMAKE_SYSTEM_NAME STREQUAL "Linux") OR (CMAKE_SYSTEM_NAME MATCHES ".*BSD"))
        set_property(GLOBAL PROPERTY HIGGS_LV2_COPY_DIR   "$ENV{HOME}/.lv2")
        set_property(GLOBAL PROPERTY HIGGS_VST_COPY_DIR   "$ENV{HOME}/.vst")
        set_property(GLOBAL PROPERTY HIGGS_VST3_COPY_DIR  "$ENV{HOME}/.vst3")
        set_property(GLOBAL PROPERTY HIGGS_UNITY_COPY_DIR "$ENV{HOME}/.unity")
    endif()
endfunction()

_higgs_set_default_properties()

# ==================================================================================================

function(higgs_add_bundle_resources_directory target folder)
    _higgs_make_absolute(folder)

    if(NOT EXISTS "${folder}")
        message(FATAL_ERROR "Could not find resource folder ${folder}")
    endif()

    get_filename_component(folder_parent_path "${folder}" DIRECTORY)
    file(GLOB_RECURSE resources RELATIVE "${folder_parent_path}" "${folder}/*")

    foreach(file IN LISTS resources)
        target_sources(${target} PRIVATE "${folder_parent_path}/${file}")
        get_filename_component(resource_parent_path "${file}" DIRECTORY)
        set_source_files_properties("${folder_parent_path}/${file}" PROPERTIES
            HEADER_FILE_ONLY TRUE
            MACOSX_PACKAGE_LOCATION "Resources/${resource_parent_path}")
    endforeach()
endfunction()

# ==================================================================================================

function(_higgs_create_linux_subprocess_helper_target)
    if(TARGET higgs_linux_subprocess_helper)
        return()
    endif()

    set(source "${HIGGS_CMAKE_UTILS_DIR}/higgs_LinuxSubprocessHelper.cpp")

    add_executable(higgs_linux_subprocess_helper ${source})
    add_executable(higgs::higgs_linux_subprocess_helper ALIAS higgs_linux_subprocess_helper)
    target_compile_features(higgs_linux_subprocess_helper PRIVATE cxx_std_17)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    target_link_libraries(higgs_linux_subprocess_helper PRIVATE Threads::Threads ${CMAKE_DL_LIBS})
endfunction()

function(_higgs_create_embedded_linux_subprocess_target output_target_name target)
    # This library needs to be created in every directory where a target wants to link against it.
    # Pre CMake 3.20 the GENERATED property is only visible in the directory of the current target,
    # and even post 3.20, CMake will not know how to generate the sources outside this directory.
    set(target_directory_key HIGGS_EMBEDDED_LINUX_SUBPROCESS_TARGET)
    get_directory_property(embedded_linux_subprocess_target ${target_directory_key})

    if(embedded_linux_subprocess_target)
        set(${output_target_name} higgs::${embedded_linux_subprocess_target} PARENT_SCOPE)
        return()
    endif()

    set(prefix "_higgs_directory_prefix")
    set(embedded_linux_subprocess_target ${prefix}_embedded_linux_subprocess)

    while(TARGET ${embedded_linux_subprocess_target})
        set(embedded_linux_subprocess_target ${prefix}_${embedded_linux_subprocess_target})
    endwhile()

    _higgs_create_linux_subprocess_helper_target()

    get_target_property(generated_sources_directory ${target} HIGGS_GENERATED_SOURCES_DIRECTORY)

    if(generated_sources_directory)
        set(higgs_linux_subprocess_helper_binary_dir "${generated_sources_directory}/$<CONFIG>/")
    else()
        set(higgs_linux_subprocess_helper_binary_dir "${CMAKE_CURRENT_BINARY_DIR}/higgs_LinuxSubprocessHelper/$<CONFIG>/")
    endif()

    set(binary_header_file  "${higgs_linux_subprocess_helper_binary_dir}/higgs_LinuxSubprocessHelperBinaryData.h")
    set(binary_source_file  "${higgs_linux_subprocess_helper_binary_dir}/higgs_LinuxSubprocessHelperBinaryData.cpp")
    set(higgsaide_input_file "${higgs_linux_subprocess_helper_binary_dir}/input_file_list")

    file(GENERATE OUTPUT ${higgsaide_input_file} CONTENT "$<TARGET_FILE:higgs_linux_subprocess_helper>")
    file(MAKE_DIRECTORY ${higgs_linux_subprocess_helper_binary_dir})

    add_custom_command(
        OUTPUT
            ${binary_header_file}
            ${binary_source_file}
        COMMAND higgs::higgsaide binarydata "LinuxSubprocessHelperBinaryData" "${binary_header_file}"
            ${higgs_linux_subprocess_helper_binary_dir} "${higgsaide_input_file}"
        COMMAND
            ${CMAKE_COMMAND} -E rename "${higgs_linux_subprocess_helper_binary_dir}/BinaryData1.cpp" "${binary_source_file}"
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        DEPENDS higgs_linux_subprocess_helper
        VERBATIM)

    add_library(${embedded_linux_subprocess_target} INTERFACE)
    target_sources(${embedded_linux_subprocess_target} INTERFACE ${binary_source_file})
    target_include_directories(${embedded_linux_subprocess_target} INTERFACE ${higgs_linux_subprocess_helper_binary_dir})
    target_compile_definitions(${embedded_linux_subprocess_target} INTERFACE HIGGS_USE_EXTERNAL_TEMPORARY_SUBPROCESS=1)
    add_library(higgs::${embedded_linux_subprocess_target} ALIAS ${embedded_linux_subprocess_target})

    set_directory_properties(PROPERTIES ${target_directory_key} ${embedded_linux_subprocess_target})
    set(${output_target_name} higgs::${embedded_linux_subprocess_target} PARENT_SCOPE)
endfunction()

function(higgs_link_with_embedded_linux_subprocess target)
    _higgs_create_embedded_linux_subprocess_target(embedded_linux_subprocess_target ${target})
    target_link_libraries(${target} PRIVATE ${embedded_linux_subprocess_target})
endfunction()

# ==================================================================================================

# Ideally, we'd check the preprocessor defs on the target to see whether
# HIGGS_USE_CURL, HIGGS_WEB_BROWSER, or HIGGS_IN_APP_PURCHASES have been explicitly turned off,
# and then link libraries as appropriate.
# Unfortunately, this doesn't work, because linking a new library (curl/webkit/StoreKit)
# updates the target's compile defs, which results in a recursion/circular-dependency.
# Instead, we ask the user to explicitly request curl/webkit/StoreKit linking if they
# know they need it. Otherwise, we won't link anything.
# See the NEEDS_CURL, NEEDS_WEB_BROWSER, and NEEDS_STORE_KIT options in the CMake/readme.md.
function(_higgs_link_optional_libraries target)
    if((CMAKE_SYSTEM_NAME STREQUAL "Linux") OR (CMAKE_SYSTEM_NAME MATCHES ".*BSD"))
        get_target_property(needs_curl ${target} HIGGS_NEEDS_CURL)
        get_target_property(needs_browser ${target} HIGGS_NEEDS_WEB_BROWSER)
        get_target_property(needs_opengl_es ${target} HIGGS_NEEDS_OPENGL_ES)

        target_compile_definitions(${target} PRIVATE
            HIGGS_USE_CURL=$<BOOL:${needs_curl}>
            HIGGS_WEB_BROWSER=$<BOOL:${needs_browser}>
            HIGGS_OPENGL_ES=$<BOOL:${needs_opengl_es}>)

        if(needs_curl)
            target_link_libraries(${target} PRIVATE higgs::pkgconfig_HIGGS_CURL_LINUX_DEPS)
        endif()

        if(needs_opengl_es)
            target_link_libraries(${target} PRIVATE higgs::pkgconfig_HIGGS_GLES_LINUX_DEPS)
        endif()

        if(needs_browser)
            target_link_libraries(${target} PRIVATE higgs::pkgconfig_HIGGS_BROWSER_LINUX_DEPS)

            get_target_property(is_plugin ${target} HIGGS_IS_PLUGIN)

            if(is_plugin)
                higgs_link_with_embedded_linux_subprocess(${target})
            endif()
        endif()
    elseif(APPLE)
        get_target_property(needs_storekit ${target} HIGGS_NEEDS_STORE_KIT)

        target_compile_definitions(${target} PRIVATE
            HIGGS_IN_APP_PURCHASES=$<BOOL:${needs_storekit}>)

        if(needs_storekit)
            _higgs_link_frameworks("${target}" PRIVATE StoreKit)
        endif()

        get_target_property(needs_camera ${target} HIGGS_CAMERA_PERMISSION_ENABLED)

        if(CMAKE_SYSTEM_NAME STREQUAL "iOS" AND needs_camera)
            _higgs_link_frameworks("${target}" PRIVATE ImageIO)
        endif()
    elseif(WIN32)
        get_target_property(needs_webview2 ${target} HIGGS_NEEDS_WEBVIEW2)

        if (needs_webview2)
            if(NOT ("${HIGGS_CMAKE_UTILS_DIR}" IN_LIST CMAKE_MODULE_PATH))
                list(APPEND CMAKE_MODULE_PATH "${HIGGS_CMAKE_UTILS_DIR}")
            endif()

            find_package(WebView2 REQUIRED)
            target_link_libraries(${target} PRIVATE higgs::higgs_webview2)
        endif()

        get_target_property(needs_windows_midi_services ${target} HIGGS_NEEDS_WINDOWS_MIDI_SERVICES)

        if(needs_windows_midi_services)
            if(NOT ("${HIGGS_CMAKE_UTILS_DIR}" IN_LIST CMAKE_MODULE_PATH))
                list(APPEND CMAKE_MODULE_PATH "${HIGGS_CMAKE_UTILS_DIR}")
            endif()

            find_package(CppWinRT REQUIRED)
            target_link_libraries(${target} PRIVATE higgs::higgs_winrt_headers)

            find_package(WindowsMIDIServices REQUIRED)
            target_link_libraries(${target} PRIVATE higgs::higgs_windows_midi_services)
        endif()
    endif()
endfunction()

# ==================================================================================================

function(_higgs_get_module_definitions target filter out_var)
    set(compile_defs $<TARGET_GENEX_EVAL:${target},$<TARGET_PROPERTY:${target},COMPILE_DEFINITIONS>>)

    if(filter)
        set(${out_var} $<FILTER:${compile_defs},EXCLUDE,HiggsPlugin_Build_|HIGGS_SHARED_CODE> PARENT_SCOPE)
    else()
        set(${out_var} ${compile_defs} PARENT_SCOPE)
    endif()
endfunction()

function(_higgs_append_record output key)
    string(ASCII 30 RS)
    string(ASCII 31 US)

    set(prev)

    if(DEFINED "${output}")
        set(prev "${${output}}")
    endif()

    set(${output} "${prev}${key}${US}${ARGN}${RS}" PARENT_SCOPE)
endfunction()

function(_higgs_append_target_property output key target property)
    get_target_property(prop ${target} ${property})

    if(prop STREQUAL "prop-NOTFOUND")
        set(prop)
    endif()

    _higgs_append_record(${output} ${key} ${prop})
    set(${output} "${${output}}" PARENT_SCOPE)
endfunction()

# This is all info that should be known at configure time (i.e. no generator expressions here!)
# We use this info to generate plists and entitlements files, also at configure time.
function(_higgs_write_configure_time_info target)
    _higgs_append_target_property(file_content EXECUTABLE_NAME                      ${target} HIGGS_PRODUCT_NAME)
    _higgs_append_target_property(file_content VERSION                              ${target} HIGGS_VERSION)
    _higgs_append_target_property(file_content BUILD_VERSION                        ${target} HIGGS_BUILD_VERSION)
    _higgs_append_target_property(file_content PLIST_TO_MERGE                       ${target} HIGGS_PLIST_TO_MERGE)
    _higgs_append_target_property(file_content BUNDLE_ID                            ${target} HIGGS_BUNDLE_ID)
    _higgs_append_target_property(file_content XCODE_EXTRA_PLIST_ENTRIES            ${target} HIGGS_XCODE_EXTRA_PLIST_ENTRIES)
    _higgs_append_target_property(file_content MICROPHONE_PERMISSION_ENABLED        ${target} HIGGS_MICROPHONE_PERMISSION_ENABLED)
    _higgs_append_target_property(file_content MICROPHONE_PERMISSION_TEXT           ${target} HIGGS_MICROPHONE_PERMISSION_TEXT)
    _higgs_append_target_property(file_content CAMERA_PERMISSION_ENABLED            ${target} HIGGS_CAMERA_PERMISSION_ENABLED)
    _higgs_append_target_property(file_content CAMERA_PERMISSION_TEXT               ${target} HIGGS_CAMERA_PERMISSION_TEXT)
    _higgs_append_target_property(file_content BLUETOOTH_PERMISSION_ENABLED         ${target} HIGGS_BLUETOOTH_PERMISSION_ENABLED)
    _higgs_append_target_property(file_content BLUETOOTH_PERMISSION_TEXT            ${target} HIGGS_BLUETOOTH_PERMISSION_TEXT)
    _higgs_append_target_property(file_content LOCAL_NETWORK_PERMISSION_ENABLED     ${target} HIGGS_LOCAL_NETWORK_PERMISSION_ENABLED)
    _higgs_append_target_property(file_content LOCAL_NETWORK_PERMISSION_TEXT        ${target} HIGGS_LOCAL_NETWORK_PERMISSION_TEXT)
    _higgs_append_target_property(file_content SEND_APPLE_EVENTS_PERMISSION_ENABLED ${target} HIGGS_SEND_APPLE_EVENTS_PERMISSION_ENABLED)
    _higgs_append_target_property(file_content SEND_APPLE_EVENTS_PERMISSION_TEXT    ${target} HIGGS_SEND_APPLE_EVENTS_PERMISSION_TEXT)
    _higgs_append_target_property(file_content SHOULD_ADD_STORYBOARD                ${target} HIGGS_SHOULD_ADD_STORYBOARD)
    _higgs_append_target_property(file_content LAUNCH_STORYBOARD_FILE               ${target} HIGGS_LAUNCH_STORYBOARD_FILE)
    _higgs_append_target_property(file_content ICON_FILE                            ${target} HIGGS_ICON_FILE)
    _higgs_append_target_property(file_content ICON_COMPOSER_BUNDLE                 ${target} HIGGS_ICON_COMPOSER_BUNDLE)
    _higgs_append_target_property(file_content PROJECT_NAME                         ${target} HIGGS_PRODUCT_NAME)
    _higgs_append_target_property(file_content COMPANY_COPYRIGHT                    ${target} HIGGS_COMPANY_COPYRIGHT)
    _higgs_append_target_property(file_content COMPANY_NAME                         ${target} HIGGS_COMPANY_NAME)
    _higgs_append_target_property(file_content DOCUMENT_EXTENSIONS                  ${target} HIGGS_DOCUMENT_EXTENSIONS)
    _higgs_append_target_property(file_content FILE_SHARING_ENABLED                 ${target} HIGGS_FILE_SHARING_ENABLED)
    _higgs_append_target_property(file_content DOCUMENT_BROWSER_ENABLED             ${target} HIGGS_DOCUMENT_BROWSER_ENABLED)
    _higgs_append_target_property(file_content STATUS_BAR_HIDDEN                    ${target} HIGGS_STATUS_BAR_HIDDEN)
    _higgs_append_target_property(file_content REQUIRES_FULL_SCREEN                 ${target} HIGGS_REQUIRES_FULL_SCREEN)
    _higgs_append_target_property(file_content BACKGROUND_AUDIO_ENABLED             ${target} HIGGS_BACKGROUND_AUDIO_ENABLED)
    _higgs_append_target_property(file_content BACKGROUND_BLE_ENABLED               ${target} HIGGS_BACKGROUND_BLE_ENABLED)
    _higgs_append_target_property(file_content PUSH_NOTIFICATIONS_ENABLED           ${target} HIGGS_PUSH_NOTIFICATIONS_ENABLED)
    _higgs_append_target_property(file_content NETWORK_MULTICAST_ENABLED            ${target} HIGGS_NETWORK_MULTICAST_ENABLED)
    _higgs_append_target_property(file_content PLUGIN_MANUFACTURER_CODE             ${target} HIGGS_PLUGIN_MANUFACTURER_CODE)
    _higgs_append_target_property(file_content PLUGIN_CODE                          ${target} HIGGS_PLUGIN_CODE)
    _higgs_append_target_property(file_content IPHONE_SCREEN_ORIENTATIONS           ${target} HIGGS_IPHONE_SCREEN_ORIENTATIONS)
    _higgs_append_target_property(file_content IPAD_SCREEN_ORIENTATIONS             ${target} HIGGS_IPAD_SCREEN_ORIENTATIONS)
    _higgs_append_target_property(file_content PLUGIN_NAME                          ${target} HIGGS_PLUGIN_NAME)
    _higgs_append_target_property(file_content PLUGIN_MANUFACTURER                  ${target} HIGGS_COMPANY_NAME)
    _higgs_append_target_property(file_content PLUGIN_DESCRIPTION                   ${target} HIGGS_DESCRIPTION)
    _higgs_append_target_property(file_content PLUGIN_AU_EXPORT_PREFIX              ${target} HIGGS_AU_EXPORT_PREFIX)
    _higgs_append_target_property(file_content PLUGIN_AU_MAIN_TYPE                  ${target} HIGGS_AU_MAIN_TYPE_CODE)
    _higgs_append_target_property(file_content PLUGIN_AU_FRAMEWORK_BUNDLE_ID        ${target} HIGGS_AU_FRAMEWORK_BUNDLE_ID)
    _higgs_append_target_property(file_content IS_AU_SANDBOX_SAFE                   ${target} HIGGS_AU_SANDBOX_SAFE)
    _higgs_append_target_property(file_content IS_PLUGIN_SYNTH                      ${target} HIGGS_IS_SYNTH)
    _higgs_append_target_property(file_content IS_PLUGIN_ARA_EFFECT                 ${target} HIGGS_IS_ARA_EFFECT)
    _higgs_append_target_property(file_content SUPPRESS_AU_PLIST_RESOURCE_USAGE     ${target} HIGGS_SUPPRESS_AU_PLIST_RESOURCE_USAGE)
    _higgs_append_target_property(file_content HARDENED_RUNTIME_ENABLED             ${target} HIGGS_HARDENED_RUNTIME_ENABLED)
    _higgs_append_target_property(file_content APP_SANDBOX_ENABLED                  ${target} HIGGS_APP_SANDBOX_ENABLED)
    _higgs_append_target_property(file_content APP_SANDBOX_INHERIT                  ${target} HIGGS_APP_SANDBOX_INHERIT)
    _higgs_append_target_property(file_content HARDENED_RUNTIME_OPTIONS             ${target} HIGGS_HARDENED_RUNTIME_OPTIONS)
    _higgs_append_target_property(file_content APP_SANDBOX_OPTIONS                  ${target} HIGGS_APP_SANDBOX_OPTIONS)
    _higgs_append_target_property(file_content APP_SANDBOX_FILE_ACCESS_HOME_RO      ${target} HIGGS_APP_SANDBOX_FILE_ACCESS_HOME_RO)
    _higgs_append_target_property(file_content APP_SANDBOX_FILE_ACCESS_HOME_RW      ${target} HIGGS_APP_SANDBOX_FILE_ACCESS_HOME_RW)
    _higgs_append_target_property(file_content APP_SANDBOX_FILE_ACCESS_ABS_RO       ${target} HIGGS_APP_SANDBOX_FILE_ACCESS_ABS_RO)
    _higgs_append_target_property(file_content APP_SANDBOX_FILE_ACCESS_ABS_RW       ${target} HIGGS_APP_SANDBOX_FILE_ACCESS_ABS_RW)
    _higgs_append_target_property(file_content APP_SANDBOX_EXCEPTION_IOKIT          ${target} HIGGS_APP_SANDBOX_EXCEPTION_IOKIT)
    _higgs_append_target_property(file_content APP_GROUPS_ENABLED                   ${target} HIGGS_APP_GROUPS_ENABLED)
    _higgs_append_target_property(file_content APP_GROUP_IDS                        ${target} HIGGS_APP_GROUP_IDS)
    _higgs_append_target_property(file_content IS_PLUGIN                            ${target} HIGGS_IS_PLUGIN)
    _higgs_append_target_property(file_content ICLOUD_PERMISSIONS_ENABLED           ${target} HIGGS_ICLOUD_PERMISSIONS_ENABLED)
    _higgs_append_target_property(file_content IS_AU_PLUGIN_HOST                    ${target} HIGGS_PLUGINHOST_AU)

    if(CMAKE_SYSTEM_NAME STREQUAL "iOS")
        _higgs_append_record(file_content IS_IOS 1)
    else()
        _higgs_append_record(file_content IS_IOS 0)
    endif()

    get_target_property(higgs_library_code ${target} HIGGS_GENERATED_SOURCES_DIRECTORY)

    set(info_file "${higgs_library_code}/Info.txt")
    file(WRITE "${info_file}" "${file_content}")
    set_target_properties(${target} PROPERTIES HIGGS_INFO_FILE "${info_file}")
endfunction()

# In this file, we put things that CMake is only able to divine at generate time, like preprocessor definitions.
# We use the target preprocessor definitions to work out which HIGGS modules should go in the HiggsHeader.h.
function(_higgs_write_generate_time_info target)
    _higgs_get_module_definitions(${target} OFF module_defs)
    _higgs_append_record(defs MODULE_DEFINITIONS ${module_defs})

    _higgs_append_target_property(defs EXECUTABLE_NAME                     ${target} HIGGS_PRODUCT_NAME)
    _higgs_append_target_property(defs PROJECT_NAME                        ${target} HIGGS_PRODUCT_NAME)
    _higgs_append_target_property(defs VERSION                             ${target} HIGGS_VERSION)
    _higgs_append_target_property(defs COMPANY_NAME                        ${target} HIGGS_COMPANY_NAME)

    get_target_property(higgs_library_code ${target} HIGGS_GENERATED_SOURCES_DIRECTORY)

    set(defs_file "${higgs_library_code}/$<CONFIG>/Defs.txt")
    file(GENERATE OUTPUT "${defs_file}" CONTENT "${defs}")
    set_target_properties(${target} PROPERTIES HIGGS_DEFS_FILE "${defs_file}")
endfunction()

# ==================================================================================================

function(higgs_add_binary_data target)
    set(one_value_args NAMESPACE HEADER_NAME)
    set(multi_value_args SOURCES)
    cmake_parse_arguments(HIGGS_ARG "" "${one_value_args}" "${multi_value_args}" ${ARGN})

    list(LENGTH HIGGS_ARG_SOURCES num_binary_files)

    if(${num_binary_files} LESS 1)
        message(FATAL_ERROR "higgs_add_binary_data must be passed at least one file to encode")
    endif()

    add_library(${target} STATIC)

    set_target_properties(${target} PROPERTIES POSITION_INDEPENDENT_CODE TRUE)

    set(higgs_binary_data_folder "${CMAKE_CURRENT_BINARY_DIR}/higgs_binarydata_${target}/HiggsLibraryCode")

    set(binary_file_names)

    foreach(index RANGE 1 ${num_binary_files})
        list(APPEND binary_file_names "${higgs_binary_data_folder}/BinaryData${index}.cpp")
    endforeach()

    file(MAKE_DIRECTORY ${higgs_binary_data_folder})

    if(NOT HIGGS_ARG_NAMESPACE)
        set(HIGGS_ARG_NAMESPACE BinaryData)
    endif()

    if(NOT HIGGS_ARG_HEADER_NAME)
        set(HIGGS_ARG_HEADER_NAME BinaryData.h)
    endif()

    list(APPEND binary_file_names "${higgs_binary_data_folder}/${HIGGS_ARG_HEADER_NAME}")

    set(newline_delimited_input)

    foreach(name IN LISTS HIGGS_ARG_SOURCES)
        _higgs_make_absolute(name)
        set(newline_delimited_input "${newline_delimited_input}${name}\n")
    endforeach()

    set(input_file_list "${higgs_binary_data_folder}/input_file_list")

    set(old_input_file_list "")

    if(EXISTS "${input_file_list}")
        file(READ "${input_file_list}" old_input_file_list)
    endif()

    if(NOT "${old_input_file_list}" STREQUAL "${newline_delimited_input}")
        file(WRITE "${input_file_list}" "${newline_delimited_input}")
    endif()

    add_custom_command(OUTPUT ${binary_file_names}
        COMMAND higgs::higgsaide binarydata "${HIGGS_ARG_NAMESPACE}" "${HIGGS_ARG_HEADER_NAME}"
            ${higgs_binary_data_folder} "${input_file_list}"
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        DEPENDS "${input_file_list}" ${HIGGS_ARG_SOURCES}
        VERBATIM)

    target_sources(${target} PRIVATE "${binary_file_names}")
    target_include_directories(${target} INTERFACE ${higgs_binary_data_folder})
    target_compile_features(${target} PRIVATE cxx_std_17)

    # This fixes an issue where Xcode is unable to find binary data during archive.
    if(CMAKE_GENERATOR STREQUAL "Xcode")
        set_target_properties(${target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "./")
    endif()

    if(HIGGS_ARG_HEADER_NAME STREQUAL "BinaryData.h")
        target_compile_definitions(${target} INTERFACE HIGGS_TARGET_HAS_BINARY_DATA=1)
    endif()
endfunction()

# ==================================================================================================

function(_higgs_version_code version_in out_var)
    string(REGEX REPLACE "\\." ";" version_list ${version_in})
    list(LENGTH version_list num_version_components)

    set(version_major 0)
    set(version_minor 0)
    set(version_patch 0)

    if(num_version_components GREATER 0)
        list(GET version_list 0 version_major)
    endif()

    if(num_version_components GREATER 1)
        list(GET version_list 1 version_minor)
    endif()

    if(num_version_components GREATER 2)
        list(GET version_list 2 version_patch)
    endif()

    math(EXPR hex "(${version_major} << 16) + (${version_minor} << 8) + ${version_patch}"
        OUTPUT_FORMAT HEXADECIMAL)
    set(${out_var} "${hex}" PARENT_SCOPE)
endfunction()

function(_higgs_to_char_literal str out_var help_text)
    string(LENGTH "${str}" string_length)

    if(NOT "${string_length}" EQUAL "4")
        message(WARNING "The ${help_text} code must contain exactly four characters, but it was set to '${str}'")
    endif()

    # Round-tripping through a file is the simplest way to convert a string to hex...
    string(SUBSTRING "${str}" 0 4 four_chars)
    string(RANDOM LENGTH 16 random_string)
    set(scratch_file "${CMAKE_CURRENT_BINARY_DIR}/${random_string}_ascii_conversion.txt")

    file(WRITE "${scratch_file}" "${four_chars}")
    file(READ "${scratch_file}" four_chars_hex HEX)
    file(REMOVE "${scratch_file}")

    string(SUBSTRING "${four_chars_hex}00000000" 0 8 four_chars_hex)
    set(${out_var} "${four_chars_hex}" PARENT_SCOPE)
endfunction()

# ==================================================================================================

function(higgs_generate_higgs_header target)
    get_target_property(higgs_library_code ${target} HIGGS_GENERATED_SOURCES_DIRECTORY)

    if(NOT higgs_library_code)
        message(FATAL_ERROR "Target ${target} does not have a generated sources directory. Ensure it was created with a higgs_add_* function")
    endif()

    set(higgs_header ${higgs_library_code}/HiggsHeader.h)
    target_sources(${target} PRIVATE ${higgs_header})

    set(defs_file $<GENEX_EVAL:$<TARGET_PROPERTY:${target},HIGGS_DEFS_FILE>>)

    set(extra_args)

    add_custom_command(OUTPUT "${higgs_header}"
        COMMAND higgs::higgsaide header "${defs_file}" "${higgs_header}" ${extra_args}
        DEPENDS "${defs_file}"
        VERBATIM)
endfunction()

# ==================================================================================================

function(_higgs_execute_higgsaide)
    if(NOT TARGET higgs::higgsaide)
        message(FATAL_ERROR "The higgsaide target does not exist")
    endif()

    get_target_property(higgsaide_location higgs::higgsaide IMPORTED_LOCATION)

    if(NOT EXISTS "${higgsaide_location}")
        message(FATAL_ERROR "higgsaide was imported, but it doesn't exist!")
    endif()

    execute_process(COMMAND "${higgsaide_location}" ${ARGN}
        RESULT_VARIABLE result_variable
        OUTPUT_VARIABLE output
        ERROR_VARIABLE output)

    if(result_variable)
        message(FATAL_ERROR "Running higgsaide failed:\ncommand: ${higgsaide_location} ${ARGN}\noutput: ${output}")
    endif()
endfunction()

function(_higgs_set_output_name target name)
    if(NOT CMAKE_SYSTEM_NAME STREQUAL "Android")
        set_target_properties(${target} PROPERTIES
            OUTPUT_NAME ${name}
            XCODE_ATTRIBUTE_PRODUCT_NAME ${name})
    endif()
endfunction()

function(_higgs_check_icon_files_exist icon_files)
    foreach(file IN LISTS icon_files)
        if(NOT EXISTS "${file}")
            message(FATAL_ERROR "Could not find icon file: ${file}")
        endif()
    endforeach()
endfunction()

function(_higgs_generate_icon source_target dest_target)
    get_target_property(higgs_library_code ${source_target} HIGGS_GENERATED_SOURCES_DIRECTORY)
    get_target_property(higgs_property_icon_big ${source_target} HIGGS_ICON_BIG)
    get_target_property(higgs_property_icon_small ${source_target} HIGGS_ICON_SMALL)
    get_target_property(higgs_property_icon_composer_bundle ${source_target} HIGGS_ICON_COMPOSER_BUNDLE)

    set(icon_args)

    if(higgs_property_icon_big)
        list(APPEND icon_args "${higgs_property_icon_big}")
    endif()

    if(higgs_property_icon_small)
        list(APPEND icon_args "${higgs_property_icon_small}")
    endif()

    get_filename_component(icon_composer_icon_name "${higgs_property_icon_composer_bundle}" NAME_WE)

    if(higgs_property_icon_composer_bundle AND (CMAKE_SYSTEM_NAME STREQUAL "Darwin" OR CMAKE_SYSTEM_NAME STREQUAL "iOS"))
        if((CMAKE_SYSTEM_NAME STREQUAL "Darwin") AND (NOT CMAKE_GENERATOR STREQUAL "Xcode"))
            add_custom_command(TARGET ${target} POST_BUILD
                COMMAND "${CMAKE_COMMAND}"
                    "-Dbundle_dir='$<TARGET_BUNDLE_DIR:${target}>'"
                    "-Dicon_path=${higgs_property_icon_composer_bundle}"
                    "-P" "${HIGGS_CMAKE_UTILS_DIR}/generateXcassetsFromIcon.cmake"
                VERBATIM)
        else()
            set_source_files_properties("${higgs_property_icon_composer_bundle}"
                PROPERTIES
                    MACOSX_PACKAGE_LOCATION Resources
                    XCODE_EXPLICIT_FILE_TYPE folder.iconcomposer.icon
            )

            target_sources(${dest_target} PRIVATE "${higgs_property_icon_composer_bundle}")

            set_target_properties(${dest_target} PROPERTIES
                XCODE_ATTRIBUTE_ASSETCATALOG_COMPILER_APPICON_NAME "${icon_composer_icon_name}")
        endif()
    endif()

    set(generated_icon)

    set(apple_app_icon_name "${icon_composer_icon_name}")

    if(NOT apple_app_icon_name)
        set(apple_app_icon_name "AppIcon")
    endif()

    if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
        if(NOT icon_args)
            return()
        endif()

        _higgs_check_icon_files_exist("${icon_args}")

        set(generated_icon "${higgs_library_code}/${apple_app_icon_name}.icns")
        # To get compiled properly, we need the icon before the plist is generated!
        _higgs_execute_higgsaide(macicon "${generated_icon}" "${apple_app_icon_name}" ${icon_args})
        set_source_files_properties(${generated_icon} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        if(NOT icon_args)
            return()
        endif()

        _higgs_check_icon_files_exist("${icon_args}")

        set(generated_icon "${higgs_library_code}/icon.ico")
        _higgs_execute_higgsaide(winicon "${generated_icon}" ${icon_args})
    elseif(CMAKE_SYSTEM_NAME STREQUAL "iOS")
        get_target_property(generated_icon ${source_target} HIGGS_CUSTOM_XCASSETS_FOLDER)

        if(icon_args AND (NOT generated_icon))
            _higgs_check_icon_files_exist("${icon_args}")

            set(out_path "${higgs_library_code}/${dest_target}")
            set(generated_icon "${out_path}/Images.xcassets")

            # To get compiled properly, we need iOS assets at configure time!
            _higgs_execute_higgsaide(iosassets "${out_path}" "${apple_app_icon_name}" ${icon_args})
        endif()

        if(NOT generated_icon)
            return()
        endif()

        get_target_property(existing_appicon ${dest_target} XCODE_ATTRIBUTE_ASSETCATALOG_COMPILER_APPICON_NAME)

        if(NOT existing_appicon)
            set_target_properties(${dest_target} PROPERTIES
                XCODE_ATTRIBUTE_ASSETCATALOG_COMPILER_APPICON_NAME "${apple_app_icon_name}")
        endif()

        get_target_property(add_storyboard ${source_target} HIGGS_SHOULD_ADD_STORYBOARD)

        if(NOT add_storyboard)
            set_target_properties(${dest_target} PROPERTIES
                XCODE_ATTRIBUTE_ASSETCATALOG_COMPILER_LAUNCHIMAGE_NAME "LaunchImage")
        endif()
    endif()

    if(generated_icon)
        _higgs_check_icon_files_exist("${generated_icon}")

        target_sources(${dest_target} PRIVATE ${generated_icon})
        set_target_properties(${source_target} ${dest_target} PROPERTIES
            HIGGS_ICON_FILE "${generated_icon}"
            RESOURCE "${generated_icon}")
    endif()
endfunction()

function(_higgs_add_xcode_entitlements source_target dest_target)
    if(NOT APPLE)
        return()
    endif()

    get_target_property(higgs_kind_string ${dest_target} HIGGS_TARGET_KIND_STRING)
    get_target_property(input_info_file ${source_target} HIGGS_INFO_FILE)

    get_target_property(higgs_library_code ${source_target} HIGGS_GENERATED_SOURCES_DIRECTORY)
    set(entitlements_file "${higgs_library_code}/${dest_target}.entitlements")

    _higgs_execute_higgsaide(entitlements "${higgs_kind_string}" "${input_info_file}" "${entitlements_file}")
    set_target_properties(${dest_target} PROPERTIES
        XCODE_ATTRIBUTE_CODE_SIGN_ENTITLEMENTS
        "${entitlements_file}"
        XCODE_ATTRIBUTE_ENABLE_HARDENED_RUNTIME
        "$<TARGET_PROPERTY:${source_target},HIGGS_HARDENED_RUNTIME_ENABLED>")
endfunction()

function(_higgs_configure_bundle source_target dest_target)
    _higgs_generate_icon(${source_target} ${dest_target})
    _higgs_write_configure_time_info(${source_target})

    if(NOT APPLE)
        return()
    endif()

    get_target_property(higgs_library_code ${source_target} HIGGS_GENERATED_SOURCES_DIRECTORY)
    get_target_property(input_info_file ${source_target} HIGGS_INFO_FILE)

    set(this_output_info_dir "${higgs_library_code}/${dest_target}")
    set(this_output_pkginfo "${this_output_info_dir}/PkgInfo")
    set(this_output_plist "${this_output_info_dir}/Info.plist")

    get_target_property(higgs_kind_string ${dest_target} HIGGS_TARGET_KIND_STRING)

    _higgs_execute_higgsaide(plist "${higgs_kind_string}" "${input_info_file}" "${this_output_plist}")
    set_target_properties(${dest_target} PROPERTIES
        BUNDLE TRUE
        MACOSX_BUNDLE_INFO_PLIST "${this_output_plist}")

    add_custom_command(OUTPUT "${this_output_pkginfo}"
        COMMAND higgs::higgsaide pkginfo "${higgs_kind_string}" "${this_output_pkginfo}"
        VERBATIM)

    set(output_folder "$<TARGET_BUNDLE_CONTENT_DIR:${dest_target}>")

    target_sources(${dest_target} PRIVATE "${this_output_pkginfo}")
    set_source_files_properties("${this_output_pkginfo}" PROPERTIES
        HEADER_FILE_ONLY TRUE
        GENERATED TRUE)
    add_custom_command(TARGET ${dest_target} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy "${this_output_pkginfo}" "${output_folder}"
        VERBATIM)

    _higgs_add_xcode_entitlements(${source_target} ${dest_target})

    if(CMAKE_SYSTEM_NAME STREQUAL "iOS")
        get_target_property(add_storyboard ${source_target} HIGGS_SHOULD_ADD_STORYBOARD)

        if(add_storyboard)
            get_target_property(storyboard_file ${source_target} HIGGS_LAUNCH_STORYBOARD_FILE)

            if(NOT EXISTS "${storyboard_file}")
                message(FATAL_ERROR "Could not find storyboard file: ${storyboard_file}")
            endif()

            target_sources(${dest_target} PRIVATE "${storyboard_file}")
            set_property(TARGET ${dest_target} APPEND PROPERTY RESOURCE "${storyboard_file}")
        endif()
    endif()

    if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
        set_target_properties(${dest_target} PROPERTIES
            XCODE_ATTRIBUTE_OTHER_CODE_SIGN_FLAGS "--timestamp")
    endif()

    set_target_properties(${dest_target} PROPERTIES
        XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY
            "$<TARGET_PROPERTY:${source_target},HIGGS_TARGETED_DEVICE_FAMILY>")

    if(higgs_kind_string STREQUAL "AUv3 AppExtension")
        get_target_property(source_bundle_id ${source_target} HIGGS_BUNDLE_ID)

        if(source_bundle_id MATCHES "\\.([^.]+)$")
            set_target_properties(${dest_target} PROPERTIES
                XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER
                    "${source_bundle_id}.${CMAKE_MATCH_1}AUv3")
        else()
            message(FATAL_ERROR "Bundle ID should contain at least one `.`!")
        endif()
    else()
        set_target_properties(${dest_target} PROPERTIES
            XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER
                $<TARGET_PROPERTY:${source_target},HIGGS_BUNDLE_ID>)
    endif()

    if(CMAKE_GENERATOR STREQUAL "Xcode")
        get_target_property(product_name ${source_target} HIGGS_PRODUCT_NAME)
        set(skip_install NO)
        set(install_path "$(LOCAL_APPS_DIR)")

        if(higgs_kind_string STREQUAL "AUv3 AppExtension")
            set(skip_install YES)
            set(install_path "")
        endif()

        set_target_properties(${dest_target} PROPERTIES
            XCODE_ATTRIBUTE_INSTALL_PATH "${install_path}"
            XCODE_ATTRIBUTE_SKIP_INSTALL "${skip_install}")
    endif()
endfunction()

function(_higgs_add_resources_rc source_target dest_target)
    if(NOT CMAKE_SYSTEM_NAME STREQUAL "Windows")
        return()
    endif()

    if(NOT TARGET ${source_target}_rc_lib)
        get_target_property(higgs_library_code ${source_target} HIGGS_GENERATED_SOURCES_DIRECTORY)
        get_target_property(input_info_file ${source_target} HIGGS_INFO_FILE)

        set(resource_rc_file "${higgs_library_code}/${source_target}_resources.rc")
        _higgs_execute_higgsaide(rcfile "${input_info_file}" "${resource_rc_file}")


        add_library(${source_target}_rc_lib OBJECT ${resource_rc_file})

        set(compile_defs $<TARGET_GENEX_EVAL:${source_target},$<TARGET_PROPERTY:${source_target},COMPILE_DEFINITIONS>>)
        set(include_dirs $<TARGET_GENEX_EVAL:${source_target},$<TARGET_PROPERTY:${source_target},INCLUDE_DIRECTORIES>>)
        set(filtered $<FILTER:${compile_defs},INCLUDE,HIGGS_USER_DEFINED_RC_FILE=>)
        set(has_custom_rc_include $<BOOL:${filtered}>)

        target_include_directories(${source_target}_rc_lib
            PRIVATE $<${has_custom_rc_include}:${include_dirs}>)
        set_source_files_properties(${resource_rc_file} PROPERTIES
            COMPILE_DEFINITIONS $<${has_custom_rc_include}:${compile_defs}>)
    endif()

    target_link_libraries(${dest_target} PRIVATE ${source_target}_rc_lib)
endfunction()

function(_higgs_configure_app_bundle source_target dest_target)
    set_target_properties(${dest_target} PROPERTIES
        HIGGS_TARGET_KIND_STRING "App"
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE)

    if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
        set(nib_path "${HIGGS_CMAKE_UTILS_DIR}/RecentFilesMenuTemplate.nib")
        target_sources("${dest_target}" PRIVATE "${nib_path}")
        set_source_files_properties("${nib_path}" PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    endif()
endfunction()

# ==================================================================================================

function(_higgs_create_windows_package source_target dest_target extension default_icon arch_string)
    get_target_property(products_folder ${dest_target} LIBRARY_OUTPUT_DIRECTORY)

    set(product_name $<TARGET_PROPERTY:${source_target},HIGGS_PRODUCT_NAME>)
    set(output_folder "${products_folder}/${product_name}.${extension}")

    set_target_properties(${dest_target}
        PROPERTIES
        PDB_OUTPUT_DIRECTORY "${products_folder}"
        LIBRARY_OUTPUT_DIRECTORY "${output_folder}/Contents/${arch_string}")

    get_target_property(icon_file ${source_target} HIGGS_ICON_FILE)

    if(NOT icon_file)
        set(icon_file "${default_icon}")
    endif()

    if(icon_file)
        set(desktop_ini "${output_folder}/desktop.ini")
        set(plugin_ico "${output_folder}/Plugin.ico")

        target_sources(${dest_target} PRIVATE "${desktop_ini}" "${icon_file}")

        file(GENERATE OUTPUT "${desktop_ini}"
            CONTENT
            "[.ShellClassInfo]\nIconResource=Plugin.ico,0\nIconFile=Plugin.ico\nIconIndex=0\n")
        add_custom_command(TARGET ${dest_target} POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy "${icon_file}" "${plugin_ico}"
            COMMAND attrib +s "${desktop_ini}"
            COMMAND attrib +s "${output_folder}"
            VERBATIM)
    endif()
endfunction()

# ==================================================================================================

function(_higgs_add_unity_plugin_prefix_if_necessary name out_var)
    string(TOLOWER "${name}" lower)

    if(NOT lower MATCHES "^audioplugin")
        set(${out_var} "audioplugin_${name}" PARENT_SCOPE)
    else()
        set(${out_var} "${name}" PARENT_SCOPE)
    endif()
endfunction()

function(_higgs_add_unity_script_file shared_target out_var)
    set(script_in "${HIGGS_CMAKE_UTILS_DIR}/UnityPluginGUIScript.cs.in")

    get_target_property(plugin_name ${shared_target} HIGGS_PLUGIN_NAME)
    get_target_property(plugin_vendor ${shared_target} HIGGS_COMPANY_NAME)
    get_target_property(plugin_description ${shared_target} HIGGS_DESCRIPTION)

    string(REGEX REPLACE " +" "_" plugin_class_name "${plugin_name}")

    get_target_property(higgs_library_code ${shared_target} HIGGS_GENERATED_SOURCES_DIRECTORY)
    _higgs_add_unity_plugin_prefix_if_necessary("${plugin_name}" script_prefix)
    set(script_out "${higgs_library_code}/${script_prefix}_UnityScript.cs")
    configure_file(${script_in} ${script_out})
    set(${out_var} "${script_out}" PARENT_SCOPE)
endfunction()

# ==================================================================================================

function(_higgs_copy_dir target from to)
    # This is a shim to make CMake copy a whole directory, rather than just
    # the contents of a directory
    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND "${CMAKE_COMMAND}"
            "-Dsrc=${from}"
            "-Ddest=${to}"
            "-P" "${HIGGS_CMAKE_UTILS_DIR}/copyDir.cmake"
        VERBATIM)
endfunction()

function(_higgs_set_copy_properties shared_code target from to_property)
    get_target_property(destination "${shared_code}" "${to_property}")

    if(destination)
        set_target_properties("${target}" PROPERTIES HIGGS_PLUGIN_COPY_DIR "${destination}")
    endif()

    set_target_properties("${target}" PROPERTIES HIGGS_PLUGIN_ARTEFACT_FILE "${from}")
endfunction()

function(_higgs_adhoc_sign target)
    if(NOT CMAKE_SYSTEM_NAME STREQUAL "Darwin")
        return()
    endif()

    get_target_property(bundle "${target}" BUNDLE)

    set(src "$<TARGET_FILE:${target}>")

    if(bundle)
        set(src "$<TARGET_BUNDLE_DIR:${target}>")
    endif()

    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND "${CMAKE_COMMAND}"
            "-Dsrc=${src}"
            "-P" "${HIGGS_CMAKE_UTILS_DIR}/checkBundleSigning.cmake"
        VERBATIM)
endfunction()

function(higgs_enable_copy_plugin_step shared_code_target)
    get_target_property(step_added ${shared_code_target} _HIGGS_PLUGIN_COPY_STEP_ADDED)

    if(step_added)
        message(WARNING "Plugin copy step requested multiple times for ${shared_code_target}")
        return()
    endif()

    set_target_properties(${shared_code_target} PROPERTIES _HIGGS_PLUGIN_COPY_STEP_ADDED TRUE)

    get_target_property(active_targets "${shared_code_target}" HIGGS_ACTIVE_PLUGIN_TARGETS)

    foreach(target IN LISTS active_targets)
        get_target_property(target_kind "${target}" HIGGS_TARGET_KIND_STRING)

        if(target_kind STREQUAL "App")
            continue()
        endif()

        _higgs_adhoc_sign("${target}")

        get_target_property(source "${target}" HIGGS_PLUGIN_ARTEFACT_FILE)

        if(NOT source)
            continue()
        endif()

        get_target_property(dest "${target}" HIGGS_PLUGIN_COPY_DIR)

        if(dest)
            _higgs_copy_dir("${target}" "${source}" "$<GENEX_EVAL:${dest}>")
        else()
            message(WARNING "Target '${target}' requested copy but no destination is set")
        endif()
    endforeach()
endfunction()

# ==================================================================================================

function(_higgs_add_lv2_manifest_helper_target)
    if(TARGET higgs_lv2_helper OR (CMAKE_SYSTEM_NAME STREQUAL "iOS") OR (CMAKE_SYSTEM_NAME STREQUAL "Android"))
        return()
    endif()

    get_target_property(module_path higgs::higgs_audio_plugin_client INTERFACE_HIGGS_MODULE_PATH)
    set(source "${module_path}/higgs_audio_plugin_client/LV2/higgs_LV2ManifestHelper.cpp")
    add_executable(higgs_lv2_helper "${source}")
    add_executable(higgs::higgs_lv2_helper ALIAS higgs_lv2_helper)
    target_compile_features(higgs_lv2_helper PRIVATE cxx_std_17)
    set_target_properties(higgs_lv2_helper PROPERTIES BUILD_WITH_INSTALL_RPATH ON)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    target_link_libraries(higgs_lv2_helper PRIVATE Threads::Threads ${CMAKE_DL_LIBS})
endfunction()

# ==================================================================================================

function(_higgs_add_vst3_manifest_helper_target shared_code_target out_target out_executable_path)
    set(helper_target ${shared_code_target}_vst3_helper)

    if(TARGET ${helper_target}
       OR (CMAKE_SYSTEM_NAME STREQUAL "iOS")
       OR (CMAKE_SYSTEM_NAME STREQUAL "Android")
       OR (CMAKE_SYSTEM_NAME MATCHES ".*BSD"))
        return()
    endif()

    get_target_property(higgs_library_code "${shared_code_target}" HIGGS_GENERATED_SOURCES_DIRECTORY)
    set(build_dir "${CMAKE_BINARY_DIR}/vst3_helpers/${shared_code_target}")
    set(helper_name "vst3_helper")

    set(shared_defs_file "${build_dir}/shared_defs_$<CONFIG>.txt")
    file(GENERATE OUTPUT "${shared_defs_file}" CONTENT "$<TARGET_PROPERTY:${shared_code_target},COMPILE_DEFINITIONS>")

    set(shared_incs_file "${build_dir}/shared_incs_$<CONFIG>.txt")
    file(GENERATE OUTPUT "${shared_incs_file}" CONTENT "$<TARGET_PROPERTY:${shared_code_target},INCLUDE_DIRECTORIES>")

    set(PASSTHROUGH_ARGS "")

    if(CMAKE_CXX_COMPILER)
        list(APPEND PASSTHROUGH_ARGS "-DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}")
    endif()

    if(CMAKE_RC_COMPILER)
        list(APPEND PASSTHROUGH_ARGS "-DCMAKE_RC_COMPILER=${CMAKE_RC_COMPILER}")
    endif()

    add_custom_target(${helper_target}
        COMMAND "${CMAKE_COMMAND}"
            "-G${CMAKE_GENERATOR}"
            "-S${HIGGS_CMAKE_UTILS_DIR}/higgs_vst3_helper"
            "-B${build_dir}"
            "-Dhelper_name=${helper_name}"
            "-Dsource_file=$<TARGET_PROPERTY:higgs_audio_plugin_client,INTERFACE_HIGGS_MODULE_PATH>/higgs_audio_plugin_client/VST3/higgs_VST3ManifestHelper.cpp"
            "-Dshared_defs_file=${shared_defs_file}"
            "-Dshared_incs_file=${shared_incs_file}"
            ${PASSTHROUGH_ARGS}

        COMMAND "${CMAKE_COMMAND}" --build "${build_dir}"

        COMMENT "Building VST3 manifest helper for ${shared_code_target}"
        VERBATIM)

    set(${out_executable_path} "${build_dir}/${helper_name}${CMAKE_EXECUTABLE_SUFFIX}" PARENT_SCOPE)
    set(${out_target} ${helper_target} PARENT_SCOPE)
endfunction()

function(higgs_enable_vst3_manifest_step shared_code_target)
    get_target_property(manifest_step_added ${shared_code_target} _HIGGS_VST3_MANIFEST_STEP_ADDED)

    if(manifest_step_added)
        message(WARNING "VST3 manifest generation has already been enabled for target ${shared_code_target}. "
            "You may need to set VST3_AUTO_MANIFEST FALSE in higgs_add_plugin, and/or check that you're "
            "not calling higgs_enable_vst3_manifest_step multiple times.")
        return()
    endif()

    get_target_property(copy_step_added ${shared_code_target} _HIGGS_PLUGIN_COPY_STEP_ADDED)

    if(copy_step_added)
        message(FATAL_ERROR "VST3 manifest generation would run after plugin copy step, so it has been disabled. "
            "If you're manually calling higgs_enable_vst3_manifest_step, then you probably need to call "
            "higgs_enable_copy_plugin_step too.")
    endif()

    set(target_name ${shared_code_target}_VST3)
    get_target_property(product ${target_name} HIGGS_PLUGIN_ARTEFACT_FILE)

    if(NOT product)
        message(FATAL_ERROR "Property HIGGS_PLUGIN_ARTEFACT_FILE not set for ${target_name}")
    endif()

    # Add a target for the helper tool
    _higgs_add_vst3_manifest_helper_target(${shared_code_target} helper_target helper_path)

    set(output_path "${product}/Contents/Resources/moduleinfo.json")

    add_dependencies(${target_name} ${helper_target})

    # Use the helper tool to write out the moduleinfo.json
    add_custom_command(TARGET ${target_name} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "creating ${output_path}"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${product}/Contents/Resources"
        COMMAND "${helper_path}" > "${output_path}")

    set_target_properties(${shared_code_target} PROPERTIES _HIGGS_VST3_MANIFEST_STEP_ADDED TRUE)
endfunction()

# ==================================================================================================

function(_higgs_disable_system_includes target)
    if(CMAKE_VERSION VERSION_GREATER "3.25")
        set_target_properties("${target}" PROPERTIES SYSTEM FALSE)
    elseif(CMAKE_VERSION VERSION_GREATER "3.23")
        set_target_properties("${target}" PROPERTIES IMPORTED_NO_SYSTEM TRUE)
    endif()
endfunction()

# ==================================================================================================

function(higgs_set_aax_sdk_path path)
    if(TARGET higgs_aax_sdk)
        message(FATAL_ERROR "higgs_set_aax_sdk_path should only be called once")
    endif()

    _higgs_make_absolute(path)

    if((NOT EXISTS "${path}")
       OR (NOT EXISTS "${path}/Interfaces")
       OR (NOT EXISTS "${path}/Interfaces/ACF"))
        message(FATAL_ERROR "Could not find AAX SDK at the specified path: ${path}")
    endif()

    if((CMAKE_SYSTEM_NAME STREQUAL "Darwin") OR (CMAKE_SYSTEM_NAME STREQUAL "Windows"))
        add_library(higgs_aax_sdk INTERFACE IMPORTED GLOBAL)
    else()
        return()
    endif()

    _higgs_disable_system_includes(higgs_aax_sdk)
    target_include_directories(higgs_aax_sdk INTERFACE
        "${path}"
        "${path}/Interfaces"
        "${path}/Interfaces/ACF")
    set_target_properties(higgs_aax_sdk PROPERTIES INTERFACE_HIGGS_AAX_DEFAULT_ICON "${path}/Utilities/PlugIn.ico")
endfunction()

function(_higgs_init_bundled_aax_sdk)
    if(TARGET higgs_aax_sdk)
        return()
    endif()

    get_target_property(module_path higgs::higgs_audio_plugin_client INTERFACE_HIGGS_MODULE_PATH)
    higgs_set_aax_sdk_path("${module_path}/higgs_audio_plugin_client/AAX/SDK")
endfunction()

# ==================================================================================================

function(_higgs_set_plugin_target_properties shared_code_target kind)
    set(target_name ${shared_code_target}_${kind})

    set_target_properties(${target_name} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "$<GENEX_EVAL:$<TARGET_PROPERTY:${shared_code_target},ARCHIVE_OUTPUT_DIRECTORY>>/${kind}"
        LIBRARY_OUTPUT_DIRECTORY "$<GENEX_EVAL:$<TARGET_PROPERTY:${shared_code_target},LIBRARY_OUTPUT_DIRECTORY>>/${kind}"
        RUNTIME_OUTPUT_DIRECTORY "$<GENEX_EVAL:$<TARGET_PROPERTY:${shared_code_target},RUNTIME_OUTPUT_DIRECTORY>>/${kind}")

    get_target_property(products_folder ${target_name} LIBRARY_OUTPUT_DIRECTORY)
    set(product_name $<TARGET_PROPERTY:${shared_code_target},HIGGS_PRODUCT_NAME>)

    if(kind STREQUAL "Standalone")
        get_target_property(is_bundle "${target_name}" BUNDLE)

        if(is_bundle)
            set_target_properties("${target_name}" PROPERTIES HIGGS_PLUGIN_ARTEFACT_FILE "$<TARGET_BUNDLE_DIR:${target_name}>")
        else()
            set_target_properties("${target_name}" PROPERTIES HIGGS_PLUGIN_ARTEFACT_FILE "$<TARGET_FILE:${target_name}>")
        endif()
    elseif(kind STREQUAL "VST3")
        set_target_properties(${target_name} PROPERTIES
            BUNDLE_EXTENSION vst3
            PREFIX ""
            SUFFIX .vst3
            BUNDLE TRUE
            XCODE_ATTRIBUTE_WRAPPER_EXTENSION vst3
            XCODE_ATTRIBUTE_LIBRARY_STYLE Bundle
            XCODE_ATTRIBUTE_GENERATE_PKGINFO_FILE YES)

        if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
            if(HIGGS_TARGET_ARCHITECTURE STREQUAL "x86_64")
                set(windows_arch "x86_64")
            elseif(HIGGS_TARGET_ARCHITECTURE STREQUAL "i386")
                set(windows_arch "x86")
            elseif(HIGGS_TARGET_ARCHITECTURE STREQUAL "arm64ec")
                set(windows_arch "arm64ec")
            elseif(HIGGS_TARGET_ARCHITECTURE STREQUAL "aarch64")
                set(windows_arch "arm64")
            else()
                message(FATAL_ERROR "Unsupported target architecture for VST3: ${HIGGS_TARGET_ARCHITECTURE}")
            endif()

            _higgs_create_windows_package(${shared_code_target} ${target_name} vst3 "" "${windows_arch}-win")
        endif()

        set(output_path "${products_folder}/${product_name}.vst3")

        if((CMAKE_SYSTEM_NAME STREQUAL "Linux") OR (CMAKE_SYSTEM_NAME MATCHES ".*BSD"))
            set_target_properties(${target_name} PROPERTIES
                SUFFIX .so
                LIBRARY_OUTPUT_DIRECTORY "${output_path}/Contents/${HIGGS_TARGET_ARCHITECTURE}-linux")
        endif()

        _higgs_set_copy_properties(${shared_code_target} ${target_name} "${output_path}" HIGGS_VST3_COPY_DIR)

        _higgs_adhoc_sign(${target_name})

        get_target_property(vst3_auto_manifest ${shared_code_target} HIGGS_VST3_AUTO_MANIFEST)

        add_custom_command(TARGET ${target_name} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E echo "removing moduleinfo.json"
            COMMAND ${CMAKE_COMMAND} -E remove -f
                "${output_path}/Contents/moduleinfo.json"
                "${output_path}/Contents/Resources/moduleinfo.json")

        if(vst3_auto_manifest)
            higgs_enable_vst3_manifest_step(${shared_code_target})
        endif()
    elseif(kind STREQUAL "VST")
        set_target_properties(${target_name} PROPERTIES
            BUNDLE_EXTENSION vst
            BUNDLE TRUE
            XCODE_ATTRIBUTE_WRAPPER_EXTENSION vst
            XCODE_ATTRIBUTE_LIBRARY_STYLE Bundle
            XCODE_ATTRIBUTE_GENERATE_PKGINFO_FILE YES)

        set(output_path "$<TARGET_FILE:${target_name}>")

        if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
            set(output_path "$<TARGET_BUNDLE_DIR:${target_name}>")
        endif()

        _higgs_set_copy_properties(${shared_code_target} ${target_name} "${output_path}" HIGGS_VST_COPY_DIR)
    elseif(kind STREQUAL "AU")
        set_target_properties(${target_name} PROPERTIES
            BUNDLE_EXTENSION component
            XCODE_ATTRIBUTE_WRAPPER_EXTENSION component
            BUNDLE TRUE
            XCODE_ATTRIBUTE_LIBRARY_STYLE Bundle
            XCODE_ATTRIBUTE_GENERATE_PKGINFO_FILE YES)

        set(output_path "$<TARGET_BUNDLE_DIR:${target_name}>")
        _higgs_set_copy_properties(${shared_code_target} ${target_name} "${output_path}" HIGGS_AU_COPY_DIR)
    elseif(kind STREQUAL "AUv3")
        set_target_properties(${target_name} PROPERTIES
            XCODE_PRODUCT_TYPE "com.apple.product-type.app-extension"
            BUNDLE_EXTENSION appex
            XCODE_ATTRIBUTE_WRAPPER_EXTENSION appex
            XCODE_ATTRIBUTE_GENERATE_PKGINFO_FILE YES)
    elseif(kind STREQUAL "AAX")
        set_target_properties(${target_name} PROPERTIES
            BUNDLE_EXTENSION aaxplugin
            PREFIX ""
            SUFFIX .aaxplugin
            XCODE_ATTRIBUTE_WRAPPER_EXTENSION aaxplugin
            BUNDLE TRUE
            XCODE_ATTRIBUTE_LIBRARY_STYLE Bundle
            XCODE_ATTRIBUTE_GENERATE_PKGINFO_FILE YES)

        _higgs_init_bundled_aax_sdk()
        get_target_property(default_icon higgs_aax_sdk INTERFACE_HIGGS_AAX_DEFAULT_ICON)

        if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
            if(HIGGS_TARGET_ARCHITECTURE STREQUAL "x86_64")
                set(windows_arch "x64")
            elseif(HIGGS_TARGET_ARCHITECTURE STREQUAL "i386")
                set(windows_arch "Win32")
            else()
                message(FATAL_ERROR "Unsupported target architecture for AAX: ${HIGGS_TARGET_ARCHITECTURE}")
            endif()

            _higgs_create_windows_package(${shared_code_target} ${target_name} aaxplugin "${default_icon}" "${windows_arch}")
        endif()

        set(output_path "${products_folder}/${product_name}.aaxplugin")
        _higgs_set_copy_properties(${shared_code_target} ${target_name} "${output_path}" HIGGS_AAX_COPY_DIR)
    elseif(kind STREQUAL "Unity")
        set_target_properties(${target_name} PROPERTIES
            BUNDLE_EXTENSION bundle
            XCODE_ATTRIBUTE_WRAPPER_EXTENSION bundle
            BUNDLE TRUE
            XCODE_ATTRIBUTE_LIBRARY_STYLE Bundle
            XCODE_ATTRIBUTE_GENERATE_PKGINFO_FILE YES)

        _higgs_add_unity_script_file(${shared_code_target} script_file)
        target_sources(${target_name} PRIVATE "${script_file}")
        set_source_files_properties("${script_file}" PROPERTIES
            GENERATED TRUE
            MACOSX_PACKAGE_LOCATION Resources)

        if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
            set(output_path "$<TARGET_BUNDLE_DIR:${target_name}>")
            _higgs_set_copy_properties(${shared_code_target} ${target_name} "${output_path}" HIGGS_UNITY_COPY_DIR)
        else()
            # On windows and linux, the gui script needs to be copied next to the unity output
            target_sources(${target_name} PRIVATE "${script_file}")
            add_custom_command(TARGET ${target_name} POST_BUILD
                COMMAND "${CMAKE_COMMAND}" -E copy "${script_file}" "${products_folder}"
                VERBATIM)

            _higgs_set_copy_properties(${shared_code_target}
                ${target_name}
                "$<TARGET_FILE:${target_name}>"
                HIGGS_UNITY_COPY_DIR)
            _higgs_set_copy_properties(${shared_code_target}
                ${target_name}
                "${script_file}"
                HIGGS_UNITY_COPY_DIR)
        endif()
    elseif(kind STREQUAL "LV2")
        if (CMAKE_SYSTEM_NAME STREQUAL "Windows" AND NOT HIGGS_WINDOWS_HELPERS_CAN_RUN)
            message(FATAL_ERROR "You cannot build a ${HIGGS_TARGET_ARCHITECTURE} LV2 plug-in on a host "
                "system processor detected to be ${CMAKE_HOST_SYSTEM_PROCESSOR}.")
        endif()

        set_target_properties(${target_name} PROPERTIES BUNDLE FALSE)

        get_target_property(HIGGS_LV2URI "${shared_code_target}" HIGGS_LV2URI)

        if(NOT HIGGS_LV2URI MATCHES "https?://.*|urn:.*")
            message(WARNING
                "LV2URI should be well-formed with an 'http' or 'urn' prefix. "
                "Check the LV2URI argument to higgs_add_plugin.")
        endif()

        set(source_header "${HIGGS_CMAKE_UTILS_DIR}/HiggsLV2Defines.h.in")
        get_target_property(higgs_library_code "${shared_code_target}" HIGGS_GENERATED_SOURCES_DIRECTORY)
        configure_file("${source_header}" "${higgs_library_code}/HiggsLV2Defines.h")

        set(output_path "${products_folder}/${product_name}.lv2")
        set_target_properties(${target_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${output_path}")

        _higgs_adhoc_sign(${target_name})

        _higgs_add_lv2_manifest_helper_target()

        add_custom_command(TARGET ${target_name} POST_BUILD
            COMMAND higgs_lv2_helper "$<TARGET_FILE:${target_name}>"
            VERBATIM)

        _higgs_set_copy_properties(${shared_code_target} ${target_name} "${output_path}" HIGGS_LV2_COPY_DIR)
    endif()
endfunction()

# Place plugin wrapper targets alongside the shared code target in IDEs
function(_higgs_set_plugin_folder_property shared_target wrapper_target)
    get_target_property(folder_to_use "${shared_target}" FOLDER)

    if(folder_to_use STREQUAL "folder_to_use-NOTFOUND")
        set_target_properties("${shared_target}" PROPERTIES FOLDER "${shared_target}")
    elseif(NOT folder_to_use MATCHES ".*${shared_target}$")
        set_target_properties("${shared_target}" PROPERTIES FOLDER "${folder_to_use}/${shared_target}")
    endif()

    get_target_property(folder_to_use "${shared_target}" FOLDER)
    set_target_properties("${wrapper_target}" PROPERTIES FOLDER "${folder_to_use}")
endfunction()

# Convert the cmake plugin kind ids to strings understood by ProjectType::Target::typeFromName
function(_higgs_get_plugin_kind_name kind out_var)
    if(kind STREQUAL "AU")
        set(${out_var} "AU" PARENT_SCOPE)
    elseif(kind STREQUAL "AUv3")
        set(${out_var} "AUv3 AppExtension" PARENT_SCOPE)
    elseif(kind STREQUAL "AAX")
        set(${out_var} "AAX" PARENT_SCOPE)
    elseif(kind STREQUAL "LV2")
        set(${out_var} "LV2" PARENT_SCOPE)
    elseif(kind STREQUAL "Standalone")
        set(${out_var} "Standalone Plugin" PARENT_SCOPE)
    elseif(kind STREQUAL "Unity")
        set(${out_var} "Unity Plugin" PARENT_SCOPE)
    elseif(kind STREQUAL "VST")
        set(${out_var} "VST" PARENT_SCOPE)
    elseif(kind STREQUAL "VST3")
        set(${out_var} "VST3" PARENT_SCOPE)
    endif()
endfunction()

function(_higgs_link_plugin_wrapper shared_code_target kind)
    set(target_name ${shared_code_target}_${kind})

    if(CMAKE_SYSTEM_NAME STREQUAL "Android")
        add_library(${target_name} SHARED)
    elseif(kind STREQUAL "Standalone")
        add_executable(${target_name} WIN32 MACOSX_BUNDLE)
    elseif(kind STREQUAL "AUv3")
        if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
            add_library(${target_name}_Framework SHARED)
            get_target_property(framework_bundle_id ${shared_code_target} HIGGS_AU_FRAMEWORK_BUNDLE_ID)
            set_target_properties(${target_name}_Framework PROPERTIES
                FRAMEWORK TRUE
                MACOSX_FRAMEWORK_IDENTIFIER ${framework_bundle_id})
            add_executable(${target_name} MACOSX_BUNDLE)
            target_link_libraries(${target_name} PRIVATE ${target_name}_Framework)
            target_sources(${target_name} PRIVATE ${HIGGS_CMAKE_UTILS_DIR}/bundleplaceholder.mm)
            _higgs_link_frameworks(${target_name} PUBLIC Foundation)
        else()
            add_executable(${target_name} MACOSX_BUNDLE)
        endif()
    else()
        add_library(${target_name} MODULE)
    endif()

    if((CMAKE_SYSTEM_NAME STREQUAL "Linux") OR (CMAKE_SYSTEM_NAME MATCHES ".*BSD"))
        target_link_options(${target_name} PRIVATE "-Wl,--no-undefined")
    endif()

    # We re-export the shared code's private include dirs, because the wrapper targets need to
    # see the module headers. We don't just link publicly, because that would introduce
    # conflicting macro definitions.
    target_include_directories(${target_name} PRIVATE
        $<TARGET_PROPERTY:${shared_code_target},INCLUDE_DIRECTORIES>)

    if("${kind};${CMAKE_SYSTEM_NAME}" STREQUAL "AUv3;Darwin")
        target_include_directories(${target_name}_Framework PRIVATE
            $<TARGET_PROPERTY:${shared_code_target},INCLUDE_DIRECTORIES>)

        target_link_libraries(${target_name}_Framework PRIVATE
            ${shared_code_target}
            higgs::higgs_audio_plugin_client_${kind})
    else()
        target_link_libraries(${target_name} PRIVATE
            ${shared_code_target}
            higgs::higgs_audio_plugin_client_${kind})
    endif()

    _higgs_set_output_name(${target_name} $<TARGET_PROPERTY:${shared_code_target},HIGGS_PRODUCT_NAME>)

    _higgs_set_plugin_folder_property("${shared_code_target}" "${target_name}")

    _higgs_get_plugin_kind_name(${kind} higgs_kind_string)
    set_target_properties(${target_name} PROPERTIES
        XCODE_ATTRIBUTE_CLANG_LINK_OBJC_RUNTIME NO
        XCODE_ATTRIBUTE_COMBINE_HIDPI_IMAGES YES
        POSITION_INDEPENDENT_CODE TRUE
        VISIBILITY_INLINES_HIDDEN TRUE
        C_VISIBILITY_PRESET hidden
        CXX_VISIBILITY_PRESET hidden
        HIGGS_TARGET_KIND_STRING "${higgs_kind_string}")

    # Under the Xcode generator, POST_BUILD commands (including the plugin copy step) run before
    # signing, but M1 macs will only load signed binaries. Setting "adhoc_codesign" forces the
    # linker to sign bundles, so that they can be loaded even if they are copied before the "real"
    # signing step. See issue 21854 on the CMake Gitlab repo.
    if("${CMAKE_GENERATOR};${CMAKE_SYSTEM_NAME};${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "Xcode;Darwin;arm64")
        target_link_options(${target_name} PRIVATE LINKER:-adhoc_codesign)
    endif()

    add_dependencies(${shared_code_target}_All ${target_name})

    if(NOT kind STREQUAL "LV2")
        _higgs_configure_bundle(${shared_code_target} ${target_name})
    else()
        _higgs_write_configure_time_info(${shared_code_target})
    endif()

    _higgs_set_plugin_target_properties(${shared_code_target} ${kind})
    _higgs_add_resources_rc(${shared_code_target} ${target_name})
endfunction()

# ==================================================================================================

function(_higgs_get_vst3_category_string target out_var)
    get_target_property(vst3_categories ${target} HIGGS_VST3_CATEGORIES)

    if((NOT Fx IN_LIST vst3_categories) AND (NOT Instrument IN_LIST vst3_categories))
        get_target_property(is_synth ${target} HIGGS_IS_SYNTH)

        if(is_synth)
            set(first_type Instrument)
        else()
            set(first_type Fx)
        endif()

        list(INSERT vst3_categories 0 ${first_type})
    else()
        if(Instrument IN_LIST vst3_categories)
            list(REMOVE_ITEM vst3_categories Instrument)
            list(INSERT vst3_categories 0 Instrument)
        endif()

        # Per the definitions in ivstaudioprocessor.h, if Fx is present, it
        # should be first in the list unless the category is Spatial|Fx
        if(Fx IN_LIST vst3_categories AND NOT (vst3_categories STREQUAL "Spatial;Fx"))
            list(REMOVE_ITEM vst3_categories Fx)
            list(INSERT vst3_categories 0 Fx)
        endif()
    endif()

    string(REGEX REPLACE ";" "|" result "${vst3_categories}")
    set(${out_var} ${result} PARENT_SCOPE)
endfunction()

function(_higgs_configure_plugin_targets target)
    _higgs_set_output_name(${target} $<TARGET_PROPERTY:${target},HIGGS_PRODUCT_NAME>_SharedCode)

    target_link_libraries(${target} PRIVATE higgs::higgs_audio_plugin_client)

    get_target_property(enabled_formats ${target} HIGGS_FORMATS)

    set(active_formats)
    _higgs_get_platform_plugin_kinds(plugin_kinds)

    foreach(kind IN LISTS plugin_kinds)
        if(kind IN_LIST enabled_formats)
            list(APPEND active_formats "${kind}")
        endif()
    endforeach()

    if((VST IN_LIST active_formats) AND (NOT TARGET higgs_vst2_sdk))
        message(FATAL_ERROR "Use higgs_set_vst2_sdk_path to set up the VST sdk before adding VST targets")
    endif()

    _higgs_add_standard_defs(${target})
    _higgs_add_plugin_definitions(${target} PRIVATE ${active_formats})

    # The plugin wrappers need to know what other modules are available, especially
    # higgs_audio_utils and higgs_gui_basics. We achieve this by searching for
    # HIGGS_MODULE_AVAILABLE_ private compile definitions, and reexporting them in
    # the interface compile definitions.
    _higgs_get_module_definitions(${target} ON enabled_modules)
    target_compile_definitions(${target} INTERFACE ${enabled_modules})

    target_compile_definitions(${target} PRIVATE HIGGS_SHARED_CODE=1)

    get_target_property(project_version_string ${target} HIGGS_VERSION)
    _higgs_version_code(${project_version_string} project_version_hex)

    get_target_property(project_manufacturer_code ${target} HIGGS_PLUGIN_MANUFACTURER_CODE)
    get_target_property(project_plugin_code ${target} HIGGS_PLUGIN_CODE)

    get_target_property(use_legacy_compatibility_plugin_code ${target} HIGGS_USE_LEGACY_COMPATIBILITY_PLUGIN_CODE)

    if(use_legacy_compatibility_plugin_code)
        set(project_manufacturer_code "proj")
    endif()

    _higgs_to_char_literal(${project_manufacturer_code} project_manufacturer_code "plugin manufacturer")
    _higgs_to_char_literal(${project_plugin_code} project_plugin_code "plugin")

    _higgs_get_vst3_category_string(${target} vst3_category_string)

    target_compile_definitions(${target} PUBLIC
        HIGGS_STANDALONE_APPLICATION=HiggsPlugin_Build_Standalone
        HiggsPlugin_IsSynth=$<BOOL:$<TARGET_PROPERTY:${target},HIGGS_IS_SYNTH>>
        HiggsPlugin_ManufacturerCode=0x${project_manufacturer_code}
        HiggsPlugin_Manufacturer="$<TARGET_PROPERTY:${target},HIGGS_COMPANY_NAME>"
        HiggsPlugin_ManufacturerWebsite="$<TARGET_PROPERTY:${target},HIGGS_COMPANY_WEBSITE>"
        HiggsPlugin_ManufacturerEmail="$<TARGET_PROPERTY:${target},HIGGS_COMPANY_EMAIL>"
        HiggsPlugin_PluginCode=0x${project_plugin_code}
        HiggsPlugin_ProducesMidiOutput=$<BOOL:$<TARGET_PROPERTY:${target},HIGGS_NEEDS_MIDI_OUTPUT>>
        HiggsPlugin_IsMidiEffect=$<BOOL:$<TARGET_PROPERTY:${target},HIGGS_IS_MIDI_EFFECT>>
        HiggsPlugin_WantsMidiInput=$<BOOL:$<TARGET_PROPERTY:${target},HIGGS_NEEDS_MIDI_INPUT>>
        HiggsPlugin_EditorRequiresKeyboardFocus=$<BOOL:$<TARGET_PROPERTY:${target},HIGGS_EDITOR_WANTS_KEYBOARD_FOCUS>>
        HiggsPlugin_Name="$<TARGET_PROPERTY:${target},HIGGS_PLUGIN_NAME>"
        HiggsPlugin_Desc="$<TARGET_PROPERTY:${target},HIGGS_DESCRIPTION>"
        HiggsPlugin_Version=${project_version_string}
        HiggsPlugin_VersionString="${project_version_string}"
        HiggsPlugin_VersionCode=${project_version_hex}
        HiggsPlugin_VSTUniqueID=HiggsPlugin_PluginCode
        HiggsPlugin_VSTCategory=$<TARGET_PROPERTY:${target},HIGGS_VST2_CATEGORY>
        HiggsPlugin_LV2PluginClass=$<TARGET_PROPERTY:${target},HIGGS_LV2_PLUGIN_CLASS>
        HiggsPlugin_Vst3Category="${vst3_category_string}"
        HiggsPlugin_AUMainType=$<TARGET_PROPERTY:${target},HIGGS_AU_MAIN_TYPE_CODE>
        HiggsPlugin_AUSubType=HiggsPlugin_PluginCode
        HiggsPlugin_AUExportPrefix=$<TARGET_PROPERTY:${target},HIGGS_AU_EXPORT_PREFIX>
        HiggsPlugin_AUExportPrefixQuoted="$<TARGET_PROPERTY:${target},HIGGS_AU_EXPORT_PREFIX>"
        HiggsPlugin_AUManufacturerCode=HiggsPlugin_ManufacturerCode
        HiggsPlugin_CFBundleIdentifier=$<TARGET_PROPERTY:${target},HIGGS_BUNDLE_ID>
        HiggsPlugin_AAXIdentifier=$<TARGET_PROPERTY:${target},HIGGS_AAX_IDENTIFIER>
        HiggsPlugin_AAXManufacturerCode=HiggsPlugin_ManufacturerCode
        HiggsPlugin_AAXProductId=HiggsPlugin_PluginCode
        HiggsPlugin_AAXCategory=$<TARGET_PROPERTY:${target},HIGGS_AAX_CATEGORY>
        HiggsPlugin_AAXDisableBypass=$<BOOL:$<TARGET_PROPERTY:${target},HIGGS_DISABLE_AAX_BYPASS>>
        HiggsPlugin_AAXDisableMultiMono=$<BOOL:$<TARGET_PROPERTY:${target},HIGGS_DISABLE_AAX_MULTI_MONO>>
        HiggsPlugin_VSTNumMidiInputs=$<TARGET_PROPERTY:${target},HIGGS_VST_NUM_MIDI_INS>
        HiggsPlugin_VSTNumMidiOutputs=$<TARGET_PROPERTY:${target},HIGGS_VST_NUM_MIDI_OUTS>
        HiggsPlugin_Enable_ARA=$<BOOL:$<TARGET_PROPERTY:${target},HIGGS_IS_ARA_EFFECT>>
        HiggsPlugin_ARAFactoryID=$<TARGET_PROPERTY:${target},HIGGS_ARA_FACTORY_ID>
        HiggsPlugin_ARADocumentArchiveID=$<TARGET_PROPERTY:${target},HIGGS_ARA_DOCUMENT_ARCHIVE_ID>
        HiggsPlugin_ARACompatibleArchiveIDs=$<TARGET_PROPERTY:${target},HIGGS_ARA_COMPATIBLE_ARCHIVE_IDS>
        HiggsPlugin_ARAContentTypes=$<TARGET_PROPERTY:${target},HIGGS_ARA_ANALYSIS_TYPES>
        HiggsPlugin_ARATransformationFlags=$<TARGET_PROPERTY:${target},HIGGS_ARA_TRANSFORMATION_FLAGS>)

    set_target_properties(${target} PROPERTIES
        POSITION_INDEPENDENT_CODE TRUE
        INTERFACE_POSITION_INDEPENDENT_CODE TRUE
        VISIBILITY_INLINES_HIDDEN TRUE
        C_VISIBILITY_PRESET hidden
        CXX_VISIBILITY_PRESET hidden)

    # A convenience target for building all plugin variations at once
    add_custom_target(${target}_All)
    _higgs_set_plugin_folder_property("${target}" "${target}_All")

    foreach(kind IN LISTS active_formats)
        _higgs_link_plugin_wrapper(${target} ${kind})

        if(TARGET ${target}_${kind})
            list(APPEND active_plugin_targets ${target}_${kind})
        endif()
    endforeach()

    set_target_properties(${target} PROPERTIES HIGGS_ACTIVE_PLUGIN_TARGETS "${active_plugin_targets}")

    if(TARGET ${target}_Standalone)
        _higgs_configure_app_bundle(${target} ${target}_Standalone)
    endif()

    if(TARGET ${target}_AAX)
        _higgs_init_bundled_aax_sdk()
        target_link_libraries(${target}_AAX PRIVATE higgs_aax_sdk)
    endif()

    if((TARGET ${target}_AUv3) AND (TARGET ${target}_Standalone))
        add_dependencies(${target}_Standalone ${target}_AUv3)
        set_target_properties(${target}_Standalone PROPERTIES
            XCODE_EMBED_APP_EXTENSIONS ${target}_AUv3)

        if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
            set_target_properties(${target}_Standalone PROPERTIES
                XCODE_EMBED_FRAMEWORKS ${target}_AUv3_Framework)
        endif()
    endif()

    get_target_property(wants_copy "${target}" HIGGS_COPY_PLUGIN_AFTER_BUILD)

    if(wants_copy)
        higgs_enable_copy_plugin_step("${target}")
    endif()
endfunction()

# ==================================================================================================

# Only sets result if the categories list contains valid entries
function(_higgs_aax_categories_to_int categories_list result)
    set(aax_category_strings
        None
        EQ
        Dynamics
        PitchShift
        Reverb
        Delay
        Modulation
        Harmonic
        NoiseReduction
        Dither
        SoundField
        HWGenerators
        SWGenerators
        WrappedPlugin
        Effect
        placeholder    # These placeholders are because there's a gap between Effect
        placeholder    # and MIDIEffect in the definition of AAX_EPlugInCategory.
        MIDIEffect)

    unset(aax_category_int)

    foreach(category_string IN LISTS categories_list)
        string(REGEX REPLACE "^AAX_[eE]PlugInCategory_" "" category_string "${category_string}")
        list(FIND aax_category_strings ${category_string} aax_index)

        if(aax_index GREATER_EQUAL 0)
            if(aax_index EQUAL 0)
                set(aax_category_bit 0)
            else()
                set(aax_category_bit "1 << (${aax_index} - 1)")
            endif()

            if(NOT DEFINED aax_category_int)
                set(aax_category_int 0)
            endif()

            math(EXPR aax_category_int "${aax_category_int} | (${aax_category_bit})")
        else()
            message(WARNING "Unrecognised AAX category: '${category_string}'. See the `CMake API.md` entry for `AAX_CATEGORY` for valid values.")
        endif()
    endforeach()

    if(DEFINED aax_category_int)
        set(${result} "${aax_category_int}" PARENT_SCOPE)
    endif()
endfunction()

# ==================================================================================================

function(_higgs_set_generic_property_if_not_set target property)
    list(LENGTH ARGN num_extra_args)

    if(num_extra_args EQUAL 0)
        return()
    endif()

    set(existing_property)
    get_target_property(existing_property ${target} ${property})

    if(existing_property STREQUAL "existing_property-NOTFOUND")
        set_target_properties(${target} PROPERTIES ${property} "${ARGN}")
    endif()
endfunction()

function(_higgs_set_property_if_not_set target property)
    _higgs_set_generic_property_if_not_set(${target} HIGGS_${property} ${ARGN})
endfunction()

function(_higgs_make_valid_4cc out_var)
    string(RANDOM LENGTH 1 ALPHABET "ABCDEFGHIJKLMNOPQRSTUVWXYZ" head)
    string(RANDOM LENGTH 3 ALPHABET "abcdefghijklmnopqrstuvwxyz" tail)
    set(${out_var} "${head}${tail}" PARENT_SCOPE)
endfunction()

# This function adds some default properties that plugin targets expect to be
# set, in order to generate the correct compile definitions.
function(_higgs_set_fallback_properties target)
    _higgs_set_property_if_not_set(${target} PRODUCT_NAME ${target})

    get_target_property(output_name ${target} HIGGS_PRODUCT_NAME)
    _higgs_set_property_if_not_set(${target} DESCRIPTION "${output_name}")
    _higgs_set_property_if_not_set(${target} PLUGIN_NAME "${output_name}")

    get_target_property(real_company_name ${target} HIGGS_COMPANY_NAME)
    _higgs_set_property_if_not_set(${target} BUNDLE_ID "com.${real_company_name}.${target}")

    get_target_property(applied_bundle_id ${target} HIGGS_BUNDLE_ID)

    if("${applied_bundle_id}" MATCHES ".* .*")
        message(WARNING "Target ${target} has HIGGS_BUNDLE_ID '${applied_bundle_id}', which contains spaces. Use the BUNDLE_ID option to specify a valid ID.")
    endif()

    _higgs_set_property_if_not_set(${target} VERSION ${PROJECT_VERSION})

    get_target_property(final_version ${target} HIGGS_VERSION)

    if(NOT final_version)
        message(FATAL_ERROR "Target ${target} must have its VERSION argument set, or must be part of a project with a PROJECT_VERSION")
    endif()

    _higgs_set_property_if_not_set(${target} BUILD_VERSION "${final_version}")

    get_target_property(custom_xcassets ${target} HIGGS_CUSTOM_XCASSETS_FOLDER)
    get_target_property(custom_storyboard ${target} HIGGS_LAUNCH_STORYBOARD_FILE)

    set(needs_storyboard TRUE)

    if((NOT custom_storyboard) AND custom_xcassets AND (EXISTS "${custom_xcassets}/LaunchImage.launchimage"))
        set(needs_storyboard FALSE)
    endif()

    set_target_properties(${target} PROPERTIES HIGGS_SHOULD_ADD_STORYBOARD ${needs_storyboard})

    _higgs_set_property_if_not_set(${target} IPHONE_SCREEN_ORIENTATIONS
        UIInterfaceOrientationPortrait UIInterfaceOrientationLandscapeLeft
        UIInterfaceOrientationLandscapeRight)

    _higgs_set_property_if_not_set(${target} IPAD_SCREEN_ORIENTATIONS
        UIInterfaceOrientationPortrait UIInterfaceOrientationLandscapeLeft
        UIInterfaceOrientationLandscapeRight)

    _higgs_set_property_if_not_set(${target}
        LAUNCH_STORYBOARD_FILE "${HIGGS_CMAKE_UTILS_DIR}/LaunchScreen.storyboard")

    _higgs_set_property_if_not_set(${target} PLUGIN_MANUFACTURER_CODE "Manu")

    # The plugin code will change on each run, unless you specify one manually!
    _higgs_make_valid_4cc(random_code)
    _higgs_set_property_if_not_set(${target} PLUGIN_CODE ${random_code})

    _higgs_set_property_if_not_set(${target} IS_SYNTH FALSE)
    _higgs_set_property_if_not_set(${target} NEEDS_MIDI_INPUT FALSE)
    _higgs_set_property_if_not_set(${target} NEEDS_MIDI_OUTPUT FALSE)
    _higgs_set_property_if_not_set(${target} IS_MIDI_EFFECT FALSE)
    _higgs_set_property_if_not_set(${target} EDITOR_WANTS_KEYBOARD_FOCUS FALSE)
    _higgs_set_property_if_not_set(${target} DISABLE_AAX_BYPASS FALSE)
    _higgs_set_property_if_not_set(${target} DISABLE_AAX_MULTI_MONO FALSE)

    _higgs_set_property_if_not_set(${target} PLUGINHOST_AU FALSE)

    get_target_property(bundle_id ${target} HIGGS_BUNDLE_ID)
    _higgs_set_property_if_not_set(${target} AAX_IDENTIFIER ${bundle_id})
    _higgs_set_property_if_not_set(${target} AU_FRAMEWORK_BUNDLE_ID ${bundle_id}.internal)

    _higgs_set_property_if_not_set(${target} VST_NUM_MIDI_INS 16)
    _higgs_set_property_if_not_set(${target} VST_NUM_MIDI_OUTS 16)

    _higgs_set_property_if_not_set(${target} AU_SANDBOX_SAFE FALSE)

    _higgs_set_property_if_not_set(${target} SUPPRESS_AU_PLIST_RESOURCE_USAGE FALSE)

    _higgs_set_property_if_not_set(${target} HARDENED_RUNTIME_ENABLED FALSE)
    _higgs_set_property_if_not_set(${target} APP_SANDBOX_ENABLED FALSE)
    _higgs_set_property_if_not_set(${target} APP_SANDBOX_INHERIT FALSE)

    _higgs_set_property_if_not_set(${target} VST3_AUTO_MANIFEST TRUE)

    get_target_property(is_synth ${target} HIGGS_IS_SYNTH)

    # VST3_CATEGORIES
    if(is_synth)
        _higgs_set_property_if_not_set(${target} VST3_CATEGORIES Instrument Synth)
    else()
        _higgs_set_property_if_not_set(${target} VST3_CATEGORIES Fx)
    endif()

    # VST2_CATEGORY
    if(is_synth)
        _higgs_set_property_if_not_set(${target} VST2_CATEGORY kPlugCategSynth)
    else()
        _higgs_set_property_if_not_set(${target} VST2_CATEGORY kPlugCategEffect)
    endif()

    # LV2 Plugin Class
    if(is_synth)
        _higgs_set_property_if_not_set(${target} LV2_PLUGIN_CLASS InstrumentPlugin)
    endif()

    _higgs_set_property_if_not_set(${target} LV2_PLUGIN_CLASS Plugin)

    get_target_property(is_midi_effect ${target} HIGGS_IS_MIDI_EFFECT)
    get_target_property(needs_midi_input ${target} HIGGS_NEEDS_MIDI_INPUT)

    # AU MAIN TYPE
    if(is_midi_effect)
        _higgs_set_property_if_not_set(${target} AU_MAIN_TYPE kAudioUnitType_MIDIProcessor)
    elseif(is_synth)
        _higgs_set_property_if_not_set(${target} AU_MAIN_TYPE kAudioUnitType_MusicDevice)
    elseif(needs_midi_input)
        _higgs_set_property_if_not_set(${target} AU_MAIN_TYPE kAudioUnitType_MusicEffect)
    else()
        _higgs_set_property_if_not_set(${target} AU_MAIN_TYPE kAudioUnitType_Effect)
    endif()

    _higgs_set_property_if_not_set(${target} TARGETED_DEVICE_FAMILY "1,2")

    set(au_category_codes
        'aufx'
        'aufc'
        'augn'
        'aumi'
        'aumx'
        'aumu'
        'aumf'
        'auol'
        'auou'
        'aupn')

    set(au_category_strings
        kAudioUnitType_Effect
        kAudioUnitType_FormatConverter
        kAudioUnitType_Generator
        kAudioUnitType_MIDIProcessor
        kAudioUnitType_Mixer
        kAudioUnitType_MusicDevice
        kAudioUnitType_MusicEffect
        kAudioUnitType_OfflineEffect
        kAudioUnitType_Output
        kAudioUnitType_Panner)

    # AU export prefix
    string(MAKE_C_IDENTIFIER ${output_name} au_prefix)
    set(au_prefix "${au_prefix}AU")
    _higgs_set_property_if_not_set(${target} AU_EXPORT_PREFIX ${au_prefix})

    # Find appropriate AU category code
    get_target_property(actual_au_category ${target} HIGGS_AU_MAIN_TYPE)
    list(FIND au_category_strings ${actual_au_category} au_index)

    if(au_index GREATER_EQUAL 0)
        list(GET au_category_codes ${au_index} au_code_representation)
        set_target_properties(${target} PROPERTIES HIGGS_AU_MAIN_TYPE_CODE ${au_code_representation})
    endif()

    # AAX category

    if(is_midi_effect)
        set(default_aax_category AAX_ePlugInCategory_MIDIEffect)
    elseif(is_synth)
        set(default_aax_category AAX_ePlugInCategory_SWGenerators)
    else()
        set(default_aax_category AAX_ePlugInCategory_None)
    endif()

    _higgs_set_property_if_not_set(${target} AAX_CATEGORY ${default_aax_category})

    get_target_property(actual_aax_category ${target} HIGGS_AAX_CATEGORY)
    _higgs_aax_categories_to_int("${actual_aax_category}" aax_category_int)

    if(DEFINED aax_category_int)
        set_target_properties(${target} PROPERTIES HIGGS_AAX_CATEGORY ${aax_category_int})
    endif()

    # Ensure this matches the MythosStudio implementation
    get_target_property(company_website ${target} HIGGS_COMPANY_WEBSITE)
    get_target_property(plugin_name ${target} HIGGS_PLUGIN_NAME)
    string(MAKE_C_IDENTIFIER "${plugin_name}" plugin_name_sanitised)
    _higgs_set_property_if_not_set(${target} LV2URI "${company_website}/plugins/${plugin_name_sanitised}")

    # ARA configuration
    # Analysis types
    set(ara_analysis_type_strings
        kARAContentTypeNotes
        kARAContentTypeTempoEntries
        kARAContentTypeBarSignatures
        kARAContentTypeStaticTuning
        kARAContentTypeKeySignatures
        kARAContentTypeSheetChords)

    get_target_property(actual_ara_analysis_types ${target} HIGGS_ARA_ANALYSIS_TYPES)

    set(ara_analysis_types_int "")

    foreach(category_string IN LISTS actual_ara_analysis_types)
        list(FIND ara_analysis_type_strings ${category_string} ara_index)

        if(ara_index GREATER_EQUAL 0)
            set(ara_analysis_types_bit "1 << ${ara_index}")

            if(ara_analysis_types_int STREQUAL "")
                set(ara_analysis_types_int 0)
            endif()

            math(EXPR ara_analysis_types_int "${ara_analysis_types_int} | (${ara_analysis_types_bit})")
        endif()
    endforeach()

    if(NOT ara_analysis_types_int STREQUAL "")
        set_target_properties(${target} PROPERTIES HIGGS_ARA_ANALYSIS_TYPES ${ara_analysis_types_int})
    endif()

    _higgs_set_property_if_not_set(${target} ARA_ANALYSIS_TYPES 0)

    # Transformation flags
    set(ara_transformation_flags_strings
        kARAPlaybackTransformationNoChanges
        kARAPlaybackTransformationTimestretch
        kARAPlaybackTransformationTimestretchReflectingTempo
        kARAPlaybackTransformationContentBasedFadeAtTail
        kARAPlaybackTransformationContentBasedFadeAtHead)

    set(default_ara_transformation_flags kARAPlaybackTransformationNoChanges)

    _higgs_set_property_if_not_set(${target} ARA_TRANSFORMATION_FLAGS ${default_ara_transformation_flags})

    get_target_property(actual_ara_transformation_flags ${target} HIGGS_ARA_TRANSFORMATION_FLAGS)

    set(ara_transformation_flags_int "")

    foreach(transformation_string IN LISTS actual_ara_transformation_flags)
        list(FIND ara_transformation_flags_strings ${transformation_string} ara_transformation_index)

        if(ara_transformation_index GREATER_EQUAL 0)
            if(ara_transformation_index EQUAL 0)
                set(ara_transformation_bit 0)
            else()
                set(ara_transformation_bit "1 << (${ara_transformation_index} - 1)")
            endif()

            if(ara_transformation_flags_int STREQUAL "")
                set(ara_transformation_flags_int 0)
            endif()

            math(EXPR ara_transformation_flags_int "${ara_transformation_flags_int} | (${ara_transformation_bit})")
        endif()
    endforeach()

    if(NOT ara_transformation_flags_int STREQUAL "")
        set_target_properties(${target} PROPERTIES HIGGS_ARA_TRANSFORMATION_FLAGS ${ara_transformation_flags_int})
    endif()

    _higgs_set_property_if_not_set(${target} IS_ARA_EFFECT FALSE)
    get_target_property(final_bundle_id ${target} HIGGS_BUNDLE_ID)
    _higgs_set_property_if_not_set(${target} ARA_FACTORY_ID "\"${final_bundle_id}.arafactory.${final_version}\"")
    _higgs_set_property_if_not_set(${target} ARA_DOCUMENT_ARCHIVE_ID "\"${final_bundle_id}.aradocumentarchive.1\"")
    _higgs_set_property_if_not_set(${target} ARA_COMPATIBLE_ARCHIVE_IDS "\"\"")
endfunction()

# ==================================================================================================

function(_higgs_initialise_target target)
    set(one_value_args
        VERSION
        BUILD_VERSION
        PRODUCT_NAME
        PLIST_TO_MERGE
        BUNDLE_ID
        MICROPHONE_PERMISSION_ENABLED
        MICROPHONE_PERMISSION_TEXT
        CAMERA_PERMISSION_ENABLED
        CAMERA_PERMISSION_TEXT
        SEND_APPLE_EVENTS_PERMISSION_ENABLED
        SEND_APPLE_EVENTS_PERMISSION_TEXT
        BLUETOOTH_PERMISSION_ENABLED
        BLUETOOTH_PERMISSION_TEXT
        LOCAL_NETWORK_PERMISSION_ENABLED
        LOCAL_NETWORK_PERMISSION_TEXT
        FILE_SHARING_ENABLED            # iOS only
        DOCUMENT_BROWSER_ENABLED        # iOS only
        LAUNCH_STORYBOARD_FILE          # iOS only
        APP_GROUPS_ENABLED              # iOS only
        ICLOUD_PERMISSIONS_ENABLED      # iOS only
        STATUS_BAR_HIDDEN               # iOS only
        BACKGROUND_AUDIO_ENABLED        # iOS only
        BACKGROUND_BLE_ENABLED          # iOS only
        CUSTOM_XCASSETS_FOLDER          # iOS only
        TARGETED_DEVICE_FAMILY          # iOS only
        REQUIRES_FULL_SCREEN            # iOS only
        ICON_BIG
        ICON_SMALL
        ICON_COMPOSER_BUNDLE            # MacOS/iOS only
        COMPANY_COPYRIGHT
        COMPANY_NAME
        COMPANY_WEBSITE
        COMPANY_EMAIL
        NEEDS_CURL                      # Set this true if you want to link curl on Linux
        NEEDS_WEB_BROWSER               # Set this true if you want to link webkit on Linux
        NEEDS_OPENGL_ES                 # Set this true if you want to use OpenGL ES rather than OpenGL on Linux
        NEEDS_WEBVIEW2                  # Set this true if you want to link WebView2 statically on Windows
        NEEDS_STORE_KIT                 # Set this true if you want in-app-purchases on Mac
        NEEDS_WINDOWS_MIDI_SERVICES     # Set this true If you want to support the newest Windows MIDI backend
        PUSH_NOTIFICATIONS_ENABLED
        NETWORK_MULTICAST_ENABLED
        HARDENED_RUNTIME_ENABLED
        APP_SANDBOX_ENABLED
        APP_SANDBOX_INHERIT
        VST3_AUTO_MANIFEST

        PLUGIN_NAME
        PLUGIN_MANUFACTURER_CODE
        PLUGIN_CODE
        DESCRIPTION
        IS_SYNTH
        NEEDS_MIDI_INPUT
        NEEDS_MIDI_OUTPUT
        IS_MIDI_EFFECT
        EDITOR_WANTS_KEYBOARD_FOCUS
        DISABLE_AAX_BYPASS
        DISABLE_AAX_MULTI_MONO
        AAX_IDENTIFIER
        VST_NUM_MIDI_INS
        VST_NUM_MIDI_OUTS
        VST2_CATEGORY
        LV2_PLUGIN_CLASS
        AU_MAIN_TYPE
        AU_EXPORT_PREFIX
        AU_SANDBOX_SAFE
        SUPPRESS_AU_PLIST_RESOURCE_USAGE
        PLUGINHOST_AU                   # Set this true if you want to host AU plugins
        USE_LEGACY_COMPATIBILITY_PLUGIN_CODE
        LV2URI
        IS_ARA_EFFECT
        ARA_FACTORY_ID
        ARA_DOCUMENT_ARCHIVE_ID

        VST_COPY_DIR
        VST3_COPY_DIR
        AAX_COPY_DIR
        AU_COPY_DIR
        UNITY_COPY_DIR
        COPY_PLUGIN_AFTER_BUILD)

    set(multi_value_args
        FORMATS
        VST3_CATEGORIES
        HARDENED_RUNTIME_OPTIONS
        APP_SANDBOX_OPTIONS
        APP_SANDBOX_FILE_ACCESS_HOME_RO
        APP_SANDBOX_FILE_ACCESS_HOME_RW
        APP_SANDBOX_FILE_ACCESS_ABS_RO
        APP_SANDBOX_FILE_ACCESS_ABS_RW
        APP_SANDBOX_EXCEPTION_IOKIT
        DOCUMENT_EXTENSIONS
        AAX_CATEGORY
        IPHONE_SCREEN_ORIENTATIONS      # iOS only
        IPAD_SCREEN_ORIENTATIONS        # iOS only
        APP_GROUP_IDS                   # iOS only
        ARA_COMPATIBLE_ARCHIVE_IDS
        ARA_ANALYSIS_TYPES
        ARA_TRANSFORMATION_FLAGS)

    cmake_parse_arguments(HIGGS_ARG "" "${one_value_args}" "${multi_value_args}" ${ARGN})

    set(base_folder "${CMAKE_CURRENT_BINARY_DIR}/${target}_artefacts")
    set(products_folder "${base_folder}/$<CONFIG>")
    set(higgs_library_code "${base_folder}/HiggsLibraryCode")

    set_target_properties(${target} PROPERTIES HIGGS_GENERATED_SOURCES_DIRECTORY "${higgs_library_code}")

    set_target_properties(${target} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${products_folder}"
        LIBRARY_OUTPUT_DIRECTORY "${products_folder}"
        RUNTIME_OUTPUT_DIRECTORY "${products_folder}")

    if(HIGGS_ARG_ICON_BIG)
        _higgs_make_absolute_and_check(HIGGS_ARG_ICON_BIG)
    endif()

    if(HIGGS_ARG_ICON_SMALL)
        _higgs_make_absolute_and_check(HIGGS_ARG_ICON_SMALL)
    endif()

    set(inherited_properties
        COMPANY_NAME
        COMPANY_WEBSITE
        COMPANY_EMAIL
        COMPANY_COPYRIGHT
        VST_COPY_DIR
        VST3_COPY_DIR
        AU_COPY_DIR
        AAX_COPY_DIR
        UNITY_COPY_DIR
        COPY_PLUGIN_AFTER_BUILD)

    # Overwrite any properties that might be inherited
    foreach(prop_string IN LISTS inherited_properties)
        if(NOT ${HIGGS_ARG_${prop_string}} STREQUAL "")
            set_target_properties(${target} PROPERTIES HIGGS_${prop_string} "${HIGGS_ARG_${prop_string}}")
        endif()
    endforeach()

    # Add each of the function arguments as target properties, so that it's easier to
    # extract them in other functions
    foreach(arg_string IN LISTS one_value_args multi_value_args)
        _higgs_set_property_if_not_set(${target} ${arg_string} "${HIGGS_ARG_${arg_string}}")
    endforeach()

    _higgs_set_fallback_properties(${target})

    target_include_directories(${target} PRIVATE
        $<TARGET_PROPERTY:${target},HIGGS_GENERATED_SOURCES_DIRECTORY>)
    target_link_libraries(${target} PUBLIC $<$<TARGET_EXISTS:higgs_vst2_sdk>:higgs_vst2_sdk>)

    get_target_property(is_pluginhost_au ${target} HIGGS_PLUGINHOST_AU)

    if(is_pluginhost_au)
        target_compile_definitions(${target} PUBLIC HIGGS_PLUGINHOST_AU=1)

        if(CMAKE_SYSTEM_NAME STREQUAL "Darwin" OR CMAKE_SYSTEM_NAME STREQUAL "iOS")
            _higgs_link_frameworks("${target}" PRIVATE CoreAudioKit)
        endif()

        if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
            _higgs_link_frameworks("${target}" PRIVATE AudioUnit)
        endif()
    endif()

    _higgs_write_generate_time_info(${target})
    _higgs_link_optional_libraries(${target})
    _higgs_fixup_module_source_groups()
    _higgs_fixup_unity_property()
endfunction()

# ==================================================================================================

function(higgs_add_console_app target)
    # The _NO_RESOURCERC option is private, and is only intended for use when building higgsaide.
    # We can't add a resources.rc to higgsaide because we need higgsaide to generate the resources.rc!
    cmake_parse_arguments(HIGGS_ARG "_NO_RESOURCERC" "" "" ${ARGN})

    add_executable(${target})
    target_compile_definitions(${target} PRIVATE HIGGS_STANDALONE_APPLICATION=1)

    if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        target_compile_definitions(${target} PRIVATE _CONSOLE=1)
    endif()

    # When building for iOS, these properties will be read in order to populate
    # a plist for the app. We probably don't care whether these values are sane;
    # if we wanted to run on iOS, we'd use higgs_gui_app instead.
    # We clear these explicitly to avoid warnings when configuring with
    # --warn-uninitialized
    set_target_properties(${target} PROPERTIES
        MACOSX_BUNDLE_BUNDLE_NAME           ""
        MACOSX_BUNDLE_BUNDLE_VERSION        ""
        MACOSX_BUNDLE_COPYRIGHT             ""
        MACOSX_BUNDLE_GUI_IDENTIFIER        ""
        MACOSX_BUNDLE_ICON_FILE             ""
        MACOSX_BUNDLE_INFO_STRING           ""
        MACOSX_BUNDLE_LONG_VERSION_STRING   ""
        MACOSX_BUNDLE_SHORT_VERSION_STRING  "")

    _higgs_initialise_target(${target} ${ARGN})

    if(NOT HIGGS_ARG__NO_RESOURCERC)
        set_target_properties(${target} PROPERTIES HIGGS_TARGET_KIND_STRING "ConsoleApp")
        _higgs_write_configure_time_info(${target})
        _higgs_add_resources_rc(${target} ${target})
        _higgs_add_xcode_entitlements(${target} ${target})
    endif()
endfunction()

function(higgs_add_gui_app target)
    if(CMAKE_SYSTEM_NAME STREQUAL "Android")
        add_library(${target} SHARED)
    else()
        add_executable(${target})
    endif()

    target_compile_definitions(${target} PRIVATE HIGGS_STANDALONE_APPLICATION=1)
    _higgs_initialise_target(${target} ${ARGN})
    _higgs_set_output_name(${target} $<TARGET_PROPERTY:${target},HIGGS_PRODUCT_NAME>)
    set_target_properties(${target} PROPERTIES HIGGS_TARGET_KIND_STRING "App")
    _higgs_configure_bundle(${target} ${target})
    _higgs_configure_app_bundle(${target} ${target})
    _higgs_add_resources_rc(${target} ${target})
endfunction()

function(higgs_add_plugin target)
    add_library(${target} STATIC)
    set_target_properties(${target} PROPERTIES HIGGS_IS_PLUGIN TRUE)
    _higgs_initialise_target(${target} ${ARGN})
    _higgs_configure_plugin_targets(${target})
endfunction()

# ==================================================================================================

function(_higgs_target_args_from_plugin_characteristics out_var)
    set(pairs
        "pluginIsSynth\;IS_SYNTH"
        "pluginWantsMidiIn\;NEEDS_MIDI_INPUT"
        "pluginProducesMidiOut\;NEEDS_MIDI_OUTPUT"
        "pluginIsMidiEffectPlugin\;IS_MIDI_EFFECT"
        "pluginEditorRequiresKeys\;EDITOR_WANTS_KEYBOARD_FOCUS")

    set(result)

    foreach(pair IN LISTS pairs)
        list(GET pair 0 old_key)

        if("${old_key}" IN_LIST ARGN)
            list(GET pair 1 new_key)
            list(APPEND result ${new_key} TRUE)
        endif()
    endforeach()

    set(${out_var} ${result} PARENT_SCOPE)
endfunction()

# ==================================================================================================

function(_higgs_get_pip_targets pip out_var)
    set(test_targets "${pip}")

    _higgs_get_all_plugin_kinds(plugin_kinds)

    foreach(plugin_kind IN LISTS plugin_kinds)
        list(APPEND test_targets "${HIGGS_PIP_NAME}_${plugin_kind}")
    endforeach()

    set(${out_var} ${test_targets} PARENT_SCOPE)
endfunction()

function(higgs_add_pip header)
    _higgs_make_absolute(header)

    _higgs_extract_metadata_block(HIGGS_PIP_METADATA "${header}" metadata_dict)

    _higgs_get_metadata("${metadata_dict}" name HIGGS_PIP_NAME)

    if(NOT HIGGS_PIP_NAME)
        message(FATAL_ERROR "Error in '${header}': PIP headers must declare a `name` field")
    endif()

    string(MAKE_C_IDENTIFIER "${HIGGS_PIP_NAME}" pip_name_sanitised)

    if(NOT HIGGS_PIP_NAME STREQUAL pip_name_sanitised)
        message(FATAL_ERROR "Error in '${header}': PIP `name` value '${HIGGS_PIP_NAME}' must be a valid C identifier")
    endif()

    if(TARGET "${HIGGS_PIP_NAME}")
        # We already added a target with this name, let's try using the filename instead
        get_filename_component(HIGGS_PIP_NAME "${header}" NAME_WE)
    endif()

    if(TARGET "${HIGGS_PIP_NAME}")
        message(FATAL_ERROR "Error in '${header}': Could not create a unique target name for PIP ${header}")
    endif()

    _higgs_get_metadata("${metadata_dict}" type pip_kind)

    if(NOT pip_kind)
        message(FATAL_ERROR "Error in '${header}': PIP headers must declare a `type` field")
    endif()

    _higgs_get_metadata("${metadata_dict}" pluginCharacteristics pip_charateristics)

    _higgs_target_args_from_plugin_characteristics(extra_target_args ${pip_charateristics})

    list(APPEND extra_target_args
        NEEDS_CURL TRUE
        NEEDS_WEB_BROWSER TRUE)

    _higgs_get_metadata("${metadata_dict}" moduleFlags pip_moduleflags)

    if("HIGGS_IN_APP_PURCHASES=1" IN_LIST pip_moduleflags)
        list(APPEND extra_target_args
            BUNDLE_ID "com.rmsl.higgsInAppPurchaseSample"
            NEEDS_STORE_KIT TRUE)
    endif()

    if("HIGGS_PLUGINHOST_AU=1" IN_LIST pip_moduleflags)
        list(APPEND extra_target_args PLUGINHOST_AU TRUE)
    endif()

    if("HIGGS_USE_WINDOWS_MIDI_SERVICES=1" IN_LIST pip_moduleflags)
        list(APPEND extra_target_args NEEDS_WINDOWS_MIDI_SERVICES TRUE)
    endif()

    if("HIGGS_USE_WIN_WEBVIEW2_WITH_STATIC_LINKING=1" IN_LIST pip_moduleflags)
        list(APPEND extra_target_args NEEDS_WEBVIEW2 TRUE)
    endif()

    if(pip_kind STREQUAL "AudioProcessor")
        _higgs_get_metadata("${metadata_dict}" documentControllerClass HIGGS_PIP_DOCUMENTCONTROLLER_CLASS)

        if(HIGGS_PIP_DOCUMENTCONTROLLER_CLASS)
            if(NOT TARGET higgs_ara_sdk)
                message(WARNING
                    "${header} specifies a documentControllerClass, but the ARA SDK could not be located. "
                    "Use higgs_set_ara_sdk_path to specify the ARA SDK location. "
                    "This PIP will not be configured.")
            endif()

            set(source_main "${HIGGS_CMAKE_UTILS_DIR}/PIPAudioProcessorWithARA.cpp.in")

            higgs_add_plugin(${HIGGS_PIP_NAME}
                FORMATS AU VST3
                IS_ARA_EFFECT TRUE
                ${extra_target_args})
        else()
            set(source_main "${HIGGS_CMAKE_UTILS_DIR}/PIPAudioProcessor.cpp.in")

            set(formats AAX AU AUv3 LV2 Standalone Unity VST3)

            # We add VST2 targets too, if the user has set up those SDKs
            if(TARGET higgs_vst2_sdk)
                list(APPEND formats VST)
            endif()

            if(CMAKE_SYSTEM_NAME STREQUAL "Windows" AND NOT HIGGS_WINDOWS_HELPERS_CAN_RUN)
                list(REMOVE_ITEM formats LV2)
            endif()

            # Standalone plugins might want to access the mic
            list(APPEND extra_target_args MICROPHONE_PERMISSION_ENABLED TRUE)

            higgs_add_plugin(${HIGGS_PIP_NAME}
                FORMATS ${formats}
                ${extra_target_args})
        endif()
    elseif(pip_kind STREQUAL "Component")
        set(source_main "${HIGGS_CMAKE_UTILS_DIR}/PIPComponent.cpp.in")
        higgs_add_gui_app(${HIGGS_PIP_NAME} ${extra_target_args})
    elseif(pip_kind STREQUAL "Console")
        set(source_main "${HIGGS_CMAKE_UTILS_DIR}/PIPConsole.cpp.in")
        higgs_add_console_app(${HIGGS_PIP_NAME} ${extra_target_args})
    else()
        message(FATAL_ERROR "Error in '${header}': PIP kind must be either AudioProcessor, Component, or Console")
    endif()

    if(NOT ARGV1 STREQUAL "")
        set("${ARGV1}" "${HIGGS_PIP_NAME}" PARENT_SCOPE)
    endif()

    # Generate Main.cpp
    _higgs_get_metadata("${metadata_dict}" mainClass HIGGS_PIP_MAIN_CLASS)
    get_target_property(higgs_library_code ${HIGGS_PIP_NAME} HIGGS_GENERATED_SOURCES_DIRECTORY)
    set(pip_main "${higgs_library_code}/Main.cpp")

    set(HIGGS_PIP_HEADER "${header}")
    configure_file(${source_main} ${pip_main})
    target_sources(${HIGGS_PIP_NAME} PRIVATE ${pip_main})

    _higgs_get_metadata("${metadata_dict}" dependencies pip_dependencies)

    higgs_generate_higgs_header(${HIGGS_PIP_NAME})

    foreach(module IN LISTS pip_dependencies)
        if(module STREQUAL "")
            continue()
        endif()

        set(discovered_module)

        if(TARGET "${module}")
            set(discovered_module "${module}")
        else()
            message(FATAL_ERROR "Error in '${header}': No such module: ${module}")
        endif()

        target_link_libraries(${HIGGS_PIP_NAME} PRIVATE ${discovered_module})
    endforeach()

    target_compile_definitions(${HIGGS_PIP_NAME}
        PRIVATE ${pip_moduleflags}
        PUBLIC
            HIGGS_VST3_CAN_REPLACE_VST2=0)

    _higgs_get_pip_targets(${HIGGS_PIP_NAME} pip_targets)

    # This keeps the PIPs slightly better organised in the HIGGS megaproject
    if((DEFINED HIGGS_SOURCE_DIR) AND (header MATCHES "^${HIGGS_SOURCE_DIR}"))
        file(RELATIVE_PATH folder "${HIGGS_SOURCE_DIR}" "${header}")
        get_filename_component(folder_parent "${folder}" DIRECTORY)

        foreach(target_name IN ITEMS ${pip_targets} ${HIGGS_PIP_NAME}_All)
            if(TARGET "${target_name}")
                set_target_properties("${target_name}" PROPERTIES
                    FOLDER "${folder_parent}/${HIGGS_PIP_NAME}")
            endif()
        endforeach()
    endif()

    # We're building HIGGS itself
    if(DEFINED HIGGS_SOURCE_DIR)
        # PIPs need to know about the resources folder
        target_compile_definitions(${HIGGS_PIP_NAME} PRIVATE
            PIP_HIGGS_EXAMPLES_DIRECTORY_STRING="${HIGGS_SOURCE_DIR}/examples")

        get_filename_component(pip_parent_path "${header}" DIRECTORY)
        target_include_directories(${HIGGS_PIP_NAME} PRIVATE "${pip_parent_path}")

        if((CMAKE_SYSTEM_NAME STREQUAL "iOS") AND (header MATCHES "^${HIGGS_SOURCE_DIR}"))
            foreach(target_name IN LISTS pip_targets)
                if(TARGET "${target_name}")
                    higgs_add_bundle_resources_directory("${target_name}" "${HIGGS_SOURCE_DIR}/examples/Assets")
                endif()
            endforeach()
        endif()

        if((CMAKE_CXX_COMPILER_ID STREQUAL "MSVC") OR (CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC"))
            target_compile_options(${HIGGS_PIP_NAME} PRIVATE /bigobj)
        endif()
    endif()
endfunction()

# ==================================================================================================

function(higgs_set_vst2_sdk_path path)
    if(TARGET higgs_vst2_sdk)
        message(FATAL_ERROR "higgs_set_vst2_sdk_path should only be called once")
    endif()

    _higgs_make_absolute(path)

    if(NOT EXISTS "${path}")
        message(FATAL_ERROR "Could not find VST2 SDK at the specified path: ${path}")
    endif()

    add_library(higgs_vst2_sdk INTERFACE IMPORTED GLOBAL)

    _higgs_disable_system_includes(higgs_vst2_sdk)
    # This is a bit of a hack, but we really need the VST2 paths to always follow the VST3 paths.
    target_include_directories(higgs_vst2_sdk INTERFACE
        $<TARGET_PROPERTY:higgs::higgs_vst3_headers,INTERFACE_INCLUDE_DIRECTORIES>
        "${path}")
endfunction()

function(higgs_set_vst3_sdk_path path)
    if(TARGET higgs_vst3_sdk)
        message(FATAL_ERROR "higgs_set_vst3_sdk_path should only be called once")
    endif()

    _higgs_make_absolute(path)

    if(NOT EXISTS "${path}")
        message(FATAL_ERROR "Could not find VST3 SDK at the specified path: ${path}")
    endif()

    add_library(higgs_vst3_sdk INTERFACE IMPORTED GLOBAL)

    _higgs_disable_system_includes(higgs_vst3_sdk)
    target_include_directories(higgs_vst3_sdk INTERFACE "${path}")
endfunction()

function(higgs_set_ara_sdk_path path)
    if(TARGET higgs_ara_sdk)
        message(FATAL_ERROR "higgs_set_ara_sdk_path should only be called once")
    endif()

    _higgs_make_absolute(path)

    if(NOT EXISTS "${path}")
        message(FATAL_ERROR "Could not find ARA SDK at the specified path: ${path}")
    endif()

    add_library(higgs_ara_sdk INTERFACE IMPORTED GLOBAL)

    _higgs_disable_system_includes(higgs_ara_sdk)
    target_include_directories(higgs_ara_sdk INTERFACE "${path}")
endfunction()

# ==================================================================================================

function(higgs_disable_default_flags)
    set(langs C CXX)
    set(modes DEBUG RELEASE RELWITHDEBINFO MINSIZEREL)

    foreach(lang IN LISTS langs)
        foreach(mode IN LISTS modes)
            list(FILTER CMAKE_${lang}_FLAGS_${mode} INCLUDE REGEX "[/-]M[TD]d?")
            set(CMAKE_${lang}_FLAGS_${mode} "${CMAKE_${lang}_FLAGS_${mode}}" PARENT_SCOPE)
        endforeach()
    endforeach()
endfunction()
