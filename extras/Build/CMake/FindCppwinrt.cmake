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

if(HIGGS_CPPWINRT_PACKAGE_LOCATION)
    set(initial_search_dir ${HIGGS_CPPWINRT_PACKAGE_LOCATION})
else()
    set(initial_search_dir "$ENV{USERPROFILE}/AppData/Local/PackageManagement/NuGet/Packages")
endif()

file(GLOB subdirs "${initial_search_dir}/Microsoft.Windows.CppWinRT.*")

if(subdirs)
    list(GET subdirs 0 search_dir)
    list(LENGTH subdirs num_midi2_packages)

    if(num_midi2_packages GREATER 1)
        message(WARNING "Multiple Windows MIDI Services packages found in the local NuGet folder. Proceeding with ${search_dir}.")
    endif()

    find_path(cppwinrt_root_dir bin/cppwinrt.exe HINTS ${search_dir})
    set(cppwinrt_exe "${cppwinrt_root_dir}/bin/cppwinrt.exe")
elseif(NOT cppwinrt_FIND_QUIETLY)
    message(WARNING "Cppwinrt wasn't found in the local packages folder.\n"
            "\n"
            "In order to use cppwinrt functionality, you must install this package."
            "To install NuGet and the CppWinRT package containing the statically linked library, "
            "open a PowerShell and issue the following commands"
            "\n"
            "> Register-PackageSource -provider NuGet -name nugetRepository -location https://www.nuget.org/api/v2\n"
            "> Install-Package Microsoft.Windows.CppWinRT -Scope CurrentUser -Source nugetRepository\n"
            "\n"
            "Alternatively you can use the HIGGS_CPPWINRT_PACKAGE_LOCATION CMake variable to specify the directory "
            "where this find script is looking for the *Microsoft.Windows.CppWinRT* package directory.")
endif()

find_package_handle_standard_args(CppWinRT DEFAULT_MSG cppwinrt_exe)

if(CppWinRT_FOUND)
    mark_as_advanced(cppwinrt_root_dir cppwinrt_exe)

    if(NOT TARGET jcppwinrt)
        add_executable(jcppwinrt IMPORTED GLOBAL)
        set_target_properties(jcppwinrt PROPERTIES IMPORTED_LOCATION "${cppwinrt_exe}")
    endif()

    if(NOT TARGET higgs_winrt_headers)
        get_target_property(cppwinrt_command jcppwinrt IMPORTED_LOCATION)
        set(cppwinrt_INCLUDE_DIRS "${CMAKE_BINARY_DIR}/cppwinrt_projection")

        execute_process(COMMAND "${cppwinrt_command}" -input sdk -output "${cppwinrt_INCLUDE_DIRS}"
            OUTPUT_VARIABLE command_output
            ERROR_VARIABLE command_output
            RESULT_VARIABLE result_variable)

        if(result_variable)
            message(FATAL_ERROR "Failed to run CppWinRT to generate Windows SDK headers\n${command_output}")
        endif()

        add_library(higgs_winrt_headers INTERFACE)
        add_library(higgs::higgs_winrt_headers ALIAS higgs_winrt_headers)
        target_link_libraries(higgs_winrt_headers INTERFACE runtimeobject)
        target_include_directories(higgs_winrt_headers INTERFACE ${cppwinrt_INCLUDE_DIRS})
    endif()
endif()
