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

higgs_add_console_app(BinaryBuilder
    NEEDS_CURL FALSE)

higgs_generate_higgs_header(BinaryBuilder)

target_sources(BinaryBuilder PRIVATE Source/Main.cpp)

target_compile_definitions(BinaryBuilder PRIVATE
    HIGGS_USE_CURL=0
    # This is a temporary workaround to allow builds to complete on Xcode 15.
    # Add -Wl,-ld_classic to the OTHER_LDFLAGS build setting if you need to
    # deploy to older versions of macOS/iOS.
    HIGGS_SILENCE_XCODE_15_LINKER_WARNING=1)

target_link_libraries(BinaryBuilder PRIVATE
    higgs::higgs_core
    higgs::higgs_recommended_config_flags
    higgs::higgs_recommended_lto_flags
    higgs::higgs_recommended_warning_flags)
