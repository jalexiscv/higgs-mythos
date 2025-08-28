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

higgs_add_gui_app(AudioPluginHost
    BUNDLE_ID                       com.higgs.pluginhost
    ICON_BIG                        "${CMAKE_CURRENT_SOURCE_DIR}/Source/HIGGSAppIcon.png"
    ICON_COMPOSER_BUNDLE            "${CMAKE_CURRENT_SOURCE_DIR}/Source/AppIcon.icon"
    MICROPHONE_PERMISSION_ENABLED   TRUE
    PLUGINHOST_AU                   TRUE
    NEEDS_WEB_BROWSER               FALSE
    NEEDS_CURL                      FALSE)

higgs_generate_higgs_header(AudioPluginHost)

target_sources(AudioPluginHost PRIVATE
    Source/HostStartup.cpp
    Source/Plugins/ARAPlugin.cpp
    Source/Plugins/IOConfigurationWindow.cpp
    Source/Plugins/InternalPlugins.cpp
    Source/Plugins/PluginGraph.cpp
    Source/UI/GraphEditorPanel.cpp
    Source/UI/MainHostWindow.cpp)

higgs_add_binary_data(AudioPluginHostData SOURCES
    ../../examples/Assets/cassette_recorder.wav
    ../../examples/Assets/cello.wav
    ../../examples/Assets/guitar_amp.wav
    ../../examples/Assets/proaudio.path
    ../../examples/Assets/reverb_ir.wav
    ../../examples/Assets/singing.ogg)

target_compile_definitions(AudioPluginHost PRIVATE
    HIGGS_ALSA=1
    HIGGS_ASIO=1
    HIGGS_CONTENT_SHARING=1
    HIGGS_DIRECTSOUND=1
    HIGGS_DISABLE_CAUTIOUS_PARAMETER_ID_CHECKING=1
    HIGGS_PLUGINHOST_LADSPA=1
    HIGGS_PLUGINHOST_LV2=1
    HIGGS_PLUGINHOST_VST3=1
    HIGGS_PLUGINHOST_VST=0
    HIGGS_PLUGINHOST_ARA=0
    HIGGS_USE_CAMERA=0
    HIGGS_USE_CDBURNER=0
    HIGGS_USE_CDREADER=0
    HIGGS_USE_CURL=0
    HIGGS_USE_FLAC=0
    HIGGS_USE_OGGVORBIS=1
    HIGGS_VST3_HOST_CROSS_PLATFORM_UID=1
    HIGGS_WASAPI=1
    HIGGS_WEB_BROWSER=0
    PIP_HIGGS_EXAMPLES_DIRECTORY_STRING="${HIGGS_SOURCE_DIR}/examples"
    # This is a temporary workaround to allow builds to complete on Xcode 15.
    # Add -Wl,-ld_classic to the OTHER_LDFLAGS build setting if you need to
    # deploy to older versions of macOS/iOS.
    HIGGS_SILENCE_XCODE_15_LINKER_WARNING=1)

target_link_libraries(AudioPluginHost PRIVATE
    AudioPluginHostData
    higgs::higgs_audio_utils
    higgs::higgs_cryptography
    higgs::higgs_dsp
    higgs::higgs_opengl
    higgs::higgs_recommended_config_flags
    higgs::higgs_recommended_lto_flags
    higgs::higgs_recommended_warning_flags)

higgs_add_bundle_resources_directory(AudioPluginHost ../../examples/Assets)
