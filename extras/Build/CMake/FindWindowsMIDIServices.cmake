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

include(FindPackageHandleStandardArgs)

if(HIGGS_WINDOWS_MIDI_SERVICES_PACKAGE_LOCATION)
    set(initial_search_dir ${HIGGS_WINDOWS_MIDI_SERVICES_PACKAGE_LOCATION})
else()
    set(initial_search_dir "$ENV{USERPROFILE}/AppData/Local/PackageManagement/NuGet/Packages")
endif()

file(GLOB subdirs "${initial_search_dir}/Windows.Devices.Midi2.*")

if(subdirs)
    list(GET subdirs 0 search_dir)
    list(LENGTH subdirs num_midi2_packages)

    if(num_midi2_packages GREATER 1)
        message(WARNING "Multiple Windows MIDI Services packages found in the local NuGet folder. Proceeding with ${search_dir}.")
    endif()

    find_path(WindowsMIDIServices_root_dir ref/native/Windows.Devices.Midi2.winmd HINTS ${search_dir})
    set(WindowsMIDIServices_winmd "${WindowsMIDIServices_root_dir}/ref/native/Windows.Devices.Midi2.winmd")
elseif(NOT WindowsMIDIServices_FIND_QUIETLY)
    message(WARNING
            "Windows MIDI Services wasn't found in the the local NuGet folder."
            "\n"
            "TODO(reuk) At the moment the WMS package isn't in the public nuget repo so it needs to be installed from a local source.")
endif()

find_package_handle_standard_args(WindowsMIDIServices DEFAULT_MSG WindowsMIDIServices_winmd)

if(WindowsMIDIServices_FOUND)
    set(WindowsMIDIServices_INCLUDE_DIRS ${WindowsMIDIServices_include_dirs})

    mark_as_advanced(WindowsMIDIServices_include_dirs WindowsMIDIServices_root_dir)

    if(NOT TARGET higgs_windows_midi_services)
        get_target_property(cppwinrt_command jcppwinrt IMPORTED_LOCATION)
        execute_process(COMMAND "${cppwinrt_command}"
                -reference sdk
                -input "${WindowsMIDIServices_winmd}"
                -output "${CMAKE_BINARY_DIR}/winmidi_projection"
            OUTPUT_VARIABLE command_output
            ERROR_VARIABLE command_output
            RESULT_VARIABLE result_variable)

        if(result_variable)
            message(FATAL_ERROR "Failed to run cppwinrt to generate Windows MIDI Services headers\n${command_output}")
        endif()

        add_library(higgs_windows_midi_services INTERFACE)
        add_library(higgs::higgs_windows_midi_services ALIAS higgs_windows_midi_services)
        target_link_libraries(higgs_windows_midi_services INTERFACE higgs_winrt_headers)
        target_include_directories(higgs_windows_midi_services INTERFACE
            "${CMAKE_BINARY_DIR}/winmidi_projection"
            "${WindowsMIDIServices_root_dir}/build/native/include")
        target_compile_definitions(higgs_windows_midi_services INTERFACE HIGGS_USE_WINDOWS_MIDI_SERVICES=1)
    endif()
endif()
