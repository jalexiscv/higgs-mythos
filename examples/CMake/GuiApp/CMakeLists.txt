# Example GUI App CMakeLists.txt

# To get started on a new GUI app, copy this entire folder (containing this file and C++ sources) to
# a convenient location, and then start making modifications. For other examples of CMakeLists for
# GUI apps, check `extras/MythosStudio` and `examples/DemoRunner` in the HIGGS repo.

# The first line of any CMake project should be a call to `cmake_minimum_required`, which checks
# that the installed CMake will be able to understand the following CMakeLists, and ensures that
# CMake's behaviour is compatible with the named version. This is a standard CMake command, so more
# information can be found in the CMake docs.

cmake_minimum_required(VERSION 3.22)

# The top-level CMakeLists.txt file for a project must contain a literal, direct call to the
# `project()` command. `project()` sets up some helpful variables that describe source/binary
# directories, and the current project version. This is a standard CMake command.

project(GUI_APP_EXAMPLE VERSION 0.0.1)

# If you've installed HIGGS somehow (via a package manager, or directly using the CMake install
# target), you'll need to tell this project that it depends on the installed copy of HIGGS. If you've
# included HIGGS directly in your source tree (perhaps as a submodule), you'll need to tell CMake to
# include that subdirectory as part of the build.

# find_package(HIGGS CONFIG REQUIRED)        # If you've installed HIGGS to your system
# or
# add_subdirectory(HIGGS)                    # If you've put HIGGS in a subdirectory called HIGGS

# If your app depends the VST2 SDK, perhaps to host VST2 plugins, CMake needs to be told where
# to find the SDK on your system. This setup should be done before calling `higgs_add_gui_app`.

# higgs_set_vst2_sdk_path(...)

# `higgs_add_gui_app` adds an executable target with the name passed as the first argument
# (GuiAppExample here). This target is a normal CMake target, but has a lot of extra properties set
# up by default. This function accepts many optional arguments. Check the readme at
# `docs/CMake API.md` in the HIGGS repo for the full list.

higgs_add_gui_app(GuiAppExample
    # VERSION ...                       # Set this if the app version is different to the project version
    # ICON_BIG ...                      # ICON_* arguments specify a path to an image file to use as an icon
    # ICON_SMALL ...
    # DOCUMENT_EXTENSIONS ...           # Specify file extensions that should be associated with this app
    # COMPANY_NAME ...                  # Specify the name of the app's author
    PRODUCT_NAME "Gui App Example"      # The name of the final executable, which can differ from the target name
    NEEDS_WEB_BROWSER FALSE             # Set to TRUE if you need to use webviews
    NEEDS_CURL FALSE)                   # Set to TRUE if you need to use curl

# `higgs_generate_higgs_header` will create a HiggsHeader.h for a given target, which will be generated
# into your build tree. This should be included with `#include <HiggsHeader.h>`. The include path for
# this header will be automatically added to the target. The main function of the HiggsHeader is to
# include all your HIGGS module headers; if you're happy to include module headers directly, you
# probably don't need to call this.

# higgs_generate_higgs_header(GuiAppExample)

# `target_sources` adds source files to a target. We pass the target that needs the sources as the
# first argument, then a visibility parameter for the sources which should normally be PRIVATE.
# Finally, we supply a list of source files that will be built into the target. This is a standard
# CMake command.

target_sources(GuiAppExample
    PRIVATE
        Main.cpp
        MainComponent.cpp)

# `target_compile_definitions` adds some preprocessor definitions to our target. In a MythosStudio
# project, these might be passed in the 'Preprocessor Definitions' field. HIGGS modules also make use
# of compile definitions to switch certain features on/off, so if there's a particular feature you
# need that's not on by default, check the module header for the correct flag to set here. These
# definitions will be visible both to your code, and also the HIGGS module code, so for new
# definitions, pick unique names that are unlikely to collide! This is a standard CMake command.

target_compile_definitions(GuiAppExample
    PRIVATE
        # HIGGS_WEB_BROWSER and HIGGS_USE_CURL would be on by default, but you might not need them.
        HIGGS_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `higgs_add_gui_app` call
        HIGGS_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `higgs_add_gui_app` call
        HIGGS_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:GuiAppExample,HIGGS_PRODUCT_NAME>"
        HIGGS_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:GuiAppExample,HIGGS_VERSION>")

# If your target needs extra binary assets, you can add them here. The first argument is the name of
# a new static library target that will include all the binary resources. There is an optional
# `NAMESPACE` argument that can specify the namespace of the generated binary data class. Finally,
# the SOURCES argument should be followed by a list of source files that should be built into the
# static library. These source files can be of any kind (wav data, images, fonts, icons etc.).
# Conversion to binary-data will happen when your target is built.

# higgs_add_binary_data(GuiAppData SOURCES ...)

# `target_link_libraries` links libraries and HIGGS modules to other libraries or executables. Here,
# we're linking our executable target to the `higgs::higgs_gui_extra` module. Inter-module
# dependencies are resolved automatically, so `higgs_core`, `higgs_events` and so on will also be
# linked automatically. If we'd generated a binary data target above, we would need to link to it
# here too. This is a standard CMake command.

target_link_libraries(GuiAppExample
    PRIVATE
        # GuiAppData            # If we'd created a binary data target, we'd link to it here
        higgs::higgs_gui_extra
    PUBLIC
        higgs::higgs_recommended_config_flags
        higgs::higgs_recommended_lto_flags
        higgs::higgs_recommended_warning_flags)
