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

# The higgsaide program generates intermediate build files including BinaryData sources, icons, and
# plists. To ensure that we always build it for the host system, and not for, say, a device or
# simulator if we're targeting iOS or Android, we reinvoke cmake here and build higgsaide during the
# configuration stage of the outer project.

if(HIGGS_BUILD_HELPER_TOOLS)
    # Build the tool for the current system
    higgs_add_console_app(higgsaide _NO_RESOURCERC
        NEEDS_CURL FALSE)

    target_sources(higgsaide PRIVATE Main.cpp)

    target_compile_definitions(higgsaide PRIVATE
        HIGGS_DISABLE_HIGGS_VERSION_PRINTING=1
        HIGGS_USE_CURL=0
        # This is a temporary workaround to allow builds to complete on Xcode 15.
        # Add -Wl,-ld_classic to the OTHER_LDFLAGS build setting if you need to
        # deploy to older versions of macOS.
        HIGGS_SILENCE_XCODE_15_LINKER_WARNING=1)

    target_link_libraries(higgsaide PRIVATE
        higgs::higgs_build_tools
        higgs::higgs_recommended_config_flags
        higgs::higgs_recommended_lto_flags
        higgs::higgs_recommended_warning_flags)

    set_target_properties(higgsaide PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded")

    export(TARGETS higgsaide
           NAMESPACE higgs_tools::
           FILE "${HIGGS_BINARY_DIR}/HIGGSToolsExport.cmake")
else()
    message(STATUS "Configuring higgsaide")

    if(CMAKE_CROSSCOMPILING)
        unset(ENV{ADDR2LINE})
        unset(ENV{AR})
        unset(ENV{ASM})
        unset(ENV{AS})
        unset(ENV{CC})
        unset(ENV{CPP})
        unset(ENV{CXXFILT})
        unset(ENV{CXX})
        unset(ENV{DLLTOOL})
        unset(ENV{DLLWRAP})
        unset(ENV{ELFEDIT})
        unset(ENV{GCC})
        unset(ENV{GCOV_DUMP})
        unset(ENV{GCOV_TOOL})
        unset(ENV{GCOV})
        unset(ENV{GPROF})
        unset(ENV{GXX})
        unset(ENV{LDFLAGS})
        unset(ENV{LD_BFD})
        unset(ENV{LD})
        unset(ENV{LTO_DUMP})
        unset(ENV{NM})
        unset(ENV{OBJCOPY})
        unset(ENV{OBJDUMP})
        unset(ENV{PKG_CONFIG_LIBDIR})
        unset(ENV{PKG_CONFIG})
        unset(ENV{RANLIB})
        unset(ENV{RC})
        unset(ENV{READELF})
        unset(ENV{SIZE})
        unset(ENV{STRINGS})
        unset(ENV{STRIP})
        unset(ENV{WIDL})
        unset(ENV{WINDMC})
        unset(ENV{WINDRES})

        if(DEFINED ENV{PATH_ORIG})
            set(ENV{PATH} "$ENV{PATH_ORIG}")
        endif()
    endif()

    # Generator platform is only supported on specific generators
    if(CMAKE_GENERATOR MATCHES "^Visual Studio.*$")
        set(ENV{CMAKE_GENERATOR_PLATFORM} "${CMAKE_HOST_SYSTEM_PROCESSOR}")
    endif()

    set(PASSTHROUGH_ARGS "")

    if(CMAKE_OSX_DEPLOYMENT_TARGET AND NOT CMAKE_CROSSCOMPILING)
        list(APPEND PASSTHROUGH_ARGS "-DCMAKE_OSX_DEPLOYMENT_TARGET=${CMAKE_OSX_DEPLOYMENT_TARGET}")
    endif()

    if(CMAKE_C_COMPILER_LAUNCHER)
        list(APPEND PASSTHROUGH_ARGS "-DCMAKE_C_COMPILER_LAUNCHER=${CMAKE_C_COMPILER_LAUNCHER}")
    endif()

    if(CMAKE_CXX_COMPILER_LAUNCHER)
        list(APPEND PASSTHROUGH_ARGS "-DCMAKE_CXX_COMPILER_LAUNCHER=${CMAKE_CXX_COMPILER_LAUNCHER}")
    endif()

    # Looks like we're bootstrapping, reinvoke CMake
    execute_process(COMMAND "${CMAKE_COMMAND}"
            "."
            "-B${HIGGS_BINARY_DIR}/tools"
            "-G${CMAKE_GENERATOR}"
            "-DCMAKE_MAKE_PROGRAM=${CMAKE_MAKE_PROGRAM}"
            "-DCMAKE_CONFIGURATION_TYPES=Custom"
            "-DCMAKE_BUILD_TYPE=Custom"
            "-DHIGGS_BUILD_HELPER_TOOLS=ON"
            "-DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}"
            ${PASSTHROUGH_ARGS}
        WORKING_DIRECTORY "${HIGGS_SOURCE_DIR}"
        OUTPUT_VARIABLE command_output
        ERROR_VARIABLE command_output
        RESULT_VARIABLE result_variable)

    if(result_variable)
        message(FATAL_ERROR "Failed to configure higgsaide\n${command_output}")
    endif()

    message(STATUS "Building higgsaide")

    execute_process(COMMAND "${CMAKE_COMMAND}"
            --build "${HIGGS_BINARY_DIR}/tools"
            --config Custom
        OUTPUT_VARIABLE command_output
        ERROR_VARIABLE command_output
        RESULT_VARIABLE result_variable)

    if(result_variable)
        message(FATAL_ERROR "Failed to build higgsaide\n${command_output}")
    endif()

    message(STATUS "Exporting higgsaide")

    # This will be generated by the recursive invocation of CMake (above)
    include("${HIGGS_BINARY_DIR}/tools/HIGGSToolsExport.cmake")

    add_executable(higgsaide IMPORTED GLOBAL)
    get_target_property(imported_location higgs_tools::higgsaide IMPORTED_LOCATION_CUSTOM)
    set_target_properties(higgsaide PROPERTIES IMPORTED_LOCATION "${imported_location}")

    add_executable(higgs::higgsaide ALIAS higgsaide)

    set(HIGGS_TOOL_INSTALL_DIR "bin/HIGGS-${HIGGS_VERSION}" CACHE STRING
        "The location, relative to the install prefix, where higgsaide will be installed")

    install(PROGRAMS "${imported_location}" DESTINATION "${HIGGS_TOOL_INSTALL_DIR}")

    get_filename_component(binary_name "${imported_location}" NAME)
    set(HIGGS_HIGGSAIDE_NAME "${binary_name}" CACHE INTERNAL "The name of the higgsaide program")

    message(STATUS "Testing higgsaide")

    execute_process(COMMAND "${imported_location}" version
        RESULT_VARIABLE result_variable
        OUTPUT_VARIABLE output
        ERROR_VARIABLE output)

    if(result_variable)
        message(FATAL_ERROR "Testing higgsaide failed:\noutput: ${output}")
    endif()

    message(STATUS "Finished setting up higgsaide")
endif()
