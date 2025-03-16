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

function(_higgs_create_atomic_target target_name)
    add_library("${target_name}" INTERFACE)
    add_library("higgs::${target_name}" ALIAS "${target_name}")

    if(NOT ((CMAKE_SYSTEM_NAME STREQUAL "Linux") OR (CMAKE_SYSTEM_NAME MATCHES ".*BSD")))
        return()
    endif()

    set(test_atomic_with_is_lock_free_file_contents
            [[
            #include <atomic>

            int main (int argc, char** argv)
            {
                std::atomic<long long> ll { static_cast<long long> (argc) };
                ll ^= static_cast<long long> (ll.is_lock_free());
                return static_cast<int> (ll);
            }
        ]])

    set(test_simple_atomic_file_contents
        [[
            #include <atomic>

            int main (int argc, char** argv)
            {
                std::atomic<long long> ll { static_cast<long long> (argc) };
                ll ^= 1;
                return static_cast<int> (ll);
            }
        ]])

    string(RANDOM LENGTH 16 random_file_string)
    set(test_file_name "${CMAKE_CURRENT_BINARY_DIR}/check_atomic_${random_file_string}.cpp")

    string(RANDOM LENGTH 16 random_dir_string)
    set(test_bindir "${CMAKE_CURRENT_BINARY_DIR}/check_atomic_dir_${random_dir_string}")

    file(WRITE "${test_file_name}" "${test_atomic_with_is_lock_free_file_contents}")

    try_compile(compile_result "${test_bindir}" "${test_file_name}"
        OUTPUT_VARIABLE test_build_output_0
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED TRUE
        CXX_EXTENSIONS FALSE)

    if(NOT compile_result)
        try_compile(compile_result "${test_bindir}" "${test_file_name}"
            OUTPUT_VARIABLE test_build_output_1
            LINK_LIBRARIES atomic
            CXX_STANDARD 17
            CXX_STANDARD_REQUIRED TRUE
            CXX_EXTENSIONS FALSE)

        if (NOT compile_result)
            file(WRITE "${test_file_name}" "${test_simple_atomic_file_contents}")

            try_compile(compile_result "${test_bindir}" "${test_file_name}"
                    OUTPUT_VARIABLE test_build_output_2
                    LINK_LIBRARIES atomic
                    CXX_STANDARD 17
                    CXX_STANDARD_REQUIRED TRUE
                    CXX_EXTENSIONS FALSE)

            if (NOT compile_result)
                message(FATAL_ERROR
                    "First build output:\n"
                    "${test_build_output_0}"
                    "\n\nSecond build output:\n"
                    "${test_build_output_1}"
                    "\n\nThird build output:\n"
                    "${test_build_output_2}"
                    "\n\nHIGGS requires support for std::atomic, but this system cannot "
                    "successfully compile a program which uses std::atomic. "
                    "You may need to install a dedicated libatomic package using your "
                    "system's package manager.")
            else()
                message(WARNING
                    "First build output:\n"
                    "${test_build_output_0}"
                    "\n\nSecond build output:\n"
                    "${test_build_output_1}"
                    "\n\nIf you are seeing this warning it means that the libatomic library"
                    "on this system doesn't support is_lock_free."
                    "Please let the HIGGS team know.")
            endif()
        endif()

        target_link_libraries("${target_name}" INTERFACE atomic)
    endif()

    file(REMOVE "${test_file_name}")
    file(REMOVE_RECURSE "${test_bindir}")
endfunction()

_higgs_create_atomic_target(higgs_atomic_wrapper)
