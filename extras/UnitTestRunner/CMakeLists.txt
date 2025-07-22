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

higgs_add_console_app(UnitTestRunner
    NEEDS_WEB_BROWSER FALSE
    NEEDS_CURL FALSE)

higgs_generate_higgs_header(UnitTestRunner)

target_sources(UnitTestRunner PRIVATE Source/Main.cpp)

target_compile_definitions(UnitTestRunner PRIVATE
    HIGGS_PLUGINHOST_AU=1
    HIGGS_PLUGINHOST_LV2=1
    HIGGS_PLUGINHOST_VST3=1
    HIGGS_UNIT_TESTS=1
    HIGGS_USE_CURL=0
    HIGGS_WEB_BROWSER=0
    # This is a temporary workaround to allow builds to complete on Xcode 15.
    # Add -Wl,-ld_classic to the OTHER_LDFLAGS build setting if you need to
    # deploy to older versions of macOS.
    HIGGS_SILENCE_XCODE_15_LINKER_WARNING=1)

target_link_libraries(UnitTestRunner PRIVATE
    higgs::higgs_analytics
    higgs::higgs_audio_utils
    higgs::higgs_build_tools
    higgs::higgs_dsp
    higgs::higgs_midi_ci
    higgs::higgs_opengl
    higgs::higgs_osc
    higgs::higgs_product_unlocking
    higgs::higgs_recommended_config_flags
    higgs::higgs_recommended_lto_flags
    higgs::higgs_recommended_warning_flags)
