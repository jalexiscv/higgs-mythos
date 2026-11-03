# Higgs Mythos

**语言：** [Español](README.md) ·
[English](README.en.md) ·
[Português](README.pt.md) ·
[Français](README.fr.md) ·
[Deutsch](README.de.md) ·
[Italiano](README.it.md) ·
中文 ·
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

*Higgs Mythos 的吉祥物。*

Higgs Mythos（在代码和模块名称中缩写为 **HIGGS**）是一个开源的跨平台 C++
应用程序框架，用于创建桌面和移动应用程序，包括 VST、VST3、AU、AUv3、AAX 和 LV2
格式的音频插件以及加载它们的宿主程序。同一份源代码可以在不更改 API 的情况下为
macOS、Windows、Linux、iOS 和 Android 编译：框架将每个操作系统的窗口系统、2D
绘图、音频和 MIDI 封装在通用的类之后，项目只需描述它需要构建的内容。

该框架可以通过 CMake 集成到现有项目中，也可以通过其项目配置工具 **Mythos
Studio** 使用。Mythos Studio 可以导出 Xcode（macOS 和 iOS）、Visual Studio、
Android Studio 和 Linux Makefile 项目，并内置源代码编辑器。它既适用于带图形界面
的通用应用程序，例如生产力工具或系统实用程序，也适用于要求实时处理、严格线程
管理以及与数字音频工作站所用插件格式兼容的专业音频软件。

Higgs Mythos 以 **AGPL-3.0-or-later** 许可证发布。原作者的版权声明保留在每个
源文件中，示例则以 ISC 许可证发布，以便可以不受额外限制地复制它们作为自己
项目的起点。

## 入门

框架位于 `modules` 目录中。每个模块命名为 `higgs_<领域>`，其公共头文件为
`<模块>/<模块>.h`，整个 C++ API 都声明在 `higgs` 命名空间中。模块是一个自包含
的单元：它在主头文件中声明依赖哪些其他模块、公开哪些头文件以及在每个平台上
需要哪些系统库，因此构建系统能够为给定项目确定必须编译的最小源文件集合。

入门时只需包含所需模块的头文件并从相应的类派生即可。下面的示例创建了一个
初始尺寸为 800 × 600 像素的图形界面组件；`Component` 是框架中所有可视元素的
基类，从完整的窗口到一个按钮都继承自它：

```cpp
#include <higgs_core/higgs_core.h>
#include <higgs_gui_basics/higgs_gui_basics.h>

class MainComponent final : public higgs::Component
{
public:
    MainComponent() { setSize (800, 600); }
};
```

配置宏保留 `HIGGS_` 前缀（例如 `HIGGS_MAJOR_VERSION` 或
`HIGGS_MODULE_AVAILABLE_higgs_core`），版本号定义在
[higgs_StandardHeader.h](/modules/higgs_core/system/higgs_StandardHeader.h)
中。当项目需要特定系统的代码时，宏 `HIGGS_WINDOWS`、`HIGGS_MAC`、
`HIGGS_LINUX`、`HIGGS_IOS` 和 `HIGGS_ANDROID` 用于标识目标平台。其余命名约定
（命名空间、CMake 函数、`.mythos` 项目扩展名）汇总在
[docs/higgs-mythos.md](/docs/higgs-mythos.md) 中。

## 模块

Higgs Mythos 由二十四个相互独立的模块组成，可根据每个项目的需要进行组合。
控制台应用程序可能只使用 `higgs_core`；图形应用程序加入 `higgs_events`、
`higgs_graphics` 和 `higgs_gui_basics`；音频插件再加上音频模块和
`higgs_audio_plugin_client`。每个模块只有在被链接时才会编译，因此最终的
二进制文件不会携带它从不使用的领域的代码。下表概括了每个模块的用途；每个模块
的完整文档位于文档站点和 Doxygen 头文件中。

模块按层次分组。基础是 `higgs_core`，其他所有模块都依赖它；`higgs_events` 和
`higgs_data_structures` 建立在它之上，不需要图形界面，因此也适用于控制台工具
和后台进程。图形层（`higgs_graphics`、`higgs_gui_basics`、`higgs_gui_extra`、
`higgs_animation`、`higgs_opengl`）构建用户界面，音频层则从数据
（`higgs_audio_basics`）一直延伸到设备、文件格式、处理器和插件格式。每个模块
在其主头文件中声明自己的依赖项，当把模块添加到项目中时，Mythos Studio 或
CMake 会自动解析这些依赖。

| 模块 | 用途 |
|---|---|
| `higgs_core` | 基础类：字符串、容器、内存、线程、文件、网络、XML、JSON、ZIP、时间和单元测试。 |
| `higgs_events` | 消息循环、定时器、异步消息、广播器和监听器、应用程序基类和进程间通信。 |
| `higgs_data_structures` | `ValueTree`、`Value`、`UndoManager` 和首选项管理。 |
| `higgs_cryptography` | RSA、Blowfish、素数以及 MD5、SHA-256 和 Whirlpool 摘要。 |
| `higgs_graphics` | 2D 引擎：`Graphics`、几何、颜色、字体、图像、SVG 和特效。 |
| `higgs_gui_basics` | `Component`、窗口、控件、菜单、布局、外观风格和无障碍功能。 |
| `higgs_gui_extra` | 代码编辑器、`WebBrowserComponent`、颜色选择器、系统托盘和通知。 |
| `higgs_animation` | 与屏幕刷新同步、带缓动函数的声明式动画。 |
| `higgs_opengl` | 在组件内进行 OpenGL 和 OpenGL ES 渲染。 |
| `higgs_video` | 视频播放和摄像头采集。 |
| `higgs_audio_basics` | `AudioBuffer`、音频源、合成器、MIDI 1.0、MPE 和 MIDI 2.0（UMP）。 |
| `higgs_audio_devices` | 通过 CoreAudio、WASAPI、ASIO、ALSA、JACK、Oboe 和 OpenSL ES 进行音频与 MIDI 输入输出。 |
| `higgs_audio_formats` | 读写 WAV、AIFF、FLAC、Ogg Vorbis、Opus、MP3 等格式。 |
| `higgs_audio_processors_headless` | `AudioProcessor`、参数、`AudioProcessorGraph` 和无界面插件宿主。 |
| `higgs_audio_processors` | 处理器编辑器、`AudioProcessorValueTreeState` 和插件扫描。 |
| `higgs_audio_plugin_client` | VST2、VST3、AU、AUv3、AAX、LV2、Unity 和独立应用的封装。 |
| `higgs_audio_utils` | `AudioAppComponent`、设备选择器、音频缩略图和 MIDI 键盘。 |
| `higgs_dsp` | 滤波器、振荡器、动态处理、混响、卷积、FFT 和 SIMD。 |
| `higgs_midi_ci` | MIDI Capability Inquiry：设备发现、配置文件和属性。 |
| `higgs_osc` | 基于 UDP 的 Open Sound Control。 |
| `higgs_javascript` | 基于 QuickJS 的 JavaScript 解释器。 |
| `higgs_box2d` | 带集成渲染器的 Box2D 2D 物理引擎。 |
| `higgs_analytics` | 使用统计的收集与批量上传。 |
| `higgs_product_unlocking` | 应用内购买以及支持在线解锁的许可系统。 |

## CMake

需要 CMake 3.22 或更高版本。检查环境是否就绪的最直接方法是编译仓库中附带的
工具和示例：第一条命令在 `cmake-build` 目录中配置构建树并启用示例和附加工具，
第二条命令编译 `DemoRunner`，即汇集框架所有演示的应用程序：

```bash
cmake . -B cmake-build -DHIGGS_BUILD_EXAMPLES=ON -DHIGGS_BUILD_EXTRAS=ON
cmake --build cmake-build --target DemoRunner
```

在 Windows 上，无论使用 Ninja 还是 MSBuild，都必须先加载 Visual Studio 环境
（`vcvars64.bat`），CMake 才能找到 MSVC 编译器。在 Linux 上，配置之前需要先
安装 [docs/Linux Dependencies.md](/docs/Linux%20Dependencies.md) 中列出的
开发包。

CMake API 使用 `higgs_` 前缀：`higgs_add_gui_app` 创建图形应用程序，
`higgs_add_console_app` 创建控制台应用程序，`higgs_add_plugin` 创建每种格式
对应一个目标的插件，`higgs_add_module` 注册自定义模块，
`higgs_generate_higgs_header` 生成项目的聚合头文件。此外还有用于嵌入资源的
`higgs_add_binary_data`，以及用于指定 VST2、AAX 或 ARA SDK 路径的
`higgs_set_<kind>_sdk_path`。导入的目标命名为 `higgs::higgs_core`、
`higgs::higgs_gui_basics` 等，可以像其他任何库一样用 `target_link_libraries`
链接。要将框架集成到现有项目中，只需对仓库副本调用 `add_subdirectory`，或对
已安装的版本调用 `find_package(HIGGS)`。完整参考见
[docs/CMake API.md](/docs/CMake%20API.md)，[examples/CMake](/examples/CMake)
包含三个最小项目（`GuiApp`、`ConsoleApp` 和 `AudioPlugin`），可以复制作为
起点。

## Mythos Studio

Mythos Studio 是随 `extras/MythosStudio` 发布的项目生成器。它把项目配置保存在
`.mythos` 文件中，并据此为每个 IDE 和平台导出原生项目：用于 macOS 和 iOS 的
Xcode、Visual Studio 2019、2022 和 2026、Linux Makefile 以及使用 Gradle 的
Android Studio。同一个 `.mythos` 文件可以同时拥有多个导出器，每个导出器都有
自己的 Debug 和 Release 配置、SDK 路径以及编译器和链接器选项，因此项目只维护
在一个地方，当模块或源文件变化时会重新生成 IDE 项目。

该工具包含带有应用程序和插件模板的起始页、能够解析依赖关系的模块管理器、
带语法高亮的代码编辑器，以及一个命令行接口（`--resave`、`--set-version`、
`--encode-binary`、`--create-project-from-pip` 等），无需打开窗口即可自动化
任务。它的配套工具 `higgsaide` 可在终端中生成项目所嵌入的二进制数据和其他
资源。Android 仅通过 Mythos Studio 支持，因为 CMake API 不会为该平台生成
项目。

Mythos Studio 可以从 `extras/MythosStudio/Builds`（Visual Studio、Xcode 或
Linux Makefile 项目）编译，也可以通过 CMake 使用 `-DHIGGS_BUILD_EXTRAS=ON`
和 `MythosStudio` 目标编译。另外还提供适用于 Windows x64、macOS 通用二进制和
Linux x64 的未签名预编译可执行文件，由 GitHub Actions 工作流
`HIGGS Mythos Studio Build` 生成，可作为每次运行的产物下载。

## 工具与示例

除了 Mythos Studio，`extras` 目录还汇集了随框架提供、使用
`-DHIGGS_BUILD_EXTRAS=ON` 编译的工具。`AudioPluginHost` 是一个完整的插件宿主，
它扫描系统中安装的格式，将它们连接到处理器图中，让你无需依赖商业工作站即可
测试自己的插件。`UnitTestRunner` 运行每个模块用 `UnitTest` 类声明的单元测试；
它支持按类别或名称筛选，是项目的验证基准。`BinaryBuilder` 将文件转换为嵌入
数据，`AudioPerformanceTest` 测量实时音频处理的性能，`NetworkGraphicsDemo`
演示多台机器之间的分布式绘图，`SBOM` 生成并检查第三方依赖清单。

`examples` 目录包含大多数功能的完整、可编译的演示，按领域组织：`GUI`、
`Audio`、`DSP`、`Plugins`、`Utilities` 和 `Assets`。它们全部都是 PIP：包含在
单个头文件中的完整项目，文件以元数据块（名称、依赖、类型和主类）开头，
`DemoRunner` 会加载它们并在源代码旁边显示，这样就可以在演示运行时阅读每个
演示的实现。`examples/CMake` 包含前面提到的三个最小项目，`examples/Plugins`
包含完整的插件（合成器、采样器、琶音器、噪声门、插件内宿主和 ARA 示例），
可作为 `AudioProcessor` 完整生命周期的参考。

## 文档

完整的西班牙语用户指南发布在 <https://jalexiscv.github.io/higgs-mythos/>，
其源文件位于 [documentacion](/documentacion)。它涵盖入门（安装、第一个应用
程序和第一个插件）、框架概念（架构、消息循环、线程、内存和约定）、每个领域
的指南（图形界面、图形、音频、MIDI、DSP、插件、宿主、OpenGL、视频、Web、
无障碍、本地化）、每个模块的参考页、CMake API、工具以及项目说明（不兼容的
变更、许可证、安全和术语表）。该站点使用 MkDocs 构建，当 `main` 分支上的文档、
模块或示例发生变化时自动部署到 GitHub Pages；每次部署都会从源代码树重新生成
参考页面。

API 文档直接由模块头文件中的 Doxygen 注释生成，源代码是现有最完整、最新的
文档。每个模块的主头文件 `higgs_<模块>/higgs_<模块>.h` 列出其公共头文件和
依赖项；模块格式在
[docs/Higgs Mythos Module Format.md](/docs/Higgs%20Mythos%20Module%20Format.md)
中描述。要查找某个类，只需在头文件中搜索其声明并阅读上方的注释块：

```bash
grep -rlE "^\s*(class|struct)\s+(HIGGS_API\s+)?AudioProcessorValueTreeState\b" modules
```

[BREAKING_CHANGES.md](BREAKING_CHANGES.md) 逐版本描述了变更内容以及如何更新
代码；[CHANGE_LIST.md](CHANGE_LIST.md) 总结每个版本的要点；
[docs/README.md](/docs/README.md) 是仓库技术文档的索引（模块格式、CMake API、
框架标识和工作协议）。

## 最低系统要求

Higgs Mythos 要求 C++17 作为最低标准，因为其公共头文件使用了该标准的特性
（每个模块在主头文件的 `minimumCppStandard` 字段中声明）；启用
`HIGGS_USE_WINDOWS_MIDI_SERVICES` 时，项目必须使用 C++20 编译，因为 Windows
MIDI Services SDK 要求如此。下面列出的编译器版本是经过测试的最旧版本：更新
版本的 Xcode、Visual Studio、GCC 和 Clang 无需更改即可工作。在 Linux 上，除了
编译器之外，还需要 [docs/Linux Dependencies.md](/docs/Linux%20Dependencies.md)
中列出的 X11、FreeType、ALSA 等开发库；在 Android 上，项目通过 Mythos Studio
导出的项目，使用 Android Studio 中的 Gradle 和 NDK 构建。下面两个列表区分了
开发机器和运行机器：第一个列表说明用什么来构建，第二个列表说明构建结果在
哪里运行。例如，Windows Arm64 插件可以在任何兼容的 Windows 机器上用 Visual
Studio 构建，而 iOS 和 Android 应用程序则分别在 macOS 或装有 Android Studio
的桌面系统上构建。

### 构建 HIGGS 项目

此列表按开发平台汇总了构建框架所需的最低工具：IDE 或编译器，以及开发机器的
操作系统版本。在 macOS 和 iOS 上使用 Xcode，它附带 Clang 和 Apple SDK；最低
系统版本取决于机器是 Intel 还是 Apple Silicon，因为 Xcode 12.4 在两种架构上
要求不同的系统。在 Windows 上使用安装了"使用 C++ 的桌面开发"工作负载和
Windows 10 SDK 的 Visual Studio，既可以在 IDE 中使用，也可以在加载
`vcvars64.bat` 后从命令行使用。在 Linux 上，任何现代的 GCC 或 Clang 配合系统
的开发库即可，Mythos Studio 导出的 Makefile 或 CMake 项目都可以从终端构建。
在 Android 上需要 Android Studio、Gradle 插件和 NDK，用它们构建 Mythos Studio
导出的项目；宿主机器可以是 Windows、macOS 或 Linux。列出的版本是经过测试的
最低版本，而不是推荐版本：每个工具的当前版本都无需更改即可使用。

- __C++ 标准__：17（启用 `HIGGS_USE_WINDOWS_MIDI_SERVICES` 构建时为 20）
- __macOS/iOS__：Xcode 12.4（Intel 为 macOS 10.15.4，Apple Silicon 为 macOS 11.0）
- __Windows__：Visual Studio 2019（Windows 10 版本 1607）
- __Linux__：g++ 7.0 或 Clang 6.0（参见 [docs/Linux Dependencies.md](/docs/Linux%20Dependencies.md)）
- __Android__：Android Studio Narwhal 3（2025.1.3），配合 Android Gradle 插件 8.13.2
  和 NDK 28，可在 Windows、macOS 或 Linux 上使用

### 部署目标

以下数值是使用框架编译的应用程序和插件所能运行的最低操作系统版本，以及生成
二进制文件所针对的处理器架构。在 macOS 和 Windows 上，通常从同一项目生成通用
二进制文件或多种架构；在 Android 上，Mythos Studio 将所选 ABI 生成到单个
软件包中。可以通过 Mythos Studio 中的导出器设置或常用的 CMake 变量为特定
项目选择更高的最低版本（例如 macOS 11 而不是 10.11），从而使用更新的系统
API；不支持低于列表中的数值，因为框架使用了早期版本中不存在的 API。在 iOS
上，x86_64 模拟器可以在 Intel Mac 上进行测试，而物理设备使用 Arm64 或
Arm64e。在 Android 上建议至少包含 arm64-v8a，即所有当前设备的架构，只有当
目标受众确有需要时才添加其他架构，因为每增加一个 ABI 都会增大软件包的
体积。

- __macOS__：macOS 10.11（x86_64、Arm64）
- __Windows__：Windows 10 版本 1607（x86_64、x86、Arm64、Arm64EC）
- __Linux__：主流发行版（x86_64、Arm64/aarch64）
- __iOS__：iOS 12（Arm64、Arm64e、x86_64 模拟器）
- __Android__：Android 7 - Nougat（API Level 24）（arm64-v8a、armeabi-v7a、x86_64、x86）

## 贡献

贡献通过 GitHub 接收。功能请求和一般性问题请在仓库的 *Discussions* 板块提出；
可复现的错误以 issue 形式提交，说明 Higgs Mythos 版本、受影响的模块、平台和
最小复现步骤，并尽可能使用 `.github/ISSUE_TEMPLATE` 中的模板。安全漏洞绝不在
issue 中公开，而是通过 [SECURITY.md](SECURITY.md) 中描述的渠道私下报告。

每个 pull request 必须专注于单一变更，并说明它修复或添加了什么。在提交之前
需要编译受影响的目标并使用 `UnitTestRunner` 运行单元测试。代码、注释和提交
信息使用英语撰写，格式为 `area: summary`。提交贡献即表示同意以与项目相同的
条款（AGPL-3.0-or-later）授权，并确认代码由本人编写或拥有提交权利；不接受从
专有二进制文件或许可证与 AGPL 不兼容的来源复制的代码。完整指南见
[.github/contributing.md](.github/contributing.md)，
[CODE_OF_CONDUCT.md](CODE_OF_CONDUCT.md) 规定了社区的行为准则。

## 许可证

Higgs Mythos 是自由软件：可以依照自由软件基金会发布的 GNU Affero 通用公共
许可证第 3 版或（由用户选择）任何更高版本（**AGPL-3.0-or-later**）的条款
重新分发和修改。完整文本及其适用说明见 [LICENSE.md](LICENSE.md)。AGPL 是一种
*copyleft* 许可证：任何被分发或通过网络向用户提供的衍生作品，都必须以相同的
许可证发布并提供源代码。这包括链接本框架的应用程序和插件。

Higgs Mythos 源自一个早期的开源代码库，其品牌已从名称、模块、宏和资源中完全
移除。按照许可证的要求，原作者的版权声明在每个源文件的头部原样保留，不得
编辑或删除。`examples` 中的示例以宽松得多的 ISC 许可证发布，以便可以复制作为
新项目的基础。第三方依赖及其许可证的清单以 SPDX 格式保存在
[HiggsMythos.spdx.json](HiggsMythos.spdx.json) 中；插件 SDK（VST3、AAX、ARA、
LV2）依照各自的许可证和协议使用，在许可证不允许时不会包含在仓库中。

## AAX 插件

AAX 插件必须使用 PACE Anti-Piracy 的签名工具进行数字签名，才能在商业版本的
Pro Tools 中运行。Avid 免费提供这些工具。在获得签名工具之前，需要使用名为
Pro Tools Developer 的特殊版本 Pro Tools 来测试未签名的插件。获取 Pro Tools
Developer 的步骤如下：

1. 在 <https://developer.avid.com/aax/> 注册成为 AAX 开发者。
2. 发送邮件至 <devauth@avid.com> 申请 Pro Tools Developer Bundle 激活码。
3. 从 Avid 开发者账户下载最新的 Pro Tools Developer 版本。

当插件已在 Pro Tools Developer 中测试和调试完毕并准备签名时，请发送邮件至
<audiosdk@avid.com>，主题为 "PACE Eden Signing Tools Request"。邮件应包含每个
插件的概述，以及展示插件在 Pro Tools Developer 中运行的屏幕录像（尽可能带有
音频），还需提供以下信息：

- 公司名称
- 管理员全名
- 电话号码

提交申请后，PACE Anti-Piracy 将直接联系你并提供插件签名的相关信息。
`higgs_add_plugin` 的 AAX 目标会生成可供签名的 `.aaxplugin` 包；AAX SDK 的
路径需在声明插件之前通过 `higgs_set_aax_sdk_path` 指定。
