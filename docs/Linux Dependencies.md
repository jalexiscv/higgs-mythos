# HIGGS Dependencies on Linux

Below is a list of the current dependencies required to build HIGGS projects on
Ubuntu, separated by module. Where the dependency is optional, the preprocessor
flag used to disable it is noted.

This has been tested on Ubuntu 16.04 LTS (Xenial Xerus), 18.04 LTS (Bionic
Beaver), and 20.04 LTS (Focal Fossa). Packages may differ in name or not be
available on other distributions.

## Compiler
A C++ compiler is required. HIGGS has been tested thoroughly with Clang and GCC:

    sudo apt update
    sudo apt install clang

or

    sudo apt update
    sudo apt install g++

## Packages

#### higgs_audio_devices
- libasound2-dev
- libjack-jackd2-dev (unless `HIGGS_JACK=0`)

#### higgs_audio_processors
- ladspa-sdk (unless `HIGGS_PLUGINHOST_LADSPA=0`)

#### higgs_core
- libcurl4-openssl-dev (unless `HIGGS_USE_CURL=0`)

#### higgs_graphics
- libfontconfig1-dev (unless `HIGGS_USE_FONTCONFIG=0`)
- libfreetype-dev (unless `HIGGS_USE_FREETYPE=0`)

These packages are available on Ubuntu 22 and 24. If libfreetype-dev is not
available you could try installing the libfreetype6-dev package.

#### higgs_gui_basics
- libx11-dev
- libxcomposite-dev
- libxcursor-dev (unless `HIGGS_USE_XCURSOR=0`)
- libxext-dev
- libxinerama-dev (unless `HIGGS_USE_XINERAMA=0`)
- libxrandr-dev (unless `HIGGS_USE_XRANDR=0`)
- libxrender-dev (unless `HIGGS_USE_XRENDER=0`)
- libxi-dev (unless `HIGGS_USE_XINPUT=0`)

#### higgs_gui_extra
- libwebkit2gtk-4.1-dev (unless `HIGGS_WEB_BROWSER=0`)

On older systems, where 4.1 is not available, you can also use

- libwebkit2gtk-4.0-dev

Compiled HIGGS applications will dynamically load whichever library version is
available during runtime.

#### higgs_opengl
- libegl-dev
- libglu1-mesa-dev and mesa-common-dev (unless `HIGGS_OPENGL_ES=1`)
- libgles-dev (if `HIGGS_OPENGL_ES=1`)

The full command is as follows:

    sudo apt update
    sudo apt install libasound2-dev libjack-jackd2-dev \
        ladspa-sdk \
        libcurl4-openssl-dev  \
        libfreetype-dev libfontconfig1-dev \
        libx11-dev libxcomposite-dev libxcursor-dev libxext-dev libxinerama-dev libxrandr-dev libxrender-dev libxi-dev \
        libwebkit2gtk-4.1-dev \
        libegl-dev \
        libglu1-mesa-dev mesa-common-dev
