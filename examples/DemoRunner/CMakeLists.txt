# ==============================================================================
#
#  This file is part of the HIGGS framework examples.
#  Copyright (c) Raw Material Software Limited
#
#  The code included in this file is provided under the terms of the ISC license
#  http://www.isc.org/downloads/software-support-policy/isc-license. Permission
#  to use, copy, modify, and/or distribute this software for any purpose with or
#  without fee is hereby granted provided that the above copyright notice and
#  this permission notice appear in all copies.
#
#  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
#  REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
#  AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
#  INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
#  LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
#  OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
#  PERFORMANCE OF THIS SOFTWARE.
#
# ==============================================================================

higgs_add_gui_app(DemoRunner
    BUNDLE_ID                       com.higgs.demorunner
    ICON_BIG                        ${CMAKE_CURRENT_SOURCE_DIR}/Source/HIGGSAppIcon.png
    ICON_COMPOSER_BUNDLE            "${CMAKE_CURRENT_SOURCE_DIR}/Source/AppIcon.icon"
    NEEDS_CURL                      TRUE
    NEEDS_WEB_BROWSER               TRUE
    MICROPHONE_PERMISSION_ENABLED   TRUE
    CAMERA_PERMISSION_ENABLED       TRUE
    BLUETOOTH_PERMISSION_ENABLED    TRUE
    FILE_SHARING_ENABLED            TRUE
    DOCUMENT_BROWSER_ENABLED        TRUE
    REQUIRES_FULL_SCREEN            FALSE
    IPAD_SCREEN_ORIENTATIONS        UIInterfaceOrientationPortrait
                                    UIInterfaceOrientationPortraitUpsideDown
                                    UIInterfaceOrientationLandscapeLeft
                                    UIInterfaceOrientationLandscapeRight
    IPHONE_SCREEN_ORIENTATIONS      UIInterfaceOrientationPortrait
                                    UIInterfaceOrientationPortraitUpsideDown
                                    UIInterfaceOrientationLandscapeLeft
                                    UIInterfaceOrientationLandscapeRight)

higgs_generate_higgs_header(DemoRunner)

target_sources(DemoRunner PRIVATE
    Source/Demos/DemoPIPs1.cpp
    Source/Demos/DemoPIPs2.cpp
    Source/Demos/HIGGSDemos.cpp
    Source/Main.cpp
    Source/UI/DemoContentComponent.cpp
    Source/UI/MainComponent.cpp)

target_compile_definitions(DemoRunner PRIVATE
    PIP_HIGGS_EXAMPLES_DIRECTORY_STRING="${HIGGS_SOURCE_DIR}/examples"
    HIGGS_ASIO=1
    HIGGS_ALLOW_STATIC_NULL_VARIABLES=0
    HIGGS_CONTENT_SHARING=1
    HIGGS_DEMO_RUNNER=1
    HIGGS_PLUGINHOST_AU=1
    HIGGS_PLUGINHOST_LV2=1
    HIGGS_PLUGINHOST_VST3=1
    HIGGS_PUSH_NOTIFICATIONS=1
    HIGGS_STRICT_REFCOUNTEDPOINTER=1
    HIGGS_UNIT_TESTS=1
    HIGGS_USE_CAMERA=1
    HIGGS_USE_CURL=1
    HIGGS_USE_MP3AUDIOFORMAT=1
    HIGGS_WEB_BROWSER=1
    # This is a temporary workaround to allow builds to complete on Xcode 15.
    # Add -Wl,-ld_classic to the OTHER_LDFLAGS build setting if you need to
    # deploy to older versions of macOS/iOS.
    HIGGS_SILENCE_XCODE_15_LINKER_WARNING=1)

target_link_libraries(DemoRunner PRIVATE
    higgs::higgs_analytics
    higgs::higgs_animation
    higgs::higgs_audio_utils
    higgs::higgs_box2d
    higgs::higgs_build_tools
    higgs::higgs_dsp
    higgs::higgs_javascript
    higgs::higgs_midi_ci
    higgs::higgs_opengl
    higgs::higgs_osc
    higgs::higgs_product_unlocking
    higgs::higgs_video
    higgs::higgs_recommended_config_flags
    higgs::higgs_recommended_lto_flags
    higgs::higgs_recommended_warning_flags)

foreach(folder IN ITEMS ../Assets ../Audio ../DSP ../GUI ../Utilities)
    higgs_add_bundle_resources_directory(DemoRunner ${folder})
endforeach()
