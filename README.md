# Higgs Mythos

Higgs Mythos (abbreviated **HIGGS** throughout the code and the module names) is
an open-source cross-platform C++ application framework for creating desktop and
mobile applications, including VST, VST3, AU, AUv3, AAX and LV2 audio plug-ins
and plug-in hosts. Higgs Mythos can be integrated with existing projects via
CMake, or used through **Mythos Studio**, its project-configuration tool, which
exports projects for Xcode (macOS and iOS), Visual Studio, Android Studio and
Linux Makefiles, and includes a source-code editor.

## Getting Started

The framework lives in the `modules` directory. Every module is named
`higgs_<area>`, its public header is `<module>/<module>.h`, and the C++ API is
declared in the `higgs` namespace:

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
`HIGGS_MAJOR_VERSION`, `HIGGS_MODULE_AVAILABLE_higgs_core`), and the version
numbers are defined in
[higgs_StandardHeader.h](/modules/higgs_core/system/higgs_StandardHeader.h).

### CMake

Version 3.22 or higher is required. To build the bundled examples and tools:

```bash
cmake . -B cmake-build -DHIGGS_BUILD_EXAMPLES=ON -DHIGGS_BUILD_EXTRAS=ON
cmake --build cmake-build --target DemoRunner
```

The CMake API uses the `higgs_` prefix (`higgs_add_gui_app`,
`higgs_add_console_app`, `higgs_add_plugin`, `higgs_add_module`,
`higgs_generate_higgs_header`), and the imported targets are
`higgs::higgs_core`, `higgs::higgs_gui_basics`, and so on.
See [docs/CMake API.md](/docs/CMake%20API.md) for the full reference, and
[examples/CMake](/examples/CMake) for minimal starting points.

### Mythos Studio

Mythos Studio is the project generator shipped in `extras/MythosStudio`. It
reads and writes `.mythos` project files, and its command-line companion is
`higgsaide`. Build it from `extras/MythosStudio/Builds` (Visual Studio, Xcode or
Linux Makefile projects) or through CMake with `-DHIGGS_BUILD_EXTRAS=ON`.

### Spanish Documentation

A complete user guide in Spanish (getting started, guides for every area of the
framework, the CMake API, tools and a reference page for each module) is
published at <https://jalexiscv.github.io/higgs-mythos/>. Its sources live in
[documentacion](/documentacion).

### API Documentation

The API documentation is generated directly from the Doxygen comments in the
module headers, and the source code is the most complete and up-to-date
documentation available.

- The main header of each module, `higgs_<module>/higgs_<module>.h`, lists the
  module's public headers and its dependencies. The module format is described
  in [docs/Higgs Mythos Module Format.md](/docs/Higgs%20Mythos%20Module%20Format.md).
- To find a class, search the headers for its declaration and read the comment
  block above it:

  ```bash
  grep -rlE "^\s*(class|struct)\s+(HIGGS_API\s+)?AudioProcessorValueTreeState\b" modules
  ```

- [BREAKING_CHANGES.md](BREAKING_CHANGES.md) describes, per release, what
  changed and how to update code.
- [examples](/examples) contains complete, buildable demonstrations of most
  features.

## Minimum System Requirements

### Building HIGGS Projects

- __C++ Standard__: 17 (20 when building with `HIGGS_USE_WINDOWS_MIDI_SERVICES` enabled)
- __macOS/iOS__: Xcode 12.4 (Intel macOS 10.15.4, Apple Silicon macOS 11.0)
- __Windows__: Visual Studio 2019 (Windows 10 version 1607)
- __Linux__: g++ 7.0 or Clang 6.0 (see [docs/Linux Dependencies.md](/docs/Linux%20Dependencies.md))
- __Android__: Android Studio Narwhal 3 (2025.1.3) with Android Gradle plugin 8.13.2
  and NDK 28 on Windows, macOS or Linux

### Deployment Targets

- __macOS__: macOS 10.11 (x86_64, Arm64)
- __Windows__: Windows 10 version 1607 (x86_64, x86, Arm64, Arm64EC)
- __Linux__: mainstream distributions (x86_64, Arm64/aarch64)
- __iOS__: iOS 12 (Arm64, Arm64e, x86_64 Simulator)
- __Android__: Android 7 - Nougat (API Level 24) (arm64-v8a, armeabi-v7a, x86_64, x86)

## Contributing

See [.github/contributing.md](.github/contributing.md).

## Licensing

See [LICENSE.md](LICENSE.md). Higgs Mythos is released under the
**AGPL-3.0-or-later**; the copyright notices of the original authors are
preserved in the source files, and the examples are under the ISC licence.

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
information about signing your plug-ins.
