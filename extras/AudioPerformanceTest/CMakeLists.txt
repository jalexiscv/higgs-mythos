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

higgs_add_gui_app(AudioPerformanceTest BUNDLE_ID com.higgs.AudioPerformanceTest
    NEEDS_CURL FALSE
    NEEDS_WEB_BROWSER FALSE)

higgs_generate_higgs_header(AudioPerformanceTest)

target_sources(AudioPerformanceTest PRIVATE
    Source/Main.cpp)

target_compile_definitions(AudioPerformanceTest PRIVATE
    HIGGS_USE_CURL=0
    HIGGS_WEB_BROWSER=0)

target_link_libraries(AudioPerformanceTest PRIVATE
    higgs::higgs_audio_utils
    higgs::higgs_recommended_config_flags
    higgs::higgs_recommended_lto_flags
    higgs::higgs_recommended_warning_flags)
