# Higgs Mythos

**Language:** [Español](README.md) ·
English ·
[Português](README.pt.md) ·
[Français](README.fr.md) ·
[Deutsch](README.de.md) ·
[Italiano](README.it.md) ·
[中文](README.zh-CN.md) ·
[日本語](README.ja.md)

```text
                                  +@@@@@@@@@@-
                             .@@@@@@@@@@@@@@@@@@@@.
               .@@@@@@    :@@@@@@@@@@@@@@@@@@@@@@@@@@-    @@@@@@-
              %@%.%@@@@  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  @@@@@.+@@
                   @@@@ :@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@= @@@@
                  @@@@. @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ .@@@@
     @@@@@@@@@  #@@@@  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  @@@@%  @@@@@@@@@
   *@@@@%%@@@@@@=@@   #@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@   @@-@@@@@@%%@@@@=
   @@       .@@@@@    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@.   @@@@@        @@
  =@     @@@@@:@@@@  %@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  @@@@=@@@@%     @-
   @  .@@@@@=  @@@@   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@   @@@@  -@@@@@   @
     @@@@@*    *@@@     @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@.    @@@#    -@@@@@:
   @@@@@@      @@@@      +@@@@@@@@@@@@@@@@@@@@@@@@@@@@%      @@@@      @@@@@@
  @@@@@.       @@@. +@   : @@@@@@@@@@@@@@@@@@@@@@@@@@ .   @* .@@@.      .@@@@@
 @@@@@        @@@@:@@@-   @%.@@@@@@@@@@@@@@@@@@@@@@-%@   .@@@:@@@@        @@@@@
#@@@@-       @@@@:@@@@@@@ @@@ @@@@@@@@@@@@@@@@@@@@ @@@ @@@@@@@-@@@@       =@@@@#
@@@@@      *@@@@ @@@@@@@@+@@@@+ @@@@@@@@@@@@@@@@ +@@@@+@@@@@@@@ @@@@*      @@@@@
@@@@@     @@@@#  *@@@@@@@@@@@  . @@@@@@@@@@@@@@ .  @@@@@@@@@@@*  #@@@@     @@@@@
+@@@@+  .@@@@     @@@@@@@@@@@*     @@@@@@@@@@     #@@@@@@@@@@@     @@@@.  +@@@@+
 @@@@@#*@@@@       @@:    @@@@@@-   @@@@@@@@   .@@@@@@    -@@       @@@@+*@@@@@
  @@@@@@@.+              *@@@@@@@@@* :@@@@: :@@@@@@@@@#              * @@@@@@@
    @@@@@@@@@@@@@+==+@@@@@@@@@@@@@@@@*    :@@@@@@@@@@@@@@@@+::-+@@@@@@@@@@@@
    @:.@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#  *@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@.+@
   @@@@@    .%@@@@@@@@@@@#=-=#@@@@@@@@@  @@@@@@@@@@@%@@@@@@@@@@@@@*.    @@@@@
   @@@@       ++       :@@@@@@@@#%@@@@@:.@@@@@%*@@@@@@@@#       =+       @@@@.
  @@@@@    @@@@@@@@* @@@@@@@@.+@@@@@@@@++@@@@%@@@+.@@@@@@@@ *@@@@@@@@    @@@@@
  @@@@@   @@    .@+@@@@@@%    @@@@.@@@@@@@@@@ @@@@    %@@@@@@=#.    @@   @@@@@
  =@@@@    =    -@@@@@@       @@@@#@@@@@@@@@@*@@@@       @@@@@@*         @@@@=
   @@@@@       @@@@@@ =      @@@@@@@@@@@@@@@@@@@@@@      . @@@@@@       @@@@@
    @@@@@@   =@@@@@ @@@    @@@@@@  @@@@#+@@@@  @@@@@@    @@@ @@@@@*   @@@@@@
     @@@@@@@@# -@# @@@@%@@@@@@@:   @@@@::@@@@   -@@@@@@@%@@@@ *@# *@@@@@@@@
       @@@@@@@@@@@@@@@@@@@@@@      @@@@  @@@@      @@@@@@@@@@@@@@@@@@@@@@.
          %@@@@@@@@@@@@@=         @@@@@  @@@@@         +@@@@@@@@@@@@@%
           %@@@@    @@@@   @@@@@@ @@@@*  =@@@@ @@@@@@   @@@@    @@@@@
            @@@@@.  #+ *@@@@@@@@ @@@@@=  +@@@@@ #@@@@@@@@ -#  .@@@@@.
            :@@@@@@@@@@@@@@-    @@@@@ @@@@ @@@@@    -@@@@@@@@@@@@@@-
              *@@@@@@@@@.-@  %@@@@@@  @@@@  @@@@@@#  @:.@@@@@@@@@+
                      @@@@@@@@@@@@-  :@..@:  -@@@@@@@@@@@@
                       .@@@@@@@@    @@    @@    %@@@@@@@:
```

*The Higgs Mythos mascot.*

Higgs Mythos (abbreviated **HIGGS** throughout the code and the module names) is
an open-source, cross-platform C++ application framework for creating desktop
and mobile applications, including VST, VST3, AU, AUv3, AAX and LV2 audio
plug-ins and the hosts that load them. The same source code builds for macOS,
Windows, Linux, iOS and Android without changing APIs: the framework wraps each
operating system's windowing, 2D drawing, audio and MIDI behind common classes,
and a project only describes what it needs to build.

The framework integrates into existing projects through CMake, or is used from
**Mythos Studio**, its project-configuration tool, which exports projects for
Xcode (macOS and iOS), Visual Studio, Android Studio and Linux Makefiles and
includes a source-code editor. It is designed both for general-purpose GUI
applications, such as productivity tools or system utilities, and for
professional audio software that demands real-time processing, strict thread
management and compatibility with the plug-in formats used by digital audio
workstations.

Higgs Mythos is distributed under the **AGPL-3.0-or-later** licence. The
copyright notices of the original authors are preserved in every source file,
and the examples are published under the ISC licence so that they can be copied
as the starting point of your own project without additional restrictions.

## Getting Started

The framework lives in the `modules` directory. Every module is named
`higgs_<area>`, its public header is `<module>/<module>.h`, and the whole C++
API is declared in the `higgs` namespace. A module is a self-contained unit: its
main header declares which other modules it depends on, which public headers it
exposes and which system libraries it needs on each platform, so the build
system can resolve the minimal set of sources that must be compiled for a given
project.

To get started, include the header of the module you need and derive from the
corresponding class. The following example creates a GUI component with an
initial size of 800 by 600 pixels; `Component` is the base class of every
visual element in the framework, from a complete window down to a button:

```cpp
#include <higgs_core/higgs_core.h>
#include <higgs_gui_basics/higgs_gui_basics.h>

class MainComponent final : public higgs::Component
{
public:
    MainComponent() { setSize (800, 600); }
};
```

Configuration macros keep the `HIGGS_` prefix (for example
`HIGGS_MAJOR_VERSION` or `HIGGS_MODULE_AVAILABLE_higgs_core`), and the version
numbers are defined in
[higgs_StandardHeader.h](/modules/higgs_core/system/higgs_StandardHeader.h).
The macros `HIGGS_WINDOWS`, `HIGGS_MAC`, `HIGGS_LINUX`, `HIGGS_IOS` and
`HIGGS_ANDROID` identify the target platform when a project needs
system-specific code. The remaining naming conventions (namespace, CMake
functions, the `.mythos` project extension) are collected in
[docs/higgs-mythos.md](/docs/higgs-mythos.md).

## Modules

Higgs Mythos is organised into twenty-four independent modules that are combined
according to what each project needs. A console application may use only
`higgs_core`; a GUI application adds `higgs_events`, `higgs_graphics` and
`higgs_gui_basics`; an audio plug-in adds the audio modules and
`higgs_audio_plugin_client`. Each module is compiled only when it is linked, so
the final binary does not carry code from areas it never uses. The following
table summarises the purpose of each one; the full documentation of every module
is on the documentation site and in the Doxygen headers.

The modules are grouped in layers. The base is `higgs_core`, on which every
other module depends; `higgs_events` and `higgs_data_structures` sit on top of
it, need no graphical interface, and therefore also serve console tools and
background processes. The graphics layer (`higgs_graphics`,
`higgs_gui_basics`, `higgs_gui_extra`, `higgs_animation`, `higgs_opengl`)
builds the user interface, and the audio layer goes from the data
(`higgs_audio_basics`) up to the devices, the file formats, the processors and
the plug-in formats. Each module declares its dependencies in its main header,
and Mythos Studio or CMake resolve them automatically when the module is added
to a project.

| Module | Purpose |
|---|---|
| `higgs_core` | Core classes: strings, containers, memory, threads, files, networking, XML, JSON, ZIP, time and unit tests. |
| `higgs_events` | Message loop, timers, asynchronous messages, broadcasters and listeners, application base class and IPC. |
| `higgs_data_structures` | `ValueTree`, `Value`, `UndoManager` and preferences management. |
| `higgs_cryptography` | RSA, Blowfish, prime numbers and the MD5, SHA-256 and Whirlpool digests. |
| `higgs_graphics` | 2D engine: `Graphics`, geometry, colours, fonts, images, SVG and effects. |
| `higgs_gui_basics` | `Component`, windows, widgets, menus, layouts, look-and-feel and accessibility. |
| `higgs_gui_extra` | Code editor, `WebBrowserComponent`, colour selector, system tray and notifications. |
| `higgs_animation` | Declarative animation with easing functions synchronised to the display. |
| `higgs_opengl` | OpenGL and OpenGL ES rendering inside components. |
| `higgs_video` | Video playback and camera capture. |
| `higgs_audio_basics` | `AudioBuffer`, audio sources, synthesiser, MIDI 1.0, MPE and MIDI 2.0 (UMP). |
| `higgs_audio_devices` | Audio and MIDI I/O with CoreAudio, WASAPI, ASIO, ALSA, JACK, Oboe and OpenSL ES. |
| `higgs_audio_formats` | Reading and writing WAV, AIFF, FLAC, Ogg Vorbis, Opus, MP3 and more. |
| `higgs_audio_processors_headless` | `AudioProcessor`, parameters, `AudioProcessorGraph` and headless plug-in hosting. |
| `higgs_audio_processors` | Processor editors, `AudioProcessorValueTreeState` and plug-in scanning. |
| `higgs_audio_plugin_client` | VST2, VST3, AU, AUv3, AAX, LV2, Unity and Standalone wrappers. |
| `higgs_audio_utils` | `AudioAppComponent`, device selectors, audio thumbnails and MIDI keyboards. |
| `higgs_dsp` | Filters, oscillators, dynamics, reverb, convolution, FFT and SIMD. |
| `higgs_midi_ci` | MIDI Capability Inquiry: discovery, profiles and properties. |
| `higgs_osc` | Open Sound Control over UDP. |
| `higgs_javascript` | JavaScript interpreter based on QuickJS. |
| `higgs_box2d` | The Box2D 2D physics engine with an integrated renderer. |
| `higgs_analytics` | Collection and batched upload of usage analytics. |
| `higgs_product_unlocking` | In-app purchases and licensing with online unlocking. |

## CMake

CMake 3.22 or higher is required. The most direct way to check that the
environment is ready is to build the tools and examples bundled with the
repository: the first command configures the build tree in the `cmake-build`
directory, enabling the examples and the extras, and the second builds
`DemoRunner`, the application that gathers every demonstration of the
framework:

```bash
cmake . -B cmake-build -DHIGGS_BUILD_EXAMPLES=ON -DHIGGS_BUILD_EXTRAS=ON
cmake --build cmake-build --target DemoRunner
```

On Windows the Visual Studio environment (`vcvars64.bat`) must be loaded first
so that CMake finds the MSVC compiler, with Ninja as well as with MSBuild. On
Linux the development packages listed in
[docs/Linux Dependencies.md](/docs/Linux%20Dependencies.md) must be installed
before configuring.

The CMake API uses the `higgs_` prefix: `higgs_add_gui_app` creates a GUI
application, `higgs_add_console_app` a console one, `higgs_add_plugin` a
plug-in with one target per format, `higgs_add_module` registers your own
module and `higgs_generate_higgs_header` generates the project's aggregate
header. There are also `higgs_add_binary_data` to embed resources and
`higgs_set_<kind>_sdk_path` to point at the VST2, AAX or ARA SDKs. The imported
targets are named `higgs::higgs_core`, `higgs::higgs_gui_basics` and so on, and
are linked with `target_link_libraries` like any other library. To integrate
the framework into an existing project, call `add_subdirectory` on a copy of the
repository or `find_package(HIGGS)` on an installation. The full reference is in
[docs/CMake API.md](/docs/CMake%20API.md), and [examples/CMake](/examples/CMake)
contains three minimal projects (`GuiApp`, `ConsoleApp` and `AudioPlugin`)
meant to be copied as starting points.

## Mythos Studio

Mythos Studio is the project generator shipped in `extras/MythosStudio`. It
stores a project's configuration in a `.mythos` file and, from it, exports
native projects for every IDE and platform: Xcode for macOS and iOS, Visual
Studio 2019, 2022 and 2026, Linux Makefiles and Android Studio with Gradle. A
single `.mythos` file can have several exporters at once, each with its own
Debug and Release configurations, SDK paths and compiler and linker options, so
the project is kept in one place and the IDE projects are regenerated whenever
the modules or the source files change.

The tool includes a start page with application and plug-in templates, a module
manager that resolves dependencies, a code editor with syntax highlighting and a
command-line interface (`--resave`, `--set-version`, `--encode-binary`,
`--create-project-from-pip`, among others) to automate tasks without opening
the window. Its companion `higgsaide` generates from the terminal the binary
data and other resources that projects embed. Android is supported only through
Mythos Studio, since the CMake API does not generate projects for that
platform.

Mythos Studio is built from `extras/MythosStudio/Builds` (Visual Studio, Xcode
or Linux Makefile projects) or through CMake with `-DHIGGS_BUILD_EXTRAS=ON` and
the `MythosStudio` target. Unsigned prebuilt executables are also available for
Windows x64, universal macOS and Linux x64, produced by the
`HIGGS Mythos Studio Build` GitHub Actions workflow and downloadable as
artifacts of each run.

## Tools and Examples

Besides Mythos Studio, the `extras` directory gathers the tools that accompany
the framework and are built with `-DHIGGS_BUILD_EXTRAS=ON`. `AudioPluginHost` is
a complete plug-in host that scans the formats installed on the system,
connects them in a processor graph and lets you test your own plug-in without
depending on a commercial workstation. `UnitTestRunner` runs the unit tests that
each module declares with the `UnitTest` class; it can filter by category or by
name and is the project's verification reference. `BinaryBuilder` turns files
into embedded data, `AudioPerformanceTest` measures real-time audio processing
performance, `NetworkGraphicsDemo` demonstrates distributed drawing across
several machines, and `SBOM` generates and checks the inventory of third-party
dependencies.

The `examples` directory contains complete, buildable demonstrations of most
features, organised by area: `GUI`, `Audio`, `DSP`, `Plugins`, `Utilities` and
`Assets`. All of them are PIPs: complete projects contained in a single header
file that starts with a metadata block (name, dependencies, type and main
class), which `DemoRunner` loads and shows next to its source code, so you can
read the implementation of each demo while it runs. `examples/CMake` contains
the three minimal projects mentioned above, and `examples/Plugins` includes
complete plug-ins (synthesisers, a sampler, an arpeggiator, a noise gate, a
host inside a plug-in and an ARA example) that serve as a reference for the
full life cycle of an `AudioProcessor`.

## Documentation

The complete user guide in Spanish is published at
<https://jalexiscv.github.io/higgs-mythos/> and its sources live in
[documentacion](/documentacion). It covers getting started (installation, first
application and first plug-in), the framework concepts (architecture, message
loop, threads, memory and conventions), one guide per area (GUI, graphics,
audio, MIDI, DSP, plug-ins, hosting, OpenGL, video, web, accessibility,
localisation), a reference page per module, the CMake API, the tools and the
project notes (breaking changes, licence, security and glossary). The site is
built with MkDocs and deployed automatically to GitHub Pages whenever the
documentation, the modules or the examples change on the `main` branch; the
reference pages are regenerated from the source tree on each deployment.

The API documentation is generated directly from the Doxygen comments in the
module headers, and the source code is the most complete and up-to-date
documentation available. The main header of each module,
`higgs_<module>/higgs_<module>.h`, lists its public headers and its
dependencies; the module format is described in
[docs/Higgs Mythos Module Format.md](/docs/Higgs%20Mythos%20Module%20Format.md).
To find a class, search the headers for its declaration and read the comment
block above it:

```bash
grep -rlE "^\s*(class|struct)\s+(HIGGS_API\s+)?AudioProcessorValueTreeState\b" modules
```

[BREAKING_CHANGES.md](BREAKING_CHANGES.md) describes, release by release, what
changed and how to update code; [CHANGE_LIST.md](CHANGE_LIST.md) summarises the
headlines of each release, and [docs/README.md](/docs/README.md) is the index of
the repository's technical documentation (module format, CMake API, framework
identity and working protocols).

## Minimum System Requirements

Higgs Mythos requires C++17 as the minimum standard because its public headers
use features of that standard (each module declares it in the
`minimumCppStandard` field of its main header); when
`HIGGS_USE_WINDOWS_MIDI_SERVICES` is enabled the project must be built with
C++20, since the Windows MIDI Services SDK requires it. The compiler versions
listed below are the oldest ones that are tested: newer versions of Xcode,
Visual Studio, GCC and Clang work without changes. On Linux, besides the
compiler, the development libraries for X11, FreeType, ALSA and others listed in
[docs/Linux Dependencies.md](/docs/Linux%20Dependencies.md) are needed; on
Android the project is built with Gradle and the NDK from Android Studio,
through the projects that Mythos Studio exports. The two lists below
distinguish the development machine from the machine where the result runs:
the first says what you build with, the second where the output executes. For
example, a Windows Arm64 plug-in is built from Visual Studio on any compatible
Windows machine, and iOS and Android applications are built from macOS or from
a desktop running Android Studio, respectively.

### Building HIGGS Projects

This list gathers, per development platform, the minimum tool with which the
framework builds: the IDE or compiler and the operating-system version of the
development machine. On macOS and iOS, Xcode is used, which bundles Clang and
the Apple SDKs; the minimum system version depends on whether the machine is
Intel or Apple Silicon, because Xcode 12.4 requires different systems on each
architecture. On Windows, Visual Studio is used with the C++ desktop workload
and the Windows 10 SDK, from the IDE as well as from the command line after
loading `vcvars64.bat`. On Linux any modern GCC or Clang works together with
the system's development libraries, and the Makefiles exported by Mythos
Studio or the CMake projects are built from the terminal. On Android, Android
Studio, the Gradle plugin and the NDK are required, and they build the projects
that Mythos Studio exports; the host machine can be Windows, macOS or Linux.
The versions listed are tested minimums, not recommendations: the current
versions of each tool work without changes.

- __C++ Standard__: 17 (20 when building with `HIGGS_USE_WINDOWS_MIDI_SERVICES` enabled)
- __macOS/iOS__: Xcode 12.4 (Intel macOS 10.15.4, Apple Silicon macOS 11.0)
- __Windows__: Visual Studio 2019 (Windows 10 version 1607)
- __Linux__: g++ 7.0 or Clang 6.0 (see [docs/Linux Dependencies.md](/docs/Linux%20Dependencies.md))
- __Android__: Android Studio Narwhal 3 (2025.1.3) with Android Gradle plugin 8.13.2
  and NDK 28 on Windows, macOS or Linux

### Deployment Targets

The values below are the minimum operating-system versions on which the
applications and plug-ins built with the framework run, and the processor
architectures for which binaries are produced. On macOS and Windows it is common
to produce universal binaries or several architectures from the same project;
on Android, Mythos Studio generates the selected ABIs in a single package.
Choosing a higher minimum version for a particular project (for example macOS
11 instead of 10.11) is possible from the exporter settings in Mythos Studio
or with the usual CMake variables, and allows newer system APIs to be used;
going below the values in the list is not supported, because the framework
uses APIs that do not exist in earlier versions. On iOS, the x86_64 simulator
allows testing on Intel Macs, and physical devices use Arm64 or Arm64e. On
Android it is advisable to include at least arm64-v8a, the architecture of
every current device, and to add the others only when the target audience
justifies it, because each additional ABI increases the package size.

- __macOS__: macOS 10.11 (x86_64, Arm64)
- __Windows__: Windows 10 version 1607 (x86_64, x86, Arm64, Arm64EC)
- __Linux__: mainstream distributions (x86_64, Arm64/aarch64)
- __iOS__: iOS 12 (Arm64, Arm64e, x86_64 Simulator)
- __Android__: Android 7 - Nougat (API Level 24) (arm64-v8a, armeabi-v7a, x86_64, x86)

## Contributing

Contributions are received through GitHub. Feature requests and general
questions belong in the repository's *Discussions* section; reproducible bugs
are filed as issues stating the Higgs Mythos version, the affected modules, the
platform and a minimal reproduction, using the templates in
`.github/ISSUE_TEMPLATE` whenever possible. Security vulnerabilities are never
posted in an issue: they are reported privately through the channel described
in [SECURITY.md](SECURITY.md).

Each pull request must focus on a single change and describe what it fixes or
adds. Before opening it, build the affected targets and run the unit tests with
`UnitTestRunner`. Code, comments and commit messages are written in English
with the `area: summary` format. By submitting a contribution you agree to
license it under the same terms as the project (AGPL-3.0-or-later) and confirm
that you wrote the code or have the right to submit it; code copied from
proprietary binaries or from sources whose licence is incompatible with the AGPL
is not accepted. The full guide is in
[.github/contributing.md](.github/contributing.md), and
[CODE_OF_CONDUCT.md](CODE_OF_CONDUCT.md) sets the community's rules of
conduct.

## Licensing

Higgs Mythos is free software: you can redistribute it and modify it under the
terms of the GNU Affero General Public License as published by the Free Software
Foundation, either version 3 or, at your option, any later version
(**AGPL-3.0-or-later**). The full text and the notes on how it applies are in
[LICENSE.md](LICENSE.md). The AGPL is a *copyleft* licence: any derived work that
is distributed, or offered to users over a network, must be published under the
same licence with its source code available. This includes the applications and
plug-ins that link the framework.

Higgs Mythos derives from an earlier open-source code base whose brand was
removed entirely from the names, the modules, the macros and the resources. The
copyright notices of the original authors are preserved intact in the header
of every source file, as the licence requires, and must not be edited or
removed. The examples in `examples` are published under the far more permissive
ISC licence so that they can be copied as the basis of a new project. The
inventory of third-party dependencies and their licences is in
[HiggsMythos.spdx.json](HiggsMythos.spdx.json), in SPDX format, and the plug-in
SDKs (VST3, AAX, ARA, LV2) are used under their own licences and agreements
without being included in the repository when those do not allow it.

## AAX Plug-Ins

AAX plug-ins need to be digitally signed using PACE Anti-Piracy's signing tools
before they will run in commercially available versions of Pro Tools. These
tools are provided free of charge by Avid. Before obtaining the signing tools,
you will need to use a special build of Pro Tools, called Pro Tools Developer,
to test your unsigned plug-ins. The steps to obtain Pro Tools Developer are:

1. Sign up as an AAX Developer at <https://developer.avid.com/aax/>.
2. Request a Pro Tools Developer Bundle activation code by emailing
   <devauth@avid.com>.
3. Download the latest Pro Tools Developer build from your Avid Developer
   account.

When your plug-ins have been tested and debugged in Pro Tools Developer, and you
are ready to digitally sign them, email <audiosdk@avid.com> with the subject
"PACE Eden Signing Tools Request". Include an overview of each plug-in along
with a screen recording showing the plug-in running in Pro Tools Developer, with
audio if possible. Please also include the following information:

- Company name
- Admin full name
- Telephone number

Once the request is submitted, PACE Anti-Piracy will contact you directly with
information about signing your plug-ins. The AAX target of `higgs_add_plugin`
produces the `.aaxplugin` bundle ready for signing; the AAX SDK path is set with
`higgs_set_aax_sdk_path` before declaring the plug-in.
